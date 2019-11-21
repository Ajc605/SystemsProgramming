#include <stdint.h>
#include "os.h"
#ifndef UTILS_H
#define UTILS_H

uint32_t getPSR(void);

uint32_t getCONTROL(void);

void reportState(void); 

void _svc_report_state(void);

#endif /* UTILS_H */
