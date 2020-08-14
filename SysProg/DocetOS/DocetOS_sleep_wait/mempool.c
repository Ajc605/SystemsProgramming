#include "mempool.h"
#include "mutex.h"
#include <stdio.h>


/* This function is used to initalise a memory pool. It takes a constent pointer  to pool_t struct
and a constent pointer OS_mutex_t structure. This function will set the */
void pool_init(OS_pool_t * const pool, OS_mutex_t * const mutex) {
	pool->head = 0;
	pool->mutex = mutex;
}

/* This function takes a constant pointer to pool_t struct and returns a pointer to the next avaible.
The next avaible block is stored within the pools head, as this is block will now be in use the 
head needs to be updated to point to the next avaible block. This is stored at the within the returned 
block.
*/
void * pool_allocate(OS_pool_t * const pool) {
	OS_mutex_acquire(pool->mutex);
	/* Next avaibe block to be returned */
	void *prt = pool->head;
	/* change the head to point to the next avaible block */
	void ** head = prt;
	pool->head = *head;
	OS_mutex_release(pool->mutex);
	return prt;
	
}

/* This function takes two arguments: constant pointer to a pool_t structure and constant pointer to a block of memory.
This fuction will add the avaible block of memeory to the pool. Either because the pool is being initialsed 
or the block was allocted and from the pool and is no longer needed.
*/
void pool_deallocate(OS_pool_t * const pool, void * const item) {
	OS_mutex_acquire(pool->mutex);
	/* item is an address to a block of memory*/
	void ** prt_item = item;
	/* Instering a pointer to the current head */
	*prt_item = pool->head;
	/* Updating the head to point to new block*/
	pool->head = item;
	
	OS_mutex_release(pool->mutex);
}
