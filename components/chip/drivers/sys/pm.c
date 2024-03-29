/***********************************************************************//** 
 * \file  pm.c
 * \brief  power management
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-10 <td>V0.0  <td>WNN   <td>initial for csi
 * <tr><td> 2023-9-22 <td>V0.1  <td>GQQ   <td>fix bug,code normalization
 * </table>
 * *********************************************************************
*/


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

static csi_error_t csi_soc_sleep(csi_pm_mode_e mode)
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
  \param[in]   mode    \ref csi_pm_mode_e
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
//			ptSysconBase->PWRKEY = 0xA67A6CC7;
//			ptSysconBase->PWRCR = (ptSysconBase->PWRCR & (~(0x0f<<16))) | 0x40002;
			csp_sleep_mainreg_power_enable(ptSysconBase); //enable main reg power under sleep mode
			csp_sleep_vos_enable(ptSysconBase); //enable VOS under sleep mode
			
			csi_soc_sleep(PM_MODE_SLEEP);	
			#ifdef CONFIG_USER_PM
			g_tPmCore.wkup_frm_sleep();		
			#endif
			break;
		case PM_MODE_DEEPSLEEP:
			#ifdef CONFIG_USER_PM
			g_tPmCore.prepare_to_deepsleep();
			#endif
			csi_soc_sleep(PM_MODE_DEEPSLEEP);	
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
csi_error_t csi_pm_set_wakeup_src(csi_wakeup_src_e eWkupSrc, bool bEnable)
{
	csi_error_t ret = CSI_OK;
	csp_syscon_t *ptSysconBase  = (csp_syscon_t*)APB_SYS_BASE;
	
	if (bEnable){
		csp_wakeup_source_enable(ptSysconBase,(wakeup_src_e)eWkupSrc);
	}
	else{
		csp_wakeup_source_disable(ptSysconBase,(wakeup_src_e)eWkupSrc);
	}
	return ret;
}

/** \brief deep sleep mode, osc enable.
 * 
 * \param[in] eSleepOsc: \ref csi_pm_clk_e
 * \return error code
 */
void csi_pm_clk_enable(csi_pm_clk_e eOsc)
{
	csp_syscon_t *ptSysconBase  = (csp_syscon_t*)APB_SYS_BASE;
	
	csp_clk_pm_enable(ptSysconBase,(clk_pm_e)eOsc);
	while(!(csp_get_gcsr(ptSysconBase) & (0x1<<eOsc)));

}

/** \brief deep sleep mode, osc disable.
 * 
 * \param[in] eSleepOsc: \ref csi_pm_clk_e
 * \return error code
 */
void csi_pm_clk_disable(csi_pm_clk_e eOsc)
{
	csp_syscon_t *ptSysconBase  = (csp_syscon_t*)APB_SYS_BASE;

	csp_clk_pm_disable(ptSysconBase,(clk_pm_e)eOsc);
	while(csp_get_gcsr(ptSysconBase) & (0x1<<eOsc));

}
