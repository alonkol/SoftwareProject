#include <opencv2/imgproc.hpp>//calcHist
#include <opencv2/core.hpp>//Mat
#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>
#include <opencv2/xfeatures2d.hpp>//SiftDescriptorExtractor
#include <opencv2/features2d.hpp>

#include "main_aux.h"
#include "sp_image_proc_util.h"

#define ALLOC_ERROR "An error occurred - allocation failure\n"
#define NOT_FOUND "Image not found!\n"

using namespace cv;

int** spGetRGBHist(char* str, int nBins)
{
    Mat src;
    int **res;
    int i,j;

    res = (int **)malloc(3*sizeof(int*));
    if(res==NULL)
    {
        printf(ALLOC_ERROR);
        free(res);
        return NULL;
    }
    /// Load image
    src = imread(str, CV_LOAD_IMAGE_COLOR);
    if (src.empty())
    {
        printf(NOT_FOUND);
        return NULL;
    }

    /// Separate the image in 3 places ( B, G and R )
    std::vector<Mat> bgr_planes;
    split(src, bgr_planes);

    /// Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 };
    const float* histRange = { range };

    //Output
    Mat b_hist, g_hist, r_hist;

    /// Compute the histograms:
    /// The results will be store in b_hist,g_hist,r_hist.
    /// The output type of the matrices is CV_32F (float)
    calcHist(&bgr_planes[0], 1, 0, Mat(), r_hist, 1, &nBins, &histRange);
    calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &nBins, &histRange);
    calcHist(&bgr_planes[2], 1, 0, Mat(), b_hist, 1, &nBins, &histRange);

    for (i=0; i<3; i++)
    {
        res[i] = (int *)malloc(nBins*sizeof(int));
        if(res[i]==NULL)
        {
            printf(ALLOC_ERROR);
            for(j=0; j<=i; j++)
            {
                free(res[j]);
            }
            return NULL;
        }
    }

    for (i=0; i<nBins; i++)
    {
        res[0][i]=cvRound(r_hist.at<float>(i));
        res[1][i]=cvRound(g_hist.at<float>(i));
        res[2][i]=cvRound(b_hist.at<float>(i));
    }
    return res;
}


double spRGBHistL2Distance(int** histA, int** histB, int nBins)
{
    int i,j;
    double dist[3]= {0.0,0.0,0.0},diff;
    for(i=0; i<3; i++)
    {
        for(j=0; j<nBins; j++)
        {
            diff = ((double)histA[i][j]-(double)histB[i][j]);
            dist[i]+=diff*diff;
        }
    }
    return 0.33*dist[0]+0.33*dist[1]+0.33*dist[2];
}

double** spGetSiftDescriptors(char* str, int maxNFeautres, int *nFeatures)
{

    Mat src;
    double **res;
    int i,j;

    src = imread(str, CV_LOAD_IMAGE_GRAYSCALE);
    if (src.empty())
    {
        printf(NOT_FOUND);
        return NULL;
    }
    //Key points will be stored in kp1;
    std::vector<KeyPoint> kp1;
    //Feature values will be stored in ds1;
    Mat ds1;
    //Creating  a Sift Descriptor extractor
    Ptr<xfeatures2d::SiftDescriptorExtractor> detect =
        xfeatures2d::SIFT::create(maxNFeautres);
    //Extracting features
    //The features will be stored in ds1
    //The output type of ds1 is CV_32F (float)
    detect->detect(src, kp1, Mat());
    detect->compute(src, kp1, ds1);
    int rows = ds1.rows;
    res =(double **)malloc(rows*sizeof(double*));
    if(res==NULL)
    {
        printf(ALLOC_ERROR);
        free(res);
        return NULL;
    }
    for (i=0; i<rows; i++)
    {
        res[i] = (double *)malloc(128*sizeof(double));
        if(res[i]==NULL)
        {
            printf(ALLOC_ERROR);
            for(j=0; j<=i; j++)
            {
                free(res[j]);
            }
            return NULL;
        }
    }

    for (i=0; i<rows; i++)
    {
        for(j=0; j<128; j++)
        {
            res[i][j]=ds1.at<float>(i,j);
        }
    }
    *nFeatures=rows;
    return res;
}

double spL2SquaredDistance(double* featureA, double* featureB)
{
    int i;
    double diff,dist=0.0;
    for(i=0; i<128; i++)
    {
        diff=featureA[i]-featureB[i];
        dist+=diff*diff;
    }
    return dist;
}

int* spBestSIFTL2SquaredDistance(int bestNFeatures, double* featureA,
                                 double*** databaseFeatures, int numberOfImages,
                                 int* nFeaturesPerImage)
{

    int i,j,numFeatures=0,cnt=0,*res;
    for(i=0; i<numberOfImages; i++)
    {
        numFeatures+=nFeaturesPerImage[i];
    }

    double *featDistances = (double *)malloc(numFeatures*sizeof(double)*2);
    if(featDistances==NULL)
    {
        printf(ALLOC_ERROR);
        free(featDistances);
        return NULL;
    }

    for(i=0; i<numberOfImages; i++)
    {
        for(j=0; j<nFeaturesPerImage[i]; j++)
        {
            featDistances[2*cnt]=(double)i; //number of Image
            featDistances[2*cnt+1]=spL2SquaredDistance(featureA,databaseFeatures[i][j]); //distance of feature
            cnt++;
        }
    }
    qsort(featDistances,numFeatures,sizeof(double)*2, cmpfunc);

    res =(int *)malloc(bestNFeatures*sizeof(int));
    if(res==NULL)
    {
        printf(ALLOC_ERROR);
        free(res);
        return NULL;
    }

    for(i=0; i<bestNFeatures; i++)
    {
        res[i]=featDistances[2*i];
    }
    free(featDistances);
    return res;
}


