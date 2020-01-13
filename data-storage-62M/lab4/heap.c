#include "heap.h"
#include <stdio.h>

/* Macro to convert heap 1-index array to C 0-index array */
#define ZERO_INDEX(x) x-1

static void heap_up(heap_t * heap) {
	/* Start with last element in heap */
	int child_index = heap->length;
	int parent_index;
	while(1) {
		/* If it's the root element, stop */
		if(child_index == 1) {
			break;
		}
		
		parent_index = child_index/2;
		/* Compare with it's parent */
		if(heap->store[ZERO_INDEX(parent_index)] < heap->store[ZERO_INDEX(child_index)]) {
			/* If the parent is smaller, stop */
			break;
		} else {
			/* Child is smaller - swap */
			int child_value = heap->store[ZERO_INDEX(child_index)];
			heap->store[ZERO_INDEX(child_index)] = heap->store[ZERO_INDEX(parent_index)];
			heap->store[ZERO_INDEX(parent_index)] = child_value;
			
			/* Change to new index */
			child_index = parent_index;
		}
	}
	
}

static void heap_down(heap_t * heap) {
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
		
		int smallest_child_index, smallest_child_value;
		
		/* Check for child 1 */
		if(heap->length >= (parent_index * 2)) {
			smallest_child_value = heap->store[ZERO_INDEX(child1_index)];
			smallest_child_index = child1_index;
			
			/* Check for child 2 */
			if(heap->length >= ((parent_index * 2) + 1)) {
				if(heap->store[ZERO_INDEX(child1_index)] > heap->store[ZERO_INDEX(child2_index)]) {
					/* child1 > child2 */
					smallest_child_value = heap->store[ZERO_INDEX(child2_index)];
					smallest_child_index = child2_index;
				}
			}
		}
		
		/* Compare with the smallest child */
		if(heap->store[ZERO_INDEX(parent_index)] > heap->store[ZERO_INDEX(smallest_child_index)]) {
			/* Parent is smaller - swap */
			heap->store[ZERO_INDEX(smallest_child_index)] = heap->store[ZERO_INDEX(parent_index)];
			heap->store[ZERO_INDEX(parent_index)] = smallest_child_value;
			
			parent_index = smallest_child_index;
		} else {
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
	printf("adding %d\r\n", value);
	heap->store[(heap->length)++] = value;
	heap_up(heap);
}

int heap_extract(heap_t *heap) {
	// The root value is extracted, and the space filled by the value from the end
	// If the heap is empty, this will fail horribly...
	int value = heap->store[0];
	heap->store[0] = heap->store[--(heap->length)];
	//heap->store[(heap->length)] = 0;
	heap_down(heap);
	return value;
}

int heap_isEmpty(heap_t *heap) {
	return !(heap->length);
}
