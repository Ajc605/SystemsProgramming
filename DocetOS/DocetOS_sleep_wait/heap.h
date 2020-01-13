#ifndef HEAP_H
#include "os.h"
#define HEAP_H

typedef struct {
	OS_TCB_t ** TCB;
	unsigned int length;
} heap_t;

void heap_init(heap_t *heap, OS_TCB_t ** tcb);
void heap_insert(heap_t *heap, OS_TCB_t * tcb);
void heap_down(heap_t * heap);
void heap_up(heap_t * heap);
OS_TCB_t * heap_extract(heap_t *heap);
int heap_isEmpty(heap_t *heap);
OS_TCB_t * top_element(heap_t *heap);

#endif /* HEAP_H */
