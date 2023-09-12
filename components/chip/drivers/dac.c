/***********************************************************************//** 
 * \file  dac.c
 * \brief  csi dac driver
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-12 <td>V0.0  <td>ZJY   <td>initial
 * <tr><td> 2021-1-8  <td>V0.1  <td>WNN   <td>modify
 * <tr><td> 2023-9-11 <td>V0.2  <td>xs    <td>modify
 * </table>
 * *********************************************************************
*/
#include "drv/dac.h"
/* Private macro------------------------------------------------------*/
/* externs function---------------------------------------------------*/
/* global variablesr--------------------------------------------------*/
csi_dac_ctrl_t g_tDacCtrl[DAC_IDX];

/* Private variablesr-------------------------------------------------*/

/** \brief get dac number 
 * 
 *  \param[in] ptDacBase: pointer of dac register structure
 *  \return dac number 0/1
 */ 
static uint8_t apt_get_dac_idx(csp_dac_t *ptDacBase)
{
	switch((uint32_t)ptDacBase)
	{
		case AHB_DAC_BASE:		//DAC0
			return 0;		
		default:
			return 0xff;		//error
	}
}
/** \brief  register dac interrupt callback function
 * 
 *  \param[in] ptDacBase: pointer of dac register structure
 *  \param[in] callback: dac interrupt handle function
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_dac_register_callback(csp_dac_t *ptDacBase, void  *callback)
{
	uint8_t byIdx = apt_get_dac_idx(ptDacBase);
	if(byIdx == 0xff)
		return CSI_ERROR;
		
	g_tDacCtrl[byIdx].callback = callback;
	
	return CSI_OK;
}
/** \brief dac interrupt handler function
 * 
 *  \param[in] ptDacBase: pointer of dac register structure
 *  \param[in] byIdx: dac idx(0)
 *  \return none
 */ 
void csi_dac_irqhandler(csp_dac_t *ptDacBase, uint8_t byIdx)
{
	uint8_t byIsr = csp_dac_get_isr(ptDacBase);
	if(g_tDacCtrl[byIdx].callback)
		g_tDacCtrl[byIdx].callback(ptDacBase, byIsr);
			
	csp_dac_clr_isr(ptDacBase, byIsr);
}

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
void csi_dac_init(csp_dac_t *ptDacBase, csi_dac_config_t *ptDacCfg)
{	
	csp_dac_clr_dac(ptDacBase);//clear data
	csp_dac_set_clk_div(ptDacBase,ptDacCfg->byClkDiv);//set clk div
	csp_dac_refsel_enable(ptDacBase,ptDacCfg->bRefsel);//reference enable
	csp_dac_set_datar(ptDacBase,ptDacCfg->hwDatarset);//set voltage data
	csp_dac_buff_enable(DAC0,ptDacCfg->bBufen);//buffer enable
}

/**
*  \brief       enable dac power  
*  \param[in]   ptDacBase:	pointer of dac register structure
v  \return      none
*/
void csi_dac_enable(csp_dac_t *ptDacBase)
{
	csp_dac_powerdown_enable(ptDacBase, ENABLE);//enable DAC power
}


/**
*  \brief       disable dac power  
*  \param[in]   ptDacBase:	pointer of dac register structure
*  \return      none
*/
void csi_dac_disable(csp_dac_t *ptDacBase)
{
	csp_dac_powerdown_enable(ptDacBase, DISABLE);//disable DAC power
}

/**
*  \brief       dac interrupt enable 
*  \param[in]   ptDacBase:	pointer of dac register structure
*  \param[in]   eIntsrc: 	dac interrupt source \ref  csi_dac_intsrc_e
*  \return      none
*/
void csi_dac_int_enable(csp_dac_t *ptDacBase, csi_dac_intsrc_e eIntsrc)
{
	csp_dac_int_enable(ptDacBase,(dac_int_e)eIntsrc);
}
/**
*  \brief       dac interrupt disable 
*  \param[in]   ptDacBase:	pointer of dac register structure
*  \param[in]   eIntsrc: 	dac interrupt source \ref  csi_dac_intsrc_e
*  \return      none
*/
void csi_dac_int_disable(csp_dac_t *ptDacBase, csi_dac_intsrc_e eIntsrc)
{
	csp_dac_int_disable(ptDacBase,(dac_int_e)eIntsrc);
}
/**
*  \brief       clear dac interrupt
*  \param[in]   ptDacBase:	pointer of dac register structure
*  \param[in]   eIntsrc: 	dac interrupt source \ref  csi_dac_intsrc_e
*  \return      none
*/
void csi_dac_clr_isr(csp_dac_t *ptDacBase, csi_dac_intsrc_e eIntsrc)
{
	csp_dac_clr_isr(ptDacBase,(dac_int_e)eIntsrc);
}
/**
*  \brief       dac sync enable 
*  \param[in]   ptDacBase:	pointer of dac register structure
*  \param[in]   eSyncin: dac sync evtrg input channel, \ref csi_bt_syncin_e
*  \return      none
*/
void csi_dac_sync_enable(csp_dac_t *ptDacBase, csi_dac_syncin_e eSyncin)
{
	csp_dac_sync_enable(ptDacBase,(dac_sync_in_e)eSyncin);
}
/**
*  \brief       dac sync disable 
*  \param[in]   ptDacBase:	pointer of dac register structure
*  \param[in]   eSyncin: dac sync evtrg input channel, \ref csi_bt_syncin_e
*  \return      none
*/
void csi_dac_sync_disable(csp_dac_t *ptDacBase, csi_dac_syncin_e eSyncin)
{
	csp_dac_sync_disable(ptDacBase,(dac_sync_in_e)eSyncin);
}
/**
*  \brief       dac step value set 
*  \param[in]   ptDacBase:	pointer of dac register structure
*  \param[in]   hwStep:	data of dac sync step
*  \return      none
*/
void csi_dac_set_step(csp_dac_t *ptDacBase, uint16_t hwStep)
{
	csp_dac_set_step(ptDacBase, hwStep);
}

/**
*  \brief       dac start 
*  \param[in]   ptDacBase:	pointer of dac register structure
*  \return      none
*/
void csi_dac_start(csp_dac_t *ptDacBase)
{
	csp_dac_start(ptDacBase);
}