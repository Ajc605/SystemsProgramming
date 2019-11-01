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
void printDouble(void * value) {
	double *ptr = value;
	printf("%f\r\n", *ptr);
}

void report(uint32_t value) {
	printf("%d\r\n", value);
}


void printInteger(void * integer) {
		int *ptr = integer;
		printf("%d\r\n", *ptr);
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
	
	test.callback = printInteger;
	
	process(&test);
	
	printInteger(&test.x);
	printInteger(&test.y);
	
	structure_t doub;
	
	doub.x = 100;
	doub.y = 50;
	double num = 25.555;
	doub.ptr = &num;
	doub.callback = printDouble;
	
	process(&doub);
	
	printInteger(&doub.x);
	printInteger(&doub.y);
		
	
	while(1);
}
