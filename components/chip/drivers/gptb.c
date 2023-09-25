/***********************************************************************//** 
 * \file  gptb.c
 * \brief  GPTB driver
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-6-17 <td>V0.0  <td>ljy   <td>initial
 * <tr><td> 2023-9-18 <td>V0.1  <td>wch   <td>code normalization
 * </table>
 * *********************************************************************
*/
#include "drv/gptb.h"

/* private macro------------------------------------------------------*/
/* externs function---------------------------------------------------*/
/* global variablesr--------------------------------------------------*/
csi_gptb_ctrl_t g_tGptbCtrl[GPTB_IDX];	
/* private variablesr-------------------------------------------------*/	

/** \brief gptb interrupt handle  function
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] byIdx: gptb idx(0/1/2/3/4/5)
 *  \return    none
 */
void csi_gptb_irqhandler(csp_gptb_t *ptGptbBase, uint8_t byIdx)
{
	volatile uint32_t wIsr    = csp_gptb_get_isr(ptGptbBase);
	volatile uint16_t hwEmIsr = csp_gptb_get_emisr(ptGptbBase);	
	
	//GPTB interrupt / GPTB emergency interrupt
	if((wIsr > 0) || (hwEmIsr > 0))
	{
		if(g_tGptbCtrl[byIdx].callback)
			g_tGptbCtrl[byIdx].callback(ptGptbBase, wIsr, hwEmIsr);
			
		csp_gptb_clr_isr(ptGptbBase, wIsr);	
		csp_gptb_clr_emisr(ptGptbBase, hwEmIsr);
	}
}

/** \brief get gptb idx 
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \return gptb id number(0~5) or error(0xff)
 */ 
static uint8_t apt_get_gptb_idx(csp_gptb_t *ptGptbBase)
{
	switch((uint32_t)ptGptbBase)
	{
		case APB_GPTB0_BASE:
			return 0;
		case APB_GPTB1_BASE:
			return 1;
		case APB_GPTB2_BASE:
			return 2;
		case APB_GPTB3_BASE:
			return 3;
		case APB_GPTB4_BASE:
			return 4;
		case APB_GPTB5_BASE:
			return 5;
		default:
			return 0xff;		//error
	}
}

/** \brief  register gptb interrupt callback function
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] callback: gptb callback handle function
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_gptb_register_callback(csp_gptb_t *ptGptbBase, void  *callback)
{
	uint8_t byIdx = apt_get_gptb_idx(ptGptbBase);
	if(byIdx == 0xff)
		return CSI_ERROR;

	g_tGptbCtrl[byIdx].callback = callback;
	
	return CSI_OK;
}

/**
 \brief  gptb capture configuration
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptGptbCapCfg   \refer csi_gptb_capture_config_t
  * 		- eWorkMode:GPTB work mode \refer csi_gptb_workmode_e
  * 		- eCountMode:GPTB counting mode \refer csi_gptb_cntmd_e
  * 		- eRunMode:GPTB run mode \refer csi_gptb_runmode_e
  * 		- eCapMode:GPTB capture mode \refer csi_gptb_capmode_e  		
  * 		- byCapStopWrap:GPTB capture counts
  * 		- bCapLdaRst:CMPA load,counter value reset or not
  * 		- bCapLdbRst:CMPB load,counter value reset or not
  * 		- bCapLdaaRst:CMPAA load,counter value reset or not
  * 		- bCapLdbaRst:CMPBA load,counter value reset or not
 \return none
*/
void csi_gptb_capture_init(csp_gptb_t *ptGptbBase, csi_gptb_capture_config_t *ptGptbCapCfg)
{
	uint32_t wCrVal;

	csi_clk_enable((uint32_t *)ptGptbBase);	
	csp_gptb_clk_enable(ptGptbBase);
	csp_gptb_wr_key(ptGptbBase);	
	csp_gptb_sw_rst(ptGptbBase);

	wCrVal = (ptGptbCapCfg -> eCountMode) | (ptGptbCapCfg -> eRunMode << GPTB_CAPMD_POS) | (ptGptbCapCfg -> eWorkMode << GPTB_MODE_POS);
	wCrVal = (wCrVal & ~(0x01<<CAPMODE_SEL_POS))|((ptGptbCapCfg -> eCapMode & 0x01)  	<< CAPMODE_SEL_POS);
	
	wCrVal = (wCrVal & ~(GPTB_STOPWRAP_MSK)) | ((ptGptbCapCfg -> byCapStopWrap & 0x03)<< GPTB_STOPWRAP_POS);
	wCrVal = (wCrVal & ~(GPTB_CMPA_RST_MSK)) | ((ptGptbCapCfg -> bCapLdaRst & 0x01)   << GPTB_CMPA_RST_POS);
	wCrVal = (wCrVal & ~(GPTB_CMPB_RST_MSK)) | ((ptGptbCapCfg -> bCapLdbRst & 0x01)   << GPTB_CMPB_RST_POS);
	wCrVal = (wCrVal & ~(GPTB_CMPC_RST_MSK)) | ((ptGptbCapCfg -> bCapLdaaRst & 0x01)  << GPTB_CMPC_RST_POS);
	wCrVal = (wCrVal & ~(GPTB_CMPD_RST_MSK)) | ((ptGptbCapCfg -> bCapLdbaRst & 0x01)  << GPTB_CMPD_RST_POS);

	wCrVal |= GPTB_CAPLD_EN;
	wCrVal |= GPTB_CAPREARM;
	
	csp_gptb_clk_enable(ptGptbBase);
	csp_gptb_set_cr(ptGptbBase, wCrVal);
	csp_gptb_set_pscr(ptGptbBase, 0);	
	csp_gptb_set_prdr(ptGptbBase, 0xFFFF);	
}

/**
 \brief  gptb pwm configuration
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptGptbPwmCfg   \refer csi_gptb_capture_config_t
  * 		- eWorkMode:GPTB work mode \refer csi_gptb_workmode_e
  * 		- eCountMode:GPTB counting mode \refer csi_gptb_cntmd_e
  * 		- eRunMode:GPTB run mode \refer csi_gptb_runmode_e
  * 		- byDutyCycle:PWM duty cycle  		
  * 		- wFreq:PWM frequency
 \return error code \ref csi_error_t
*/
csi_error_t csi_gptb_pwm_init(csp_gptb_t *ptGptbBase, csi_gptb_pwm_config_t *ptGptbPwmCfg)
{
    uint32_t wClkDiv;
	uint32_t wCrVal;
	uint32_t wCmpLoad; 
	uint32_t wPrdrLoad; 
	
	if(ptGptbPwmCfg->wFreq == 0 ){return CSI_ERROR;}

	csi_clk_enable((uint32_t *)ptGptbBase);							
	
	csp_gptb_clk_enable(ptGptbBase);
	csp_gptb_wr_key(ptGptbBase);                                     
	csp_gptb_sw_rst(ptGptbBase);
	
	if(ptGptbPwmCfg -> eCountMode == GPTB_CNT_UPDN)
	{
		wClkDiv = (csi_get_pclk_freq() / ptGptbPwmCfg->wFreq / 2 / 30000);		
		if(wClkDiv == 0)wClkDiv = 1;
		wPrdrLoad  = (csi_get_pclk_freq() / ptGptbPwmCfg->wFreq / 2 / wClkDiv);	
	}
	else
	{
		wClkDiv = (csi_get_pclk_freq() / ptGptbPwmCfg->wFreq / 30000);		
		if(wClkDiv == 0)wClkDiv = 1;
		wPrdrLoad  = (csi_get_pclk_freq() / ptGptbPwmCfg->wFreq / wClkDiv);	  
	}

	wCrVal = (ptGptbPwmCfg -> eCountMode) | (ptGptbPwmCfg -> eRunMode << GPTB_RUNMD_POS) | (ptGptbPwmCfg -> eWorkMode << GPTB_MODE_POS);

	csp_gptb_clk_enable(ptGptbBase);
	csp_gptb_set_cr(ptGptbBase, wCrVal);								
	csp_gptb_set_pscr(ptGptbBase, (uint16_t)wClkDiv - 1);				
	csp_gptb_set_prdr(ptGptbBase, (uint16_t)wPrdrLoad);				   

	if(ptGptbPwmCfg->byDutyCycle >= 100){wCmpLoad = 0;}
	else if(ptGptbPwmCfg->byDutyCycle == 0){wCmpLoad = wPrdrLoad + 1;}
	else {wCmpLoad = wPrdrLoad - (wPrdrLoad * ptGptbPwmCfg -> byDutyCycle /100);}	
	csp_gptb_set_cmpa(ptGptbBase, (uint16_t)wCmpLoad);				
	csp_gptb_set_cmpb(ptGptbBase, (uint16_t)wCmpLoad);
	
	return CSI_OK;
}

/** \brief gptb timer initialize
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] ptGptbTimCfg of gptb timing parameter config structure
 * 			   	- wTimeVal: timing value, unit: us
 * 			   	- eRunMode: gptb run mode, \ref csi_gptb_runmode_e
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_gptb_timer_init(csp_gptb_t *ptGptbBase, csi_gptb_time_config_t *ptGptbTimCfg)
{
    uint32_t wClkDiv;
	uint32_t wCrVal;
	uint32_t wPrdrLoad; 
	
	if((ptGptbTimCfg -> wTimeVal) == 0 ){return CSI_ERROR;}
		
	csi_clk_enable((uint32_t *)ptGptbBase);					
	
	csp_gptb_clk_enable(ptGptbBase);
	csp_gptb_wr_key(ptGptbBase);                                   
	csp_gptb_sw_rst(ptGptbBase);								
		
	if(csi_get_pclk_freq() < 6000000)
	{
		wClkDiv = csi_get_pclk_freq() / 1000 * (ptGptbTimCfg -> wTimeVal) / 1000 / 60000;		
		if(wClkDiv == 0)
			wClkDiv  = 1;
		wPrdrLoad = csi_get_pclk_freq() / 1000 * (ptGptbTimCfg -> wTimeVal) / 1000 / wClkDiv;	
		if(wPrdrLoad > 0xffff)
		{
			wClkDiv += 1;
			wPrdrLoad = csi_get_pclk_freq() / 1000 * (ptGptbTimCfg -> wTimeVal) / 1000 / wClkDiv ;	
		}		
	}
	else 
	{
		wClkDiv = csi_get_pclk_freq() / 1000000 * (ptGptbTimCfg -> wTimeVal) / 60000;		
		if(wClkDiv == 0)
			wClkDiv  = 1;
		wPrdrLoad = csi_get_pclk_freq() / 1000000 * (ptGptbTimCfg -> wTimeVal) / wClkDiv;	
		if(wPrdrLoad > 0xffff)
		{
			wClkDiv += 1;
			wPrdrLoad = csi_get_pclk_freq() / 1000000 * (ptGptbTimCfg -> wTimeVal) / wClkDiv ;
		}			
	}
	wCrVal = GPTB_CNT_UP | (GPTB_SYNC_START << GPTB_STARTMODE_POS) | (GPTB_WORK_WAVE << GPTB_MODE_POS);
	wCrVal = (wCrVal & ~(GPTB_PSCLD_MSK)) | ((GPTB_LDPSCR_ZRO & 0x03) << GPTB_PSCLD_POS);	

	csp_gptb_clk_enable(ptGptbBase);
	csp_gptb_set_cr(ptGptbBase, wCrVal);								
	csi_gptb_set_run_mode(ptGptbBase, (ptGptbTimCfg -> eRunMode));     
	csp_gptb_set_pscr(ptGptbBase, (uint16_t)wClkDiv - 1);			
	csp_gptb_set_prdr(ptGptbBase, (uint16_t)wPrdrLoad);			

	csp_gptb_int_enable(ptGptbBase, GPTB_INT_PEND);	

return CSI_OK;					
}

/** \brief set gptb running mode
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eRunMode: gptb running mode, one pulse/continuous, \ref csi_gptb_runmode_e
 *  \return none
 */ 
void csi_gptb_set_run_mode(csp_gptb_t *ptGptbBase, csi_gptb_run_mode_e eRunMode)
{
	csp_gptb_set_run_mode(ptGptbBase, (gptb_runmd_e)eRunMode);
}

/** \brief gptb cgfilter config
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure 
 *  \param[in] eCgflt \ref csi_gptb_cgflt_e
 *  \return none
 */
void csi_gptb_set_cgflt(csp_gptb_t *ptGptbBase,csi_gptb_cgflt_e eCgflt)
{
	csp_gptb_set_flt(ptGptbBase,(gptb_cgflt_e)eCgflt);
	csp_gptb_flt_enable(ptGptbBase);
}

/** \brief gptb burst config
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] byCgsrc \ref csi_gptb_cgsrc_e 
 *  \return none
 */
void csi_gptb_set_burst(csp_gptb_t *ptGptbBase,csi_gptb_cgsrc_e eCgsrc)
{
	csp_gptb_set_cgsrc(ptGptbBase,(gptb_cgsrc_e)eCgsrc);
	csp_gptb_burst_enable(ptGptbBase);
}

 /**
 \brief  PWM Channel configuration
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptPwmChCfg   	\refer to csi_gptb_pwm_channel_config_t
 \param  channel        \refer to csi_gptb_channel_e
 \return none
*/
csi_error_t csi_gptb_pwm_ch_init(csp_gptb_t *ptGptbBase, csi_gptb_pwm_ch_config_t *ptPwmChCfg, csi_gptb_channel_e eChannel)
{
    uint32_t wVal;
	
	wVal=  ptPwmChCfg -> eActionZRO 
		| (ptPwmChCfg -> eActionPRD << GPTB_ACT_PRD_POS)
		| (ptPwmChCfg -> eActionC1U << GPTB_ACT_C1U_POS)
		| (ptPwmChCfg -> eActionC1D << GPTB_ACT_C1D_POS)
		| (ptPwmChCfg -> eActionC2U << GPTB_ACT_C2U_POS)
		| (ptPwmChCfg -> eActionC2D << GPTB_ACT_C2D_POS)
		| (ptPwmChCfg -> eActionT1U << GPTB_ACT_T1U_POS)
		| (ptPwmChCfg -> eActionT1D << GPTB_ACT_T1D_POS)
		| (ptPwmChCfg -> eActionT2U << GPTB_ACT_T2U_POS)
		| (ptPwmChCfg -> eActionT2D << GPTB_ACT_T2D_POS)
		| (ptPwmChCfg -> eC1Sel  	<< GPTB_C1SEL_POS  )
		| (ptPwmChCfg -> eC2Sel  	<< GPTB_C2SEL_POS  );

	switch (eChannel)
	{	
		case (GPTB_CHANNEL_1):csp_gptb_set_aqcr1(ptGptbBase,wVal);
			break;
			
		case (GPTB_CHANNEL_2):csp_gptb_set_aqcr2(ptGptbBase,wVal);
			break;
			
		default:return CSI_ERROR;
			break;
	}
	
	return CSI_OK;
}  
 
/**
 \brief  gptb deadzone init
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptCfg   \refer csi_gptb_deadzone_config_t
  * 		-eChxOutSel_S1S0:DeadZone switch S0S1 config:DeadZone output Source Select. \refer csi_gptb_db_outsel_e
  * 		-eChxPol_S3S2:DeadZone switch S2S3 config:DeadZone output polarity Control. \refer csi_gptb_db_pol_e
  * 		-eChxInSel_S5S4:DeadZone switch S4S5 config:DeadZone delay module intput Select. \refer csi_gptb_dbcha_insel_e
  * 		-eChxOutSwap_S8S7:DeadZone switch S7S8 config:DeadZone output Swap Control. \refer csi_gptb_dbcha_outswap_e
  * 		-eDclkSel:DeadZone Clock Source. \refer csi_gptb_dbclksrc_e
  * 		-eChxDedb_S6:DeadZone switch S6 config:DeadZone output Select. \refer csi_gptb_dedb_e
  * 		-hwDpsc:DeadZone Clock Divider
  * 		-wRisingEdgeTime:DeadZone rising edge delay time,unit:ns
  * 		-wFallingEdgeTime:DeadZone falling edge delay time,unit:ns
 \param  eChannel   \refer csi_gptb_channel_e
 \return CSI_OK /CSI_ERROR
*/
csi_error_t csi_gptb_deadzone_init(csp_gptb_t *ptGptbBase, csi_gptb_deadzone_config_t *ptDeadzoneCfg, csi_gptb_channel_e eChannel)
{  
	uint32_t w_Val;
	
	w_Val=csp_gptb_get_dbcr(ptGptbBase);	
	
	switch (eChannel)
	{	
		case (GPTB_CHANNEL_1): 
			w_Val = (w_Val & ~(GPTB_CHA_OUTSEL_MSK)) | (ptDeadzoneCfg-> eChxOutSel_S1S0  << GPTB_CHA_OUTSEL_POS);
			w_Val = (w_Val & ~(GPTB_CHA_POL_MSK))    | (ptDeadzoneCfg-> eChxPol_S3S2	 << GPTB_CHA_POL_POS);
			w_Val = (w_Val & ~(GPTB_CHA_INSEL_MSK))  | (ptDeadzoneCfg-> eChxInSel_S5S4   << GPTB_CHA_INSEL_POS);
			w_Val = (w_Val & ~(GPTB_CHA_OUTSWAP_MSK))| (ptDeadzoneCfg-> eChxOutSwap_S8S7 << GPTB_CHA_OUTSWAP_POS);
			w_Val = (w_Val & ~(GPTB_DCKSEL_MSK))     | (ptDeadzoneCfg-> eDclkSel 		 << GPTB_DCKSEL_POS);
			w_Val = (w_Val & ~(GPTB_CHA_DEDB_MSK))   | (ptDeadzoneCfg-> eChxDedb_S6	  	 << GPTB_CHA_DEDB_POS);
			break;

		default:return CSI_ERROR;
			break;
	}
	csp_gptb_set_dbcr(ptGptbBase, w_Val);	 
	csp_gptb_set_dpscr(ptGptbBase, ptDeadzoneCfg -> hwDpsc);
	
	w_Val=csi_get_pclk_freq();
	w_Val=(1000000000 / (w_Val / (ptDeadzoneCfg -> hwDpsc+1)));	
	csp_gptb_set_dbdtr(ptGptbBase, ptDeadzoneCfg -> wRisingEdgeTime / w_Val);
	csp_gptb_set_dbdtf(ptGptbBase, ptDeadzoneCfg -> wFallingEdgeTime / w_Val);
	
	return CSI_OK;	
}

/**
 \brief  gptb emergency init
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptEmCfg   \refer csi_gptb_emergency_config_t
  * 		-eEp:Emergency EP channel select:EP 0/1/2/3. \refer csi_gptb_ep_e
  * 		-eEmSrc:Emergency source:EBIx/CMPx. \refer csi_gptb_emsrc_e
  * 		-eEmPol:Emergency polarity:LOW/HIGH. \refer csi_gptb_empol_e
  * 		-eFltPace0:Emergency filter0 pace config. \refer csi_gptb_epflt_e
  * 		-eFltPace1:Emergency filter1 pace config. \refer csi_gptb_epflt_e
  * 		-eEpxLockMode:Emergency EP lock mode:SLCK/HLCK. \refer csi_gptb_ep_lckmd_e
  * 		-eSlckClrMode:Emergency SLCK clear mode. \refer csi_gptb_slclrmd_e
  * 		-eOsrShdw:Emergency EMOSR shadow mode. \refer csi_gptb_osrshdw_e
  * 		-eOsrLoadMode:Emergency EMSOR Shadow load mode. \refer csi_gptb_ldemosr_e
  * 		-byORL0:Emergency ORL0 source
  * 		-byORL1:Emergency ORL1 source
 \return CSI_OK /CSI_ERROR
*/
csi_error_t csi_gptb_emergency_init(csp_gptb_t *ptGptbBase, csi_gptb_emergency_config_t *ptEmCfg)
{ 
	uint32_t wEmsrc;
	uint32_t wEmsrc2;
	uint32_t wEmpol;
	uint32_t wEmecr;
	
	if(ptEmCfg -> eEmSrc == GPTB_EMSRC_ORL0)
	{
		wEmsrc2 = csp_gptb_get_emsrc2(ptGptbBase);
		wEmsrc2 = (wEmsrc2 & (~GPTB_EPPACE0_MSK)) | (ptEmCfg -> eFltPace0  << GPTB_EPPACE0_POS);
		wEmsrc2 = (wEmsrc2 & ~0xff) | (ptEmCfg -> byORL0);
		csp_gptb_set_emsrc2(ptGptbBase,wEmsrc2);
	}
    
	if(ptEmCfg -> eEmSrc == GPTB_EMSRC_ORL1)	
	{
		wEmsrc2 = csp_gptb_get_emsrc2(ptGptbBase);
		wEmsrc2 = (wEmsrc2 & (~GPTB_EPPACE1_MSK)) | (ptEmCfg -> eFltPace1 << GPTB_EPPACE1_POS);
		wEmsrc2 = (wEmsrc2 & ~0xff0000) | ((ptEmCfg -> byORL1) <<16);
		csp_gptb_set_emsrc2(ptGptbBase,wEmsrc2);
	}
	
	wEmsrc = csp_gptb_get_emsrc(ptGptbBase);
    wEmsrc=(wEmsrc &~ GPTB_SEL_MSK_EP(ptEmCfg -> eEp)) | (ptEmCfg -> eEmSrc << GPTB_SEL_POS_EP(ptEmCfg -> eEp));
    csp_gptb_set_emsrc(ptGptbBase,wEmsrc);
	
	if((ptEmCfg -> eEmSrc) > 0)
	{
		wEmpol=csp_gptb_get_empol(ptGptbBase);	
		switch (ptEmCfg -> eEmSrc)
		{    
			case (GPTB_EMSRC_EBI0):  wEmpol = (wEmpol & ~GPTB_POL_MSK_EBI(0)) | (ptEmCfg -> eEmPol << GPTB_POL_POS_EBI(0));break;
			case (GPTB_EMSRC_EBI1):  wEmpol = (wEmpol & ~GPTB_POL_MSK_EBI(1)) | (ptEmCfg -> eEmPol << GPTB_POL_POS_EBI(1));break;
			case (GPTB_EMSRC_EBI2):  wEmpol = (wEmpol & ~GPTB_POL_MSK_EBI(2)) | (ptEmCfg -> eEmPol << GPTB_POL_POS_EBI(2));break;
			case (GPTB_EMSRC_EBI3):  wEmpol = (wEmpol & ~GPTB_POL_MSK_EBI(3)) | (ptEmCfg -> eEmPol << GPTB_POL_POS_EBI(3));break;
			case (GPTB_EMSRC_CMP0):  wEmpol = (wEmpol & ~GPTB_POL_MSK_EBI(4)) | (ptEmCfg -> eEmPol << GPTB_POL_POS_EBI(4));break;
			case (GPTB_EMSRC_CMP1):  wEmpol = (wEmpol & ~GPTB_POL_MSK_EBI(5)) | (ptEmCfg -> eEmPol << GPTB_POL_POS_EBI(5));break;
			case (GPTB_EMSRC_CMP2):  wEmpol = (wEmpol & ~GPTB_POL_MSK_EBI(6)) | (ptEmCfg -> eEmPol << GPTB_POL_POS_EBI(6));break;
			case (GPTB_EMSRC_ORL0):  break;
			case (GPTB_EMSRC_ORL1):  break;
			default:return CSI_ERROR;break;
		}
		csp_gptb_set_empol(ptGptbBase,wEmpol);
	}

	if(ptEmCfg -> eEpxLockMode)
	{
		wEmecr =  csp_gptb_get_emecr(ptGptbBase);	
		wEmecr =(wEmecr & (~GPTB_LCKMD_MSK_EP(ptEmCfg ->eEp))) | (ptEmCfg  -> eEpxLockMode     <<  GPTB_LCKMD_POS_EP(ptEmCfg -> eEp));
		wEmecr =(wEmecr & (~GPTB_EMECR_SLCLRMD_MSK         ))  | (ptEmCfg  -> eSlckClrMode     <<  GPTB_EMECR_SLCLRMD_POS          ); 
		wEmecr =(wEmecr & (~GPTB_EMECR_OSRSHDW_MSK         ))  | ((ptEmCfg -> eOsrShdw & 0x01) <<  GPTB_EMECR_OSRSHDW_POS          );
		wEmecr =(wEmecr & (~GPTB_EMECR_OSRLDMD_MSK         ))  | (ptEmCfg  -> eOsrLoadMode     <<  GPTB_EMECR_OSRLDMD_POS          );
		csp_gptb_set_emecr(ptGptbBase,wEmecr);
	}
    
	return CSI_OK;
}

/**
 \brief  State of emergency configuration 
 \param  ptGptbBase    	pointer of gptb register structure
 \param  eOsrch         \refer to csi_gptb_osrchx_e
 \param  eEmout         \refer to csi_gptb_emout_e
 \return CSI_OK /CSI_ERROR
*/
csi_error_t csi_gptb_set_emergency_out(csp_gptb_t *ptGptbBase, csi_gptb_osrchx_e eOsrch, csi_gptb_emout_e eEmOut)
{ 
	uint32_t wEmosr;
    wEmosr=csp_gptb_get_emosr(ptGptbBase);	
	switch (eOsrch)
	{    
		case (GPTB_EMCOAX): wEmosr = (wEmosr & ~(GPTB_EMOSR_EM_COAX_MSK)) | (eEmOut << GPTB_EMOSR_EM_COAX_POS);break;
		case (GPTB_EMCOBX): wEmosr = (wEmosr & ~(GPTB_EMOSR_EM_COBX_MSK)) | (eEmOut << GPTB_EMOSR_EM_COBX_POS);break;
		case (GPTB_EMCOAY): wEmosr = (wEmosr & ~(GPTB_EMOSR_EM_COAY_MSK)) | (eEmOut << GPTB_EMOSR_EM_COAY_POS);break;		
		default:return CSI_ERROR;break;
	}
    csp_gptb_set_emosr(ptGptbBase,wEmosr);
	
	return CSI_OK;
}

/** \brief Channel CMPLDR configuration
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eLoadMode: 		\ref to csi_gptb_ldmd_e
 *  \param[in] eShdwLoadMode:   \ref to csi_gptb_shdwldmd_e
 *  \param[in] eChannel: 		\ref to csi_gptb_comp_e
 *  \return error code \ref csi_error_t
 */
void csi_gptb_set_cmpload(csp_gptb_t *ptGptbBase, csi_gptb_ldmd_e eLoadMode, csi_gptb_shdwldmd_e eShdwLoadMode,csi_gptb_comp_e eCmp)
{			 
	csp_gptb_set_cmpldr(ptGptbBase,(gptb_ldmd_e)eLoadMode, (gptb_shdwldmd_e)eShdwLoadMode, (gptb_comp_e)eCmp);
}

/** \brief Channel AQLDR configuration
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eLoadMode: 	  \ref to csi_gptb_ldmd_e
 *  \param[in] eShdwLoadMode: \ref to csi_gptb_shdwldmd_e
 *  \param[in] eChannel:      \ref to csi_gptb_channel_e
 *  \return error code \ref csi_error_t
 */
void csi_gptb_set_aqload(csp_gptb_t *ptGptbBase, csi_gptb_ldmd_e eLoadMode, csi_gptb_shdwldmd_e eShdwLoadMode ,csi_gptb_channel_e eChannel)
{			
	csp_gptb_set_aqldr(ptGptbBase,(gptb_ldmd_e)eLoadMode, (gptb_shdwldmd_e)eShdwLoadMode, (gptb_channel_e)eChannel);  
}

/**
 \brief  DeadZoneTime configuration loading 
 \param  ptGptbBase    	pointer of gptb register structure
 \param  eDbldr         \ref to csi_gptb_dbldr_e
 \param  eDbldmd        \ref to csi_gptb_dbldmd_e
 \param  eShdwdbldmd	\ref to csi_gptb_shdwdbldmd_e
 \return CSI_OK /CSI_ERROR
*/
csi_error_t csi_gptb_set_dbload(csp_gptb_t *ptGptbBase, csi_gptb_dbldr_e eDbldr,csi_gptb_dbldmd_e eDbldmd, csi_gptb_shdwdbldmd_e eShdwdbldmd)
{   
	uint32_t w_Val;
	w_Val=csp_gptb_get_dbldr(ptGptbBase);
	switch (eDbldr)
	{	
		case (GPTB_DBCR):
			w_Val = ( w_Val & ~(GPTB_DBLDR_SHDWCR_MSK))   | (eDbldmd     << GPTB_DBLDR_SHDWCR_POS);
			w_Val = ( w_Val & ~(GPTB_DBLDR_LDCRMODE_MSK)) | (eShdwdbldmd << GPTB_DBLDR_LDCRMODE_POS);
			break;
			
		case (GPTB_DBDTR):
			w_Val = ( w_Val & ~GPTB_DBLDR_SHDWDTR_MSK)    | (eDbldmd     << GPTB_DBLDR_SHDWDTR_POS);
			w_Val = ( w_Val & ~GPTB_DBLDR_LDDTRMD_MSK)    | (eShdwdbldmd << GPTB_DBLDR_LDDTRMD_POS);
		    break;
			
		case (GPTB_DBDTF):
			w_Val = ( w_Val & ~GPTB_DBLDR_SHDWDTF_MSK)    | (eDbldmd     << GPTB_DBLDR_SHDWDTF_POS);
			w_Val = ( w_Val & ~GPTB_DBLDR_LDDTFMD_MSK)    | (eShdwdbldmd << GPTB_DBLDR_LDDTFMD_POS);
            break;
			
		case (GPTB_DCKPSC):
			w_Val = ( w_Val & ~GPTB_DBLDR_SHDWPSC_MSK) 	  | (eDbldmd     << GPTB_DBLDR_SHDWPSC_POS);
			w_Val = ( w_Val & ~GPTB_DBLDR_LDPSCMD_MSK)	  | (eShdwdbldmd << GPTB_DBLDR_LDPSCMD_POS);
		    break;
			
		default:return CSI_ERROR;
			break;
	}
	csp_gptb_set_dbldr(ptGptbBase,w_Val);
	
	return CSI_OK;
}

/**
 \brief  gptb global load config
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptGldCfg       \refer csi_gptb_global_load_config_t
  * 		- eGldEnable:Global load enable control \refer csi_gptb_gld_en_e
  * 		- eGldMode:Global load mode \refer csi_gptb_gldmd_e
  * 		- eGldOstMode:Global oneshot control \refer csi_gptb_gld_ostmd_e
  * 		- byGldprd:Global load period select   		
  * 		- byGldcnt:Global event count
 \return CSI_OK /CSI_ERROR
*/
void csi_gptb_set_gldcr(csp_gptb_t *ptGptbBase, csi_gptb_global_load_config_t *ptGldCfg)
{   
	uint32_t wGLDCR;	
	
	wGLDCR =0;
    wGLDCR = (wGLDCR & ~GPTB_GLDEN_MSK)       | ((ptGldCfg -> eGldEnable  & 0x01) << GPTB_GLDEN_POS);
	wGLDCR = (wGLDCR & ~GPTB_GLDMD_MSK)       | ((ptGldCfg -> eGldMode    & 0x0f) << GPTB_GLDMD_POS);
	wGLDCR = (wGLDCR & ~GPTB_GLDCR_OSTMD_MSK) | ((ptGldCfg -> eGldOstMode & 0x01) << GPTB_GLDCR_OSTMD_POS);
	wGLDCR = (wGLDCR & ~GPTB_GLDPRD_MSK)      | ((ptGldCfg -> byGldprd 	  & 0x07) << GPTB_GLDPRD_POS);
	csp_gptb_set_gldcr(ptGptbBase,wGLDCR);	
}

/** \brief GLDCFG loading
 * 
 *  \param[in] ptGPTBBase of GPTB register structure
 *  \param[in] Glo:  \ref csi_gptb_gldcfg_e
 *  \param[in] bEnable ENABLE or DISABLE
 *  \return CSI_OK
 */
void csi_gptb_set_gldcfg(csp_gptb_t *ptGptbBase ,csi_gptb_gldcfg_e eGloCfg, bool bEnable)
{
   	switch (eGloCfg)
	{	
		case (GPTB_GLO_PRDR): 
			csp_gptb_set_gldcfg(ptGptbBase, GPTB_LD_PRDR_MSK, (bEnable << GPTB_LD_PRDR_POS));
			break;
	
		case (GPTB_GLO_CMPA): 
			csp_gptb_set_gldcfg(ptGptbBase, GPTB_LD_CMPA_MSK, (bEnable << GPTB_LD_CMPA_POS));
			break;
			
		case (GPTB_GLO_CMPB): 
			csp_gptb_set_gldcfg(ptGptbBase, GPTB_LD_CMPB_MSK, (bEnable << GPTB_LD_CMPB_POS));
		    break;
			
		case (GPTB_GLO_DBDTR): 
			csp_gptb_set_gldcfg(ptGptbBase, GPTB_LD_DBDTR_MSK, (bEnable << GPTB_LD_DBDTR_POS));
			break;
			
		case (GPTB_GLO_DBDTF): 
			csp_gptb_set_gldcfg(ptGptbBase, GPTB_LD_DBDTF_MSK, (bEnable << GPTB_LD_DBDTF_POS));
			break;
			
		case (GPTB_GLO_DBCR): 
			csp_gptb_set_gldcfg(ptGptbBase, GPTB_LD_DBCR_MSK, (bEnable << GPTB_LD_DBCR_POS));
		    break;
			
		case (GPTB_GLO_AQCR1):
			csp_gptb_set_gldcfg(ptGptbBase, GPTB_LD_AQCR1_MSK, (bEnable << GPTB_LD_AQCR1_POS));
		    break;
			
		case (GPTB_GLO_AQCR2):
			csp_gptb_set_gldcfg(ptGptbBase, GPTB_LD_AQCR2_MSK, (bEnable << GPTB_LD_AQCR2_POS));
		    break;
			
	    case (GPTB_GLO_AQCSF):
			csp_gptb_set_gldcfg(ptGptbBase, GPTB_LD_AQCSF_MSK, (bEnable << GPTB_LD_AQCSF_POS));
			break;
			
		case (GPTB_GLO_EMOSR):
			csp_gptb_set_gldcfg(ptGptbBase, GPTB_LD_EMOSR_MSK, (bEnable << GPTB_LD_EMOSR_POS));
			break;
			
		default: 
			break;
	}   
}

/**
 \brief  Software global load
 \param  ptGptbBase    	pointer of gptb register structure
 \return CSI_OK
*/
void csi_gptb_sw_global_load(csp_gptb_t *ptGptbBase) 
{
	csp_gptb_set_gldcr2(ptGptbBase,GPTB_SW_GLD);
}

/**
 \brief  rearm  loading
 \param  ptGptbBase    	pointer of gptb register structure
 \return CSI_OK
*/
void csi_gptb_global_rearm(csp_gptb_t *ptGptbBase)
{
	csp_gptb_set_gldcr2(ptGptbBase,GPTB_OSREARM_EN);
}

/** \brief start GPTB counter
 *  \param ptGptbBase:  pointer of gptb register structure
 *  \return none
 */ 
void csi_gptb_start(csp_gptb_t *ptGptbBase)
{   
	csp_gptb_wr_key(ptGptbBase); 
	csp_gptb_start(ptGptbBase);
}

/**
 \brief stop GPTB counter
 \param ptGptbBase  pointer of gptb register structure
 \return none
*/
void csi_gptb_stop(csp_gptb_t *ptGptbBase)
{
  csp_gptb_wr_key(ptGptbBase);
  csp_gptb_stop(ptGptbBase);
}

/**
 \brief GPTB clock source select
 \param ptGptbBase pointer of gptb register structure
 \param eClkSrc \ref to csi_gptb_clksrc_e
 \return none
*/
void csi_gptb_set_clksrc(csp_gptb_t *ptGptbBase,csi_gptb_clksrc_e eClkSrc)
{
  csp_gptb_set_clksrc(ptGptbBase, (gptb_clksrc_e)eClkSrc);
}

/**
 \brief set GPTB start mode. 
 \param ptGptbBase    pointer of gptb register structure
 \param eMode \ref csi_gptb_start_mode_e
 \return none
*/
void csi_gptb_set_start_mode(csp_gptb_t *ptGptbBase, csi_gptb_start_mode_e eMode)
{
	csp_gptb_set_start_mode(ptGptbBase, (gptb_startmode_e)eMode);
}

/**
 \brief set GPTB pwm stop status
 \param ptGptbBase    pointer of gptb register structure
 \param eStpSt \ref csi_gptb_stpst_e
 \return none
*/
void csi_gptb_set_stop_status(csp_gptb_t *ptGptbBase, csi_gptb_stpst_e eStpSt)
{	
  csp_gptb_set_stop_status(ptGptbBase,(gptb_stpst_e)eStpSt);
}

/**
 \brief get counter period to calculate the duty cycle. 
 \param ptGptbBase    pointer of gptb register structure
 \return counter period (reg data)
*/
uint16_t csi_gptb_get_prdr(csp_gptb_t *ptGptbBase)
{
	return csp_gptb_get_prdr(ptGptbBase);
}

/** \brief  update gptb PRDR reg value
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] hwPrdr: gptb PRDR reg  value
 *  \return none
 */
void csi_gptb_prdr_update(csp_gptb_t *ptGptbBase, uint16_t hwPrdr) 
{
	csp_gptb_set_prdr(ptGptbBase, (uint16_t)hwPrdr);
}

/** \brief  update gptb CMPx reg value
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eComp: \ref  csi_gptb_comp_e.select which COMP to set(COMPA or COMPB)
 *  \param[in] hwCmp: gptb COMP reg value
 *  \return none
 */
void csi_gptb_cmp_update(csp_gptb_t *ptGptbBase,csi_gptb_comp_e eComp, uint16_t hwCmp) 
{
	switch (eComp)
	{	
		case (GPTB_COMPA):
			csp_gptb_set_cmpa(ptGptbBase, (uint16_t)hwCmp);	//set GPTB COMPA Value
			break;
			
		case (GPTB_COMPB):
			csp_gptb_set_cmpb(ptGptbBase, (uint16_t)hwCmp);	//set GPTB COMPB Value
			break;

		default: 
			break;
	}
}

/** \brief  update gptb pwm freq and duty cycle
 * 
 *  \param[in] ptBtBase: pointer of gptb register structure
 *  \param[in] wFreq: pwm frequency  
 *  \param[in] byDutyCycle: pwm duty cycle(0 -> 100)
 *  \param[in] eComp: \refer to csi_gptb_comp_e
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_gptb_pwm_update(csp_gptb_t *ptGptbBase, uint32_t wFreq, uint8_t byDutyCycle, csi_gptb_comp_e eComp)
{
	uint8_t byCntMd = 0;
	uint16_t hwClkDiv = 0;
	uint16_t hwCmpLoad = 0; 
	uint16_t hwPrdrLoad = 0; 
	
	if(wFreq == 0 ){return CSI_ERROR;}

	byCntMd = csp_gptb_get_count_mode(ptGptbBase);
	
	if((csi_gptb_cnt_mode_e)byCntMd == GPTB_CNT_UPDN)
	{
		hwClkDiv = (csi_get_pclk_freq() / wFreq / 2 / 30000);		
		if(hwClkDiv == 0)hwClkDiv = 1;
		hwPrdrLoad  = (csi_get_pclk_freq() / wFreq / 2 / hwClkDiv);	
	}
	else
	{
		hwClkDiv = (csi_get_pclk_freq() / wFreq / 30000);		
		if(hwClkDiv == 0)hwClkDiv = 1;
		hwPrdrLoad  = (csi_get_pclk_freq() / wFreq / hwClkDiv);	  
	}
	
	csp_gptb_set_pscr(ptGptbBase, (uint16_t)hwClkDiv - 1);				
	csp_gptb_set_prdr(ptGptbBase, (uint16_t)hwPrdrLoad);				   

	if(byDutyCycle >= 100){hwCmpLoad = 0;}
	else if(byDutyCycle == 0){hwCmpLoad = hwPrdrLoad + 1;}
	else {hwCmpLoad = hwPrdrLoad - (hwPrdrLoad * byDutyCycle /100);}	
	
	switch (eComp)
	{	
		case (GPTB_COMPA):csp_gptb_set_cmpa(ptGptbBase, (uint16_t)hwCmpLoad);
			break;
			
		case (GPTB_COMPB):csp_gptb_set_cmpb(ptGptbBase, (uint16_t)hwCmpLoad);
			break;

		default: 
			break;
	}
	
	return CSI_OK;
}

/**
 \brief get harklock status
 \param ptGptbBase    pointer of gptb register structure
 \return  0 or 1
*/
bool csi_gptb_get_hdlck_status(csp_gptb_t *ptGptbBase)
{	
	return (csp_gptb_get_emhdlck(ptGptbBase));
}

/**
 \brief clear harklock status
 \param ptGptbBase   pointer of gptb register structure
 \return  eEp: emergency channel:  \refer to csi_gptb_ep_e                 
*/
void csi_gptb_clr_hdlck(csp_gptb_t *ptGptbBase, csi_gptb_ep_e eEp)
{
	csp_gptb_clr_emhdlck(ptGptbBase, (gptb_ep_e)eEp);
}

/**
 \brief get softlock status
 \param ptGptbBase  pointer of gptb register structure
 \return  0 or 1
*/
bool csi_gptb_get_sftlck_status(csp_gptb_t *ptGptbBase)
{	
	return (csp_gptb_get_emsdlck(ptGptbBase));
}

/**
 \brief clear softlock status
 \param ptGptbBase  pointer of gptb register structure
 \return eEp: emergency channel:  \refer to csi_gptb_ep_e
*/
void csi_gptb_clr_sftlck(csp_gptb_t *ptGptbBase, csi_gptb_ep_e eEp)
{	
	csp_gptb_clr_emsdlck(ptGptbBase, (gptb_ep_e)eEp);
}

/** \brief software force emergency
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eEp: emergency channel:  \refer to csi_gptb_ep_e
 *  \return none
 */
void csi_gptb_force_em(csp_gptb_t *ptGptbBase, csi_gptb_ep_e eEp)
{
	csp_gptb_force_em(ptGptbBase, (gptb_ep_e)eEp);
}

/**
  \brief       enable gptb in debug mode
  \param[in]   ptGptbBase  pointer of gptb register structure
*/
void csi_gptb_debug_enable(csp_gptb_t *ptGptbBase)
{
	csp_gptb_dbg_enable(ptGptbBase);
}

/**
  \brief       disable gptb in debug mode
  \param[in]   ptGptbBase pointer of gptb register structure
*/
void csi_gptb_debug_disable(csp_gptb_t *ptGptbBase)
{
	csp_gptb_dbg_disable(ptGptbBase);
}

/**
  \brief       enable gptb emergency interrupt
  \param[in]   ptGptbBase  pointer of gptb register structure
  \param[in]   eEmint	\refer to csi_gptb_emint_e
*/
void csi_gptb_emint_enable(csp_gptb_t *ptGptbBase, csi_gptb_emint_e eEmint)
{
    csp_gptb_emint_enable(ptGptbBase,(gptb_emint_e)eEmint);
}

/**
  \brief       disable gptb emergency interrupt
  \param[in]   ptGptbBase   pointer of gptb register structure
  \param[in]   eEmint		\refer to csi_gptb_emint_e
*/
void csi_gptb_emint_disable(csp_gptb_t *ptGptbBase, csi_gptb_emint_e eEmint)
{
    csp_gptb_emint_disable(ptGptbBase,(gptb_emint_e)eEmint);
}

/**
  \brief   enable/disable gptb out trigger 
  \param   ptGptbBase   pointer of gptb register structure
  \param   eTrgEv		\refer to csi_gptb_trgev_e
  \return CSI_OK /CSI_ERROR
*/
csi_error_t csi_gptb_evtrg_enable(csp_gptb_t *ptGptbBase, csi_gptb_trgev_e eTrgEv)
{	
	if (eTrgEv > GPTB_TRG_EV1)
		return CSI_ERROR;
		
    csp_gptb_evtrg_enable(ptGptbBase, (gptb_trgev_e)eTrgEv);
	return CSI_OK;
}

/**
  \brief   Onetime software waveform control 
  \param   ptGptbBase    pointer of gptb register structure 
  \param   eChannel	    \refer to csi_gptb_channel_e  		
  \param   eAction 		\refer to csi_gptb_action_e
  \return  none
*/
void csi_gptb_set_aqosf(csp_gptb_t *ptGptbBase, csi_gptb_channel_e eChannel, csi_gptb_action_e eAction)
{	
	csp_gptb_set_aqosf(ptGptbBase, (gptb_channel_e)eChannel, (gptb_action_e)eAction);
}

/** \brief  Continuous software waveform loading control
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eLoadMode:  \refer to csi_gptb_aqcsf_ldmd_e
 *  \return  none
 */
void csi_gptb_set_aqcsf_loadmode(csp_gptb_t *ptGptbBase, csi_gptb_aqcsf_ldmd_e eLoadMode)
{
	csp_gptb_set_aqcsf_load_mode(ptGptbBase, (gptb_aqcsf_ldmd_e)eLoadMode);
}

/** \brief Continuous software waveform control
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] byCh        \refer to csi_gptb_channel_e
 *  \param[in] eAction:    \refer to  csi_gptb_aqcsf_e
 *  \return  none
 */
void csi_gptb_set_aqcsf(csp_gptb_t *ptGptbBase, csi_gptb_channel_e eChannel, csi_gptb_aqcsf_act_e eAction)
{
	csp_gptb_set_aqcsf(ptGptbBase, (gptb_channel_e)eChannel, (gptb_aqcsf_act_e)eAction);
}

/** \brief gptb interrupt clear  
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eInt:       \ref csi_gptb_int_e
 *  \return none;
 */
void csi_gptb_clr_isr(csp_gptb_t *ptGptbBase, csi_gptb_int_e eInt)
{  
	csp_gptb_clr_isr(ptGptbBase,(gptb_int_e)eInt);
}

/** \brief gptb interrupt enable  
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eInt:       \ref csi_gptb_int_e
 *  \return none;
 */
void csi_gptb_int_enable(csp_gptb_t *ptGptbBase, csi_gptb_int_e eInt)
{  
	csp_gptb_clr_isr(ptGptbBase,(gptb_int_e)eInt);
	csp_gptb_int_enable(ptGptbBase,(gptb_int_e)eInt);
}

/** \brief gptb interrupt disable   
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eInt:       \ref csi_gptb_int_e
 *  \return none;
 */
void csi_gptb_int_disable(csp_gptb_t *ptGptbBase, csi_gptb_int_e eInt)
{  
	csp_gptb_int_disable(ptGptbBase,(gptb_int_e)eInt);
}

/** \brief gptb sync input evtrg config  
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eSyncIn:    \refer to csi_gptb_syncin_e
 *  \param[in] eSyncMode:  \refer to csi_gptb_syncmode_e
 *  \param[in] eAutoRearm: \refer to csi_gptb_arearm_e 
 *  \return none
 */
void csi_gptb_set_sync(csp_gptb_t *ptGptbBase, csi_gptb_syncin_e eSyncIn, csi_gptb_syncmode_e eSyncMode, csi_gptb_arearm_e eAutoRearm)
{
	csp_gptb_set_sync_mode(ptGptbBase, (gptb_syncin_e)eSyncIn, (gptb_syncmd_e)eSyncMode);
	csp_gptb_set_auto_rearm(ptGptbBase, (gptb_arearm_e)eAutoRearm);
}

/** \brief gptb sync enable
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eSyncIn: gptb sync input channel, \ref csi_gptb_syncin_e
 *  \return none
 */
void csi_gptb_sync_enable(csp_gptb_t *ptGptbBase, csi_gptb_syncin_e eSyncIn)
{
	csp_gptb_sync_enable(ptGptbBase, (gptb_syncin_e)eSyncIn);
}

/** \brief gptb sync->trgout select
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eSyncIn:  \refer to csi_gptb_syncin_e
 *  \param[in] eTrgOut: \refer to csi_gptb_trgout_e
 *  \return error code \ref csi_error_t
 */
void csi_gptb_set_extsync_bypass(csp_gptb_t *ptGptbBase, csi_gptb_syncin_e eSyncIn, csi_gptb_trgout_e eTrgOut)
{
	switch(eTrgOut)
	{
		case GPTB_TRGOUT0_SEL:
			csp_gptb_sync_trg0sel(ptGptbBase, (gptb_syncin_e)eSyncIn);
			break;
			
		case GPTB_TRGOUT1_SEL:
			csp_gptb_sync_trg1sel(ptGptbBase, (gptb_syncin_e)eSyncIn);
			break;
			
		default:
			break;
	}
}

/** \brief gptb sync input filter config  
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] ptFilter: pointer of sync input filter parameter config structure
 * 				- eFltSrc:Filter input signal source \refer csi_gptb_fltsrc_e
 * 				- eWinInv:Filter window inversion control \refer csi_gptb_wininv_e
 * 				- eAlignMode:Filter window alignment mode control \refer csi_gptb_alignmd_e
 * 				- eCrossMode:Filter window cross mode control \refer csi_gptb_crossmd_e  		
 * 				- hwWinOffset:Filter window offset config
 * 				- hwWinWidth:Filter window width config
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_set_sync_filter(csp_gptb_t *ptGptbBase, csi_gptb_filter_config_t *ptFilterCfg)
{
	uint32_t wFiltCr;
	uint32_t wWindow;
	
	if((ptFilterCfg -> eFltSrc) > GPTB_FLTSRC_SYNCIN6)
		return CSI_ERROR;
		
	wFiltCr = (ptFilterCfg -> eFltSrc) | ((ptFilterCfg -> eWinInv) << GPTB_FLTBLKINV_POS) | 
			((ptFilterCfg -> eAlignMode) << GPTB_ALIGNMD_POS) | ((ptFilterCfg -> eCrossMode) << GPTB_CROSSMD_POS);
	wWindow = (ptFilterCfg -> hwWinOffset) | ((ptFilterCfg -> hwWinWidth) << GPTB_FLT_WDW_POS);
	
	csp_gptb_set_trgftcr(ptGptbBase, wFiltCr);
	csp_gptb_set_trgftwr(ptGptbBase, wWindow);
	
	return CSI_OK;
}

/** \brief rearm gptb sync rearm  
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eSyncin: gptb sync evtrg input channel(0~6), \ref to csi_gptb_syncin_e
 *  \return none
 */
void csi_gptb_sync_rearm(csp_gptb_t *ptGptbBase,csi_gptb_syncin_e eSyncin)
{
	csp_gptb_sync_rearm(ptGptbBase, (gptb_syncin_e)eSyncin);
}

/** \brief gptb evtrg output config
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eTrgEv: \ref to csi_gptb_trgev_e
 *  \param[in] eTrgSrc: \ref to csi_gptb_trgsrc_e 
 *  \return error code \ref csi_error_t
 */
void csi_gptb_set_evtrg(csp_gptb_t *ptGptbBase, csi_gptb_trgev_e eTrgEv, csi_gptb_trgsrc_e eTrgSrc)
{
	csp_gptb_set_evtrg_src(ptGptbBase, (gptb_trgev_e)eTrgEv, (gptb_trgsrc_e)eTrgSrc);			    
}

/** \brief gptb evtrg cntxinit control
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eCntChx: evtrg countinit channel(0~1)   \ref to csi_gptb_cntinit_e
 *  \param[in] byCntVal: evtrg cnt value(1~16)
 *  \param[in] byCntInitVal: evtrg cntxinit value(1~16)
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_set_evcntinit(csp_gptb_t *ptGptbBase, csi_gptb_cntinit_e eCntChx, uint8_t byCntVal, uint8_t byCntInitVal)
{
	if(eCntChx > GPTB_CNT1INIT)
		return CSI_ERROR;
 
	csp_gptb_set_evtrg_prd(ptGptbBase, (gptb_trgev_e)eCntChx, byCntVal);    //evtrg count
	csp_gptb_set_evcntinit(ptGptbBase, (gptb_trgev_e)eCntChx, byCntInitVal);
	csp_gptb_evtrg_cntiniten_enable(ptGptbBase, (gptb_trgev_e)eCntChx);
 
	return CSI_OK;
}

/**
 \brief  gptb reg link configuration
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptGlobal       \refer csi_gptb_reglk_config_t
 \return none
*/
void csi_gptb_set_reglk(csp_gptb_t *ptGptbBase,csi_gptb_reglk_config_t *ptGlobal)
{   
	uint32_t wRegLk;	
	wRegLk =0;
    wRegLk = (wRegLk & ~GPTB_PRDR_MSK) | ((ptGlobal-> byPrdr & 0xF) << GPTB_PRDR_POS);
	wRegLk = (wRegLk & ~GPTB_CMPA_MSK) | ((ptGlobal-> byCmpa & 0xF) << GPTB_CMPA_POS);
	wRegLk = (wRegLk & ~GPTB_CMPB_MSK) | ((ptGlobal-> byCmpb & 0xF) << GPTB_CMPB_POS);
	wRegLk = (wRegLk & ~GPTB_GLD2_MSK) | ((ptGlobal-> byGld2 & 0xF) << GPTB_GLD2_POS);
	wRegLk = (wRegLk & ~GPTB_RSSR_MSK) | ((ptGlobal-> byRssr & 0xF) << GPTB_RSSR_POS);
	csp_gptb_set_reglk(ptGptbBase,wRegLk);
	
	wRegLk =0;
	wRegLk = (wRegLk & ~GPTB_EMSLCLR_MSK) | ((ptGlobal-> byEmslclr & 0xF) << GPTB_EMSLCLR_POS);
	wRegLk = (wRegLk & ~GPTB_EMHLCLR_MSK) | ((ptGlobal-> byEmhlclr & 0xF) << GPTB_EMHLCLR_POS);
	wRegLk = (wRegLk & ~GPTB_EMICR_MSK)   | ((ptGlobal-> byEmicr   & 0xF) << GPTB_EMICR_POS);
	wRegLk = (wRegLk & ~GPTB_EMFRCR_MSK)  | ((ptGlobal-> byEmfrcr  & 0xF) << GPTB_EMFRCR_POS);
	wRegLk = (wRegLk & ~GPTB_AQOSF_MSK)   | ((ptGlobal-> byAqosf   & 0xF) << GPTB_AQOSF_POS);
	wRegLk = (wRegLk & ~GPTB_AQCSF_MSK)   | ((ptGlobal-> byAqcsf   & 0xF) << GPTB_AQCSF_POS);
	csp_gptb_set_reglk2(ptGptbBase,wRegLk);	
}