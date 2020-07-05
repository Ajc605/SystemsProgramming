#include <stdio.h>
#include <stdint.h>
#include "serial.h"

int32_t calculate(int32_t x, int32_t y);

int main(void) {
	serial_init();

	printf("Returned value: %d\r\n", calculate(10, 2));

	while(1);
}
