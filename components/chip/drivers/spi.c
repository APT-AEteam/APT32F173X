/***********************************************************************//** 
 * \file  spi.c
 * \brief  csi spi driver
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-28  <td>V0.0  <td>ZJY    <td>initial
 * <tr><td> 2021-07-02 <td>V0.1  <td>LQ     <td>modify
 * <tr><td> 2023-05-8  <td>V0.2  <td>WCH    <td>modify
 * <tr><td> 2023-9-25  <td>V0.3  <td>WCH    <td>code normalization
 * </table>
 * *********************************************************************
*/
#include "drv/spi.h"

/* Private macro------------------------------------------------------*/
/* externs function---------------------------------------------------*/
/* Private function------------------------------------------------------*/
static uint8_t apt_get_spi_idx(csp_spi_t *ptSpiBase);
/* externs variablesr-------------------------------------------------*/
/* Private variablesr-------------------------------------------------*/ 
csi_spi_ctrl_t g_tSpiCtrl[SPI_IDX];	

/** \brief spi interrupt handle weak function
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \return none
 */ 
void csi_spi_irqhandler(csp_spi_t *ptSpiBase)
{	
#if 0
	uint32_t wStatus = csp_spi_get_isr(ptSpiBase);
	volatile uint8_t receive_data[4];
	//fifo rx 
	if(wStatus & SPI_INT_RXIM)
	{
		//for reference
		if(0==g_tSpiTransmit.byWorkMode)//主机
		{
			apt_spi_intr_recv_data(ptSpiBase);//when use"csi_spi_send_receive_async"function need open
		}
		else//从机
		{
			for(uint8_t byIdx = 0; byIdx < g_tSpiTransmit.byRxFifoLength; byIdx++)
			{
				receive_data[byIdx] = csi_spi_receive_slave(ptSpiBase);
				csi_spi_send_slave(ptSpiBase, receive_data[byIdx]);
			}
		}
	}
	//fifo tx 
	if(wStatus & SPI_INT_TXIM)		
	{
		//for reference
		apt_spi_intr_send_data(ptSpiBase);
	}
	
	//fifo overflow
	if(wStatus & SPI_INT_ROTIM)
	{	
		//for reference
		csp_spi_sw_rst(ptSpiBase,SPI_RXFIFO_RST);
		csp_spi_clr_isr(ptSpiBase, SPI_INT_ROTIM);
	}
	
	//fifo rx timeout
	if(wStatus & SPI_INT_RTIM)		
	{	
		//for reference
		csp_spi_clr_isr(ptSpiBase, SPI_INT_RTIM);
		
		for(uint8_t byIdx = 0; byIdx < g_tSpiTransmit.byRxFifoLength-1; byIdx++)
		{
			
			if(csp_spi_get_sr(ptSpiBase) & SPI_RNE)
			{
				*g_tSpiTransmit.pbyRxData = csp_spi_get_data(ptSpiBase);		//receive data
				g_tSpiTransmit.pbyRxData++;
			}
			else
			{
				break;
			}
			
		}		
		g_tSpiTransmit.byRxSize = 0;
		g_tSpiTransmit.byReadable = SPI_STATE_IDLE;
		csp_spi_int_disable(ptSpiBase, SPI_INT_RXIM | SPI_INT_RTIM);			//disable fifo rx int
	}
#endif
}

/** \brief initialize spi data structure
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] ptSpiCfg: pointer of user spi parameter config
 * 				-eWorkMode:SPI work mode:master/slave
 * 				-eCpolCpohMode:SPI CPOL/CPOH mode:0/1/2/3
 * 				-eFrameLen;SPI data size:4-16 bit
 * 				-eRxFifoTrg:SPI rxfifo Trigger point
 * 				-wSpiBaud:SPI clk
 *  \return none
 */ 
void csi_spi_init(csp_spi_t *ptSpiBase,csi_spi_config_t *ptSpiCfg)
{
	csi_clk_enable((uint32_t *)ptSpiBase);				       //打开时钟

	if((ptSpiCfg->eRxFifoTrg != SPI_RXFIFOTRG_1_8) && (ptSpiCfg->eRxFifoTrg != SPI_RXFIFOTRG_1_4) && (ptSpiCfg->eRxFifoTrg != SPI_RXFIFOTRG_1_2))
		ptSpiCfg->eRxFifoTrg = SPI_RXFIFOTRG_1_8;
	csp_spi_set_rxifl(ptSpiBase, (spi_rxifl_e)ptSpiCfg->eRxFifoTrg);	//set fxfifo and enable

	csi_spi_set_work_mode(ptSpiBase, ptSpiCfg->eWorkMode);			   //主从机
	csi_spi_set_cpol_cpoh(ptSpiBase, ptSpiCfg->eCpolCpohMode); //极性和相位设置	
	csi_spi_set_data_size(ptSpiBase, ptSpiCfg->eFrameLen);	   //格式帧长度设置
	csi_spi_set_clk(ptSpiBase, ptSpiCfg->wSpiBaud);              //通信速率		
}

/** \brief enable spi interrupt 
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] eSpiInt: spi interrupt source, \ref csi_spi_intsrc_e
 *  \return none
 */
void csi_spi_int_enable(csp_spi_t *ptSpiBase, csi_spi_intsrc_e eSpiInt)
{
	csp_spi_clr_isr(ptSpiBase, (spi_int_e)eSpiInt);
	csp_spi_int_enable(ptSpiBase, (spi_int_e)eSpiInt);
}

/** \brief disable spi interrupt 
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] eSpiInt: spi interrupt source, \ref csi_spi_intsrc_e
 *  \return none
 */
void csi_spi_int_disable(csp_spi_t *ptSpiBase, csi_spi_intsrc_e eSpiInt)
{
	csp_spi_int_disable(ptSpiBase, (spi_int_e)eSpiInt);
}

/** \brief clear spi interrupt status
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] eIntSta: spi interrupt status, \ref csi_spi_intsrc_e
 *  \return none
 */
void csi_spi_clr_isr(csp_spi_t *ptSpiBase, csi_spi_intsrc_e eSpiInt)
{
	csp_spi_clr_isr(ptSpiBase, (spi_int_e)eSpiInt);
}

/** \brief start spi
 *  \param ptSpiBase:  pointer of spi register structure
 *  \return none
 */ 
void csi_spi_start(csp_spi_t *ptSpiBase)
{    
	csp_spi_enable(ptSpiBase);	
}

/**
 \brief stop spi
 \param ptSpiBase  pointer of spi register structure
 \return none
*/
void csi_spi_stop(csp_spi_t *ptSpiBase)
{
  csp_spi_disable(ptSpiBase);	
}

/** \brief set spi work mode, master or slave
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] eMode: \ref to csi_spi_mode_e
 *  \return none
 */ 
void csi_spi_set_work_mode(csp_spi_t *ptSpiBase, csi_spi_work_mode_e eMode)
{
	csp_spi_set_work_mode(ptSpiBase, (spi_mode_e)eMode);
}

/** \brief set spi cp format
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] eFormat: \ref to csi_spi_cp_format_e
 *  \return none
 */
void csi_spi_set_cpol_cpoh(csp_spi_t *ptSpiBase, csi_spi_cpol_cpoh_mode_e eFormat)
{
    csp_spi_set_spo_sph(ptSpiBase, (spi_spo_h_e)eFormat);
} 

/** \brief set spi work frequency
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] wBaud: spi work baud
 *  \return spi config frequency
 */
uint32_t csi_spi_set_clk(csp_spi_t *ptSpiBase, uint32_t wBaud)
{
    uint32_t wDiv;
    uint32_t wFreq = 0U;
 
	if(wBaud == 0) return 0;

	wDiv = (csi_get_pclk_freq() >> 1) / wBaud;//baud = FPCLK/ CPSDVR / (1 + SCR))
	
	if(wDiv > 0)
		wDiv --;
	
	csp_spi_set_clk_div(ptSpiBase, wDiv, 2);

	if(wDiv > 0)
		wFreq = (csi_get_pclk_freq() >> 1) / (wDiv + 1);
	else
		wFreq = (csi_get_pclk_freq() >> 1) ;
		
    return wFreq;
}

/** \brief set spi frame length
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] eLength: \ref to csi_spi_frame_len_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_spi_set_data_size(csp_spi_t *ptSpiBase, csi_spi_frame_len_e eLength)
{	
    if ((eLength < SPI_FRAME_LEN_4) || (eLength > SPI_FRAME_LEN_16)) 
		return CSI_ERROR;
	csp_spi_set_data_size(ptSpiBase, (spi_data_size_e)eLength);
	
    return CSI_OK;
}

/** \brief sending data to spi transmitter(received data is ignored),blocking mode
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] pData: pointer to buffer with data to send to spi transmitter
 *  \param[in] wSize: number of data to send(byte)
 *  \return return the num of data or  return Error code
 */
int32_t csi_spi_send(csp_spi_t *ptSpiBase, const void *pData, uint32_t wSize)
{
	int32_t i; 
	uint32_t wTimeOut;
	uint8_t *pbySend = (uint8_t *)pData;
	
	if(NULL == pData || 0 == wSize)
		return -1;
	
	csp_spi_sw_rst(ptSpiBase,SPI_TXFIFO_RST);        //clear tx fifo
	
	for(i = 0; i < wSize; i++)
	{
		wTimeOut = SPI_SEND_TIMEOUT;
		while(!(csp_spi_get_sr(ptSpiBase) & SPI_TNF) && wTimeOut --);	//fifo full? wait;			
		csp_spi_set_data(ptSpiBase, *(pbySend + i));
	}
	
	wTimeOut = SPI_SEND_TIMEOUT;
	while(csp_spi_get_sr(ptSpiBase) & SPI_BSY)								//wait for transmition finish
	{
		wTimeOut --;
		if(wTimeOut == 0)
			break;
	}
	
	return i;
}

/** \brief sending data to spi transmitter, non-blocking mode(interrupt mode)
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] pData: pointer to buffer with data to send to spi transmitter
 *  \param[in] wSize: number of data to send(byte)
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_spi_send_int(csp_spi_t *ptSpiBase, const void *pData, uint32_t wSize)
{		
	uint8_t byIdx = apt_get_spi_idx(ptSpiBase);
	
	if(NULL == pData || 0 == wSize)
		return CSI_ERROR;

	g_tSpiCtrl[byIdx].wTxSize 	= wSize;
	g_tSpiCtrl[byIdx].pbyTxBuf = (uint8_t *)pData;
	g_tSpiCtrl[byIdx].eTxState = SPI_STATE_SEND;
	
	csp_spi_sw_rst(ptSpiBase,SPI_TXFIFO_RST);        //clear tx fifo
	csp_spi_int_enable(ptSpiBase, SPI_INT_TXIM);	//enable tx fifo int
	
	return CSI_OK;
}

/** \brief  receiving data from spi receiver, blocking mode
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] pData: pointer to buffer with data to receive
 *  \param[in] wSize: number of data to receive(byte)
 *  \return return the num of data or  return Error code
 */
int32_t csi_spi_receive(csp_spi_t *ptSpiBase, void *pData, uint32_t wSize)
{	
	uint32_t wTimeOut;
	uint8_t *pbyRecv = (uint8_t *)pData;
	volatile uint32_t hwRecvNum = 0;
	
	if(NULL == pData || 0 == wSize)
		return -1;

	csp_spi_sw_rst(ptSpiBase,SPI_RXFIFO_RST);        //clear rx fifo

	while(hwRecvNum < wSize)
	{
		wTimeOut = SPI_RECV_TIMEOUT;
		while(!(csp_spi_get_sr(ptSpiBase) & SPI_RNE) && wTimeOut --);		//recv fifo empty? wait	
		if(wTimeOut ==0)
			break;
		pbyRecv[hwRecvNum ++] = csp_spi_get_data(ptSpiBase);	//recv data
	}
	return hwRecvNum;
}

/** \brief  receiving data from spi receiver, not-blocking mode(interrupt mode)
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] pData: pointer to buffer with data to send to spi transmitter
 *  \param[in] wSize: number of data to receive(byte)
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_spi_receive_int(csp_spi_t *ptSpiBase, void *pData, uint32_t wSize)
{	
	uint8_t byIdx = apt_get_spi_idx(ptSpiBase);
	
	if(NULL == pData || 0 == wSize)
		return CSI_ERROR;

	g_tSpiCtrl[byIdx].pbyRxBuf 	= (uint8_t *)pData;
	g_tSpiCtrl[byIdx].wRxSize 	= wSize;
	g_tSpiCtrl[byIdx].wTransNum = 0;
	g_tSpiCtrl[byIdx].eRxState 	= SPI_STATE_RECV;

	csp_spi_sw_rst(ptSpiBase,SPI_RXFIFO_RST); 					//clear rx fifo  
	csp_spi_int_enable(ptSpiBase, SPI_INT_RXIM | SPI_INT_RTIM);	//enable rx_fifo & rx_timeout int
	
	return CSI_OK;
}

/** \brief  receiving data from spi receiver,blocking mode
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] pTXdata: pointer to buffer with data to send to spi transmitter
 *  \param[in] pRXdata: pointer to buffer with data to receive to spi transmitter
 *  \param[in] wSize: number of data to send or receive(byte)
 *  \return error code \ref csi_error_t
 */
int32_t csi_spi_send_receive(csp_spi_t *ptSpiBase, void *pDataout, void *pDatain, uint32_t wSize)
{
	int32_t i;
	uint32_t wTimeOut;
	uint8_t *pbySend = (uint8_t *)pDataout;
	uint8_t *pbyRecv = (uint8_t *)pDatain;
	
	if((0 == wSize) || ((NULL == pDataout) && (NULL == pDatain)))
		return -1;

	csp_spi_sw_rst(ptSpiBase,SPI_RXFIFO_RST);	//clear rx fifo		
	csp_spi_sw_rst(ptSpiBase,SPI_TXFIFO_RST);	//clear tx fifo		

	for(i = 0; i < wSize; i++)
	{
		//send
		wTimeOut = SPI_SEND_TIMEOUT;
		while(!(csp_spi_get_sr(ptSpiBase) & SPI_TNF) && wTimeOut --);	//fifo full? wait;	
		if(wTimeOut ==0)
			break;		
		if(pbySend)
			csp_spi_set_data(ptSpiBase, *(pbySend + i));
		else
			csp_spi_set_data(ptSpiBase,0x00);

		//receive
		wTimeOut = SPI_RECV_TIMEOUT;
		while(!(csp_spi_get_sr(ptSpiBase) & SPI_RNE) && wTimeOut --);		//recv fifo empty? wait	
		if(wTimeOut ==0)
			break;
		if(pbyRecv)
			pbyRecv[i++] = csp_spi_get_data(ptSpiBase);			//recv data
		else
			csp_spi_get_data(ptSpiBase);
	}

	return i;
}

#if 0
/** \brief  receiving data from spi receiver, not-blocking mode
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] pDataout: pointer to buffer with data to send to spi transmitter
 *  \param[in] pDatain: number of data to receive(byte)
 *  \param[in] wSize: number of data to receive(byte)
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_spi_send_receive_async(csp_spi_t *ptSpiBase, void *pDataout, void *pDatain, uint32_t wSize)
{
    csi_error_t tRet = CSI_OK;
	
	if( (0 == wSize) || ( (NULL == pDataout) && (NULL == pDatain)  ) )
		tRet = CSI_ERROR;
	if((g_tSpiTransmit.byWriteable == SPI_STATE_BUSY) || (g_tSpiTransmit.byReadable == SPI_STATE_BUSY))
	{ 
        tRet = CSI_BUSY;
	}
		
	if(tRet == CSI_OK) 
	{
			
		g_tSpiTransmit.byTxSize = wSize;
		g_tSpiTransmit.byRxSize = wSize;

		g_tSpiTransmit.byWriteable = SPI_STATE_BUSY;
		g_tSpiTransmit.byReadable  = SPI_STATE_BUSY;
		g_tSpiTransmit.pbyTxData = (uint8_t *)pDataout;

		g_tSpiTransmit.pbyRxData = (uint8_t *)pDatain;

		csp_spi_enable(ptSpiBase);
		csp_spi_sw_rst(ptSpiBase,SPI_RXFIFO_RST);	//clear rx fifo		
		csp_spi_sw_rst(ptSpiBase,SPI_TXFIFO_RST);	//clear tx fifo	
	
		csp_spi_int_enable(ptSpiBase, g_tSpiTransmit.byInt & (uint8_t)SPI_INT_RXIM);//enable rx int
		csp_spi_int_enable(ptSpiBase, g_tSpiTransmit.byInt & (uint8_t)SPI_INT_RTIM);//enable rx timeout int
		csp_spi_int_enable(ptSpiBase, g_tSpiTransmit.byInt & (uint8_t)SPI_INT_TXIM);//enable tx int
			
    } 
		
	return tRet;
}


/** \brief spi slave receive data
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \return the slave receive data
 */ 
uint16_t csi_spi_receive_slave(csp_spi_t *ptSpiBase)
{
	while(!(csp_spi_get_sr(ptSpiBase) & SPI_RNE));	//receive not empty:read
	return csp_spi_get_data(ptSpiBase);
}

/** \brief spi slave receive data
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] hwDataout: data of send
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_spi_send_slave(csp_spi_t *ptSpiBase, uint16_t hwDataout)
{
	while(!(csp_spi_get_sr(ptSpiBase) & SPI_TNF));	//send fifo not full: write 	
	csp_spi_set_data(ptSpiBase, hwDataout);
	
	return CSI_OK;
}
//------------------------------------------------------------------------------------------
//interrupt process,just for reference
//------------------------------------------------------------------------------------------

/** \brief spi receive data,interrupt call 
 * 
 *  \param[in] ptSpiBase: pointer of SPI reg structure.
 *  \return none
 */ 
void apt_spi_intr_recv_data(csp_spi_t *ptSpiBase)
{
	if((g_tSpiTransmit.pbyRxData == NULL) || (g_tSpiTransmit.byRxSize == 0U))//调用异步收发函数，但是又不关心接收时
	{
		csp_spi_sw_rst(ptSpiBase,SPI_RXFIFO_RST);
		g_tSpiTransmit.byReadable = SPI_STATE_IDLE;
		csp_spi_int_disable(ptSpiBase, g_tSpiTransmit.byInt & (uint8_t)SPI_INT_RXIM);//disable rx int
		csp_spi_int_disable(ptSpiBase, SPI_INT_RTIM);//disable rx timeout int
	}
	else
	{
		for(uint8_t byIdx = 0; byIdx < g_tSpiTransmit.byRxFifoLength; byIdx++)
		{
			*g_tSpiTransmit.pbyRxData = csp_spi_get_data(ptSpiBase);			//receive data
			g_tSpiTransmit.pbyRxData++;
			g_tSpiTransmit.byRxSize--;
			if(g_tSpiTransmit.byRxSize == 0)
			{
				break;
			}
		}
		
		if(g_tSpiTransmit.byRxSize == 0)
		{
			g_tSpiTransmit.byReadable = SPI_STATE_IDLE;
			csp_spi_int_disable(ptSpiBase, SPI_INT_RXIM | SPI_INT_RTIM);			//disable fifo rx int
		}
	}
}

/** \brief spi send data,interrupt call
 * 
 *  \param[in] ptSpiBase: pointer of SPI reg structure.
 *  \return none
 */ 
void apt_spi_intr_send_data(csp_spi_t *ptSpiBase)
{	
	uint8_t byCount = 0;
	uint32_t wTimeStart = SPI_SEND_TIMEOUT;
	if( (ptSpiBase->CR1 & SPI_SSE_MSK) && (g_tSpiTransmit.byTxSize) )//Make sure that spi is enabled (if SPI is not enabled,just enable Tx int,Tx interrupts also come in )
	{
		
		if(g_tSpiTransmit.byTxSize >= (8 - g_tSpiTransmit.byTxFifoLength) )//剩余传输数据大于fifo深度-阈值
		{
			for(byCount = 0; byCount < g_tSpiTransmit.byTxFifoLength;byCount++)
			{
				if(g_tSpiTransmit.pbyTxData)//关心发送的数据
				{
					csp_spi_set_data(ptSpiBase,*g_tSpiTransmit.pbyTxData);//send data
					g_tSpiTransmit.pbyTxData ++;
				}	
				else//发送的数据为0，纯粹只为产生接收时钟
				{
					csp_spi_set_data(ptSpiBase,0x00);//send data
				}
			}
			g_tSpiTransmit.byTxSize = g_tSpiTransmit.byTxSize - g_tSpiTransmit.byTxFifoLength;	
		}
		else//剩余传输数据小于fifo深度-阈值
		{
			for(byCount = 0; byCount < g_tSpiTransmit.byTxSize;byCount++)
			{
				if(g_tSpiTransmit.pbyTxData)
				{
					csp_spi_set_data(ptSpiBase,*g_tSpiTransmit.pbyTxData);//send data
					g_tSpiTransmit.pbyTxData ++;
				}	
				else
				{
					csp_spi_set_data(ptSpiBase,0x00);//send data
				}
			}
			g_tSpiTransmit.byTxSize = 0;	
		}
		
		if (g_tSpiTransmit.byTxSize <= 0U) 
		{
			while((csp_spi_get_sr(ptSpiBase) & SPI_BSY) && (wTimeStart --) );
			g_tSpiTransmit.byWriteable =  SPI_STATE_IDLE;
			csp_spi_int_disable(ptSpiBase,SPI_INT_TXIM);			//disable fifo tx int
		}
	}
	else
	{
		while((csp_spi_get_sr(ptSpiBase) & SPI_BSY) && (wTimeStart --) );
		g_tSpiTransmit.byWriteable =  SPI_STATE_IDLE;
		csp_spi_int_disable(ptSpiBase, SPI_INT_TXIM);			//disable fifo tx int	
	}
}

//-----------------------------------------------------------------------------------------------------------
//high speed spi function for reference
//-----------------------------------------------------------------------------------------------------------

/** \brief csi_spi_send_receive_1byte
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] byData :the send data 
 *  \return the receive data
 */ 
uint8_t csi_spi_send_receive_1byte(csp_spi_t *ptSpiBase,uint8_t byData)
{
	uint8_t byReceiveData = 0;
	uint32_t wTimeStart = SPI_SEND_TIMEOUT;
	 
	wTimeStart = SPI_SEND_TIMEOUT;
	while( ( !((uint32_t)(ptSpiBase->SR) & SPI_TNF) ) && (wTimeStart --) ){;} //send not full:write
	ptSpiBase->DR = byData; //send data

	wTimeStart = SPI_SEND_TIMEOUT;
	while( (!((uint32_t)(ptSpiBase->SR) & SPI_RNE) ) && (wTimeStart --) ){;}  //receive not empty:read
	byReceiveData = ptSpiBase->DR;//receive data
	
	return byReceiveData;

}

/** \brief spi send buff(this funtion will ignore the receive)
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] pDataOut :send data buffer pointer
 *  \param[in] bySize ：length
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_spi_buff_send(csp_spi_t *ptSpiBase,void *pDataOut,uint8_t bySize)
{		
	csi_error_t tRet = CSI_OK;
	uint8_t byIcount = 0;
	uint32_t wTimeStart = SPI_SEND_TIMEOUT;
	
	if( g_tSpiTransmit.byWriteable == SPI_STATE_BUSY ) 
	{
		tRet = CSI_BUSY;
		return tRet;
	}
	
	g_tSpiTransmit.byWriteable = SPI_STATE_BUSY;
	g_tSpiTransmit.pbyTxData = (uint8_t *)pDataOut;
	
	for(byIcount = 0;byIcount <bySize;byIcount++)
	{
		wTimeStart = SPI_SEND_TIMEOUT;
		while( ( !((uint32_t)(ptSpiBase->SR) & SPI_TNF) ) && (wTimeStart --) ){;}  //send not full:write
		ptSpiBase->DR = *(g_tSpiTransmit.pbyTxData+byIcount);
	}
	
	wTimeStart = SPI_SEND_TIMEOUT;//ensure spi process is finish
	while( ((uint32_t)(ptSpiBase->SR) & SPI_BSY) && (wTimeStart --) ){;} //not busy:finish*/
	
	g_tSpiTransmit.byWriteable =  SPI_STATE_IDLE;
	
	return tRet;
}

/** \brief spi send and receive(equal to 8 or less than eight bytes)
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] pDataOut :send data buffer pointer
 *  \param[in] pDataIn :receive data buffer pointer
 *  \param[in] wSize ：length
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_spi_send_receive_x8(csp_spi_t *ptSpiBase, void *pDataOut,void *pDataIn,uint32_t wSize)//小于等于八个的发送接收,这种场景通常适合用来发送指令，读取状态。（大块的数据读取不合适）
{	
	csi_error_t tRet = CSI_OK;
	uint8_t byCount = 0;
	uint8_t bySize = 0;
	uint32_t wTimeStart = SPI_SEND_TIMEOUT;

	if((g_tSpiTransmit.byWriteable == SPI_STATE_BUSY) || (g_tSpiTransmit.byReadable == SPI_STATE_BUSY)) 
	{
		tRet = CSI_BUSY;
		return tRet;
	}
	
	if(wSize > 8)
		bySize = 8;
	else
		bySize = (uint8_t)wSize;
	
	csp_spi_sw_rst(ptSpiBase,SPI_RXFIFO_RST);
	
	g_tSpiTransmit.byWriteable = SPI_STATE_BUSY;
	g_tSpiTransmit.byReadable  = SPI_STATE_BUSY;
	g_tSpiTransmit.pbyTxData = (uint8_t *)pDataOut;
	g_tSpiTransmit.pbyRxData = (uint8_t *)pDataIn;
	
	while( ((uint32_t)(ptSpiBase->SR) & SPI_BSY ) && (wTimeStart --) ){;} 
	ptSpiBase->CR1 &= ~SPI_SSE_MSK;
	for(byCount = 0;byCount<bySize;byCount++)
	{
		ptSpiBase->DR = g_tSpiTransmit.pbyTxData[byCount];
	}
	csp_spi_enable(ptSpiBase);//SPI使能
	wTimeStart = SPI_SEND_TIMEOUT;
	while( ( (uint32_t)(ptSpiBase->SR) & SPI_BSY ) && (wTimeStart --) ){;} 
	
	while(bySize--)
	{
		*g_tSpiTransmit.pbyRxData++ = ptSpiBase->DR;
	}
	
	wTimeStart = SPI_SEND_TIMEOUT;//ensure spi process is finish
	while( ((uint32_t)(ptSpiBase->SR) & SPI_BSY) && (wTimeStart --) ){;} //not busy:finish*/
	
	g_tSpiTransmit.byWriteable =  SPI_STATE_IDLE;
	g_tSpiTransmit.byReadable =  SPI_STATE_IDLE;
	
	return tRet;		
}

/** \brief spi send and receive(equal to 8 bytes or  more than eight bytes)
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] pDataOut :send data buffer pointer 
 *  \param[in] pDataIn  :receive data buffer pointer 
 *  \param[in] wSize ：length
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_spi_send_receive_d8(csp_spi_t *ptSpiBase, uint8_t *pDataOut,uint8_t *pDataIn, uint32_t wSize)//大于等于八个的发送和读
{
		csi_error_t tRet = CSI_OK;
		uint8_t byTxsize = wSize;
		uint32_t wTimeStart = SPI_SEND_TIMEOUT;
		
		if((g_tSpiTransmit.byWriteable == SPI_STATE_BUSY) || (g_tSpiTransmit.byReadable == SPI_STATE_BUSY)) 
		{
			tRet = CSI_BUSY;
			return tRet;
		}
		
		g_tSpiTransmit.byWriteable = SPI_STATE_BUSY;
		g_tSpiTransmit.byReadable  = SPI_STATE_BUSY;
		g_tSpiTransmit.pbyTxData = (uint8_t *)pDataOut;
		g_tSpiTransmit.pbyRxData = (uint8_t *)pDataIn;
#if 1
		uint8_t byOutidx = 0;
		uint8_t byIdx;
		uint8_t byRemainder = 0;
		uint8_t byZheng = 0;
		uint8_t byLast8Times = 0;
		uint8_t byLastTxBuff[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

		csp_spi_sw_rst(ptSpiBase,SPI_RXFIFO_RST);

		byZheng = (wSize >> 3);
		byRemainder = wSize & 0x07;
		byLast8Times = byZheng << 3;//position
		
		memcpy((void *)byLastTxBuff,(void *)&g_tSpiTransmit.pbyTxData[byLast8Times], byRemainder);
		wTimeStart = SPI_SEND_TIMEOUT;
		while( ((uint32_t)(ptSpiBase->SR) & SPI_BSY) && (wTimeStart --) ){;} 

		while(byTxsize >=8U)
		{
			ptSpiBase->DR = g_tSpiTransmit.pbyTxData[byOutidx];
			ptSpiBase->DR = g_tSpiTransmit.pbyTxData[byOutidx+1];  //csp_spi_set_data(spi_base,*spi->pbyTxData);	//send data
			ptSpiBase->DR = g_tSpiTransmit.pbyTxData[byOutidx+2];
			ptSpiBase->DR = g_tSpiTransmit.pbyTxData[byOutidx+3];
			ptSpiBase->DR = g_tSpiTransmit.pbyTxData[byOutidx+4];
			ptSpiBase->DR = g_tSpiTransmit.pbyTxData[byOutidx+5];
			ptSpiBase->DR = g_tSpiTransmit.pbyTxData[byOutidx+6];
			ptSpiBase->DR = g_tSpiTransmit.pbyTxData[byOutidx+7];
	
			wTimeStart = SPI_SEND_TIMEOUT;
			while( ((uint32_t)(ptSpiBase->SR) & SPI_BSY) && (wTimeStart --) ){;} 		

			byTxsize -= 8;
			g_tSpiTransmit.pbyRxData[byOutidx] = ptSpiBase->DR;
			g_tSpiTransmit.pbyRxData[byOutidx+1] = ptSpiBase->DR;
			g_tSpiTransmit.pbyRxData[byOutidx+2] = ptSpiBase->DR;
			g_tSpiTransmit.pbyRxData[byOutidx+3] = ptSpiBase->DR;
			g_tSpiTransmit.pbyRxData[byOutidx+4] = ptSpiBase->DR;
			g_tSpiTransmit.pbyRxData[byOutidx+5] = ptSpiBase->DR;
			g_tSpiTransmit.pbyRxData[byOutidx+6] = ptSpiBase->DR;
			g_tSpiTransmit.pbyRxData[byOutidx+7] = ptSpiBase->DR;
			
			byOutidx += 8;	
		}
		
	if(byRemainder != 0){	
		if(byRemainder == 1)
		{
			ptSpiBase->DR = byLastTxBuff[0];
		}
		else if(byRemainder == 2)
		{
			ptSpiBase->DR = byLastTxBuff[0];ptSpiBase->DR = byLastTxBuff[1];
		}
		else if(byRemainder == 3)
		{
			ptSpiBase->DR = byLastTxBuff[0];ptSpiBase->DR = byLastTxBuff[1];ptSpiBase->DR = byLastTxBuff[2];
		}
		else if(byRemainder == 4)
		{
			ptSpiBase->DR = byLastTxBuff[0];ptSpiBase->DR = byLastTxBuff[1];ptSpiBase->DR = byLastTxBuff[2];ptSpiBase->DR = byLastTxBuff[3];
		}
		else if(byRemainder == 5)
		{
			ptSpiBase->DR = byLastTxBuff[0];ptSpiBase->DR = byLastTxBuff[1];ptSpiBase->DR = byLastTxBuff[2];ptSpiBase->DR = byLastTxBuff[3];
			ptSpiBase->DR = byLastTxBuff[4];
		}
		else if(byRemainder == 6)
		{
			ptSpiBase->DR = byLastTxBuff[0];ptSpiBase->DR = byLastTxBuff[1];ptSpiBase->DR = byLastTxBuff[2];ptSpiBase->DR = byLastTxBuff[3];
			ptSpiBase->DR = byLastTxBuff[4];ptSpiBase->DR = byLastTxBuff[5];
		}
		else if(byRemainder == 7)
		{
			ptSpiBase->DR = byLastTxBuff[0];ptSpiBase->DR = byLastTxBuff[1];ptSpiBase->DR = byLastTxBuff[2];ptSpiBase->DR = byLastTxBuff[3];
			ptSpiBase->DR = byLastTxBuff[4];ptSpiBase->DR = byLastTxBuff[5];ptSpiBase->DR = byLastTxBuff[6];
		}
		else
		{
			ptSpiBase->DR = byLastTxBuff[0];ptSpiBase->DR = byLastTxBuff[1];ptSpiBase->DR = byLastTxBuff[2];ptSpiBase->DR = byLastTxBuff[3];
			ptSpiBase->DR = byLastTxBuff[4];ptSpiBase->DR = byLastTxBuff[5];ptSpiBase->DR = byLastTxBuff[6];ptSpiBase->DR = byLastTxBuff[7];
		}
		
		wTimeStart = SPI_SEND_TIMEOUT;
		while( ((uint32_t)(ptSpiBase->SR) & SPI_BSY) && (wTimeStart --) ){;} 

		for(byIdx=0;byIdx<byRemainder;byIdx++)		//read buffer data
			g_tSpiTransmit.pbyRxData[byOutidx+byIdx] = ptSpiBase->DR;		
	}

#else 	
		csi_spi_clrRx_fifo(ptSpiBase);
		byTxsize = wSize;

		while(byTxsize > 0U)
		{
			wTimeStart = SPI_SEND_TIMEOUT;
			while( ((uint32_t)(ptSpiBase->SR) & SPI_BSY) && (wTimeStart --) ){;} 
			ptSpiBase->DR = *g_tSpiTransmit.pbyTxData;  //csp_spi_set_data(spi_base,*spi->pbyTxData);	//send data

			wTimeStart = SPI_SEND_TIMEOUT;
			while( ((uint32_t)(ptSpiBase->SR) & SPI_BSY) && (wTimeStart --) ){;} 
		
			byTxsize --;
			*g_tSpiTransmit.pbyRxData = ptSpiBase->DR;
			g_tSpiTransmit.pbyTxData ++;
			g_tSpiTransmit.pbyRxData ++;
			
		}
#endif
		
	wTimeStart = SPI_SEND_TIMEOUT;//ensure spi process is finish
	while( ((uint32_t)(ptSpiBase->SR) & SPI_BSY) && (wTimeStart --) ){;} //not busy:finish*/
	
	g_tSpiTransmit.byWriteable =  SPI_STATE_IDLE;
	g_tSpiTransmit.byReadable =  SPI_STATE_IDLE;
	
	return tRet;		
}
#endif
//---------------------------------------------DMA-------------------------------------------------
/** \brief send data of spi by DMA
 * 
 *  \param[in] ptSpiBase: pointer of SPI reg structure.
 *  \param[in] pData: pointer to buffer data of SPI transmitter.
 *  \param[in] hwSize: number of data to send (byte).
 *  \param[in] ptDmaBase: pointer of DMA reg structure.
 *  \param[in] byDmaCh: channel of DMA(0 -> 5)
 *  \return  error code \ref csi_error_t
 */
csi_error_t csi_spi_send_dma(csp_spi_t *ptSpiBase, const void *pData, uint16_t hwSize, csp_dma_t *ptDmaBase,uint8_t byDmaCh)
{
	if(hwSize > 0xfff) return CSI_ERROR;
	csp_spi_set_txdma(ptSpiBase, SPI_TDMA_EN, SPI_TDMA_FIFO_NFULL);
	csi_dma_ch_start(ptDmaBase, byDmaCh, (void *)pData, (void *)&(ptSpiBase->DR), 1, hwSize);
	
	return CSI_OK;
}

/** \brief receive data of spi by DMA
 * 
 *  \param[in] ptSpiBase: pointer of SPI reg structure.
 *  \param[in] pbyRecv: pointer to buffer data of SPI receive.
 *  \param[in] hwSize: number of data to receive (byte).
 *  \param[in] ptDmaBase: pointer of DMA reg structure.
 *  \param[in] byDmaCh: channel of DMA(0 -> 5)
 *  \return  error code \ref csi_error_t
 */
csi_error_t csi_spi_recv_dma(csp_spi_t *ptSpiBase, void *pbyRecv, uint16_t hwSize, csp_dma_t *ptDmaBase,uint8_t byDmaCh)
{
	if(hwSize > 0xfff) return CSI_ERROR;
	csp_spi_set_rxdma(ptSpiBase, SPI_RDMA_EN, SPI_RDMA_FIFO_NSPACE);
	csi_dma_ch_start(ptDmaBase, byDmaCh, (void *)&(ptSpiBase->DR),(void *)pbyRecv, 1, hwSize);
	
	return CSI_OK;
}

/** \brief get spi idx 
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \return spi id number(0~1) or error(0xff)
 */ 
static uint8_t apt_get_spi_idx(csp_spi_t *ptSpiBase)
{
	switch((uint32_t)ptSpiBase)
	{
		case APB_SPI0_BASE:
			return 0;
		case APB_SPI1_BASE:
			return 1;
		default:
			return 0xff;		//error
	}
}