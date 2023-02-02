/***********************************************************************//** 
 * \file  etcb_demo.c
 * \brief  ETCB_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-13 <td>V0.0 <td>ZJY    <td>initial
 * <tr><td> 2021-7-28 <td>V0.1 <td>YYM    <td>modify
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include "drv/etb.h"
#include "drv/pin.h"
#include "drv/bt.h"
#include "drv/lpt.h"
#include "drv/adc.h"
#include "demo.h"
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

/*
 * 多个一对一触发模式
 *EXI1 		 --> BT_SYNC0 
 *BT0_TRGOUT --> LPT_SYNC0 
*/
int etcb_one_trg_one_demo0(void)
{
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				                //ETB 参数配置结构体		
	
	csi_pin_set_mux(PA1,PA1_INPUT);		
	csi_pin_pull_mode(PA1, GPIO_PULLUP);						//PA01 上拉
	csi_pin_irq_mode(PA1,EXI_GRP1, GPIO_IRQ_FALLING_EDGE);		//PA01 下降沿产生中断	
	csi_exi_set_evtrg(1, TRGSRC_EXI1, 1);
	
	csi_bt_start_sync(BT0, 200);
	csi_bt_set_sync(BT0,BT_TRGIN_SYNCEN0, BT_TRG_ONCE, ENABLE);  
	csi_bt_set_evtrg(BT0, 0, BT_TRGSRC_PEND);
	
	csi_lpt_start_sync(LPT,LPT_CLK_PCLK_DIV4,50);
	csi_lpt_set_sync(LPT, LPT_TRG_SYNCIN0, LPT_SYNC_ONCE, ENABLE);
	
	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  		//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_EXI_TRGOUT1 ;  	    //EXI1 触发输出0作为触发源
	tEtbConfig.bySrcIp1 = ETB_SRC_NOT_USE;          //无触发源2      
	tEtbConfig.bySrcIp2 = ETB_SRC_NOT_USE;          //无触发源3
	tEtbConfig.byDstIp =  ETB_BT0_SYNCIN0;   	    //BT0 同步输入作为目标事件
	tEtbConfig.byDstIp1 = ETB_DST_NOT_USE;          //无目标事件2
	tEtbConfig.byDstIp2 = ETB_DST_NOT_USE;          //无目标事件3
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
   
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  		//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_BT0_TRGOUT ;  	    //BT0 触发输出0作为触发源
	tEtbConfig.bySrcIp1 = ETB_SRC_NOT_USE;          //无触发源2       
	tEtbConfig.bySrcIp2 = ETB_SRC_NOT_USE;          //无触发源3
	tEtbConfig.byDstIp =  ETB_LPT_SYNCIN;   	    //LPT0 同步输入作为目标事件
	tEtbConfig.byDstIp1 = ETB_DST_NOT_USE;          //无目标事件2
	tEtbConfig.byDstIp2 = ETB_DST_NOT_USE;          //无目标事件3
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
   
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);	
	
	return iRet;
}

/*
 * 一对一触发模式
 *EPT --> ADC_SYNC0 
*/
uint8_t  byAdcChnlNum;                                          //采样序列的通道数
const csi_adc_seq_t SeqCfg1[] =
{
	//输入通道		//连续重复采样次数		//平均系数			//触发源选择
	{ADCIN1,		ADC_CV_COUNT_1,			ADC_AVG_COF_1,		ADCSYNC_IN0},
	{ADCIN2,		ADC_CV_COUNT_1,			ADC_AVG_COF_1,		ADCSYNC_IN0},
	{ADCIN3,		ADC_CV_COUNT_1,			ADC_AVG_COF_1,		ADCSYNC_IN0},
	{ADCIN10,		ADC_CV_COUNT_1,			ADC_AVG_COF_1,		ADCSYNC_IN0},
	{ADCIN11,		ADC_CV_COUNT_1,			ADC_AVG_COF_1,		ADCSYNC_IN0},	
	{ADCIN12,		ADC_CV_COUNT_1,			ADC_AVG_COF_1,		ADCSYNC_IN0},	
}; 

//中断回调函数，使用ETCB模式，触发ADC采集的过程中，将中断处理函数apt_adc_irqhandler替换成apt_etcb_adc_irqhandler
int16_t hwAdc_Data[6] = {0};
void apt_etcb_adc_irqhandler(csp_adc_t *ptAdcBase)
{
	uint32_t wIntStat = csp_adc_get_sr(ptAdcBase) & csp_adc_get_isr(ptAdcBase);
	
	csi_pin_toggle(PA5);
	
	//ADC EOC interrupt
	if(wIntStat&ADC12_EOC)
	{
		for(uint8_t adi = 0;adi < byAdcChnlNum;adi ++)
		{
			hwAdc_Data[adi] = csi_adc_read_channel(ADC0, adi);
		}
		csp_adc_clr_sr(ptAdcBase, ADC12_EOC);	
	}
}

/*
 * 多对一触发模式
 *EXI0 EXI1 EXI3--> ADC_SYNC0 
*/
const csi_adc_seq_t SeqCfg3[] =
{
	//输入通道		//连续重复采样次数		//平均系数			//触发源选择
	{ADCIN10,		ADC_CV_COUNT_1,			ADC_AVG_COF_1,		ADCSYNC_IN0},
	{ADCIN11,		ADC_CV_COUNT_1,			ADC_AVG_COF_1,		ADCSYNC_IN0},	
	{ADCIN12,		ADC_CV_COUNT_1,			ADC_AVG_COF_1,		ADCSYNC_IN0},	
};

void etcb_adc_config13(void)
{
	csi_adc_config_t tAdcConfig;
	
	//adc 输入管脚配置
//	csi_pin_set_mux(PA010, PA010_ADC_AIN10);
//	csi_pin_set_mux(PA011, PA011_ADC_AIN11);
//	csi_pin_set_mux(PA012, PA012_ADC_AIN12);

	byAdcChnlNum = 3;                                           //通道数配置为3路                               
	
	//adc 参数配置初始化
	tAdcConfig.byClkDiv = 2;									//ADC clk两分频：clk = pclk/2
	tAdcConfig.bySampHold = 0x06;								//ADC 采样时间： time = 16 + 6 = 22(ADC clk周期)
	tAdcConfig.byConvMode = ADC_CONV_ONESHOT;					//ADC 转换模式： 单次转换；
	tAdcConfig.byVrefSrc = ADCVERF_VDD_VSS;						//ADC 参考电压： 系统VDD
	tAdcConfig.wInt = ADC_INTSRC_EOC;                         //打开EOC中断

	tAdcConfig.ptSeqCfg = (csi_adc_seq_t *)SeqCfg3;		        //ADC 采样序列： 具体参考结构体变量 SeqCfg3
	
	csi_adc_init(ADC0, &tAdcConfig);							 //初始化ADC参数配置	
	csi_adc_set_seqx(ADC0, tAdcConfig.ptSeqCfg, byAdcChnlNum);	 //配置ADC采样序列
	csi_adc_set_sync(ADC0, ADC_TRG_SYNCEN0, ADC_TRG_CONTINU, 50);     //选择ADC_SYNCEN0同步事件  
	csi_adc_start(ADC0);										 //启动ADC	
}

int etcb_more_trg_one_demo(void)
{	
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				               //ETB 参数配置结构体		
	
	csi_pin_set_mux(PA5,PA5_OUTPUT);	
	
	csi_pin_set_mux(PA0,PA0_INPUT);	
	csi_pin_set_mux(PA1,PA1_INPUT);	
	csi_pin_set_mux(PA3,PA3_INPUT);	
	
	csi_pin_pull_mode(PA0, GPIO_PULLNONE);						//PA00 无上下拉
	csi_pin_pull_mode(PA1, GPIO_PULLUP);						//PA01 上拉
	csi_pin_pull_mode(PA3, GPIO_PULLUP);						//PA03 上拉
	
	csi_pin_irq_mode(PA0,EXI_GRP0, GPIO_IRQ_RISING_EDGE);		//PA00 上升沿产生中断	
	csi_pin_irq_mode(PA1,EXI_GRP1, GPIO_IRQ_FALLING_EDGE);		//PA01 下降沿产生中断		
	csi_pin_irq_mode(PA3,EXI_GRP3, GPIO_IRQ_FALLING_EDGE);		//PA03 下降沿产生中断	
	
	csi_exi_set_evtrg(1, TRGSRC_EXI0, 4);                       //PA00 4次上升沿触发目标事件     
	csi_exi_set_evtrg(2, TRGSRC_EXI1, 1);
	csi_exi_set_evtrg(3, TRGSRC_EXI3, 1);

	etcb_adc_config13();
	
	tEtbConfig.byChType = ETB_MORE_TRG_ONE;  		//多个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_EXI_TRGOUT1 ;  	    //EXI1 触发输出0作为触发源
	tEtbConfig.bySrcIp1 = ETB_EXI_TRGOUT2;      
	tEtbConfig.bySrcIp2 = ETB_EXI_TRGOUT3;
	tEtbConfig.byDstIp =  ETB_ADC0_SYNCIN0;   	    //ADC0 同步输入作为目标事件
	tEtbConfig.byDstIp1 = 0xff;
	tEtbConfig.byDstIp2 = 0xff;
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
   
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
	return iRet;
}

/*
 * 多对一/一对一混合触发模式
 *EXI0 EXI1 EXI3--> BT_SYNC0 
 *BT0_TRGOUT    --> LPT_SYNC0 
 *LPT_TRGOUT    --> ADC_SYNC0 
*/
int etcb_mix_demo(void)
{	
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				               //ETB 参数配置结构体		
	
	csi_pin_set_mux(PA5,PA5_OUTPUT);	
	
	csi_pin_set_mux(PA0,PA0_INPUT);	
	csi_pin_set_mux(PA1,PA1_INPUT);	
	csi_pin_set_mux(PA3,PA3_INPUT);	
	
	csi_pin_pull_mode(PA0, GPIO_PULLNONE);						//PA00 无上下拉
	csi_pin_pull_mode(PA1, GPIO_PULLUP);						//PA01 上拉
	csi_pin_pull_mode(PA3, GPIO_PULLUP);						//PA03 上拉
	
	csi_pin_irq_mode(PA0,EXI_GRP0, GPIO_IRQ_RISING_EDGE);		//PA00 上升沿产生中断	
	csi_pin_irq_mode(PA1,EXI_GRP1, GPIO_IRQ_FALLING_EDGE);		//PA01 下降沿产生中断		
	csi_pin_irq_mode(PA3,EXI_GRP3, GPIO_IRQ_FALLING_EDGE);		//PA03 下降沿产生中断	
	
	csi_exi_set_evtrg(1, TRGSRC_EXI0, 4);                       //PA00 4次上升沿触发目标事件     
	csi_exi_set_evtrg(2, TRGSRC_EXI1, 1);
	csi_exi_set_evtrg(3, TRGSRC_EXI3, 1);

	csi_bt_start_sync(BT0, 200);
	csi_bt_set_sync(BT0,BT_TRGIN_SYNCEN0, BT_TRG_ONCE, ENABLE);  
	
	csi_bt_set_evtrg(BT0, 0, BT_TRGSRC_PEND);
		
	csi_lpt_start_sync(LPT,LPT_CLK_ISCLK,100);
	csi_lpt_set_sync(LPT, 0, LPT_SYNC_ONCE, ENABLE);
	csi_lpt_set_evtrg(LPT, 0, LPT_TRGSRC_PRD, 1);
	
	etcb_adc_config13();
	
	tEtbConfig.byChType = ETB_MORE_TRG_ONE;  		//多个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_EXI_TRGOUT1 ;  	    //EXI1 触发输出0作为触发源
	tEtbConfig.bySrcIp1 = ETB_EXI_TRGOUT2;      
	tEtbConfig.bySrcIp2 = ETB_EXI_TRGOUT3;
	tEtbConfig.byDstIp =  ETB_BT0_SYNCIN0;   	    //BT0 同步输入作为目标事件
	tEtbConfig.byDstIp1 = 0xff;
	tEtbConfig.byDstIp2 = 0xff;
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
   
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  		//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_BT0_TRGOUT ;  	    //BT0 触发输出0作为触发源
	tEtbConfig.bySrcIp1 = 0xff;      
	tEtbConfig.bySrcIp2 = 0xff;
	tEtbConfig.byDstIp =  ETB_LPT_SYNCIN;   	    //LPT 同步输入作为目标事件
	tEtbConfig.byDstIp1 = 0xff;
	tEtbConfig.byDstIp2 = 0xff;
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
   
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);	
	
	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  		//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_LPT_TRGOUT0 ;  	    //LPT 触发输出0作为触发源
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
		
	return iRet;
}
