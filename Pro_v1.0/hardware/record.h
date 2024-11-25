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
{//ϵͳʱ��ṹ��
        unsigned short year;	//���
        unsigned char mon;    	//�·�
        unsigned char day;   	//��
        unsigned char hour;    	//Сʱ
        unsigned char min;    	//����
        unsigned char sec;      //��(Ԥ��)
}stu_system_time;


void Record(void);
#endif