/***********************************************************************//** 
 * \file  rtc.c
 * \brief  rtc driver
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-6-2 <td>V2.0 <td>WNN    <td>initial
 * </table>
 * *********************************************************************
*/

/* include ----------------------------------------------------------------*/
#include "drv/rtc.h"
#include "rtc_alg.h"
#include "board_config.h"

/* externs function--------------------------------------------------------*/
/* private function--------------------------------------------------------*/


static void apt_rtc_alm_set_time(csp_rtc_t *ptRtc, uint8_t byAlm, uint8_t byDay, bool byPm, uint8_t byHor, uint8_t byMin,uint8_t bySec);
static csp_error_t apt_rtc_set_time(csp_rtc_t *ptRtc, bool bPm, uint8_t byHor, uint8_t byMin,uint8_t bySec);
static csp_error_t apt_rtc_set_date(csp_rtc_t *ptRtc, uint8_t byYear, uint8_t byMon, uint8_t byWday, uint8_t byDay);
//csp_error_t apt_rtc_set_trgsrc(csp_rtc_t *ptRtc, csi_rtc_trgout_e eTrg, csi_rtc_trgsrc_e eSrc);
//csp_error_t apt_rtc_set_trgprd(csp_rtc_t *ptRtc, csi_rtc_trgout_e eTrg, uint8_t byPrd);
static uint8_t apt_get_rtc_idx(csp_rtc_t *ptRtcBase);

/* externs variable------------------------------------------------------*/
csi_rtc_ctrl_t g_tRtcCtrl[RTC_IDX];

/* Private variable------------------------------------------------------*/


/** \brief rtc interrupt handler function
 * 
 *  \param[in] ptRtcBase: pointer of rtc register structure
 *  \param[in] byIdx: rtc idx(0)
 *  \return none
 */ 
void csi_rtc_irqhandler(csp_rtc_t *ptRtcBase,  uint8_t byIdx)
{
	rtc_int_e eIsr = csp_rtc_get_isr(ptRtcBase);
	
	if(g_tRtcCtrl[byIdx].callback)
		g_tRtcCtrl[byIdx].callback(ptRtcBase, eIsr);
			
	csp_rtc_clr_isr(ptRtcBase, eIsr);
}

/**\brief	Initialize RTC Interface. Initializes the resources needed for the RTC interface
 * 
 * \param	ptRtc : rtc handle
 * \param	eOsc : clock source 
 * \param	eFmt : rtc format: RTC_24FMT/RTC_12FMT
 * \return	none
*/
void csi_rtc_init(csp_rtc_t *ptRtc, csi_rtc_config_t *tConfig)
{
    uint8_t byDiva = 0;
	uint16_t hwDivs = 0;
	
	soc_clk_enable(RTC_SYS_CLK);
	
	csp_rtc_set_key(ptRtc);		
	csp_rtc_stop(ptRtc);
	csp_rtc_clr_key(ptRtc);
	while(csp_rtc_get_status(ptRtc));
	
	switch (tConfig->eClkSrc)
	{ 
		case (RTC_CLKSRC_ISOSC):
			csi_isosc_enable();
			byDiva = 49;
			hwDivs = 269;
			break;
		case (RTC_CLKSRC_ESOSC):
			csi_esosc_enable();
			byDiva = 3;
			hwDivs = 4095;
			break;
		case (RTC_CLKSRC_IMOSC_DIV4):
			switch(csp_get_imosc_freq(SYSCON))
			{
				case (0):	//5.556MHz
					byDiva = 124;
					hwDivs = 5555;
					break;
				case (1):	//4.194MHz
					byDiva = 124;
					hwDivs = 4193;
					break;
				case (2):	//2.097MHz
					byDiva = 124;
					hwDivs = 2096;
					break;
				case (3):	//131.072KHz
					byDiva = 3;
					hwDivs = 4095;
					break;
				default:
					break;
			}			
			break;
		case (RTC_CLKSRC_EMOSC_DIV4):
			csi_emosc_enable(EMOSC_VALUE); // EMOSC_VALUE = 24MHz, defined in "board_config.h "
			byDiva = 99;                   
			hwDivs = 29999;                
			break;
		default:
			break;
	}
	
	csp_rtc_set_key(ptRtc);
	csp_rtc_set_src_clk(ptRtc,(rtc_clksrc_e)tConfig->eClkSrc,byDiva,hwDivs);
	csp_rtc_clr_key(ptRtc);
	while((ptRtc->CCR & RTC_CLK_STABLE) == 0);
	
	csp_rtc_set_key(ptRtc);
	csp_rtc_rb_enable(ptRtc);
	csp_rtc_set_fmt(ptRtc, (rtc_fmt_e)tConfig->eFmt);
	csp_rtc_alm_disable(ptRtc, RTC_ALMB);
	csp_rtc_alm_disable(ptRtc, RTC_ALMA);
	
	csp_rtc_clr_key(ptRtc);
	while(csp_rtc_get_status(ptRtc));
	
	csp_rtc_int_disable(ptRtc, RTC_INT_ALL);
	csp_rtc_clr_isr(ptRtc,RTC_INT_ALL);

}

/**\brief	Read back function enable control.
 * 
 * \param	ptRtc： handle pointer of rtc register structure to operate   
 * \return	none
*/
void csi_rtc_rb_enable(csp_rtc_t *ptRtc)
{
	csp_rtc_set_key(ptRtc);
	csp_rtc_rb_enable(ptRtc);
	csp_rtc_clr_key(ptRtc);
	while(csp_rtc_get_status(ptRtc));
}

/**\brief	 To save current, disable read back.
 * 
 * \param	ptRtc： handle pointer of rtc register structure to operate  
 * \return	none
*/
void csi_rtc_rb_disable(csp_rtc_t *ptRtc)
{
	csp_rtc_set_key(ptRtc);
	csp_rtc_rb_disable(ptRtc);
	csp_rtc_clr_key(ptRtc);
	while(csp_rtc_get_status(ptRtc));
}
/**\brief To set/change RTC format
 * 
 * \param  rtc handle rtc handle to operate
 * \param  eFmt \ref rtc_fmt_e    
 * \return none
*/
void csi_rtc_change_fmt(csp_rtc_t *ptRtc,  rtc_fmt_e eFmt)
{
	csp_rtc_set_key(ptRtc);
	csp_rtc_set_fmt(ptRtc, (rtc_fmt_e)eFmt);
	csp_rtc_clr_key(ptRtc);
	while(csp_rtc_get_status(ptRtc));
}

/**\brief To start RTC 
 * 
 * \param  rtc handle rtc handle to operate
 * \return none
*/
void csi_rtc_start(csp_rtc_t *ptRtc)
{
	csp_rtc_set_key(ptRtc);
	csp_rtc_run(ptRtc);
	csp_rtc_clr_key(ptRtc);
	while(csp_rtc_get_status(ptRtc));
}


/**\brief To stop RTC 
 * 
 * \param  pointer of rtc register structure pointer of rtc register structure to operate
 * \return  none
*/
void csi_rtc_stop(csp_rtc_t *ptRtc)
{
	csp_rtc_set_key(ptRtc);		
	csp_rtc_stop(ptRtc);
	csp_rtc_clr_key(ptRtc);
	while(csp_rtc_get_status(ptRtc));
}

/**\brief To reset RTC 
 * 
 * \param pointer of rtc register structure pointer of rtc register structure to operate
 * \return  none
*/
void csi_rtc_sw_rst(csp_rtc_t *ptRtc)
{
	csp_rtc_sw_rst(ptRtc);
}

/**\brief       Set system date and run after setting
 * 				week day will be calculated in this function
 * \param       rtc        handle pointer of rtc register structure to operate
 * \param       rtctime    pointer to rtc time
 * \return      error code \ref csi_error_t
*/
csi_error_t csi_rtc_set_current_time(csp_rtc_t *ptRtc, csi_rtc_time_t *ptRtcTime)
{   
    csi_error_t ret = CSI_OK;
	
		
	do {
		
		ret = (csi_error_t) clock_check_tm_ok((const struct tm *)ptRtcTime);
        if (ret < CSI_OK) {
            break;
		
        }
		
		csp_rtc_set_key(ptRtc);		
		csp_rtc_stop(ptRtc);
		csp_rtc_clr_key(ptRtc);
		while(csp_rtc_get_status(ptRtc));
		
		ptRtcTime->iWday = get_week_by_date((struct tm *)ptRtcTime);
		
		
		apt_rtc_set_date(ptRtc, ptRtcTime->iYear, ptRtcTime->iMon, ptRtcTime->iWday, ptRtcTime->iMday);
		
	
		if ((ptRtcTime->iHour == 12) && (csp_rtc_get_fmt(ptRtc) == RTC_24FMT))
			ret =  (csi_error_t)apt_rtc_set_time(ptRtc, RTC_PM, ptRtcTime->iHour, ptRtcTime->iMin,ptRtcTime->iSec);
		else
			ret =  (csi_error_t)apt_rtc_set_time(ptRtc, ptRtcTime->iPm, ptRtcTime->iHour, ptRtcTime->iMin,ptRtcTime->iSec);
		
		
		if (ret < CSI_OK) {
            break;
        }
	}while(0);
	
	return ret;
}

/**\brief   Config RTC alarm
 * 
 * \param   ptRtc      	handle pointer of rtc register structure to operate
 * \param   byAlm		RTC_ALMA/RTC_ALMB
 * \param   ptAlmTime    alarm time(s) 
 * \param   byMode	  	0: day       hour min sec
 *						1: weekday   hour min sec
 *						2:           hour min sec
 *
 * \return  error code \ref csi_error_t
*/
csi_error_t csi_rtc_set_alarm(csp_rtc_t *ptRtc, uint8_t byAlm, uint8_t byMode, csi_rtc_time_t *ptAlmTime)
{ 	
	bool bDmsk = 0;		//day mask
	bool bWdsel = 0;	//weekday select
	bool bHmsk = 0;		//hour mask
	bool bMmsk = 0;		//minute mask
	bool bSmsk = 0;		//second mask
	bool bFmt = 0;		//formt
	
	if(byAlm > 1)
		return CSI_ERROR;

	if (ptAlmTime -> iYday == 0xff || byMode == 2) {
		bDmsk = 1;
		ptAlmTime->iMday = 0;			
	}
	if (ptAlmTime -> iHour == 0xff) {
		bHmsk = 1;
		ptAlmTime->iHour = 0;
	}
	if (ptAlmTime -> iMin == 0xff) {
		bMmsk = 1;
		ptAlmTime->iMin = 0;
	}
	if (ptAlmTime -> iSec == 0xff) {
		bSmsk = 1;	
		ptAlmTime->iSec = 0;
	}
	switch (byMode)
	{
		case 0:	bDmsk = 0;
				bWdsel = 0;
			break;
		case 1: bDmsk = 0;
				bWdsel = 1;
			break;
		case 2: bDmsk = 1;
				bWdsel = 0;
			break;
	}
	
	
	if(csp_rtc_get_fmt(ptRtc) == RTC_24FMT) {		
		if (ptAlmTime -> iHour == 12) 
			bFmt = RTC_PM;
		else
			bFmt = RTC_AM;
	}
	switch (byAlm)
	{
		case (RTC_ALMA): 	csi_rtc_int_enable(ptRtc, RTCINT_ALMA);
							break;
		case (RTC_ALMB):	csi_rtc_int_enable(ptRtc, RTCINT_ALMB);
							break;
		default:
			return CSI_ERROR;
	}
	
	csp_rtc_set_key(ptRtc);
	csp_rtc_alm_disable(ptRtc, byAlm);
	csp_rtc_clr_key(ptRtc);
	while(csp_rtc_get_status(ptRtc));
	
	apt_rtc_alm_set_time(ptRtc, byAlm, ptAlmTime->iMday, bFmt,  ptAlmTime->iHour, ptAlmTime->iMin,ptAlmTime->iSec);
	
	csp_rtc_set_key(ptRtc);	
	if(byAlm == RTC_ALMA)
	{
		csp_rtc_alma_set_mode(ptRtc, bWdsel, bDmsk, bHmsk, bMmsk, bSmsk);
	}
	else {
		csp_rtc_almb_set_mode(ptRtc, bWdsel, bDmsk, bHmsk, bMmsk, bSmsk);
	}
	csp_rtc_alm_enable(ptRtc, byAlm);
	csp_rtc_clr_key(ptRtc);
	while(csp_rtc_get_status(ptRtc));
	
	return CSI_OK;
}

/**\brief Cancel the rtc alarm, keep last alarm time and mode setting
 * 
 * \param ptRtc    pointer of rtc register structure to operate
 * \param byAlm	RTC_ALMA/RTC_ALMB
 * \return none
*/
void csi_rtc_cancel_alarm(csp_rtc_t *ptRtc, uint8_t byAlm)
{
	csp_rtc_set_key(ptRtc);
	csp_rtc_alm_disable(ptRtc, byAlm);
	csp_rtc_clr_key(ptRtc);
	while(csp_rtc_get_status(ptRtc));	
	
	switch (byAlm)
	{
		case (RTC_ALMA): 	csi_rtc_int_disable(ptRtc, RTCINT_ALMA);
							csp_rtc_clr_isr(ptRtc, RTC_INT_ALMA);
							break;
		case (RTC_ALMB):	csi_rtc_int_disable(ptRtc, RTCINT_ALMB);
							csp_rtc_clr_isr(ptRtc, RTC_INT_ALMB);
							break;
		default: break;
	}
}

/**\brief  Config RTC alarm ture timer
 * 
 * \param[in]   ptRtc      handle pointer of rtc register structure to operate
 * \param[in]   eOut     rtc output
 * \return      none
*/
void csi_rtc_set_alarm_out(csp_rtc_t *ptRtc, csi_rtc_osel_e eOut)
{	
	csp_rtc_set_key(ptRtc);
	csp_rtc_set_osel(ptRtc, (rtc_osel_e)eOut);
	csp_rtc_clr_key(ptRtc);
	while(csp_rtc_get_status(ptRtc));
}


/**\brief use rtc as a timer
 * 
 * \param	callback  callback function to be called when time = ePrd
 * \param 	ePrd    time interval of the timer.
 * \param[in]   rtc    pointer of rtc register structure to operate
*/
void csi_rtc_set_timer(csp_rtc_t *ptRtc, csi_rtc_timer_e ePrd)
{	
	csp_rtc_set_key(ptRtc);
	csp_rtc_set_cprd(ptRtc, (rtc_cprd_e)ePrd);
	csp_rtc_clr_key(ptRtc);
	while(csp_rtc_get_status(ptRtc));
	csi_rtc_int_enable(ptRtc, RTCINT_CPRD);
}

/**\brief   RTC interrupt enable
 * 
 * \param   ptRtc      handle pointer of rtc register structure to operate
 * \param   eIntSrc	  interrupt source	
 * \return  none
*/
void csi_rtc_int_enable(csp_rtc_t *ptRtc, csi_rtc_int_e eIntSrc)
{
	csi_vic_enable_irq(RTC_IRQn);
	csp_rtc_clr_isr(ptRtc, (rtc_int_e)eIntSrc);
	csp_rtc_int_enable(ptRtc, (rtc_int_e)eIntSrc);
}

/**\brief   RTC interrupt disable
 * 
 * \param   ptRtc      handle pointer of rtc register structure to operate
 * \param   eIntSrc	  interrupt source	
 * \return  none
*/
void csi_rtc_int_disable(csp_rtc_t *ptRtc, csi_rtc_int_e eIntSrc)
{
	csp_rtc_int_disable(ptRtc, (rtc_int_e)eIntSrc);
	if (eIntSrc == csp_rtc_get_imcr(ptRtc)) {
		csi_vic_disable_irq(RTC_IRQn);
	}
}

/**\brief   RTC interrupt enable
 * 
 * \param   ptRtc      handle pointer of rtc register structure to operate
 * \param   eIntSrc	  interrupt source	
 * \return  none
*/
void csi_rtc_clr_isr(csp_rtc_t *ptRtc, csi_rtc_int_e eIntSrc)
{
	csp_rtc_clr_isr(ptRtc, (rtc_int_e)eIntSrc);
	
}
/**\brief Get system time
 * 
 * \param[in]   rtc        handle pointer of rtc register structure to operate
 * \param[out]  rtctime    pointer to rtc time
 * \return      none
*/
void csi_rtc_get_time(csp_rtc_t *ptRtc, csi_rtc_time_t *rtctime)
{
	csi_rtc_rb_enable(ptRtc); 
	rtctime->iPm = csp_rtc_read_pm(ptRtc);
	rtctime->iSec = csp_rtc_read_sec(ptRtc);
	rtctime->iMin = csp_rtc_read_min(ptRtc);
	rtctime->iHour = csp_rtc_read_hour(ptRtc);
	rtctime->iMday = csp_rtc_read_mday(ptRtc);
	rtctime->iWday = csp_rtc_read_wday(ptRtc);
	rtctime->iMon = csp_rtc_read_mon(ptRtc);
	rtctime->iYear = csp_rtc_read_year(ptRtc);
	csi_rtc_rb_disable(ptRtc);
}

/**\brief       Get alarm remaining time
 * 
 * \param[in]   rtc    pointer of rtc register structure to operate
 * \param	   byAlm  RTC_ALMA/RTC_ALMB
 * \return      the remaining time(s)
*/
uint32_t csi_rtc_get_alarm_remaining_time(csp_rtc_t *ptRtc, uint8_t byAlm)
{
    volatile csi_rtc_time_t tCurrentTime ;
	volatile csi_rtc_time_t tAlmTime;
	uint32_t wCurrentTime, wAlmTime;

	tCurrentTime.iMday = csp_rtc_read_mday(ptRtc); 
	tCurrentTime.iWday = csp_rtc_read_wday(ptRtc); 
	tCurrentTime.iHour = csp_rtc_read_hour(ptRtc);
	tCurrentTime.iMin = csp_rtc_read_min(ptRtc); 
	tCurrentTime.iSec = csp_rtc_read_sec(ptRtc); 
	
	if(byAlm == RTC_ALMA)
	{
		tAlmTime.iMday = csp_rtc_alma_read_mday(ptRtc); 
		tAlmTime.iWday = csp_rtc_alma_read_wday(ptRtc); 
		tAlmTime.iHour = csp_rtc_alma_read_hour(ptRtc);
		tAlmTime.iMin = csp_rtc_alma_read_min(ptRtc); 
		tAlmTime.iSec = csp_rtc_alma_read_sec(ptRtc); 
		
		if (csp_rtc_alma_read_dmsk(ptRtc) == 1) {
		wCurrentTime = tCurrentTime.iHour * 3600 + tCurrentTime.iMin * 60 + tCurrentTime.iSec;
		wAlmTime = tAlmTime.iHour * 3600 + tAlmTime.iMin * 60 + tAlmTime.iSec;
		if(wAlmTime < wCurrentTime)
			return (24*3600 - wCurrentTime + wAlmTime);
	
		}
		else {
			if (csp_rtc_alma_read_wdsel(ptRtc) == 1) {
				wCurrentTime = tCurrentTime.iWday * 86400 + tCurrentTime.iHour * 3600 + tCurrentTime.iMin * 60 + tCurrentTime.iSec;
				wAlmTime = tAlmTime.iWday * 86400 + tAlmTime.iHour * 3600 + tAlmTime.iMin * 60 + tAlmTime.iSec;
				if(wAlmTime < wCurrentTime)
					return (7*24*3600 - wCurrentTime + wAlmTime);
			}
			else {
				wCurrentTime = tCurrentTime.iMday * 86400 + tCurrentTime.iHour * 3600 + tCurrentTime.iMin * 60 + tCurrentTime.iSec;
				wAlmTime = tAlmTime.iMday * 86400 + tAlmTime.iHour * 3600 + tAlmTime.iMin * 60 + tAlmTime.iSec;
				if(wAlmTime < wCurrentTime)
					return CSI_UNSUPPORTED;	
			}
		
		}
	}
	else 
	{
		tAlmTime.iMday = csp_rtc_almb_read_mday(ptRtc); 
		tAlmTime.iWday = csp_rtc_almb_read_wday(ptRtc); 
		tAlmTime.iHour = csp_rtc_almb_read_hour(ptRtc);
		tAlmTime.iMin = csp_rtc_almb_read_min(ptRtc); 
		tAlmTime.iSec = csp_rtc_almb_read_sec(ptRtc); 
		
		if (csp_rtc_almb_read_dmsk(ptRtc) == 1) {
		wCurrentTime = tCurrentTime.iHour * 3600 + tCurrentTime.iMin * 60 + tCurrentTime.iSec;
		wAlmTime = tAlmTime.iHour * 3600 + tAlmTime.iMin * 60 + tAlmTime.iSec;
		if(wAlmTime < wCurrentTime)
			return (24*3600 - wCurrentTime + wAlmTime);
	
		}
		else {
			if (csp_rtc_almb_read_wdsel(ptRtc) == 1) {
				wCurrentTime = tCurrentTime.iWday * 86400 + tCurrentTime.iHour * 3600 + tCurrentTime.iMin * 60 + tCurrentTime.iSec;
				wAlmTime = tAlmTime.iWday * 86400 + tAlmTime.iHour * 3600 + tAlmTime.iMin * 60 + tAlmTime.iSec;
				if(wAlmTime < wCurrentTime)
					return (7*24*3600 - wCurrentTime + wAlmTime);
			}
			else {
				wCurrentTime = tCurrentTime.iMday * 86400 + tCurrentTime.iHour * 3600 + tCurrentTime.iMin * 60 + tCurrentTime.iSec;
				wAlmTime = tAlmTime.iMday * 86400 + tAlmTime.iHour * 3600 + tAlmTime.iMin * 60 + tAlmTime.iSec;
				if(wAlmTime < wCurrentTime)
					return CSI_UNSUPPORTED;	
			}
		
		}
	}
	
	return (wAlmTime - wCurrentTime);
}


/** \brief rtc event trigger source output config  
 * 
 *  \param[in] ptRtc: RTC handle to operate
 *  \param[in] eTrg: rtc evtrgout channel \ref csi_rtc_trgout_e
 *  \param[in] eTrgSrc: rtc evtrg source  \ref csi_rtc_trgsrc_e
 *  \param[in] trg_prd: event count period 
 *  \return none
 */
void csi_rtc_set_evtrg(csp_rtc_t *ptRtc, csi_rtc_trgout_e eTrg, csi_rtc_trgsrc_e eTrgSrc, uint8_t byTrgPrd)
{
	
	csp_rtc_set_evtrg(ptRtc,(rtc_trgsrc_e)eTrgSrc,(rtc_trgout_e)eTrg);		//set trigger source and trigger out
	
	csp_rtc_set_prd(ptRtc,(rtc_trgout_e)eTrg,byTrgPrd);	//set trigger period
	
//	csp_rtc_evtrg_enable(ptRtc,(rtc_trgout_e)eTrg);				//enable event trigger
}


/** \brief rtc event trigger enable  
 * 
 *  \param[in] ptRtc: RTC handle to operate
 *  \param[in] eTrg: rtc evtrgout channel \ref csi_rtc_trgout_e
 *  \return none
 */
void csi_rtc_evtrg_enable(csp_rtc_t *ptRtc, csi_rtc_trgout_e eTrg)
{
	
	csp_rtc_evtrg_enable(ptRtc,(rtc_trgout_e)eTrg);
}

/** \brief  register rtc interrupt callback function
 * 
 *  \param[in] ptRtcBase: pointer of rtc register structure
 *  \param[in] callback: rtc interrupt handle function
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_rtc_register_callback(csp_rtc_t *ptRtcBase, void  *callback)
{
	uint8_t byIdx = apt_get_rtc_idx(ptRtcBase);
	
	if(byIdx == 0xff)
		return CSI_ERROR;
		
	g_tRtcCtrl[byIdx].callback = callback;
	
	return CSI_OK;
}



//*****************************************************************************//

/** \brief get rtc number 
 * 
 *  \param[in] ptRtcBase: pointer of rtc register structure
 *  \return rtc number 0
 */ 
static uint8_t apt_get_rtc_idx(csp_rtc_t *ptRtcBase)
{
	switch((uint32_t)ptRtcBase)
	{
		case APB_RTC_BASE:		//rtc
			return 0;		

		default:
			return 0xff;		//error
	}
}

uint8_t  apt_dec2bcd(uint32_t wData)
{	
	return (((wData / 10) << 4) | (wData % 10));
}


static csp_error_t apt_rtc_set_date(csp_rtc_t *ptRtc, uint8_t byYear, uint8_t byMon, uint8_t byWday, uint8_t byDay)
{
	uint8_t byVal;
	csp_error_t ret = CSP_SUCCESS;
	if (csp_rtc_is_running(ptRtc))
		return CSP_FAIL;
		
	csp_rtc_set_key(ptRtc);
	byVal = apt_dec2bcd(byYear);
	csp_rtc_set_year(ptRtc, byVal);
	byVal = apt_dec2bcd(byMon);
	csp_rtc_set_mon(ptRtc, byVal);
	byVal = apt_dec2bcd(byWday);
	csp_rtc_set_wday(ptRtc, byVal);
	byVal = apt_dec2bcd(byDay);
	csp_rtc_set_day(ptRtc, byVal);	
	csp_rtc_clr_key(ptRtc);
	
	return ret;
}


static csp_error_t apt_rtc_set_time(csp_rtc_t *ptRtc, bool bPm, uint8_t byHor, uint8_t byMin,uint8_t bySec)
{
	uint8_t byVal;
	csp_error_t ret = CSP_SUCCESS;

	if (csp_rtc_is_running(ptRtc))
		return CSP_FAIL;
			
	csp_rtc_set_key(ptRtc);
	byVal = apt_dec2bcd(byHor);
	csp_rtc_set_hour(ptRtc, bPm, byVal);
	byVal = apt_dec2bcd(byMin);
	csp_rtc_set_min(ptRtc, byVal);
	byVal = apt_dec2bcd(bySec);
	csp_rtc_set_sec(ptRtc, byVal);
	csp_rtc_clr_key(ptRtc);

	
	return ret;
}


static void apt_rtc_alm_set_time(csp_rtc_t *ptRtc, uint8_t byAlm, uint8_t byDay, bool byPm, uint8_t byHor, uint8_t byMin,uint8_t bySec)
{
	uint8_t byVal;
	if(byAlm == RTC_ALMA)	
	{
		csp_rtc_set_key(ptRtc);
		
		byVal = apt_dec2bcd(byDay);
		csp_rtc_alma_set_day(ptRtc,  byVal);
		byVal = apt_dec2bcd(byHor);
		csp_rtc_alma_set_hour(ptRtc, byPm, byVal);
		byVal = apt_dec2bcd(byMin);
		csp_rtc_alma_set_min(ptRtc, byVal);
		byVal = apt_dec2bcd(bySec);
		csp_rtc_alma_set_sec(ptRtc, byVal);
	
		csp_rtc_clr_key(ptRtc);
	}
	else 
	{
		csp_rtc_set_key(ptRtc);
		
		byVal = apt_dec2bcd(byDay);
		csp_rtc_almb_set_day(ptRtc,  byVal);
		byVal = apt_dec2bcd(byHor);
		csp_rtc_almb_set_hour(ptRtc, byPm, byVal);
		byVal = apt_dec2bcd(byMin);
		csp_rtc_almb_set_min(ptRtc,  byVal);
		byVal = apt_dec2bcd(bySec);
		csp_rtc_almb_set_sec(ptRtc,  byVal);
	
		csp_rtc_clr_key(ptRtc);
	}
}




//csp_error_t apt_rtc_set_trgsrc(csp_rtc_t *ptRtc, csi_rtc_trgout_e eTrg, csi_rtc_trgsrc_e eSrc)
//{
//	if(eTrg == RTC_TRGOUT0)
//		ptRtc -> EVTRG = (ptRtc->EVTRG & ~(RTC_TRGSEL0_MSK)) | eSrc | RTC_TRG0OE;
//	else if(eTrg == RTC_TRGOUT1)
//		ptRtc -> EVTRG = (ptRtc->EVTRG & ~(RTC_TRGSEL1_MSK)) | (eSrc << RTC_TRGSEL1_POS) | RTC_TRG1OE;
//	else
//		return CSP_FAIL;
//	return CSP_SUCCESS;
//}
//
//
//csp_error_t apt_rtc_set_trgprd(csp_rtc_t *ptRtc, csi_rtc_trgout_e eTrg, uint8_t byPrd)
//{	
//	if(eTrg == RTC_TRGOUT0)
//		ptRtc -> EVPS = (ptRtc->EVPS & ~(RTC_TRGEV0PRD_MSK)) | byPrd ;
//	else if(eTrg == RTC_TRGOUT1)
//		ptRtc -> EVPS = (ptRtc->EVPS & ~(RTC_TRGSEL1_MSK)) | (byPrd << RTC_TRGEV1PRD_POS);
//	else
//		return CSP_FAIL;
//		
//	return CSP_SUCCESS;
//}

