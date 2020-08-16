#include "semaphore.h"
#include "os.h"
#include "priority.h"

/* init_seamphore is used to initialise a semaphore, it takes a pointer to a semaphore as an argument
and returns nothing. This function will set the counter field, this will determine whether the semaphore
is a binary or counting semaphore: counter set to '1' would be binary and anything greater than '1' 	
would be a counting semaphore implementation. And the pointer to a TCB is set to zero.
*/
void init_semaphore (OS_semaphore_t * semaphore) {
	semaphore->counter = 2;
	semaphore->prt_waiting_TCB = 0;
}

/* OS_semaphore_acquire, this function will assess whether a TCB can hold the semaphore or needs to wait. It 
takes a pointer to a OS_semaphore_t structure and returns nothing. Check code is retrieved  at the start of 
this function, if the TCB needs to be put to a wait state this is passed to betermine of an ISR has occurred  
since this function started. LDERW is used to load the counter value, if this value is not zero then 
it is decreased and stored back with STREW. If the counter is zero then a CLERX is called and wait is called and the TCB is added to the linked
list of waiting TCB's.
*/
void OS_semaphore_acquire(OS_semaphore_t * semaphore) {
	/* Get the current runing TCB */
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
			__CLREX();
			OS_wait(OS_currentTCB(), checkCode);
			
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
				} else if(TCB == OS_currentTCB()) {
					break;
				}else if(!TCB->prt_TCB) {
					/* Reached end of list, add current TCB to end of list*/
					uint32_t store = __STREXW((uint32_t)OS_currentTCB(), &(TCB->prt_TCB));
					/* (2b) Check if store was sucessful*/
					if(store == 0) {
						/* Successful*/
						break;
					}
				} else {
					TCB = TCB->prt_TCB;
				}
			}
		}
	}
}


/* OS_semaphore_release, this function is by TCB to release a semaphore. This function takes a pointer to a 
'OS_semaphore_t' structure and returns nothing. First LDREW is used to load the counter value, then it is 
increased and stored back with STREW. Call notify and pass the semaphore's pointer to 'OS_TCB_t' field as
a argument. This will free the semaphore for the TCB at the head of the linked list.
*/
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
