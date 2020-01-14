#ifndef __heapScheduler_h__
#define __heapScheduler_h__

#include "os.h"

// How many tasks can this scheduler cope with?
#define SCHEDULER_MAX_TASKS 8

#define SCHEDULER_MAX_PRIORITY 10

extern OS_Scheduler_t const heapScheduler;

void scheduler_init(void);
void OS_sleep(uint32_t addtionalTicks);

#endif /* __heapScheduler_h__ */
