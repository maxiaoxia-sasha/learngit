#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "hc32_common.h"

static en_result_t UartPutChar(M4_USART_TypeDef *USARTx, char cData);
void Uart_init(uint32_t  u32Baudrate);
void Usart1_init(uint32_t  u32Baudrate);
void Usart2_init(uint32_t  u32Baudrate);
void Usart_SentByte(M4_USART_TypeDef* pUSARTx, unsigned char data);
void Usart_SendStr(M4_USART_TypeDef* pUSARTx, unsigned char *str);
//void Record_Start(void);
#endif


