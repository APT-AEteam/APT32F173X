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

	csi_adc_config_t tAdcConfig = {0};
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
	csi_adc_start(ADC0);																//启动ADC
}

/** \brief  获取当前温度传感器温度值
 *  \brief   计算步骤： 
 *                    STEP1: ADC输入源选择INTVREF，且FVR_CR[INTVREF_SEL] = 0x3时，采样到的值为ADC_TS
 *                    STEP2: ADC输入源选择INTVREF，且FVR_CR[INTVREF_SEL] = 0x1时，采样到的值为ADC_1V    
 *			           STEP3: 读取芯片内部标定值
 *                    STEP4: 公式计算
 * 
 *  \brief   标定值说明：
 *                     标定值(4Bytes)已经保存在对应的地址中，在adc.h文件中已用宏定义表示
 *                            名称        地址                说明                                                     adc.h中的名字
 *                            TS1         0x80140         常温下温度传感器信息                      CALIBRATION_TS1
 *                            IR1          0x80144         常温下内部1V电压参考信息             CALIBRATION_IR1
 *                            TS2         0x80180         高温下温度传感器信息                      CALIBRATION_TS2
 *                            IR2          0x80184         高温下内部1V电压参考信息              CALIBRATION_IR2
 *                    每个标定值的对应字段说明如下
 *                            [31:28]FLAG + [27:16]TEMP + [15:12]rsvd + [11:0]DATA
 *                                                       |
 *                                            [27]为TEMP的符号位，0表示正温度，1表示负温度  
 *                    举例:  
 *                          若  TS1=  0x5d800673 
 *                          则  TS1_FLAG   = 0x5        有效数据
 *                                TS1_TEMP  = 0xd80   此温度值为负温度，此值为补码。计算时需要将其还原成原码
 *                                TS1_DATA  = 0x673   温度传感器的电压采样值
 * 
 *  \brief   公式如下：
 *              
 *                                                                  (TS2_TEMP - TS1_TEMP) * 0.0625           TS2_DATA            ADC_TS
 *              Tx = TS2_TEMP * 0.0625 - ————————————————— * (—————— - ——————)
 *                                                                      TS2_DATA             TS1_DATA                IR2_DATA            ADC_1V
 *                                                                   ——————  -  ———————
 *                                                                       IR2_DATA              IR1_DATA
 *              其中：xxx_TEMP的获取
 *                                如果是正温度：
 *                                                          TS1_TEMP  =   (float) (TS1 & 0x0fff0000) >> 16;
 *                                                          TS2_TEMP  =   (float) (TS2 & 0x0fff0000) >> 16;
 *                                如果是负温度：
 *                                                          TS1_TEMP  =   (float) (0 - ((((uint16_t)((TS1 & 0x0fff0000) >> 16) - 1) ^ 0x7FF) & 0x7FF));
 *                                                          TS2_TEMP  =   (float) (0 - ((((uint16_t)((TS2 & 0x0fff0000) >> 16) - 1) ^ 0x7FF) & 0x7FF));
 *                         xxx_DATA的获取
 *                                                          TS1_DATA = (float) (TS1 & 0x00000fff);
 *                                                          TS2_DATA = (float) (TS2 & 0x00000fff);
 *                                                          IR1_DATA  = (float) (IR1 & 0x00000fff);
 *                                                          IR2_DATA  = (float) (IR2 & 0x00000fff);
 * 
 * 
 *  \param[in] none
 *  \return temperature value
 */
float adc_temperature_demo(void)
{	
	int iChnlNum = 1;											//设置总通道个数	
	
#if (USE_GUI == 0)	
	//BUF 管脚配置
	csi_gpio_set_mux(GPIOD,PD4, PD4_INPUT);					//设置BUF输入管脚PD4
	csi_gpio_set_mux(GPIOB,PB8, PB8_BUF);					//设置BUF输出管脚PB8
#endif	

	//adc 参数配置初始化	
	csi_adc_config_t tAdcConfig= {0};
	tAdcConfig.byClkDiv = 0x2;									//ADC clk两分频：clk = pclk/2
	tAdcConfig.eClkSel = ADC_CLK_HFOSC;							//ADC clk选择：PCLK
	tAdcConfig.bySampHold = 0xFF;								//ADC 采样时间： time = 16 + 6 = 22(ADC clk周期)
	tAdcConfig.eRunMode = ADC_RUN_ONCE;							//ADC 转换模式： 单次转换；
	tAdcConfig.eVrefSrc = ADCVERF_VDD_VSS;						//ADC 参考电压： 系统VDD
	csi_adc_init(ADC0, &tAdcConfig);							//初始化ADC参数配置	
	
	csi_adc_set_seq_num(ADC0,iChnlNum);													//配置ADC总采样通道个数
	csi_adc_set_seqx(ADC0,0,ADCIN_INTVREF,ADC_CV_COUNT_1,ADC_AVG_COF_1,ADCSYNC_NONE);	//配置ADC采样通道0采样INTVREF值
	
	//设置温度传感器需要的参数
	int16_t hwADC_TS;
	int16_t hwADC_1V;
	int16_t hw_chipTemp= 0;
	
	float fTS1_DATA, fTS1_TEMP, fTS2_DATA, fTS2_TEMP;
	float fIR1_DATA, fIR2_DATA;
	float fADC_TS, fADC_1V;
	float fTEMP_X = 0;
	
	uint8_t valid1,valid2,valid3,valid4;

	//STEP1: ADC输入源选择INTVREF，且INTVREF_SEL= 0x3时，采样到的值为hwADC_TS
	csi_set_buf(FVR,FVR_BUF_LEVEL_TS);				//设置buf的输出为温度传感器
	csi_fvr_enable(FVR);
	csi_adc_start(ADC0);							//启动ADC	
	while(!(ADC0->SR & ADC_SEQ(0)));
	hwADC_TS = csi_adc_read_channel(ADC0, 0);		//采集温度传感器的值	
	
	//STEP2: ADC输入源选择INTVREF，且ADCC_CR[INTVREF_SEL] = 0x2时，采样到的值为hwADC_1V
	csi_set_buf(FVR,FVR_BUF_LEVEL_1V);				//设置buf的输出为内部1V
	csi_fvr_enable(FVR);
	csi_adc_start(ADC0);							//启动ADC	
	while(!(ADC0->SR & ADC_SEQ(0)));
	hwADC_1V = csi_adc_read_channel(ADC0, 0);		//采集内部1V的值
	
	//STEP3: 读取芯片内部标定值
	//已经用宏定义表示CALIBRATION_TS1 CALIBRATION_IR1 CALIBRATION_TS2 CALIBRATION_IR2
	valid1 = (CALIBRATION_TS1& 0xf0000000) >> 28;
	valid2 = (CALIBRATION_IR1& 0xf0000000) >> 28;
	valid3 = (CALIBRATION_TS2 & 0xf0000000) >> 28;
	valid4 = (CALIBRATION_IR2 & 0xf0000000) >> 28;
	

	//STEP4: 公式计算
	//当前温度(℃)= xxx_TEMP * 0.0625
	if( (0x5 == valid1) && (0x5 == valid2) && (0x5 == valid3) && (0x5 == valid4) )
	{
		//温度
		if((CALIBRATION_TS1 & 0x08000000) == 0x08000000) //负数
		{
			fTS1_TEMP = (float)(0 - ((((uint16_t)((CALIBRATION_TS1 & 0x0fff0000) >> 16) - 1) ^ 0x7FF) & 0x7FF));
		}
		else//正数
		{
			fTS1_TEMP = (float)((CALIBRATION_TS1 & 0x0fff0000) >> 16 );
		}
		
		if((CALIBRATION_TS2 & 0x08000000) == 0x08000000) //负数
		{
			fTS2_TEMP = (float)(0 - ((((uint16_t)((CALIBRATION_TS2 & 0x0fff0000) >> 16) - 1) ^ 0x7FF) & 0x7FF));
		}
		else//正数
		{
			fTS2_TEMP = (float)((CALIBRATION_TS2 & 0x0fff0000) >> 16 );
		}
		
		fTS1_TEMP = fTS1_TEMP * 0.0625;
		fTS2_TEMP = fTS2_TEMP * 0.0625;
		
		//ts
		fTS1_DATA = (float)(CALIBRATION_TS1 & 0x00000fff);
		fTS2_DATA = (float)(CALIBRATION_TS2 & 0x00000fff);
		fADC_TS = (float)(hwADC_TS & 0x0fff);
		
		//ir
		fIR1_DATA = (float)(CALIBRATION_IR1 & 0x00000fff);
		fIR2_DATA = (float)(CALIBRATION_IR2 & 0x00000fff);
		fADC_1V = (float)(hwADC_1V & 0x00000fff);
		
		fTEMP_X = fTS2_TEMP - (fTS2_TEMP - fTS1_TEMP) / (fTS2_DATA/fIR2_DATA - fTS1_DATA/fIR1_DATA ) * (fTS2_DATA/fIR2_DATA - fADC_TS/fADC_1V);
		hw_chipTemp = (int16_t)(fTEMP_X * 100);
		my_printf("173_temp = %d  \r\n", hw_chipTemp);//打印当前温度值，打印值前两位为整数位，后两位为小数位
	}
	else 
	{
		my_printf("\r\nCalibration data is invalid !!!\r\n");
	}
	
	return fTEMP_X;
}

