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
/* externs function--------------------------------------------------------*/
/* private function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/
/* device instance ------------------------------------------------------*/


/**
  \brief       counterA 定时中断示例
  \return      int
*/
int cnta_timer_demo(void)
{	
	int iRet = 0;
	csi_conta_timer_config_t tTimerCfg;
	
	csi_pin_set_mux(PA10, PA10_OUTPUT);         //配置输出
	csi_pin_output_mode(PA10,GPIO_PUSH_PULL);   //推挽
	csi_pin_set_high(PA10);					    //初始设置为高，在cnta的中断中翻转
	
	tTimerCfg.eClkDiv = CNTA_CK_DIV8;
	tTimerCfg.wTime = 1000;                     //1000us,if cnta clk is 3M,the timeout for timer((0.333us * 1) ->(0.333us * 65535): 0.3333us -> 21.845ms)
	tTimerCfg.eRunMode = CNTA_REPEAT_MODE;
	tTimerCfg.byInter = CNTA_PENDL_INT;
	
	csi_cnta_timer_init(CA0,&tTimerCfg);
	csi_cnta_start(CA0);
	while(1)
	{
		nop;
	}
	return iRet;
}

/**
  \brief       counterA pwm示例
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
	
	csi_cnta_pwm_init(CA0,&tPwmCfg);
	//csi_cnta_bt0_sync(CA0, PEND_CARRIERON_SET, MATCH_CARRIERON_CLR,CNTA_HW_DIS);//结合bt0设置硬件自动打开或者关闭载波
	csi_cnta_start(CA0);
	while(1)
	{
		nop;
	}
	return iRet;
}