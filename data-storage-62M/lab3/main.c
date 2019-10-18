#include <stdio.h>
#include "serial.h"

int queue[10];
int insert = 0;
int retrieve = 0;

void put(int item) {
	queue[insert] = item;
	insert = (insert + 1) % 10;
	printf("insert: %d\r\n",insert);
}

int get() {
	int value = queue[retrieve];
	retrieve = (retrieve + 1) % 10;
	return value;
}

int main(void) {
	serial_init();
	
	put(4);
	put(7);
	put(2);
	put(1);
	put(2);
	put(3);
	put(4);
	put(5);
	put(6);
	put(7);
	put(10);
	printf("start\r\n");
	printf("Removed %d from queue\r\n", get());
	printf("Removed %d from queue\r\n", get());
	printf("Removed %d from queue\r\n", get());
	printf("Removed %d from queue\r\n", get());
	printf("Removed %d from queue\r\n", get());
	printf("Removed %d from queue\r\n", get());
	printf("Removed %d from queue\r\n", get());
	printf("Removed %d from queue\r\n", get());
	printf("Removed %d from queue\r\n", get());
	printf("Removed %d from queue\r\n", get());
	printf("Removed %d from queue\r\n", get());
	printf("finish\r\n");
}
