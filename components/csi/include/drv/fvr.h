/***********************************************************************//** 
 * \file  fvr.h
 * \brief fvr csi driver head file
 * \copyright Copyright (C) 2015-2022 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-02-28 <td>V0.0  <td>XS    <td>initial
 * </table>
 * *********************************************************************
*/
#ifndef _DRV_FVR_H_
#define _DRV_FVR_H_

#include <drv/common.h>
#include "csp.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef enum{
	LVL_1V	= 0,
	LVL_2V,
	LVL_3V,
	LVL_4V
}csi_fvr_lvl_e;

typedef enum{
	CMPREF_AVDD	= 0,
	CMPREF_FVR,
}csi_fvr_cmpref_e;

typedef enum{
	BUFLVL_INPUT	= 0,
	BUFLVL_1V,
	BUFLVL_TS = 3
}csi_fvr_buflvl_e;
/**
  \brief       set fvr lvl 
  \param[in]   ptFvrBase	pointer of fvr register structure
  \return      none
*/
void csi_fvr_lvl_set(csp_fvr_t *ptFvrBase, csi_fvr_lvl_e eLvl);

/**
  \brief       set fvr cmp reference 
  \param[in]   ptFvrBase	pointer of fvr register structure
  \return      none
*/
void csi_fvr_cmpref_set(csp_fvr_t *ptFvrBase, csi_fvr_cmpref_e eOpacmpref);

/**
  \brief       start fvr Hardware module 
  \param[in]   ptFvrBase	pointer of fvr register structure
  \return      none
*/
void csi_fvr_start(csp_fvr_t *ptFvrBase);

/**
  \brief       stop fvr Hardware module 
  \param[in]   ptFvrBase	pointer of fvr register structure
  \return      none
*/
void csi_fvr_stop(csp_fvr_t *ptFvrBase);

/**
  \brief       fvr buffer int 
  \param[in]   ptFvrBase	pointer of fvr register structure
  \return      none
*/
void csi_fvr_buf_init(csp_fvr_t *ptFvrBase,csi_fvr_buflvl_e eBuflvl);

/**
  \brief       set fvr opacm 
  \param[in]   ptFvrBase	pointer of fvr register structure
  \return      none
*/
void csi_fvr_opacm(csp_fvr_t *ptFvrBase);

#ifdef __cplusplus
}
#endif

#endif /* _DRV_FVR_H_ */