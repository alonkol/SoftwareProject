
#include "SPPoint.h"
#include "SPKDTree.h"
#include <stdlib.h>



KDTreeNode* create(SPKDArray* kdArr, SPConfig config){
    return createRec(kdArr, getSplitMethod(config), 0);
}

KDTreeNode* createRec(SPKDArray* kdArr, SPLIT_METHOD method, int prevDim){
    int dim,i;
    double spread;
    double maxSpread = 0;
    int maxDim = 0;

    if (kdArr->size == 1){
        KDTreeNode* res = (KDTreeNode*)malloc(sizeof(KDTreeNode));
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

    KDTreeNode* res = (KDTreeNode*)malloc(sizeof(KDTreeNode));
    res->dim = dim;
    res->left = createRec(splitKDArrs->kdLeft, method, dim);
    res->right = createRec(splitKDArrs->kdRight, method, dim);
    SPPoint medianPnt = splitKDArrs->kdLeft->points[splitKDArrs->kdLeft->size - 1];
    res->val = spPointGetAxisCoor(medianPnt, dim);
    res->data=NULL;
    return res;
}

void destroyKDTree(KDTreeNode* node){
    if (node==NULL) return;
    destroyKDTree(node->left);
    destroyKDTree(node->right);
    free(node);
}


