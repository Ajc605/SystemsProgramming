#include "mutex.h"
#include "os.h"
#include "priority.h"

//__align(8)

void init_mutex (OS_mutex_t * mutex) {
	mutex->prt_running_TCB = mutex->prt_waiting_TCB = 0;
	mutex->counter = 0;
}

void OS_mutex_acquire(OS_mutex_t * mutex) {
	/* getting the currentlu runing TCB */
	OS_TCB_t * cur_TCB = OS_currentTCB();
	uint32_t checkCode = getCheckSum();
	while(1) {
		/* (1)Lodaing the mutex's TCB field*/
	 OS_TCB_t * mutex_TCB = (OS_TCB_t *) __LDREXW(&(mutex->prt_running_TCB));
		/* check if there is anything in the mutexs TCB field*/
		if(mutex_TCB == 0) {
			/* (2) Field is empty */
			/* (2a) try and store the current TCB in mutex*/
			uint32_t store = __STREXW((uint32_t)cur_TCB, &(mutex->prt_running_TCB));
			/* (2b) Check if store was sucessful*/
			if(store == 0) {
				/* Successful*/
				break;
			}
		} else {
			if(mutex_TCB != cur_TCB) {
				OS_wait(mutex_TCB, checkCode);
			} else {
				__CLREX();
				break;
			}
		}
	}
	mutex->counter++;
}

void OS_mutex_release(OS_mutex_t * mutex) {
	if(mutex->prt_running_TCB == OS_currentTCB()) {
		mutex->counter--;
		if(mutex->counter == 0) {
			mutex->prt_running_TCB = 0;
			OS_notify(mutex->prt_waiting_TCB);
		}
	}
}
