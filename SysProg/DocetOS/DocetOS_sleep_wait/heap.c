#include "heap.h"
#include "os.h"
#include <stdio.h>

static void heap_up(heap_t *heap) {
	/* check heap isn't empty */
	if(heap->length -1 != 0) {
		int possition = heap->length;
		while(1) {
			if(possition != 1) {
				/* Compare with parent */
				if(heap->TCB[(possition/2)-1]->priority > heap->TCB[possition-1]->priority ) {
					/* Parent is bigger, swap */
					//printf("Parent: %d, is bigger vlaue: %d\r\n", heap->store[(possition/2)-1], heap->store[(possition)-1]);
					OS_TCB_t *  value = heap->TCB[possition-1];
					heap->TCB[possition-1] = heap->TCB[(possition/2)-1];
					heap->TCB[(possition/2)-1] = value;
					/* Set the new possition */
					possition = possition/2;
				} else {
					/* Parent is smaller */
					//printf("Parent: %d, is smaller vlaue: %d\r\n", heap->store[(possition/2)-1], heap->store[(possition)-1]);
					break;
				}
			} else {
				/* Root element */ 
				break;
			}
		}
	} else {
		//printf("Empty\r\n");
	}
}

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

void heap_init(heap_t *heap, OS_TCB_t **tcb) {
	heap->TCB = tcb;
	heap->length = 0;
}

void heap_insert(heap_t *heap, OS_TCB_t *tcb) {
	heap->TCB[(heap->length)++] = tcb;
	heap_up(heap);
}

OS_TCB_t *  heap_extract(heap_t *heap) {
	OS_TCB_t *  value = heap->TCB[0];
	heap->TCB[0] = heap->TCB[--(heap->length)];
	heap_down(heap);
	return value;
}

int heap_isEmpty(heap_t *heap) {
	return !(heap->length);
}
