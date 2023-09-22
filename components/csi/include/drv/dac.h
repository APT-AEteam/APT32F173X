/***********************************************************************//** 
 * \file  dac.h
 * \brief dac csi driver head file
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  		<th>Version  <th>Author  <th>Description
 * <tr><td> 2023-02-28  <td>V0.0  	 <th>XS    	 <td>initial
 * <tr><td> 2023-9-11   <td>V0.1  	 <th>xs      <td>fix bug,code normalization
 * </table>
 * *********************************************************************
*/
#ifndef _DRV_DAC_H_
#define _DRV_DAC_H_

#include "csp.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
 * \enum     csi_dac_config_t
 * \brief    DAC init config 
 */
typedef struct {
	uint8_t				byClkDiv;		//dac clkdiv, dac clk = FHCLK/(byClkDiv+1)
	uint16_t			hwDatarset;		//dac output voltage data set
	bool				bRefSel;		//dac reference enable
	bool				bBufen;			//dac buf enable
} csi_dac_config_t;
/**
 * \enum     csi_dac_intsrc_e
 * \brief    DAC interrupt source 
 */
typedef enum{
	DAC_INTSRC_EOC			    = (0x01uL << 0),
	DAC_INTSRC_WRERR			= (0x01uL << 1),
	DAC_INTSRC_SYNCERR			= (0x01uL << 2),	
	DAC_INTSRC_ALL				= (0x07uL << 0),	
}csi_dac_intsrc_e;
/**
 * \enum     csi_dac_syncin_e
 * \brief    DAC sync trigger input 
 */
typedef enum{
	DAC_SYNCIN0			= (0x01uL << 0),
	DAC_SYNCIN1			= (0x01uL << 1),
	DAC_SYNCIN2			= (0x01uL << 2),	
}csi_dac_syncin_e;

/// \struct csi_dac_ctrl_t
/// \brief  dac control handle, not open to users  
typedef struct 
{
    void(*callback)(csp_dac_t *ptDacBase, uint8_t byIsr);
} csi_dac_ctrl_t;

extern csi_dac_ctrl_t g_tDacCtrl[DAC_IDX];

/** \brief  register dac interrupt callback function
 * 
 *  \param[in] ptDacBase: pointer of dac register structure
 *  \param[in] callback: dac interrupt handle function
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_dac_register_callback(csp_dac_t *ptDacBase, void  *callback);

/** \brief dac interrupt handler function
 * 
 *  \param[in] ptDacBase: pointer of dac register structure
 *  \param[in] byIdx: dac idx(0)
 *  \return none
 */ 
void csi_dac_irqhandler(csp_dac_t *ptDacBase, uint8_t byIdx);

/** \brief initialize dac data structure
 * 
 *  \param[in] ptDacBase: pointer of dac register structure
 *  \param[in] ptDacCfg: pointer of dac parameter config structure
 *	           - byClkDiv : dac clkdiv, dac clk = FHCLK/(byClkDiv+1)
 *	           - byRefsel : dac reference enable
 *	           - byDatarset : dac output voltage data set
 *	           - byBufsel : dac buf enable, \ref csi_dac_config_t
 *  \return error code \ref csi_error_t
 */ 
void csi_dac_init(csp_dac_t *ptDacBase, csi_dac_config_t *ptDacCfg);

/**
*  \brief       enable dac power  
*  \param[in]   ptDacBase:	pointer of dac register structure
v  \return      none
*/
void csi_dac_enable(csp_dac_t *ptDacBase);

/**
*  \brief       disable dac power  
*  \param[in]   ptDacBase:	pointer of dac register structure
*  \return      none
*/
void csi_dac_disable(csp_dac_t *ptDacBase);

/**
*  \brief       dac interrupt enable 
*  \param[in]   ptDacBase:	pointer of dac register structure
*  \param[in]   eIntsrc: 	dac interrupt source \ref  csi_dac_intsrc_e
*  \return      none
*/
void csi_dac_int_enable(csp_dac_t *ptDacBase, csi_dac_intsrc_e eIntsrc);

/**
*  \brief       dac interrupt disable 
*  \param[in]   ptDacBase:	pointer of dac register structure
*  \param[in]   eIntsrc: 	dac interrupt source \ref  csi_dac_intsrc_e
*  \return      none
*/
void csi_dac_int_disable(csp_dac_t *ptDacBase, csi_dac_intsrc_e eIntsrc);

/**
*  \brief       clear dac interrupt
*  \param[in]   ptDacBase:	pointer of dac register structure
*  \param[in]   eIntsrc: 	dac interrupt source \ref  csi_dac_intsrc_e
*  \return      none
*/
void csi_dac_clr_isr(csp_dac_t *ptDacBase, csi_dac_intsrc_e eIntsrc);

/**
*  \brief       dac sync enable 
*  \param[in]   ptDacBase:	pointer of dac register structure
*  \param[in]   eSyncin: dac sync evtrg input channel, \ref csi_bt_syncin_e
*  \return      none
*/
void csi_dac_sync_enable(csp_dac_t *ptDacBase, csi_dac_syncin_e eSyncin);

/**
*  \brief       dac sync disable 
*  \param[in]   ptDacBase:	pointer of dac register structure
*  \param[in]   eSyncin: dac sync evtrg input channel, \ref csi_bt_syncin_e
*  \return      none
*/
void csi_dac_sync_disable(csp_dac_t *ptDacBase, csi_dac_syncin_e eSyncin);

/**
*  \brief       dac step value set 
*  \param[in]   ptDacBase:	pointer of dac register structure
*  \param[in]   hwStep:	data of dac sync step
*  \return      none
*/
void csi_dac_set_step(csp_dac_t *ptDacBase, uint16_t hwStep);

/**
*  \brief       dac start 
*  \param[in]   ptDacBase:	pointer of dac register structure
*  \return      none
*/
void csi_dac_start(csp_dac_t *ptDacBase);

#ifdef __cplusplus
}
#endif

#endif 