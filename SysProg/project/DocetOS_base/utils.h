#include <stdint.h>
#include "os.h"
#ifndef UTILS_H
#define UTILS_H

uint32_t getPSR(void);

uint32_t getCONTROL(void);

void  reportState(void);

//uint32_t _svc_report_state(_OS_SVC_StackFrame_t const * const stack

#endif /* UTILS_H */
