#ifndef SPBPRIORITYQUEUE_H_
#define SPBPRIORITYQUEUE_H_
#include "SPListElement.h"
#include <stdbool.h>
/**
 * SP Bounded Priority Queue summary
 * A minimum priority queue with a fixed upper bound on the number of elements
 * that can be stored. if the queue is at capacity, and a new element is added,
 * the element with the highest priority value is ejected from the queue.
 * Elements in the queue will have an int index and a double value.
 *
 * spBPQueueCreate           - creates an empty queue with a given maximum capacity
 * spBPQueueCopy             - creates a copy of a given queue
 * spBPQueueDestroy          - frees all memory allocation associated with the queue
 * spBPQueueClear            - removes all the elements in the queue
 * spBPQueueGetSize          - returns the number of elements in the queue
 * spBPQueueGetMaxSize       - returns the maximum capacity of the queue
 * spBPQueueEnqueue          - Inserts a NEW COPY (must be allocated) element to the queue
 * spBPQueueDequeue          - removes the element with the lowest value
 * spBPQueuePeek             - returns a NEW COPY of the element with the lowest value
 * spBPQueuePeekLast         – returns a NEW COPY of the element with the highest value
 * spBPQueueMinValue         - returns the minimum value in the queue
 * spBPQueueMaxValue         - returns the maximum value in the queue
 * spBPQueueIsEmpty          – returns true if the queue is empty
 * spBPQueueIsFull           - returns true if the queue is full
 *
 */

/** type used to define Bounded priority queue **/
typedef struct sp_bp_queue_t* SPBPQueue;

/** type for error reporting **/
typedef enum sp_bp_queue_msg_t {
	SP_BPQUEUE_OUT_OF_MEMORY,
	SP_BPQUEUE_FULL,
	SP_BPQUEUE_EMPTY,
	SP_BPQUEUE_INVALID_ARGUMENT,
	SP_BPQUEUE_SUCCESS
} SP_BPQUEUE_MSG;

/**
 * Allocates a queue in the memory.
 * Given a maximal size for the queue.
 *
 * @param maxSize - The desired maximal size of the queue
 * @return
 * NULL in case allocation failure ocurred OR maxSize < 0
 * Otherwise, the new queue is returned
 */
SPBPQueue spBPQueueCreate(int maxSize);

/**
 * Creates a separate copy of a queue,
 * Given an original queue.
 *
 * @param source - The queue to be copied
 * @return
 * NULL in case allocation failure ocurred
 * Otherwise, the new queue is returned
 */
SPBPQueue spBPQueueCopy(SPBPQueue source);

/**
 * Free all memory allocation associated with a queue,
 * if queue is NULL nothing happens.
 * @param source - The queue to be destroyed
 */
void spBPQueueDestroy(SPBPQueue source);

/**
 * Free all memory allocation associated with the contents of a queue,
 * not including the queue itself.
 * if queue is NULL nothing happens.
 * @param source - The queue to be cleared
 */
void spBPQueueClear(SPBPQueue source);

/**
 * Get the current size of the queue
 *
 * @param source - The source queue
 * @return
 * Current size of the queue
 * -1 if source is NULL
 */
int spBPQueueSize(SPBPQueue source);

/**
 * Get the maximal size of the queue
 *
 * @param source - The source queue
 * @return
 * Maximal size of the queue
 * -1 if source is NULL
 */
int spBPQueueGetMaxSize(SPBPQueue source);

/**
 * Add a new element to the queue
 *
 * @param SPBPQueue - the source queue
 * @param SPListElement - the element to be added to the queue
 * @return
 * SP_BPQUEUE_OUT_OF_MEMORY     - In case of memory allocation failure
 * SP_BPQUEUE_FULL              - In case queue is full,
 *                                and element's value is larger than any other element.
 * SP_BPQUEUE_INVALID_ARGUMENT  - source or element are NULL
 * SP_BPQUEUE_SUCCESS           - In case the element has been successfully added
 */
SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element);

/**
 * Remove the minimal element from the queue
 *
 * @param SPBPQueue - the source queue
 * @return
 * SP_BPQUEUE_INVALID_ARGUMENT  - source is NULL
 * SP_BPQUEUE_EMPTY             - Queue is empty and there is nothing to dequeue
 * SP_BPQUEUE_SUCCESS           - In case an element has been successfully dequeued
 */
SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source);

/**
 * Get the minimal element in the queue
 *
 * @param SPBPQueue - the source queue
 * @return
 * Minimal element in the queue
 * NULL if source is either NULL or empty.
 */
SPListElement spBPQueuePeek(SPBPQueue source);

/**
 * Get the maximal element in the queue
 *
 * @param SPBPQueue - the source queue
 * @return
 * Maximal element in the queue
 * NULL if source is either NULL or empty.
 */
SPListElement spBPQueuePeekLast(SPBPQueue source);

/**
 * Get the value of the minimal element in the queue
 *
 * @param SPBPQueue - the source queue
 * @return
 * Value of the minimal element in the queue
 * -1 if source is NULL
 */
double spBPQueueMinValue(SPBPQueue source);

/**
 * Get the value of the maximal element in the queue
 *
 * @param SPBPQueue - the source queue
 * @return
 * Value of the maximal element in the queue
 * -1 if source is NULL
 */
double spBPQueueMaxValue(SPBPQueue source);

/**
 * Check whether the queue is empty or not
 *
 * @param SPBPQueue - the source queue
 * @assert source!=NULL
 * @return True if queue is empty
 */
bool spBPQueueIsEmpty(SPBPQueue source);

/**
 * Check whether the queue is full or not
 *
 * @param SPBPQueue - the source queue
 * @assert source!=NULL
 * @return True if queue is full
 */
bool spBPQueueIsFull(SPBPQueue source);

#endif
