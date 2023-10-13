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
#include "csi_drv.h"
#include "board_config.h"

/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

/** \brief	exi trg bt start：EXI通过ETCB触发BT启动的demo
 * 
 *  \brief	PB1外部下降沿触发BT0启动，对应VIC中断不开启，即不进入PB1的外部中断；若需要进入PB1外部中断，需
 * 			开启对应的VIC中断，调用csi_gpio_vic_irq_enable接口函数
 *  
 *  \param[in] none
 *  \return error code
 */
int exi_etcb_bt_start_demo(void)
{
	int iRet = 0;
	volatile uint8_t ch = 0;
	csi_etcb_config_t tEtbConfig;				               			//ETCB 参数配置结构体	
	csi_bt_time_config_t tTimConfig;									//BT 定时初始化参数结构体	

#if (USE_GUI == 0)			
	csi_gpio_set_mux(GPIOB, PB1, PB1_INPUT);							//PB1 配置为输入
	csi_gpio_pull_mode(GPIOB, PB1, GPIO_PULLUP);						//PB1 上拉
	csi_gpio_int_enable(GPIOB, PB1);									//PB1 中断使能	
	csi_gpio_irq_mode(GPIOB, PB1, EXI_GRP1, GPIO_IRQ_FALLING_EDGE);		//PB1 下降沿产生中断，选择中断组1
	csi_exi_set_evtrg(EXI_TRGOUT1, EXI_TRGSRC_GRP1, 0);					//EXI 触发输出配置
	csi_exi_evtrg_enable(EXI_TRGOUT1);									//使能 EXI_TRGOUT1触发输出
	
	csi_gpio_set_mux(GPIOA, PA6, PA6_OUTPUT);							//PA6 output ，并在BT0中断里面翻转IO
	csi_gpio_set_high(GPIOA, PA6);										//PA6 output high;		
#endif
	
	tTimConfig.wTimeVal = 1000;											//BT定时值 = 1000us
	tTimConfig.eRunMode  = BT_RUN_CONT;									//BT计数器工作模式
	csi_bt_timer_init(BT0,&tTimConfig);									//BT0 定时	
	csi_bt_set_sync(BT0, BT_SYNCIN0, BT_SYNC_CONT, BT_TRG_SYCAREARM);	//外部触发bt0启动(SYNCIN0)
	csi_bt_sync_enable(BT0, BT_SYNCIN0);								//BT0 同步输入2使能
	
	//ETCB 初始化
	tEtbConfig.eChType = ETCB_ONE_TRG_ONE;  								//单个源触发单个目标
	tEtbConfig.eSrcIp  = ETCB_EXI_TRGOUT1;  	    						//EXI_TRGOUT1作为触发源
	tEtbConfig.eDstIp =  ETCB_BT0_SYNCIN0;   	   						//BT0 同步输入作为目标事件
	tEtbConfig.eTrgMode = ETCB_HARDWARE_TRG;
	ch = csi_etcb_ch_alloc(tEtbConfig.eChType);	    					//自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    					//ch < 0,则获取通道号失败
	iRet = csi_etcb_ch_init(ch, &tEtbConfig);
	
	while(1)
	{
		nop;
	}
	return iRet;
}


/** \brief exi trg bt stop/start: EXI通过ETCB触发BT停止/启动demo
 * 
 *  \brief	PB1外部下降沿触发BT0停止/启动，当BT处于运行状态时才有效，即第一次触发关闭BT，再次触发开启BT，
 * 			依次类推。
 *
 *  \param[in] none
 *  \return error code
// */
int exi_etcb_bt_stop_demo(void)
{
	int iRet = 0;
	volatile uint8_t ch = 0;
	csi_etcb_config_t tEtbConfig;				               				//ETCB 参数配置结构体
	csi_bt_time_config_t tTimConfig;										//BT 定时初始化参数结构体				

#if (USE_GUI == 0)			
	csi_gpio_set_mux(GPIOB, PB1, PB1_INPUT);								//PB1 配置为输入
	csi_gpio_pull_mode(GPIOB, PB1, GPIO_PULLUP);							//PB1 上拉
	csi_gpio_int_enable(GPIOB, PB1);										//PB1 中断使能	
	csi_gpio_irq_mode(GPIOB, PB1, EXI_GRP1, GPIO_IRQ_FALLING_EDGE);			//PB1 下降沿产生中断，选择中断组1
	csi_exi_set_evtrg(EXI_TRGOUT1, EXI_TRGSRC_GRP1, 0);						//EXI 触发输出配置
	csi_exi_evtrg_enable(EXI_TRGOUT1);										//使能 EXI_TRGOUT1触发输出
	
	csi_gpio_set_mux(GPIOA, PA6, PA6_OUTPUT);								//PA6 output ，并在BT0中断里面翻转IO
	csi_gpio_set_high(GPIOA, PA6);											//PA6 output high;	
#endif	
	
	tTimConfig.wTimeVal = 1000;												//BT定时值 = 1000us
	tTimConfig.eRunMode  = BT_RUN_CONT;										//BT计数器工作模式
	csi_bt_timer_init(BT0,&tTimConfig);										//BT0 定时	
	csi_bt_set_sync(BT0, BT_SYNCIN1, BT_SYNC_CONT, BT_TRG_AUTOAREARM);		//外部触发BT0停止(SYNCIN1)，连续模式
	csi_bt_sync_enable(BT0, BT_SYNCIN1);									//BT0 同步输入1使能
	
	csi_bt_start(BT0);	    												//启动BT0
	
	tEtbConfig.eChType = ETCB_ONE_TRG_ONE;  								//单个源触发单个目标
	tEtbConfig.eSrcIp  = ETCB_EXI_TRGOUT1;  	    						//EXI_TRGOUT1作为触发源
	tEtbConfig.eDstIp =  ETCB_BT0_SYNCIN1;   	    						//BT0 同步输入作为目标事件
	tEtbConfig.eTrgMode = ETCB_HARDWARE_TRG;
	ch = csi_etcb_ch_alloc(tEtbConfig.eChType);	    						//自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    						//ch < 0,则获取通道号失败
	iRet = csi_etcb_ch_init(ch, &tEtbConfig);
	
	while(1)
	{
		nop;
	}
	return iRet;	
}

/** \brief	exi trg adc samp：EXI通过ETCB触发ADC采样的demo
 * 
 *  \brief	PB1外部下降沿触发ADC采样，对应VIC中断不开启，即不进入PB1的外部中断；若需要进入PB1外部中断，需
 * 			开启对应的VIC中断，调用csi_gpio_vic_irq_enable接口函数
 *  
 *  \param[in] none
 *  \return error code
 */
int exi_etcb_adc_samp_demo(void)
{
	int iRet = 0;	
	int iChnlNum = 1;
	volatile uint8_t ch = 0;											//设置ADC总通道数
	csi_etcb_config_t tEtbConfig;				               			//ETCB 参数配置结构体	
	csi_adc_config_t tAdcConfig;										//ADC初始化参数结构体	

#if (USE_GUI == 0)			
	csi_gpio_set_mux(GPIOB, PB1, PB1_INPUT);							//PB1 配置为输入
	csi_gpio_pull_mode(GPIOB, PB1, GPIO_PULLUP);						//PB1 上拉
	csi_gpio_int_enable(GPIOB, PB1);									//PB1 中断使能	
	csi_gpio_irq_mode(GPIOB, PB1, EXI_GRP1, GPIO_IRQ_FALLING_EDGE);		//PB1 下降沿产生中断，选择中断组1
	csi_exi_set_evtrg(EXI_TRGOUT1, EXI_TRGSRC_GRP1, 0);					//EXI 触发输出配置
	csi_exi_evtrg_enable(EXI_TRGOUT1);									//使能 EXI_TRGOUT1触发输出
	csi_gpio_set_mux(GPIOC,PC13, PC13_ADC_INA0);						//ADC 输入通道配置
	
#endif
	
	tAdcConfig.byClkDiv = 0x02;									//ADC clk两分频：clk = pclk/2
	tAdcConfig.eClkSel = ADC_CLK_PCLK;							//ADC clk选择：PCLK
	tAdcConfig.bySampHold = 0x06;								//ADC 采样时间： time = 16 + 6 = 22(ADC clk周期)
	tAdcConfig.eRunMode = ADC_RUN_ONCE;							//ADC 转换模式： 单次转换；
	tAdcConfig.eVrefSrc = ADCVERF_VDD_VSS;						//ADC 参考电压： 系统VDD	
	csi_adc_init(ADC0, &tAdcConfig);							//初始化ADC参数配置	
	
	csi_adc_set_seq_num(ADC0,iChnlNum);													//配置ADC总采样通道个数
	csi_adc_set_seqx(ADC0,0,ADC_INA0,ADC_CV_COUNT_1,ADC_AVG_COF_1,ADCSYNC_IN0);			//配置ADC采样通道0，触发信号为ADCSYNC_IN0
	csi_adc_set_sync(ADC0, ADC_SYNCIN0, ADC_SYNC_CONT, 0); 		//选择ADC_SYNCEN0同步事件，同步事件发生后延迟0us@pclk=40Mhz启动ADC
	
	//ETCB 初始化
	tEtbConfig.eChType = ETCB_ONE_TRG_ONE;  							//单个源触发单个目标
	tEtbConfig.eSrcIp  = ETCB_EXI_TRGOUT1;  	    					//EXI_TRGOUT1作为触发源
	tEtbConfig.eDstIp =  ETCB_ADC0_SYNCIN0;   	   						//ADC0 同步输入作为目标事件
	tEtbConfig.eTrgMode = ETCB_HARDWARE_TRG;
	ch = csi_etcb_ch_alloc(tEtbConfig.eChType);	    					//自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    					//ch < 0,则获取通道号失败
	iRet = csi_etcb_ch_init(ch, &tEtbConfig);
	
	while(1)
	{
		nop;
	}
	return iRet;
}