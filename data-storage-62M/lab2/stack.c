#include "stack.h"

void push(int **sp, int value) {
	/* Finish this! */
	**sp = value;
	*sp++;
}

int pop(int **sp) {
	/* Finish this! */	
	int value = **sp;
	*sp = *sp -1; 
	return value;
}
