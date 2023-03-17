/***********************************************************************//** 
 * \file  fvr.c
 * \brief fvr csi driver
 * \copyright Copyright (C) 2015-2022 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-02-28 <td>V0.0  <td>XS    <td>initial
 * </table>
 * *********************************************************************
*/
#include <drv/fvr.h>
#include "csp_fvr.h"
/**
  \brief       initializes the resources needed for the FVR interface.
  \param[in]   ptFvrBase	pointer of fvr register structure
  \param[in]   ptFvrCfg    pointer of fvr parameter config structure
  \return      error code.
*/
/**
  \brief       set fvr lvl 
  \param[in]   ptFvrBase	pointer of fvr register structure
  \return      none
*/
void csi_fvr_lvl_set(csp_fvr_t *ptFvrBase, fvr_lvl_e eLvl)
{
	csp_fvr_set_lvl(ptFvrBase,eLvl);

}
/**
  \brief       set fvr cmp reference 
  \param[in]   ptFvrBase	pointer of fvr register structure
  \return      none
*/
void csi_fvr_cmpref_set(csp_fvr_t *ptFvrBase, fvr_cmpref_e eOpacmpref)
{
	csp_fvr_cmpref_set(ptFvrBase,eOpacmpref);
}

/**
  \brief       start fvr Hardware module 
  \param[in]   ptFvrBase	pointer of fvr register structure
  \return      none
*/
void csi_fvr_start(csp_fvr_t *ptFvrBase)
{
	csp_fvr_en(ptFvrBase);
}


/**
  \brief       stop fvr Hardware module 
  \param[in]   ptFvrBase	pointer of fvr register structure
  \return      none
*/
void csi_fvr_stop(csp_fvr_t *ptFvrBase)
{
	csp_fvr_dis(ptFvrBase);
	
}

/**
  \brief       fvr buffer int 
  \param[in]   ptFvrBase	pointer of fvr register structure
  \return      none
*/
void csi_fvr_buf_init(csp_fvr_t *ptFvrBase,fvr_buflvl_e eBuflvl)
{
	csp_fvr_bufkey_unlock(ptFvrBase);
	csp_fvr_bufen_en(ptFvrBase);
	if(eBuflvl == FVR_BUFLVL_INPUT)
	{	
		csp_fvr_set_buflvl(ptFvrBase, eBuflvl);
		csp_fvr_bufin_en(ptFvrBase);
	}
	else 
	{
		csp_fvr_bufin_dis(ptFvrBase);
		csp_fvr_set_buflvl(ptFvrBase, eBuflvl);
	}
}

/**
  \brief       set fvr opacm 
  \param[in]   ptFvrBase	pointer of fvr register structure
  \return      none
*/
void csi_fvr_opacm(csp_fvr_t *ptFvrBase)
{
	csp_fvr_opacm_set(ptFvrBase, FVR_OPACM_FVR);
	
}