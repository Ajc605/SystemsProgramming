#include "priority.h"
#include "heap.h"
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
static OS_TCB_t const * priority_scheduler(void);
static void priority_addTask(OS_TCB_t * const tcb);
static void priority_taskExit(OS_TCB_t * const tcb);
static void priority_wait(uint32_t checkSum);
static void priority_notify(void);
static int waitingTaskFIFOPut(OS_TCB_t * const tcb);
static int waitingTaskFIFOGet(OS_TCB_t * const tcb);

/* values for waiting task FIFO */
static OS_TCB_t * waitingTaskFIFO[PRIORITY_MAX_TASKS] = {0};
static int insert = 0;
static int retrieve = 0;
static int status = 0; 

/* Heap values */
static heap_t runningHeap;
static heap_t sleepHeap;
static OS_TCB_t * store[PRIORITY_MAX_TASKS];

/* Scheduler block for the priority */
OS_Scheduler_t const priorityScheduler = {
	.preemptive = 1,
	.scheduler_callback = priority_scheduler,
	.addtask_callback = priority_addTask,
	.taskexit_callback = priority_taskExit,
	.wait_callback = priority_wait,
	.notify_callback = priority_notify
};

/* scheduler*/
void scheduler_init(void ) {
	heap_init(&runningHeap, store);
	heap_init(&sleepHeap, store);
}

/* Round-robin scheduler callback */
static OS_TCB_t const * priority_scheduler(void) {
	/* Check sleeping heap */
	while(sleepHeap.length){
		/* Check is enough time has passed for the top of the heap */
		if(sleepHeap.TCB[0]->data < OS_elapsedTicks()){
			OS_TCB_t * woken_tcb = heap_extract(&sleepHeap);
			woken_tcb->state &= ~TASK_STATE_SLEEP;
			woken_tcb->data = 0;
			heap_insert(&runningHeap, woken_tcb);
		} else {
			break;
		}
	}
		
	OS_TCB_t * curr_tcb = OS_currentTCB();
	curr_tcb->state &= TASK_STATE_YIELD;
	
//	if(curr_tcb->priority > runningHeap.TCB[0]->priority) {
	if(runningHeap.length) {
		return heap_extract(&runningHeap);
	}
	
	
	// No tasks in the list, so return the idle task
	return OS_idleTCB_p;
}

/* 'Add task' callback */
static void priority_addTask(OS_TCB_t * const tcb) {
	if(PRIORITY_MAX_TASKS > runningHeap.length) {
		heap_insert(&runningHeap, tcb);
	}
}

/* 'Task exit' callback */
static void priority_taskExit(OS_TCB_t * const tcb) {
	// Remove the given TCB from the list of tasks so it won't be run again
	printf("TaskExit callback");
}

/* 'wait' callback 
		Set the TCB to a waiting state and set the reason to the address of the mutex it is waiting for.*/
static void priority_wait(uint32_t checkSum) {
	/* Check if an ISR has happend been calling this function and completeing it. */
	if(checkSum == getCheckSum()) {
		OS_TCB_t *cur_TCB = OS_currentTCB();
		
		uint32_t task_state =  __LDREXW(&(cur_TCB->state));
		if(!(task_state & TASK_STATE_WAIT)) {
			uint32_t store = 1;
			while(store != 0) {
				store = __STREXW((uint32_t)(task_state |= TASK_STATE_WAIT), &(cur_TCB->state));
			}
		}
		
		while(1) {
			if(waitingTaskFIFOPut(cur_TCB)){
				break;
			}
		}
			
		SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
	}
}

/* 'notify' callback 		
		Check all elemenets in the RR array if they are waiting to the mutex that as just been relases*/
static void priority_notify(){
	__CLREX();
	OS_TCB_t * prt;
	if(waitingTaskFIFOGet(prt)) {
		prt->state &= ~TASK_STATE_WAIT;
	}
}

/*
	This function tries to put the tcb into the waiting FIFO.
	First this function will check if the FIFO is full by comparing the insert and retrieve vlaues.
	If the FIFO is full the tcb will not be stored and a '0' is return to indicate the store has failed.
	If the FIFO is not full then the tcb is stored and the insert is increased, '1' is returned to indicate this was successful
	
*/
static int waitingTaskFIFOPut(OS_TCB_t * const tcb) {
	/* Set the status value */
	status = 0; 
	/* Check if the FIFO is full */
	if((insert+1)%PRIORITY_MAX_TASKS != retrieve) {
		waitingTaskFIFO[insert] = tcb; 
		/* insert value is increaed and mod by PRIORITY_MAX_TASKS (to stop it going past the value of PRIORITY_MAX_TASKS) */
		insert = (insert+1)%PRIORITY_MAX_TASKS; 
		/* Update the status to indicate the success of storing the tcb to the waiting FIFO */
		status = 1;
	}else {
		//printf("Full %d\n\r", tcb);
		//printf("Insert: %d, Retrieve: %d\n\r", insert, retrieve);
	}
	return status;
}

/*
	This function tries to set the passed pointer to tcb to point to the next waiting task.
	If the FIFO is empty then '0' is returned to indacte the get has failed.
	If the FIFO is not empty then the passed pointer is set to point to the next waiting task, 
	and '1' is returned to indacte this was successful
	
*/
static int waitingTaskFIFOGet(OS_TCB_t * const tcb) {
	int status = 0;
	/* Check if the FIFO is empty */
	if(insert != retrieve) {
		/* Store the releast tcb in pointer passed to function*/
		*tcb = *waitingTaskFIFO[retrieve]; 
		/* Retrieve value is increaed and mod by PRIORITY_MAX_TASKS (to stop it going past PRIORITY_MAX_TASKS)*/
		retrieve = (retrieve+1)%PRIORITY_MAX_TASKS; 
		status = 1;
	} else {
		printf("Empty\n\r");
	}
	return status;
}
