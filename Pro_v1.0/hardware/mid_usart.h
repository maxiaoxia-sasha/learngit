#ifndef __MID_USART_H
#define __MID_USART_H

#include "hc32_ddl.h"
#include "usart.h"
#include "key.h"


typedef struct SYSTEM_TIME
{//ϵͳʱ��ṹ��
        unsigned short year;	//���
        unsigned char mon;    	//�·�
        unsigned char day;   	//��
        unsigned char hour;    	//Сʱ
        unsigned char min;    	//����
        unsigned char sec;      //��(Ԥ��)
}

void Record_Start(void);


#endif
