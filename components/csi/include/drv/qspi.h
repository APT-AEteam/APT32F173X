/***********************************************************************//** 
 * \file  qspi.h
 * \brief  head file of csi qspi
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version   <th>Author  <th>Description
 * <tr><td> 2021-7-13 <td>V0.0  <td>LQ      <td>initial
 * </table>
 * *********************************************************************
*/

#ifndef _DRV_QSPI_H_
#define _DRV_QSPI_H_

#include <stdint.h>
#include <drv/common.h>
#include <drv/gpio.h>
#include "csp_qspi.h"

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------------------------
//datatype
typedef struct 
{	
	uint8_t         	byRxFifoLevel;			//(0-15)fifo trigger rx interrupt
	uint8_t				byTxFifoLevel;			//(0-15)fifo trigger tx interrupt
	qspi_spo_h_e        eQspiPolarityPhase;  	//0:(0 0)   1:(0 1)  2:(1 0) 3:(1 1)
	qspi_frame_size_e   eQspiFrameLen;       	//4-32 bit
	uint32_t            dwQspiBaud;				//spi clk
	uint8_t             byInter;            	//int source
}csi_qspi_config_t;

typedef struct
{
	uint8_t             *pbyTxData;      // Output data buf
	uint8_t             *pbyRxData;      // Input  data buf
    uint8_t             byTxSize;        // Output data size specified by user
    uint8_t             byRxSize;        // Input  data size specified by user
	uint8_t             byRxFifoLength;  // receive fifo  threshold
	uint8_t             byTxFifoLength;  // send fifo  threshold
	uint8_t             byInter;  		 // interrupt
    uint8_t    			byReadable;		 // 0:idle  1:busy
	uint8_t    			byWriteable;     // 0:idle  1:busy
}csi_qspi_transmit_t;
extern csi_qspi_transmit_t g_tQspiTransmit; 


//Function declaration
//-----------------------------------------------------------------------------------------------

/** \brief initialize qspi data structure
 * 
 *  \param[in] ptQspiBase: pointer of qspi register structure
 *  \param[in] ptQspiCfg: pointer of user qspi parameter config
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_qspi_init(csp_qspi_t *ptQspiBase,csi_qspi_config_t *ptQspiCfg);

/** \brief uninit qspi data structure
 * 
 *  \param[in] ptQspiBase: pointer of qspi register structure
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_qspi_uninit(csp_qspi_t *ptQspiBase);

/** \brief csi_qspi_nss_high 
 * 
 *  \param[in] ePinName:which pin use as nss
 *  \return none
 */ 
void csi_qspi_nss_high(pin_name_e ePinName);

/** \brief csi_qspi_nss_low 
 * 
 *  \param[in] ePinName:which pin use as nss
 *  \return none
 */ 
void csi_qspi_nss_low(pin_name_e ePinName);

/** \brief init spi gpio 
 * 
 *  \param[in] none
 *  \return none
 */ 
void csi_qspi_gpio_init(void);

/** \brief config qspi work frequence
 * 
 *  \param[in] ptQspiBase: pointer of qspi register structure
 *  \param[in] wBaud: qspi work baud
 *  \return qspi config frequency
 */
uint32_t csi_qspi_baud(csp_qspi_t *ptQspiBase, uint32_t wBaud);

/** \brief config qspi Internal variables
 * 
 *  \param[in] ptQspiCfg: pointer of internal variables struct
 *  \return none
 */
void csi_qspi_Internal_variables_init(csi_qspi_config_t *ptQspiCfg);

/** \brief Wait status flag with timeout
 * 
 *  \param[in] ptQspiBase: pointer of qspi register structure
 *  \param[in] bySource: flag type
 *  \param[in] bySta: desired state
 *  \param[in] wTime: timeout
 *  \return 0:success  1:failure
 */ 
uint8_t csi_qspi_waitflag(csp_qspi_t *ptQspiBase,uint8_t bySource,uint8_t bySta,uint32_t wTime);

/** \brief set waitcycle
 * 
 *  \param[in] ptQspiBase: pointer of qspi register structure
 *  \param[in] bySta: desired state
 *  \return none
 */ 
void csi_qspi_set_waitcycle(csp_qspi_t *ptQspiBase,uint8_t byCycle);

/** \brief clr interrupt
 * 
 *  \param[in] ptQspiBase: pointer of qspi register structure
 *  \param[in] byInt: interrupt source
 *  \return none
 */ 
void csi_qspi_clr_int(csp_qspi_t *ptQspiBase,uint8_t byInt);

/** \brief qspi receive
 * 
 *  \param[in] ptQspiBase: pointer of qspi register structure
 *  \param[in] byInst: instruct
 *  \param[in] wAddr: read start address
 * 	\param[in] ptRecv: pointer of receive data buff
 *  \param[in] wLen: data size
 *  \return 0:read failure   1:success
 */ 
uint8_t csi_qspi_receive(csp_qspi_t *ptQspiBase,uint8_t byInst,uint32_t wAddr,uint8_t *ptRecv,uint32_t wLen);


/** \brief qspi send
 * 
 *  \param[in] ptQspiBase: pointer of qspi register structure
 *  \param[in] byInst: instruct
 *  \param[in] wAddr: start addr of write data (qspi在一页（0-4095），写入少于256个字节的数据)
 *  \param[in] pbySend: pointer of write data buf
 *  \param[in] hwLen: length of write data(0-256)
 *  \return 0:read failure   1:success
 */
uint8_t csi_qspi_send(csp_qspi_t *ptQspiBase,uint8_t byInst,uint32_t wAddr,uint8_t *pbySend,uint16_t hwLen);

/** \brief set send_receive or send only or receive only
 * 
 *  \param[in] ptQspiBase: pointer of qspi register structure
 *  \param[in] eMode: QSPI_SEND_RECV,QSPI_SEND,QSPI_RECV
 *  \param[in] eFrf: select standard,dual,or quad spi
 * 	\param[in] byPara: bit0:3(addre len)  bit4:5(instruct len)  bit6:7(addre and instruct frametype)
 *  \param[in] wLen: data size
 *  \return 0:send failure   1:success
 */ 
void csi_qspi_set_cmd(csp_qspi_t *ptQspiBase,qspi_tmod_e eMode,qspi_frf_e eFrf,uint8_t byPara,qspi_se_e eSlvnss);

/** \brief qspi async send
 * 
 *  \param[in] ptQspiBase: pointer of qspi register structure
 *  \param[in] pData:pointer of qspi send buff
 *  \param[in] wSize: data length
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_qspi_asyns_send(csp_qspi_t *ptQspiBase,void *pData,uint32_t wSize);

/** \brief qspi async receive
 * 
 *  \param[in] ptQspiBase: pointer of qspi register structure
 *  \param[in] pData:pointer of qspi send buff
 *  \param[in] wSize: data length
 *  \param[in] byInst: instruct
 *  \param[in] wAddr: read address
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_qspi_asyns_receive(csp_qspi_t *ptQspiBase,void *pData,uint32_t wSize,uint8_t byInst,uint32_t wAddr);

/** \brief qspi dma receive init
 * 
 *  \param[in] ptQspiBase: pointer of qspi register structure
 *  \param[in] wSize: data length
 *  \param[in] byInst: instruct
 *  \param[in] wAddr: read address
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_qspi_dma_receive_init(csp_qspi_t *ptQspiBase,uint32_t wSize,uint8_t byInst,uint32_t wAddr);

/** \brief send data of qspi by DMA
 * 
 *  \param[in] ptQspiBase: pointer of QSPI reg structure.
 *  \param[in] pData: pointer to buffer data of QSPI transmitter.
 *  \param[in] hwSize: number of data to send (byte).
 *  \param[in] ptDmaBase: pointer of DMA reg structure.
 *  \param[in] byDmaCh: channel of DMA(0 -> 5)
 *  \return  none
 */
void csi_qspi_send_dma(csp_qspi_t *ptSpiBase, const void *pData, uint16_t hwSize, csp_dma_t *ptDmaBase,uint8_t byDmaCh);

/** \brief receive data of qspi by DMA
 * 
 *  \param[in] ptQspiBase: pointer of QSPI reg structure.
 *  \param[in] pbyRecv: pointer to buffer data of QSPI receive.
 *  \param[in] hwSize: number of data to receive (byte).
 *  \param[in] ptDmaBase: pointer of DMA reg structure.
 *  \param[in] byDmaCh: channel of DMA(0 -> 5)
 *  \return  none
 */
void csi_qspi_recv_dma(csp_qspi_t *ptSpiBase, void *pbyRecv, uint16_t hwSize,csp_dma_t *ptDmaBase, uint8_t byDmaCh);



/** \brief qspi interrupt handle weak function
 * 
 *  \param[in] ptQspiBase: pointer of qspi register structure
 *  \return none
 */ 
void qspi_irqhandler(csp_qspi_t *ptQspiBase);


#ifdef __cplusplus
}
#endif

#endif /* _DRV_SPI_H_ */
