#include "record.h"
static unsigned char sta = 1;//0-Â¼Òô 1-Î´Â¼Òô 2-Í£Ö¹Â¼Òô
stu_system_time stuSystemtime = {4,11,21,11,11,22};
void Record(void)
{

	
	unsigned char order1[16] = {0x7E,0x03,0xC2,0XC5,0x7E};//²éÑ¯µ±Ç°¹¤×÷×´Ì¬
	unsigned char order2[16] = {0x7E, 0x03, 0xD9,0xDC,0x7E};//Í£Ö¹Â¼Òô
	unsigned char order3[20];
	order3[0] = 0x7E;
	order3[1] = 0x0E;
	order3[2] = 0xD8;
	order3[3] = 0x30 + stuSystemtime.year;
	order3[4] = 0x30 + stuSystemtime.mon/10;
	order3[5] = 0x30 + stuSystemtime.mon%10;
	order3[6] = 0x30 + stuSystemtime.day/10;
	order3[7] = 0x30 + stuSystemtime.day%10;
	order3[8] = 0x30 + stuSystemtime.hour/10;
	order3[9] = 0x30 + stuSystemtime.hour%10;
	order3[10] = 0x30 + stuSystemtime.min/10;
	order3[11] = 0x30 + stuSystemtime.min%10;
	order3[12] = 0x30 + stuSystemtime.sec/10;
	order3[13] = 0x30 + stuSystemtime.sec%10;
	order3[14] = 0x07;
	order3[15] = 0x7E;
	

	unsigned char data = 0xFF;
	static unsigned char sta = 1;
	
	if(Key_scan()==Reset)//°´ÏÂ°´¼ü
	{
		
		if(sta != 0)
		{
			Usart_SendStr(M4_USART1,&order3[0]);//½«Â¼ÒôÃüÁî·¢ËÍ¸øÂ¼ÒôÄ£¿é
			sta = 0;
			Led_on();
			data = USART_RecData(M4_USART1);			
			if((data == 0x00) && (sta == 0))
			{
				Usart_SendStr(M4_USART2,"start\r\n........................\r\n");
				
			}else if((data == 0x01) && (sta == 0))
			{
				Usart_SendStr(M4_USART2,"start_full.......................\r\n");
			}else if((data == 0x02) && (sta == 0))
			{
				Usart_SendStr(M4_USART2,"start_error......\r\n");
			}else
			{
				Usart_SendStr(M4_USART2,"´íÎó");
			}
		}
		else
		{
			Usart_SendStr(M4_USART1,&order2[0]);//Í£Ö¹Â¼Òô
			sta = 2;	
			data = USART_RecData(M4_USART1);
			if((data == 0x00)&& (sta == 2))
				Usart_SendStr(M4_USART2,"stop\r\n");
				Led_off();
		}
			
	}

	
}