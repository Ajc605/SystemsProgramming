#ifndef __priority_h__
#define __priority_h__

#include "os.h"

// How many tasks can this scheduler cope with?
#define PRIORITY_MAX_TASKS 8

extern OS_Scheduler_t const priorityScheduler;


#endif /* __priority_h__ */
