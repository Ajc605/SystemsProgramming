#include <stdio.h>
#include "serial.h"

int queue[10];
int insert = 0;
int retrieve = 0;

int put(int item) {
	int done = 1;
	if((insert == 9 && retrieve == 0)||(insert == retrieve -1)) {
		printf("full\r\n");
		done = 0;
	} else {
		printf("not full\r\n");
		queue[insert] = item;
		insert = (insert + 1) % 10;
		//printf("insert: %d\r\n",insert);
	}
	return done;
}

int get(int *p) {
	int done = 1;
	if(insert == retrieve) {
		done = 0;
		printf("Fail\r\n");
	} else {
		*p = queue[retrieve];
		retrieve = (retrieve + 1) % 10;
	
		//printf("Success\r\n");
	}
	return done;
}

int main(void) {
	serial_init();
	int value;
	printf("\r\n\r\nNothing is pushed: %d\r\n", get(&value));
	put(0);
	printf("Success or fail: %d, pushed value: %d\r\n", get(&value), value);
	put(1);
	put(2);
	put(3);
	put(4);
	put(5);
	put(6);
	put(7);
	put(8);
	put(9);
	put(10);
	printf("Success or fail: %d, pushed value: %d\r\n", get(&value), value);
	put(11);
	put(11);
	printf("Success or fail: %d, pushed value: %d\r\n", get(&value), value);
	printf("Success or fail: %d, pushed value: %d\r\n", get(&value), value);
	printf("Success or fail: %d, pushed value: %d\r\n", get(&value), value);
	printf("Success or fail: %d, pushed value: %d\r\n", get(&value), value);
	printf("Success or fail: %d, pushed value: %d\r\n", get(&value), value);
	printf("Success or fail: %d, pushed value: %d\r\n", get(&value), value);
	printf("Success or fail: %d, pushed value: %d\r\n", get(&value), value);
	printf("Success or fail: %d, pushed value: %d\r\n", get(&value), value);
	printf("Success or fail: %d, pushed value: %d\r\n", get(&value), value);
	printf("Success or fail: %d, pushed value: %d\r\n", get(&value), value);

	printf("finish\r\n");
}/*

#include <stdio.h>
#include "serial.h"

int queue[10];
int insert = 0;
int retrieve = 0;

void put(int item) {
	queue[insert] = item;
	insert = (insert + 1) % 10;
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
	printf("Removed %d from queue\r\n", get());
	printf("Removed %d from queue\r\n", get());
	printf("Removed %d from queue\r\n", get());
}
*/
