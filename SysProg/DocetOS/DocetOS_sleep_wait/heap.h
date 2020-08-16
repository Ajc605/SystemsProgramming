#ifndef HEAP_H
#define HEAP_H
#include "os.h"

typedef struct {
	OS_TCB_t ** TCB;
	unsigned int length;
} heap_t;

void heap_init(heap_t *heap, OS_TCB_t ** tcb);
void heap_insert(heap_t *heap, OS_TCB_t * tcb);
OS_TCB_t *  heap_extract(heap_t *heap);

#endif /* HEAP_H */
