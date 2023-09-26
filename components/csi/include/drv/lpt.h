/***********************************************************************//** 
 * \file  lpt.h
 * \brief header file for lpt driver
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-9-14 <td>V0.0  <td>ZJY   <td>initial
 * <tr><td> 2021-1-8  <td>V0.1  <td>WNN   <td>modify
 * <tr><td> 2021-5-20  <td>V0.2  <td>YYM   <td>modify
 * <tr><td> 2023-9-14 <td>V0.2  <td>YT     <td>code normalization
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

/**
 * \enum     csi_lpt_idlepol_e
 * \brief    The idle state  when LPT pwm output is stopped
 */
typedef enum
{     
    LPT_PWM_IDLE_Z      = 0, 
	LPT_PWM_IDLE_LOW		    
}csi_lpt_idlepol_e;


/**
 * \enum     csi_lpt_startpol_e
 * \brief    LPT pwm output polarity control 
 */
typedef enum
{
	LPT_PWM_START_HIGH = 0,
	LPT_PWM_START_LOW,      
}csi_lpt_startpol_e;


/**
 * \enum     csi_lpt_syncmode_e
 * \brief    LPT sync mode 
 */
typedef enum
{
    LPT_SYNC_CONT 	= (0x00ul),  	       
	LPT_SYNC_ONCE	= (0x01ul)	
}csi_lpt_syncmode_e;

/**
 * \enum     csi_lpt_syncin_e
 * \brief    LPT sync trigger input 
 */
typedef enum{
	LPT_TRG_SYNCIN0	= 0,	//sync evtrr input0		
}csi_lpt_syncin_e;

/**
 * \enum     csi_lpt_trgout_e
 * \brief    LPT trigger out
 */
typedef enum
{
	LPT_TRGOUT	= 0,  		   
}csi_lpt_trgout_e;


/**
 * \enum     csi_bt_trgsrc_e
 * \brief    LPT event trigger source
 */
typedef enum
{
    LPT_TRGSRC_DIS 		= (0x00ul ),  	       
	LPT_TRGSRC_ZRO 		= (0x01ul ),
	LPT_TRGSRC_PRD 		= (0x02ul ),
	LPT_TRGSRC_ZRO_PRD 	= (0x03ul ),
	LPT_TRGSRC_CMP		= (0x04ul )
}csi_lpt_trgsrc_e;

/**
 * \enum     csi_lpt_clksrc_e
 * \brief    LPT clock source
 */
typedef enum
{
	LPT_CLK_PCLK_DIV4 	= (0x00ul ),
    LPT_CLK_ISCLK 		= (0x01ul),
	LPT_CLK_IMCLK_DIV4  = (0x02ul),
	LPT_CLK_EMCLK 		= (0x03ul ),
	LPT_CLK_ESCLK 		= (0x04ul ),
	LPT_CLK_IN_RISE 	= (0x05ul ),
	LPT_CLK_IN_FALL 	= (0x06ul),	
}csi_lpt_clksrc_e;

/**
 * \enum     csi_lpt_runmode_e
 * \brief    LPT count mode  
 */
typedef enum{
	LPT_CNT_CONT	= 0,			//continuous count mode	
	LPT_CNT_ONCE 					//once count mode	
}csi_lpt_runmode_e;


/// \struct csi_lpt_time_config_t
/// \brief  lpt timer parameter configuration, open to users  
typedef struct {
	uint32_t			wTimeVal;			//TIMER Timing Value (0-636291451 us)
	csi_lpt_runmode_e	eRunMode;			//TIMER RunMode: continuous/once
	csi_lpt_clksrc_e    eClksrc;            //TIMER Clock Source
} csi_lpt_time_config_t;

/// \struct csi_lpt_ctrl_t
/// \brief  lpt control handle, not open to users  
typedef struct 
{
    void(*callback)(csp_lpt_t *ptBtBase, uint8_t byIsr);
} csi_lpt_ctrl_t;

extern csi_lpt_ctrl_t g_tLptCtrl[LPT_IDX];


/**
 * \enum     csi_lpt_intsrc_e
 * \brief    LPT interrupt source 
 */
typedef enum
{
	LPT_INTSRC_NONE   =	(0x00ul << 0), 	//NONE interrupt
	LPT_INTSRC_TRGEV0 = (0x01ul << 0), 	//TRGEV0 interrupt
	LPT_INTSRC_MATCH  = (0x01ul << 1), 	//MATCH interrupt
	LPT_INTSRC_PEND	  = (0x01ul << 2), 	//PEND interrupt
	LPT_INTSRC_ALL  =	(0x07ul << 0)	//ALL interrupt
}csi_lpt_intsrc_e;


/**
 * \enum     csi_lpt_update_e
 * \brief    LPT update mode
 */
typedef enum
{
    UPDATA_IM 			= 0,  	       
	UPDATA_PEND		
}csi_lpt_update_e;

///**
// * \enum     csi_lpt_int_e
// * \brief    LPT interrupt source 
// */
//typedef enum
//{
//	LPT_INTSRC_TRGEV0 = 0,
//	LPT_INTSRC_MATCH,
//	LPT_INTSRC_PEND,
//}csi_lpt_int_e;

/**
 * \enum     csi_lpt_pwm_config_t
 * \brief    LPT pwm parameter configuration 
 */
typedef struct
{
	csi_lpt_startpol_e eStartPol;
	csi_lpt_idlepol_e  eIdlePol;
	csi_lpt_clksrc_e   eClkSrc;
	uint8_t            byDutyCycle;  
	uint8_t            byRev1;
	uint8_t            byRev2;	
	uint8_t            byRev3;	
	uint32_t           wFreq;
	
}csi_lpt_pwm_config_t;


/** \brief initialize lpt data structure
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \param[in] ptLptTimCfg: ptLptTimCfg: pointer of lpt timing parameter config structure
 *             - wTimeVal: timing value, unit: us
 *             - eWkMode: lpt count work mode, \ref csi_lpt_wkmode_e
 *             - eClksrc: Clock Source, \ref csi_lpt_clksrc_e
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_lpt_timer_init(csp_lpt_t *ptLptBase,csi_lpt_time_config_t *ptLptTimCfg);

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
  \brief       LPT interrupt enable control
  \param[in]   ptLptBase:pointer of lpt register structure
  \param[in]   eLptInt:irq mode
*/
void csi_lpt_int_enable(csp_lpt_t *ptLptBase, csi_lpt_intsrc_e eLptInt);

/**
  \brief       LPT interrupt disable control
  \param[in]   ptLptBase:pointer of lpt register structure
  \param[in]   eLptInt:irq mode
*/
void csi_lpt_int_disable(csp_lpt_t *ptLptBase, csi_lpt_intsrc_e eLptInt);

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
  \brief update lpt pwm freq para: (prdr and cmp value)
  \param[in] ptLptBase:pointer of lpt register structure
  \param[in] hwCmp: duty cycle
  \param[in] hwPrdr: period 
  \param[in] mode_update: update mode 
  \return none
 */
void csi_lpt_pwm_para_update(csp_lpt_t *ptLptBase, uint16_t hwCmp, uint16_t hwPrdr, csi_lpt_update_e mode_update);

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
//csi_error_t csi_lpt_set_sync(csp_lpt_t *ptLptBase, uint8_t bySync, csi_lpt_syncmode_e tSyncMode, bool bARearmEnable);
csi_error_t csi_lpt_set_sync(csp_lpt_t *ptLptBase, csi_lpt_syncin_e eSync, csi_lpt_syncmode_e eSyncMode, bool bARearmEnable);

/** \brief restart lpt sync 
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \param bySync: sync select
 *  \return error code \ref csi_error_t
 */
//csi_error_t csi_lpt_rearm_sync(csp_lpt_t *ptLptBase, uint8_t bySync);
csi_error_t csi_lpt_rearm_sync(csp_lpt_t *ptLptBase, csi_lpt_syncin_e eSync);

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

/** \brief lpt software generates a trigger event
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \return none
 */
void csi_lpt_soft_evtrg(csp_lpt_t *ptLptBase);

/** \brief  register lpt interrupt callback function
 * 
 *  \param[in] ptLptBase: pointer of lpt register structure
 *  \param[in] callback: bt interrupt handle function
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_lpt_register_callback(csp_lpt_t *ptLptBase, void  *callback);


/** \brief lpt interrupt handler function
 * 
 *  \param[in] ptLptBase: pointer of bt register structure
 *  \param[in] byIdx: lpt idx(0)
 *  \return none
 */ 
void csi_lpt_irqhandler(csp_lpt_t *ptLptBase, uint8_t byIdx);

/** \brief LPT sync enable control
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \return void
 */
void csi_lpt_sync_enable(csp_lpt_t *ptLptBase);

/** \brief LPT sync enable control
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \return void
 */
void csi_lpt_sync_disable(csp_lpt_t *ptLptBase);

/** \brief LPT trigger enable control
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \return void
 */
void csi_lpt_trg_enable(csp_lpt_t *ptLptBase);

/** \brief LPT trigger enable control
 * 
 *  \param[in] ptLptBase:pointer of lpt register structure
 *  \return void
 */
void csi_lpt_trg_disable(csp_lpt_t *ptLptBase);

/** \brief clear lpt interrupt 
 * 
 *  \param[in] ptLptBase: pointer of lpt register structure
 *  \param[in] eIntSrc: lpt interrupt source
 *  \return none
 */ 
void csi_lpt_clr_isr(csp_lpt_t *ptLptBase, csi_lpt_intsrc_e eIntSrc);

/** \brief disable lpt in debug mode
 * 
 *  \param[in]  ptLptBase      pointer of gpta register structure
 *  \return none
 */
void csi_lpt_debug_disable(csp_lpt_t *ptLptBase);

/** \brief enable lpt in debug mode
 * 
 *  \param[in]  ptLptBase      pointer of gpta register structure
 *  \return none
 */
void csi_lpt_debug_enable(csp_lpt_t *ptLptBase);

#ifdef __cplusplus
}
#endif

#endif /* _DRV_TIMER_H_ */