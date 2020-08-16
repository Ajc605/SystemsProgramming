#include "priority.h"
#include "heap.h"
#include "mutex.h"
#include "os.h"
#include <stdio.h>
#include <stm32f4xx.h>

/* This is an implementation of an pirority scheduler.

	When tasks are added they are put into a pirority heap, this will store them in priority
	order with the highest priority at the top. When tasks are finished they are removed
	from the heap and the next priority task is ran. If the heaps is empty, a pointer to the 
	idle task is returned instead.
*/

/* Prototypes (functions are static, so these aren't in the header file) */
static OS_TCB_t const * priority_scheduler(void);
static void priority_addTask(OS_TCB_t * const tcb);
static void priority_taskExit(OS_TCB_t * const tcb);
static void priority_wait(uint32_t checkSum);
static void priority_notify(OS_TCB_t * const tcb);

/* Heap */
static heap_t runningHeap;
static heap_t sleepHeap;
static OS_TCB_t * runningStore[PRIORITY_MAX_TASKS];
static OS_TCB_t * sleepStore[PRIORITY_MAX_TASKS];

/* Scheduler block for the priority */
OS_Scheduler_t const priorityScheduler = {
	.preemptive = 1,
	.scheduler_callback = priority_scheduler,
	.addtask_callback = priority_addTask,
	.taskexit_callback = priority_taskExit,
	.wait_callback = priority_wait,
	.notify_callback = priority_notify
};

/* scheduler */
void OS_scheduler_init(void ) {
	heap_init(&runningHeap, runningStore);
	heap_init(&sleepHeap, sleepStore);
}

/* Priority scheduler callback */
/* This is a priority scheduler that will run the TCB with the highest priority first.
First it checks if any task exists is't in either running or sleep heap. Then it will check
if there is anything in the sleep heap specifically. If so the top element is checked
if enough time has passed, if so then the task is taken out of the heap, the data is 
set to zero and priority is set back. This is done as the priority was overwritten when 
the TCB was added to the sleep heap. The sleep bit is also cleared and the TCB is added 
back to the running heap. Then the checks are complete for the new top element. Once this
is complete a check is done on the running heap to see if any TCB exists there. If so 
then the sleep bit of the top element is checked. If that is set then the TCB's priority
is set to the data, this will store the sleep elements in time to wake order. This is then 
added to the sleep heap. If the bit is not set, then the priority is set to '0'. This will 
fix the TCB to the top of the heap. It is then returned.
*/
static OS_TCB_t const * priority_scheduler(void) {
	/* Clear the yield flag if it's set - we simply don't care */
	OS_currentTCB()->state &= ~TASK_STATE_YIELD;
	OS_TCB_t * tcb = OS_currentTCB();
	
	/* Loop while tasks are in running or sleep heap */
	while(runningHeap.length || sleepHeap.length) {
		/* Check sleep heap */
		while(sleepHeap.length){
			/* Check is enough time has passed for the top of the heap */
			uint32_t tick = OS_elapsedTicks();
			if(sleepHeap.TCB[0]->priority < tick){
				/* Extract sleeping task, as enough time as passed */
				OS_TCB_t * woken_tcb = heap_extract(&sleepHeap);
				/* Clare the sleep bit */
				woken_tcb->state &= ~TASK_STATE_SLEEP;
				/* Clear the data field */
				woken_tcb->data = 0;
				/* Place back into the running heap */
				woken_tcb->priority = woken_tcb->fixed_priority;
				heap_insert(&runningHeap, woken_tcb);	
			} else {
				break;
			}
		}
		/* Check if there are any tasks in the running heap */
		if(runningHeap.length){
			if(!(runningHeap.TCB[0]->state & TASK_STATE_SLEEP)) {
				runningHeap.TCB[0]->priority = 0;
				return runningHeap.TCB[0];
			} else {
				/* Extract the task as in a sleep state */
				OS_TCB_t * sleep_TCB = heap_extract(&runningHeap);
				/* Swop the priority and data fields, as heap is arranged in priority order */
				tcb->priority = tcb->data;
				/* Store in the sleeping heap */
				heap_insert(&sleepHeap, tcb);
			}
		}
	}
	
	/* No tasks in the list, so return the idle task */
	return OS_idleTCB_p;
}

/* 'Add task' callback */
static void priority_addTask(OS_TCB_t * const tcb) {
	if(PRIORITY_MAX_TASKS > runningHeap.length) {
		heap_insert(&runningHeap, tcb);
		return;
	}
	// If we get here, there are no free TCB slots, so the task just won't be added
}

/* 'Task exit' callback */
static void priority_taskExit(OS_TCB_t * const tcb) {
	// Remove the given TCB from the list of tasks so it won't be run again
	heap_extract(&runningHeap);
}

/* 'wait' callback 
This function takes a pointer to a OS_TCB_t struct and a uint32_t checkSum.
The check sum is used to see if an ISR has been called, if so this task my not need to wait and should
check again. This function will add the current waiting task to a linked list. The head of this list is
the passed TCB to this function. This function will function will loop through the list until it reaches 
the end
*/
static void priority_wait(uint32_t const checkSum) {
	/* Check if an ISR has happend been calling this function and completeing it. */
	if(checkSum == getCheckSum()) {
		
		OS_TCB_t *cur_TCB = OS_currentTCB();
		cur_TCB->state |= TASK_STATE_WAIT;
		
	}
		SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
}

/* 'notify' callback 		
Check all elemenets in the RR array if they are waiting to the mutex that as just been relases 
*/
static void priority_notify(OS_TCB_t * const tcb){
	__CLREX();
	if(tcb != 0) {
		OS_TCB_t * prt_tcb = tcb->prt_TCB;
		prt_tcb->state &= TASK_STATE_WAIT;
	}
}


