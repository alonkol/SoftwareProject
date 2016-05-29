#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "sp_image_proc_util.h"
#include "main_aux.h"

#define PICS_TO_SHOW 5
#define PATH_MAX_LENGTH 1024
#define IMAGES_DIRECTORY "Enter images directory path:\n"
#define IMAGES_PREFIX "Enter images prefix:\n"
#define IMAGES_NUMBER "Enter number of images:\n"
#define IMAGES_NUMBER_ERROR "An error occurred - invalid number of images\n"
#define IMAGES_SUFFIX "Enter images suffix:\n"
#define BINS_NUMBER "Enter number of bins:\n"
#define BINS_NUMBER_ERROR "An error occurred - invalid number of bins\n"
#define FEATURES_NUMBER "Enter number of features:\n"
#define FEATURES_NUMBER_ERROR "An error occurred - invalid number of features\n"
#define QUERY_IMAGE "Enter a query image or # to terminate:\n"
#define GLOBAL_DESCRIPTORS "Nearest images using global descriptors:\n"
#define LOCAL_DESCRIPTORS "Nearest images using local descriptors:\n"
#define EXIT_MSG "Exiting...\n"
#define ALLOC_ERROR "An error occurred - allocation failure\n"

int main()
{
    char folderPath[PATH_MAX_LENGTH],prefix[PATH_MAX_LENGTH], suffix[10];
    int imgAmount,nBins,nFeatures;

    printf(IMAGES_DIRECTORY);
    scanf("%s",folderPath);
    printf(IMAGES_PREFIX);
    scanf("%s",prefix);
    printf(IMAGES_NUMBER);
    scanf("%d",&imgAmount);
    if(imgAmount<1)
    {
        printf(IMAGES_NUMBER_ERROR);
        return -1;
    }
    printf(IMAGES_SUFFIX);
    scanf("%s",suffix);
    printf(BINS_NUMBER);
    scanf("%d",&nBins);
    if(nBins<1)
    {
        printf(BINS_NUMBER_ERROR);
        return -1;
    }
    printf(FEATURES_NUMBER);
    scanf("%d",&nFeatures);
    if(nFeatures<1)
    {
        printf(FEATURES_NUMBER_ERROR);
        return -1;
    }

    int ***histograms=(int***)malloc(sizeof(int**) * imgAmount);
    int *featuresPerImage=(int*)malloc(sizeof(int) * imgAmount);
    double ***siftDescriptors=(double***)malloc(sizeof(double**) * imgAmount);
    int i,nDigits = floor(log10(imgAmount))+1;
    char fullPath[PATH_MAX_LENGTH];
    char *strI=(char*)malloc(sizeof(char)*nDigits);

    if(histograms==NULL || featuresPerImage==NULL || siftDescriptors ==NULL || strI==NULL){
        free(histograms);
        free(featuresPerImage);
        free(siftDescriptors);
        free(strI);
        printf(ALLOC_ERROR);
        printf(EXIT_MSG);
    }

    for(i=0; i<imgAmount; i++)
    {
        //calculate path
        strcpy(fullPath, folderPath);
        strcat(fullPath, prefix);
        sprintf(strI,"%d",i);
        strcat(fullPath, strI);
        strcat(fullPath, suffix);
        //calculate histograms and features
        histograms[i]=spGetRGBHist(fullPath,nBins);
        if(histograms[i]==NULL)
        {
            printf(EXIT_MSG);
            return -1;
        }
        siftDescriptors[i]=spGetSiftDescriptors(fullPath,nFeatures,&featuresPerImage[i]);
        if(siftDescriptors[i]==NULL)
        {
            printf(EXIT_MSG);
            return -1;
        }
    }
    free(strI);

    char imgPath[PATH_MAX_LENGTH];
    int **imgHistogram,*closeHist,*closeSift,imgFeatures,j;
    double** imgSift;
    imgPath[0]=0;
    printf(QUERY_IMAGE);
    scanf("%s",imgPath);

    while (strcmp(imgPath,"#")!=0)
    {
        imgHistogram=spGetRGBHist(imgPath,nBins);
        if(imgHistogram==NULL)
        {
            break;
        }
        imgSift=spGetSiftDescriptors(imgPath,nFeatures,&imgFeatures);
        if(imgSift==NULL)
        {
            for(i=0; i<3; i++)
            {
                free(imgHistogram[i]);
            }
            free(imgHistogram);
            break;
        }

        closeHist=spBestHISTL2SquaredDistance(PICS_TO_SHOW,imgHistogram,histograms,imgAmount,nBins);
        printf(GLOBAL_DESCRIPTORS);
        for(i=0; i<PICS_TO_SHOW-1; i++)
        {
            printf("%d, ",closeHist[i]);
        }
        printf("%d\n",closeHist[PICS_TO_SHOW-1]);

        closeSift=spClosestFeat(PICS_TO_SHOW,imgSift,siftDescriptors,imgAmount,imgFeatures,featuresPerImage);
        printf(LOCAL_DESCRIPTORS);
        for(i=0; i<PICS_TO_SHOW-1; i++)
        {
            printf("%d, ",closeSift[i]);
        }
        printf("%d\n",closeSift[PICS_TO_SHOW-1]);
        for(i=0; i<3; i++)
        {
            free(imgHistogram[i]);
        }
        free(imgHistogram);
        for (i=0; i<imgFeatures; i++)
        {
            free(imgSift[i]);
        }
        free(imgSift);
        free(closeSift);
        free(closeHist);

        printf(QUERY_IMAGE);
        scanf("%s",imgPath);
    }

    for(i=0; i<imgAmount; i++)
    {
        for(j=0; j<3; j++)
        {
            free(histograms[i][j]);
        }
        free(histograms[i]);
        for (j=0; j<featuresPerImage[i]; j++)
        {
            free(siftDescriptors[i][j]);
        }
        free(siftDescriptors[i]);
    }
    free(histograms);
    free(featuresPerImage);
    free(siftDescriptors);

    printf(EXIT_MSG);
    return 0;


}
