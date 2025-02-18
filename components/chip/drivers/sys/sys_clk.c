/***********************************************************************//** 
 * \file  sys_clk.c
 * \brief  system clock management for cpu clock(HCLK) and peri clock(PCLK)
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-10 <td>V0.0  <td>WNN   <td>initial
 * <tr><td> 2021-5-13 <td>V0.0  <td>ZJY   <td>initial
 * <tr><td> 2023-9-22 <td>V0.1  <td>GQQ   <td>fix bug,code normalization
 * </table>
 * *********************************************************************
*/

#include "csp.h"
#include "sys_clk.h"
#include "board_config.h"

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

/** \brief apt_pll_config: config div_m,nul,ckp_div and ckq_div
 * 
 *  \param[in] eSrc: source clock \ref  csi_clk_src_e
 *  \param[in] wFreq:  pll clk freq 
 *  \return csi_error_t.
 */	
static csi_error_t apt_pll_config(csi_clk_src_e eSrc,uint32_t wFreq)
{
	
	csi_pll_disable();
	
	if(eSrc == SRC_HF_PLL)
		csp_pll_set_clk_src(SYSCON, PLL_CLK_SEL_HFOSC);	
	else if(eSrc == SRC_EM_PLL)
		csp_pll_set_clk_src(SYSCON, PLL_CLK_SEL_EMOSC);	
	else
		 return CSI_ERROR;
		
	csp_pll_set_div_m(SYSCON, g_tPllClkConfig.byDivM);
	csp_pll_set_nul(SYSCON, g_tPllClkConfig.byNul);
	csp_pll_set_ckp_div(SYSCON, g_tPllClkConfig.byCkp_Div);
	csp_pll_set_ckq_div(SYSCON, g_tPllClkConfig.byCkq_Div);
	csp_pll_clk_enable(SYSCON);
	csi_pll_enable();
	
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
	
	csp_eflash_lpmd_disable(SYSCON);					//disable Flash LP Mode	
	wTargetSclk = wFreq/s_wHclkDiv[tClkCfg.eSdiv];
	eSrc = tClkCfg.eClkSrc;
	csi_imosc_enable(byFreqIdx);		 //enable IM_5M
	csp_set_clksrc(SYSCON, SRC_IMOSC);
	
	switch (eSrc)
	{
		case (SRC_ISOSC): 
			eSckSel = SEL_ISOSC;
			csi_isosc_enable();
			byFlashLp = 1;
			break;
		case (SRC_IMOSC):	
			eSckSel = SEL_IMOSC;
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
			eSckSel = SEL_EMOSC;
			if (wFreq == EMOSC_32K_VALUE)
				csp_em_lfmd_enable(SYSCON);
			ret = csi_emosc_enable(wFreq);
			break;
		case (SRC_HFOSC):	
			eSckSel = SEL_HFOSC;
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
			csi_hfosc_enable(byFreqIdx);
			break;
			
		case (SRC_HF_PLL):
			eSckSel = SEL_PLL;
			switch (g_tPllClkConfig.eClkSel) 	
			{
				case (PLL_SEL_HFOSC_24M): byFreqIdx = 0;
					wFreq = HFOSC_24M_VALUE;
					break;
				case (PLL_SEL_HFOSC_12M): byFreqIdx = 1;
					wFreq = HFOSC_12M_VALUE;
					break;
				case (PLL_SEL_HFOSC_6M):  byFreqIdx = 2;
					wFreq = HFOSC_6M_VALUE;
					break;
				default: ret = CSI_ERROR;
					return ret;
					break;
			}
			csi_hfosc_enable(byFreqIdx);
			break;
		case (SRC_EM_PLL):
			eSckSel = SEL_PLL;
			wFreq = EMOSC_24M_VALUE;
			ret = csi_emosc_enable(wFreq);
			break;
		case(SRC_ESOSC):
			eSckSel = SEL_ESOSC;
			csi_esosc_enable();
			byFlashLp = 1;
			break;
		default: 
			break;
	}
	
	if(eSrc >= SRC_HF_PLL)  //config and enable pll register
	{
		apt_pll_config(eSrc,wFreq);  
		
		wFreq = wFreq /(g_tPllClkConfig.byDivM+1) * g_tPllClkConfig.byNul / (g_tPllClkConfig.byCkp_Div+1);
		wTargetSclk = wFreq/s_wHclkDiv[tClkCfg.eSdiv];
	}

	IFC->CEDR = IFC_CLKEN;
	if (wTargetSclk > 80000000) {
		csp_ifc_flash_set_speed_wait(IFC, IFC_HIGH_SPEED,IFC_PF_WAIT4);
	}
	else if(wTargetSclk > 64000000) {
		csp_ifc_flash_set_speed_wait(IFC, IFC_HIGH_SPEED,IFC_PF_WAIT3);
	}
	else if(wTargetSclk > 48000000) {
		csp_ifc_flash_set_speed_wait(IFC, IFC_HIGH_SPEED,IFC_PF_WAIT2);
	}
	else if(wTargetSclk > 16000000) {
		csp_ifc_flash_set_speed_wait(IFC, IFC_HIGH_SPEED,IFC_PF_WAIT1);
	}
	else if(wTargetSclk > 8000000) {
		csp_ifc_flash_set_speed_wait(IFC, IFC_HIGH_SPEED,IFC_PF_WAIT0);
	}
	else{
		csp_ifc_flash_set_speed_wait(IFC, IFC_LOW_SPEED,IFC_PF_WAIT0);
	}
	csp_set_sdiv(SYSCON, tClkCfg.eSdiv);
	csp_set_clksrc(SYSCON, eSckSel);
	
	if(byFlashLp)
		csp_eflash_lpmd_enable(SYSCON);
	
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
 csi_error_t csi_pll_manual_config(csi_pll_config_t tPllCfg,uint32_t wFreq)
 {
	csi_error_t ret = CSI_OK;
	uint8_t byFreqIdx = 0;

	csi_pll_disable();
	if(tPllCfg.eClkSel == PLL_SEL_EMOSC_24M)
	{
		csi_emosc_enable(EMOSC_VALUE);         //EMOSC_VALUE
		csp_pll_set_clk_src(SYSCON, PLL_CLK_SEL_EMOSC);	
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
		csp_pll_set_clk_src(SYSCON, PLL_CLK_SEL_HFOSC);
		
		g_tPllClkConfig.byDivM = tPllCfg.byDivM;
		g_tPllClkConfig.byNul = tPllCfg.byNul;
		g_tPllClkConfig.byCkp_Div = tPllCfg.byCkp_Div;
	}
	csp_pll_set_div_m(SYSCON, g_tPllClkConfig.byDivM);
	csp_pll_set_nul(SYSCON, g_tPllClkConfig.byNul);
	csp_pll_set_ckp_div(SYSCON, g_tPllClkConfig.byCkp_Div);
	csp_pll_set_ckq_div(SYSCON, g_tPllClkConfig.byCkq_Div);
	csp_pll_clk_enable(SYSCON);
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
void soc_clk_enable(csi_pclk_e ePclk)
{
    //TODO
	if(ePclk < 32U)
		csp_pcer0_clk_enable(SYSCON, (uint32_t)ePclk);
	else
		csp_pcer1_clk_enable(SYSCON, ((uint32_t)ePclk - 32U));
}

/** \brief prei clk disable in SYSCON level
 *
 *  \param[in] wModule: module name
 *  \return none
 */
void soc_clk_disable(csi_pclk_e ePclk)
{
    //TODO
	if(ePclk < 32U)
		csp_pder0_clk_disable(SYSCON, (uint32_t)ePclk);
	else
		csp_pder1_clk_disable(SYSCON, ((uint32_t)ePclk - 32U));
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
		eClkSrc = ((csi_sclk_sel_e) csp_get_clk_src(SYSCON));
		switch(eClkSrc)
		{ 	case (SEL_ISOSC): 	
				g_tClkConfig.wFreq = ISOSC_VALUE;
				break;
			case (SEL_EMOSC): 	
				g_tClkConfig.wFreq = EMOSC_VALUE;
				break;
			case (SEL_IMOSC):	
				wImoFreq = csp_get_imosc_freq(SYSCON);
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
				wHfoFreq =  csp_get_hfosc_freq(SYSCON);
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
			case (SEL_PLL): ///add
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
