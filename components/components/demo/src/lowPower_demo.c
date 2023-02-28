/***********************************************************************//** 
 * \file  lowPower_demo.c
 * \brief  low power demo code
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-6-2 <td>V2.0 <td>WNN    <td>initial
 * </table>
 * *********************************************************************
*/

/* include ----------------------------------------------------------------*/
#include <drv/pm.h>
#include <drv/pin.h>
#include <drv/rtc.h>
#include <drv/lpt.h>
#include <drv/irq.h>
#include <drv/iwdt.h>
#include <drv/tick.h>
#include <drv/common.h> 
#include <drv/reliability.h>

#include "demo.h"
#include "iostring.h"
#include "csp_syscon.h"
#include "board_config.h"

/* Private macro------------------------------------------------------*/
#define	SRAM1_LEN		256

/* externs function---------------------------------------------------*/
/* externs variablesr-------------------------------------------------*/
/* Private variablesr-------------------------------------------------*/
uint8_t *bypSramAddr = (uint8_t *)0x20002000;	


volatile uint8_t byExiFlg = 0x00;

/** \brief  delay, imprecise delay timer
 * 
 *  \param[in] t: delay timer; unit: 10us systick = 48M
 *  \return none
 */ 
void delay_ums(uint32_t t)
{
	volatile uint32_t i,j ,k=0;
	j = 25* t;
	for ( i = 0; i < j; i++ )
	{
		k++;
	}
}

/** \brief 进入低功耗模式前的准备动作
 *  \param[in] none
 *  \return error code
 */
static void prepare_lp(void)				  
{
	//USER CODE，如记忆管脚状态
}

/** \brief 退出低功耗模式后恢复动作
 *  \param[in] none
 *  \return error code
 */
static void wkup_lp(void)					   
{
	//USER CODE，如恢复管脚状态
	csi_pin_set_low(PB2);
}


/** \brief 通过LPT唤醒DEEPSLEEP模式
 * 
 *  \param  none
 *  \return none
 */
void lp_lpt_wakeup_deepsleep_demo(void)
{
	csi_pm_mode_e ePmMode = PM_MODE_DEEPSLEEP;		
	
	csi_pin_set_mux(PA5,PA5_OUTPUT);				//PA05 OUTPUT
	
	csi_pin_toggle(PA5);
	delay_ums(200);
	csi_pin_toggle(PA5);
	delay_ums(200);
	csi_pin_toggle(PA5);
	delay_ums(200);
	csi_pin_toggle(PA5);
	delay_ums(200);
	csi_pin_toggle(PA5);
	delay_ums(200);
	csi_pin_toggle(PA5);
	delay_ums(200);
	csi_pin_toggle(PA5);
	delay_ums(200);
	csi_pin_toggle(PA5);
	delay_ums(200);

#ifdef CONFIG_USER_PM	
	csi_pm_attach_callback(ePmMode, prepare_lp, wkup_lp);	//需要在工程设置compiler tab下加入define CONFIG_USER_PM=1;
#endif

	csi_pm_config_wakeup_source(WKUP_LPT, ENABLE);			//配置唤醒源
//	csi_pm_clk_enable(DP_ISOSC, ENABLE);					//SNOOZE模式下时钟开启/关闭
//	csi_pm_clk_enable(DP_IMOSC, ENABLE);
//	csi_pm_clk_enable(DP_ESOSC, ENABLE);
//	csi_pm_clk_enable(DP_EMOSC, ENABLE);
	
	//lpt初始化配置
	csi_lpt_timer_init(LPT,LPT_CLK_ISCLK, 500);       		//初始化lpt,选用内部超低功耗时钟,定时500ms,默认采用PEND中断
	csi_lpt_start(LPT);	  
	delay_ums(200);
	
	switch(ePmMode)
	{
		case PM_MODE_SLEEP:
			my_printf("Enter Sleep Mode\n");
			break;
		case PM_MODE_DEEPSLEEP:
		//	my_printf("Enter Deep-Sleep mode\n");
			break;
		default:
			break;
	}
	
	while(1) 
	{
		csi_pin_set_high(PA5);
		csi_pm_enter_sleep(ePmMode);
	//	my_printf("Wakeup From Deep-Sleep Mode...\n");
		csi_pin_set_low(PA5);
		delay_ums(200);
	}
}

/** \brief 各种源唤醒低功耗的示例代码，低功耗的模式=sleep/deepsleep/snooze/shutdown
 * 		   sleep模式时,tick中断会唤醒cpu，测试时需要注意(可在system_init中注释掉tick初始化)
 * 
 *  \param  none
 *  \return none
 */
void lp_wakeup_demo(void)
{
	csi_pm_mode_e ePmMode = PM_MODE_SLEEP;			//PM_MODE_SLEEP/PM_MODE_DEEPSLEEP/PM_MODE_SNOOZE/PM_MODE_SHUTDOWN
	uint16_t hwRstSrc = csi_get_rst_reason();
	
	if(hwRstSrc)									//获取并打印复位信息
	{
		my_printf("System Reset Source = 0x%x \n", hwRstSrc);
		csi_clr_rst_reason(hwRstSrc);				//清除复位信息
	}
	
	csi_pin_set_mux(PB2,PB2_OUTPUT);				//PB02 OUTPUT
	
	csi_pin_toggle(PB2);
	mdelay(200);
	csi_pin_toggle(PB2);
	mdelay(200);
	csi_pin_toggle(PB2);
	mdelay(200);
	csi_pin_toggle(PB2);
	mdelay(200);
	csi_pin_toggle(PB2);
	mdelay(200);
	csi_pin_toggle(PB2);
	mdelay(200);
	csi_pin_toggle(PB2);
	mdelay(200);
	csi_pin_toggle(PB2);
	mdelay(200);
	
#ifdef CONFIG_USER_PM	
	csi_pm_attach_callback(ePmMode, prepare_lp, wkup_lp);	//需要在工程设置compiler tab下加入define CONFIG_USER_PM=1;
#endif
	
	csi_pin_set_mux(PB1,PB1_INPUT);								//PB1 输入							
	csi_pin_pull_mode(PB1, GPIO_PULLUP);						//PB1 上拉
	csi_pin_irq_mode(PB1,EXI_GRP1, GPIO_IRQ_FALLING_EDGE);		//PB1 下降沿产生中断
	csi_pin_irq_enable(PB1, ENABLE);							//PB1 中断使能，选择中断组1
	csi_pin_vic_irq_enable(EXI_GRP18, ENABLE);					//PB1 VIC中断使能，选择中断组1	
	csi_vic_get_pending_irq(EXI1_IRQ_NUM);
	
	csi_pm_clk_enable(SP_IDLE_PCLK, DISABLE);					//sleep模式下关闭PCLK
	csi_pm_clk_enable(SP_IDLE_HCLK, DISABLE);					//sleep模式下关闭HCLK
//	csi_pm_clk_enable(DP_ISOSC, ENABLE);
//	csi_pm_clk_enable(DP_IMOSC, ENABLE);
//	csi_pm_clk_enable(DP_ESOSC, ENABLE);
//	csi_pm_clk_enable(DP_EMOSC, ENABLE);
	
//	csi_pm_config_wakeup_source(WKUP_RTC, ENABLE);

	
	//LPT WAKEUP DeepSleep/snooze
//	csi_lpt_timer_init(LPT,LPT_CLK_ISCLK, 500);       		//初始化lpt,选用内部超低功耗时钟,定时200ms,默认采用PEND中断
//	csi_lpt_start(LPT);	  
	
	//LVD WAKEUP	DeepSleep/snooze/shutdown
	//csi_lvd_int_enable(LVD_INTF,LVD_30);  						//VDD掉电到3.9V即触发LVD中断
	
	
	//CMP WAKUP DeepSleep
//	cmp_base_demo();
	
//	csi_pin_set_mux(SXIN_PIN, SXIN_PIN_FUNC);		//ESOSC管脚使能	
//	csi_pin_set_mux(SXOUT_PIN, SXOUT_PIN_FUNC);		//ESOSC管脚使能	
//	csi_pin_set_mux(PA03, PA03_OSC_XI);
//	csi_pin_set_mux(PA04, PA04_OSC_XO);
	//RTC WAKEUP DeepSleep/snooze/shutdown
//	{
//		csi_rtc_config_t tRtcConfig;
//		
//		tRtcConfig.byClkSrc = RTC_CLKSRC_ISOSC;  		//选择时钟源
//		tRtcConfig.byFmt = RTC_24FMT;				  	//选择时间模式
//		csi_rtc_init(RTC, &tRtcConfig);				  	//初始化RTC
//		csi_rtc_start_as_timer(RTC, RTC_TIMER_0_5S);	//每1s进一次中断
//		csi_rtc_start(RTC);	
//	}

	//IWDT WAKEUP DeepSleep/snooze/shutdown
//	csi_iwdt_init(IWDT_TO_1024);						//初始化看门狗，溢出时间为1000ms(系统复位时间)
//	csi_iwdt_irq_enable(IWDT_ALARMTO_4_8, ENABLE);		//使能看门狗报警中断，报警时间为4/8溢出时间(500ms)
//	csi_iwdt_open();	
//	csi_iwdt_feed();
	
	mdelay(10);
	
	switch(ePmMode)
	{
		case PM_MODE_SLEEP:
			my_printf("Enter Sleep Mode\n");
			break;
		case PM_MODE_DEEPSLEEP:
			my_printf("Enter Deep-Sleep mode\n");
			break;
		default:
			break;
	}
	
	while(1) 
	{
		csi_pin_set_high(PB2);
		
		csi_pm_enter_sleep(ePmMode);
		//csi_iwdt_feed();
		//mdelay(100);
		csi_pin_set_low(PB2);
		delay_ums(100);
		//my_printf("Wakeup From Sleep Mode...\n");
	}
}


