/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     usart.h
 * @brief    header file for usart driver
 * @version  V1.0
 * @date     08. Apr 2020
 * @model    usart
 ******************************************************************************/

#ifndef _DRV_USART_H_
#define _DRV_USART_H_

#include <drv/etb.h>
#include <drv/dma.h>
#include <tick.h>
#include <csp.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \enum     csi_usart_clksrc_e
 * \brief    USART Clk Source
 */
typedef enum {
    USART_CLKSRC_DIV1	= 0,	 //CLK = PCLK
    USART_CLKSRC_DIV8 ,          //CLK = PCLK/8
	USART_CLKSRC_CK0  = 2        //CLK = USART0_CK pin
} csi_usart_clksrc_e;

/**
 * \enum     csi_usart_mode_e
 * \brief    USART Mode async/sync
 */
typedef enum {
    USART_MODE_ASYNC	= 0,	//async mode
    USART_MODE_SYNC          	//sync mode
} csi_usart_mode_e;

/**
 * \enum     csi_usart_data_bits_e
 * \brief    USART Mode Parameters: Data Bits
 */
typedef enum {
    USART_DATA_BITS_5	= 0,	//5 Data bits
    USART_DATA_BITS_6,          //6 Data bit
    USART_DATA_BITS_7,          //7 Data bits
    USART_DATA_BITS_8,          //8 Data bits (default)
    USART_DATA_BITS_9           //9 Data bits
} csi_usart_data_bits_e;

/**
 * \enum     csi_usart_parity_e
 * \brief    USART Mode Parameters: Parity
 */
typedef enum {
    
    USART_PARITY_EVEN = 0,          //Even Parity
    USART_PARITY_ODD,               //Odd Parity
	USART_PARITY_ZERO,			    //0 Parity
	USART_PARITY_ONE,               //1 Parity
	USART_PARITY_NONE,        		//No Parity (default) 4/5
	USART_PARITY_MD   = 6,     		//multi-drop mode 6/7
} csi_usart_parity_e;

/**
 * \enum     csi_usart_stop_bits_e
 * \brief    USART Mode Parameters: Stop bits
 */
typedef enum {
    USART_STOP_BITS_1	= 0,    //1 Stop bit (default)
	//USART_STOP_BITS_1_5,      //1.5 Stop bits for async mode, reserved for sync mode
    USART_STOP_BITS_2  = 2 ,    //2 Stop bits
    
} csi_usart_stop_bits_e;

/**
 * \enum     csi_usart_flowctrl_e
 * \brief    USART Mode Parameters:  Flow Control
 */
typedef enum {
    USART_FLOWCTRL_NONE	= 0,    //none flowctrl
    USART_FLOWCTRL_RTS,         //RTS
    USART_FLOWCTRL_CTS,         //CTS
    USART_FLOWCTRL_RTS_CTS      //RTS & CTS
} csi_usart_flowctrl_e;

/**
 * \enum     csi_uart_rxfifo_e
 * \brief    rx fifo trigger point
 */
typedef enum{
	USART_RXFIFOTRG_ONE		= 1,
	USART_RXFIFOTRG_TWO		= 2,
	USART_RXFIFOTRG_FOUR	= 4
}csi_usart_rxfifo_trg_e;


/**
 * \enum     csi_usart_func_e
 * \brief    USART tx/rx function
 */
typedef enum{
	USART_FUNC_RX		= 0,		//uart only support rx 
	USART_FUNC_TX,					//uart only support tx 
	USART_FUNC_RX_TX				//uart support rx and tx 
}csi_usart_func_e;

/**
 * \enum     csi_usart_wkmode_e
 * \brief    USART wort mode(rx/tx)
 */
typedef enum{
	USART_SEND	=	0,			//usart send 
	USART_RECV	=	1,			//usart receive
	USART_RTO					//usart receive
}csi_usart_wkmode_e;

/**
 * \enum     csi_usart_event_e
 * \brief    USART working event(state)
 */
typedef enum {
	USART_EVENT_IDLE		= 0,		//usart idle(rx/tx)
	USART_EVENT_RECV,					//usart receiving 
	USART_EVENT_SEND,					//usart sending 
	USART_EVENT_RX_TO,					//usart receive timeout(dynamic end)
	USART_EVENT_RX_DNE,					//usart receive complete
	USART_EVENT_TX_DNE,					//usart send complete
	USART_EVENT_RX_BREAK,				//usart receive break
	USART_EVENT_PAR_ERR,				//usart PARITY ERROR
} csi_usart_event_e;

typedef csi_usart_event_e  csi_usart_state_e ;



typedef enum{
	USDMA_TX_FIFO_NFULL  = 0,
	USDMA_TX_FIF0_TRG
}csi_usdma_txfifo_md_e;

typedef enum{
	USDMA_RX_FIFO_NSPACE  = 0,
	USDMA_RX_FIF0_TRG
}csi_usdma_rxfifo_md_e;
/**
 * \enum     csi_usart_intsrc_e
 * \brief    USART interrupt source 
 */
typedef enum
{
	USART_INTSRC_NONE 			= (0x00ul << 0),		//USART none interrupt
	USART_INTSRC_RXRDY   		= (0x01ul << 0), 		//RXRDY interrupt
	USART_INTSRC_TXRDY  		= (0x01ul << 1),		//TXRDY interrupt
	USART_INTSRC_RXBRK   		= (0x01ul << 2),		//RXBRK interrupt
	USART_INTSRC_OVRE   		= (0x01ul << 5),		//OVER interrupt
	USART_INTSRC_FRAME_ERR		= (0x01ul << 6),		//FRAME ERROR interrupt
	USART_INTSRC_PARE_ERR  		= (0x01ul << 7),		//PARE ERROR interrupt
	USART_INTSRC_TIMEOUT  		= (0x01ul << 8),		//TIMEOUT interrupt
	USART_INTSRC_TXEMPTY		= (0x01ul << 9),		//TXEMPTY OVER interrupt   
	USART_INTSRC_IDLE   		= (0x01ul << 10), 		//IDLE interrupt
	USART_INTSRC_RXFIFO   		= (0x01ul << 12), 		//RX FIFO interrupt   
	USART_INTSRC_RXFIFO_OV  	= (0x01ul << 13), 		//RX FIFO OVER interrupt 
	USART_INTSRC_TXFIFO   		= (0x01ul << 14) 		//TX FIFO interrupt  
}csi_usart_intsrc_e;


/**
 * \enum     csi_usart_callbackid_e
 * \brief    USART callback id
 */
typedef enum{
	USART_CALLBACK_ID_RECV	=	0,		//usart rteceive callback id
	USART_CALLBACK_ID_SEND,				//usart send callback id
	USART_CALLBACK_ID_ERR,				//usart error callback id
}csi_usart_callback_id_e;



/// \struct csi_usart_config_t
/// \brief  usart parameter configuration, open to users  
typedef struct {
	uint32_t           	 		wBaudRate;			//baud rate	
	uint16_t					hwRecvTo;			//receive timeout
	uint8_t						byParity;           //parity type 
	uint8_t						byDatabit;			//data bits
	uint8_t						byStopbit;			//stop bits
	uint8_t						byClkSrc;			//clk source
	uint8_t						byMode;				//usart mode, sync/async
	csi_usart_rxfifo_trg_e 		eRxFifoTrg;     	//rxfifo Trigger point

} csi_usart_config_t;



/// \struct csi_usart_ctrl_t
/// \brief  usart transport handle, not open to users  
typedef struct {
	uint8_t				byTxState;			//send status
	uint8_t				byRxState;			//receive status
	uint16_t            hwTxSize;			//tx data size
	uint16_t            hwRxSize;			//rx data size
	uint8_t				*pbyTxBuf;			//pointer of Tx buf 
	uint8_t				*pbyRxBuf;			//pointer of Rx buf 
	uint16_t			hwTransNum;			//send/receive data num	

	void(*recv_callback)(csp_usart_t *ptUsartBase, csi_usart_event_e eEvent, uint8_t *pbyBuf, uint16_t *hwSzie);
	void(*send_callback)(csp_usart_t *ptUsartBase);
	void(*err_callback)(csp_usart_t *ptUsartBase, csi_usart_event_e eEvent);

} csi_usart_ctrl_t;

extern csi_usart_ctrl_t g_tUsartCtrl[USART_IDX];	

/**
  \brief       initializes the resources needed for the USART interface.
  \param[in]   ptUsartBase	pointer of usart register structure
  \param[in]   ptUartCfg    pointer of usart parameter config structure
  \return      error code.
*/
csi_error_t csi_usart_init(csp_usart_t *ptUsartBase, csi_usart_config_t *ptUartCfg);

/** \brief  register usart interrupt callback function
 * 
 *  \param[in] ptUsartBase: pointer of uart register structure
 *  \param[in] eCallBkId: usart interrupt callback type, \ref csi_usart_callback_id_e
 *  \param[in] callback: usart interrupt handle function
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_usart_register_callback(csp_usart_t *ptUsartBase, csi_usart_callback_id_e eCallBkId, void  *callback);

/** \brief USART中断函数，接收数据使用中断方式(FIFO/RX两种中断)，在此中断函数中接收数据
 * 
 *  \param[in] ptUsartBase: pointer of usart register structure
 *  \param[in] byIdx: usart id number(0)
 */
void csi_usart_irqhandler(csp_usart_t *ptUsartBase,uint8_t byIdx);

/** 
  \brief 	   enable/disable usart interrupt 
  \param[in]   ptSioBase	pointer of usart register structure
  \param[in]   eIntSrc		usart interrupt source
  \param[in]   bEnable		enable/disable interrupt
  \return 	   none
*/

void csi_usart_int_enable(csp_usart_t *ptUsartBase, csi_usart_intsrc_e eIntSrc);

/** \brief disable usart interrupt 
 * 
 *  \param[in] ptSioBase: pointer of usart register structure
 *  \param[in] eIntSrc: usart interrupt source
 *  \return none
 */
void csi_usart_int_disable(csp_usart_t *ptUsartBase, csi_usart_intsrc_e eIntSrc);

/** 
  \brief 	   start(enable) usart rx/tx
  \param[in]   ptUsartBase	pointer of usart register structure
  \param[in]   eFunc: rx/tx function, \ref csi_usart_func_e
  \return      error code \ref csi_error_t
 */ 
csi_error_t csi_usart_start(csp_usart_t *ptUsartBase, csi_usart_func_e eFunc);


/** 
  \brief 	   stop(disable) usart rx/tx
  \param[in]   ptUsartBase	pointer of usart register structure
  \param[in]   eFunc: rx/tx function, \ref csi_usart_func_e
  \return      error code \ref csi_error_t
 */ 
csi_error_t csi_usart_stop(csp_usart_t *ptUsartBase, csi_usart_func_e eFunc);

/**
  \brief       Start send data to USART transmitter, this function is blocking.
  \param[in]   usart     	usart handle to operate.
  \param[in]   data     	pointer to buffer with data to send to USART transmitter.
  \param[in]   size     	number of data to send (byte).
  \param[in]   timeout  	the timeout between bytes(ms).
  \return      the num of data which is sent successfully or CSI_ERROR.
*/
int16_t csi_usart_send(csp_usart_t *ptUsartBase, const void *pData, uint16_t hwSize);

/** \brief usart dma send mode init
 * 
 *  \param[in] ptUsartBase: pointer of usart register structure
 *  \param[in] eTxDmaMode: usart dma tx mode \ref csi_usdma_txfifo_md_e
 *  \param[in] bEnable: ENABLE/DISABLE
 *  \return  error code \ref csi_error_t
 */
void csi_usart_set_txdma(csp_usart_t *ptUsartBase, csi_usdma_txfifo_md_e eTxDmaMode , bool bEnable); 

/** \brief usart dma rx mode set
 * 
 *  \param[in] ptUsartBase: pointer of usart register structure
 *  \param[in] eRxDmaMode: usart dma tx mode \ref csi_usdma_rxfifo_md_e
 *  \param[in] bEnable: ENABLE/DISABLE
 *  \return  error code \ref csi_error_t
 */
void csi_usart_set_rxdma(csp_usart_t *ptUsartBase, csi_usdma_rxfifo_md_e eRxDmaMode , bool bEnable); 

/** \brief send data from usart, this function is dma transfer
 * 
 *  \param[in] ptUartBase: pointer of usart register structure
 *  \param[in] ptDmaBase: pointer of dma register structure
 *  \param[in] pData: pointer to buffer with data to send to usart transmitter.
 *  \param[in] hwSize: number of data to send (byte), hwSize <= 0xfff.
 *  \return  error code \ref csi_error_t
 */
csi_error_t csi_usart_send_dma(csp_usart_t *ptUsartBase,  const void *pData, uint8_t byDmaCh, uint16_t hwSize);

/** 
  \brief 	   receive data to usart transmitter, this function is dma mode
  \param[in]   ptUsartBase	pointer of usart register structure
  \param[in] ptDmaBase: pointer of dma register structure
  \param[in]   pData		pointer to buffer with data to send to usart transmitter.
  \param[in]   wSize		number of data to send (byte), hwSize <= 0xfff.
  \return      error code \ref csi_error_t
 */
csi_error_t csi_usart_recv_dma(csp_usart_t *ptUsartBase, void *pData, uint8_t byDmaCh, uint16_t hwSize);

/**
  \brief       Query data from USART receiver FIFO, this function is blocking.
  \param[in]   ptUsartBase   pointer of usart register structure
  \param[out]  data     	pointer to buffer for data to receive from USART receiver.
  \param[in]   size     	number of data to receive(byte).
  \param[in]   timeout  	the timeout between bytes(ms).
  \return      the num of data witch is received successfully or CSI_ERROR.
*/
int32_t csi_usart_receive(csp_usart_t *ptUsartBase, void *pData, uint16_t hwSize, uint32_t wTimeOut);


/** \brief usart send data with int mode.
 * 
 *  \param[in] ptUsartBase: pointer of usart register structure
 *  \param[in] pData: pointer to buffer with data to send by usart transmitter.
 *  \param[in] hwSize: number of data to send (byte).
 *  \return   CSI_ERROR/CSI_OK
 */
int16_t csi_usart_send_int(csp_usart_t *ptUsartBase, const void *pData, uint16_t hwSize);

/** \brief receive data from usart, this function is interrupt receive
 * 
 *  \param[in] ptUsartBase: pointer of usart register structure
 *  \param[in] pData: pointer to buffer with data to be received.
 *  \param[in] hwSize: number of data to receive (byte).
 *  \return  error code \ref csi_error_t
 */	
csi_error_t csi_usart_receive_int(csp_usart_t *ptUsartBase, void *pData, uint16_t hwSize);

/**
  \brief       Get character in query mode.
  \param[in]   ptUsartBase   pointer of usart register structure
  \return      the character to get.
*/
uint8_t csi_usart_getc(csp_usart_t *ptUsartBase);

/**
  \brief       Send character in query mode.
  \param[in]   ptUsartBase   pointer of usart register structure
  \param[in]   byData   	the character to be send.
  \return      none
*/
void csi_usart_putc(csp_usart_t *ptUsartBase, uint8_t byData);

/** 
  \brief 	   get usart receive/send complete message and (Do not) clear message
  \param[in]   ptUsartBase	pointer of usart reg structure.
  \param[in]   eWkMode		tx or rx mode
  \param[in]   bClrEn: clear usart receive/send complete message enable; ENABLE: clear , DISABLE: Do not clear 
  \return  	   bool type true/false
 */ 
bool csi_usart_get_msg(csp_usart_t *ptUsartBase, csi_usart_wkmode_e eWkMode, bool bClrEn);

/** 
  \brief 	   clr usart receive/send status message (set usart recv/send status idle) 
  \param[in]   ptUsartBase	pointer of usart reg structure.
  \param[in]   eWkMode		tx or rx mode
  \return      none
 */ 
void csi_usart_clr_msg(csp_usart_t *ptUsartBase, csi_usart_wkmode_e eWkMode);

#ifdef __cplusplus
}
#endif

#endif /* _DRV_USART_H_ */
