#include "mutex.h"

void initialiseMutex(OS_mutex_t * mutex) {
	mutex->counter = 0;
	mutex->prtTCB = 0;
}

void OS_mutex_acquire(OS_mutex_t * mutex) {
	uint32_t mutexTCBField = __LDREXW(&mutex->prtTCB);
	if(mutexTCBField == 0) {
		uint32_t store = __STREXW((uint32_t)OS_currentTCB(),&mutex->prtTCB);
		if(store == 1) {
			OS_mutex_acquire(mutex);
		}
	} else if ((mutexTCBField != 0) && (mutexTCBField != (uint32_t)OS_currentTCB())) {
		OS_wait(&mutex);
		OS_mutex_acquire(mutex);
	}
	mutex->counter++;
}

void OS_mutex_release(OS_mutex_t * mutex) {
	mutex->counter--;
	if(mutex == 0) {
		mutex->prtTCB =0;
		OS_notify(&mutex);
	}
}
