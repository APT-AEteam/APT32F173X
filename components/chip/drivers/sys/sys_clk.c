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
static const uint32_t s_wHclkDiv[] = {
	1, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 24, 32, 36, 64, 128, 256
};

/** \brief get hclk freq
 * 
 *  \param[in] none
 *  \return hclk freq
 */	
static uint32_t apt_get_hclk(void)
{
	uint32_t wRslt;
	wRslt = g_tClkConfig.wSclk;
	return (wRslt);
}

/** \brief auto pll config
 * 
 *  \param[in] wPllFreq: pll clk freq 
 *  \return csi_error_t.
 */	
static csi_error_t apt_auto_pll_cfg(uint32_t wPllFreq)
{
	uint8_t byStep;
	uint32_t wNulFreq,wValue1[4],wValue,wFreqCkp;
	
	for(byStep = 2;byStep < 10;byStep += 2)
	{
		wNulFreq = wPllFreq * byStep;
		if((wNulFreq >= 150000000)&&(wNulFreq <= 300000000))
		{
			g_tPllClkConfig.byCkp_Div = byStep-1;
			break;
		}
	}
	
	byStep = (wNulFreq + 24000000)/48000000;
	if((byStep%2) == 0)
	{
		g_tPllClkConfig.byCkq_Div = byStep/2 - 1;
	}
	else 
	{
		g_tPllClkConfig.byCkq_Div = byStep/2;
	}
	for(byStep=3;byStep < 7;byStep ++)
	{
		wValue = 24000000/byStep;
		wValue1[byStep-3] = wNulFreq/wValue;
		wValue1[byStep-3] *= wValue;
	}
	wValue = wValue1[0];
	for(byStep=1;byStep < 4;byStep ++)
	{
		if(wValue < wValue1[byStep])
		{
			wValue = wValue1[byStep];
		}
	}
	for(byStep=0;byStep < 4;byStep ++)
	{
		if(wValue == wValue1[byStep])
		{
			g_tPllClkConfig.byDivM = byStep + 2;         //byStep + 3 -1;
			wFreqCkp = 24000000/(byStep+3);
			g_tPllClkConfig.byNul = wNulFreq/wFreqCkp;	
			return CSI_OK;	
		}
	}	
	return CSI_OK;	
}

/** \brief sysctem clock (HCLK) configuration
 * 
 *  To set CPU frequence according to g_tClkConfig
 * 
 *  \param[in] none.
 *  \return csi_error_t.
 */ 
csi_error_t csi_sysclk_config(csi_clk_config_t tClkCfg)
{	
	csi_error_t ret = CSI_OK;
	uint8_t byFreqIdx = 0;
	uint32_t wFreq,wTargetSclk;
	csi_clk_src_e  eSrc;
	csi_sclk_sel_e eSckSel = SEL_IMOSC;
	uint8_t byFlashLp = 0;
	wFreq = tClkCfg.wFreq;
	
	wTargetSclk = wFreq/s_wHclkDiv[tClkCfg.eSdiv];
	eSrc = tClkCfg.eClkSrc;
	csp_set_clksrc(SYSCON, SRC_IMOSC);
	csp_eflash_lpmd_enable(SYSCON, 0);					//disable Flash LP Mode					
	
	switch (eSrc)
	{
		case (SRC_ISOSC): 
			eSckSel = SEL_ISOSC;	
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
			eSckSel = SEL_IMOSC;
			csi_imosc_enable(byFreqIdx);
			if (wFreq == IM_131K)
				byFlashLp = 1;
			break;
		case (SRC_EMOSC):
			eSckSel = SEL_EMOSC;
			csp_em_flt_sel(SYSCON,EM_FLT_10NS);
			csp_em_flt_enable(SYSCON,ENABLE);
			if (wFreq == EMOSC_32K_VALUE)
				csp_set_em_lfmd(SYSCON, 1);
			ret = csi_emosc_enable(wFreq);
			break;
		case (SRC_HFOSC):	
			switch (wFreq) 	
			{
				case (HFOSC_24M_VALUE): byFreqIdx = 0;
					break;
				case (HFOSC_12M_VALUE): byFreqIdx = 1;
					break;
				case (HFOSC_6M_VALUE):  byFreqIdx = 2;
					break;
				case (HFOSC_3M_VALUE):  byFreqIdx = 3;
					break;
				default: ret = CSI_ERROR;
					return ret;
					break;
			}
			eSckSel = SEL_HFOSC;
			csi_hfosc_enable(byFreqIdx);
			break;
		case (SRC_AUTO_HF_PLL):	
			eSckSel = SEL_PLL;
			csi_pll_disable();
			if((wFreq < 2000000UL)||(wFreq > 120000000UL))
				return CSI_ERROR;
			apt_auto_pll_cfg(wFreq);	
			csi_hfosc_enable(0);   
			csp_pll_clk_sel(SYSCON, PLL_CLK_SEL_HFOSC);
			
			csp_pll_set_div_m(SYSCON, g_tPllClkConfig.byDivM);
			csp_pll_set_nul(SYSCON, g_tPllClkConfig.byNul);
			csp_pll_set_ckp_div(SYSCON, g_tPllClkConfig.byCkp_Div);
			csp_pll_set_ckq_div(SYSCON, g_tPllClkConfig.byCkq_Div);
			csp_pll_clk_enable(SYSCON, ENABLE);
			csi_pll_enable();			
			break;
		case (SRC_AUTO_EM_PLL):	
			eSckSel = SEL_PLL;
			csi_pll_disable();
			if((wFreq < 2000000UL)||(wFreq > 120000000UL))
				return CSI_ERROR;
			apt_auto_pll_cfg(wFreq);	
			csp_em_flt_sel(SYSCON,EM_FLT_10NS);
			csp_em_flt_enable(SYSCON,ENABLE);
			csi_emosc_enable(EMOSC_VALUE);         //EMOSC_VALUE
			csp_pll_clk_sel(SYSCON, PLL_CLK_SEL_EMOSC);	

			csp_pll_set_div_m(SYSCON, g_tPllClkConfig.byDivM);
			csp_pll_set_nul(SYSCON, g_tPllClkConfig.byNul);
			csp_pll_set_ckp_div(SYSCON, g_tPllClkConfig.byCkp_Div);
			csp_pll_set_ckq_div(SYSCON, g_tPllClkConfig.byCkq_Div);
			csp_pll_clk_enable(SYSCON, ENABLE);
			csi_pll_enable();					
			break;			
		case (SRC_MANUAL_PLL):	
			eSckSel = SEL_PLL;
			csi_pll_disable();
			if(g_tPllClkConfig.eClkSel == PLL_SEL_EMOSC_24M)
			{
				csp_em_flt_sel(SYSCON,EM_FLT_10NS);
				csp_em_flt_enable(SYSCON,ENABLE);
				csi_emosc_enable(EMOSC_VALUE);         //EMOSC_VALUE
				csp_pll_clk_sel(SYSCON, PLL_CLK_SEL_EMOSC);				
			}			
			else
			{
				switch(g_tPllClkConfig.eClkSel)
				{
					break;
					case (PLL_SEL_HFOSC_24M):	byFreqIdx = 0;
					break;
					case (PLL_SEL_HFOSC_12M):  byFreqIdx = 1;
					break;
					case (PLL_SEL_HFOSC_6M):   byFreqIdx = 2;
					break;
					default:
					break;
				}
				csi_hfosc_enable(byFreqIdx);   
				csp_pll_clk_sel(SYSCON, PLL_CLK_SEL_HFOSC);
				
			}
			csp_pll_set_div_m(SYSCON, g_tPllClkConfig.byDivM);
			csp_pll_set_nul(SYSCON, g_tPllClkConfig.byNul);
			csp_pll_set_ckp_div(SYSCON, g_tPllClkConfig.byCkp_Div);
			csp_pll_set_ckq_div(SYSCON, g_tPllClkConfig.byCkq_Div);
			csp_pll_clk_enable(SYSCON, ENABLE);
			csi_pll_enable();
			
			if(g_tPllClkConfig.eClkSel == PLL_SEL_EMOSC_24M)
			{
				wFreq = 24000000;
			}
			else if(g_tPllClkConfig.eClkSel == PLL_SEL_HFOSC_24M)
			{
				wFreq = 24000000;
			}
			else if(g_tPllClkConfig.eClkSel == PLL_SEL_HFOSC_12M)
			{
				wFreq = 12000000;				
			}
			else 
			{
				wFreq = 6000000;				
			}
			wFreq = wFreq /(g_tPllClkConfig.byDivM+1) * g_tPllClkConfig.byNul / (g_tPllClkConfig.byCkp_Div+1);
			wTargetSclk = wFreq/s_wHclkDiv[tClkCfg.eSdiv];
			
			break;
		case(SRC_ESOSC):
			eSckSel = SEL_ESOSC;
			csi_esosc_enable(wFreq);
			byFlashLp = 1;
			break;
		default: 
			break;
	}
	IFC->CEDR = IFC_CLKEN;
	if (wTargetSclk > 80000000) {
		csp_ifc_flash_set_speed_wait(IFC, HIGH_SPEED,PF_WAIT4);
	}
	else if(wTargetSclk > 64000000) {
		csp_ifc_flash_set_speed_wait(IFC, HIGH_SPEED,PF_WAIT3);
	}
	else if(wTargetSclk > 48000000) {
		csp_ifc_flash_set_speed_wait(IFC, HIGH_SPEED,PF_WAIT2);
	}
	else if(wTargetSclk > 16000000) {
		csp_ifc_flash_set_speed_wait(IFC, HIGH_SPEED,PF_WAIT1);
	}
	else if(wTargetSclk > 8000000) {
		csp_ifc_flash_set_speed_wait(IFC, HIGH_SPEED,PF_WAIT0);
	}
	else{
		csp_ifc_flash_set_speed_wait(IFC, LOW_SPEED,PF_WAIT0);
	}
	csp_set_sdiv(SYSCON, tClkCfg.eSdiv);
	csp_set_clksrc(SYSCON, eSckSel);
	
	
	csp_eflash_lpmd_enable(SYSCON, (bool)byFlashLp);
	
	csp_set_pdiv(SYSCON, tClkCfg.ePdiv);
	
	//update wSclk and wPclk in g_tClkConfig
	g_tClkConfig.wFreq = wTargetSclk * s_wHclkDiv[tClkCfg.eSdiv];
	g_tClkConfig.wSclk = wTargetSclk;
	g_tClkConfig.wPclk = g_tClkConfig.wSclk/(0x1<<tClkCfg.ePdiv);
	return ret;
}

/** \brief PLL clk manual config
 * 
 *  \param[in] tPllCfg: pll clock configuration 
 *  \param[in] wFreq: pll clk freq 
 *  \return csi_error_t.
 */
 csi_error_t csi_pll_manual_config(csi_pll_manual_config_t tPllCfg,uint32_t wFreq)
 {
	csi_error_t ret = CSI_OK;
	uint8_t byFreqIdx = 0;

	csi_pll_disable();
	if(tPllCfg.eClkSel == PLL_SEL_EMOSC_24M)
	{
		csp_em_flt_sel(SYSCON,EM_FLT_10NS);
		csp_em_flt_enable(SYSCON,ENABLE);
		csi_emosc_enable(EMOSC_VALUE);         //EMOSC_VALUE
		csp_pll_clk_sel(SYSCON, PLL_CLK_SEL_EMOSC);	
		g_tPllClkConfig.byDivM = tPllCfg.byDivM;
		g_tPllClkConfig.byNul = tPllCfg.byNul;
		g_tPllClkConfig.byCkp_Div = tPllCfg.byCkp_Div;
	}
	else
	{
		switch(tPllCfg.eClkSel)
		{
			break;
			case (PLL_SEL_HFOSC_24M):	byFreqIdx = 0;
			break;
			case (PLL_SEL_HFOSC_12M):  byFreqIdx = 1;
			break;
			case (PLL_SEL_HFOSC_6M):   byFreqIdx = 2;
			break;
			default:
			break;
		}
		csi_hfosc_enable(byFreqIdx);   
		csp_pll_clk_sel(SYSCON, PLL_CLK_SEL_HFOSC);
		
		g_tPllClkConfig.byDivM = tPllCfg.byDivM;
		g_tPllClkConfig.byNul = tPllCfg.byNul;
		g_tPllClkConfig.byCkp_Div = tPllCfg.byCkp_Div;
	}
	csp_pll_set_div_m(SYSCON, g_tPllClkConfig.byDivM);
	csp_pll_set_nul(SYSCON, g_tPllClkConfig.byNul);
	csp_pll_set_ckp_div(SYSCON, g_tPllClkConfig.byCkp_Div);
	csp_pll_set_ckq_div(SYSCON, g_tPllClkConfig.byCkq_Div);
	csp_pll_clk_enable(SYSCON, ENABLE);
	csi_pll_enable();

	return ret;
 }

/** \brief PLL clk auto config
 * 
 *  \param[in] ePllAutoSel: auto clk source
 *  \param[in] wFreq: pll clk freq 
 *  \return csi_error_t.
 */
 csi_error_t csi_pll_auto_config(csi_pll_auto_sel_e ePllAutoSel,uint32_t wFreq)
 {
	csi_error_t ret = CSI_OK;

	csi_pll_disable();

	if(ePllAutoSel == PLL_HFOSC_24M_AUTO)
	{
		if((wFreq < 2000000UL)||(wFreq > 120000000UL))
			return CSI_ERROR;
		apt_auto_pll_cfg(wFreq);	
		csi_hfosc_enable(0);   
		csp_pll_clk_sel(SYSCON, PLL_CLK_SEL_HFOSC);
	}
	else if(ePllAutoSel == PLL_EMOSC_24M_AUTO)
	{
		if((wFreq < 2000000UL)||(wFreq > 120000000UL))
			return CSI_ERROR;
		apt_auto_pll_cfg(wFreq);	
		csp_em_flt_sel(SYSCON,EM_FLT_10NS);
		csp_em_flt_enable(SYSCON,ENABLE);
		csi_emosc_enable(EMOSC_VALUE);         //EMOSC_VALUE
		csp_pll_clk_sel(SYSCON, PLL_CLK_SEL_EMOSC);		
	}
	else
	{
		return CSI_ERROR;
	}
	csp_pll_set_div_m(SYSCON, g_tPllClkConfig.byDivM);
	csp_pll_set_nul(SYSCON, g_tPllClkConfig.byNul);
	csp_pll_set_ckp_div(SYSCON, g_tPllClkConfig.byCkp_Div);
	csp_pll_set_ckq_div(SYSCON, g_tPllClkConfig.byCkq_Div);
	csp_pll_clk_enable(SYSCON, ENABLE);
	csi_pll_enable();

	return ret;
 }

/** \brief Clock output configuration
 * 
 *  \param[in] eCloSrc: source to output csi_clo_src_e
 *  \param[in] eCloDiv: clo divider csi_clo_div_e
 *  \return csi_error_t.
 */
csi_error_t csi_clo_config(csi_clo_src_e eCloSrc, csi_clo_div_e eCloDiv)
{ 	
	csi_error_t ret = CSI_OK;
	csp_set_clo_src(SYSCON, (clo_src_e)eCloSrc);
	csp_set_clo_div(SYSCON, (clo_div_e)eCloDiv);
	return ret;
}

/** \brief prei clk enable in SYSCON level
 *
 *  \param[in] wModule: module name
 *  \return none
 */
void soc_clk_enable(csi_clk_module_e eModule)
{
    //TODO
	if(eModule < 32U)
		csp_pcer0_clk_en(SYSCON, (uint32_t)eModule);
	else
		csp_pcer1_clk_en(SYSCON, (uint32_t)eModule - 32U);
}

/** \brief prei clk disable in SYSCON level
 *
 *  \param[in] wModule: module name
 *  \return none
 */
void soc_clk_disable(csi_clk_module_e eModule)
{
    //TODO
	if(eModule < 32U)
		csp_pder0_clk_dis(SYSCON, (uint32_t)eModule);
	else
		csp_pder1_clk_dis(SYSCON, (uint32_t)eModule - 32U);
}

/** \brief to calculate SCLK and PCLK frequence according to the current reg content
 *  g_tClkConfig.wSclk and g_tClkConfig.wPclk will be updated after excuting this function
 *  \param[in] none.
 *  \return csi_error_t.
 */
csi_error_t csi_calc_clk_freq(void)
{
	csi_sclk_sel_e eClkSrc;
	uint8_t  byHclkDiv;
	uint32_t wHfoFreq;
	uint32_t wImoFreq;
	
	//calculate sclk
	{
		eClkSrc = ((csi_sclk_sel_e) csp_get_clksrc(SYSCON));
		switch(eClkSrc)
		{ 	case (SEL_ISOSC): 	
				g_tClkConfig.wFreq = ISOSC_VALUE;
				break;
			case (SEL_EMOSC): 	
				g_tClkConfig.wFreq = EMOSC_VALUE;
				break;
			case (SEL_IMOSC):	
				wImoFreq = csp_get_imosc_fre(SYSCON);
				switch (wImoFreq)
				{
					case (0): 
						g_tClkConfig.wFreq = IMOSC_5M_VALUE;
						break;
					case (1): 
						g_tClkConfig.wFreq = IMOSC_4M_VALUE;
						break;
					case (2): 
						g_tClkConfig.wFreq = IMOSC_2M_VALUE;	
						break;
					case (3): 
						g_tClkConfig.wFreq = IMOSC_131K_VALUE;	
						break;
					default: 
						return CSI_ERROR;	
						break;
				}
				break;
			case  (SEL_HFOSC):	
				wHfoFreq =  csp_get_hfosc_fre(SYSCON);
				switch (wHfoFreq)
				{
					case (0): 
						g_tClkConfig.wFreq = HFOSC_24M_VALUE;
						break;
					case (1): 
						g_tClkConfig.wFreq = HFOSC_12M_VALUE;	
						break;
					case (2): 
						g_tClkConfig.wFreq = HFOSC_6M_VALUE;	
						break;
					default:  
						return CSI_ERROR;	
						break;
				}
				break;
			case (SEL_PLL): 
				break;
			case (SEL_ESOSC): 	
				g_tClkConfig.wFreq = ESOSC_VALUE;
				break;
			default:
				return CSI_ERROR;
				break;
		}
		byHclkDiv = csp_get_hclk_div(SYSCON);
		g_tClkConfig.wSclk = g_tClkConfig.wFreq/s_wHclkDiv[byHclkDiv];
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
		
		g_tClkConfig.wPclk = g_tClkConfig.wSclk / wPdiv;
	}

	return CSI_OK;
}

/** \brief to get SCLK frequence 
 *  \param[in] none.
 *  \return sclk value
 */ 
uint32_t csi_get_sclk_freq(void)
{	
	return g_tClkConfig.wSclk;
}

/** \brief To get PCLK frequence 
 *  \param[in] none.
 *  \return pclk value
 */ 
uint32_t csi_get_pclk_freq(void)
{
	return g_tClkConfig.wPclk;
}

/** \brief To get CORET frequence.
 *  Make sure to excute soc_get_cpu_freq() after clock block changing
 *  \param[in] none.
 *  \return g_wSystemClk.
 */ 
uint32_t soc_get_coret_freq(void)
{
	return g_tClkConfig.wSclk;
}
/** \brief to set clock status in PM mode 
 *  when IWDT is enabled, trying to stop ISOSC in stop mode would be invalid
 *  refer to GCER in SYSCON chapter for detailed description
 *  \param[in] eClk: clock to be configured
 *  \param[in] bEnable: enable or disable
 *  \return none.
 */ 
void csi_clk_pm_enable(csi_clk_pm_e eClk, bool bEnable)
{
	csp_clk_pm_enable(SYSCON, (clk_pm_e)eClk, bEnable);
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