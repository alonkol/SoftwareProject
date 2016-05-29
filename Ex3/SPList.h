#ifndef SPLIST_H_
#define SPLIST_H_

#include <stdbool.h>
#include "SPListElement.h"
/**
 * List Container Summary
 *
 * Implements a list container type.
 * The elements of the list are of type SPListElement, please refer
 * to SPListElement.h for usage.
 * The list has an internal iterator for external use. For all functions
 * where the state of the iterator after calling that function is not stated,
 * the state of the iterator is undefined. That is you cannot assume anything about it.
 *
 * The following functions are available:
 *
 *   spListCreate               - Creates a new empty list
 *   spListDestroy              - Deletes an existing list and frees all resources
 *   spListCopy                 - Copies an existing list
 *   spListSize                 - Returns the size of a given list
 *   spListFirst                - Sets the internal iterator to the first element
 *                                in the list, and returns it.
 *   spListNext                 - Advances the internal iterator to the next
 *                                element and returns it.
 *   spListInsertFirst          - Inserts an element in the beginning of the list
 *   spListInsertLast           - Inserts an element in the end of the list
 *   spListInsertBeforeCurrent  - Inserts an element right before the place of
 *                                internal iterator
 *   spListInsertAfterCurrent   - Inserts an element right after the place of the
 *                                internal iterator
 *   spListRemoveCurrent        - Removes the element pointed by the internal
 *                                iterator
 *   spListGetCurrent           - Return the current element (pointed by the
 *                                internal iterator)
 *   spListGetFirst             - Sets the internal iterator (also called current
 *                                element) to be the first element in the list and
 *                                returns it.
 *   spListGetLast				- Sets the internal iterator (also called current
 *                                element) to be the last element in the list and
 *                                returns it.
 *   spListGetNext              - Advances the list's iterator to the next element
 *                                and returns it
 *   spListGetPrevious		    - Moves the list's iterator to the previous element
 *                                and return it
 *   spListClear		      	- Clears all the data from the list
 */

/** Type for defining the list */
typedef struct sp_list_t *SPList;

/** Type used for returning error codes from list functions */
typedef enum sp_list_msg_t {
	SP_LIST_SUCCESS,
	SP_LIST_NULL_ARGUMENT,
	SP_LIST_OUT_OF_MEMORY,
	SP_LIST_INVALID_CURRENT,
} SP_LIST_MSG;

/**
 * Allocates a new List.
 *
 * This function creates a new empty list.
 * @return
 * 	NULL - If allocations failed.
 * 	A new List in case of success.
 */
SPList spListCreate();

/**
 * Creates a copy of target list.
 *
 * The new copy will contain all the elements from the source list in the same
 * order. The internal iterator for both the new copy and the target list will not be
 * defined afterwards.
 *
 * @param list The target list to copy
 * @return
 * NULL if a NULL was sent or a memory allocation failed.
 * A List containing the same elements with same order as list otherwise.
 */
SPList spListCopy(SPList list);

/**
 * Returns the number of elements in a list. the iterator state will not change.
 *
 * @param list The target list which size is requested.
 * @return
 * -1 if a NULL pointer was sent.
 * Otherwise the number of elements in the list.
 */
int spListGetSize(SPList list);

/**
 * Sets the internal iterator to the first element and retrieves it.
 *
 * The list has an internal iterator (also called current element) to allow
 * iteration over the list's elements. This function sets the iterator to point
 * to the first element in the list and return it.
 * Use this to start iterating over the list, searching from the beginning of
 * the list and/or get the first element in the list.
 * (To continue iteration use listGetNext)
 * @code
 * void f(SPList list) {
 *   ListElement first = listGetFirst(list);
 *   printf("The first element is at address %x\n", first);
 * }
 * @endcode
 *
 * @param list The list for which to set the iterator and return the first
 * element.
 * @return
 * NULL is a NULL pointer was sent or the list is empty.
 * The first element of the list otherwise
 */
SPListElement spListGetFirst(SPList list);

/**
 * Sets the internal iterator to the last element and retrieves it.
 *
 * The list has an internal iterator (also called current element) to allow
 * iteration over the list's elements. This function sets the iterator to point
 * to the last element in the list and return it.
 * Use this to start iterating over the list, searching from the end of
 * the list and/or get the last element in the list.
 * (To continue iteration use listGetPrevious)
 * @code
 * void f(SPList list) {
 *   SPListElement first = spListGetLast(list);
 *   printf("The last element is at address %x\n", first);
 * }
 * @endcode
 *
 * @param list The list for which to set the iterator and return the last
 * element.
 * @return
 * NULL is a NULL pointer was sent or the list is empty.
 * The last element of the list otherwise
 */
SPListElement spListGetLast(SPList list);

/**
 * Advances the list's iterator to the next element and return it. In case
 * the return value is NULL, the state of the iterator will not be defined, otherwise
 * it will point to the next element in the list.
 *
 * @param list The list for which to advance the iterator
 * @return
 * NULL if reached the end of the list, the iterator is at an invalid state or
 * a NULL sent as argument
 * The next element on the list in case of success
 */
SPListElement spListGetNext(SPList list);

/**
 * Moves the list's iterator to the previous element and return it. In case
 * the return value is NULL, the state of the iterator will not be defined, otherwise
 * it will point to the previous element in the list.
 *
 * @param list The list for which to advance the iterator
 * @return
 * NULL if reached the head of the tail, the iterator is at an invalid state or
 * a NULL sent as argument
 * The previous element on the list in case of success
 */
SPListElement spListGetPrevious(SPList list);

/**
 * Returns the current element (pointed by the iterator)
 *
 * @param list The list for which to get the iterator
 * @return
 * NULL if the iterator is at an invalid state or a NULL sent as argument
 * The current element on the list in case of success
 */
SPListElement spListGetCurrent(SPList list);

/**
 * Adds a new element to the list, the new element will be the first element. The state
 * of the iterator will not be changed
 *
 * @param list The list for which to add an element in its start
 * @param element The element to insert. A copy of the element will be
 * inserted
 * @return
 * SP_LIST_NULL_ARGUMENT if a NULL was sent as list
 * SP_LIST_OUT_OF_MEMORY if an allocation failed
 * SP_LIST_SUCCESS the element has been inserted successfully
 */
SP_LIST_MSG spListInsertFirst(SPList list, SPListElement element);

/**
 * Adds a new element to the list, the new element will be the last element. The state
 * of the iterator will not be changed.
 *
 * @param list The list for which to add an element in its end
 * @param element The element to insert. A copy of the element will be
 * inserted
 * @return
 * SP_LIST_NULL_ARGUMENT if a NULL was sent as list
 * SP_LIST_OUT_OF_MEMORY if an allocation failed
 * SP_LIST_SUCCESS the element has been inserted successfully
 */
SP_LIST_MSG spListInsertLast(SPList list, SPListElement element);

/**
 * Adds a new element to the list, the new element will be placed right before
 * the current element (As pointed by the inner iterator of the list). The state
 * of the iterator will not be changed
 *
 * @param list The list for which to add an element before its current element
 * @param element The element to insert. A copy of the element will be
 * inserted
 * @return
 * SP_LIST_NULL_ARGUMENT if a NULL was sent as list or element
 * SP_LIST_INVALID_CURRENT if the list's iterator is in an invalid state (Does
 * not point to a legal element in the list)
 * SP_LIST_OUT_OF_MEMORY if an allocation failed
 * SP_LIST_SUCCESS the element has been inserted successfully
 */
SP_LIST_MSG spListInsertBeforeCurrent(SPList list, SPListElement element);

/**
 * Adds a new element to the list, the new element will be place right after
 * the current element (As pointed by the inner iterator be of the list). The state
 * of the iterator will not be changed.
 *
 * @param list The list for which to add an element after its current element
 * @param element The element to insert. A copy of the element will be
 * inserted
 * @return
 * SP_LIST_NULL_ARGUMENT if a NULL was sent as list
 * SP_LIST_INVALID_CURRENT if the list's iterator is in an invalid state (Does
 * not point to a legal element in the list)
 * SP_LIST_OUT_OF_MEMORY if an allocation failed
 * SP_LIST_SUCCESS the element has been inserted successfully
 */
SP_LIST_MSG spListInsertAfterCurrent(SPList list, SPListElement element);

/**
 * Removes the currently pointed element of the list using the stored freeing
 * function. The state of the current element will not be defined afterwards.
 *
 * @param list The list for which the current element will be removed
 * @return
 * SP_LIST_NULL_ARGUMENT if list is NULL
 * SP_LIST_INVALID_CURRENT if the current pointer of the list is in invalid state
 * SP_LIST_SUCCESS the current element was removed successfully
 */
SP_LIST_MSG spListRemoveCurrent(SPList list);

/**
 * Removes all elements from target list. The state of the current element will not be defined afterwards.
 *
 * The elements are deallocated using the stored freeing function
 * @param list Target list to remove all element from
 * @return
 * SP_LIST_NULL_ARGUMENT - if a NULL pointer was sent.
 * SP_LIST_SUCCESS - Otherwise.
 */
SP_LIST_MSG spListClear(SPList list);

/**
 * listDestroy: Deallocates an existing list. Clears all elements by using the
 * stored free function.
 *
 * @param list Target list to be deallocated. If list is NULL nothing will be
 * done
 */
void spListDestroy(SPList list);

/**
 * Macro for iterating over a list.
 *
 * Declares a new variable to hold each element of the list.
 * Use this macro for iterating through the list conveniently.
 * Note that this mcaro modifies the internal iterator.
 *
 * For example, the following code will go through all elements
 * of the list and returns the minimum value
 *
 * @code
 * double minElementValue(SPList list) {
 *   double minVal = -1.0;//Elements in List has non-negative values
 *   if(list==NULL){
 *   	return -1.0;
 *   }
 *   SP_LIST_FOREACH(SPListElement, e, list) {
 *     double eVal = spListElementGetValue(e)
 *     if(minVal == -1.0 || eVal<minVal){ //Updates minimum
 *     		minVal = eVal;
 *     }
 *   }
 *   return minVal;
 * }
 *
 * @endcode
 *
 * @param type The type of the elements in the list
 * @param iterator The name of the variable to hold the next list element
 * @param list the list to iterate over
 */
#define SP_LIST_FOREACH(type,iterator,list) \
	for(type iterator = spListGetFirst(list) ; \
		iterator ;\
		iterator = spListGetNext(list))


#endif /* SPLIST_H_ */
