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
#define INVALID_CMD "Invalid command line : use -c <config_filename>\n"
#define PREPROCCESING_SECCESSFULL "Preprocessing finished successfully."
#define ENTER_IMAGE_PATH "Please enter an image path:\n"

using namespace sp;

/** This function produces features array based on the configuration file
* @param config - the configuration file.
* @param imgProc - the imageProc class instance.
* @param featArrSize - used to return the resulting features array.
* @param loggerMsg - used to identify if the logger was successfully initialized.
* @return features array containing all the features it extracted/loaded in success or NULL in failure.
**/
SPPoint* produceFeatures(SPConfig config,ImageProc *imgProc,int* featArrSize,SP_LOGGER_MSG loggerMsg);

/** This function chooses for any query its SPKNN images that are most related to it
*   and displays them based on the configuration file.
* @param query - file path for the query image.
* @param config - the configuration file.
* @param imgProc - the imageProc class instance.
* @param root - the root of the SPKDTree.
* @return 0 on success, -1 on failure.
**/
int showSimilarQuery(char* query,SPConfig config,ImageProc *imgProc,SPKDTreeNode *root);

/** This function clears all the memory used in main and return exitVal
* @param config - configuration file.
* @param root - root of SPKDTree.
* @param allFeats - features array.
* @param featArrSize - size of features Array.
* @param imgProc - the imageProc class instance.
* @param exitVal - the exitVal to be returned.
* @return exitVal.
**/
int freeMemMain(SPConfig config, SPKDTreeNode* root,SPPoint* allFeats,int featArrSize, ImageProc *imgProc,int exitVal);


int main(int argc,char** argv)
{
    char configFileName[MAXLINESIZE], query[MAXLINESIZE];
    SP_CONFIG_MSG msg;
    SPConfig config=NULL;
    int featArrSize=0,searchRes=0;
    ImageProc *imgProc=NULL;
    SPPoint *allFeats=NULL;
    SPKDTreeNode* root=NULL;
    SP_LOGGER_MSG loggerMsg;

    //checks that the input is legit.
    if(argc>3 || argc==2)
    {
        printf(INVALID_CMD);
        return 1;
    }
    else if ((argc==3) && (strcmp(argv[1],"-c")==0))
    {
        strcpy(configFileName,argv[2]);
    }
    else
    {
        strcpy(configFileName,DEFUALT_CONFIG_FILE_NAME);
    }
    //creates config file.
    config = spConfigCreate(configFileName,&msg);
    if(config==NULL || msg!=SP_CONFIG_SUCCESS)
    {
        printf(EXIT_MSG);
        return 1;
    }
    //if config file is created successfully the preprocessing can continue.
    loggerMsg=spLoggerCreate(spConfigGetLoggerFile(config),spConfigGetLoggerLevel(config));
    imgProc = new ImageProc(config);
    allFeats=produceFeatures(config,imgProc,&featArrSize,loggerMsg);
    root = spKDTreeCreate(allFeats,featArrSize,config);

    //root is NULL if somewhere in the process at least one of the following returned NULL: logger,imgProc,allFeats,root
    if(root==NULL) {
        return freeMemMain(config,root,allFeats,featArrSize,imgProc,1);
    }
    spLoggerPrintInfo(PREPROCCESING_SECCESSFULL);

    //We can start the image query check.
    printf(ENTER_IMAGE_PATH);
    scanf("%s",query);
    while (strcmp(query,"<>")!=0)
    {
        searchRes = showSimilarQuery(query,config,imgProc,root);
        if(searchRes==-1)
        {
             return freeMemMain(config,root,allFeats,featArrSize,imgProc,1);
        }
        printf(ENTER_IMAGE_PATH);
        scanf("%s",query);
    }
    return freeMemMain(config,root,allFeats,featArrSize,imgProc,0);
}

int freeMemMain(SPConfig config, SPKDTreeNode* root,SPPoint* allFeats,int featArrSize, ImageProc *imgProc,int exitVal)
{
    spConfigDestroy(config);
    destroyKDTree(root);
    destroyPointsArr(allFeats,featArrSize);
    spLoggerDestroy();
    delete imgProc;
    printf(EXIT_MSG);
    return exitVal;
}

SPPoint* produceFeatures(SPConfig config,ImageProc *imgProc,int* featArrSize,SP_LOGGER_MSG loggerMsg)
{
    SP_CONFIG_MSG msg;
    SPPoint *allFeats=NULL,*imgFeats;
    char buff[MAXLINESIZE],imagePath[MAXLINESIZE];
    int totalSize = 0, numFeats = 0, i,numImages;

    if(config==NULL || imgProc==NULL || loggerMsg!=SP_LOGGER_SUCCESS) return NULL;
    numImages = spConfigGetNumOfImages(config,&msg);

    if(spConfigIsExtractionMode(config,&msg))
    // if we nned to extract features.
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
    //if we need to only load features.
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
    int numFeats,i,j,*cntSim,numberOfImages;
    char buff[MAXLINESIZE],imgPath[MAXLINESIZE];
    SPListElement node;
    SPBPQueue bpq;
    SPPoint* queryFeat;

    numberOfImages = spConfigGetNumOfImages(config,&msg);
    cntSim = (int*)malloc(numberOfImages*sizeof(int)*2); //this will act as a counter array for the images, see below.
    if(cntSim==NULL)
    {
        spLoggerPrintError(ALLOC_ERROR_MSG,__FILE__,__func__,__LINE__);
        return -1;
    }
    //get the query features.
    queryFeat = imgProc -> getImageFeatures(query,numberOfImages+1,&numFeats);
    if(queryFeat==NULL)
    {
        free(cntSim);
        return -1;
    }
    sprintf(buff,"Extracted from query %d features.",numFeats);
    spLoggerPrintInfo(buff);
    //initilaize similar array
    for(i=0; i<numberOfImages; i++)
    {
        cntSim[2*i]=i; //this will be the index of the image
        cntSim[2*i+1]=0; //this will act as the counter
    }
    /* for every feat find the spKNN closest features, now for each node in the resulting bpq,
    update the correct counter in the similar images array. */
    for(i=0; i<numFeats; i++)
    {
        bpq = kNearestNeighbors(config,root,queryFeat[i]);
        if(bpq==NULL)
        {
            free(cntSim);
            destroyPointsArr(queryFeat,numFeats);
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
    //sort the array of similar image counters, the first spKNN will be the most similar images.
    qsort(cntSim,numberOfImages,sizeof(int)*2,cmpfunc);

    //show those similar images according to the configuration file.
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
    destroyPointsArr(queryFeat,numFeats);
    sprintf(buff,"Finished working with query %s.",query);
    spLoggerPrintInfo(buff);
    return 0;
}


