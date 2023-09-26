/***********************************************************************//** 
 * \file  lpt.c
 * \brief  csi lpt driver
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-9-28 <td>V0.0  <td>ZJY   <td>initial
 * <tr><td> 2021-1-8  <td>V0.1  <td>WNN   <td>modify
 * <tr><td> 2021-5-20 <td>V0.2  <td>YYM   <td>modify
 * </table>
 * *********************************************************************
*/
#include "drv/lpt.h"
#include "board_config.h"

/* Private macro-----------------------------------------------------------*/
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/
/* Global variablesr------------------------------------------------------*/
csi_lpt_ctrl_t g_tLptCtrl[LPT_IDX];

int32_t g_wLptPrd = 0;

/** \brief get lpt clk
 * 
 *  \param[in] eClk: clk source selection
 *  \return lpt clk
 */ 
static uint32_t apt_get_lpt_clk(csi_lpt_clksrc_e eClk)
{
	uint32_t wLptClk =0;
	switch (eClk)
	{
		case (LPT_CLK_PCLK_DIV4): 
			wLptClk =  csi_get_pclk_freq()>> 2; 
			break;
		case (LPT_CLK_ISCLK): 
			csi_isosc_enable();
			wLptClk = ISOSC_VALUE;
			break;
		case (LPT_CLK_IMCLK_DIV4):  
			switch (csp_get_imosc_freq(SYSCON))
			{
				case (0): wLptClk = IMOSC_5M_VALUE >> 2;
					break;
				case (1): wLptClk = IMOSC_4M_VALUE >> 2;
					break;
				case (2): wLptClk = IMOSC_2M_VALUE >> 2;
					break;
				case (3): wLptClk = IMOSC_131K_VALUE >> 2;
					break;
				default:
					break;
			}
			break;
		case (LPT_CLK_EMCLK): 
			csi_emosc_enable(EMOSC_VALUE);
			wLptClk = EMOSC_VALUE;
			break;
		case (LPT_CLK_ESCLK): 
			csi_esosc_enable();
			wLptClk = ESOSC_VALUE;
			break;

		default: break;
	}	
	return wLptClk;
}

/** \brief set lpt clk and get lpt prd
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \param[in] eClk: clk source selection
 *  \param[in] wMult_t: converted wMult value 
 *  \return lpt prd
 */ 
static int32_t apt_set_lpt_clk(csp_lpt_t *ptLptBase,csi_lpt_clksrc_e eClk,uint32_t wMult_t)
{
	uint32_t hg_wLptPrd,wDiv = 0;
	lpt_pscdiv_e ePscDiv;
	
	if (wMult_t < 0x10000) {
		ePscDiv = LPT_PSC_DIV1;
	}
	else if (wMult_t < 0x20000) {
		ePscDiv = LPT_PSC_DIV2;
	}
	else if (wMult_t < 0x40000) {
		ePscDiv = LPT_PSC_DIV4;
	}
	else if (wMult_t < 0x80000){
		ePscDiv = LPT_PSC_DIV8;
	}
	else if (wMult_t < 0x100000) {
		ePscDiv = LPT_PSC_DIV16;
	}
	else if (wMult_t < 0x200000) {
		ePscDiv = LPT_PSC_DIV32;
	}
	else if (wMult_t < 0x400000) {
		ePscDiv = LPT_PSC_DIV64;
	}
	else if (wMult_t < 0x800000){
		ePscDiv = LPT_PSC_DIV128;
	}
	else if (wMult_t < 0x1000000){
		ePscDiv = LPT_PSC_DIV256;
	}
	else if (wMult_t < 0x2000000){
		ePscDiv = LPT_PSC_DIV512;
	}
	else if (wMult_t < 0x4000000){
		ePscDiv = LPT_PSC_DIV1024;
	}
	else if (wMult_t < 0x8000000){
		ePscDiv = LPT_PSC_DIV2048;
	}
	else if (wMult_t < 0x10000000){
		ePscDiv = LPT_PSC_DIV4096;
	}
	else
	{
		return CSI_UNSUPPORTED;	 		
	}
	wDiv = 1<<ePscDiv;
	csp_lpt_set_clk(ptLptBase, (lpt_css_e) eClk, ePscDiv);
	hg_wLptPrd = wMult_t/wDiv;
	return hg_wLptPrd;	
}

/**
  \brief       LPT interrupt enable control
  \param[in]   ptLptBase:pointer of lpt register structure
  \param[in]   eLptInt:irq mode
*/
void csi_lpt_int_enable(csp_lpt_t *ptLptBase, csi_lpt_intsrc_e eLptInt)
{
	csp_lpt_clr_isr(ptLptBase, (lpt_int_e)eLptInt);
	csp_lpt_int_enable(ptLptBase,(lpt_int_e)eLptInt);
}

/**
  \brief       LPT interrupt disable control
  \param[in]   ptLptBase:pointer of lpt register structure
  \param[in]   eLptInt:irq mode
*/
void csi_lpt_int_disable(csp_lpt_t *ptLptBase, csi_lpt_intsrc_e eLptInt)
{

	csp_lpt_int_disable(ptLptBase, (lpt_int_e)eLptInt);
}

/** \brief initialize lpt data structure
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \param[in] ptLptTimCfg: ptLptTimCfg: pointer of lpt timing parameter config structure
 *              - wTimeVal: timing value, unit: us
 *              - eWkMode: lpt count work mode, \ref csi_lpt_wkmode_e
 *              - eClksrc: Clock Source, \ref csi_lpt_clksrc_e
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_lpt_timer_init(csp_lpt_t *ptLptBase,csi_lpt_time_config_t *ptLptTimCfg)
{

	uint32_t wLptClk =0, wMult = 0;
	csi_error_t tRet = CSI_OK;
	
	csi_clk_enable((uint32_t *)ptLptBase);
	csp_lpt_sw_rst(ptLptBase);
	csp_lpt_clk_enable(ptLptBase);
	
	csp_lpt_set_opmd(ptLptBase, (lpt_opm_e) ptLptTimCfg->eRunMode);
	
	wLptClk = apt_get_lpt_clk(ptLptTimCfg->eClksrc);
//	wLptClk = 2000000;   // If using an external clock as an LPT clock source, open this line of code and mask the previous line of code
	wMult = wLptClk/1000*ptLptTimCfg->wTimeVal;
		
	g_wLptPrd = apt_set_lpt_clk(ptLptBase,ptLptTimCfg->eClksrc,wMult);
	if(g_wLptPrd == CSI_UNSUPPORTED)
	{
		tRet = CSI_UNSUPPORTED;
	}
	else
	{
		csp_lpt_set_prdr(ptLptBase, (uint16_t)g_wLptPrd);
	}
	csi_lpt_int_enable(ptLptBase,LPT_INTSRC_PEND|LPT_INTSRC_TRGEV0);	 //enable PEND interrupt  
	
	return tRet;	
}

/** \brief stop lpt
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \return error code \ref csi_error_t
 */ 
void csi_lpt_stop(csp_lpt_t *ptLptBase)
{
	
    csp_lpt_stop(ptLptBase);
}

/** \brief get lpt remaining value
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \return none
 */ 
uint32_t csi_lpt_get_remaining_value(csp_lpt_t *ptLptBase)
{
	uint16_t hwTimeVal = 0;
	
    hwTimeVal = csp_lpt_get_prdr(ptLptBase) - csp_lpt_get_cnt(ptLptBase);
	
	return hwTimeVal;
}

/** \brief get lpt load value
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \return none
 */ 
uint32_t csi_lpt_get_load_value(csp_lpt_t *ptLptBase)
{
	return csp_lpt_get_prdr(ptLptBase);
}

/** \brief get lpt working status
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \return rue: running; false: stopped
 */ 
bool csi_lpt_is_running(csp_lpt_t *ptLptBase)
{
    return (csp_lpt_get_work_state(ptLptBase) ? true : false);
}

/** \brief update lpt pwm freq para: (prdr and cmp value)
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \param[in] hwCmp: duty cycle
 *  \param[in] hwPrdr: period 
 *  \param[in] eModeUpdata: update mode 
 *  \return none
 */
void csi_lpt_pwm_para_update(csp_lpt_t *ptLptBase, uint16_t hwCmp, uint16_t hwPrdr, csi_lpt_update_e eModeUpdata)
{
	
	csp_lpt_data_update(ptLptBase, hwCmp, hwPrdr);
	if(eModeUpdata == UPDATA_IM)
	{
		csp_lpt_set_prdr(ptLptBase, hwPrdr);
		csp_lpt_set_cmp(ptLptBase, hwCmp);
	}
}

/** \brief restart lpt sync 
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \param[in] bySync: sync select
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_lpt_rearm_sync(csp_lpt_t *ptLptBase, csi_lpt_syncin_e eSync)
{
    csi_error_t ret = CSI_OK;
	if(eSync > 0)
		return CSI_ERROR;
	csp_lpt_sync_rearm(ptLptBase);
	
	return ret;
}

/** \brief lpt evtrg source output config  
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \param[in] eTrgsrc: lpt evtrg source(1~4) 
 *  \param[in] byTrgprd: event count period 
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_lpt_set_evtrg(csp_lpt_t *ptLptBase, uint8_t byEvtrg, csi_lpt_trgsrc_e eTrgsrc, uint8_t byTrgprd)
{
	csi_error_t ret = CSI_OK;
	if(byEvtrg > 0)
		return CSI_ERROR;
	
	csp_lpt_set_evtrg(ptLptBase,(lpt_trgsrc_e) eTrgsrc);
	csp_lpt_set_trgprd(ptLptBase, byTrgprd-1);
	
	return ret;
}

/** \brief LPT trigger enable control
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \return void
 */
void csi_lpt_trg_enable(csp_lpt_t *ptLptBase)
{
	csp_lpt_evtrg_enable(ptLptBase);
}

/** \brief LPT trigger enable control
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \return void
 */
void csi_lpt_trg_disable(csp_lpt_t *ptLptBase)
{
	csp_lpt_evtrg_disable(ptLptBase);
}

/** \brief lpt set frequency 
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \param[in] eClk: lpt clock source selection
 *  \param[in] hwHz: frequency
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_lpt_set_fre(csp_lpt_t *ptLptBase, csi_lpt_clksrc_e eClk, uint16_t hwHz)
{

	uint32_t wLptClk =0, wMult = 0;
	csi_error_t tRet = CSI_OK;
	
	wLptClk = apt_get_lpt_clk(eClk);
	wMult = wLptClk/hwHz;
	
	g_wLptPrd = apt_set_lpt_clk(ptLptBase,eClk,wMult);	

	if(g_wLptPrd == ERR_LPT_CLK)
	{
		tRet = CSI_UNSUPPORTED;
	}	
	else
	{
		csp_lpt_set_prdr(ptLptBase, (uint16_t)g_wLptPrd);	
	}
		
	return tRet;
}

/** \brief lpt pwm init
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \param[in] ptLptPara: pointer of lpt parameter structure
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_lpt_pwm_init(csp_lpt_t *ptLptBase, csi_lpt_pwm_config_t *ptLptPara)
{
	csi_error_t tRet = CSI_OK;
	uint32_t wLptClk = 0,wMult = 0,hwCmp = 0;
	
	csi_clk_enable((uint32_t *)ptLptBase);
	csp_lpt_sw_rst(ptLptBase);
	csp_lpt_clk_enable(ptLptBase);
	
	csp_lpt_set_opmd(ptLptBase, LPT_OPM_CONT);
	csp_lpt_stopshadow_enable(ptLptBase);
	csp_lpt_set_pol(ptLptBase, ptLptPara->eStartPol);
	csp_lpt_set_idle_st(ptLptBase, ptLptPara->eIdlePol);
	csp_lpt_out_enable(ptLptBase);

	
	if(ptLptPara->wFreq== 0 )
		return CSI_ERROR;
		
	wLptClk = apt_get_lpt_clk(ptLptPara->eClkSrc);

	wMult = wLptClk/ptLptPara->wFreq;
	
	g_wLptPrd = apt_set_lpt_clk(ptLptBase,ptLptPara->eClkSrc,wMult);	
	
	if(g_wLptPrd == ERR_LPT_CLK)
	{
		tRet = CSI_UNSUPPORTED;
	}
	else
	{
		csp_lpt_set_prdr(ptLptBase, (uint16_t)g_wLptPrd);
		
		if(ptLptPara->byDutyCycle >= 100)
		{
			hwCmp = g_wLptPrd + 1;
		}
		else
		{
			hwCmp = g_wLptPrd*ptLptPara->byDutyCycle/100;		
		}
		csp_lpt_set_cmp(ptLptBase, (uint16_t)hwCmp);
		
	}
	return tRet;	
		
}

/** \brief start lpt pwm by external sync
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \param[in] ptLptPara: pointer of lpt parameter structure
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_lpt_pwm_start_sync(csp_lpt_t *ptLptBase, csi_lpt_pwm_config_t *ptLptPara) 
{
	csi_error_t tRet = CSI_OK;
	uint32_t wLptClk = 0,wMult = 0 ,hwCmp = 0;
	
	if(ptLptPara->wFreq == 0 )
		return CSI_ERROR;
	
	csi_clk_enable((uint32_t *)ptLptBase);
	csp_lpt_sw_rst(ptLptBase);
	csp_lpt_clk_enable(ptLptBase);
	
	wLptClk = apt_get_lpt_clk(ptLptPara->eClkSrc);
	wMult = wLptClk/ptLptPara->wFreq;
	g_wLptPrd = apt_set_lpt_clk(ptLptBase,ptLptPara->eClkSrc,wMult);	
	if(g_wLptPrd == ERR_LPT_CLK)
	{
		tRet = CSI_UNSUPPORTED;
	}
	else
	{
		csp_lpt_set_prdr(ptLptBase, (uint16_t)g_wLptPrd);
		
		if(ptLptPara->byDutyCycle >= 100)
		{
			hwCmp = g_wLptPrd + 1;
		}
		else
		{
			hwCmp = g_wLptPrd*ptLptPara->byDutyCycle/100;		
		}
		csp_lpt_prdr_ldmode(ptLptBase, LPT_PRDLD_IM);
		csp_lpt_cmp_ldmode(ptLptBase, LPT_CMPLD_IM);
		csp_lpt_set_prdr(ptLptBase, (uint16_t)g_wLptPrd);
		csp_lpt_set_cmp(ptLptBase, (uint16_t)hwCmp);
		
		csp_lpt_immediate_start(ptLptBase, ENABLE);
		
	}
	return tRet;	

}


/** \brief change lpt duty cycle
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \param[in] wDutyCycle: lpt clock source selection
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_lpt_change_duty(csp_lpt_t *ptLptBase, uint32_t wDutyCycle)
{
	uint16_t hwCmp;
	
	if(wDutyCycle >= 100)
	{
		hwCmp = g_wLptPrd + 1;
	}
	else
	{
		hwCmp = g_wLptPrd*wDutyCycle/100;		
	}
	csp_lpt_set_cmp(ptLptBase, hwCmp);
	return CSI_OK;
}

/** \brief lpt start
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_lpt_start(csp_lpt_t *ptLptBase)
{
	csp_lpt_sw_start(ptLptBase);
	csp_lpt_immediate_start(ptLptBase, ENABLE);
	csp_lpt_start_mode(ptLptBase,LPT_SYNC);
	return CSI_OK;
}

/** \brief lpt work as a timer(sync start)
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \param[in] eClk: lpt clock source selection
 *  \param[in] wTimeMs: ms
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_lpt_start_sync(csp_lpt_t *ptLptBase, csi_lpt_clksrc_e eClk, uint32_t wTimeMs)
{
	uint32_t wLptClk =0, wMult = 0;
	csi_error_t tRet = CSI_OK;
	
	csi_clk_enable((uint32_t *)ptLptBase);
	csp_lpt_sw_rst(ptLptBase);
	csp_lpt_clk_enable(ptLptBase);	
	
	wLptClk = apt_get_lpt_clk(eClk);
	wMult = wLptClk/1000*wTimeMs;

	g_wLptPrd = apt_set_lpt_clk(ptLptBase,eClk,wMult);	

	if(g_wLptPrd == ERR_LPT_CLK)
	{
		tRet = CSI_UNSUPPORTED;
	}
	else
	{	
		csp_lpt_prdr_ldmode(ptLptBase, LPT_PRDLD_IM);
		csp_lpt_set_prdr(ptLptBase, (uint16_t)g_wLptPrd);
	}
	csi_lpt_int_enable(ptLptBase,LPT_INTSRC_PEND);	 //enable PEND interrupt
	return tRet;
}

/** \brief lpt sync config  
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \param[in] eSync: select sync ÓÃÃ¶¾Ù
 *  \param[in] eSyncMode: LPT_TRG_CONT/LPT_TRG_ONCE
 *  \param[in] bARearmEnable: auto rearm enable/disable
 *  \return error code \ref csi_error_t
 */
 
csi_error_t csi_lpt_set_sync(csp_lpt_t *ptLptBase, csi_lpt_syncin_e eSync, csi_lpt_syncmode_e eSyncMode, bool bARearmEnable)
{
	if(eSync > 0)
		return CSI_ERROR;		
	csp_lpt_set_sync_mode(ptLptBase, (lpt_syncmd_e)eSyncMode);
	if(bARearmEnable==1)
		csp_lpt_autorearm_enable(ptLptBase);
	else
		csp_lpt_autorearm_enable(ptLptBase);
	return CSI_OK;
}

/** \brief LPT ync enable control
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \return void
 */
void csi_lpt_sync_enable(csp_lpt_t *ptLptBase)
{
	csp_lpt_sync_enable(ptLptBase);
}

/** \brief LPT ync enable control
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \return void
 */
void csi_lpt_sync_disable(csp_lpt_t *ptLptBase)
{
	csp_lpt_sync_enable(ptLptBase);
}



/** \brief lpt software generates a trigger event
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \return none
 */
void csi_lpt_sw_evtrg(csp_lpt_t *ptLptBase)
{
	csi_clk_enable((uint32_t *)ptLptBase);										
	csp_lpt_evtrg_enable(ptLptBase);
	csp_lpt_sw_evtrg(ptLptBase);
}


/** \brief lpt sync window config  
 * 
 *  \param[in] lpt: LPT handle to operate
 *  \param bCrossEnable: window cross enable/disable
 *  \param bInvEnable: window invert enable/disable
 *  \param hwOffset: window start point from CNT=ZRO, us
 *  \param hwWindow: window width, us
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_lpt_set_sync_window(csp_lpt_t *ptLptBase, bool bCrossEnable, bool bInvEnable, uint16_t hwOffset, uint16_t hwWindow)
{
	uint32_t wOffset, wWindow;
	
	wOffset = (long long )csi_get_pclk_freq() * hwOffset / 1000000;
	wWindow = (long long )csi_get_pclk_freq() * hwWindow / 1000000;
	
	if((wWindow > 0xffff) || (wOffset > 0xffff))
		return CSI_ERROR;
		
	csp_lpt_sync_window_cross_enable(ptLptBase, bCrossEnable);
	csp_lpt_sync_window_inv_enable(ptLptBase, bInvEnable);
	
	csp_lpt_set_sync_offset(ptLptBase, (uint16_t)wOffset);
	csp_lpt_set_sync_window(ptLptBase, (uint16_t)wWindow);	
	csp_lpt_sync_window_enable(ptLptBase);
	
	return CSI_OK;


}

/** \brief get lpt number 
 * 
 *  \param[in] ptLptBase: pointer of lpt register structure
 *  \return lpt 0/error
 */ 
static uint8_t apt_get_lpt_idx(csp_lpt_t *ptLptBase)
{
	switch((uint32_t)ptLptBase)
	{
		case APB_LPT_BASE:		//LPT
			return 0;		
		default:
			return 0xff;		//error
	}
}


/** \brief  register lpt interrupt callback function
 * 
 *  \param[in] ptLptBase: pointer of lpt register structure
 *  \param[in] callback: bt interrupt handle function
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_lpt_register_callback(csp_lpt_t *ptLptBase, void  *callback)
{
	uint8_t byIdx = apt_get_lpt_idx(ptLptBase);
	if(byIdx == 0xff)
		return CSI_ERROR;
		
	g_tLptCtrl[byIdx].callback = callback;
	
	return CSI_OK;
}

/** \brief lpt interrupt handler function
 * 
 *  \param[in] ptLptBase: pointer of bt register structure
 *  \param[in] byIdx: lpt idx(0)
 *  \return none
 */ 
void csi_lpt_irqhandler(csp_lpt_t *ptLptBase, uint8_t byIdx)
{
	uint8_t byIsr = csp_lpt_get_isr(ptLptBase);
	
	if(g_tLptCtrl[byIdx].callback)
		g_tLptCtrl[byIdx].callback(ptLptBase, byIsr);
			
	csp_lpt_clr_isr(ptLptBase, byIsr);
}

/** \brief clear lpt interrupt 
 * 
 *  \param[in] ptLptBase: pointer of lpt register structure
 *  \param[in] eIntSrc: lpt interrupt source
 *  \return none
 */ 
void csi_lpt_clr_isr(csp_lpt_t *ptLptBase, csi_lpt_intsrc_e eIntSrc)
{
	csp_lpt_clr_isr(ptLptBase, (lpt_int_e)eIntSrc);	
}

/** \brief enable lpt in debug mode
 * 
 *  \param[in]  ptLptBase      pointer of gpta register structure
 *  \return none
 */
void csi_lpt_debug_enable(csp_lpt_t *ptLptBase)
{
	csp_lpt_debug_enable(ptLptBase);
}

/** \brief disable lpt in debug mode
 * 
 *  \param[in]  ptLptBase      pointer of gpta register structure
 *  \return none
 */
void csi_lpt_debug_disable(csp_lpt_t *ptLptBase)
{
	csp_lpt_debug_disable(ptLptBase);
}