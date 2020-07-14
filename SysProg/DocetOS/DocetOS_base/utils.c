#include "utils.h"
#include <stdio.h>
#include <stdint.h>
#include "os_internal.h"

uint32_t getPSR(void);
uint32_t getCONTROL(void);

void reportState(void) {
	uint32_t psr = getPSR();
	uint32_t control = getCONTROL();
	
	
	if((psr & 0x09) == 0) {
		printf("thread mode, ");
	} else {
		printf("handler mode, ");
	}
	
	if((control & 0x01) == 0) {
		printf("Privileged, %d, ",(int)control);
	} else {
		printf("Not privileged, %d, ",(int)control);
	}
	
	if((control & 0x02) == 0) {
		printf("MSP in use\r\n");
	} else {
		printf("PSP in use\r\n");
	}
}

void _svc_report(_OS_SVC_StackFrame_t * const stack) {
	printf("%d",stack->r0);
	stack->r0++;
}
