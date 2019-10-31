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

void process(structure_t * s);

void report(uint32_t value) {
	printf("%d\r\n", value);
}


void printInteger(int *integer) {
		printf("tring to print\r\n");
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
	
	void (*fptr) (int*);
	fptr = printInteger;
	test.callback = fptr;
	
	process(&test);
	
	printInteger(&test.x);
	printInteger(&test.y);
		
	
	while(1);
}
