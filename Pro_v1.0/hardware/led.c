#include "led.h"

#include "hc32f460_utility.h"
#include "hc32f460_clk.h"
#include "hc32f460_efm.h"
#include "hc32f460_gpio.h"
#include "hc32f460_exint_nmi_swi.h"
#include "hc32f460_interrupts.h"
#include "hc32f460_pwc.h"
#include "hc32f460_sram.h"

void Led_init(void)
{
    stc_port_init_t stcPortInit;   
    MEM_ZERO_STRUCT(stcPortInit);
    stcPortInit.enPinMode = Pin_Mode_Out;
	stcPortInit.enPullUp = Enable;		
    /* Initialize LED pin */	
	PORT_Init(LED1_PORT, LED1_PIN, &stcPortInit);
	Led_off();
}


void Led_on(void)
{
	PORT_ResetBits(LED1_PORT, LED1_PIN);	
}


void Led_off(void)
{	
	PORT_SetBits(LED1_PORT, LED1_PIN);		
}


void Led_toggle(void)
{	
	 PORT_Toggle(LED1_PORT, LED1_PIN);	
}