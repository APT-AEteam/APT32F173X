/***********************************************************************//** 
 * \file  csp_rtc.h
 * \brief RTC description and #define functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-25 <td>V0.0  <td>WNN   <td>initial
 *  * <tr><td> 2021-1-8 <td>V0.0  <td>WNN   <td>modify
 * </table>
 * *********************************************************************
*/

#ifndef CSP_RTC_H
#define CSP_RTC_H

#include "csi_core.h"
#include "stdint.h"
#include "stdbool.h"
#include "csp_common.h"


/******************************************************************************
************************* EPWM Structure Definition ***********************
******************************************************************************/
/**
*******************************************************************como.************
@brief EPWM bits Structure
*******************************************************************************
*/

 typedef struct
 {
   __IOM uint32_t 	TIMR;  		//0x0000	Time Control Register
   __IOM uint32_t 	DATR;		//0x0004	Date Control Register
   __IOM uint32_t 	CR;  		//0x0008	Control Register
   __IOM uint32_t	CCR;  		//0x000C	Clock Control register
   __IOM uint32_t	ALRA;		//0x0010	Alarm A
   __IOM uint32_t	ALRB;		//0x0014	Alarm B
   __IOM uint32_t	SSR;    	//0x0018	Sub second
   __IOM uint32_t	CAL;   		//0x001C	Calibration
   __IM uint32_t   	RISR;       //0x0020
   __IOM uint32_t	IMCR;   	//0x0024
   __IM	uint32_t	MISR;     	//0x0028
   __OM	uint32_t	ICR;   		//0x002C
   __IOM uint32_t	KEY;    	//0x0030
   __IOM uint32_t	EVTRG;  	//0x0034
   __IOM uint32_t	EVPS;   	//0x0038
   __OM	uint32_t    EVSWF;  		//0x003C
} csp_rtc_t;

/****** RTC time *****/
typedef struct {
    int sec;             ///< Second.      [0-59]
    int min;             ///< Minute.      [0-59]
    int hour;            ///< Hour.        [0-23]
    int mday;            ///< Day.         [1-31]
    int mon;             ///< Month.       [0-11]
    int year;            ///< Year-1900.   [70- ]      !NOTE:100=2000       
    int wday;            ///< Day of week. [0-6 ]      !NOTE:Sunday = 0     
    int yday;            ///< Days in year.[0-365]     !NOTE:January 1st = 0
} csp_rtc_time_t;

typedef struct {
    int sec;             ///< Second.      [0-59]
    int min;             ///< Minute.      [0-59]
    int hour;            ///< Hour.        [0-23]
    int wdaymsk;         ///< Week Day.         [1-31]

} csp_rtc_almtime_t;
/******************************************************************************
************************** RTC Registers Definition ****************************
******************************************************************************/

/******************************************************************************
* RTC Regiser
******************************************************************************/


///TIMR
#define RTC_SECU_POS	(0ul)
#define RTC_SECU_MSK	(0xf << RTC_SECU_POS)
#define RTC_SEC_MSK		(0x7f << RTC_SECU_POS)
#define RTC_SECT_POS	(4ul)
#define RTC_SECT_MSK	(0x7 << RTC_SECT_POS)
#define RTC_MINU_POS	(8ul)
#define RTC_MINU_MSK	(0xf << RTC_MINU_POS)
#define RTC_MIN_MSK		(0x7f << RTC_MINU_POS)
#define RTC_MINT_POS	(12ul)
#define RTC_MINT_MSK	(0x7 << RTC_MINT_POS)
#define RTC_HORU_POS	(16ul)
#define RTC_HORU_MSK	(0xf << RTC_HORU_POS)
#define RTC_HOR_MSK		(0x3f << RTC_HORU_POS)
#define RTC_HORT_POS	(20ul)
#define RTC_HORT_MSK	(0x3 << RTC_HORT_POS)

#define RTC_SECU(n)    ((n)<<RTC_SECU_POS)
#define RTC_SECT(n)    ((n)<<RTC_SECT_POS)
#define RTC_MINU(n)    ((n)<<RTC_MINU_POS)
#define RTC_MINT(n)    ((n)<<RTC_MINT_POS)
#define RTC_HORU(n)    ((n)<<RTC_HORU_POS)
#define RTC_HORT(n)    ((n)<<RTC_HORT_POS)
#define RTC_PM_POS		(22)
#define RTC_PM_MSK		(0x1 << RTC_PM_POS)
#define RTC_AM	0x0
#define RTC_PM 	0x1

///DATR
#define RTC_DAYU_POS	(0ul)
#define RTC_DAYU_MSK	(0xf << RTC_DAYU_POS)
#define RTC_DAY_MSK		(0x3f << RTC_DAYU_POS)
#define RTC_DAYT_POS	(4ul)
#define RTC_DAYT_MSK	(0x3 << RTC_DAYT_POS)
#define RTC_MONU_POS	(8ul)
#define RTC_MONU_MSK	(0xf << RTC_MONU_POS)
#define RTC_MON_MSK		(0x1f << RTC_MONU_POS)
#define RTC_MONT_POS	(12ul)
#define RTC_MONT_MSK	(0x1 << RTC_MONT_POS)
#define RTC_YEAU_POS	(16ul)
#define RTC_YEAU_MSK	(0xf << RTC_YEAU_POS)
#define RTC_YEA_MSK		(0xff << RTC_YEAU_POS)
#define RTC_YEAT_POS	(20ul)
#define RTC_YEAT_MSK	(0xf << RTC_YEAT_POS)
#define RTC_WKD_POS		(24ul)
#define RTC_WKD_MSK		(0x7 << RTC_WKD_POS)

#define RTC_DAYU(n)    ((n)<<RTC_DAYU_POS)
#define RTC_DAYT(n)    ((n)<<RTC_DAYT_POS)
#define RTC_MONU(n)    ((n)<<RTC_MONU_POS)
#define RTC_MONT(n)    ((n)<<RTC_MONT_POS)
#define RTC_YARU(n)    ((n)<<RTC_YEAU_POS)
#define RTC_YART(n)    ((n)<<RTC_YEAT_POS)
#define RTC_WKD(n)     ((n)<<RTC_WKD_POS)

///CR
#define RTC_INIT            (1ul)
#define RTC_BSY				(0x2ul)
#define RTC_ALMAE           (1ul << 3)
#define RTC_ALMBE           (1ul << 4)

#define RTC_FMT_POS			(5)
#define RTC_FMT_MSK			(0x1ul << RTC_FMT_POS)
typedef enum {
	RTC_24FMT = 0,
	RTC_12FMT
}rtc_fmt_e;

#define RTC_OSEL_POS		(10ul)
#define RTC_OSEL_MSK		(0x7 << RTC_OSEL_POS)


typedef enum {
	RTC_OUT_ALMA_PULSE = 0,
	RTC_OUT_ALMA_HIGH,
	RTC_OUT_ALMA_LOW,
	RTC_OUT_ALMB_PULSE = 4,
	RTC_OUT_ALMB_HIGH,
	RTC_OUT_ALMB_LOW,
	RTC_OUT_NONE,
}rtc_osel_e;

#define RTC_CPRD_POS		(13ul)
#define RTC_CPRD_MSK		(0x7 << RTC_CPRD_POS)
typedef enum {
	RTC_CPRD_DIS = 0,
	RTC_CPRD_05SEC,
	RTC_CPRD_1SEC,
	RTC_CPRD_1MIN,
	RTC_CPRD_1HOR,
	RTC_CPRD_1DAY,
	RTC_CPRD_1MON
}rtc_cprd_e;

#define RTC_RBEN			(0x1 << 16)
#define RTC_TKEYTRG_POS		(17ul)
#define RTC_TKEYTRT_MSK		(0x3 << RTC_TKEYTRG_POS)

#define RTC_RESET          (0x5 << 20)
///CCR
#define RTC_DIVS_POS	(0ul)
#define RTC_DIVS_MSK	(0x7fff << RTC_DIVS_POS)
#define RTC_DIVA_POS	(16ul)
#define RTC_DIVA_MSK	(0x7f << RTC_DIVA_POS)
#define RTC_DIVS(n)    ((n))
#define RTC_DIVA(n)    ((n)<<RTC_DIVA_POS)
#define RTC_DBGEN      (1ul<<23)
#define RTC_CLKSRC_POS	(24ul)
#define RTC_CLKSRC_MSK	(0x3 << RTC_CLKSRC_POS)
typedef enum{
	RTC_ISOSC = 0,
	RTC_IMOSC_DIV4,
	RTC_ESOSC,
	RTC_EMOSC_DIV4
}rtc_clksrc_e;

#define RTC_CLK_STABLE	(0x1 << 26)
#define RTC_CLKEN	   (1ul << 27)

///ALRA/ALRB
#define RTC_ALMA		(0)
#define RTC_ALMB		(1)
#define RTC_ALM_DAY_POS	(24ul)
#define RTC_ALM_DAY_MSK	(0x3f << RTC_ALM_DAY_POS)
#define RTC_ALM_DAYT_POS	(28)
#define RTC_ALM_DAYT_MSK	(0x3 << RTC_ALM_DAYT_POS)
#define RTC_ALM_DAYU_POS	(24)
#define RTC_ALM_DAYU_MSK	(0xf << RTC_ALM_DAYU_POS)
#define RTC_ALM_DMSK_POS	(31)
#define RTC_ALM_DMSK_MSK	(0x1 << RTC_ALM_DMSK_POS)
#define RTC_ALM_WDSEL_POS	(30ul)
#define RTC_ALM_WDSEL_MSK	(0x1 << RTC_ALM_WDSEL_POS)
#define RTC_ALM_DSEL	(0x0)
#define RTC_ALM_WSEL	(0x1)
#define RTC_ALM_DAYU(n) ((n) << 24)
#define RTC_ALM_DAYT(n) ((n) << 28)


#define RTC_ALM_HOR_POS (16ul)
#define RTC_ALM_HOR_MSK	(0x3f << RTC_ALM_HOR_POS)
#define RTC_ALM_HORT_POS	(20)
#define RTC_ALM_HORT_MSK	(0x3 << RTC_ALM_HORT_POS)
#define RTC_ALM_HORU_POS	(16)
#define RTC_ALM_HORU_MSK	(0xf << RTC_ALM_HORU_POS)
#define RTC_ALM_HMSK_POS	(23)
#define RTC_ALM_PM		(0x1 << 22)
#define RTC_ALM_HORU(n) ((n) << 20)
#define RTC_ALM_HORT(n) ((n) << 16)

#define RTC_ALM_MIN_POS	(8)
#define RTC_ALM_MIN_MSK	(0x7f << RTC_ALM_MIN_POS)
#define RTC_ALM_MINT_POS	(12)
#define RTC_ALM_MINT_MSK	(0x7 << RTC_ALM_MINT_POS)
#define RTC_ALM_MINU_POS	(8)
#define RTC_ALM_MINU_MSK	(0xf << RTC_ALM_MINU_POS)
#define RTC_ALM_MMSK_POS    (15)
#define RTC_ALM_MINU(n) ((n) << 12)
#define RTC_ALM_MINT(n) ((n) << 8)

#define RTC_ALM_SEC_POS	(0)
#define RTC_ALM_SEC_MSK	(0x7f << RTC_ALM_SEC_POS)
#define RTC_ALM_SECT_POS	(4)
#define RTC_ALM_SECT_MSK	(0x7 << RTC_ALM_SECT_POS)
#define RTC_ALM_SECU_POS	(0)
#define RTC_ALM_SECU_MSK	(0xf << RTC_ALM_SECU_POS)
#define RTC_ALM_SMSK_POS	(7)
#define RTC_ALM_SECU(n) ((n) << 4)
#define RTC_ALM_SECT(n) ((n) << 0)

#define RTC_ALM_MODE 	(0xC0808080)

///SSR
#define RTC_SSR_MSK		(0xffff)

/// Interrupt Related
#define ALRA_INT_POS 	(0)
#define ALRB_INT_POS	(1)
#define CPRD_INT_POS	(2)
#define TRGEV0_INT_POS	(3)
#define TRGEV1_INT_POS	(4)
typedef enum{
	RTC_INT_ALMA = 0x1 << ALRA_INT_POS,
	RTC_INT_ALMB = 0x1 << ALRB_INT_POS,
	RTC_INT_CPRD = 0x1 <<CPRD_INT_POS,
	RTC_INT_TRGEV0 = 0x1 <<TRGEV0_INT_POS,
	RTC_INT_TRGEV1 = 0x1 <<TRGEV1_INT_POS
}rtc_int_e;


///KEY
#define RTC_KEY		(0xca53)

///EVTRG
#define RTC_TRGSEL0_POS	(0)
#define RTC_TRGSEL0_MSK	(0xf)
#define RTC_TRGSEL1_POS	(4)
#define RTC_TRGSEL1_MSK	(0xf << RTC_TRGSEL1_POS)
typedef enum {
	RTC_TRG_DIS = 0,
	RTC_TRG_A,
	RTC_TRG_B,
	RTC_TRG_CPRD
}csp_rtc_trgsel_e;
#define RTC_TRG0OE		(0x1 << 20)
#define RTC_TRG1OE		(0x1 << 21)

///EVPS
#define RTC_TRGEV0PRD_POS	(0)
#define RTC_TRGEV0PRD_MSK	(0xf)
#define RTC_TRGEV1PRD_POS	(1)
#define RTC_TRGEV1PRD_MSK	(0xf << RTC_TRGEV1PRD_POS)
#define RTC_TRGEV0CNT_POS	(1)
#define RTC_TRGEV0CNT_MSK	(0xf << RTC_TRGEV0CNT_POS)

//EVSWF
#define RTC_EV0SWF 	(0x1)
#define RTC_EV1SWF  (0x2)


/// #define functions
#define csp_rtc_clk_enable(RTCx)						(RTCx->CCR |= RTC_CLKEN)			///key is needed!

#define csp_rtc_clk_disable(RTCx)						(RTCx->CCR &= (~RTC_CLKEN))			///key is needed!
															
#define csp_rtc_update_status(RTCx)						(RTCx->CR & RTC_BSY)

#define csp_rtc_set_key(RTCx)							(RTCx->KEY = 0xCA53)

#define csp_rtc_clr_key(RTCx)							(RTCx->KEY = 0x0)

#define csp_rtc_set_fmt(RTCx,bFmt)						(RTCx->CR = (RTCx->CR & (~RTC_FMT_MSK)) | (bFmt << RTC_FMT_POS))	///key is needed!
											
#define csp_rtc_get_fmt(RTCx)							((rtc_fmt_e)((RTCx->CR) & RTC_FMT_MSK) >> RTC_FMT_POS)

#define csp_rtc_debug_enable(RTCx)						(RTCx->CCR = (RTCx->CCR &(~RTC_DBGEN)) | (1 << 23))  	///key is needed!

#define csp_rtc_debug_disable(RTCx)						(RTCx->CCR = (RTCx->CCR &(~RTC_DBGEN)) | (0 << 23))		///key is needed!
												
#define csp_rtc_rb_enable(RTCx)							(RTCx->CR = (RTCx->CR &(~RTC_RBEN)) | (1 << 16))		///key is needed!
												
#define csp_rtc_rb_disable(RTCx)						(RTCx->CR = (RTCx->CR &(~RTC_RBEN)) | (0 << 16))		///key is needed!

#define csp_rtc_set_cprd(RTCx,eCprd)					(RTCx->CR  = (RTCx->CR & (~RTC_CPRD_MSK)) | (eCprd << RTC_CPRD_POS))	///key is needed!  \param eCprd \ref rtc_cprd_e

#define csp_rtc_set_osel(RTCx,eOsel)					(RTCx->CR  = (RTCx->CR & (~RTC_OSEL_MSK)) | (eOsel << RTC_OSEL_POS))  	///key is needed!  \param eOsel \ref rtc_osel_e

#define csp_rtc_stop(RTCx)								(RTCx->CR |= RTC_INIT)			///key is needed!

#define csp_rtc_run(RTCx)								(RTCx->CR &= (~RTC_INIT))		///key is needed!

#define csp_rtc_reset(RTCx)								(RTCx->CR |= RTC_RESET)

#define csp_rtc_is_busy(RTCx)							((bool)((RTCx->CR & RTC_BSY) >> 1))

#define csp_rtc_is_running(RTCx)						((bool)(!(RTCx->CR & RTC_INIT)))

#define csp_rtc_set_time_hour(RTCx,bPm,byVal)			(RTCx->TIMR = (RTCx->TIMR  & (~RTC_HOR_MSK) & (~RTC_PM_MSK)) |(byVal << RTC_HORU_POS) | ((bool)bPm << RTC_PM_POS))

#define csp_rtc_set_time_min(RTCx,byVal)				(RTCx->TIMR = (RTCx->TIMR  & (~RTC_MIN_MSK)) | (byVal << RTC_MINU_POS))

#define csp_rtc_set_time_sec(RTCx,byVal)				(RTCx->TIMR = (RTCx->TIMR  & (~RTC_SEC_MSK)) |  (byVal << RTC_SECU_POS))

#define csp_rtc_set_date_year(RTCx,byVal)				(RTCx->DATR = (RTCx->DATR  & (~RTC_YEA_MSK)) |  (byVal << RTC_YEAU_POS))

#define csp_rtc_set_date_mon(RTCx,byVal)				(RTCx->DATR = (RTCx->DATR  & (~RTC_MON_MSK)) |  (byVal << RTC_MONU_POS))

#define csp_rtc_set_date_wday(RTCx,byVal)				(RTCx->DATR = (RTCx->DATR  & (~RTC_WKD_MSK)) | (byVal << RTC_WKD_POS))

#define csp_rtc_set_date_day(RTCx,byVal)				(RTCx->DATR = (RTCx->DATR  & (~RTC_DAY_MSK)) |  (byVal << RTC_DAYU_POS))

#define csp_rtc_set_date_week(RTCx,byVal)				(RTCx->DATR = (RTCx->DATR  & (~RTC_WKD_MSK)) | (byVal << RTC_WKD_POS))

#define csp_rtc_read_year(RTCx)							((uint8_t)((RTCx->DATR & RTC_YEAT_MSK) >> RTC_YEAT_POS) * 10 + ((RTCx->DATR & RTC_YEAU_MSK) >> RTC_YEAU_POS))

#define csp_rtc_read_mon(RTCx)							((uint8_t)((RTCx->DATR & RTC_MONT_MSK) >> RTC_MONT_POS) * 10 + ((RTCx->DATR & RTC_MONU_MSK) >> RTC_MONU_POS))

#define csp_rtc_read_mday(RTCx)							((uint8_t)((RTCx->DATR & RTC_DAYT_MSK) >> RTC_DAYT_POS) * 10 + ((RTCx->DATR & RTC_DAYU_MSK) >> RTC_DAYU_POS))

#define csp_rtc_read_wday(RTCx)							((uint8_t)((RTCx->DATR & RTC_WKD_MSK) >> RTC_WKD_POS))

#define csp_rtc_read_hour(RTCx)							((uint8_t)((RTCx->TIMR & RTC_HORT_MSK) >> RTC_HORT_POS) * 10 + ((RTCx->TIMR & RTC_HORU_MSK) >> RTC_HORU_POS))

#define csp_rtc_read_pm(RTCx)							((uint8_t)((RTCx->TIMR & RTC_PM_MSK) >> RTC_PM_POS))

#define csp_rtc_read_min(RTCx)							((uint8_t)((RTCx->TIMR & RTC_MINT_MSK) >> RTC_MINT_POS) * 10 + ((RTCx->TIMR & RTC_MINU_MSK) >> RTC_MINU_POS))

#define csp_rtc_read_sec(RTCx)							((uint8_t)((RTCx->TIMR & RTC_SECT_MSK) >> RTC_SECT_POS) * 10 + ((RTCx->TIMR & RTC_SECU_MSK) >> RTC_SECU_POS))

#define csp_rtc_alm_enable(RTCx,byAlm)					(RTCx->CR |= 0x1 << (3 + byAlm))		///key is needed!
														
#define csp_rtc_alm_disable(RTCx,byAlm)					(RTCx->CR &= ~(0x1 << (3 + byAlm)))		///key is needed!												

#define csp_rtc_alm_set_day(RTCx,byAlm,byVal)			((byAlm == RTC_ALMA)?\
															(RTCx->ALRA = (RTCx->ALRA & (~RTC_ALM_DAY_MSK)) |(byVal << RTC_ALM_DAYU_POS)):\
															(RTCx->ALRB = (RTCx->ALRB & (~RTC_ALM_DAY_MSK))|(byVal << RTC_ALM_DAYU_POS)))

#define csp_rtc_alm_set_hour(RTCx,byAlm,byPm,byVal)		((byAlm == RTC_ALMA)?\
															(RTCx->ALRA = (RTCx->ALRA & (~RTC_ALM_HOR_MSK) & (~RTC_ALM_PM)) | (byVal << RTC_ALM_HORU_POS) | (byPm<<22)):\
															(RTCx->ALRB = (RTCx->ALRB & (~RTC_ALM_HOR_MSK) & (~RTC_ALM_PM)) |  (byVal << RTC_ALM_HORU_POS) | (byPm<<22)))


#define csp_rtc_alm_set_min(RTCx,byAlm,byVal)			((byAlm == RTC_ALMA)?\
															(RTCx->ALRA = (RTCx->ALRA & (~RTC_ALM_MIN_MSK)) | (byVal << RTC_ALM_MINU_POS)):\
															(RTCx->ALRB = (RTCx->ALRB & (~RTC_ALM_MIN_MSK)) |  (byVal << RTC_ALM_MINU_POS)))

#define csp_rtc_alm_set_sec(RTCx,byAlm,byVal)			((byAlm == RTC_ALMA)?\
															(RTCx->ALRA = (RTCx->ALRA & (~RTC_ALM_SEC_MSK)) | (byVal << RTC_ALM_SECU_POS)):\
															(RTCx->ALRB = (RTCx->ALRB & (~RTC_ALM_SEC_MSK)) | (byVal << RTC_ALM_SECU_POS)))

#define csp_rtc_alm_read_mday(RTCx,byAlm)				((byAlm == RTC_ALMA)?\
															(((RTCx->ALRA & RTC_ALM_DAYT_MSK) >> RTC_ALM_DAYT_POS) * 10 + ((RTCx->ALRA & RTC_ALM_DAYU_MSK) >> RTC_ALM_DAYU_POS)):\
															(((RTCx->ALRB & RTC_ALM_DAYT_MSK) >> RTC_ALM_DAYT_POS) * 10 + ((RTCx->ALRB & RTC_ALM_DAYU_MSK) >> RTC_ALM_DAYU_POS)))

#define csp_rtc_alm_read_wday(RTCx,byAlm)				((byAlm == RTC_ALMA)?\
															(uint8_t)((RTCx->ALRA & RTC_ALM_DAYU_MSK) >> RTC_ALM_DAYU_POS):\
															(uint8_t)((RTCx->ALRB & RTC_ALM_DAYU_MSK) >> RTC_ALM_DAYU_POS))

#define csp_rtc_alm_read_hour(RTCx,byAlm)				((byAlm == RTC_ALMA)?\
															(uint8_t)((RTCx->ALRA & RTC_ALM_HORT_MSK) >> RTC_ALM_HORT_POS) * 10 + ((RTCx->ALRA & RTC_ALM_HORU_MSK) >> RTC_ALM_HORU_POS):\
															(uint8_t)((RTCx->ALRB & RTC_ALM_HORT_MSK) >> RTC_ALM_HORT_POS) * 10 + ((RTCx->ALRB & RTC_ALM_HORU_MSK) >> RTC_ALM_HORU_POS))

#define csp_rtc_alm_read_min(RTCx,byAlm)				((byAlm == RTC_ALMA)?\
															(uint8_t)((RTCx->ALRA & RTC_ALM_MINT_MSK) >> RTC_ALM_MINT_POS) * 10 + ((RTCx->ALRA & RTC_ALM_MINU_MSK) >> RTC_ALM_MINU_POS):\
															(uint8_t)((RTCx->ALRB & RTC_ALM_MINT_MSK) >> RTC_ALM_MINT_POS) * 10 + ((RTCx->ALRB & RTC_ALM_MINU_MSK) >> RTC_ALM_MINU_POS))

#define csp_rtc_alm_read_sec(RTCx,byAlm)				((byAlm == RTC_ALMA)?\
															(uint8_t)((RTCx->ALRA & RTC_ALM_SECT_MSK) >> RTC_ALM_SECT_POS) * 10 + ((RTCx->ALRA & RTC_ALM_SECU_MSK) >> RTC_ALM_SECU_POS):\
															(uint8_t)((RTCx->ALRB & RTC_ALM_SECT_MSK) >> RTC_ALM_SECT_POS) * 10 + ((RTCx->ALRB & RTC_ALM_SECU_MSK) >> RTC_ALM_SECU_POS))

#define csp_rtc_alm_read_dmsk(RTCx,byAlm)				((byAlm == RTC_ALMA)?\
															(bool)( (RTCx->ALRA & RTC_ALM_DMSK_MSK) >> RTC_ALM_DMSK_POS):\
															(bool)( (RTCx->ALRB & RTC_ALM_DMSK_MSK) >> RTC_ALM_DMSK_POS))

#define csp_rtc_alm_read_wdsel(RTCx,byAlm)				((byAlm == RTC_ALMA)?\
															(bool)( (RTCx->ALRA & RTC_ALM_WDSEL_MSK) >> RTC_ALM_WDSEL_POS):\
															(bool)( (RTCx->ALRB & RTC_ALM_WDSEL_MSK) >> RTC_ALM_WDSEL_POS))

#define csp_rtc_alm_set_mode(RTCx,byAlm,bWdsel,\
		bDmsk,bHmsk,bMmsk,bSmsk)						((byAlm == RTC_ALMA)?\
															(RTCx->ALRA = (RTCx->ALRA & (~RTC_ALM_MODE)) | (bool)bWdsel << RTC_ALM_WDSEL_POS |((bool)bDmsk << RTC_ALM_DMSK_POS) | ((bool)bHmsk << RTC_ALM_HMSK_POS) \
																| ((bool)bMmsk << RTC_ALM_MINU_POS) |((bool)bSmsk << RTC_ALM_SMSK_POS)):\
															(RTCx->ALRB = (RTCx->ALRB & (~RTC_ALM_MODE)) | (bool)bWdsel << RTC_ALM_WDSEL_POS |((bool)bDmsk << RTC_ALM_DMSK_POS) | ((bool)bHmsk << RTC_ALM_HMSK_POS) \
																| ((bool)bMmsk << RTC_ALM_MINU_POS) |((bool)bSmsk << RTC_ALM_SMSK_POS)))    ///key is needed!


#define csp_rtc_int_enable(RTCx,eInt)					(RTCx->IMCR |= eInt)	///\param eInt \ref rtc_int_e

#define csp_rtc_int_disable(RTCx,eInt)					(RTCx->IMCR &= ~eInt)	///\param eInt \ref rtc_int_e

#define csp_rtc_clr_isr(RTCx,eInt)						(RTCx->ICR = eInt)		///\param eInt \ref rtc_int_e

#define csp_rtc_get_isr(RTCx)							(RTCx->MISR)

#define csp_rtc_get_imcr(RTCx)							(RTCx->IMCR)

#define csp_rtc_swf_trg(RTCx,byTrg)						(RTCx -> EVSWF = 0x1 << byTrg)


#endif   /* CSP_EPWM_H */


