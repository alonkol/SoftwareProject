#include <stdio.h>
#include "SPBPriorityQueue.h"

void printQueue(SPBPQueue q);

int main(){
    SPBPQueue q = spBPQueueCreate(4);
    spBPQueueEnqueue(q, spListElementCreate(5, 1.0));
    spBPQueueEnqueue(q, spListElementCreate(5, 2.0));
    spBPQueueEnqueue(q, spListElementCreate(2, 10.0));
    spBPQueueEnqueue(q, spListElementCreate(3, 2.1));
    spBPQueueEnqueue(q, spListElementCreate(7, 2.05));
    spBPQueueEnqueue(q, spListElementCreate(100, 20.0));
    printQueue(q);
}


