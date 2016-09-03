#include "SPBPriorityQueue.h"
#include "SPKDTree.h"

SPBPQueue kNearestNeighbors(SPConfig config,SPKDTreeNode *root, SPPoint p);
void kNearestNeighborsRec(SPKDTreeNode *curr, SPBPQueue bpq, SPPoint p);
bool diffLessThanPrio(SPKDTreeNode *curr,SPBPQueue bpq, SPPoint p);
