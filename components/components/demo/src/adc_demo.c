/***********************************************************************//** 
 * \file  adc_demo.c
 * \brief ADC_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-6-7 <td>V0.0  <td>ZJY     <td>initial
 * <tr><td> 2023-9-19<td>V0.1  <td>XS    <td>fix bug,code normalization
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include "csi_drv.h"
#include "board_config.h"
#include <iostring.h>
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

#if (USE_ADC_CALLBACK == 0)	
/** \brief	adcx_int_handler: ADC中断服务函数
 * 
 *  \brief 	ADC发生中断时会调用此函数，函数在interrupt.c里定义为弱(weak)属性，默认不做处理；用户用到ADC中
 * 			断时，请重新定义此函数，在此函数中进行对应中断处理，也可直接在interrupt.c里的函数里进行处理
 * 
 *  \param[in] none
 *  \return none
 */
ATTRIBUTE_ISR void adc0_int_handler(void)
{
	//用户直接在中断服务接口函数里处理中断，建议客户使用此模式
	csi_adc_read_channel(ADC0, 0);				//seq end中断清除为读清
	csi_adc_read_channel(ADC0, 1);				//seq end中断清除为读清
	csi_adc_read_channel(ADC0, 2);				//seq end中断清除为读清	
	csp_adc_clr_sr(ADC0, csp_adc_get_isr(ADC0));//清除seq和eoc以外的中断
}
#endif

/** \brief ADC单次采样模式
 *  \brief ADC采样，轮询、单次转换模式。
 *  \brief 单次转换模式：ADC启动后进行整个序列采样直到结束，中间不能停止；进行下一次序列转换需重新启动
 * 
 *  \param[in] none
 *  \return error code
 */
void adc_samp_oneshot_demo(void)
{
	uint8_t i;
	int iChnlNum = 3;											//设置总通道个数	
	volatile int16_t nDataBuf[3] = {0,0,0};						//存放三通道采样值

	csi_adc_config_t tAdcConfig;
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
	tAdcConfig.eRunMode = ADC_RUN_ONCE;							//ADC 转换模式： 单次转换；
	tAdcConfig.eVrefSrc = ADCVERF_VDD_VSS;						//ADC 参考电压： 系统VDD	
	csi_adc_init(ADC0, &tAdcConfig);							//初始化ADC参数配置	
	
	csi_adc_set_seq_num(ADC0,iChnlNum);													//配置ADC总采样通道个数
	csi_adc_set_seqx(ADC0,0,ADC_INA0,ADC_CV_COUNT_1,ADC_AVG_COF_1,ADCSYNC_NONE);		//配置ADC采样通道0
	csi_adc_set_seqx(ADC0,1,ADC_INA1,ADC_CV_COUNT_1,ADC_AVG_COF_1,ADCSYNC_NONE);		//配置ADC采样通道1
	csi_adc_set_seqx(ADC0,2,ADC_INA2,ADC_CV_COUNT_1,ADC_AVG_COF_1,ADCSYNC_NONE);		//配置ADC采样通道2
	csi_adc_start(ADC0);																//启动ADC
	
	while(1)
	{
		for(i = 0; i < iChnlNum; i++)
		{
			nDataBuf[i] = csi_adc_read_channel(ADC0, i);					//分别读ADC采样序列通道：0~iChnlNum			
			my_printf("ADC channel value of seq: %d \n", nDataBuf[i]);		//串口打印采样值
		}	
	}
}

/** \brief ADC连续采样模式
 *  \brief ADC采样，轮询、连续转换模式。
 *  \brief 连续转换模式：ADC启动后进行整个序列采样，序列采样完成后，继续从序列第一个通道开始，如此循环。采样期间可以停止
 * 
 *  \param[in] none
 *  \return error code
 */
void adc_samp_continuous_demo(void)
{	
	uint8_t i;
	int iChnlNum = 3;											//设置总通道个数	
	volatile int16_t nDataBuf[3] = {0,0,0};						//存放三通道采样值
	
	csi_adc_config_t tAdcConfig;	
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
	tAdcConfig.eRunMode = ADC_RUN_CONT;							//ADC 转换模式： 连续转换
	tAdcConfig.eVrefSrc = ADCVERF_VDD_VSS;						//ADC 参考电压： 系统VDD	
	csi_adc_init(ADC0, &tAdcConfig);							//初始化ADC参数配置	
	
	csi_adc_set_seq_num(ADC0,iChnlNum);													//配置ADC总采样通道个数
	csi_adc_set_seqx(ADC0,0,ADC_INA0,ADC_CV_COUNT_1,ADC_AVG_COF_1,ADCSYNC_NONE);		//配置ADC采样通道0
	csi_adc_set_seqx(ADC0,1,ADC_INA1,ADC_CV_COUNT_1,ADC_AVG_COF_1,ADCSYNC_NONE);		//配置ADC采样通道1
	csi_adc_set_seqx(ADC0,2,ADC_INA2,ADC_CV_COUNT_1,ADC_AVG_COF_1,ADCSYNC_NONE);		//配置ADC采样通道2
	csi_adc_start(ADC0);																//启动ADC

	while(1)
	{
		for(i = 0; i < iChnlNum; i++)
		{
			nDataBuf[i] = csi_adc_read_channel(ADC0, i);					//分别读ADC采样序列通道：0~iChnlNum			
			my_printf("ADC channel value of seq: %d \n", nDataBuf[i]);		//串口打印采样值
		}	
	}
}


/** \brief ADC中断单次模式
 *  \brief ADC采样，中断、单次转换模式。
 *  \brief 单次转换模式：ADC启动后进行整个序列采样直到结束，中间不能停止；进行下一次序列转换需重新启动
 * 
 *  \param[in] none
 *  \return error code
 */
void adc_samp_oneshot_int_demo(void)
{
			int iChnlNum = 3;									//设置总通道个数	

	csi_adc_config_t tAdcConfig;
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
	tAdcConfig.eRunMode = ADC_RUN_ONCE;							//ADC 转换模式： 单次转换；
	tAdcConfig.eVrefSrc = ADCVERF_VDD_VSS;						//ADC 参考电压： 系统VDD	
	csi_adc_init(ADC0, &tAdcConfig);							//初始化ADC参数配置	
	
	csi_adc_set_seq_num(ADC0,iChnlNum);													//配置ADC总采样通道个数
	csi_adc_set_seqx(ADC0,0,ADC_INA0,ADC_CV_COUNT_1,ADC_AVG_COF_1,ADCSYNC_NONE);		//配置ADC采样通道0
	csi_adc_set_seqx(ADC0,1,ADC_INA1,ADC_CV_COUNT_1,ADC_AVG_COF_1,ADCSYNC_NONE);		//配置ADC采样通道1
	csi_adc_set_seqx(ADC0,2,ADC_INA2,ADC_CV_COUNT_1,ADC_AVG_COF_1,ADCSYNC_NONE);		//配置ADC采样通道2
	
	csi_adc_int_enable(ADC0, ADC_INTSRC_SEQ0|ADC_INTSRC_SEQ1|ADC_INTSRC_SEQ2);			//设置采样通道0，1，2的转换完成中断
	csi_adc_start(ADC0);																//启动ADC

}

/** \brief ADC中断连续模式
 *  \brief ADC采样，中断、连续转换模式。
 *  \brief 连续转换模式：ADC启动后进行整个序列采样，序列采样完成后，继续从序列第一个通道开始，如此循环。采样期间可以停止
 * 
 *  \param[in] none
 *  \return error code
 */
void adc_samp_continuous_int_demo(void)
{
	int iChnlNum = 3;											//设置总通道个数	

	csi_adc_config_t tAdcConfig;
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
	csi_adc_start(ADC0);																//启动ADC
}



