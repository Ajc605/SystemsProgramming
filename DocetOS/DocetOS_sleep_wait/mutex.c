#include "mutex.h"
#include "os.h"

void ini_mutex(OS_mutex_t * mutex) {
	mutex->counter = 0;
	mutex->prt_TBC = 0;	
}

void OS_mutex_acquire(OS_mutex_t * mutex) {
	uint32_t loadReturn = LDREXW(mutex->prt_TBC);
	if(loadReturn == 0) {
		uint32_t success = STREX(mutex->prt_TBC, );
		if(success != 0) {
			OS_mutex_acquire(mutex);
		}			
	} else if (mutex->prt_TBC != OS_currentTCB()) {
		OS_wait(mutex);
		OS_mutex_acquire(mutex);
	}
	mutex->counter++;
}

void OS_mutex_release(OS_mutex_t * mutex) {
	mutex->counter--;
	OS_notify(mutex); 
}
