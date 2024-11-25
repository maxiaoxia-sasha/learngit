#ifndef __KEY__H
#define __KEY__H

#include "hc32_ddl.h"


#define KEY1_PORT       (PortB)
#define KEY1_PIN        (Pin15)


void Key_init(void);

en_flag_status_t  Key_scan(void);
#endif