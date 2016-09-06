#include <cstdlib>
#include <cstdbool>
#include <cstdio>
#include <iostream>

#include "SPImageProc.h"
extern "C" {
#include "SPFeatExtract.h"
#include "SPSearch.h"
}

#define MAXLINESIZE 1024
#define ALLOC_ERROR_MSG "Memory Allocation Error."
#define DEFUALT_CONFIG_FILE_NAME "spcbir.config"
#define EXIT_MSG "Exiting...\n"

using namespace sp;
SPPoint* produceFeatures(SPConfig config,ImageProc *imgProc,int* featArrSize);
int showSimilarQuery(char* query,SPConfig config,ImageProc *imgProc,SPKDTreeNode *root);
int cmpfunc(const void *a,const void *b);

int main(int argc,char** argv)
{
    char configFileName[MAXLINESIZE], query[MAXLINESIZE];
    SP_CONFIG_MSG msg;
    SPConfig config;
    SP_LOGGER_MSG loggerMsg;
    int featArrSize,i,searchRes=0;
    ImageProc *imgProc;
    SPPoint *allFeats;
    SPKDArray* kdArray;
    SPKDTreeNode* root;


    //if more than one parameter given
    if(argc>2){
        printf("Invalid command line : use -c <config_filename>\n");
        return 1;
    }else if (argc==2){
        strcpy(configFileName,argv[1]);
    }else{
        strcpy(configFileName,DEFUALT_CONFIG_FILE_NAME);
    }

    config = spConfigCreate(configFileName,&msg);
    if(config==NULL){
        printf(EXIT_MSG);
        return 1;
    }

    loggerMsg = spLoggerCreate(spConfigGetLoggerFile(config),spConfigGetLoggerLevel(config));
    if(loggerMsg==SP_LOGGER_OUT_OF_MEMORY){
        spConfigDestroy(config);
        printf(EXIT_MSG);
        return 1;
    }

    imgProc = new ImageProc(config);

    allFeats=produceFeatures(config,imgProc,&featArrSize);
    if(allFeats==NULL)
    {
        spConfigDestroy(config);
        spLoggerDestroy();
        delete imgProc;
        printf(EXIT_MSG);
        return 1;
    }

    kdArray = spKDArrayInit(allFeats,featArrSize);

    if (kdArray==NULL)
    {
        spConfigDestroy(config);
        spLoggerDestroy();
        delete imgProc;
        for(i=0; i<featArrSize; i++)
        {
            spPointDestroy(allFeats[i]);
        }
        free(allFeats);
        printf(EXIT_MSG);
        return 1;
    }

    root = spKDTreeCreate(kdArray,config);

    if(root==NULL)
    {
        spConfigDestroy(config);
        spLoggerDestroy();
        delete imgProc;
        for(i=0; i<featArrSize; i++)
        {
            spPointDestroy(allFeats[i]);
        }
        free(allFeats);
        spKDArrayDestroy(kdArray);
        printf(EXIT_MSG);
        return 1;
    }

    spLoggerPrintInfo("Preprocessing finished successfully.");

    printf("Please enter an image path:\n");
    scanf("%s",query);
    while (strcmp(query,"<>")!=0)
    {
        searchRes = showSimilarQuery(query,config,imgProc,root);
        if(searchRes==-1){
            break;
        }
        printf("Please enter an image path:\n");
        scanf("%s",query);
    }

    spKDArrayDestroy(kdArray);
    destroyKDTree(root);
    for(i=0; i<featArrSize; i++)
    {
        spPointDestroy(allFeats[i]);
    }
    free(allFeats);
    spConfigDestroy(config);
    delete imgProc;
    printf(EXIT_MSG);
    return 0;
}

SPPoint* produceFeatures(SPConfig config,ImageProc *imgProc,int* featArrSize)
{
    SP_CONFIG_MSG msg;
    SPPoint *allFeats=NULL,*imgFeats;
    char buff[MAXLINESIZE],imagePath[MAXLINESIZE];
    int totalSize = 0, numFeats = 0, i;
    int numImages = spConfigGetNumOfImages(config,&msg);
    if(spConfigIsExtractionMode(config,&msg))
    {
        for (i=0; i<numImages; i++)
        {
            msg = spConfigGetImagePath(imagePath,config,i);
            if(msg!=SP_CONFIG_SUCCESS)
            {
                //declare error
                return NULL;
            }
            imgFeats = imgProc -> getImageFeatures(imagePath,i,&numFeats);
            sprintf(buff,"From img %d extracted %d features.",i,numFeats);
            spLoggerPrintInfo(buff);
            allFeats = spUpdateAndSaveFeats(allFeats,imgFeats,totalSize,numFeats,i,config);
            totalSize += numFeats;
        }
    }
    else
    {
        allFeats=spLoadImgFeats(config,numImages,&totalSize);
    }
    sprintf(buff,"Done Extracting/Loading! totalSize of featuresArray: %d",totalSize);
    spLoggerPrintInfo(buff);
    *featArrSize = totalSize;
    return allFeats;

}

int showSimilarQuery(char* query,SPConfig config,ImageProc *imgProc,SPKDTreeNode *root)
{
    SP_CONFIG_MSG msg;
    int numFeats,j,i;
    char buff[MAXLINESIZE],imgPath[MAXLINESIZE];
    SPListElement node;
    SPBPQueue bpq;
    SPPoint* queryFeat;
    int numberOfImages = spConfigGetNumOfImages(config,&msg);
    int* cntSim = (int*)malloc(numberOfImages*sizeof(int)*2);
    if(cntSim==NULL)
    {
        spLoggerPrintError(ALLOC_ERROR_MSG,__FILE__,__func__,__LINE__);
        return -1;
    }
    queryFeat = imgProc -> getImageFeatures(query,numberOfImages+1,&numFeats);
    if(queryFeat==NULL)
    {
        spLoggerPrintError(ALLOC_ERROR_MSG,__FILE__,__func__,__LINE__);
        free(cntSim);
        return -1;
    }
    sprintf(buff,"Extracted from query %d features.",numFeats);
    spLoggerPrintInfo(buff);

    for(i=0; i<numberOfImages; i++)
    {
        cntSim[2*i]=i;
        cntSim[2*i+1]=0;
    }
    for(i=0; i<numFeats; i++)
    {
        bpq = kNearestNeighbors(config,root,queryFeat[i]);
        if(bpq==NULL)
        {
            free(cntSim);
            for(i=0; i<numFeats; i++)
            {
                spPointDestroy(queryFeat[i]);
                queryFeat[i]=NULL;
            }
            free(queryFeat);
            return -1;

        }
        for(j=0; j<spConfigGetKNN(config); j++)
        {
            node = spBPQueuePeek(bpq);
            spBPQueueDequeue(bpq);
            cntSim[spListElementGetIndex(node)*2+1]++;
            spListElementDestroy(node);
        }
        spBPQueueDestroy(bpq);
    }
    qsort(cntSim,numberOfImages,sizeof(int)*2,cmpfunc);


    if(!spConfigMinimalGui(config,&msg)) printf("Best candidates for - %s - are:\n",query);
    for(i=0; i<spConfigGetNumOfSimilar(config); i++)
    {
        spConfigGetImagePath(imgPath,config,cntSim[2*i]);
        if(spConfigMinimalGui(config,&msg))
        {
            imgProc->showImage(imgPath);
        }
        else
        {
            printf("%s\n",imgPath);
        }
    }
    free(cntSim);
    for(i=0; i<numFeats; i++)
    {
        spPointDestroy(queryFeat[i]);
    }
    free(queryFeat);
    sprintf(buff,"Finished working with query %s.",query);
    spLoggerPrintInfo(buff);
    return 0;
}

int cmpfunc(const void *a,const void *b)
{
    const int* da = (const int*)a;
    const int* db = (const int*)b;
    if(db[1]>da[1]) return 1;
    if(db[1]<da[1]) return -1;
    if(db[0]<da[0]) return 1;
    return -1;
}
