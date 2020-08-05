#include "simpleRoundRobin.h"
#include "heap.h"
#include "mutex.h"
#include "os.h"
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
static void roundRobin_wait(OS_mutex_t * mutex, uint32_t checkSum);
static void simpleRoundRobin_notify(OS_TCB_t * const tcb);

static OS_TCB_t * sleepHeapExtract(void);

/* Heap */
static heap_t runningHeap;
static heap_t sleepHeap;
static OS_TCB_t * runningStore[SIMPLE_RR_MAX_TASKS];
static OS_TCB_t * sleepStore[SIMPLE_RR_MAX_TASKS];

/* Scheduler block for the simple round-robin */
OS_Scheduler_t const simpleRoundRobinScheduler = {
	.preemptive = 1,
	.scheduler_callback = simpleRoundRobin_scheduler,
	.addtask_callback = simpleRoundRobin_addTask,
	.taskexit_callback = simpleRoundRobin_taskExit,
	.wait_callback = roundRobin_wait,
	.notify_callback = simpleRoundRobin_notify
};

/* scheduler*/
void scheduler_init(void ) {
	heap_init(&runningHeap, runningStore);
	heap_init(&sleepHeap, sleepStore);
}

/* Round-robin scheduler callback */
static OS_TCB_t const * simpleRoundRobin_scheduler(void) {
	// Clear the yield flag if it's set - we simply don't care
	OS_currentTCB()->state &= ~TASK_STATE_YIELD;
	OS_TCB_t * tcb = OS_currentTCB();
	
	/* Loop while tasks are in running or sleep heap */
	while(runningHeap.length || sleepHeap.length) {
		/* Check sleep heap */
		while(sleepHeap.length){
			/* Check is enough time has passed for the top of the heap */
			uint32_t tick = OS_elapsedTicks();
			if(sleepHeap.TCB[0]->priority < tick){
				/* Extract sleeping task, as enough time as pasted */
				OS_TCB_t * woken_tcb = heap_extract(&sleepHeap);
				/* Swop the priority and data fields, as heap is arranged in priority order */
				uint32_t priority = tcb->priority;
				woken_tcb->priority = tcb->data;
				woken_tcb->data = priority;
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
		if(runningHeap.length){
			if(!(runningHeap.TCB[0]->state & TASK_STATE_SLEEP)) {
				runningHeap.TCB[0]->priority = 0;
				return runningHeap.TCB[0];
			} else {
				/* Extract the task as in a sleep state*/
				OS_TCB_t * sleep_TCB = heap_extract(&runningHeap);
				/* Swop the priority and data fields, as heap is arranged in priority order */
				uint32_t priority = tcb->priority;
				tcb->priority = tcb->data;
				tcb->data = priority;
				/* Store in the sleeping Heap*/
				heap_insert(&sleepHeap, tcb);
			}
		}
	}
	
	// No tasks in the list, so return the idle task
	return OS_idleTCB_p;
}

/* 'Add task' callback */
static void simpleRoundRobin_addTask(OS_TCB_t * const tcb) {
	if(SIMPLE_RR_MAX_TASKS > runningHeap.length) {
		heap_insert(&runningHeap, tcb);
		return;
	}
	// If we get here, there are no free TCB slots, so the task just won't be added
}

/* 'Task exit' callback */
static void simpleRoundRobin_taskExit(OS_TCB_t * const tcb) {
	// Remove the given TCB from the list of tasks so it won't be run again
	heap_extract(&runningHeap);
}

/* 'wait' callback 
		Set the TCB to a waiting state and set the reason to the address of the mutex it is waiting for.*/
static void roundRobin_wait(OS_mutex_t * const mutex, uint32_t const checkSum) {
	/* Check if an ISR has happend been calling this function and completeing it. */
	if(checkSum == getCheckSum()) {
		
		OS_TCB_t *cur_TCB = OS_currentTCB();
		cur_TCB->state |= TASK_STATE_WAIT;
		
		uint32_t address = 0;
		OS_TCB_t * tcb = 0;
		/* Check mutex waitng pointer */
		if(!(mutex->prt_waiting_TCB)) {
			/* Not set, so set it as head of list */
			//tcb = (mutex->prt_waiting_TCB);
			mutex->prt_waiting_TCB = cur_TCB;
		} else {
			tcb = mutex->prt_waiting_TCB;
			/* loop to end of list */
			while(!(tcb->prt_TCB)) {
				tcb = tcb->prt_TCB;
			}
			mutex->prt_waiting_TCB = tcb;
			/* Add to end of the list */
			
		}
		
		/*
		uint32_t store = 1;		
		while(store != 0) {
			uint32_t task_state =  __LDREXW(&tcb);
			if(!(task_state & TASK_STATE_WAIT)) {		
				store = __STREXW((uint32_t)(cur_TCB), &tcb);
			}
		}*/
		
		
		SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
	}
}

/* 'notify' callback 		
		Check all elemenets in the RR array if they are waiting to the mutex that as just been relases*/
static void simpleRoundRobin_notify(OS_TCB_t * const tcb){
	__CLREX();
	if(tcb != 0) {
		OS_TCB_t * prt_tcb = tcb->prt_TCB;
		prt_tcb->state &= TASK_STATE_WAIT;
	}
}

/* Heaps  designed for TCB are store with the highest priority at the top of the heap, therefore
	 for the sleeping heap the TCB's data and priority fields are temperarly swapped. This will then
	 store the tcb that needs to wake up at the top of the heap. These fields are then set back to 
	 once the extracted from the heap*/
 void sleepHeapInsert(OS_TCB_t * tcb) {
	if(tcb == runningHeap.TCB[0]) {
		heap_extract(&runningHeap);
	}
	uint32_t priority = tcb->priority;
	tcb->priority = tcb->data;
	tcb->data = priority;
	heap_insert(&sleepHeap, tcb);
}

/* Heaps  designed for TCB with the highest priority to be stored at the top of the heap, therefore
	 for the sleep heap the TCB's data and priority fields are temperarly swapped.*/
static OS_TCB_t * sleepHeapExtract(void) {
	OS_TCB_t * tcb = heap_extract(&sleepHeap);
	uint32_t priority = tcb->priority;
	tcb->priority = tcb->data;
	tcb->data = priority;
	return tcb;
}
