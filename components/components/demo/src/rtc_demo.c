/***********************************************************************//** 
 * \file  rtc_demo.c
 * \brief  rtc demo code
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-27 <td>V0.0 <td>WNN    <td>initial
 * <tr><td> 2023-9-22 <td>V0.1  <td>GQQ   <td>fix bug,code normalization
 * </table>
 * *********************************************************************
*/

/* include ----------------------------------------------------------------*/
#include "csi_drv.h"
#include "iostring.h"
#include "board_config.h"

/* externs function--------------------------------------------------------*/
/* private function--------------------------------------------------------*/
/* externs variable------------------------------------------------------*/
/* Private variable------------------------------------------------------*/
csi_rtc_alm_t  tAlmA;
csi_rtc_alm_t  tAlmB;

#if (USE_RTC_CALLBACK == 0)	
 /** \brief rtc interrupt handle function
 * 
 *  \param[in][in] ptAdcBase: pointer of rtc register structure
 *  \return none
 */ 
ATTRIBUTE_ISR void rtc_int_handler(void)
{
	if(((csp_rtc_get_isr(RTC) & RTC_INT_ALMA))==RTC_INT_ALMA)
	{
		tAlmA.byAlmSt = 1;
		csp_rtc_clr_isr(RTC,RTC_INT_ALMA);	
	}	
	if(((csp_rtc_get_isr(RTC) & RTC_INT_ALMB))==RTC_INT_ALMB)
	{
		csp_rtc_clr_isr(RTC,RTC_INT_ALMB);	
	}	
	if(((csp_rtc_get_isr(RTC) & RTC_INT_CPRD))==RTC_INT_CPRD)
	{
		csp_rtc_clr_isr(RTC,RTC_INT_CPRD);	
	}	
	if(((csp_rtc_get_isr(RTC) & RTC_INT_TRGEV0))==RTC_INT_TRGEV0)
	{
		csp_rtc_clr_isr(RTC,RTC_INT_TRGEV0);	
	}	
	if(((csp_rtc_get_isr(RTC) & RTC_INT_TRGEV1))==RTC_INT_TRGEV1)
	{
		csp_rtc_clr_isr(RTC,RTC_INT_TRGEV1);	
	}	
}
#endif


/** \brief rtc_set_time_demo:该例程演示了如何设置RTC时间，并回读时间
 * 			-注意：因为RTC的时钟源频率远低于系统时钟，故设置完时间，需要延时一段时间，再回读。
 * 			
 * 		eClkSrc 通过配置该参数可以选择四种不同的时钟源，具体可参见csi_rtc_clksrc_e
 * 		本例程使用RTC_CLKSRC_ESOSC，即外部副时钟（32.768K）作为RTC的时钟源
 * 			
 *  \param[in] none
 *  \return    none
 */
void rtc_set_time_demo(void)
{	
	csi_rtc_config_t tRtcConfig;
	csi_rtc_time_t tRtcTime,tRtcTimeRdbk;
	uint32_t wSec = 0xff;
	
	
#if (USE_GUI==0)
	csi_gpio_set_mux(SXIN_PORT,SXIN_PIN, SXIN_PIN_FUNC);        //如果使用外部副晶振作为RTC时钟源，必须先对管脚进行配置
	csi_gpio_set_mux(SXOUT_PORT,SXOUT_PIN, SXOUT_PIN_FUNC);     //如果使用外部副晶振作为RTC时钟源，必须先对管脚进行配置	
#endif

	tRtcConfig.eClkSrc = RTC_CLKSRC_ESOSC;						//选择时钟源:外部副晶振 32.768K
	tRtcConfig.eFmt = RTC_MODE_24FMT;							//选择时间模式
	csi_rtc_init(RTC, &tRtcConfig);								//初始化设置
	
	//设置时间
	tRtcTime.iYear = 13;										//年份 = iYear + 2000,年份范围[2000,2099]
	tRtcTime.iMon  = 11;										//月
	tRtcTime.iMday = 25;										//日
	tRtcTime.iHour = 15;										//如果是12小时制，回读到的hour范围为1->12；如果是24小时制，回读到的hour范围是0->23
	tRtcTime.iMin  = 15;
	tRtcTime.iSec  = 0;	
	csi_rtc_set_current_time(RTC, &tRtcTime);					//设置时间
	csi_rtc_start(RTC);											//RTC开始计时
	
	mdelay(1000);												//因为RTC时钟源频率远低于系统时钟频率，故设置完时间，需要延时一段时间，进行回读，才能保证读到的时间是需要的时间
	
	
	csi_rtc_get_time(RTC,  &tRtcTimeRdbk);						//回读当前时间，并打印
	my_printf("24FMT: %d:%d:%d\n", tRtcTimeRdbk.iHour, tRtcTimeRdbk.iMin, tRtcTimeRdbk.iSec);

	while(1){
		csi_rtc_get_time(RTC,  &tRtcTimeRdbk);
		
		if((wSec != tRtcTimeRdbk.iSec) ) {						//秒发生变化时打印时间，即一秒打印一次时间
			wSec = tRtcTimeRdbk.iSec;
			my_printf("%d:%d:%d pm= %d\n", tRtcTimeRdbk.iHour, tRtcTimeRdbk.iMin, tRtcTimeRdbk.iSec, tRtcTimeRdbk.iPm);
		}
	}
}


/** \brief rtc_alarm_demo:设置rtc 闹钟的示例代码, 并打印当前时间回读和获取距离闹表时间到还有多久
 * 			
 * 			-tAlmA.byAlmSt ：闹钟状态，当闹钟设定时间到时，会在中断中进行置位处理。否则会一直打印当前时间
 * 
 * 			-tAlmA.byAlmMode ：可以写0/1/2，表示闹钟触发关注的时间
 * 				-0 : 日   时 分 秒
 * 				-1 : 星期 时 分 秒
 * 				-2 :      时 分 秒
 * 			另外，如果不需要关注的量，例如 秒 可以配置tAlmTime.iSec = 0xff
 * 			
 * 		eClkSrc 通过配置该参数可以选择四种不同的时钟源，具体可参见csi_rtc_clksrc_e
 * 		本例程使用RTC_CLKSRC_IMOSC_DIV4，即当前使能的IM时钟
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
	csi_rtc_set_current_time(RTC, &tRtcTime);					//设置时间
	csi_rtc_start(RTC);									//RTC开始工作
	
	//RTC闹钟时间设置
	tAlmA.byAlmMode = 2;								//小时，分钟，秒模式
	tAlmA.byAlmSt   = 0;								//清除alarm时间到标志位
	tAlmTime.iMday  = 25;
	tAlmTime.iHour  = 15;
	tAlmTime.iMin   = 16;
	tAlmTime.iSec   = 0xff;								//不要比较sec（0xFF意味着不要比较）
	csi_rtc_set_alarm(RTC, RTC_ALMA, tAlmA.byAlmMode, &tAlmTime);	//设置闹钟A	
	
	while(tAlmA.byAlmSt == 0){										//如果闹钟时间没有到，每秒打印一次当前时间和距离闹钟的时间
		csi_rtc_get_time(RTC,  &tRtcTimeRdbk);
		wTemp0 = csi_rtc_get_alarm_remaining_time(RTC, RTC_ALMA);

		if( wSec!= tRtcTimeRdbk.iSec) {
			wSec = tRtcTimeRdbk.iSec;
			my_printf("\n %d:%d:%d - %d\n", tRtcTimeRdbk.iHour, tRtcTimeRdbk.iMin, tRtcTimeRdbk.iSec,wTemp0);
			
		}
	}
	my_printf("RTC Alarm happens!\n");				//一旦alarm时间到，AlarmA中断处理函数中会置位tAlmA.byAlmSt
	csi_rtc_cancel_alarm(RTC, RTC_ALMA);			//取消闹钟，保持原闹钟时间
	tAlmA.byAlmSt = 0;								//清alarm时间到标志位	

	while(1);

}



/** \brief rtc_timer_demo:配置RTC作为一个简单的定时器使用，注意，此时的中断只能是CPRD
 * 			
 * 		eClkSrc 通过配置该参数可以选择四种不同的时钟源，具体可参见csi_rtc_clksrc_e
 * 		本例程使用RTC_CLKSRC_EMOSC，即外部主时钟（24M）作为RTC的时钟源
 * 			
 *  \param[in] none
 *  \return    none
 */
void rtc_timer_demo(void)
{
	csi_rtc_config_t tRtcConfig;
	
#if (USE_GUI ==0)	
	csi_gpio_set_mux(XIN_PORT, XIN_PIN, XIN_PIN_FUNC);				//如果使用外部主晶振作为RTC时钟源，必须先对管脚进行配置
	csi_gpio_set_mux(XOUT_PORT, XOUT_PIN, XOUT_PIN_FUNC);        	//如果使用外部主晶振作为RTC时钟源，必须先对管脚进行配置
#endif
	
	tRtcConfig.eClkSrc = RTC_CLKSRC_EMOSC_DIV4;						//选择时钟源
	tRtcConfig.eFmt = RTC_MODE_24FMT;								//选择时间模式: 24小时制
	
	csi_rtc_init(RTC, &tRtcConfig);									//初始化设置

	csi_rtc_set_timer(RTC, RTC_TIMER_1S);	 				 		//每1s进一次中断
	csi_rtc_start(RTC);	
	
	while(1)
	{
		nop;  
	}						
}

/** \brief	rtc_etcb_bt_pwm_demo: rtc 通过ETCB 触发 BT0 输出PWM
 *  \brief		-rtc时间到达ALARMB设定的时间时，会通过ETCB 触发BT0输出PWM。并每秒打印一次时间
 * 
* 		eClkSrc 通过配置该参数可以选择四种不同的时钟源，具体可参见csi_rtc_clksrc_e
 * 		本例程使用RTC_CLKSRC_ISOSC，即内部IS时钟（27K）作为RTC的时钟源 		
 *  \param[in] none
 *  \return error code
 */
void rtc_etcb_bt_pwm_demo(void)	
 {
	csi_bt_pwm_config_t tPwmCfg;
	
	csi_etcb_config_t tEtbConfig1;
	csi_rtc_config_t tRtcConfig;
	csi_rtc_time_t tRtcTime,tRtcTimeRdbk,tAlmTime;
	
	volatile uint8_t ch;
	uint32_t wSec = 0;
	
	//RTC初始化配置
	tRtcConfig.eClkSrc = RTC_CLKSRC_ISOSC;		//选择时钟源 
	tRtcConfig.eFmt = RTC_MODE_24FMT;			//选择时间模式
	csi_rtc_init(RTC, &tRtcConfig);				//初始化设置
	
	//RTC 当前时间设置
	tRtcTime.iYear = 22;
	tRtcTime.iMon  = 2;
	tRtcTime.iMday = 14;
	tRtcTime.iHour = 9;						  	//如果是12小时制，回读到的hour范围为1->12；如果是24小时制，回读到的hour范围是0->23
	tRtcTime.iMin = 59;
	tRtcTime.iSec = 55;
	tRtcTime.iPm  = RTC_AM;	
	csi_rtc_set_current_time(RTC, &tRtcTime);			//设置时间
	csi_rtc_start(RTC);							//RTC开始计时
	
	
	//触发源 闹钟B配置
	tAlmB.byAlmMode =  2;                		//只关注时，分，秒
	tAlmB.byAlmSt =0;							//时钟状态标志
	tAlmTime.iMday  = 24;
	tAlmTime.iHour  = 10;
	tAlmTime.iMin   = 0;
	tAlmTime.iSec   = 0;								
	csi_rtc_set_alarm(RTC, RTC_ALMB, tAlmA.byAlmMode, &tAlmTime);		//设置闹钟B	
	
	//RTC触发设置	
	csi_rtc_set_evtrg(RTC, RTC_TRGOUT1, RTC_TRGOUT_ALRMB, 0);  			//RTC TRGEV1 闹铃B到时产生trigger event
	csi_rtc_evtrg_enable(RTC, RTC_TRGOUT1);
	
	//ETCB 配置
	tEtbConfig1.eChType = ETCB_ONE_TRG_ONE;  							//单个源触发单个目标
	tEtbConfig1.eSrcIp  = ETCB_RTC_TRGOUT1 ;  	   						//ETCB_RTC_TRGOUT1作为触发源
	tEtbConfig1.eDstIp  = ETCB_BT0_SYNCIN0;   	    					//ETCB_BT0_SYNCIN0 同步输入作为目标事件
	tEtbConfig1.eTrgMode = ETCB_HARDWARE_TRG;       					//硬件触发
	ch = csi_etcb_ch_alloc(tEtbConfig1.eChType);						//自动获取空闲通道号,ch >= 0 获取成功
	csi_etcb_ch_init(ch,&tEtbConfig1);
	
	
	//触发目标事件BT0配置
#if (USE_GUI ==0)	
	csi_gpio_set_mux(GPIOC,PC10, PC10_BT0_OUT);							//PC10 作为BT0 PWM输出引脚
#endif
	
	tPwmCfg.eIdleLevel = BT_PWM_IDLE_HIGH;								//PWM 输出空闲电平
	tPwmCfg.eStartLevel= BT_PWM_START_HIGH;								//PWM 输出起始电平
	tPwmCfg.byDutyCycle = 50;											//PWM 输出占空比(0 < DutyCycle < 100)		
	tPwmCfg.wFreq 		= 1000;											//PWM 输出频率
	csi_bt_pwm_init(BT0, &tPwmCfg);										//初始化BT0 PWM输出
	csi_bt_set_sync(BT0, BT_SYNCIN0, BT_SYNC_ONCE, BT_TRG_AUTOAREARM);	//外部触发bt启动(SYNCIN0)
	csi_bt_sync_enable(BT0, BT_SYNCIN0);
		
	while(1)
	{
		csi_rtc_get_time(RTC,  &tRtcTimeRdbk);
		
		if(wSec != tRtcTimeRdbk.iSec ) 									//每秒打印一次时间
		{
			wSec = tRtcTimeRdbk.iSec;
			my_printf("20%d.%d.%d  %d:%d:%d pm= %d\n",tRtcTimeRdbk.iYear,tRtcTimeRdbk.iMon,tRtcTimeRdbk.iMday, tRtcTimeRdbk.iHour, tRtcTimeRdbk.iMin, tRtcTimeRdbk.iSec, tRtcTimeRdbk.iPm);
		}

	}
 }
 
