/***********************************************************************//** 
 * \file  cmp.c
 * \brief  csi cmp driver
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-10-14 <td>V0.0  <td>YYM   <td>modify
 * <tr><td> 2023-09-07 <td>V0.1  <td>LHY   <td>modify
 * </table>
 * *********************************************************************
*/
#include "drv/cmp.h"


csi_cmp_ctrl_t g_tCmpCtrl[CMP_IDX];

/* Private macro-----------------------------------------------------------*/
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/


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
	csp_cmp_clk_enable(ptCmpBase);
	csp_cmp_set_inpcr(ptCmpBase ,ptCmpCfg->byNsel,ptCmpCfg->byPsel);
	csp_cmp_set_hystpol(ptCmpBase , ptCmpCfg->byPhystpol,ptCmpCfg->byNhystpol);
	csp_cmp_set_speedup(ptCmpBase , SPEEDUP_FAST);
	csp_cmp_set_polarity(ptCmpBase , ptCmpCfg->byPolarity);
	csp_cmp_set_cposel(ptCmpBase , ptCmpCfg->byCpoSel);
	return tRet;
}

/** \brief start cmp
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \return none
 */
void csi_cmp_start(csp_cmp_t *ptCmpBase)
{
	csp_cmp_start(ptCmpBase);
}

/** \brief stop cmp
 * 
 *  \param[in] ptBtBase: pointer of cmp register structure
 *  \return none
 */ 
void csi_cmp_stop(csp_cmp_t *ptCmpBase)
{
    csp_cmp_stop(ptCmpBase);
}

/** \brief cmp set dflt1
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] bEnable: dflt1 enable or disable
 *  \param[in] ptCmpDfltCfg: pointer of cmp dlft config structure
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_cmp_set_dflt1(csp_cmp_t *ptCmpBase,csi_cmp_dflt1_config_t *ptCmpDfltCfg,bool bEnable)
{
	csi_error_t tRet = CSI_OK;
	if(bEnable == ENABLE)
	{
		csp_cmp_dflt1_enable(ptCmpBase);
		csp_cmp_set_dfcr1(ptCmpBase,ptCmpDfltCfg->byDepth1,ptCmpDfltCfg->byDivn1,ptCmpDfltCfg->byDivm1);
	}
	else
	{
		csp_cmp_dflt1_disable(ptCmpBase);		
	}
	return tRet;
}

/** \brief cmp set dflt2 
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] bEnable: dflt2 enable or disable
 *  \param[in] ptCmpDfltCfg: pointer of cmp dlft config structure
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_cmp_set_dflt2(csp_cmp_t *ptCmpBase,csi_cmp_dflt2_config_t *ptCmpDfltCfg,bool bEnable)
{
	csi_error_t tRet = CSI_OK;
	if(bEnable == ENABLE)
	{	
		csp_cmp_dflt2_enable(ptCmpBase);
		csp_cmp_set_dfcr2(ptCmpBase,ptCmpDfltCfg->byDepth2,ptCmpDfltCfg->byDivn2,ptCmpDfltCfg->byDivm2);
	}
	else
	{
		csp_cmp_dflt2_disable(ptCmpBase);
	}
	return tRet;
}

/** \brief cmp set wflt
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] ptCmpWfcrCfg: pointer of cmp wflt config structure
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_cmp_set_wfcr(csp_cmp_t *ptCmpBase,csi_cmp_wfcr_config_t *ptCmpWfcrCfg)
{
	csi_error_t tRet = CSI_OK;

	csp_cmp_set_wf(ptCmpBase, ptCmpWfcrCfg->byWfalign, ptCmpWfcrCfg->byWoset, ENABLE);
	csp_cmp_set_wfcr(ptCmpBase, ptCmpWfcrCfg->hwWcnt, ptCmpWfcrCfg->byClkDiv, ptCmpWfcrCfg->byDcnt);
	
	return tRet;
}

/** \brief cmp set evtrg output
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] eTrgSrc: evtrg eve sel(0~3) 
 *  \return none
 */
void csi_cmp_set_evtrg(csp_cmp_t *ptCmpBase,csi_cmp_trgsrc_e eTrgSrc)
{
	csp_cmp_set_evtrg(ptCmpBase,(cmp_trgsrc_e)eTrgSrc);
}

/** \brief cmp evtrg enable
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \return none
 */
void csi_cmp_evtrg_enable(csp_cmp_t *ptCmpBase)
{
	csp_cmp_evtrg_enable(ptCmpBase);
}

/** \brief cmp evtrg disable
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \return none
 */
void csi_cmp_evtrg_disable(csp_cmp_t *ptCmpBase)
{
	csp_cmp_evtrg_disable(ptCmpBase);
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
void csi_cmp_clr_isr(csp_cmp_t *ptCmpBase,csi_cmp_intsrc_e eIntMode)
{
	csp_cmp_clr_isr(ptCmpBase,(cmp_int_e)eIntMode);
}

/** \brief CMP interrupt enable control
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] eIntSrc: cmp interrupt source \ref csi_cmp_intsrc_e
 *  \return none
 */ 
void csi_cmp_int_enable(csp_cmp_t *ptCmpBase, csi_cmp_intsrc_e eIntSrc)
{
	csp_cmp_int_enable(ptCmpBase, (cmp_int_e)eIntSrc);
}

/** \brief CMP interrupt disable control
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] eIntSrc: cmp interrupt source \ref csi_cmp_intsrc_e
 *  \return none
 */ 
void csi_cmp_int_disable(csp_cmp_t *ptCmpBase, csi_cmp_intsrc_e eIntSrc)
{
	csp_cmp_int_disable(ptCmpBase, (cmp_int_e)eIntSrc);
}


/** \brief get cmp number 
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \return cmp number 0/1/2
 */ 
static uint8_t apt_get_cmp_idx(csp_cmp_t *ptCmpBase)
{
	switch((uint32_t)ptCmpBase)
	{
		case APB_CMP0_BASE:                //CMP0
			return 0;     
		case APB_CMP1_BASE:                //CMP1
			return 1; 
		case APB_CMP2_BASE:                //CMP2
			return 2;            
		default:
			return 0xff;                //error
	}
}

/** \brief  register cmp interrupt callback function
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] callback: cmp interrupt handle function
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_cmp_register_callback(csp_cmp_t *ptCmpBase, void  *callback)
{
	uint8_t byIdx = apt_get_cmp_idx(ptCmpBase);
	if(byIdx == 0xff)
		return CSI_ERROR;
			
	g_tCmpCtrl[byIdx].callback = callback;
	
	return CSI_OK;
}

/** \brief cmp interrupt handler function
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] byIdx: cmp idx(0)
 *  \return none
 */ 
void csi_cmp_irqhandler(csp_cmp_t *ptCmpBase,  uint8_t byIdx)
{
	uint8_t byIsr = csp_cmp_get_isr(ptCmpBase);
	
	if(g_tCmpCtrl[byIdx].callback)
		g_tCmpCtrl[byIdx].callback(ptCmpBase, byIsr);
			
	csp_cmp_clr_isr(ptCmpBase, CMP_EDGEDET_INT);
}