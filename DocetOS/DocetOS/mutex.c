#include "mutex.h"
#include "os.h"

void initialiseMutex(OS_mutex_t * prtMutex) {
	prtMutex->counter = 0;
	prtMutex->prtTCB = 0;
}

void OS_mutex_acquire(OS_mutex_t * prtMutex) {
	OS_TCB_t * prtCurTBC = OS_currentTCB();
	while(1) {
		/* Load the TCB being used in the mutex */
		uint32_t mutexTCBField = __LDREXW(&(prtMutex->prtTCB));
		/* Check if the mutex is free */
		if(mutexTCBField == 0) {
			/* Try to store the current TCB in the mutex */
			uint32_t store = __STREXW((uint32_t)prtCurTBC,&(prtMutex->prtTCB));
			/* Check if store was successfuly */
			if(store == 0) {
				break;
			}
		} else { 
			/* Check if its the curretn TCB */
			if (mutexTCBField != (uint32_t)prtCurTBC) {
				/* Set TCB to wait */
				OS_wait(prtMutex, checksum());
			}
		}
	}
	prtMutex->counter++;	
}

void OS_mutex_release(OS_mutex_t * prtMutex) {
	/* Check that mutex is owned by the task its calling */
	if(prtMutex->prtTCB == OS_currentTCB()) {
		prtMutex->counter--;
		if(prtMutex->counter == 0) {
			/* Reset the mutex's */ 
			prtMutex->prtTCB =0;
			OS_notify(prtMutex);
		}
	}
}
