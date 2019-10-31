#include <stdio.h>
#include <stdint.h>
#include "serial.h"

#pragma pack(push)
#pragma pack(1)

typedef struct {
	int32_t x;
	int32_t y;
	void *ptr;
	void (*callback) (void *);
} structure_t;

#pragma pack(pop)

void process(structure_t * s){};

void printInteger(int *integer) {
		
		printf("%d\r\n", *integer);
	}

int main(void) {
	serial_init();

	// Your code goes here...
	printf("\r\nstart\r\n");
	
	
	structure_t test;
	test.x = 5;
	test.y = 10;
	
	int32_t number = 15;
	test.ptr = &number;
	
	test.callback(&printInteger);
	printf("test\r\n");
	
	process(&test);
	
	printInteger(&test.x);
	printInteger(&test.y);
		
	
	while(1);
}
