#include <stdio.h>
#include "SPBPriorityQueue.h"

void printQueue(SPBPQueue q);

int main(){
    SPBPQueue q = spBPQueueCreate(4);
    spBPQueueEnqueue(q, spListElementCreate(5, 1.0));
    printQueue(q);
    spBPQueueEnqueue(q, spListElementCreate(5, 2.0));
     printQueue(q);
    spBPQueueEnqueue(q, spListElementCreate(2, 10.0));
     printQueue(q);
    spBPQueueEnqueue(q, spListElementCreate(3, 2.1));
     printQueue(q);
    //spBPQueueEnqueue(q, spListElementCreate(7, 2.05));
     //printQueue(q);
    spBPQueueEnqueue(q, spListElementCreate(100, 20.0));
    printQueue(q);
    return 0;
}


