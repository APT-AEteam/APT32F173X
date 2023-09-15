/***********************************************************************//** 
 * \file  sio.c
 * \brief  csi sio driver
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-9-03 <td>V0.0  <td>XB   <td>initial
 * <tr><td> 2021-1-03 <td>V0.0  <td>ZJY   <td>modify
 * <tr><td> 2021-12-03 <td>V0.1  <td>LQ   <td>modify
 * </table>
 * *********************************************************************
*/
#include "drv/sio.h"

/* Private macro------------------------------------------------------*/
#define SIO_RESET_VALUE  (0x00000000)
#define SIO_RX_TIMEOUT		(0x10ff)
#define SIO_TX_TIMEOUT		(0x1FFF)
/* externs function---------------------------------------------------*/
/* externs variablesr-------------------------------------------------*/
/* Private variablesr-------------------------------------------------*/
csi_sio_ctrl_t g_tSioCtrl[SIO_IDX];	

/** \brief get sio idx 
 * 
 *  \param[in] ptSioBase: pointer of sio register structure
 *  \return sio id number(0~1) or error(0xff)
 */ 
static uint8_t apt_get_sio_idx(csp_sio_t *ptSioBase)
{
	switch((uint32_t)ptSioBase)
	{
		case APB_SPI0_BASE:
			return 0;
		case APB_SPI1_BASE:
			return 1;
		default:
			return 0xff;		//error
	}
}
/** \brief  register sio interrupt callback function
 * 
 *  \param[in] ptSioBase: pointer of sio register structure
 *  \param[in] eCallBkId: sio interrupt callback type, \ref csi_uart_callback_id_e
 *  \param[in] callback: sio interrupt handle function
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_sio_register_callback(csp_sio_t *ptSioBase, csi_sio_callback_id_e eCallBkId, void  *callback)
{
	uint8_t byIdx = apt_get_sio_idx(ptSioBase);
	if(byIdx == 0xff)
		return CSI_ERROR;

	switch(eCallBkId)
	{
		case SIO_CALLBACK_RECV:
			g_tSioCtrl[byIdx].recv_callback = callback;
			break;
		case SIO_CALLBACK_SEND:
			g_tSioCtrl[byIdx].send_callback = callback;
			break;
		case SIO_CALLBACK_ERR:
			g_tSioCtrl[byIdx].err_callback = callback;
			break;
		default:
			return CSI_ERROR;
	}
	return CSI_OK;
}
/** \brief sio interrupt handler function
 * 
 *  \param[in] ptSioBase: pointer of uart sio structure
 *  \param[in] byIdx: sio idx(0/1)
 *  \return none
 */ 
void csi_sio_irqhandler(csp_sio_t *ptSioBase, uint8_t byIdx)
{
	volatile uint8_t byIsr = csp_sio_get_isr(ptSioBase) & 0x2f;
	
	switch(byIsr)
	{
		case SIO_RXBUFFULL:										
		case SIO_RXDNE:
			if(NULL == g_tSioCtrl[byIdx].pwData || 0 == g_tSioCtrl[byIdx].hwSize)
			{
				csp_sio_get_rxbuf(ptSioBase);
				g_tSioCtrl[byIdx].byRxStat = SIO_STATE_ERROR;				//receive error
			}
			else
			{
				*(g_tSioCtrl[byIdx].pwData + g_tSioCtrl[byIdx].hwTransNum) = csp_sio_get_rxbuf(ptSioBase);	//receive data
				g_tSioCtrl[byIdx].hwTransNum ++;
				if(g_tSioCtrl[byIdx].hwTransNum >= g_tSioCtrl[byIdx].hwSize)
				{
					g_tSioCtrl[byIdx].byRxStat = SIO_STATE_FULL;			//receive buf full, g_tSioCtrl.hwTransNum = receive data len = receive buf len
					csp_sio_woke_rst(ptSioBase);
					
					if(g_tSioCtrl[byIdx].recv_callback)
						g_tSioCtrl[byIdx].recv_callback(ptSioBase, g_tSioCtrl[byIdx].pwData, g_tSioCtrl[byIdx].hwSize);
				}
			}
			csp_sio_clr_isr(ptSioBase, SIO_RXDNE | SIO_RXBUFFULL);
			break;
	
		case SIO_TXBUFEMPT:
			csp_sio_clr_isr(ptSioBase, SIO_TXBUFEMPT);
		 	ptSioBase->TXBUF = *(g_tSioCtrl[byIdx].pwData);
			g_tSioCtrl[byIdx].pwData++;
			g_tSioCtrl[byIdx].hwTransNum++;
			
			if(g_tSioCtrl[byIdx].hwTransNum >= g_tSioCtrl[byIdx].hwSize)
			{
				csp_sio_int_disable(ptSioBase, (sio_int_e)SIO_INTSRC_TXBUFEMPT);
				g_tSioCtrl[byIdx].hwTransNum = 0;
				g_tSioCtrl[byIdx].byTxStat = SIO_STATE_IDLE;
				
				//call back
				if(g_tSioCtrl[byIdx].send_callback)
					g_tSioCtrl[byIdx].send_callback(ptSioBase);	
			}
			break;
		case SIO_TXDNE:
			csp_sio_clr_isr(ptSioBase, SIO_TXDNE);
			break;
		case SIO_TIMEOUT:
			if(g_tSioCtrl[byIdx].err_callback)
				g_tSioCtrl[byIdx].err_callback(ptSioBase, byIsr);
			csp_sio_clr_isr(ptSioBase, SIO_TIMEOUT);
			break;
		case SIO_BREAK:												//receive break interrupt ,reset receive module
			if(g_tSioCtrl[byIdx].err_callback)
				g_tSioCtrl[byIdx].err_callback(ptSioBase, byIsr);
			csp_sio_clr_isr(ptSioBase, SIO_BREAK);
			break;
		default:
			csp_sio_clr_isr(ptSioBase, 0x2f);
			break;
	}
}
/** \brief Init sio tx, Initializes the resources needed for the sio instance 
 * 
 *  \param[in] ptSioBase: pointer of sio register structure
 *  \param[in] ptTxCfg: pointer of sio parameter config structure
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_sio_tx_init(csp_sio_t *ptSioBase, csi_sio_tx_config_t *ptTxCfg)
{
	csi_clk_enable((uint32_t *)ptSioBase);		//sio peripheral clk en
	csp_sio_clk_enable(ptSioBase);				//enable clk
	csp_sio_set_mode(ptSioBase, SIO_MODE_TX);	//sio tx mode
	
	if(ptTxCfg->wTxFreq > 1)					//tx clk config
		csp_sio_set_tx_clkdiv(ptSioBase, (csi_get_pclk_freq()/ptTxCfg->wTxFreq - 1));
	else
		return CSI_ERROR;
	
	//set TX CR0~1 reg
	csp_sio_set_txcr0(ptSioBase, (sio_idlest_e)ptTxCfg->eIdleLev, (sio_tdir_e)ptTxCfg->eTxDir, ptTxCfg->byTxBufLen - 1, ptTxCfg->byTxCnt - 1);	
	csp_sio_set_d0(ptSioBase, ptTxCfg->byD0Len - 1);					//set d0 clk len
	csp_sio_set_d1(ptSioBase, ptTxCfg->byD1Len - 1);					//set d1 clk len
	csp_sio_set_dl(ptSioBase, ptTxCfg->byDLLen - 1, ptTxCfg->byDLLsq);	//set dl clk len and lsq
	csp_sio_set_dh(ptSioBase, ptTxCfg->byDHLen - 1, ptTxCfg->byDHHsq);	//set dl clk len and hsq
	csi_irq_enable((uint32_t*)ptSioBase);								//enable sio vic interrupt 

	return CSI_OK;
}

/** \brief Init sio rx, Initializes the resources needed for the sio instance 
 * 
 *  \param[in] ptSioBase: pointer of sio register structure
 *  \param[in] ptRxCfg: pointer of sio parameter config structure
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_sio_rx_init(csp_sio_t *ptSioBase, csi_sio_rx_config_t *ptRxCfg)
{
	csi_clk_enable((uint32_t *)ptSioBase);		//sio peripheral clk en
	csp_sio_clk_enable(ptSioBase);					//enable clk
	csp_sio_set_mode(ptSioBase, SIO_MODE_RX);	//sio rx mode
	
	if(ptRxCfg->wRxFreq > 1)					//tx clk config
		csp_sio_set_rx_clkdiv(ptSioBase, (csi_get_pclk_freq()/ptRxCfg->wRxFreq - 1));
	else
		return CSI_ERROR;
	
	//rx receive config
	csp_sio_set_rx_deb(ptSioBase, ptRxCfg->byDebPerLen - 1, ptRxCfg->byDebClkDiv - 1);													//set rx sampling debounce 
	csp_sio_set_sample_mode(ptSioBase,(sio_bstsel_e)ptRxCfg->eTrgEdge,(sio_trgmode_e)ptRxCfg->eTrgMode,(sio_rmode_e)ptRxCfg->eSpMode);	//set rx samping mode
	csp_sio_set_sample(ptSioBase, (sio_extract_e)ptRxCfg->eSpExtra, SIO_ALIGN_EN, ptRxCfg->bySpBitLen - 1, ptRxCfg->byHithr);			//set rx samping control
	csp_sio_set_recv(ptSioBase, (sio_rdir_e)ptRxCfg->eRxDir, ptRxCfg->byRxBufLen - 1, ptRxCfg->byRxCnt - 1);							//set receive para
	
	csi_irq_enable((uint32_t*)ptSioBase);		//enable sio vic interrupt 
	
	return CSI_OK;
}
/** \brief sio receive break reset config
 * 
 *  \param[in] ptSioBase: pointer of sio register structure
 *  \param[in] eBkLev: break reset detect level
 *  \param[in] byBkCnt: break reset detect period
 *  \param[in] bEnable: ENABLE/DISABLE break reset and break interrupt
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_sio_set_break(csp_sio_t *ptSioBase, csi_sio_bklev_e eBkLev, uint8_t byBkCnt, bool bEnable)
{
	if(byBkCnt == 0)
		return CSI_ERROR;
		
	csp_sio_set_break(ptSioBase, bEnable, (sio_breaklel_e)eBkLev, byBkCnt - 1);

	return CSI_OK; 
}

/** \brief sio receive timeout reset config
 * 
 *  \param[in] ptSioBase: pointer of sio register structure
 *  \param[in] byToCnt: sample timeout period
 *  \param[in] bEnable: ENABLE/DISABLE sample timeout reset
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_sio_set_samp_timeout(csp_sio_t *ptSioBase, uint8_t byToCnt ,bool bEnable)
{
	if(byToCnt == 0)
		return CSI_ERROR;
		
	csp_sio_set_torst(ptSioBase, bEnable, byToCnt - 1);
	
	return CSI_OK; 
}
/** \brief sio transfer mode set,send(tx)/receive(rx)
 * 
 *  \param[in] ptSioBase: pointer of sio register structure
 *  \param[in] eWorkMd: sio working mode, send(tx)/receive(rx)
 *  \return none
 */
void csi_sio_set_mode(csp_sio_t *ptSioBase, csi_sio_wkmode_e eWorkMd)
{
	csp_sio_set_mode(ptSioBase, (sio_mode_e)eWorkMd);
}
/** \brief enable/disable sio interrupt 
 * 
 *  \param[in] ptSioBase: pointer of sio register structure
 *  \param[in] eIntSrc: sio interrupt source
 *  \return none
 */
void csi_sio_int_enable(csp_sio_t *ptSioBase, csi_sio_intsrc_e eIntSrc)
{
	csp_sio_int_enable(ptSioBase, (sio_int_e)eIntSrc);
}
/** \brief enable/disable sio interrupt 
 * 
 *  \param[in] ptSioBase: pointer of sio register structure
 *  \param[in] eIntSrc: sio interrupt source
 *  \return none
 */
void csi_sio_int_disable(csp_sio_t *ptSioBase, csi_sio_intsrc_e eIntSrc)
{
	csp_sio_int_disable(ptSioBase, (sio_int_e)eIntSrc);
}

/** \brief send data from sio, this function is polling mode(sync mode)
 * 
 * \param[in] ptSioBase: pointer of sio register structure
 * \param[in] pwData: pointer to buffer with data to send 
 * \param[in] hwSize: send data size
 * \return error code \ref csi_error_t or receive data size
 */
int32_t csi_sio_send(csp_sio_t *ptSioBase, const uint32_t *pwData, uint16_t hwSize)
{
	uint16_t  i;

	if(NULL == pwData || 0 == hwSize)
		return -1;
		
	for(i = 0; i < hwSize; i++)									
	{
		csp_sio_set_txbuf(ptSioBase,pwData[i]);
		nop;nop;nop;nop;
		while(!(csp_sio_get_risr(ptSioBase) & SIO_TXBUFEMPT));
	}
	while(!(csp_sio_get_risr(ptSioBase) & SIO_TXDNE));
	csp_sio_clr_isr(ptSioBase, SIO_TXDNE);
	
	return i;
}

/** \brief send data from sio, this function is polling mode(sync mode)
 * 
 * \param[in] ptSioBase: pointer of sio register structure
 * \param[in] pwData: pointer to buffer with data to send 
 * \param[in] hwSize: send data size
 * \return error code \ref csi_error_t or receive data size
 */
csi_error_t csi_sio_send_int(csp_sio_t *ptSioBase, const uint32_t *pwSend, uint16_t hwSize)
{
	uint8_t byIdx = apt_get_sio_idx(ptSioBase);
	
	if(NULL == pwSend || 0 == hwSize)
		return CSI_ERROR;
		
	g_tSioCtrl[byIdx].pwData = (uint32_t *)pwSend;
	g_tSioCtrl[byIdx].hwSize = hwSize;
	g_tSioCtrl[byIdx].hwTransNum = 0;
	g_tSioCtrl[byIdx].byTxStat = SIO_STATE_SEND;
	csp_sio_int_enable(SIO0, SIO_TXBUFEMPT);;
	
	return CSI_OK;
}
/** \brief set sio receive data buffer and length
 * 
 *  \param[in] pwData: pointer of sio transport data buffer
 *  \param[in] hwLen: sio transport data length
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_sio_set_buffer(uint32_t *pwData, uint16_t hwLen)
{
	
	if(NULL == pwData || hwLen == 0)
		return CSI_ERROR;
	
//	g_tSioTran.pwData 	 = pwData;
//	g_tSioTran.hwSize 	 = hwLen;
//	g_tSioTran.hwTransNum = 0;
//	g_tSioTran.byRxStat  = SIO_STATE_IDLE;
//	g_tSioTran.byTxStat  = SIO_STATE_IDLE;
	
	return CSI_OK;
}
/** \brief receive data to sio transmitter, asynchronism mode
 * 
 * \param[in] ptSioBase: pointer of sio register structure
 * \param[in] pwRecv: pointer of sio receive data
 * \param[in] hwSize: receive data size
 * \return error code \ref csi_error_t or receive data len
 */
int32_t csi_sio_receive_int(csp_sio_t *ptSioBase, uint32_t *pwRecv, uint16_t hwSize)
{
	uint8_t byIdx = apt_get_sio_idx(ptSioBase);
	
	if(NULL == pwRecv || hwSize == 0)
		return CSI_ERROR;
	
	g_tSioCtrl[byIdx].pwData	= pwRecv;
	g_tSioCtrl[byIdx].hwSize 	= hwSize;
	g_tSioCtrl[byIdx].hwTransNum = 0;
	g_tSioCtrl[byIdx].byRxStat  = SIO_STATE_RECV;
	csp_sio_int_enable(SIO0, SIO_RXBUFFULL);
	
	return CSI_OK;
}
/** \brief send data from sio, this function is dma mode
 * 
 *  \param[in] ptSioBase: pointer of sio register structure
 *  \param[in] ptDmaBase: pointer of dma register structure
 *  \param[in] eDmaCh: channel number of dma, eDmaCh: DMA_CH0` DMA_CH5
 *  \param[in] pData: pointer to buffer with data to send to uart transmitter.
 *  \param[in] hwSize: number of data to send (byte); hwSize <= 0xfff
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_sio_send_dma(csp_sio_t *ptSioBase, csp_dma_t *ptDmaBase, csi_dma_ch_e eDmaCh, const void *pData, uint16_t hwSize)
{
	if(hwSize > 0xfff)
		return CSI_ERROR;
		
	csp_sio_txdma_enable(ptSioBase, ENABLE);
	csi_dma_ch_start(ptDmaBase, eDmaCh, (void *)pData, (void *)&(ptSioBase->TXBUF), hwSize, 1);
	
	return CSI_OK;
}
/** \brief send data from sio, this function is dma mode
 * 
 *  \param[in] ptSioBase: pointer of sio register structure
 *  \param[in] ptDmaBase: pointer of dma register structure
 *  \param[in] eDmaCh: channel number of dma, eDmaCh: DMA_CH0` DMA_CH5
 *  \param[in] pData: pointer to buffer with data to send to uart transmitter.
 *  \param[in] hwSize: number of data to send (byte); hwSize <= 0xfff
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_sio_recv_dma(csp_sio_t *ptSioBase, csp_dma_t *ptDmaBase, csi_dma_ch_e eDmaCh, const void *pData, uint16_t hwSize)
{
	if(hwSize > 0xfff)
		return CSI_ERROR;
	csp_sio_rxdma_enable(ptSioBase, ENABLE);
	csi_dma_ch_start(ptDmaBase, eDmaCh, (void *)&(ptSioBase->RXBUF), (void *)pData, hwSize, 1);
	
	return CSI_OK;
}
/** \brief get sio receive/send complete message and (Do not) clear message
 * 
 *  \param[in] ptSioBase: pointer of sio reg structure.
 *  \param[in] eWkMode: tx or rx mode
 *  \param[in] bClrEn: clear sio receive/send complete message enable; ENABLE: clear , DISABLE: Do not clear
 *  \return  bool type true/false
 */ 
bool csi_sio_get_msg(csp_sio_t *ptSioBase, csi_sio_wkmode_e eWkMode, bool bClrEn)
{
	bool bRet = false;
	
//	switch(eWkMode)
//	{
//		case SIO_SEND:
//			if(g_tSioTran.byTxStat == SIO_STATE_DONE)
//			{
//				if(bClrEn)
//					g_tSioTran.byTxStat = SIO_STATE_IDLE;		//clear send status
//				bRet = true;
//			}
//			break;
//		case SIO_RECV:
//			if(g_tSioTran.byRxStat == SIO_STATE_FULL)
//			{
//				if(bClrEn)
//					g_tSioTran.byRxStat = SIO_STATE_IDLE;		//clear receive status
//				bRet = true;
//			}
//			break;
//		default:
//			break;
//	}
	
	return bRet;
}
/** \brief clr sio receive/send status message (set sio recv/send status idle) 
 * 
 *  \param[in] ptSioBase: pointer of sio reg structure.
 *  \param[in] eMode: tx or rx mode
 *  \return none
 */ 
void csi_sio_clr_msg(csp_sio_t *ptSioBase, csi_sio_wkmode_e eWkMode)
{
//	
//	if(eWkMode == SIO_SEND)
//		g_tSioTran.byTxStat = SIO_STATE_IDLE;		//clear send status
//	else
//		g_tSioTran.byRxStat = SIO_STATE_IDLE;		//clear receive status		
}