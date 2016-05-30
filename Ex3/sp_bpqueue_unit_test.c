#include <stdio.h>
#include "SPBPriorityQueue.h"

void printQueue(SPBPQueue q);

int main(){
    SPBPQueue q = spBPQueueCreate(4);
    printf("Empty - %d\n",spBPQueueIsEmpty(q));
    spBPQueueEnqueue(q, spListElementCreate(5, 1.0));
    printQueue(q);
    spBPQueueEnqueue(q, spListElementCreate(5, 2.0));
     printQueue(q);
    spBPQueueEnqueue(q, spListElementCreate(2, 10.0));
     printQueue(q);
    spBPQueueEnqueue(q, spListElementCreate(3, 2.1));
     printQueue(q);
    printf("Full - %d\n",spBPQueueIsFull(q));
    spBPQueueEnqueue(q, spListElementCreate(7, 2.05));
     printQueue(q);
    spBPQueueEnqueue(q, spListElementCreate(100, 20.0));
    printQueue(q);
    spBPQueueEnqueue(q, spListElementCreate(100, 1.5));
    printQueue(q);
    printf("COPY\n");
    SPBPQueue copyq = spBPQueueCopy(q);
    printQueue(copyq);
    spBPQueueClear(copyq);
    printQueue(copyq);
    spBPQueueDestroy(copyq);
    printf("size=%d ",spBPQueueSize(q));
    printf("Maxsize=%d\n",spBPQueueGetMaxSize(q));
    spBPQueueDequeue(q);
    printQueue(q);
    printf("First=%d,%f\n",spListElementGetIndex(spBPQueuePeek(q)),spListElementGetValue(spBPQueuePeek(q)));
    printf("Last=%d,%f\n",spListElementGetIndex(spBPQueuePeekLast(q)),spListElementGetValue(spBPQueuePeekLast(q)));
    printf("Max=%f ",spBPQueueMaxValue(q));
    printf("Min=%f\n",spBPQueueMinValue(q));
    printf("Empty - %d ",spBPQueueIsEmpty(q));
    printf("Full - %d\n",spBPQueueIsFull(q));
}
