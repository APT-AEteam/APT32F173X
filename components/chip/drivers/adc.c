/***********************************************************************//** 
 * \file  adc.c
 * \brief  csi adc driver
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-12 <td>V0.0  <td>ZJY   <td>initial
 * <tr><td> 2021-1-8  <td>V0.1  <td>WNN   <td>modify
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
	uint8_t byIsr = csp_adc_get_isr(ptAdcBase);
	
	if(g_tAdcCtrl[byIdx].callback)
			g_tAdcCtrl[byIdx].callback(ptAdcBase, byIsr);
			
	csp_adc_clr_sr(ptAdcBase, byIsr);
}
/** \brief initialize adc data structure
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] ptAdcCfg: pointer of adc parameter config structure
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_adc_init(csp_adc_t *ptAdcBase, csi_adc_config_t *ptAdcCfg)
{
	csi_error_t ret = CSI_OK;
	
	csi_clk_enable((uint32_t *)ptAdcBase);				//adc peripheral clk enable
	csp_adc_clk_en(ptAdcBase);							//adc clk enable
	csp_adc_soft_rst(ptAdcBase);						//adc mode reset
	csp_adc_clk_en(ptAdcBase);							//adc clk enable
	csp_adc_set_resolution(ptAdcBase, ADC12_12BIT);		//adc 12bit
	csp_adc_en(ptAdcBase);								//enable adc mode
	
	if(ptAdcCfg->byClkDiv > 62)
		ptAdcCfg->byClkDiv = 62;
	//adc sample hold period
	if(ptAdcCfg->bySampHold < 3)
		ptAdcCfg->bySampHold = 3;
	
	//general configuration 
	csp_adc_set_clk_div(ptAdcBase, ptAdcCfg->byClkDiv);		//adc clk div, clk = pclk/div
	csp_adc_clk_sel(ptAdcBase,ptAdcCfg->byClksel);		//adc clk sel
	csp_adc_hold_cycle(ptAdcBase, ptAdcCfg->bySampHold);	//adc sample hold period, sample time = (bySampHold + 16) clk period
	csp_adc_set_conv_mode(ptAdcBase, ptAdcCfg->byConvMode);	//adc conversion mode, continuous/one shot
	csp_adc_set_vref(ptAdcBase,ptAdcCfg->byVrefSrc);		//adc vref
	
	//adc interrupt
	if(ptAdcCfg->wInt)
	{
		csp_adc_int_enable(ptAdcBase, ptAdcCfg->wInt, ENABLE);	//enable adc interrupt
	}	
	return ret;
}
/** \brief config adc sample sequence
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] ptSeqx: pointer of adc seq parameter config structure
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
		if(ptSeqx->byRepCnt)					//config continuous repeat cample 
			bAvgEn = ENABLE;
		else
			bAvgEn = DISABLE;
		csp_adc_set_seqx(ptAdcBase, i, ptSeqx->byInChnl, ptSeqx->byRepCnt, ptSeqx->byAvgCof, bAvgEn, ptSeqx->byTrgSrc);
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
	
	while(!(ptAdcBase->SR & ADC12_READY) && wTimeOut --);	//adc ready ok?
	if(wTimeOut)
		csp_adc_start(ptAdcBase);									//start adc
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
	
	if(csp_adc_get_sr(ptAdcBase) & ADC12_CTCVS)		//continuous mode
		csp_adc_stop(ptAdcBase);					//stop adc
	else											//one shot mode, can't stop
		ret = CSI_ERROR; 
	
	return ret;
}
/** \brief set adc conversion mode, continue/one shot
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] eConvMode: conversion mode, continuous/one shot/wait
 *  \return none
 */
void csi_adc_conv_mode(csp_adc_t *ptAdcBase, csi_adc_conv_mode_e eConvMode)
{
	csp_adc_set_conv_mode(ptAdcBase, (adc_conv_mode_e)eConvMode);
}
/** \brief set adc conversion sequence priority
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] byPri: conversion priority
 *  \return none
 */
void csi_adc_conv_pri(csp_adc_t *ptAdcBase, uint8_t byPri)
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
		if(csp_adc_get_sr(ptAdcBase) & ADC12_SEQ(byChIdx))		//sequence channel sample complete?
		{
			ret = csp_adc_get_data(ptAdcBase, byChIdx);			//get adc channel value
			csp_adc_clr_sr(ptAdcBase, ADC12_SEQ(byChIdx));		//clr channel status
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
/** \brief set adc clk,;adc clk = pclk/div
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] byDiv: Frequency division num; byDiv: 1~62
 *  \return adc clk
 */ 
uint32_t csi_adc_freq_div(csp_adc_t *ptAdcBase, uint8_t byDiv)
{
	if(0 == byDiv)
		byDiv = 1;
	else if(byDiv > 62)
		byDiv = 62;
		
	csp_adc_set_clk_div(ptAdcBase,byDiv);	
	return  csi_get_pclk_freq()/byDiv;		
}
/** \brief get adc clk 
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \return adc clk
 */
uint32_t csi_adc_get_freq(csp_adc_t *ptAdcBase)
{
    uint8_t byDiv = csp_adc_get_clk_div(ptAdcBase);
	
	if(0 == byDiv)
		return  csi_get_pclk_freq();
	else
		return  csi_get_pclk_freq()/(byDiv << 1);
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
	csp_adc_set_cmp_mode(ptAdcBase, ADC12_CMP_ACROSS);
	csp_adc_set_cmp0(ptAdcBase, wCmpData,byCmpChnl) ;
	
	switch(eDir)
	{
		case ADC_CMP_H:	
			csp_adc_int_enable(ptAdcBase,(adc_int_e)ADC12_CMP0H , ENABLE);
			break;
		case ADC_CMP_L:	
			csp_adc_int_enable(ptAdcBase,(adc_int_e)ADC12_CMP0L , ENABLE);
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
	csp_adc_set_cmp_mode(ptAdcBase, ADC12_CMP_ACROSS);
	csp_adc_set_cmp1(ptAdcBase, wCmpData, byCmpChnl);
	
	switch(eDir)
	{
		case ADC_CMP_H:	
			csp_adc_int_enable(ptAdcBase,(adc_int_e)ADC12_CMP1H , ENABLE);
			break;
		case ADC_CMP_L:	
			csp_adc_int_enable(ptAdcBase,(adc_int_e)ADC12_CMP1L , ENABLE);
			break;
		default:		
			return CSI_ERROR;
	}
	
	return CSI_OK;
}

/** \brief set adc sync 
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] eTrgIn: sync(0~5) of adc input channels
 *  \param[in] eTrgMode: adc sync mode, continuous/once
 *  \param[in] byDelay: adc input delay, delay timer =  (trg_delay+1)*4 PCLK
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_adc_set_sync(csp_adc_t *ptAdcBase, csi_adc_trgin_e eTrgIn, csi_adc_trgmode_e eTrgMode, uint8_t byDelay)
{
	//set sync delay
    if(eTrgIn < ADC_TRG_SYNCEN3)		
	{
		ptAdcBase->TDL0 = (ptAdcBase->TDL0 & ~(0xFFul << (eTrgIn * 8))) | byDelay;
	}
	else if(eTrgIn <= ADC_TRG_SYNCEN5)
	{
		ptAdcBase->TDL1 = (ptAdcBase->TDL1 & ~(0xFFul << ((eTrgIn - 3)  * 8))) | byDelay;
	}
	else
		return CSI_ERROR;
	
	//set sync trgin and trgmode
	ptAdcBase->SYNCR = ptAdcBase->SYNCR & (~(0x01 << (8 + eTrgIn)));
	ptAdcBase->SYNCR |= ((0x01ul << eTrgIn) | (eTrgMode << (8 + eTrgIn)));
	
	return CSI_OK;
}
/** \brief rearm adc sync
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] eTrgIn: adc sync evtrg input channel(0~5)
 *  \return none
 */
void csi_adc_rearm_sync(csp_adc_t *ptAdcBase, csi_adc_trgin_e eTrgIn)
{
	csp_adc_rearm_sync(ptAdcBase, (adc_sync_e)eTrgIn);
}

/** \brief set adc evtrg output
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] byTrgOut: adc evtrg out port (0~1)
 *  \param[in] eTrgSrc: adc evtrg source(0~23) 
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_adc_set_evtrg(csp_adc_t *ptAdcBase, csi_adc_trgout_e eTrgOut, csi_adc_trgsrc_e eTrgSrc)
{
	switch(eTrgOut)
	{
		case ADC_TRGOUT0:		//event trigger out0
			ptAdcBase->EVTRG = (ptAdcBase->EVTRG & (~ADC12_TRGSRC0_MSK)) | (eTrgSrc << ADC12_TRGSRC0_POS);
			ptAdcBase->EVTRG |= ADC12_TRG0OE;
			break;
		case ADC_TRGOUT1:		//event trigger out1
			ptAdcBase->EVTRG = (ptAdcBase->EVTRG & (~ADC12_TRGSRC1_MSK)) | (eTrgSrc << ADC12_TRGSRC1_POS);
			ptAdcBase->EVTRG |= ADC12_TRG1OE;
			break;
		default:
			return CSI_ERROR;
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
csi_error_t csi_adc_set_epvs(csp_adc_t *ptAdcBase, csi_adc_trgout_e eTrgOut, uint8_t byPeriod)
{
	switch(eTrgOut)
	{
		case ADC_TRGOUT0:		//event trigger out0
			ptAdcBase->EPVS = (ptAdcBase->EPVS & (~ADC12_EV0PRD_MSK)) | ADC12_TRGEV0PRD(byPeriod);
			break;
		case ADC_TRGOUT1:		//event trigger out1
			ptAdcBase->EPVS = (ptAdcBase->EPVS & (~ADC12_EV1PRD_MSK)) | ADC12_TRGEV1PRD(byPeriod);
			break;
		default:
			return CSI_ERROR;
	}
	return CSI_OK;
}
/** \brief enable/disable adc INT status
 * 
 *  \param[in] ptAdcBase: ADC handle to operate
 *  \param[in] eIntSrc:  INT
 */
void csi_adc_int_enable(csp_adc_t *ptAdcBase, csi_adc_intsrc_e eIntSrc, bool bEnable)
{
	csp_adc_int_enable(ptAdcBase, (adc_int_e)eIntSrc, bEnable);
	
	if(bEnable)
		csi_irq_enable((uint32_t *)ptAdcBase);
	else
		csi_irq_disable((uint32_t *)ptAdcBase);
}
/** \brief select adc clock
 * 
 *  \param[in] ptAdcBase: ADC handle to operate
 *  \param[in] eClksel:  ADC clock select
 */
void csi_adc_set_clk(csp_adc_t *ptAdcBase,csi_adc_clksel_e eClksel)
{
	csp_adc_clk_sel(ptAdcBase,(adc_clksel_e)eClksel);
}


