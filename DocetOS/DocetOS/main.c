#include "os.h"
#include <stdio.h>
#include "utils/serial.h"
#include "heapScheduler.h"
#include "mutex.h"

static OS_mutex_t mutex;

void task1(void const *const args) {
	for(int i = 0; i < 2; i++) {
		OS_mutex_acquire(&mutex);
		printf("Message from Task 1, priority 3\r\n");
		OS_mutex_release(&mutex);
		OS_sleep(20);
	}
}
void task2(void const *const args) {
	for(int i = 0; i < 2; i++) {
		OS_mutex_acquire(&mutex);
		printf("Message from Task 2, priority 2\r\n");
		OS_mutex_release(&mutex);
		OS_sleep(95);
	}
}

void task3(void const *const args) {
	for(int i = 0; i < 2; i++) {
		OS_mutex_acquire(&mutex);
		printf("Message from Task 3, priority 1\r\n");
		OS_mutex_release(&mutex);
		OS_sleep(95);
	}
}

/* MAIN FUNCTION */

int main(void) {
	/* Initialise the serial port so printf() works */
	serial_init();

	printf("\r\nDocetOS Heap Scheduler\r\n");
	
	initialiseMutex(&mutex);

	/* Reserve memory for two stacks and two TCBs.
	   Remember that stacks must be 8-byte aligned. */
	__align(8)
	static uint32_t stack1[64], stack2[64], stack3[64], stack4[64];
	static OS_TCB_t TCB1, TCB2, TCB3, TCB4;

	/* Initialise the TCBs using the two functions above */
	OS_initialiseTCB(&TCB1, stack1+64, task1, 0, 3);
	OS_initialiseTCB(&TCB2, stack2+64, task2, 0, 2);
	OS_initialiseTCB(&TCB3, stack3+64, task3, 0, 1);
	/* Initialise and start the OS */
	OS_init(&heapScheduler);
	OS_addTask(&TCB1);
	OS_addTask(&TCB2);
	OS_addTask(&TCB3);
	OS_start();
}
