#include "heap.h"

static void heap_up(heap_t *heap) {
	// Write me!
	//Start eith the least element in the heap
	//if it's the rrot element, stop
	if((heap->length)-1 != 0){
		int value = heap->store[(heap->length)-1];
	} else {
		printf("First element\r\n");
	}
	
	
	//Compare it with its parent
	//if the parent is smaller, stop
	//swap the element with its parent
	//with the element in its new location, go back to step 2
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
