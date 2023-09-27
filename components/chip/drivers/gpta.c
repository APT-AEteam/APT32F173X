/***********************************************************************//** 
 * \file  gpta.c
 * \brief  Enhanced purpose timer driver
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-6-17 <td>V0.0  <td>ljy   <td>initial
 * <tr><td> 2023-5-16 <td>V0.0  <td>YYM   <td>initial
 * <tr><td> 2023-9-07 <td>V1.0  <td>LHY   <td>code normalization
 * </table>
 * *********************************************************************
*/
#include "drv/gpta.h"

csi_gpta_ctrl_t g_tGptaCtrl[GPTA_IDX];

/** \brief capture configuration
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] ptGptaPwmCfg: refer to csi_ept_captureconfig_t
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gpta_capture_init(csp_gpta_t *ptGptaBase, csi_gpta_capture_config_t *ptGptaCapCfg)
{
	uint32_t wCrVal;

	csi_clk_enable((uint32_t *)ptGptaBase);	
	csp_gpta_clken(ptGptaBase);
	csp_gpta_wr_key(ptGptaBase);	
	csp_gpta_sw_rst(ptGptaBase);

	wCrVal = (ptGptaCapCfg -> eCountMode) | (ptGptaCapCfg -> eRunMode << GPTA_CAPMD_POS) | (ptGptaCapCfg -> eWorkMode << GPTA_MODE_POS);
	wCrVal = (wCrVal & ~(0x01<<CAPMODE_SEL_POS))|((ptGptaCapCfg -> eCapMode & 0x01)  	<< CAPMODE_SEL_POS);
	
	wCrVal = (wCrVal & ~(GPTA_STOPWRAP_MSK)) | ((ptGptaCapCfg -> byCapStopWrap & 0x03)<< GPTA_STOPWRAP_POS);
	wCrVal = (wCrVal & ~(GPTA_CMPA_RST_MSK)) | ((ptGptaCapCfg -> byCapLdaret & 0x01)  << GPTA_CMPA_RST_POS);
	wCrVal = (wCrVal & ~(GPTA_CMPB_RST_MSK)) | ((ptGptaCapCfg -> byCapLdbret & 0x01)  << GPTA_CMPB_RST_POS);
	wCrVal = (wCrVal & ~(GPTA_CMPAA_RST_MSK)) | ((ptGptaCapCfg -> byCapLdcret & 0x01)  << GPTA_CMPAA_RST_POS);
	wCrVal = (wCrVal & ~(GPTA_CMPBA_RST_MSK)) | ((ptGptaCapCfg -> byCapLddret & 0x01)  << GPTA_CMPBA_RST_POS);

	wCrVal |= GPTA_CAPLD_MSK;
	wCrVal |= GPTA_CAPREARM_MSK;

    csp_gpta_clken(ptGptaBase);  
	csp_gpta_set_cr(ptGptaBase, wCrVal);
	csp_gpta_set_pscr(ptGptaBase, 0);	
	csp_gpta_set_prdr(ptGptaBase, 0xFFFF);	
	
	return CSI_OK;
}

/** \brief wave configuration
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] ptGptaPwmCfg: refer to csi_ept_pwmconfig_t
 *  \return error code \ref csi_error_t
 */
csi_error_t  csi_gpta_pwm_init(csp_gpta_t *ptGptaBase, csi_gpta_pwm_config_t *ptGptaPwmCfg)
{
    uint32_t wClkDiv;
	uint32_t wCrVal;
	uint32_t wCmpLoad; 
	uint32_t wPrdrLoad; 
	
	if(ptGptaPwmCfg->wFreq == 0 )
	{
		return CSI_ERROR;
	}
		
	csi_clk_enable((uint32_t *)ptGptaBase);				// clk enable
	csp_gpta_clken(ptGptaBase);
	csp_gpta_wr_key(ptGptaBase);                        // Unlock
	csp_gpta_sw_rst(ptGptaBase);						// reset 
	

	if(ptGptaPwmCfg->eCountMode == GPTA_CNT_UPDOWN)
	{
		wClkDiv = (csi_get_pclk_freq() / ptGptaPwmCfg->wFreq / 2 / 30000);		 // clk div value
		if(wClkDiv == 0)
		{
			wClkDiv = 1;
		}	
		wPrdrLoad  = (csi_get_pclk_freq() / ptGptaPwmCfg->wFreq / 2 / wClkDiv);	 //prdr load value
	}
	else
	{
		wClkDiv = (csi_get_pclk_freq() / ptGptaPwmCfg->wFreq / 30000);		     // clk div value
		if(wClkDiv == 0)
		{
			wClkDiv = 1;	
		}
		wPrdrLoad  = (csi_get_pclk_freq()/ptGptaPwmCfg->wFreq/wClkDiv);	         //prdr load value
	}
		
//	wCrVal = ptGptaPwmCfg->byCountingMode | (ptGptaPwmCfg->byStartSrc<<GPTA_STARTMODE_POS) |
//	         ptGptaPwmCfg->byOneshotMode<<GPTA_RUNMODE_POS | (ptGptaPwmCfg->byWorkmod<<GPTA_MODE_POS);
//    
//	wCrVal = (wCrVal & ~(GPTA_PSCLD_MSK)) | ((ptGptaPwmCfg->byPscld&0x03) << GPTA_PSCLD_POS);

	wCrVal = (ptGptaPwmCfg -> eCountMode) | (ptGptaPwmCfg -> eRunMode << GPTA_RUNMODE_POS) | (ptGptaPwmCfg -> eWorkMode << GPTA_MODE_POS);
		
    csp_gpta_clken(ptGptaBase);                                   // clkEN
	csp_gpta_set_cr(ptGptaBase, wCrVal);						  // set bt work mode
	csp_gpta_set_pscr(ptGptaBase, (uint16_t)wClkDiv - 1);		  // clk div
	csp_gpta_set_prdr(ptGptaBase, (uint16_t)wPrdrLoad);			  // prdr load value
		
	if(ptGptaPwmCfg->byDutyCycle >= 100)
	{
		wCmpLoad=0;
	}
	else if(ptGptaPwmCfg->byDutyCycle == 0)
	{
		wCmpLoad = wPrdrLoad + 1;
	}
	else
	{
		wCmpLoad = wPrdrLoad-(wPrdrLoad * ptGptaPwmCfg->byDutyCycle / 100);
	}		
	csp_gpta_set_cmpa(ptGptaBase, (uint16_t)wCmpLoad);			  // cmp load value
	csp_gpta_set_cmpb(ptGptaBase, (uint16_t)wCmpLoad);
		
	return CSI_OK;	
}

/** \brief initialize gpta data structure
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] ptGptaTimCfg of gpta timing parameter config structure
 * 			   	- wTimeVal: timing value, unit: us
 * 			   	- eRunMode: gpta run mode, \ref csi_gpta_runmode_e
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_gpta_timer_init(csp_gpta_t *ptGptaBase,  csi_gpta_time_config_t *ptGptaTimCfg)
{
    uint32_t wClkDiv;
	uint32_t wCrVal;
	uint32_t wPrdrLoad; 
	
	if(ptGptaTimCfg->wTimeVal == 0 )
	{
		return CSI_ERROR;
	}
		
	csi_clk_enable((uint32_t *)ptGptaBase);			// clk enable
	csp_gpta_clken(ptGptaBase);
	csp_gpta_wr_key(ptGptaBase);                    // Unlock
	csp_gpta_sw_rst(ptGptaBase);					// reset 
		
	if(csi_get_pclk_freq() < 6000000)
	{
		wClkDiv = csi_get_pclk_freq() / 1000 * (ptGptaTimCfg->wTimeVal) / 1000 / 60000;		    //gpta clk div value
		if(wClkDiv == 0)
		{
			wClkDiv  = 1;
		}
		wPrdrLoad = csi_get_pclk_freq() / 1000 * (ptGptaTimCfg->wTimeVal) / 1000 / wClkDiv;	    //gpta prdr load value
		if(wPrdrLoad > 0xffff)
		{
			wClkDiv += 1;
			wPrdrLoad = csi_get_pclk_freq() / 1000 * (ptGptaTimCfg->wTimeVal) / 1000 / wClkDiv; //gpta prdr load value
		}		
	}
	else 
	{
		wClkDiv = csi_get_pclk_freq() / 1000000 * (ptGptaTimCfg->wTimeVal) / 60000;		        //gpta clk div value
		if(wClkDiv == 0)
		{
			wClkDiv  = 1;
		}
		wPrdrLoad = csi_get_pclk_freq() / 1000000 * (ptGptaTimCfg->wTimeVal) / wClkDiv;	        //gpta prdr load value
		if(wPrdrLoad > 0xffff)
		{
			wClkDiv += 1;
			wPrdrLoad = csi_get_pclk_freq() / 1000000 * (ptGptaTimCfg->wTimeVal) / wClkDiv ;	//gpta prdr load value
		}			
	}
	
	wCrVal = GPTA_CNT_UP | (GPTA_SYNC_START << GPTA_STARTMODE_POS) | (GPTA_WORK_WAVE << GPTA_MODE_POS);
	wCrVal = (wCrVal & ~(GPTA_PSCLD_MSK)) | ((GPTA_LDPSCR_ZRO & 0x03) << GPTA_PSCLD_POS);	

    csp_gpta_clken(ptGptaBase);                                         // clkEN
	csp_gpta_set_cr(ptGptaBase, wCrVal);							    // set gpta work mode
	csi_gpta_set_runmode(ptGptaBase, ptGptaTimCfg->eRunMode);           // gpta count mode
	csp_gpta_set_pscr(ptGptaBase, (uint16_t)wClkDiv - 1);			    // clk div
	csp_gpta_set_prdr(ptGptaBase, (uint16_t)wPrdrLoad);				    // prdr load value

	csp_gpta_int_enable(ptGptaBase, GPTA_INT_PEND);	                    //enable interrupt
	
	return CSI_OK;					
}

/** \brief set gpta count mode
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] eRunMode: gpta runmode, once/continuous
 *  \return none
 */ 
void csi_gpta_set_runmode(csp_gpta_t *ptGptaBase, csi_gpta_runmode_e eRunMode)
{
	csp_gpta_set_runmode(ptGptaBase, (gpta_runmode_e)eRunMode);
}

/** \brief set gpta burst 
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] byCgsrc \ref csi_gpta_cgsrc_e 
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gpta_set_burst(csp_gpta_t *ptGptaBase,csi_gpta_cgsrc_e eCgsrc)
{
	csp_gpta_burst_enable(ptGptaBase);	
	csp_gpta_set_cgsrc(ptGptaBase,(gpta_cgsrc_e)eCgsrc);
	return CSI_OK;
}

/** \brief set gpta cgfilter 
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure 
 *  \param[in] eCgflt \ref csi_gpta_cgflt_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gpta_set_cgflt(csp_gpta_t *ptGptaBase,csi_gpta_cgflt_e eCgflt)
{
	csp_gpta_flt_enable(ptGptaBase);	
	csp_gpta_set_flt(ptGptaBase,(gpta_cgflt_e)eCgflt);
	return CSI_OK;
}

/** \brief Channel CMPLDR configuration
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] eLdmd: \ref csi_gpta_ldmd_e
 *  \param[in] eShowmdld: \ref csi_gpta_shdwldmd_e
 *  \param[in] eChannel: \ref csi_gpta_camp_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gpta_set_cmpload(csp_gpta_t *ptGptaBase, csi_gpta_ldmd_e eLoadMode, csi_gpta_shdwldmd_e eShdwLoadMode ,csi_gpta_comp_e eChannel)
{		
	csp_gpta_set_cmpldr(ptGptaBase,(gpta_ldmd_e)eLoadMode, (gpta_shdwldmd_e)eShdwLoadMode, (gpta_comp_e)eChannel);
	return CSI_OK;	  
}

/** \brief Set Channel
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] ptPwmCfg: \ref csi_gpta_pwm_channel_config_t
 *  \param[in] eChannel:  \ref csi_gpta_channel_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gpta_set_channel(csp_gpta_t *ptGptaBase, csi_gpta_pwm_channel_config_t *ptPwmCfg, csi_gpta_channel_e eChannel)
{
    uint32_t w_AQCRx_Val;
	
	w_AQCRx_Val= ptPwmCfg -> eActionZro 
				  | (ptPwmCfg -> eActionPrd  << GPTA_ACT_PRD_POS)
				  | (ptPwmCfg -> eActionC1u  << GPTA_ACT_C1U_POS)
				  | (ptPwmCfg -> eActionC1d  << GPTA_ACT_C1D_POS)
				  | (ptPwmCfg -> eActionC2u  << GPTA_ACT_C2U_POS)
				  | (ptPwmCfg -> eActionC2d  << GPTA_ACT_C2D_POS)
				  | (ptPwmCfg -> eActionT1u  << GPTA_ACT_T1U_POS)
				  | (ptPwmCfg -> eActionT1d  << GPTA_ACT_T1D_POS)
				  | (ptPwmCfg -> eActionT2u  << GPTA_ACT_T2U_POS)
				  | (ptPwmCfg -> eActionT2d  << GPTA_ACT_T2D_POS)
				  | (ptPwmCfg -> eC1sel  << GPTA_C1SEL_POS)
				  | (ptPwmCfg -> eC2sel  << GPTA_C2SEL_POS);
			  
	switch (eChannel)
	{	
		case (GPTA_CHANNEL_1):
			csp_gpta_set_aqcr1(ptGptaBase,w_AQCRx_Val);
			break;
		case (GPTA_CHANNEL_2):
			csp_gpta_set_aqcr2(ptGptaBase,w_AQCRx_Val);
			break;
		default:
			return CSI_ERROR;
			break;
	}
	return CSI_OK;
}

/** \brief Set Channel AQLDR 
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] eLoadMode: \ref csi_gpta_ldmd_e
 *  \param[in] eShdwLoadMode: \ref csi_gpta_shdwldmd_e
 *  \param[in] eChannel: \ref csi_gpta_channel_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gpta_channel_set_aqload(csp_gpta_t *ptGptaBase, csi_gpta_ldmd_e eLoadMode, csi_gpta_shdwldmd_e eShdwLoadMode ,csi_gpta_channel_e eChannel)
{			  
	csp_gpta_set_cmpldr(ptGptaBase,(gpta_ldmd_e)eLoadMode, (gpta_shdwldmd_e)eShdwLoadMode, (gpta_comp_e)eChannel);
	return CSI_OK;
}

/** \brief  set gpta gldcr
*
*   \param[in]  ptGptaBase: pointer of gpta register structure
*   \param[in]  ptGlobal: refer to csi_gpta_global_load_config_t
*   \return error code \ref csi_error_t
*/
csi_error_t csi_gpta_set_gldcr(csp_gpta_t *ptGptaBase,csi_gpta_global_load_config_t *ptGldCfg)
{   
	uint32_t wGLDCR;	
	wGLDCR = 0;
    wGLDCR = (wGLDCR &~GPTA_GLDEN_MSK)       | ((ptGldCfg->eGldEnable & 0x01)<<GPTA_GLDEN_POS);
	wGLDCR = (wGLDCR &~GPTA_GLDMD_MSK)       | ((ptGldCfg->eGldMode & 0x0f)<<GPTA_GLDMD_POS);
	wGLDCR = (wGLDCR &~GPTA_GLDCR_OSTMD_MSK) | ((ptGldCfg->eGldOstMode & 0x01)<<GPTA_GLDCR_OSTMD_POS);
	wGLDCR = (wGLDCR &~GPTA_GLDPRD_MSK)      | ((ptGldCfg->byGldprd & 0x07)<<GPTA_GLDPRD_POS);
	csp_gpta_set_gldcr(ptGptaBase,wGLDCR);	
	
	return CSI_OK;
}

/** \brief CLDCFG loading
 * 
 *  \param[in] ptGptaBase of gpta register structure
 *  \param[in] eGloCfg:  \ref csi_gpta_gldcfg_e  
 *  \param[in] bEnable：ENABLE or DISABLE
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gpta_set_gldcfg(csp_gpta_t *ptGptaBase, csi_gpta_gldcfg_e eGloCfg, bool bEnable)
{
   	switch (eGloCfg)
	{	
		case (GPTA_PRDR_A): 
			csp_gpta_set_gldcfg(ptGptaBase, GPTA_LD_PRDR_MSK, (bEnable << GPTA_LD_PRDR_POS));
			break;
			
		case (GPTA_CMPA_A): 
			csp_gpta_set_gldcfg(ptGptaBase, GPTA_LD_CMPA_MSK, (bEnable << GPTA_LD_CMPA_POS));
			break;
			
		case (GPTA_CMPB_A): 
		    csp_gpta_set_gldcfg(ptGptaBase, GPTA_LD_CMPB_MSK, (bEnable << GPTA_LD_CMPB_POS));
			break;
		
		case (GPTA_AQCRA_A):
		    csp_gpta_set_gldcfg(ptGptaBase, GPTA_LD_AQCR1_MSK, (bEnable << GPTA_LD_AQCR1_POS));
			break;
			
		case (GPTA_AQCRB_A):
			csp_gpta_set_gldcfg(ptGptaBase, GPTA_LD_AQCR2_MSK, (bEnable << GPTA_LD_AQCR2_POS));
			break;
		
	    case (GPTA_AQCSF_A):
			csp_gpta_set_gldcfg(ptGptaBase, GPTA_LD_AQCSF_MSK, (bEnable << GPTA_LD_AQCSF_POS));
			break;
	 
		default: 
			return CSI_ERROR;
			break;
	}   
	return CSI_OK;
}

/** \brief Software trigger loading
 * 
 *  \param[in] ptGptaBase pointer of gpta register structure
 *  \return none
*/
void csi_gpta_global_sw(csp_gpta_t *ptGptaBase)
{
	csp_gpta_set_gldcr2(ptGptaBase, GPTA_SW_GLD);
}

/** \brief rearm  loading
 * 
 *  \param[in] ptGptaBase pointer of gpta register structure
 *  \return none
*/
void csi_gpta_global_rearm(csp_gpta_t *ptGptaBase)
{
	csp_gpta_set_gldcr2(ptGptaBase, GPTA_OSREARM_EN);
}

/** \brief start gpta
 * 
 *  \param[in] ptGptaBase pointer of gpta register structure
 *  \return none
*/
void csi_gpta_start(csp_gpta_t *ptGptaBase)
{   csp_gpta_wr_key(ptGptaBase); 
	csp_gpta_start(ptGptaBase);
}

/** \brief  stop gpta
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \return none
*/
void csi_gpta_stop(csp_gpta_t *ptGptaBase)
{
	csp_gpta_wr_key(ptGptaBase);
	csp_gpta_stop(ptGptaBase);
}

/** \brief set gpta start mode. 
 * 
 *  \param[in] ptGptaBase £ºpointer of gpta register structure
 *  \param[in] eMode \ref csi_gpta_startmd_e
 *  \return none
 */
void csi_gpta_set_start_mode(csp_gpta_t *ptGptaBase, csi_gpta_startmd_e eMode)
{
	csp_gpta_set_start_mode(ptGptaBase, (gpta_startmode_e)eMode);
}

/** \brief set gpta stop status
 * 
 *  \param[in] ptGptaBase :   pointer of gpta register structure
 *  \param[in] eStopSt 	 csi_gpta_stopst_e
 *  \return none
 */
void csi_gpta_set_stop_status(csp_gpta_t *ptGptaBase, csi_gpta_stopst_e eStopSt)
{	
  csp_gpta_set_stop_st(ptGptaBase,(gpta_stpst_e)eStopSt);
}

/** \brief  set gpta phsr  
 * 
 *  \param[in] ptGptaBase ：pointer of gpta register structure
 *  \param[in] bwPhsr  :    phsr value
 *  \return none
 */
void csi_gpta_set_phsr(csp_gpta_t *ptGptaBase, uint16_t bwPhsr)
{
	csp_gpta_set_phsr(ptGptaBase, bwPhsr);
}

/** \brief enable gpta phsr  
 * 
 *  \param[in] ptGptaBase ：pointer of gpta register structure
 *  \return none
 */
void csi_gpta_phsr_enable(csp_gpta_t *ptGptaBase)
{
	csp_gpta_phsen_enable(ptGptaBase);
}

/** \brief disable gpta phsr  
 * 
 *  \param[in] ptGptaBase ：pointer of gpta register structure
 *  \return none
 */
void csi_gpta_phsr_disable(csp_gpta_t *ptGptaBase)
{
	csp_gpta_phsen_disable(ptGptaBase);
}

/** \brief get counter period to calculate the duty cycle. 
 * 
 *  \param[in] ptGptaBase  :  pointer of gpta register structure
 *  \return counter period (reg data)
 */
uint16_t csi_gpta_get_prdr(csp_gpta_t *ptGptaBase)
{
	return csp_gpta_get_prdr(ptGptaBase);
}

/** \brief  update gpta PRDR reg value
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] hwPrdr: gpta PRDR reg  value
 *  \return none
 */
csi_error_t csi_gpta_prdr_update(csp_gpta_t *ptGptaBase, uint16_t hwPrdr) 
{
	csp_gpta_set_prdr(ptGptaBase, (uint16_t)hwPrdr);		//set GPTA PRDR Value
    return CSI_OK;
}

/** \brief  update gpta CMPx reg value
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] eComp: \ref  csi_gpta_comp_e.select which COMP to set(COMPA or COMPB)
 *  \param[in] hwCmp: gpta COMP reg value
 *  \return none
 */
csi_error_t csi_gpta_cmp_update(csp_gpta_t *ptGptaBase,csi_gpta_comp_e eComp, uint16_t hwCmp) 
{
	switch (eComp)
	{	
		case (GPTA_COMPA):
			csp_gpta_set_cmpa(ptGptaBase, (uint16_t)hwCmp);	//set GPTA COMPA Value
			break;
			
		case (GPTA_COMPB):
			csp_gpta_set_cmpb(ptGptaBase, (uint16_t)hwCmp);	//set GPTA COMPB Value
			break;

		default: 
			return CSI_ERROR;
			break;
	}
    return (CSI_OK);
}

/** \brief change gpta output dutycycle. 
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] eCh: \ref csi_gpta_comp_e
 *	\param[in] wDuty : cmpx data to be set directly
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gpta_update_ch_duty(csp_gpta_t *ptGptaBase, csi_gpta_comp_e eCh, uint32_t wDuty)
{ 
	uint16_t  hwCmpLoad;
    uint16_t  hwPrdr;
    hwPrdr = csp_gpta_get_prdr(ptGptaBase);
	if(wDuty>=100)
	{
		hwCmpLoad=0;
	}
	else if(wDuty==0)
	{
		hwCmpLoad=hwPrdr+1;
	}
	else
	{
		hwCmpLoad =hwPrdr-(hwPrdr * wDuty /100);
	}

	switch (eCh)
	{	
		case (GPTA_COMPA):
			csp_gpta_set_cmpa(ptGptaBase, (uint16_t)hwCmpLoad);
			break;
			
		case (GPTA_COMPB):
			csp_gpta_set_cmpb(ptGptaBase, (uint16_t)hwCmpLoad);
			break;

		default: 
			return CSI_ERROR;
			break;
	}
    return (CSI_OK);
}

/** \brief enable gpta in debug mode
 * 
 *  \param[in]  ptGptaBase      pointer of gpta register structure
 *  \return none
 */
void csi_gpta_debug_enable(csp_gpta_t *ptGptaBase)
{
	csp_gpta_dbg_enable(ptGptaBase);
}

/** \brief disable gpta in debug mode
 * 
 *  \param[in]  ptGptaBase      pointer of gpta register structure
 *  \return none
 */
void csi_gpta_debug_disable(csp_gpta_t *ptGptaBase)
{
	csp_gpta_dbg_disable(ptGptaBase);
}

/**
  \brief   One time software output 
  \param   ptGptaBase    pointer of gpta register structure 
  \param   eChannel	    \refer to csi_gpta_channel_e  		
  \param   eAction 		\refer to csi_gpta_action_e
*/
csi_error_t csi_gpta_set_aqosf(csp_gpta_t *ptGptaBase, csi_gpta_channel_e eChannel, csi_gpta_action_e eAction)
{	
	csp_gpta_set_aqosf(ptGptaBase, (gpta_channel_e)eChannel, (gpta_action_e)eAction);
	return CSI_OK;
}

/** \brief  Continuous software waveform loading control
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] eLoadMode:  \refer to csi_gpta_aqcsf_ldmd_e
 *  \return  none
 */
void csi_gpta_set_aqcsf_loadmode(csp_gpta_t *ptGptaBase, csi_gpta_aqcsf_ldmd_e eLoadMode)
{
	csp_gpta_set_aqcsf_ldmd(ptGptaBase, (gpta_aqcsf_ldmd_e)eLoadMode);
}

/** \brief Continuous software waveform control
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] eChannel    \refer to csi_gpta_channel_e
 *  \param[in] eAction:    \refer to  csi_gpta_aqcsf_act_e
 *  \return  none
 */
csi_error_t csi_gpta_set_aqcsf(csp_gpta_t *ptGptaBase, csi_gpta_channel_e eChannel, csi_gpta_aqcsf_act_e eAction)
{
	csp_gpta_set_aqcsf(ptGptaBase, (gpta_channel_e)eChannel, (gpta_aqcsf_act_e)eAction);
	return CSI_OK;
}

/** \brief gpta software trigger
 *  \param[in] ptGptaBase:pointer of gpta register structure
 *  \param[in] eTgrOut: \refer csi_gpta_trgev_e
 *  \return none
 */
void csi_gpta_sw_trg(csp_gpta_t *ptGptaBase, csi_gpta_trgev_e eTgrOut)
{
	csp_gpta_sw_trg(ptGptaBase, eTgrOut);
}

/** \brief gpta interrupt enable control
 *  \param[in] ptGptaBase:pointer of gpta register structure
 *  \param[in] eInt:     refer to to csi_gpta_intsrc_e
 *  \return CSI_OK;
 */
void csi_gpta_int_enable(csp_gpta_t *ptGptaBase, csi_gpta_intsrc_e eInt)
{ 
	csp_gpta_clr_isr(ptGptaBase, (gpta_int_e)eInt);	
	csp_gpta_int_enable(ptGptaBase,(gpta_int_e)eInt);
}

/** \brief gpta interrupt disable control
 *  \param[in] ptGptaBase:pointer of gpta register structure
 *  \param[in] eInt: \refer csi_gpta_intsrc_e
 *  \return CSI_OK;
 */
void csi_gpta_int_disable(csp_gpta_t *ptGptaBase, csi_gpta_intsrc_e eInt)
{ 
	csp_gpta_int_disable(ptGptaBase,(gpta_int_e)eInt);
}

/** \brief clear gpta int
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] eInt: \refer csi_gpta_intsrc_e
 *  \return none
 */
void csi_gpta_clr_isr(csp_gpta_t *ptGptaBase, csi_gpta_intsrc_e eInt)
{
	csp_gpta_clr_isr(ptGptaBase, (gpta_int_e)eInt);	
}

/** \brief gpta sync input evtrg config  
 * 
 *  \param[in] ptGptaBase:pointer of gpta register structure
 *  \param[in] eSyncIn: \refer csi_gpta_syncin_e, gpta sync evtrg input channel(0~5)
 *  \param[in] eTrgMode: \refer csi_gpta_syncmode_e, gpta sync evtrg mode, continuous/once
 *  \param[in] eAutoRearm: \refer csi_gpta_arearm_e 
 *  \return none
 */
void csi_gpta_set_sync(csp_gpta_t *ptGptaBase, csi_gpta_syncin_e eSyncIn, csi_gpta_syncmode_e eSyncMode, csi_gpta_arearm_e eAutoRearm)
{
	csp_gpta_set_sync_mode(ptGptaBase, (gpta_syncin_e)eSyncIn, (gpta_syncmd_e)eSyncMode);
	csp_gpta_set_auto_rearm(ptGptaBase, (gpta_arearm_e)eAutoRearm);
}

/** \brief gpta sync enable  
 * 
 *  \param[in] ptGptaBase:pointer of gpta register structure
 *  \param[in] eSyncIn: \refer csi_gpta_syncin_e, gpta sync evtrg input channel(0~5)
 *  \return none
 */
void csi_gpta_sync_enable(csp_gpta_t *ptGptaBase, csi_gpta_syncin_e eSyncIn)
{
	csp_gpta_sync_enable(ptGptaBase, (gpta_syncin_e)eSyncIn);
}

/** \brief gpta sync disable  
 * 
 *  \param[in] ptGptaBase:pointer of gpta register structure
 *  \param[in] eSyncIn: \refer csi_gpta_syncin_e, gpta sync evtrg input channel(0~5)
 *  \return none
 */
void csi_gpta_sync_disable(csp_gpta_t *ptGptaBase, csi_gpta_syncin_e eSyncIn)
{
	csp_gpta_sync_disable(ptGptaBase, (gpta_syncin_e)eSyncIn);
}

/** \brief gpta extsync input select
 * 
 *  \param[in] ptGptaBase:pointer of gpta register structure
 *  \param[in] eSyncIn: \ref csi_gpta_syncin_e
 *  \param[in] eTrgChx: \ref csi_gpta_trgout_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gpta_set_extsync_chnl(csp_gpta_t *ptGptaBase, csi_gpta_syncin_e eSyncIn, csi_gpta_trgout_e eTrgChx)
{
	switch(eTrgChx)
	{
		case GPTA_TRGOUT0_SEL:
			csp_gpta_sync_trg0sel(ptGptaBase, (gpta_syncin_e)eSyncIn);
			break;
			
		case GPTA_TRGOUT1_SEL:
			csp_gpta_sync_trg1sel(ptGptaBase, (gpta_syncin_e)eSyncIn);
			break;
			
		default:
			return CSI_ERROR;
	}
	return CSI_OK;
}

/** \brief gpta sync input filter config  
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] ptFilter: pointer of sync input filter parameter config structure
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gpta_set_sync_filter(csp_gpta_t *ptGptaBase, csi_gpta_filter_config_t *ptFilterCfg)
{
	uint32_t wFiltCr;
	uint32_t wWindow;
	
	if(ptFilterCfg->eFltSrc > GPTA_FLTSRC_SYNCIN6)
	{
		return CSI_ERROR;
	}
	wFiltCr = ptFilterCfg->eFltSrc | (ptFilterCfg->eWinInv << GPTA_FLTBLKINV_POS) | 
			(ptFilterCfg->eAlignMode << GPTA_ALIGNMD_POS) | (ptFilterCfg->eCrossMode << GPTA_CROSSMD_POS);
	wWindow = ptFilterCfg->hwWinOffset | (ptFilterCfg->hwWinWidth << GPTA_FLT_WDW_POS);
	
	csp_gpta_set_trgftcr(ptGptaBase, wFiltCr);
	csp_gpta_set_trgftwr(ptGptaBase, wWindow);
	
	return CSI_OK;
}

/** \brief rearm gpta sync evtrg  
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] eSyncIn: \ref csi_gpta_syncin_e, gpta sync evtrg input channel(0~5)
 *  \return none
 */
void csi_gpta_sync_rearm(csp_gpta_t *ptGptaBase, csi_gpta_syncin_e eSyncIn)
{
	csp_gpta_sync_rearm(ptGptaBase, (uint8_t)eSyncIn);
}

/** \brief gpta evtrg output config
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] eTrgOut: \ref csi_gpta_trgev_e, evtrg out port(0~1)
 *  \param[in] eTrgSrc: \ref csi_gpta_trgsrc_e(1~15) 
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gpta_set_evtrg(csp_gpta_t *ptGptaBase, csi_gpta_trgev_e eTrgEv, csi_gpta_trgsrc_e eTrgSrc)
{
	csp_gpta_set_evtrgsel(ptGptaBase, (gpta_trgev_e)eTrgEv, (gpta_trgsrc_e)eTrgSrc);
	csp_gpta_evtrg_enable(ptGptaBase, (gpta_trgev_e)eTrgEv);				//evtrg out enable
	
	return CSI_OK;
}

/** \brief enable gpta out trigger 
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] eCh: \ref csi_gpta_trgev_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gpta_evtrg_enable(csp_gpta_t *ptGptaBase, csi_gpta_trgev_e eTrgEv)
{	
	if(eTrgEv > GPTA_TRG_EV1)
	{
		return CSI_ERROR;
	}
    csp_gpta_evtrg_enable(ptGptaBase, (gpta_trgev_e)eTrgEv);
	return CSI_OK;
}

/** \brief disable gpta out trigger 
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] eCh: \ref csi_gpta_trgev_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gpta_evtrg_disable(csp_gpta_t *ptGptaBase, csi_gpta_trgev_e eTrgEv)
{	
	if (eTrgEv > GPTA_TRG_EV1)
	{
		return CSI_ERROR;
	}
    csp_gpta_evtrg_disable(ptGptaBase, (gpta_trgev_e)eTrgEv);
	return CSI_OK;
}

/** \brief gpta evtrg cntxinit control
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] byCntChx: evtrg countinit channel(0~3)
 *  \param[in] byCntVal: evtrg cnt value
 *  \param[in] byCntInitVal: evtrg cntxinit value
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gpta_set_evcntinit(csp_gpta_t *ptGptaBase, uint8_t byCntChx, uint8_t byCntVal, uint8_t byCntInitVal)
{
	if(byCntChx > GPTA_CNT1INIT)
	{
		return CSI_ERROR;
	}

	csp_gpta_set_evtrg_prd(ptGptaBase, byCntChx, byCntVal - 1);    //evtrg count
	csp_gpta_set_evcntinit(ptGptaBase, byCntChx, byCntInitVal);
	csp_gpta_evtrg_cntxiniten_enable(ptGptaBase, byCntChx);

	return CSI_OK;
}

/**
 \brief  gpta configuration Loading
 \param  ptGptaBase  pointer of gpta register structure
 \param  ptGlobal  refer to csi_gpta_reglk_config_t
 \return CSI_OK /CSI_ERROR
*/
csi_error_t csi_gpta_set_reglk(csp_gpta_t *ptGptaBase,csi_gpta_reglk_config_t *ptGlobal)
{   
	uint32_t wRegLk;	
	wRegLk =0;
    wRegLk = (wRegLk & ~GPTA_PRDR_MSK )| ((ptGlobal-> byPrdr & 0xF)<< GPTA_PRDR_POS);
	wRegLk = (wRegLk & ~GPTA_CMPA_MSK )| ((ptGlobal-> byCmpa & 0xF)<< GPTA_CMPA_POS);
	wRegLk = (wRegLk & ~GPTA_CMPB_MSK )| ((ptGlobal-> byCmpb & 0xF)<< GPTA_CMPB_POS);
	wRegLk = (wRegLk & ~GPTA_GLD2_MSK )| ((ptGlobal-> byGld2 & 0xF)<< GPTA_GLD2_POS);
	wRegLk = (wRegLk & ~GPTA_RSSR_MSK )| ((ptGlobal-> byRssr & 0xF)<< GPTA_RSSR_POS);
	csp_gpta_set_reglk(ptGptaBase,wRegLk);
	wRegLk =0;
	wRegLk = (wRegLk & ~GPTA_EMSLCLR_MSK )| ((ptGlobal-> byEmslclr & 0xF)<< GPTA_EMSLCLR_POS);
	wRegLk = (wRegLk & ~GPTA_EMHLCLR_MSK )| ((ptGlobal-> byEmhlclr & 0xF)<< GPTA_EMHLCLR_POS);
	wRegLk = (wRegLk & ~GPTA_EMICR_MSK )  | ((ptGlobal-> byEmicr   & 0xF)<< GPTA_EMICR_POS);
	wRegLk = (wRegLk & ~GPTA_EMFRCR_MSK ) | ((ptGlobal-> byEmfrcr  & 0xF)<< GPTA_EMFRCR_POS);
	wRegLk = (wRegLk & ~GPTA_AQOSF_MSK )  | ((ptGlobal-> byAqosf   & 0xF)<< GPTA_AQOSF_POS);
	wRegLk = (wRegLk & ~GPTA_AQCSF_MSK )  | ((ptGlobal-> byAqcsf   & 0xF)<< GPTA_AQCSF_POS);
	csp_gpta_set_reglk2(ptGptaBase,wRegLk);	
	
	return CSI_OK;
}

/** \brief get gpta number 
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \return gpta number 0/1/2/3
 */ 
static uint8_t apt_get_gpta_idx(csp_gpta_t *ptGptaBase)
{
	switch((uint32_t)ptGptaBase)
	{
		case APB_GPTA0_BASE:            //GPTA0
			return 0;                
		case APB_GPTA1_BASE:            //GPTA1
			return 1;  
		case APB_GPTA2_BASE:            //GPTA2
			return 2;  
		case APB_GPTA3_BASE:            //GPTA3
			return 3;  
		default:
			return 0xff;                //error
	}
}

/** 
  \brief       register gpta interrupt callback function
  \param[in]   ptGptaBase    pointer of cnta register structure
  \param[in]   callback  gpta interrupt handle function
  \return      error code \ref csi_error_t
 */ 
csi_error_t csi_gpta_register_callback(csp_gpta_t *ptGptaBase, void  *callback)
{
	uint8_t byIdx = apt_get_gpta_idx(ptGptaBase);
	if(byIdx == 0xff)
		return CSI_ERROR;
			
	g_tGptaCtrl[byIdx].callback = callback;
	
	return CSI_OK;
}

/** \brief gpta interrupt handle weak function
 * 
 *  \param[in] ptGptaBase: pointer of cnta register structure
 *  \return none
 */
void csi_gpta_irqhandler(csp_gpta_t *ptGptaBase,  uint8_t byIdx)
{
	uint32_t wIsr = csp_gpta_get_isr(ptGptaBase);
	
	if(g_tGptaCtrl[byIdx].callback)
		g_tGptaCtrl[byIdx].callback(ptGptaBase, wIsr);
	
	csp_gpta_clr_isr(ptGptaBase, wIsr);
}