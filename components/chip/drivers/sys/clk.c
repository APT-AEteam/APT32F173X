
/***********************************************************************//** 
 * \file  clk.c
 * \brief  clock management for all peripherals
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-10 <td>V0.0 <td>WNN   <td>initial
 * <tr><td> 2021-5-13 <td>V0.0 <td>ZJY   <td>initial
 * <tr><td> 2023-9-22 <td>V0.1  <td>GQQ   <td>fix bug,code normalization
 * </table>
 * *********************************************************************
*/

#include "csp.h"
#include "drv/clk.h"


extern csi_clkmap_t g_tClkMap[];

/** \brief enable peripheral clock
 * 
 *  \param[in] pIpBase: pointer of peripheral Base address
 *  \return none.
 */
void csi_clk_enable(uint32_t *pIpBase)
{
    csi_clkmap_t *ptMap = g_tClkMap;

    while(ptMap->wRegBase) 
	{
        if((uint32_t)pIpBase == ptMap->wRegBase) 
		{
            soc_clk_enable((csi_pclk_e)ptMap->wModule);
            break;
        }
		
        ptMap++;
    }
}

/** \brief disable peripheral clock 
 * 
 *  \param[in] pIpBase: pointer of peripheral Base address
 *  \return none.
 */
void csi_clk_disable(uint32_t *pIpBase)
{
    csi_clkmap_t *ptMap = g_tClkMap;

	while(ptMap->wRegBase)
	{
        if((uint32_t)pIpBase == ptMap->wRegBase) 
		{
            soc_clk_disable((csi_pclk_e)ptMap->wModule);
            break;
        }
        ptMap++;
    }
}

/** \brief fast enable peripheral clock
 * 
 *  \param[in] ePclk: peripheral clock \ref csi_pclk_e
 *  \return none.
 */
void csi_pclk_enable(csi_pclk_e ePclk)
{
	soc_clk_enable(ePclk);
}

/** \brief fast disable peripheral clock
 * 
 *  \param[in] ePclk: peripheral clock \ref csi_pclk_e
 *  \return none.
 */
void csi_pclk_disable(csi_pclk_e ePclk)
{
	soc_clk_disable(ePclk);
}

/** \brief emosc filter enable
 * 
 * 
 *  \param[in] eFltSel: filter select \ref csi_em_fltsel_e
 *  \return none
 */
void csi_emosc_flt_enable(csi_em_fltsel_e eFltSel)
{
	csp_em_set_flt(SYSCON,(em_fltsel_e)eFltSel);		//	filter pulse
	csp_em_flt_enable(SYSCON);							//  enable filter
	
}

/** \brief emosc enable
 * 
 *  enable external main oscillator in SYSCON
 * 
 *  \param none
 *  \return csi_error_t
 */
csi_error_t csi_emosc_enable(uint32_t wFreq)
{
	csp_em_set_flt(SYSCON,(em_fltsel_e)EMFLT_10NS);	//	filter <10ns pulse
	csp_em_flt_enable(SYSCON);			//  enable filter
	
	if (wFreq > 20000000)
		csp_em_set_gain(SYSCON, 0x1f);
	else
		csp_em_set_gain(SYSCON, 0x7);
		
	csp_src_clk_enable(SYSCON,EMOSC);
	//wait for EMOSC to stable
	while(!(csp_get_ckst(SYSCON)& EMOSC));
	return CSI_OK;
}

/** \brief emosc disable
 * 
 *  disable external main oscillator in SYSCON
 * 
 *  \param none
 *  \return csi_error_t.
 */
csi_error_t csi_emosc_disable(void)
{
	if((csp_get_clk_src(SYSCON) == SC_PLL)&&(csp_pll_get_clk_src(SYSCON) == PLL_CLK_SEL_EMOSC)) //PLL base on EM
		return CSI_ERROR;
	else
	{
		csp_src_clk_disable(SYSCON,EMOSC);
		return CSI_OK;
	}
}

/** \brief esosc enable
 * 
 *  enable external sub oscillator in SYSCON
 * 
 *  \param[in] none
 *  \return csi_error_t
 */
csi_error_t csi_esosc_enable(void)
{
	csp_es_set_gain(SYSCON, 0x7);
	
	csp_src_clk_enable(SYSCON,ESOSC);
	//wait for ESOSC to stable
	while(!(csp_get_ckst(SYSCON)& ESOSC));
	return CSI_OK;
}

/** \brief esosc disable
 * 
 *  disable external sub oscillator in SYSCON
 * 
 *  \param[in] none
 *  \return none
 */
void csi_esosc_disable(void)
{
	csp_src_clk_disable(SYSCON,ESOSC);
}

/** \brief imosc enable
 * 
 *  enable internal main oscillator in SYSCON
 * 
 *  \param byFre. 	0 - 5MHz
 * 					1 - 4MHz
 * 					2 - 2MHz
					3 - 131KHz
 *  \return csi_error_t
 */
csi_error_t csi_imosc_enable(uint8_t byFre)
{
	csp_set_imosc_freq(SYSCON, byFre);
	csp_src_clk_enable(SYSCON,IMOSC);
	//wait for IMOSC to stable
	while(!(csp_get_ckst(SYSCON)& IMOSC));
	return CSI_OK;
}

/** \brief imosc disable
 * 
 *  disable internal main oscillator in SYSCON
 * 
 *  \param[in] none
 *  \return none.
 */
void csi_imosc_disable(void)
{
	csp_src_clk_disable(SYSCON,IMOSC);
}

 /** \brief hfosc enable
 * 
 *  enable high frequency oscillator in SYSCON
 * 
 *  \param byFre. 	0 - 24MHz
 * 					1 - 12MHz
 * 					2 - 6MHz
					3 - 3MHz
 *  \return csi_error_t.
 */
csi_error_t csi_hfosc_enable(uint8_t byFre)
{
	csp_set_hfosc_freq(SYSCON, byFre);	
	
	csp_src_clk_enable(SYSCON,HFOSC);
	//wait for HFOSC to stable
	while(!(csp_get_ckst(SYSCON)& HFOSC));
	return CSI_OK;
}

 /** \brief hfosc disable
 * 
 *  disable high frequency oscillator in SYSCON
 * 
 *  \param none
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_hfosc_disable(void)
{
	if((csp_get_clk_src(SYSCON) == SC_PLL)&&(csp_pll_get_clk_src(SYSCON) == PLL_CLK_SEL_HFOSC)) //PLL base one HF
		return CSI_ERROR;
		
	csp_src_clk_disable(SYSCON,HFOSC);
	return CSI_OK;
}

/** \brief isosc enable
 * 
 *  enable internal sub oscillator in SYSCON
 * 
 *  \param none
 *  \return csi_error_t
 */
csi_error_t csi_isosc_enable(void)
{
	csp_src_clk_enable(SYSCON,ISOSC);
	//wait for ISOSC to stable
	while(!(csp_get_ckst(SYSCON)& ISOSC));
	return CSI_OK;
}

/** \brief isosc disable
 * 
 *  disable internal sub oscillator in SYSCON
 * 
 *  \param none
 *  \return csi_error_t.
 */
csi_error_t csi_isosc_disable(void)
{
	if(csp_iwdt_get_status(SYSCON))
		return CSI_ERROR;
	else 
	{
		csp_src_clk_disable(SYSCON,ISOSC);
		return CSI_OK;
	}	
}

/** \brief pll enable
 * 
 *  enable internal sub pll in SYSCON
 * 
 *  \param none
 *  \return csi_error_t
 */
csi_error_t csi_pll_enable(void)
{
	csp_src_clk_enable(SYSCON,PLL);
	while(!(csp_get_ckst(SYSCON)& PLL));
	return CSI_OK;
}

/** \brief pll disable
 * 
 *  disable internal sub pll in SYSCON
 * 
 *  \param none
 *  \return none.
 */
void csi_pll_disable(void)
{
	csp_src_clk_disable(SYSCON,PLL);
}