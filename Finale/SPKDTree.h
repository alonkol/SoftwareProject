
#include "SPKDArray.h"
#include "SPConfig.h"


typedef struct kdnode{
    int dim;
    double val;
    struct kdnode *left;
    struct kdnode *right;
    SPPoint data;
} SPKDTreeNode;

SPKDTreeNode* spKDTreeCreate(SPKDArray* kdArr, SPConfig config);
SPKDTreeNode* spKDTreeCreateRec(SPKDArray* kdArr, SPLIT_METHOD method, int prevDim);
void destroyKDTree(SPKDTreeNode* node);
bool isLeaf(SPKDTreeNode* node);
