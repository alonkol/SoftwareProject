#include "SPList.h"
#include <stdlib.h>

typedef struct node_t {
	SPListElement data;
	struct node_t* next;
	struct node_t* previous;
}*Node;
Node createNode(Node previous, Node next, SPListElement element);
void destroyNode(Node node);

struct sp_list_t {
	Node head;
	Node tail;
	Node current;
	int size;
};

Node createNode(Node previous, Node next, SPListElement element) {
	SPListElement newElement = spListElementCopy(element);
	if (newElement == NULL) {
		return NULL;
	}
	Node newNode = (Node) malloc(sizeof(*newNode));
	if (newNode == NULL) {
		spListElementDestroy(newElement);
		return NULL;
	}
	newNode->data = newElement;
	newNode->previous = previous;
	newNode->next = next;
	return newNode;
}

void destroyNode(Node node) {
	if (node == NULL) {
		return;
	}
	if (node->data != NULL) {
		spListElementDestroy(node->data);
	}
	free(node);
}

SPList spListCreate() {
	SPList list = (SPList) malloc(sizeof(*list));
	if (list == NULL) {
		return NULL;
	} else {
		list->head = (Node) malloc(sizeof(*list->head));
		if (list->head == NULL) {
			free(list);
			return NULL;
		}
		list->tail = (Node) malloc(sizeof(*list->tail));
		if (list->tail == NULL) {
			free(list->head);
			free(list);
			return NULL;
		}
		list->head->data = NULL;
		list->head->next = list->tail;
		list->head->previous = NULL;
		list->tail->data = NULL;
		list->tail->next = NULL;
		list->tail->previous = list->head;
		list->current = NULL;
		list->size = 0;
		return list;

	}
}

SPList spListCopy(SPList list) {
	if (list == NULL) {
		return NULL;
	}
	SPList copyList = spListCreate();
	if (copyList == NULL) {
		return NULL;
	}
	SPListElement currentElement = spListGetFirst(list);
	while (currentElement) {
		if (spListInsertLast(copyList, currentElement) != SP_LIST_SUCCESS) {
			spListDestroy(copyList);
			list->current = NULL;
			return NULL;
		}
		currentElement = spListGetNext(list);
	}
	list->current = NULL;
	copyList->current = NULL;
	return copyList;
}

int spListGetSize(SPList list) {
	return list == NULL ? -1 : list->size;
}

SPListElement spListGetFirst(SPList list) {
	if (list == NULL || spListGetSize(list) == 0) {
		return NULL;
	} else {
		list->current = list->head->next;
		return list->current->data;
	}
}

SPListElement spListGetLast(SPList list){
	if (list == NULL || spListGetSize(list) == 0) {
		return NULL;
	} else {
		list->current = list->tail->previous;
		return list->current->data;
	}
}

SPListElement spListGetNext(SPList list) {
	if (list == NULL || spListGetSize(list) == 0 || list->current == NULL) {
		return NULL;
	} else {
		if (list->current->next == list->tail) {
			list->current = NULL;
			return NULL;
		} else {
			list->current = list->current->next;
			return list->current->data;
		}
	}
}

SPListElement spListGetPrevious(SPList list) {
	if (list == NULL || spListGetSize(list) == 0 || list->current == NULL) {
		return NULL;
	} else {
		if (list->current->previous == list->head) {
			list->current = NULL;
			return NULL;
		} else {
			list->current = list->current->previous;
			return list->current->data;
		}
	}
}

SPListElement spListGetCurrent(SPList list) {
	if (list == NULL || spListGetSize(list) == 0 || list->current == NULL) {
		return NULL;
	} else {
		return list->current->data;
	}
}

SP_LIST_MSG spListInsertFirst(SPList list, SPListElement element) {
	if (list == NULL || element == NULL) {
		return SP_LIST_NULL_ARGUMENT;
	}
	Node newNode = createNode(list->head, list->head->next, element);
	if (newNode == NULL) {
		return SP_LIST_OUT_OF_MEMORY;
	}
	list->head->next->previous = newNode;
	list->head->next = newNode;
	list->size++;
	return SP_LIST_SUCCESS;
}

SP_LIST_MSG spListInsertLast(SPList list, SPListElement element) {
	if (list == NULL || element == NULL) {
		return SP_LIST_NULL_ARGUMENT;
	}
	Node newNode = createNode(list->tail->previous, list->tail, element);
	if (newNode == NULL) {
		return SP_LIST_OUT_OF_MEMORY;
	}
	list->tail->previous->next = newNode;
	list->tail->previous = newNode;
	list->size++;
	return SP_LIST_SUCCESS;
}

SP_LIST_MSG spListInsertBeforeCurrent(SPList list, SPListElement element) {
	if (list == NULL || element == NULL) {
		return SP_LIST_NULL_ARGUMENT;
	}
	if (list->current == NULL) {
		return SP_LIST_INVALID_CURRENT;
	}
	Node newNode = createNode(list->current->previous, list->current, element);
	if (newNode == NULL) {
		return SP_LIST_OUT_OF_MEMORY;
	}
	list->current->previous->next = newNode;
	list->current->previous = newNode;
	list->size++;
	return SP_LIST_SUCCESS;
}

SP_LIST_MSG spListInsertAfterCurrent(SPList list, SPListElement element) {
	if (list == NULL || element == NULL) {
		return SP_LIST_NULL_ARGUMENT;
	}
	if (list->current == NULL) {
		return SP_LIST_INVALID_CURRENT;
	} else if (list->current == list->tail->previous) {
		return spListInsertLast(list, element);
	} else {
		Node tempCurrent = list->current;
		list->current = list->current->next;
		SP_LIST_MSG res = spListInsertBeforeCurrent(list, element);
		list->current = tempCurrent;
		return res;
	}
}

SP_LIST_MSG spListRemoveCurrent(SPList list) {
	if (list == NULL) {
		return SP_LIST_NULL_ARGUMENT;
	}
	if (list->current == NULL) {
		return SP_LIST_INVALID_CURRENT;
	}
	list->current->previous->next = list->current->next;
	list->current->next->previous = list->current->previous;
	destroyNode(list->current);
	list->current = NULL;
	list->size--;
	return SP_LIST_SUCCESS;
}

SP_LIST_MSG spListClear(SPList list) {
	if (list == NULL) {
		return SP_LIST_NULL_ARGUMENT;
	}
	while (spListGetFirst(list)) {
		spListRemoveCurrent(list);
	}
	return SP_LIST_SUCCESS;
}

void spListDestroy(SPList list) {
	if (list == NULL) {
		return;
	}
	spListClear(list);
	destroyNode(list->head);
	destroyNode(list->tail);
	free(list);
}
