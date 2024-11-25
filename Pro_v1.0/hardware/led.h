#ifndef __LED_H
#define __LED_H	 
#include "hc32_common.h"

#define LED1_PORT       (PortB)
#define LED1_PIN        (Pin14)


void Led_init(void);//≥ı ºªØ
void Led_on(void);
void Led_off(void);
void Led_toggle(void);
		 				    
#endif
