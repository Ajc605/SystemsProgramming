#include "heapScheduler.h"
#include "heap.h"
#include <stm32f4xx.h>

/* 
	 This implementation of a scheduler uses heaps to place the 
	 task with the highest priority at the top, also known as the 
	 root node. This allows for quick access to the next runable 
	 task. The scheduler also modifies a task's priority, dependant
	 on it's state e.g. wait or sleep. Such that task with a priority 
	 higher then 'SCHEDULER_MAX_TASKS' won't be checked for a runnable task.
	 
	 In this sheduler 0 is considered the highest priority.
*/

/* Prototypes (functions are static, so these aren't in the header file) */
static OS_TCB_t const * heap_scheduler(void);
static void heap_addTask(OS_TCB_t * const tcb);
static void heap_taskExit(OS_TCB_t * const tcb);
static void heap_wait(void * const reason, uint32_t check);
static void heap_notify(void * const reason);
static int find_tcb_in_heap(heap_t * heap, OS_TCB_t * tcb);
static heap_t runningHeap;
static OS_TCB_t * runningHeapStore[SCHEDULER_MAX_TASKS];
static heap_t sleepingHeap;
static OS_TCB_t * sleepingHeapStore[SCHEDULER_MAX_TASKS];

/* Scheduler block for the heap scheduler */
OS_Scheduler_t const heapScheduler = {
	.preemptive = 1,
	.scheduler_callback = heap_scheduler,
	.addtask_callback = heap_addTask,
	.taskexit_callback = heap_taskExit,
	.wait_callback = heap_wait,
	.notify_callback = heap_notify
};


/* 
	 Heap scheduler
	 The scheduler will first check the sleep heap for any TCB's that 
	 need to be woken. Then checks the whether the TCB at the top of 
	 'runningHeap' has the right priority to be run. Only TCB with a 
	 priority filed lower then 'SCHEDULER_MAX_PRIORITY' are classed as runnable
*/
static OS_TCB_t const * heap_scheduler(void) {	
  /* Check task's in the sleepingHeap */
	while(!heap_isEmpty(&sleepingHeap)){ 
		/* Check if the top task needs to be woken */
		if((top_element(&sleepingHeap))->data < OS_elapsedTicks()){
			OS_TCB_t * woken_tcb = heap_extract(&sleepingHeap);
			/* Take the TCB out of a sleep state */
			woken_tcb->state &= ~TASK_STATE_SLEEP;
			/* Reset it's priority */
			woken_tcb->priority_affected = woken_tcb->priority_assigned;
			woken_tcb->data = 0;
			/* Fix the runningHeap with the TCB new priority */
			int tcb_heap_index = find_tcb_in_heap(&runningHeap, woken_tcb);
			if(tcb_heap_index >= 0) {
				heap_fix(&runningHeap, tcb_heap_index);
			}
		} else {
			break;
		}
	}
	OS_TCB_t * curr_tcb = OS_currentTCB();
	/* Clear the yield flag if it's set - we simply don't care */
	curr_tcb->state &= ~TASK_STATE_YIELD;
	OS_TCB_t * next_tcb = top_element(&runningHeap);
	/* Check if the top TCB is eligible to run */
	if(next_tcb->priority_affected < (SCHEDULER_MAX_PRIORITY + 1)) {
		return next_tcb;
	}
	/* No TCB's in the list that are eligible to run, so return the idle TCB */
	return OS_idleTCB_p;
}

/* 'Add task' callback and sets the TCB's priority fileds*/
static void heap_addTask(OS_TCB_t * const tcb) {
	heap_insert(&runningHeap, tcb);
}

/* 'Task exit' callback */
static void heap_taskExit(OS_TCB_t * const tcb) {
	/* Set the TCB's priority so that it is never run */
	tcb->priority_affected = SCHEDULER_MAX_PRIORITY + 1;
	int tcb_heap_index = find_tcb_in_heap(&runningHeap, tcb);
	if(tcb_heap_index >= 0) {
		heap_fix(&runningHeap, tcb_heap_index);
	}
}

static void heap_wait(void * const reason, uint32_t check) {
	if(check == checksum()) {
		OS_TCB_t * prtCurTCB = OS_currentTCB();
		/* Save the reason in the TCB's data field*/ 
		prtCurTCB->data = (uint32_t)reason;
		/* Set the wait bit in TCB's status field */
		prtCurTCB->state |= TASK_STATE_WAIT;
		/* Increase the affected priority so it doesnt get checked by sheduler */
		prtCurTCB->priority_affected = SCHEDULER_MAX_PRIORITY + 1;
		/* fix the heap with the TCB's new priority */
		int tcb_heap_index = find_tcb_in_heap(&runningHeap, prtCurTCB);
		if(tcb_heap_index >= 0) {
			heap_fix(&runningHeap, tcb_heap_index);
		}
		SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
	}
}

/* Notify an waiting TCB's of the mutex completion */
static void heap_notify(void * const reason) {
	/* Loop through all TCB's */
	for (int i = 0; i < SCHEDULER_MAX_TASKS; i++) {
		OS_TCB_t * tcb = runningHeap.TCB[i];
		/* Check if the TCB is in a wait state */
		if (tcb->state & TASK_STATE_WAIT) {
			/* Check the reason of TCB is waiting */ 
			if(tcb->data == (uint32_t)reason) {
				/* TCB was waitng for this mutex to finish */ 
				/* Take the TCB out of a wait state */
				tcb->state &= ~TASK_STATE_WAIT;
				/* Reset it's priority */
				tcb->priority_affected = tcb->priority_assigned;
				tcb->data = 0;
				/* Fix heap with the TCB's new priority */
				int tcb_heap_index = find_tcb_in_heap(&runningHeap, tcb);
				if(tcb_heap_index >= 0) {
					heap_fix(&runningHeap, tcb_heap_index);
				}
			}
		}
	}	
}

/* Initailise two working heaps */
void scheduler_init(void) {
	heap_init(&runningHeap, runningHeapStore, HEAP_SORT_PRIORITY_AFFECTED);
	heap_init(&sleepingHeap, sleepingHeapStore, HEAP_SORT_DATA);
}

/* Find th posisition of a TCB in the heap */
int find_tcb_in_heap(heap_t * heap, OS_TCB_t * tcb) {
	for(int index = 0; index < heap->length; index++) {
		if(heap->TCB[index] == tcb) {
			/* TCB located, return index */
			return index;
		}
	}
	/* TCB not found */
	return -1;
}

/* Set a task to a sleeping state */
void OS_sleep(uint32_t addtionalTicks) {
	OS_TCB_t * tcb = OS_currentTCB();
	/* Set the TCB's data field to its wake time */
	tcb->data = OS_elapsedTicks() +	addtionalTicks;
	/* Set the wait bit in status field */
	tcb->state |= TASK_STATE_SLEEP;
	heap_insert(&sleepingHeap, tcb);
	/* Increase the affected priority so it doesnt get checked by sheduler */
	tcb->priority_affected = SCHEDULER_MAX_PRIORITY + 1;
	/* Fix the heap with teh TCB's new priority */
	int tcb_heap_index = find_tcb_in_heap(&runningHeap, tcb);
	if(tcb_heap_index >= 0) {
		heap_fix(&runningHeap, tcb_heap_index);
	}
	OS_yield();
}
