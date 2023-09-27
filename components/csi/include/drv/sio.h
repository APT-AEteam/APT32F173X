/***********************************************************************//** 
 * \file  sio.h
 * \brief  head file of csi sio
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-9-03 <td>V0.0  <td>XB   <td>initial
 * <tr><td> 2021-1-03 <td>V0.1  <td>ZJY   <td>modify
 * <tr><td> 2023-9-21 <td>V0.2  <td>ZJY   <td>code normalization 
 * </table>
 * *********************************************************************
*/

#ifndef _DRV_SIO_H_
#define _DRV_SIO_H_

#include <drv/dma.h>
#include "csp.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SIO_RESET_VALUE  	(0x00000000)
#define SIO_RX_TIMEOUT		(0x10ff)
#define SIO_TX_TIMEOUT		(0x1FFF)

//sio tx 
/**
 * \enum     csi_sio_idlest_e
 * \brief    SIO idle ouput level
 */
typedef enum{
	SIO_IDLE_HRS		= 0,	//High resistance	
	SIO_IDLE_HIGH,				//High level
	SIO_IDLE_LOW				//Low level
}csi_sio_idlest_e;

/**
 * \enum     csi_sio_tdir_e
 * \brief    SIO txbuf bit shift mode
 */
typedef enum{
	SIO_TXDIR_LSB		= 0,		
	SIO_TXDIR_MSB
}csi_sio_tdir_e;

/**
 * \enum     csi_sio_txbuf_data_e
 * \brief    SIO txbuffer data definition
 */
typedef enum
{
	SIO_TXBUF_D0     	=	0x00,  
	SIO_TXBUF_D1    	=	0x01,      
	SIO_TXBUF_DL   		=	0x02, 
	SIO_TXBUF_DH		=	0x03
}csi_sio_txbuf_data_e;   

//sio rx
/**
 * \enum     csi_sio_trgedge_e
 * \brief    SIO rx sample edge mode
 */
typedef enum{
	SIO_TRG_RISE		= 0,
	SIO_TRG_FALL,
	SIO_TRG_BOTH,
	SIO_TRG_R
}csi_sio_trgedge_e;

/**
 * \enum     csi_sio_trgmode_e
 * \brief    SIO rx sample trigger mode
 */
typedef enum{
	SIO_TRGMODE_DEB		= 0,
	SIO_TRGMODE_FIL,
}csi_sio_trgmode_e;

/**
 * \enum     csi_sio_rxpmode_e
 * \brief    SIO rx sample mode
 */
typedef enum{
	SIO_SAMPMODE_EDGE		= 0,
	SIO_SAMPMODE_NOEDGE,
}csi_sio_spmode_e;

/**
 * \enum     csi_sio_deb_e
 * \brief    SIO rx debounce period
 */
typedef enum{
	SIO_DEB_PER_1		= 0,
	SIO_DEB_PER_2,
	SIO_DEB_PER_3,
	SIO_DEB_PER_4,
	SIO_DEB_PER_5,
	SIO_DEB_PER_6,
	SIO_DEB_PER_7,
	SIO_DEB_PER_8
}csi_sio_deb_e;

/**
 * \enum     csi_sio_extract_e
 * \brief    SIO rx extract
 */
typedef enum{
	SIO_EXTRACT_0	= 0,
	SIO_EXTRACT_1,
	SIO_EXTRACT_2,
	SIO_EXTRACT_3,
	SIO_EXTRACT_4,
	SIO_EXTRACT_5,
	SIO_EXTRACT_6,
	SIO_EXTRACT_7,
	SIO_EXTRACT_8,
	SIO_EXTRACT_9,
	SIO_EXTRACT_10,
	SIO_EXTRACT_11,
	SIO_EXTRACT_12,
	SIO_EXTRACT_13,
	SIO_EXTRACT_14,
	SIO_EXTRACT_15,
	SIO_EXTRACT_16,
	SIO_EXTRACT_17,
	SIO_EXTRACT_18,
	SIO_EXTRACT_19,
	SIO_EXTRACT_20,
	SIO_EXTRACT_21,
	SIO_EXTRACT_22,
	SIO_EXTRACT_23,
	SIO_EXTRACT_24,
	SIO_EXTRACT_25,
	SIO_EXTRACT_26,
	SIO_EXTRACT_27,
	SIO_EXTRACT_28,
	SIO_EXTRACT_29,
	SIO_EXTRACT_30,
	SIO_EXTRACT_31,
	SIO_EXTRACT_HI
}csi_sio_extract_e;

/**
 * \enum     csi_sio_rdir_e
 * \brief    SIO rxbuf shift mode
 */
typedef enum{
	SIO_RXDIR_LSB	=	 0,
	SIO_RXDIR_MSB
}csi_sio_rdir_e;

/**
 * \enum     csi_sio_rdir_e
 * \brief    SIO break detect level
 */
typedef enum{
	SIO_BKLEV_LOW		= 0,
	SIO_BKLEV_HIGH
}csi_sio_bklev_e;


/**
 * \enum     csi_sio_intsrc_e
 * \brief    SIO interrupt source
 */
typedef enum
{
	SIO_INTSRC_NONE     	=	(0x00ul << 0),  
	SIO_INTSRC_TXDNE     	=	(0x01ul << 0),  
	SIO_INTSRC_RXDNE    	=	(0x01ul << 1),      
	SIO_INTSRC_TXBUFEMPT   	=	(0x01ul << 2), 
	SIO_INTSRC_RXBUFFULL	=	(0x01ul << 3), 
	SIO_INTSRC_BREAK		=	(0x01ul << 4), 
	SIO_INTSRC_TIMEOUT		=	(0x01ul << 5)
}csi_sio_intsrc_e; 


/**
 * \enum     csi_sio_state_e
 * \brief    SIO working status
 */
typedef enum {
	SIO_STATE_IDLE		= 0,	//sio idle(rx/tx)
	SIO_STATE_RECV,				//sio receiving 
	SIO_STATE_SEND,				//sio sending 
	SIO_STATE_RX_DNE,			//sio receive complete(full)
	SIO_STATE_TX_DNE,			//sio send complete
	SIO_STATE_ERROR,			//sio recv/send error
	SIO_STATE_TIMEOUT			//sio receive timeout
} csi_sio_state_e;

/**
 * \enum     csi_sio_wkmode_e
 * \brief    SIO work mode(rx/tx)
 */
typedef enum{
	SIO_SEND	=	0,			//sio (tx)send mode 
	SIO_RECV	=	1,			//sio (rx)receive mode
}csi_sio_wkmode_e;

/**
 * \enum     csi_sio_callback_id_e
 * \brief    SIO callback id
 */
typedef enum{
	SIO_CALLBACK_RECV	=	0,		//sio rteceive callback id
	SIO_CALLBACK_SEND,				//sio send callback id
	SIO_CALLBACK_ERR,				//sio error callback id
}csi_sio_callback_id_e;
		
/**
 * \enum     csi_sio_mode_e
 * \brief    SIO send/receive 
 */
//typedef enum{
//	SIO_SEND_MODE		=	0,	//send(tx) mode
//	SIO_RECV_MODE		=	1,	//receive(rx) mode
//}csi_sio_mode_e;

/// \struct csi_sio_tx_config_t
/// \brief  sio parameter configuration, open to users  
typedef struct {
	uint8_t				byD0Len;	//D0 send length (1~8)个bit, all 0 sequence(out low(0)) 
	uint8_t				byD1Len;	//D1 send length (1~8)个bit, all 1 sequence(out high(1)) 
	uint8_t				byDLLen;	//DL send length (1~8)个bit
	uint8_t				byDHLen;	//DH send length (1~8)个bit
	uint8_t				byDLLsq;	//DL data sequence (0~0xff),send sequence bit0 - bitdatahlen
	uint8_t				byDHHsq;	//DH data sequence (0~0xff),send sequence bit0 - bitdatallen
	uint8_t				byTxCnt;	//sio tx bit count, Mux Num = 256bit(32bytes)	 
	uint8_t				byTxBufLen;	//sio tx buffer length, Max Len = 16bit(2bytes)	
	csi_sio_tdir_e		eTxDir; 	//sio output dir, LSB OR MSB
	csi_sio_idlest_e	eIdleLev;	//idle sio ouput state	
	uint32_t			wTxFreq;	//sio tx frequency 
} csi_sio_tx_config_t;


/// \struct csi_sio_tx_config_t
/// \brief  sio parameter configuration, open to users  
typedef struct {

	csi_sio_trgedge_e	eTrgEdge;		//receive samping trigger edge
	csi_sio_trgmode_e	eTrgMode;		//receive samping trigger mode
	csi_sio_spmode_e	eSampMode;		//receive samping mode
	csi_sio_extract_e	eSampExtra;		//receive samping extract select
	csi_sio_rdir_e		eRxDir;			//sio receive dir, LSB OR MSB
	uint8_t				byDebLen;		//debounce period length, (1~8)period
	uint8_t				byDebClkDiv;	//debounce clk div
	uint8_t				byHiThres;		//extract high Threshold 
	uint8_t				bySampBitLen;	//receive samping one bit count length
	uint8_t				byRxCnt;		//sio rx bit count, Mux Num = 256bit(32bytes)	 
	uint8_t				byRxBufLen;		//sio rx buffer length, Max Len = 32bit(4bytes)
	uint32_t			wRxFreq;		//sio rx samping frequency 
} csi_sio_rx_config_t;

/// \struct csi_sio_transfer_t
/// \brief  sio transport handle, not open to users  
typedef struct {
	
	uint32_t		*pwData;			//transport data buffer
	uint16_t        hwSize;				//transport data buffer size
	uint16_t        hwTransNum;			//transport data size
	uint8_t			byRxStat;			//sio receive status
	uint8_t			byTxStat;			//sio send status
	//CallBack		
	void(*recv_callback)(csp_sio_t *ptSioBase, uint8_t byIsr, uint32_t *pwBuf, uint16_t hwSzie);
	void(*send_callback)(csp_sio_t *ptSioBase);
	void(*err_callback)(csp_sio_t *ptSioBase, uint8_t byIsr);
} csi_sio_ctrl_t;

extern csi_sio_ctrl_t g_tSioCtrl[SIO_IDX];	


/** 
  \brief  	   register sio interrupt callback function
  \param[in]   ptSioBase		pointer of sio register structure
  \param[in]   eCallBkId		sio interrupt callback type, \ref csi_uart_callback_id_e
  \param[in]   callback			sio interrupt handle function
  \return 	   error code \ref csi_error_t
 */ 
csi_error_t csi_sio_register_callback(csp_sio_t *ptSioBase, csi_sio_callback_id_e eCallBkId, void  *callback);

/** 
  \brief 	   sio interrupt handler function
  \param[in]   ptSioBase		pointer of uart sio structure
  \param[in]   byIdx			sio idx(0/1)
  \return none
 */ 
void csi_sio_irqhandler(csp_sio_t *ptSioBase, uint8_t byIdx);

/**
  \brief       configure sio tx
               configure the resources needed for the sio instance
  \param[in]   ptSioBase		pointer of sio register structure
  \param[in]   ptTxCfg    		pointer of sio parameter config structure
  				- byD0Len		D0 send length (1~8)个bit, all 0 sequence(out low(0))
  				- byD1Len		D1 send length (1~8)个bit, all 1 sequence(out high(1))
  				- byDLLen		DL send length (1~8)个bit
  				- byDHLen		DH send length (1~8)个bit
  				- byDLLsq		DL data sequence (0~0xff),send sequence bit0 - bitdatahlen
  				- byDHHsq		DH data sequence (0~0xff),send sequence bit0 - bitdatallen
  				- byTxCnt		sio tx bit count, Mux Num = 256bit(32bytes)
  				- byTxBufLen	sio tx buffer length, Max Len = 16bit(2bytes)	
  				- eTxDir:		sio output dir, LSB OR MSB
  				- eIdleLev		idle sio ouput state
 				- wTxFreq		sio tx frequency 
  \return      error code \ref csi_error_t
*/
csi_error_t csi_sio_tx_init(csp_sio_t *ptSioBase, csi_sio_tx_config_t *ptTxCfg);

/**
  \brief       configure sio tx
               configure the resources needed for the sio instance
  \param[in]   ptSioBase		pointer of sio register structure
  \param[in]   ptTxCfg    		pointer of sio parameter config structure
				- eTrgEdge		receive samping trigger edge	
  				- eTrgMode		receive samping trigger mode	
  				- eSampMode		receive samping mode
  				- eSampExtra	receive samping extract select
  				- eRxDir		sio receive dir, LSB OR MSB
  				- byDebLen		debounce period length, (1~8)period
  				- byDebClkDiv	debounce clk div
 				- byHiThres		extract high Threshold 
  				- bySpBitLen	receive samping one bit count length
  				- byRxCnt		sio rx bit count, Mux Num = 256bit(32bytes)
  				- byRxBufLen	sio rx buffer length, Max Len = 32bit(4bytes)
  				- wRxFreq		sio rx sampingfrequency 
  \return      error code \ref csi_error_t
*/
csi_error_t csi_sio_rx_init(csp_sio_t *ptSioBase, csi_sio_rx_config_t *ptRxCfg);

/** 
  \brief 	   sio work mode set,send(tx)/receive(rx)
  \param[in]   ptSioBase	pointer of sio register structure
  \param[in]   eWorkMd		sio working mode, \ref csi_dma_ch_e
  \return 	   none
*/
void csi_sio_set_wkmode(csp_sio_t *ptSioBase, csi_sio_wkmode_e eWorkMd);

/** 
  \brief 	   enable sio interrupt 
  \param[in]   ptSioBase	pointer of sio register structure
  \param[in]   eIntSrc		sio interrupt source, \ref csi_sio_intsrc_e
  \return 	   none
 */
void csi_sio_int_enable(csp_sio_t *ptSioBase, csi_sio_intsrc_e eIntSrc);

/** 
  \brief 	   disable sio interrupt 
  \param[in]   ptSioBase	pointer of sio register structure
  \param[in]   eIntSrc		sio interrupt source, \ref csi_sio_intsrc_e
  \return 	   none
 */
void csi_sio_int_disable(csp_sio_t *ptSioBase, csi_sio_intsrc_e eIntSrc);

/** 
  \brief 	   clear sio interrupt status
  \param[in]   ptSioBase	pointer of sio register structure
  \param[in]   eIntSrc		sio interrupt status, \ref csi_sio_intsrc_e
  \return 	   none
 */
void csi_sio_clr_isr(csp_sio_t *ptSioBase, csi_sio_intsrc_e eIntSrc);

/**
  \brief       sio receive break reset config
  \param[in]   ptSioBase	pointer of sio register structure
  \param[in]   eBkLev    	break reset detect level, \ref csi_sio_bklev_e
  \param[in]   byBkCnt    	break reset detect period
  \param[in]   bEnable    	ENABLE/DISABLE break reset
  \return      error code \ref csi_error_t
*/
csi_error_t csi_sio_set_break(csp_sio_t *ptSioBase, csi_sio_bklev_e eBkLev, uint8_t byBkCnt, bool bEnable);

/**
  \brief       sio receive sample timeout reset config
  \param[in]   ptSioBase	pointer of sio register structure
  \param[in]   byToCnt    	sample timeout period
  \param[in]   bEnable    	ENABLE/DISABLE sample timeout reset
  \return      error code \ref csi_error_t
*/
csi_error_t csi_sio_set_timeout(csp_sio_t *ptSioBase, uint8_t byToCnt ,bool bEnable);

/**
  \brief	   send data from sio, this function is polling  
  \param[in]   ptSioBase	pointer of sio register structure
  \param[in]   pwData    	pointer to buffer with data to send 
  \param[in]   hwSize    	send data size
  \return      error code \ref csi_error_t or data size
*/
int32_t csi_sio_send(csp_sio_t *ptSioBase, const uint32_t *pwData, uint16_t hwSize);

/** 
  \brief 	   send data from sio, this function is interrupt mode    
  \param[in]   ptSioBase	pointer of sio register structure
  \param[in]   pwData		pointer to buffer with data to send 
  \param[in]   hwSize		send data size
  \return error code \ref csi_error_t or receive data size
 */
csi_error_t csi_sio_send_int(csp_sio_t *ptSioBase, const uint32_t *pwSend, uint16_t hwSize);

/**
  \brief       sio receive data, use rxbuffull interrupt 
  \param[in]   ptSioBase	pointer of sio register structure
  \param[in]   pwRecv		pointer of sio receive data
  \param[in]   hwLen		length receive data
  \return      error code \ref csi_error_t or receive data len
*/
csi_error_t csi_sio_receive_rxfull_int(csp_sio_t *ptSioBase, uint32_t *pwRecv, uint16_t hwSize);

/**
  \brief       sio receive data, use rxdne interrupt 
  \param[in]   ptSioBase	pointer of sio register structure
  \param[in]   pwRecv		pointer of sio receive data
  \param[in]   hwLen		length receive data
  \return      error code \ref csi_error_t or receive data len
*/
csi_error_t csi_sio_receive_rxdne_int(csp_sio_t *ptSioBase, uint32_t *pwRecv, uint16_t hwSize);

/** 
  \brief 	   receive data to sio transmitter, asynchronism mode
  \param[in]   ptSioBase	pointer of sio register structure
  \param[in]   pwRecv		pointer of sio receive data
  \param[in]   hwSize		receive data size
  \return 	   error code \ref csi_error_t or receive data len
 */
int32_t csi_sio_receive(csp_sio_t *ptSioBase, uint32_t *pwRecv, uint16_t hwSize);


/** 
  \brief 	   sio send dma enable
  \param[in]   ptSioBase	pointer of sio register structure
  \return  	   none
 */
void csi_sio_send_dma_enable(csp_sio_t *ptSioBase);

/** 
  \brief 	   sio receive dma enable
  \param[in]   ptSioBase	pointer of sio register structure
  \return  	   none
 */
void csi_sio_receive_dma_enable(csp_sio_t *ptSioBase);

/** 
  \brief 	   send data from sio, this function is dma mode
  \param[in]   ptSioBase	pointer of sio register structure
  \param[in]   ptDmaBase	pointer of dma register structure
  \param[in]   eDmaCh		channel number of dma, \ref csi_dma_ch_e
  \param[in]   pData		pointer to buffer with data to send to uart transmitter.
  \param[in]   hwSize		number of data to send (byte); hwSize <= 0xfff
  \return error code \ref csi_error_t
 */
csi_error_t csi_sio_send_dma(csp_sio_t *ptSioBase, csp_dma_t *ptDmaBase, csi_dma_ch_e eDmaCh, const void *pData, uint16_t hwSize);

/** 
  \brief 	   send data from sio, this function is dma mode
  \param[in]   ptSioBase	pointer of sio register structure
  \param[in]   ptDmaBase	pointer of dma register structure
  \param[in]   eDmaCh		channel number of dma, \ref csi_dma_ch_e
  \param[in]   pData		pointer to buffer with data to send to uart transmitter.
  \param[in]   hwSize		number of data to send (byte); hwSize <= 0xfff
  \return error code \ref csi_error_t
 */
csi_error_t csi_sio_receive_dma(csp_sio_t *ptSioBase, csp_dma_t *ptDmaBase, csi_dma_ch_e eDmaCh, const void *pData, uint16_t hwSize);


#ifdef __cplusplus
}
#endif

#endif 
