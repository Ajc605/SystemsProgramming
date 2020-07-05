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

void process(structure_t * s);

void printInteger(void * var) {
		int * int_var = var;
		printf("var: %d\r\n", *int_var);
	}	

void printDouble(void * var) {
	double * double_var = var;
	printf("var: %f\r\n", *double_var);
}

#pragma pack(pop)

int main(void) {
	serial_init();

	/* 3.4.1 */
	structure_t test;
	test.x = 10;
	test.y = 5;
	int32_t z = 200;
	test.ptr = &z;
	test.callback = printInteger;
	process(&test);
	printf("Test => X: %d, Y: %d\r\n", test.x, test.y);
	
	/* 3.4.2 */
	structure_t test_double;
	test_double.x = 300;
	test_double.y = 150;
	double double_var = 5.5;
	test_double.ptr = &double_var;
	test_double.callback = printDouble;
	process(&test_double);
	printf("Double test => X: %d, Y: %d\r\n", test_double.x, test_double.y);
	
	
	
	while(1);
}
