/***********************************************************************//** 
 * \file  adc_demo.c
 * \brief ADC_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-6-7 <td>V0.0  <td>ZJY     <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include <drv/adc.h>
#include <drv/pin.h>
#include "drv/etcb.h"
#include "board_config.h"
/* externs function--------------------------------------------------------*/
//ADC通道采样深度(每通道采样数据次数)，连续转换模式时支持
//单次转换模式时，需将次参数配置为1	
#define		ADC_DATA_DEPTH		0x0a	
	
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

//ADC采样序列通道参数配置，默认情况，重复采样和平均系数为1(ADC采样值不平均)
//ADC触发根据实际应用进行配置
const csi_adc_seq_t tSeqCfg[] =
{
	//输入通道		//连续重复采样次数		//平均系数			//触发源选择
	{ADCIN0,		ADC_CV_COUNT_1,			ADC_AVG_COF_1,		ADCSYNC_NONE},
	//{ADCIN1,		ADC_CV_COUNT_1,			ADC_AVG_COF_1,		ADCSYNC_IN1},
	//{ADCIN2,		ADC_CV_COUNT_1,			ADC_AVG_COF_1,		ADCSYNC_IN1},

}; 

//采样序列的通道数
volatile uint8_t byChnlNum = sizeof(tSeqCfg)/sizeof(tSeqCfg[0]);

//ADC value buffer
//连续转换模式，支持采样深度设置(每通道采样N次), N = ADC_DATA_DEPTH
//单次转换模式，ADC_DATA_DEPTH = 1
#if ADC_DATA_DEPTH > 1
	volatile uint16_t	g_hwAdcBuf[sizeof(tSeqCfg)/sizeof(tSeqCfg[0])][ADC_DATA_DEPTH];
#else
	volatile uint16_t	g_hwAdcBuf[sizeof(tSeqCfg)/sizeof(tSeqCfg[0])];
#endif


/** \brief ADC sample,one shot mode, ADC_DATA_DEPTH = 1
 *  \brief ADC采样，轮询、单次转换模式。ADC通道采样深度为1（每通道采样次数为1）
 *  \brief 单次转换模式：ADC启动后进行整个序列采样直到结束，中间不能停止；进行下一次序列转换需重新启动
 * 
 *  \param[in] none
 *  \return error code
 */
int adc_samp_oneshot_demo(void)
{
	int iRet = 0;
	csi_adc_config_t tAdcConfig;
#if !defined(USE_GUI)	
	//adc 输入管脚配置
	csi_pin_set_mux(PC13, PC13_ADC_INA0);				//ADC GPIO作为输入通道
//	csi_pin_set_mux(PA010, PA010_ADC_AIN11);
//	csi_pin_set_mux(PA011, PA011_ADC_AIN12);
#endif		
	//adc 参数配置初始化
	tAdcConfig.byClkDiv = 0x02;							//ADC clk两分频：clk = pclk/2
	tAdcConfig.byClksel = ADC_PCLK;						//ADC clk选择：PCLK
	tAdcConfig.bySampHold = 0x06;						//ADC 采样时间： time = 16 + 6 = 22(ADC clk周期)
	tAdcConfig.byConvMode = ADC_CONV_ONESHOT;			//ADC 转换模式： 单次转换；
	tAdcConfig.byVrefSrc = ADCVERF_VDD_VSS;				//ADC 参考电压： 系统VDD
	tAdcConfig.wInt = ADC_INTSRC_NONE;				//ADC 中断配置： 无中断
	tAdcConfig.ptSeqCfg = (csi_adc_seq_t *)tSeqCfg;		//ADC 采样序列： 具体参考结构体变量 tSeqCfg
	
	csi_adc_init(ADC0, &tAdcConfig);							//初始化ADC参数配置	
	csi_adc_set_seqx(ADC0, tAdcConfig.ptSeqCfg, byChnlNum);		//配置ADC采样序列
	csi_adc_start(ADC0);										//启动ADC
	while(0);
	
	return iRet;
}

/** \brief ADC sample, countinuous mode ADC_DATA_DEPTH = N（N > 0）
 *  \brief ADC采样，轮询、连续转换模式。ADC通道采样深度为N（每通道采样次数为N）
 *  \brief 连续转换模式：ADC启动后进行整个序列采样，序列采样完成后，继续从序列第一个通道开始，如此循环。采样期间可以停止
 * 
 *  \param[in] none
 *  \return error code
 */
int adc_samp_continuous_demo(void)
{
	int iRet = 0;
	
	csi_adc_config_t tAdcConfig;
	
#if !defined(USE_GUI)	
	//adc 输入管脚配置
	csi_pin_set_mux(PC13, PC13_ADC_INA0);				//ADC GPIO作为输入通道
//	csi_pin_set_mux(PA010, PA010_ADC_AIN11);
//	csi_pin_set_mux(PA011, PA011_ADC_AIN12);
#endif	
	//adc 参数配置初始化
	tAdcConfig.byClkDiv = 0x02;							//ADC clk两分频：clk = pclk/2
	tAdcConfig.byClksel = ADC_PCLK;						//ADC clk选择：PCLK
	tAdcConfig.bySampHold = 0x06;						//ADC 采样时间： time = 16 + 6 = 22(ADC clk周期)
	tAdcConfig.byConvMode = ADC_CONV_CONTINU;			//ADC 转换模式： 连续转换
	tAdcConfig.byVrefSrc = ADCVERF_VDD_VSS;				//ADC 参考电压： 系统VDD
	tAdcConfig.wInt = ADC_INTSRC_NONE;				//ADC 中断配置： 无中断
	tAdcConfig.ptSeqCfg = (csi_adc_seq_t *)tSeqCfg;		//ADC 采样序列： 具体参考结构体变量 tSeqCfg
	
	csi_adc_init(ADC0, &tAdcConfig);							//初始化ADC参数配置	
	csi_adc_set_seqx(ADC0, tAdcConfig.ptSeqCfg, byChnlNum);		//配置ADC采样序列
	csi_adc_set_buffer((uint16_t *)g_hwAdcBuf, ADC_DATA_DEPTH);	//传递ADC采样buffer，ADC采样值存放于此buffer中
	csi_adc_start(ADC0);										//启动ADC
	
	return iRet;
}


/** \brief ADC sample,one shot mode, ADC_DATA_DEPTH = 1
 *  \brief ADC采样，中断、单次转换模式。ADC通道采样深度为1（每通道采样次数为1）
 *  \brief 单次转换模式：ADC启动后进行整个序列采样直到结束，中间不能停止；进行下一次序列转换需重新启动
 * 
 *  \param[in] none
 *  \return error code
 */
int adc_samp_oneshot_int_demo(void)
{
	int iRet = 0;
	csi_adc_config_t tAdcConfig;
#if !defined(USE_GUI)	
	//adc 输入管脚配置
	csi_pin_set_mux(PC13, PC13_ADC_INA0);				//ADC GPIO作为输入通道
//	csi_pin_set_mux(PA010, PA010_ADC_AIN11);
//	csi_pin_set_mux(PA011, PA011_ADC_AIN12);
#endif	
	//adc 参数配置初始化
	tAdcConfig.byClkDiv = 48;									//ADC clk两分频：clk = pclk/2
	tAdcConfig.byClksel = ADC_PCLK;								//ADC clk选择：PCLK
	tAdcConfig.bySampHold = 0x06;								//ADC 采样时间： time = 16 + 6 = 22(ADC clk周期)
	tAdcConfig.byConvMode =ADC_CONV_ONESHOT;                    //ADC 转换模式： 单次转换；ADC_CONV_CONTINU
	tAdcConfig.byVrefSrc = ADCVERF_VDD_VSS;						//ADC 参考电压： 系统VDD
	tAdcConfig.wInt = ADC_INTSRC_SEQ0|ADC_INTSRC_SEQ1|ADC_INTSRC_SEQ2;		//ADC 中断配置：SEQ_END(0~2)中断使能，默认(推荐使用)SEQ_END中断
	tAdcConfig.ptSeqCfg = (csi_adc_seq_t *)tSeqCfg;				//ADC 采样序列： 具体参考结构体变量 tSeqCfg
	
	csi_adc_init(ADC0, &tAdcConfig);							//初始化ADC参数配置	
	csi_adc_set_seqx(ADC0, tAdcConfig.ptSeqCfg, byChnlNum);		//配置ADC采样序列
	csi_adc_set_buffer((uint16_t *)g_hwAdcBuf, 1);				//传递ADC采样buffer，ADC采样值存放于此buffer中
	csi_adc_start(ADC0);										//启动ADC

	return iRet;
}

/** \brief ADC sample, countinuous mode ADC_DATA_DEPTH = N（N > 0）
 *  \brief ADC采样，轮询、连续转换模式。ADC通道采样深度为N（每通道采样次数为N）
 *  \brief 连续转换模式：ADC启动后进行整个序列采样，序列采样完成后，继续从序列第一个通道开始，如此循环。采样期间可以停止
 * 
 *  \param[in] none
 *  \return error code
 */
int adc_samp_continuous_int_demo(void)
{
	int iRet = 0;
	csi_adc_config_t tAdcConfig;
	
#if !defined(USE_GUI)	
	//adc 输入管脚配置
	csi_pin_set_mux(PC13, PC13_ADC_INA0);				//ADC GPIO作为输入通道
//	csi_pin_set_mux(PA010, PA010_ADC_AIN11);
//	csi_pin_set_mux(PA011, PA011_ADC_AIN12);
#endif	
	//adc 参数配置初始化
	tAdcConfig.byClkDiv = 8;									//ADC clk两分频：clk = pclk/2
	tAdcConfig.byClksel = ADC_PCLK;								//ADC clk选择：PCLK
	tAdcConfig.bySampHold = 0x06;								//ADC 采样时间： time = 16 + 6 = 22(ADC clk周期)
	tAdcConfig.byConvMode = ADC_CONV_CONTINU;					//ADC 转换模式： 连续转换；
	tAdcConfig.byVrefSrc = ADCVERF_VDD_VSS;						//ADC 参考电压： 系统VDD
	tAdcConfig.wInt = ADC_INTSRC_SEQ0 | ADC_INTSRC_SEQ1 |
				ADC_INTSRC_SEQ2;								//ADC 中断配置：SEQ_END(0~2)中断使能，默认(推荐使用)SEQ_END中断
	tAdcConfig.ptSeqCfg = (csi_adc_seq_t *)tSeqCfg;				//ADC 采样序列： 具体参考结构体变量 tSeqCfg
	
	csi_adc_init(ADC0, &tAdcConfig);							//初始化ADC参数配置	
	csi_adc_set_seqx(ADC0, tAdcConfig.ptSeqCfg, byChnlNum);		//配置ADC采样序列
	csi_adc_set_buffer((uint16_t *)g_hwAdcBuf, ADC_DATA_DEPTH);	//传递ADC采样buffer，ADC采样值存放于此buffer中
	csi_adc_start(ADC0);										//启动ADC
	
	return iRet;
}

/** \brief adc interrupt handle function
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \return none
 */ 
__attribute__((weak)) void adc_irqhandler(csp_adc_t *ptAdcBase)
{
	uint32_t wIntStat = csp_adc_get_isr(ptAdcBase);//ADC_MISR
	if (wIntStat != 0) 
	{
		csp_adc_clr_sr(ptAdcBase,wIntStat);
	}
}

