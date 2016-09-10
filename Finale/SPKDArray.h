#include "SPPoint.h"
#include "SPLogger.h"

/** This file contains structs and functions related to KDARRAY **/

/**
* This struct defines SPKDARRAY:
* @param size - number of points / size of the kd array.
* @param dim - dimension of every point in its points array.
* @param points - array of points.
* @param pointsMat - matrix containing the order of indexes according to each dimension.
**/
typedef struct sp_kd_array
{
    int dim; //dim for each points
    int size; //number of points
    SPPoint* points;
    int** pointsMat;
} SPKDArray;

/**
* This struct defines SplitRes:
* @param kdLeft - points to its left SPKDARRAY.
* @param kdRight - points to its right SPKDARRAY.
**/
typedef struct split_res
{
    SPKDArray *kdLeft;
    SPKDArray *kdRight;
} SplitRes;

/**
* This function creates a new SPKDArray.
* @param points - array of points.
* @param dim - the dimension of each of the points.
* @param size - the size of the points array.
* @return pointer to KDArray in case of success, or NULL in failure.
**/
SPKDArray* spKDArrayCreate(SPPoint* points,int dim,int size);

/**
* This function gets array of points, creates a KDARRAY for it and calculates its index Matrix.
* @param arr - array of points.
* @param size - the size of the points array.
* @return pointer to KDArray in case of success, or NULL in failure.
**/
SPKDArray* spKDArrayInit(SPPoint* arr,int size);

/**
* This is a helper function used to sort features by a specific dimension
**/
int pntCmp(const void* pntA,const void* pntB);

/**
* This function splits a KDARRAY into two KDArrays based on the coordinate given to it.
* @param kdArr - pointer to KDArray.
* @param coor - the dimension you wand the kdArray to be splitted.
* @return SplitRes containg its left and right new KDArrays or NULL in failure.
**/
SplitRes* spKDArraySplit(SPKDArray *kdArr,int coor);

/**
 * The function destroys an array consisted of SPPoint.
 * @param   pointsArr - pointer to array of points.
 * @param   arrSize - size of the array.
 * @return  void
**/
void spPointsArrayDestroy(SPPoint* points, int size);

/**
 * The function destroys a KDArray.
 * @param   KdArr - the KDArray to be destroyed.
 * @return  void
**/
void spKDArrayDestroy(SPKDArray *kdArr);

/**
 * The function destroys a SplitRes.
 * @param   spRes - the splitRes to be destroyed.
 * @return  void
**/
void splitResDestroy(SplitRes* spRes);
