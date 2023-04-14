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
	csi_pin_irq_enable(PA1, ENABLE);							//使能GPIO中断	
	
	csi_bt_start_sync(BT0, 200);
	csi_bt_set_sync(BT0,BT_TRGIN_SYNCEN0, BT_TRG_ONCE, BT_TRG_AUTOAREARM);  
	csi_bt_set_evtrg(BT0, BT_TRGSRC_PEND, ENABLE);
	
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
 * 多个一对一触发模式
 *EXI1 		 --> BT_SYNC0 
 *BT0_TRGOUT --> LPT_SYNC0 
*/
int etcb_one_trg_more_demo0(void)
{
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				                //ETB 参数配置结构体		
	
	csi_pin_set_mux(PA1,PA1_INPUT);		
	csi_pin_pull_mode(PA1, GPIO_PULLUP);						//PA01 上拉
	csi_pin_irq_mode(PA1,EXI_GRP1, GPIO_IRQ_FALLING_EDGE);		//PA01 下降沿产生中断	
	csi_exi_set_evtrg(1, TRGSRC_EXI1, 1);
	csi_pin_irq_enable(PA1, ENABLE);							//使能GPIO中断	
	
	
	csi_bt_start_sync(BT0, 200);
	csi_bt_set_sync(BT0,BT_TRGIN_SYNCEN0, BT_TRG_ONCE, BT_TRG_AUTOAREARM);  
	
	csi_bt_start_sync(BT1, 200);
	csi_bt_set_sync(BT1,BT_TRGIN_SYNCEN0, BT_TRG_ONCE, BT_TRG_AUTOAREARM);  

	
	csi_bt_start_sync(BT2, 200);
	csi_bt_set_sync(BT2,BT_TRGIN_SYNCEN0, BT_TRG_ONCE, BT_TRG_AUTOAREARM);  	
	
	
	tEtbConfig.byChType = ETB_ONE_TRG_MORE;  		//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_EXI_TRGOUT1 ;  	    //EXI1 触发输出0作为触发源
	tEtbConfig.bySrcIp1 = ETB_SRC_NOT_USE;          //无触发源2      
	tEtbConfig.bySrcIp2 = ETB_SRC_NOT_USE;          //无触发源3
	tEtbConfig.byDstIp =  ETB_BT0_SYNCIN0;   	    //BT0 同步输入作为目标事件
	tEtbConfig.byDstIp1 = ETB_BT1_SYNCIN0;          //无目标事件2
	tEtbConfig.byDstIp2 = ETB_BT2_SYNCIN0;          //无目标事件3
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
   
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
	return iRet;
}

