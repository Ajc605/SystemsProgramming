#include "heap.h"

static void heap_up(heap_t *heap) {
	// Write me!
	//Start eith the least element in the heap
	//if it's the rrot element, stop
	if(((heap->length)-1) != 0){
		int position = (heap->length)-1;
		printf("position: %d\r\n",position);
		int value = heap->store[position];
		int parent_p;
		int parent;
		//Compare it with its parent
		if(position % 2 == 0) {
			//even
			parent_p = (position/2);
			parent = heap->store[parent_p];
		} else {
			//odd
			parent_p = (position)/2;
			parent = heap->store[parent_p];
		}
		printf("parent: %d\r\n",parent_p);
		//if the parent is smaller, stop
		printf("checking %d > %d\r\n",parent,value);
		if(parent > value){
			printf("swap\r\n");
			//swap the element with its parent
			heap->store[parent_p] = value;
			heap->store[position] = parent;
			//with the element in its new location, go back to step 2
			heap_up(heap);
		}
	} else {
		printf("First\r\n");
	}
	
}

static void heap_down(heap_t *heap) {
	// Write me!
	
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
	heap_down(heap);
	return value;
}

int heap_isEmpty(heap_t *heap) {
	return !(heap->length);
}
