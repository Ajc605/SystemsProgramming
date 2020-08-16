#include "mempool.h"
#include "mutex.h"
#include <stdio.h>


/* pool_init, this function is used to initialise a memory pool. It takes a constant pointer  to pool_t struct
and a constant pointer OS_mutex_t structure. This function will set the head to zero, mutex pointer to point 
to mutex passed to the function. It will also initiate the mutex.
*/
void pool_init(OS_pool_t * const pool, OS_mutex_t * const mutex) {
	pool->head = 0;
	pool->mutex = mutex;
	init_mutex(pool->mutex);
}

/* pool_allocate, this function takes a constant pointer to pool_t struct and returns a pointer to the next 
available  block. This can be found in the pool's head field and the head is updated to point to the next
available  block, which is stored within the current block the head is pointing to.
*/
void * pool_allocate(OS_pool_t * const pool) {
	OS_mutex_acquire(pool->mutex);
	/* Next avaibe block to be returned */
	void *prt = pool->head;
	/* Change the head to point to the next avaible block */
	void ** head = prt;
	pool->head = *head;
	OS_mutex_release(pool->mutex);
	return prt;
	
}

/* pool_deallocate, this function takes two arguments constant pointer to a pool_t structure and constant 
pointer to a block of memory. This function will add the available block of memory to the pool. Either 
because the pool is being initialised or the block was allocated and from the pool and is no longer needed.
The head is updated to point to the new block, and the new bock will store a pointer to the previous  value
of the head.
*/
void pool_deallocate(OS_pool_t * const pool, void * const item) {
	OS_mutex_acquire(pool->mutex);
	/* item is an address to a block of memory */
	void ** prt_item = item;
	/* Instering a pointer to the current head */
	*prt_item = pool->head;
	/* Updating the head to point to new block */
	pool->head = item;
	
	OS_mutex_release(pool->mutex);
}
