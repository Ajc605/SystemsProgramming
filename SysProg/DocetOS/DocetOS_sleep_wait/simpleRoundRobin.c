#include "simpleRoundRobin.h"
#include <stdio.h>
#include <stm32f4xx.h>

/* This is an implementation of an extremely simple (and inefficient!) round-robin scheduler.

   An array of pointers to TCBs is declared, and when tasks are added they are inserted into
	 this array.  When tasks finish, they are removed from the array (pointers are set back to
	 zero).  When the scheduler is invoked, it simply looks for the next available non-zero
	 pointer in the array, and returns it.  If there are no non-zero TCB pointers in the array,
	 a pointer to the idle task is returned instead.
	 
	 The inefficiency in this implementation arises mainly from the way arrays are used for list
	 storage.  If the array is large and the number of tasks is small, the scheduler will spend
	 a lot of time looking through the array each time it is called. */

/* Prototypes (functions are static, so these aren't in the header file) */
static OS_TCB_t const * simpleRoundRobin_scheduler(void);
static void simpleRoundRobin_addTask(OS_TCB_t * const tcb);
static void simpleRoundRobin_taskExit(OS_TCB_t * const tcb);
static void roundRobin_wait(void * const reason, uint32_t checkSum);
static void simpleRoundRobin_notify(void * const reason);

static OS_TCB_t * tasks[SIMPLE_RR_MAX_TASKS] = {0};

/* Scheduler block for the simple round-robin */
OS_Scheduler_t const simpleRoundRobinScheduler = {
	.preemptive = 1,
	.scheduler_callback = simpleRoundRobin_scheduler,
	.addtask_callback = simpleRoundRobin_addTask,
	.taskexit_callback = simpleRoundRobin_taskExit,
	.wait_callback = roundRobin_wait,
	.notify_callback = simpleRoundRobin_notify
};


/* Round-robin scheduler callback */
static OS_TCB_t const * simpleRoundRobin_scheduler(void) {
	static int i = 0;
	// Clear the yield flag if it's set - we simply don't care
	OS_currentTCB()->state &= ~TASK_STATE_YIELD;
	for (int j = 1; j <= SIMPLE_RR_MAX_TASKS; j++) {
		i = (i + 1) % SIMPLE_RR_MAX_TASKS;
		if (tasks[i] != 0) {
			/* Check sleep bit */
			if(tasks[i]->state & TASK_STATE_SLEEP) {
				/* Check if enough time has past */
				if(OS_elapsedTicks() >= tasks[i]->data){
					/* Clear sleep bit */
				tasks[i]->state &= ~TASK_STATE_SLEEP;
					return tasks[i];
				}
			} else {
				if(!(tasks[i]->state & TASK_STATE_WAIT)) {
					return tasks[i];
				}
			}
		}
	}
	// No tasks in the list, so return the idle task
	return OS_idleTCB_p;
}

/* 'Add task' callback */
static void simpleRoundRobin_addTask(OS_TCB_t * const tcb) {
	for (int i = 0; i < SIMPLE_RR_MAX_TASKS; i++) {
		if (tasks[i] == 0) {
			tasks[i] = tcb;
			return;
		}
	}
	// If we get here, there are no free TCB slots, so the task just won't be added
}

/* 'Task exit' callback */
static void simpleRoundRobin_taskExit(OS_TCB_t * const tcb) {
	// Remove the given TCB from the list of tasks so it won't be run again
	for (int i = 0; i < SIMPLE_RR_MAX_TASKS; i++) {
		if (tasks[i] == tcb) {
			tasks[i] = 0;
		}
	}	
}

/* 'wait' callback 
		Set the TCB to a waiting state and set the reason to the address of the mutex it is waiting for.*/
static void roundRobin_wait(void * const reason, uint32_t checkSum) {
	/* Check if an ISR has happend been calling this function and completeing it. */
	if(checkSum == getCheckSum()) {
		OS_TCB_t *cur_TCB = OS_currentTCB();
		uint32_t *data = reason;
		cur_TCB->data = *data;
		cur_TCB->state |= TASK_STATE_WAIT;
		SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
	}
}

/* 'notify' callback 		
		Check all elemenets in the RR array if they are waiting to the mutex that as just been relases*/
static void simpleRoundRobin_notify( void * const reason){
	__CLREX();
	static int i = 0;
	uint32_t *data = reason;
	/* Loop through the array of tasks */
	for (int j = 1; j <= SIMPLE_RR_MAX_TASKS; j++) {
		i = (i + 1) % SIMPLE_RR_MAX_TASKS;
		if (tasks[i] != 0) {
			/* Check wait bit and reason for waiting*/
			if((tasks[i]->state & TASK_STATE_WAIT)) { 
				if (tasks[i]->data == *data) { 
				tasks[i]->state &= ~TASK_STATE_WAIT;
				tasks[i]->data = 0;
				}
			}
		}
	}
}
