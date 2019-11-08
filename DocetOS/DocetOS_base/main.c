#include "os.h"
#include <stdio.h>
#include "utils/serial.h"
#include "simpleRoundRobin.h"

void task1(void const *const args) {
	for (int i = 0; i < 100; ++i) {
		printf("A");
	}
}
#include "stm32f4xx.h"
void task2(void const *const args) {
	for (int i = 0; i < 200; ++i) {
		printf("B");
	}
}

void task3(void const *const args) {
	for (int i = 0; i < 100; ++i) {
		printf("C");
	}
}

/* MAIN FUNCTION */

int main(void) {
	/* Initialise the serial port so printf() works */
	serial_init();

	printf("\r\nDocetOS Demo\r\n");

	/* Reserve memory for two stacks and two TCBs.
	   Remember that stacks must be 8-byte aligned. */
	__align(8)
	//Set up values for functions in OS
	static uint32_t stack1[64], stack2[64], stack3[64];
	static OS_TCB_t TCB1, TCB2, TCB3;

	/* Initialise the TCBs using the two functions above */
	//Creating function in OS
	OS_initialiseTCB(&TCB1, stack1+64, task1, NULL);
	OS_initialiseTCB(&TCB2, stack2+64, task2, NULL);
	OS_initialiseTCB(&TCB3, stack3+64, task3, NULL);
	OS_init(&simpleRoundRobinScheduler);

	

	/* Initialise and start the OS */
	
	//Calling functions in OS
	OS_addTask(&TCB1);
	OS_addTask(&TCB2);
	OS_addTask(&TCB3);
	
	OS_start();
	
}
