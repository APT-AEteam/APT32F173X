/***********************************************************************//** 
 * \file  eft_elec_test.c
 * \brief  eft电气参数测试代码
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-4-6 <td>V0.0  <td>YYM   <td>initial
 * </table>
 * *********************************************************************
*/


/* include ----------------------------------------------------------------*/
#include "reliability.h"
#include "pin.h"
#include "sys_clk.h"

/** \brief HF EFT测试
 *	
 * HF为主时钟，程序上电，指示灯有6秒钟左右慢闪，然后进主循环，指示灯开始快闪
 * EFT设备测试：远方EMS61000-4B（智能型群脉冲发生器）
 * 测试电压： +4000V/100KHZ  -4000V/100KHZ
 * 
 *  \param[in] none
 *  \return none
 */
void eft_hf_test(void)
{
	csi_pin_set_mux(PA1, PA1_OUTPUT);		//PA1 output					//delay 100ms
	csi_pin_set_mux(PA2, PA2_OUTPUT);		//PA2 output
	
	tClkConfig.eClkSrc = SRC_HFOSC;
	tClkConfig.wFreq = HFOSC_24M_VALUE;
	tClkConfig.eSdiv = SCLK_DIV1;
	tClkConfig.ePdiv = PCLK_DIV1;
	csi_sysclk_config(tClkConfig);	
	
	mdelay(1000);
	csi_pin_set_high(PA1);				
	csi_pin_set_low(PA2);					
	mdelay(1000);
	csi_pin_set_low(PA1);				
	csi_pin_set_high(PA2);					
	mdelay(1000);
	csi_pin_set_high(PA1);				
	csi_pin_set_low(PA2);						
	mdelay(1000);
	csi_pin_set_low(PA1);				
	csi_pin_set_high(PA2);					
	mdelay(1000);
	csi_pin_set_high(PA1);				
	csi_pin_set_low(PA2);					
	mdelay(1000);
	csi_pin_set_low(PA1);				
	csi_pin_set_high(PA2);				
	while(1)
	{
		mdelay(200);						//delay 200ms
		csi_pin_toggle(PA1);				//PA1 toggle
		csi_pin_toggle(PA2);				//PA2 toggle				
	}		
}

/** \brief HF EFT测试
 *	
 * EM为主时钟，程序上电，指示灯有6秒钟左右慢闪，然后进主循环，指示灯开始快闪
 * EFT设备测试：远方EMS61000-4B（智能型群脉冲发生器）
 * 测试电压： +4000V/100KHZ  -4000V/100KHZ
 * 
 *  \param[in] none
 *  \return none
 */
void eft_em_test(void)
{
	csi_pin_set_mux(PA1, PA1_OUTPUT);		//PA1 output					//delay 100ms
	csi_pin_set_mux(PA2, PA2_OUTPUT);		//PA2 output
	
	csi_pin_set_mux(PD0, PD0_XIN);
	csi_pin_set_mux(PD1, PD1_XOUT);
	
	tClkConfig.eClkSrc = SRC_EMOSC;
	tClkConfig.wFreq = 240000000;
	tClkConfig.eSdiv = SCLK_DIV1;
	tClkConfig.ePdiv = PCLK_DIV1;
	csi_sysclk_config(tClkConfig);
	
	mdelay(1000);
	csi_pin_set_high(PA1);				
	csi_pin_set_low(PA2);					
	mdelay(1000);
	csi_pin_set_low(PA1);				
	csi_pin_set_high(PA2);					
	mdelay(1000);
	csi_pin_set_high(PA1);				
	csi_pin_set_low(PA2);						
	mdelay(1000);
	csi_pin_set_low(PA1);				
	csi_pin_set_high(PA2);					
	mdelay(1000);
	csi_pin_set_high(PA1);				
	csi_pin_set_low(PA2);					
	mdelay(1000);
	csi_pin_set_low(PA1);				
	csi_pin_set_high(PA2);				
	while(1)
	{
		mdelay(200);						//delay 200ms
		csi_pin_toggle(PA1);				//PA1 toggle
		csi_pin_toggle(PA2);				//PA2 toggle				
	}	
}

/** \brief HF EFT测试
 *	
 * pll为主时钟，pll时钟源为HF,程序上电，指示灯有6秒钟左右慢闪，然后进主循环，指示灯开始快闪
 * EFT设备测试：远方EMS61000-4B（智能型群脉冲发生器）
 * 测试电压： +4000V/100KHZ  -4000V/100KHZ
 * 
 *  \param[in] none
 *  \return none
 */
void eft_pll_hf_test(void)
{
	csi_pin_set_mux(PA1, PA1_OUTPUT);		//PA1 output					//delay 100ms
	csi_pin_set_mux(PA2, PA2_OUTPUT);		//PA2 output
	
	csi_pin_set_mux(PD0, PD0_XIN);
	csi_pin_set_mux(PD1, PD1_XOUT);
	
	tClkConfig.eClkSrc = SRC_AUTO_HF_PLL;
	tClkConfig.wFreq = 105000000;
	tClkConfig.eSdiv = SCLK_DIV1;
	tClkConfig.ePdiv = PCLK_DIV1;
	csi_sysclk_config(tClkConfig);
	
	mdelay(1000);
	csi_pin_set_high(PA1);				
	csi_pin_set_low(PA2);					
	mdelay(1000);
	csi_pin_set_low(PA1);				
	csi_pin_set_high(PA2);					
	mdelay(1000);
	csi_pin_set_high(PA1);				
	csi_pin_set_low(PA2);						
	mdelay(1000);
	csi_pin_set_low(PA1);				
	csi_pin_set_high(PA2);					
	mdelay(1000);
	csi_pin_set_high(PA1);				
	csi_pin_set_low(PA2);					
	mdelay(1000);
	csi_pin_set_low(PA1);				
	csi_pin_set_high(PA2);				
	while(1)
	{
		mdelay(200);						//delay 200ms
		csi_pin_toggle(PA1);				//PA1 toggle
		csi_pin_toggle(PA2);				//PA2 toggle				
	}		
}

/** \brief HF EFT测试
 *	
 * pll为主时钟，pll时钟源为EM,程序上电，指示灯有6秒钟左右慢闪，然后进主循环，指示灯开始快闪
 * EFT设备测试：远方EMS61000-4B（智能型群脉冲发生器）
 * 测试电压： +4000V/100KHZ  -4000V/100KHZ
 * 
 *  \param[in] none
 *  \return none
 */
void eft_pll_em_test(void)
{
	csi_pin_set_mux(PA1, PA1_OUTPUT);		//PA1 output					//delay 100ms
	csi_pin_set_mux(PA2, PA2_OUTPUT);		//PA2 output
	
	csi_pin_set_mux(PD0, PD0_XIN);
	csi_pin_set_mux(PD1, PD1_XOUT);
	
	tClkConfig.eClkSrc = SRC_AUTO_HF_PLL;
	tClkConfig.wFreq = 105000000;
	tClkConfig.eSdiv = SCLK_DIV1;
	tClkConfig.ePdiv = PCLK_DIV1;
	csi_sysclk_config(tClkConfig);
	
	mdelay(1000);
	csi_pin_set_high(PA1);				
	csi_pin_set_low(PA2);					
	mdelay(1000);
	csi_pin_set_low(PA1);				
	csi_pin_set_high(PA2);					
	mdelay(1000);
	csi_pin_set_high(PA1);				
	csi_pin_set_low(PA2);						
	mdelay(1000);
	csi_pin_set_low(PA1);				
	csi_pin_set_high(PA2);					
	mdelay(1000);
	csi_pin_set_high(PA1);				
	csi_pin_set_low(PA2);					
	mdelay(1000);
	csi_pin_set_low(PA1);				
	csi_pin_set_high(PA2);				
	while(1)
	{
		mdelay(200);						//delay 200ms
		csi_pin_toggle(PA1);				//PA1 toggle
		csi_pin_toggle(PA2);				//PA2 toggle				
	}
}