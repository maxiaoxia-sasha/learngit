#include "key.h"

#include "hc32f460_utility.h"
#include "hc32f460_clk.h"
#include "hc32f460_efm.h"
#include "hc32f460_gpio.h"
#include "hc32f460_exint_nmi_swi.h"
#include "hc32f460_interrupts.h"
#include "hc32f460_pwc.h"
#include "hc32f460_sram.h"



void Key_init(void)
{
	stc_port_init_t stcPortInit;   
    MEM_ZERO_STRUCT(stcPortInit);
    stcPortInit.enPinMode = Pin_Mode_In;
	stcPortInit.enPullUp = Enable;		
    /* Initialize LED pin */	
	PORT_Init(KEY1_PORT, KEY1_PIN, &stcPortInit);
	
}


en_flag_status_t  Key_scan(void)
{
	en_flag_status_t key_status;
    static 	en_flag_status_t key_status_last=Set;
	key_status=PORT_GetBit(KEY1_PORT , KEY1_PIN);
		
	if ((key_status==Set)&&(key_status_last==Reset))
	{		
		key_status_last=key_status;
		return Reset;
	}else{
		key_status_last=key_status;
		return Set;	
	}
	
}


/*
	if (key_status==Reset){
		Ddl_Delay1ms(10);
		if (PORT_GetBit(KEY1_PORT , KEY1_PIN)==Reset)
			return Reset;//°´ÏÂ		
	}else{
		return Set;		
	}

*/

