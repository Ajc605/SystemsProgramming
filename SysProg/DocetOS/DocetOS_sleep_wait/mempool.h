#ifndef MEMPOOL_H
#define MEMPOOL_H
#include <stddef.h>
#include "mutex.h"

typedef struct {
	void * head;
	OS_mutex_t * mutex;
} OS_pool_t;

void pool_init(OS_pool_t * const pool, OS_mutex_t * const mutex);
void * pool_allocate(OS_pool_t * const pool);
void pool_deallocate(OS_pool_t * const pool, void * const item);

#define pool_add pool_deallocate

#endif /* MEMPOOL_H */
