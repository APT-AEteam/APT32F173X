/***********************************************************************//** 
 * \file  rtc.h
 * \brief  head file for RTC
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-9-8 <td>V0.0  <td>WNN   <td>initial
 * <tr><td> 2021-1-8 <td>V0.0  <td>WNN   <td>modify
 * <tr><td> 2023-9-22 <td>V0.1  <td>GQQ   <td>code normalization
 * </table>
 * *********************************************************************
*/

#ifndef _DRV_RTC_H_
#define _DRV_RTC_H_

#include "csp.h"

#ifdef __cplusplus
extern "C" {
#endif  


#define RTC_TIME_BASE_YEAR                          (2000)                                 //< Year,      Effective range[2000,2099]

#define RTC_TIME_MAX_VAL_YEAR                       (99)                                   ///< Year,     Maximum value
#define RTC_TIME_MAX_VAL_MON                        (12)                                   ///< Month,    Maximum value
#define RTC_TIME_MAX_VAL_DAY                        (31)                                   ///< Day,      Maximum value
#define RTC_TIME_MAX_VAL_HOUR                       (23)                                   ///< Hour,     Maximum value
#define RTC_TIME_MAX_VAL_MIN                        (59)                                   ///< Minute,   Maximum value
#define RTC_TIME_MAX_VAL_SEC                        (59)                                   ///< Second,   Maximum value


#define RTC_TIME_MIN_VAL_YEAR                       (0)                                   ///< Year,     Minimum value
#define RTC_TIME_MIN_VAL_DAY                        (1)                                    ///< Day,      Minimum value

/**
 * \brief Determine if it is a leap year
 * 0 - this is not leapyear 
 * 1  - this is leapyear 
*/
#define RTC_IS_LEAPYEAR(_year_)                  (((_year_) % 400) ? (((_year_) % 100) ? (((_year_) % 4) ? (int)0 : (int)1) : (int)0) : (int)1)

/**
 * week:                 tm_wday:
 * 0 is Monday           0 is Sunday
 * 1 is Tuesday          1 is Monday
 * 2 is Wednesday        2 is Tuesday
 * 3 is Thursday         3 is Wednesday
 * 4 is Friday           4 is Thursday
 * 5 is Saturday         5 is Friday
 * 6 is Sunday           6 is Saturday
*/
#define RTC_CALCULATE_WEEK(_week_)               ((_week_ == 6) ? 0 : (_week_ + 1))


typedef struct {
	uint8_t		byAlmMode;	//mode = 0:  day       hour min sec 
							//mode = 1:  weekday   hour min sec
							//mode = 2:            hour min sec
	uint8_t		byAlmSt;	// 0: not yet
							// 1: time up
}csi_rtc_alm_t;

/****** RTC time *****/
typedef struct {
    int iSec;             //< Second.      [0-59]
    int iMin;             //< Minute.      [0-59]
    int iHour;            //< Hour.        [0-23]
    int iMday;            //< Day.         [1-31]
    int iMon;             //< Month.       [1-12]
    int iYear;            //< Year= iYear+2000. [0- 99]      !NOTE:0=2000 
	int iWday;			  // weekday	   [1-7]	     
    int iYday;            //< Days in year.[0-365]     !NOTE:January 1st = 0
	int iIsdst;			  // Non-0 if daylight savings time is in effect
	int iPm;			  //< PM.		  [0/1]
} csi_rtc_time_t;

/****** definition for rtc *****/
typedef struct csi_rtc csi_rtc_t;
struct csi_rtc {
    csi_dev_t           dev;
    void               (*callback)(csi_rtc_t *rtc, void *arg);
    void               *arg;
    void               *priv;
} ;

/// \struct csi_rtc_ctrl_t
/// \brief  rtc control handle, not open to users  
typedef struct 
{
    void(*callback)(csp_rtc_t *ptRtcBase, uint8_t byIsr);
} csi_rtc_ctrl_t;

extern csi_rtc_ctrl_t g_tRtcCtrl[RTC_IDX];


typedef enum{
	RTC_CLKSRC_ISOSC = 0,
	RTC_CLKSRC_IMOSC_DIV4,
	RTC_CLKSRC_ESOSC,
	RTC_CLKSRC_EMOSC_DIV4
}csi_rtc_clksrc_e;

typedef enum {
	RTC_TIMER_DIS = 0,
	RTC_TIMER_0_5S,
	RTC_TIMER_1S,
	RTC_TIMER_1MIN,
	RTC_TIMER_1H,
	RTC_TIMER_1DAY,
	RTC_TIMER_1MON	
}csi_rtc_timer_e;

typedef enum {
	RTC_ALMA_P = 0,
	RTC_ALMA_H,
	RTC_ALMA_L,
	RTC_ALMB_P,
	RTC_ALMB_H,
	RTC_ALMB_L,
	
}csi_rtc_osel_e;

typedef enum{
	RTC_TRGOUT0 = 0,
	RTC_TRGOUT1
}csi_rtc_trgout_e;


typedef enum{
	RTC_TRGOUT_DIS = 0,
	RTC_TRGOUT_ALRMA,
	RTC_TRGOUT_ALRMB,
	RTC_TRGOUT_ALRMA_ALRMB,
	RTC_TRGOUT_CPRD
}csi_rtc_trgsrc_e;

typedef enum{
	RTCINT_ALMA = 1<<0,
	RTCINT_ALMB = 1<<1,
	RTCINT_CPRD = 1<<2,
	RTCINT_TRGEV0 = 1<<3,
	RTCINT_TRGEV1 = 1<<4
}csi_rtc_int_e;

typedef enum {
	RTC_MODE_24FMT = 0,
	RTC_MODE_12FMT
}csi_rtc_fmt_e;


typedef struct  {
	csi_rtc_clksrc_e	eClkSrc;	// clock source
	csi_rtc_fmt_e		eFmt;		//  timer format
} csi_rtc_config_t;


/**\brief	Initialize RTC Interface. Initializes the resources needed for the RTC interface
 * 
 * \param[in]	RtcConfig : Pointer of RTC config structure
 * \return	none
*/
void csi_rtc_init(csp_rtc_t *ptRtc, csi_rtc_config_t *tConfig);

/**\brief	Read back function enable control. To save current, disable read back.
 * 
 * \param[in]	ptRtc： handle pointer of rtc register structure to operate 
 * \return	none
*/
void csi_rtc_rb_enable(csp_rtc_t *ptRtc);

/**\brief	 To save current, disable read back.
 * 
 * \param[in]	ptRtc： handle pointer of rtc register structure to operate  
 * \return	none
*/
void csi_rtc_rb_disable(csp_rtc_t *ptRtc);

/**\brief       De-initialize RTC Interface. stops operation and releases the software resources used by the interface
 * 
 * \param[in]   rtc    rtc handle to operate
 * \return      None
*/
void csi_rtc_uninit(csi_rtc_t *rtc);

/**\brief       Set system date and wait for synchro
 * 
 * \param[in]   rtc        rtc handle to operate
 * \param[in]   rtctime    pointer to rtc time
 * \return      error code \ref csi_error_t
*/
csi_error_t csi_rtc_set_current_time(csp_rtc_t *rtc, csi_rtc_time_t *rtctime);

/**\brief       To start RTC 
 * 
 * \param[in]       rtc handle rtc handle to operate
 * \return      none
*/
void csi_rtc_start(csp_rtc_t *ptRtc);

/**\brief       To set/change RTC format
 * 
 * \param[in]       rtc handle rtc handle to operate
 * \param[in]       eFmt \ref rtc_fmt_e    
 * \return      none
*/
void csi_rtc_change_fmt(csp_rtc_t *ptRtc,  rtc_fmt_e eFmt);

/**\brief   RTC interrupt enable/disable
 * 
 * \param[in]   ptRtc      handle rtc handle to operate
 * \param[in]   eIntSrc	  interrupt source	
 * \return  none
*/
void csi_rtc_int_enable(csp_rtc_t *ptRtc, csi_rtc_int_e eIntSrc);

/**\brief   RTC interrupt disable
 * 
 * \param[in]   ptRtc      handle pointer of rtc register structure to operate
 * \param[in]   eIntSrc	  interrupt source	
 * \return  none
*/
void csi_rtc_int_disable(csp_rtc_t *ptRtc, csi_rtc_int_e eIntSrc);

/**\brief To reset RTC 
 * 
 * \param[in] pointer of rtc register structure pointer of rtc register structure to operate
 * \return  none
*/
void csi_rtc_sw_rst(csp_rtc_t *ptRtc);

/**\brief       Get system date
 * 
 * \param[in]   rtc        rtc handle to operate
 * \param[out]  rtctime    pointer to rtc time
 * \return      none
*/
void csi_rtc_get_time(csp_rtc_t *rtc, csi_rtc_time_t *rtctime);

/**\brief       Get alarm remaining time
 * 
 * \param[in]   rtc    rtc handle to operate
 * \param[in]	   byAlm  RTC_ALMA/RTC_ALMB
 * \return      the remaining time(s)
*/
uint32_t csi_rtc_get_alarm_remaining_time(csp_rtc_t *rtc, uint8_t byAlm);

/**\brief   Config RTC alarm
 * 
 * \param[in]   ptRtc      handle rtc handle to operate
 * \param[in]   byAlm	  RTC_ALMA/RTC_ALMB
 * \param[in]   rtctime    alarm time(s) 
 * \param[in]   byMode	  	0: day       hour min sec
 *						1: weekday   hour min sec
 *						2:           hour min sec
 *
 * \return  error code \ref csi_error_t
*/
csi_error_t csi_rtc_set_alarm(csp_rtc_t *ptRtc, uint8_t byAlm, uint8_t byMode, csi_rtc_time_t *tpRtcTime);

/**\brief       Cancel the rtc alarm
 * 
 * \param[in]       ptRtc    rtc handle to operate
 * \param[in]       byAlm	RTC_ALMA/RTC_ALMB
 * \return      none
*/
void csi_rtc_cancel_alarm(csp_rtc_t *ptRtc, uint8_t byAlm);

/**\brief       Judge rtc is running
 * 
 * \param[in]   rtc    rtc handle to operate
 * \return      
 *              true  - rtc is running 
 *              false - rtc is not running 
*/
bool csi_rtc_is_running(csi_rtc_t *rtc);


/**\brief    use rtc as a timer
 * 
 * \param[in]	callback  callback function to be called when time = ePrd
 * \param[in] 	ePrd    time interval of the timer.
 * \param[in]   rtc    rtc handle to operate
*/
void csi_rtc_set_timer(csp_rtc_t *ptRtc, csi_rtc_timer_e ePrd);

/** \brief       Config RTC alarm ture timer
 * 
 * \param[in]   ptRtc      handle rtc handle to operate
 * \param[in]   eOut     rtc output
 * \return      none
*/
void csi_rtc_set_alarm_out(csp_rtc_t *ptRtc, csi_rtc_osel_e eOut);

/** \brief rtc event trigger source output config  
 * 
 *  \param[in] ptRtc: RTC handle to operate
 *  \param[in] eTrg: rtc evtrgout channel \ref csi_rtc_trgout_e
 *  \param[in] eTrgSrc: rtc evtrg source  \ref csi_rtc_trgsrc_e
 *  \param[in] trg_prd: event count period 
 *  \return none
 */
void csi_rtc_set_evtrg(csp_rtc_t *ptRtc, csi_rtc_trgout_e eTrg, csi_rtc_trgsrc_e eTrgSrc, uint8_t byTrgPrd);

/** \brief rtc event trigger enable  
 * 
 *  \param[in] ptRtc: RTC handle to operate
 *  \param[in] eTrg: rtc evtrgout channel \ref csi_rtc_trgout_e
 *  \return none
 */
void csi_rtc_evtrg_enable(csp_rtc_t *ptRtc, csi_rtc_trgout_e eTrg);

/** \brief  register rtc interrupt callback function
 * 
 *  \param[in] ptRtcBase: pointer of rtc register structure
 *  \param[in] callback: rtc interrupt handle function
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_rtc_register_callback(csp_rtc_t *ptRtcBase, void  *callback);

/** \brief rtc interrupt handler function
 * 
 *  \param[in] ptRtcBase: pointer of rtc register structure
 *  \param[in] byIdx: rtc idx(0)
 *  \return none
 */ 
void csi_rtc_irqhandler(csp_rtc_t *ptRtcBase,  uint8_t byIdx);

#ifdef __cplusplus
}
#endif

#endif /* _DRV_RTC_H_ */
