#ifndef __RECORD_H
#define __RECORD_H

#include "hc32f460_utility.h"
#include "hc32f460_clk.h"
#include "hc32f460_efm.h"
#include "hc32f460_gpio.h"
#include "hc32f460_exint_nmi_swi.h"
#include "hc32f460_interrupts.h"
#include "hc32f460_pwc.h"
#include "hc32f460_sram.h"
#include "hc32_common.h"
#include "key.h"
#include "led.h"
#include "usart.h"
typedef struct SYSTEM_TIME
{//系统时间结构体
        unsigned short year;	//年份
        unsigned char mon;    	//月份
        unsigned char day;   	//日
        unsigned char hour;    	//小时
        unsigned char min;    	//分钟
        unsigned char sec;      //秒(预留)
}stu_system_time;


void Record(void);
#endif