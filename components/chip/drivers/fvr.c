/***********************************************************************//** 
 * \file  fvr.c
 * \brief fvr csi driver
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-02-28 <td>V0.0  <td>XS    <td>initial
 * <tr><td> 2023-09-15 <td>V0.1  <td>XS    <td>fix bug,code normalization
 * </table>
 * *********************************************************************
*/
#include "drv/fvr.h"

/**
  \brief       set fvr lvl 
  \param[in]   ptFvrBase:	pointer of fvr register structure
  \param[in]   eLvl: 	    voltage  level selection
  \return      none
*/
void csi_fvr_set_lvl(csp_fvr_t *ptFvrBase, csi_fvr_lvl_e eLvl)
{
	csp_fvr_set_lvl(ptFvrBase,(fvr_lvl_e)eLvl);
}

/**
  \brief       set fvr cmp reference 
  \param[in]   ptFvrBase;	pointer of fvr register structure
  \param[in]   eCmpref:	 	compare voltage reference select 
  \return      none
*/
void csi_fvr_set_cmpref(csp_fvr_t *ptFvrBase, csi_fvr_cmpref_e eCmpref)
{
	csp_fvr_set_cmpref(ptFvrBase,(fvr_cmpref_e)eCmpref);
}

/**
  \brief       enable fvr Hardware module 
  \param[in]   ptFvrBase:	pointer of fvr register structure
  \return      none
*/
void csi_fvr_enable(csp_fvr_t *ptFvrBase)
{
	csp_fvr_enable(ptFvrBase);
}

/**
  \brief       disable fvr Hardware module 
  \param[in]   ptFvrBase:	pointer of fvr register structure
  \return      none
*/
void csi_fvr_disable(csp_fvr_t *ptFvrBase)
{
	csp_fvr_disable(ptFvrBase);
}

/**
  \brief       fvr buffer int 
  \param[in]   ptFvrBase:	pointer of fvr register structure
  \param[in]   eBuflvl:		buffin voltage  level selection
  \return      none
*/
void csi_buf_init(csp_fvr_t *ptFvrBase,csi_fvr_buflvl_e eBuflvl)
{
	csi_clk_enable((uint32_t *)ptFvrBase);
	csp_fvr_bufkey_unlock(ptFvrBase);
	csp_fvr_buf_enable(ptFvrBase);
	if(eBuflvl == FVR_BUF_LEVEL_EXTERNAL)
	{	
		csp_fvr_set_buflvl(ptFvrBase, (fvr_buflvl_e)eBuflvl);
		csp_fvr_bufin_enable(ptFvrBase);
	}
	else 
	{
		csp_fvr_bufin_disable(ptFvrBase);
		csp_fvr_set_buflvl(ptFvrBase, (fvr_buflvl_e)eBuflvl);
	}
}

/**
  \brief       set fvr opacm 
  \param[in]   ptFvrBase:	pointer of fvr register structure
  \param[in]   eOpacm:		opa voltage reference select
  \return      none
*/
void csi_fvr_set_opacm(csp_fvr_t *ptFvrBase,csi_fvr_opacm_e eOpacm)
{
	csp_fvr_set_opacm(ptFvrBase, (fvr_opacm_e)eOpacm);
}

/** \brief fvr init 
 * 
 *  \param[in] ptFvrBase:	pointer of fvr register structure
 *  \param[in] ptFvrCfg: 	pointer of fvr config structure
 *             	- eFvrLevel:Fvr output level, \ref csi_fvr_lvl_e
 *             	- eCmpref: 	Fvr to compare reference, \ref csi_fvr_cmpref_e
 * 			   	- eOpacm: 	Fvr to compare reference, \ref csi_fvr_opacm_e
 * 			   
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_fvr_init(csp_fvr_t *ptFvrBase,csi_fvr_config_t *ptFvrCfg)
{	
	csi_clk_enable((uint32_t *)ptFvrBase);
	csi_fvr_set_lvl(FVR, ptFvrCfg->eFvrLevel);
	csi_fvr_set_cmpref(FVR,ptFvrCfg->eCmpref);
	csi_fvr_set_opacm(FVR,ptFvrCfg->eOpacm);
	return CSI_OK;
}
