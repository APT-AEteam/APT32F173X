/***********************************************************************//** 
 * \file  uart.c
 * \brief  csi uart driver
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author	<th>Description
 * <tr><td> 2020-8-12 <td>V0.0 <td>ZJY   	<td>initial
 * </table>
 * *********************************************************************
*/
#include <drv/uart.h>
#include <drv/tick.h>

/* private macro------------------------------------------------------*/
/* externs function---------------------------------------------------*/
/* global variablesr--------------------------------------------------*/
csi_uart_ctrl_t g_tUartCtrl[UART_IDX];	

/* private variablesr-------------------------------------------------*/


//static uint32_t s_wCtrlRegBack = 0;	

/** \brief get uart idx 
 * 
 *  \param[in] ptUartBase: pointer of uart register structure
 *  \return uart id number(0~2) or error(0xff)
 */ 
static uint8_t apt_get_uart_idx(csp_uart_t *ptUartBase)
{
	switch((uint32_t)ptUartBase)
	{
		case APB_UART0_BASE:
			return 0;
		case APB_UART1_BASE:
			return 1;
		case APB_UART2_BASE:
			return 2;
		default:
			return 0xff;		//error
	}
}
/** \brief initialize uart parameter structure
 * 
 *  \param[in] ptUartBase: pointer of uart register structure
 *  \param[in] ptUartCfg: pointer of uart parameter config structure
 * 			   - wBaudRate: baud rate
 * 			   - hwRecvTo: rx byte timeout
 * 			   - eParity: parity bit, \ref csi_uart_rxfifo_trg_e
 * 			   - eRxFifoTrg: rx FIFO level to trigger UART_RNE interrupt, \ref csi_uart_rxfifo_trg_e
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_uart_init(csp_uart_t *ptUartBase, csi_uart_config_t *ptUartCfg)
{
	uart_parity_e eParity = UART_PAR_NONE;
	uint32_t wBrDiv;
	
	csi_clk_enable((uint32_t *)ptUartBase);						//uart peripheral clk enable
	csp_uart_soft_rst(ptUartBase);								//reset ip
	
	wBrDiv = csi_get_pclk_freq()/ptUartCfg->wBaudRate;	
	if(wBrDiv < 16)
		wBrDiv = 16;
	csp_uart_set_brdiv(ptUartBase, wBrDiv);						//set uart baud rate 
	
	//databits = 8/stopbits = 1; fixed, can not be configured 
	if(ptUartCfg->eParity == UART_PARITY_ODD)					//set uart parity bits
		eParity = UART_PAR_ODD;
	else if(ptUartCfg->eParity == UART_PARITY_EVEN)
		eParity = UART_PAR_EVEN;
	else if(ptUartCfg->eParity == UART_PARITY_NONE)
		 eParity = UART_PAR_NONE;
	else
		 return CSI_ERROR;
	csp_uart_set_parity(ptUartBase, eParity);					//parity
	
	if((ptUartCfg->eRxFifoTrg != UART_RXFIFOTRG_ONE) && (ptUartCfg->eRxFifoTrg != UART_RXFIFOTRG_TWO) && (ptUartCfg->eRxFifoTrg != UART_RXFIFOTRG_FOUR))
		ptUartCfg->eRxFifoTrg = UART_RXFIFOTRG_ONE;
	csp_uart_set_fifo(ptUartBase, (uart_fifo_bit_e)ptUartCfg->eRxFifoTrg, ENABLE);	//set fxfifo and enable
	
	if(ptUartCfg->hwRecvTo > 0)	
	{
		csp_uart_set_rtor(ptUartBase, ptUartCfg->hwRecvTo);			//set receive timeout 
		csp_uart_rto_enable(ptUartBase);
	}
	else
		csp_uart_rto_disable(ptUartBase);					
	
	csi_irq_enable((uint32_t *)ptUartBase);							//enable uart vic interrupt		

	return CSI_OK;
}

/** \brief  register uart interrupt callback function
 * 
 *  \param[in] ptUartBase: pointer of uart register structure
 *  \param[in] eCallBkId: uart interrupt callback type, \ref csi_uart_callback_id_e
 *  \param[in] callback: uart interrupt handle function
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_uart_register_callback(csp_uart_t *ptUartBase, csi_uart_callback_id_e eCallBkId, void  *callback)
{
	uint8_t byIdx = apt_get_uart_idx(ptUartBase);
	if(byIdx == 0xff)
		return CSI_ERROR;

	switch(eCallBkId)
	{
		case UART_CALLBACK_RECV:
			g_tUartCtrl[byIdx].recv_callback = callback;
			break;
		case UART_CALLBACK_SEND:
			g_tUartCtrl[byIdx].send_callback = callback;
			break;
		case UART_CALLBACK_ERR:
			g_tUartCtrl[byIdx].err_callback = callback;
			break;
		default:
			return CSI_ERROR;
	}
	return CSI_OK;
}
/** \brief uart interrupt handler function
 * 
 *  \param[in] ptUartBase: pointer of uart register structure
 *  \param[in] byIdx: uart idx(0/1/2)
 *  \return none
 */ 
void csi_uart_irqhandler(csp_uart_t *ptUartBase, uint8_t byIdx)
{
	uint16_t hwIsr = csp_uart_get_isr(ptUartBase) & 0x000670;				//RXFIFO/TXFIFO/RXTO/RXBRAK/PAR_ERR中断状态
	switch(hwIsr)							
	{
		case UART_RXFIFO_INT_S:												//使用RXFIFO中断接收数据
			while(csp_uart_get_sr(ptUartBase) & UART_RNE)					//接收FIFO非空
			{
				if(g_tUartCtrl[byIdx].hwTransNum < g_tUartCtrl[byIdx].hwRxSize)
					g_tUartCtrl[byIdx].pbyRxBuf[g_tUartCtrl[byIdx].hwTransNum ++] = csp_uart_get_data(ptUartBase);		//读数据
				else														//接收完成											
				{
					csp_uart_rxfifo_rst(ptUartBase);						//复位rxfifo
					csp_uart_int_disable(ptUartBase, UART_RXFIFO_INT);		//关闭接收中断 
					csp_uart_rto_disable(ptUartBase);						//关闭接收超时
					g_tUartCtrl[byIdx].hwTransNum = 0;						//清除接收计数
					g_tUartCtrl[byIdx].byRxState = UART_STATE_RX_DNE;		//接收完成标志，即接收指定长度
					
					//回调，接收完用户指定长度数据
					if(g_tUartCtrl[byIdx].recv_callback)
						g_tUartCtrl[byIdx].recv_callback(ptUartBase, UART_STATE_RX_DNE, g_tUartCtrl[byIdx].pbyRxBuf, &g_tUartCtrl[byIdx].hwRxSize);
					
					csp_uart_rto_enable(ptUartBase);						//使能接收超时
				}
			}
			break;
	
		case UART_RXTO_INT_S:												 //接收超时中断
			if(g_tUartCtrl[byIdx].byRxState != UART_STATE_RX_DNE)
			{
				while(csp_uart_get_sr(ptUartBase) & UART_RNE)
				{
					g_tUartCtrl[byIdx].pbyRxBuf[g_tUartCtrl[byIdx].hwTransNum ++] = csp_uart_get_data(ptUartBase);
				}
				
				if(g_tUartCtrl[byIdx].hwTransNum  < g_tUartCtrl[byIdx].hwRxSize)	//接收处理
				{
					g_tUartCtrl[byIdx].byRxState = UART_STATE_RX_TO;				//接收超时标志，即接收到一串字符
				
					//回调，接收完一串字符
					if(g_tUartCtrl[byIdx].recv_callback)									//用户回调
						g_tUartCtrl[byIdx].recv_callback(ptUartBase, UART_STATE_RX_TO, g_tUartCtrl[byIdx].pbyRxBuf, &g_tUartCtrl[byIdx].hwTransNum);
				}
				else																//用户回调,接收完用户指定长度
				{
					g_tUartCtrl[byIdx].hwTransNum = 0;								//清除接收计数
					g_tUartCtrl[byIdx].byRxState = UART_STATE_RX_DNE;				//接收完成标志，即接收到用户指定长度数据
					
					//回调，接收完指定数据
					if(g_tUartCtrl[byIdx].recv_callback)
						g_tUartCtrl[byIdx].recv_callback(ptUartBase, UART_STATE_RX_DNE, g_tUartCtrl[byIdx].pbyRxBuf, &g_tUartCtrl[byIdx].hwRxSize);
				}
			}
			csp_uart_clr_isr(ptUartBase, UART_RXTO_INT_S);							//清除中断标志(状态)
			csp_uart_rto_enable(ptUartBase);										//使能字节接收超时
			
			break;
		case UART_TXFIFO_INT_S:														//TXFIFO中断	
			csp_uart_set_data(ptUartBase, *g_tUartCtrl[byIdx].pbyTxBuf);			//发送数据
			g_tUartCtrl[byIdx].hwTxSize --;
			g_tUartCtrl[byIdx].pbyTxBuf ++;
			
			if(g_tUartCtrl[byIdx].hwTxSize == 0)	
			{	
				g_tUartCtrl[byIdx].byTxState = UART_STATE_TX_DNE;					//发送完成标志
				csp_uart_int_disable(ptUartBase, UART_TXFIFO_INT);					//关闭中断
				
				//回调，发送完成
				if(g_tUartCtrl[byIdx].send_callback)
					g_tUartCtrl[byIdx].send_callback(ptUartBase);
			}
			break;
		case UART_RXBRK_INT_S:
			csp_uart_clr_isr(ptUartBase, UART_RXBRK_INT_S);							//清除中断标志(状态)
			if(g_tUartCtrl[byIdx].err_callback)
				g_tUartCtrl[byIdx].err_callback(ptUartBase, hwIsr);
			break;
		case UART_PARERR_INT_S:
			csp_uart_clr_isr(ptUartBase, UART_PARERR_INT_S);						//清除中断标志(状态)
			if(g_tUartCtrl[byIdx].err_callback)
				g_tUartCtrl[byIdx].err_callback(ptUartBase, hwIsr);
			break;
		default:
			csp_uart_clr_isr(ptUartBase, 0x806ff);					
			break;
	}
}
/** \brief enable/disbale receive timeout function
 * 
 *  \param[in] ptSioBase: pointer of uart register structure
 *  \param[in] bEnable: enable/disable receive timeout function
 *  \return none
 */
void csi_uart_rto_enable(csp_uart_t *ptUartBase, uint16_t hwTimeOut, bool bEnable)
{
	if(bEnable)
		csp_uart_rto_enable(ptUartBase);
	else
		csp_uart_rto_disable(ptUartBase);
}

/** \brief enable/disbale receive fifo function
 * 
 *  \param[in] ptSioBase: pointer of uart register structure
 *  \param[in] bEnable: enable/disable receive fifo function
 *  \return none
 */
void csi_uart_rxfifo_enable(csp_uart_t *ptUartBase, bool bEnable)
{
	if(bEnable)
		csp_uart_fifo_enable(ptUartBase);
	else
		csp_uart_fifo_disable(ptUartBase);
}
/** \brief enable uart interrupt 
 * 
 *  \param[in] ptUartBase: pointer of uart register structure
 *  \param[in] eIntSrc: uart interrupt source, \ref csi_uart_intsrc_e
 *  \param[in] bEnable: enable interrupt
 *  \return none
 */
void csi_uart_int_enable(csp_uart_t *ptUartBase, csi_uart_intsrc_e eIntSrc)
{
	csp_uart_int_enable(ptUartBase, (uart_int_e)eIntSrc);
}
/** \brief disable uart interrupt 
 * 
 *  \param[in] ptUartBase: pointer of uart register structure
 *  \param[in] eIntSrc: uart interrupt source, \ref csi_uart_intsrc_e
 *  \param[in] bEnable: disable interrupt
 *  \return none
 */
void csi_uart_int_disable(csp_uart_t *ptUartBase, csi_uart_intsrc_e eIntSrc)
{
	csp_uart_int_disable(ptUartBase, (uart_int_e)eIntSrc);
}
/** \brief start(enable) uart rx/tx
 * 
 *  \param[in] ptUartBase: pointer of uart register structure
 *  \param[in] eFunc: rx/tx function, \ref csi_uart_func_e
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_uart_start(csp_uart_t *ptUartBase, csi_uart_func_e eFunc)
{
	switch(eFunc)
	{
		case UART_FUNC_RX:
			ptUartBase->CTRL |= (UART_RX_EN << UART_RX_POS);
			break;
		case UART_FUNC_TX:
			ptUartBase->CTRL |= UART_TX_EN;
			break;
		case UART_FUNC_RX_TX:
			ptUartBase->CTRL |= (UART_TX_EN | (UART_RX_EN << UART_RX_POS));
			break;
		default:
			return CSI_ERROR;
	}
	return CSI_OK;
}
/** \brief stop(disable) uart rx/tx
 * 
 *  \param[in] ptUartBase: pointer of uart register structure
 *  \param[in] eFunc: rx/tx function, \ref csi_uart_func_e
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_uart_stop(csp_uart_t *ptUartBase, csi_uart_func_e eFunc)
{
	switch(eFunc)
	{
		case UART_FUNC_RX:
			ptUartBase->CTRL &= ~UART_RX_MSK;
			break;
		case UART_FUNC_TX:
			ptUartBase->CTRL &= ~UART_TX_MSK ;
			break;
		case UART_FUNC_RX_TX:
			ptUartBase->CTRL &= ~(UART_TX_MSK | UART_RX_MSK);
			break;
		default:
			return CSI_ERROR;
	}
	return CSI_OK;
}

/** \brief uart send character
 * 
 *  \param[in] ptUartBase: pointer of uart register structure
 *  \param[in] byData: the character to be send
 *  \return  none
 */
void csi_uart_putc(csp_uart_t *ptUartBase, uint8_t byData)
{
	while((csp_uart_get_sr(ptUartBase) & UART_TX_FULL));		
	csp_uart_set_data(ptUartBase, byData);
}
/** \brief uart get character
 * 
 *  \param[in] uart: pointer of uart register structure
 *  \return  the character to get
 */
uint8_t csi_uart_getc(csp_uart_t *ptUartBase)
{
	while(!(csp_uart_get_sr(ptUartBase) & UART_RNE));
	return csp_uart_get_data(ptUartBase);
}

/** \brief send data from uart, this function is no interrup(polling).
 * 
 *  \param[in] ptUartBase: pointer of uart register structure
 *  \param[in] pData: pointer to buffer with data to send to uart transmitter.
 *  \param[in] hwSize: number of data to send (byte).
 *  \return  the num of data which is send successfully or CSI_ERROR/CSI_OK
 */
int32_t csi_uart_send(csp_uart_t *ptUartBase, const void *pData, uint16_t hwSize)
{
	int32_t i; 
	uint8_t *pbySend = (uint8_t *)pData;
	
	if(NULL == pData || 0 == hwSize)
		return -1;
	
	for(i = 0; i < hwSize; i++)
	{
		while((csp_uart_get_sr(ptUartBase) & UART_TX_FULL));			
		csp_uart_set_data(ptUartBase, *(pbySend+i));
	}
	
	return i;
}
/** \brief receive data from uart, this function is no interrupt polling
 * 
 *  \param[in] ptUartBase: pointer of uart register structure
 *  \param[in] pData: pointer to buffer with data to be received.
 *  \param[in] hwSize: number of data to receive (byte).
 *  \param[in] wTimeOut: the timeout between bytes(ms), unit: ms 
 *  \return  the num of data which is receive successfully/error code
 */
int32_t csi_uart_receive(csp_uart_t *ptUartBase, void *pData, uint16_t hwSize, uint32_t wTimeOut)
{
	uint8_t *pbyRecv = (uint8_t *)pData;
	volatile int16_t hwRecvNum = 0;
	
	csp_uart_rxfifo_rst(ptUartBase);								//reset rxfifo，clear rxfifo
	if(NULL == pData) 
		return -1;
	
	if(wTimeOut)	//handle with wTimeOut
	{
		uint32_t wRecvStart = csi_tick_get_ms();	
		while(hwRecvNum < hwSize)
		{
			while(!(csp_uart_get_sr(ptUartBase) & UART_RNE))		//fifo empty? wait	
			{
				if((csi_tick_get_ms() - wRecvStart) >= wTimeOut) 
				{
					return hwRecvNum;
				}
			}
			pbyRecv[hwRecvNum ++] = csp_uart_get_data(ptUartBase);
			wRecvStart = csi_tick_get_ms();	
		}
	}
	else			//handle without wTimeOut
	{
		while(hwRecvNum < hwSize)
		{
			while(!(csp_uart_get_sr(ptUartBase) & UART_RNE));		//fifo empty? wait	
			pbyRecv[hwRecvNum ++] = csi_uart_getc(ptUartBase);
		}
	}

	return hwRecvNum;
}
/** \brief send data from uart, this function is interrupt mode.
 * 
 *  \param[in] ptUartBase: pointer of uart register structure
 *  \param[in] pData: pointer to buffer with data to send to uart transmitter.
 *  \param[in] hwSize: number of data to send (byte).
 *  \return  error code \ref csi_error_t
 */
csi_error_t csi_uart_send_int(csp_uart_t *ptUartBase, const void *pData, uint16_t hwSize)
{
	uint8_t byIdx = apt_get_uart_idx(ptUartBase);
	 
	if((NULL == pData) || (0 == hwSize) || (g_tUartCtrl[byIdx].byTxState== UART_STATE_SEND))
		return CSI_ERROR;
	
	g_tUartCtrl[byIdx].pbyTxBuf = (uint8_t *)pData;					
	g_tUartCtrl[byIdx].hwTxSize = hwSize;
	g_tUartCtrl[byIdx].byTxState = UART_STATE_SEND;
	
	csp_uart_int_enable(ptUartBase, UART_TXFIFO_INT);
		
	return CSI_OK;
}
/** \brief receive data from uart, this function is interrupt receive
 * 
 *  \param[in] ptUartBase: pointer of uart register structure
 *  \param[in] pData: pointer to buffer with data to be received.
 *  \param[in] hwSize: number of data to receive (byte).
 *  \return  error code \ref csi_error_t
 */	
csi_error_t csi_uart_receive_int(csp_uart_t *ptUartBase, void *pData, uint16_t hwSize)
{
	uint8_t byIdx = apt_get_uart_idx(ptUartBase);
	
	if((NULL == pData) || (0 == hwSize) || (g_tUartCtrl[byIdx].byTxState == UART_STATE_RECV)) 
		return CSI_ERROR;
		
	g_tUartCtrl[byIdx].pbyRxBuf = (uint8_t *)pData;
	g_tUartCtrl[byIdx].hwRxSize = hwSize;
	g_tUartCtrl[byIdx].hwTransNum = 0;
	g_tUartCtrl[byIdx].byRxState = UART_STATE_RECV;
	
	csp_uart_rxfifo_rst(ptUartBase);										//reset rxfifo
	csp_uart_rto_enable(ptUartBase);										//enable rto
	csp_uart_int_enable(ptUartBase, UART_RXFIFO_INT | UART_RXTO_INT);		//rxfifo and receive timeout int
	
	return CSI_OK;
}
/** \brief uart dma receive mode init
 * 
 *  \param[in] ptUartBase: pointer of uart register structure
 *  \param[in] eDmaCh: channel number of dma, eDmaCh: DMA_CH0` DMA_CH3
 *  \param[in] eEtbCh: channel id number of etb, eEtbCh >= ETB_CH8
 *  \return  error code \ref csi_error_t
 */
//csi_error_t csi_uart_dma_rx_init(csp_uart_t *ptUartBase, csi_dma_reload_e eReload, csp_dma_t *ptDmaBase, csi_dma_ch_e eDmaCh, csi_etb_ch_e eEtbCh)
csi_error_t csi_uart_dma_rx_init(csp_uart_t *ptUartBase, csi_dma_ch_e eDmaCh, csi_etb_ch_e eEtbCh)
{
	csi_error_t ret = CSI_OK;
	csi_dma_ch_config_t tDmaConfig;				
	csi_etb_config_t 	tEtbConfig;				
	uint8_t byUartIdx = apt_get_uart_idx(ptUartBase);	
	
	//dma config
	tDmaConfig.bySrcLinc 	= DMA_ADDR_CONSTANT;		//低位传输原地址固定不变
	tDmaConfig.bySrcHinc 	= DMA_ADDR_CONSTANT;		//高位传输原地址固定不变
	tDmaConfig.byDetLinc 	= DMA_ADDR_CONSTANT;		//低位传输目标地址固定不变
	tDmaConfig.byDetHinc 	= DMA_ADDR_INC;				//高位传输目标地址自增
	tDmaConfig.byDataWidth 	= DMA_DSIZE_8_BITS;			//传输数据宽度8bit
	tDmaConfig.byReload 	= DMA_RELOAD_ENABLE;		//禁止自动重载
	tDmaConfig.byTransMode 	= DMA_TRANS_CONTINU;			//DMA服务模式(传输模式)，连续服务
	tDmaConfig.byTsizeMode  = DMA_TSIZE_ONE_DSIZE;		//传输数据大小，一个 DSIZE , 即DSIZE定义大小
	tDmaConfig.byReqMode	= DMA_REQ_HARDWARE;			//DMA请求模式，硬件请求
	tDmaConfig.wInt			= DMA_INTSRC_TCIT;			//使用TCIT中断
	
	//etb config
	tEtbConfig.eChType = ETB_ONE_TRG_ONE_DMA;					//单个源触发单个目标，DMA方式
	tEtbConfig.eSrcIp 	= ETB_UART0_RXSRC + (byUartIdx << 1);	//UART TXSRC作为触发源
	tEtbConfig.eDstIp 	= ETB_DMA0_CH0 + eDmaCh;					//ETB DMA通道 作为目标实际
	tEtbConfig.eTrgMode = ETB_HARDWARE_TRG;					//通道触发模式采样硬件触发
	
	ret = csi_etb_ch_config(eEtbCh, &tEtbConfig);				//初始化ETB，DMA ETB CHANNEL > ETB_CH19_ID
	if(ret < CSI_OK)
		return CSI_ERROR;
	ret = csi_dma_ch_init(DMA0, eDmaCh, &tDmaConfig);			//初始化DMA
	csp_uart_set_rxdma(ptUartBase, UART_RDMA_EN, UART_RDMA_FIFO_NSPACE);
	return ret;
}
/** \brief uart dma send mode init
 * 
 *  \param[in] ptUartBase: pointer of uart register structure
 *  \param[in] ptDmaBase: pointer of dma register structure
 *  \param[in] eDmaCh: channel number of dma, eDmaCh: DMA_CH0` DMA_CH3
 *  \param[in] eEtbCh: channel id number of etb, eEtbCh >= ETB_CH8
 *  \return  error code \ref csi_error_t
 */
csi_error_t csi_uart_dma_tx_init(csp_uart_t *ptUartBase, csi_dma_ch_e eDmaCh, csi_etb_ch_e eEtbCh)
{
	csi_error_t ret = CSI_OK;
	csi_dma_ch_config_t tDmaConfig;				
	csi_etb_config_t 	tEtbConfig;	
	uint8_t byUartIdx = apt_get_uart_idx(ptUartBase);				

	//dma config
	tDmaConfig.bySrcLinc 	= DMA_ADDR_CONSTANT;		//低位传输原地址固定不变
	tDmaConfig.bySrcHinc 	= DMA_ADDR_INC;				//高位传输原地址自增
	tDmaConfig.byDetLinc 	= DMA_ADDR_CONSTANT;		//低位传输目标地址固定不变
	tDmaConfig.byDetHinc 	= DMA_ADDR_CONSTANT;		//高位传输目标地址固定不变
	tDmaConfig.byDataWidth 	= DMA_DSIZE_8_BITS;			//传输数据宽度8bit
	tDmaConfig.byReload 	= DMA_RELOAD_DISABLE;		//禁止自动重载
	tDmaConfig.byTransMode 	= DMA_TRANS_CONTINU;			//DMA服务模式(传输模式)，连续服务
	tDmaConfig.byTsizeMode  = DMA_TSIZE_ONE_DSIZE;		//传输数据大小，一个 DSIZE , 即DSIZE定义大小
	tDmaConfig.byReqMode	= DMA_REQ_HARDWARE;			//DMA请求模式，软件请求（软件触发）
	tDmaConfig.wInt			= DMA_INTSRC_TCIT;		//使用TCIT中断
	
	//etb config
	tEtbConfig.eChType = ETB_ONE_TRG_ONE_DMA;					//单个源触发单个目标，DMA方式
	tEtbConfig.eSrcIp 	= ETB_UART0_TXSRC + (byUartIdx << 1);	//UART TXSRC作为触发源
	tEtbConfig.eSrcIp1 = 0xff;						
	tEtbConfig.eSrcIp2 = 0xff;
	tEtbConfig.eDstIp 	= ETB_DMA0_CH0 + eDmaCh;					//ETB DMA通道 作为目标实际
	tEtbConfig.eDstIp1 = 0xff;
	tEtbConfig.eDstIp2 = 0xff;
	tEtbConfig.eTrgMode = ETB_HARDWARE_TRG;					//通道触发模式采样硬件触发
	
	ret = csi_etb_ch_config(eEtbCh, &tEtbConfig);				//初始化ETB，DMA ETB CHANNEL > ETB_CH19_ID
	if(ret < CSI_OK)
		return CSI_ERROR;
	ret = csi_dma_ch_init(DMA0, eDmaCh, &tDmaConfig);		//初始化DMA
	csp_uart_set_txdma(ptUartBase, UART_TDMA_EN, UART_TDMA_FIFO_NFULL);
	return ret;
}
/** \brief send data from uart, this function is dma mode
 * 
 *  \param[in] ptUartBase: pointer of uart register structure
 *  \param[in] ptDmaBase: pointer of dma register structure
 *  \param[in] eDmaCh: channel number of dma, eDmaCh: DMA_CH0` DMA_CH3
 *  \param[in] pData: pointer to buffer with data to send to uart transmitter.
 *  \param[in] hwSize: number of data to send (byte).
 *  \return  none
 */
void csi_uart_send_dma(csp_uart_t *ptUartBase, csi_dma_ch_e eDmaCh, const void *pData, uint16_t hwSize)
{
	csi_dma_ch_start(DMA0, eDmaCh, (void *)pData, (void *)&(ptUartBase->DATA), hwSize,1);
}
/** \brief receive data from uart, this function is dma mode
 * 
 *  \param[in] ptUartBase: pointer of uart register structure
 *  \param[in] ptDmaBase: pointer of dma register structure
 *  \param[in] eDmaCh: channel number of dma, eDmaCh: DMA_CH0` DMA_CH3
 *  \param[in] pData: pointer to buffer with data to receive to uart transmitter.
 *  \param[in] hwSize: number of data to receive (byte).
 *  \return  none
 */
void csi_uart_recv_dma(csp_uart_t *ptUartBase, csi_dma_ch_e eDmaCh, void *pData, uint16_t hwSize)
{
	csi_dma_ch_start(DMA0, eDmaCh, (void *)&(ptUartBase->DATA), (void *)pData, hwSize,1);
}
/** \brief get uart receive/send complete message and (Do not) clear message
 * 
 *  \param[in] ptUartBase: pointer of uart reg structure.
 *  \param[in] eWkMode: tx or rx mode, \ref csi_uart_wkmode_e
 *  \param[in] bClrEn: clear uart receive/send complete message enable; ENABLE: clear , DISABLE: Do not clear
 *  \return  bool type true/false
 */ 
bool csi_uart_get_msg(csp_uart_t *ptUartBase, csi_uart_wkmode_e eWkMode, bool bClrEn)
{
	uint8_t byIdx = apt_get_uart_idx(ptUartBase);
	bool bRet = false;
	
	switch(eWkMode)
	{
		case UART_SEND:
			if(g_tUartCtrl[byIdx].byTxState == UART_STATE_TX_DNE)
			{
				if(bClrEn)
					g_tUartCtrl[byIdx].byTxState = UART_STATE_IDLE;		//clear send status
				bRet = true;
			}
			break;
		case UART_RECV:
			if(g_tUartCtrl[byIdx].byTxState == UART_STATE_RX_DNE)
			{
				if(bClrEn)
					g_tUartCtrl[byIdx].byRxState = UART_STATE_IDLE;		//clear receive status
				bRet = true;
			}
			break;
		case UART_RTO:
			if(g_tUartCtrl[byIdx].byTxState == UART_STATE_RX_TO)
			{
				if(bClrEn)
					g_tUartCtrl[byIdx].byRxState = UART_STATE_IDLE;		//clear receive status
				bRet = true;
			}
			break;
		default:
		
			break;
	}
	return bRet;
}
/** \brief clr uart receive/send status message (set uart recv/send status idle) 
 * 
 *  \param[in] ptUartBase: pointer of uart reg structure.
 *  \param[in] eMode: tx or rx mode, \ref csi_uart_wkmode_e
 *  \return none
 */ 
void csi_uart_clr_msg(csp_uart_t *ptUartBase, csi_uart_wkmode_e eWkMode)
{
	uint8_t byIdx = apt_get_uart_idx(ptUartBase);
	
	if(eWkMode == UART_SEND)
		g_tUartCtrl[byIdx].byTxState = UART_STATE_IDLE;		//clear send status
	else
		g_tUartCtrl[byIdx].byRxState = UART_STATE_IDLE;		//clear receive status
}

