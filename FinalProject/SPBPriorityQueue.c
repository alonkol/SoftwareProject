#include "SPBPriorityQueue.h"
#include "SPListElement.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

struct Qnode* copyNode(struct Qnode* n);
struct Qnode* newNode(int index, double value);

/**
 * The queue is implemented as a linked list, consisted of nodes.
 */
struct sp_bp_queue_t{
  struct Qnode* first;
  struct Qnode* last;
  int maxSize;
  int size;
};

/**
 * A Qnode contains an element and a pointer to the next Qnode.
 */
struct Qnode{
    SPListElement element;
    struct Qnode* next;
};

/**
 * Allocates a Qnode in the memory.
 *
 * @param node to copy
 * @return
 * NULL in case allocation failure occurred
 * Otherwise, the new node is returned
 */
 struct Qnode* newNode(int index, double value){
     struct Qnode *res = (struct Qnode*)malloc(sizeof(struct Qnode));
     if (!res){
        return NULL;
     }

    res->element = spListElementCreate(index, value);
    res->next=NULL;
    return res;
 }

/**
 * Creates a separate copy of a node
 *
 * @param node to be copied
 * @return
 * NULL in case allocation failure occurred
 * Otherwise, the new node is returned
 */
 struct Qnode* copyNode(struct Qnode *n){
     return newNode(spListElementGetIndex(n->element), spListElementGetValue(n->element));
 }

SPBPQueue spBPQueueCreate(int maxSize){
    struct sp_bp_queue_t *q;
    if (maxSize <= 0) return NULL;

    q = (struct sp_bp_queue_t *)malloc(sizeof(struct sp_bp_queue_t));
    if (!q){
        return NULL;
    }

    q->maxSize = maxSize;
    q->size = 0;
    q->first=NULL;
    q->last=NULL;
    return q;
}

SPBPQueue spBPQueueCopy(SPBPQueue source){
    if(source==NULL) return NULL;
    struct Qnode *n;
    struct Qnode *newNode;
    struct Qnode *prevNode;
    SPBPQueue res = spBPQueueCreate(source->maxSize);

    if (!res){
        return NULL;
     }

    res->size = source->size;
    n = source->first;

    res->first = copyNode(n);
    prevNode = res->first;
    n = n->next;

    while(n){
        newNode = copyNode(n);
        prevNode->next = newNode;

        prevNode = newNode;
        n = n->next;
    }
    res->last = prevNode;

    return res;
}

void spBPQueueDestroy(SPBPQueue source){
    spBPQueueClear(source);
    free(source);
}

void spBPQueueClear(SPBPQueue source){
    struct Qnode* n;
    struct Qnode* next;

    if (!source || !source->first) return;

    n = source->first;
    next = n->next;

    while(n){
        next = n->next;
        spListElementDestroy(n->element);
        free(n);
        n = next;
    }

    source->size = 0;
    source->first=NULL;
    source->last=NULL;
}

int spBPQueueSize(SPBPQueue source){
    if (!source) return -1;
    return source->size;
}

int spBPQueueGetMaxSize(SPBPQueue source){
    if (!source) return -1;
    return source->maxSize;
}

SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element){
    int i;
    struct Qnode* new_node;
    struct Qnode* n;
    struct Qnode* oldNext;

    if (!source || !element){
        return SP_BPQUEUE_INVALID_ARGUMENT;
    }


    // create new node using the element's data
    new_node = newNode(spListElementGetIndex(element), spListElementGetValue(element));


    if (!new_node){
        return SP_BPQUEUE_OUT_OF_MEMORY;
    }

    n = source->first;

    // if queue is full
    if (source->size >= source->maxSize){
        // compare the new element to the last element
        if (spListElementCompare(new_node->element, source->last->element) == 1){
            spListElementDestroy(new_node->element);
            free(new_node);
            return SP_BPQUEUE_FULL;
        }
        // new element is smaller, therefore destroy current last node
        spListElementDestroy(source->last->element);
        free(source->last);

        // set the new last node pointer to NULL
        for(i=0;i<source->size-2;i++){
            n=n->next;
        }
        n->next=NULL;
    }

    n = source->first;

    // if queue is empty
    if (!source->first){
        source->first = new_node;
        source->last = new_node;
    } else {
        if (spListElementCompare(n->element, new_node->element) == 1){
            // in case new node is smallest
            source->first = new_node;
            new_node->next = n;
        } else {
            while (n->next && spListElementCompare(n->next->element, new_node->element) == -1){
                n = n->next;
            }
            oldNext = n->next;
            n->next = new_node;
            new_node->next = oldNext;
        }
    }

    n = source->first;
    while (n->next){
       n = n->next;
    }
    source->last = n;

    // if queue is not full, increment size
    if (source->size != source->maxSize){
        source->size += 1;
    }

	return SP_BPQUEUE_SUCCESS;
}

SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source){
    struct Qnode* nextNode;

    if (!source) return SP_BPQUEUE_INVALID_ARGUMENT;
    if (source->size == 0) return SP_BPQUEUE_EMPTY;

    nextNode = source->first->next;
    spListElementDestroy(source->first->element);
    free(source->first);
    source->first = nextNode;
    source->size -= 1;

    return SP_BPQUEUE_SUCCESS;
}

SPListElement spBPQueuePeek(SPBPQueue source){
    if (!source) return NULL;
    if (source->size==0) return NULL;
    return spListElementCopy(source->first->element);
}

SPListElement spBPQueuePeekLast(SPBPQueue source){
    if (!source) return NULL;
    if (source->size==0) return NULL;
    return spListElementCopy(source->last->element);
}

double spBPQueueMinValue(SPBPQueue source){
    if (!source || source->size==0) return -1;
    return spListElementGetValue(source->first->element);
}

double spBPQueueMaxValue(SPBPQueue source){
    if (!source || source->size==0) return -1;
    return spListElementGetValue(source->last->element);
}

bool spBPQueueIsEmpty(SPBPQueue source){
    assert(source);
    return source->size==0;
}

bool spBPQueueIsFull(SPBPQueue source){
    assert(source);
    return source->size==source->maxSize;
}



