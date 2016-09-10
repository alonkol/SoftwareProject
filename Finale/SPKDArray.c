#include "SPKDArray.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define ALLOC_ERROR_MSG "Memory Allocation Error."

// variable used in order to sort based on specific coordinate.
int sortedDim;

// this is a helper struct used in spKDArrayInit to calculate the index matrix.
typedef struct point_wrapper
{
    SPPoint point;
    int index;
} Wrapper ;


SPKDArray* spKDArrayCreate(SPPoint* points,int dim,int size)
{
    int i,j;
    SPKDArray *kdArr = (SPKDArray*)malloc(sizeof(SPKDArray));
    if (kdArr == NULL)
    {
        spLoggerPrintError(ALLOC_ERROR_MSG, __FILE__, __func__, __LINE__);
        return NULL;
    }
    kdArr->dim = dim;
    kdArr->size=size;
    kdArr->points=points;

    // allocate matrix
    kdArr->pointsMat = (int**)malloc(sizeof(int*)*dim);

    // in case of allocation failure, free all memory and return null
    if (kdArr->pointsMat == NULL)
    {
        free(kdArr);
        spLoggerPrintError(ALLOC_ERROR_MSG, __FILE__, __func__, __LINE__);
        return NULL;
    }

    // allocate matrix rows
    for(i=0; i<dim; i++)
    {
        kdArr->pointsMat[i]=(int*)malloc(sizeof(int)*size);

        // in case of allocation failure, free all memory and return null
        if (kdArr->pointsMat[i] == NULL)
        {
            for (j = 0; j < i; j++)
            {
                free(kdArr->pointsMat[j]);
            }
            free(kdArr->pointsMat);
            free(kdArr);
            spLoggerPrintError(ALLOC_ERROR_MSG, __FILE__, __func__, __LINE__);
            return NULL;
        }
    }
    return kdArr;
}

SPKDArray* spKDArrayInit(SPPoint* points,int size)
{
    Wrapper* tmpPoints;
    int i,dim,j;
    if(points==NULL) return NULL;
    SPPoint* pointsCopy = (SPPoint*)malloc(sizeof(SPPoint)*size);
    // alloc failure: return NULL
    if (pointsCopy == NULL)
    {
        spLoggerPrintError(ALLOC_ERROR_MSG, __FILE__, __func__, __LINE__);
        return NULL;
    }
    tmpPoints = (Wrapper*)malloc(sizeof(Wrapper)*size);
    // alloc failure: free pointsCopy and return NULL
    if (tmpPoints == NULL)
    {
        free(pointsCopy);
        spLoggerPrintError(ALLOC_ERROR_MSG, __FILE__, __func__, __LINE__);
        return NULL;
    }
    for(i=0; i<size; i++)
    {
        tmpPoints[i].point = spPointCopy(points[i]);
        tmpPoints[i].index = i;
        pointsCopy[i] = spPointCopy(points[i]);

        // in case of allocation failure
        if (tmpPoints[i].point == NULL || pointsCopy[i] == NULL)
        {
            spLoggerPrintError(ALLOC_ERROR_MSG, __FILE__, __func__, __LINE__);
            for (j = 0; j <= i; j++)
            {
                spPointDestroy(tmpPoints[j].point);
                spPointDestroy(pointsCopy[j]);
            }
            free(pointsCopy);
            free(tmpPoints);
            return NULL;
        }
    }

    dim = spPointGetDimension(points[0]);
    //create a new KDArray.
    SPKDArray* kdArr = spKDArrayCreate(pointsCopy,dim,size);
    if (kdArr == NULL)
    {
        // alloc failure already logged
        for (j = 0; j < size; j++)
        {
            spPointDestroy(tmpPoints[j].point);
            spPointDestroy(pointsCopy[j]);
        }
        free(pointsCopy);
        free(tmpPoints);
        return NULL;
    }
    /* in order to calculate the index matrix we use our wrapper struct array,
       each time we sort it by a different coordinate,
       and then we copy the indexes of the points into the correct matrix line */
    for(i=0; i<dim; i++)
    {
        sortedDim = i;
        qsort(tmpPoints,size,sizeof(Wrapper),pntCmp);
        for(j=0; j<size; j++)
        {
            kdArr->pointsMat[i][j] = tmpPoints[j].index;
        }
    }
    for(i=0; i<size; i++)
    {
        spPointDestroy(tmpPoints[i].point);
    }
    free(tmpPoints);
    return kdArr;
}

int pntCmp(const void* pntA,const void* pntB)
{
    Wrapper W1 =*((Wrapper*)pntA);
    Wrapper W2 =*((Wrapper*)pntB);
    double diff = spPointGetAxisCoor(W1.point,sortedDim) - spPointGetAxisCoor(W2.point,sortedDim);
    if(diff<0) return -1;
    if(diff>0) return 1;
    return 0;
}

SplitRes* spKDArraySplit(SPKDArray *kdArr,int coor)
{
    int i,j,index,ctrLeft = 0,ctrRight = 0,oldIndex,newIndex,leftSize,rightSize;
    int *newIndexes,*isInKdleft;
    SPPoint* left;
    SPPoint* right;
    SplitRes* spRes;
    //calculate sizes for sides of KDARRAYS.
    leftSize = ceil(kdArr->size / 2.0);
    rightSize = kdArr->size - leftSize;

    isInKdleft = (int*)calloc(kdArr->size,sizeof(int));
    newIndexes = (int*)malloc(sizeof(int)*kdArr->size);
    left = (SPPoint*)malloc(leftSize*sizeof(SPPoint));
    right = (SPPoint*)malloc(rightSize*sizeof(SPPoint));

    if (right == NULL || left==NULL || newIndexes==NULL || isInKdleft==NULL)
    {
        free(isInKdleft);
        free(newIndexes);
        free(left);
        free(right);
        spLoggerPrintError(ALLOC_ERROR_MSG, __FILE__, __func__, __LINE__);
        return NULL;
    }
    //copy the points into split's left side.
    for (i=0; i<leftSize; i++)
    {
        index = kdArr->pointsMat[coor][i];
        left[i] = spPointCopy(kdArr->points[index]);
        if(left[i]==NULL)
        {
            // alloc failure
            spPointsArrayDestroy(left, i);
            free(isInKdleft);
            free(newIndexes);
            free(right);
            spLoggerPrintError(ALLOC_ERROR_MSG, __FILE__, __func__, __LINE__);
            return NULL;
        }
        newIndexes[index]=i;
        isInKdleft[index]= 1;
    }
    //copy the points into split's right side.
    for (i=leftSize; i<kdArr->size; i++)
    {
        index = kdArr->pointsMat[coor][i];
        right[i-leftSize] = spPointCopy(kdArr->points[index]);
        if(right[i-leftSize]==NULL)
        {
            // alloc failure
            spPointsArrayDestroy(right, i-leftSize);
            spPointsArrayDestroy(left, leftSize);
            free(isInKdleft);
            free(newIndexes);
            spLoggerPrintError(ALLOC_ERROR_MSG, __FILE__, __func__, __LINE__);
            return NULL;
        }
        newIndexes[index]=i-leftSize;
    }

    spRes = (SplitRes*)malloc(sizeof(SplitRes));
    if (spRes == NULL)
    {
        free(isInKdleft);
        free(newIndexes);
        spPointsArrayDestroy(left, leftSize);
        spPointsArrayDestroy(right, rightSize);
        spLoggerPrintError(ALLOC_ERROR_MSG, __FILE__, __func__, __LINE__);
        return NULL;
    }
    spRes->kdLeft = spKDArrayCreate(left,kdArr->dim,leftSize);
    spRes->kdRight = spKDArrayCreate(right,kdArr->dim,rightSize);

    if (spRes->kdRight == NULL ||spRes->kdLeft == NULL)
    {
        // alloc failure: already logged
        free(isInKdleft);
        free(newIndexes);
        spPointsArrayDestroy(left, leftSize);
        spPointsArrayDestroy(right, rightSize);
        spKDArrayDestroy(spRes->kdLeft);
        free(spRes);
        return NULL;
    }

    // update the KDArrays Matrices based on their "parent" KDArray.
    for(i=0; i<kdArr->dim; i++)
    {
        ctrLeft = 0;
        ctrRight = 0;
        for(j=0; j<kdArr->size; j++)
        {
            oldIndex = kdArr->pointsMat[i][j];
            newIndex = newIndexes[oldIndex];
            if(isInKdleft[oldIndex])
            {
                spRes->kdLeft->pointsMat[i][ctrLeft] = newIndex;
                ctrLeft++;
            }
            else
            {
                spRes->kdRight->pointsMat[i][ctrRight] = newIndex;
                ctrRight++;
            }
        }
    }
    //after finishing splitting we can destroy the kdarray.
    spKDArrayDestroy(kdArr);
    free(isInKdleft);
    free(newIndexes);

    return spRes;
}

void spPointsArrayDestroy(SPPoint* points, int size)
{
    if (points == NULL) return;
    int j;
    for (j=0; j<size; j++)
    {
        spPointDestroy(points[j]);
    }
    free(points);
}

void spKDArrayDestroy(SPKDArray *kdArr)
{
    int i;
    if(kdArr==NULL) return;
    spPointsArrayDestroy(kdArr->points,kdArr->size);
    for(i=0; i<kdArr->dim; i++)
    {
        free(kdArr->pointsMat[i]);
    }
    free(kdArr->pointsMat);
    free(kdArr);
}

void splitResDestroy(SplitRes* spRes)
{
    if (spRes==NULL) return;
    spKDArrayDestroy(spRes->kdLeft);
    spKDArrayDestroy(spRes->kdRight);
    free(spRes);
}
