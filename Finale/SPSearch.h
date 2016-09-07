#include "SPBPriorityQueue.h"
#include "SPKDTree.h"

SPBPQueue kNearestNeighbors(SPConfig config,SPKDTreeNode *root, SPPoint p);
int kNearestNeighborsRec(SPKDTreeNode *curr, SPBPQueue bpq, SPPoint p);
bool diffLessThanPrio(SPKDTreeNode *curr,SPBPQueue bpq, SPPoint p);
int cmpfunc(const void *a,const void *b);
