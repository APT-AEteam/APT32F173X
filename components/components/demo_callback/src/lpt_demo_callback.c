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

#include "lpt.h"
#include "pin.h"
#include "etcb.h"
#include "bt.h"
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
	if(byIsr & LPT_PEND_INT)
	{
		csi_gpio_toggle(GPIOA,PA1);						//PA1翻转
	}
}

/** \brief lpt timer
 * 
 *  \param[in] none
 *  \return error code
 */
int lpt_timer_callback_demo(void)
{
	int iRet = 0;

	csi_lpt_time_config_t tTimConfig;
	
	tTimConfig.wTimeVal = 200;					//LPT定时值 = 200ms
	tTimConfig.eWkMode  = LPT_CNT_CONTINU;		//LPT计数器工作模式，连续
	tTimConfig.eClksrc=LPT_CLK_PCLK_DIV4;  		//LPT时钟源  
	csi_lpt_timer_init(LPT,&tTimConfig);        //初始化lpt,默认采用PEND中断
	
	csi_lpt_register_callback(LPT, user_lpt_callback);	//注册中断回调函数
	
	csi_lpt_start(LPT);	                             //启动lpt
	
	return iRet;	
}


/** \brief lpt pwm ouput
 * 
 *  \param[in] none
 *  \return error code
 */
int lpt_pwm_callback_demo(void)
{
	int iRet = 0;
	
	csi_lpt_pwm_config_t tLptPwmCfg;  	
#if (USE_GUI == 0)		
	csi_gpio_set_mux(GPIOB,PB6, PB6_LPT_OUT);	                           //将PB6设为LPT_OUT
#endif

	tLptPwmCfg.byClksrc = LPT_CLK_PCLK_DIV4;                          //PWM 时钟选择
	tLptPwmCfg.byStartpol = LPT_PWM_START_LOW;                    //初始低电平
	tLptPwmCfg.byIdlepol  = LPT_PWM_IDLE_LOW;                     //停止时highZ
	tLptPwmCfg.byCycle = 30;                                     //PWM 输出占空比(0~100)	
	tLptPwmCfg.wFreq = 1000;                                     //PWM 输出频率
	csi_lpt_int_enable(LPT,LPT_INTSRC_PEND);	 //enable PEND interrupt
	
	csi_lpt_register_callback(LPT, user_lpt_callback);	//注册中断回调函数
	
	if(csi_lpt_pwm_init(LPT, &tLptPwmCfg) == CSI_OK){            //初始化lpt

		csi_lpt_start(LPT);                                      //启动lpt
	}	
	return iRet;	
}

#endif