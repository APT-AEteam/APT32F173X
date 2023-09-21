/***********************************************************************//** 
 * \file  sio.c
 * \brief  csi sio driver
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-9-03 <td>V0.0  <td>XB   <td>initial
 * <tr><td> 2021-1-03 <td>V0.1  <td>ZJY   <td>modify
 * <tr><td> 2023-9-21 <td>V0.2  <td>ZJY   <td>code normalization 
 * </table>
 * *********************************************************************
*/
#include "drv/sio.h"

/* Private macro------------------------------------------------------*/
/* externs function---------------------------------------------------*/
/* global variablesr--------------------------------------------------*/
csi_sio_ctrl_t g_tSioCtrl[SIO_IDX];	

/* Private variablesr-------------------------------------------------*/


/** \brief get sio idx 
 * 
 *  \param[in] ptSioBase: pointer of sio register structure
 *  \return sio id number(0~1) or error(0xff)
 */ 
static uint8_t apt_get_sio_idx(csp_sio_t *ptSioBase)
{
	switch((uint32_t)ptSioBase)
	{
		case APB_SIO0_BASE:
			return 0;
		case APB_SIO1_BASE:
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
 *  \param[in] ptSioBase: pointer of sio sio structure
 *  \param[in] byIdx: sio idx(0/1)
 *  \return none
 */ 
void csi_sio_irqhandler(csp_sio_t *ptSioBase, uint8_t byIdx)
{
	volatile uint8_t byIsr = csp_sio_get_isr(ptSioBase) & SIO_INT_ALL;
	
	switch(byIsr)
	{
		case SIO_INT_RXBUFFULL:										
		case SIO_INT_RXDNE:
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
					g_tSioCtrl[byIdx].byRxStat = SIO_STATE_RX_DNE;			//receive buf full, g_tSioCtrl.hwTransNum = receive data len = receive buf len
					csp_sio_logic_sw_rst(ptSioBase);
					
					if(g_tSioCtrl[byIdx].recv_callback)
						g_tSioCtrl[byIdx].recv_callback(ptSioBase, byIsr, g_tSioCtrl[byIdx].pwData, g_tSioCtrl[byIdx].hwSize);
				}
			}
			csp_sio_clr_isr(ptSioBase, SIO_INT_RXDNE | SIO_INT_RXBUFFULL);
			break;
	
		case SIO_INT_TXBUFEMPT:
			csp_sio_clr_isr(ptSioBase, SIO_INT_TXBUFEMPT);
		 	ptSioBase->TXBUF = *(g_tSioCtrl[byIdx].pwData);
			g_tSioCtrl[byIdx].pwData++;
			g_tSioCtrl[byIdx].hwTransNum++;
			
			if(g_tSioCtrl[byIdx].hwTransNum >= g_tSioCtrl[byIdx].hwSize)
			{
				csp_sio_int_disable(ptSioBase, (sio_int_e)SIO_INTSRC_TXBUFEMPT);
				g_tSioCtrl[byIdx].hwTransNum = 0;
				g_tSioCtrl[byIdx].byTxStat = SIO_STATE_TX_DNE;
				
				//call back
				if(g_tSioCtrl[byIdx].send_callback)
					g_tSioCtrl[byIdx].send_callback(ptSioBase);	
			}
			break;
		case SIO_INT_TXDNE:
			csp_sio_clr_isr(ptSioBase, SIO_INT_TXDNE);
			break;
		case SIO_INT_TIMEOUT:
			if(g_tSioCtrl[byIdx].err_callback)
				g_tSioCtrl[byIdx].err_callback(ptSioBase, byIsr);
			csp_sio_clr_isr(ptSioBase, SIO_INT_TIMEOUT);
			break;
		case SIO_INT_BREAK:												//receive break interrupt ,reset receive module
			if(g_tSioCtrl[byIdx].err_callback)
				g_tSioCtrl[byIdx].err_callback(ptSioBase, byIsr);
			csp_sio_clr_isr(ptSioBase, SIO_INT_BREAK);
			break;
		default:
			csp_sio_clr_isr(ptSioBase, SIO_INT_ALL);
			break;
	}
}
/** \brief Init sio tx, Initializes the resources needed for the sio instance 
 * 
 *  \param[in] ptSioBase: pointer of sio register structure
 *  \param[in] ptTxCfg: pointer of sio parameter config structure, \ref csi_sio_tx_config_t
 * 				- byD0Len: D0 send length (1~8)个bit, all 0 sequence(out low(0))
 * 				- byD1Len: D1 send length (1~8)个bit, all 1 sequence(out high(1))
 * 				- byDLLen: DL send length (1~8)个bit
 * 				- byDHLen: DH send length (1~8)个bit
 * 				- byDLLsq: DL data sequence (0~0xff),send sequence bit0 - bitdatahlen
 * 				- byDHHsq: DH data sequence (0~0xff),send sequence bit0 - bitdatallen
 * 				- byTxCnt: sio tx bit count, Mux Num = 256bit(32bytes)
 * 				- byTxBufLen: sio tx buffer length, Max Len = 16bit(2bytes)	
 * 				- eTxDir: sio output dir, LSB OR MSB
 * 				- eIdleLev: idle sio ouput state
 * 				- wTxFreq: sio tx frequency 
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_sio_tx_init(csp_sio_t *ptSioBase, csi_sio_tx_config_t *ptTxCfg)
{
	uint8_t byIdx = apt_get_sio_idx(ptSioBase);
	
	csi_clk_enable((uint32_t *)ptSioBase);		//sio peripheral clk en
	csp_sio_clk_enable(ptSioBase);				//enable clk
	csp_sio_set_wkmode(ptSioBase, SIO_MODE_TX);	//sio tx mode
	
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
	
	//callback init
	g_tSioCtrl[byIdx].send_callback = NULL;
	g_tSioCtrl[byIdx].err_callback = NULL;
	
	return CSI_OK;
}

/** \brief Init sio rx, Initializes the resources needed for the sio instance 
 * 
 *  \param[in] ptSioBase: pointer of sio register structure
 *  \param[in] ptRxCfg: pointer of sio parameter config structure, \ref csi_sio_rx_config_t
 * 				- eTrgEdge：receive samping trigger edge	
 * 				- eTrgMode：receive samping trigger mode	
 * 				- eSampMode：receive samping mode
 * 				- eSampExtra：receive samping extract select
 * 				- eRxDir：sio receive dir, LSB OR MSB
 * 				- byDebPerLen：debounce period length, (1~8)period
 * 				- byDebClkDiv： debounce clk div
 * 				- byHithr：extract high Threshold 
 * 				- bySpBitLen：receive samping one bit count length
 * 				- byRxCnt：sio rx bit count, Mux Num = 256bit(32bytes)
 * 				- byRxBufLen：sio rx buffer length, Max Len = 32bit(4bytes)
 * 				- wRxFreq： sio rx sampingfrequency 
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_sio_rx_init(csp_sio_t *ptSioBase, csi_sio_rx_config_t *ptRxCfg)
{
	uint8_t byIdx = apt_get_sio_idx(ptSioBase);
	
	csi_clk_enable((uint32_t *)ptSioBase);		//sio peripheral clk en
	csp_sio_clk_enable(ptSioBase);				//enable clk
	csp_sio_set_wkmode(ptSioBase, SIO_MODE_RX);	//sio rx mode
	
	if(ptRxCfg->wRxFreq > 1)					//tx clk config
		csp_sio_set_rx_clkdiv(ptSioBase, (csi_get_pclk_freq()/ptRxCfg->wRxFreq - 1));
	else
		return CSI_ERROR;
	
	//rx receive config
	csp_sio_set_rx_debounce(ptSioBase, ptRxCfg->byDebLen - 1, ptRxCfg->byDebClkDiv - 1);													//set rx sampling debounce 
	csp_sio_set_samp_mode(ptSioBase,(sio_bstsel_e)ptRxCfg->eTrgEdge,(sio_trgmode_e)ptRxCfg->eTrgMode,(sio_rmode_e)ptRxCfg->eSampMode);		//set rx samping mode
	csp_sio_set_samp_extract(ptSioBase, (sio_extract_e)ptRxCfg->eSampExtra, SIO_ALIGN_EN, ptRxCfg->bySampBitLen - 1, ptRxCfg->byHiThres);	//set rx samping control
	csp_sio_set_receive(ptSioBase, (sio_rdir_e)ptRxCfg->eRxDir, ptRxCfg->byRxBufLen - 1, ptRxCfg->byRxCnt - 1);								//set receive para
	
	//callback init
	g_tSioCtrl[byIdx].send_callback = NULL;
	g_tSioCtrl[byIdx].err_callback = NULL;
	
	return CSI_OK;
}
/** \brief sio receive break reset config
 * 
 *  \param[in] ptSioBase: pointer of sio register structure
 *  \param[in] eBkLev: break reset detect level, \ref csi_sio_bklev_e
 *  \param[in] byBkCnt: break reset detect period
 *  \param[in] bEnable: ENABLE/DISABLE break reset and break interrupt
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_sio_set_break(csp_sio_t *ptSioBase, csi_sio_bklev_e eBkLev, uint8_t byBkCnt, bool bEnable)
{
	if(byBkCnt == 0)
		return CSI_ERROR;
		
	csp_sio_set_break(ptSioBase, (sio_breaklel_e)eBkLev, (byBkCnt - 1), bEnable);

	return CSI_OK; 
}

/** \brief sio receive sample timeout reset config
 * 
 *  \param[in] ptSioBase: pointer of sio register structure
 *  \param[in] byToCnt: sample timeout period
 *  \param[in] bEnable: ENABLE/DISABLE sample timeout reset
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_sio_set_timeout(csp_sio_t *ptSioBase, uint8_t byToCnt ,bool bEnable)
{
	if(byToCnt == 0)
		return CSI_ERROR;
		
	csp_sio_set_timeout(ptSioBase, (byToCnt - 1), bEnable);
	
	return CSI_OK; 
}
/** \brief sio work mode set,send(tx)/receive(rx)
 * 
 *  \param[in] ptSioBase: pointer of sio register structure
 *  \param[in] eWorkMd: sio working mode, \ref csi_sio_wkmode_e
 *  \return none
 */
void csi_sio_set_wkmode(csp_sio_t *ptSioBase, csi_sio_wkmode_e eWorkMd)
{
	csp_sio_set_wkmode(ptSioBase, (sio_mode_e)eWorkMd);
}
/** \brief enable/disable sio interrupt 
 * 
 *  \param[in] ptSioBase: pointer of sio register structure
 *  \param[in] eIntSrc: sio interrupt source, \ref csi_sio_intsrc_e
 *  \return none
 */
void csi_sio_int_enable(csp_sio_t *ptSioBase, csi_sio_intsrc_e eIntSrc)
{
	csp_sio_clr_isr(ptSioBase, (sio_int_e)eIntSrc);
	csp_sio_int_enable(ptSioBase, (sio_int_e)eIntSrc);
}
/** \brief enable/disable sio interrupt 
 * 
 *  \param[in] ptSioBase: pointer of sio register structure
 *  \param[in] eIntSrc: sio interrupt source, \ref csi_sio_intsrc_e
 *  \return none
 */
void csi_sio_int_disable(csp_sio_t *ptSioBase, csi_sio_intsrc_e eIntSrc)
{
	csp_sio_int_disable(ptSioBase, (sio_int_e)eIntSrc);
}
/** \brief clear sio interrupt status
 * 
 *  \param[in] ptSioBase: pointer of sio register structure
 *  \param[in] eIntSrc: sio interrupt status, \ref csi_sio_intsrc_e
 *  \return none
 */
void csi_sio_clr_isr(csp_sio_t *ptSioBase, csi_sio_intsrc_e eIntSrc)
{
	csp_sio_clr_isr(ptSioBase, (sio_int_e)eIntSrc);
}
/** \brief send data from sio, this function is polling mode(sync mode)
 * 
 * \param[in] ptSioBase: pointer of sio register structure
 * \param[in] pwData: pointer to buffer with data to send 
 * \param[in] hwSize: send data size
 * \return error code \ref csi_error_t or send data size
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
		while(!(csp_sio_get_risr(ptSioBase) & SIO_INT_TXBUFEMPT));
	}
	while(!(csp_sio_get_risr(ptSioBase) & SIO_INT_TXDNE));
	csp_sio_clr_isr(ptSioBase, SIO_INT_TXDNE);
	
	return i;
}

/** \brief send data from sio, this function is polling mode(sync mode)
 * 
 * \param[in] ptSioBase: pointer of sio register structure
 * \param[in] pwSend: pointer to buffer with data to send 
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
	csp_sio_int_enable(ptSioBase, SIO_INT_TXBUFEMPT);
	
	return CSI_OK;
}
/** \brief sio receive data,, use rxbuffull interrupt 
 * 
 * \param[in] ptSioBase: pointer of sio register structure
 * \param[in] pwRecv: pointer of sio receive data
 * \param[in] hwSize: receive data size
 * \return error code \ref csi_error_t or receive data len
 */
csi_error_t csi_sio_receive_rxfull_int(csp_sio_t *ptSioBase, uint32_t *pwRecv, uint16_t hwSize)
{
	uint8_t byIdx = apt_get_sio_idx(ptSioBase);
	
	if(NULL == pwRecv || hwSize == 0)
		return CSI_ERROR;
	
	g_tSioCtrl[byIdx].pwData	= pwRecv;
	g_tSioCtrl[byIdx].hwSize 	= hwSize;
	g_tSioCtrl[byIdx].hwTransNum = 0;
	g_tSioCtrl[byIdx].byRxStat  = SIO_STATE_RECV;
	
	csp_sio_clr_isr(ptSioBase, SIO_INT_RXBUFFULL);
	csp_sio_int_enable(SIO0, SIO_INT_RXBUFFULL);
	
	return CSI_OK;
}

/** \brief sio receive data, use rxdne interrupt 
 * 
 * \param[in] ptSioBase: pointer of sio register structure
 * \param[in] pwRecv: pointer of sio receive data
 * \param[in] hwSize: receive data size
 * \return error code \ref csi_error_t or receive data len
 */
csi_error_t csi_sio_receive_rxdne_int(csp_sio_t *ptSioBase, uint32_t *pwRecv, uint16_t hwSize)
{
	uint8_t byIdx = apt_get_sio_idx(ptSioBase);
	
	if(NULL == pwRecv || hwSize == 0)
		return CSI_ERROR;
	
	g_tSioCtrl[byIdx].pwData	= pwRecv;
	g_tSioCtrl[byIdx].hwSize 	= hwSize;
	g_tSioCtrl[byIdx].hwTransNum = 0;
	g_tSioCtrl[byIdx].byRxStat  = SIO_STATE_RECV;
	
	csp_sio_clr_isr(ptSioBase, SIO_INT_RXDNE);
	csp_sio_int_enable(SIO0, SIO_INT_RXDNE);
	
	return CSI_OK;
}

/** \brief sio send dma enable
 * 
 *  \param[in] ptSioBase: pointer of sio register structure
 *  \return  none
 */
void csi_sio_send_dma_enable(csp_sio_t *ptSioBase)
{
	csp_sio_txdma_enable(ptSioBase);
}
/** \brief sio receive dma enable
 * 
 *  \param[in] ptSioBase: pointer of sio register structure
 *  \return  none
 */
void csi_sio_receive_dma_enable(csp_sio_t *ptSioBase)
{
	csp_sio_rxdma_enable(ptSioBase);
}
/** \brief send data from sio, this function is dma mode
 * 
 *  \param[in] ptSioBase: pointer of sio register structure
 *  \param[in] ptDmaBase: pointer of dma register structure
 *  \param[in] eDmaCh: channel number of dma, \ref csi_dma_ch_e
 *  \param[in] pData: pointer to buffer with data to send to sio transmitter.
 *  \param[in] hwSize: number of data to send (byte); hwSize <= 0xfff
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_sio_send_dma(csp_sio_t *ptSioBase, csp_dma_t *ptDmaBase, csi_dma_ch_e eDmaCh, const void *pData, uint16_t hwSize)
{
	if(hwSize > 0xfff)
		return CSI_ERROR;
		
	csi_dma_ch_start(ptDmaBase, eDmaCh, (void *)pData, (void *)&(ptSioBase->TXBUF), hwSize, 1);
	
	return CSI_OK;
}
/** \brief send data from sio, this function is dma mode
 * 
 *  \param[in] ptSioBase: pointer of sio register structure
 *  \param[in] ptDmaBase: pointer of dma register structure
 *  \param[in] eDmaCh: channel number of dma, \ref csi_dma_ch_e
 *  \param[in] pData: pointer to buffer with data to send to sio transmitter.
 *  \param[in] hwSize: number of data to send (byte); hwSize <= 0xfff
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_sio_recv_dma(csp_sio_t *ptSioBase, csp_dma_t *ptDmaBase, csi_dma_ch_e eDmaCh, const void *pData, uint16_t hwSize)
{
	if(hwSize > 0xfff)
		return CSI_ERROR;
		
	csi_dma_ch_start(ptDmaBase, eDmaCh, (void *)&(ptSioBase->RXBUF), (void *)pData, hwSize, 1);
	
	return CSI_OK;
}

