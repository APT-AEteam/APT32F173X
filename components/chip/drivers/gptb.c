/***********************************************************************//** 
 * \file  gptb.c
 * \brief  GPTB driver
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-6-17 <td>V0.0  <td>ljy   <td>initial
 * </table>
 * *********************************************************************
*/
#include "sys_clk.h"
#include "drv/common.h"
#include "drv/gptb.h"
#include "csp_gptb.h"
#include "drv/irq.h"

/**
 \brief  Basic configuration
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptGptbPwmCfg   	refer to csi_gptb_config_t
 \return CSI_OK/CSI_ERROR
*/
csi_error_t csi_gptb_config_init(csp_gptb_t *ptGptbBase, csi_gptb_config_t *ptGptbPwmCfg)
{
	uint32_t wClkDiv;
	uint32_t wCrVal;
	uint32_t wCmpLoad; 
	uint32_t wPrdrLoad; 
	
	if(ptGptbPwmCfg->wFreq == 0 ){ptGptbPwmCfg->wFreq =100;}
	
	
	csi_clk_enable((uint32_t *)ptGptbBase);								// clk enable
	
	csp_gptb_clken(ptGptbBase);
	csp_gptb_wr_key(ptGptbBase);                                           //Unlocking
	csp_gptb_reset(ptGptbBase);											// reset 
	
	wClkDiv = (csi_get_pclk_freq() / ptGptbPwmCfg->wFreq / 60000);		// clk div value
	if(wClkDiv == 0)wClkDiv = 1;
	
	wPrdrLoad  = (csi_get_pclk_freq()/ptGptbPwmCfg->wFreq/wClkDiv);	    //prdr load value
			
	wCrVal =ptGptbPwmCfg->byCountingMode | (ptGptbPwmCfg->byStartSrc<<GPTB_STARTSRC_POS) |
	        ptGptbPwmCfg->byOneshotMode<<GPTB_OPMD_POS | (ptGptbPwmCfg->byWorkmod<<GPTB_MODE_POS);
    
	wCrVal=(wCrVal & ~(GPTB_PSCLD_MSK))   |((ptGptbPwmCfg->byPscld&0x03)   <<GPTB_PSCLD_POS);
	
	if(ptGptbPwmCfg->byWorkmod==GPTB_CAPTURE)
	{
	 	wCrVal=(wCrVal & ~(GPTB_CAPMD_MSK))   |((ptGptbPwmCfg->byCaptureCapmd&0x01)   <<GPTB_CAPMD_POS);
		wCrVal=(wCrVal & ~(GPTB_STOPWRAP_MSK))|((ptGptbPwmCfg->byCaptureStopWrap&0x03)<<GPTB_STOPWRAP_POS);
//		wCrVal=(wCrVal & ~(GPTB_CMPA_RST_MSK))|((ptGptbPwmCfg->byCaptureLdaret&0x01)  <<GPTB_CMPA_RST_POS);
//		wCrVal=(wCrVal & ~(GPTB_CMPB_RST_MSK))|((ptGptbPwmCfg->byCaptureLdbret&0x01)  <<GPTB_CMPB_RST_POS);
//		wCrVal=(wCrVal & ~(GPTB_CMPC_RST_MSK))|((ptGptbPwmCfg->byCaptureLdcret&0x01)  <<GPTB_CMPC_RST_POS);
//		wCrVal=(wCrVal & ~(GPTB_CMPD_RST_MSK))|((ptGptbPwmCfg->byCaptureLddret&0x01)  <<GPTB_CMPD_RST_POS);
		
		if(ptGptbPwmCfg->byCaptureCapLden)wCrVal|=GPTB_CAPLD_EN;
		if(ptGptbPwmCfg->byCaptureRearm)  wCrVal|=GPTB_CAPREARM;
		
		wPrdrLoad=0xFFFF;
	}
	
	if(ptGptbPwmCfg->byBurst)
	{
		wCrVal=(wCrVal & ~(GPTB_CGSRC_MSK))|((ptGptbPwmCfg->byCgsrc&0x03)<<GPTB_CGSRC_POS);
		wCrVal=(wCrVal & ~(GPTB_CGFLT_MSK))|((ptGptbPwmCfg->byCgflt&0x07)<<GPTB_CGFLT_POS);
	}
	
    csp_gptb_clken(ptGptbBase);                                             // clkEN
	csp_gptb_set_cr(ptGptbBase, wCrVal);									// set bt work mode
	csp_gptb_set_pscr(ptGptbBase, (uint16_t)wClkDiv - 1);					// clk div
	csp_gptb_set_prdr(ptGptbBase, (uint16_t)wPrdrLoad);				        // prdr load value
		
	if(ptGptbPwmCfg->byDutyCycle){
	wCmpLoad =wPrdrLoad-(wPrdrLoad * ptGptbPwmCfg->byDutyCycle /100);	    // cmp load value
	csp_gptb_set_cmpa(ptGptbBase, (uint16_t)wCmpLoad);					    // cmp load value
	csp_gptb_set_cmpb(ptGptbBase, (uint16_t)wCmpLoad);
//	csp_gptb_set_cmpc(ptGptbBase, (uint16_t)wCmpLoad);
//	csp_gptb_set_cmpd(ptGptbBase, (uint16_t)wCmpLoad);
	}
	
	if(ptGptbPwmCfg->wInt)
	{
		csp_gptb_int_enable(ptGptbBase, ptGptbPwmCfg->wInt, true);		//enable interrupt
		csi_irq_enable((uint32_t *)ptGptbBase);							//enable  irq
	}
	
	return CSI_OK;
}

 /**
 \brief  capture configuration
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptGptbPwmCfg   	refer to csi_gptb_captureconfig_t
 \return CSI_OK /CSI_ERROR
//*/
csi_error_t csi_gptb_capture_init(csp_gptb_t *ptGptbBase, csi_gptb_captureconfig_t *ptGptbPwmCfg)
{
	uint32_t wClkDiv=0;
	uint32_t wCrVal;
	uint32_t wPrdrLoad; 
			
	csi_clk_enable((uint32_t *)ptGptbBase);								 // clk enable
	csp_gptb_clken(ptGptbBase);
	csp_gptb_wr_key(ptGptbBase);	                                     //Unlocking	
	csp_gptb_reset(ptGptbBase);											 // reset 
			                                                             // clk div value
	if(wClkDiv == 0){wClkDiv = 1;}
					
	wCrVal =ptGptbPwmCfg->byCountingMode | (ptGptbPwmCfg->byStartSrc<<GPTB_STARTSRC_POS) |
	        ptGptbPwmCfg->byOneshotMode<<GPTB_OPMD_POS | (ptGptbPwmCfg->byWorkmod<<GPTB_MODE_POS);
    
	wCrVal=(wCrVal & ~(GPTB_PSCLD_MSK))   |((ptGptbPwmCfg->byPscld&0x03)   <<GPTB_PSCLD_POS);
	
	wCrVal=(wCrVal & ~(GPTB_CAPMD_MSK))   |((ptGptbPwmCfg->byCaptureCapmd&0x01)   <<GPTB_CAPMD_POS);
	wCrVal=(wCrVal & ~(GPTB_STOPWRAP_MSK))|((ptGptbPwmCfg->byCaptureStopWrap&0x03)<<GPTB_STOPWRAP_POS);
	wCrVal=(wCrVal & ~(GPTB_CMPA_RST_MSK))|((ptGptbPwmCfg->byCaptureLdaret&0x01)  <<GPTB_CMPA_RST_POS);
	wCrVal=(wCrVal & ~(GPTB_CMPB_RST_MSK))|((ptGptbPwmCfg->byCaptureLdbret&0x01)  <<GPTB_CMPB_RST_POS);
	
	wCrVal=(wCrVal & ~(0x01<<CAPMODE_SEL_POS))|((ptGptbPwmCfg->byCapSrcMode&0x01)  << CAPMODE_SEL_POS);

	wCrVal|=GPTB_CAPLD_EN;//CAPMODE_SEL
	wCrVal|=GPTB_CAPREARM;
	wPrdrLoad=0xFFFF;

    csp_gptb_clken(ptGptbBase);                                         // clkEN
	csp_gptb_set_cr(ptGptbBase, wCrVal);								// set bt work mode
	csp_gptb_set_pscr(ptGptbBase, (uint16_t)wClkDiv - 1);				// clk div
	csp_gptb_set_prdr(ptGptbBase, (uint16_t)wPrdrLoad);				    // prdr load value
	
	if(ptGptbPwmCfg->wInt)
	{
		csp_gptb_int_enable(ptGptbBase, ptGptbPwmCfg->wInt, true);   //enable interrupt
		csi_irq_enable((uint32_t *)ptGptbBase);							//enable  irq
	}
	
	return CSI_OK;
}

 /**
 \brief  wave configuration
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptGptbPwmCfg   	refer to csi_gptb_pwmconfig_t
 \return CSI_OK /CSI_ERROR
*/
csi_error_t  csi_gptb_wave_init(csp_gptb_t *ptGptbBase, csi_gptb_pwmconfig_t *ptGptbPwmCfg)
{
    uint32_t wClkDiv;
	uint32_t wCrVal;
	uint32_t wCmpLoad; 
	uint32_t wPrdrLoad; 
	
	if(ptGptbPwmCfg->wFreq == 0 ){return CSI_ERROR;}
		
	csi_clk_enable((uint32_t *)ptGptbBase);								// clk enable
	
	csp_gptb_clken(ptGptbBase);
	csp_gptb_wr_key(ptGptbBase);                                        //Unlocking
	csp_gptb_reset(ptGptbBase);											// reset 
	
	if(ptGptbPwmCfg->byCountingMode==GPTB_UPDNCNT){
		wClkDiv = (csi_get_pclk_freq() / ptGptbPwmCfg->wFreq / 2 / 30000);		// clk div value
		if(wClkDiv == 0)wClkDiv = 1;
		wPrdrLoad  = (csi_get_pclk_freq()/ptGptbPwmCfg->wFreq/ 2 / wClkDiv);	    //prdr load value
	}else{
		wClkDiv = (csi_get_pclk_freq() / ptGptbPwmCfg->wFreq / 30000);		// clk div value
		if(wClkDiv == 0)wClkDiv = 1;
		wPrdrLoad  = (csi_get_pclk_freq()/ptGptbPwmCfg->wFreq/ wClkDiv);	    //prdr load value
	}

	wCrVal =ptGptbPwmCfg->byCountingMode | (ptGptbPwmCfg->byStartSrc<<GPTB_STARTSRC_POS) |
	        ptGptbPwmCfg->byOneshotMode<<GPTB_OPMD_POS | (ptGptbPwmCfg->byWorkmod<<GPTB_MODE_POS);
    
	wCrVal=(wCrVal & ~(GPTB_PSCLD_MSK))   |((ptGptbPwmCfg->byPscld&0x03)   <<GPTB_PSCLD_POS);
		
    csp_gptb_clken(ptGptbBase);                                           // clkEN
	csp_gptb_set_cr(ptGptbBase, wCrVal);									// set bt work mode
	csp_gptb_set_pscr(ptGptbBase, (uint16_t)wClkDiv - 1);					// clk div
	csp_gptb_set_prdr(ptGptbBase, (uint16_t)wPrdrLoad);				    // prdr load value
		
	if(ptGptbPwmCfg->byDutyCycle >= 100){wCmpLoad = 0;}
	else if(ptGptbPwmCfg->byDutyCycle == 0){wCmpLoad = wPrdrLoad + 1;}
	else {wCmpLoad =wPrdrLoad-(wPrdrLoad * ptGptbPwmCfg->byDutyCycle /100);}	// cmp load value
	csp_gptb_set_cmpa(ptGptbBase, (uint16_t)wCmpLoad);					// cmp load value
	csp_gptb_set_cmpb(ptGptbBase, (uint16_t)wCmpLoad);
	
	if(ptGptbPwmCfg->wInt)
	{
		csp_gptb_int_enable(ptGptbBase, ptGptbPwmCfg->wInt, true);		//enable interrupt
		csi_irq_enable((uint32_t *)ptGptbBase);							//enable  irq
	}
	
	return CSI_OK;	
}

/** \brief initialize gptb data structure
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] wTimeOut: the timeout for gptb, unit: us, 20us < wTimeOut < 3S
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_gptb_timer_init(csp_gptb_t *ptGptbBase, uint32_t wTimeOut)
{
    uint32_t wClkDiv;
	uint32_t wCrVal;
	uint32_t wPrdrLoad; 
	
	if(wTimeOut == 0 ){return CSI_ERROR;}
		
	csi_clk_enable((uint32_t *)ptGptbBase);								// clk enable
	
	csp_gptb_clken(ptGptbBase);
	csp_gptb_wr_key(ptGptbBase);                                           //Unlocking
	csp_gptb_reset(ptGptbBase);											// reset 
		
	if(csi_get_pclk_freq() < 6000000)
	{
		wClkDiv = csi_get_pclk_freq() / 1000 * wTimeOut / 1000 / 60000;		//gptb clk div value
		if(wClkDiv == 0)
			wClkDiv  = 1;
		wPrdrLoad = csi_get_pclk_freq() / 1000 * wTimeOut / 1000 / wClkDiv;	//gptb prdr load value
		if(wPrdrLoad > 0xffff)
		{
			wClkDiv += 1;
			wPrdrLoad = csi_get_pclk_freq() / 1000 * wTimeOut / 1000 / wClkDiv ;	//gptb prdr load value
		}		
	}
	else 
	{
		wClkDiv = csi_get_pclk_freq() / 1000000 * wTimeOut / 60000;		//gptb clk div value
		if(wClkDiv == 0)
			wClkDiv  = 1;
		wPrdrLoad = csi_get_pclk_freq() / 1000000 * wTimeOut / wClkDiv;	//gptb prdr load value
		if(wPrdrLoad > 0xffff)
		{
			wClkDiv += 1;
			wPrdrLoad = csi_get_pclk_freq() / 1000000 * wTimeOut / wClkDiv ;	//gptb prdr load value
		}			
		
	}
	wCrVal =GPTB_UPCNT | (GPTB_SYNC_START<<GPTB_STARTSRC_POS) | (GPTB_WAVE<<GPTB_MODE_POS);
	wCrVal=(wCrVal & ~(GPTB_PSCLD_MSK))   |((GPTB_LDPSCR_ZRO&0x03)   <<GPTB_PSCLD_POS);	

    csp_gptb_clken(ptGptbBase);                                           // clkEN
	csp_gptb_set_cr(ptGptbBase, wCrVal);									// set gptb work mode
	csi_gptb_count_mode(ptGptbBase, GPTB_OP_CONT);                          // gptb count mode
	csp_gptb_set_pscr(ptGptbBase, (uint16_t)wClkDiv - 1);					// clk div
	csp_gptb_set_prdr(ptGptbBase, (uint16_t)wPrdrLoad);				    // prdr load value

	csp_gptb_int_enable(ptGptbBase, GPTB_INT_PEND, ENABLE);		        //enable interrupt
	csi_irq_enable((uint32_t *)ptGptbBase);							    //enable  irq
	
	return CSI_OK;					
}

/** \brief set gptb count mode
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eCntMode: gptb count mode, one pulse/continuous
 *  \return none
 */ 
void csi_gptb_count_mode(csp_gptb_t *ptGptbBase, csi_gptb_opmd_e eCntMode)
{
	csp_gptb_set_opmd(ptGptbBase, (csp_gptb_opmd_e)eCntMode);
}

/** \brief enable/disable gptb burst 
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] byCgsrc:cgr src 
 *  \param[in] byCgflt:cfg flt
 *  \param[in] bEnable: ENABLE/DISABLE
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_burst_enable(csp_gptb_t *ptGptbBase,uint8_t byCgsrc,uint8_t byCgflt, bool bEnable)
{
	csp_gptb_set_burst(ptGptbBase,byCgsrc,bEnable);	
	csp_gptb_flt_init(ptGptbBase,byCgflt,bEnable);
	return CSI_OK;
}

 /**
 \brief  Channel configuration
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptPwmCfg   	    refer to csi_gptb_pwmchannel_config_t
 \param  channel        Channel label
 \return CSI_OK /CSI_ERROR
*/
csi_error_t csi_gptb_channel_config(csp_gptb_t *ptGptbBase, csi_gptb_pwmchannel_config_t *ptPwmCfg, csi_gptb_channel_e eChannel)
{
    uint32_t w_AQCRx_Val;
	
	w_AQCRx_Val=  ptPwmCfg -> byActionZro 
	              | ( ptPwmCfg -> byActionPrd  << GPTB_ACT_PRD_POS  )
				  | ( ptPwmCfg -> byActionC1u  << GPTB_ACT_C1U_POS  )
				  | ( ptPwmCfg -> byActionC1d  << GPTB_ACT_C1D_POS  )
				  | ( ptPwmCfg -> byActionC2u  << GPTB_ACT_C2U_POS  )
				  | ( ptPwmCfg -> byActionC2d  << GPTB_ACT_C2D_POS  )
				  | ( ptPwmCfg -> byActionT1u  << GPTB_ACT_T1U_POS  )
				  | ( ptPwmCfg -> byActionT1d  << GPTB_ACT_T1D_POS  )
				  | ( ptPwmCfg -> byActionT2u  << GPTB_ACT_T2U_POS  )
				  | ( ptPwmCfg -> byActionT2d  << GPTB_ACT_T2D_POS  )
				  | ( ptPwmCfg -> byChoiceC1sel  << GPTB_C1SEL_POS  )
				  | ( ptPwmCfg -> byChoiceC2sel  << GPTB_C2SEL_POS  );
				  
	switch (eChannel)
	{	
		case (GPTB_CHANNEL_1):csp_gptb_set_aqcra(ptGptbBase,w_AQCRx_Val);
			break;
		case (GPTB_CHANNEL_2):csp_gptb_set_aqcrb(ptGptbBase,w_AQCRx_Val);
			break;

		default:return CSI_ERROR;
			break;
	}
	return CSI_OK;
}

/** \brief Channel CMPLDR configuration
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eLdmd: refer to csi_gptb_ldmd_e
 *  \param[in] eShdwldmd: refer to csi_gptb_shdwldmd_e
 *  \param[in] eChannel: refer to csi_gptb_comp_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_channel_cmpload_config(csp_gptb_t *ptGptbBase, csi_gptb_ldmd_e eLdmd, csi_gptb_shdwldmd_e eShdwldmd ,csi_gptb_comp_e eChannel)
{			  
	switch (eChannel)
	{	
		case (GPTB_COMPA):   ptGptbBase -> CMPLDR = (ptGptbBase -> CMPLDR &~(GPTB_CMPA_LD_MSK) )    |  (eLdmd    << GPTB_CMPA_LD_POS);
		                     ptGptbBase -> CMPLDR = (ptGptbBase -> CMPLDR &~(GPTB_CMPA_LDTIME_MSK) )|  (eShdwldmd <<GPTB_CMPA_LDTIME_POS);
			break;
		case (GPTB_COMPB):   ptGptbBase -> CMPLDR = (ptGptbBase -> CMPLDR &~(GPTB_CMPB_LD_MSK) )    |  (eLdmd    << GPTB_CMPB_LD_POS);
		                     ptGptbBase -> CMPLDR = (ptGptbBase -> CMPLDR &~(GPTB_CMPB_LDTIME_MSK) )|  (eShdwldmd << GPTB_CMPB_LDTIME_POS);
			break;

		default:return CSI_ERROR;
			break;
	}
	return CSI_OK;
}

/**
 \brief  DeadZoneTime configuration 
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptCfg           refer to csi_ept_deadzone_config_t
 \return CSI_OK /CSI_ERROR
*/
csi_error_t csi_gptb_dz_config(csp_gptb_t *ptGptbBase, csi_gptb_deadzone_config_t *ptCfg)
{  uint32_t w_Val;
   
	w_Val=csp_gptb_get_dbcr(ptGptbBase);	
	w_Val=(w_Val&~(GPTB_DCKSEL_MSK))  |(ptCfg-> byDcksel <<GPTB_DCKSEL_POS);
	w_Val=(w_Val&~(GPTB_CHA_DEDB_MSK))|(ptCfg-> byChaDedb<<GPTB_CHA_DEDB_POS);
	csp_gptb_set_dbcr( ptGptbBase, w_Val);	 
	csp_gptb_set_dpscr(ptGptbBase, ptCfg-> hwDpsc);
	
	w_Val=csi_get_pclk_freq();
	w_Val=(1000000000/(w_Val/(ptCfg->hwDpsc+1)));    //NS/(1/(48000000/(DPSC+1))*10^9) // 500NS/(1000/48) = 24;	
	csp_gptb_set_dbdtr(ptGptbBase	,ptCfg-> wRisingEdgeTime /w_Val);
	csp_gptb_set_dbdtf(ptGptbBase	,ptCfg-> wFallingEdgeTime/w_Val);
	
	return CSI_OK;	
}
 /**
 \brief  DeadZoneTime configuration loading 
 \param  ptGptbBase    	pointer of gptb register structure
 \param  eDbldr         refer to csi_gptb_dbldr_e
 \param  eDbldmd         refer to csi_gptb_dbldmd_e
 \param  eShdwdbldmd	       refer to csi_gptb_shdwdbldmd_e
 \return CSI_OK /CSI_ERROR
*/
csi_error_t csi_gptb_dbldrload_config(csp_gptb_t *ptGptbBase, csi_gptb_dbldr_e eDbldr,csi_gptb_dbldmd_e eDbldmd, csi_gptb_shdwdbldmd_e eShdwdbldmd)
{   uint32_t w_Val;
	w_Val=csp_gptb_get_dbldr(ptGptbBase);
	switch (eDbldr)
	{	
		case (GPTB_DBCR) :w_Val=( w_Val &~(GPTB_DBLDR_CRSHDWEN_MSK) )|(eDbldmd << GPTB_DBLDR_CRSHDWEN_POS);
		             w_Val=( w_Val &~(GPTB_DBLDR_LDCRMODE_MSK))|(eShdwdbldmd << GPTB_DBLDR_LDCRMODE_POS);
			break;
		case (GPTB_DBDTR):w_Val=( w_Val &~GPTB_DBLDR_SHDWDTR_MSK )|(eDbldmd << GPTB_DBLDR_SHDWDTR_POS );
		             w_Val=( w_Val &~GPTB_DBLDR_LDDTRMD_MSK )|(eShdwdbldmd << GPTB_DBLDR_LDDTRMD_POS);
		    break;
		case (GPTB_DBDTF):w_Val=( w_Val &~GPTB_DBLDR_SHDWDTF_MSK )|(eDbldmd << GPTB_DBLDR_SHDWDTF_POS);
		             w_Val=( w_Val &~GPTB_DBLDR_LDDTFMD_MSK )|(eShdwdbldmd <<GPTB_DBLDR_LDDTFMD_POS);
            break;
		case (GPTB_DCKPSC):w_Val=( w_Val &~GPTB_DBLDR_SHDWPSC_MSK)|(eDbldmd << GPTB_DBLDR_SHDWPSC_POS);
		              w_Val=( w_Val &~GPTB_DBLDR_LDPSCMD_MSK)|(eShdwdbldmd << GPTB_DBLDR_LDPSCMD_POS);
		    break;
		default:return CSI_ERROR;
			break;
	}
	csp_gptb_set_dbldr(ptGptbBase,w_Val);
			
	return CSI_OK;
}   
 /**
 \brief  DeadZoneTime configuration 
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptCfg           refer to csi_gptb_deadzone_config_t
 \return CSI_OK /CSI_ERROR
*/
csi_error_t csi_gptb_dbcr_config(csp_gptb_t *ptGptbBase, csi_gptb_deadzone_config_t *ptCfg)
{  uint32_t w_Val;
	w_Val=csp_gptb_get_dbcr(ptGptbBase);	
	w_Val=(w_Val&~(GPTB_DCKSEL_MSK))|(ptCfg-> byDcksel <<GPTB_DCKSEL_POS);
	w_Val=(w_Val&~(GPTB_CHA_DEDB_MSK))|(ptCfg-> byChaDedb<<GPTB_CHA_DEDB_POS);
	csp_gptb_set_dbcr( ptGptbBase, w_Val);	 
	csp_gptb_set_dpscr(ptGptbBase	,ptCfg-> hwDpsc);
	csp_gptb_set_dbdtr(ptGptbBase	,ptCfg-> wRisingEdgeTime);
	csp_gptb_set_dbdtf(ptGptbBase	,ptCfg-> wFallingEdgeTime);
	return CSI_OK;	
}
 /**
 \brief  channelmode configuration 
 \param  ptGptbBase    pointer of gptb register structure
 \param  ptCfg         refer to csi_gptb_deadzone_config_t
 \param  eChannel         refer to csi_gptb_channel_e
 \return CSI_OK /CSI_ERROR  
*/
csi_error_t csi_gptb_channelmode_config(csp_gptb_t *ptGptbBase,csi_gptb_deadzone_config_t *ptCfg,csi_gptb_channel_e eChannel)
{    uint32_t w_Val;
     w_Val=csp_gptb_get_dbcr(ptGptbBase);	
	 switch (eChannel)
	{	
		case (GPTB_CHANNEL_1): w_Val=(w_Val&~(DB_CHA_OUTSEL_MSK)) |(ptCfg-> byChxOuselS1S0   <<DB_CHA_OUTSEL_POS);
		                      w_Val=(w_Val&~(DB_CHA_POL_MSK))    |(ptCfg-> byChxPolarityS3S2<<DB_CHA_POL_POS);
							  w_Val=(w_Val&~(DB_CHA_INSEL_MSK))  |(ptCfg-> byChxInselS5S4   <<DB_CHA_INSEL_POS);
							  w_Val=(w_Val&~(DB_CHA_OUTSWAP_MSK))|(ptCfg-> byChxOutSwapS8S7 <<DB_CHA_OUTSWAP_POS);
			break;

		default:return CSI_ERROR;
			break;
	}
	csp_gptb_set_dbcr( ptGptbBase, w_Val);
	return CSI_OK;
}

/**
 \brief  State of emergency configuration 
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptCfg           refer to csi_gptb_emergency_config_t
 \return CSI_OK /CSI_ERROR
//*/
csi_error_t csi_gptb_emergency_cfg(csp_gptb_t *ptGptbBase, csi_gptb_emergency_config_t *ptCfg)
{ uint32_t wEmsrc;
  uint32_t wEmsrc2;
  uint32_t wEmpol;
  uint32_t wEmecr;
	
	if(ptCfg -> byEpxInt==GPTB_ORL0)
	{
		wEmsrc2=csp_gptb_get_src2(ptGptbBase);
		wEmsrc2=(wEmsrc2 & (~GPTB_EPPACE0_MSK)) | (ptCfg -> byFltpace0  << GPTB_EPPACE0_POS);
		wEmsrc2=(wEmsrc2 &~0xff)     |  ptCfg ->byOrl0 ;
		csp_gptb_set_src2(ptGptbBase,wEmsrc2);
	}
    
	if(ptCfg -> byEpxInt==GPTB_ORL1)	
	{
		wEmsrc2=csp_gptb_get_src2(ptGptbBase);
		wEmsrc2=(wEmsrc2  & (~GPTB_EPPACE1_MSK) ) | (ptCfg -> byFltpace1 << GPTB_EPPACE1_POS);
		wEmsrc2=(wEmsrc2 &~0xff0000) |  ptCfg ->byOrl1 <<16;
		csp_gptb_set_src2(ptGptbBase,wEmsrc2);
	}
	
	wEmsrc = csp_gptb_get_src(ptGptbBase);
    wEmsrc=(  wEmsrc &~ GPTB_SEL_MSK_EP(ptCfg -> byEpx) )|( ptCfg -> byEpxInt  << GPTB_SEL_POS_EP(ptCfg -> byEpx));
    csp_gptb_set_src(ptGptbBase,wEmsrc);
	
	if(ptCfg ->byEpxInt>0)
	{
		wEmpol=csp_gptb_get_empol(ptGptbBase);	
		switch (ptCfg ->byEpxInt)
		{    
			case (GPTB_EBI0):  wEmpol=( wEmpol  &~ GPTB_POL_MSK_EBI(0)) | (ptCfg -> byPolEbix <<GPTB_POL_POS_EBI(0) );break;
			case (GPTB_EBI1):  wEmpol=( wEmpol  &~ GPTB_POL_MSK_EBI(1)) | (ptCfg -> byPolEbix <<GPTB_POL_POS_EBI(1) );break;
			case (GPTB_EBI2):  wEmpol=( wEmpol  &~ GPTB_POL_MSK_EBI(2)) | (ptCfg -> byPolEbix <<GPTB_POL_POS_EBI(2) );break;
			case (GPTB_EBI3):  wEmpol=( wEmpol  &~ GPTB_POL_MSK_EBI(3)) | (ptCfg -> byPolEbix <<GPTB_POL_POS_EBI(3) );break;
			case (GPTB_CMP0):  wEmpol=( wEmpol  &~ GPTB_POL_MSK_EBI(4)) | (ptCfg -> byPolEbix <<GPTB_POL_POS_EBI(4) );break;
			case (GPTB_CMP1):  wEmpol=( wEmpol  &~ GPTB_POL_MSK_EBI(5)) | (ptCfg -> byPolEbix <<GPTB_POL_POS_EBI(5) );break;
			case (GPTB_CMP2):  wEmpol=( wEmpol  &~ GPTB_POL_MSK_EBI(6)) | (ptCfg -> byPolEbix <<GPTB_POL_POS_EBI(6) );break;
			case (GPTB_ORL0):  break;
			case (GPTB_ORL1):  break;
			default:return CSI_ERROR;break;
		}
		csp_gptb_set_empol(ptGptbBase,wEmpol);
	}

	if(ptCfg ->byEpxLckmd)
	{
		wEmecr =  csp_gptb_get_emecr(ptGptbBase);	
		wEmecr =(wEmecr & (~GPTB_LCKMD_MSK_EP(ptCfg ->byEpx))) | (   ptCfg ->byEpxLckmd      <<  GPTB_LCKMD_POS_EP(ptCfg ->byEpx));
		wEmecr =(wEmecr & (~GPTB_EMECR_OSRSHDW_MSK         )) | (  (ptCfg ->byOsrshdw&0x01) <<  GPTB_EMECR_OSRSHDW_POS         );
		wEmecr =(wEmecr & (~GPTB_EMECR_OSRLDMD_MSK         )) | (   ptCfg ->byOsrldmd       <<  GPTB_EMECR_OSRLDMD_POS         );
		wEmecr =(wEmecr & (~GPTB_EMECR_SLCLRMD_MSK         )) | (   ptCfg ->bySlclrmd       <<  GPTB_EMECR_SLCLRMD_POS         ); 
		csp_gptb_set_emecr(ptGptbBase,wEmecr);
	}
    
	return CSI_OK;
}
/**
 \brief  State of emergency configuration 
 \param  ptGptbBase    	pointer of gptb register structure
 \param  eOsrch         refer to csi_gptb_osrchx_e
 \param  eEmout         refer to csi_gptb_emout_e
 \return CSI_OK /CSI_ERROR
//*/
csi_error_t csi_gptb_emergency_pinxout(csp_gptb_t *ptGptbBase,csi_gptb_osrchx_e  eOsrch ,csi_gptb_emout_e eEmout)
{ uint32_t wEmosr;
    wEmosr=csp_gptb_get_emosr(ptGptbBase);	
	 switch (eOsrch)
	 {    case (GPTB_EMCOAX): wEmosr=( wEmosr &~(GPTB_EMOSR_EM_COAX_MSK) )|( eEmout <<GPTB_EMOSR_EM_COAX_POS);break;
		  case (GPTB_EMCOBX): wEmosr=( wEmosr &~(GPTB_EMOSR_EM_COBX_MSK) )|( eEmout <<GPTB_EMOSR_EM_COBX_POS);break;
		  case (GPTB_EMCOAY): wEmosr=( wEmosr &~(GPTB_EMOSR_EM_COAY_MSK) )|( eEmout <<GPTB_EMOSR_EM_COAY_POS);break;		
		  default:return CSI_ERROR;break;
	 }
    csp_gptb_set_emosr(ptGptbBase,wEmosr);
	return CSI_OK;
}

/** \brief Channel AQLDR configuration
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eLdmd: refer to csi_gptb_ldmd_e
 *  \param[in] eShdwldmd: refer to csi_gptb_shdwldmd_e
 *  \param[in] eChannel: refer to csi_gptb_channel_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_channel_aqload_config(csp_gptb_t *ptGptbBase, csi_gptb_ldmd_e eLdmd, csi_gptb_shdwldmd_e eShdwldmd ,csi_gptb_channel_e eChannel)
{			  
	switch (eChannel)
	{	
		case (GPTB_CHANNEL_1):ptGptbBase -> AQLDR = (ptGptbBase -> AQLDR &~(GPTB_AQCR1_SHDWEN_MSK) )|  (eLdmd << GPTB_AQCR1_SHDWEN_POS);
		                      ptGptbBase -> AQLDR = (ptGptbBase -> AQLDR &~(GPTB_LDAMD_MSK) )|  (eShdwldmd << GPTB_LDAMD_POS);
			break;
		case (GPTB_CHANNEL_2):ptGptbBase -> AQLDR = (ptGptbBase -> AQLDR &~(GPTB_AQCR2_SHDWEN_MSK) )|  (eLdmd << GPTB_AQCR2_SHDWEN_POS);
		                      ptGptbBase -> AQLDR = (ptGptbBase -> AQLDR &~(GPTB_LDBMD_MSK) )|  (eShdwldmd << GPTB_LDBMD_POS);
			break;

		default:return CSI_ERROR;
			break;
	}
	return CSI_OK;
}

/**
 \brief  gptb configuration Loading
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptGlobal       refer to csi_gptb_Global_load_control_config_t
 \return CSI_OK /CSI_ERROR
*/
csi_error_t csi_gptb_global_config(csp_gptb_t *ptGptbBase,csi_gptb_Global_load_control_config_t *ptGlobal)
{   uint32_t wGLDCR;	
	wGLDCR =0;
    wGLDCR = (wGLDCR &~GPTB_GLDEN_MSK)       | ((ptGlobal->bGlden & 0x01)<<GPTB_GLDEN_POS);
	wGLDCR = (wGLDCR &~GPTB_GLDMD_MSK)       | ((ptGlobal->byGldmd & 0x0f)<<GPTB_GLDMD_POS);
	wGLDCR = (wGLDCR &~GPTB_GLDCR_OSTMD_MSK) | ((ptGlobal->bOstmd & 0x01)<<GPTB_GLDCR_OSTMD_POS);
	wGLDCR = (wGLDCR &~GPTB_GLDPRD_MSK)      | ((ptGlobal->byGldprd & 0x07)<<GPTB_GLDPRD_POS);
	csp_gptb_set_gldcr(ptGptbBase,wGLDCR);	
	return CSI_OK;
}

/** \brief GLDCFG loading
 * 
 *  \param[in] ptGPTBBase of GPTB register structure
 *  \param[in] Glo:  csi_GPTB_Global_load_gldcfg  
 *  \param[in] bEnable ENABLE or DISABLE
 *  \return CSI_OK
 */
csi_error_t csi_gptb_gldcfg(csp_gptb_t *ptGptbBase ,csi_gptb_Global_load_gldcfg_e eGlo,bool bEnable)
{
   	switch (eGlo)
	{	
		case (GPTB_GLO_PRDR): ptGptbBase  -> GLDCFG   = (ptGptbBase -> GLDCFG  & ~(GPTB_LD_PRDR_MSK))   |(bEnable << GPTB_LD_PRDR_POS) ;
			break;
		case (GPTB_GLO_CMPA): ptGptbBase  -> GLDCFG   = (ptGptbBase -> GLDCFG  & ~(GPTB_LD_CMPA_MSK))   |(bEnable << GPTB_LD_CMPA_POS) ;
			break;
		case (GPTB_GLO_CMPB): ptGptbBase  -> GLDCFG   = (ptGptbBase -> GLDCFG  & ~(GPTB_LD_CMPB_MSK))   |(bEnable << GPTB_LD_CMPB_POS) ;
		    break;
		case (GPTB_GLO_DBDTR): ptGptbBase -> GLDCFG   = (ptGptbBase -> GLDCFG  & ~(GPTB_LD_DBDTR_MSK))  |(bEnable << GPTB_LD_DBDTR_POS) ;
			break;
		case (GPTB_GLO_DBDTF): ptGptbBase -> GLDCFG   = (ptGptbBase -> GLDCFG  & ~(GPTB_LD_DBDTF_MSK))  |(bEnable << GPTB_LD_DBDTF_POS) ;
			break;
		case (GPTB_GLO_DBCR): ptGptbBase  -> GLDCFG   = (ptGptbBase -> GLDCFG  & ~(GPTB_LD_DBCR_MSK))   |(bEnable << GPTB_LD_DBCR_POS) ;
		    break;
		case (GPTB_GLO_AQCR1):ptGptbBase  -> GLDCFG   = (ptGptbBase -> GLDCFG  & ~(GPTB_LD_AQCR1_MSK))  |(bEnable << GPTB_LD_AQCR1_POS );
		    break;
		case (GPTB_GLO_AQCR2):ptGptbBase  -> GLDCFG   = (ptGptbBase -> GLDCFG  & ~(GPTB_LD_AQCR2_MSK))  |(bEnable << GPTB_LD_AQCR2_POS );
		    break;
	    case (GPTB_GLO_AQCSF):ptGptbBase  -> GLDCFG   = (ptGptbBase -> GLDCFG  & ~(GPTB_LD_AQCSF_MSK))  |(bEnable << GPTB_LD_AQCSF_POS );
			 break;
		case (GPTB_GLO_EMOSR):ptGptbBase  -> GLDCFG   = (ptGptbBase -> GLDCFG  & ~(GPTB_LD_EMOSR_MSK))  |(bEnable << GPTB_LD_EMOSR_POS );
			 break;
		default: return CSI_ERROR;
			break;
	}   
	return CSI_OK;
}

/**
 \brief  Software trigger loading
 \param  ptGptbBase    	pointer of gptb register structure
 \return CSI_OK
*/
csi_error_t csi_gptb_global_sw(csp_gptb_t *ptGptbBase)
{
	csp_gptb_set_gldcr2(ptGptbBase,GPTB_SW_GLD);
	return CSI_OK;
}
/**
 \brief  rearm  loading
 \param  ptGptbBase    	pointer of gptb register structure
 \return CSI_OK
*/
csi_error_t csi_gptb_global_rearm(csp_gptb_t *ptGptbBase)
{
	csp_gptb_set_gldcr2(ptGptbBase,GPTB_OSREARM_EN);
	return CSI_OK;
}
/** \brief start gptb
 *  \param ptGptbBase:  pointer of bt register structure
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_gptb_start(csp_gptb_t *ptGptbBase)
{   
	csp_gptb_wr_key(ptGptbBase); 
	csp_gptb_start(ptGptbBase);
	return CSI_OK;
}
/**
 \brief SW stop GPTB counter
 \param ptGptbBase    pointer of gptb register structure
*/
void csi_gptb_swstop(csp_gptb_t *ptGptbBase)
{
  csp_gptb_wr_key(ptGptbBase);
  csp_gptb_stop(ptGptbBase);
}
/**
 \brief set GPTB start mode. 
 \param ptGptbBase    pointer of gptb register structure
 \return eMode 隆锚oGPTB_SW/GPTB_SYNC
*/
void csi_gptb_set_start_mode(csp_gptb_t *ptGptbBase, csi_gptb_stmd_e eMode)
{
	csp_gptb_set_start_src(ptGptbBase, (csp_gptb_startsrc_e)eMode);
}
/**
 \brief set GPTB operation mode
 \param ptGptbBase    pointer of gptb register structure
 \param eMode 	 GPTB_OP_CONT/GPTB_OP_OT
*/
void csi_gptb_set_os_mode(csp_gptb_t *ptGptbBase, csi_gptb_opmd_e eMode)
{
	csp_gptb_set_opmd(ptGptbBase, (csp_gptb_opmd_e)eMode);
}

/**
 \brief set GPTB stop status
 \param ptGptbBase    pointer of gptb register structure
 \param eSt 	 GPTB_STP_HZ/GPTB_STP_LOW
*/
void csi_gptb_set_stop_st(csp_gptb_t *ptGptbBase, csi_gptb_stpst_e eSt)
{	
  csp_gptb_set_stop_st(ptGptbBase,(csp_gptb_stpst_e)eSt);
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

/** \brief  update gptb PRDR and CMPx reg value
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eComp: select which COMP to set(COMPA or COMPB)
 *  \param[in] hwPrdr: gptb PRDR reg  value
 *  \param[in] hwCmp: gptb COMP reg value
 *  \return none
 */
csi_error_t csi_gptb_prdr_cmp_update(csp_gptb_t *ptGptbBase,csi_gptb_comp_e eComp, uint16_t hwPrdr, uint16_t hwCmp) 
{
	csp_gptb_set_prdr(ptGptbBase, (uint16_t)hwPrdr);		//set GPTB PRDR Value
	switch (eComp)
	{	
		case (GPTB_COMPA):
			csp_gptb_set_cmpa(ptGptbBase, (uint16_t)hwCmp);	//set GPTB COMPA Value
			break;
			
		case (GPTB_COMPB):
			csp_gptb_set_cmpb(ptGptbBase, (uint16_t)hwCmp);	//set GPTB COMPB Value
			break;

		default: 
			return CSI_ERROR;
			break;
	}
    return (CSI_OK);
}

/**
 \brief change gptb output dutycycle. 
 \param ptGptbBase   pointer of gptb register structure
 \param eCh          refer to csi_gptb_comp_e
 \param wDuty        duty of PWM:0%-100%
*/
csi_error_t csi_gptb_change_ch_duty(csp_gptb_t *ptGptbBase, csi_gptb_comp_e eCh, uint32_t wDuty)
{ 
	uint16_t  hwCmpLoad;
	
	uint16_t  hwPrd;
    hwPrd = csp_gptb_get_prd(ptGptbBase);
	if(wDuty >= 100)
	{
		hwCmpLoad = 0;
	}
	else if(wDuty == 0)
	{
		hwCmpLoad = hwPrd+1;
	}
	else
	{
		hwCmpLoad = hwPrd - ( hwPrd * wDuty / 100 );
	}
    
	switch (eCh)
	{	
		case (GPTB_COMPA):csp_gptb_set_cmpa(ptGptbBase, (uint16_t)hwCmpLoad);
			break;
		case (GPTB_COMPB):csp_gptb_set_cmpb(ptGptbBase, (uint16_t)hwCmpLoad);
			break;

		default: return (CSI_ERROR);
			break;
	}
    return (CSI_OK);
}

/**
 \brief get harklock status
 \param ptEpt    pointer of gptb register structure
 \return uint8_t 0 or 1
*/
uint8_t csi_gptb_get_hdlck_st(csp_gptb_t *ptGptbBase)
{	
	return (csp_gptb_get_emHdlck(ptGptbBase));
}

/**
 \brief clear harklock status
 \param ptEpt   pointer of gptb register structure
 \return  eEp 	 external emergency input: csi_gptb_ep_e                 
*/
void csi_gptb_clr_hdlck(csp_gptb_t *ptGptbBase, csi_gptb_ep_e eEp)
{
	csp_gptb_clr_emHdlck(ptGptbBase, (csp_gptb_ep_e)eEp);
}

/**
 \brief get 	 softlock status
 \param ptEpt    pointer of gptb register structure
 \return uint8_t 0 or 1
*/
uint8_t csi_gptb_get_sftlck_st(csp_gptb_t *ptGptbBase)
{	
	return (csp_gptb_get_emSdlck(ptGptbBase));
}

/**
 \brief clear softlock status
 \param ptEpt   pointer of gptb register structure
 \return  eEp 	 external emergency input: csi_gptb_ep_e
*/
void csi_gptb_clr_sftlck(csp_gptb_t *ptGptbBase, csi_gptb_ep_e eEp)
{	
	csp_gptb_clr_emSdlck(ptGptbBase, (csp_gptb_ep_e)eEp);
}

/** \brief software force emergency
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eEp: external emergency input: GPTB_EP0~3
 *  \return none
 */
void csi_gptb_force_em(csp_gptb_t *ptGptbBase, csi_gptb_ep_e eEp)
{
	csp_gptb_force_em(ptGptbBase, (csp_gptb_ep_e)eEp);
}

/**
  \brief       enable/disable gptb in debug mode
  \param[in]   ptEpt      pointer of gptb register structure
  \param[in]   bEnable		ENABLE/DISABLE
*/

void csi_gptb_debug_enable(csp_gptb_t *ptGptbBase, bool bEnable)
{
	csp_gptb_dbg_enable(ptGptbBase, bEnable);
}

/**
  \brief       enable/disable gptb emergencyinterruption
  \param[in]   ptGptbBase       pointer of gptb register structure
  \param[in]   eEmint		       refer to csi_gptb_emint_e
*/
void csi_gptb_emint_en(csp_gptb_t *ptGptbBase, csi_gptb_emint_e eEmint)
{
    csp_gptb_Emergency_emimcr(ptGptbBase,(csp_gptb_emint_e)eEmint);
	csi_irq_enable((uint32_t *)ptGptbBase);	
}

/**
  \brief   enable/disable gptb out trigger 
  \param   ptGptbBase   pointer of gptb register structure
  \param   byCh			0/1
  \param   bEnable		ENABLE/DISABLE
*/
csi_error_t csi_gptb_evtrg_enable(csp_gptb_t *ptGptbBase, uint8_t byCh, bool bEnable)
{	
	if (byCh > 1)return CSI_ERROR;
    csp_gptb_trg_xoe_enable(ptGptbBase, byCh, bEnable);
	return CSI_OK;
}

/**
  \brief   One time software output 
  \param   ptGptbBase      pointer of gptb register structure 
  \param   byCh	         GPTB_OSTSFA/GPTB_OSTSFB		
  \param   eAction 		GPTB_LDAQCR_ZRO/GPTB_LDAQCR_PRD/GPTB_LDAQCR_ZROPRD
*/
csi_error_t csi_gptb_onetimesoftwareforce_output(csp_gptb_t *ptGptbBase, csi_gptb_channel_e eChannel, csi_gptb_action_e eAction)
{	
	switch (eChannel)
	{
	case GPTB_CHANNEL_1: 
		ptGptbBase ->AQOSF |= GPTB_OSTSFA;
		ptGptbBase ->AQOSF = (ptGptbBase ->AQOSF &~(GPTB_ACTA_MSK))|((eAction&0x03)<<GPTB_ACTA_POS);
		break;
	
	case GPTB_CHANNEL_2:
		ptGptbBase ->AQOSF |= GPTB_OSTSFB;
		ptGptbBase ->AQOSF = (ptGptbBase ->AQOSF &~(GPTB_ACTB_MSK))|((eAction&0x03)<<GPTB_ACTB_POS);
		break;	

	default: 
		return CSI_ERROR;
		break;
    }
	return CSI_OK;
}
/** \brief  Continuous software waveform loading control
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eLoadtime:    refer to csi_gptb_aqosf_e
 *  \return  none
 */
void csi_gptb_loading_method_aqcsf(csp_gptb_t *ptGptbBase, csi_gptb_aqosf_e eLoadtime)
{
	ptGptbBase ->AQOSF  = (ptGptbBase ->AQOSF &~(GPTB_AQCSF_LDTIME_MSK))|((eLoadtime&0x03)<<GPTB_AQCSF_LDTIME_POS);
}
/** \brief Continuous software waveform control
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] byCh        refer to csi_gptb_channel_e
 *  \param[in] eAction:    refer to  csi_gptb_aqosf_e
 *  \return  none
 */
csi_error_t csi_gptb_continuoussoftwareforce_output(csp_gptb_t *ptGptbBase, csi_gptb_channel_e eChannel, csi_gptb_aqcsf_e eAction)
{
	
	switch (eChannel){
	case GPTB_CHANNEL_1:  ptGptbBase ->AQCSF = (ptGptbBase ->AQCSF &~(0x03))|(eAction&0x03);            
	     break;
	case GPTB_CHANNEL_2:  ptGptbBase ->AQCSF = (ptGptbBase ->AQCSF &~(0x0c))|(eAction&0x03)<<2;
	     break;	

	default: return CSI_ERROR;
	     break;
    }		
	return CSI_OK;
}

/** \brief gptb  input  config  
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eInt:     refer to to csp_gptb_int_e
 *  \param[in] bEnable:  ENABLE/DISABLE
 *  \return CSI_OK;
 */
csi_error_t csi_gptb_int_enable(csp_gptb_t *ptGptbBase, csi_gptb_int_e eInt, bool bEnable)
{  
	csi_irq_enable((uint32_t *)ptGptbBase);							//enable  irq
	csp_gptb_int_enable(ptGptbBase,(csp_gptb_int_e)eInt,bEnable);
	return CSI_OK;
}

/** \brief gptb sync input evtrg config  
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eTrgin: gptb sync evtrg input channel(0~6)
 *  \param[in] eTrgMode: gptb sync evtrg mode, continuous/once
 *  \param[in] eAutoRearm: refer to csi_gptb_arearm_e 
 *  \return none
 */
void csi_gptb_set_sync(csp_gptb_t *ptGptbBase, csi_gptb_trgin_e eTrgIn, csi_gptb_trgmode_e eTrgMode, csi_gptb_arearm_e eAutoRearm)
{
	csp_gptb_set_sync_mode(ptGptbBase, eTrgIn, (csp_gptb_syncmd_e)eTrgMode);
	csp_gptb_set_auto_rearm(ptGptbBase, (csp_gptb_arearm_e)eAutoRearm);
	csp_gptb_sync_enable(ptGptbBase, eTrgIn, ENABLE);
}

/** \brief gptb extsync input select
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eTrgin: gptb sync evtrg input channel(0~6)
 *  \param[in] byTrgChx: trgxsel channel(0~1)
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_set_extsync_chnl(csp_gptb_t *ptGptbBase, csi_gptb_trgin_e eTrgIn, csi_gptb_syncrout_e eTrgChx)
{
	switch(eTrgChx)
	{
		case 0:
			csp_gptb_sync_trg0sel(ptGptbBase, eTrgIn);
			break;
		case 1:
			csp_gptb_sync_trg1sel(ptGptbBase, eTrgIn);
			break;
		default:
			return CSI_ERROR;
		
	}
	return CSI_OK;
}

/** \brief gptb sync input filter config  
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] ptFilter: pointer of sync input filter parameter config structure
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_set_sync_filter(csp_gptb_t *ptGptbBase, csi_gptb_filter_config_t *ptFilter)
{
	uint32_t wFiltCr;
	uint32_t wWindow;
	
	if(ptFilter->byFiltSrc > GPTB_TRGIN_SYNCEN6)
		return CSI_ERROR;
	wFiltCr = ptFilter->byFiltSrc | (ptFilter->byWinInv << GPTB_FLTBLKINV_POS) | 
			(ptFilter->byWinAlign << GPTB_ALIGNMD_POS) | (ptFilter->byWinCross << GPTB_CROSSMD_POS);
	wWindow = ptFilter->hwWinOffset | (ptFilter->hwWinWidth << GPTB_FLT_WDW_POS);
	
	csp_gptb_set_trgftcr(ptGptbBase, wFiltCr);
	csp_gptb_set_trgftwr(ptGptbBase, wWindow);
	
	return CSI_OK;
}
/** \brief rearm gptb sync evtrg  
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eTrgin: gptb sync evtrg input channel(0~6)
 *  \return none
 */
void csi_gptb_rearm_sync(csp_gptb_t *ptGptbBase,csi_gptb_trgin_e eTrgin)
{
	csp_gptb_rearm_sync(ptGptbBase, eTrgin);
}
/** \brief gptb evtrg output config
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] byTrgOut: evtrg out port(0~1)
 *  \param[in] eTrgSrc: evtrg source(1~15) 
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_set_evtrg(csp_gptb_t *ptGptbBase, csi_gptb_trgout_e eTrgOut, csi_gptb_trgsrc_e eTrgSrc)
{
	csp_gptb_set_trgsel01(ptGptbBase, eTrgOut, (csp_gptb_trgsrc_e)eTrgSrc);			    
	csp_gptb_trg_xoe_enable(ptGptbBase, eTrgOut, ENABLE);				//evtrg out enable
	
	return CSI_OK;//
}


/** \brief gptb evtrg cntxinit control
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] byCntChx: evtrg countinit channel(0~1)
 *  \param[in] byCntVal: evtrg cnt value(1~16)
 *  \param[in] byCntInitVal: evtrg cntxinit value(1~16)
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_set_evcntinit(csp_gptb_t *ptGptbBase, csi_gptb_cntinit_e eCntChx, uint8_t byCntVal, uint8_t byCntInitVal)
{

 if(eCntChx > GPTB_CNT1INIT)
  return CSI_ERROR;
 
 csp_gptb_set_trgprd(ptGptbBase, eCntChx, byCntVal - 1);    //evtrg count
 csp_gptb_trg_cntxinit(ptGptbBase, eCntChx, byCntInitVal);
 csp_gptb_trg_cntxiniten_enable(ptGptbBase, eCntChx, ENABLE);
 
 return CSI_OK;
}

/**
 \brief  gptb configuration Loading
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptCfg           refer to csi_gptb_reglk_config_t
 \return CSI_OK /CSI_ERROR
*/
csi_error_t csi_gptb_reglk_config(csp_gptb_t *ptGptbBase,csi_gptb_reglk_config_t *ptGlobal)
{   uint32_t wRegLk;	
	wRegLk =0;
    wRegLk = (wRegLk & ~GPTB_PRDR_MSK )| ((ptGlobal-> byPrdr & 0xF)<< GPTB_PRDR_POS);
	wRegLk = (wRegLk & ~GPTB_CMPA_MSK )| ((ptGlobal-> byCmpa & 0xF)<< GPTB_CMPA_POS);
	wRegLk = (wRegLk & ~GPTB_CMPB_MSK )| ((ptGlobal-> byCmpb & 0xF)<< GPTB_CMPB_POS);
	wRegLk = (wRegLk & ~GPTB_GLD2_MSK )| ((ptGlobal-> byGld2 & 0xF)<< GPTB_GLD2_POS);
	wRegLk = (wRegLk & ~GPTB_RSSR_MSK )| ((ptGlobal-> byRssr & 0xF)<< GPTB_RSSR_POS);
	csp_gptb_set_reglk(ptGptbBase,wRegLk);
	wRegLk =0;
	wRegLk = (wRegLk & ~GPTB_EMSLCLR_MSK )| ((ptGlobal-> byEmslclr & 0xF)<< GPTB_EMSLCLR_POS);
	wRegLk = (wRegLk & ~GPTB_EMHLCLR_MSK )| ((ptGlobal-> byEmhlclr & 0xF)<< GPTB_EMHLCLR_POS);
	wRegLk = (wRegLk & ~GPTB_EMICR_MSK )  | ((ptGlobal-> byEmicr   & 0xF)<< GPTB_EMICR_POS);
	wRegLk = (wRegLk & ~GPTB_EMFRCR_MSK ) | ((ptGlobal-> byEmfrcr  & 0xF)<< GPTB_EMFRCR_POS);
	wRegLk = (wRegLk & ~GPTB_AQOSF_MSK )  | ((ptGlobal-> byAqosf   & 0xF)<< GPTB_AQOSF_POS);
	wRegLk = (wRegLk & ~GPTB_AQCSF_MSK )  | ((ptGlobal-> byAqcsf   & 0xF)<< GPTB_AQCSF_POS);
	csp_gptb_set_reglk2(ptGptbBase,wRegLk);	
	return CSI_OK;
}
