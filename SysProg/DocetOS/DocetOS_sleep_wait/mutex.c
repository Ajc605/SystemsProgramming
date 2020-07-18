#include "mutex.h"
#include "os.h"
#include "simpleRoundRobin.h"

//__align(8)

void init_mutex (OS_mutex_t * mutex) {
	mutex->counter = 0;
	mutex->prt_TCB = 0;
}

void OS_mutex_acquire(OS_mutex_t * mutex) {
	/* getting the currentlu runing TCB */
	OS_TCB_t * cur_TCB = OS_currentTCB();
	
	while(1) {
		/* (1)Lodaing the mutex's TCB field*/
	 uint32_t mutex_TCB =  __LDREXW(&(mutex->prt_TCB));
		
		/* check if there is anything in the mutexs TCB field*/
		if(mutex_TCB == 0) {
			/* (2) Field is empty */
			
			/* (2a) try and store the current TCB in mutex*/
			uint32_t store = __STREXW((uint32_t)cur_TCB, &(mutex->prt_TCB));
			/* (2b) Check if store was sucessful*/
			if(store == 0) {
				/* Successful*/
				break;
			}
		} else {
			if(mutex_TCB != (unsigned)cur_TCB) {
				OS_wait(mutex, getCheckSum());
			} 
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
