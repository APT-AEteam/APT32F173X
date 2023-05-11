/***********************************************************************//** 
 * \file  dac_demo.c
 * \brief  DAC_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-3-17 <td>V0.0 <td>xs     <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include "sys_clk.h"
#include <drv/dac.h>
#include "pin.h"

#include "demo.h"
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

/** \brief 配置DAC的初始电平码值，设置时钟分频，配置DAC中断及触发模式
 * 
 *  \param[in] none
 *  \return none
 */
void dac_demo(void)	
{
	csi_dac_config_t tDacConfig;

	//dac 参数配置初始化
	tDacConfig.byClkDiv 	= 0x02;				//DAC clk两分频：FCK = FHCLK / 2
	tDacConfig.byRefsel	 	= DISABLE;			//DAC 参考电平选择
	tDacConfig.byDatarset 	= 0x00;				//DAC 电平码值设置
	tDacConfig.byBufsel 	= ENABLE;			//DAC BUF enable时，PA4输出，BUF disable时，PA8输出
	
	csi_dac_init(DAC0, &tDacConfig);	
	csi_dac_en(DAC0);
	
	csi_dac_irq_enable(DAC0, EOC,1);	//使能EOC中断
	csi_dac_irq_enable(DAC0, WRERR,1);	//使能WRERR中断
	csi_dac_irq_enable(DAC0, SYNCERR,1);//使能SYNCERR中断
	
	csi_dac_syncr_enable(DAC0, SYNCIN0,true);//开启DAC_SYNCIN0触发
	csi_dac_syncr_enable(DAC0, SYNCIN1,true);//开启DAC_SYNCIN1触发
	csi_dac_syncr_enable(DAC0, SYNCIN2,true);//开启DAC_SYNCIN2触发
	csi_dac_step_val(DAC0, 409);//设置触发增减值
	
	csi_dac_start(DAC0);
}

/** \brief dac interrupt handle function
 * 
 *  \param[in] ptDacBase: pointer of dac register structure
 *  \return none
 */ 
__attribute__((weak)) void dac_irqhandler(csp_dac_t *ptDacBase)
{
	volatile uint32_t wVal = csp_dac_get_misr(ptDacBase);	
	csp_dac_irq_clr(ptDacBase,wVal);
}
