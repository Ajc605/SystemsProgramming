#include "mutex.h"
#include "os.h"

//__align(8)

void init_mutex (OS_mutex_t * mutex) {
	mutex->counter = 0;
	mutex->prt_TCB = 0;
}

void OS_mutex_acquire(OS_mutex_t * mutex) {
	OS_TCB_t * cur_TCB = OS_currentTCB();
	uint32_t mutex_TCB =  __LDREXW(&(mutex->prt_TCB));
	while(1) {
		if(mutex_TCB == 0) {
			uint32_t store = __STREXW((uint32_t)cur_TCB, &(mutex->prt_TCB));
			if(store == 0) {
				break;
			}
		} else {
			OS_wait(mutex, getCheckSum());
		}
	}
	mutex->counter++;
}

void OS_mutex_release(OS_mutex_t * mutex) {
	if(mutex->prt_TCB == OS_currentTCB()) {
		mutex->counter--;
		if(mutex->counter == 0) {
			mutex->prt_TCB = 0;
			OS_notify(mutex);
		}
	}
}
