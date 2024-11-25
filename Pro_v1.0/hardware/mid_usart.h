#ifndef __MID_USART_H
#define __MID_USART_H

#include "hc32_ddl.h"
#include "usart.h"
#include "key.h"


typedef struct SYSTEM_TIME
{//系统时间结构体
        unsigned short year;	//年份
        unsigned char mon;    	//月份
        unsigned char day;   	//日
        unsigned char hour;    	//小时
        unsigned char min;    	//分钟
        unsigned char sec;      //秒(预留)
}

void Record_Start(void);


#endif
