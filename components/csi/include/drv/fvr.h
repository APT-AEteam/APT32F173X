/***********************************************************************//** 
 * \file  fvr.h
 * \brief fvr csi driver head file
 * \copyright Copyright (C) 2015-2022 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-02-28 <td>V0.0  <td>XS    <td>initial
 * <tr><td> 2023-09-15 <td>V0.1  <td>XS    <td>modified
 * </table>
 * *********************************************************************
*/
#ifndef _DRV_FVR_H_
#define _DRV_FVR_H_

#include "csp.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum{
	LVL_1V			= 0,
	LVL_2V,
	LVL_3V,
	LVL_4V
}csi_fvr_lvl_e;

typedef enum{
	CMPREF_AVDD		= 0,
	CMPREF_FVR,
}csi_fvr_cmpref_e;

typedef enum{
	BUFLVL_EXTERNAL	= 0,
	BUFLVL_1V,
	BUFLVL_TS 		= 3
}csi_fvr_buflvl_e;

typedef enum{
	OPACM_HALFAVDD	= 0,
	OPACM_HALFFVR,
}csi_fvr_opacm_e;

/// \struct csi_fvr_config_t
/// \brief  fvr configuration, open to users  
typedef struct {
	csi_fvr_lvl_e		eFvrLevel;			//Fvr output level
	csi_fvr_cmpref_e	eCmpref;			//Fvr to compare reference 
	csi_fvr_opacm_e		eOpacm;				//Fvr to compare reference 
} csi_fvr_config_t;

/**
  \brief       set fvr lvl 
  \param[in]   ptFvrBase:	pointer of fvr register structure
  \param[in]   eLvl: 	    voltage  level selection
  \return      none
*/
void csi_fvr_set_lvl(csp_fvr_t *ptFvrBase, csi_fvr_lvl_e eLvl);

/**
  \brief       set fvr cmp reference 
  \param[in]   ptFvrBase;	pointer of fvr register structure
  \param[in]   eCmpref:	 	compare voltage reference select 
  \return      none
*/
void csi_fvr_set_cmpref(csp_fvr_t *ptFvrBase, csi_fvr_cmpref_e eCmpref);

/**
  \brief       enable fvr Hardware module 
  \param[in]   ptFvrBase:	pointer of fvr register structure
  \return      none
*/
void csi_fvr_enable(csp_fvr_t *ptFvrBase);

/**
  \brief       disable fvr Hardware module 
  \param[in]   ptFvrBase:	pointer of fvr register structure
  \return      none
*/
void csi_fvr_disable(csp_fvr_t *ptFvrBase);

/**
  \brief       fvr buffer int 
  \param[in]   ptFvrBase:	pointer of fvr register structure
  \param[in]   eBuflvl:		buffin voltage  level selection
  \return      none
*/
void csi_buf_init(csp_fvr_t *ptFvrBase,csi_fvr_buflvl_e eBuflvl);

/**
  \brief       set fvr opacm 
  \param[in]   ptFvrBase:	pointer of fvr register structure
  \param[in]   eOpacm:		opa voltage reference select
  \return      none
*/
void csi_fvr_set_opacm(csp_fvr_t *ptFvrBase,csi_fvr_opacm_e eOpacm);

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
csi_error_t csi_fvr_init(csp_fvr_t *ptFvrBase,csi_fvr_config_t *ptFvrCfg);

#ifdef __cplusplus
}
#endif

#endif /* _DRV_FVR_H_ */