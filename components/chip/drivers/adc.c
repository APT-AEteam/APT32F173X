/***********************************************************************//** 
 * \file  adc.c
 * \brief  csi adc driver
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-12 <td>V0.0  <td>ZJY   <td>initial
 * <tr><td> 2021-1-8  <td>V0.1  <td>WNN   <td>modify
 * <tr><td> 2023-9-19 <td>V0.2  <td>XS    <td>fix bug,code normalization
 * </table>
 * *********************************************************************
*/
#include "drv/adc.h"
/* Private macro-----------------------------------------------------------*/
#define	ADC_SAMP_TIMEOUT		0xFFFF
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/
csi_adc_ctrl_t g_tAdcCtrl[ADC_IDX];

/** \brief get adc number 
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \return adc number 0/1
 */ 
static uint8_t apt_get_adc_idx(csp_adc_t *ptAdcBase)
{
	switch((uint32_t)ptAdcBase)
	{
		case APB_ADC0_BASE:		//adc0
			return 0;		
		case APB_ADC1_BASE:		//adc1
			return 1;
		default:
			return 0xff;		//error
	}
}
/** \brief  register adc interrupt callback function
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] callback: adc interrupt handle function
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_adc_register_callback(csp_adc_t *ptAdcBase, void  *callback)
{
	uint8_t byIdx = apt_get_adc_idx(ptAdcBase);
	if(byIdx == 0xff)
		return CSI_ERROR;
		
	g_tAdcCtrl[byIdx].callback = callback;
	
	return CSI_OK;
}
/** \brief adc interrupt handler function
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] byIdx: acd idx(0/1)
 *  \return none
 */ 
void csi_adc_irqhandler(csp_adc_t *ptAdcBase, uint8_t byIdx)
{
	adc_int_e eIsr = csp_adc_get_isr(ptAdcBase);
	
	if(g_tAdcCtrl[byIdx].callback)
			g_tAdcCtrl[byIdx].callback(ptAdcBase, eIsr);
			
	csp_adc_clr_sr(ptAdcBase, (adc_sr_e)eIsr);
}
/** \brief initialize adc data structure
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] ptAdcCfg: pointer of adc parameter config structure
 * 				- byClkDiv: adc clkdiv, adc clk = PCLK/(2*byClkDiv)
 * 				- eClksel: adc clk select, \ref csi_adc_clksel_e
 *  			- bySampHold: adc sample hold period,unit: us
 *  			- bySampHold: adc sample hold period,unit: us 
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_adc_init(csp_adc_t *ptAdcBase, csi_adc_config_t *ptAdcCfg)
{
	csi_error_t ret = CSI_OK;
	
	csi_clk_enable((uint32_t *)ptAdcBase);				//adc peripheral clk enable
	csp_adc_clk_enable(ptAdcBase);						//adc clk enable
	csp_adc_sw_rst(ptAdcBase);							//adc mode reset
	csp_adc_clk_enable(ptAdcBase);						//adc clk enable
	csp_adc_set_resolution(ptAdcBase, ADC_12BIT);		//adc 12bit
	csp_adc_enable(ptAdcBase);							//enable adc mode
	
	if(ptAdcCfg->byClkDiv > 62)
		ptAdcCfg->byClkDiv = 62;
	//adc sample hold period
	if(ptAdcCfg->bySampHold < 3)
		ptAdcCfg->bySampHold = 3;
	
	//general configuration 
	csp_adc_set_clkdiv(ptAdcBase, ptAdcCfg->byClkDiv);					//adc clk div, clk = pclk/div
	csp_adc_set_clksrc(ptAdcBase, (adc_clksel_e)ptAdcCfg->eClksel);		//adc clk sel
	csp_adc_set_hold_cycle(ptAdcBase, ptAdcCfg->bySampHold);			//adc sample hold period, sample time = (bySampHold + 16) clk period
	csp_adc_set_runmode(ptAdcBase, (adc_runmode_e)ptAdcCfg->eRunMode);	//adc conversion mode, continuous/one shot/wait
	csp_adc_set_vref(ptAdcBase, (adc_vref_e)ptAdcCfg->eVrefSrc);		//adc vref

	return ret;
}
/** \brief config adc sample sequence
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] ptSeqx: pointer of adc seq parameter config structure
 * 			   	- eInChnl: adc input channel , \ref csi_adc_ch_e
 * 			   	- eCvCnt: continuous repeat sample count , \ref csi_adc_cv_cnt_e
 * 			   	- eAvgCof: average coefficient , \ref csi_adc_avg_cof_e
 * 			   	- eTrgSrc: sync source, \ref csi_adc_syncsrc_e
 *  \param[in] byChNum: channel number of sequence
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_adc_set_seqx(csp_adc_t *ptAdcBase, csi_adc_seq_t *ptSeqx, uint8_t byChNum)
{
	uint8_t i;
	bool bAvgEn;
	
	if(NULL == ptSeqx || byChNum == 0 || byChNum > 16)
		return CSI_ERROR;
		
	for(i = 0; i < byChNum; i++)				//config adc sample sequence
	{
		if(ptSeqx->eCvCnt)						//config continuous repeat cample 
			bAvgEn = ENABLE;
		else
			bAvgEn = DISABLE;
		csp_adc_set_seqx(ptAdcBase, i, (adc_channel_e)ptSeqx->eInChnl, (adc_cnt_e)ptSeqx->eCvCnt, 
		(adc_avg_e)ptSeqx->eAvgCof, bAvgEn, (adc_sync_source_e)ptSeqx->eSyncSrc);
		ptSeqx ++;
	}
	
	csp_adc_set_seq_num(ptAdcBase, byChNum);	//config number of channel  

	return CSI_OK;
}
/** \brief start adc 
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_adc_start(csp_adc_t *ptAdcBase)
{	
	csi_error_t ret = CSI_OK;
	uint32_t wTimeOut = 0xffff;
	
	while(!(ptAdcBase->SR & ADC_READY) && wTimeOut --);		//adc ready ok?
	if(wTimeOut)
		csp_adc_start(ptAdcBase);							//start adc
	else
		ret = CSI_ERROR;
		
	return ret;
}
/** \brief stop adc 
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \return error code \ref csi_error_t
 */  
csi_error_t csi_adc_stop(csp_adc_t *ptAdcBase)
{
	csi_error_t ret = CSI_OK;
	
	if(csp_adc_get_sr(ptAdcBase) & ADC_CTCVS)		//continuous mode
		csp_adc_stop(ptAdcBase);					//stop adc
	else											//one shot mode, can't stop
		ret = CSI_ERROR; 
	
	return ret;
}
/** \brief set adc run mode, continue/one shot/wait
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] eRunMode: conversion mode, continuous/one shot/wait
 *  \return none
 */
void csi_adc_set_runmode(csp_adc_t *ptAdcBase, csi_adc_runmode_e eRunMode)
{
	csp_adc_set_runmode(ptAdcBase, (adc_runmode_e)eRunMode);
}
/** \brief set adc conversion sequence priority
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] byPri: conversion priority
 *  \return none
 */
void csi_adc_set_pri(csp_adc_t *ptAdcBase, uint8_t byPri)
{
	csp_adc_set_pri(ptAdcBase, byPri);
}
/** \brief get adc value of sequence channel
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] byChIdx: channel id number of sequence
 *  \return error code \ref csi_error_t or adc value
 */
int16_t csi_adc_read_channel(csp_adc_t *ptAdcBase, uint8_t byChIdx)
{
	csi_error_t  ret = CSI_OK;
	uint32_t wTimeOut = 0;
	
	do{
		if(csp_adc_get_sr(ptAdcBase) & ADC_SEQ(byChIdx))		//sequence channel sample complete?
		{
			ret = csp_adc_get_data(ptAdcBase, byChIdx);			//get adc channel value
			csp_adc_clr_sr(ptAdcBase, ADC_SEQ(byChIdx));		//clr channel status
			break;
		}
		else
		{
			wTimeOut ++;
			if(wTimeOut > ADC_SAMP_TIMEOUT)
			{
				ret = CSI_TIMEOUT;
				break;
			}
		}
	}while(1);
	
	return ret;
}

/** \brief set adc vref
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] eVrefSrc: source of adc reference voltage
 *  \return none
 */  
void csi_adc_set_vref(csp_adc_t *ptAdcBase, csi_adc_vref_e eVrefSrc)
{
	csp_adc_set_vref(ptAdcBase,(adc_vref_e)eVrefSrc);
}
 /** \brief adc cmp0 config
 * 
 *  \param[in] ptAdcBase: pointer of ADC reg structure.
 *  \param[in] byCmpChnl: cmp0 channel (adc seqx)
 *  \param[in] wCmpData: cmp0 data
 *  \param[in] eDir: direction(High/Low)
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_adc_set_cmp0(csp_adc_t *ptAdcBase, uint8_t byCmpChnl, uint32_t wCmpData, csi_adc_cmp_dir_e eDir)
{
	csp_adc_set_cmp_mode(ptAdcBase, ADC_CMP_ACROSS);
	csp_adc_set_cmp0(ptAdcBase, wCmpData,byCmpChnl);
	
	switch(eDir)
	{
		case ADC_CMP_H:	
			csp_adc_int_enable(ptAdcBase,(adc_int_e)ADC_CMP0H);
			break;
		case ADC_CMP_L:	
			csp_adc_int_enable(ptAdcBase,(adc_int_e)ADC_CMP0L);
			break;
		default:		
			return CSI_ERROR;
	}
	
	return CSI_OK;
}
 /** \brief adc cmp1 config
 * 
 *  \param[in] ptAdcBase: pointer of ADC reg structure.
 *  \param[in] byCmpChnl: cmp1 channel (adc seqx)
 *  \param[in] wCmpData: cmp1 data
 *  \param[in] eDir: direction(High/Low)
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_adc_set_cmp1(csp_adc_t *ptAdcBase, uint8_t byCmpChnl, uint32_t wCmpData, csi_adc_cmp_dir_e eDir)
{
	csp_adc_set_cmp_mode(ptAdcBase, ADC_CMP_ACROSS);
	csp_adc_set_cmp1(ptAdcBase, wCmpData, byCmpChnl);
	
	switch(eDir)
	{
		case ADC_CMP_H:	
			csp_adc_int_enable(ptAdcBase,(adc_int_e)ADC_CMP1H);
			break;
		case ADC_CMP_L:	
			csp_adc_int_enable(ptAdcBase,(adc_int_e)ADC_CMP1L);
			break;
		default:		
			return CSI_ERROR;
	}
	
	return CSI_OK;
}

/** \brief set adc sync 
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] eSyncIn: sync(0~5) of adc input channels
 *  \param[in] eSyncMode: adc sync mode, continuous/once
 *  \param[in] byDelay: adc input delay, delay timer =  (trg_delay+1)*4 PCLK
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_adc_set_sync(csp_adc_t *ptAdcBase, csi_adc_syncin_e eSyncIn, csi_adc_syncmode_e eSyncMode, uint8_t byDelay)
{
	//set sync delay
    if(eSyncIn < ADC_SYNCIN3)		
	{
		ptAdcBase->TDL0 = (ptAdcBase->TDL0 & ~(0xFFul << (eSyncIn * 8))) | byDelay;
	}
	else if(eSyncIn <= ADC_SYNCIN5)
	{
		ptAdcBase->TDL1 = (ptAdcBase->TDL1 & ~(0xFFul << ((eSyncIn - 3)  * 8))) | byDelay;
	}
	else
		return CSI_ERROR;
	
	//set sync source and mode
	csp_adc_sync_enable(ptAdcBase, (adc_sync_in_e)eSyncIn);
	csp_adc_set_sync_mode(ptAdcBase, (adc_sync_in_e)eSyncIn,(adc_ostmd_e)eSyncMode);
	
	return CSI_OK;
}
/** \brief rearm adc sync
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] eSyncIn: adc sync evtrg input channel(0~5)
 *  \return none
 */
void csi_adc_sync_rearm(csp_adc_t *ptAdcBase, csi_adc_syncin_e eSyncIn)
{
	csp_adc_sync_rearm(ptAdcBase, (adc_sync_in_e)eSyncIn);
}
/** \brief enable adc sync
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] eSyncIn: adc sync evtrg input channel(0~5)
 *  \return none
 */
void csi_adc_sync_enable(csp_adc_t *ptAdcBase, csi_adc_syncin_e eSyncIn)
{
	csp_adc_sync_enable(ptAdcBase, (adc_sync_in_e)eSyncIn);
}
/** \brief disable adc sync
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] eSyncIn: adc sync evtrg input channel(0~5)
 *  \return none
 */
void csi_adc_sync_disable(csp_adc_t *ptAdcBase, csi_adc_syncin_e eSyncIn)
{
	csp_adc_sync_disable(ptAdcBase, (adc_sync_in_e)eSyncIn);
}
/** \brief set adc evtrg output
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] byTrgOut: adc evtrg out port (0~1)
 *  \param[in] eTrgSrc: adc evtrg source(0~23) 
 *  \return error code \ref csi_error_t
 */
csi_error_t  csi_adc_set_evtrg(csp_adc_t *ptAdcBase, csi_adc_trgsrc_e eTrgSrc, csi_adc_trgout_e eTrgOut)
{	
	if(eTrgOut>1)
	return CSI_ERROR;
	else
	{
		csp_adc_set_evtrg_src(ptAdcBase,(adc_evtrg_src_e)eTrgSrc ,(adc_evtrg_out_e) eTrgOut);
		csp_adc_evtrg_enable(ptAdcBase ,(adc_evtrg_out_e) eTrgOut);
	}
	return CSI_OK;
}
/** \brief set adc epvs 
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] eTrgOut: adc evtrg out port (0~1)
 *  \param[in] byPeriod: The event triggers the count 
 *  \return error code \ref csi_error_t
 */
void csi_adc_set_evps(csp_adc_t *ptAdcBase, csi_adc_trgout_e eTrgOut, uint8_t byPeriod)
{
	csp_adc_set_trgprd(ptAdcBase,(adc_evtrg_out_e)eTrgOut,byPeriod);
}
/** \brief enable adc INT status
 * 
 *  \param[in] ptAdcBase: ADC handle to operate
 *  \param[in] eIntSrc:  INT
 */
void csi_adc_int_enable(csp_adc_t *ptAdcBase, csi_adc_intsrc_e eIntSrc)
{
	csp_adc_clr_sr(ptAdcBase, (adc_sr_e)eIntSrc);
	csp_adc_int_enable(ptAdcBase, (adc_int_e)eIntSrc);
}
/** \brief disable adc INT status
 * 
 *  \param[in] ptAdcBase: ADC handle to operate
 *  \param[in] eIntSrc:  INT
 */
void csi_adc_int_disable(csp_adc_t *ptAdcBase, csi_adc_intsrc_e eIntSrc)
{
	csp_adc_int_disable(ptAdcBase, (adc_int_e)eIntSrc);
}
/** \brief select adc clock
 * 
 *  \param[in] ptAdcBase: ADC handle to operate
 *  \param[in] eClksel:  ADC clock select
 */
void csi_adc_set_clksrc(csp_adc_t *ptAdcBase,csi_adc_clksel_e eClksel)
{
	csp_adc_set_clksrc(ptAdcBase,(adc_clksel_e)eClksel);
}


