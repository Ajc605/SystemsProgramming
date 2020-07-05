#include "heap.h"
#include <stdio.h>
#include "serial.h"

static void heap_up(heap_t *heap) {
	/* check heap isn't empty */
	if(heap->length -1 != 0) {
		
		int possition = heap->length;
		
		while(1) {
			if(possition != 1) {
				/* Compare with parent */
				if(heap->store[(possition/2)-1] > heap->store[possition-1] ) {
					/* Parent is bigger, swap */
					//printf("Parent: %d, is bigger vlaue: %d\r\n", heap->store[(possition/2)-1], heap->store[(possition)-1]);
					int value = heap->store[possition-1];
					heap->store[possition-1] = heap->store[(possition/2)-1];
					heap->store[(possition/2)-1] = value;
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
	// Write me!
	int parent_index = 1;
	while(1) {
		//Check for children
		if(parent_index*2-1 < heap->length) {
			//printf("\r\n");
			
			int child_1_index = parent_index*2;
			int child_2_index = (parent_index*2)+1;
			int smallest_child_index;
			int smallest_child_value;
			
			/* check for child 1*/
			if(heap->length >= child_1_index) {
				/* Check for child 2 */
				if(heap->length >= child_2_index){
					/* Compare children*/
					if(heap->store[child_1_index-1] > heap->store[child_2_index-1]) {
						/* Child 2 is smaller */ 
						//printf("child 2: %d is smaller child 1: %d\r\n", heap->store[child_2_index-1],heap->store[child_1_index-1]);
						smallest_child_index = child_2_index;
						smallest_child_value = heap->store[child_2_index-1];
					} else {
						/* Child 1 is smaller */ 
						//printf("child 1: %d is smaller child 2: %d\r\n", heap->store[child_1_index-1],heap->store[child_2_index-1]);
						smallest_child_index = child_1_index;
						smallest_child_value = heap->store[child_1_index-1];
					}
				} else {
					/* Only one child*/
					//printf("One child\r\n");
					smallest_child_index = child_1_index;
					smallest_child_value = heap->store[child_1_index-1];
				}
			}
			/* Compare smallest child with parent */
			//printf("compare smallest child: %d and parent: %d\r\n", smallest_child_value, heap->store[parent_index-1]);
			if(heap->store[parent_index-1] > smallest_child_value) {
				/* Parent is smaller, swap */
				//printf("Parent: %d is smaller value: %d, swap\r\n",heap->store[parent_index-1], smallest_child_value);
				heap->store[smallest_child_index-1] = heap->store[parent_index-1];
				heap->store[parent_index-1] = smallest_child_value;
				parent_index = smallest_child_index;
			} else {
				/* Parent is smaller then children, stop */
				//printf("Parent: %d is smaller value: %d, stop\r\n",heap->store[parent_index-1], smallest_child_value);
				break;
			}
		} else {
			/* No children */
			//printf("No children\r\n");
			break;
		}
	}
	
}

void heap_init(heap_t *heap, int *store) {
	heap->store = store;
	heap->length = 0;
}

void heap_insert(heap_t *heap, int value) {
	// The new element is always added to the end of a heap
	heap->store[(heap->length)++] = value;
	heap_up(heap);
}

int heap_extract(heap_t *heap) {
	// The root value is extracted, and the space filled by the value from the end
	// If the heap is empty, this will fail horribly...
	int value = heap->store[0];
	heap->store[0] = heap->store[--(heap->length)];
	heap_down(heap);
	return value;
}

int heap_isEmpty(heap_t *heap) {
	return !(heap->length);
}
