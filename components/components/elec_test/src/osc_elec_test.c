/***********************************************************************//** 
 * \file  osc_elec_test.c
 * \brief  osc电气参数测试代码
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
#include "bt.h"

/** \brief 进入DEEPSLEEP模式，IM频率电气参数测试
 * 
 * 主时钟分别配置成 {SRC_IMOSC, IMOSC_5M_VALUE, SCLK_DIV1, PCLK_DIV1,5556000, 5556000};
 *	                {SRC_IMOSC, IMOSC_4M_VALUE, SCLK_DIV1, PCLK_DIV1,5556000, 5556000};
 *	                {SRC_IMOSC, IMOSC_2M_VALUE, SCLK_DIV1, PCLK_DIV1,5556000, 5556000};
 *	                {SRC_IMOSC, IMOSC_131K_VALUE, SCLK_DIV1, PCLK_DIV1,5556000, 5556000};
 * 打开DP模式下，IM输出功能
 * 记录IM 1分频后的输出频率
 *  \param  none
 *  \return none
 */
void osc_deepsleep_imosc_test(void)
{
	
	mdelay(1000);
	mdelay(1000);
	
	csi_clo_config(CLO_IMCLK, CLO_DIV1, PB3);	
	csi_cmos_autotrim();
	csi_pm_mode_e ePmMode = PM_MODE_DEEPSLEEP;		  //PM_MODE_DEEPSLEEP  PM_MODE_SLEEP
	csi_pm_clk_enable(DP_IMOSC, ENABLE);                    
	switch(ePmMode)
	{
		case PM_MODE_SLEEP:
	//		my_printf("Enter Sleep Mode\n");
			break;
		case PM_MODE_DEEPSLEEP:
	//		my_printf("Enter Deep-Sleep mode\n");
			break;
		default:
			break;
	}
	while(1) 
	{
		csi_pin_set_high(PA5);
		csi_pm_enter_sleep(ePmMode);
		csi_pin_set_low(PA5);	
	}
}

/** \brief 进入DEEPSLEEP模式，IS频率电气参数测试
 * 
 * 主时钟分别配置成 {SRC_IMOSC, IMOSC_5M_VALUE, SCLK_DIV1, PCLK_DIV1,5556000, 5556000};
 * 打开DP模式下，IS输出功能(默认情况下IS输出已经打开了，可不配置)                
 * 记录IS 1分频后的输出频率              
 *  \param  none
 *  \return none
 */
void osc_deepsleep_isosc_test(void)
{
	
	mdelay(1000);
	mdelay(1000);
	
	csi_clo_config(CLO_ISCLK, CLO_DIV1, PB3);	
	
	csi_cmos_autotrim();
	csi_pm_mode_e ePmMode = PM_MODE_DEEPSLEEP;		  //PM_MODE_DEEPSLEEP  PM_MODE_SLEEP

	csi_pm_clk_enable(DP_ISOSC, ENABLE);			  //DEEPSLEEP模式下时钟开启/关闭

	switch(ePmMode)
	{
		case PM_MODE_SLEEP:
	//		my_printf("Enter Sleep Mode\n");
			break;
		case PM_MODE_DEEPSLEEP:
	//		my_printf("Enter Deep-Sleep mode\n");
			break;
		default:
			break;
	}
	
	while(1) 
	{
		csi_pin_set_high(PA5);
		csi_pm_enter_sleep(ePmMode);
		csi_pin_set_low(PA5);	
	}
}

/** \brief 进入SLEEP模式，IM频率电气参数测试
 * 
 * 主时钟分别配置成 {SRC_IMOSC, IMOSC_5M_VALUE, SCLK_DIV1, PCLK_DIV1,5556000, 5556000};
 *	                {SRC_IMOSC, IMOSC_4M_VALUE, SCLK_DIV1, PCLK_DIV1,5556000, 5556000};
 *	                {SRC_IMOSC, IMOSC_2M_VALUE, SCLK_DIV1, PCLK_DIV1,5556000, 5556000};
 *	                {SRC_IMOSC, IMOSC_131K_VALUE, SCLK_DIV1, PCLK_DIV1,5556000, 5556000};
 * 记录IM 1分频后的输出频率
 *  \param  none
 *  \return none
 */
void osc_sleep_imosc_test(void)
{
	
	mdelay(1000);
	mdelay(1000);
	csi_clo_config(CLO_IMCLK, CLO_DIV1, PB3);	
	SYSCON->PWRKEY = 0xA67A6CC7;
	SYSCON->PWRCR = (SYSCON->PWRCR & (~(0x0f<<16))) | 0x40002;
	
	csi_pm_mode_e ePmMode = PM_MODE_SLEEP;		  //PM_MODE_DEEPSLEEP  PM_MODE_SLEEP
	csi_pin_set_mux(PA5,PA5_OUTPUT);			  //PA05 OUTPUT
	csi_bt_stop(BT3);                             //关闭BT3,BT3中断会唤醒sleep
	switch(ePmMode)
	{
		case PM_MODE_SLEEP:
	//		my_printf("Enter Sleep Mode\n");
			break;
		case PM_MODE_DEEPSLEEP:
	//		my_printf("Enter Deep-Sleep mode\n");
			break;
		default:
			break;
	}
	
	while(1) 
	{
		csi_pin_set_high(PA5);
		csi_pm_enter_sleep(ePmMode);
		csi_pin_set_low(PA5);	
	}
}

/** \brief 进入SLEEP模式，HF频率电气参数测试
 * 
 * 主时钟配置成 {SRC_HFOSC, HFOSC_24M_VALUE, SCLK_DIV1, PCLK_DIV1, 5556000, 5556000};
 * 记录HF 4分频后的输出频率
 *  \param  none
 *  \return none
 */
void osc_sleep_hfosc_test(void)
{
	
	mdelay(1000);
	mdelay(1000);
	csi_clo_config(CLO_HFCLK, CLO_DIV4, PB3);	

	SYSCON->PWRKEY = 0xA67A6CC7;
	SYSCON->PWRCR = (SYSCON->PWRCR & (~(0x0f<<16))) | 0x40002;
	
	csi_pm_mode_e ePmMode = PM_MODE_SLEEP;		  //PM_MODE_DEEPSLEEP  PM_MODE_SLEEP
	csi_pin_set_mux(PA5,PA5_OUTPUT);			  //PA05 OUTPUT
	csi_bt_stop(BT3);                             //关闭BT3,BT3中断会唤醒sleep
	switch(ePmMode)
	{
		case PM_MODE_SLEEP:
	//		my_printf("Enter Sleep Mode\n");
			break;
		case PM_MODE_DEEPSLEEP:
	//		my_printf("Enter Deep-Sleep mode\n");
			break;
		default:
			break;
	}
	
	while(1) 
	{
		csi_pin_set_high(PA5);
		csi_pm_enter_sleep(ePmMode);
		csi_pin_set_low(PA5);	
	}
}

/** \brief 进入SLEEP模式，IS频率输出电气参数测试
 * 
 * 主时钟配置成 {SRC_HFOSC, HFOSC_24M_VALUE, SCLK_DIV1, PCLK_DIV1, 5556000, 5556000};
 * 记录IS 1分频后的输出频率
 *  \param  none
 *  \return none
 */
void osc_sleep_isosc_test(void)
{
	
	mdelay(1000);
	mdelay(1000);
	csi_clo_config(CLO_ISCLK, CLO_DIV1, PB3);

	SYSCON->PWRKEY = 0xA67A6CC7;
	SYSCON->PWRCR = (SYSCON->PWRCR & (~(0x0f<<16))) | 0x40002;
	
	csi_pm_mode_e ePmMode = PM_MODE_SLEEP;		  //PM_MODE_DEEPSLEEP  PM_MODE_SLEEP
	csi_pin_set_mux(PA5,PA5_OUTPUT);			  //PA05 OUTPUT
	csi_bt_stop(BT3);                             //关闭BT3,BT3中断会唤醒sleep
	switch(ePmMode)
	{
		case PM_MODE_SLEEP:
	//		my_printf("Enter Sleep Mode\n");
			break;
		case PM_MODE_DEEPSLEEP:
	//		my_printf("Enter Deep-Sleep mode\n");
			break;
		default:
			break;
	}
	
	while(1) 
	{
		csi_pin_set_high(PA5);
		csi_pm_enter_sleep(ePmMode);
		csi_pin_set_low(PA5);	
	}
}

/** \brief 进入SLEEP模式，PLL频率电气参数测试
 * 
 * 主时钟配置成 {SRC_PLL, PLL_105M_VALUE, SCLK_DIV2, PCLK_DIV1, 5556000, 5556000};
 * 记录PLL 16分频后的输出频率
 *  \param  none
 *  \return none
 */
void osc_sleep_pll_test(void)
{
	
	mdelay(1000);
	mdelay(1000);
	csi_clo_config(CLO_PLL_PCLK, CLO_DIV16, PB3);		

	SYSCON->PWRKEY = 0xA67A6CC7;
	SYSCON->PWRCR = (SYSCON->PWRCR & (~(0x0f<<16))) | 0x40002;
	
	csi_pm_mode_e ePmMode = PM_MODE_SLEEP;		  //PM_MODE_DEEPSLEEP  PM_MODE_SLEEP
	csi_pin_set_mux(PA5,PA5_OUTPUT);			  //PA05 OUTPUT
	csi_bt_stop(BT3);                             //关闭BT3,BT3中断会唤醒sleep
	switch(ePmMode)
	{
		case PM_MODE_SLEEP:
	//		my_printf("Enter Sleep Mode\n");
			break;
		case PM_MODE_DEEPSLEEP:
	//		my_printf("Enter Deep-Sleep mode\n");
			break;
		default:
			break;
	}
	
	while(1) 
	{
		csi_pin_set_high(PA5);
		csi_pm_enter_sleep(ePmMode);
		csi_pin_set_low(PA5);	
	}
}
