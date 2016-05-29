#include "SPPoint.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

/**
 * SPPoint Summary
 * Encapsulates a point with variable length dimension. The coordinates
 * values are double types, and each point has a non-negative index which
 * represents the image index to which the point belongs.
 *
 * The following functions are supported:
 *
 * spPointCreate        	- Creates a new point
 * spPointCopy				- Create a new copy of a given point
 * spPointDestroy 			- Free all resources associated with a point
 * spPointGetDimension		- A getter of the dimension of a point
 * spPointGetIndex			- A getter of the index of a point
 * spPointGetAxisCoor		- A getter of a given coordinate of the point
 * spPointL2SquaredDistance	- Calculates the L2 squared distance between two points
 *
 */

struct sp_point_t{
    double* data;
    int dim;
    int index;
};

/**
 * Allocates a new point in the memory.
 * Given data array, dimension dim and an index.
 * The new point will be P = (p_0,p_2,...,p_{dim-1})
 * such that the following holds
 *
 * - The ith coordinate of the P will be p_i
 * - p_i = data[i]
 * - The index of P = index
 *
 * @return
 * NULL in case allocation failure ocurred OR data is NULL OR dim <=0 OR index <0
 * Otherwise, the new point is returned
 */
SPPoint spPointCreate(double* data, int dim, int index){
    struct sp_point_t* res = malloc(sizeof(struct sp_point_t*));
    if (res == NULL){
        return NULL;
    }

    res->dim = dim;
    res->index = index;

    res->data = (double*)malloc(dim*sizeof(double));
    if (res->data == NULL){
        return NULL;
    }

    int i;
    for (i = 0; i < dim; i++){
        res->data[i] = data[i];
    }

    return res;
}

/**
 * Allocates a copy of the given point.
 *
 * Given the point source, the functions returns a
 * new pint P = (P_1,...,P_{dim-1}) such that:
 * - P_i = source_i (The ith coordinate of source and P are the same)
 * - dim(P) = dim(source) (P and source have the same dimension)
 * - index(P) = index(source) (P and source have the same index)
 *
 * @param source - The source point
 * @assert (source != NULL)
 * @return
 * NULL in case memory allocation occurs
 * Others a copy of source is returned.
 */
SPPoint spPointCopy(SPPoint source){
    assert (source != NULL);

    struct sp_point_t* res = malloc(sizeof(struct sp_point_t*));
    if (res == NULL){
        return NULL;
    }

    res = spPointCreate(source->data, source->dim, source->index);

    return res;
}

/**
 * Free all memory allocation associated with point,
 * if point is NULL nothing happens.
 */
void spPointDestroy(SPPoint point){
    free(point->data);
    free(point);
}

/**
 * A getter for the dimension of the point
 *
 * @param point - The source point
 * @assert point != NULL
 * @return
 * The dimension of the point
 */
int spPointGetDimension(SPPoint point){
    assert (point != NULL);
    return point->dim;
}

/**
 * A getter for the index of the point
 *
 * @param point - The source point
 * @assert point != NULL
 * @return
 * The index of the point
 */
int spPointGetIndex(SPPoint point){
    assert (point != NULL);
    return point->index;
}

/**
 * A getter for specific coordinate value
 *
 * @param point - The source point
 * @param axis  - The coordinate of the point which
 * 				  its value will be retreived
 * @assert point!=NULL && axis < dim(point)
 * @return
 * The value of the given coordinate (p_axis will be returned)
 */
double spPointGetAxisCoor(SPPoint point, int axis){
    assert (point != NULL && axis < point->dim);
    return point->data[axis];
}

/**
 * Calculates the L2-squared distance between p and q.
 * The L2-squared distance is defined as:
 * (p_1 - q_1)^2 + (p_2 - q_1)^2 + ... + (p_dim - q_dim)^2
 *
 * @param p - The first point
 * @param q - The second point
 * @assert p!=NULL AND q!=NULL AND dim(p) == dim(q)
 * @return
 * The L2-Squared distance between p and q
 */
double spPointL2SquaredDistance(SPPoint p, SPPoint q){
    assert (p!=NULL && q!=NULL && p->dim == q->dim);

    double res = 0;
    double diff;

    int i;
    for (i = 0; i < p->dim; i++){
        diff = p->data[i] - q->data[i];
        res += diff*diff;
    }

    return res;
}

// TEMP: print point
void printPoint(SPPoint p){
    printf("dim: %d \n", p->dim);
    printf("index: %d \n", p->index);
    int i;
    for (i = 0; i < p->dim; i++){
        printf(" %f ", p->data[i]);
    }
    printf(" \n");
}
