/***********************************************************************//** 
 * \file  cnta_demo.c
 * \brief  counterA 示例代码

 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version    <th>Author  <th>Description
 * <tr><td> 2020-09-21 <td>V0.0  <td>ZJY     <td>initial
 * <tr><td> 2020-05-27 <td>V0.1  <td>LQ      <td>modify
 * <tr><td> 2023-09-05 <td>V0.2  <td>LHY     <td>modify
 * </table>
 * *********************************************************************
*/

/* include ----------------------------------------------------------------*/
#include "cnta.h"
//#include "gpio.h"
//#include "pin.h"
#include "board_config.h"
#include "demo.h"
#include <csi_drv.h>

/* externs function--------------------------------------------------------*/
/* private function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/
/* device instance ------------------------------------------------------*/


/** \brief  cnta_callback：cnta中断回调函数
 * 
 * 	\brief	用户定义，使用csi标准库，中断发生时会自动调用用户注册的回调函
 * 			数，用户可在回调函数里做自己的处理，而不需要关注具体的底层中断处理。
 * 
 *  \param[out] ptCntaBase: CNTA寄存器结构体指针，指向CNTA的基地址 
 *  \return none
 */ 
void cnta_callback(csp_cnta_t *ptCntaBase)
{
	//user code
}



/**
  \brief       CounterA 定时中断示例
  \return      int
*/
int cnta_timer_callback_demo(void)
{	
	int iRet = 0;
	csi_cnta_timer_config_t tTimerCfg;

#if !defined(USE_GUI)		
	csi_pin_set_mux(PA10, PA10_OUTPUT);              //配置输出
	csi_pin_output_mode(PA10,GPIO_PUSH_PULL);        //推挽
	csi_pin_set_high(PA10);					         //初始为高，在cnta的中断中翻转
#endif
	
	tTimerCfg.eClkDiv = CNTA_CK_DIV8;
	tTimerCfg.wTime = 1000;                          //1000us,if cnta clk is 3M,the timeout for timer((0.333us * 1) ->(0.333us * 65535): 0.3333us -> 21.845ms)
	tTimerCfg.eRunMode = CNTA_REPEAT_MODE;
	csi_cnta_timer_init(CA0,&tTimerCfg);             //初始化CountA
	csi_cnta_int_enable(CA0, CNTA_INTSRC_PENDL);     //若需使用中断，请调该接口使能对应中断，这里使用PENDL中断
	csi_cnta_register_callback(CA0, cnta_callback);	 //注册中断回调函数
	csi_cnta_start(CA0);                             //启动CountA
	while(1)
	{
		nop;
	}
	return iRet;
}


