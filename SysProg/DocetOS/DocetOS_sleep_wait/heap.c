#include "heap.h"
#include "os.h"
#include <stdio.h>

/* heap_up, takes a pointer to a heap_t structure and returns nothing.
This starts with the top element in the heap, if it is the root element
then it stops. If not then compare its pirority value it's parent, if the 
parent is smaller ten stop. else swap the two elements and do the same checks.
*/
static void heap_up(heap_t *heap) {
	/* check heap isn't empty */
	if(heap->length -1 != 0) {
		int possition = heap->length;
		while(1) {
			if(possition != 1) {
				/* Compare with parent */
				if(heap->TCB[(possition/2)-1]->priority > heap->TCB[possition-1]->priority ) {
					/* Parent is bigger, swap */
					OS_TCB_t *  value = heap->TCB[possition-1];
					heap->TCB[possition-1] = heap->TCB[(possition/2)-1];
					heap->TCB[(possition/2)-1] = value;
					/* Set the new possition */
					possition = possition/2;
				} else {
					/* Parent is smaller */
					break;
				}
			} else {
				/* Root element */ 
				break;
			}
		}
	} 
}

/* heap_down, takes a pointer to a heap_t strcutre and returns nothing.
Starting with the top element, if it has childern then compare their 
priority value. If the parent is smaller stop, else swap the parent with 
the smaller child. Repeat untill reach a stop.
*/
static void heap_down(heap_t *heap) {
	int parent_index = 1;
	while(1) {
		/* Check for children */
		if(parent_index*2-1 < heap->length) {
			int child_1_index = parent_index*2;
			int child_2_index = (parent_index*2)+1;
			int smallest_child_index;
			OS_TCB_t *  smallest_child_value;
			/* check for child 1 */
			if(heap->length >= child_1_index) {
				/* Check for child 2 */
				if(heap->length >= child_2_index){
					/* Compare children */
					if(heap->TCB[child_1_index-1]->priority > heap->TCB[child_2_index-1]->priority) {
						/* Child 2 is smaller */ 
						smallest_child_index = child_2_index;
						smallest_child_value = heap->TCB[child_2_index-1];
					} else {
						/* Child 1 is smaller */ 
						smallest_child_index = child_1_index;
						smallest_child_value = heap->TCB[child_1_index-1];
					}
				} else {
					/* Only one child */
					smallest_child_index = child_1_index;
					smallest_child_value = heap->TCB[child_1_index-1];
				}
			}
			/* Compare smallest child with parent */
			if(heap->TCB[parent_index-1]->priority > heap->TCB[smallest_child_index-1]->priority) {
				/* Parent is smaller, swap */
				heap->TCB[smallest_child_index-1] = heap->TCB[parent_index-1];
				heap->TCB[parent_index-1] = smallest_child_value;
				parent_index = smallest_child_index;
			} else {
				/* Parent is smaller then children, stop */
				break;
			}
		} else {
			/* No children */
			break;
		}
	}
}

/* heap_init, takes a pointer to heap_t structure and a double pointer to a OS_TCB_t
structure and returns nothing. The heap is initailised byt setting the length to zero
and TCB field to the passed double pointer to tcb.
*/
void heap_init(heap_t *heap, OS_TCB_t **tcb) {
	heap->TCB = tcb;
	heap->length = 0;
}

/* heap_insert, this function takes a pointer to heap_t structure and pointer to OS_TCB_t 
structure and returns nothing. The task is added to the end of the list and the length is 
field of the heap is increased, then heap up is called.
*/
void heap_insert(heap_t *heap, OS_TCB_t *tcb) {
	heap->TCB[(heap->length)++] = tcb;
	heap_up(heap);
}

/* heap_extract, takes a pointer to a heap_t structure and retursn a pointer to OS_TCB_t
First it stores a local value of the top element, then replaces the the top element with 
the last one, deacreas the length filed of the heap and calles heap up. the local value 
is returned.
*/
OS_TCB_t * heap_extract(heap_t *heap) {
	OS_TCB_t * topElement = heap->TCB[0];
	heap->TCB[0] = heap->TCB[--(heap->length)];
	heap_down(heap);
	return topElement;
}
