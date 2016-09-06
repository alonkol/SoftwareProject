#include "SPPoint.h"
#include "SPLogger.h"

typedef struct sp_kd_array
{
    int dim; //dim for each points
    int size; //number of points
    SPPoint* points;
    int** pointsMat;
} SPKDArray;

typedef struct split_res
{
    SPKDArray *kdLeft;
    SPKDArray *kdRight;
} SplitRes;

SPKDArray* spKDArrayCreate(SPPoint* points,int dim,int size);
void spKDArrayDestroy(SPKDArray *kdArr);
SPKDArray* spKDArrayInit(SPPoint* arr,int size);
int pntCmp(const void* pntA,const void* pntB);
SplitRes* spKDArraySplit(SPKDArray *kdArr,int coor);
void splitResDestroy(SplitRes* spRes);
void printKDARR(SPKDArray *k);
void spPointsArrayDestroy(SPPoint* points, int size);
