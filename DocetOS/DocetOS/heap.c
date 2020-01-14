#include "heap.h"
#include "os.h"
#include <stdio.h>

void heap_up(heap_t * heap, int start_element) {
	int child_index = start_element + 1;
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
		unsigned int parent_sort, child_sort;
		if(heap->sort == 0) {
			parent_sort = parent_tcb->priority_affected;
			child_sort = child_tcb->priority_affected;
		} else {
			parent_sort = parent_tcb->data;
			child_sort = child_tcb->data;
		}
		
		if(parent_sort < child_sort) {
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

void heap_down(heap_t * heap, int start_element) {
	/* Start with the root element */
	//int parent_index = 1;
	int parent_index = start_element + 1;
	while(1) {
		/* If it has no children, stop */
		if(heap->length == 1) {
			break;
		}
		
		int child1_index, child2_index;
		child1_index = (parent_index * 2);
		child2_index = (parent_index * 2) + 1;
		
		int smallest_child_index; 
		OS_TCB_t * smallest_child_tcb;
		OS_TCB_t * child1_tcb;
		OS_TCB_t * child2_tcb;
		OS_TCB_t * parent_tcb;
		unsigned int parent_sort, child1_sort, child2_sort, smallest_child_sort;
		
		/* Check for child 1 */
		if(heap->length >= (parent_index * 2)) {
			smallest_child_tcb = heap->TCB[child1_index - 1];
			smallest_child_index = child1_index;
			
			/* Check for child 2 */
			if(heap->length >= ((parent_index * 2) + 1)) {
				child1_tcb = heap->TCB[child1_index - 1];
				child2_tcb = heap->TCB[child2_index - 1];
				
				if(heap->sort == 0) {
					child1_sort = child1_tcb->priority_affected;
					child2_sort = child2_tcb->priority_affected;
				} else {
					child1_sort = child1_tcb->data;
					child2_sort = child2_tcb->data;
				}
				/* Find the smallest child */
				if(child1_sort > child2_sort) {
					/* child1 > child2 */
					smallest_child_tcb = heap->TCB[child2_index - 1];
					smallest_child_index = child2_index;
				}
			}
		}
		parent_tcb = heap->TCB[parent_index - 1];
		
		if(heap->sort == 0) {
					parent_sort = parent_tcb->priority_affected;
					smallest_child_sort = smallest_child_tcb->priority_affected;
				} else {
					parent_sort = parent_tcb->data;
					smallest_child_sort = smallest_child_tcb->data;
				}
		/* Compare the smallest child with parent */ 
		if(parent_sort > smallest_child_sort) {
			/* Parent is bigger - swap */
			OS_TCB_t * temp = heap->TCB[smallest_child_index - 1];
			heap->TCB[smallest_child_index - 1] = heap->TCB[parent_index-1];
			heap->TCB[parent_index - 1] = temp;
			
			parent_index = smallest_child_index;
		} else {
			break;
		}
	}
}

void heap_fix(heap_t * heap, int start_element) {
	int node_index = start_element + 1;
	if(node_index == 1) {
		heap_down(heap, start_element);
	} else {
		unsigned int parent_sort, child_sort;
		if(heap->sort == 0) {
			parent_sort = heap->TCB[node_index]->priority_affected;
			child_sort = heap->TCB[node_index/2]->priority_affected;
		} else {
			parent_sort = heap->TCB[node_index]->data;
			child_sort = heap->TCB[node_index/2]->data;
		}
		
		if(parent_sort < child_sort) {
			heap_up(heap, start_element);
		} else {
			heap_down(heap, start_element);
		}
	}
}

void heap_init(heap_t *heap, OS_TCB_t ** tcb, unsigned int sort) {
	heap->TCB = tcb;
	heap->length = 0;
	heap->sort = sort;
}

void heap_insert(heap_t *heap, OS_TCB_t * tcb) {
	// The new element is always added to the end of a heap
	//printf("adding %d\r\n", value);
	heap->TCB[(heap->length)++] = tcb;
	heap_up(heap, (heap->length - 1));
}

OS_TCB_t * heap_extract(heap_t *heap) {
	// The root value is extracted, and the space filled by the value from the end
	// If the heap is empty, this will fail horribly...
	OS_TCB_t * value = heap->TCB[0];
	heap->TCB[0] = heap->TCB[--(heap->length)];
	//heap->store[(heap->length)] = 0;
	heap_down(heap, 0);
	return value;
}

int heap_isEmpty(heap_t *heap) {
	return !(heap->length);
}

OS_TCB_t * top_element(heap_t *heap) {
	return heap->TCB[0];
}
