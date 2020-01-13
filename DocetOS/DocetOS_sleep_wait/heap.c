#include "heap.h"
#include "os.h"
#include <stdio.h>

void heap_up(heap_t * heap) {
	/* Start with last element in heap */
	int child_index = heap->length;
	int parent_index;
	OS_TCB_t * parent_tcb;
	OS_TCB_t * child_tcb;
	while(1) {
		/* If it's the root element, stop */
		if(child_index == 1) {
			break;
		}
		parent_index = child_index/2;
		parent_tcb = heap->TCB[parent_index - 1];
		child_tcb = heap->TCB[child_index - 1];
		/* Compare with it's parent */
		if(parent_tcb->priority_affected < child_tcb->priority_affected) {
			/* If the parent is smaller, stop */
			break;
		} else {
			/* Child is smaller - swap */
			OS_TCB_t * child_value = heap->TCB[child_index - 1];
			heap->TCB[child_index - 1] = heap->TCB[parent_index - 1];
			heap->TCB[parent_index - 1] = child_value;
			
			/* Change to new index */
			child_index = parent_index;
		}
	}
}

void heap_down(heap_t * heap) {
	/* Start with the root element */
	int parent_index = 1;
	while(1) {
		/* If it has no children, stop */
		if(heap->length == 1) {
			break;
		}
		
		int child1_index, child2_index;
		child1_index = (parent_index * 2);
		child2_index = (parent_index * 2) + 1;
		
		int smallest_child_index; 
		OS_TCB_t * smallest_child_TCB;
		OS_TCB_t * child1_tcb;
		OS_TCB_t * child2_tcb;
		OS_TCB_t * parent_tcb;
		
		/* Check for child 1 */
		if(heap->length >= (parent_index * 2)) {
			smallest_child_TCB = heap->TCB[child1_index - 1];
			smallest_child_index = child1_index;
			
			/* Check for child 2 */
			if(heap->length >= ((parent_index * 2) + 1)) {
				child1_tcb = heap->TCB[child1_index - 1];
				child2_tcb = heap->TCB[child2_index - 1];
				if(child1_tcb->priority_affected > child2_tcb->priority_affected) {
					/* child1 > child2 */
					smallest_child_TCB = heap->TCB[child2_index - 1];
					smallest_child_index = child2_index;
				}
			}
		}
		/* Compare with the smallest child */
		parent_tcb = heap->TCB[parent_index - 1];
		if(parent_tcb->priority_affected > smallest_child_TCB->priority_affected) {
			/* Parent is smaller - swap */
			heap->TCB[smallest_child_index - 1] = heap->TCB[parent_index-1];
			heap->TCB[parent_index - 1] = smallest_child_TCB;
			parent_index = smallest_child_index;
		} else {
			break;
		}
	}
}

void heap_init(heap_t *heap, OS_TCB_t ** tcb) {
	heap->TCB = tcb;
	heap->length = 0;
}

void heap_insert(heap_t *heap, OS_TCB_t * tcb) {
	// The new element is always added to the end of a heap
	//printf("adding %d\r\n", value);
	heap->TCB[(heap->length)++] = tcb;
	heap_up(heap);
}

OS_TCB_t * heap_extract(heap_t *heap) {
	// The root value is extracted, and the space filled by the value from the end
	// If the heap is empty, this will fail horribly...
	OS_TCB_t * value = heap->TCB[0];
	heap->TCB[0] = heap->TCB[--(heap->length)];
	//heap->store[(heap->length)] = 0;
	heap_down(heap);
	return value;
}

int heap_isEmpty(heap_t *heap) {
	return !(heap->length);
}

OS_TCB_t * top_element(heap_t *heap) {
	return heap->TCB[0];
}
