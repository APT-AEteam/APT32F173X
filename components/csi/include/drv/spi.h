/***********************************************************************//** 
 * \file  spi.h
 * \brief  head file of csi spi
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version   <th>Author  <th>Description
 * <tr><td> 2020-8-28  <td>V0.0  <td>ZJY     <td>initial
 * <tr><td> 2021-1-12  <td>V0.1  <td>ZJY     <td>modify
 * <tr><td> 2021-07-02 <td>V0.2  <td>LQ      <td>modify
 * <tr><td> 2023-9-25  <td>V0.3  <td>WCH     <td>code normalization
 * </table>
 * *********************************************************************
*/

#ifndef _DRV_SPI_H_
#define _DRV_SPI_H_

#include "drv/dma.h"
#include "csp.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  \enum    csi_spi_mode_e
 *  \brief   Function mode of spi
 */
typedef enum {
    SPI_MODE_MASTER,             //SPI work in master mode
    SPI_MODE_SLAVE               //SPI work in slave mode
}csi_spi_work_mode_e;

/**
 *  \enum     csi_spi_frame_len_e
 *  \brief    SPI data width (4bit ~ 16bit)
 */
typedef enum {
    SPI_FRAME_LEN_4  = 3,
    SPI_FRAME_LEN_5,
    SPI_FRAME_LEN_6,
    SPI_FRAME_LEN_7,
    SPI_FRAME_LEN_8,
    SPI_FRAME_LEN_9,
    SPI_FRAME_LEN_10,
    SPI_FRAME_LEN_11,
    SPI_FRAME_LEN_12,
    SPI_FRAME_LEN_13,
    SPI_FRAME_LEN_14,
    SPI_FRAME_LEN_15,
    SPI_FRAME_LEN_16
}csi_spi_frame_len_e;

/**
 *  \enum     csi_spi_cp_format_e
 *  \brief    Timing format of spi
 */
typedef enum {
    SPI_MODE_CPOL0_CPHA0 = 0,  //Clock Polarity 0, Clock Phase 0
    SPI_MODE_CPOL0_CPHA1,      //Clock Polarity 0, Clock Phase 1
    SPI_MODE_CPOL1_CPHA0,      //Clock Polarity 1, Clock Phase 0
    SPI_MODE_CPOL1_CPHA1       //Clock Polarity 1, Clock Phase 1
}csi_spi_cpol_cpoh_mode_e;

typedef enum
{
	SPI_RXFIFOTRG_1_8	= (0x01ul),            
	SPI_RXFIFOTRG_1_4   = (0x02ul),        
	SPI_RXFIFOTRG_1_2   = (0x04ul)   
}csi_spi_rxfifo_trg_e;

/**
 *  \enum     csi_spi_state_e
 *  \brief    SPI working state
 */
typedef enum {
    SPI_STATE_IDLE,           	//spi idle(rx/tx)
    SPI_STATE_RECV,        		//spi receiving
    SPI_STATE_SEND,   			//spi sending
    SPI_STATE_RX_DNE,    		//spi receive complete
	SPI_STATE_TX_DNE,          	//spi send complete
    SPI_STATE_RX_TO,         	//spi receive timeout(dynamic end)
}csi_spi_state_e;

/**
 *  \enum     csi_spi_intsrc_e
 *  \brief    Interrupt source set of spi
 */
typedef enum
{
	SPI_INTSRC_ROTIM	= (0x01ul << 0), 		//Receive Overflow Interrupt              
	SPI_INTSRC_RTIM    	= (0x01ul << 1), 		//Receive Timeout Interrupt          
	SPI_INTSRC_RXIM   	= (0x01ul << 2),		//Receive FIFO Interrupt           
	SPI_INTSRC_TXIM		= (0x01ul << 3),    	//Transmit FIFO interrupt 
	SPI_INTSRC_ALL		= (0x0F),				//ALL interrupt 
}csi_spi_intsrc_e;

typedef struct 
{	
	csi_spi_work_mode_e      	eWorkMode;	    //SPI work mode:master/slave
	csi_spi_cpol_cpoh_mode_e 	eCpolCpohMode;  //SPI CPOL/CPOH mode:0/1/2/3
	csi_spi_frame_len_e 		eFrameLen;      //SPI data size:4-16 bit
	csi_spi_rxfifo_trg_e		eRxFifoTrg;     //SPI rxfifo Trigger point 
	uint32_t     				wSpiBaud;		//SPI clk
}csi_spi_config_t;

typedef struct {
	csi_spi_state_e 	eTxState;				//send state
	csi_spi_state_e 	eRxState;				//receive state	
	uint32_t            wTxSize;				//tx send data size
	uint32_t            wRxSize;				//rx receive data size
	uint32_t			wTransNum;				//send/receive data num
	uint8_t				*pbyTxBuf;				//pointer of send buf 
	uint8_t				*pbyRxBuf;				//pointer of recv buf 
	//CallBack		
	void(*recv_callback)(csp_spi_t *ptSpiBase, csi_spi_state_e eState, uint8_t *pbyBuf, uint16_t *hwSzie);
	void(*send_callback)(csp_spi_t *ptSpiBase);
}csi_spi_ctrl_t;

//----------------------------------Functions declaration-------------------------------------
/** \brief spi interrupt handle weak function
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \return none
 */ 
void csi_spi_irqhandler(csp_spi_t *ptSpiBase);

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
void csi_spi_init(csp_spi_t *ptSpiBase,csi_spi_config_t *ptSpiCfg);

/** \brief enable spi interrupt 
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] eSpiInt: spi interrupt source, \ref csi_spi_intsrc_e
 *  \return none
 */
void csi_spi_int_enable(csp_spi_t *ptSpiBase, csi_spi_intsrc_e eSpiInt);

/** \brief disable spi interrupt 
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] eSpiInt: spi interrupt source, \ref csi_spi_intsrc_e
 *  \return none
 */
void csi_spi_int_disable(csp_spi_t *ptSpiBase, csi_spi_intsrc_e eSpiInt);

/** \brief clear spi interrupt status
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] eIntSta: spi interrupt status, \ref csi_spi_intsrc_e
 *  \return none
 */
void csi_spi_clr_isr(csp_spi_t *ptSpiBase, csi_spi_intsrc_e eSpiInt);

/** \brief start spi
 *  \param ptSpiBase:  pointer of spi register structure
 *  \return none
 */ 
void csi_spi_start(csp_spi_t *ptSpiBase);

/**
 \brief stop spi
 \param ptSpiBase  pointer of spi register structure
 \return none
*/
void csi_spi_stop(csp_spi_t *ptSpiBase);

/** \brief set spi work mode, master or slave
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] eMode: \ref to csi_spi_mode_e
 *  \return none
 */ 
void csi_spi_set_work_mode(csp_spi_t *ptSpiBase, csi_spi_work_mode_e eMode);

/** \brief set spi cp format
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] eFormat: \ref to csi_spi_cp_format_e
 *  \return none
 */
void csi_spi_set_cpol_cpoh(csp_spi_t *ptSpiBase, csi_spi_cpol_cpoh_mode_e eFormat);

/** \brief set spi work frequency
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] wBaud: spi work baud
 *  \return spi config frequency
 */
uint32_t csi_spi_set_clk(csp_spi_t *ptSpiBase, uint32_t wBaud);

/** \brief set spi frame length
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] eLength: \ref to csi_spi_frame_len_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_spi_set_data_size(csp_spi_t *ptSpiBase, csi_spi_frame_len_e eLength);

/** \brief get the tState of spi device
 * 
 *  \param[in] eWorkMode
 *  \return read/write state or CSI_ERROR/CSI_OK
 */ 
int8_t csi_spi_get_state(csi_spi_work_mode_e eWorkMode);

/** \brief sending data to spi transmitter(received data is ignored),blocking mode
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] pData: pointer to buffer with data to send to spi transmitter
 *  \param[in] wSize: number of data to send(byte)
 *  \return return the num of data or  return Error code
 */
int32_t csi_spi_send(csp_spi_t *ptSpiBase, const void *pData, uint32_t wSize);

/** \brief sending data to spi transmitter, non-blocking mode(interrupt mode)
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] pData: pointer to buffer with data to send to spi transmitter
 *  \param[in] wSize: number of data to send(byte)
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_spi_send_int(csp_spi_t *ptSpiBase, const void *pData, uint32_t wSize);

/** \brief  receiving data from spi receiver, blocking mode
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] pData: pointer to buffer with data to receive
 *  \param[in] wSize: number of data to receive(byte)
 *  \return return the num of data or  return Error code
 */
int32_t csi_spi_receive(csp_spi_t *ptSpiBase, void *pData, uint32_t wSize);

/** \brief  receiving data from spi receiver, not-blocking mode(interrupt mode)
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] pData: pointer to buffer with data to send to spi transmitter
 *  \param[in] wSize: number of data to receive(byte)
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_spi_receive_int(csp_spi_t *ptSpiBase, void *pData, uint32_t wSize);

/** \brief  receiving data from spi receiver,blocking mode
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] pTXdata: pointer to buffer with data to send to spi transmitter
 *  \param[in] pRXdata: pointer to buffer with data to receive to spi transmitter
 *  \param[in] wSize: number of data to send or receive(byte)
 *  \return error code \ref csi_error_t
 */
int32_t csi_spi_send_receive(csp_spi_t *ptSpiBase, void *pDataout, void *pDatain, uint32_t wSize);

/** \brief send data of spi by DMA
 * 
 *  \param[in] ptSpiBase: pointer of SPI reg structure.
 *  \param[in] pData: pointer to buffer data of SPI transmitter.
 *  \param[in] hwSize: number of data to send (byte).
 *  \param[in] ptDmaBase: pointer of DMA reg structure.
 *  \param[in] byDmaCh: channel of DMA(0 -> 5)
 *  \return  error code \ref csi_error_t
 */
csi_error_t csi_spi_send_dma(csp_spi_t *ptSpiBase, const void *pData, uint16_t hwSize, csp_dma_t *ptDmaBase,uint8_t byDmaCh);

/** \brief receive data of spi by DMA
 * 
 *  \param[in] ptSpiBase: pointer of SPI reg structure.
 *  \param[in] pbyRecv: pointer to buffer data of SPI receive.
 *  \param[in] hwSize: number of data to receive (byte).
 *  \param[in] ptDmaBase: pointer of DMA reg structure.
 *  \param[in] byDmaCh: channel of DMA(0 -> 5)
 *  \return  error code \ref csi_error_t
 */
csi_error_t csi_spi_recv_dma(csp_spi_t *ptSpiBase, void *pbyRecv, uint16_t hwSize, csp_dma_t *ptDmaBase,uint8_t byDmaCh);

#ifdef __cplusplus
}
#endif

#endif /* _DRV_SPI_H_ */
