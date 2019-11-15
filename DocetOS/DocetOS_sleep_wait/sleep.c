#include "sleep.h"
#include "os_internal.h"

void OS_sleep (uint32_t addtionalTicks) {
	OS_TCB_t * prtCurTBC = OS_currentTCB();
	uint32_t currTicks = OS_elapsedTicks(); 
	prtCurTBC->data = currTicks + addtionalTicks;
	OS_yield();
}


