#include "mutex.h"

void initialiseMutex(OS_mutex_t * prtMutex) {
	prtMutex->counter = 0;
	prtMutex->prtTCB = 0;
}

void OS_mutex_acquire(OS_mutex_t * prtMutex) {
	OS_TCB_t * prtCurTBC = OS_currentTCB();
	while(1) {
		uint32_t mutexTCBField = __LDREXW(&(prtMutex->prtTCB));
		if(mutexTCBField == 0) {
			uint32_t store = __STREXW((uint32_t)prtCurTBC,&(prtMutex->prtTCB));
			if(store == 0) {
				break;
			}
		} else { 
			if (mutexTCBField != (uint32_t)prtCurTBC) {
				OS_wait(prtMutex);
			}
		}
	}
	prtMutex->counter++;	
}

void OS_mutex_release(OS_mutex_t * prtMutex) {
	prtMutex->counter--;
	if(prtMutex->counter == 0) {
		prtMutex->prtTCB =0;
		OS_notify(prtMutex);
	}
}
