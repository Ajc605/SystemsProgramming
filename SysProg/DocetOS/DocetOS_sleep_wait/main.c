#include "os.h"
#include <stdio.h>
#include "utils/serial.h"
#include "priority.h"
#include "sleep.h"
#include "mutex.h"
#include "semaphore.h"

static OS_mutex_t mutex;
static OS_semaphore_t semaphore;


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
		//OS_sleep(11);
		//printf("Ticks %d\r\n",OS_elapsedTicks());
	}
}

void task2(void const *const args) {
	while(1) {
		OS_mutex_acquire(&mutex);
		printf("Message from Task 2\r\n");
		OS_mutex_release(&mutex);
		OS_sleep(1);
	}
}

void task4(void const *const args) {
		OS_mutex_acquire(&mutex);
		printf("Message from Task 4\r\n");
		OS_mutex_release(&mutex);
}

void task5(void const *const args) {
		OS_mutex_acquire(&mutex);
		printf("Message from Task 5\r\n");
		OS_mutex_release(&mutex);
}

void task6(void const *const args) {
		OS_mutex_acquire(&mutex);
		printf("Message from Task 6\r\n");
		OS_mutex_release(&mutex);
}

void task7(void const *const args) {
		OS_mutex_acquire(&mutex);
		printf("Message from Task 7\r\n");
		OS_mutex_release(&mutex);
}


/* MAIN FUNCTION */

int main(void) {
	/* Initialise the serial port so printf() works */
	serial_init();
	
	init_mutex(&mutex);
	init_semaphore(&semaphore);

	printf("\r\nDocetOS Sleep and Mutex\r\n");

	/* Reserve memory for two stacks and two TCBs.
	   Remember that stacks must be 8-byte aligned. */
	__align(8)
	static uint32_t stack1[64], stack2[64], stack4[64], stack5[64], stack6[64], stack7[64];
	static OS_TCB_t TCB1, TCB2, TCB4, TCB5, TCB6, TCB7;

	/* Initialise the TCBs using the two functions above */
	OS_initialiseTCB(&TCB1, stack1+64, task1, 0, 1);
	OS_initialiseTCB(&TCB2, stack2+64, task2, 0, 1);
	OS_initialiseTCB(&TCB4, stack4+64, task4, 0, 1);
	OS_initialiseTCB(&TCB5, stack5+64, task5, 0, 1);
	OS_initialiseTCB(&TCB6, stack6+64, task6, 0, 3);
	OS_initialiseTCB(&TCB7, stack7+64, task7, 0, 2);

	/* Initialise and start the OS */
	OS_init(&priorityScheduler);
	OS_addTask(&TCB1);
	/*OS_addTask(&TCB2);
	OS_addTask(&TCB4);
	OS_addTask(&TCB5);
	OS_addTask(&TCB6);
	OS_addTask(&TCB7);*/
	OS_start();
}
