//#include "SPPoint.h"
#include "SPKDTree.h"
#include <stdlib.h>
#define ALLOC_ERROR_MSG "Memory Allocation Error."


SPKDTreeNode* spKDTreeCreate(SPKDArray* kdArr, SPConfig config){
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


    if (kdArr->size == 1){
        res = (SPKDTreeNode*)malloc(sizeof(SPKDTreeNode));
        if (res == NULL){
            spLoggerPrintError(ALLOC_ERROR_MSG, __FILE__, __func__, __LINE__);
            return NULL;
        }
        res->data = spPointCopy(*kdArr->points);
        res->dim=-1;
        res->left=NULL;
        res->right=NULL;
        //spKDArrayDestroy(kdArr);
        printf("Leaf added\n");
        return res;
    }

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
    splitKDArrs = spKDArraySplit(kdArr,dim);
    if(splitKDArrs==NULL){
        return NULL;
    }
    res = (SPKDTreeNode*)malloc(sizeof(SPKDTreeNode));
    if (res == NULL){
        spLoggerPrintError(ALLOC_ERROR_MSG, __FILE__, __func__, __LINE__);
        splitResDestroy(splitKDArrs);
        return NULL;
    }
    res->dim = dim;
    //printf("::%d %d\n",splitKDArrs->kdLeft->size,splitKDArrs->kdRight->size);
    medianPnt = splitKDArrs->kdLeft->points[splitKDArrs->kdLeft->size - 1];
    res->val = spPointGetAxisCoor(medianPnt, dim);
    res->data=NULL;
    res->left = spKDTreeCreateRec(splitKDArrs->kdLeft, method, dim);
    res->right = spKDTreeCreateRec(splitKDArrs->kdRight, method, dim);
    if (res->right == NULL || res->left == NULL){
        // alloc failure: already logged
        printf("NULLLLLLLLL\n");
        destroyKDTree(res); // frees both nodes
        splitResDestroy(splitKDArrs);
        return NULL;
    }

    //splitResDestroy(splitKDArrs); //causes problems
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
