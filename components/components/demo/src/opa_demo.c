/***********************************************************************//** 
 * \file  opa_demo.c
 * \brief  LIN_TEST description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2022-03-18 <td>V0.0 <td>XB     <td>initial
 * </table>
 * *********************************************************************
*/

/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include <drv/usart.h>
#include <drv/uart.h>
#include <drv/opa.h>
#include "pin.h"
#include "adc.h"
#include "iostring.h"

//#include "demo.h"

/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/

/* Private variablesr------------------------------------------------------*/
//ADC采样序列通道参数配置，默认情况，重复采样和平均系数为1(ADC采样值不平均)
//ADC触发根据实际应用进行配置
const csi_adc_seq_t tSeqCfg[] =
{
	//输入通道		//连续重复采样次数		//平均系数			//触发源选择
	{ADCIN2,		ADC_CV_COUNT_32,		ADC_AVG_COF_32,		ADCSYNC_NONE},
	{ADCIN3,		ADC_CV_COUNT_32,		ADC_AVG_COF_32,		ADCSYNC_NONE},
	{ADCIN4,		ADC_CV_COUNT_32,		ADC_AVG_COF_32,		ADCSYNC_NONE},
	{ADCIN5,		ADC_CV_COUNT_32,		ADC_AVG_COF_32,		ADCSYNC_NONE},
	{ADCIN6,		ADC_CV_COUNT_32,		ADC_AVG_COF_32,		ADCSYNC_NONE},
	{ADCIN7,		ADC_CV_COUNT_32,		ADC_AVG_COF_32,		ADCSYNC_NONE},
}; 

//采样序列的通道数
volatile uint8_t byChnlNum = sizeof(tSeqCfg)/sizeof(tSeqCfg[0]);

//ADC value buffer

volatile uint16_t	g_hwAdcBuf[sizeof(tSeqCfg)/sizeof(tSeqCfg[0])];

/********************************************************************
 * 内部增益模式接线图  Vout =  vp * GAIN ；
                |╲
            Vp  |  ╲
            ----|+   ╲
                |      ╲______Vout
             NC |      ╱   
			 ×-|-   ╱    
                |  ╱      
                |╱         
********************************************************************/
/** \brief opa internal gain mode test 
 *  \brief OPA内部增益模式测试
 * 
 *  \param[in] none
 *  \return error code
 */
void opa_internal_gain_mode_test(void)
{
	csi_adc_config_t tAdcConfig;
	csi_opa_config_t ptOpaConfig_t;
	
	csi_pin_set_mux(PA0,PA0_OPA0P); //PIN14 输入正端
	csi_pin_set_mux(PA1,PA1_OPA0N); //PIN15 输入负端
	csi_pin_set_mux(PA2,PA2_OPA0X); //PIN16 输出
	csi_pin_set_mux(PB3,PB3_OPA1P); //PIN55 输入正端
	csi_pin_set_mux(PB4,PB4_OPA1N); //PIN56 输入负端
	csi_pin_set_mux(PB5,PB5_OPA1X); //PIN57 输出
	csi_pin_set_mux(PB0,PB0_OPA2P); //PIN26 输入正端
	csi_pin_set_mux(PA7,PA7_OPA2N); //PIN23 输入负端
	csi_pin_set_mux(PA6,PA6_OPA2X); //PIN22 输出
	csi_pin_set_mux(PA3,PA3_OPA3P); //PIN17 输入正端
	csi_pin_set_mux(PA4,PA4_OPA3N); //PIN20 输入负端
	csi_pin_set_mux(PA5,PA5_OPA3X); //PIN21 输出
	
	//adc 参数配置
	tAdcConfig.byClkDiv = 0x02;							//ADC clk两分频：clk = pclk/2
	tAdcConfig.bySampHold = 0x06;						//ADC 采样时间： time = 16 + 6 = 22(ADC clk周期)
	tAdcConfig.byConvMode = ADC_CONV_ONESHOT;			//ADC 转换模式： 单次转换；
	tAdcConfig.byVrefSrc = ADCVERF_VDD_VSS;				//ADC 参考电压： 系统VDD
	tAdcConfig.wInt = ADC_INTSRC_NONE;				//ADC 中断配置： 无中断
	tAdcConfig.ptSeqCfg = (csi_adc_seq_t *)tSeqCfg;		//ADC 采样序列： 具体参考结构体变量 tSeqCfg
	
	csi_adc_init(ADC0, &tAdcConfig);							//初始化ADC参数配置	
	csi_adc_set_seqx(ADC0, tAdcConfig.ptSeqCfg, byChnlNum);		//配置ADC采样序列
	csi_adc_set_buffer((uint16_t *)g_hwAdcBuf, 1);				//传递ADC采样buffer，ADC采样值存放于此buffer中

	
	//OPA 参数配置
	ptOpaConfig_t.byMode = OPA_INTERNAL_MODE;
	ptOpaConfig_t.byInternalGain = OPA_PGAC_X4;
	ptOpaConfig_t.byInputMode = SINGEL;
	ptOpaConfig_t.byOpaNegativeInput = DISABLE;
	ptOpaConfig_t.byOpaOutput = ENABLE;
	
	csi_opa_init(OPA0,&ptOpaConfig_t);
	csi_opa_start(OPA0);
	
	csi_opa_init(OPA1,&ptOpaConfig_t);
	csi_opa_start(OPA1);  
	
	csi_opa_init(OPA2,&ptOpaConfig_t);
	csi_opa_start(OPA2); 
	
	csi_opa_init(OPA3,&ptOpaConfig_t);
	csi_opa_start(OPA3); 
	
	while(1)
	{
		csi_adc_start(ADC0);
		for(int i=0;i<6;i++)
		{
			g_hwAdcBuf[i] = csi_adc_read_channel(ADC0, i);
		}
		//需要使用my_printf串口输出，则需要手动配置输出的串口，因为默认的配置和OPA管脚冲突。
		//修改方法参考....\trunk\doc\AN1780_QuickStart_APT32F171x系列_CSI.doc文档的5.4.2小节的说明。
		//my_printf("OPA1 : Vp = %d , Vn = %d ,Vout = %d\n",g_hwAdcBuf[0],g_hwAdcBuf[2],g_hwAdcBuf[1]);
		//my_printf("OPA0 : Vp = %d , Vn = %d ,Vout = %d\n",g_hwAdcBuf[4],g_hwAdcBuf[5],g_hwAdcBuf[3]);
 
		mdelay(200);
	}
	
}

/********************************************************************
 * OPA外部增益模式使用接线示意
 * 场景① 外部增益模式接线图  
 * Vout =  vp * (1 + R2/R1)
 * gain =  1 + R2/R1
	 
					|╲
				Vp  |  ╲
				----|+   ╲
					|      ╲______Vout
					|      ╱  |
				Vn _|_   ╱    |
				  | |  ╱      |
			 R1   | |╱   R2   |
	GND___/\/\/\__|_____/\/\/\_| 		

 * 场景② 差分输入外部增益接线示意图
 * Vout =  (((R1+R2)/(R3+R4)) * (R4/R1) * V2) - ((R2/R1) * V1)
 * 当R1 = R3且R2 = R4时，Vout =（R2/R1）* (V2 - V1)
 * 
			 R1                R2 
	V1_____/\/\/\____________/\/\/\_
					|			   |
					|	|╲        |
					| N |  ╲      |
					|___|_   ╲    |
						|      ╲__|____Vout
			 R3		  P |      ╱  
	V2_____/\/\/\_______|+   ╱    
					|   |  ╱      
				R4  |   |╱     
	  GND___/\/\/\__|

 * 
********************************************************************/
/** \brief opa external gain mode test 
 *  \brief OPA外部增益模式测试
 * 
 *  \param[in] none
 *  \return error code
 */
 
void opa_external_gain_mode_test(void)
{
	csi_adc_config_t tAdcConfig;
	csi_opa_config_t ptOpaConfig_t;
	

	csi_pin_set_mux(PA0,PA0_OPA0P); //PIN14 输入正端
	csi_pin_set_mux(PA1,PA1_OPA0N); //PIN15 输入负端
	csi_pin_set_mux(PA2,PA2_OPA0X); //PIN16 输出
	csi_pin_set_mux(PB3,PB3_OPA1P); //PIN55 输入正端
	csi_pin_set_mux(PB4,PB4_OPA1N); //PIN56 输入负端
	csi_pin_set_mux(PB5,PB5_OPA1X); //PIN57 输出
	csi_pin_set_mux(PB0,PB0_OPA2P); //PIN26 输入正端
	csi_pin_set_mux(PA7,PA7_OPA2N); //PIN23 输入负端
	csi_pin_set_mux(PA6,PA6_OPA2X); //PIN22 输出
	csi_pin_set_mux(PA3,PA3_OPA3P); //PIN17 输入正端
	csi_pin_set_mux(PA4,PA4_OPA3N); //PIN20 输入负端
	csi_pin_set_mux(PA5,PA5_OPA3X); //PIN21 输出
	
	
	
	//adc 参数配置
	tAdcConfig.byClkDiv = 0x02;							//ADC clk两分频：clk = pclk/2
	tAdcConfig.bySampHold = 0x06;						//ADC 采样时间： time = 16 + 6 = 22(ADC clk周期)
	tAdcConfig.byConvMode = ADC_CONV_ONESHOT;			//ADC 转换模式： 单次转换；
	tAdcConfig.byVrefSrc = ADCVERF_VDD_VSS;				//ADC 参考电压： 系统VDD
	tAdcConfig.wInt = ADC_INTSRC_NONE;				//ADC 中断配置： 无中断
	tAdcConfig.ptSeqCfg = (csi_adc_seq_t *)tSeqCfg;		//ADC 采样序列： 具体参考结构体变量 tSeqCfg
	
	csi_adc_init(ADC0, &tAdcConfig);							//初始化ADC参数配置	
	csi_adc_set_seqx(ADC0, tAdcConfig.ptSeqCfg, byChnlNum);		//配置ADC采样序列
	csi_adc_set_buffer((uint16_t *)g_hwAdcBuf, 1);				//传递ADC采样buffer，ADC采样值存放于此buffer中
	//csi_adc_start(ADC0);		
	
	//OPA 参数配置
	ptOpaConfig_t.byMode = OPA_EXTERNAL_MODE;
	ptOpaConfig_t.byOpaNegativeInput = ENABLE;
	ptOpaConfig_t.byOpaOutput = ENABLE;
	
	csi_opa_init(OPA0,&ptOpaConfig_t);
	csi_opa_start(OPA0);
	
	csi_opa_init(OPA1,&ptOpaConfig_t);
	csi_opa_start(OPA1);
	
	csi_opa_init(OPA2,&ptOpaConfig_t);
	csi_opa_start(OPA2);
	
	csi_opa_init(OPA3,&ptOpaConfig_t);
	csi_opa_start(OPA3);
	
	while(1)
	{
		csi_adc_start(ADC0);
		for(int i=0;i<6;i++)
		{
			g_hwAdcBuf[i] = csi_adc_read_channel(ADC0, i);
		}
		//需要使用my_printf串口输出，则需要手动配置输出的串口，因为默认的配置和OPA管脚冲突。
		//修改方法参考....\trunk\doc\AN1780_QuickStart_APT32F171x系列_CSI.doc文档的5.4.2小节的说明。
		//my_printf("OPA1 : Vp = %d , Vn = %d ,Vout = %d\n",g_hwAdcBuf[0],g_hwAdcBuf[2],g_hwAdcBuf[1]);
		//my_printf("OPA0 : Vp = %d , Vn = %d ,Vout = %d\n",g_hwAdcBuf[4],g_hwAdcBuf[5],g_hwAdcBuf[3]);

		mdelay(200);
	}
	
}

