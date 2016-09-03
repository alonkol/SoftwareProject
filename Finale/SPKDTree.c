
#include "SPPoint.h"
#include "SPKDTree.h"
#include <stdlib.h>



SPKDTreeNode* spKDTreeCreate(SPKDArray* kdArr, SPConfig config){
    return spKDTreeCreateRec(kdArr, getSplitMethod(config), 0);
}

SPKDTreeNode* spKDTreeCreateRec(SPKDArray* kdArr, SPLIT_METHOD method, int prevDim){
    int dim,i;
    double spread;
    double maxSpread = 0;
    int maxDim = 0;

    if (kdArr->size == 1){
        SPKDTreeNode* res = (SPKDTreeNode*)malloc(sizeof(SPKDTreeNode));
        res->data = *kdArr->points;
        res->dim=-1;
        res->left=NULL;
        res->right=NULL;
        printPoint((res->data));
        return res;
    }

    if (method == RANDOM){
        dim = rand()% kdArr->dim;
    } else if (method == MAX_SPREAD){
        for (i = 0; i < kdArr->dim; i++){
            SPPoint maxPnt = kdArr->points[kdArr->pointsMat[i][kdArr->size - 1]];
            SPPoint minPnt = kdArr->points[kdArr->pointsMat[i][0]];
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
    SplitRes* splitKDArrs = spKDArraySplit(kdArr,dim);

    SPKDTreeNode* res = (SPKDTreeNode*)malloc(sizeof(SPKDTreeNode));
    res->dim = dim;
    res->left = spKDTreeCreateRec(splitKDArrs->kdLeft, method, dim);
    res->right = spKDTreeCreateRec(splitKDArrs->kdRight, method, dim);
    SPPoint medianPnt = splitKDArrs->kdLeft->points[splitKDArrs->kdLeft->size - 1];
    res->val = spPointGetAxisCoor(medianPnt, dim);
    res->data=NULL;
    return res;
}

void destroyKDTree(SPKDTreeNode* node){
    if (node==NULL) return;
    destroyKDTree(node->left);
    destroyKDTree(node->right);
    free(node);
}


