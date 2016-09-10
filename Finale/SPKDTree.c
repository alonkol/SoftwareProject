//#include "SPPoint.h"
#include "SPKDTree.h"
#include <stdlib.h>
#define ALLOC_ERROR_MSG "Memory Allocation Error."
int nodeIndex = 0;
int nodes[100]={0};

SPKDTreeNode* spKDTreeCreate(SPPoint* allFeats,int featArrSize, SPConfig config){
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

        res->index=nodeIndex;
        nodes[nodeIndex]=1;
        nodeIndex++;

        spKDArrayDestroy(kdArr);
        //printf("Leaf added\n");
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

    res->index = nodeIndex;
    nodes[nodeIndex]=1;
    nodeIndex++;


    res->left = spKDTreeCreateRec(splitKDArrs->kdLeft, method, dim);
    res->right = spKDTreeCreateRec(splitKDArrs->kdRight, method, dim);

    if (res->right == NULL || res->left == NULL){
        // alloc failure: already logged
        destroyKDTree(res); // frees both nodes
        splitResDestroy(splitKDArrs);
        return NULL;
    }
    //free(splitKDArrs);
    splitResDestroy(splitKDArrs); //causes problems
    return res;
}

void destroyKDTree(SPKDTreeNode* node){
    if (node==NULL) return;
    destroyKDTree(node->left);
    destroyKDTree(node->right);
    spPointDestroy(node->data);


    if(nodes[node->index]==0) printf("TREE PROBLEM!!!! %d\n",node->index);

    nodes[node->index]=0;
    free(node);
    node=NULL;
}

bool isLeaf(SPKDTreeNode* node){
    return (node->dim==-1);
}
void printNodes(){
    printf("KDTREENODES:\n");
    int i;
    for(i=0;i<100;i++){
        printf("%d ",nodes[i]);
    }
    printf("\n\n");
}
