#include "stack.h"

void push(int **sp, int value) {
	/* Finish this! */
	**sp = value;
	*sp = *sp + 1;
}

int pop(int **sp) {
	/* Finish this! */
	*sp = *sp - 1;
	int value = **sp;
	return value;
}
