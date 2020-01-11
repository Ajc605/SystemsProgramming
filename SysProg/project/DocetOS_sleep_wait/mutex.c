#include "mutex.h"

void initialiseMutex(OS_mutex_t * prtMutex) {
	prtMutex->counter = 0;
	prtMutex->prtTCB = 0;
}

void OS_mutex_acquire(OS_mutex_t * prtMutex) {
	uint32_t mutexTCBField = __LDREXW(&prtMutex->prtTCB);
	if(mutexTCBField == 0) {
		uint32_t store = __STREXW((uint32_t)OS_currentTCB(),&prtMutex->prtTCB);
		if(store == 1) {
			OS_mutex_acquire(prtMutex);
		}
	} else if ((mutexTCBField != 0) && (mutexTCBField != (uint32_t)OS_currentTCB())) {
		OS_wait(prtMutex, checkSum());
		OS_mutex_acquire(prtMutex);
	}
	prtMutex->counter++;
}

void OS_mutex_release(OS_mutex_t * prtMutex) {
	prtMutex->counter--;
	if(prtMutex == 0) {
		prtMutex->prtTCB =0;
		OS_notify(&prtMutex);
	}
}
