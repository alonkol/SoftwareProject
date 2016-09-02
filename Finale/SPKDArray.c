#include "SPPoint.h"
#include "SPKDArray.h"
#include <stdlib.h>
#include <math.h>

int sortedDim;

typedef struct point_wrapper
{
    SPPoint p;
    int idx;
} Wrapper ;


SPKDArray* init(SPPoint* arr,int size)
{
    Wrapper* tmpPoints;
    int i,dim,j;
    tmpPoints = (Wrapper*)malloc(sizeof(Wrapper)*size);
    for(i=0; i<size; i++)
    {
        tmpPoints[i].p = spPointCopy(arr[i]);
        tmpPoints[i].idx = i;
    }
    dim = spPointGetDimension(arr[0]);
    SPKDArray* kdArr = spKDArrayCreate(arr,dim,size);
    for(i=0; i<dim; i++)
    {
        sortedDim = i;
        qsort(tmpPoints,size,sizeof(Wrapper),pntCmp);
        for(j=0; j<size; j++)
        {
            kdArr->pointsMat[i][j] = tmpPoints[j].idx;
        }
    }

    for(i=0; i<size; i++)
    {
        spPointDestroy(arr[i]);
        //free(tmpPoints[i]);
    }
    free(tmpPoints);
    return kdArr;
}

SPKDArray* spKDArrayCreate(SPPoint* points,int dim,int size)
{
    int i;
    SPKDArray *kdArr = (SPKDArray*)malloc(sizeof(SPKDArray));
    kdArr->dim = dim;
    kdArr->size=size;
    kdArr->points=points;
    kdArr->pointsMat = (int**)malloc(sizeof(int*)*dim);
    for(i=0; i<dim; i++)
    {
        kdArr->pointsMat[i]=(int*)malloc(sizeof(int)*(size));
    }
    return kdArr;
}

int pntCmp(const void* pntA,const void* pntB)
{
    Wrapper W1 =*((Wrapper*)pntA);
    Wrapper W2 =*((Wrapper*)pntB);
    double diff = spPointGetAxisCoor(W1.p,sortedDim) - spPointGetAxisCoor(W2.p,sortedDim);
    if(diff<0) return -1;
    if(diff>0) return 1;
    return 0;
}



SplitRes* split(SPKDArray *kdArr,int coor)
{
    int n = ceil(kdArr->size / 2.0);

    int i,j,index;
    SPPoint* left = (SPPoint*)malloc(sizeof(SPPoint)*n);
    SPPoint* right = (SPPoint*)malloc(sizeof(SPPoint)*(kdArr->size-n));
    for (i=0; i<n; i++)
    {
        index = kdArr->pointsMat[coor][i];
        left[i] = spPointCopy(kdArr->points[index]);
        if(left[i]==NULL) printf("NULL\t");
    }
    for (i=n; i<kdArr->size; i++)
    {
        index = kdArr->pointsMat[coor][i];
        right[i-n] = spPointCopy(kdArr->points[index]);
        if(right[i-n]==NULL) printf("NULL\t");
    }


    printf("Left Points:\n");

    for(i=0;i<n;i++){
        printf("(%d)\t",spPointGetIndex(left[i]));
    }

    printf("right Points:\n");

    for(i=n;i<kdArr->size;i++){
        printf("(%d)\t",spPointGetIndex(right[i-n]));
    }




    SplitRes* spRes = (SplitRes*)malloc(sizeof(SplitRes));
    spRes->kdLeft = spKDArrayCreate(left,kdArr->dim,n);
    spRes->kdRight = spKDArrayCreate(right,kdArr->dim,kdArr->size-n);

    int* isInKdleft = (int*)calloc(kdArr->size,sizeof(int));
    for(i=0; i<n; i++)
    {
        index = kdArr->pointsMat[coor][i];
        isInKdleft[i] = 1;
    }

    int ctrLeft = 0;
    int ctrRight = 0;
    int* newIndexes;

    newIndexes = (int*)malloc(sizeof(int)*kdArr->size);

    for (i = 0; i < kdArr->size; i++)
    {
        if (isInKdleft[i])
        {
            newIndexes[i] = ctrLeft;
            ctrLeft++;
        }
        else
        {
            newIndexes[i] = ctrRight;
            ctrRight++;
        }
    }

    int oldIndex;
    int newIndex;

    for(i=0;i<kdArr->dim;i++){
        ctrLeft = 0;
        ctrRight = 0;
        for(j=0;j<kdArr->size;j++){
            oldIndex = kdArr->pointsMat[i][j];
            newIndex = newIndexes[oldIndex];
            if(isInKdleft[oldIndex]){
                spRes->kdLeft->pointsMat[i][ctrLeft] = newIndex;
                ctrLeft++;
            } else {
                spRes->kdRight->pointsMat[i][ctrRight] = newIndex;
                ctrRight++;
            }
        }
    }

    return spRes;
}

void printKDARR(SPKDArray *k){
    int i,j;
    printf("%d %d \n",k->size, k->dim);
    printf("Points..:\n");
    for(i=0;i<k->size;i++){
        printf("(%d)\t",spPointGetIndex(k->points[i]));
    }
    printf("Mat:\n");
    for(i=0;i<k->dim;i++){
        for(j=0;j<k->size;j++){
            printf("%d\t",k->pointsMat[i][j]);
        }
        printf("\n");
    }

}
