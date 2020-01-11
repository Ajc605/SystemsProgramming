#ifndef MUTEX_H
#include "os_internal.h"
#include "stdint.h"
#define MUTEX_H

typedef struct {
	OS_TCB_t * prtTCB;
	uint32_t counter;
} OS_mutex_t;

void initialiseMutex(OS_mutex_t * prtMutex);

void OS_mutex_acquire(OS_mutex_t * prtMutex);

void OS_mutex_release(OS_mutex_t * prtMutex);

#endif /* MUTEX_H */