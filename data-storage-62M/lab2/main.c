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
	push(&stack_p, 15);
	push(&stack_p, 10);
	printf("Popped value: %d\r\n", pop(&stack_p));
	printf("Popped value: %d\r\n", pop(&stack_p));
	printf("Popped value: %d\r\n", pop(&stack_p));
	printf("Popped value: %d\r\n", pop(&stack_p));
}
