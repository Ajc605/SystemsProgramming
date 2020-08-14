#include "semaphore.h"
#include "os.h"
#include "priority.h"

//__align(8)

void init_semaphore (OS_semaphore_t * semaphore) {
	semaphore->counter = 2;
	semaphore->prt_waiting_TCB = 0;
}

void OS_semaphore_acquire(OS_semaphore_t * semaphore) {
	/* getting the current runing TCB */
	uint32_t checkCode = getCheckSum();
	while(1) {
		/* Lodaing the semaphore counter */
		uint32_t counter_value =  __LDREXW(&(semaphore->counter));
		/* check if its non zero */
		if(counter_value != 0) {
			/* Descrease and store */
			uint32_t store = __STREXW(--counter_value, &(semaphore->counter));
			/* Check if store was sucessful*/
			if(store == 0) {
				/* Successful*/
				break;
			}
		} else {
			/* Wait and try again */	
			while(1) {
				/* Load TCB */
			  OS_TCB_t * TCB = (OS_TCB_t *) __LDREXW(&(semaphore->prt_waiting_TCB));
				/* Check TCB is a set*/
				if(!TCB) {
					/* The pointer point is not set in either mutex or semaphore */
					uint32_t store = __STREXW((uint32_t)OS_currentTCB(), &(semaphore->prt_waiting_TCB));
					/* Check if store was sucessful*/
					if(store == 0) {
						/* Successful*/
						break;
					}
				} else if(TCB->prt_TCB == OS_currentTCB) {
					__CLREX();
					break;
				}else if(!TCB->prt_TCB) {
					/* Reached end of list, add current TCB to end of list*/
					uint32_t store = __STREXW((uint32_t)OS_currentTCB(), &(semaphore->prt_waiting_TCB->prt_TCB));
					/* (2b) Check if store was sucessful*/
					if(store == 0) {
						/* Successful*/
						break;
					}
				} else {
					TCB = TCB->prt_TCB;
				}
			}
			OS_wait(OS_currentTCB(), checkCode);
		}
	}
}

void OS_semaphore_release(OS_semaphore_t * semaphore) {
	while(1) {
		/* Lodaing the semaphore counter */
		uint32_t counter_value =  __LDREXW(&(semaphore->counter));
		/* Increase by one and store*/
		uint32_t store = __STREXW(++counter_value, &(semaphore->counter));
		/* Check if store was sucessful*/
		if(store == 0) {
			/* Successful*/
			break;
		}
	}
	if(semaphore->prt_waiting_TCB != 0){
		OS_TCB_t * notify = semaphore->prt_waiting_TCB;
		semaphore->prt_waiting_TCB = notify->prt_TCB;
		OS_notify(notify);
	}
}
