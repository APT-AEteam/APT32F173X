/***********************************************************************//** 
 * \file  lowPower_demo.c
 * \brief  low power demo code
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-6-2 <td>V2.0 <td>WNN    <td>initial
 * <tr><td> 2023-9-22 <td>V0.1  <td>GQQ   <td>fix bug,code normalization
 * </table>
 * *********************************************************************
*/

/* include ----------------------------------------------------------------*/

#include "csi_drv.h"
#include "iostring.h"
#include "board_config.h"


/* externs function---------------------------------------------------*/
/* externs variable-------------------------------------------------*/
/* Private variable-------------------------------------------------*/


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
	csi_gpio_set_low(GPIOB, PB2);
}


/** \brief lp_lpt_wakeup_demo通过LPT唤醒DEEPSLEEP模式，低功耗的模式=sleep/deepsleep
 * 		   sleep模式时,tick中断会唤醒cpu，测试时需要注意(可在system_init中注释掉tick初始化)
 * 			
 *  \param  none
 *  \return none
 */
void lp_lpt_wakeup_demo(void)
{
	csi_pm_mode_e ePmMode = PM_MODE_DEEPSLEEP;		
	csi_lpt_time_config_t tTimConfig; 

#if (USE_GUI==0)	
	csi_gpio_set_mux(GPIOA,PA5,PA5_OUTPUT);					//配置PA05输出,用来指示程序从deepsleep 模式唤醒
#endif	

#ifdef CONFIG_USER_PM	
	csi_pm_attach_callback(ePmMode, prepare_lp, wkup_lp);	//需要在工程设置compiler tab下加入define CONFIG_USER_PM=1;
#endif

	csi_pm_set_wakeup_src(SRC_WKUP_LPT, ENABLE);			//配置唤醒源
//	csi_pm_clk_enable(DP_ISOSC);							//低功耗模式下时钟开启/关闭
//	csi_pm_clk_enable(DP_IMOSC);
//	csi_pm_clk_enable(DP_ESOSC);
//	csi_pm_clk_enable(DP_EMOSC);
	
	//lpt初始化配置
	tTimConfig.wTimeVal = 500;								//LPT定时值 = 500ms
	tTimConfig.eRunMode  = LPT_CNT_CONT;					//LPT计数器工作模式，连续
	tTimConfig.eClksrc=LPT_CLK_ISCLK;  						//LPT时钟源  
	csi_lpt_timer_init(LPT,&tTimConfig);        			//初始化lpt,默认采用PEND中断

	csi_lpt_start(LPT);	  
	
	delay_ums(200);											
	
	
	while(1) 
	{
		csi_pm_enter_sleep(ePmMode);
		csi_gpio_toggle(GPIOA,PA5);							//PA5 500ms翻转一次
	}
}


/** \brief lp_exi_wakeup_demo：exi唤醒低功耗的示例代码，低功耗的模式=sleep/deepsleep
 * 		   sleep模式时,tick中断会唤醒cpu，测试时需要注意(可在system_init中注释掉tick初始化)
 * 
 * 			注意：exi 唤醒deepsleep不需要配置唤醒源
 *  \param  none
 *  \return none
 */
void lp_exi_wakeup_demo(void)
{
	csi_pm_mode_e ePmMode = PM_MODE_DEEPSLEEP;						//PM_MODE_SLEEP/PM_MODE_DEEPSLEEP
	

#ifdef CONFIG_USER_PM	
	csi_pm_attach_callback(ePmMode, prepare_lp, wkup_lp);	//需要在工程设置compiler tab下加入define CONFIG_USER_PM=1;
#endif

#if (USE_GUI==0)	
	csi_gpio_set_mux(GPIOB,PB2,PB2_OUTPUT);							//PB02 OUTPUT，指示程序从低功耗模式唤醒
	
	csi_gpio_set_mux(GPIOB,PB1,PB1_INPUT);							//PB1 输入,作为低功耗唤醒源						
	csi_gpio_pull_mode(GPIOB,PB1, GPIO_PULLUP);						//PB1 上拉
	csi_gpio_int_enable(GPIOB,PB1);  
	csi_gpio_irq_mode(GPIOB,PB1,EXI_GRP1, GPIO_IRQ_FALLING_EDGE);	//PB1 下降沿产生中断
	csi_gpio_vic_irq_enable(EXI_GRP1, ENABLE);						//PB1 中断使能，选择中断组1
#endif
	
	csi_pm_clk_disable(SP_IDLE_PCLK);								//sleep模式下关闭PCLK
	csi_pm_clk_disable(SP_IDLE_HCLK);								//sleep模式下关闭HCLK
//	csi_pm_clk_enable(DP_ISOSC);
//	csi_pm_clk_enable(DP_IMOSC);
//	csi_pm_clk_enable(DP_ESOSC);
//	csi_pm_clk_enable(DP_EMOSC);
	
	delay_ums(200);
	
	while(1) 
	{
		csi_pm_enter_sleep(ePmMode);								//进入低功耗模式
		
		csi_gpio_toggle(GPIOB,PB2);									//退出低功耗模式，PB2管脚翻转
		delay_ums(100);
		csi_gpio_toggle(GPIOB,PB2);									
		delay_ums(100);
		csi_gpio_toggle(GPIOB,PB2);									
		delay_ums(100);
		csi_gpio_toggle(GPIOB,PB2);									
		delay_ums(100);
		csi_gpio_toggle(GPIOB,PB2);									
		delay_ums(100);
		csi_gpio_toggle(GPIOB,PB2);									
		delay_ums(100);
	}
}

/** \brief lp_lvd_wakeup_demo：lvd唤醒低功耗的示例代码，低功耗的模式=sleep/deepsleep
 * 		   sleep模式时,tick中断会唤醒cpu，测试时需要注意(可在system_init中注释掉tick初始化)
 * 
 *  \param  none
 *  \return none
 */
void lp_lvd_wakeup_demo(void)
{
	csi_pm_mode_e ePmMode = PM_MODE_DEEPSLEEP;					//PM_MODE_SLEEP/PM_MODE_DEEPSLEEP

#if (USE_GUI==0)
	csi_gpio_set_mux(GPIOB,PB2,PB2_OUTPUT);						//PB02 OUTPUT
#endif
	
#ifdef CONFIG_USER_PM	
	csi_pm_attach_callback(ePmMode, prepare_lp, wkup_lp);	//需要在工程设置compiler tab下加入define CONFIG_USER_PM=1;
#endif
	
	csi_pm_clk_disable(SP_IDLE_PCLK);							//sleep模式下关闭PCLK
	csi_pm_clk_disable(SP_IDLE_HCLK);							//sleep模式下关闭HCLK
//	csi_pm_clk_enable(DP_ISOSC);
//	csi_pm_clk_enable(DP_IMOSC);
//	csi_pm_clk_enable(DP_ESOSC);
//	csi_pm_clk_enable(DP_EMOSC);
	
	csi_pm_set_wakeup_src(SRC_WKUP_LVD, ENABLE);				//配置LVD为中断唤醒源

	//LVD WAKEUP DeepSleep
	csi_set_lvd(LVD_INTF,LVD_30);  								//VDD掉电到3.9V即触发LVD中断
	csi_lvd_lvr_enable();
	
	delay_ums(200);
	
	while(1) 
	{
		csi_pm_enter_sleep(ePmMode);							//进入低功耗模式
		
		csi_gpio_toggle(GPIOB,PB2);								//退出低功耗模式，PB2管脚翻转
		delay_ums(100);
		csi_gpio_toggle(GPIOB,PB2);									
		delay_ums(100);
		csi_gpio_toggle(GPIOB,PB2);									
		delay_ums(100);
		csi_gpio_toggle(GPIOB,PB2);									
		delay_ums(100);
		csi_gpio_toggle(GPIOB,PB2);									
		delay_ums(100);
		csi_gpio_toggle(GPIOB,PB2);									
		delay_ums(100);
	}
}


/** \brief lp_rtc_wakeup_demo：rtc唤醒低功耗的示例代码，低功耗的模式=sleep/deepsleep
 * 		   sleep模式时,tick中断会唤醒cpu，测试时需要注意(可在system_init中注释掉tick初始化)
 * 
 *  \param  none
 *  \return none
 */
void lp_rtc_wakeup_demo(void)
{
	csi_pm_mode_e ePmMode = PM_MODE_DEEPSLEEP;					//PM_MODE_SLEEP/PM_MODE_DEEPSLEEP
	csi_rtc_config_t tRtcConfig;
	
#if (USE_GUI==0)	
	csi_gpio_set_mux(GPIOB,PB2,PB2_OUTPUT);						//PB02 OUTPUT
	
	csi_gpio_set_mux(XIN_PORT,XIN_PIN, XIN_PIN_FUNC);			//EMOSC管脚使能	RTC使用外部时钟源时，必须使能相应的外部时钟管脚
	csi_gpio_set_mux(XOUT_PORT,XOUT_PIN, XOUT_PIN_FUNC);		//EMOSC管脚使能	RTC使用外部时钟源时，必须使能相应的外部时钟管脚
#endif
	
#ifdef CONFIG_USER_PM	
	csi_pm_attach_callback(ePmMode, prepare_lp, wkup_lp);	//需要在工程设置compiler tab下加入define CONFIG_USER_PM=1;
#endif
	
	csi_pm_clk_disable(SP_IDLE_PCLK);							//sleep模式下关闭PCLK
	csi_pm_clk_disable(SP_IDLE_HCLK);							//sleep模式下关闭HCLK
//	csi_pm_clk_enable(DP_ISOSC);
//	csi_pm_clk_enable(DP_IMOSC);
//	csi_pm_clk_enable(DP_ESOSC);
//	csi_pm_clk_enable(DP_EMOSC);
	
	csi_pm_set_wakeup_src(SRC_WKUP_RTC, ENABLE);

	//RTC WAKEUP DeepSleep
	tRtcConfig.eClkSrc = RTC_CLKSRC_ISOSC;  					//选择时钟源
	tRtcConfig.eFmt = RTC_MODE_24FMT;				  			//选择时间模式
	csi_rtc_init(RTC, &tRtcConfig);				  				//初始化RTC
	csi_rtc_set_timer(RTC, RTC_TIMER_0_5S);						//每0.5s进一次中断
	csi_rtc_start(RTC);	
	
	delay_ums(200);
	
	while(1) 
	{
		csi_pm_enter_sleep(ePmMode);							//进入低功耗模式
		csi_gpio_toggle(GPIOB,PB2);								//退出低功耗模式，PB2管脚翻转
	}
}

/** \brief lp_iwdt_wakeup_demo：iwdt唤醒低功耗的示例代码，低功耗的模式=sleep/deepsleep
 * 		   sleep模式时,tick中断会唤醒cpu，测试时需要注意(可在system_init中注释掉tick初始化)
 * 
 *  \param  none
 *  \return none
 */
void lp_iwdt_wakeup_demo(void)
{
	csi_pm_mode_e ePmMode = PM_MODE_DEEPSLEEP;			//PM_MODE_SLEEP/PM_MODE_DEEPSLEEP

#if (USE_GUI==0)	
	csi_gpio_set_mux(GPIOB,PB2,PB2_OUTPUT);				//PB02 OUTPUT
#endif	
	
#ifdef CONFIG_USER_PM	
	csi_pm_attach_callback(ePmMode, prepare_lp, wkup_lp);	//需要在工程设置compiler tab下加入define CONFIG_USER_PM=1;
#endif

	csi_pm_clk_disable(SP_IDLE_PCLK);							//sleep模式下关闭PCLK
	csi_pm_clk_disable(SP_IDLE_HCLK);							//sleep模式下关闭HCLK
//	csi_pm_clk_enable(DP_ISOSC);
//	csi_pm_clk_enable(DP_IMOSC);
//	csi_pm_clk_enable(DP_ESOSC);
//	csi_pm_clk_enable(DP_EMOSC);
	
	csi_pm_set_wakeup_src(SRC_WKUP_IWDT, ENABLE);

	//IWDT WAKEUP DeepSleep
	csi_iwdt_init(IWDT_TO_1024);								//初始化看门狗，溢出时间为1024ms(系统复位时间)
	csi_iwdt_int_enable(IWDT_ALARMTO_4_8);						//使能看门狗报警中断，报警时间为4/8溢出时间(500ms)
	csi_iwdt_open();	
	csi_iwdt_feed();
	
	delay_ums(200);
	
	while(1) 
	{
		
		csi_pm_enter_sleep(ePmMode);							//进入低功耗模式
		csi_iwdt_feed();										//退出低功耗之后，及时喂狗，防止复位
		csi_gpio_toggle(GPIOB,PB2);								//退出低功耗模式，PB2管脚翻转
	}
}


