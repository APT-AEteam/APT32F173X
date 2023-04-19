/***********************************************************************//** 
 * \file  wkup_time_elec_test.c
 * \brief  WKUP_TIME_ELEC_TEST description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-4-19 <td>V0.0 <td>ZJY     <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include <iostring.h>
#include <csi_drv.h>

/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/


/** \brief sleep唤醒时间测试： EXI唤醒Sleep，Sleep模式下配置PCLK关闭，EXI下降沿唤醒，CLO输出PCLK，唤醒后PCLK自动打开；
 *   测试EXI的管脚下降沿和CLO管脚PCLK开始输出时钟的时间为唤醒时间；EXI = PB1, CLO = PD4
 *   
 *  \param[in] bySlpRcvTim: 低功耗唤醒稳定时间，value：0/1/2/3,
 *  \return none
 */
void sleep_wkup_time_test(uint8_t bySlpRcvTim)
{
	csi_pin_set_mux(PD0, PD0_OUTPUT);	 
	csi_pin_set_high(PD0);										//PD0 OUTPUT H
	
	//sleep模式下系统时钟选择IM_5M
	csi_clo_config(CLO_PCLK, CLO_DIV2, PD4);					//CLO = PD4 OUTPUT PCLK
	
	//PB1唤醒sleep，PB1的输入用信号发生器,周期100ms
	csi_pin_set_mux(PB1,PB1_INPUT);								//PB1 输入							
	csi_pin_pull_mode(PB1, GPIO_PULLUP);						//PB1 上拉
	csi_pin_irq_mode(PB1,EXI_GRP1, GPIO_IRQ_FALLING_EDGE);		//PB1 下降沿产生中断
	csi_pin_irq_enable(PB1, ENABLE);							//PB1 中断使能，选择中断组1
	csi_pin_vic_irq_enable(EXI_GRP1, ENABLE);					//PB1 VIC中断使能，选择中断组1	
	csi_vic_get_pending_irq(EXI1_IRQ_NUM);
	
	csi_pm_clk_enable(SP_IDLE_PCLK, DISABLE);					//sleep模式下关闭PCLK
	csi_pm_clk_enable(SP_IDLE_HCLK, DISABLE);					//sleep模式下关闭HCLK
	
	SYSCON->PWROPT |= (SYSCON->PWROPT & ~SLP_PRCVTIM_MSK) | PWROPT_KEY | (bySlpRcvTim << SLP_PRCVTIM_POS);		//sleep delay
	
	//延时，防止sleep下代码烧录不进去
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	
	
	while(1)
	{
		csi_pm_enter_sleep(PM_MODE_SLEEP);					//进入sleep模式，PD0高
		
		mdelay(10);
		csi_pin_set_low(PD0);
		mdelay(10);
		csi_pin_set_high(PD0);
		mdelay(10);
		csi_pin_set_low(PD0);
		mdelay(10);
		csi_pin_set_high(PD0);
		mdelay(10);
		csi_pin_set_low(PD0);
		mdelay(10);
		csi_pin_set_high(PD0);
	}
}

/** \brief deepsleep唤醒时间测试： EXI唤醒DeepSleep，DeepSleep模式下配置PCLK关闭，EXI下降沿唤醒，CLO输出PCLK，唤醒后PCLK自动打开；
 *   测试EXI的管脚下降沿和CLO管脚PCLK开始输出时钟的时间为唤醒时间；EXI = PB1, CLO = PD4
 *   
 *  \param[in] wSclkFreq: 系统时钟值，IMOSC_5M_VALUE/HFOSC_24M_VALUE/PLL_105M_VALUE
 *  \param[in] byDspRcvTim: 低功耗唤醒稳定时间，value：0/1/2/3,
 *  \return none
 */
void deepsleep_wkup_time_test(uint32_t wSclkFreq, uint8_t byDspRcvTim)
{
	clo_div_e eCloDiv = CLO_DIV2;
	
	csi_pin_set_mux(PD0, PD0_OUTPUT);	 
	csi_pin_set_high(PD0);										//PD0 OUTPUT H
	
	if(wSclkFreq == IMOSC_5M_VALUE)								//sleep模式下系统时钟选择IM_5M/HF_24M/PLL_105M
		eCloDiv = CLO_DIV2;					
	else if(wSclkFreq == HFOSC_24M_VALUE)
		eCloDiv = CLO_DIV8;
	else if(wSclkFreq == PLL_105M_VALUE)
		eCloDiv = CLO_DIV16;
			
	csi_clo_config(CLO_PCLK, eCloDiv, PD4);						//CLO = PD4 OUTPUT PCLK
	
	//PB1唤醒deepsleep，PB1的输入用信号发生器,,周期100ms
	csi_pin_set_mux(PB1,PB1_INPUT);								//PB1 输入							
	csi_pin_pull_mode(PB1, GPIO_PULLUP);						//PB1 上拉
	csi_pin_irq_mode(PB1,EXI_GRP1, GPIO_IRQ_FALLING_EDGE);		//PB1 下降沿产生中断
	csi_pin_irq_enable(PB1, ENABLE);							//PB1 中断使能，选择中断组1
	csi_pin_vic_irq_enable(EXI_GRP1, ENABLE);					//PB1 VIC中断使能，选择中断组1	
	csi_vic_get_pending_irq(EXI1_IRQ_NUM);
	
	csi_pm_clk_enable(SP_IDLE_PCLK, DISABLE);					//deepsleep模式下关闭PCLK
	csi_pm_clk_enable(SP_IDLE_HCLK, DISABLE);					//deepsleep模式下关闭HCLK
	
	SYSCON->PWROPT |= (SYSCON->PWROPT & ~DSP_PRCVTIM_MSK) | PWROPT_KEY | (byDspRcvTim << DSP_PRCVTIM_POS);		//deepsleep delay
	
	//延时，防止sleep下代码烧录不进去
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	
	while(1)
	{
		csi_pm_enter_sleep(PM_MODE_DEEPSLEEP);					//进入deepsleep模式，PD0高
		
		mdelay(10);
		csi_pin_set_low(PD0);
		mdelay(10);
		csi_pin_set_high(PD0);
		mdelay(10);
		csi_pin_set_low(PD0);
		mdelay(10);
		csi_pin_set_high(PD0);
		mdelay(10);
		csi_pin_set_low(PD0);
		mdelay(10);
		csi_pin_set_high(PD0);
	}
}