#include <stdio.h>
#include "serial.h"

typedef struct {
	int x;
	int y;
} test_t;

int main(void) {
	serial_init();
	
	// Your code goes here
	printf("start\r\n");
	test_t test;
	test_t *test_p = &test;
	test.x = 10;
	test.y = 5;
	printf("X: %d, Y: %d\r\n", test.x, test.y);
	test_p->x = 100;
	test_p->y = -1;
	printf("X: %d, Y: %d\r\n", test.x, test.y);
	
	printf("finishr\r\n");
}
