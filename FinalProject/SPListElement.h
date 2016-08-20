#ifndef LISTELEMENT_H_
#define LISTELEMENT_H_

/**
 * List Element Summary
 *
 * Implements a list element which contains an index and a double value.
 * An element contains:
 * 		int int
 * 		double value
 * Two elements e1 and e2 are said to be equal iff:
 * 		(e1.index == e2.index) AND (e1.value == e2.value)
 * Element e1 is less than element e2 iff:
 * 		(e1.value < e2.value)   OR (e1.value == e2.value AND e1.index < e2.index)
 * Element e1 is greater than element e2 iff:
 * 		(e2 is less than e1)
 *
 * The following functions are available
 *	spListElementCreate    - Creates a new element the corresponding int and double value
 *	spListElementCopy 	   - Creates a new copy of the target element
 *	spListElementDestroy   - Free all memory allocations associated with an element
 *	spListElementcompare   - Compares two elements
 *	spListElementSetIndex  - Sets a new index to the target element
 *  spListElementGetIndex  - Gets a the index of the target  element
 *  spListElementSetValue  - Sets a new value to the target element.
 *  spLostElementGetValue  - Gets a the value of the target element
 */

/** Type used for error reporting in SPListElement*/
typedef enum SP_LIST_ELEMENT_RESULT_t {
	SP_ELEMENT_SUCCESS,
	SP_ELEMENT_INVALID_ARGUMENT,
	SP_ELEMENT_OUT_OF_MEMORY
} SP_ELEMENT_MSG;

/** Type used represent an element in the list **/
typedef struct sp_list_element_t * SPListElement;

/**
 * Creates a new element with the specific index and value.
 *
 * @param index  The index value of the element (index >= 0)
 * @param value  The value of the element (value >= 0.0)
 * @return
 * NULL in case of memory allocation fails.
 * A new element with the corresponding index and value .
 */
SPListElement spListElementCreate(int index, double value);

/**
 * Creates a copy of target element.
 *
 * The new copy will contain the same index and value
 * as the target element.
 *
 * The comparison is made based on the following relation:
 *
 * Two elements e1 and e2 are said to be equal iff:
 * 		(e1.index == e2.index) AND (e1.value == e2.value)
 * Element e1 is less than element e2 iff:
 * 		(e1.value < e2.value)   OR (e1.value == e2.value AND e1.index < e2.index)
 * Element e1 is greater than element e2 iff:
 * 		(e2 is less than e1)
 *
 * @param data - The target element which will be copied
 * @return
 * NULL if a NULL was sent or memory allocation failed.
 * Otherwise the address of the new copy
 **/
SPListElement spListElementCopy(SPListElement data);

/**
 * Destroys an element.
 * All memory allocation associated with the element will be freed
 *
 * @param data the target element which will be freed.
 * 			   if data is NULL, then nothing is done
 */
void spListElementDestroy(SPListElement data);

/**
 * Compares two elements e1 and e2. The function asserts that
 * both e1 and e2 are not NULL pointers.
 *
 * @param e1 The first element to be compared
 * @param e2 The second element to be compared with
 * @assert e1!=NULL AND e2!=NULL
 * @return
 * -1 if e1 is less than e2
 * 0  if e1 and e2 are equal
 * 1  if e1 is bigger than e2
 */
int spListElementCompare(SPListElement e1, SPListElement e2);

/**
 * 	A setter for the index of the target element.
 *  The new index must be greater or equal to 0
 *
 *  @param data   The target element
 *  @param index  The new index to be set
 *  @return
 *  SP_ELEMENT_INVALID_ARGUMENT in case data==NULL || index<0
 *  SP_ELEMENT_OUT_OF_MEMORY in case memory allocation fails
 *  SP_ELEMENT_SUCCESS otherwise
 */
SP_ELEMENT_MSG spListElementSetIndex(SPListElement data, int index);

/**
 * A getter for the index of the target element.
 *
 * @param data The target element
 * @return
 * -1 in case data==NULL
 * otherwise, the index of the target element
 */
int spListElementGetIndex(SPListElement data);

/**
 * 	A setter for the value of the target element.
 *  The new index must be greater or equal to 0
 *
 *  @param data   The target element
 *  @param newValue  The new index
 *  @return
 *  SP_ELEMENT_INVALID_ARGUMENT in case data==NULL || newValue<0
 *  SP_ELEMENT_OUT_OF_MEMORY in case memory allocation fails
 *  SP_ELEMENT_SUCCESS otherwise
 */
SP_ELEMENT_MSG spListElementSetValue(SPListElement data, double newValue);

/**
 * A getter for the value of the target element.
 *
 * @param data The target element
 * @return
 * -1.0 in case data==NULL
 * otherwise, the index of the target element
 */
double spListElementGetValue(SPListElement data);

#endif /* LISTELEMENT_H_ */
