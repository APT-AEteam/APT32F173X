/***********************************************************************//** 

 * *********************************************************************
*/
#include "sys_clk.h"
#include "drv/common.h"
#include "drv/gptb.h"
#include "csp_gptb.h"
#include "drv/pin.h"
#include "drv/irq.h"
#include "drv/etb.h"


//to maximize the  speed
 uint32_t gGptb0Prd;
 uint32_t gGptb1Prd;
 uint32_t gGptb2Prd;
 uint32_t gGptb3Prd;
 uint32_t gGptb4Prd;
 uint32_t gGptb5Prd;


 /**
 \brief  Basic configuration
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptgptbPwmCfg   	refer to csi_gptb_config_t
 \return CSI_OK/CSI_ERROR
*/
csi_error_t csi_gptb_config_init(csp_gptb_t *ptGptbBase, csi_gptb_config_t *ptgptbPwmCfg)
{
	uint32_t wClkDiv;
	uint32_t wCrVal;
	uint32_t wCmpLoad; 
	uint32_t wPrdrLoad; 
	
	if(ptgptbPwmCfg->wFreq == 0 ){ptgptbPwmCfg->wFreq =100;}
	
	
	csi_clk_enable((uint32_t *)ptGptbBase);								// clk enable   ??????????????
	
	csp_gptb_clken(ptGptbBase);
	csp_gptb_wr_key(ptGptbBase);                                           //Unlocking
	csp_gptb_reset(ptGptbBase);											// reset 
	
	wClkDiv = (csi_get_pclk_freq() / ptgptbPwmCfg->wFreq / 60000);		// clk div value
	if(wClkDiv == 0)wClkDiv = 1;
	
	wPrdrLoad  = (csi_get_pclk_freq()/ptgptbPwmCfg->wFreq/wClkDiv);	    //prdr load value
			
	wCrVal =ptgptbPwmCfg->byCountingMode | (ptgptbPwmCfg->byStartSrc<<GPTB_STARTSRC_POS) |
	        ptgptbPwmCfg->byOneshotMode<<GPTB_OPMD_POS | (ptgptbPwmCfg->byWorkmod<<GPTB_MODE_POS);
    
	wCrVal=(wCrVal & ~(GPTB_PSCLD_MSK))   |((ptgptbPwmCfg->byPscld&0x03)   <<GPTB_PSCLD_POS);
	
	if(ptgptbPwmCfg->byWorkmod==GPTB_CAPTURE)
	{
	 	wCrVal=(wCrVal & ~(GPTB_CAPMD_MSK))   |((ptgptbPwmCfg->byCaptureCapmd&0x01)   <<GPTB_CAPMD_POS);
		wCrVal=(wCrVal & ~(GPTB_STOPWRAP_MSK))|((ptgptbPwmCfg->byCaptureStopWrap&0x03)<<GPTB_STOPWRAP_POS);
//		wCrVal=(wCrVal & ~(GPTB_CMPA_RST_MSK))|((ptgptbPwmCfg->byCaptureLdaret&0x01)  <<GPTB_CMPA_RST_POS);
//		wCrVal=(wCrVal & ~(GPTB_CMPB_RST_MSK))|((ptgptbPwmCfg->byCaptureLdbret&0x01)  <<GPTB_CMPB_RST_POS);
//		wCrVal=(wCrVal & ~(GPTB_CMPC_RST_MSK))|((ptgptbPwmCfg->byCaptureLdcret&0x01)  <<GPTB_CMPC_RST_POS);
//		wCrVal=(wCrVal & ~(GPTB_CMPD_RST_MSK))|((ptgptbPwmCfg->byCaptureLddret&0x01)  <<GPTB_CMPD_RST_POS);
		
		if(ptgptbPwmCfg->byCaptureCapLden)wCrVal|=GPTB_CAPLD_EN;
		if(ptgptbPwmCfg->byCaptureRearm)  wCrVal|=GPTB_CAPREARM;
		
		wPrdrLoad=0xFFFF;
	}
	
	if(ptgptbPwmCfg->byBurst)
	{
		wCrVal=(wCrVal & ~(GPTB_CGSRC_MSK))|((ptgptbPwmCfg->byCgsrc&0x03)<<GPTB_CGSRC_POS);
		wCrVal=(wCrVal & ~(GPTB_CGFLT_MSK))|((ptgptbPwmCfg->byCgflt&0x07)<<GPTB_CGFLT_POS);
	}
	
    csp_gptb_clken(ptGptbBase);                                             // clkEN
	csp_gptb_set_cr(ptGptbBase, wCrVal);									// set bt work mode
	csp_gptb_set_pscr(ptGptbBase, (uint16_t)wClkDiv - 1);					// clk div
	csp_gptb_set_prdr(ptGptbBase, (uint16_t)wPrdrLoad);				        // prdr load value
		
	if(ptgptbPwmCfg->byDutyCycle){
	wCmpLoad =wPrdrLoad-(wPrdrLoad * ptgptbPwmCfg->byDutyCycle /100);	    // cmp load value
	csp_gptb_set_cmpa(ptGptbBase, (uint16_t)wCmpLoad);					    // cmp load value
	csp_gptb_set_cmpb(ptGptbBase, (uint16_t)wCmpLoad);
//	csp_gptb_set_cmpc(ptGptbBase, (uint16_t)wCmpLoad);
//	csp_gptb_set_cmpd(ptGptbBase, (uint16_t)wCmpLoad);
	}
	
	if(ptgptbPwmCfg->byInter)
	{
		csp_gptb_int_enable(ptGptbBase, ptgptbPwmCfg->byInter, true);		//enable interrupt
		csi_irq_enable((uint32_t *)ptGptbBase);							//enable  irq
	}
	
	gGptb0Prd=wPrdrLoad;
	
	return CSI_OK;
}

 /**
 \brief  capture configuration
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptgptbPwmCfg   	refer to csi_gptb_captureconfig_t
 \return CSI_OK /CSI_ERROR
//*/
csi_error_t csi_gptb_capture_init(csp_gptb_t *ptGptbBase, csi_gptb_captureconfig_t *ptgptbPwmCfg)
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
					
	wCrVal =ptgptbPwmCfg->byCountingMode | (ptgptbPwmCfg->byStartSrc<<GPTB_STARTSRC_POS) |
	        ptgptbPwmCfg->byOneshotMode<<GPTB_OPMD_POS | (ptgptbPwmCfg->byWorkmod<<GPTB_MODE_POS);
    
	wCrVal=(wCrVal & ~(GPTB_PSCLD_MSK))   |((ptgptbPwmCfg->byPscld&0x03)   <<GPTB_PSCLD_POS);
	
	wCrVal=(wCrVal & ~(GPTB_CAPMD_MSK))   |((ptgptbPwmCfg->byCaptureCapmd&0x01)   <<GPTB_CAPMD_POS);
	wCrVal=(wCrVal & ~(GPTB_STOPWRAP_MSK))|((ptgptbPwmCfg->byCaptureStopWrap&0x03)<<GPTB_STOPWRAP_POS);
	wCrVal=(wCrVal & ~(GPTB_CMPA_RST_MSK))|((ptgptbPwmCfg->byCaptureLdaret&0x01)  <<GPTB_CMPA_RST_POS);
	wCrVal=(wCrVal & ~(GPTB_CMPB_RST_MSK))|((ptgptbPwmCfg->byCaptureLdbret&0x01)  <<GPTB_CMPB_RST_POS);
	
	wCrVal=(wCrVal & ~(0x01<<CAPMODE_SEL_POS))|((ptgptbPwmCfg->byCapSrcMode&0x01)  << CAPMODE_SEL_POS);

	wCrVal|=GPTB_CAPLD_EN;//CAPMODE_SEL
	wCrVal|=GPTB_CAPREARM;
	wPrdrLoad=0xFFFF;

    csp_gptb_clken(ptGptbBase);                                         // clkEN
	csp_gptb_set_cr(ptGptbBase, wCrVal);								// set bt work mode
	csp_gptb_set_pscr(ptGptbBase, (uint16_t)wClkDiv - 1);				// clk div
	csp_gptb_set_prdr(ptGptbBase, (uint16_t)wPrdrLoad);				    // prdr load value
	
	if(ptgptbPwmCfg->byInter)
	{
		csp_gptb_int_enable(ptGptbBase, ptgptbPwmCfg->byInter, true);   //enable interrupt
		csi_irq_enable((uint32_t *)ptGptbBase);							//enable  irq
	}
	
	gGptb0Prd=wPrdrLoad;
	
	return CSI_OK;
}

 /**
 \brief  wave configuration
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptgptbPwmCfg   	refer to csi_gptb_pwmconfig_t
 \return CSI_OK /CSI_ERROR
*/
csi_error_t  csi_gptb_wave_init(csp_gptb_t *ptGptbBase, csi_gptb_pwmconfig_t *ptgptbPwmCfg)
{
    uint32_t wClkDiv;
	uint32_t wCrVal;
	uint32_t wCmpLoad; 
	uint32_t wPrdrLoad; 
	
	if(ptgptbPwmCfg->wFreq == 0 ){return CSI_ERROR;}
		
	csi_clk_enable((uint32_t *)ptGptbBase);								// clk enable
	
	csp_gptb_clken(ptGptbBase);
	csp_gptb_wr_key(ptGptbBase);                                           //Unlocking
	csp_gptb_reset(ptGptbBase);											// reset 
	
	wClkDiv = (csi_get_pclk_freq() / ptgptbPwmCfg->wFreq / 60000);		// clk div value
	if(wClkDiv == 0)wClkDiv = 1;
	
	wPrdrLoad  = (csi_get_pclk_freq()/ptgptbPwmCfg->wFreq/wClkDiv);	    //prdr load value
		
	wCrVal =ptgptbPwmCfg->byCountingMode | (ptgptbPwmCfg->byStartSrc<<GPTB_STARTSRC_POS) |
	        ptgptbPwmCfg->byOneshotMode<<GPTB_OPMD_POS | (ptgptbPwmCfg->byWorkmod<<GPTB_MODE_POS);
    
	wCrVal=(wCrVal & ~(GPTB_PSCLD_MSK))   |((ptgptbPwmCfg->byPscld&0x03)   <<GPTB_PSCLD_POS);
		
    csp_gptb_clken(ptGptbBase);                                           // clkEN
	csp_gptb_set_cr(ptGptbBase, wCrVal);									// set bt work mode
	csp_gptb_set_pscr(ptGptbBase, (uint16_t)wClkDiv - 1);					// clk div
	csp_gptb_set_prdr(ptGptbBase, (uint16_t)wPrdrLoad);				    // prdr load value
		
	if(ptgptbPwmCfg->byDutyCycle){
	wCmpLoad =wPrdrLoad-(wPrdrLoad * ptgptbPwmCfg->byDutyCycle /100);	// cmp load value
	csp_gptb_set_cmpa(ptGptbBase, (uint16_t)wCmpLoad);					// cmp load value
	csp_gptb_set_cmpb(ptGptbBase, (uint16_t)wCmpLoad);

	}
	
	if(ptgptbPwmCfg->byInter)
	{
		csp_gptb_int_enable(ptGptbBase, ptgptbPwmCfg->byInter, true);		//enable interrupt
		csi_irq_enable((uint32_t *)ptGptbBase);							//enable  irq
	}
	
	gGptb0Prd=wPrdrLoad;
	
	return CSI_OK;	
}
 /**
 \brief  Channel configuration
 \param  ptGptbBase    	pointer of gptb register structure
 \param  tPwmCfg   	    refer to csi_gptb_pwmchannel_config_t
 \param  channel        Channel label
 \return CSI_OK /CSI_ERROR
*/
csi_error_t csi_gptb_channel_config(csp_gptb_t *ptGptbBase, csi_gptb_pwmchannel_config_t *tPwmCfg, csi_gptb_channel_e channel)
{
    uint32_t w_AQCRx_Val;
	
	w_AQCRx_Val=  tPwmCfg -> byActionZro 
	              | ( tPwmCfg -> byActionPrd  << GPTB_ACT_PRD_POS  )
				  | ( tPwmCfg -> byActionCau  << GPTB_ACT_CAU_POS  )
				  | ( tPwmCfg -> byActionCad  << GPTB_ACT_CAD_POS  )
				  | ( tPwmCfg -> byActionCbu  << GPTB_ACT_CBU_POS  )
				  | ( tPwmCfg -> byActionCbd  << GPTB_ACT_CBD_POS  )
				  | ( tPwmCfg -> byActionT1u  << GPTB_ACT_T1U_POS  )
				  | ( tPwmCfg -> byActionT1d  << GPTB_ACT_T1D_POS  )
				  | ( tPwmCfg -> byActionT2u  << GPTB_ACT_T2U_POS  )
				  | ( tPwmCfg -> byActionT2d  << GPTB_ACT_T2D_POS  )
				  | ( tPwmCfg -> byChoiceCasel  << GPTB_CASEL_POS  )
				  | ( tPwmCfg -> byChoiceCbsel  << GPTB_CBSEL_POS  );
				  
	switch (channel)
	{	
		case (GPTB_CHANNEL_A):csp_gptb_set_aqcra(ptGptbBase,w_AQCRx_Val);
			break;
		case (GPTB_CHANNEL_B):csp_gptb_set_aqcrb(ptGptbBase,w_AQCRx_Val);
			break;

		default:return CSI_ERROR;
			break;
	}
	return CSI_OK;
}

/**
 \brief  DeadZoneTime configuration 
 \param  ptGptbBase    	pointer of ept register structure
 \param  tCfg           refer to csi_ept_deadzone_config_t
 \return CSI_OK /CSI_ERROR
*/
csi_error_t csi_gptb_dz_config(csp_gptb_t *ptGptbBase, csi_gptb_deadzone_config_t *tCfg)
{  uint32_t w_Val;
   
	w_Val=csp_gptb_get_dbcr(ptGptbBase);	
	w_Val=(w_Val&~(GPTB_DCKSEL_MSK))  |(tCfg-> byDcksel <<GPTB_DCKSEL_POS);
	w_Val=(w_Val&~(GPTB_CHA_DEDB_MSK))|(tCfg-> byChaDedb<<GPTB_CHA_DEDB_POS);
//	w_Val=(w_Val&~(GPTB_CHB_DEDB_MSK))|(tCfg-> byChbDedb<<GPTB_CHB_DEDB_POS);
//	w_Val=(w_Val&~(GPTB_CHC_DEDB_MSK))|(tCfg-> byChcDedb<<GPTB_CHC_DEDB_POS);
	csp_gptb_set_dbcr( ptGptbBase, w_Val);	 
	csp_gptb_set_dpscr(ptGptbBase, tCfg-> hwDpsc);
	
	w_Val=csi_get_pclk_freq();
	w_Val=(1000000000/(w_Val/(tCfg->hwDpsc+1)));    //NS/(1/(48000000/(DPSC+1))*10^9) // 500NS/(1000/48) = 24;	
	csp_gptb_set_dbdtr(ptGptbBase	,tCfg-> hwRisingEdgereGister /w_Val);
	csp_gptb_set_dbdtf(ptGptbBase	,tCfg-> hwFallingEdgereGister/w_Val);
	
	return CSI_OK;	
}
 /**
 \brief  DeadZoneTime configuration loading 
 \param  ptGptbBase    	pointer of gptb register structure
 \param  byVal         refer to csi_gptb_dbdldr_e
 \param  byWod         refer to csp_gptb_shdw_e
 \param  byWod2	       refer to csp_gptb_lddb_e
 \return CSI_OK /CSI_ERROR
*/
csi_error_t csi_gptb_dbldrload_config(csp_gptb_t *ptGptbBase, csi_gptb_dbdldr_e byVal,csp_gptb_shdw_e byWod,csp_gptb_lddb_e byWod2)
{   uint32_t w_Val;
	w_Val=csp_gptb_get_dbldr(ptGptbBase);
	switch (byVal)
	{	
		case (GPTB_DBCR) :w_Val=( w_Val &~(GPTB_DBLDR_CRSHDWEN_MSK) )|(byWod << GPTB_DBLDR_CRSHDWEN_POS);
		             w_Val=( w_Val &~(GPTB_DBLDR_LDCRMODE_MSK))|(byWod2 << GPTB_DBLDR_LDCRMODE_POS);
			break;
		case (GPTB_DBDTR):w_Val=( w_Val &~GPTB_DBLDR_SHDWDTR_MSK )|(byWod << GPTB_DBLDR_SHDWDTR_POS );
		             w_Val=( w_Val &~GPTB_DBLDR_LDDTRMD_MSK )|(byWod2 << GPTB_DBLDR_LDDTRMD_POS);
		    break;
		case (GPTB_DBDTF):w_Val=( w_Val &~GPTB_DBLDR_SHDWDTF_MSK )|(byWod << GPTB_DBLDR_SHDWDTF_POS);
		             w_Val=( w_Val &~GPTB_DBLDR_LDDTFMD_MSK )|(byWod2 <<GPTB_DBLDR_LDDTFMD_POS);
            break;
		case (GPTB_DCKPSC):w_Val=( w_Val &~GPTB_DBLDR_SHDWPSC_MSK)|(byWod << GPTB_DBLDR_SHDWPSC_POS);
		              w_Val=( w_Val &~GPTB_DBLDR_LDPSCMD_MSK)|(byWod2 << GPTB_DBLDR_LDPSCMD_POS);
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
 \param  tCfg           refer to csi_gptb_deadzone_config_t
 \return CSI_OK /CSI_ERROR
*/
csi_error_t csi_gptb_dbcr_config(csp_gptb_t *ptGptbBase, csi_gptb_deadzone_config_t *tCfg)
{  uint32_t w_Val;
	w_Val=csp_gptb_get_dbcr(ptGptbBase);	
	w_Val=(w_Val&~(GPTB_DCKSEL_MSK))|(tCfg-> byDcksel <<GPTB_DCKSEL_POS);
	w_Val=(w_Val&~(GPTB_CHA_DEDB_MSK))|(tCfg-> byChaDedb<<GPTB_CHA_DEDB_POS);
//	w_Val=(w_Val&~(GPTB_CHB_DEDB_MSK))|(tCfg-> byChbDedb<<GPTB_CHB_DEDB_POS);
//	w_Val=(w_Val&~(GPTB_CHC_DEDB_MSK))|(tCfg-> byChcDedb<<GPTB_CHC_DEDB_POS);
	csp_gptb_set_dbcr( ptGptbBase, w_Val);	 
	csp_gptb_set_dpscr(ptGptbBase	,tCfg-> hwDpsc);
	csp_gptb_set_dbdtr(ptGptbBase	,tCfg-> hwRisingEdgereGister);
	csp_gptb_set_dbdtf(ptGptbBase	,tCfg-> hwFallingEdgereGister);
	return CSI_OK;	
}
 /**
 \brief  channelmode configuration 
 \param  ptGptbBase    pointer of gptb register structure
 \param  tCfg         refer to csi_gptb_deadzone_config_t
 \param  byCh         refer to csi_gptb_channel_e
 \return CSI_OK /CSI_ERROR  
*/
csi_error_t csi_gptb_channelmode_config(csp_gptb_t *ptGptbBase,csi_gptb_deadzone_config_t *tCfg,csi_gptb_channel_e byCh)
{    uint32_t w_Val;
     w_Val=csp_gptb_get_dbcr(ptGptbBase);	
	 switch (byCh)
	{	
		case (GPTB_CHANNEL_A): w_Val=(w_Val&~(DB_CHA_OUTSEL_MSK)) |(tCfg-> byChxOuselS1S0   <<DB_CHA_OUTSEL_POS);
		                      w_Val=(w_Val&~(DB_CHA_POL_MSK))    |(tCfg-> byChxPolarityS3S2<<DB_CHA_POL_POS);
							  w_Val=(w_Val&~(DB_CHA_INSEL_MSK))  |(tCfg-> byChxInselS5S4   <<DB_CHA_INSEL_POS);
							  w_Val=(w_Val&~(DB_CHA_OUTSWAP_MSK))|(tCfg-> byChxOutSwapS8S7 <<DB_CHA_OUTSWAP_POS);
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
 \param  tCfg           refer to csi_gptb_emergency_config_t
 \return CSI_OK /CSI_ERROR
//*/
csi_error_t csi_gptb_emergency_cfg(csp_gptb_t *ptGptbBase, csi_gptb_emergency_config_t *tCfg)
{ uint32_t wEmsrc;
  uint32_t wEmsrc2;
  uint32_t wEmpol;
  uint32_t wEmecr;

    wEmsrc2=csp_gptb_get_src2(ptGptbBase);
	wEmsrc2=(wEmsrc2 & (~GPTB_EMSRC2_FLT_PACE0_MSK) & (~GPTB_EMSRC2_FLT_PACE1_MSK) ) | (tCfg -> byFltpace1 << GPTB_EMSRC2_FLT_PACE1_POS) | (tCfg ->byFltpace0  << GPTB_EMSRC2_FLT_PACE0_POS);
	wEmsrc2=(wEmsrc2 &~0xff0000) |  tCfg ->byOrl1 <<16;
	wEmsrc2=(wEmsrc2 &~0xff)     |  tCfg ->byOrl0 ;
	csp_gptb_set_src2(ptGptbBase,wEmsrc2);
		
	wEmsrc = csp_gptb_get_src(ptGptbBase);
    wEmsrc=(  wEmsrc &~ GPTB_SEL_MSK_EP(tCfg -> byEpx) )|( tCfg -> byEpxInt  << GPTB_SEL_POS_EP(tCfg -> byEpx));
    csp_gptb_set_src(ptGptbBase,wEmsrc);
	
    wEmpol=csp_gptb_get_empol(ptGptbBase);	
	 switch (tCfg ->byEpxInt)
	 {    case (EBI0): wEmpol=( wEmpol  &~ POL_MSK_EBI(0)) | (tCfg -> byPolEbix <<POL_POS_EBI(0) );break;
		  case (EBI1): wEmpol=( wEmpol  &~ POL_MSK_EBI(1)) | (tCfg -> byPolEbix <<POL_POS_EBI(1) );break;
		  case (EBI2): wEmpol=( wEmpol  &~ POL_MSK_EBI(2)) | (tCfg -> byPolEbix <<POL_POS_EBI(2) );break;
		  case (EBI3): wEmpol=( wEmpol  &~ POL_MSK_EBI(3)) | (tCfg -> byPolEbix <<POL_POS_EBI(3) );break;
		  case (EBI4): wEmpol=( wEmpol  &~ POL_MSK_EBI(4)) | (tCfg -> byPolEbix <<POL_POS_EBI(4) );break;
		  default:return CSI_ERROR;break;
	 }
	csp_gptb_set_empol(ptGptbBase,wEmpol);

    wEmecr =  csp_gptb_get_emecr(ptGptbBase);	
	wEmecr =(wEmecr & (~GPTB_LCKMD_MSK_EP(tCfg ->byEpx))) | (   tCfg ->byEpxLckmd <<  GPTB_LCKMD_POS_EP(tCfg ->byEpx));
	csp_gptb_set_emecr(ptGptbBase,wEmecr);
			
	return CSI_OK;
}
/**
 \brief  State of emergency configuration 
 \param  ptGptbBase    	pointer of gptb register structure
 \param  tCfg           refer to csi_gptb_emergency_config_t//csp_ept_emout_e
 \return CSI_OK /CSI_ERROR
//*/
csi_error_t csi_gptb_emergency_pinxout(csp_gptb_t *ptGptbBase,csi_gptb_osrchx_e  byCh ,csp_gptb_emout_e byCh2)
{ uint32_t wEmosr;
    wEmosr=csp_gptb_get_emosr(ptGptbBase);	
	 switch (byCh)
	 {    case (GPTB_EMCOAX): wEmosr=( wEmosr &~(GPTB_EMOSR_EM_COAX_MSK) )|( byCh2 <<GPTB_EMOSR_EM_COAX_POS);break;
		  case (GPTB_EMCOBX): wEmosr=( wEmosr &~(GPTB_EMOSR_EM_COBX_MSK) )|( byCh2 <<GPTB_EMOSR_EM_COBX_POS);break;
		  case (GPTB_EMCOAY): wEmosr=( wEmosr &~(GPTB_EMOSR_EM_COAY_MSK) )|( byCh2 <<GPTB_EMOSR_EM_COAY_POS);break;		
		  default:return CSI_ERROR;break;
	 }
    csp_gptb_set_emosr(ptGptbBase,wEmosr);
	return CSI_OK;
}

/**
 \brief  gptb configuration Loading
 \param  ptGptbBase    	pointer of gptb register structure
 \param  tCfg           refer to csi_gptb_Global_load_control_config_t
 \return CSI_OK /CSI_ERROR
*/
csi_error_t csi_gptb_global_config(csp_gptb_t *ptGptbBase,csi_gptb_Global_load_control_config_t *Global)
{   uint32_t w_GLDCR;	
	w_GLDCR =0;
    w_GLDCR = (w_GLDCR &~GPTB_GLDEN_MSK)       | ((Global->bGlden & 0x01)<<GPTB_GLDEN_POS);
	w_GLDCR = (w_GLDCR &~GPTB_GLDMD_MSK)       | ((Global->byGldmd & 0x0f)<<GPTB_GLDMD_POS);
	w_GLDCR = (w_GLDCR &~GPTB_GLDCR_OSTMD_MSK) | ((Global->bOstmd & 0x01)<<GPTB_GLDCR_OSTMD_POS);
	w_GLDCR = (w_GLDCR &~GPTB_GLDPRD_MSK)      | ((Global->bGldprd & 0x07)<<GPTB_GLDPRD_POS);
	csp_gptb_set_gldcr(ptGptbBase,w_GLDCR);	
	return CSI_OK;
}

/**
 \brief  Software trigger loading
 \param  ptGptbBase    	pointer of gptb register structure
 \return CSI_OK
*/
csi_error_t csi_gptb_global_sw(csp_gptb_t *ptGptbBase)
{
	csp_gptb_set_gldcr2(ptGptbBase,GPTB_OSREARM_EN);
	return CSI_OK;
}
/**
 \brief  rearm  loading
 \param  ptGptbBase    	pointer of gptb register structure
 \return CSI_OK
*/
csi_error_t csi_gptb_global_rearm(csp_gptb_t *ptGptbBase)
{
	csp_gptb_set_gldcr2(ptGptbBase,GPTB_SW_GLD);
	return CSI_OK;
}
/** \brief start gptb
 *  \param ptgptbBase:  pointer of bt register structure
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_gptb_start(csp_gptb_t *ptgptbBase)
{   
	csp_gptb_wr_key(ptgptbBase); 
	csp_gptb_start(ptgptbBase);
	return CSI_OK;
}
/**
 \brief SW stop GPTB counter
 \param ptgptbBase    pointer of gptb register structure
*/
void csi_gptb_swstop(csp_gptb_t *ptgptbBase)
{
  csp_gptb_wr_key(ptgptbBase);
  csp_gptb_stop(ptgptbBase);
}
/**
 \brief set GPTB start mode. 
 \param ptgptbBase    pointer of gptb register structure
 \return eMode ：GPTB_SW/GPTB_SYNC
*/
void csi_gptb_set_start_mode(csp_gptb_t *ptgptbBase, csi_gptb_stmd_e eMode)
{
	csp_gptb_set_start_src(ptgptbBase, eMode);
}
/**
 \brief set GPTB operation mode
 \param ptgptbBase    pointer of gptb register structure
 \param eMode 	 GPTB_OP_CONT/GPTB_OP_OT
*/
void csi_gptb_set_os_mode(csp_gptb_t *ptgptbBase, csi_gptb_opmd_e eMode)
{
	csp_gptb_set_opmd(ptgptbBase, eMode);
}

/**
 \brief set GPTB stop status
 \param ptgptbBase    pointer of gptb register structure
 \param eSt 	 GPTB_STP_HZ/GPTB_STP_LOW
*/
void csi_gptb_set_stop_st(csp_gptb_t *ptgptbBase, csp_gptb_stpst_e eSt)
{	
  csp_gptb_set_stop_st(ptgptbBase,eSt);
}

/**
 \brief get counter period to calculate the duty cycle. 
 \param ptgptbBase    pointer of gptb register structure
 \return counter period (reg data)
*/
uint16_t csi_gptb_get_prdr(csp_gptb_t *ptGptbBase)
{
	return csp_gptb_get_prdr(ptGptbBase);
}

/**
 \brief change gptb output dutycycle. 
 \param ptGptbBase     pointer of gptb register structure
 \param eCh           refer to csi_gptb_chtype_e
 \param wActiveTime   cmpx data to be set directly
*/
csi_error_t csi_gptb_change_ch_duty(csp_gptb_t *ptGptbBase, csi_gptb_chtype_e eCh, uint32_t wActiveTime)
{ uint16_t  wCmpLoad;//gGptb0Prd
    
//   wPrdLoad=0;
    wCmpLoad =gGptb0Prd-(gGptb0Prd * wActiveTime /100);
    
	switch (eCh)
	{	
		case (GPTB_CH_A):csp_gptb_set_cmpa(ptGptbBase, (uint16_t)wCmpLoad);
			break;
		case (GPTB_CH_B):csp_gptb_set_cmpb(ptGptbBase, (uint16_t)wCmpLoad);
			break;

		default: return (1);
			break;
	}
    return (0);
}

/**
 \brief software force lock
 \param ptEpt    pointer of gptb register structure
 \param eEbi 	 external emergency input: GPTB_EPI0~7 （EBI4 = LVD）
 \return none
*/
//void csi_gptb_force_em(csp_gptb_t *ptGptbBase, csp_gptb_ep_e byEbi)
//{
//	csp_gptb_force_em(ptGptbBase,byEbi);
//}

/**
 \brief get harklock status
 \param ptEpt    pointer of gptb register structure
 \return uint8_t 0b_ _ _ _ _ _ _ _
*/
uint8_t csi_gptb_get_hdlck_st(csp_gptb_t *ptGptbBase)
{	
	return (csp_gptb_get_emHdlck(ptGptbBase));
}

/**
 \brief clear harklock status
 \param ptEpt   pointer of gptb register structure
 \return  eEbi 	 external emergency input: csp_gptb_ep_e                 
*/
void csi_gptb_clr_hdlck(csp_gptb_t *ptGptbBase, csp_gptb_ep_e eEbi)
{
	csp_gptb_clr_emHdlck(ptGptbBase, eEbi);
}

/**
 \brief get 	 softlock status
 \param ptEpt    pointer of gptb register structure
 \return uint8_t 0b_ _ _ _ _ _ _ _
*/
uint8_t csi_gptb_get_sftlck_st(csp_gptb_t *ptGptbBase)
{	
	return (csp_gptb_get_emSdlck(ptGptbBase));
}

/**
 \brief clear softlock status
 \param ptEpt   pointer of gptb register structure
 \return  eEpi 	 external emergency input: csp_gptb_ep_e
*/
void csp_gptb_clr_sftlck(csp_gptb_t *ptGptbBase, csp_gptb_ep_e eEpi)
{	
	csp_gptb_clr_emSdlck(ptGptbBase, eEpi);
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
  \param[in]   eEbi		       refer to csp_gptb_emint_e
*/
void csi_gptb_emergency_interruption_en(csp_gptb_t *ptGptbBase, csp_gptb_emint_e eEbi)
{
    csp_gptb_Emergency_emimcr(ptGptbBase,eEbi);
}

/**
  \brief   enable/disable gptb out trigger 
  \param   ptGptbBase      pointer of gptb register structure
  \param 	eCh			0/1
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
  \param   byCh	         GPTB_OSTSFA/GPTB_OSTSFB/GPTB_OSTSFC/GPTB_OSTSFD		
  \param   bEnable 		GPTB_LDAQCR_ZRO/GPTB_LDAQCR_PRD/GPTB_LDAQCR_ZROPRD
*/
csi_error_t csi_gptb_Onetimesoftware_output(csp_gptb_t *ptGptbBase, uint16_t byCh, csp_gptb_action_e bEnable)
{	
	switch (byCh){
	case GPTB_OSTSFA: ptGptbBase ->AQOSF |= GPTB_OSTSFA;
	                 ptGptbBase ->AQOSF = (ptGptbBase ->AQOSF &~(GPTB_ACTA_MSK))|((bEnable&0x03)<<GPTB_ACTA_POS);
	     break;
	case GPTB_OSTSFB: ptGptbBase ->AQOSF |= GPTB_OSTSFB;
	                 ptGptbBase ->AQOSF = (ptGptbBase ->AQOSF &~(GPTB_ACTB_MSK))|((bEnable&0x03)<<GPTB_ACTB_POS);
	     break;	

	default: return CSI_ERROR;
	     break;
    }
	return CSI_OK;
}
/** \brief  Continuous software waveform loading control
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] bEnable:    refer to csp_gptb_aqosf_e
 *  \return  none
 */
void csi_gptb_loading_method_aqcsf(csp_gptb_t *ptGptbBase, csp_gptb_aqosf_e bEnable)
{
	ptGptbBase ->AQOSF  = (ptGptbBase ->AQOSF &~(GPTB_AQCSF_LDTIME_MSK))|((bEnable&0x03)<<GPTB_AQCSF_LDTIME_POS);
}
/** \brief Continuous software waveform control
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] byCh        refer to csi_gptb_channel_e
 *  \param[in] bEnable:    refer to  csp_gptb_aqosf_e
 *  \return  none
 */
csi_error_t csi_gptb_continuous_software_waveform(csp_gptb_t *ptGptbBase, csi_gptb_channel_e byCh, csp_gptb_aqcsf_e bEnable)
{
	
	switch (byCh){
	case GPTB_CHANNEL_A:  ptGptbBase ->AQCSF = (ptGptbBase ->AQCSF &~(0x03))|(bEnable&0x03);            
	     break;
	case GPTB_CHANNEL_B:  ptGptbBase ->AQCSF = (ptGptbBase ->AQCSF &~(0x0c))|(bEnable&0x03)<<2;
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
csi_error_t csi_gptb_int_enable(csp_gptb_t *ptGptbBase, csp_gptb_int_e eInt, bool bEnable)
{  
	csp_gptb_int_enable(ptGptbBase,eInt,bEnable);
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
	csp_gptb_set_sync_mode(ptGptbBase, eTrgIn, eTrgMode);
	csp_gptb_set_auto_rearm(ptGptbBase, eAutoRearm);
	csp_gptb_sync_enable(ptGptbBase, eTrgIn, ENABLE);
}

/** \brief gptb extsync input select
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eTrgin: gptb sync evtrg input channel(0~6)
 *  \param[in] byTrgChx: trgxsel channel(0~1)
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_set_extsync_chnl(csp_gptb_t *ptGptbBase, csi_gptb_trgin_e eTrgIn, csi_gptb_syncrout_e byTrgChx)
{
	switch(byTrgChx)
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
	wWindow = ptFilter->byWinOffset | (ptFilter->byWinWidth << GPTB_FLT_WDW_POS);
	
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
csi_error_t csi_gptb_set_evtrg(csp_gptb_t *ptGptbBase, csi_gptb_trgout_e byTrgOut, csi_gptb_trgsrc_e eTrgSrc)
{
	csp_gptb_set_trgsel01(ptGptbBase, byTrgOut, eTrgSrc);			    
	csp_gptb_trg_xoe_enable(ptGptbBase, byTrgOut, ENABLE);				//evtrg out enable
	
	return CSI_OK;//
}


/** \brief gptb evtrg cntxinit control
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] byCntChx: evtrg countinit channel(0~3)
 *  \param[in] byCntVal: evtrg cnt value
 *  \param[in] byCntInitVal: evtrg cntxinit value
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_set_evcntinit(csp_gptb_t *ptGptbBase, uint8_t byCntChx, uint8_t byCntVal, uint8_t byCntInitVal)
{

 if(byCntChx > 1)
  return CSI_ERROR;
 
 csp_gptb_set_trgprd(ptGptbBase, byCntChx, byCntVal - 1);    //evtrg count
 csp_gptb_trg_cntxinit(ptGptbBase, byCntChx, byCntInitVal);
 csp_gptb_trg_cntxiniten_enable(ptGptbBase, byCntChx, ENABLE);
 
 return CSI_OK;
}

/**
 \brief  gptb configuration Loading
 \param  ptGptbBase    	pointer of gptb register structure
 \param  tCfg           refer to csi_gptb_feglk_config_t
 \return CSI_OK /CSI_ERROR
*/
csi_error_t csi_gptb_feglk_config(csp_gptb_t *ptGptbBase,csi_gptb_feglk_config_t *Global)
{   uint32_t w_GLK;	
	w_GLK =0;
    w_GLK = (w_GLK & ~GPTB_PRDR_MSK )| ((Global-> byPrdr & 0xF)<< GPTB_PRDR_POS);
	w_GLK = (w_GLK & ~GPTB_CMPA_MSK )| ((Global-> byCmpa & 0xF)<< GPTB_CMPA_POS);
	w_GLK = (w_GLK & ~GPTB_CMPB_MSK )| ((Global-> byCmpb & 0xF)<< GPTB_CMPB_POS);
	w_GLK = (w_GLK & ~GPTB_GLD2_MSK )| ((Global-> byGld2 & 0xF)<< GPTB_GLD2_POS);
	w_GLK = (w_GLK & ~GPTB_RSSR_MSK )| ((Global-> byRssr & 0xF)<< GPTB_RSSR_POS);
	csp_gptb_set_feglk(ptGptbBase,w_GLK);
	w_GLK =0;
	w_GLK = (w_GLK & ~GPTB_EMSLCLR_MSK )| ((Global-> byEmslclr & 0xF)<< GPTB_EMSLCLR_POS);
	w_GLK = (w_GLK & ~GPTB_EMHLCLR_MSK )| ((Global-> byEmhlclr & 0xF)<< GPTB_EMHLCLR_POS);
	w_GLK = (w_GLK & ~GPTB_EMICR_MSK )  | ((Global-> byEmicr   & 0xF)<< GPTB_EMICR_POS);
	w_GLK = (w_GLK & ~GPTB_EMFRCR_MSK ) | ((Global-> byEmfrcr  & 0xF)<< GPTB_EMFRCR_POS);
	w_GLK = (w_GLK & ~GPTB_AQOSF_MSK )  | ((Global-> byAqosf   & 0xF)<< GPTB_AQOSF_POS);
	w_GLK = (w_GLK & ~GPTB_AQCSF_MSK )  | ((Global-> byAqcsf   & 0xF)<< GPTB_AQCSF_POS);
	csp_gptb_set_feglk2(ptGptbBase,w_GLK);	
	return CSI_OK;
}

