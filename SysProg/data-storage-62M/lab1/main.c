#include <stdio.h>
#include "serial.h"

typedef struct {
	int x;
	int y;
} test_t;

int main(void) {
	serial_init();
	
	test_t test;
	
	test_t *test_p = &test;
	
	test.x = 5;
	test.y = 10;
	
	printf("%d, %d \r\n", test.x, test.y);
	
	test_p->x = 100;
	test_p->y = -1;
	
	printf("%d, %d \r\n", test.x, test.y);
	
	// Your code goes here
}
