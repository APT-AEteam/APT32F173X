/***********************************************************************//** 
 * \file  adc_demo_callback.c
 * \brief ADC_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-6-11 <td>V0.0 <td>ZJY     <td>initial
 * <tr><td> 2023-9-19 <td>V0.1 <td>XS      <td>fix bug,code normalization
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include "csi_drv.h"
#include "board_config.h"

/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

#if (USE_ADC_CALLBACK == 1)									

/** \brief  user_adc0_callback：ADC0中断回调函数
 * 
 * 	\brief	用户定义，支持各种中断处理，使用csi标准库，中断发生时会自动调用用户注册的回调函
 * 			数，用户可在回调函数里做自己的处理，而不需要关注具体的底层中断处理。
 * 
 *  \param[in]  ptAdcBase: 	ADCx寄存器结构体指针，指向ADCx的基地址 
 *  \param[out] byIsr: 		ADCx中断状态
 *  \return none
 */ 
void user_adc0_callback(csp_bt_t *ptAdcBase,  uint32_t wIsr)//seqend判定
{
	if(wIsr & ADC_INTSRC_SEQ0)
	{
		csi_adc_read_channel(ADC0, 0);							//seq end中断清除为读清
	}
	if(wIsr & ADC_INTSRC_SEQ1)
	{
		csi_adc_read_channel(ADC0, 1);							//seq end中断清除为读清
	}
	if(wIsr & ADC_INTSRC_SEQ2)
	{
		csi_adc_read_channel(ADC0, 2);							//seq end中断清除为读清
	}
}

/** \brief ADC连续采样模式
 *  \brief ADC采样，轮询、连续转换模式。
 *  \brief 连续转换模式：ADC启动后进行整个序列采样，序列采样完成后，继续从序列第一个通道开始，如此循环。采样期间可以停止
 * 
 *  \param[in] none
 *  \return error code
 */
void adc_continuous_callback_demo(void)
{
	volatile int iChnlNum = 3;									//设置总通道个数	

	csi_adc_config_t tAdcConfig= {0};
#if (USE_GUI == 0)	
	//adc 输入管脚配置
	csi_gpio_set_mux(GPIOC,PC13, PC13_ADC_INA0);				//ADC GPIO作为输入通道
	csi_gpio_set_mux(GPIOC,PA14, PC14_ADC_INA1);
	csi_gpio_set_mux(GPIOC,PA15, PC15_ADC_INA2);
#endif		
	//adc 参数配置初始化
	tAdcConfig.byClkDiv = 0x02;									//ADC clk两分频：clk = pclk/2
	tAdcConfig.eClkSel = ADC_CLK_PCLK;							//ADC clk选择：PCLK
	tAdcConfig.bySampHold = 0x06;								//ADC 采样时间： time = 16 + 6 = 22(ADC clk周期)
	tAdcConfig.eRunMode = ADC_RUN_CONT;							//ADC 转换模式： 单次转换；
	tAdcConfig.eVrefSrc = ADCVERF_VDD_VSS;						//ADC 参考电压： 系统VDD	
	csi_adc_init(ADC0, &tAdcConfig);							//初始化ADC参数配置	
	
	csi_adc_set_seq_num(ADC0,iChnlNum);													//配置ADC总采样通道个数
	csi_adc_set_seqx(ADC0,0,ADC_INA0,ADC_CV_COUNT_1,ADC_AVG_COF_1,ADCSYNC_NONE);		//配置ADC采样通道0
	csi_adc_set_seqx(ADC0,1,ADC_INA1,ADC_CV_COUNT_1,ADC_AVG_COF_1,ADCSYNC_NONE);		//配置ADC采样通道1
	csi_adc_set_seqx(ADC0,2,ADC_INA2,ADC_CV_COUNT_1,ADC_AVG_COF_1,ADCSYNC_NONE);		//配置ADC采样通道2
	
	csi_adc_int_enable(ADC0, ADC_INTSRC_SEQ0|ADC_INTSRC_SEQ1|ADC_INTSRC_SEQ2);			//设置采样通道0，1，2的转换完成中断	
	csi_adc_register_callback(ADC0, user_adc0_callback);								//注册中断回调函数
	csi_adc_start(ADC0);																//启动ADC
}

#endif