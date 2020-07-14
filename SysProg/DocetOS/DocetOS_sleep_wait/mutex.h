#ifndef _MUTEX_H_
#define _MUTEX_H_
#include "OS_internal.h"
#include "stdint.h"

typedef struct {
		OS_TCB_t * prt_TCB;
		uint32_t counter;
} OS_mutex_t;

void init_mutex (OS_mutex_t * mutex);

void OS_mutex_acquire(OS_mutex_t * mutex);

void OS_mutex_release(OS_mutex_t * mutex);

#endif
