/***********************************************************************//** 
 * \file  csp_fvr.h
 * \brief OPA description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2022 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-02-28 <td>V0.0  <td>XS    <td>initial
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CSP_FVR_H
#define _CSP_FVR_H

/* Includes ------------------------------------------------------------------*/
#include <soc.h>

/// \struct csp_fvr_t
/// \brief FVR reg description   
typedef volatile struct
{
	__IOM  uint32_t  CR;		//0x0000	Control Register                      
} 
csp_fvr_t;

/******************************************************************************
* CR: LCD Control Register 
******************************************************************************/
#define FVR_EN_POS			(0)				//FVR Enable/Disable
#define FVR_EN_MSK			(0x01ul << FVR_EN_POS)
typedef enum{
	FVR_DIS			= 0,
	FVR_EN
}fvr_en_e;

#define FVR_LVL_POS		(4)				//FVR LVL Control
#define FVR_LVL_MSK		(0x03ul << FVR_LVL_POS)
typedef enum{
	FVR_LVL_1V	= 0,
	FVR_LVL_2V,
	FVR_LVL_3V,
	FVR_LVL_4V
}fvr_lvl_e;

#define FVR_CMPREF_POS		(8)			//FVR CMPREF Control
#define FVR_CMPREF_MSK		(0x01ul << FVR_CMPREF_POS)
typedef enum{
	FVR_CMPREF_AVDD	= 0,
	FVR_CMPREF_FVR,
}fvr_cmpref_e;

#define FVR_OPACM_POS		(12)				//FVR OPA Control
#define FVR_OPACM_MSK		(0x01ul << FVR_OPACM_POS)
typedef enum{
	FVR_OPACM_VDD	= 0,
	FVR_OPACM_FVR,
}fvr_opacm_e;

#define FVR_BUFEN_POS		(16)				//FVR BUFEN Control
#define FVR_BUFEN_MSK		(0x01ul << FVR_BUFEN_POS)
typedef enum{
	FVR_BUFEN_DIS	= 0,
	FVR_BUFEN_EN,
}fvr_bufen_e;

#define FVR_BUFLVL_POS		(20)				//FVR BUFLVL Control
#define FVR_BUFLVL_MSK		(0x03ul << FVR_BUFLVL_POS)
typedef enum{
	FVR_BUFLVL_INPUT	= 0,
	FVR_BUFLVL_1V,
	FVR_BUFLVL_TS = 3
}fvr_buflvl_e;

#define FVR_BUFIN_POS		(23)				//FVR BUFIN Control
#define FVR_BUFIN_MSK		(0x01ul << FVR_BUFIN_POS)
typedef enum{
	FVR_BUFIN_DIS	= 0,
	FVR_BUFIN_EN,
}fvr_bufin_e;

#define FVR_BUFKEY_POS		(24)				//FVR BUFKEY 
#define FVR_BUFKEY_MSK		(0xfful << FVR_BUFKEY_POS)
/******************************************************************************
********************** FVR inline Functions Declaration ***********************
******************************************************************************/
static inline void csp_fvr_en(csp_fvr_t *ptFvrBase)
{
	ptFvrBase->CR |= FVR_EN_MSK;
}

static inline void csp_fvr_dis(csp_fvr_t *ptFvrBase)
{
	ptFvrBase->CR &= ~FVR_EN_MSK;
}

static inline void csp_fvr_set_lvl(csp_fvr_t *ptFvrBase, fvr_lvl_e eLvl)
{	
	ptFvrBase->CR = (ptFvrBase->CR & (~FVR_LVL_MSK)) | ((eLvl << FVR_LVL_POS));
}

static inline void csp_fvr_cmpref_set(csp_fvr_t *ptFvrBase,  fvr_cmpref_e eOpacmpref)
{
	ptFvrBase->CR = (ptFvrBase->CR & (~FVR_CMPREF_MSK)) | ((eOpacmpref << FVR_CMPREF_POS));
}

static inline void csp_fvr_opacm_set(csp_fvr_t *ptFvrBase, fvr_opacm_e eOpacm)
{
	ptFvrBase->CR = (ptFvrBase->CR & (~FVR_OPACM_MSK)) | ((eOpacm << FVR_OPACM_POS));
}

static inline void csp_fvr_bufen_en(csp_fvr_t *ptFvrBase)
{
	ptFvrBase->CR |= FVR_BUFEN_MSK;
}

static inline void csp_fvr_bufen_dis(csp_fvr_t *ptFvrBase)
{
	ptFvrBase->CR &= ~FVR_BUFEN_MSK;
}

static inline void csp_fvr_set_buflvl(csp_fvr_t *ptFvrBase, fvr_buflvl_e eBuflvl)
{
	ptFvrBase->CR |= eBuflvl << FVR_BUFLVL_POS;
}

static inline void csp_fvr_bufin_en(csp_fvr_t *ptFvrBase)
{
	ptFvrBase->CR |= FVR_BUFIN_MSK;
}

static inline void csp_fvr_bufin_dis(csp_fvr_t *ptFvrBase)
{
	ptFvrBase->CR  &= ~FVR_BUFIN_MSK;
}

static inline void csp_fvr_bufkey_unlock(csp_fvr_t *ptFvrBase)
{
	ptFvrBase->CR |= 0xb5 << FVR_BUFIN_POS;
}

#endif