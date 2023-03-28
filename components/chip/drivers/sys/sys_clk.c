/***********************************************************************//** 
 * \file  sys_clk.c
 * \brief  system clock management for cpu clock(HCLK) and peri clock(PCLK)
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-10 <td>V0.0  <td>WNN   <td>initial
 * <tr><td> 2021-5-13 <td>V0.0  <td>ZJY   <td>initial
 * </table>
 * *********************************************************************
*/
#include <stdint.h>
#include <sys_clk.h>
#include <pin.h>
#include "board_config.h"

#include <csp.h>


//extern system_clk_config_t g_tSystemClkConfig[];

///to match the real div to reg setting
const uint32_t g_wHclkDiv[] = {
	1, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 24, 32, 36, 64, 128, 256
};

static uint32_t apt_get_hclk(void)
{
	uint32_t tRslt;
	tRslt = tClkConfig.wSclk;
	return (tRslt);
}

/** \brief sysctem clock (HCLK) configuration
 * 
 *  To set CPU frequence according to tClkConfig
 * 
 *  \param[in] none.
 *  \return csi_error_t.
 */ 
csi_error_t csi_sysclk_config(csi_clk_config_t tClkCfg)
{	
	csi_error_t ret = CSI_OK;
	uint8_t byFreqIdx = 0;
	uint32_t wFreq,wTargetSclk;
	cclk_src_e eSrc;
	uint8_t byFlashLp = 0;
	wFreq = tClkCfg.wFreq;
	
	wTargetSclk = wFreq/g_wHclkDiv[tClkCfg.eSdiv];
	eSrc = tClkCfg.eClkSrc;
	csp_set_clksrc(SYSCON, SRC_IMOSC);
	csp_eflash_lpmd_enable(SYSCON, 0);					//disable Flash LP Mode					
	
	switch (eSrc)
	{
		case (SRC_ISOSC): 	
			csi_isosc_enable();
			byFlashLp = 1;
			break;
		case (SRC_IMOSC):	
			switch (wFreq) 	
			{
				case (IMOSC_5M_VALUE):   byFreqIdx = 0;
					break;
				case (IMOSC_4M_VALUE):   byFreqIdx = 1;
					break;
				case (IMOSC_2M_VALUE):   byFreqIdx = 2;
					break;
				case (IMOSC_131K_VALUE): byFreqIdx = 3;
					break;
				default: ret = CSI_ERROR;	
					break;
			}
			csi_imosc_enable(byFreqIdx);
		
			if (wFreq == IM_131K)
				byFlashLp = 1;
			break;
		case (SRC_EMOSC):
			csp_em_flt_sel(SYSCON,EM_FLT_10NS);
			csp_em_flt_enable(SYSCON,ENABLE);
			csi_pin_set_mux(PD0, PD0_XIN);
			csi_pin_set_mux(PD1, PD1_XOUT);
			if (wFreq == EMOSC_32K_VALUE)
				csp_set_em_lfmd(SYSCON, 1);
			ret = csi_emosc_enable(wFreq);
			break;
		case (SRC_HFOSC):	
			switch (wFreq) 	
			{
				case (HFOSC_48M_VALUE): byFreqIdx = 0;
					break;
				case (HFOSC_24M_VALUE): byFreqIdx = 1;
					break;
				case (HFOSC_12M_VALUE): byFreqIdx = 2;
					break;
				case (HFOSC_6M_VALUE):  byFreqIdx = 3;
					break;
				default: ret = CSI_ERROR;
					return ret;
					break;
			}
			csi_hfosc_enable(byFreqIdx);
			break;
		case (SRC_PLL):	
			csi_pll_disable();
			csi_hfosc_enable(1);           // HFOSC_24M_VALUE
			csp_pll_clk_sel(SYSCON, PLL_CLK_SEL_HFOSC);
			csp_pll_set_div_m(SYSCON, 3);
			csp_pll_set_nul(SYSCON, 35);
			csp_pll_set_ckp_div(SYSCON, 1);
			csp_pll_clk_enable(SYSCON, ENABLE);
			csi_pll_enable();
			break;
		case(SRC_ESOSC):
 		//	csi_pin_set_mux(PC14, PC14_SXIN);
		//	csi_pin_set_mux(PC15, PC15_SXOUT);   // Config pins before use ESOSC 
			csi_esosc_enable(wFreq);
			byFlashLp = 1;
			break;
		default: 
			break;
	}
	IFC->CEDR = IFC_CLKEN;
	if (wTargetSclk > 80000000) {
		IFC->MR = (IFC->MR & (~PF_SPEED_MSK) & (~PF_WAIT_MSK)) | HIGH_SPEED | PF_WAIT4;
	}
	else if(wTargetSclk > 64000000) {
		IFC->MR = (IFC->MR & (~PF_SPEED_MSK) & (~PF_WAIT_MSK)) | HIGH_SPEED | PF_WAIT3;
	}
	else if(wTargetSclk > 48000000) {
		IFC->MR = (IFC->MR & (~PF_SPEED_MSK) & (~PF_WAIT_MSK)) | HIGH_SPEED | PF_WAIT2;
	}
	else if(wTargetSclk > 16000000) {
		IFC->MR = (IFC->MR & (~PF_SPEED_MSK) & (~PF_WAIT_MSK)) | HIGH_SPEED | PF_WAIT1;
	}
	else if(wTargetSclk > 8000000) {
		IFC->MR = (IFC->MR & (~PF_SPEED_MSK) & (~PF_WAIT_MSK)) | HIGH_SPEED | PF_WAIT0;
	}
	else{
		IFC->MR = (IFC->MR & (~PF_SPEED_MSK) & (~PF_WAIT_MSK)) | LOW_SPEED | PF_WAIT0;
	}
	csp_set_sdiv(SYSCON, tClkCfg.eSdiv);
	csp_set_clksrc(SYSCON, eSrc);
	
	
	csp_eflash_lpmd_enable(SYSCON, (bool)byFlashLp);
	
	csp_set_pdiv(SYSCON, tClkCfg.ePdiv);
	
	//update wSclk and wPclk in tClkConfig
	tClkConfig.wSclk = wTargetSclk;
	tClkConfig.wPclk = tClkConfig.wSclk/(0x1<<tClkCfg.ePdiv);
	return ret;
}

/** \brief Clock output configuration
 * 
 *  \param[in] eCloSrc: source to output
 *  \param[in] eCloDiv: clo divider 
 *  \param[in] tPin: output pin
 *  \return csi_error_t.
 */
csi_error_t csi_clo_config(clo_src_e eCloSrc, clo_div_e eCloDiv, pin_name_e ePin)
{ 	
	csi_error_t ret = CSI_OK;
	switch (ePin)
	{
		case (PA2):
			csi_pin_set_mux(PA2, PA2_CLO);
			break;
		case (PB3):
			csi_pin_set_mux(PB3, PB3_CLO);
			break;
		case (PD3):
			csi_pin_set_mux(PD3, PD3_CLO);
			break;
		case (PD4):
			csi_pin_set_mux(PD4, PD4_CLO);
			break;
		default:
			ret = CSI_ERROR;
			break;
	}
	
	csp_set_clo_src(SYSCON, eCloSrc);
	csp_set_clo_div(SYSCON, eCloDiv);
	return ret;
}

/** \brief prei clk enable in SYSCON level
 *
 *  \param[in] wModule: module name
 *  \return none
 */
void soc_clk_enable(int32_t wModule)
{
    //TODO
	if(wModule < 32U)
		csp_pcer0_clk_en(SYSCON, (uint32_t)wModule);
	else
		csp_pcer1_clk_en(SYSCON, (uint32_t)wModule - 32U);
}

/** \brief prei clk disable in SYSCON level
 *
 *  \param[in] wModule: module name
 *  \return none
 */
void soc_clk_disable(int32_t wModule)
{
    //TODO
	if(wModule < 32U)
		csp_pder0_clk_dis(SYSCON, (uint32_t)wModule);
	else
		csp_pder1_clk_dis(SYSCON, (uint32_t)wModule - 32U);
}

/** \brief to calculate SCLK and PCLK frequence according to the current reg content
 *  tClkConfig.wSclk and tClkConfig.wPclk will be updated after excuting this function
 *  \param[in] none.
 *  \return csi_error_t.
 */
csi_error_t csi_calc_clk_freq(void)
{
	cclk_src_e eClkSrc;
	uint8_t  byHclkDiv;
	uint32_t wHfoFreq;
	uint32_t wImoFreq;
	
	//calculate sclk
	{
		eClkSrc = ((cclk_src_e) csp_get_clksrc(SYSCON));
		switch(eClkSrc)
		{ 	case (SRC_ISOSC): 	
				tClkConfig.wSclk = ISOSC_VALUE;
				break;
			case (SRC_EMOSC): 	
				tClkConfig.wSclk = EMOSC_VALUE;
				break;
			case (SRC_IMOSC):	
				wImoFreq = csp_get_imosc_fre(SYSCON);
				switch (wImoFreq)
				{
					case (0): 
						tClkConfig.wSclk = IMOSC_5M_VALUE;
						break;
					case (1): 
						tClkConfig.wSclk = IMOSC_4M_VALUE;
						break;
					case (2): 
						tClkConfig.wSclk = IMOSC_2M_VALUE;	
						break;
					case (3): 
						tClkConfig.wSclk = IMOSC_131K_VALUE;	
						break;
					default: 
						return CSI_ERROR;	
						break;
				}
				break;
			case  (SRC_HFOSC):	
				wHfoFreq =  csp_get_hfosc_fre(SYSCON);
				switch (wHfoFreq)
				{
					case (0): 
						tClkConfig.wSclk = HFOSC_48M_VALUE;
						break;
					case (1): 
						tClkConfig.wSclk = HFOSC_24M_VALUE;
						break;
					case (2): 
						tClkConfig.wSclk = HFOSC_12M_VALUE;	
						break;
					case (3): 
						tClkConfig.wSclk = HFOSC_6M_VALUE;	
						break;
					default:  
						return CSI_ERROR;	
						break;
				}
				break;
			case (SRC_PLL): 	
				tClkConfig.wSclk = PLL_105M_VALUE;
				break;
			case (SRC_ESOSC): 	
				tClkConfig.wSclk = ESOSC_VALUE;
				break;
			default:
				return CSI_ERROR;
				break;
		}
		byHclkDiv = csp_get_hclk_div(SYSCON);
		tClkConfig.wSclk = tClkConfig.wSclk/g_wHclkDiv[byHclkDiv];
	}

	//calculate pclk 
	{
		 uint32_t wDiv, wPdiv = 1;
		wDiv = csp_get_pdiv(SYSCON);
		if(wDiv == 0)
			wPdiv = 1;
		else if(wDiv == 1)
			wPdiv = 2;
		else if(wDiv & 0x08)
			wPdiv = 16;
		else if(wDiv & 0x04)
			wPdiv = 8;
		else if(wDiv & 0x02)
			wPdiv = 4;
		
		tClkConfig.wPclk = tClkConfig.wSclk / wPdiv;
	}

	return CSI_OK;
}

/** \brief to get SCLK frequence 
 *  \param[in] none.
 *  \return sclk value
 */ 
uint32_t csi_get_sclk_freq(void)
{	
	return tClkConfig.wSclk;
}

/** \brief To get PCLK frequence 
 *  \param[in] none.
 *  \return pclk value
 */ 
uint32_t csi_get_pclk_freq(void)
{
	return tClkConfig.wPclk;
}

/** \brief To get CORET frequence.
 *  Make sure to excute soc_get_cpu_freq() after clock block changing
 *  \param[in] none.
 *  \return g_wSystemClk.
 */ 
uint32_t soc_get_coret_freq(void)
{
/*	switch ((CORETIMER->CTRL & 0x4) >> 2)
	{
		case 0: return tClkConfig.wSclk/8;
			break;
		case 1: return tClkConfig.wSclk;
			break;
		default:
			return tClkConfig.wSclk;
			break;
	}*/
	return tClkConfig.wSclk;
}
/** \brief to set clock status in PM mode 
 *  when IWDT is enabled, trying to stop ISOSC in stop mode would be invalid
 *  refer to GCER in SYSCON chapter for detailed description
 *  \param[in] eClk: clock to be configured
 *  \param[in] bEnable: enable or disable
 *  \return none.
 */ 
void soc_clk_pm_enable(clk_pm_e eClk, bool bEnable)
{
	csp_clk_pm_enable(SYSCON, eClk, bEnable);
}
/** \brief       Soc get bt frequence.
 *  \param[in]   byIdx: id of bt
 *  \return      coret frequence
*/
uint32_t soc_get_bt_freq(uint8_t byIdx)
{
	csp_bt_t *bt_base  = NULL;
	switch(byIdx)
	{
		case 0:
			bt_base = (csp_bt_t *)APB_BT0_BASE;
			break;
		case 1:
			bt_base = (csp_bt_t *)APB_BT1_BASE;
			break;
		default:
			return csi_get_pclk_freq();
	}
	
	return csi_get_pclk_freq()/(csp_bt_get_pscr(bt_base) + 1);
}