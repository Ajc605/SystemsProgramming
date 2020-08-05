#include <stdio.h>
#include "serial.h"
#include "stack.h"

int main(void) {
	serial_init();	
	
	/* Array to hold stack entries */
	int stack[10];
	/* Stack pointer */
	int *stack_p = stack;

	/* Finish this! */
	push(&stack_p, 5);
	push(&stack_p, 10);
	
	for(int i = 0; i <= 100; i++) {
		push(&stack_p, i);
	}
	
	//printf("Popped value: %d\r\n", pop(&stack_p));
	
	for(int i = 0; i <= 11; i++) {
		printf("Popped value: %d\r\n", pop(&stack_p));
	}
}
