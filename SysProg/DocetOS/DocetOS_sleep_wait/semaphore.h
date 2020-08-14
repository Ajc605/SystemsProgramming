#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_
#include "OS_internal.h"
#include "stdint.h"
#include "mempool.h"

typedef struct {
	uint32_t counter;
	OS_TCB_t * prt_waiting_TCB;
} OS_semaphore_t;

void init_semaphore (OS_semaphore_t * semaphore);

void OS_semaphore_acquire(OS_semaphore_t * semaphore);

void OS_semaphore_release(OS_semaphore_t * semaphore);

#endif
