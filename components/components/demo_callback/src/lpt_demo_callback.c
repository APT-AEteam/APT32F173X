/***********************************************************************//** 
 * \file  lpt_demo.c
 * \brief  LPT_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-9-14 <td>V0.0  <td>YT     <td>initial
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

#if (USE_LPT_CALLBACK == 1)	
	
/** \brief  user_lpt_callback：lpt中断回调函数
 * 
 * 	\brief	用户定义，支持指PEND/TRGEV/MATCH三种中断处理，使用csi标准库，中断发生时会自动调用用户注册的回调函
 * 			数，用户可在回调函数里做自己的处理，而不需要关注具体的底层中断处理。
 * 
 *  \param[out] ptLptBase: 	LPTx寄存器结构体指针，指向LPTx的基地址 
 *  \param[out] byIsr: 		LPTx中断状态
 *  \return none
 */ 
void user_lpt_callback(csp_lpt_t *ptLptBase, uint8_t byIsr)
{
	if(byIsr & LPT_INTSRC_PEND)
	{
		csi_gpio_toggle(GPIOA,PA1);						//PA1翻转
	}
}

/** \brief lpt_timer_int_callback_demo：LPT做基本定时器功能demo，使用callback
* 	\brief  csi接口默认使用(开启)周期结束中断，并在中断里面翻转IO(需要打开PA1IO配置），若不需要开启中断，可调
 * 			用csi_bt_int_disable接口函数，关闭周期结束中断
 *  \param[in] none
 *  \return error code
 */
int lpt_timer_int_callback_demo(void)
{
	int iRet = 0;

	csi_lpt_time_config_t tTimConfig;

	csi_gpio_set_mux(GPIOA, PA1, PA1_OUTPUT);			//初始化PA1为输出
	
	tTimConfig.wTimeVal = 200;					//LPT定时值 = 200ms
	tTimConfig.eRunMode  = LPT_CNT_CONT;		//LPT计数器工作模式，连续
	tTimConfig.eClksrc=LPT_CLK_PCLK_DIV4;  		//LPT时钟源  
	csi_lpt_timer_init(LPT,&tTimConfig);        //初始化lpt,默认采用PEND中断
	
	csi_lpt_register_callback(LPT, user_lpt_callback);	//注册中断回调函数
	
	csi_lpt_start(LPT);	                             //启动lpt
	
	return iRet;	
}



#endif