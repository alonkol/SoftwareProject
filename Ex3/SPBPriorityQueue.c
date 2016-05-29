#include "SPBPriorityQueue.h"
#include "SPListElement.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

struct node* copyNode(struct node* n);
struct node* newNode(int index, double value);

struct sp_bp_queue_t{
  struct node* first;
  struct node* last;
  int maxSize;
  int size;
};

struct node{
    SPListElement element;
    struct node* next;
};


/**
 * TODO Complete documentation
 */
SPBPQueue spBPQueueCreate(int maxSize){
    struct sp_bp_queue_t *q = malloc(sizeof(struct sp_bp_queue_t));
    q->maxSize = maxSize;
    q->size = 0;
    q->first=NULL;
    q->last=NULL;
    return q;
}

/**
 * TODO Complete documentation
 */
SPBPQueue spBPQueueCopy(SPBPQueue source){
    SPBPQueue res = spBPQueueCreate(source->maxSize);
    //struct sp_bp_queue_t *res = malloc(sizeof(struct sp_bp_queue_t));
    if (!res){
        return NULL;
     }
     //spBPQueueCreate
    //res->maxSize = source->maxSize;
    res->size = source->size;
    printQueue(res);
    struct node *n = source->first;
    struct node *newNode;
    struct node *prevNode;

    res->first = copyNode(n);
    printQueue(res);
    prevNode = res->first;
    n = n->next;

    while(n){
        newNode = copyNode(n);
        prevNode->next = newNode;

        prevNode = newNode;
        n = n->next;
        printQueue(res);
    }
    res->last = prevNode;

    return res;
}


/**
 * TODO Complete documentation
 */
 struct node* copyNode(struct node *n){
     struct node *res = malloc(sizeof(struct node));
     if (!res){
        return NULL;
     }

     res->element = spListElementCopy(n->element);
     res->next=NULL;
     return res;
 }

 /**
 * TODO Complete documentation
 */
 struct node* newNode(int index, double value){
     struct node *res = malloc(sizeof(struct node));
     if (!res){
        return NULL;
     }

     res->element = spListElementCreate(index, value);
    res->next=NULL;
     return res;
 }


/**
 * TODO Complete documentation
 */
void spBPQueueDestroy(SPBPQueue source){
    spBPQueueClear(source);
    free(source);
}

/**
 * TODO Complete documentation
 */
void spBPQueueClear(SPBPQueue source){
    struct node* n = source->first;
    struct node* next = n->next;
    while(n){
        next = n->next;
        spListElementDestroy(n->element);
        free(n);
        n=NULL;
        n = next;
    }
    source->size = 0;
    source->first=NULL;
    source->last=NULL;
}

/**
 * TODO Complete documentation
 */
int spBPQueueSize(SPBPQueue source){
    return source->size;
}

/**
 * TODO Complete documentation
 */
int spBPQueueGetMaxSize(SPBPQueue source){
    return source->maxSize;
}

/**
 * TODO Complete documentation
 */
SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element){

    if (!source || !element){
        return SP_BPQUEUE_INVALID_ARGUMENT;
    }

    struct node* new_node = newNode(spListElementGetIndex(element), spListElementGetValue(element));
    if (!new_node){
        return SP_BPQUEUE_OUT_OF_MEMORY;
    }

    struct node* n = source->first;
    int i;

    if (source->size == source->maxSize){
        if (spListElementCompare(new_node->element, source->last->element) == 1) return SP_BPQUEUE_FULL;
        free(source->last);

        for(i=0;i<source->size-2;i++){
            n=n->next;
        }
        n->next=NULL;
    }
    n=source->first;
    if (source->first==NULL){
        source->first = new_node;
        source->last = new_node;
    } else {
        while (n->next && spListElementCompare(n->next->element, new_node->element) == -1){
            n = n->next;
        }

        if (!n->next){
            source->last = new_node;
        }

        struct node* oldNext = n->next;
        n->next = new_node;
        new_node->next = oldNext;
    }

    if (source->size == source->maxSize){
        n = source->first;
        for (i=0; i < source->size - 1; i++){
            n = n->next;
        }
        source->last = n;
    } else {
        source->size = source->size + 1;
    }

	return SP_BPQUEUE_SUCCESS;
}

/**
 * TODO Complete documentation
 */
SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source){

    if (!source) return SP_BPQUEUE_INVALID_ARGUMENT;
    if (source->size == 0) return SP_BPQUEUE_EMPTY;

    struct node* nextNode = source->first->next;
    spListElementDestroy(source->first->element);
    free(source->first);
    source->first = nextNode;
    source->size -= 1;

    return SP_BPQUEUE_SUCCESS;
}

/**
 * TODO Complete documentation
 */
SPListElement spBPQueuePeek(SPBPQueue source){
    return spListElementCopy(source->first->element);
}

/**
 * TODO Complete documentation
 */
SPListElement spBPQueuePeekLast(SPBPQueue source){
    return spListElementCopy(source->last->element);
}

/**
 * TODO Complete documentation
 */
double spBPQueueMinValue(SPBPQueue source){
    if (!source || source->size==0) return -1;
    return spListElementGetValue(source->first->element);
}

/**
 * TODO Complete documentation
 */
double spBPQueueMaxValue(SPBPQueue source){
    if (!source || source->size==0) return -1;
    return spListElementGetValue(source->last->element);
}

/**
 * TODO Complete documentation
 */
bool spBPQueueIsEmpty(SPBPQueue source){
    assert(source);
    return source->size==0;
}

/**
 * TODO Complete documentation
 */
bool spBPQueueIsFull(SPBPQueue source){
    assert(source);
    return source->size==source->maxSize;
}


/** REMOVE LATER **/
void printQueue(SPBPQueue q){
    printf("size: %d\n", spBPQueueSize(q));
    printf("maxSize: %d\n", spBPQueueGetMaxSize(q));

    struct node* n = q->first;
    while(n){
        printf("(%d,%f)->",spListElementGetIndex(n->element),spListElementGetValue(n->element));
        n = n->next;
    }
    printf(" \n");
}

