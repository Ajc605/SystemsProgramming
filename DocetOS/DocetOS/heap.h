#ifndef HEAP_H
#include "os.h"
#define HEAP_H

#define HEAP_SORT_PRIORITY_AFFECTED 0
#define HEAP_SORT_DATA 1

typedef struct {
	OS_TCB_t ** TCB;
	unsigned int length;
	unsigned int sort;
} heap_t;

void heap_init(heap_t *heap, OS_TCB_t ** tcb, unsigned int sort);
void heap_insert(heap_t *heap, OS_TCB_t * tcb);
void heap_down(heap_t * heap, int start_element);
void heap_up(heap_t * heap, int start_element);
void heap_fix(heap_t * heap, int start_element);
OS_TCB_t * heap_extract(heap_t *heap);
int heap_isEmpty(heap_t *heap);
OS_TCB_t * top_element(heap_t *heap);

#endif /* HEAP_H */
