#include "SPBPriorityQueue.h"
#include "SPKDTree.h"

/**
 * This function initializes and returns a BPQ with the K nearest neighbors of a point p.
 *
 * @param config - the configuration structure
 * @param root - a pointer to the root of the KDTree
 * @param p - the query point
 * @return SPBPQueue - the result BPQueue
 */
SPBPQueue kNearestNeighbors(SPConfig config,SPKDTreeNode *root, SPPoint p);

/**
 * This is a recursive function used by kNearestNeighbors to find the K nearest neighbors
 * of a point p. It adds the relevant points to bpq, which is given as input.
 *
 * @param curr - the current node of the KDTree to search
 * @param bpq - the BPQueue to use during the search
 * @param p - the query point
 * @return 0 on success, -1 on failure
 */
int kNearestNeighborsRec(SPKDTreeNode *curr, SPBPQueue bpq, SPPoint p);

/**
 * A boolean function used to calculate a mathematical expression,
 * checking if the squared distance from p to curr is smaller than the maximal value in bpq
 * @param p - the point queried
 * @param bpq - the BPQueue
 * @param curr - the current node in search
 * @return true if and only if (curr_val - p_val)^2 < maxValueInBPQ
 */
bool diffLessThanPrio(SPKDTreeNode *curr,SPBPQueue bpq, SPPoint p);

/**
 * A comparator function used in main.cpp to sort the most relevant pictures.
 */
int cmpfunc(const void *a,const void *b);
