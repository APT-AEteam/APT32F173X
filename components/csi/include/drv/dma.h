/***********************************************************************//** 
 * \file  dma.h
 * \brief  head file for DMA
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-12-23 <td>V0.0  <td>ZJY   <td>initial
 * <tr><td> 2023-9-22 <td>V0.1  <td>GQQ   <td>code normalization
 * </table>
 * *********************************************************************
*/

#ifndef _DRV_DMA_H_
#define _DRV_DMA_H_


#include <csp.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \enum     csi_dma_addr_inc_e
 * \brief    DMA addr inc mode
 */
typedef enum {
    DMA_ADDR_INC    	= 0,
    DMA_ADDR_CONSTANT
} csi_dma_addr_inc_e;

/**
 * \enum     csi_dma_dsize_t
 * \brief    DMA transfer data size width
 */
typedef enum {
    DMA_DSIZE_8_BITS  	= 0,
    DMA_DSIZE_16_BITS,
    DMA_DSIZE_32_BITS
} csi_dma_dsize_e;

/**
 * \enum     csi_dma_reload_e
 * \brief    DMA auto reload enable
 */
typedef enum{
	DMA_RELOAD_ENABLE	= 0,
	DMA_RELOAD_DISABLE
}csi_dma_reload_e;

/**
 * \enum     csi_dma_runmode_e
 * \brief    DMA serve mode 
 */
typedef enum{
	DMA_RUN_ONCE		= 0,
	DMA_RUN_CONT				
}csi_dma_runmode_e;

/**
 * \enum     csi_dma_tsize_e
 * \brief    DMA Tsize mode 
 */
typedef enum{
	DMA_TSIZE_ONE_DSIZE	= 0,
	DMA_TSIZE_FOUR_DSIZE,
	
}csi_dma_tsize_e;

/**
 * \enum     csi_dma_req_e
 * \brief    DMA request mode 
 */
typedef enum{
	DMA_REQ_SOFTWARE	= 0,
	DMA_REQ_HARDWARE
}csi_dma_req_e;

/**
 * \enum     csi_etb_chid_e
 * \brief    ETB channel id(number 0~31)
 */
typedef enum {
    DMA_CH0				= 0,	//dma channel 0 
	DMA_CH1,					//dma channel 1 
	DMA_CH2,					//dma channel 2 
	DMA_CH3,					//dma channel 3 
	DMA_CH4,					//dma channel 4 
	DMA_CH5,					//dma channel 5 
} csi_dma_ch_e;

/**
 * \enum     csi_dma_int_msg_e
 * \brief    DMA interrupt message
 */
typedef enum {
    DMA_CH0_LTCIT_MSG	= (0x01ul << 0),	//dma channel 0 LTCIT messsage
	DMA_CH1_LTCIT_MSG	= (0x01ul << 1),	//dma channel 1 LTCIT messsage
	DMA_CH2_LTCIT_MSG	= (0x01ul << 2),	//dma channel 2 LTCIT messsage
	DMA_CH3_LTCIT_MSG	= (0x01ul << 3),	//dma channel 3 LTCIT messsage
	DMA_CH4_LTCIT_MSG	= (0x01ul << 4),	//dma channel 4 LTCIT messsage
	DMA_CH5_LTCIT_MSG	= (0x01ul << 5),	//dma channel 5 LTCIT messsage
	DMA_CH0_TCIT_MSG	= (0x01ul << 16),	//dma channel 0 TCIT messsage
	DMA_CH1_TCIT_MSG	= (0x01ul << 17),	//dma channel 1 TCIT messsage
	DMA_CH2_TCIT_MSG	= (0x01ul << 18),	//dma channel 2 TCIT messsage
	DMA_CH3_TCIT_MSG	= (0x01ul << 19),	//dma channel 3 TCIT messsage
	DMA_CH4_TCIT_MSG	= (0x01ul << 20),	//dma channel 4 TCIT messsage
	DMA_CH5_TCIT_MSG	= (0x01ul << 21),	//dma channel 5 TCIT messsage
} csi_dma_int_msg_e;

/**
 * \enum     csi_dma_intsrc_e
 * \brief    DMA interrupt source 
 */
typedef enum
{
	DMA_INTSRC_LTCIT  	=	(0x01ul << 29),		//LTICT interrupt
	DMA_INTSRC_TCIT   	=	(0x01ul << 30),		//LTICT interrupt
	DMA_INTSRC_ALL		=   (0x03ul << 29)
}csi_dma_intsrc_e;

/**
 * \enum     csi_dma_intsrc_e
 * \brief    DMA interrupt source 
 */
typedef enum
{
	DMA_INTSTA_CH0  	=	(0x01ul << 0),		//CH0 INT STATUS
	DMA_INTSTA_CH1   	=	(0x01ul << 1),		//CH1 INT STATUS
	DMA_INTSTA_CH2		=	(0x01ul << 2),		//CH2 INT STATUS
	DMA_INTSTA_CH3  	=	(0x01ul << 3),		//CH3 INT STATUS
	DMA_INTSTA_CH4   	=	(0x01ul << 4),		//CH4 INT STATUS
	DMA_INTSTA_CH5		=	(0x01ul << 5),		//CH5 INT STATUS		
}csi_dma_intsta_e;


/// \struct csi_dma_ch_config_t
/// \brief  dma parameter configuration, open to users  
typedef struct
{
	csi_dma_addr_inc_e		eSrcLinc;		//low transfer count src addr inc control
	csi_dma_addr_inc_e		eSrcHinc;		//high transfer count src addr inc control
	csi_dma_addr_inc_e		eDetLinc;		//lowtransfer count det addr inc control
	csi_dma_addr_inc_e		eDetHinc;		//high transfer count det addr inc control
	csi_dma_dsize_e			eDataWidth;		//transfer data size width
	csi_dma_reload_e		eReload;		//auto reload	
	csi_dma_runmode_e		eRunMode;		//dma serve(transfer) mode
	csi_dma_tsize_e			eTsizeMode;		//Tsize transfer mode
	csi_dma_req_e			eReqMode;		//request mode
} csi_dma_ch_config_t;

/** \brief dma interrupt handle function
 * 
 *  \param[in] ptDmaBase: pointer of dma register structure
 *  \return none
 */ 
void csi_dma_irqhandler(csp_dma_t *ptDmaBase);

/** 
  \brief 	   Init dma channel parameter config structure
  \param[in]   ptDmaBase	pointer of dma reg structure.
  \param[in]   eDmaCh		channel num of dma(6 channel: 0->5)
  \param[in]   ptChCfg		pointer of uart parameter config structure
  \return 	   error code \ref csi_error_t
 */ 
csi_error_t csi_dma_ch_init(csp_dma_t *ptDmaBase, csi_dma_ch_e eDmaCh, csi_dma_ch_config_t *ptChCfg);

/** \brief dma channel transfer start
 * 
 *  \param[in] ptDmaBase: pointer of dma reg structure.
 *  \param[in] eDmaCh: channel num of dma(6channel: 0->5)
 *  \param[in] pSrcAddr: src addr of transfer 
 *  \param[in] pDstAddr: dst addr of transfer 
 *  \param[in] hwHTranNum: high transfer num, hwHTranNum <= 0xfff; transfer number = hwHTranNum * hwLTranNum(TSIZE = ONCE)
 *  \param[in] hwLTranNum: low transfer num,  hwLTranNum <= 0xfff; transfer number = hwHTranNum * hwLTranNum(TSIZE = ONCE)
 * 			   transfer length (unit: bytes), if set data_width is 16, the length should be the multiple of 2, and
			   if set data_width is 32, the length should be the multiple of 4
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_dma_ch_start(csp_dma_t *ptDmaBase, csi_dma_ch_e eDmaCh, void *pSrcAddr, void *pDstAddr, uint16_t hwHTranNum, uint16_t hwLTranNum);

/** \brief dma channel transfer restart
 * 
 *  \param[in] ptDmaBase: pointer of dma reg structure.
 *  \param[in] eDmaCh: channel num of dma(6channel: 0->5)
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_dma_ch_restart(csp_dma_t *ptDmaBase, csi_dma_ch_e eDmaCh);

/** 
  \brief 	   enable/disable dma interrupt 
  \param[in]   ptDmaBase	pointer of dma register structure
  \param[in]   eDmaCh		channel num of dma(6 channel: 0->5)
  \param[in]   eIntSrc		dma interrupt source
  \param[in]   bEnable		enable/disable interrupt
  \return none
 */
void csi_dma_int_enable(csp_dma_t *ptDmaBase,  csi_dma_ch_e eDmaCh, csi_dma_intsrc_e eIntSrc);

/** \brief disable dma interrupt 
 * 
 *  \param[in] ptDmaBase: pointer of dma register structure
 *  \param[in] eDmaCh: channel num of dma(6 channel: 0->5)
 *  \param[in] eIntSrc: dma interrupt source
 *  \return none
 */
void csi_dma_int_disable(csp_dma_t *ptDmaBase, csi_dma_ch_e eDmaCh, csi_dma_intsrc_e eIntSrc);

/** \brief clear dma interrupt 
 * 
 *  \param[in] ptDmaBase: pointer of dma register structure
 *  \param[in] eIntSta: dma interrupt status
 *  \return none
 */
void csi_dma_clr_isr(csp_dma_t *ptDmaBase,  csi_dma_intsta_e eIntSta);

/**
  \brief       Stop a dma channel
  \param[in]   ptDmaBase	pointer of dma register structure
  \param[in]   eDmaCh		channel num of dma(6 channel: 0->5)
  \return      none
*/
void csi_dma_ch_stop(csp_dma_t *ptDmaBase, csi_dma_ch_e eDmaCh);

/**
  \brief       Reset dma module
  \param[in]   ptDmaBase	pointer of dma register structure
  \return      none
*/
void csi_dma_sw_rst(csp_dma_t *ptDmaBase);


/** 
  \brief 	   get dma idx 
  \param[in]   ptDmaBase	pointer of uart register structure
  \return      dma id number(0~1) or error(0xff)
 */ 
uint8_t csi_get_dma_idx(csp_dma_t *ptDmaBase);

/** 
  \brief 	   get dma interrupt message and (D0 not)clear message
  \param[in]   eDmaCh		dma channel number, channel 0->5
  \param[in]   bClrEn		bClrEn: clear dma interrupt message enable; ENABLE: clear , DISABLE: Do not clear
  \return 	   bool type true/false
 */ 
bool csi_dma_get_msg(csp_dma_t *ptDmaBase, csi_dma_ch_e eDmaCh, bool bClrEn);


#ifdef __cplusplus
}
#endif

#endif /* _CSI_DMA_H_ */
