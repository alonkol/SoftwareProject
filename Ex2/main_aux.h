#ifndef MAIN_AUX_H_INCLUDED
#define MAIN_AUX_H_INCLUDED

int* spBestHISTL2SquaredDistance(int bestNHist, int** histA,
		int*** databaseHistograms, int numberOfImages,int nBins);

int* spClosestFeat(int bestFeat,double** imgSift,double ***siftDescriptors,
                   int numberOfImages, int imgFeatures,int* featuresPerImage);

int cmpfunc(const void *a,const void *b);
int cmpfunc2(const void *a,const void *b);

#endif // MAIN_AUX_H_INCLUDED
