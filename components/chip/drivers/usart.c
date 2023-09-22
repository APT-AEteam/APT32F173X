/***********************************************************************//** 
 * \file  usart.c
 * \brief  csi usart driver
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-8-03 <td>V0.0  <td>ZJY   <td>initial
 * <tr><td> 2023-9-22 <td>V0.1  <td>GQQ   <td>fix bug,code normalization
 * </table>
 * *********************************************************************
*/

#include "tick.h"
#include "drv/usart.h"

/* Private macro------------------------------------------------------*/
/* externs function---------------------------------------------------*/
/* externs variablesr-------------------------------------------------*/
/* Private variablesr-------------------------------------------------*/
csi_usart_ctrl_t g_tUsartCtrl[USART_IDX];	

/** \brief get usart idx 
 * 
 *  \param[in] ptUsartBase: pointer of usart register structure
 *  \return usart id number(0~1) or error(0xff)
 */ 
static uint8_t apt_get_usart_idx(csp_usart_t *ptUsartBase)
{
	switch((uint32_t)ptUsartBase)
	{
		case APB_USART0_BASE:
			return 0;
		case APB_USART1_BASE:
			return 1;		
		default:
			return 0xff;		//error
	}
}

/** \brief initialize usart parameter structure
 * 
 *  \param[in] ptUsartBase: pointer of usart register structure
 *  \param[in] ptUartCfg: pointer of usart parameter config structure
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_usart_init(csp_usart_t *ptUsartBase, csi_usart_config_t *ptUsartCfg)
{
	usart_par_e eParity = US_PAR_NONE;
	uint8_t byClkDiv = 1;
	
	
	if(ptUsartCfg->wBaudRate == 0)						//Baud
		return CSI_ERROR;
	
	csi_clk_enable((uint32_t *)ptUsartBase);			//usart peripheral clk enable
	csp_usart_clk_en(ptUsartBase);						//usart clk enable
	csp_usart_sw_rst(ptUsartBase);
	csp_usart_rxfifo_sw_rst(ptUsartBase);
	csp_usart_txfifo_sw_rst(ptUsartBase);
	
	csp_usart_set_ckdiv(ptUsartBase, ptUsartCfg->byClkSrc);			//clk source
	csp_usart_set_mode(ptUsartBase, ptUsartCfg->byMode, US_NORMAL);	//work mode normal
	
	//set dadabits/stopbits/datalen
	switch(ptUsartCfg->byParity)					
	{
		case USART_PARITY_NONE:
			eParity = US_PAR_NONE;
			break;
		case USART_PARITY_ODD:
			eParity = US_PAR_ODD;
			break;
		case USART_PARITY_EVEN:
			eParity = US_PAR_EVEN; 
			break;
		case USART_PARITY_ZERO:
			eParity = US_PAR_SPACE;
			break;
		case USART_PARITY_ONE:
			eParity = US_PAR_MARK;
			break;
		case USART_PARITY_MD:
			eParity = US_PAR_MULTI_DROP;
			break;
		default:
			return CSI_ERROR;
	}
	csp_usart_set_format(ptUsartBase, ptUsartCfg->byDatabit, eParity, ptUsartCfg->byStopbit);		
	
	//set baudrate
	if(ptUsartCfg->byClkSrc != US_CLK_CK0 ) // Select CK input as usart clock ,then the baud rate setting is meanless
	{
		if(csp_usart_get_clks(ptUsartBase) == US_CLK_DIV8)
			byClkDiv = 8;
		else 
			byClkDiv = 1;


		if(csp_usart_get_mode(ptUsartBase) == US_ASYNC)
			csp_usart_set_brdiv(ptUsartBase, ptUsartCfg->wBaudRate, csi_get_pclk_freq()/byClkDiv);
		else 
			csp_usart_set_brdiv(ptUsartBase, ptUsartCfg->wBaudRate, (csi_get_pclk_freq() << 4) /byClkDiv);
	}
	
	if((ptUsartCfg->eRxFifoTrg != USART_RXFIFOTRG_ONE) && (ptUsartCfg->eRxFifoTrg != USART_RXFIFOTRG_TWO) && (ptUsartCfg->eRxFifoTrg != USART_RXFIFOTRG_FOUR))
		ptUsartCfg->eRxFifoTrg = USART_RXFIFOTRG_ONE;
		
	csp_usart_set_fifo(ptUsartBase, (usart_rxfifo_e)ptUsartCfg->eRxFifoTrg, US_FIFO_EN);

	if(ptUsartCfg -> hwRecvTo > 0)	
	{
		csp_usart_set_rtor(ptUsartBase, ptUsartCfg->hwRecvTo);			//set receive timeover time
//		csp_usart_clr_isr(ptUsartBase, US_RXTO_INT_S);
		csp_usart_rtor_enable(ptUsartBase);
	}

	return CSI_OK;
}

/** \brief  register usart interrupt callback function
 * 
 *  \param[in] ptUsartBase: pointer of uart register structure
 *  \param[in] eCallBkId: usart interrupt callback type, \ref csi_usart_callback_id_e
 *  \param[in] callback: usart interrupt handle function
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_usart_register_callback(csp_usart_t *ptUsartBase, csi_usart_callback_id_e eCallBkId, void  *callback)
{
	uint8_t byIdx = apt_get_usart_idx(ptUsartBase);
	if(byIdx == 0xff)
		return CSI_ERROR;

	switch(eCallBkId)
	{
		case USART_CALLBACK_ID_RECV:
			g_tUsartCtrl[byIdx].recv_callback = callback;
			break;
		case USART_CALLBACK_ID_SEND:
			g_tUsartCtrl[byIdx].send_callback = callback;
			break;
		case USART_CALLBACK_ID_ERR:
			g_tUsartCtrl[byIdx].err_callback = callback;
			break;
		default:
			return CSI_ERROR;
	}
	return CSI_OK;
}

/** \brief USART中断函数，接收数据使用中断方式(FIFO/RX两种中断)，在此中断函数中接收数据
 * 
 *  \param[in] ptUsartBase: pointer of usart register structure
 *  \param[in] byIdx: usart id number(0)
 */
void csi_usart_irqhandler(csp_usart_t *ptUsartBase,uint8_t byIdx)
{
	switch(csp_usart_get_isr(ptUsartBase) & 0x5101)								//get rxfifo/tx/txfifo/rxtimeout interrupt status
	{
		case US_INT_RXFIFO:														//rxfifo int			
			
			while(csp_usart_get_sr(ptUsartBase) & US_RNE)						//rxfifo not empty
			{
				if(g_tUsartCtrl[byIdx].hwTransNum < g_tUsartCtrl[byIdx].hwRxSize)
					g_tUsartCtrl[byIdx].pbyRxBuf[g_tUsartCtrl[byIdx].hwTransNum ++] = csp_usart_get_data(ptUsartBase);		//get data
				else															//RX completed										
				{
					csp_usart_rxfifo_sw_rst(ptUsartBase);							//reset rxfifo
					csp_usart_int_disable(ptUsartBase, US_INT_RXFIFO);			//disable rxfifo int 
					csp_usart_set_rtor(ptUsartBase,0);							//disable time over by setting OVER_TIME to 0.
					g_tUsartCtrl[byIdx].hwTransNum = 0;							//clear rx counter
					g_tUsartCtrl[byIdx].byRxState = USART_EVENT_RX_DNE;			//rx completed
					
					//callback,send rx done event
					if(g_tUsartCtrl[byIdx].recv_callback)
						g_tUsartCtrl[byIdx].recv_callback(ptUsartBase, USART_EVENT_RX_DNE, g_tUsartCtrl[byIdx].pbyRxBuf, &g_tUsartCtrl[byIdx].hwRxSize);
					
//					csp_usart_clr_isr(ptUsartBase, US_RXTO_INT_S);
					csp_usart_rtor_enable(ptUsartBase);					//enable timeout int
				}
			}
			break;
			
		case US_INT_RXTO:				
			if(g_tUsartCtrl[byIdx].byRxState != USART_EVENT_RX_DNE)
			{
				while(csp_usart_get_sr(ptUsartBase) & US_RNE)
				{
					g_tUsartCtrl[byIdx].pbyRxBuf[g_tUsartCtrl[byIdx].hwTransNum ++] = csp_usart_get_data(ptUsartBase);
				}
				
				if(g_tUsartCtrl[byIdx].hwTransNum  < g_tUsartCtrl[byIdx].hwRxSize)	//rx handle
				{
					g_tUsartCtrl[byIdx].byRxState = USART_EVENT_RX_TO;				//timeout state
				
					//callback，send rx timeout event 
					if(g_tUsartCtrl[byIdx].recv_callback)							//user callback
						g_tUsartCtrl[byIdx].recv_callback(ptUsartBase, USART_EVENT_RX_TO, g_tUsartCtrl[byIdx].pbyRxBuf, &g_tUsartCtrl[byIdx].hwTransNum);
				}
				else																//user callback,rx completed
				{
					g_tUsartCtrl[byIdx].hwTransNum = 0;								//clear rx counter
					g_tUsartCtrl[byIdx].byRxState = USART_EVENT_RX_DNE;				//rx completed state
					
					//callback,send rx done event
					if(g_tUsartCtrl[byIdx].recv_callback)
						g_tUsartCtrl[byIdx].recv_callback(ptUsartBase, USART_EVENT_RX_DNE, g_tUsartCtrl[byIdx].pbyRxBuf, &g_tUsartCtrl[byIdx].hwRxSize);
				}
			}
			csp_usart_clr_isr(ptUsartBase, US_INT_RXTO_S);							//clear interrupt
			csp_usart_rtor_enable(ptUsartBase);	
			break;
			
		case US_INT_TXFIFO:					
			csp_usart_set_data(ptUsartBase, *g_tUsartCtrl[byIdx].pbyTxBuf);			//send data
			g_tUsartCtrl[byIdx].hwTxSize --;
			g_tUsartCtrl[byIdx].pbyTxBuf ++;
			
			if(g_tUsartCtrl[byIdx].hwTxSize == 0)	
			{	
				g_tUsartCtrl[byIdx].byTxState = USART_EVENT_TX_DNE;					//send completed
				csp_usart_int_disable(ptUsartBase, US_INT_TXFIFO);					//disable interrupt
				
				//callback, send data completed
				if(g_tUsartCtrl[byIdx].send_callback)
					g_tUsartCtrl[byIdx].send_callback(ptUsartBase);
			}
			break;

		default:
			csp_usart_clr_isr(ptUsartBase, US_INT_ALL_S);
			break;
	}
}
/** \brief enable/disbale receive fifo function
 * 
 *  \param[in] ptSioBase: pointer of uart register structure
 *  \param[in] bEnable: enable/disable receive fifo function
 *  \return none
 */
void csi_usart_rxfifo_enable(csp_usart_t *ptUsartBase, bool bEnable)
{
	if(bEnable)
		csp_usart_fifo_enable(ptUsartBase);
	else
		csp_usart_fifo_disable(ptUsartBase);
}

/** \brief enable usart interrupt 
 * 
 *  \param[in] ptSioBase: pointer of usart register structure
 *  \param[in] eIntSrc: usart interrupt source
 *  \return none
 */
void csi_usart_int_enable(csp_usart_t *ptUsartBase, csi_usart_intsrc_e eIntSrc)
{
	csp_usart_clr_isr(ptUsartBase, (usart_int_e)eIntSrc);
	csp_usart_int_enable(ptUsartBase, (usart_int_e)eIntSrc);
}

/** \brief disable usart interrupt 
 * 
 *  \param[in] ptSioBase: pointer of usart register structure
 *  \param[in] eIntSrc: usart interrupt source
 *  \return none
 */
void csi_usart_int_disable(csp_usart_t *ptUsartBase, csi_usart_intsrc_e eIntSrc)
{
	csp_usart_int_disable(ptUsartBase, (usart_int_e)eIntSrc);
}

/** \brief clear usart interrupt status
 * 
 *  \param[in] ptUsartBase: pointer of usart register structure
 *  \param[in] eIntSta: usart interrupt status, \ref csi_usart_intsta_e
 *  \return none
 */
void csi_usart_clr_isr(csp_usart_t *ptUsartBase, csi_usart_intsta_e eIntSta)
{
	csp_usart_clr_isr(ptUsartBase, (usart_isr_e)eIntSta);
}
/** \brief start(enable) usart rx/tx
 * 
 *  \param[in] ptUsartBase: pointer of usart register structure
 *  \param[in] eFunc: rx/tx function, \ref csi_usart_func_e
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_usart_start(csp_usart_t *ptUsartBase, csi_usart_func_e eFunc)
{
	switch(eFunc)
	{
		case USART_FUNC_RX:
			csp_usart_rx_enable(ptUsartBase);				//enable RX
			break;
		case USART_FUNC_TX:
			csp_usart_tx_enable(ptUsartBase);				//enable TX
			break;
		case USART_FUNC_RX_TX:
			csp_usart_rx_enable(ptUsartBase);
			csp_usart_tx_enable(ptUsartBase);		
			break;
		default:
			return CSI_ERROR;
	}
	return CSI_OK;
	
}
/** \brief stop(disable) usart rx/tx
 * 
 *  \param[in] ptUsartBase: pointer of usart register structure
 *  \param[in] eFunc: rx/tx function, \ref csi_usart_func_e
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_usart_stop(csp_usart_t *ptUsartBase, csi_usart_func_e eFunc)
{
	switch(eFunc)
	{
		case USART_FUNC_RX:
			csp_usart_rx_disable(ptUsartBase);	 				//disable RX
			break;
		case USART_FUNC_TX:
			csp_usart_tx_disable(ptUsartBase);				//disable TX
			break;
		case USART_FUNC_RX_TX:
			csp_usart_rx_disable(ptUsartBase);		//disable TX/RX
			csp_usart_tx_disable(ptUsartBase);
			break;
		default:
			return CSI_ERROR;
	}
	return CSI_OK;
}

/** \brief enable(disable) usart clkout
 * 
 *  \param[in] ptUsartBase: pointer of usart register structure
 *  \param[in] bEnable: ENABLE/DISABLE
 *  \return none
 */ 
void csi_usart_clkout(csp_usart_t *ptUsartBase, bool bEnable)
{
	if(bEnable)
	{
		csp_usart_clko_enable(ptUsartBase);
	}
	else 
	{
		csp_usart_clko_disable(ptUsartBase);
	}
}
/** \brief usart send character
 * 
 *  \param[in] ptUsartBase: pointer of usart register structure
 *  \param[in] byData: the character to be send
 *  \return  none
 */
void csi_usart_putc(csp_usart_t *ptUsartBase, uint8_t byData)
{
	while(!(csp_usart_get_sr(ptUsartBase) & US_INT_TXRDY));
	csp_usart_set_data(ptUsartBase, byData);				
	
}
/** \brief usart get character
 * 
 *  \param[in] ptUsartBase: pointer of usart register structure
 *  \return  the character to get
 */
uint8_t csi_usart_getc(csp_usart_t *ptUsartBase)
{
	while(!(csp_usart_get_sr(ptUsartBase) & US_RNE));
	return csp_usart_get_data(ptUsartBase);	
}

/** \brief usart send data with polling mode.
 * 
 *  \param[in] ptUsartBase: pointer of usart register structure
 *  \param[in] pData: pointer to buffer with data to send by usart transmitter.
 *  \param[in] hwSize: number of data to send (byte).
 *  \return  the num of data which is send successfully or CSI_ERROR
 */
int16_t csi_usart_send(csp_usart_t *ptUsartBase, const void *pData, uint16_t hwSize)
{
	int32_t i; 
	uint8_t *pbySend = (uint8_t *)pData;
	
	if(NULL == pData || 0 == hwSize)
		return CSI_ERROR;
		
	for(i = 0; i < hwSize; i++)
	{
		while(!(csp_usart_get_sr(ptUsartBase) & US_TNF));
		csp_usart_set_data(ptUsartBase, *(pbySend+i));	
	}
	return i;
}


/** \brief receive data from usart, this function is polling.
 * 
 *  \param[in] ptUsartBase: pointer of usart register structure
 *  \param[in] pData: pointer to buffer with data to be received.
 *  \param[in] hwSize: number of data to receive (byte).
 *  \param[in] wTimeOut: the timeout between bytes(ms), unit: ms 
 *  \return  the num of data which is receive successfully
 */
int32_t csi_usart_receive(csp_usart_t *ptUsartBase, void *pData, uint16_t hwSize, uint32_t wTimeOut)
{
	uint8_t *pbyRecv = (uint8_t *)pData;
 	volatile uint16_t hwRecvNum = 0;
	
	csp_usart_rxfifo_sw_rst(ptUsartBase);								//reset rxfifo，clear rxfifo
	
	if(NULL == pData)
		return CSI_ERROR;
		
	if(wTimeOut)				//handle with wTimeOut
	{
		uint32_t wRecvStart = csi_tick_get_ms();	
		while(hwRecvNum < hwSize)
		{
			while(!(csp_usart_get_sr(ptUsartBase) & US_RNE))		//fifo empty? wait	
			{
				if((csi_tick_get_ms() - wRecvStart) >= wTimeOut) 
					return hwRecvNum;
			}
			pbyRecv[hwRecvNum ++] = csp_usart_get_data(ptUsartBase);
			wRecvStart = csi_tick_get_ms();	
		}
	}
	else						//handle without wTimeOut
	{
		while(hwRecvNum < hwSize)
		{
			while(!(csp_usart_get_sr(ptUsartBase) & US_RNE));		//fifo empty? wait	
			pbyRecv[hwRecvNum ++] = csp_usart_get_data(ptUsartBase);
		}
	}
	
	return hwRecvNum;
}

/** \brief usart send data with int mode.
 * 
 *  \param[in] ptUsartBase: pointer of usart register structure
 *  \param[in] pData: pointer to buffer with data to send by usart transmitter.
 *  \param[in] hwSize: number of data to send (byte).
 *  \return   CSI_ERROR/CSI_OK
 */
int16_t csi_usart_send_int(csp_usart_t *ptUsartBase, const void *pData, uint16_t hwSize)
{
	uint8_t byIdx = apt_get_usart_idx(ptUsartBase);
	
	if(hwSize == 0 || byIdx >= USART_IDX || g_tUsartCtrl[byIdx].byTxState == USART_EVENT_SEND) 
		return CSI_ERROR;
				
	g_tUsartCtrl[byIdx].pbyTxBuf =(uint8_t *)pData;
	g_tUsartCtrl[byIdx].hwTxSize = hwSize;
	g_tUsartCtrl[byIdx].byTxState = USART_EVENT_SEND;				//set usart send status, sending
	
	csp_usart_int_enable(ptUsartBase, (usart_int_e)US_INT_TXFIFO);	//enable usart txfifo interrupt
	
	return CSI_OK;
}

/** \brief receive data from usart, this function is interrupt receive
 * 
 *  \param[in] ptUsartBase: pointer of usart register structure
 *  \param[in] pData: pointer to buffer with data to be received.
 *  \param[in] hwSize: number of data to receive (byte).
 *  \return  error code \ref csi_error_t
 */	
csi_error_t csi_usart_receive_int(csp_usart_t *ptUsartBase, void *pData, uint16_t hwSize)
{
	uint8_t byIdx = apt_get_usart_idx(ptUsartBase);
	
	if((NULL == pData) || (0 == hwSize) || (g_tUsartCtrl[byIdx].byTxState == USART_EVENT_RECV)) 
		return CSI_ERROR;
		
	g_tUsartCtrl[byIdx].pbyRxBuf = (uint8_t *)pData;
	g_tUsartCtrl[byIdx].hwRxSize = hwSize;
	g_tUsartCtrl[byIdx].hwTransNum = 0;
	g_tUsartCtrl[byIdx].byRxState = USART_EVENT_RECV;
	
	csp_usart_rxfifo_sw_rst(ptUsartBase);									//reset rxfifo
//	csp_usart_clr_isr(ptUsartBase, US_RXTO_INT_S);
	csp_usart_rtor_enable(ptUsartBase);								//enable rto
	csp_usart_int_enable(ptUsartBase, US_INT_RXFIFO | US_INT_RXTO);		//rxfifo and receive timeout int
	
	return CSI_OK;
}

/** \brief usart dma send mode set
 * 
 *  \param[in] ptUsartBase: pointer of usart register structure
 *  \param[in] eTxDmaMode: usart dma tx mode \ref csi_usdma_txfifo_md_e
 *  \param[in] bEnable: ENABLE/DISABLE
 *  \return  error code \ref csi_error_t
 */
void csi_usart_set_send_dma(csp_usart_t *ptUsartBase, csi_usdma_txfifo_md_e eTxDmaMode ) 
{
	csp_usart_set_txdma(ptUsartBase, (usart_tdma_md_e)eTxDmaMode, ENABLE); 
}

/** \brief usart dma rx mode set
 * 
 *  \param[in] ptUsartBase: pointer of usart register structure
 *  \param[in] eRxDmaMode: usart dma tx mode \ref csi_usdma_rxfifo_md_e
 *  \param[in] bEnable: ENABLE/DISABLE
 *  \return  error code \ref csi_error_t
 */
void csi_usart_set_recieve_dma(csp_usart_t *ptUsartBase, csi_usdma_rxfifo_md_e eRxDmaMode ) 
{
	csp_usart_set_rxdma(ptUsartBase, (usart_rdma_md_e)eRxDmaMode,ENABLE); 
}

/** \brief send data from usart, this function is dma transfer
 * 
 *  \param[in] ptUartBase: pointer of usart register structure
 *  \param[in] ptDmaBase: pointer of dma register structure
 *  \param[in] pData: pointer to buffer with data to send to usart transmitter.
 *  \param[in] hwSize: number of data to send (byte), hwSize <= 0xfff.
 *  \return  error code \ref csi_error_t
 */
csi_error_t csi_usart_send_dma(csp_usart_t *ptUsartBase, const void *pData, uint8_t byDmaCh, uint16_t hwSize)
{
	if(hwSize > 0xfff)
		return CSI_ERROR;
	csi_dma_ch_start(DMA0, byDmaCh, (void *)pData, (void *)&(ptUsartBase->THR), hwSize, 1);
	
	return CSI_OK;
}

/** 
  \brief 	   receive data to usart transmitter, this function is dma mode
  \param[in]   ptUsartBase	pointer of usart register structure
  \param[in]   ptDmaBase: pointer of dma register structure
  \param[in]   pData		pointer to buffer with data to send to usart transmitter.
  \param[in]   wSize		number of data to send (byte), hwSize <= 0xfff.
  \return      error code \ref csi_error_t
 */
csi_error_t csi_usart_recv_dma(csp_usart_t *ptUsartBase, void *pData, uint8_t byDmaCh, uint16_t hwSize)
{
	if(hwSize > 0xfff)
		return CSI_ERROR;
	csi_dma_ch_start(DMA0, byDmaCh, (void *)&(ptUsartBase->RHR), (void *)pData, hwSize, 1);
	
	return CSI_OK;
}

/** \brief get usart receive/send complete message and (Do not) clear message
 * 
 *  \param[in] ptUsartBase: pointer of usart reg structure.
 *  \param[in] eMode: tx or rx mode
 *  \param[in] bClrEn: clear usart receive/send complete message enable; ENABLE: clear , DISABLE: Do not clear 
 *  \return  bool type true/false
 */ 
bool csi_usart_get_msg(csp_usart_t *ptUsartBase, csi_usart_wkmode_e eWkMode, bool bClrEn)
{
	uint8_t byIdx = apt_get_usart_idx(ptUsartBase);
	bool bRet = false;
	
	switch(eWkMode)
	{
		case USART_SEND:
			if(g_tUsartCtrl[byIdx].byTxState == USART_EVENT_TX_DNE)
			{
				if(bClrEn)
					g_tUsartCtrl[byIdx].byTxState = USART_EVENT_IDLE;		//clear send status
				bRet = true;
			}
			break;
		case USART_RECV:
			if(g_tUsartCtrl[byIdx].byRxState == USART_EVENT_RX_DNE)
			{
				if(bClrEn)
					g_tUsartCtrl[byIdx].byRxState = USART_EVENT_IDLE;		//clear receive status
				bRet = true;
			}
			break;
		case USART_RTO:
			if(g_tUsartCtrl[byIdx].byTxState == USART_EVENT_RX_TO)
			{
				if(bClrEn)
					g_tUsartCtrl[byIdx].byRxState = USART_EVENT_IDLE;		//clear receive status
				bRet = true;
			}
			break;
		default:
			break;
	}
	
	return bRet;
}
/** \brief clr usart receive/send status message (set usart recv/send status idle) 
 * 
 *  \param[in] ptUsartBase: pointer of usart reg structure.
 *  \param[in] eMode: tx or rx mode
 *  \return none
 */ 
void csi_usart_clr_msg(csp_usart_t *ptUsartBase, csi_usart_wkmode_e eMode)
{
	uint8_t byIdx = apt_get_usart_idx(ptUsartBase);
	
	if(eMode == USART_SEND)
		g_tUsartCtrl[byIdx].byTxState = USART_EVENT_IDLE;		//clear send status
	else
		g_tUsartCtrl[byIdx].byTxState = USART_EVENT_IDLE;		//clear receive status
}