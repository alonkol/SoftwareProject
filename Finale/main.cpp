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
#include "SPKDTree.h"
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
    KDTreeNode* t = create(k,config);

    destroyKDTree(t);
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
