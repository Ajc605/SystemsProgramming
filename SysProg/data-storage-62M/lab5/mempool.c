#include "mempool.h"
#include <stdio.h>

void pool_init(pool_t *pool) {
    // Initialise the pool
	pool->head = 0;
}

void *pool_allocate(pool_t *pool) {
    // Return the head of the list of blocks
    // Update the head pointer
	pool->head = &pool->head + 1;
	return pool->head;
	
}

void pool_deallocate(pool_t *pool, void *item) {
    // Add the new item to the head of the list
	if(pool->head == 0) {
		/* First item in the list */
		printf("first item in list\r\n");
		pool->head = item; 
	} else {
		/* Loop through the list to find the last element*/
		void *element = &pool->head;
		while(1) {
				
		}
	}
	
}
