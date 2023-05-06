/***********************************************************************//** 
 * \file  adc_test.c
 * \brief  ADC_TEST description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-4-28 <td>V0.0 <td>xs     <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include "sys_clk.h"
#include <drv/adc.h>
#include "pin.h"
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

/** \brief 用于测试ADC工作电流
 * 直接在调试模式下打开关闭ADC模块
 *  \param[in] none
 *  \return none
 */

void adc_work_current_test(void)
{	
	const csi_adc_seq_t tSeqCfg[] =
	{
	//输入通道		//连续重复采样次数		//平均系数			//触发源选择
	{ADCIN0,		ADC_CV_COUNT_1,			ADC_AVG_COF_1,		ADCSYNC_NONE},
}; 

	csi_adc_config_t tAdcConfig;
	
	uint8_t byChnlNum = sizeof(tSeqCfg)/sizeof(tSeqCfg[0]);
	uint16_t	g_hwAdcBuf[sizeof(tSeqCfg)/sizeof(tSeqCfg[0])];
	uint16_t DATAADC;
	//adc 输入管脚配置
	csi_pin_set_mux(PC13, PC13_ADC_INA0);						//ADC GPIO作为输入通道
//	csi_pin_set_mux(PC11, PC11_ADC_INA8);

	tAdcConfig.byClkDiv = 2;									//ADC clk两分频：clk = pclk/2
	tAdcConfig.bySampHold = 0x06;								//ADC 采样时间： time = 16 + 6 = 22(ADC clk周期)
	tAdcConfig.byConvMode = ADC_CONV_CONTINU;					//ADC 转换模式： 连续转换；
	tAdcConfig.byVrefSrc = ADCVERF_VDD_VSS;						//ADC 参考电压： 系统VDD
	tAdcConfig.wInt = ADC_INTSRC_NONE;								//ADC 中断配置：SEQ_END(0~2)中断使能，默认(推荐使用)SEQ_END中断
	tAdcConfig.ptSeqCfg = (csi_adc_seq_t *)tSeqCfg;				//ADC 采样序列： 具体参考结构体变量 tSeqCfg
	
	csi_adc_init(ADC0, &tAdcConfig);							//初始化ADC参数配置	
	csi_adc_set_seqx(ADC0, tAdcConfig.ptSeqCfg, byChnlNum);		//配置ADC采样序列
	csi_adc_start(ADC0);										//启动ADC
}

/** \brief 用于测试ADC工作电压
 * 使用串口查看打印值并逐渐降低芯片电压，直到打印值误差过大或者芯片无法正常工作
 *  \param[in] none
 *  \return none
 */
void adc_work_voltage_test(void)
{	
	const csi_adc_seq_t tSeqCfgDj[] =
	{
	//输入通道		//连续重复采样次数		//平均系数			//触发源选择
	{ADCIN0,		ADC_CV_COUNT_1,			ADC_AVG_COF_1,		ADCSYNC_NONE},
}; 

	csi_adc_config_t tAdcConfig;
	
	uint8_t byChnlNum = sizeof(tSeqCfgDj)/sizeof(tSeqCfgDj[0]);
	uint16_t	g_hwAdcBuf[sizeof(tSeqCfgDj)/sizeof(tSeqCfgDj[0])];
	uint16_t DATAADC;
	//adc 输入管脚配置
	csi_pin_set_mux(PC13, PC13_ADC_INA0);						//ADC GPIO作为输入通道
//	csi_pin_set_mux(PC11, PC11_ADC_INA8);

	tAdcConfig.byClkDiv = 2;									//ADC clk两分频：clk = pclk/2
	tAdcConfig.bySampHold = 0x06;								//ADC 采样时间： time = 16 + 6 = 22(ADC clk周期)
	tAdcConfig.byConvMode = ADC_CONV_CONTINU;					//ADC 转换模式： 连续转换；
	tAdcConfig.byVrefSrc = ADCVERF_VDD_VSS;						//ADC 参考电压： 系统VDD
	tAdcConfig.wInt = ADC_INTSRC_NONE;								//ADC 中断配置：SEQ_END(0~2)中断使能，默认(推荐使用)SEQ_END中断
	tAdcConfig.ptSeqCfg = (csi_adc_seq_t *)tSeqCfgDj;				//ADC 采样序列： 具体参考结构体变量 tSeqCfg
	
	csi_adc_init(ADC0, &tAdcConfig);							//初始化ADC参数配置	
	csi_adc_set_seqx(ADC0, tAdcConfig.ptSeqCfg, byChnlNum);		//配置ADC采样序列
	csi_adc_start(ADC0);										//启动ADC
	
	while(1)
	{
		//csi_adc_start(ADC0);  //single pen his
		DATAADC = csi_adc_read_channel(ADC0, 0);
		my_printf("%d ",DATAADC); //	
	}
}

/** \brief 用于测试ADC精度
 * 配置ADC采样次数后，使用串口查看打印值
 *  \param[in] none
 *  \return none
 */
 void adc_accuracy_test(void)
{	
	const csi_adc_seq_t tSeqCfgDj[] =
	{
	//输入通道		//连续重复采样次数		//平均系数			//触发源选择
	{ADCIN0,		ADC_CV_COUNT_1,			ADC_AVG_COF_1,		ADCSYNC_NONE},
}; 
	csi_adc_config_t tAdcConfig;
	
	uint8_t byChnlNum = sizeof(tSeqCfgDj)/sizeof(tSeqCfgDj[0]);
	uint16_t	g_hwAdcBuf[sizeof(tSeqCfgDj)/sizeof(tSeqCfgDj[0])];
	uint16_t DATAADC;
	//adc 输入管脚配置
	csi_pin_set_mux(PC13, PC13_ADC_INA0);						//ADC GPIO作为输入通道
//	csi_pin_set_mux(PC11, PC11_ADC_INA8);

	tAdcConfig.byClkDiv = 2;									//ADC clk两分频：clk = pclk/2
	tAdcConfig.bySampHold = 0x06;								//ADC 采样时间： time = 16 + 6 = 22(ADC clk周期)
	tAdcConfig.byConvMode = ADC_CONV_CONTINU;					//ADC 转换模式： 连续转换；
	tAdcConfig.byVrefSrc = ADCVERF_VDD_VSS;						//ADC 参考电压： 系统VDD
	tAdcConfig.wInt = ADC_INTSRC_NONE;								//ADC 中断配置：SEQ_END(0~2)中断使能，默认(推荐使用)SEQ_END中断
	tAdcConfig.ptSeqCfg = (csi_adc_seq_t *)tSeqCfgDj;				//ADC 采样序列： 具体参考结构体变量 tSeqCfg
	
	csi_adc_init(ADC0, &tAdcConfig);							//初始化ADC参数配置	
	csi_adc_set_seqx(ADC0, tAdcConfig.ptSeqCfg, byChnlNum);		//配置ADC采样序列
	csi_adc_start(ADC0);										//启动ADC
	
	while(1)
	{
		//csi_adc_start(ADC0);  //single pen his
		DATAADC = csi_adc_read_channel(ADC0, 0);
		my_printf("%d ",DATAADC); //	
	}
}

/** \brief 用于测试ADC转换速度
 * 使用示波器抓取IO口翻转间隔时间，除以512即为转换速度
 *  \param[in] none
 *  \return none
 */
 void adc_speed_test(void)
{
	csi_adc_config_t tAdcConfig;
	const csi_adc_seq_t SeqCfg3[] =
{
	//输入通道		//连续重复采样次数		//平均系数			//触发源选择
	{ADCIN0,		ADC_CV_COUNT_512,			ADC_CV_COUNT_512,		ADCSYNC_NONE},
};
	//adc 输入管脚配置
	csi_pin_set_mux(PC13, PC13_ADC_INA0);//pin39                            
	
	//adc 参数配置初始化
	tAdcConfig.byClkDiv = 0x02;									//ADC clk两分频：clk = pclk/2
	tAdcConfig.bySampHold = 0x00;								//ADC 采样时间： time = 16 + 6 = 22(ADC clk周期)
	tAdcConfig.byConvMode = ADC_CONV_ONESHOT;					//ADC 转换模式： 单次转换；
	tAdcConfig.byVrefSrc = ADCVERF_VDD_VSS;						//ADC 参考电压： 系统VDD
	tAdcConfig.wInt = ADC_INTSRC_NONE;     //中断设置
	tAdcConfig.ptSeqCfg = (csi_adc_seq_t *)SeqCfg3;		        //ADC 采样序列： 具体参考结构体变量 SeqCfg3	
	csi_adc_init(ADC0, &tAdcConfig);//初始化ADC参数配置	
	csi_adc_set_seqx(ADC0, tAdcConfig.ptSeqCfg, 1);	 //配置ADC采样序列
	uint16_t hwData = 0;
	csi_pin_set_mux(PA5,PA5_OUTPUT);
	csi_pin_set_low(PA5);	
	ADC0->ECR |= 0x1 << 16;
	GPIOA->SODR = 0x20;//pa0.5 high
	ADC0->CR |= 0x1 << ADC12_START;
	while(!(ADC0->SR & ADC12_SEQ(0)));
	GPIOA->CODR = 0x20;//pa0.5 high	
	csi_clo_config(CLO_PCLK, CLO_DIV8,PD4);
	while(1)
	{	
	delay_ums(100);
	}
}

/** \brief 用于测试ADC偏移误差
 * 使用串口查看打印值
 *  \param[in] none
 *  \return none
 */
 void adc_offset_test(void)
{		
	uint16_t hwData[10] = {0};
	uint32_t wSum = 0;
	uint32_t wAve = 0;
	uint32_t wMin = 8096;
	uint32_t wMax = 0;
	uint8_t byTimes =20;
	csi_adc_config_t tAdcConfig;
	const csi_adc_seq_t SeqCfg3[] =
{
	//输入通道		//连续重复采样次数		//平均系数			//触发源选择
	{ADCIN0,		ADC_CV_COUNT_512,			ADC_CV_COUNT_512,		ADCSYNC_NONE},
};
	//adc 输入管脚配置
	csi_pin_set_mux(PC13, PC13_ADC_INA0);//pin39                            
	
	//adc 参数配置初始化
	tAdcConfig.byClkDiv = 0x02;									//ADC clk两分频：clk = pclk/2
	tAdcConfig.bySampHold = 0x06;								//ADC 采样时间： time = 16 + 6 = 22(ADC clk周期)
	tAdcConfig.byConvMode = ADC_CONV_ONESHOT;					//ADC 转换模式： 单次转换；
	tAdcConfig.byVrefSrc = ADCVERF_VDD_VSS;						//ADC 参考电压： 系统VDD
	tAdcConfig.wInt = ADC_INTSRC_NONE;     //中断设置
	tAdcConfig.ptSeqCfg = (csi_adc_seq_t *)SeqCfg3;		        //ADC 采样序列： 具体参考结构体变量 SeqCfg3	
	csi_adc_init(ADC0, &tAdcConfig);//初始化ADC参数配置	
	csi_adc_set_seqx(ADC0, tAdcConfig.ptSeqCfg, 1);	 //配置ADC采样序列
	while(1)
	{
		for(uint8_t i =0;i<byTimes;i++)
		{
			csi_adc_start(ADC0);
			hwData[i] = csi_adc_read_channel(ADC0, 0);
			 if(wMin > hwData[i])
			 {
				 wMin = hwData[i];
			 }
			 
			 if(wMax < hwData[i])
			 {
				 wMax = hwData[i];
			 }
			 
			 wSum = wSum + hwData[i];
		}
		 wAve = wSum/byTimes;
		
		my_printf("the min data is: %d\n",wMin);
		my_printf("the max data is: %d\n",wMax);
		my_printf("the sum data is: %d\n",wSum);
		my_printf("the ave data is: %d\n\n",wAve);
		
		wSum = 0;
		wAve = 0;
		wMin = 8096;
		wMax = 0;
		mdelay(1000);
		
	}		
}

/** \brief 用于测试ADC外部信号源电阻
 * 使用串口查看打印值，直到采样值相对误差不能稳定在1-3个LSB
 *  \param[in] none
 *  \return none
 */
 void adc_rain_test(void)
{	
	const csi_adc_seq_t tSeqCfgDj[] =
	{
	//输入通道		//连续重复采样次数		//平均系数			//触发源选择
	{ADCIN0,		ADC_CV_COUNT_1,			ADC_AVG_COF_1,		ADCSYNC_NONE},
}; 

	csi_adc_config_t tAdcConfig;
	
	uint8_t byChnlNum = sizeof(tSeqCfgDj)/sizeof(tSeqCfgDj[0]);
	uint16_t	g_hwAdcBuf[sizeof(tSeqCfgDj)/sizeof(tSeqCfgDj[0])];
	uint16_t DATAADC;
	//adc 输入管脚配置
	csi_pin_set_mux(PC13, PC13_ADC_INA0);						//ADC GPIO作为输入通道
//	csi_pin_set_mux(PC11, PC11_ADC_INA8);

	tAdcConfig.byClkDiv = 2;									//ADC clk两分频：clk = pclk/2
	tAdcConfig.bySampHold = 0x06;								//ADC 采样时间： time = 16 + 6 = 22(ADC clk周期)
	tAdcConfig.byConvMode = ADC_CONV_CONTINU;					//ADC 转换模式： 连续转换；
	tAdcConfig.byVrefSrc = ADCVERF_VDD_VSS;						//ADC 参考电压： 系统VDD
	tAdcConfig.wInt = ADC_INTSRC_NONE;								//ADC 中断配置：SEQ_END(0~2)中断使能，默认(推荐使用)SEQ_END中断
	tAdcConfig.ptSeqCfg = (csi_adc_seq_t *)tSeqCfgDj;				//ADC 采样序列： 具体参考结构体变量 tSeqCfg
	
	csi_adc_init(ADC0, &tAdcConfig);							//初始化ADC参数配置	
	csi_adc_set_seqx(ADC0, tAdcConfig.ptSeqCfg, byChnlNum);		//配置ADC采样序列
	csi_adc_start(ADC0);										//启动ADC
	
	while(1)
	{
		//csi_adc_start(ADC0);  //single pen his
		DATAADC = csi_adc_read_channel(ADC0, 0);
		my_printf("%d ",DATAADC); //	
	}
}

/** \brief 用于测试ADC INL和DNL，详细测试方法见知识库
 * 
 *  \param[in] none
 *  \return none
 */
  void adc_inl_test(void)
{	
	const csi_adc_seq_t tSeqCfgDj[] =
	{
	//输入通道		//连续重复采样次数		//平均系数			//触发源选择
	{ADCIN0,		ADC_CV_COUNT_512,			ADC_AVG_COF_512,		ADCSYNC_NONE},
}; 

	csi_adc_config_t tAdcConfig;
	
	uint8_t byChnlNum = sizeof(tSeqCfgDj)/sizeof(tSeqCfgDj[0]);
	uint16_t	g_hwAdcBuf[sizeof(tSeqCfgDj)/sizeof(tSeqCfgDj[0])];
	uint16_t DATAADC;
	//adc 输入管脚配置
	csi_pin_set_mux(PC13, PC13_ADC_INA0);						//ADC GPIO作为输入通道
//	csi_pin_set_mux(PC11, PC11_ADC_INA8);

	tAdcConfig.byClkDiv = 2;									//ADC clk两分频：clk = pclk/2
	tAdcConfig.bySampHold = 0x06;								//ADC 采样时间： time = 16 + 6 = 22(ADC clk周期)
	tAdcConfig.byConvMode = ADC_CONV_CONTINU;					//ADC 转换模式： 连续转换；
	tAdcConfig.byVrefSrc = ADCVERF_VDD_VSS;						//ADC 参考电压： 系统VDD
	tAdcConfig.wInt = ADC_INTSRC_NONE;								//ADC 中断配置：SEQ_END(0~2)中断使能，默认(推荐使用)SEQ_END中断
	tAdcConfig.ptSeqCfg = (csi_adc_seq_t *)tSeqCfgDj;				//ADC 采样序列： 具体参考结构体变量 tSeqCfg
	
	csi_adc_init(ADC0, &tAdcConfig);							//初始化ADC参数配置	
	csi_adc_set_seqx(ADC0, tAdcConfig.ptSeqCfg, byChnlNum);		//配置ADC采样序列
	csi_adc_start(ADC0);										//启动ADC
	
	while(1)
	{
		//csi_adc_start(ADC0);  //single pen his
		DATAADC = csi_adc_read_channel(ADC0, 0);
		my_printf("%d ",DATAADC); //	
	}
}
