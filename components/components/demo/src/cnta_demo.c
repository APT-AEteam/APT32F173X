/***********************************************************************//** 
 * \file  cnta_demo.c
 * \brief  counterA 示例代码

 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version    <th>Author  <th>Description
 * <tr><td> 2020-09-21 <td>V0.0  <td>ZJY     <td>initial
 * <tr><td> 2020-05-27 <td>V0.1  <td>LQ      <td>modify
 * </table>
 * *********************************************************************
*/

/* include ----------------------------------------------------------------*/
#include "cnta.h"
#include "gpio.h"
#include "pin.h"
#include "board_config.h"
#include "demo.h"
#include <csi_drv.h>

/* externs function--------------------------------------------------------*/
/* private function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/
/* device instance ------------------------------------------------------*/


/**
  \brief       CounterA 定时中断示例
  \return      int
*/
int cnta_timer_demo(void)
{	
	int iRet = 0;
	csi_conta_timer_config_t tTimerCfg;
	
	csi_pin_set_mux(PA10, PA10_OUTPUT);         //配置输出
	csi_pin_output_mode(PA10,GPIO_PUSH_PULL);   //推挽
	csi_pin_set_high(PA10);					    //初始为高，在cnta的中断中翻转
	
	tTimerCfg.eClkDiv = CNTA_CK_DIV8;
	tTimerCfg.wTime = 1000;                     //1000us,if cnta clk is 3M,the timeout for timer((0.333us * 1) ->(0.333us * 65535): 0.3333us -> 21.845ms)
	tTimerCfg.eRunMode = CNTA_REPEAT_MODE;
	tTimerCfg.byInter = CNTA_PENDL_INT;
	csi_cnta_timer_init(CA0,&tTimerCfg);        //初始化CountA
	csi_cnta_start(CA0);  //启动CountA
	while(1)
	{
		nop;
	}
	return iRet;
}

/**
  \brief       CounterA pwm示例
  \return      int
*/
int cnta_pwm_demo(void)
{		
	int iRet = 0;
	csi_conta_pwm_config_t tPwmCfg;
	
	tPwmCfg.eClkDiv = CNTA_CK_DIV8;		//时钟8分频
	tPwmCfg.byStartLevel = POLAR_LOW;	//开始极性低
	tPwmCfg.byStopLevel = STOP_LOW;     //结束极性低
	tPwmCfg.byDutyCycle = 33;           //占空比
	tPwmCfg.wFreq = 38000;              //频率(hz)
	tPwmCfg.byInter = CNTA_NONE_INT;    //无中断源
	
	//cnta作为pwm输出口
	csi_pin_set_mux(PA10,PA10_CNTA_BUZ);//set counter output pin	
	//csi_pin_set_mux(PD3,PD3_CNTA_BUZ);//set counter output pin	
	//csi_pin_set_mux(PD5,PD5_CNTA_BUZ);//set counter output pin
	
	csi_cnta_pwm_init(CA0,&tPwmCfg);    //初始化CountA  
	csi_cnta_start(CA0);  //启动CountA
	while(1)
	{
		nop;
	}
	return iRet;
}

/** \brief CounterA 和BT0搭配包络输出PWM示例
 *  \param[in] none
 *  \return error code
 */
int cnta_envelope_demo(void)
{
	int iRet = 0;
	csi_conta_pwm_config_t tPwmCfg;
	csi_bt_pwm_config_t tBTPwmCfg;							//BT PWM输出参数初始化配置结构体
	
	//BT0 初始化
	csi_pin_set_mux(PA0,  PA0_BT0_OUT);					    //PA0  作为BT0 PWM输出引脚
	//init timer pwm para config
	tBTPwmCfg.byIdleLevel = BT_PWM_IDLE_HIGH;				//PWM 输出空闲电平
	tBTPwmCfg.byStartLevel= BT_PWM_START_HIGH;				//PWM 输出起始电平
	tBTPwmCfg.byDutyCycle = 50;								//PWM 输出占空比(0 < DutyCycle < 100)		
	tBTPwmCfg.wFreq 	  = 100;							//PWM 输出频率
//	tPwmCfg.byInter 	= BT_INTSRC_PEND | BT_INTSRC_CMP;	//PWM 中断配置(PEND and CMP)
	tBTPwmCfg.byInter		= BT_INTSRC_NONE;
	
	csi_bt_pwm_init(BT0, &tBTPwmCfg);						//初始化BT0 PWM输出
	csi_bt_start(BT0);										//启动BT0	
	
	//CountA 初始化
	tPwmCfg.eClkDiv      = CNTA_CK_DIV1;   //时钟8分频
	tPwmCfg.byStartLevel = POLAR_LOW;	   //开始极性低
	tPwmCfg.byStopLevel  = STOP_LOW;       //结束极性低
	tPwmCfg.byDutyCycle  = 60;             //占空比
	tPwmCfg.wFreq        = 380000;         //频率(hz)
	tPwmCfg.byInter      = CNTA_NONE_INT;  //无中断源
	
	csi_pin_set_mux(PA10,PA10_CNTA_BUZ);//set counter output pin	
	//csi_pin_set_mux(PD3,PD3_CNTA_BUZ);//set counter output pin	
	//csi_pin_set_mux(PD5,PD5_CNTA_BUZ);//set counter output pin
	csi_cnta_pwm_init(CA0,&tPwmCfg);
	
	csp_cnta_set_carrier(CA0, CNTA_CARRIER_EN, PWM_CARRIER, STOP_LOW, POLAR_LOW);  //载波输出	
//	csp_cnta_set_carrier(CA0, CNTA_CARRIER_EN, PWM_ENVELOPE, STOP_LOW, POLAR_LOW); //包络输出

//	csp_cnta_set_sync(CA0, PEND_CARRIERON_CLR, MATCH_CARRIERON_SET,CNTA_HW_TCPEND);//BT脉冲匹配中断发生时，CARRIERON位会被硬件自动置位
	csp_cnta_set_sync(CA0, PEND_CARRIERON_SET, MATCH_CARRIERON_CLR,CNTA_HW_TCPEND);//BT周期结束中断发生时，CARRIERON位会被硬件自动置位
//	csp_cnta_set_sync(CA0, PEND_CARRIERON_DIS, MATCH_CARRIERON_DIS,CNTA_HW_TCPEND);//禁止CARRIERON的硬件自动触发
	
	csi_cnta_start(CA0);  //启动CountA
	while(1)
	{
		nop;
	}
	return iRet;
}