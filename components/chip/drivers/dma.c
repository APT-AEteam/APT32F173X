/***********************************************************************//** 
 * \file  dma.c
 * \brief  DMA description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-12-23 <td>V0.0  <td>ZJY   <td>initial
 * <tr><td> 2023-9-22 <td>V0.1  <td>GQQ   <td>code normalization
 * </table>
 * *********************************************************************
*/

/* Includes ---------------------------------------------------------------*/

#include "drv/dma.h"


/* Private macro-----------------------------------------------------------*/
/* externs function--------------------------------------------------------*/
/* Private function--------------------------------------------------------*/
static uint8_t apt_get_dma_idx(csp_dma_t *ptDmaBase);
static uint8_t apt_dma_post_msg(csp_dma_t *ptDmaBase, csi_dma_int_msg_e eIntMsg, uint8_t byPost);

/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/
static uint16_t s_hwDmaMsg[2]	= {0, 0};


/** \brief dma interrupt handle function
 * 
 *  \param[in] ptDmaBase: pointer of dma register structure
 *  \return none
 */ 
void csi_dma_irqhandler(csp_dma_t *ptDmaBase)
{
	volatile uint32_t wIsr = csp_dma_get_isr(ptDmaBase) & 0x003f003f;
	
	switch(wIsr)
	{
		//LTCIT
		case DMA_CH0_LTCIT_SR:
		case DMA_CH1_LTCIT_SR:
		case DMA_CH2_LTCIT_SR:
		case DMA_CH3_LTCIT_SR:
		case DMA_CH4_LTCIT_SR:
		case DMA_CH5_LTCIT_SR:
			csp_dma_clr_isr(ptDmaBase, (uint8_t)wIsr);		///clear LTCIT status
			apt_dma_post_msg(ptDmaBase, wIsr, 1);			//post LTCIT interrupt message
			break;
		
		//TCIT 
		case DMA_CH0_TCIT_SR:
		case DMA_CH1_TCIT_SR:
		case DMA_CH2_TCIT_SR:
		case DMA_CH3_TCIT_SR:
		case DMA_CH4_TCIT_SR:
		case DMA_CH5_TCIT_SR:
			csp_dma_clr_isr(ptDmaBase, (wIsr >> 16));		//clear LTCIT status
			apt_dma_post_msg(ptDmaBase, (wIsr >> 10), 1);	//post TCIT interrupt message
			break;
		default:
			break;
	}
}


/** \brief Init dma channel parameter config structure
 * 
 *  \param[in] ptDmaBase: pointer of dma reg structure.
 *  \param[in] eDmaCh: channel num of dma(6 channel: 0->5) \ref csi_dma_ch_e
 *  \param[in] ptChCfg: pointer of uart parameter config structure
 * 					-eSrcLinc: low transfer count src addr inc control \ref csi_dma_addr_inc_e
 * 					-eSrcHinc: high transfer count src addr inc control \ref csi_dma_addr_inc_e
 * 					-eDetLinc: low transfer count det addr inc control \ref csi_dma_addr_inc_e
 * 					-eDetHinc: high transfer count det addr inc control \ref csi_dma_addr_inc_e
 * 					-eDataWidth: transfer data size width \ref csi_dma_dsize_e
 * 					-eReload: auto reload \ref csi_dma_reload_e
 * 					-eRunMode: dma serve(transfer) mode \ref csi_dma_runmode_e
 * 					-eTsizeMode: Tsize transfer mode \ref csi_dma_tsize_e
 * 					-eReqMode: request mode ,software request or hardware request \ref csi_dma_req_e
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_dma_ch_init(csp_dma_t *ptDmaBase, csi_dma_ch_e eDmaCh, csi_dma_ch_config_t *ptChCfg)
{
	csp_dma_t *ptDmaChBase = (csp_dma_t *)DMA_REG_BASE(ptDmaBase, eDmaCh);
	
	if(eDmaCh >= DMA_CH_MAX_NUM)
		return CSI_ERROR;
	
	csp_dma_set_ch_saddr_mode(ptDmaChBase, (dma_linc_e)ptChCfg->eSrcLinc, (dma_hinc_e)ptChCfg->eSrcHinc);										//Src addr control mode
	csp_dma_set_ch_daddr_mode(ptDmaChBase, (dma_linc_e)ptChCfg->eDetLinc, (dma_hinc_e)ptChCfg->eDetHinc);										//Det addr control mode
	csp_dma_set_ch(ptDmaChBase, (dma_dsize_e)ptChCfg->eDataWidth, (dma_reload_e)ptChCfg->eReload, (dma_smode_e)ptChCfg->eRunMode, (dma_tsize_e)ptChCfg->eTsizeMode);	//dma ch para config
	csp_dma_set_ch_req(ptDmaChBase, (dma_req_e)ptChCfg->eReqMode);																//software or hardware request
		
	return CSI_OK;	
}
/** \brief dma channel transfer start
 * 
 *  \param[in] ptDmaBase: pointer of dma reg structure.
 *  \param[in] eDmaCh: channel num of dma(6 channel: 0->5)
 *  \param[in] pSrcAddr: src addr of transfer 
 *  \param[in] pDstAddr: dst addr of transfer 
 *  \param[in] hwHTranNum: high transfer num, hwHTranNum <= 0xfff; transfer number = hwHTranNum * hwLTranNum(TSIZE = ONCE)
 *  \param[in] hwLTranNum: low transfer num,  hwLTranNum <= 0xfff; transfer number = hwHTranNum * hwLTranNum(TSIZE = ONCE)
 * 			   transfer length (unit: bytes), if set data_width is 16, the length should be the multiple of 2, and
			   if set data_width is 32, the length should be the multiple of 4
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_dma_ch_start(csp_dma_t *ptDmaBase, csi_dma_ch_e eDmaCh, void *pSrcAddr, void *pDstAddr, uint16_t hwHTranNum, uint16_t hwLTranNum)
{
	csp_dma_t *ptDmaChBase = (csp_dma_t *)DMA_REG_BASE(ptDmaBase, eDmaCh);
	
	if((eDmaCh >= DMA_CH_MAX_NUM) || ((hwHTranNum == 0) && (hwLTranNum == 0)))
		return CSI_ERROR;

	csp_dma_set_ch_trans_num(ptDmaChBase, hwLTranNum, hwHTranNum);	//continuous mode: data length
	csp_dma_set_ch_src_addr(ptDmaChBase, (uint32_t)pSrcAddr);		//Src addr
	csp_dma_set_ch_dst_addr(ptDmaChBase, (uint32_t)pDstAddr);		//dst addr
	
	while(csp_dma_get_ltcst(ptDmaChBase) == 1);						//waite until DMA idle
	csp_dma_ch_enable(ptDmaChBase);									//channel enable

	if(!csp_dma_get_rsrx(ptDmaChBase))
		csp_dma_ch_sw_trg(ptDmaChBase);								//sw triger 
	
	return CSI_OK;
}

/** \brief dma channel transfer restart
 * 
 *  \param[in] ptDmaBase: pointer of dma reg structure.
 *  \param[in] eDmaCh: channel num of dma(6 channel: 0->5)
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_dma_ch_restart(csp_dma_t *ptDmaBase, csi_dma_ch_e eDmaCh)
{

	csp_dma_t *ptDmaChBase = (csp_dma_t *)DMA_REG_BASE(ptDmaBase, eDmaCh);
	
	if(eDmaCh >= DMA_CH_MAX_NUM)
		return CSI_ERROR;

	if(csp_dma_get_crx(ptDmaChBase) & DMA_RELOAD_MSK)               ///if reload disable,enable channel
		csp_dma_ch_enable(ptDmaChBase);									
	if(!csp_dma_get_rsrx(ptDmaChBase))
		csp_dma_ch_sw_trg(ptDmaChBase);								///sw triger 
	
	return CSI_OK;
}

/** \brief enable dma interrupt 
 * 
 *  \param[in] ptDmaBase: pointer of dma register structure
 *  \param[in] eDmaCh: channel num of dma(6 channel: 0->5)
 *  \param[in] eIntSrc: dma interrupt source
 *  \return none
 */
void csi_dma_int_enable(csp_dma_t *ptDmaBase, csi_dma_ch_e eDmaCh, csi_dma_intsrc_e eIntSrc)
{
	csp_dma_t *ptDmaChBase = (csp_dma_t *)DMA_REG_BASE(ptDmaBase, eDmaCh);
	
	csp_dma_clr_isr(ptDmaChBase, 0x1<<eDmaCh);
	csp_dma_int_enable(ptDmaChBase, (dma_int_e)eIntSrc);
}

/** \brief disable dma interrupt 
 * 
 *  \param[in] ptDmaBase: pointer of dma register structure
 *  \param[in] eDmaCh: channel num of dma(6 channel: 0->5)
 *  \param[in] eIntSrc: dma interrupt source
 *  \return none
 */
void csi_dma_int_disable(csp_dma_t *ptDmaBase, csi_dma_ch_e eDmaCh, csi_dma_intsrc_e eIntSrc)
{
	csp_dma_t *ptDmaChBase = (csp_dma_t *)DMA_REG_BASE(ptDmaBase, eDmaCh); 
	
	csp_dma_int_disable(ptDmaChBase, (dma_int_e)eIntSrc);
	
}

/** \brief clear dma interrupt 
 * 
 *  \param[in] ptDmaBase: pointer of dma register structure
 *  \param[in] eIntSta: dma interrupt status
 *  \return none
 */
void csi_dma_clr_isr(csp_dma_t *ptDmaBase,  csi_dma_intsta_e eIntSta)
{
	csp_dma_clr_isr(ptDmaBase, (dma_icr_e)eIntSta);
	
}
/** \brief dma channel transfer stop
 * 
 *  \param[in] ptDmaBase: pointer of dma reg structure.
 *  \param[in] eDmaCh: channel num of dma(6 channel: 0->5)
 *  \return none
 */
void csi_dma_ch_stop(csp_dma_t *ptDmaBase, csi_dma_ch_e eDmaCh)
{
	csp_dma_t *ptDmaChBase = (csp_dma_t *)DMA_REG_BASE(ptDmaBase, eDmaCh);
	csp_dma_ch_stop(ptDmaChBase);
}

/** \brief Software reset dma module
 * 
 *  \param[in] ptDmaBase: pointer of dma reg structure.
 *  \return none
 */ 
void csi_dma_sw_rst(csp_dma_t *ptDmaBase)
{
	csp_dma_sw_rst(ptDmaBase);
}

/** \brief get dma number 
 * 
 *  \param[in] ptDmaBase: pointer of dma register structure
 *  \return dma number 0/1
 */ 
static uint8_t apt_get_dma_idx(csp_dma_t *ptDmaBase)
{
	switch((uint32_t)ptDmaBase)
	{
		case APB_DMA0_BASE:		//dma0
			return 0;		
		case APB_DMA1_BASE:		//dma1
			return 1;
		default:
			return 0xff;		//error
	}
}

/** \brief dma interrupt handle function
 * 
 *  \param[in] eIntMsg: dma interrupt message
 *  \param[in] byPost: dma interrupt message post
 *  \return none
 */ 
static uint8_t apt_dma_post_msg(csp_dma_t *ptDmaBase, csi_dma_int_msg_e eIntMsg, uint8_t byPost)
{
	uint8_t byIdx = apt_get_dma_idx(ptDmaBase);
	
	if(0 == (s_hwDmaMsg[byIdx] & eIntMsg))
	{
		if(byPost)
			s_hwDmaMsg[byIdx] |= eIntMsg;
		return true;
	}
	else
		return false;

}

/** \brief get dma interrupt message and (D0 not)clear message
 * 
 *  \param[in] ptDmaBase: pointer of dma reg structure.
 *  \param[in] eDmaCh: channel num of dma(6 channel: 0->5)
 *  \param[in] bClrEn: clear dma interrupt message enable; ENABLE: clear , DISABLE: Do not clear
 *  \return  bool type true/false
 */ 
bool csi_dma_get_msg(csp_dma_t *ptDmaBase, csi_dma_ch_e eDmaCh, bool bClrEn)
{
	bool bRet = false;
	uint8_t byIdx = apt_get_dma_idx(ptDmaBase);
	
	if(0 != (s_hwDmaMsg[byIdx] & (0x01ul << eDmaCh))) 
	{
		if(bClrEn)
			s_hwDmaMsg[byIdx] &= ~(0x01ul << eDmaCh);
		
		bRet |= true;
	}
	
	if(0 != (s_hwDmaMsg[byIdx] & (0x01ul << (eDmaCh + 6))))
	{
		if(bClrEn)
			s_hwDmaMsg[byIdx] &= ~(0x01ul << (eDmaCh + 6));
		bRet |= true;
	}
	
	return bRet;
}


