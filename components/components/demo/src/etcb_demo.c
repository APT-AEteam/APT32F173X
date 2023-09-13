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
	csi_bt_set_sync(BT0, BT_SYNCIN0, BT_TRG_CONTINU, BT_TRG_SYCAREARM);	//外部触发bt0启动(SYNCIN0)
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
	csi_bt_set_sync(BT0, BT_SYNCIN1, BT_TRG_CONTINU, BT_TRG_AUTOAREARM);	//外部触发BT0停止(SYNCIN1)，连续模式
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

