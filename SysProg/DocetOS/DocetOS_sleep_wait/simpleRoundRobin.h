#ifndef __simpleRoundRobin_h__
#define __simpleRoundRobin_h__

#include "os.h"

// How many tasks can this scheduler cope with?
#define SIMPLE_RR_MAX_TASKS 8

extern OS_Scheduler_t const simpleRoundRobinScheduler;

void sleepHeapInsert(OS_TCB_t * tcb);

#endif /* __simpleRoundRobin_h__ */
