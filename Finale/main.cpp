#include <cstdlib>
#include <cstdbool>
#include <cstdio>
#include <iostream>


#include "SPImageProc.h"
extern "C" {
#include "SPLogger.h"
#include "SPConfig.h"
#include "SP_aux.h"
#include "SPPoint.h"
//#include "SPKDTree.h"
#include "SPListElement.h"
#include "SPBPriorityQueue.h"
#include "SPSearch.h"
}

#define MAXLINESIZE 1024


using namespace sp;
SPPoint* produceFeatures(SPConfig config,ImageProc *imgProc,int* featArrSize);
int cmpfunc(const void *a,const void *b);
void showSimilarQuery(char* query,SPConfig config,ImageProc *imgProc,SPKDTreeNode *root);

int main(int argc,char** argv)
{

    SP_CONFIG_MSG msg;
    SPConfig config = spConfigCreate(argv[1],&msg);
    spConfigPrint(config);
    SPLogger logger = spLoggerCreate(spConfigGetLoggerFile(config),spConfigGetLoggerLevel(config));

    int featArrSize,i;
    ImageProc *imgProc = new ImageProc(config);
    SPPoint *allFeats=produceFeatures(config,imgProc,&featArrSize);
    SPKDArray* k = spKDArrayInit(allFeats,featArrSize);
    SPKDTreeNode* root = spKDTreeCreate(k,config);
    char query[MAXLINESIZE];

    printf("Please enter an image path:\n");
    scanf("%s",query);
    while (strcmp(query,"<>")!=0){
        showSimilarQuery(query,config,imgProc,root);
        printf("Please enter an image path:\n");
        scanf("%s",query);
    }


    spKDArrayDestroy(k);
    destroyKDTree(root);
    printf("Tree and arr destroyed\n");

    for(i=0; i<featArrSize; i++)
    {
        spPointDestroy(allFeats[i]);
        allFeats[i]=NULL;
        //printf("Destroyed %d\t",i);
    }
    spLoggerDestroy(logger);
    printf("\nfreeing the arr\n");
    free(allFeats);
    printf("And now config...\n");
    spConfigDestroy(config);
    printf("Last imgProc\n");
    delete imgProc;
    return 0;
}

SPPoint* produceFeatures(SPConfig config,ImageProc *imgProc,int* featArrSize)
{
    SP_CONFIG_MSG msg;
    int numImages = spConfigGetNumOfImages(config,&msg);
    SPPoint *allFeats=NULL,*imgFeats;
    int totalSize = 0, numFeats = 0, i;
    if(spConfigIsExtractionMode(config,&msg))
    {
        printf("Extracting!\n");
        for (i=0; i<numImages; i++)
        {
            char imagePath[MAXLINESIZE];
            msg = spConfigGetImagePath(imagePath,config,i);
            if(msg!=SP_CONFIG_SUCCESS)
            {
                //declare error
                return NULL;
            }
            imgFeats = imgProc -> getImageFeatures(imagePath,i,&numFeats);
            printf("img %d got %d Features\t",i,numFeats);
            allFeats = spUpdateAndSaveFeats(allFeats,imgFeats,totalSize,numFeats,i,config);
            printf("img %d Saved\n",i);
            totalSize += numFeats;
        }
    }
    else
    {
        printf("Loading!\n");
        allFeats=spLoadImgFeats(config,numImages,&totalSize);
    }
    printf("Done Extracting/Loading! totalSize of featuresArray: %d\n",totalSize);
    *featArrSize = totalSize;
    return allFeats;

}

void showSimilarQuery(char* query,SPConfig config,ImageProc *imgProc,SPKDTreeNode *root){
    SP_CONFIG_MSG msg;
    int numberOfImages = spConfigGetNumOfImages(config,&msg);
    int* cntSim = (int*)malloc(numberOfImages*sizeof(int)*2);
    if(cntSim==NULL) printf("Failed to allocate cntSim\n");
    int numFeats,j,i;
    SPListElement node;
    SPBPQueue bpq;
    SPPoint* queryFeat = imgProc -> getImageFeatures(query,numberOfImages+1,&numFeats);
    printf("Got query %d Features \n",numFeats);

    for(i=0; i<numberOfImages; i++)
    {
        cntSim[2*i]=i;
        cntSim[2*i+1]=0;
    }
    for(i=0; i<numFeats; i++)
    {
        bpq = kNearestNeighbors(config,root,queryFeat[i]);
        for(j=0; j<spConfigGetKNN(config); j++)
        {
            node = spBPQueuePeek(bpq);
            spBPQueueDequeue(bpq);
            cntSim[spListElementGetIndex(node)*2+1]++;
            spListElementDestroy(node);
        }
        spBPQueueDestroy(bpq);
        //printf("%d finished\t",i);
    }
    qsort(cntSim,numberOfImages,sizeof(int)*2,cmpfunc);

    char imgPath[MAXLINESIZE];
    if(!spConfigMinimalGui(config,&msg)) printf("Best candidates for - %s - are:\n",query);
    printf("%d\n",spConfigGetNumOfSimilar(config));
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
    printf("CntSim freed\n");
    for(i=0;i<numFeats;i++){
        spPointDestroy(queryFeat[i]);
        queryFeat[i]=NULL;
       // printf("Destroyed query feat %d\t",i);
    }
    free(queryFeat);
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
