#include "mutex.h"
#include "os.h"
#include "priority.h"


/* init_mutex is used to initalise a mutex, it takes a pointer to a OS_mutex_t struture as an arguement
and returns nothing. This function will set both pointers to OS_TCB_t structures to zero as
well as the counter value to zero.
*/
void init_mutex (OS_mutex_t * mutex) {
	mutex->prt_running_TCB = mutex->prt_waiting_TCB = 0;
	mutex->counter = 0;
}

/* OS_mutex_acquire used by a TCB to acquire a mutex, it takes a pointer to a OS_mutex_t structure as an argument
and returns nothing. Firstly it gets the check code, as this will determine whether  notify has been called during 
the time it tasks this function to put a TCB into a wait state. It will then determine if the mutex is free, if 
so then the current TCB will hold the mutex, if not then the task is put into a waiting state and then tries to 
acquire the mutex again. If the same TCB tried to acquire the mutex more then once, this will increase the counter
and the TCB will need to release the mutex the same number of times otherwise this will cause  an error where nothing 
can acquire that mutex. LDREW and STREXW are used respectively to load and store the 'prt_running_TCB' of the mutex, 
a CLREW is called when the counter is increased, as LDREW was used and STREW is not called.
*/
void OS_mutex_acquire(OS_mutex_t * mutex) {
	/* Get the check code to prevent race conditions */
	uint32_t checkCode = getCheckSum();
	while(1) {
		/* (1)Lodaing the mutex's TCB field*/
	 OS_TCB_t * mutex_TCB = (OS_TCB_t *) __LDREXW(&(mutex->prt_running_TCB));
		/* check if there is anything in the mutexs TCB field*/
		if(mutex_TCB == 0) {
			/* (2) Field is empty */
			/* (2a) try and store the current TCB in mutex*/
			uint32_t store = __STREXW((uint32_t)OS_currentTCB(), &(mutex->prt_running_TCB));
			/* (2b) Check if store was sucessful*/
			if(store == 0) {
				/* Successful*/
				break;
			}
		} else {
			if(mutex_TCB != OS_currentTCB()) {
				OS_wait(checkCode);
				while(1) {
					/* Load TCB */
					OS_TCB_t * TCB = (OS_TCB_t *) __LDREXW(&(mutex->prt_waiting_TCB));
					/* Check TCB is a set*/
					if(!TCB) {
						/* The pointer point is not set in either mutex or semaphore */
						uint32_t store = __STREXW((uint32_t)OS_currentTCB(), &(mutex->prt_waiting_TCB));
						/* Check if store was sucessful*/
						if(store == 0) {
							/* Successful*/
							break;
						}
					} else if(TCB == OS_currentTCB()) {
						__CLREX();
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
			} else {
				__CLREX();
				break;
			}
		}
	}
	mutex->counter++;
}

/* OS_mutex_release used by TCB to release a mutex, it takes a pointer to a OS_mutex_t structure	 as an argument
and returns nothing. It first checks that the current TCB has hold of the mutex, then decreases the counter by 1.
If the new counter value is now zero, then set the mutex's 'prt_running_TCB' to zero and call notify.
*/
void OS_mutex_release(OS_mutex_t * mutex) {
	if(mutex->prt_running_TCB == OS_currentTCB()) {
		mutex->counter--;
		if(mutex->counter == 0) {
			mutex->prt_running_TCB = 0;
			OS_notify(mutex->prt_waiting_TCB);
		}
	}
}
