#include <stdio.h>
#include "serial.h"

int queue[10];
int insert = 0;
int retrieve = 0;

/*

*/
int put(int item) {
	int status = 0; 
	
	if((insert+1)%10 != retrieve) {
		queue[insert] = item; //Value is stored in the array at the location given by the insert
		insert = (insert + 1) % 10; //insert value is increaed and mod by 10 (to stop it going past 9
		status = 1;
	}else {
		printf("Full %d\n\r", item);
		printf("Insert: %d, Retrieve: %d\n\r", insert, retrieve);
	}
		
	return 0;
}

int get(int *value) {
	int status = 0;
	
	if(insert != retrieve) {
		status = 1;
		*value = queue[retrieve]; //Get the return value
		retrieve = (retrieve + 1) % 10; //insert value is increaed and mod by 10 (to stop it going past 9
	} else {
		printf("Empty\n\r");
	}
	
	return status;
}

int main(void) {
	serial_init();
	int value;
	
	put(4);
	put(7);
	put(2);
	if(get(&value) == 1) {
			printf("Removed: %d\r\n", value);
		}	else {
			printf("Empty\r\n");
		}
	
	
	/*printf("Removed %d from queue\r\n", get());
	printf("Removed %d from queue\r\n", get());
	printf("Removed %d from queue\r\n", get());*/
	printf("Break\n\r");
	
	for(int i = 0; i <= 11; i++) {
		put(i);
	}

	for(int i = 0; i <= 11; i++) {
		if(get(&value) == 1) {
			printf("Removed: %d\r\n", value);
		}	else {
			printf("Empty\r\n");
		}
	}
}
