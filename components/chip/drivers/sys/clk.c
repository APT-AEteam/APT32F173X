
/***********************************************************************//** 
 * \file  clk.c
 * \brief  clock management for all peripherals
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-10 <td>V0.0 <td>WNN   <td>initial
 * <tr><td> 2021-5-13 <td>V0.0 <td>ZJY   <td>initial
 * </table>
 * *********************************************************************
*/
#include <drv/clk.h>
#include <drv/pin.h>

#include "csp.h"
#include "board_config.h"

extern csi_clkmap_t g_tClkMap[];

/** \brief clock enable
 * 
 *  Enable peripheral clock in SYSCON
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
            soc_clk_enable((csi_clk_module_e)ptMap->wModule);
            break;
        }
		
        ptMap++;
    }
}

/** \brief clock disable
 * 
 *  Disable peripheral clock in SYSCON
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
            soc_clk_disable((csi_clk_module_e)ptMap->wModule);
            break;
        }
        ptMap++;
    }
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
	if (wFreq > 20000000)
		csp_set_em_gain(SYSCON, 0x1f);
	else
		csp_set_em_gain(SYSCON, 0x7);
		
	SYSCON->GCER = EMOSC;
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
	if ((SYSCON->SCLKCR & SYSCLK_SRC_MSK) == SC_EMOSC)
		return CSI_ERROR;
	else
	{
		SYSCON->GCDR = EMOSC;
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
csi_error_t csi_esosc_enable(uint32_t wFreq)
{
	csp_set_es_gain(SYSCON, 0x7);
	

	SYSCON->GCER = ESOSC;
	//wait for EMOSC to stable
	while(!(csp_get_ckst(SYSCON)& ESOSC));
	return CSI_OK;
}

/** \brief esosc disable
 * 
 *  disable external sub oscillator in SYSCON
 * 
 *  \param[in] none
 *  \return csi_error_t.
 */
csi_error_t csi_esosc_disable(void)
{
	if ((SYSCON->SCLKCR & SYSCLK_SRC_MSK) == SC_ESOSC)
		return CSI_ERROR;
	else
	{
		SYSCON->GCDR = ESOSC;
		return CSI_OK;
	}
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
	csp_set_imosc_fre(SYSCON, byFre);
	SYSCON->GCER = IMOSC;
	//wait for IMOSC to stable
	while(!(csp_get_ckst(SYSCON)& IMOSC));
	return CSI_OK;
}

/** \brief imosc disable
 * 
 *  disable internal main oscillator in SYSCON
 * 
 *  \param none
 *  \return csi_error_t.
 */
csi_error_t csi_imosc_disable(void)
{
	if ((SYSCON->SCLKCR & SYSCLK_SRC_MSK) == SC_IMOSC)
		return CSI_ERROR;
	else{
		SYSCON->GCDR = IMOSC;
		return CSI_OK;
	}
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
	csp_set_hfosc_fre(SYSCON, byFre);	
	SYSCON->GCER = HFOSC;
	//wait for HFOSC to stable
	while(!(csp_get_ckst(SYSCON)& HFOSC));
	return CSI_OK;
}

 /** \brief hfosc disable
 * 
 *  disable high frequency oscillator in SYSCON
 * 
 *  \param none
 *  \return csi_error_t
 */
csi_error_t csi_hfosc_disable(void)
{
	if ((SYSCON->SCLKCR & SYSCLK_SRC_MSK) == SC_HFOSC)
		return CSI_ERROR;
	else
	{
		SYSCON->GCDR = HFOSC;
		return CSI_OK;
	}
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
	SYSCON->GCER = ISOSC;
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
	if((SYSCON->SCLKCR & SYSCLK_SRC_MSK) == SC_ISOSC || csp_iwdt_rd_st(SYSCON))
		return CSI_ERROR;
	else 
	{
		SYSCON->GCDR = ISOSC;
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
	SYSCON->GCER = PLL;
	while(!(csp_get_ckst(SYSCON)& PLL));
	return CSI_OK;
}

/** \brief pll disable
 * 
 *  disable internal sub pll in SYSCON
 * 
 *  \param none
 *  \return csi_error_t.
 */
csi_error_t csi_pll_disable(void)
{
	if((SYSCON->SCLKCR & SYSCLK_SRC_MSK) == SC_PLL)
		return CSI_ERROR;
	else 
	{
		SYSCON->GCDR = PLL;
		return CSI_OK;
	}	
}