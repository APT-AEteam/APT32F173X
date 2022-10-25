/***********************************************************************//** 
 * \file  csp_qspi.h
 * \brief  QSPI description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-7-2 <td>V0.0  <td>LQ     <td>initial
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CSP_QSPI_H
#define _CSP_QSPI_H

/* Includes ------------------------------------------------------------------*/
#include <soc.h>

/// \struct csp_qspi_t
/// \brief QSPI reg description
typedef struct
{
	__IOM  uint32_t  CR0;			//0x0000	Control Register 0 			 
	__IOM  uint32_t  CR1;			//0x0004	Control Register 1 
	__IOM  uint32_t  ENR; 		    //0x0008	Qspi enable register 
	__OM   uint32_t  MWCR;			//0x000C	Microwire control register 
	__IOM  uint32_t  SER;			//0x0010	Slave mode enable register         
	__IOM  uint32_t  CPSR;			//0x0014	Clk div register    
	__IOM  uint32_t  TXFIFO_TLR;	//0x0018	Send FIFO interrupt threshold register   
	__IOM  uint32_t  RXFIFO_TLR;	//0x001C	Receive FIFO interrupt threshold register   
	__IOM  uint32_t  TXFLR;			//0x0020	Sends the current valid data length of the FIFO 
    __IOM  uint32_t  RXFLR;     	//0x0024    Receive the current valid data length of the FIFO 
	__IM   uint32_t  SR;			//0x0028	State register
	__IOM  uint32_t  IMCR;			//0x002C	Interrupt enable register
	__IM   uint32_t  MISR;			//0x0030	Interrupt status register
	__IM   uint32_t  RISR;			//0x0034	Raw interrupt status register
	__IM   uint32_t  TXFOICR;		//0x0038	Send FIFO overflow interrupt clear register
	__IM   uint32_t  RXFOICR;		//0x003C	Recv FIFO overflow interrupt clear register
	__IM   uint32_t  RXFUICR;		//0x0040	Recv FIFO under overflow interrupt clear register
	__IM   uint32_t  MSTICR;		//0x0044    Multi - host contended interrupt clear register	
	__OM   uint32_t  ICR;		    //0x0048    Interrupt clear register
	__IOM  uint32_t  DMACR;         //0x004C    DMA enable control register
	__IOM  uint32_t  DMATDLR;       //0x0050    DMA Transmit control register
	__IOM  uint32_t	 DMARDLR;		//0x0054    DMA Receive control register
	__IM   uint32_t  IDR;		    //0x0058    The ID information of the current QSPI module register
	__IM   uint32_t  COMPVR;		//0x005C    Component version register 
	__IOM  uint32_t  DRx[36];		//0x0060    ~0x00EC,Data Register
	__IOM  uint32_t  RSD;		    //0x00F0    Receive Sampling Delay Register
	__IOM  uint32_t  SPICR;		    //0x00F4    QSPI control register
	__IOM  uint32_t  DDREDGE;		//0x00F8    DDR sends edge register
	__IOM  uint32_t  XIPMDBITS;		//0x00FC    XIP mode bit register
} csp_qspi_t;


/************************** macro definition *****************************/
#define QSPI_TIMEOUT		(0xFFFF)

//QSPI Registers RST  Value
#define QSPI_CR0_RST          	(0x00000007)    //CR0 reset value    
#define QSPI_CR1_RST          	(0x00000000)    //CR1 reset value    
#define QSPI_ENR_RST   			(0x00000000)    //ENR reset value 
#define QSPI_SER_RST			(0x00000000)    //SER reset value 
#define QSPI_IMCR_RST			(0x00000000)    //IMCR reset value 

#define QSPI_NEW                0x01 //
//*****************************************************************************
// QSPI_CR0 : Control Register 0

//data frame size 	 
#define	QSPI_DFS_POS		(0)							   
#define	QSPI_DFS_MSK		(0x1Ful << QSPI_DFS_POS)
typedef enum
{
	QSPI_DFS_4		= 3,            
	QSPI_DFS_5,            
	QSPI_DFS_6,            
	QSPI_DFS_7,            
	QSPI_DFS_8,  
	QSPI_DFS_9,      
	QSPI_DFS_10,            
	QSPI_DFS_11,            
	QSPI_DFS_12,            
	QSPI_DFS_13,            
	QSPI_DFS_14,  
	QSPI_DFS_15,  
	QSPI_DFS_16,
	QSPI_DFS_17,
	QSPI_DFS_18,
	QSPI_DFS_19,
	QSPI_DFS_20,
	QSPI_DFS_21,
	QSPI_DFS_22,
	QSPI_DFS_23,
	QSPI_DFS_24,
	QSPI_DFS_25,
	QSPI_DFS_26,
	QSPI_DFS_27,
	QSPI_DFS_28,
	QSPI_DFS_29,
	QSPI_DFS_30,
	QSPI_DFS_31,
	QSPI_DFS_32
}qspi_frame_size_e;


//frame protocol 	
#define	QSPI_FRP_POS		(6)								
#define	QSPI_FRP_MSK		(0x03ul << QSPI_FRP_POS)
typedef enum
{
	QSPI_RFP_MOTOROLA	= 0,                    
}qspi_frame_protocol_e;


//spi clk polarity / phase  
#define QSPI_SPO_H_POS	(8)							 
#define QSPI_SPO_H_MSK	(0x03ul << QSPI_SPO_H_POS)
#if QSPI_NEW
	typedef enum
	{
		QSPI_SPO0_SPH0	= (0x00ul),            
		QSPI_SPO0_SPH1	= (0x02ul),  
		QSPI_SPO1_SPH0	= (0x01ul),            
		QSPI_SPO1_SPH1   = (0x03ul)            
	}qspi_spo_h_e;
#else
	typedef enum
	{
		QSPI_SPO0_SPH0	= (0x00ul),            
		QSPI_SPO0_SPH1	= (0x01ul),  
		QSPI_SPO1_SPH0	= (0x02ul),            
		QSPI_SPO1_SPH1   = (0x03ul)            
	}qspi_spo_h_e;
#endif

//transmission mode
#define QSPI_TMOD_POS    (10)
#define QSPI_TMOD_MSK    (0x03ul << QSPI_TMOD_POS)
typedef enum
{
	QSPI_SEND_RECV = 0,
	QSPI_SEND,
	QSPI_RECV
}qspi_tmod_e;


//slave mode output enable
#define	QSPI_SLVOE_POS		(12)					
#define	QSPI_SLVOE_MSK		(0x01ul << QSPI_SLVOE_POS)
typedef enum
{
	QSPI_SLVOE_TXEN	= 0,            
	QSPI_SLVOE_TXDIS      
}qspi_slvoe_e;


//loop back test
#define	QSPI_SRLB_POS		(13)			 				
#define	QSPI_SRLB_MSK		(0x01ul << QSPI_SRLB_POS)
typedef enum
{
	QSPI_SRLB_DIS		= 0,            
	QSPI_SRLB_EN   
}qspi_srlb_e;


//slave toggle enable
#define QSPI_SLVSTE_POS    (14)
#define QSPI_SLVSTE_MSK	   (0x01ul << QSPI_SLVSTE_POS)
typedef enum
{
	QSPI_SLVSTE_DIS   = 0,
	QSPI_SLVSTE_EN
}qspi_slvste_e;


//frame format
#define QSPI_FRF_POS    (22)
#define QSPI_FRF_MSK    (0x03 << QSPI_FRF_POS)
typedef enum
{
	QSPI_FRF_STD  = 0,
	QSPI_FRF_TWO,
	QSPI_FRF_FOUR
}qspi_frf_e;


//*****************************************************************************
// QSPI_ENR : qspi enable register

//qspi enable
#if QSPI_NEW
	#define QSPI_SSE_POS    (5)
	#define QSPI_SSE_MSK    (0x01 << QSPI_SSE_POS)
	typedef enum
	{
		QSPI_DIS,
		QSPI_EN
	}qspi_sse_e;
#else
	#define QSPI_SSE_POS    (0)
	#define QSPI_SSE_MSK    (0x01 << QSPI_SSE_POS)
	typedef enum
	{
		QSPI_DIS,
		QSPI_EN
	}qspi_sse_e;
#endif


//*****************************************************************************
// QSPI_SER : slave mode enable register

//slave enable
typedef enum
{
	QSPI_S0EN = 0x01,
	QSPI_S1EN = 0x02,
	QSPI_S2EN = 0x04,
	QSPI_S3EN = 0x08
}qspi_se_e;


//*****************************************************************************
// QSPI_SR : state register
typedef enum
{
	QSPI_BSY		= (0x01ul << 0), 			//spi busy flag                  
	QSPI_TNF     	= (0x01ul << 1), 			//transmit fifo is not full          
	QSPI_TFE    	= (0x01ul << 2),			//transmit fifo is empty             
	QSPI_RNE		= (0x01ul << 3),   			//receive fifo is not empty                   
	QSPI_RFF     	= (0x01ul << 4), 			//receive fifo is full  
    QSPI_TXE     	= (0x01ul << 5), 			//tx error  
    QSPI_DCOL     	= (0x01ul << 6) 			//data collision error            
}qspi_sr_e;


//******************************************************************************
// QSPI_IMCR : QSPI_MISR : QSPI_RISR 
// IMCR : Interrupt enable register
// MISR : Interrupt status register
// RISR : Raw interrupt status register

typedef enum
{
	QSPI_NONE_INT		= (0x00ul << 0),   		//none interrupt 
	QSPI_TXE_INT		= (0x01ul << 0), 		//send fifo empty interrupt    
	QSPI_TXO_INT    	= (0x01ul << 1), 		//send fifo overflow interrupt   
	QSPI_RXU_INT   		= (0x01ul << 2),	    //receive fifo underflow interrupt           
	QSPI_RXO_INT		= (0x01ul << 3),   		//receive fifo overflow interrupt
	QSPI_RXF_INT		= (0x01ul << 4),   		//receive fifo full interrupt
	QSPI_MST_INT		= (0x01ul << 5),   		//Multi - host contention interrupt
}qspi_int_e;



//******************************************************************************
// QSPI_RSD: qspi Receive sample delay register 
typedef enum
{
	QSPI_RSD_NONE = 0,
	QSPI_RSD_T1,
	QSPI_RSD_T2,
	QSPI_RSD_T3,
	QSPI_RSD_T4
}qspi_rsd_e;


//******************************************************************************
// QSPI_SPICR: qspi control register 

//select std or qspi
#define QSPI_FRAMETYPE_POS     (0)
#define QSPI_FRAMETYPE_MSK     (0x03 <<  QSPI_FRAMETYPE_POS)
typedef enum
{
	QSPI_INSSTD_ADDSTD =0,
	QSPI_INSSTD_ADDSTDX,
	QSPI_INSSTDX_ADDSTDX
}qspi_frametype_e;


//address lenght
#define QSPI_ADDRL_POS  (2)
#define QSPI_ADDRL_MSK  (0x0f << QSPI_ADDRL_POS)
typedef enum
{
	ADDRL_NONE = 0,
	ADDRL_4,
	ADDRL_8,
	ADDRL_12,
	ADDRL_16,
	ADDRL_20,
	ADDRL_24,
	ADDRL_28,
	ADDRL_32,
	ADDRL_36,
	ADDRL_40,
	ADDRL_44,
	ADDRL_48,
	ADDRL_52,
	ADDRL_56,
	ADDRL_60
}qspi_addrl_e;


//instruct lenght
#define QSPI_INSTL_POS   (8)
#define QSPI_INSTL_MSK   (0x03 << QSPI_INSTL_POS)
typedef enum 
{
	INSTL_NONE = 0,
	INSTL_4,
	INSTL_8,
	INSTL_16
}qspi_instl_e;


//qspi ddre
#define QSPI_DDRE_POS  (16)
#define QSPI_DDRE_MSK  (0x01 << QSPI_DDRE_POS)
typedef enum
{
	QSPI_DDRE_DIS = 0,
	QSPI_DDRE_EN
}qspi_ddre_e;


//qspi instddre
#define QSPI_INSTDDRE_POS  (17)
#define QSPI_INSTDDRE_MSK  (0x01 << QSPI_INSTDDRE_POS)
typedef enum
{
	QSPI_INSTDDRE_DIS = 0,
	QSPI_INSTDDRE_EN
}qspi_instddre_e;


#define QSPI_WCYCLE_POS  (11)
#define QSPI_WCYCLE_MSK  (0x1f << QSPI_WCYCLE_POS)

//******************************************************************************
// QSPI_DMACR: qspi DMA enable register

//dma rx enable
#define QSPI_RDMAE_POS  (0)
#define QSPI_RDMAE_MSK   (0x01 << QSPI_RDMAE_POS)
typedef enum
{
	QSPI_RDMAE_DIS,
	QSPI_RDMAE_EN
}qspi_rdmae_e;

//dma tx enable
#define QSPI_TDMAE_POS  (1)
#define QSPI_TDMAE_MSK   (0x01 << QSPI_TDMAE_POS)
typedef enum
{
	QSPI_TDMAE_DIS,
	QSPI_TDMAE_EN
}qspi_tdmae_e;

//******************************************************************************
// QSPI_DMATDLR: qspi DMA transmit register
typedef enum
{
	QSPI_DMA_TX_LEVEL0 = 0,
	QSPI_DMA_TX_LEVEL1,
	QSPI_DMA_TX_LEVEL2,
	QSPI_DMA_TX_LEVEL3,
	QSPI_DMA_TX_LEVEL4,
	QSPI_DMA_TX_LEVEL5,
	QSPI_DMA_TX_LEVEL6,
	QSPI_DMA_TX_LEVEL7,
	QSPI_DMA_TX_LEVEL8,
	QSPI_DMA_TX_LEVEL9,
	QSPI_DMA_TX_LEVEL10,
	QSPI_DMA_TX_LEVEL11,
	QSPI_DMA_TX_LEVEL12,
	QSPI_DMA_TX_LEVEL13,
	QSPI_DMA_TX_LEVEL14,
	QSPI_DMA_TX_LEVEL15
}qspi_dma_tx_level_e;

//******************************************************************************
// QSPI_DMATDLR: qspi DMA transmit register
typedef enum
{
	QSPI_DMA_RX_LEVEL1 = 0,
	QSPI_DMA_RX_LEVEL2,
	QSPI_DMA_RX_LEVEL3,
	QSPI_DMA_RX_LEVEL4,
	QSPI_DMA_RX_LEVEL5,
	QSPI_DMA_RX_LEVEL6,
	QSPI_DMA_RX_LEVEL7,
	QSPI_DMA_RX_LEVEL8,
	QSPI_DMA_RX_LEVEL9,
	QSPI_DMA_RX_LEVEL10,
	QSPI_DMA_RX_LEVEL11,
	QSPI_DMA_RX_LEVEL12,
	QSPI_DMA_RX_LEVEL13,
	QSPI_DMA_RX_LEVEL14,
	QSPI_DMA_RX_LEVEL15,
	QSPI_DMA_RX_LEVEL16	
}qspi_dma_rx_level_e;
//-----------------------------------------------------------------------------------------------
//inline function
static inline void csp_qspi_set_int(csp_qspi_t *ptQspiBase,uint8_t byQspiInt,bool bEnable)
{
	if(bEnable)
		ptQspiBase->IMCR |= byQspiInt;
	else
		ptQspiBase->IMCR &= ~byQspiInt;

}

static inline void csp_qspi_default_init(csp_qspi_t *ptQspiBase)
{
	ptQspiBase->CR0	 = QSPI_CR0_RST;
	ptQspiBase->CR1	 = QSPI_CR1_RST;
	ptQspiBase->ENR  = QSPI_ENR_RST;
	ptQspiBase->SER  = QSPI_SER_RST;
	ptQspiBase->IMCR  = QSPI_IMCR_RST;
}

static inline void csp_qspi_set_lbm(csp_qspi_t *ptQspiBase, qspi_srlb_e eSrlb)
{
	ptQspiBase->CR0  = (ptQspiBase->CR0 & (~QSPI_SRLB_MSK)) | (eSrlb << QSPI_SRLB_POS);
}

static inline void csp_qspi_en(csp_qspi_t *ptQspiBase)
{
	#if QSPI_NEW
		ptQspiBase->CR0  = (ptQspiBase->CR0 & (~QSPI_SSE_MSK)) | (QSPI_EN << QSPI_SSE_POS);
	#else
		ptQspiBase->ENR |= (QSPI_EN << QSPI_SSE_POS);
	#endif
}

static inline void csp_qspi_dis(csp_qspi_t *ptQspiBase)
{
	#if QSPI_NEW
		ptQspiBase->CR0  = (ptQspiBase->CR0 & (~QSPI_SSE_MSK)) | (QSPI_DIS << QSPI_SSE_POS);
	#else
		ptQspiBase->ENR &= ~QSPI_SSE_MSK;
	#endif
}

static inline void csp_qspi_set_rxifl(csp_qspi_t *ptQspiBase, uint8_t byLevel)
{
	ptQspiBase->RXFIFO_TLR = byLevel;
}

static inline void csp_qspi_set_txifl(csp_qspi_t *ptQspiBase, uint8_t byLevel)
{
	ptQspiBase->TXFIFO_TLR = byLevel;
}

static inline void csp_qspi_set_cp_format(csp_qspi_t *ptQspiBase, qspi_spo_h_e ePolarityPhase)
{
	ptQspiBase->CR0 = (ptQspiBase->CR0 & (~QSPI_SPO_H_MSK) ) | ( ePolarityPhase << QSPI_SPO_H_POS ) ;
} 

static inline void csp_qspi_set_tmod(csp_qspi_t *ptQspiBase, qspi_tmod_e eTmod)
{
	ptQspiBase->CR0 = (ptQspiBase->CR0 & (~QSPI_TMOD_MSK) ) | ( eTmod << QSPI_TMOD_POS ) ;
} 

static inline void csp_qspi_set_cpsr(csp_qspi_t *ptQspiBase, uint32_t wCpsr)
{
	ptQspiBase->CPSR = ( wCpsr & 0xffff );
}

static inline uint8_t csp_qspi_get_sr(csp_qspi_t *ptQspiBase)
{
	return (uint8_t)(ptQspiBase->SR & 0x7f);
}

static inline void csp_qspi_set_frameformat(csp_qspi_t *ptQspiBase, qspi_frf_e eFrameformat)
{
	ptQspiBase->CR0 = (ptQspiBase->CR0 & (~QSPI_FRF_MSK) ) | ( eFrameformat << QSPI_FRF_POS ) ;
}

static inline void csp_qspi_set_frametype(csp_qspi_t *ptQspiBase, qspi_frametype_e eFrametype)
{
	ptQspiBase->SPICR = (ptQspiBase->SPICR & (~QSPI_FRAMETYPE_MSK) ) | ( eFrametype << QSPI_FRAMETYPE_POS ) ;
}

static inline void csp_qspi_set_instl(csp_qspi_t *ptQspiBase, qspi_instl_e eInstl)
{
	ptQspiBase->SPICR = (ptQspiBase->SPICR & (~QSPI_INSTL_MSK) ) | ( eInstl << QSPI_INSTL_POS ) ;
}

static inline uint8_t csp_qspi_get_instl(csp_qspi_t *ptQspiBase)
{
	return  (uint8_t)( (ptQspiBase->SPICR & QSPI_INSTL_MSK) >> QSPI_INSTL_POS );
}

static inline void csp_qspi_set_addrl(csp_qspi_t *ptQspiBase, qspi_addrl_e eAddrl)
{
	ptQspiBase->SPICR = (ptQspiBase->SPICR & (~QSPI_ADDRL_MSK) ) | ( eAddrl << QSPI_ADDRL_POS ) ;
}

static inline uint8_t csp_qspi_get_addrl(csp_qspi_t *ptQspiBase)
{
	 return (uint8_t) ( (ptQspiBase->SPICR & QSPI_ADDRL_MSK) >>  QSPI_ADDRL_POS );
}

static inline void csp_qspi_set_frame_len(csp_qspi_t *ptQspiBase, qspi_frame_size_e eFrameSize)
{
	ptQspiBase->CR0 = (ptQspiBase->CR0 & (~QSPI_DFS_MSK) ) | ( eFrameSize << QSPI_DFS_POS ) ;
}

static inline void csp_qspi_set_slvste(csp_qspi_t *ptQspiBase, qspi_slvste_e eSlvste)
{
	ptQspiBase->CR0 = (ptQspiBase->CR0 & (~QSPI_SLVSTE_MSK) ) | ( eSlvste << QSPI_SLVSTE_POS ) ;
}

static inline void csp_qspi_set_ser(csp_qspi_t *ptQspiBase, uint8_t bySe,bool bEnable)
{
	if(bEnable)
		ptQspiBase->SER |= bySe;
	else
		ptQspiBase->SER &= ~bySe;	
}

static inline uint8_t csp_qspi_get_intmsk(csp_qspi_t *ptQspiBase)
{
	return (ptQspiBase->MISR & 0x3f);
}

static inline void csp_qspi_clr_int(csp_qspi_t *ptQspiBase,uint8_t byIntsrc)
{
	ptQspiBase->ICR = byIntsrc & 0x1f;
}

static inline void csp_qspi_write_data(csp_qspi_t *ptQspiBase,uint32_t wData)
{
	ptQspiBase->DRx[0] = wData;
}

static inline uint32_t csp_qspi_read_data(csp_qspi_t *ptQspiBase)
{
	return ptQspiBase->DRx[0] ;
}

static inline void csp_qspi_set_waitcycle(csp_qspi_t *ptQspiBase,uint8_t byWcycle)
{
	ptQspiBase->SPICR = (ptQspiBase->SPICR & (~QSPI_WCYCLE_MSK) ) | ( byWcycle << QSPI_WCYCLE_POS ) ;
}

static inline void csp_qspi_set_read_number(csp_qspi_t *ptQspiBase,uint16_t hwNumber)
{
	ptQspiBase->CR1 = ( (hwNumber-1) & 0xffff);
}

static inline void csp_qspi_set_rxdma_enable(csp_qspi_t *ptQspiBase, qspi_rdmae_e eRdmae)
{
	ptQspiBase->DMACR = (ptQspiBase->DMACR & (~QSPI_RDMAE_MSK) ) | ( eRdmae << QSPI_RDMAE_POS ) ;
}

static inline void csp_qspi_set_txdma_enable(csp_qspi_t *ptQspiBase, qspi_tdmae_e eTdmae)
{
	ptQspiBase->DMACR = (ptQspiBase->DMACR & (~QSPI_TDMAE_MSK) ) | ( eTdmae << QSPI_TDMAE_POS ) ;
}

static inline void csp_qspi_set_rxdma_level(csp_qspi_t *ptQspiBase, qspi_dma_rx_level_e eRxlevel)
{
	ptQspiBase->DMARDLR = (eRxlevel & 0x0f);
}

static inline void csp_qspi_set_txdma_level(csp_qspi_t *ptQspiBase, qspi_dma_tx_level_e eTxlevel)
{
	ptQspiBase->DMATDLR = (eTxlevel & 0x0f);
}

static inline void csp_qspi_set_ddre(csp_qspi_t *ptQspiBase,qspi_ddre_e eDdre)
{
	ptQspiBase->SPICR = (ptQspiBase->SPICR & (~QSPI_DDRE_MSK) ) | ( eDdre << QSPI_DDRE_POS ) ;
}

static inline void csp_qspi_set_instddre(csp_qspi_t *ptQspiBase,qspi_instddre_e eInstddre)
{
	ptQspiBase->SPICR = (ptQspiBase->SPICR & (~QSPI_INSTDDRE_MSK) ) | ( eInstddre << QSPI_INSTDDRE_POS ) ;
}

static inline uint8_t csp_qspi_read_txflr(csp_qspi_t *ptQspiBase)
{
	return (uint8_t)( (ptQspiBase->TXFLR) & 0x0f) ;
}

static inline uint8_t csp_qspi_read_rxflr(csp_qspi_t *ptQspiBase)
{
	return (uint8_t)( (ptQspiBase->RXFLR) & 0x0f) ;
}


#endif

