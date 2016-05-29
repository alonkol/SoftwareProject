#include "unit_test_util.h"
#include "../SPListElement.h"
#include "../SPList.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

static SPList quickList(int size, ...) {
	va_list items;
	SPList list = spListCreate();
	va_start(items, size);
	for (int i = 0; i < size; i++) {
		spListInsertLast(list, va_arg(items, SPListElement));
	}
	va_end(items);
	return list;
}

static bool testElementCreate() {
	SPListElement e = spListElementCreate(1, 0.0);
	ASSERT_TRUE(e!=NULL);
	spListElementDestroy(e);
	return true;
}

static bool testElementCompare() {
	SPListElement element1 = spListElementCreate(1, 0.0);
	SPListElement element2 = spListElementCreate(2, 0.0);
	SPListElement element3 = spListElementCreate(1, 1.0);
	SPListElement element4 = spListElementCreate(1, 0.0);
	ASSERT_TRUE(spListElementCompare(element1, element1) == 0);
	ASSERT_TRUE(spListElementCompare(element1, element2) < 0);
	ASSERT_TRUE(spListElementCompare(element2, element1) > 0);
	ASSERT_TRUE(spListElementCompare(element1, element3) < 0);
	ASSERT_TRUE(spListElementCompare(element3, element1) > 0);
	ASSERT_TRUE(spListElementCompare(element1, element4) == 0);
	ASSERT_TRUE(spListElementCompare(element4, element1) == 0);
	spListElementDestroy(element1);
	spListElementDestroy(element2);
	spListElementDestroy(element3);
	spListElementDestroy(element4);
	return true;
}

static bool testElementGetIndex() {
	SPListElement element1 = spListElementCreate(1, 0.0);
	SPListElement element2 = spListElementCreate(2, 0.0);
	SPListElement element3 = spListElementCreate(3, 1.0);
	SPListElement element4 = spListElementCreate(4, 0.0);
	ASSERT_TRUE(spListElementGetIndex(element1) == 1);
	ASSERT_TRUE(spListElementGetIndex(element2) == 2);
	ASSERT_TRUE(spListElementGetIndex(element3) == 3);
	ASSERT_TRUE(spListElementGetIndex(element4) == 4);
	ASSERT_TRUE(spListElementGetIndex(NULL) == -1);
	spListElementDestroy(element1);
	spListElementDestroy(element2);
	spListElementDestroy(element3);
	spListElementDestroy(element4);
	return true;
}

static bool testElementSetIndex() {
	ASSERT_TRUE(spListElementGetIndex(NULL) == -1);
	ASSERT_TRUE(spListElementSetIndex(NULL,-1) ==SP_ELEMENT_INVALID_ARGUMENT);
	ASSERT_TRUE(spListElementSetIndex(NULL,1) ==SP_ELEMENT_INVALID_ARGUMENT);
	ASSERT_TRUE(spListElementSetIndex(NULL,-1) ==SP_ELEMENT_INVALID_ARGUMENT);
	SPListElement element = spListElementCreate(1, 0.0);
	ASSERT_TRUE(
			spListElementSetIndex(element, -1) == SP_ELEMENT_INVALID_ARGUMENT);
	ASSERT_TRUE(spListElementGetIndex(element) == 1);
	ASSERT_TRUE(spListElementSetIndex(element, 2) == SP_ELEMENT_SUCCESS);
	ASSERT_TRUE(spListElementGetIndex(element) == 2);
	spListElementDestroy(element);
	return true;
}
static bool testIsElementGetValue() {
	SPListElement element1 = spListElementCreate(1, 0.0);
	SPListElement element2 = spListElementCreate(2, 1.0);
	SPListElement element3 = spListElementCreate(3, 1.0);
	SPListElement element4 = spListElementCreate(4, 2.0);
	ASSERT_TRUE(spListElementGetValue(element1) == 0.0);
	ASSERT_TRUE(spListElementGetValue(element2) == 1.0);
	ASSERT_TRUE(spListElementGetValue(element3) == 1.0);
	ASSERT_TRUE(spListElementGetValue(element4) == 2.0);
	spListElementDestroy(element1);
	spListElementDestroy(element2);
	spListElementDestroy(element3);
	spListElementDestroy(element4);
	return true;
}

static bool testElementSetValue() {
	ASSERT_TRUE(spListElementGetValue(NULL) == -1.0);
	ASSERT_TRUE(spListElementSetValue(NULL,-1.0) ==SP_ELEMENT_INVALID_ARGUMENT);
	ASSERT_TRUE(spListElementSetValue(NULL,1.0) ==SP_ELEMENT_INVALID_ARGUMENT);
	ASSERT_TRUE(spListElementSetValue(NULL,-1.0) ==SP_ELEMENT_INVALID_ARGUMENT);
	SPListElement element = spListElementCreate(1, 0.0);
	ASSERT_TRUE(
			spListElementSetValue(element, -1.0)
					== SP_ELEMENT_INVALID_ARGUMENT);
	ASSERT_TRUE(spListElementGetValue(element) == 0.0);
	ASSERT_TRUE(spListElementSetValue(element, 1.0) == SP_ELEMENT_SUCCESS);
	ASSERT_TRUE(spListElementGetValue(element) == 1.0);
	spListElementDestroy(element);
	return true;
}
static bool testElementCopy() {
	ASSERT_TRUE(spListElementCopy(NULL) == NULL);
	SPListElement element = spListElementCreate(1, 0.0);
	SPListElement elementCopy = spListElementCopy(element);
	ASSERT_TRUE(elementCopy!=NULL);
	ASSERT_TRUE(spListElementCompare(element, elementCopy) == 0);
	ASSERT_TRUE(spListElementSetIndex(elementCopy, 2) == SP_ELEMENT_SUCCESS);
	ASSERT_TRUE(spListElementCompare(element, elementCopy) < 0);
	ASSERT_TRUE(spListElementGetIndex(elementCopy) == 2);
	ASSERT_TRUE(spListElementGetIndex(element) == 1);
	ASSERT_TRUE(spListElementSetValue(element, 1.0) == SP_ELEMENT_SUCCESS);
	ASSERT_TRUE(spListElementCompare(element, elementCopy) > 0);
	spListElementDestroy(element);
	spListElementDestroy(elementCopy);
	return true;
}

static bool testListCreate() {
	SPList list = spListCreate();
	ASSERT_TRUE(list != NULL);
	spListDestroy(list);
	return true;
}

static bool testListCopy() {
	ASSERT_TRUE(spListCopy(NULL) == NULL);
	SPList list = spListCreate();
	SPList copy = spListCopy(list);
	ASSERT_TRUE(copy != NULL);
	ASSERT_TRUE(0 == spListGetSize(copy));
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	spListInsertFirst(list, e1);
	ASSERT_TRUE(0 == spListGetSize(copy));
	SPList list2 = quickList(4, e1, e2, e3, e4);
	SPList copy2 = spListCopy(list2);
	ASSERT_TRUE(4 == spListGetSize(copy2));
	ASSERT_TRUE(spListElementCompare(e1, spListGetFirst(copy2)) == 0);
	ASSERT_TRUE(spListElementCompare(e2, spListGetNext(copy2)) == 0);
	ASSERT_TRUE(spListElementCompare(e3, spListGetNext(copy2)) == 0);
	ASSERT_TRUE(spListElementCompare(e4, spListGetNext(copy2)) == 0);
	ASSERT_TRUE(spListGetNext(copy2) == NULL);
	spListDestroy(list);
	spListDestroy(list2);
	spListDestroy(copy);
	spListDestroy(copy2);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	return true;
}

static bool testListGetSize() {
	SPList list = quickList(0);
	ASSERT_TRUE(0 == spListGetSize(list));
	SPListElement e1 = spListElementCreate(1, 1.0);
	spListInsertFirst(list, e1);
	ASSERT_TRUE(1 == spListGetSize(list));
	spListElementSetIndex(e1, 2);
	spListElementSetValue(e1, 2.0);
	spListInsertFirst(list, e1);
	ASSERT_TRUE(2 == spListGetSize(list));
	spListGetFirst(list);
	spListRemoveCurrent(list);
	ASSERT_TRUE(1 == spListGetSize(list));
	spListDestroy(list);
	spListElementDestroy(e1);
	return true;
}

static bool testListGetFirst() {
	SPList list = spListCreate();
	ASSERT_TRUE(spListGetFirst(list) == NULL);
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	SPList list2 = quickList(4, e1, e2, e3, e4);
	SPListElement first = spListGetFirst(list2);
	ASSERT_TRUE(spListElementCompare(e1, first) == 0);
	ASSERT_TRUE(
			spListElementCompare(first, spListGetFirst(list2)) == 0
					&& spListGetFirst(list2) == first);
	spListDestroy(list);
	spListDestroy(list2);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	return true;
}

static bool testListGetLast() {
	SPList list = spListCreate();
	ASSERT_TRUE(spListGetLast(list) == NULL);
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	SPList list2 = quickList(4, e1, e2, e3, e4);
	SPListElement last = spListGetLast(list2);
	ASSERT_TRUE(spListElementCompare(e4, last) == 0);
	ASSERT_TRUE(
			spListElementCompare(last, spListGetLast(list2)) == 0
					&& spListGetLast(list2) == last);
	spListDestroy(list);
	spListDestroy(list2);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	return true;
}

static bool testListGetNext() {
	ASSERT_TRUE(spListGetNext(NULL) == NULL);
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	SPList list2 = quickList(4, e1, e2, e3, e4);
	ASSERT_TRUE(spListGetNext(list2) == NULL);
	spListDestroy(list2);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	return true;
}

static bool testListGetPrevious() {
	ASSERT_TRUE(spListGetNext(NULL) == NULL);
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	SPList list2 = quickList(4, e1, e2, e3, e4);
	ASSERT_TRUE(spListGetPrevious(list2) == NULL);
	spListDestroy(list2);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	return true;
}

static bool testListInsertFirst() {
	ASSERT_TRUE(SP_LIST_NULL_ARGUMENT == spListInsertFirst(NULL, NULL));
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	SPList list2 = quickList(3, e1, e2, e3);
	ASSERT_TRUE(SP_LIST_SUCCESS == spListInsertFirst(list2, e4));
	ASSERT_TRUE(4 == spListGetSize(list2));
	ASSERT_TRUE(spListElementCompare(e4, spListGetFirst(list2))==0);
	ASSERT_TRUE(spListElementCompare(e1, spListGetNext(list2))==0);
	ASSERT_TRUE(spListElementCompare(e2, spListGetNext(list2))==0);
	ASSERT_TRUE(spListElementCompare(e3, spListGetNext(list2))==0);
	ASSERT_TRUE(spListGetNext(list2) == NULL);
	ASSERT_TRUE(spListGetNext(list2) == NULL);
	spListDestroy(list2);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	return true;
}

static bool testListInsertLast() {
	ASSERT_TRUE(SP_LIST_NULL_ARGUMENT == spListInsertLast(NULL, NULL));
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	SPList list2 = quickList(3, e2, e3, e4);
	ASSERT_TRUE(SP_LIST_SUCCESS == spListInsertLast(list2, e1));
	ASSERT_TRUE(4 == spListGetSize(list2));
	ASSERT_TRUE(spListElementCompare(e2, spListGetFirst(list2))==0);
	ASSERT_TRUE(spListElementCompare(e3, spListGetNext(list2))==0);
	ASSERT_TRUE(spListElementCompare(e4, spListGetNext(list2))==0);
	ASSERT_TRUE(spListElementCompare(e1, spListGetNext(list2))==0);
	ASSERT_TRUE(spListGetNext(list2) == NULL);
	spListDestroy(list2);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	return true;
}

static bool testListInsertBeforeCurrent() {
	ASSERT_TRUE(SP_LIST_NULL_ARGUMENT == spListInsertBeforeCurrent(NULL, NULL));
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	SPList list2 = quickList(3, e2, e3, e4);
	ASSERT_TRUE(SP_LIST_NULL_ARGUMENT == spListInsertBeforeCurrent(list2,NULL));
	ASSERT_TRUE(
			SP_LIST_INVALID_CURRENT == spListInsertBeforeCurrent(list2, e1));
	ASSERT_TRUE(3 == spListGetSize(list2));
	spListGetFirst(list2);
	spListGetNext(list2);
	ASSERT_TRUE(SP_LIST_SUCCESS == spListInsertBeforeCurrent(list2, e1));
	ASSERT_TRUE(4 == spListGetSize(list2));
	ASSERT_TRUE(spListElementCompare(e2, spListGetFirst(list2))==0);
	ASSERT_TRUE(spListElementCompare(e1, spListGetNext(list2))==0);
	ASSERT_TRUE(spListElementCompare(e3, spListGetNext(list2))==0);
	ASSERT_TRUE(spListElementCompare(e4, spListGetNext(list2))==0);
	ASSERT_TRUE(spListGetNext(list2) == NULL);
	spListDestroy(list2);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	return true;
}

static bool testListInsertAfterCurrent() {
	ASSERT_TRUE(SP_LIST_NULL_ARGUMENT == spListInsertAfterCurrent(NULL, NULL));

	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	SPList list2 = quickList(3, e2, e3, e4);
	ASSERT_TRUE(SP_LIST_NULL_ARGUMENT == spListInsertAfterCurrent(list2,NULL));
	ASSERT_TRUE(SP_LIST_INVALID_CURRENT == spListInsertAfterCurrent(list2, e1));
	spListGetFirst(list2);
	spListGetNext(list2);
	ASSERT_TRUE(SP_LIST_SUCCESS == spListInsertAfterCurrent(list2, e1));
	ASSERT_TRUE(4 == spListGetSize(list2));
	ASSERT_TRUE(spListElementCompare(e2, spListGetFirst(list2))==0);
	ASSERT_TRUE(spListElementCompare(e3, spListGetNext(list2))==0);
	ASSERT_TRUE(spListElementCompare(e1, spListGetNext(list2))==0);
	ASSERT_TRUE(spListElementCompare(e4, spListGetNext(list2))==0);
	ASSERT_TRUE(spListGetNext(list2) == NULL);
	ASSERT_TRUE(spListGetNext(list2) == NULL);
	spListDestroy(list2);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	return true;
}

static bool testListClear() {
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	SPList list2 = quickList(4, e1, e2, e3, e4);
	ASSERT_TRUE(spListClear(list2) == SP_LIST_SUCCESS);
	ASSERT_TRUE(0 == spListGetSize(list2));
	SPList list = spListCreate();
	spListClear(list);
	ASSERT_TRUE(0 == spListGetSize(list));
	spListDestroy(list);
	spListDestroy(list2);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	return true;
}
static bool testListDestroy() {
	spListDestroy(NULL);
	return true;
}
static bool testListForEach() {
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
    SPListElement e5 = spListElementCreate(5, 5.0);
	SPListElement arr[5] = {e1,e2,e3,e4,e5};
	int i = 1;
	SPList list = quickList(3, e2, e3, e4);
	SP_LIST_FOREACH(SPListElement,e,list){
		ASSERT_TRUE(spListElementCompare(e,arr[i]) == 0);
		i++;
	}
	ASSERT_TRUE(spListInsertFirst(list,e1) == SP_LIST_SUCCESS);
	i=0;
	SP_LIST_FOREACH(SPListElement,e,list){
		ASSERT_TRUE(spListElementCompare(e,arr[i]) == 0);
		i++;
	}
	ASSERT_TRUE(i==4);//number of elements = 4
	ASSERT_TRUE(spListInsertLast(list,e5) == SP_LIST_SUCCESS);
	i=0;
	SP_LIST_FOREACH(SPListElement,e,list){
		ASSERT_TRUE(spListElementCompare(e,arr[i]) ==  0);
		i++;
	}
	ASSERT_TRUE(i==5);//number of elements should be 5
	ASSERT_TRUE(spListClear(list) == SP_LIST_SUCCESS);
	SP_LIST_FOREACH(SPListElement,e,list){
		ASSERT_TRUE(false);//Should not enter this loop because list is empty
	}
	spListDestroy(list);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	spListElementDestroy(e5);
	return true;
}
int main() {
	RUN_TEST(testElementCreate);
	RUN_TEST(testElementCopy);
	RUN_TEST(testElementCompare);
	RUN_TEST(testElementGetIndex);
	RUN_TEST(testIsElementGetValue);
	RUN_TEST(testElementSetIndex);
	RUN_TEST(testElementSetValue);
	RUN_TEST(testListCreate);
	RUN_TEST(testListCopy);
	RUN_TEST(testListGetSize);
	RUN_TEST(testListGetFirst);
	RUN_TEST(testListGetLast);
	RUN_TEST(testListGetNext);
	RUN_TEST(testListGetPrevious);
	RUN_TEST(testListInsertFirst);
	RUN_TEST(testListInsertLast);
	RUN_TEST(testListInsertBeforeCurrent);
	RUN_TEST(testListInsertAfterCurrent);
	RUN_TEST(testListClear);
	RUN_TEST(testListDestroy);
	RUN_TEST(testListForEach);
	return 0;
}
