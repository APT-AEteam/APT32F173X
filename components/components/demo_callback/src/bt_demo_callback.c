/***********************************************************************//** 
 * \file  bt_demo_callback.c
 * \brief  BT_DEMO_CALLBACK description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-6-11 <td>V0.0 <td>ZJY     <td>initial
 * <tr><td> 2023-9-19 <td>V0.1 <td>ZJY     <td>initial
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

#if (USE_BT_CALLBACK == 0)									

/** \brief  user_bt0_callback：BT0中断回调函数
 * 
 * 	\brief	用户定义，支持指PEND/CMP/EVTRG三种中断处理，使用csi标准库，中断发生时会自动调用用户注册的回调函
 * 			数，用户可在回调函数里做自己的处理，而不需要关注具体的底层中断处理。
 * 
 *  \param[in]  ptBtBase: 	BTx寄存器结构体指针，指向BTx的基地址 
 *  \param[out] byIsr: 		BTx中断状态
 *  \return none
 */ 
void user_bt0_callback(csp_bt_t *ptBtBase, uint8_t byIsr)
{
	if(byIsr & BT_INT_PEND)
	{
		csi_gpio_toggle(GPIOA,PA6);						//PA6翻转
	}
}

/** \brief	bt_timer_callback_demo：BT做基本定时器功能demo，使用callback

 * 	\brief	csi初始化里不开启中断，需要进中断函数，需调用csi_bt_int_enable接口；demo默认使用PEND中断，并在中断
 * 			里面翻转IO(需要打开PA6 IO配置注释)；若不需要开启中断，调用csi_bt_int_disable接口，关闭周期结束中断
 *  
 * @ 工作模式:	BT_RUN_CONT: 连续工作模式, 指计数结束，计数器重新开始计数，周期执行
 * 				BT_RUN_ONCE: 单次工作模式，指计数结束，计数器停止工作
 * 
 *  \param[in] none
 *  \return 0/-1
 */
int bt_timer_callback_demo(void)
{
	int iRet = 0;
	csi_bt_time_config_t tTimConfig= {0};
	
	csi_gpio_set_mux(GPIOA, PA6, PA6_OUTPUT);			//初始化PA6为输出
	
	tTimConfig.wTimeVal = 1000;							//BT定时值，单位：us；即定时值 = 1000us
	tTimConfig.eRunMode  = BT_RUN_CONT;					//BT计数器工作模式：连续/单次
	csi_bt_timer_init(BT0, &tTimConfig);				//初始化BT	
	
	csi_bt_int_enable(BT1,BT_INTSRC_PEND);				//使能BT PEND中断
	csi_bt_register_callback(BT0, user_bt0_callback);	//注册中断回调函数
	csi_bt_start(BT0);									//启动BT定时器,定时默认需使用PEND中断

	while(1)
	{
		mdelay(100);
	}
	return iRet;
}

/** \brief  bt1_callback：BT1中断回调函数
 * 
 * 	\brief	用户定义，支持指PEND/CMP/EVTRG三种中断处理，使用csi标准库，中断发生时会自动调用用户注册的回调函
 * 			数，用户可在回调函数里做自己的处理，而不需要关注具体的底层中断处理。
 * 
 *  \param[in]  ptBtBase:	BTx寄存器结构体指针，指向BTx的基地址 
 *  \param[out] byIsr: 		BTx中断状态
 *  \return none
 */ 
void user_bt1_callback(csp_bt_t *ptBtBase, uint8_t byIsr)
{
	if(byIsr & BT_INTSRC_CMP)
	{
		csi_gpio_toggle(GPIOA, PA6);					//PA6翻转
	}
}

/** \brief  bt_pwm_callback_demo: BT0 PWM输出，中断使用callback
 * 
 * @ 工作模式:	BT_RUN_CONT: 连续工作模式, 指计数结束，计数器重新开始计数，周期执行
 * 				BT_RUN_ONCE: 单次工作模式，指计数结束，计数器停止工作
 * 
 *  \param[in] none
 *  \return error code
 */
int bt_pwm_callback_demo(void)
{
	int iRet = 0;
	csi_bt_pwm_config_t tPwmConfig= {0};						//BT PWM输出参数初始化配置结构体

	csi_gpio_set_mux(GPIOA,PA6,PA6_OUTPUT);				//初始化PA6为输出

#if (USE_GUI == 0)		
	csi_gpio_set_mux(GPIOC, PC11, PC11_BT1_OUT);		//PC11 作为BT1 PWM输出引脚
#endif
	
	tPwmConfig.eIdleLevel 	= BT_PWM_IDLE_HIGH;			//PWM 输出空闲电平
	tPwmConfig.eStartLevel	= BT_PWM_START_HIGH;		//PWM 输出起始电平
	tPwmConfig.eRunMode  	= BT_RUN_CONT;				//BT计数器工作模式：连续/单次
	tPwmConfig.byDutyCycle 	= 70;						//PWM 输出占空比(0 < DutyCycle < 100)		
	tPwmConfig.wFreq 		= 50;						//PWM 输出频率
	csi_bt_pwm_init(BT1, &tPwmConfig);					//初始化BT1 PWM输出
	
	csi_bt_int_enable(BT1, BT_INTSRC_CMP);				//若需使用中断，请调该接口使能对应中断，这里使用CMP中断
	csi_bt_register_callback(BT1, user_bt1_callback);	//注册中断回调函数
	csi_bt_start(BT1);									//启动BT1
	
	while(1)
	{
		nop;
	}
	
	return iRet;
}

#endif