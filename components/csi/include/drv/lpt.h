/***********************************************************************//** 
 * \file  lpt.h
 * \brief header file for lpt driver
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-9-14 <td>V0.0  <td>ZJY   <td>initial
 * <tr><td> 2021-1-8  <td>V0.1  <td>WNN   <td>modify
 * <tr><td> 2021-5-20  <td>V0.2  <td>YYM   <td>modify
 * </table>
 * *********************************************************************
*/

#ifndef _DRV_LPT_H_
#define _DRV_LPT_H_

#include <drv/common.h>
#include "csp.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ERR_LPT_CLK      0xfffff

typedef enum
{     
    LPT_PWM_IDLE_Z      = 0, 
	LPT_PWM_IDLE_LOW		    
}csi_lpt_idlepol_e;

typedef enum
{
	LPT_PWM_START_HIGH = 0,
	LPT_PWM_START_LOW,      
}csi_lpt_startpol_e;

typedef enum
{
    LPT_SYNC_CONT 	= (0x00ul),  	       
	LPT_SYNC_ONCE	= (0x01ul)	
}csi_lpt_syncmode_e;

typedef enum{
	LPT_TRG_SYNCIN0	= 0,	//sync evtrr input0		
}csi_lpt_trgin_e;

typedef enum
{
	LPT_TRGOUT	= 0,  		   
}csi_lpt_trgout_e;

typedef enum
{
    LPT_TRGSRC_DIS 		= (0x00ul ),  	       
	LPT_TRGSRC_ZRO 		= (0x01ul ),
	LPT_TRGSRC_PRD 		= (0x02ul ),
	LPT_TRGSRC_ZRO_PRD 	= (0x03ul ),
	LPT_TRGSRC_CMP		= (0x04ul )
}csi_lpt_trgsrc_e;

typedef enum
{
	LPT_CLK_PCLK_DIV4 	= (0x00ul ),
    LPT_CLK_ISCLK 		= (0x01ul),
	LPT_CLK_IMCLK_DIV4  = (0x02ul),
	LPT_CLK_EMCLK 		= (0x03ul ),
	LPT_CLK_IN_RISE 	= (0x04ul ),
	LPT_CLK_IN_FALL 	= (0x05ul),	
}csi_lpt_clksrc_e;

typedef enum
{
	LPT_INTSRC_NONE   =	(0x00ul << 0), 	//NONE interrupt
	LPT_INTSRC_TRGEV0 = (0x01ul << 0),
	LPT_INTSRC_MATCH  = (0x01ul << 1),
	LPT_INTSRC_PEND	  = (0x01ul << 2),
}csi_lpt_intsrc_e;

typedef enum
{
    UPDATA_IM 			= 0,  	       
	UPDATA_PEND		
}csi_lpt_updata_e;

typedef enum
{
	LPT_INT_TRGEV0 = 0,
	LPT_INT_MATCH,
	LPT_INT_PEND,
}csi_lpt_int_e;

typedef struct
{
	uint8_t            byStartpol;
	uint8_t            byIdlepol;
	uint8_t            byClksrc;
	uint8_t            byCycle;
	uint8_t            byInt;
	uint8_t            byRev1;
	uint8_t            byRev2;	
	uint8_t            byRev3;	
	uint32_t           wFreq;
	
}csi_lpt_pwm_config_t;


/** \brief initialize lpt data structure
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \param[in] eClk: clk source selection
 *  \param[in] wTimeOut: the timeout for bt, unit: ms
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_lpt_timer_init(csp_lpt_t *ptLptBase,csi_lpt_clksrc_e eClk, uint32_t wms);

/**
  \brief       De-initialize LPT Interface. stops operation and releases the software resources used by the interface
  \param[in]   ptLptBase:pointer of lpt register structure
  \return      None
*/
void csi_lpt_uninit(csp_lpt_t *ptLptBase);

/** \brief lpt start
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_lpt_start(csp_lpt_t *ptLptBase);

/**
  \brief       Stop lpt
  \param[in]   ptLptBase:pointer of lpt register structure
  \return      None
*/
void csi_lpt_stop(csp_lpt_t *ptLptBase);

/**
  \brief       Get lpt remaining value
  \param[in]   ptLptBase:pointer of lpt register structure
  \return      lpt    remaining value
*/
uint32_t csi_lpt_get_remaining_value(csp_lpt_t *ptLptBase);

/**
  \brief       Get lpt load value
  \param[in]   ptLptBase:pointer of lpt register structure
  \return      lpt    load value
*/
uint32_t csi_lpt_get_load_value(csp_lpt_t *ptLptBase);

/**
  \brief       get lpt working status
  \param[in]   ptLptBase:pointer of lpt register structure
  \return      true->running, false->stopped
*/
bool csi_lpt_is_running(csp_lpt_t *ptLptBase);

/**
  \brief       Enable lpt power manage
  \param[in]   ptLptBase:pointer of lpt register structure
  \param[in]   eLptInt:irq mode
  \param[in]   bEnable:lpt irq enable or disable
*/
void csi_lpt_int_enable(csp_lpt_t *ptLptBase, csi_lpt_intsrc_e eLptInt,bool bEnable);

/**
  \brief       Enable lpt power manage
  \param[in]   ptLptBase:pointer of lpt register structure
  \return      error code \ref csi_error_t
*/
csi_error_t csi_lpt_enable_pm(csp_lpt_t *ptLptBase);

/**
  \brief       Disable lpt power manage
  \param[in]   ptLptBase:pointer of lpt register structure
*/
void csi_lpt_disable_pm(csp_lpt_t *ptLptBase);

/** \brief lpt pwm init
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \param[in] ptLptPara: pointer of lpt parameter structure
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_lpt_pwm_init(csp_lpt_t *ptLptBase, csi_lpt_pwm_config_t* ptLptPara);


/** \brief lpt work as a timer(sync start)
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \param[in] eClk: lpt clock source selection
 *  \param[in] wms: ms
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_lpt_start_sync(csp_lpt_t *ptLptBase, csi_lpt_clksrc_e eClk, uint32_t wms);

/** \brief start lpt pwm by external sync
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \param[in] ptLptPara: pointer of lpt parameter structure
 *  \return error code \ref csi_error_t
 */

csi_error_t csi_lpt_pwm_start_sync(csp_lpt_t *ptLptBase, csi_lpt_pwm_config_t *ptLptPara); 

/** \brief change lpt duty cycle
 * 
 *  \param ptLptBase:pointer of lpt register structure
 *  \param duty_cycle: lpt clock source selection
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_lpt_change_duty(csp_lpt_t *ptLptBase, uint32_t duty_cycle);

/** 
  \brief updata lpt pwm freq para: (prdr and cmp value)
  \param[in] ptLptBase:pointer of lpt register structure
  \param[in] hwCmp: duty cycle
  \param[in] hwPrdr: period 
  \param[in] mode_updata: updata mode 
  \return none
 */
void csi_lpt_pwm_para_updata(csp_lpt_t *ptLptBase, uint16_t hwCmp, uint16_t hwPrdr, csi_lpt_updata_e mode_updata);

/** \brief lpt sync window config  
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \param bCrossEnable: window cross enable/disable
 *  \param bInvEnable: window invert enable/disable
 *  \param hwOffset: window start point from CNT=ZRO
 *  \param hwWindow: window width
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_lpt_set_sync_window(csp_lpt_t *ptLptBase, bool bCrossEnable, bool bInvEnable, uint16_t hwOffset, uint16_t hwWindow);

/** \brief lpt sync config  
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \param bySync: select sync
 *  \param tSyncMode: LPT_TRG_CONT/LPT_TRG_ONCE
 *  \param bARearmEnable: auto rearm enable/disable
 *  \return csi_error_t
 */
csi_error_t csi_lpt_set_sync(csp_lpt_t *ptLptBase, uint8_t bySync, csi_lpt_syncmode_e tSyncMode, bool bARearmEnable);


/** \brief restart lpt sync 
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \param bySync: sync select
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_lpt_rearm_sync(csp_lpt_t *ptLptBase, uint8_t bySync);

/** \brief lpt evtrg source output config  
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \param[in] lpt_trgsrc: lpt evtrg source(1~4) 
 *  \param[in] trg_prd: event count period 
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_lpt_set_evtrg(csp_lpt_t *ptLptBase, uint8_t  byEvtrg, csi_lpt_trgsrc_e tTrgSrc, uint8_t trg_prd);

/** \brief lpt set frequency 
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \param[in] eClk: lpt clock source selection
 *  \param[in] wHz: frequency
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_lpt_set_fre(csp_lpt_t *ptLptBase, csi_lpt_clksrc_e eClk, uint16_t wHz);

/** \brief LPT software sync enable control
 * 
 *  \param ptLptBase:pointer of lpt register structure
 *  \param bEnable: ENABLE/DISABLE
 *  \return void
 */
void csi_lpt_swsync_enable(csp_lpt_t *ptLptBase, bool bEnable);


/** \brief lpt software generates a trigger event
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \return none
 */
void csi_lpt_soft_evtrg(csp_lpt_t *ptLptBase);

#ifdef __cplusplus
}
#endif

#endif /* _DRV_TIMER_H_ */