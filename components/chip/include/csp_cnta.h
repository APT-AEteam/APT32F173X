/***********************************************************************//** 
 * \file  csp_cnta.h
 * \brief CNTA description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date       <th>Version   <th>Author  <th>Description
 * <tr><td> 2020-9-10  <td>V0.0      <td>ZJY     <td>initial
 * <tr><td> 2021-1-9   <td>V0.1      <td>ZJY     <td>modify
 * <tr><td> 2021-5-27  <td>V0.2      <td>LQ      <td>modify
 * <tr><td> 2023-09-07 <td>V0.1      <td>LHY     <td>modify
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CSP_CNTA_H
#define _CSP_CNTA_H

/* Includes ------------------------------------------------------------------*/
#include "soc.h"

/// \struct csp_cnta_t
/// \brief cnta reg description   
typedef struct
{
	__IOM uint32_t      CADATAH;        //0x0000 DATA High Register        
    __IOM uint32_t      CADATAL;        //0x0004 DATA Low Register        
    __IOM uint32_t      CACON;          //0x0008 Control Register         
    __IOM uint32_t      INTMASK;        //0x000C Interrupt Mask CR        
} csp_cnta_t ;  

/*****************************************************************************
************************** Cnta Function defined *****************************
******************************************************************************/
#define	CNTA_RESET_VALUE	(0x00000000)

/******************************************************************************
* CADATAH : Cnta Output High Level Length Register 
******************************************************************************/
#define	CNTA_CADATAH_POS	(0)
#define	CNTA_CADATAH_MSK	(0xFFFFul << CNTA_CADATAH_POS)

/******************************************************************************
* CADATAL : Cnta Output Low Level Length Register 
******************************************************************************/
#define	CNTA_CADATAL_POS	(0)
#define	CNTA_CADATAL_MSK	(0xFFFFul << CNTA_CADATAL_POS)

/******************************************************************************
* CACON : Cnta Control Register 
******************************************************************************/
#define	CNTA_OSP_POS		(0)		
#define	CNTA_OSP_MSK		(0x01ul << CNTA_OSP_POS)
typedef enum
{
	CNTA_OSP_LOW		= 0,
	CNTA_OSP_HIGH        
}cnta_osp_e;

#define	CNTA_MODE_POS		(1)		
#define	CNTA_MODE_MSK		(0x01ul << CNTA_MODE_POS)
typedef enum
{
	CNTA_RUN_ONCE		= 0, //one shot
	CNTA_RUN_CONT            //continuous
}cnta_runmode_e; 

#define	CNTA_START_POS		(2)
#define	CNTA_START_MSK		(0x01ul << CNTA_START_POS)
#define	CNTA_START			(0x01ul)

#define	CNTA_STOP_POS		(3)
#define	CNTA_STOP_MSK		(0x01ul << CNTA_STOP_POS)
#define	CNTA_STOP			(0x01ul)

#define	CNTA_CKDIV_POS		(4)		
#define	CNTA_CKDIV_MSK		(0x03ul << CNTA_CKDIV_POS)
typedef enum
{
	CNTA_CK_DIV1		= 0,
	CNTA_CK_DIV2, 
	CNTA_CK_DIV4,  
	CNTA_CK_DIV8  
}cnta_ckdiv_e;

#define	CNTA_SW_STROBE_POS	(16)		
#define	CNTA_SW_STROBE_MSK	(0x01ul << CNTA_SW_STROBE_POS)
#define	CNTA_SW_STROBE		(0x01ul)

#define	CNTA_HW_STROBE_POS	(17)		
#define	CNTA_HW_STROBE_MSK	(0x03ul << CNTA_HW_STROBE_POS)
typedef enum
{
	CNTA_HW_STROBE_MATCH = 1, 
	CNTA_HW_STROBE_PEND      
}cnta_hwstrobe_e;

#define	CNTA_MATCH_REM_POS	(19)		
#define	CNTA_MATCH_REM_MSK	(0x03ul << CNTA_MATCH_REM_POS)
typedef enum
{
	CNTA_MATCHREM_OFF	= 0,
	CNTA_MATCHREM_CLR, 
	CNTA_MATCHREM_SET       
}cnta_matchrem_e;

#define	CNTA_PEND_REM_POS	(21)		
#define CNTA_PEND_REM_MSK	(0x03ul << CNTA_PEND_REM_POS)
typedef enum
{
	CNTA_PENDREM_OFF	= 0,
	CNTA_PENDREM_CLR, 
	CNTA_PENDREM_SET      
}cnta_pendrem_e;

#define	CNTA_REMSTAT_POS	(23)		
#define	CNTA_REMSTAT_MSK	(0x01ul << CNTA_REMSTAT_POS)
typedef enum
{
	CNTA_REMSTAT_LOW	= 0,
	CNTA_REMSTAT_HIGH,     
}cnta_remstat_e;

#define	CNTA_ENVELOPE_POS	(24)		
#define	CNTA_ENVELOPE_MSK	(0x01ul << CNTA_ENVELOPE_POS)
typedef enum
{
	CNTA_CARRIER_OUT	= 0,
	CNTA_ENVELOPE_OUT,     
}cnta_envelope_e;


#define	CNTA_CARRIER_POS	(25)		
#define	CNTA_CARRIER_MSK	(0x01ul << CNTA_CARRIER_POS)
typedef enum
{
	CNTA_CARRIER_DIS	= 0,
	CNTA_CARRIER_EN 
}cnta_carrier_e;

/******************************************************************************
* INTMASK : Interrupt Mask 
******************************************************************************/
typedef enum
{
	CNTA_NONE_INT		= (0x00ul << 0),
	CNTA_PENDH_INT		= (0x01ul << 0),
	CNTA_PENDL_INT		= (0x01ul << 1),
	CNTA_ALL_INT		= (0x03ul << 0),
}cnta_int_e;

/******************************************************************************
********************* CNTA inline Functions Declaration ***********************
******************************************************************************/	
//datah																		
static inline void csp_cnta_set_datah(csp_cnta_t *ptCntaBase,uint16_t hwData)
{
	ptCntaBase->CADATAH = hwData;
}

static inline uint16_t csp_cnta_get_datah(csp_cnta_t *ptCntaBase)
{
	return (uint16_t)(ptCntaBase->CADATAH & CNTA_CADATAH_MSK);
}

//datal
static inline void csp_cnta_set_datal(csp_cnta_t *ptCntaBase,uint16_t hwData)
{
	ptCntaBase->CADATAL = hwData;
}

static inline uint16_t csp_cnta_get_datal(csp_cnta_t *ptCntaBase)
{
	return (uint16_t)(ptCntaBase->CADATAL & CNTA_CADATAL_MSK);
}

//ckdiv
static inline uint8_t csp_cnta_get_ckdiv(csp_cnta_t *ptCntaBase)
{
	return (uint8_t)((ptCntaBase->CACON & CNTA_CKDIV_MSK) >> CNTA_CKDIV_POS);
}

static inline void csp_cnta_set_ckdiv(csp_cnta_t *ptCntaBase,cnta_ckdiv_e eClkDiv, cnta_runmode_e eMode)
{
	ptCntaBase->CACON = (ptCntaBase->CACON & ~(CNTA_CKDIV_MSK | CNTA_MODE_MSK));
	ptCntaBase->CACON |= (eClkDiv << CNTA_CKDIV_POS) | (eMode << CNTA_MODE_POS);
}

//start/stop
static inline void csp_cnta_start(csp_cnta_t *ptCntaBase)
{
	ptCntaBase->CACON = (ptCntaBase->CACON & 0xfffffff3) | (CNTA_START << CNTA_START_POS);
}
static inline void csp_cnta_stop(csp_cnta_t *ptCntaBase)
{
	ptCntaBase->CACON = (ptCntaBase->CACON & 0xfffffff7) | (CNTA_STOP << CNTA_STOP_POS);
}

//software reset
static inline void csp_cnta_sw_rst(csp_cnta_t *ptCntaBase)
{
	ptCntaBase->CADATAH = CNTA_RESET_VALUE;             //default init valu
    ptCntaBase->CADATAL = CNTA_RESET_VALUE;				//default init valu
    ptCntaBase->CACON   = CNTA_RESET_VALUE;				//default init valu
    ptCntaBase->INTMASK = CNTA_RESET_VALUE;				//default init valu
}

//int enable/disable
static inline void csp_cnta_int_enable(csp_cnta_t *ptCntaBase, cnta_int_e eCntaInt)
{
	ptCntaBase->INTMASK |= eCntaInt; 
}

static inline void csp_cnta_int_disable(csp_cnta_t *ptCntaBase, cnta_int_e eCntaInt)
{
	ptCntaBase->INTMASK  &= ~eCntaInt; 
}

//set carrier
static inline void csp_cnta_set_carrier(csp_cnta_t *ptCntaBase, cnta_carrier_e eCarCtrl, cnta_envelope_e eEnvelope, 
						cnta_remstat_e eRemsta, cnta_osp_e eOsp)
{
	ptCntaBase->CACON = (ptCntaBase->CACON & ~(CNTA_CARRIER_MSK | CNTA_ENVELOPE_MSK | CNTA_REMSTAT_MSK | CNTA_OSP_MSK));
	ptCntaBase->CACON |= (eCarCtrl << CNTA_CARRIER_POS) | (eEnvelope << CNTA_ENVELOPE_POS) | (eRemsta << CNTA_REMSTAT_POS) | (eOsp << CNTA_OSP_POS);
}

//software update
static inline void csp_cnta_soft_update(csp_cnta_t *ptCntaBase)
{
	ptCntaBase->CACON |= (CNTA_SW_STROBE << CNTA_SW_STROBE_POS);	
}

//sync
static inline void csp_cnta_set_sync(csp_cnta_t *ptCntaBase, cnta_pendrem_e eTcPend, cnta_matchrem_e eTcMatch, cnta_hwstrobe_e eHwstrobe)
{
	ptCntaBase->CACON = (ptCntaBase->CACON & ~(CNTA_PEND_REM_MSK | CNTA_MATCH_REM_MSK | CNTA_HW_STROBE_MSK));
	ptCntaBase->CACON |= (eTcPend << CNTA_PEND_REM_POS) | (eTcMatch << CNTA_MATCH_REM_POS) | (eHwstrobe << CNTA_HW_STROBE_POS);
}																			
																			
#endif
