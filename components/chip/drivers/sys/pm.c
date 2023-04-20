/***********************************************************************//** 
 * \file  pm_102.c
 * \brief  power management
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-10 <td>V0.0  <td>WNN   <td>initial for csi
 * </table>
 * *********************************************************************
*/

#include <drv/common.h>
#include <csp_syscon.h>
#include <soc.h>
#include "pm.h"

#ifdef CONFIG_USER_PM
/// to make user defined prepaare_to_stop() and wkup_frm_stop() possible
pm_core_t g_tPmCore;

/**
  \brief       to attach user defined functions, 
   * to use in case user wants to preserve the scene in lp mode 
  \param	eMd: low power mode
  \param   pBeforeSlp: funtion to be called before lp
  \param   pBeforeSlp: funtion to be called after wakeup 
  \return      None.
*/
void soc_pm_attach_callback(csi_pm_mode_t eMd, void *pBeforeSlp, void *pWkup)
{
	switch(eMd)
	{
		case (PM_MODE_SLEEP):g_tPmCore.prepare_to_sleep = pBeforeSlp;
				g_tPmCore.wkup_frm_sleep = pWkup;
			break;
		case (PM_MODE_DEEPSLEEP):g_tPmCore.prepare_to_deepsleep = pBeforeSlp;
				g_tPmCore.wkup_frm_deepsleep = pWkup;
			break;
		default:
			break;
	}
	
}
#endif

static csi_error_t soc_sleep(csi_pm_mode_e mode)
{
	csi_error_t ret = CSI_OK;
	switch (mode)
	{
		case (PM_MODE_SLEEP):
			__DOZE();
			break;
		case (PM_MODE_DEEPSLEEP):
			__STOP();
			break;
		default:
			ret = CSI_ERROR;
			break;
	}
	return ret;
}
/**
  \brief       choose the pmu mode to enter
  \param[in]   handle  pmu handle to operate.
  \param[in]   mode    \ref pmu_mode_e
  \return      error code
*/
csi_error_t csi_pm_enter_sleep(csi_pm_mode_e mode)
{
	csp_syscon_t *ptSysconBase  = (csp_syscon_t*)APB_SYS_BASE;
	switch (mode)
	{
		case PM_MODE_SLEEP:		
			#ifdef CONFIG_USER_PM
			g_tPmCore.prepare_to_sleep();
			#endif
			ptSysconBase->PWRKEY = 0xA67A6CC7;
			ptSysconBase->PWRCR = (ptSysconBase->PWRCR & (~(0x0f<<16))) | 0x40002;
			soc_sleep(PM_MODE_SLEEP);	
			#ifdef CONFIG_USER_PM
			g_tPmCore.wkup_frm_sleep();		
			#endif
			break;
		case PM_MODE_DEEPSLEEP:
			#ifdef CONFIG_USER_PM
			g_tPmCore.prepare_to_deepsleep();
			#endif
			soc_sleep(PM_MODE_DEEPSLEEP);	
			#ifdef CONFIG_USER_PM
			g_tPmCore.wkup_frm_deepsleep();
			#endif
			break;
		default:
            return CSI_ERROR;
	}
	return CSI_OK;
}

/**
  \brief       Config the wakeup source.
  \param[in]   wakeup_num wakeup source num
  \param[in]   enable  flag control the wakeup source is enable or not
  \return      error code
*/
csi_error_t csi_pm_config_wakeup_source(csi_wakeup_src_e eWkupSrc, bool bEnable)
{
	csi_error_t ret = CSI_OK;
	csp_syscon_t *ptSysconBase  = (csp_syscon_t*)APB_SYS_BASE;
	
	if (bEnable){
		ptSysconBase->WKCR |= 0x1 << (eWkupSrc);
	}
	else{
		ptSysconBase->WKCR &= ~(0x1 << (eWkupSrc));
	}
	return ret;
}

/** \brief deep sleep mode, osc enable/disable.
 * 
 * \param[in] eSleepOsc: \ref csi_sleep_osc_e
 * \param[in] enable: enable/disable sleep/deepsleep osc
 * \return error code
 */
void csi_pm_clk_enable(csi_pm_clk_e eOsc, bool bEnable)
{
	csp_syscon_t *ptSysconBase  = (csp_syscon_t*)APB_SYS_BASE;
	
	if(bEnable)
	{
		ptSysconBase->GCER |= eOsc;
		while(!(csp_get_gcsr(ptSysconBase) & eOsc));
	}
	else
	{
		ptSysconBase->GCDR |= eOsc;
		while(csp_get_gcsr(ptSysconBase) & eOsc);
	}
}

/** \brief clear wkalv int status
 * 
 *  \param[in] byWkInt: WkInt Mask WKINT0~3 
 *  \return none
 */
void csi_pm_clr_wkint(uint8_t byWkInt)
{
	csp_syscon_t *ptSysconBase  = (csp_syscon_t*)APB_SYS_BASE;
	ptSysconBase->ICR = ((byWkInt & 0x0f) << 24);				//PA00/PB011/PA12/PB011
}
/** \brief clear wkalv int status
 * 
 *  \param[in] none
 *  \return WkInt Mask
 */
uint8_t csi_pm_get_wkint(void)
{
	csp_syscon_t *ptSysconBase  = (csp_syscon_t*)APB_SYS_BASE;
	return (uint8_t)((ptSysconBase->RISR >> 24) & 0x0f);
}