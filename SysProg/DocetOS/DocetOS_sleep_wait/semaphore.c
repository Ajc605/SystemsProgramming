#include "semaphore.h"
#include "os.h"
#include "priority.h"

//__align(8)

void init_semaphore (OS_semaphore_t * semaphore) {
	semaphore->counter = 1;
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
	OS_notify(semaphore->prt_waiting_TCB);
}
