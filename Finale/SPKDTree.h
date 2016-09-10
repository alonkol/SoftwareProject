#include "SPKDArray.h"
#include "SPConfig.h"


// structure of a KDTree node
typedef struct kdnode{
    int dim;
    double val;
    struct kdnode *left;
    struct kdnode *right;
    SPPoint data;
} SPKDTreeNode;

/**
 * This function creates a KDTree.
 * This function is non-recursive, initializes a KDArray and then
 * uses the recursive version, with the parameters relevant to
 * the split method from config.
 *
 * @param allFeats - the features to use when creating the tree.
 * @param featArrSize - the size of allFeats
 * @param config - the configuration structure
 * @return SPKDTreeNode* - a pointer to the root of the new KDTree
 */
SPKDTreeNode* spKDTreeCreate(SPPoint* allFeats, int featArrSize, SPConfig config);

/**
 * A function that creates a KDTree recursively.
 *
 * @param kdArr - a KDArray which contains the features
 * @param method - the split method to use
 * @param prevDim - the dimension used on the previous iteration of the recursive function,
                    this parameter is used specifically when the split method is INCREMENTAL.
 * @return SPKDTreeNode* - a pointer to the root of the new KDTree
 */
SPKDTreeNode* spKDTreeCreateRec(SPKDArray* kdArr, SPLIT_METHOD method, int prevDim);

/**
 * This function destroys a KDTree recursively.
 * @param node - the root of the KDTree to be destroyed
 */
void destroyKDTree(SPKDTreeNode* node);

/**
 * A boolean function that returns true if node is a leaf.
 * @param node
 * @return true if node is a leaf
 */
bool isLeaf(SPKDTreeNode* node);

