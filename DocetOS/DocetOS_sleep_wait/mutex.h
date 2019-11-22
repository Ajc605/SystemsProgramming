#include "task.h"

typedef struct {
	OS_TCB_t * prt_TBC;
	uint32_t counter;
} OS_mutex_t;

void ini_mutex(OS_mutex_t * mutex);

