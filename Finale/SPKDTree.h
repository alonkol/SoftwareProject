#include "SPKDArray.h"
#include "SPConfig.h"

typedef struct kdnode{
    int dim;
    double val;
    struct kdnode *left;
    struct kdnode *right;
    SPPoint data;
    int index;
} SPKDTreeNode;

SPKDTreeNode* spKDTreeCreate(SPPoint* allFeats,int featArrSize, SPConfig config);
SPKDTreeNode* spKDTreeCreateRec(SPKDArray* kdArr, SPLIT_METHOD method, int prevDim);
void destroyKDTree(SPKDTreeNode* node);
bool isLeaf(SPKDTreeNode* node);
void printNodes();
