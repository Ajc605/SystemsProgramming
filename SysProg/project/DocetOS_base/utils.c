#include "utils.h"
#include <stdint.h>
#include <stdio.h>
#include "os_internal.h"

void reportState() {
	uint32_t PSR = getPSR();
	uint32_t CONTROL = getCONTROL();
	
	if((PSR & 0x09)==0) {
		printf("Thread mode, ");
	} else {
		printf("Handler mode, ");
	}
	
	if(!((CONTROL & 0x01)==0)) {
		printf("privileged, ");
	} else {
		printf("not privileged, ");
	}
	
	if((CONTROL & 0x02)==0) {
		printf("MSP in use\r\n");
	} else {
		printf("PSP in use\r\n");
	} 
}

uint32_t _svc_report_state(_OS_SVC_StackFrame_t const * const stack) {
	uint32_t val = stack->r0;
	//stack->ro =val*val;
	return val;
}