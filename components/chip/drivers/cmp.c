/***********************************************************************//** 
 * \file  cmp.c
 * \brief  csi cmp driver
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-10-14 <td>V0.0  <td>YYM   <td>modify
 * </table>
 * *********************************************************************
*/
#include <sys_clk.h>
#include <drv/lpt.h>
#include <drv/irq.h>
#include <drv/gpio.h>
#include <drv/pin.h>
#include <drv/tick.h>
#include <drv/cmp.h>
#include "board_config.h"

/* Private macro-----------------------------------------------------------*/
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

/** \brief Enable cmp power manage
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] eIntSrc: cmp interrupt source
 *  \param[in] bEnable: cmp irq enable or disable
 *  \return none
 */
void csi_cmp_int_enable(csp_cmp_t *ptCmpBase, csi_cmp_intsrc_e eIntSrc,bool bEnable)
{
	csp_cmp_int_enable(ptCmpBase, (cmp_int_e)eIntSrc,bEnable);
	if (bEnable) 
	{
		csi_irq_enable((uint32_t *)ptCmpBase);
	}
	else 
	{
		if (csp_cmp_get_imcr(ptCmpBase)) 
		{
			csi_irq_disable((uint32_t *)ptCmpBase);
		}
	}
}

/** \brief init cmp
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] ptCmpCfg: pointer of cmp parameter config structure
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_cmp_init(csp_cmp_t *ptCmpBase,csi_cmp_config_t *ptCmpCfg)
{
	csi_error_t tRet = CSI_OK;
	csi_clk_enable((uint32_t *)ptCmpBase);
	csp_cmp_clk_enable(ptCmpBase, ENABLE);
	csp_cmp_inpcr(ptCmpBase ,ptCmpCfg->byNsel,ptCmpCfg->byPsel);
	csp_cmp_hystpol(ptCmpBase , ptCmpCfg->byPhystpol,ptCmpCfg->byNhystpol);
	csp_cmp_speedup(ptCmpBase , SPEEDUP_FAST);
	csp_cmp_polarity(ptCmpBase , ptCmpCfg->byPolarity);
	csp_cmp_out(ptCmpBase , ptCmpCfg->byCpoSel);
	csi_cmp_int_enable(ptCmpBase, ptCmpCfg->wInt,ENABLE);
	return tRet;
}

/** \brief start cmp
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \return none
 */
void csi_cmp_start(csp_cmp_t *ptCmpBase)
{
	csp_cmp_enable(ptCmpBase);
}

/** \brief stop cmp
 * 
 *  \param[in] ptBtBase: pointer of cmp register structure
 *  \return none
 */ 
void csi_cmp_stop(csp_cmp_t *ptCmpBase)
{
    csp_cmp_disable(ptCmpBase);
}

/** \brief cmp dflt1 config
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] bEnable: dflt1 enable or disable
 *  \param[in] ptCmpDfltCfg: pointer of cmp dlft config structure
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_cmp_dflt1_config(csp_cmp_t *ptCmpBase,bool bEnable,csi_cmp_dflt1_config_t *ptCmpDfltCfg)
{
	csi_error_t tRet = CSI_OK;
	if(bEnable == ENABLE)
	{
		csp_cmp_dflt1_enable(ptCmpBase ,ENABLE);
		csp_cmp_dfcr1(ptCmpBase,ptCmpDfltCfg->byDepth1,ptCmpDfltCfg->byDivn1,ptCmpDfltCfg->byDivm1);
	}
	else
	{
		csp_cmp_dflt1_enable(ptCmpBase ,DISABLE);		
	}
	return tRet;
}

/** \brief cmp dflt2 config
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] bEnable: dflt2 enable or disable
 *  \param[in] ptCmpDfltCfg: pointer of cmp dlft config structure
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_cmp_dflt2_config(csp_cmp_t *ptCmpBase,bool bEnable,csi_cmp_dflt2_config_t *ptCmpDfltCfg)
{
	csi_error_t tRet = CSI_OK;
	if(bEnable == ENABLE)
	{	
		csp_cmp_dflt2_enable(ptCmpBase ,ENABLE);
		csp_cmp_dfcr2(ptCmpBase,ptCmpDfltCfg->byDepth2,ptCmpDfltCfg->byDivn2,ptCmpDfltCfg->byDivm2);
	}
	else
	{
		csp_cmp_dflt2_enable(ptCmpBase ,DISABLE);
	}
	return tRet;
}

/** \brief cmp wflt config
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] ptCmpWfcrCfg: pointer of cmp wflt config structure
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_cmp_wfcr_config(csp_cmp_t *ptCmpBase,csi_cmp_wfcr_config_t *ptCmpWfcrCfg)
{
	csi_error_t tRet = CSI_OK;

	csp_cmp_wf_set(ptCmpBase,ENABLE,ptCmpWfcrCfg->byWfalign,ptCmpWfcrCfg->byWoset);
	csp_cmp_wfcr(ptCmpBase , ptCmpWfcrCfg->hwWcnt,ptCmpWfcrCfg->byClkDiv,ptCmpWfcrCfg->byDcnt);
	
	return tRet;
}

/** \brief cmp evtrg output config
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] eEveSel: evtrg eve sel(0~3) 
 *  \param[in] bEnable: cmp evtrg enable or disable
 *  \return none
 */
void csi_cmp_set_evtrg(csp_cmp_t *ptCmpBase,csi_eve_sel_e eEveSel, bool bEnable)
{
	csp_cmp_evtrg(ptCmpBase , bEnable,(cmp_eve_sel_e)eEveSel);
}

/** \brief cmp out status
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] byOutCh: cmp out ch sel(cmp0~cmp3)
 *  \return out status(0~1)
 */
uint8_t csi_cmp_get_out(csp_cmp_t *ptCmpBase,uint8_t byOutCh)
{
	if(byOutCh == CMP0_OUT)
		return csp_cmp_get_out0(ptCmpBase);
	else if(byOutCh == CMP1_OUT)
		return csp_cmp_get_out1(ptCmpBase);
	else if(byOutCh == CMP2_OUT)
		return csp_cmp_get_out2(ptCmpBase);
	else 
		return csp_cmp_get_out3(ptCmpBase);
}

/** \brief clear cmp int
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] eIntMode: EDGEDET_MODE or RAWDET_MODE
 *  \return none
 */
void csi_cmp_int_clear(csp_cmp_t *ptCmpBase,csi_cmp_intsrc_e eIntMode)
{
	csp_cmp_int_clear(ptCmpBase,(cmp_int_e)eIntMode);
}

/** \brief get cmp status
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \return cmp int status
 */
uint32_t csi_cmp_get_misr(csp_cmp_t *ptCmpBase)
{
	return csp_cmp_get_misr(ptCmpBase);
}
