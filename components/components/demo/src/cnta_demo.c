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
#include "drv/gpio.h"
#include "drv/cnta.h"
#include "drv/bt.h"
#include "board_config.h"

	
/* externs function--------------------------------------------------------*/
/* private function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/
/* device instance ------------------------------------------------------*/


#if (USE_CNTA_CALLBACK == 0)
/** \brief	cnta_int_handler: cnta中断服务函数
 * 
 *  \brief 	CNTA发生中断时会调用此函数，函数在interrupt.c里定义为弱(weak)属性，默认不做处理；用户用到CNTA中
 * 			断时，请重新定义此函数，在此函数中进行对应中断处理，也可直接在interrupt.c里的函数里进行处理
 * 
 *  \param[in] none
 *  \return none
 */
ATTRIBUTE_ISR  void cnta_int_handler(void)
{
	//用户直接在中断服务接口函数里处理中断，建议客户使用此模式
	
}

#endif

/** \brief       CounterA 定时中断示例
 *  \param[in]   无参数
 *  \return      返回int类型
*/
int cnta_timer_demo(void)
{	
	int iRet = 0;
	csi_cnta_timer_config_t tTimerCfg;

#if (USE_GUI == 0)		
	csi_gpio_set_mux(GPIOA, PA10, PA10_OUTPUT);         //配置输出
	csi_gpio_output_mode(GPIOA, PA10,GPIO_PUSH_PULL);   //推挽
	csi_gpio_set_high(GPIOA, PA10);					    //初始为高，在cnta的中断中翻转
#endif
	
	tTimerCfg.eClkDiv = CNTA_CK_DIV8;                   //时钟8分频
	tTimerCfg.wTime = 1000;                             //周期1000us
	tTimerCfg.eRunMode = CNTA_RUN_CONT;                 //重复模式
	csi_cnta_timer_init(CA0, &tTimerCfg);               //初始化CountA
	
	csi_cnta_start(CA0);                                //启动CountA
	
	while(1)
	{
		nop;
	}
	return iRet;
}

/** \brief       CounterA pwm示例
 *  \param[in]   无参数
 *  \return      返回int类型
 */
int cnta_pwm_demo(void)
{		
	int iRet = 0;
	csi_cnta_pwm_config_t tPwmCfg;
	
#if (USE_GUI == 0)	
	csi_gpio_set_mux(GPIOA, PA10,PA10_CNTA_BUZ);	   //cnta作为pwm输出口
#endif
	
	tPwmCfg.eClkDiv = CNTA_CK_DIV8;		               //时钟8分频
	tPwmCfg.byStartLevel = CNTA_POLAR_LOW;	           //开始极性低
	tPwmCfg.byStopLevel = CNTA_STOP_LOW;               //结束极性低
	tPwmCfg.byDutyCycle = 33;                          //占空比
	tPwmCfg.wFreq = 38000;                             //频率(hz)
	csi_cnta_pwm_init(CA0,&tPwmCfg);                   //初始化CountA 
	
	csi_cnta_start(CA0);                               //启动CountA
	while(1)
	{
		nop;
	}
	return iRet;
}

/** \brief     CounterA 和BT0搭配包络输出PWM示例
 *  \param[in] 无参数
 *  \return    错误码
 */
int cnta_envelope_demo(void)
{
	int iRet = 0;
	csi_cnta_pwm_config_t tPwmCfg;
	csi_bt_pwm_config_t tBTPwmCfg;					   //BT PWM输出参数初始化配置结构体
	
#if (USE_GUI == 0)	
	csi_gpio_set_mux(GPIOA, PA0, PA0_BT0_OUT);		   //PA0  作为BT0 PWM输出引脚
	csi_gpio_set_mux(GPIOA, PA10,PA10_CNTA_BUZ);
#endif

	//BT0 初始化 
	tBTPwmCfg.eIdleLevel = BT_PWM_IDLE_HIGH;		   //PWM 输出空闲电平
	tBTPwmCfg.eStartLevel= BT_PWM_START_HIGH;		   //PWM 输出起始电平
	tBTPwmCfg.byDutyCycle = 50;						   //PWM 输出占空比(0 < DutyCycle < 100)		
	tBTPwmCfg.wFreq 	  = 100;					   //PWM 输出频率
	csi_bt_pwm_init(BT0, &tBTPwmCfg);				   //初始化BT0 PWM输出
	
	csi_bt_start(BT0);								   //启动BT0	
	
	//CountA 初始化
	tPwmCfg.eClkDiv      = CNTA_CK_DIV1;               //时钟8分频
	tPwmCfg.byStartLevel = CNTA_POLAR_LOW;	           //开始极性低
	tPwmCfg.byStopLevel  = CNTA_STOP_LOW;              //结束极性低
	tPwmCfg.byDutyCycle  = 60;                         //占空比
	tPwmCfg.wFreq        = 380000;                     //频率(hz)
	csi_cnta_pwm_init(CA0,&tPwmCfg);

//	csi_cnta_bt0_set_sync(CA0, PEND_CARRIERON_CLR, MATCH_CARRIERON_SET,CNTA_HW_TCPEND);//BT脉冲匹配中断发生时，CARRIERON位会被硬件自动置位
	csi_cnta_bt0_set_sync(CA0, CNTA_PEND_CARRIERON_SET, CNTA_MATCH_CARRIERON_CLR,CNTA_HW_TCPEND);//BT周期结束中断发生时，CARRIERON位会被硬件自动置位
//	csi_cnta_bt0_set_sync(CA0, PEND_CARRIERON_DIS, MATCH_CARRIERON_DIS,CNTA_HW_TCPEND);//禁止CARRIERON的硬件自动触发
	
	csi_cnta_start(CA0);    							//启动CountA
	while(1)
	{
		nop;
	}
	return iRet;
}
