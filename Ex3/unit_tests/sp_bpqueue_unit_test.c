#include <stdio.h>
#include "../SPBPriorityQueue.h"
#include "unit_test_util.h"
#include "../SPListElement.h"

static bool CreateQueueTest(){
    SPBPQueue q = spBPQueueCreate(4);
    ASSERT_TRUE(spBPQueueGetMaxSize(q)==4);
    ASSERT_TRUE(spBPQueueSize(q)==0);
    ASSERT_TRUE(spBPQueuePeek(q)==NULL);
    ASSERT_TRUE(spBPQueuePeekLast(q)==NULL);
    spBPQueueDestroy(q);
    ASSERT_TRUE(spBPQueueCreate(-1)==NULL);
    return true;
}

static bool EnqueueDequeueTest(){
    SPBPQueue q = spBPQueueCreate(4);
    SPListElement e1 =spListElementCreate(5, 1.0);
    SPListElement e2 =spListElementCreate(5, 2.0);
    SPListElement e3 =spListElementCreate(2, 10.0);
    SPListElement e4 =spListElementCreate(3, 2.1);

    spBPQueueEnqueue(q, e1);
    spBPQueueEnqueue(q, e2);
    spBPQueueEnqueue(q, e3);
    spBPQueueEnqueue(q, e4);

    SPListElement e = spBPQueuePeek(q);
    ASSERT_TRUE(spListElementCompare(e,e1)==0);
    spBPQueueDequeue(q);
    spListElementDestroy(e);
    e=spBPQueuePeek(q);
    ASSERT_TRUE(spListElementCompare(e,e2)==0);
    spBPQueueDequeue(q);
    spListElementDestroy(e);
    e=spBPQueuePeek(q);
    ASSERT_TRUE(spListElementCompare(e,e4)==0);
    spBPQueueDequeue(q);
    spListElementDestroy(e);
    e=spBPQueuePeek(q);
    ASSERT_TRUE(spListElementCompare(e,e3)==0);
    spBPQueueDequeue(q);
    ASSERT_TRUE(spBPQueueDequeue(q)==SP_BPQUEUE_EMPTY);

    spBPQueueDestroy(q);
    spListElementDestroy(e);
    spListElementDestroy(e1);
    spListElementDestroy(e2);
    spListElementDestroy(e3);
    spListElementDestroy(e4);
    return true;
}

static bool SizeQueueTest(){
    SPBPQueue q = spBPQueueCreate(2);
    SPListElement e1 =spListElementCreate(5, 1.0);
    SPListElement e2 =spListElementCreate(5, 2.0);
    ASSERT_TRUE(spBPQueueSize(q)==0);
    spBPQueueEnqueue(q, e1);
    ASSERT_TRUE(spBPQueueSize(q)==1);
    spBPQueueEnqueue(q, e2);
    ASSERT_TRUE(spBPQueueSize(q)==2);
    spBPQueueDequeue(q);
    ASSERT_TRUE(spBPQueueSize(q)==1);
    spBPQueueDestroy(q);
    spListElementDestroy(e1);
    spListElementDestroy(e2);
    return true;
}

static bool EmptyFullQueueTest(){
    SPBPQueue q = spBPQueueCreate(2);
    ASSERT_TRUE(spBPQueueIsEmpty(q));
    ASSERT_FALSE(spBPQueueIsFull(q));
    SPListElement e1 = spListElementCreate(5, 1.0);
    SPListElement e2 = spListElementCreate(5, 2.0);
    spBPQueueEnqueue(q, e1);
    spBPQueueEnqueue(q, e2);
    ASSERT_FALSE(spBPQueueIsEmpty(q));
    ASSERT_TRUE(spBPQueueIsFull(q));
    spBPQueueClear(q);
    ASSERT_TRUE(spBPQueueIsEmpty(q));
    ASSERT_FALSE(spBPQueueIsFull(q));
    spBPQueueDestroy(q);
    spListElementDestroy(e1);
    spListElementDestroy(e2);
    return true;
}

static bool CopyQueueTest(){
    SPBPQueue q = spBPQueueCreate(2);
    SPListElement e1 = spListElementCreate(5, 1.0);
    SPListElement e2 = spListElementCreate(5, 2.0);
    spBPQueueEnqueue(q, e1);
    spBPQueueEnqueue(q, e2);
    SPBPQueue copyq = spBPQueueCopy(q);
    SPListElement peeker, cpeeker;

    ASSERT_TRUE(spBPQueueSize(q)==spBPQueueSize(copyq));
    ASSERT_TRUE(spBPQueueGetMaxSize(q)==spBPQueueGetMaxSize(copyq));

    peeker=spBPQueuePeek(q);
    cpeeker = spBPQueuePeek(copyq);
    ASSERT_TRUE(spListElementCompare(peeker,cpeeker)==0);
    spListElementDestroy(peeker);
    spListElementDestroy(cpeeker);

    peeker=spBPQueuePeekLast(q);
    cpeeker = spBPQueuePeekLast(copyq);
    ASSERT_TRUE(spListElementCompare(peeker,cpeeker)==0);
    spListElementDestroy(peeker);
    spListElementDestroy(cpeeker);

    spBPQueueDequeue(q);
    peeker=spBPQueuePeek(q);
    cpeeker = spBPQueuePeek(copyq);
    ASSERT_FALSE(spListElementCompare(peeker,cpeeker)==0);
    spListElementDestroy(peeker);
    spListElementDestroy(cpeeker);

    spBPQueueDestroy(q);
    spBPQueueDestroy(copyq);
    spListElementDestroy(e1);
    spListElementDestroy(e2);
    return true;
}

static bool PeekQueueTest(){
    SPBPQueue q = spBPQueueCreate(4);
    SPListElement e1 =spListElementCreate(5, 1.0);
    SPListElement e2 =spListElementCreate(5, 2.0);
    SPListElement e3 =spListElementCreate(2, 10.0);
    SPListElement e4 =spListElementCreate(3, 2.1);

    ASSERT_TRUE(spBPQueuePeek(q)==NULL);
    ASSERT_TRUE(spBPQueuePeekLast(q)==NULL);

    spBPQueueEnqueue(q, e1);
    spBPQueueEnqueue(q, e2);
    spBPQueueEnqueue(q, e3);
    spBPQueueEnqueue(q, e4);

    SPListElement ePeek= spBPQueuePeek(q);
    SPListElement ePeekLast = spBPQueuePeekLast(q);

    SPListElement peeker = spBPQueuePeek(q);
    ASSERT_TRUE(spListElementCompare(peeker,ePeek)==0);
    spListElementDestroy(peeker);

    peeker = spBPQueuePeekLast(q);
    ASSERT_TRUE(spListElementCompare(peeker,ePeekLast)==0);
    spListElementDestroy(peeker);

    spBPQueueDequeue(q);
    peeker = spBPQueuePeek(q);
    ASSERT_FALSE(spListElementCompare(peeker,ePeek)==0);
    spListElementDestroy(peeker);

    spBPQueueDestroy(q);
    spListElementDestroy(e1);
    spListElementDestroy(e2);
    spListElementDestroy(e3);
    spListElementDestroy(e4);
    spListElementDestroy(ePeek);
    spListElementDestroy(ePeekLast);

    return true;
}

static bool MaxMinQueueTest(){
    SPBPQueue q = spBPQueueCreate(4);
    SPListElement e1 =spListElementCreate(5, 1.0);
    SPListElement e2 =spListElementCreate(5, 2.0);
    SPListElement e3 =spListElementCreate(2, 10.0);
    SPListElement e4 =spListElementCreate(3, 2.1);

    spBPQueueEnqueue(q, e1);
    spBPQueueEnqueue(q, e2);
    spBPQueueEnqueue(q, e3);
    spBPQueueEnqueue(q, e4);

    ASSERT_TRUE(spBPQueueMaxValue(q)==10.0);
    ASSERT_TRUE(spBPQueueMinValue(q)==1.0);
    spBPQueueDequeue(q);
    ASSERT_TRUE(spBPQueueMinValue(q)==2.0);

    spBPQueueDestroy(q);
    spListElementDestroy(e1);
    spListElementDestroy(e2);
    spListElementDestroy(e3);
    spListElementDestroy(e4);
    return true;
}

static bool EnqueueWhenFullQueueTest(){
    SPBPQueue q = spBPQueueCreate(4);
    SPListElement e1 =spListElementCreate(5, 1.0);
    SPListElement e2 =spListElementCreate(5, 2.0);
    SPListElement e3 =spListElementCreate(2, 10.0);
    SPListElement e4 =spListElementCreate(3, 2.1);
    spBPQueueEnqueue(q, e1);
    spBPQueueEnqueue(q, e2);
    spBPQueueEnqueue(q, e3);
    spBPQueueEnqueue(q, e4);

    ASSERT_TRUE(spBPQueueIsFull(q));

    SPListElement e5 =spListElementCreate(4, 20.0);
    SPListElement e6 =spListElementCreate(3, 1.5);

    ASSERT_TRUE(spBPQueueEnqueue(q, e5)==SP_BPQUEUE_FULL);
    ASSERT_FALSE(spBPQueueEnqueue(q, e6)==SP_BPQUEUE_FULL);

    spBPQueueDestroy(q);
    spListElementDestroy(e1);
    spListElementDestroy(e2);
    spListElementDestroy(e3);
    spListElementDestroy(e4);
    spListElementDestroy(e5);
    spListElementDestroy(e6);
    return true;
}

int main(){
    RUN_TEST(CreateQueueTest);
    RUN_TEST(EnqueueDequeueTest);
    RUN_TEST(SizeQueueTest);
    RUN_TEST(EmptyFullQueueTest);
    RUN_TEST(CopyQueueTest);
    RUN_TEST(PeekQueueTest);
    RUN_TEST(MaxMinQueueTest);
    RUN_TEST(EnqueueWhenFullQueueTest);
    return 0;
}

