#include "sp_image_proc_util.h"
#include "main_aux.h"
#include <stdlib.h>
#include <stdio.h>

#define PICS_TO_SHOW 5
#define ALLOC_ERROR "An error occurred - allocation failure\n"

int* spBestHISTL2SquaredDistance(int bestNHist, int** histA,
		int*** databaseHistograms, int numberOfImages,int nBins){

    int i,*res;
    //double distHist[numberOfImages][2];
    double *distHist = (double *)malloc(numberOfImages*sizeof(double)*2);
    if(distHist==NULL){
        printf(ALLOC_ERROR);
        free(distHist);
        return NULL;
    }
    for(i=0;i<numberOfImages;i++){
        distHist[2*i]=i;
        distHist[2*i+1]=spRGBHistL2Distance(histA,databaseHistograms[i],nBins);
    }
    qsort(distHist,numberOfImages,2*sizeof(double), cmpfunc);
    res = (int *)malloc(PICS_TO_SHOW*sizeof(int));
    if(res==NULL){
            printf(ALLOC_ERROR);
            free(res);
            return NULL;
    }
    for(i=0;i<bestNHist;i++){
        res[i]=distHist[2*i];
    }
    free(distHist);
    return res;

}

int* spClosestFeat(int bestFeat,double** imgSift,double ***siftDescriptors,
                   int numberOfImages, int imgFeatures,int* featuresPerImage){
    int *closeFeat,*res,i,j;
    int *cntFeat = (int *)malloc(numberOfImages*sizeof(int)*2);
    if(cntFeat==NULL){
        printf(ALLOC_ERROR);
        free(cntFeat);
        return NULL;
    }
    for(i=0;i<numberOfImages;i++){
        cntFeat[2*i]=i;
        cntFeat[2*i+1]=0;
    }

    for(i=0;i<imgFeatures;i++){
        closeFeat=spBestSIFTL2SquaredDistance(PICS_TO_SHOW,imgSift[i],siftDescriptors,numberOfImages,featuresPerImage);
        for(j=0;j<PICS_TO_SHOW;j++){
            cntFeat[2*closeFeat[j]+1]++;
        }
    }
    qsort(cntFeat,numberOfImages,sizeof(int)*2,cmpfunc2);

    res = (int *)malloc(PICS_TO_SHOW*sizeof(int));
    if(res==NULL){
        printf(ALLOC_ERROR);
        free(res);
        return NULL;
    }
    for(i=0;i<bestFeat;i++){
        res[i]=cntFeat[2*i];
    }
    free(closeFeat);
    free(cntFeat);
    return res;
}

int cmpfunc(const void *a,const void *b){

   double* da = (double*)a;
   double* db = (double*)b;
   if(db[1]<da[1]) return 1;
   if(db[1]>da[1]) return -1;
   if(db[0]<da[0]) return 1;
   return -1;

}

int cmpfunc2(const void *a,const void *b){
   const int* da = (const int*)a;
   const int* db = (const int*)b;
   if(db[1]>da[1]) return 1;
   if(db[1]<da[1]) return -1;
   if(db[0]<da[0]) return 1;
   return -1;

}



