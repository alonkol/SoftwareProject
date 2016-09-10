//#include "SPPoint.h"
#include "SPKDTree.h"
#include <stdlib.h>
#define ALLOC_ERROR_MSG "Memory Allocation Error."

SPKDTreeNode* spKDTreeCreate(SPPoint* allFeats, int featArrSize, SPConfig config){
    SPKDArray* kdArr = spKDArrayInit(allFeats,featArrSize);
    if(kdArr==NULL) return NULL;
    return spKDTreeCreateRec(kdArr, getSplitMethod(config), 0);
}

SPKDTreeNode* spKDTreeCreateRec(SPKDArray* kdArr, SPLIT_METHOD method, int prevDim){
    int dim,i;
    double spread;
    double maxSpread = 0;
    int maxDim = 0;
    SPPoint medianPnt;
    SPKDTreeNode* res;
    SPPoint maxPnt;
    SPPoint minPnt;
    SplitRes* splitKDArrs;

    // on kdArray of size 1, create new leaf node, and destroy kdArray.
    if (kdArr->size == 1){
        res = (SPKDTreeNode*)malloc(sizeof(SPKDTreeNode));
        if (res == NULL){
            spLoggerPrintError(ALLOC_ERROR_MSG, __FILE__, __func__, __LINE__);
            return NULL;
        }
        res->data = spPointCopy(kdArr->points[0]);
        res->dim=-1;
        res->left=NULL;
        res->right=NULL;
        spKDArrayDestroy(kdArr);
        return res;
    }

    // set dim with respect to the method
    if (method == RANDOM){
        dim = rand()% kdArr->dim;
    } else if (method == MAX_SPREAD){
        for (i = 0; i < kdArr->dim; i++){
            maxPnt = kdArr->points[kdArr->pointsMat[i][kdArr->size - 1]];
            minPnt = kdArr->points[kdArr->pointsMat[i][0]];
            spread = spPointGetAxisCoor(maxPnt, i) - spPointGetAxisCoor(minPnt, i);

            if (spread > maxSpread){
                maxSpread = spread;
                maxDim = i;
            }
        }
        dim = maxDim;
    } else if (method == INCREMENTAL){
        dim = (prevDim + 1) % kdArr->dim;
    }

    // split to two kdArrays
    splitKDArrs = spKDArraySplit(kdArr,dim);
    if(splitKDArrs==NULL){
        return NULL;
    }

    // allocate node to return
    res = (SPKDTreeNode*)malloc(sizeof(SPKDTreeNode));
    if (res == NULL){
        spLoggerPrintError(ALLOC_ERROR_MSG, __FILE__, __func__, __LINE__);
        splitResDestroy(splitKDArrs);
        return NULL;
    }

    // set node values
    res->dim = dim;
    medianPnt = splitKDArrs->kdLeft->points[splitKDArrs->kdLeft->size - 1];
    res->val = spPointGetAxisCoor(medianPnt, dim);
    res->data=NULL;

    // set left and right nodes recursively
    res->left = spKDTreeCreateRec(splitKDArrs->kdLeft, method, dim);
    res->right = spKDTreeCreateRec(splitKDArrs->kdRight, method, dim);

    if (res->right == NULL || res->left == NULL){
        // alloc failure (already logged)
        destroyKDTree(res); // frees both nodes
        splitResDestroy(splitKDArrs);
        return NULL;
    }

    // free splitKDArrs, children already destroyed on recursive call
    free(splitKDArrs);
    return res;
}

void destroyKDTree(SPKDTreeNode* node){
    if (node==NULL) return;
    destroyKDTree(node->left);
    destroyKDTree(node->right);
    spPointDestroy(node->data);
    free(node);
}

bool isLeaf(SPKDTreeNode* node){
    return (node->dim==-1);
}
