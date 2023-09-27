/***********************************************************************//** 
 * \file  wdt.h
 * \brief  head file of window WDT and indepedent WDT 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-24 <td>V0.0 <td>WNN     <td>initial
 * <tr><td> 2021-5-13 <td>V0.0 <td>ZJY     <td>initial
 * <tr><td> 2023-9-14 <td>V0.2  <td>YT     <td>code normalization
 * </table>
 * *********************************************************************
*/

#ifndef _WWDT_H_
#define _WWDT_H_

#include "csp.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \enum     csi_wwdt_intsrc_e
 * \brief    WWDT interrupt source 
 */
typedef enum
{
	WWDT_INTSRC_NONE   =	(0x00ul << 0), 		//NONE interrupt
	WWDT_INTSRC_EVI    =	(0x01ul << 0),		//EVI interrupt
	WWDT_INTSRC_ALL    =    (0x01ul << 0)		//ALL interrupt
}csi_wwdt_intsrc_e;


/// \struct csi_wwdt_ctrl_t
/// \brief  wwdt control handle, not open to users  
typedef struct 
{
    void(*callback)(csp_wwdt_t *ptBtBase);
} csi_wwdt_ctrl_t;

extern csi_wwdt_ctrl_t g_tWwdtCtrl[WWDT_IDX];



/**
  \brief       Initialize wwdt Interface. Initializes the resources needed for the WDT interface 
  \param[in]   eTimeOut    time length of window, unit: ms 
  \return      error code \ref csi_error_t
*/
csi_error_t csi_wwdt_init(uint32_t eTimeOut);

/** 
  \brief 	   open(start) wwdt
  \param[in]   none
  \return 	   none
 */ 
void csi_wwdt_open(void);

/** 
  \brief 	   close(stop) wwdt
  \param[in]   none
  \return 	   error code \ref csi_error_t
 */ 
csi_error_t csi_wwdt_close(void);

/**
  \brief       feed the wwdt
  \param[in]   none
  \return      none
*/
void csi_wwdt_feed(void);

/**
  \brief	   set window time for wwdt
  \param[in]   wTimeOut		the timeout value of window time
  \return 	   error code \ref csi_error_t
*/
csi_error_t csi_wwdt_set_window_time(uint32_t wTimeOut);

/**
  \brief       get the remaining time to timeout
  \param[in]   none
  \return      the remaining time of wwdt, unit: ms
*/
uint32_t csi_wwdt_get_remaining_time(void);
    
/**
  \brief       check if wdt is running
  \param[in]   none
  \return      true->running, false->stopped
*/
bool csi_wwdt_is_running(void);

/** \brief iwdt INT enable
 * 
 *  \param[in] none
 *  \return none
 */
void csi_wwdt_int_enable(void);

/** \brief iwdt INT disable
 * 
 *  \param[in] none
 *  \return none
 */
void csi_wwdt_int_disable(void);

/** \brief enable  WDT when stop in debug mode
 * 
 *  \param[in] ptWwdtBase:pointer of wwdt register structure
 *  \return  none
 */
void csi_wwdt_debug_enable(csp_wwdt_t * ptWwdtBase);

/** \brief disable WDT when stop in debug mode
 * 
 *  \param[in] ptWwdtBase :pointer of wwdt register structure
 *  \return  none
 */
void csi_wwdt_debug_disable(csp_wwdt_t * ptWwdtBase);

/** \brief  register wwdt interrupt callback function
 * 
 *  \param[in] ptWwdtBase: pointer of wwdt register structure
 *  \param[in] callback: wwdt interrupt handle function
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_wwdt_register_callback(csp_wwdt_t * ptWwdtBase, void  *callback);

/** \brief wwdt interrupt handler function
 * 
 *  \param[in] ptWwdtBase: pointer of wwdt register structure
 *  \param[in] byIdx: wwdt idx 0 
 *  \return none
 */ 
void csi_wwdt_irqhandler(csp_wwdt_t * ptWwdtBase, uint8_t byIdx);

/** \brief clear wwdt interrupt 
 * 
 *  \param[in] ptWwdtBase: pointer of wwdt register structure
 *  \param[in] eIntSrc: wwdt interrupt source
 *  \return none
 */ 
void csi_wwdt_clr_isr(csp_wwdt_t * ptWwdtBase);

#ifdef __cplusplus
}
#endif

#endif /* _WWDT_H_ */
