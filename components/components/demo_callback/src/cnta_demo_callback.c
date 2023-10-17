/***********************************************************************//** 
 * \file  cnta_demo.c
 * \brief  counterA 示例代码

 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version    <th>Author  <th>Description
 * <tr><td> 2020-09-21 <td>V0.0  <td>ZJY     <td>initial
 * <tr><td> 2020-05-27 <td>V0.1  <td>LQ      <td>modify
 * <tr><td> 2023-09-05 <td>V1.0  <td>LHY     <td>code normalization
 * </table>
 * *********************************************************************
*/
/* include ----------------------------------------------------------------*/
#include "csi_drv.h"
#include "board_config.h"

#if (USE_CNTA_CALLBACK == 1)
/* externs function--------------------------------------------------------*/
/* private function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/
/* device instance ------------------------------------------------------*/


/** \brief     cnta_callback：cnta中断回调函数
 * 
 * 	\brief	   用户定义，使用csi标准库，中断发生时会自动调用用户注册的回调函
 * 			   数，用户可在回调函数里做自己的处理，而不需要关注具体的底层中断处理。
 * 
 *  \param[in] ptCntaBase: CNTA寄存器结构体指针，指向CNTA的基地址 
 *  \return    无返回
 */ 
void cnta_callback(csp_cnta_t *ptCntaBase)
{
	//user code
}



/** \brief      CounterA 定时中断示例
 *  \param[in]  无参数
 *  \return     返回int类型
 */
int cnta_timer_int_callback_demo(void)
{	
	int iRet = 0;
	csi_cnta_timer_config_t tTimerCfg;

#if (USE_GUI == 0)			
	csi_gpio_set_mux(GPIOA, PA10, PA10_OUTPUT);              //配置输出
	csi_gpio_output_mode(GPIOA, PA10,GPIO_PUSH_PULL);        //推挽
	csi_gpio_set_high(GPIOA, PA10);					         //初始为高，在cnta的中断中翻转
#endif
	
	tTimerCfg.eClkDiv = CNTA_CK_DIV8;                        //8分频
	tTimerCfg.wTime = 1000;                                  //周期1000us,if cnta clk is 3M,the timeout for timer((0.333us * 1) ->(0.333us * 65535): 0.3333us -> 21.845ms)
	tTimerCfg.eRunMode = CNTA_RUN_CONT;                      //重复模式
	csi_cnta_timer_init(CA0,&tTimerCfg);                     //初始化CountA
	
	csi_cnta_int_enable(CA0, CNTA_INTSRC_PENDL);             //使能cnta PENDL中断
	csi_cnta_register_callback(CA0, cnta_callback);	         //注册中断回调函数
	
	csi_cnta_start(CA0);                                     //启动CountA
	
	while(1)
	{
		nop;
	}
	return iRet;
}
#endif
