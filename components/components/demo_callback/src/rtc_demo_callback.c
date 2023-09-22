/***********************************************************************//** 
 * \file  rtc_demo.c
 * \brief  rtc demo code
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-27 <td>V2.0 <td>WNN    <td>initial
 * </table>
 * *********************************************************************
*/

/* include ----------------------------------------------------------------*/
#include "csi_drv.h"
#include "iostring.h"
#include "board_config.h"


#if (USE_RTC_CALLBACK == 1)		
/* externs function--------------------------------------------------------*/
/* private function--------------------------------------------------------*/
/* externs variable------------------------------------------------------*/
/* Private variable------------------------------------------------------*/

csi_rtc_alm_t  tAlmA;

/** \brief  user_rtc_callback：RTC中断回调函数
 * 
 * 	\brief	用户定义，支持指ALMA/ALAMB/CPRD/TRGEV0/TRGEV1三种中断处理，使用csi标准库，中断发生时会自动调用用户注册的回调函
 * 			数，用户可在回调函数里做自己的处理，而不需要关注具体的底层中断处理。
 * 
 *  \param[in]  ptRtcBase: 	RTC寄存器结构体指针，指向RTC的基地址 
 *  \param[out] byIsr: 		RTC中断状态
 *  \return none
 */ 
void user_rtc_callback(csp_rtc_t *ptRtcBase, uint8_t byIsr)
{
	if(byIsr & RTCINT_ALMA)
	{
		my_printf("RTC ALARM is triggered!");
		tAlmA.byAlmSt   = 1; 
	}
}



/** \brief	rtc_alarm_callback_demo：RTC设置闹钟A的demo，使用callback

 * 	\brief  csi接口默认使用(开启)ALAM中断，并在中断里面打印信息，若不需要开启中断，可调
 * 			用csi_bt_int_disable接口函数，关闭ALAM中断
 *  
 * 
 *  \param[in] none
 *  \return 0/-1
 */
void rtc_alarm_callback_demo(void)
{
	csi_rtc_time_t tRtcTime, tAlmTime;
	csi_rtc_config_t tRtcConfig;

	
	//RTC初始化
	tRtcConfig.eClkSrc = RTC_CLKSRC_IMOSC_DIV4;  		//选择时钟源：IMOSC
	tRtcConfig.eFmt = RTC_MODE_24FMT;				  	//选择时间模式
	csi_rtc_init(RTC, &tRtcConfig);				  		//初始化RTC
	
	//RTC时间设置
	tRtcTime.iYear = 13;
	tRtcTime.iMon  = 11;
	tRtcTime.iMday = 25;
	tRtcTime.iHour = 15;
	tRtcTime.iMin  = 15;
	tRtcTime.iSec  = 50;	
	csi_rtc_set_time(RTC, &tRtcTime);					//设置时间
	csi_rtc_register_callback(RTC, user_rtc_callback);	//注册中断回调函数
	csi_rtc_start(RTC);	
	
	//RTC闹钟时间设置
	tAlmA.byAlmMode = 2;								//小时，分钟，秒模式
	tAlmA.byAlmSt   = 0;								//清除alarm时间到标志位
	tAlmTime.iMday  = 25;
	tAlmTime.iHour  = 15;
	tAlmTime.iMin   = 16;
	tAlmTime.iSec   = 0xff;								//不要比较sec（0xFF意味着不要比较）
	csi_rtc_set_alarm(RTC, RTC_ALMA, tAlmA.byAlmMode, &tAlmTime);	//设置闹钟A	
	
	while(tAlmA.byAlmSt ==0);							//等alarm时间到标志到
	csi_rtc_cancel_alarm(RTC, RTC_ALMA);				//取消闹钟
	
	while(1)
	{
		nop;
	}							
}


#endif