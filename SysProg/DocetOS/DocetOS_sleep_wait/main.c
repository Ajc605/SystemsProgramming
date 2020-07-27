#include "os.h"
#include <stdio.h>
#include "utils/serial.h"
#include "priority.h"
#include "sleep.h"
#include "mutex.h"

static OS_mutex_t mutex;

void task1(void const *const args) {
	//while (1) {
		OS_mutex_acquire(&mutex);
		printf("Message from Task 1\r\n");
		OS_mutex_release(&mutex);
		OS_sleep(1);
	//}
}

void task2(void const *const args) {
	//while (1) {
		OS_mutex_acquire(&mutex);
		printf("Message from Task 2\r\n");
		OS_mutex_release(&mutex);
		OS_sleep(1);
	//}
}

/* MAIN FUNCTION */

int main(void) {
	/* Initialise the serial port so printf() works */
	serial_init();
	
	init_mutex(&mutex);

	printf("\r\nDocetOS Sleep and Mutex\r\n");

	/* Reserve memory for two stacks and two TCBs.
	   Remember that stacks must be 8-byte aligned. */
	__align(8)
	static uint32_t stack1[64], stack2[64];
	static OS_TCB_t TCB1, TCB2;

	/* Initialise the TCBs using the two functions above */
	OS_initialiseTCB(&TCB1, stack1+64, task1, 0, 2);
	OS_initialiseTCB(&TCB2, stack2+64, task2, 0, 7);

	/* Initialise and start the OS */
	OS_init(&priorityScheduler);
	OS_addTask(&TCB1);
	OS_addTask(&TCB2);
	OS_start();
}
