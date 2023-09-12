/***********************************************************************//** 
 * \file  dac_demo.c
 * \brief  DAC_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  		<th>Version  <th>Author  <th>Description
 * <tr><td> 2023-9-6 	<td>V0.0 	 <td>xs      <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include "drv/dac.h"
#include "drv/gpio.h"
#include "board_config.h"

#if (USE_DAC_CALLBACK == 1)
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/


/** \brief  dac_callback：DAC中断回调函数
 * 
 * 	\brief	用户定义，支持指EOC/WRERR/SYNCERR中断处理，使用csi标准库，中断发生时会自动调用用户注册的回调函
 * 			数，用户可在回调函数里做自己的处理，而不需要关注具体的底层中断处理。
 * 
 *  \param[out] ptDacBase: 	DACx寄存器结构体指针，指向DACx的基地址 
 *  \param[out] byIsr: 		DACx中断状态
 *  \return none
 */ 
void dac_callback(csp_dac_t *ptDacBase, uint8_t byIsr)
{
	if(byIsr & DAC_INTSRC_EOC)
	{
		nop;						//user code
	}
}
/** \brief	dac_callback_demo：dac转换结束功能demo，使用callback
 * 	\brief  csi接口默认使用(开启)转换结束中断，若不需要开启中断，可调用csi_dac_int_disable接口函数，关闭转换结束中断
 * 	 \brief DAC可以选择PA4或者PA8作为输出口，BUF enable时，PA4输出，BUF disable时，PA8输出
 *  \param[in] none
 *  \return 0/-1
 */
int dac_callback_demo(void)
{
	int iRet = 0;

	//dac 输出管脚配置
	csi_gpio_set_mux(GPIOA,PA4,PA4_DAC0_OUT);	
	//dac 参数配置初始化
	csi_dac_config_t tDacConfig;
	tDacConfig.byClkDiv 	= 0x02;				//DAC clk两分频：FCK = FHCLK / 2
	tDacConfig.bRefsel	 	= DISABLE;			//DAC 参考电平选择
	tDacConfig.hwDatarset 	= 0x00;				//DAC 电平码值设置
	tDacConfig.bBufen 		= ENABLE;			//DAC BUF enable时，PA4输出，BUF disable时，PA8输出	
	csi_dac_init(DAC0, &tDacConfig);
	
	csi_dac_enable(DAC0);	
	csi_dac_int_enable(DAC0, DAC_INTSRC_EOC);		//使能EOC中断
	csi_dac_register_callback(DAC0, dac_callback);	//注册中断回调函数
	
	csi_dac_start(DAC0);
	while(1)
	{
		mdelay(100);
	}
	return iRet;
}
#endif
