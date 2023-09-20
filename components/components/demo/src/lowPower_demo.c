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


/* externs function---------------------------------------------------*/
/* externs variablesr-------------------------------------------------*/
/* Private variablesr-------------------------------------------------*/


/** \brief prepare_lp
 * 			-进入低功耗模式前的准备动作
 *  \param[in] none
 *  \return error code
 */
static void prepare_lp(void)				  
{
	//USER CODE，如记忆管脚状态
}

/** \brief wkup_lp
 * 			-退出低功耗模式后恢复动作
 *  \param[in] none
 *  \return error code
 */
static void wkup_lp(void)					   
{
	//USER CODE，如恢复管脚状态
	csi_pin_set_low(PB2);
}


/** \brief 通过LPT唤醒DEEPSLEEP模式
 * 			- 配置PA5作为程序运行指示：PA5以200ms翻转时，显示代码运行到此函数，
 * 			-PA5以500ms运行的时候，表示LPT正常唤醒deepsleep。PA5快速翻转，表示
 * 			-程序未正常进入低功耗或者异常唤醒。
 *  \param  none
 *  \return none
 */
void lp_lpt_wakeup_deepsleep_demo(void)
{
	csi_pm_mode_e ePmMode = PM_MODE_DEEPSLEEP;		
	
	csi_gpio_set_mux(GPIOA,PA5,PA5_OUTPUT);							//配置PA05输出
	
	csi_gpio_toggle(GPIOA,PA5);
	delay_ums(200);
	csi_gpio_toggle(GPIOA,PA5);
	delay_ums(200);
	csi_gpio_toggle(GPIOA,PA5);
	delay_ums(200);
	csi_gpio_toggle(GPIOA,PA5);
	delay_ums(200);
	csi_gpio_toggle(GPIOA,PA5);
	delay_ums(200);
	csi_gpio_toggle(GPIOA,PA5);
	delay_ums(200);
	csi_gpio_toggle(GPIOA,PA5);
	delay_ums(200);
	csi_gpio_toggle(GPIOA,PA5);
	delay_ums(200);

#ifdef CONFIG_USER_PM	
	csi_pm_attach_callback(ePmMode, prepare_lp, wkup_lp);	//需要在工程设置compiler tab下加入define CONFIG_USER_PM=1;
#endif

	csi_pm_config_wakeup_source(SRC_WKUP_LPT, ENABLE);			//配置唤醒源
//	csi_pm_clk_enable(DP_ISOSC);								//低功耗模式下时钟开启/关闭
//	csi_pm_clk_enable(DP_IMOSC);
//	csi_pm_clk_enable(DP_ESOSC);
//	csi_pm_clk_enable(DP_EMOSC);
	
	//lpt初始化配置
	csi_lpt_time_config_t tTimConfig;
	tTimConfig.wTimeVal = 500;									//LPT定时值 = 500ms
	tTimConfig.eRunMode  = LPT_CNT_CONT;						//LPT计数器工作模式，连续
	tTimConfig.eClksrc=LPT_CLK_ISCLK;  							//LPT时钟源  
	csi_lpt_timer_init(LPT,&tTimConfig);        				//初始化lpt,默认采用PEND中断

	csi_lpt_start(LPT);	  
	delay_ums(200);
	
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
		csi_gpio_set_high(GPIOA,PA5);
		csi_pm_enter_sleep(ePmMode);
		my_printf("Wakeup From Deep-Sleep Mode...\n");
		csi_gpio_set_high(GPIOA,PA5);
	}
}

/** \brief 各种源唤醒低功耗的示例代码，低功耗的模式=sleep/deepsleep
 * 		   sleep模式时,tick中断会唤醒cpu，测试时需要注意(可在system_init中注释掉tick初始化)
 * 
 *  \param  none
 *  \return none
 */
void lp_exi_wakeup_demo(void)
{
	csi_pm_mode_e ePmMode = PM_MODE_SLEEP;			//PM_MODE_SLEEP/PM_MODE_DEEPSLEEP/PM_MODE_SNOOZE/PM_MODE_SHUTDOWN
	uint16_t hwRstSrc = csi_get_rst_reason();
	
	if(hwRstSrc)									//获取并打印复位信息
	{
		my_printf("System Reset Source = 0x%x \n", hwRstSrc);
		csi_clr_rst_reason(hwRstSrc);				//清除复位信息
	}
	
	csi_gpio_set_mux(GPIOB,PB2,PB2_OUTPUT);				//PB02 OUTPUT
	
	csi_gpio_toggle(GPIOB,PB2);
	mdelay(200);
	csi_gpio_toggle(GPIOB,PB2);
	mdelay(200);
	csi_gpio_toggle(GPIOB,PB2);
	mdelay(200);
	csi_gpio_toggle(GPIOB,PB2);
	mdelay(200);
	csi_gpio_toggle(GPIOB,PB2);
	mdelay(200);
	csi_gpio_toggle(GPIOB,PB2);
	mdelay(200);
	csi_gpio_toggle(GPIOB,PB2);
	mdelay(200);
	csi_gpio_toggle(GPIOB,PB2);
	mdelay(200);
	
#ifdef CONFIG_USER_PM	
	csi_pm_attach_callback(ePmMode, prepare_lp, wkup_lp);	//需要在工程设置compiler tab下加入define CONFIG_USER_PM=1;
#endif
	
	csi_gpio_set_mux(GPIOB,PB1,PB1_INPUT);								//PB1 输入							
	csi_gpio_pull_mode(GPIOB,PB1, GPIO_PULLUP);							//PB1 上拉
	csi_gpio_int_enable(GPIOB,PB1);  
	csi_gpio_irq_mode(GPIOB,PB1,EXI_GRP1, GPIO_IRQ_FALLING_EDGE);		//PB1 下降沿产生中断
	csi_gpio_vic_irq_enable(EXI_GRP1, ENABLE);							//PB1 中断使能，选择中断组1

	
	csi_pm_clk_disable(SP_IDLE_PCLK);					//sleep模式下关闭PCLK
	csi_pm_clk_disable(SP_IDLE_HCLK);					//sleep模式下关闭HCLK
//	csi_pm_clk_enable(DP_ISOSC);
//	csi_pm_clk_enable(DP_IMOSC);
//	csi_pm_clk_enable(DP_ESOSC);
//	csi_pm_clk_enable(DP_EMOSC);
	
//	csi_pm_config_wakeup_source(WKUP_RTC, ENABLE);

	
	//LPT WAKEUP DeepSleep/snooze
//	csi_lpt_timer_init(LPT,LPT_CLK_ISCLK, 500);       		//初始化lpt,选用内部超低功耗时钟,定时200ms,默认采用PEND中断
//	csi_lpt_start(LPT);	  
	
	//LVD WAKEUP	DeepSleep/snooze/shutdown
	//csi_lvd_int_enable(LVD_INTF,LVD_30);  						//VDD掉电到3.9V即触发LVD中断
	
	
	//CMP WAKUP DeepSleep
//	cmp_base_demo();
	
//	csi_gpio_set_mux(SXIN_PORT,SXIN_PIN, SXIN_PIN_FUNC);		//ESOSC管脚使能	
//	csi_gpio_set_mux(SXOUT_PORT,SXOUT_PIN, SXOUT_PIN_FUNC);		//ESOSC管脚使能	
//	
//	csi_gpio_set_mux(XIN_PORT,XIN_PIN, XIN_PIN_FUNC);			//EMOSC管脚使能	
//	csi_gpio_set_mux(XOUT_PORT,XOUT_PIN, XOUT_PIN_FUNC);		//EMOSC管脚使能	
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
		csi_gpio_set_high(GPIOB,PB2);
		
		csi_pm_enter_sleep(ePmMode);
		//csi_iwdt_feed();
		csi_gpio_set_high(GPIOB,PB2);
		delay_ums(100);
		my_printf("Wakeup From Sleep Mode...\n");
	}
}


