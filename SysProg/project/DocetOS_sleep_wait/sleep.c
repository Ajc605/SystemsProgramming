#include "sleep.h"
#include "os_internal.h"

void OS_sleep(uint32_t addtionalTicks) {
	OS_currentTCB()->data = OS_elapsedTicks() +	addtionalTicks;
	OS_yield();
}
