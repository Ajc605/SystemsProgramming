#include "utils.h" 
#include <stdio.h>
#include <stdint.h>

void reportState() {
	uint32_t psr = getPSR();
	uint32_t control = getCONTROL();
	
	printf("psr %u\n\r",(int)psr);
	printf("control %u\n\r",(int)control);
	
	if((psr & 0x09)==0) {
		printf("Thread mode\r\n");
	} else {
		printf("Handler mode\r\n");
	} 
	
	if((control & 0x02)==0) {
		printf("MSP\r\n");
	} else {
		printf("PSP\r\n");
	} 
	
	if(!((control & 0x01)==0)) {
		printf("privileged\r\n");
	}
}

void _svc_report_state(void) {
	reportState();
}
