#include "mempool.h"
#include <stdio.h>

void pool_init(pool_t *pool) {
    // Initialise the pool
	pool->head = 0;
}

void *pool_allocate(pool_t *pool) {
    // Return the head of the list of blocks
    // Update the head pointer
	void *prt = &(*pool->head);
	
	int *x = (int*)pool->head;
	
	return prt;
	
}

void pool_deallocate(pool_t *pool, void *item) {
	/* item is an address to a block of memory*/
    // Add the new item to the head of the list
	
		/* Stores the address of the head into item */
		void * prt = item;
		void * head = &(*pool->head);

		item = (void *)&(*pool->head);
		pool->head = prt; 
	
	//void *prtHead = pool->head;
		//*item = prtHead;
		/* update head to point to new item */
}
