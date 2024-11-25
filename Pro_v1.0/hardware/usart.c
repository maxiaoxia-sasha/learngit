#include "usart.h"	 
#include "hc32f460_utility.h"
#include "hc32f460_clk.h"
#include "hc32f460_efm.h"
#include "hc32f460_gpio.h"
#include "hc32f460_exint_nmi_swi.h"
#include "hc32f460_interrupts.h"
#include "hc32f460_pwc.h"
#include "hc32f460_sram.h"
#include "hc32f460_usart.h"
#include "key.h"
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int32_t x) 
{ 
	x = x; 
} 
//重定义fputc函数 
//int fputc(int32_t ch, FILE *f)
//{      
//	//while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
//    //USART1->DR = (u8) ch;      
//	return ch;
//}

int fputc(int ch, FILE *f)
{
    (void)f;  /* Prevent unused argument compilation warning */

    return (Ok == UartPutChar(M4_USART1, (char)ch)) ? ch: -1;
	//return (Ok == UartPutChar(M4_USART2, (char)ch)) ? ch: -1;
}



#endif 

static int32_t m_u32PrintfTimeout;

static en_result_t UartPutChar(M4_USART_TypeDef *USARTx, char cData)
{
    uint32_t u32TxEmpty;
    en_result_t enRet = ErrorInvalidParameter;
    __IO uint32_t u32Timeout = m_u32PrintfTimeout;

    if (NULL != USARTx)
    {
        /* Wait TX data register empty */
        do
        {
            u32Timeout--;
            u32TxEmpty = USARTx->SR_f.TXE;
        } while ((u32Timeout > 0ul) && (0ul == u32TxEmpty));

        if (u32TxEmpty > 0ul)
        {
            USARTx->DR = (uint32_t)cData;
            enRet = Ok;
        }
        else
        {
            enRet = ErrorTimeout;
        }
    }

    return enRet;
}




//寄存器
void Uart_init(uint32_t  u32Baudrate)
{
    en_result_t enRet = ErrorInvalidParameter;
	/* Initialize port */
	PORT_SetFunc(PortA, Pin09, Func_Usart1_Tx, Disable);
	
	/* Enable clock */
	M4_MSTP->FCG1_f.USART1 = 0ul;  //PWR_FCG1 通用同步异步收发器USART1功能无效 
	/* Initialize USART */
	M4_USART1->CR1_f.ML = 0ul;      /* LSB */
	M4_USART1->CR1_f.MS = 0ul;      /* UART mode */
	M4_USART1->CR1_f.OVER8 = 1ul;   /* 8bit sampling mode */
	M4_USART1->CR1_f.M = 0ul;       /* 8 bit data length */
	M4_USART1->CR1_f.PCE = 0ul;     /* no parity bit */
	
	 /* Set baudrate */
    enRet = USART_SetBaudrate(M4_USART1, u32Baudrate);
    if (enRet != Ok)
    {
        while (1)
        {
        }
    }
    else
    {
		M4_USART1->CR2 = 0ul;       /* 1 stop bit, single uart mode */
		M4_USART1->CR3 = 0ul;       /* CTS disable, Smart Card mode disable */
		M4_USART1->CR1_f.TE = 1ul;  /* TX enable */  
		m_u32PrintfTimeout = (SystemCoreClock / u32Baudrate);//200000000/115200
    }
		
}
static void Usart1RxIrqCallback(void)
{

//	uint16_t u16Data = USART_RecData(M4_USART1);
//	USART_SendData(M4_USART2, u16Data);
//	while(USART_GetStatus(M4_USART2, UsartTxEmpty) == Reset );

}

static void Usart1TxIrqCallback(void)
{

    
}

//库
void Usart1_init(uint32_t  u32Baudrate)
{
	en_result_t enRet = Ok;
    stc_irq_regi_conf_t stcIrqRegiCfg;  
	PORT_SetFunc(PortA, Pin09, Func_Usart1_Tx, Disable);//华大的单片机几乎所有通用输入输出IO都可配置为串口	
	PORT_SetFunc(PortA, Pin10, Func_Usart1_Rx, Disable);	
	
    const stc_usart_uart_init_t stcInitCfg = {
        UsartIntClkCkNoOutput,//Select internal clock source and don't output clock.
        UsartClkDiv_64,// PCLK/1 200M主频 波特率小于57600时，可采用UsartClkDiv_64
        UsartDataBits8,///< 8 Bit
        UsartDataLsbFirst,///< LSB first
        UsartOneStopBit,///< 1 Stop Bit
        UsartParityNone,///< No parity bit is used.
        UsartSampleBit8,///< 8 Bit
        UsartStartBitFallEdge,///< Start bit: RD pin falling edge
        UsartRtsEnable,///< Enable RTS function.
    };
	
	//stcInitCfg.enClkDiv=(u32Baudrate < 115200) ? UsartClkDiv_64 : UsartClkDiv_1;
	 	
	/* Enable peripheral clock */
	PWC_Fcg1PeriphClockCmd(PWC_FCG1_PERIPH_USART1, Enable);

    /* Initialize UART */
    enRet = USART_UART_Init(M4_USART1, &stcInitCfg);
    if (enRet != Ok)
    {
        while (1)
        {
        }
    }
    else
    {
    }

    /* Set baudrate */
    enRet = USART_SetBaudrate(M4_USART1, u32Baudrate);
    if (enRet != Ok)
    {
        while (1)
        {
        }
    }
    else
    {		
		
		m_u32PrintfTimeout = (SystemCoreClock / u32Baudrate);		
    }
	
	 /* Set USART RX IRQ */
    stcIrqRegiCfg.enIRQn = Int001_IRQn;
    stcIrqRegiCfg.pfnCallback = &Usart1RxIrqCallback;
    stcIrqRegiCfg.enIntSrc = INT_USART1_RI;
    enIrqRegistration(&stcIrqRegiCfg);
    NVIC_SetPriority(stcIrqRegiCfg.enIRQn, DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_ClearPendingIRQ(stcIrqRegiCfg.enIRQn);
    NVIC_EnableIRQ(stcIrqRegiCfg.enIRQn);
	
	    /* Set USART TX IRQ */
    stcIrqRegiCfg.enIRQn = Int004_IRQn;
    stcIrqRegiCfg.pfnCallback = &Usart1TxIrqCallback;
    stcIrqRegiCfg.enIntSrc = INT_USART1_TI;
    enIrqRegistration(&stcIrqRegiCfg);
    NVIC_SetPriority(stcIrqRegiCfg.enIRQn, DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_ClearPendingIRQ(stcIrqRegiCfg.enIRQn);
    NVIC_EnableIRQ(stcIrqRegiCfg.enIRQn);
	
	/*
	USART RX error IRQ 
	USART TX complete IRQ
		参考华大官方例程
	*/
	USART_FuncCmd(M4_USART1, UsartTx, Enable);
	USART_FuncCmd(M4_USART1, UsartRx, Enable);
	USART_FuncCmd(M4_USART1, UsartRxInt, Enable);

}
void Usart_SentByte(M4_USART_TypeDef* pUSARTx, unsigned char data)
{/* 发送一个字节 */
	USART_SendData(pUSARTx,data);//发送data给USART
	//等待发送寄存器为空
	while(USART_GetStatus(pUSARTx, UsartTxEmpty) == Reset );
}
void Usart_SendStr(M4_USART_TypeDef* pUSARTx, unsigned char *str)
{/* 发送字符串 */
	unsigned char i = 0;
	do
	{
		Usart_SentByte(pUSARTx, *(str+i));
		i++;
	}while(*(str+i) != '\0');
	//多字节传输完成
	while(USART_GetStatus(pUSARTx, UsartTxComplete) == Reset);
}

static void Usart2RxIrqCallback(void)
{

}

static void Usart2TxIrqCallback(void)
{

    
}



//
void Usart2_init(uint32_t  u32Baudrate)
{
	en_result_t enRet = Ok;
    stc_irq_regi_conf_t stcIrqRegiCfg;  
	PORT_SetFunc(PortA, Pin00, Func_Usart2_Tx, Disable);//华大的单片机几乎所有通用输入输出IO都可配置为串口	
	PORT_SetFunc(PortA, Pin01, Func_Usart2_Rx, Disable);	
	
    const stc_usart_uart_init_t stcInitCfg = {
        UsartIntClkCkNoOutput,//Select internal clock source and don't output clock.
        UsartClkDiv_64,// PCLK/1 200M主频 波特率小于57600时，可采用UsartClkDiv_64
        UsartDataBits8,///< 8 Bit
        UsartDataLsbFirst,///< LSB first
        UsartOneStopBit,///< 1 Stop Bit
        UsartParityNone,///< No parity bit is used.
        UsartSampleBit8,///< 8 Bit
        UsartStartBitFallEdge,///< Start bit: RD pin falling edge
        UsartRtsEnable,///< Enable RTS function.
    };
	
	//stcInitCfg.enClkDiv=(u32Baudrate < 115200) ? UsartClkDiv_64 : UsartClkDiv_1;
	 	
	/* Enable peripheral clock */
	PWC_Fcg1PeriphClockCmd(PWC_FCG1_PERIPH_USART2, Enable);

    /* Initialize UART */
    enRet = USART_UART_Init(M4_USART2, &stcInitCfg);
    if (enRet != Ok)
    {
        while (1)
        {
        }
    }
    else
    {
    }

    /* Set baudrate */
    enRet = USART_SetBaudrate(M4_USART2, u32Baudrate);
    if (enRet != Ok)
    {
        while (1)
        {
        }
    }
    else
    {		
		
		m_u32PrintfTimeout = (SystemCoreClock / u32Baudrate);		
    }
	
	 /* Set USART RX IRQ */
    stcIrqRegiCfg.enIRQn = Int000_IRQn;
    stcIrqRegiCfg.pfnCallback = &Usart2RxIrqCallback;
    stcIrqRegiCfg.enIntSrc = INT_USART2_RI;
    enIrqRegistration(&stcIrqRegiCfg);
    NVIC_SetPriority(stcIrqRegiCfg.enIRQn, DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_ClearPendingIRQ(stcIrqRegiCfg.enIRQn);
    NVIC_EnableIRQ(stcIrqRegiCfg.enIRQn);
	
	    /* Set USART TX IRQ */
    stcIrqRegiCfg.enIRQn = Int002_IRQn;
    stcIrqRegiCfg.pfnCallback = &Usart2TxIrqCallback;
    stcIrqRegiCfg.enIntSrc = INT_USART2_TI;
    enIrqRegistration(&stcIrqRegiCfg);
    NVIC_SetPriority(stcIrqRegiCfg.enIRQn, DDL_IRQ_PRIORITY_DEFAULT);
    NVIC_ClearPendingIRQ(stcIrqRegiCfg.enIRQn);
    NVIC_EnableIRQ(stcIrqRegiCfg.enIRQn);
	
	/*
	USART RX error IRQ 
	USART TX complete IRQ
		参考华大官方例程
	*/
	USART_FuncCmd(M4_USART2, UsartTx, Enable);
	USART_FuncCmd(M4_USART2, UsartRx, Enable);
	USART_FuncCmd(M4_USART2, UsartRxInt, Enable);
	
}


