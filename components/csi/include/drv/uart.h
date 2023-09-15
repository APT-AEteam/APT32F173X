/***********************************************************************//** 
 * \file  uart.h
 * \brief  head file of csi uart 
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author	<th>Description
 * <tr><td> 2020-8-12 <td>V0.0 <td>ZJY   	<td>initial
 * </table>
 * *********************************************************************
*/

#ifndef _DRV_UART_H_
#define _DRV_UART_H_

#include "drv/dma.h"
#include "csp.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \enum     csi_uart_data_bits_e
 * \brief    UART Mode Parameters: Data Bits
 */
typedef enum {
    UART_DATA_BITS_5	= 0,			//5 Data bits
    UART_DATA_BITS_6,                   //6 Data bit
    UART_DATA_BITS_7,                   //7 Data bits
    UART_DATA_BITS_8,                   //8 Data bits (default)
    UART_DATA_BITS_9                    //9 Data bits
} csi_uart_data_bits_e;

/**
 * \enum     csi_uart_parity_e
 * \brief    UART Mode Parameters: Parity
 */
typedef enum {
    UART_PARITY_NONE	= 0,    		//No Parity (default)
    UART_PARITY_EVEN,                   //Even Parity
    UART_PARITY_ODD,                    //Odd Parity
} csi_uart_parity_e;

/**
 * \enum     csi_uart_stop_bits_e
 * \brief    UART Mode Parameters: Stop bits
 */
typedef enum {
    UART_STOP_BITS_1	= 0,    		//1 Stop bit (default)
    UART_STOP_BITS_2,                   //2 Stop bits
    UART_STOP_BITS_1_5,                 //1.5 Stop bits
} csi_uart_stop_bits_e;

/**
 * \enum     csi_uart_flowctrl_e
 * \brief    UART Mode Parameters:  Flow Control
 */
//typedef enum {
//    UART_FLOWCTRL_NONE	= 0,    		//none flowctrl
//    UART_FLOWCTRL_RTS,                  //RTS
//    UART_FLOWCTRL_CTS,                  //CTS
//    UART_FLOWCTRL_RTS_CTS               //RTS & CTS
//} csi_uart_flowctrl_e;

/**
 * \enum     csi_uart_rxfifo_e
 * \brief    rx fifo trigger point
 */
typedef enum{
	UART_RXFIFOTRG_ONE	= 1,
	UART_RXFIFOTRG_TWO	= 2,
	UART_RXFIFOTRG_FOUR	= 4
}csi_uart_rxfifo_trg_e;

/**
 * \enum     csi_uart_func_e
 * \brief    UART tx/rx function
 */
typedef enum{
	UART_FUNC_RX		= 0,		//uart only support rx 
	UART_FUNC_TX,					//uart only support tx 
	UART_FUNC_RX_TX					//uart support rx and tx 
}csi_uart_func_e;

/**
 * \enum     csi_uart_wkmode_e
 * \brief    UART work mode(rx/tx)
 */
typedef enum{
	UART_SEND	=	0,				//uart send 
	UART_RECV,						//uart receive
	UART_RTO						//uart receive
}csi_uart_wkmode_e;

/**
 * \enum     csi_uart_event_e
 * \brief    UART working event(state)
 */
typedef enum {
	UART_STATE_IDLE		= 0,		//uart idle(rx/tx)
	UART_STATE_RECV,				//uart receiving 
	UART_STATE_SEND,				//uart sending 
	UART_STATE_RX_TO,				//uart receive timeout(dynamic end)
	UART_STATE_RX_DNE,				//uart receive complete
	UART_STATE_TX_DNE				//uart send complete
} csi_uart_state_e;


/**
 * \enum     csi_uart_intsrc_e
 * \brief    UART interrupt source 
 */
typedef enum
{
	UART_INTSRC_NONE 		= (0x00ul << 0),		//UART none interrupt
	UART_INTSRC_TX   		= (0x01ul << 2), 		//TX interrupt
	UART_INTSRC_RX  		= (0x01ul << 3),		//RX interrupt
	UART_INTSRC_TX_OV   	= (0x01ul << 4),		//TX OVER interrupt
	UART_INTSRC_RX_OV   	= (0x01ul << 5),		//RX OVER interrupt
	UART_INTSRC_PAR_ERR		= (0x01ul << 7),		//PARITY ERROR interrupt
	UART_INTSRC_TXFIFO  	= (0x01ul << 12),		//TX FIFO interrupt
	UART_INTSRC_RXFIFO  	= (0x01ul << 13),		//RX FIFO interrupt
	UART_INTSRC_RXFIFO_OV	= (0x01ul << 18),		//RX FIFO OVER interrupt   
	UART_INTSRC_TXDONE   	= (0x01ul << 19), 		//TX DONE interrupt
	UART_INTSRC_RXTO   		= (0x01ul << 21), 		//RX TIME OUT
	UART_INTSRC_RXBRK  		= (0x01ul << 22) 		//RX BREAK
}csi_uart_intsrc_e;

typedef enum{
	UART_INTSTA_TX  		= (0x01ul << 0),	//Transmitter INT Status      
	UART_INTSTA_RX			= (0x01ul << 1),	//Receiver INT Status          
	UART_INTSTA_TX_OV   	= (0x01ul << 2),	//Transmitter Over INT Status  
	UART_INTSTA_RX_OV  	 	= (0x01ul << 3),	//Receiver Over INT Status   
	UART_INTSTA_PARERR    	= (0x01ul << 4),	//Parity Error INT Status  
	UART_INTSTA_TXFIFO    	= (0x01ul << 5),	//Transmitter FIFO INT Status      
	UART_INTSTA_RXFIFO    	= (0x01ul << 6),	//Receiver FIFO INT Status 
	UART_INTSTA_RXFIFO_OV_	= (0x01ul << 7),	//Receiver FIFO Over INT Status 
	UART_INTSTA_RXTO    	= (0x01ul << 9),	//Receiver FIFO INT Status 
	UART_INTSTA_RXBRK_		= (0x01ul << 10),	//Receiver FIFO Over INT Status 
	UART_INTSTA_TXDONE    	= (0x01ul << 19),	//Transmitter Complete INT Status
	UART_INTSTA_ALL    		= (0x806FFul << 0)	//All INT Status 
}csi_uart_intsta_e;

/**
 * \enum     csi_uart_dma_md_e
 * \brief    UART dma mode 
 */
typedef enum{
	//rx
	UART_DMA_RXFIFO_NSPACE 	= 0,	//RXFIFO no space
	UART_DMA_RXFIFO_TRG		= 1,	//RXFIFO receive trigger piont
	//tx
	UART_DMA_TXFIFO_NFULL 	= 0,	//TXFIFO no full
	UART_DMA_TXFIFO_TRG		= 1		//TXFIFO <= 1/2
}csi_uart_dma_md_e;

/**
 * \enum     csi_uart_callbackid_e
 * \brief    UART callback id
 */
typedef enum{
	UART_CALLBACK_RECV	=	0,		//uart rteceive callback id
	UART_CALLBACK_SEND,				//uart send callback id
	UART_CALLBACK_ERR,				//uart error callback id
}csi_uart_callback_id_e;

/// \struct csi_uart_config_t
/// \brief  uart parameter configuration, open to users  
typedef struct {
	uint32_t            	wBaudRate;		//baud rate	
	uint16_t				hwRecvTo;		//receive timeout
	csi_uart_rxfifo_trg_e 	eRxFifoTrg;     //rxfifo Trigger point 
	csi_uart_parity_e		eParity;        //parity type 
} csi_uart_config_t;

/// \struct csi_uart_ctrl_t
/// \brief  uart control handle, not open to users  
typedef struct {
	uint16_t            hwTxSize;			//tx send data size
	uint16_t            hwRxSize;			//rx receive data size
	uint8_t				*pbyTxBuf;			//pointer of send buf 
	uint8_t				*pbyRxBuf;			//pointer of recv buf 
	uint16_t			hwTransNum;			//send/receive data num
	uint8_t 			byTxState;			//send state
	uint8_t 			byRxState;			//receive state	
	//CallBack		
	void(*recv_callback)(csp_uart_t *ptUartBase, csi_uart_state_e eState, uint8_t *pbyBuf, uint16_t *hwSzie);
	void(*send_callback)(csp_uart_t *ptUartBase);
	void(*err_callback)(csp_uart_t *ptUartBase, uint16_t hwIsr);
} csi_uart_ctrl_t;

extern csi_uart_ctrl_t g_tUartCtrl[UART_IDX];	


/**
  \brief       initializes the resources needed for the UART interface.
  \param[in]   ptUartBase	pointer of uart register structure
  \param[in]   ptUartCfg    pointer of uart parameter config structure
  \return      error code.
*/
/** 
  \brief 	   initialize uart parameter structure
  \param[in]   ptUartBase	pointer of uart register structure
  \param[in]   ptUartCfg	pointer of uart parameter config structure
				- wBaudRate	baud rate
				- hwRecvTo	rx byte timeout
				- eParity	parity bit, \ref csi_uart_rxfifo_trg_e
				- eRxFifoTrg rx FIFO level to trigger UART_RNE interrupt, \ref csi_uart_rxfifo_trg_e
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_uart_init(csp_uart_t *ptUartBase, csi_uart_config_t *ptUartCfg);

/** 
  \brief  	   register uart interrupt callback function
  \param[in]   ptUartBase	pointer of uart register structure
  \param[in]   eCallBkId	receive/error callback id, \ref csi_uart_callback_id_e
  \param[in]   callback		uart interrupt handle function
  \return 	   error code \ref csi_error_t
 */ 
csi_error_t csi_uart_register_callback(csp_uart_t *ptUartBase, csi_uart_callback_id_e eCallBkId, void  *callback);

/** 
  \brief  	   uart send interrupt callback function
  \param[in]   ptUartBase	pointer of uart register structure
  \return 	   none
 */ 
void csi_uart_send_callback(csp_uart_t *ptUartBase);

/** 
  \brief  	   uart receive interrupt callback function
  \param[in]   ptUartBase	pointer of uart register structure
  \param[in]   eState		uart interrupt callback state event, \ref csi_uart_state_e
  \param[in]   pbyBuf		pointer of uart interrupt receive data buffer
  \param[in]   hwLen		length receive data
  \return 	   none
 */ 
void csi_uart_recv_callback(csp_uart_t *ptUartBase, csi_uart_state_e eState, uint8_t *pbyBuf, uint16_t *hwLen);

/** 
  \brief  	   uart send interrupt callback function
  \param[in]   ptUartBase	pointer of uart register structure
  \param[in]   eState		uart err interrupt callback state event, \ref csi_uart_state_e
  \return 	   none
 */ 
void csi_uart_err_callback(csp_uart_t *ptUartBase, csi_uart_state_e eState);

/** 
  \brief  	   uart interrupt handler function
  \param[in]   ptUartBase	pointer of uart register structure
  \param[in]   byIdx		uart idx(0/1/2)
  \return 	   none
 */ 
void csi_uart_irqhandler(csp_uart_t *ptUartBase, uint8_t byIdx);

/** 
  \brief 	   enable/disable uart interrupt 
  \param[in]   ptUartBase	pointer of uart register structure
  \param[in]   eIntSrc		uart interrupt source, \ref csi_uart_intsrc_e
  \return 	   none
 */
void csi_uart_int_enable(csp_uart_t *ptUartBase, csi_uart_intsrc_e eIntSrc);

/** 
  \brief 	   disable uart interrupt 
  \param[in]   ptUartBase	pointer of uart register structure
  \param[in]   eIntSrc		uart interrupt source, \ref csi_uart_intsrc_e
  \return 	   none
 */
void csi_uart_int_disable(csp_uart_t *ptUartBase, csi_uart_intsrc_e eIntSrc);

/** 
  \brief clear uart interrupt status
  \param[in] ptUartBase: pointer of uart register structure
  \param[in] eIntSta: uart interrupt status, \ref csi_uart_intsta_e
  \return none
 */
void csi_uart_clr_isr(csp_uart_t *ptUartBase, csi_uart_intsta_e eIntSta);
/** 
  \brief 	   start(enable) uart rx/tx
  \param[in]   ptUartBase	pointer of uart register structure
  \param[in]   eFunc: rx/tx function, \ref csi_uart_func_e
  \return      error code \ref csi_error_t
 */ 
csi_error_t csi_uart_start(csp_uart_t *ptUartBase, csi_uart_func_e eFunc);

/** 
  \brief 	   stop(disable) uart rx/tx
  \param[in]   ptUartBase	pointer of uart register structure
  \param[in]   eFunc: rx/tx function, \ref csi_uart_func_e
  \return      error code \ref csi_error_t
 */ 
csi_error_t csi_uart_stop(csp_uart_t *ptUartBase, csi_uart_func_e eFunc);

/**
  \brief       Start send data to UART transmitter, this function is blocking.
  \param[in]   uart     	uart handle to operate.
  \param[in]   data     	pointer to buffer with data to send to UART transmitter.
  \param[in]   size     	number of data to send (byte).
  \return      the num of data which is sent successfully or CSI_ERROR.
*/
int32_t csi_uart_send(csp_uart_t *ptUartBase, const void *pData, uint16_t hwSize);

/** 
  \brief send data from uart, this function is interrupt mode.
  \param[in] ptUartBase: pointer of uart register structure
  \param[in] pData: pointer to buffer with data to send to uart transmitter.
  \param[in] hwSize: number of data to send (byte).
  \return  the num of data which is send successfully or CSI_ERROR/CSI_OK
 */
csi_error_t csi_uart_send_int(csp_uart_t *ptUartBase, const void *pData, uint16_t hwSize);

/** 
  \brief 	   set uart tx dma mode and enable
  \param[in]   ptUartBase	pointer of uart register structure
  \param[in]   eDmaMode		ctx dma mode, \ref csi_uart_dma_md_e
  \return  	   none
 */
void csi_uart_set_txdma(csp_uart_t *ptUartBase, csi_uart_dma_md_e eDmaMode);

/** 
  \brief 	   set uart rx dma mode and enable
  \param[in]   ptUartBase	pointer of uart register structure
  \param[in]   eDmaMode		rx dma mode, \ref csi_uart_dma_md_e
  \return  	   none
 */
void csi_uart_set_rxdma(csp_uart_t *ptUartBase, csi_uart_dma_md_e eDmaMode);

/** 
  \brief 	   send data from uart, this function is dma mode
  \param[in]   ptUartBase	pointer of uart register structure
  \param[in]   eDmaCh	    channel number of dma(20~31), \ref csi_dma_ch_e
  \param[in]   pData		pointer to buffer with data to send to uart transmitter.
  \param[in]   hwSize		number of data to send (byte).
  \return  	   none
 */
void csi_uart_send_dma(csp_uart_t *ptUartBase, csp_dma_t *ptDmaBase, csi_dma_ch_e eDmaCh, const void *pData, uint16_t hwSize);

/** 
  \brief 	   receive data from uart, this function is dma mode
  \param[in]   ptUartBase	pointer of uart register structure
  \param[in]   ptDmaBase	pointer of dma register structure
  \param[in]   eDmaCh	    channel number of dma(20~31), \ref csi_dma_ch_e
  \param[in]   pData		pointer to buffer with data to receive to uart transmitter.
  \param[in]   hwSize		number of data to receive (byte).
  \return  	   none
 */
void csi_uart_recv_dma(csp_uart_t *ptUartBase, csp_dma_t *ptDmaBase, csi_dma_ch_e eDmaCh, void *pData, uint16_t hwSize);

/**
  \brief       Query data from UART receiver FIFO, this function is blocking.
  \param[in]   ptUartBase   pointer of uart register structure
  \param[out]  data     	pointer to buffer for data to receive from UART receiver.
  \param[in]   size     	number of data to receive(byte).
  \param[in]   timeout  	the timeout between bytes(ms).
  \return      the num of data witch is received successfully or CSI_ERROR.
*/
int32_t csi_uart_receive(csp_uart_t *ptUartBase, void *pData, uint16_t hwSize, uint32_t wTimeOut);

/** 
  \brief 	   receive data to uart transmitter,assign length ;this function is interrupt mode(async/no-blocking),
  \param[in]   ptUartBase   pointer of uart register structure
  \param[in]   pData		pointer to buffer with data to be received.
  \param[in]   wSize		number of data to receive (byte).
  \return      the num of data which is receive successfully
 */
csi_error_t csi_uart_receive_int(csp_uart_t *ptUartBase, void *pData, uint16_t hwSize);

/**
  \brief       Get character in query mode.
  \param[in]   ptUartBase   pointer of uart register structure
  \return      the character to get.
*/
uint8_t csi_uart_getc(csp_uart_t *ptUartBase);

/**
  \brief       Send character in query mode.
  \param[in]   ptUartBase   pointer of uart register structure
  \param[in]   byData   	the character to be send.
  \return      none
*/
void csi_uart_putc(csp_uart_t *ptUartBase, uint8_t byData);

/** 
  \brief 	   get uart receive/send complete message and (Do not) clear message
  \param[in]   ptUartBase	pointer of uart reg structure.
  \param[in]   eWkMode		tx or rx mode, \ref csi_uart_wkmode_e
  \param[in]   bClrEn		clear uart receive/send complete message enable; ENABLE: clear , DISABLE: Do not clear 
  \return  	   bool type true/false
 */ 
bool csi_uart_get_msg(csp_uart_t *ptUartBase, csi_uart_wkmode_e eWkMode, bool bClrEn);

/** 
  \brief 	   clr uart receive/send status message (set uart recv/send status idle) 
  \param[in]   ptUartBase	pointer of uart reg structure.
  \param[in]   eWkMode		tx or rx mode, \ref csi_uart_wkmode_e
  \return      none
 */ 
void csi_uart_clr_msg(csp_uart_t *ptUartBase, csi_uart_wkmode_e eWkMode);

#ifdef __cplusplus
}
#endif

#endif /* _DRV_UART_H_ */
