/***********************************************************************//** 
 * \file  adc_test.c
 * \brief ADC_TEST description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-5-6 <td>V0.0  <td>XS     <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include <drv/adc.h>
#include <drv/pin.h>
#include <iostring.h>
#include "drv/etb.h"
#include <drv/gpta.h>
#include <drv/dma.h>
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/
/** \brief adc interrupt handle function
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \return none
 */ 
__attribute__((weak)) void adc_irqhandler(csp_adc_t *ptAdcBase)
{
	uint32_t wIntStat = csp_adc_get_sr(ptAdcBase) & csp_adc_get_imr(ptAdcBase);//ADC_SR & ADC_IMR
	while (wIntStat != 0) 
	{
		csp_adc_clr_sr(ptAdcBase,wIntStat);
	}
}
//ADC通道采样深度(每通道采样数据次数)，连续转换模式时支持
//单次转换模式时，需将次参数配置为1	
#define		ADC_DATA_DEPTH		0x0a	

//ADC采样序列通道参数配置，默认情况，重复采样和平均系数为1(ADC采样值不平均)
//ADC触发根据实际应用进行配置
const csi_adc_seq_t tSeqCfg[] =
{
	//输入通道		//连续重复采样次数		//平均系数			//触发源选择
	{ADCIN1,		ADC_CV_COUNT_32,			ADC_AVG_COF_32,		ADCSYNC_NONE},
	//{ADCIN2,		ADC_CV_COUNT_1,			ADC_AVG_COF_1,		ADCSYNC_NONE},
	//{ADCIN3,		ADC_CV_COUNT_1,			ADC_AVG_COF_1,		ADCSYNC_NONE},
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

/** \brief ADC sample,one shot test, ADC_DATA_DEPTH = 1
 *  \brief ADC采样，轮询、单次转换模式。ADC通道采样深度为1（每通道采样次数为1）
 *  \brief 单次转换模式：ADC启动后进行整个序列采样直到结束，中间不能停止；进行下一次序列转换需重新启动
 * 
 *  \param[in] none
 *  \return error code
 */
int adc_samp_oneshot_test(void)
{
	int iRet = 0;
	uint8_t i;
	csi_adc_config_t tAdcConfig;	
	//adc 输入管脚配置
	csi_pin_set_mux(PC13, PC13_ADC_INA0);
	csi_pin_set_mux(PC14, PC14_ADC_INA1);
	csi_pin_set_mux(PC15, PC15_ADC_INA2);
	
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
	csi_adc_set_buffer((uint16_t *)g_hwAdcBuf, 1);				//传递ADC采样buffer，ADC采样值存放于此buffer中
	csi_adc_start(ADC0);										//启动ADC
	
	do
	{
		//读ADC采样序列，整个采样序列所有通道读到采样buffer 
		if(csi_adc_read_seqx(ADC0) == CSI_OK)			//采样通道读取完成，ADC value 保持在 buffer中 
		{
			for(i = 0; i < byChnlNum; i++)
				my_printf("ADC channel value of seq: %d \n", g_hwAdcBuf[i]);
		}
		else											//采样通道读取超时(异常)									
		{
			my_printf("ADC sample timeout...\n");
		}
		
		//单个通道分别读取
		iRet = csi_adc_start(ADC0);						//单次转换模式，序列转换完成后自动停止。若重新采样序列通道，需要重新启动ADC
		if(iRet < 0)
			my_printf("ADC start failure ...\n");
			
#if ADC_DATA_DEPTH < 2

		g_hwAdcBuf[0] = csi_adc_read_channel(ADC0, 0);
		my_printf("ADC channel 0 value of seq: %d \n", g_hwAdcBuf[0]);
		
		g_hwAdcBuf[1] = csi_adc_read_channel(ADC0, 1);
		my_printf("ADC channel 1 value of seq: %d \n", g_hwAdcBuf[1]);
		
		g_hwAdcBuf[2] = csi_adc_read_channel(ADC0, 2);
		my_printf("ADC channel 2 value of seq: %d \n", g_hwAdcBuf[2]);
#endif
		nop;
		nop;
		
	}while(0);
	
	return iRet;
}

/** \brief ADC sample, countinuous mode ADC_DATA_DEPTH = N（N > 0）
 *  \brief ADC采样，轮询、连续转换模式。ADC通道采样深度为N（每通道采样次数为N）
 *  \brief 连续转换模式：ADC启动后进行整个序列采样，序列采样完成后，继续从序列第一个通道开始，如此循环。采样期间可以停止
 * 
 *  \param[in] none
 *  \return error code
 */
int adc_samp_continuous_test(void)
{
	int iRet = 0;
	uint8_t i;	
	csi_adc_config_t tAdcConfig;	
	//adc 输入管脚配置
	csi_pin_set_mux(PC13, PC13_ADC_INA0);
	
	//adc 参数配置初始化
	tAdcConfig.byClkDiv = 0x02;							//ADC clk两分频：clk = pclk/2
	tAdcConfig.byClksel = ADC_PCLK;						//ADC clk选择：PCLK
	tAdcConfig.bySampHold = 0x06;						//ADC 采样时间： time = 16 + 6 = 22(ADC clk周期)
	tAdcConfig.byConvMode = ADC_CONV_CONTINU;			//ADC 转换模式： 连续转换
	tAdcConfig.byVrefSrc = ADCVERF_VDD_VSS;				//ADC 参考电压： 系统VDD
	tAdcConfig.wInt = ADC_INTSRC_NONE;				//ADC 中断配置： 无中断
	tAdcConfig.ptSeqCfg = (csi_adc_seq_t *)tSeqCfg;		//ADC 采样序列： 具体参考结构体变量 tSeqCfg
	
	csi_adc_init(ADC0, &tAdcConfig);							//初始化ADC参数配置
	ADC0->CR |= (0x07 << 16);	                                //采集内部温度传感器，并且输出到管脚
	csi_adc_set_seqx(ADC0, tAdcConfig.ptSeqCfg, byChnlNum);		//配置ADC采样序列
	csi_adc_set_buffer((uint16_t *)g_hwAdcBuf, ADC_DATA_DEPTH);	//传递ADC采样buffer，ADC采样值存放于此buffer中
	csi_adc_start(ADC0);										//启动ADC
	
	do
	{
		
#if	ADC_DATA_DEPTH < 2
		
		//ADC采样深度(每通道采样次数=1)
		if(ADC_DATA_DEPTH == 1)
		{
			//读ADC采样序列，整个采样序列所有通道读到采样buffer 
			if(csi_adc_read_seqx(ADC0) == CSI_OK)			
			{
				csi_adc_stop(ADC0);													//停止转换
				for(i = 0; i < byChnlNum; i++)
					my_printf("ADC channel value of seq %d: %d \n",i, g_hwAdcBuf[i]);	//采样通道读取完成，ADC value 保持在 buffer中 
			}
			else																			
				my_printf("ADC sample timeout...\n");								//采样通道读取超时(异常)	
				
			
			//上面的采样关闭了, 重新启动ADC
			csi_adc_set_buffer((uint16_t *)g_hwAdcBuf, ADC_DATA_DEPTH);		//传递ADC采样buffer，ADC采样值存放于此buffer中	
			//ADC0->DRMASK = 0x01;	//序列0的结果禁止更新
			//ADC0->PRI = 0x01;//比1低的序列有更高的优先级
			iRet = csi_adc_start(ADC0);										//再次启动ADC		
			if(iRet < 0)
				my_printf("ADC start failure ...\n");
				
			do{
				//读ADC采样序列，整个采样序列所有通道读到采样buffer 
				if(csi_adc_read_seqx(ADC0) == CSI_OK)			
				{
					for(i = 0; i < byChnlNum; i++)
						my_printf("ADC channel value of seq %d: %d \n",i, g_hwAdcBuf[i]);	//采样通道读取完成，ADC value 保持在 buffer中 
					
					csi_adc_set_buffer((uint16_t *)g_hwAdcBuf, ADC_DATA_DEPTH);			//传递ADC采样buffer，ADC采样值存放于此buffer中	
				}
				else																			
					my_printf("ADC sample timeout...\n");
	
			}while(1);
		}

#else 
		//if(ADC_DATA_DEPTH > 1)
		{
			uint32_t j;
			volatile uint32_t wDataMin = 4096;
			volatile uint32_t wDataMax = 0;
			volatile uint32_t wDataAve = 0;
			volatile uint32_t wDataSum = 0;
			
			//读ADC采样序列，整个采样序列所有通道读到采样buffer 
			if(csi_adc_read_seqx(ADC0) == CSI_OK)									//采样通道读取完成，ADC value 保持在 buffer中 
			{
				csi_adc_stop(ADC0);													//停止转换
				for(i = 0; i < byChnlNum; i++)
				{
					//my_printf("ADC CHANNEL IDx OF SEQ IS : %d \n", i);
					for(j = 0; j < ADC_DATA_DEPTH; j++)
						//my_printf("ADC channel value of seq: %d \n", g_hwAdcBuf[i][j]);
					
					
					for(j = 0; j < ADC_DATA_DEPTH; j++)
					{
						wDataSum = wDataSum + g_hwAdcBuf[i][j];
						if(wDataMin > g_hwAdcBuf[i][j])
						{
							wDataMin = g_hwAdcBuf[i][j];
						}
						if(wDataMax < g_hwAdcBuf[i][j])
						{
							wDataMax = g_hwAdcBuf[i][j];
						}
					}
					
					wDataAve = wDataSum / ADC_DATA_DEPTH;
					my_printf("\nADC max value of seq: %d \n", wDataMax);
					my_printf("ADC min value of seq: %d \n", wDataMin);
					my_printf("ADC ave value of seq: %d \n\n", wDataAve);
					
					//while(wDataAve > 0x0f)
					//{
						//mdelay(500);
						//csi_pin_set_low(PB02);
						//my_printf("ADC voltage low!\n");
					//}
				}
			}
			else																			
				my_printf("ADC sample timeout...\n");								//采样通道读取超时(异常)	
				
			
			//上面的采样关闭了,重新启动ADC
			csi_adc_set_buffer((uint16_t *)g_hwAdcBuf, ADC_DATA_DEPTH);		//传递ADC采样buffer，ADC采样值存放于此buffer中	
			iRet = csi_adc_start(ADC0);										//再次启动ADC		
			if(iRet < 0)
				my_printf("ADC start failure ...\n");
			
			do{
				//读ADC采样序列，整个采样序列所有通道读到采样buffer 
				wDataMin = 4096;
				wDataMax = 0;
				wDataAve = 0;
				wDataSum = 0;
				if(csi_adc_read_seqx(ADC0) == CSI_OK)									//采样通道读取完成，ADC value 保持在 buffer中 
				{
					for(i = 0; i < byChnlNum; i++)
					{
						//my_printf("ADC CHANNEL IDx OF SEQ IS : %d \n", i);
						for(j = 0; j < ADC_DATA_DEPTH; j++)
							//my_printf("ADC channel value of seq: %d \n", g_hwAdcBuf[i][j]);
						
						for(j = 0; j < ADC_DATA_DEPTH; j++)
						{
							wDataSum = wDataSum + g_hwAdcBuf[i][j];
							if(wDataMin > g_hwAdcBuf[i][j])
							{
								wDataMin = g_hwAdcBuf[i][j];
							}
							if(wDataMax < g_hwAdcBuf[i][j])
							{
								wDataMax = g_hwAdcBuf[i][j];
							}
						}
						
						wDataAve = wDataSum / ADC_DATA_DEPTH;
						my_printf("\nADC max value of seq: %d \n", wDataMax);
						my_printf("ADC min value of seq: %d \n", wDataMin);						
						my_printf("ADC ave value of seq: %d \n", wDataAve);
						
						//while(wDataAve > 0x0f)
						//{
							mdelay(500);
							//csi_pin_set_low(PB02);
							//my_printf("ADC voltage low!\n");
						//}
						
						
					}
					csi_adc_set_buffer((uint16_t *)g_hwAdcBuf, ADC_DATA_DEPTH);			//传递ADC采样buffer，ADC采样值存放于此buffer中
				}
				else																			
					my_printf("ADC sample timeout...\n");								//采样通道读取超时(异常)
	
			}while(1);	
		}
#endif

	}while(0);
	
	return iRet;
}

/** \brief ADC sample,one shot mode, ADC_DATA_DEPTH = 1
 *  \brief ADC采样，中断、单次转换模式。ADC通道采样深度为1（每通道采样次数为1）
 *  \brief 单次转换模式：ADC启动后进行整个序列采样直到结束，中间不能停止；进行下一次序列转换需重新启动
 * 
 *  \param[in] none
 *  \return error code
 */
int adc_samp_oneshot_int_test(void)
{
	int iRet = 0;
	uint8_t i;
	csi_adc_config_t tAdcConfig;

	//adc 输入管脚配置
	csi_pin_set_mux(PC13, PC13_ADC_INA0);
	csi_pin_set_mux(PC14, PC14_ADC_INA1);
	csi_pin_set_mux(PC15, PC15_ADC_INA2);
	
	//adc 参数配置初始化
	tAdcConfig.byClkDiv = 0x02;									//ADC clk两分频：clk = pclk/2
	tAdcConfig.byClksel = ADC_PCLK;								//ADC clk选择：PCLK
	tAdcConfig.bySampHold = 0x06;								//ADC 采样时间： time = 16 + 6 = 22(ADC clk周期)
	tAdcConfig.byConvMode = ADC_CONV_ONESHOT;					//ADC 转换模式： 单次转换；
	tAdcConfig.byVrefSrc = ADCVERF_VDD_VSS;						//ADC 参考电压： 系统VDD
	tAdcConfig.wInt = ADC_INTSRC_SEQ0 | ADC_INTSRC_SEQ1 |
				ADC_INTSRC_SEQ2;								//ADC 中断配置：SEQ_END(0~2)中断使能，默认(推荐使用)SEQ_END中断
	tAdcConfig.ptSeqCfg = (csi_adc_seq_t *)tSeqCfg;				//ADC 采样序列： 具体参考结构体变量 tSeqCfg
	
	csi_adc_init(ADC0, &tAdcConfig);							//初始化ADC参数配置	
	csi_adc_set_seqx(ADC0, tAdcConfig.ptSeqCfg, byChnlNum);		//配置ADC采样序列
	csi_adc_set_buffer((uint16_t *)g_hwAdcBuf, 1);				//传递ADC采样buffer，ADC采样值存放于此buffer中
	csi_adc_start(ADC0);										//启动ADC
	
	
	do
	{
		//读ADC采样序列，整个采样序列所有通道读到采样buffer 
		while(1)
		{
			if(csi_adc_get_status(ADC0) == ADC_STATE_DONE)			//采样通道读取完成，ADC value 保持在 buffer中 
			{
				csi_adc_clr_status(ADC0);
				for(i = 0; i < byChnlNum; i++)
					my_printf("ADC channel value of seq: %d \n", g_hwAdcBuf[i]);
				break;
			}
		}
		
		//若继续采样ADC序列，需再次启动ADC
		iRet = csi_adc_start(ADC0);								//再次启动ADC

		while(1)
		{
			if(csi_adc_get_status(ADC0) == ADC_STATE_DONE)			//采样通道读取完成，ADC value 保持在 buffer中 
			{
				csi_adc_clr_status(ADC0);
				for(i = 0; i < byChnlNum; i++)
					my_printf("ADC channel value of seq: %d \n", g_hwAdcBuf[i]);
				break;
			}		
		}		
		nop;
		
	}while(0);
	
	return iRet;
}

/** \brief ADC sample, countinuous mode ADC_DATA_DEPTH = N（N > 0）
 *  \brief ADC采样，轮询、连续转换模式。ADC通道采样深度为N（每通道采样次数为N）
 *  \brief 连续转换模式：ADC启动后进行整个序列采样，序列采样完成后，继续从序列第一个通道开始，如此循环。采样期间可以停止
 * 
 *  \param[in] none
 *  \return error code
 */
int adc_samp_continuous_int_test(void)
{
	int iRet = 0;
	uint8_t i;
	csi_adc_config_t tAdcConfig;
	
	//adc 输入管脚配置
	csi_pin_set_mux(PC13, PC13_ADC_INA0);
	csi_pin_set_mux(PC14, PC14_ADC_INA1);
	csi_pin_set_mux(PC15, PC15_ADC_INA2);
	
	//adc 参数配置初始化
	tAdcConfig.byClkDiv = 0x02;									//ADC clk两分频：clk = pclk/2
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
	
	do
	{
		
#if	ADC_DATA_DEPTH < 2
    
	if(ADC_DATA_DEPTH == 1)
	{
		while(1)
		{	
			if(csi_adc_get_status(ADC0) == ADC_STATE_DONE)			//采样通道读取完成，ADC value 保持在 buffer中 
			{
				csi_adc_stop(ADC0);									//停止ADC
				csi_adc_clr_status(ADC0);							//清除ADC工作状态
				for(i = 0; i < byChnlNum; i++)
					my_printf("ADC channel value of seq %d: %d \n", i,g_hwAdcBuf[i]);
				break;
			}
		}
		
		//上面已关闭ADC,若继续采样ADC序列，需再次启动ADC
		iRet = csi_adc_start(ADC0);								//再次启动ADC	
		while(1)
		{
			if(csi_adc_get_status(ADC0) == ADC_STATE_DONE)		//采样通道读取完成，ADC value 保持在 buffer中 
			{
				csi_adc_clr_status(ADC0);
				for(i = 0; i < byChnlNum; i++)
					my_printf("ADC channel value of seq %d: %d \n", i,g_hwAdcBuf[i]);
			}
			nop;
		}
	}

#else
	//if(ADC_DATA_DEPTH > 1)
	{
		uint32_t j;
		if(csi_adc_get_status(ADC0) == ADC_STATE_DONE)			//采样通道读取完成，ADC value 保持在 buffer中 
		{
			csi_adc_stop(ADC0);									//停止ADC
			csi_adc_clr_status(ADC0);							//清除ADC工作状态
			for(i = 0; i < byChnlNum; i++)
			{
				my_printf("ADC CHANNEL IDx OF SEQ IS : %d \n", i);
				for(j = 0; j < ADC_DATA_DEPTH; j++)
					my_printf("ADC channel value of seq: %d \n", g_hwAdcBuf[i][j]);
			}
		}
		
		//上面已关闭ADC,若继续采样ADC序列，需再次启动ADC
		csi_adc_set_buffer((uint16_t *)g_hwAdcBuf, ADC_DATA_DEPTH);		//传递ADC采样buffer，ADC采样值存放于此buffer中	
		iRet = csi_adc_start(ADC0);										//再次启动ADC	
		while(1)
		{
			if(csi_adc_get_status(ADC0) == ADC_STATE_DONE)				//采样通道读取完成，ADC value 保持在 buffer中 
			{
				csi_adc_clr_status(ADC0);
				for(i = 0; i < byChnlNum; i++)
				{
					my_printf("ADC CHANNEL IDx OF SEQ IS : %d \n", i);
					for(j = 0; j < ADC_DATA_DEPTH; j++)
						my_printf("ADC channel value of seq: %d \n", g_hwAdcBuf[i][j]);
				}
				csi_adc_set_buffer((uint16_t *)g_hwAdcBuf, ADC_DATA_DEPTH);		//传递ADC采样buffer，ADC采样值存放于此buffer中	
			}
		}
	}

#endif
		
	}while(0);
	
	return iRet;
}

//adc 由大变小，由小变大,测试比较功能
int adc_samp_continuous_cmp_cross(void)
{
	int iRet = 0;
	uint8_t i;
	csi_adc_config_t tAdcConfig;
	
	//adc 输入管脚配置
	csi_pin_set_mux(PC13, PC13_ADC_INA0);
	csi_pin_set_mux(PC14, PC14_ADC_INA1);
	csi_pin_set_mux(PC15, PC15_ADC_INA2);
	
	//adc 参数配置初始化
	tAdcConfig.byClkDiv = 0x02;									//ADC clk两分频：clk = pclk/2
	tAdcConfig.byClksel = ADC_PCLK;								//ADC clk选择：PCLK	
	tAdcConfig.bySampHold = 0x06;								//ADC 采样时间： time = 16 + 6 = 22(ADC clk周期)
	tAdcConfig.byConvMode = ADC_CONV_CONTINU;			//ADC 转换模式： 单次转换；
	tAdcConfig.byVrefSrc = ADCVERF_VDD_VSS;				//ADC 参考电压： 系统VDD
	tAdcConfig.wInt = ADC_INTSRC_CMP1H;				//ADC 中断配置： 比cmp0大，触发中断
	tAdcConfig.ptSeqCfg = (csi_adc_seq_t *)tSeqCfg;		//ADC 采样序列： 具体参考结构体变量 tSeqCfg
	
	csi_adc_init(ADC0, &tAdcConfig);							//初始化ADC参数配置	
	csi_adc_set_seqx(ADC0, tAdcConfig.ptSeqCfg, byChnlNum);		//配置ADC采样序列
	csi_adc_set_buffer((uint16_t *)g_hwAdcBuf, 1);				//传递ADC采样buffer，ADC采样值存放于此buffer中
	
	//csp_adc_set_cmp0(ADC0, 4000, 1);//转换序列1的值大于或者小于cmp0的值
	csp_adc_set_cmp1(ADC0, 1000, 1);//转换序列1的值大于或者小于cmp1的值
	csp_adc_set_cmp_mode(ADC0, ADC12_CMP_ACROSS);//采集值先小于再大于才进中断
	
	csi_adc_start(ADC0);										//启动ADC
	
	do
	{
		
#if	ADC_DATA_DEPTH < 2
		
		//ADC采样深度(每通道采样次数=1)
		if(ADC_DATA_DEPTH == 1)
		{
			//读ADC采样序列，整个采样序列所有通道读到采样buffer 
			if(csi_adc_read_seqx(ADC0) == CSI_OK)			
			{
				csi_adc_stop(ADC0);													//停止转换
				for(i = 0; i < byChnlNum; i++)
					my_printf("ADC channel value of seq %d: %d \n",i, g_hwAdcBuf[i]);	//采样通道读取完成，ADC value 保持在 buffer中 
			}
			else																			
				my_printf("ADC sample timeout...\n");								//采样通道读取超时(异常)	
				
			
			//上面的采样关闭了, 重新启动ADC
			mdelay(100);
			csi_adc_set_buffer((uint16_t *)g_hwAdcBuf, ADC_DATA_DEPTH);		//传递ADC采样buffer，ADC采样值存放于此buffer中	
			iRet = csi_adc_start(ADC0);										//再次启动ADC		
			if(iRet < 0)
				my_printf("ADC start failure ...\n");
				
			do{
				//读ADC采样序列，整个采样序列所有通道读到采样buffer 
				mdelay(100);
				if(csi_adc_read_seqx(ADC0) == CSI_OK)			
				{
					for(i = 0; i < byChnlNum; i++)
						my_printf("ADC channel value of seq %d: %d \n",i, g_hwAdcBuf[i]);	//采样通道读取完成，ADC value 保持在 buffer中 
					
					csi_adc_set_buffer((uint16_t *)g_hwAdcBuf, ADC_DATA_DEPTH);			//传递ADC采样buffer，ADC采样值存放于此buffer中	
				}
				else																			
					my_printf("ADC sample timeout...\n");
	
			}while(1);
		}

#else 
		//if(ADC_DATA_DEPTH > 1)
		{
			uint32_t j;
			
			//读ADC采样序列，整个采样序列所有通道读到采样buffer 
			if(csi_adc_read_seqx(ADC0) == CSI_OK)									//采样通道读取完成，ADC value 保持在 buffer中 
			{
				csi_adc_stop(ADC0);													//停止转换
				for(i = 0; i < byChnlNum; i++)
				{
					my_printf("ADC CHANNEL IDx OF SEQ IS : %d \n", i);
					for(j = 0; j < ADC_DATA_DEPTH; j++)
						my_printf("ADC channel value of seq: %d \n", g_hwAdcBuf[i][j]);
				}
			}
			else																			
				my_printf("ADC sample timeout...\n");								//采样通道读取超时(异常)	
				
			
			//上面的采样关闭了,重新启动ADC
			csi_adc_set_buffer((uint16_t *)g_hwAdcBuf, ADC_DATA_DEPTH);		//传递ADC采样buffer，ADC采样值存放于此buffer中	
			iRet = csi_adc_start(ADC0);										//再次启动ADC		
			if(iRet < 0)
				my_printf("ADC start failure ...\n");
			
			do{
				//读ADC采样序列，整个采样序列所有通道读到采样buffer 
				if(csi_adc_read_seqx(ADC0) == CSI_OK)									//采样通道读取完成，ADC value 保持在 buffer中 
				{
					for(i = 0; i < byChnlNum; i++)
					{
						my_printf("ADC CHANNEL IDx OF SEQ IS : %d \n", i);
						for(j = 0; j < ADC_DATA_DEPTH; j++)
							my_printf("ADC channel value of seq: %d \n", g_hwAdcBuf[i][j]);
					}
					csi_adc_set_buffer((uint16_t *)g_hwAdcBuf, ADC_DATA_DEPTH);			//传递ADC采样buffer，ADC采样值存放于此buffer中
				}
				else																			
					my_printf("ADC sample timeout...\n");								//采样通道读取超时(异常)
	
			}while(1);	
		}
#endif
	}while(0);
	
	return iRet;
}


//每次都比较
int adc_samp_oneshot_cmp_always(void)
{
	int iRet = 0;
	uint8_t i;
	csi_adc_config_t tAdcConfig;
	
	//adc 输入管脚配置
	csi_pin_set_mux(PC13, PC13_ADC_INA0);
	csi_pin_set_mux(PC14, PC14_ADC_INA1);
	csi_pin_set_mux(PC15, PC15_ADC_INA2);
	
	//adc 参数配置初始化
	tAdcConfig.byClkDiv = 0x02;									//ADC clk两分频：clk = pclk/2
	tAdcConfig.byClksel = ADC_PCLK;								//ADC clk选择：PCLK	
	tAdcConfig.bySampHold = 0x06;								//ADC 采样时间： time = 16 + 6 = 22(ADC clk周期)
	tAdcConfig.byConvMode = ADC_CONV_ONESHOT;			//ADC 转换模式： 单次转换；
	tAdcConfig.byVrefSrc = ADCVERF_VDD_VSS;				//ADC 参考电压： 系统VDD
	tAdcConfig.wInt = ADC_INTSRC_CMP0H;				//ADC 中断配置： 无中断
	tAdcConfig.ptSeqCfg = (csi_adc_seq_t *)tSeqCfg;		//ADC 采样序列： 具体参考结构体变量 tSeqCfg
	
	csi_adc_init(ADC0, &tAdcConfig);							//初始化ADC参数配置	
	csi_adc_set_seqx(ADC0, tAdcConfig.ptSeqCfg, byChnlNum);		//配置ADC采样序列
	csi_adc_set_buffer((uint16_t *)g_hwAdcBuf, 1);				//传递ADC采样buffer，ADC采样值存放于此buffer中
	
	csp_adc_set_cmp1(ADC0, 1000, 1);//转换序列1的值大于或者小于cmp1的值
	csp_adc_set_cmp_mode(ADC0, ADC12_CMP_ALWAYS);//采集值先小于再大于才进中断
	
	csi_adc_start(ADC0);										//启动ADC
	
	do
	{
		//读ADC采样序列，整个采样序列所有通道读到采样buffer 
		if(csi_adc_read_seqx(ADC0) == CSI_OK)			//采样通道读取完成，ADC value 保持在 buffer中 
		{
			for(i = 0; i < byChnlNum; i++)
				my_printf("ADC channel value of seq %d: %d \n", i,g_hwAdcBuf[i]);
		}
		else											//采样通道读取超时(异常)									
		{
			my_printf("ADC sample timeout...\n");
		}
		
		//单个通道分别读取
		iRet = csi_adc_start(ADC0);						//单次转换模式，序列转换完成后自动停止。若重新采样序列通道，需要重新启动ADC
		if(iRet < 0)
			my_printf("ADC start failure ...\n");
			
#if ADC_DATA_DEPTH < 2

		g_hwAdcBuf[0] = csi_adc_read_channel(ADC0, 0);
		my_printf("ADC channel 0 value of seq %d: %d \n", 0,g_hwAdcBuf[0]);
		
		g_hwAdcBuf[1] = csi_adc_read_channel(ADC0, 1);
		my_printf("ADC channel 1 value of seq %d: %d \n", 1,g_hwAdcBuf[1]);
		
		g_hwAdcBuf[2] = csi_adc_read_channel(ADC0, 2);
		my_printf("ADC channel 2 value of seq %d: %d \n", 2,g_hwAdcBuf[2]);
#endif
		nop;
		nop;
		
	}while(0);
	
	return iRet;
}


void etcb_adc_config_test(void)
{
	csi_adc_config_t tAdcConfig;
	
	//adc 输入管脚配置
	csi_pin_set_mux(PC13, PC13_ADC_INA0);
	csi_pin_set_mux(PC14, PC14_ADC_INA1);
	csi_pin_set_mux(PC15, PC15_ADC_INA2);
	
	//adc 参数配置初始化
	tAdcConfig.byClkDiv = 0x02;									//ADC clk两分频：clk = pclk/2
	tAdcConfig.byClksel = ADC_PCLK;								//ADC clk选择：PCLK
	tAdcConfig.bySampHold = 0x06;								//ADC 采样时间： time = 16 + 6 = 22(ADC clk周期)
	tAdcConfig.byConvMode = ADC_CONV_ONESHOT;					//ADC 转换模式： 单次转换；
	tAdcConfig.byVrefSrc = ADCVERF_VDD_VSS;						//ADC 参考电压： 系统VDD
	tAdcConfig.wInt = ADC_INTSRC_NONE;     //中断设置

	tAdcConfig.ptSeqCfg = (csi_adc_seq_t *)tSeqCfg;		        //ADC 采样序列： 具体参考结构体变量 SeqCfg3	
	csi_adc_init(ADC0, &tAdcConfig);//初始化ADC参数配置	
	csi_adc_set_seqx(ADC0, tAdcConfig.ptSeqCfg, 1);	 //配置ADC采样序列
	//csi_adc_set_sync(ADC0, ADC_TRG_SYNCEN0, ADC_TRG_CONTINU, 50);     //选择ADC_TRG_SYNCEN0同步事件
	//csi_adc_set_sync(ADC0, ADC_TRG_SYNCEN1, ADC_TRG_CONTINU, 50);     //选择ADC_TRG_SYNCEN1同步事件
	//csi_adc_set_sync(ADC0, ADC_TRG_SYNCEN2, ADC_TRG_CONTINU, 50);     //选择ADC_TRG_SYNCEN2同步事件
	//csi_adc_set_sync(ADC0, ADC_TRG_SYNCEN3, ADC_TRG_CONTINU, 50);     //选择ADC_TRG_SYNCEN3同步事件
	//csi_adc_set_sync(ADC0, ADC_TRG_SYNCEN4, ADC_TRG_CONTINU, 50);     //选择ADC_TRG_SYNCEN4同步事件
	//csi_adc_set_sync(ADC0, ADC_TRG_SYNCEN5, ADC_TRG_CONTINU, 50);     //选择ADC_TRG_SYNCEN5同步事件
	
	//csi_adc_set_evtrg(ADC0, 0, ADC_TRGSRC_SEQEND0);//选择事件触发输出0，事件为序列0转换完成
	//csi_pin_toggle(PA05);
	//csi_adc_start(ADC0);										  //启动ADC	
	//ADC0->PRI = 0x02;//比1低的序列有更高的优先级，平常不会转换，触发时才会转换
	//ADC0->TRGFCR = 0x01 | (0x01 << 4);//使能TRGUSR0滤波,窗口不反转
	//ADC0->TRGFWR = (0x6 << 0)| (0xff << 16);//OFFSET和WINDOW值设定
	
}
/* * 一对一触发模式,同时验证ready和ovr中断,验证syncr
 *EXI0--> ADC_SYNC0 
*/
int etcb_one_trg_one_test(void)
{	
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				               //ETB 参数配置结构体		
	
	csi_pin_set_mux(PA5,PA5_OUTPUT);	
	csi_pin_set_low(PA5);	
	csi_pin_set_mux(PA6,PA6_INPUT);		
	csi_pin_pull_mode(PA6, GPIO_PULLNONE);						//PA06 无上下拉
	csi_pin_irq_mode(PA6,EXI_GRP6, GPIO_IRQ_FALLING_EDGE);		//PA06 下降沿产生中断			
	csi_exi_set_evtrg(1, TRGSRC_EXI6, 2);                       //PA06 4次下降沿触发目标事件      | ADC_INTSRC_OVR

	etcb_adc_config_test();
	
	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  		//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_EXI_TRGOUT1 ;  	    //EXI6 触发输出作为触发源
	tEtbConfig.bySrcIp1 = 0xff;      
	tEtbConfig.bySrcIp2 = 0xff;
	tEtbConfig.byDstIp =  ETB_ADC0_SYNCIN0;   	    //ADC0 同步输入作为目标事件
	tEtbConfig.byDstIp1 = 0xff;
	tEtbConfig.byDstIp2 = 0xff;
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
   
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
//	while(1)
//	{
//		csi_adc_start(ADC0);
//		mdelay(3000);	
//	}
	
	return iRet;
}

/* * 一对多触发模式
 *EXI0 --> ADC_SYNC0 ,ADC_SYNC1,ADC_SYNC2
*/
int etcb_one_trg_more_test(void)
{	
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				               //ETB 参数配置结构体		
	
	csi_pin_set_mux(PA5,PA5_OUTPUT);		
	csi_pin_set_mux(PA6,PA6_INPUT);		
	csi_pin_pull_mode(PA6, GPIO_PULLNONE);						//PA06 无上下拉	
	csi_pin_irq_mode(PA6,EXI_GRP6, GPIO_IRQ_FALLING_EDGE);		//PA06 下将沿产生中断	
	
	
	csi_exi_set_evtrg(1, TRGSRC_EXI6, 4);                       //PA06 4次上升沿触发目标事件     

	etcb_adc_config_test();
	
	tEtbConfig.byChType = ETB_ONE_TRG_MORE;  		//单个源触发多个目标
	tEtbConfig.bySrcIp  = ETB_EXI_TRGOUT1 ;  	    //EXI6 触发输出1作为触发源
	tEtbConfig.bySrcIp1 = 0xff;      
	tEtbConfig.bySrcIp2 = 0xff;
	tEtbConfig.byDstIp =  ETB_ADC0_SYNCIN0;   	    //ADC0 同步输入作为目标事件,可能
	tEtbConfig.byDstIp1 = ETB_ADC0_SYNCIN1;
	tEtbConfig.byDstIp2 = ETB_ADC0_SYNCIN2;
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
   
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
	return iRet;
}

//ADC的触发事件输出
int etcb_event_trigger_out_test(void)
{	
	int iRet = 0;
	uint8_t bySend = 0x20;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				               //ETB 参数配置结构体	

	csi_dma_ch_config_t tDmaConfig; //发送dma配置
	
	//send dma para config
	tDmaConfig.bySrcLinc 	= DMA_ADDR_CONSTANT;		//低位传输原地址固定不变
	tDmaConfig.bySrcHinc 	= DMA_ADDR_CONSTANT;		//高位传输原地址固定不变
	tDmaConfig.byDetLinc 	= DMA_ADDR_CONSTANT;		//低位传输目标地址固定不变
	tDmaConfig.byDetHinc 	= DMA_ADDR_CONSTANT;		//高位传输目标地址固定不变
	tDmaConfig.byDataWidth 	= DMA_DSIZE_8_BITS;			//传输数据宽度8bit
	tDmaConfig.byReload 	= DMA_RELOAD_DISABLE;		//禁止自动重载
	tDmaConfig.byTransMode 	= DMA_TRANS_ONCE;			//DMA服务模式(传输模式)，连续服务
	tDmaConfig.byTsizeMode  = DMA_TSIZE_ONE_DSIZE;		//传输数据大小，一个 DSIZE , 即DSIZE定义大小
	tDmaConfig.byReqMode	= DMA_REQ_HARDWARE;			//DMA请求模式，硬件请求（硬件触发）
	tDmaConfig.wInt		= DMA_INTSRC_NONE;			//不使用中断
	
	csi_pin_set_mux(PA5,PA5_OUTPUT);	
	csi_pin_set_low(PA5);
	
	csi_pin_set_mux(PA6,PA6_INPUT);
	csi_pin_pull_mode(PA6, GPIO_PULLNONE);						//PA06 无上下拉

	etcb_adc_config_test();
	
	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  		//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_ADC0_TRGOUT0 ;  	    //ADC_TRGOUT0作为触发源
	tEtbConfig.bySrcIp1 = 0xff;      
	tEtbConfig.bySrcIp2 = 0xff;
	tEtbConfig.byDstIp =  ETB_DMA0_CH0;   	    //dma通道0输入作为目标事件
	tEtbConfig.byDstIp1 = 0xff;
	tEtbConfig.byDstIp2 = 0xff;
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
   
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
	csi_dma_soft_rst(DMA0);
	csi_dma_ch_init(DMA0, 0, &tDmaConfig);				//初始化DMA
	csi_dma_ch_start(DMA0, 0, (void *)(&bySend), (void *)&(GPIOA->SODR), 0,1);
	
	mdelay(3000);
	csi_adc_start(ADC0);
	return iRet;
}


