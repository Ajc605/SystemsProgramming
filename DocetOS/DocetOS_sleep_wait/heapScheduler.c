#include "heapScheduler.h"
#include "heap.h"
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
static OS_TCB_t const * heap_scheduler(void);
static void heap_addTask(OS_TCB_t * const tcb);
static void heap_taskExit(OS_TCB_t * const tcb);
static void heap_wait(void * const reason, uint32_t check);
static void heap_notify(void * const reason);

static heap_t runningHeap;
static OS_TCB_t * runningHeapStore[SCHEDULER_MAX_TASKS];

/* Scheduler block for the heap scheduler */
OS_Scheduler_t const heap_Scheduler = {
	.preemptive = 1,
	.scheduler_callback = heap_scheduler,
	.addtask_callback = heap_addTask,
	.taskexit_callback = heap_taskExit,
	.wait_callback = heap_wait,
	.notify_callback = heap_notify
};


/* Round-robin scheduler callback */
static OS_TCB_t const * heap_scheduler(void) {
	static int i = 0;
	// Clear the yield flag if it's set - we simply don't care
	OS_currentTCB()->state &= ~TASK_STATE_YIELD;
	for (int j = 1; j <= SCHEDULER_MAX_TASKS; j++) {
		i = (i + 1) % SCHEDULER_MAX_TASKS;
		OS_TCB_t * tcb = runningHeap.TCB[i];
		if (tcb != 0) {
			if(!(tcb->state & TASK_STATE_WAIT)) {
				if(!(tcb->state & TASK_STATE_SLEEP)) {
					//take's sleep bit not set	
					return tcb;
				} else {
					if((uint32_t)tcb->data < OS_elapsedTicks()) {
						//Clear sleep bit
						tcb->state &= ~TASK_STATE_SLEEP;
						//tasks[i]->data = 0;
						return tcb;
					}
				}
			}
		}
	}
	// No tasks in the list, so return the idle task
	return OS_idleTCB_p;
}

/* 'Add task' callback and sets the TCB's priority fileds*/
/* Need to add the TCB to the running task heap*/
static void heap_addTask(OS_TCB_t * const tcb) {
	//Loop throught the task
	for (int i = 0; i < SCHEDULER_MAX_TASKS; i++) {
			heap_insert(&runningHeap, tcb);
		}
	}
	// If we get here, there are no free TCB slots, so the task just won't be added

/* 'Task exit' callback */
static void heap_taskExit(OS_TCB_t * const tcb) {
	// Remove the given TCB from the list of tasks so it won't be run again
	for (int i = 0; i < SCHEDULER_MAX_TASKS; i++) {
		if (runningHeap.TCB[i] == tcb) {
			runningHeap.TCB[i] = 0;
		}
	}	
}

static void heap_wait(void * const reason, uint32_t check) {
	if(check == checksum()) {
		OS_TCB_t * prtCurTCB = OS_currentTCB();
		prtCurTCB->data = (uint32_t)reason;
		prtCurTCB->state |= TASK_STATE_WAIT;
		SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
	}
}

static void heap_notify(void * const reason) {
	for (int i = 0; i < SCHEDULER_MAX_TASKS; i++) {
		OS_TCB_t * tcb = runningHeap.TCB[i];
		if (tcb->state & TASK_STATE_WAIT) {
			if(tcb->data == (uint32_t)reason) {
				tcb->state &= ~TASK_STATE_WAIT;
				tcb->data = 0;
			}
		}
	}	
}
