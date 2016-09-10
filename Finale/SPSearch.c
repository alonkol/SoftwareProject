#include "SPSearch.h"
#define ALLOC_ERROR_MSG "Memory Allocation Error."

SPBPQueue kNearestNeighbors(SPConfig config,SPKDTreeNode *root, SPPoint p){
	SPBPQueue bpq = spBPQueueCreate(spConfigGetKNN(config));
	if(kNearestNeighborsRec(root,bpq,p)==-1){
        spBPQueueDestroy(bpq);
        return NULL;
	};
	return bpq;
}


int kNearestNeighborsRec(SPKDTreeNode *curr, SPBPQueue bpq, SPPoint p){
	if (curr == NULL) return 0;

	SPListElement element;

	/* Add the current point to the BPQ */
	if (isLeaf(curr)){
        //create temporary element to enqueue
		element = spListElementCreate(spPointGetIndex(curr->data), spPointL2SquaredDistance(curr->data,p));
		if (element == NULL){
            spLoggerPrintError(ALLOC_ERROR_MSG, __FILE__, __func__, __LINE__);
            return -1;
		}
		// try to queue element to bpq, and handle failure if necessary
		if (spBPQueueEnqueue(bpq, element) == SP_BPQUEUE_OUT_OF_MEMORY){
            spLoggerPrintError(ALLOC_ERROR_MSG, __FILE__, __func__, __LINE__);
            spListElementDestroy(element);
            return -1;
		}

		// destroy temporary element (bpqenqueue uses a copy)
		spListElementDestroy(element);
		return 0;
	}

	/* Recursively search the half of the tree that contains the test point. */
	 if(spPointGetAxisCoor(p, curr->dim) <= curr->val){
		kNearestNeighborsRec(curr->left, bpq, p);
		if (!spBPQueueIsFull(bpq) || diffLessThanPrio(curr,bpq,p)){
			kNearestNeighborsRec(curr->right, bpq, p);
		}
	 } else {
		kNearestNeighborsRec(curr->right, bpq, p);
		if (!spBPQueueIsFull(bpq) || diffLessThanPrio(curr,bpq,p)){
			kNearestNeighborsRec(curr->left, bpq, p);
		}
	 }
	 return 0;
}

bool diffLessThanPrio(SPKDTreeNode *curr,SPBPQueue bpq, SPPoint p){
	double diff = (curr->val - spPointGetAxisCoor(p,curr->dim));
	return diff*diff < spBPQueueMaxValue(bpq);
}

int cmpfunc(const void *a,const void *b)
{
    const int* da = (const int*)a;
    const int* db = (const int*)b;
    if(db[1]>da[1]) return 1;
    if(db[1]<da[1]) return -1;
    if(db[0]<da[0]) return 1;
    return -1;
}
