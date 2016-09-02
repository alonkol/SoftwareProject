
#include "SPKDArray.h"
#include "SPConfig.h"

typedef struct kdnode{
    int dim;
    double val;
    struct kdnode *left;
    struct kdnode *right;
    SPPoint data;
} KDTreeNode;

KDTreeNode* create(SPKDArray* kdArr, SPConfig config);
KDTreeNode* createRec(SPKDArray* kdArr, SPLIT_METHOD method, int prevDim);
void destroyKDTree(KDTreeNode* node);
