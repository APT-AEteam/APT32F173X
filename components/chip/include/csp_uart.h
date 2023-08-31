/***********************************************************************//** 
 * \file  csp_uart.h
 * \brief  UART description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-8-14 <td>V1.0  <td>ZJY   <td>initial
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CSP_UART_H
#define _CSP_UART_H

/* Includes ------------------------------------------------------------------*/
#include <soc.h>

/// \struct csp_uart_t
/// \brief UART reg description
typedef struct
{
    __IOM	uint32_t 	DATA;		//0x0000	Write and Read Data Register  
    __IOM  	uint32_t 	SR;        	//0x0004 	Status Register               
    __IOM  	uint32_t 	CTRL;      	//0x0008 	Control Register              
    __IOM  	uint32_t  	ISR;       	//0x000C 	Interrupt Status Register     
    __IOM  	uint32_t  	BRDIV;     	//0x0010 	Baud Rate Generator Register  
    __IOM  	uint32_t  	DMACR;		//0x0014	DMA Control Register
	__IOM  	uint32_t	RTOR;		//0x0018	Receive timeout 
	__IOM  	uint32_t	TTGR;		//0x001C	Send time-guard
	__OM  	uint32_t	SRR;		//0x0020	SoftWare reset 
//	__IM  	uint32_t	RXFL;		//0x0024	Data number of receive fifo
//	__IM  	uint32_t	TXFL;		//0x0028	Data number of send fifo
} csp_uart_t; 

/*****************************************************************************
************************** UART Para Macro defined ***************************
******************************************************************************/
#define UART_RESET_VALUE	(0x00000000)
#define UART_RX_TIMEOUT		(0x1FFF)
#define UART_TX_TIMEOUT		(0x1FFF)
#define UART_BUF_SIZE		(36)

/******************************************************************************
* SR : UART Status Register
******************************************************************************/
// SR : UART Status Register											
typedef enum{
	UART_TX_FULL		= (0x01ul << 0),		// Transmitter full        
	UART_RX_FULL		= (0x01ul << 1),		// Receiver full           
	UART_TX_OVER     	= (0x01ul << 2),		// Transmitter buff over   
	UART_RX_OVER     	= (0x01ul << 3), 		// Receiver buff over      
	UART_PAR_ERR     	= (0x01ul << 4), 
	UART_TFE			= (0x01ul << 5),	
	UART_TNF			= (0x01ul << 6),	
	UART_RNE			= (0x01ul << 7),	
	UART_RFF			= (0x01ul << 8)	
}uart_sr_e;

/******************************************************************************
* CTRL : UART Control Register	
******************************************************************************/
// CTRL : UART Control Register		
#define	UART_TX_POS			(0)			// Transmitter Enable/disable 	 
#define	UART_TX_MSK			(0x01ul << UART_TX_POS)								
typedef enum{
	UART_TX_DIS			= 0,
	UART_TX_EN	
}tx_ctrl_e;

#define	UART_RX_POS			(1)			// Receiver Enable/disable         
#define	UART_RX_MSK			(0x01ul << UART_RX_POS)		
typedef enum{
	UART_RX_DIS			= 0,
	UART_RX_EN
}rx_ctrl_e;

#define	UART_PARITY_POS		(8)			// Uart Parity select			       
#define	UART_PARITY_MSK		(0x07ul << UART_PARITY_POS)
typedef enum{
	UART_PAR_NONE		= 0x00,
	UART_PAR_EVEN		= 0x04,
	UART_PAR_ODD    	= 0x05,
	UART_PAR_SPACE 	 	= 0x06,
	UART_PAR_MARK   	= 0x07
}uart_parity_e;

#define	UART_FIFO_POS		(11)		// Receiver FIFO level            
#define	UART_FIFO_MSK		(0x01ul << UART_FIFO_POS)		
typedef enum{
	UART_FIFO_DIS		= 0,
	UART_FIFO_EN
}uart_fifo_e;

#define	UART_RXFIFO_POS		(14)		// Receiver FIFO level            
#define	UART_RXFIFO_MSK		(0x07ul << UART_RXFIFO_POS)		
typedef enum{
	UART_FIFO_1_8		= 1,
	UART_FIFO_1_4		= 2,
	UART_FIFO_1_2		= 4
}uart_fifo_bit_e;

#define	UART_STTTO_POS		(20)		// Receiver FIFO level            
#define	UART_STTTO_MSK		(0x01ul << UART_STTTO_POS)		
typedef enum{
	UART_STTTO_DIS		= 0,
	UART_STTTO_EN
}uart_sttto_e;

#define	UART_BREAK_POS		(23)		// Receiver FIFO level            
#define	UART_BREAK_MSK		(0x01ul << UART_STTTO_POS)		
typedef enum{
	UART_BREAK_EN		= 1,
	UART_BREAK_DIS
}uart_break_e;

#define	UART_DBUG_POS		(31)		// Receiver FIFO level            
#define	UART_DBUG_MSK		(0x01ul << UART_DBUG_POS)		
typedef enum{
	UART_DBUG_DIS		= 0,
	UART_DBUG_EN
}uart_dbug_e;

typedef enum{
	UART_TX_INT     	= (0x01ul << 2),
	UART_RX_INT     	= (0x01ul << 3),
	UART_TX_OV_INT    	= (0x01ul << 4),
	UART_RX_OV_INT     	= (0x01ul << 5),
	UART_PARERR_INT 	= (0x01ul << 7),
	UART_TXFIFO_INT     = (0x01ul << 12),
	UART_RXFIFO_INT     = (0x01ul << 13),
	UART_RXFIFO_OV_INT	= (0x01ul << 18),
	UART_TXDONE_INT 	= (0x01ul << 19),
	UART_RXTO_INT		= (0x01ul << 21),
	UART_RXBK_INT		= (0x01ul << 22)
}uart_int_e;

/******************************************************************************
* ISR : UART Interrupt Status Register	
******************************************************************************/
// ISR : UART Interrupt Status Register									
typedef enum{
	UART_TX_INT_S   		= (0x01ul << 0),	//Transmitter INT Status      
	UART_RX_INT_S			= (0x01ul << 1),	//Receiver INT Status          
	UART_TX_OV_INT_S    	= (0x01ul << 2),	//Transmitter Over INT Status  
	UART_RX_OV_INT_S   	 	= (0x01ul << 3),	//Receiver Over INT Status   
	UART_PARERR_INT_S     	= (0x01ul << 4),	//Parity Error INT Status  
	UART_TXFIFO_INT_S     	= (0x01ul << 5),	//Transmitter FIFO INT Status      
	UART_RXFIFO_INT_S     	= (0x01ul << 6),	//Receiver FIFO INT Status 
	UART_RXFIFO_OV_INT_S	= (0x01ul << 7),	//Receiver FIFO Over INT Status 
	UART_RXTO_INT_S     	= (0x01ul << 9),	//Receiver FIFO INT Status 
	UART_RXBRK_INT_S		= (0x01ul << 10),	//Receiver FIFO Over INT Status 
	UART_TXDONE_INT_S     	= (0x01ul << 19)	//Transmitter Complete INT Status
}uart_isr_e;

/******************************************************************************
* BRDIV : BRDIV register 
******************************************************************************/
// BRDIV : BRDIV register 
#define UART_BRDIV_POS		(0) 
#define	UART_BRDIV_MSK		(0x000FFFFF << UART_BRDIV_POS)	

/******************************************************************************
* DMACR : DMA Control register 
******************************************************************************/
#define	UART_RDMA_EN_POS		(0)		
#define	UART_RDMA_EN_MSK		(0x01ul << UART_RDMA_EN_POS)	
typedef enum{
	UART_RDMA_DIS		= 0,
	UART_RDMA_EN
}uart_rdma_en_e;

#define	UART_TDMA_EN_POS		(1)	
#define	UART_TDMA_EN_MSK		(0x01ul << UART_TDMA_EN_POS)	
typedef enum{
	UART_TDMA_DIS		= 0,
	UART_TDMA_EN
}uart_tdma_en_e;

#define	UART_RDMA_MD_POS	(2)			 
#define	UART_RDMA_MD_MSK	(0x01ul << UART_RDMA_MD_POS)	
typedef enum{
	UART_RDMA_FIFO_NSPACE = 0,
	UART_RDMA_FIFO_TRG
}uart_rdma_md_e;

#define	UART_TDMA_MD_POS	(3)			 
#define	UART_TDMA_MD_MSK	(0x01ul << UART_TDMA_MD_POS)	
typedef enum{
	UART_TDMA_FIFO_NFULL		= 0,
	UART_TDMA_FIF0_TRG
}uart_tdma_md_e;

/******************************************************************************
* RTOR : receive timeout register
******************************************************************************/
#define	UART_RTOR_POS			(0)			 
#define	UART_RTOR_MSK			(0xFFFFul << UART_RTOR_POS)

/******************************************************************************
* TTGR : TX time-guard register
******************************************************************************/
#define	UART_TTGR_POS			(0)			 
#define	UART_TTGR_MSK			(0xFFul << UART_TTGR_POS)

/******************************************************************************
* SRR : software reset register 
******************************************************************************/
#define	UART_SWRST_POS			(0)			 
#define	UART_SWRST_MSK			(0x01ul << UART_SWRST_POS)

#define	UART_RXFIFO_RST_POS		(8)			 
#define UART_RXFIFO_RST_MSK		(0x01ul << UART_RXFIFO_RST_POS)

#define	UART_TXFIFO_RST_POS		(16)			 
#define UART_TXFIFO_RST_MSK		(0x01ul << UART_TXFIFO_RST_POS)

/******************************************************************************
* RXFL : receive fifo data number register 
******************************************************************************/
#define	UART_RXFL_POS			(0)			 
#define	UART_RXFL_MSK			(0x01ul << UART_RXFL_POS)

/******************************************************************************
* RXFL : receive fifo data number register 
******************************************************************************/
#define	UART_TXFL_POS			(0)			 
#define	UART_TXFL_MSK			(0x01ul << UART_TXFL_POS)

/******************************************************************************
* TXFL : send fifo data number register 
******************************************************************************/

/******************************************************************************
********************* UARTx Macro Definition Declaration **********************
******************************************************************************/
//reset
#define csp_uart_soft_rst(UARTx)		((UARTx)->SRR = UART_SWRST_MSK)		
#define csp_uart_rxfifo_rst(UARTx)		((UARTx)->SRR = UART_RXFIFO_RST_MSK)	
#define	csp_uart_txfifo_rst(UARTx)		((UARTx)->SRR = UART_TXFIFO_RST_MSK)	

//TX/RX  enable/disable
#define csp_uart_tx_enable(UARTx) 		((UARTx)->CTRL |= UART_TX_MSK)
#define csp_uart_tx_disable(UARTx) 		((UARTx)->CTRL &= ~UART_TX_MSK)
#define csp_uart_rx_enable(UARTx) 		((UARTx)->CTRL |= UART_RX_MSK)
#define csp_uart_rx_disable(UARTx) 		((UARTx)->CTRL &= ~UART_RX_MSK)

//TX and RX enable/disable
#define csp_uart_enable(UARTx)			((UARTx)->CTRL |= (UART_TX_MSK | UART_RX_MSK)) 
#define csp_uart_disable(UARTx)			((UARTx)->CTRL &= ~(UART_TX_MSK | UART_RX_MSK)) 

//receive timeout and break	enable/disable	
#define csp_uart_rto_enable(UARTx)		((UARTx)->CTRL |= UART_STTTO_MSK)
#define csp_uart_rto_disable(UARTx)		((UARTx)->CTRL &= ~UART_STTTO_MSK)
#define csp_uart_brk_enable(UARTx)		((UARTx)->CTRL |= UART_BREAK_MSK)
#define csp_uart_brk_disable(UARTx)		((UARTx)->CTRL &= ~UART_BREAK_MSK)

//fifo and parity
#define csp_uart_fifo_disable(UARTx)	((UARTx)->CTRL &= ~UART_FIFO_MSK)	
#define csp_uart_fifo_enable(UARTx)		((UARTx)->CTRL &=  UART_FIFO_MSK)	

//read reg
#define csp_uart_get_data(UARTx)		((UARTx)->DATA)
#define csp_uart_get_ctrl(UARTx)		((UARTx)->CTRL)
#define csp_uart_get_sr(UARTx)			((UARTx)->SR)
//clr sr
#define csp_uart_clr_all_sr(UARTx)		((UARTx)->SR = 0x01ff)				

//interrupt
#define csp_uart_get_isr(UARTx)				((UARTx)->ISR)
#define csp_uart_clr_isr(UARTx, eIsr)		((UARTx)->ISR = eIsr)
#define csp_uart_int_enable(UARTx, eInt)	((UARTx)->CTRL |= eInt)
#define csp_uart_int_disable(UARTx, eInt)	((UARTx)->CTRL &= ~eInt)

//set reg
#define csp_uart_set_ctrl(UARTx, wVal)		((UARTx)->CTRL = wVal)
#define csp_uart_set_brdiv(UARTx, wVal)		((UARTx)->BRDIV = wVal)
#define csp_uart_set_ttgr(UARTx, byVal)		((UARTx)->TTGR = byVal)
#define csp_uart_set_rtor(UARTx, hwVal)		((UARTx)->RTOR = hwVal)
#define csp_uart_set_data(UARTx, byVal)		((UARTx)->DATA = byVal)

//set patity/fifo/radma/txdma
#define csp_uart_set_parity(UARTx, eParity)	((UARTx)->CTRL = ((UARTx)->CTRL & ~UART_PARITY_MSK) | (eParity << UART_PARITY_POS))

#define csp_uart_set_fifo(UARTx, eFiBit, bEnable)		\
	((UARTx)->CTRL = ((UARTx)->CTRL & ~(UART_RXFIFO_MSK | UART_FIFO_MSK)) | (eFiBit << UART_RXFIFO_POS) | (bEnable << UART_FIFO_POS))

#define csp_uart_set_rxdma(UARTx, eRxDmaEn, eRxDmaMode)	\
	((UARTx)->DMACR = ((UARTx)->DMACR & ~(UART_RDMA_EN_MSK | UART_RDMA_MD_MSK)) | (eRxDmaEn << UART_RDMA_EN_POS) | (eRxDmaMode << UART_RDMA_MD_POS))

#define csp_uart_set_txdma(UARTx, eTxDmaEn, eTxDmaMode)	\
	((UARTx)->DMACR = ((UARTx)->DMACR & ~(UART_TDMA_EN_MSK | UART_TDMA_MD_MSK)) | (eTxDmaEn << UART_TDMA_EN_POS) | (eTxDmaMode << UART_TDMA_MD_POS))


#endif
