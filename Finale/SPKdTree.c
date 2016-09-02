#include <stdlib.h>


typedef struct kdnode{
    int dim;
    double val;
    KDTreeNode* left = NULL;
    KDTreeNode* right = NULL;
    SPPoint* data = NULL;
} KDTreeNode;

KDTreeNode* create(KDArray kdArr, SPConfig config){
    return createRec(kdArr, getSplitMethod(config), 0);
}

KDTreeNode* createRec(KDArray* kdArr, SPLIT_METHOD method, int prevDim){
    int dim;
    double spread;
    double maxSpread = 0;
    int maxDim = 0;

    if (kdArr->size == 1){
        KDTreeNode* res = (KDTreeNode*)malloc(sizeof(KDTreeNode));
        res->data = kdArr->points;
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
    SplitRes* splitKDArrs = split(kdArr,dim);

    KDTreeNode* res = (KDTreeNode*)malloc(sizeof(KDTreeNode));
    res->dim = dim;
    res->left = createRec(splitKDArrs->kdLeft, method, dim);
    res->right = createRec(splitKDArrs->kdRight, method, dim);
    SPPoint medianPnt = splitKDArrs->kdLeft->points[splitKDArrs->kdLeft->size - 1]
    res->val = spPointGetAxisCoor(medianPnt, dim);
    return res;


}
