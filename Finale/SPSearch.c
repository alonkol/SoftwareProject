#include "SPSearch.h"


SPBPQueue kNearestNeighbors(SPConfig config,SPKDTreeNode *root, SPPoint p){
	SPBPQueue bpq = spBPQueueCreate(spConfigGetKNN(config));

	kNearestNeighborsRec(root,bpq,p);
	return bpq;
}


void kNearestNeighborsRec(SPKDTreeNode *curr, SPBPQueue bpq, SPPoint p){
	if (curr == NULL) return;

	SPListElement element;

	/* Add the current point to the BPQ. Note that this is a no-op if the
	 * point is not as good as the points we've seen so far.*/
	if (isLeaf(curr)){
		element = spListElementCreate(spPointGetIndex(curr->data), spPointL2SquaredDistance(curr->data,p));
		spBPQueueEnqueue(bpq, element);
		return;
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
}


bool diffLessThanPrio(SPKDTreeNode *curr,SPBPQueue bpq, SPPoint p){
	double diff = (curr->val - spPointGetAxisCoor(p,curr->dim));
	return diff*diff < spBPQueueMaxValue(bpq);
}

