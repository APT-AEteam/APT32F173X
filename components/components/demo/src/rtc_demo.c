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
#include "rtc.h"
#include "pin.h"
#include "iostring.h"

/* externs function--------------------------------------------------------*/
/* private function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/
static uint8_t s_hwRtcMsg = 0;
csi_rtc_alm_t  tAlmA;
csi_rtc_alm_t  tAlmB;
/** \brief 设置rtc时间的示例代码：包括时钟源、时间模式、时间设置，当前时间回读函数
 * 
 *  \param[in] none
 *  \return    none
 */
void rtc_set_time_demo(void)
{	
	csi_rtc_config_t tRtcConfig;
	csi_rtc_time_t tRtcTime,tRtcTimeRdbk;
	uint32_t wSec;
	
	wSec = 0xff;

/**********    RTC_CLKSRC_EMOSC_DIV4     ****************/	
//	csi_pin_set_mux(PD0, PD0_XIN);			//设置PD0为SXIN，如果使用外部主晶振作为RTC时钟源，必须先对管脚进行配置
//	csi_pin_set_mux(PD1, PD1_XOUT);         //设置PD1为SXOUT，如果使用外部主晶振作为RTC时钟源，必须先对管脚进行配置
//	
//	tRtcConfig.byClkSrc = RTC_CLKSRC_EMOSC_DIV4;		//选择时钟源
//	tRtcConfig.byFmt = RTC_12FMT;				//选择时间模式
//	csi_rtc_init(RTC, &tRtcConfig);				//初始化设置
/***********************************************************/	


/**********    RTC_CLKSRC_ESOSC     ****************/		
//	csi_pin_set_mux(PC14, PC14_SXIN);        //设置PC14为XIN，如果使用外部副晶振作为RTC时钟源，必须先对管脚进行配置
//	csi_pin_set_mux(PC15, PC15_SXOUT);        //设置PC15为XOUT，如果使用外部副晶振作为RTC时钟源，必须先对管脚进行配置

//	tRtcConfig.byClkSrc = RTC_CLKSRC_ESOSC;		//选择时钟源
//	tRtcConfig.byFmt = RTC_12FMT;				//选择时间模式
//	csi_rtc_init(RTC, &tRtcConfig);				//初始化设置
/***********************************************************/	

/**********    RTC_CLKSRC_IMOSC_DIV4 /RTC_CLKSRC_ISOSC    ****************/	
	tRtcConfig.byClkSrc = RTC_CLKSRC_IMOSC_DIV4;  //选择时钟源
	tRtcConfig.byFmt = RTC_24FMT;				  //选择时间模式
	csi_rtc_init(RTC, &tRtcConfig);				  //初始化RTC
/***********************************************************/	
	
	tRtcTime.iYear = 13;
	tRtcTime.iMon = 11;
	tRtcTime.iMday = 25;
	tRtcTime.iHour = 15;						//如果是12小时制，回读到的hour范围为1->12；如果是24小时制，回读到的hour范围是0->23
	tRtcTime.iMin = 15;
	tRtcTime.iSec = 0;	
	csi_rtc_set_time(RTC, &tRtcTime);			//设置时间
	csi_rtc_start(RTC);							//RTC开始计时
	
	csi_rtc_change_fmt(RTC, RTC_12FMT);			//修改时间模式为12小时制
	tRtcTime.iYear = 150;
	tRtcTime.iMon = 12;
	tRtcTime.iMday = 26;
	tRtcTime.iPm = 0;							//4s后，pm=0；24小时制无需关注此位
	tRtcTime.iHour = 9;
	tRtcTime.iMin = 59;
	tRtcTime.iSec = 56;	
	csi_rtc_set_time(RTC, &tRtcTime);			//修改时间以后需要重新start rtc
	csi_rtc_start(RTC);
	
	csi_rtc_get_time(RTC,  &tRtcTimeRdbk);		//回读当前时间
	my_printf("12FMT: %d:%d:%d\n", tRtcTimeRdbk.iHour, tRtcTimeRdbk.iMin, tRtcTimeRdbk.iSec);
	
	csi_rtc_change_fmt(RTC, RTC_24FMT);			//修改时间模式为24小时制
	csi_rtc_get_time(RTC,  &tRtcTimeRdbk);		//回读当前时间
	my_printf("24FMT: %d:%d:%d\n", tRtcTimeRdbk.iHour, tRtcTimeRdbk.iMin, tRtcTimeRdbk.iSec);
	
	csi_rtc_change_fmt(RTC, RTC_12FMT);			//修改时间模式为12小时制
	
	while(1){
		csi_rtc_get_time(RTC,  &tRtcTimeRdbk);
		
		if((wSec != tRtcTimeRdbk.iSec) ) {
			wSec = tRtcTimeRdbk.iSec;
			my_printf("%d:%d:%d pm= %d\n", tRtcTimeRdbk.iHour, tRtcTimeRdbk.iMin, tRtcTimeRdbk.iSec, tRtcTimeRdbk.iPm);
			
		}
	}
}

/** \brief 设置rtc 闹钟的示例代码：包括闹表模式、闹钟时间设置，当前时间回读和获取距离闹表时间到还有多久
 * 
 *  \param[in] none
 *  \return    none
 */

void rtc_alarm_demo(void)	
{	
	uint32_t wTemp0;
	uint32_t wSec = 0;
	csi_rtc_time_t tRtcTime, tAlmTime, tRtcTimeRdbk;
	csi_rtc_config_t tRtcConfig;
/**********    RTC_CLKSRC_EMOSC_DIV4     ****************/	
//	csi_pin_set_mux(PD0, PD0_XIN);			//设置PD0为SXIN，如果使用外部主晶振作为RTC时钟源，必须先对管脚进行配置
//	csi_pin_set_mux(PD1, PD1_XOUT);         //设置PD1为SXOUT，如果使用外部主晶振作为RTC时钟源，必须先对管脚进行配置
//	
//	tRtcConfig.byClkSrc = RTC_CLKSRC_EMOSC_DIV4;		//选择时钟源
//	tRtcConfig.byFmt = RTC_12FMT;				//选择时间模式
//	csi_rtc_init(RTC, &tRtcConfig);				//初始化设置
/***********************************************************/	


/**********    RTC_CLKSRC_ESOSC     ****************/		
//	csi_pin_set_mux(PC14, PC14_SXIN);        //设置PC14为XIN，如果使用外部副晶振作为RTC时钟源，必须先对管脚进行配置
//	csi_pin_set_mux(PC15, PC15_SXOUT);        //设置PC15为XOUT，如果使用外部副晶振作为RTC时钟源，必须先对管脚进行配置

//	tRtcConfig.byClkSrc = RTC_CLKSRC_ESOSC;		//选择时钟源
//	tRtcConfig.byFmt = RTC_12FMT;				//选择时间模式
//	csi_rtc_init(RTC, &tRtcConfig);				//初始化设置
/***********************************************************/	

/**********    RTC_CLKSRC_IMOSC_DIV4 /RTC_CLKSRC_ISOSC    ****************/	
	tRtcConfig.byClkSrc = RTC_CLKSRC_IMOSC_DIV4;  //选择时钟源
	tRtcConfig.byFmt = RTC_24FMT;				  //选择时间模式
	csi_rtc_init(RTC, &tRtcConfig);				  //初始化RTC
/***********************************************************/	
	tRtcTime.iYear = 13;
	tRtcTime.iMon = 11;
	tRtcTime.iMday = 25;
	tRtcTime.iHour = 15;
	tRtcTime.iMin = 15;
	tRtcTime.iSec = 50;	
	csi_rtc_set_time(RTC, &tRtcTime);				//设置时间
	csi_rtc_start(RTC);								//RTC开始工作
	
	tAlmA.byAlmMode = 2;							//日，小时，分钟，秒模式
	tAlmA.byAlmSt = 0;								//清除alarm时间到标志位
	tAlmTime.iMday = 25;
	tAlmTime.iHour = 15;
	tAlmTime.iMin = 16;
	tAlmTime.iSec = 0xff;							//不要比较sec（0xFF意味着不要比较）
	csi_rtc_set_alarm(RTC, RTC_ALMA, tAlmA.byAlmMode, &tAlmTime);	//设置闹钟A	
	while(tAlmA.byAlmSt == 0){										//如果闹钟时间没有到，每秒打印一次当前时间和距离闹钟的时间
		csi_rtc_get_time(RTC,  &tRtcTimeRdbk);
		wTemp0 = csi_rtc_get_alarm_remaining_time(RTC, RTC_ALMA);

		if( wSec!= tRtcTimeRdbk.iSec) {
			wSec = tRtcTimeRdbk.iSec;
			my_printf("%d:%d:%d - %d\n", tRtcTimeRdbk.iHour, tRtcTimeRdbk.iMin, tRtcTimeRdbk.iSec, wTemp0);
			
		}
	}
	my_printf("RTC Alarm happens!\n");				//一旦alarm时间到，AlarmA中断处理函数中会置位tAlmA.byAlmSt
	csi_rtc_cancel_alarm(RTC, RTC_ALMA);			//取消闹钟，保持原闹钟时间
	tAlmA.byAlmSt = 0;								//清alarm时间到标志位			

}

/** \brief 示例如何将RTC当做一个简单timer来使用
 * 
 *  \param[in] none
 *  \return    none
 */
void rtc_timer_demo(void)
{
	csi_rtc_config_t tRtcConfig;
	
	/**********    RTC_CLKSRC_EMOSC_DIV4     ****************/	
//	csi_pin_set_mux(PD0, PD0_XIN);			//设置PD0为SXIN，如果使用外部主晶振作为RTC时钟源，必须先对管脚进行配置
//	csi_pin_set_mux(PD1, PD1_XOUT);         //设置PD1为SXOUT，如果使用外部主晶振作为RTC时钟源，必须先对管脚进行配置
//	
//	tRtcConfig.byClkSrc = RTC_CLKSRC_EMOSC_DIV4;		//选择时钟源
//	tRtcConfig.byFmt = RTC_12FMT;				//选择时间模式
//	csi_rtc_init(RTC, &tRtcConfig);				//初始化设置
/***********************************************************/	


/**********    RTC_CLKSRC_ESOSC     ****************/		
//	csi_pin_set_mux(PC14, PC14_SXIN);        //设置PC14为XIN，如果使用外部副晶振作为RTC时钟源，必须先对管脚进行配置
//	csi_pin_set_mux(PC15, PC15_SXOUT);        //设置PC15为XOUT，如果使用外部副晶振作为RTC时钟源，必须先对管脚进行配置

//	tRtcConfig.byClkSrc = RTC_CLKSRC_ESOSC;		//选择时钟源
//	tRtcConfig.byFmt = RTC_12FMT;				//选择时间模式
//	csi_rtc_init(RTC, &tRtcConfig);				//初始化设置
/***********************************************************/	

/**********    RTC_CLKSRC_IMOSC_DIV4 /RTC_CLKSRC_ISOSC    ****************/	
	tRtcConfig.byClkSrc = RTC_CLKSRC_IMOSC_DIV4;  //选择时钟源
	tRtcConfig.byFmt = RTC_24FMT;				  //选择时间模式
	csi_rtc_init(RTC, &tRtcConfig);				  //初始化RTC
/***********************************************************/	
	
	csi_rtc_start_as_timer(RTC, RTC_TIMER_1MIN);	  	//每1s进一次中断
	csi_rtc_start(RTC);	
	
	while(1)
	{
		//my_printf("%x\n", (uint32_t)RTC->SSR);
	}							//RTC开始工作
}


/** \brief 示例RTC通过ETCB触发BT开始工作
 * 这个示例并不完整，如果要实现RTC定时触发功能，需要ETCB和其他被触发模块合作
 * 
 *  \param[in] none
 *  \return    none
 */
 void rtc_trgev_demo(void)	
 {
	csi_rtc_config_t tRtcConfig;
	
	tRtcConfig.byClkSrc = RTC_CLKSRC_IMOSC_DIV4;  	//选择时钟源
	tRtcConfig.byFmt = RTC_24FMT;				  	//选择时间模式
	csi_rtc_init(RTC, &tRtcConfig);				  	//初始化RTC
	
	csi_rtc_start_as_timer(RTC, RTC_TIMER_1S);	  	//每1s进一次中断
	csi_rtc_int_enable(RTC, RTC_INT_CPRD , DISABLE);//不需要中断的话，可以关掉
	csi_rtc_start(RTC);								//RTC开始工作
	
	csi_rtc_set_evtrg(RTC, 0, RTC_TRGOUT_CPRD, 2);  //RTC TRGEV0 每两秒钟输出一次trigger event
	while(1);
	
 }
 
 /** \brief rtc interrupt handle function
 * 
 *  \param[in][in] ptAdcBase: pointer of rtc register structure
 *  \return none
 */ 
__attribute__((weak)) void rtc_irqhandler(csp_rtc_t *ptRtcBase)
{
	if(((csp_rtc_get_isr(ptRtcBase) & RTC_INT_ALMA))==RTC_INT_ALMA)
	{
		s_hwRtcMsg |= RTC_INT_ALMA;
		csp_rtc_clr_isr(ptRtcBase,RTC_INT_ALMA);	
	}	
	if(((csp_rtc_get_isr(ptRtcBase) & RTC_INT_ALMB))==RTC_INT_ALMB)
	{
		s_hwRtcMsg |= RTC_INT_ALMB;
		csp_rtc_clr_isr(ptRtcBase,RTC_INT_ALMB);	
	}	
	if(((csp_rtc_get_isr(ptRtcBase) & RTC_INT_CPRD))==RTC_INT_CPRD)
	{
		s_hwRtcMsg |= RTC_INT_CPRD;
		csp_rtc_clr_isr(ptRtcBase,RTC_INT_CPRD);	
	}	
	if(((csp_rtc_get_isr(ptRtcBase) & RTC_INT_TRGEV0))==RTC_INT_TRGEV0)
	{
		s_hwRtcMsg |= RTC_INT_TRGEV0;
		csp_rtc_clr_isr(ptRtcBase,RTC_INT_TRGEV0);	
	}	
	if(((csp_rtc_get_isr(ptRtcBase) & RTC_INT_TRGEV1))==RTC_INT_TRGEV1)
	{
		s_hwRtcMsg |= RTC_INT_TRGEV1;
		csp_rtc_clr_isr(ptRtcBase,RTC_INT_TRGEV1);	
	}	
}