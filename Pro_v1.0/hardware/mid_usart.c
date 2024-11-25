#include "mid_usart.h"

stu_system_time systime={4,00,00,00,00,00};
//void Mid_UsartConfig(void)
//{
//	
//	QueueEmpty(DebugTxMsg);
//}

//工作状态状态 0-录音 1-未录音  2-停止录音
void Record_Start(void)
{
//	unsigned char order[16];
//	order[0] = 0x7E;//起始码
//	order[1]= 0X0E;//长度
//	order[2]= 0xD8;//功能码
//	order[3] = systime.year;
//	order[4] = systime.mon/10 + 0x30;
//	order[5] = systime.mon%10 + 0x30;
//	order[6] = systime.day/10 + 0x30;
//	order[7] = systime.day%10 + 0x30;
//	order[8] = systime.hour/10 + 0x30;
//	order[9] = systime.hour%10 + 0x30;
//	order[10] = systime.min/10 + 0x30;
//	order[11] = systime.min%10 + 0x30;
//	order[12] = systime.sec/10 + 0x30;
//	order[13] = systime.sec%10 + 0x30;
//	order[14] = 0x4A;//校验码(E6+)
//	order[15] = 0x7E;//结束码
	
	unsigned char order1[16] = {0x7E, 0x07,0xD6, 0x54, 0x30, 0x30, 0x32, 0xC3, 0x7E};//录音
	unsigned char order2[16] = {0x7E, 0x03, 0xD9,0xDC,0x7E};//停止录音
	unsigned char data;
	static unsigned char sta = 1;
	if(Key_scan()==Reset)//按下按键
	{
		if(sta != 0)
		{
			Usart_SendStr(M4_USART2,&order1[0]);//将录音命令发送给录音模块
			sta = 0;
			data = USART_RecData(M4_USART2);
			Usart_SendStr(M4_USART1,"start\r\n");

		}
		else
		{
			Usart_SendStr(M4_USART2,&order2[0]);//停止录音
			sta = 2;	
			data = USART_RecData(M4_USART2);
		}


		if((data == 0x00) && (sta == 0))
			Usart_SendStr(M4_USART1,"start\r\n");

		else if((data == 0x00)&& (sta == 2))
			Usart_SendStr(M4_USART1,"stop\r\n");

		

	}
	
}

	

