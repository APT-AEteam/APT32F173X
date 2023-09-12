/***********************************************************************//** 
 * \file  dac_demo.c
 * \brief  DAC_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  		<th>Version  <th>Author  <th>Description
 * <tr><td> 2023-3-17   <td>V0.0 	 <td>xs      <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include "drv/dac.h"
#include "pin.h"
#include "board_config.h"
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

#if (USE_DAC_CALLBACK == 0)
/** \brief	DACx_int_handler: DAC中断服务函数
 * 
 *  \brief 	DAC发生中断时会调用此函数，函数在interrupt.c里定义为弱(weak)属性，默认不做处理；用户用到DAC中
 * 			断时，请重新定义此函数，在此函数中进行对应中断处理，也可直接在interrupt.c里的函数里进行处理
 * 
 *  \param[in] none
 *  \return none
 */
ATTRIBUTE_ISR  void dac0_int_handler(void)
{
	//用户直接在中断服务接口函数里处理中断，建议客户使用此模式
	volatile uint32_t wMisr = csp_dac_get_isr(DAC0);
	
	if(wMisr & DAC_INTSRC_EOC)					//PEND interrupt
	{
		csi_dac_clr_isr(DAC0, DAC_INTSRC_EOC);
	}
	if(wMisr & DAC_INTSRC_WRERR)				//CMP interrupt
	{
		csi_dac_clr_isr(DAC0, DAC_INTSRC_WRERR);	
	}
	if(wMisr & DAC_INTSRC_SYNCERR)				//EVTRG interrupt
	{
		csi_dac_clr_isr(DAC0, DAC_INTSRC_SYNCERR);
	}
}
#endif

/** \brief 配置DAC的初始电平码值，设置时钟分频
 *  \brief DAC可以选择PA4或者PA8作为输出口，BUF enable时，PA4输出，BUF disable时，PA8输出
 *  \param[in] none
 *  \return none
 */
void dac_demo(void)	
{
#if (USE_GUI == 0)	
	//dac 输出管脚配置
	csi_gpio_set_mux(GPIOA,PA4,PA4_DAC0_OUT);	
	csi_gpio_set_mux(GPIOA,PA8,PA8_DAC0_OUT);
	csi_gpio_output_mode(GPIOA,PA8,GPIO_OPEN_DRAIN);
#endif	
	//dac 参数配置初始化
	csi_dac_config_t tDacConfig;
	tDacConfig.byClkDiv 	= 0x02;				//DAC clk两分频：FCK = FHCLK / 2
	tDacConfig.bRefsel	 	= DISABLE;			//DAC 参考电平选择
	tDacConfig.hwDatarset 	= 0x00;				//DAC 电平码值设置
	tDacConfig.bBufen 		= ENABLE;			//DAC BUF enable时，PA4输出，BUF disable时，PA8输出	
	csi_dac_init(DAC0, &tDacConfig);
	
	csi_dac_enable(DAC0);
	csi_dac_start(DAC0);
}
/** \brief 配置DAC的中断及触发模式
 *  \brief DAC可以选择PA4或者PA8作为输出口，BUF enable时，PA4输出，BUF disable时，PA8输出
 *  \param[in] none
 *  \return none
 */
void dac_sync_demo(void)	
{
#if (USE_GUI == 0)
	//dac 输出管脚配置
	csi_gpio_set_mux(GPIOA,PA4,PA4_DAC0_OUT);	
	csi_gpio_set_mux(GPIOA,PA8,PA8_DAC0_OUT);
	csi_gpio_output_mode(GPIOA,PA8,GPIO_OPEN_DRAIN);
#endif	
	//dac 参数配置初始化
	csi_dac_config_t tDacConfig;
	tDacConfig.byClkDiv 	= 0x02;				//DAC clk两分频：FCK = FHCLK / 2
	tDacConfig.bRefsel	 	= DISABLE;			//DAC 参考电平选择
	tDacConfig.hwDatarset 	= 0x00;				//DAC 电平码值设置
	tDacConfig.bBufen 		= ENABLE;			//DAC BUF enable时，PA4输出，BUF disable时，PA8输出
	csi_dac_init(DAC0, &tDacConfig);
	
	csi_dac_enable(DAC0);
	
	csi_dac_int_enable(DAC0, DAC_INTSRC_EOC);		//使能EOC中断
	csi_dac_int_enable(DAC0, DAC_INTSRC_WRERR);	//使能WRERR中断
	csi_dac_int_enable(DAC0, DAC_INTSRC_SYNCERR);	//使能SYNCERR中断
	
	csi_dac_sync_enable(DAC0, SYNCIN0);//开启DAC_SYNCIN0触发
	csi_dac_sync_enable(DAC0, SYNCIN1);//开启DAC_SYNCIN1触发
	csi_dac_sync_enable(DAC0, SYNCIN2);//开启DAC_SYNCIN2触发
	csi_dac_set_step(DAC0, 409);//设置触发增减值
	
	csi_dac_start(DAC0);
}


