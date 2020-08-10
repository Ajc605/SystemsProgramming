#include "sleep.h"
#include "os_internal.h"

//__align(8)

void OS_sleep(uint32_t val) {
	OS_TCB_t *cur_TCB = OS_currentTCB();
	cur_TCB->data = (unsigned)(OS_elapsedTicks() + val);
	cur_TCB->state |= TASK_STATE_SLEEP;
	OS_yield();
}
