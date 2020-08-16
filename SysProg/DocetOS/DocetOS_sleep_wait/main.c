#include "os.h"
#include <stdio.h>
#include <string.h>
#include "utils/serial.h"
#include "priority.h"
#include "sleep.h"
#include "mutex.h"
#include "mempool.h"
#include "semaphore.h"

static OS_mutex_t runningMutex, poolMutex;
static OS_semaphore_t semaphore;
static	OS_pool_t pool;

/* Example data packet structure, just for demonstration purposes */
typedef struct {
	uint32_t id;
	char data[10];
} packet_t;

void task3(void) {
	//OS_mutex_acquire(&mutex);
	OS_semaphore_acquire(&semaphore);
	printf("Message from Task 3\r\n");
	//OS_mutex_release(&mutex);
	OS_semaphore_release(&semaphore);
}

void task1(void const *const args) {
	while(1) {
		//OS_mutex_acquire(&mutex);
		OS_semaphore_acquire(&semaphore);
		printf("Message from Task 1\r\n");
		task3();
		//OS_mutex_release(&mutex);
		OS_semaphore_release(&semaphore);
		
		//printf("Ticks %d\r\n",OS_elapsedTicks());
		OS_sleep(11);
		//printf("Ticks %d\r\n",OS_elapsedTicks());
	}
}

void task2(void const *const args) {
	while(1) {
		OS_mutex_acquire(&runningMutex);
		//OS_semaphore_acquire(&semaphore);
		printf("Message from Task 2\r\n");
		OS_mutex_release(&runningMutex);
		//OS_semaphore_release(&semaphore);
		OS_sleep(1);
	}
}

void task4(void const *const args) {
	OS_mutex_acquire(&runningMutex);
	
	printf("Message from Task 4\r\n");
	
	/* Allocate two blocks for data packets and fill them in */
	packet_t *p1 = pool_allocate(&pool);
	p1->id = 0;
	strncpy(p1->data, "first", 10);

	packet_t *p2 = pool_allocate(&pool);
	p2->id = 1;
	strncpy(p2->data, "second", 10);

	printf("First allocated packet (id %d, data '%s') at address %p\r\n", p1->id, p1->data, p1);
	printf("Second allocated packet (id %d, data '%s') at address %p\r\n", p2->id, p2->data, p2);
	/* Return the first packet to the pool */
	pool_deallocate(&pool, p1);
	
	OS_mutex_release(&runningMutex);
}

void task5(void const *const args) {
	OS_mutex_acquire(&runningMutex);
	printf("Message from Task 5\r\n");
	packet_t *p3 = pool_allocate(&pool);
	p3->id = 2;
	strncpy(p3->data, "third", 10);

	packet_t *p4 = pool_allocate(&pool);
	p4->id = 3;
	strncpy(p4->data, "fourth", 10);

	printf("Third allocated packet (id %d, data '%s') at address %p\r\n", p3->id, p3->data, p3);
	printf("Fourth allocated packet (id %d, data '%s') at address %p\r\n", p4->id, p4->data, p4);
	OS_mutex_release(&runningMutex);
}

void task6(void const *const args) {
	OS_mutex_acquire(&runningMutex);
	printf("Message from Task 6\r\n");
	OS_mutex_release(&runningMutex);
}

void task7(void const *const args) {
	OS_mutex_acquire(&runningMutex);
	printf("Message from Task 7\r\n");
	OS_mutex_release(&runningMutex);
}





/* MAIN FUNCTION */

int main(void) {
	/* Initialise the serial port so printf() works */
	serial_init();
	
	init_mutex(&runningMutex);
	//init_mutex(&poolMutex);
	init_semaphore(&semaphore);
	
	printf("\r\nDocetOS\r\n\r\n");

	/* Reserve memory for two stacks and two TCBs.
	   Remember that stacks must be 8-byte aligned. */
	__align(8)
	static uint32_t stack1[64], stack2[64], stack4[64], stack5[64], stack6[64], stack7[64];
	static OS_TCB_t TCB1, TCB2, TCB4, TCB5, TCB6, TCB7;

	/* Initialise the TCBs using the two functions above */
	OS_initialiseTCB(&TCB1, stack1+64, task1, 0, 1);
	OS_initialiseTCB(&TCB2, stack2+64, task2, 0, 1);
	OS_initialiseTCB(&TCB4, stack4+64, task4, 0, 3);
	OS_initialiseTCB(&TCB5, stack5+64, task5, 0, 5);
	OS_initialiseTCB(&TCB6, stack6+64, task6, 0, 8);
	OS_initialiseTCB(&TCB7, stack7+64, task7, 0, 2);
	
	

	/* Initialise and start the OS */
	OS_init(&priorityScheduler);
	
	static packet_t poolElements[10];
	pool_init(&pool, &poolMutex);
	for (int i = 0; i < 10; ++i) {
		void *prt = &poolElements[i];
		pool_add(&pool, prt);
	}
	
	OS_addTask(&TCB1);
	OS_addTask(&TCB2);
	OS_addTask(&TCB4);
	OS_addTask(&TCB5);
	OS_addTask(&TCB6);
	OS_addTask(&TCB7);
	OS_start();
}
