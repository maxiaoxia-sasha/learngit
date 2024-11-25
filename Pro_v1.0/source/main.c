#include "hc32_ddl.h"
#include "hc32_common.h"
#include "hc32f460_utility.h"
#include "hc32f460_clk.h"
#include "hc32f460_efm.h"
#include "hc32f460_gpio.h"
#include "hc32f460_exint_nmi_swi.h"
#include "hc32f460_interrupts.h"
#include "hc32f460_pwc.h"
#include "hc32f460_sram.h"

#include "led.h"
#include "usart.h"

#include "key.h"

/*

 需要使用的库在ddl_config.h文件中开启

 华大的库，个人觉得不适合初学者,修改printf
*/
typedef struct SYSTEM_TIME
{//系统时间结构体
        unsigned short year;	//年份
        unsigned char mon;    	//月份
        unsigned char day;   	//日
        unsigned char hour;    	//小时
        unsigned char min;    	//分钟
        unsigned char sec;      //秒(预留)
}stu_system_time;

static unsigned char sta = 1;//0-录音 1-未录音 2-停止录音
stu_system_time stuSystemtime = {4,11,21,11,00,01};
static void CLK_Init(void);


int32_t main(void)
{
	unsigned char order1[16] = {0x7E,0x03,0xC2,0XC5,0x7E};//查询当前工作状态
	unsigned char order2[16] = {0x7E, 0x03, 0xD9,0xDC,0x7E};//停止录音
	unsigned char order3[20];
	order3[0] = 0x7E;
	order3[1] = 0x0E;//长度码
	order3[2] = 0xD8;//功能码
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
	order3[14] = 0x02;//校验码必须改
	order3[15] = 0x7E;
	

	unsigned char data = 0xFF;
	static unsigned char sta = 1;
	CLK_Init();
	Led_init();
	Key_init();
	Usart1_init(9600);
	Usart2_init(9600);
	
	Usart_SendStr(M4_USART2,"按下按键开始录音\r\n................\r\n");
    while(1)
    {
//		
		Ddl_Delay1ms(20);
			
	
		if(Key_scan()==Reset)//按下按键
		{
			
			if(sta != 0)
			{
				Usart_SendStr(M4_USART1,&order3[0]);//将录音命令发送给录音模块
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
					Usart_SendStr(M4_USART2,"错误");
				}
			}
			else
			{
				Usart_SendStr(M4_USART1,&order2[0]);//停止录音
				sta = 2;	
				data = USART_RecData(M4_USART1);
				if((data == 0x00)&& (sta == 2))
					Usart_SendStr(M4_USART2,"stop\r\n");
					Led_off();
			}
				
		}
    };
}



/**
 * @brief  BSP clock initialize.
 *         Set board system clock to MPLL@200MHz
 * @param  None
 * @retval None
 */

static void CLK_Init(void)
{
    stc_clk_sysclk_cfg_t    stcSysClkCfg;
    stc_clk_xtal_cfg_t      stcXtalCfg;
    stc_clk_mpll_cfg_t      stcMpllCfg;
    stc_sram_config_t       stcSramConfig;

    MEM_ZERO_STRUCT(stcSysClkCfg);
    MEM_ZERO_STRUCT(stcXtalCfg);
    MEM_ZERO_STRUCT(stcMpllCfg);
    MEM_ZERO_STRUCT(stcSramConfig);

    /* Set bus clk div. */
    stcSysClkCfg.enHclkDiv  = ClkSysclkDiv1;
    stcSysClkCfg.enExclkDiv = ClkSysclkDiv2;
    stcSysClkCfg.enPclk0Div = ClkSysclkDiv1;
    stcSysClkCfg.enPclk1Div = ClkSysclkDiv2;
    stcSysClkCfg.enPclk2Div = ClkSysclkDiv4;
    stcSysClkCfg.enPclk3Div = ClkSysclkDiv4;
    stcSysClkCfg.enPclk4Div = ClkSysclkDiv2;
    CLK_SysClkConfig(&stcSysClkCfg);

    /* Config Xtal and Enable Xtal */
    stcXtalCfg.enMode = ClkXtalModeOsc;
    stcXtalCfg.enDrv = ClkXtalLowDrv;
    stcXtalCfg.enFastStartup = Enable;
    CLK_XtalConfig(&stcXtalCfg);
    CLK_XtalCmd(Enable);

    /* sram init include read/write wait cycle setting */
    stcSramConfig.u8SramIdx = Sram12Idx | Sram3Idx | SramHsIdx | SramRetIdx;
    stcSramConfig.enSramRC = SramCycle2;
    stcSramConfig.enSramWC = SramCycle2;
    SRAM_Init(&stcSramConfig);

    /* flash read wait cycle setting */
    EFM_Unlock();
    EFM_SetLatency(EFM_LATENCY_5);
    EFM_Lock();

    /* MPLL config (XTAL / pllmDiv * plln / PllpDiv = 200M). */
    stcMpllCfg.pllmDiv = 1ul;
    stcMpllCfg.plln    = 50ul;
    stcMpllCfg.PllpDiv = 2ul;
    stcMpllCfg.PllqDiv = 2ul;
    stcMpllCfg.PllrDiv = 2ul;
    CLK_SetPllSource(ClkPllSrcXTAL);
    CLK_MpllConfig(&stcMpllCfg);

    /* Enable MPLL. */
    CLK_MpllCmd(Enable);
    /* Wait MPLL ready. */
    while(Set != CLK_GetFlagStatus(ClkFlagMPLLRdy))
    {
        ;
    }
    /* Switch driver ability */
    PWC_HS2HP();
    /* Switch system clock source to MPLL. */
    CLK_SetSysClkSource(CLKSysSrcMPLL);
}
