/***********************************************************************//** 
 * \file  wdt.h
 * \brief  head file of window WDT and indepedent WDT 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-24 <td>V0.0 <td>WNN     <td>initial
 * <tr><td> 2021-5-13 <td>V0.0 <td>ZJY     <td>initial
 * </table>
 * *********************************************************************
*/

#ifndef _DRV_WDT_H_
#define _DRV_WDT_H_

#include "csp.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \enum     csi_iwdt_alarm_e
 * \brief    IWDT interrupt alarm timer(timer out) select
 */
typedef enum{
	IWDT_ALARMTO_1_8 	= 0,
	IWDT_ALARMTO_2_8,
	IWDT_ALARMTO_3_8, 
	IWDT_ALARMTO_4_8, 
	IWDT_ALARMTO_5_8, 
	IWDT_ALARMTO_6_8, 
	IWDT_ALARMTO_7_8
}csi_iwdt_alarm_e;

/**
 * \enum     csi_iwdt_tv_e
 * \brief    IWDT time out, unit: ms
 */
typedef enum{
	IWDT_TO_128 	= 0,	//128ms
	IWDT_TO_256,			//256ms
	IWDT_TO_512,			//512ms
	IWDT_TO_1024,			//1024ms 
	IWDT_TO_2048,			//2048ms
	IWDT_TO_3072,			//3072ms 
	IWDT_TO_4096, 			//4096ms 
	IWDT_TO_8192			//8192ms 
}csi_iwdt_to_e;

/**
 * \enum     csi_iwdt_intsrc_e
 * \brief    IWDT interrupt source 
 */
typedef enum
{
	IWDT_INTSRC_NONE   =	(0x00ul << 0), 		//NONE interrupt
	IWDT_INTSRC_ALARM    =	(0x01ul << 8)		//ALARM interrupt
}csi_iwdt_intsrc_e;

/// \struct csi_iwdt_ctrl_t
/// \brief  iwdt control handle, not open to users  
typedef struct 
{
    void(*callback)(csp_syscon_t * ptSysBase);
} csi_iwdt_ctrl_t;

extern csi_iwdt_ctrl_t g_tIwdtCtrl[IWDT_IDX];

/**
  \brief       Initialize iwdt Interface. Initializes the resources needed for the WDT interface 
  \param[in]   eTimeOut    time length of system reset
  \return      error code \ref csi_error_t
*/
csi_error_t csi_iwdt_init(csi_iwdt_to_e eTimeOut);

/** 
  \brief 	   open(start) iwdt
  \param[in]   none
  \return 	   none
 */ 
void csi_iwdt_open(void);

/** 
  \brief 	   close(stop) iwdt
  \param[in]   none
  \return 	   none
 */ 
void csi_iwdt_close(void);

/**
  \brief       feed the iwdt
  \param[in]   none
  \return      none
*/
void csi_iwdt_feed(void);

/**
  \brief       get the remaining time to timeout
  \param[in]   none
  \return      the remaining time of iwdt, unit: ms
*/
uint32_t csi_iwdt_get_remaining_time(void);
    
/**
  \brief       check if wdt is running
  \param[in]   none
  \return      true->running, false->stopped
*/
bool csi_iwdt_is_running(void);

/** \brief iwdt int enable
 * 
 *  \param[in] eAlarmTo: iwdt interrupt alarm timer length(timer out), 1/2/3/4/5/6/7_8
 *  \return none
 */
void csi_iwdt_int_enable(csi_iwdt_alarm_e eAlarmTo);

/** \brief iwdt int disable
 * 
 *  \param[in] eAlarmTo: iwdt interrupt alarm timer length(timer out), 1/2/3/4/5/6/7_8
 *  \return none
 */
void csi_iwdt_int_disable(csi_iwdt_alarm_e eAlarmTo);

/** \brief enable iwdt when stop in debug mode
 * 
 *  \param[in] none 
 *  \return  none
*/
void csi_iwdt_debug_enable(void);

/** \brief disable iwdt when stop in debug mode
 * 
 *  \param[in] none
 *  \return  none
*/
void csi_iwdt_debug_disable();

/** \brief  register iwdt interrupt callback function
 * 
 *  \param[in] ptSysBase: pointer of iwdt register structure
 *  \param[in] callback: iwdt interrupt handle function
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_iwdt_register_callback(csp_syscon_t * ptSysBase, void  *callback);


/** \brief iwdt interrupt handler function
 * 
 *  \param[in] ptWwdtBase: pointer of wwdt register structure
 *  \param[in] byIdx: wwdt idx 0 
 *  \return none
 */ 
void csi_iwdt_irqhandler(csp_syscon_t * ptSysBase, uint8_t byIdx);

#ifdef __cplusplus
}
#endif

#endif /* _DRV_WDT_H_ */
