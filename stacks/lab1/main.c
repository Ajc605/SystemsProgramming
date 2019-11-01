#include <stdio.h>
#include <stdint.h>
#include "serial.h"

void test(void) {
	printf("Hello!\r\n");
}

int main(void) {
	serial_init();

	uint32_t array[10];
	
	uint32_t * ptr;
  // First try overflowing the stack with garbage and see what happens
	ptr = array;
	*(ptr+1)= 1;
	*(ptr+2)= 2;
	*(ptr+3)= 3;
	*(ptr+4)= 4;
	*(ptr+5)= 5;
	*(ptr+6)= 6;
	*(ptr+7)= 7;
	*(ptr+8)= 8;
	*(ptr+9)= 9;
	*(ptr+10)= 10;
	*(ptr+11)= (uint32_t)test;
	
	// Then try to overwrite the link register here so that test() is run
}
