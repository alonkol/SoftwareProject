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

int main(int argc,char** argv)
{

    SP_CONFIG_MSG msg;
    SPConfig config = spConfigCreate(argv[1],&msg);
    spConfigPrint(config);
    // init logger /////////////////////////////////////

    int featArrSize,i;
    ImageProc *imgProc = new ImageProc(config);
    SPPoint *allFeats=produceFeatures(config,imgProc,&featArrSize);
/*
    SPKDArray* k = spKDArrayInit(allFeats,featArrSize);
    printKDARR(k);

    SplitRes* t = spKDArraySplit(k,0);
    printf("\nLeft:\n");
    printKDARR(t->kdLeft);
    printf("\nRight:\n");
    printKDARR(t->kdRight);
    splitResDestroy(t);
*/
    SPKDArray* k = spKDArrayInit(allFeats,featArrSize);
    SPKDTreeNode* root = spKDTreeCreate(k,config);

    char* query = "./images/img0.png";
    int* res = (int*)calloc(spConfigGetNumOfImages(config,&msg),sizeof(int));
    int numFeats,j;
    SPListElement node;
    SPPoint* queryFeat = imgProc -> getImageFeatures(query,1000,&numFeats);

    for(i=0;i<numFeats;i++){
            printPoint(queryFeat[i]);
        SPBPQueue bpq = kNearestNeighbors(config,root,queryFeat[i]);

        for(j=0;j<spConfigGetKNN(config);j++){
            node = spBPQueuePeek(bpq);
            spBPQueueDequeue(bpq);
            res[spListElementGetIndex(node)]+=1;
            spListElementDestroy(node);
        }
        spBPQueueDestroy(bpq);
    }

    for(i=0;i<spConfigGetNumOfImages(config,&msg);i++)
        printf("%d\t",res[i]);

    free(res);
    destroyKDTree(root);
    spKDArrayDestroy(k);
    delete imgProc;
    for(i=0;i<featArrSize;i++){
        spPointDestroy(allFeats[i]);
    }
    free(allFeats);
    spConfigDestroy(config);

    return 0;
}

SPPoint* produceFeatures(SPConfig config,ImageProc *imgProc,int* featArrSize){

    SP_CONFIG_MSG msg;
    int numImages = spConfigGetNumOfImages(config,&msg);
    SPPoint *allFeats=NULL,*imgFeats;
    int totalSize = 0, numFeats = 0, i;
    if(spConfigIsExtractionMode(config,&msg))
    {
        printf("Extracting!\n");
        for (i=0;i<numImages;i++){
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
