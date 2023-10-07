/***********************************************************************//** 
 * \file  csp_spi.h
 * \brief  SPI description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-27 <td>V0.0  <td>ZJY    <td>initial
 * <tr><td> 2021-1-12 <td>V0.1  <td>ZJY    <td>macro definition style, modify
 * <tr><td> 2021-5-31 <td>V0.2  <td>LQ     <td>macro definition style, modify
 * <tr><td> 2023-9-25 <td>V0.3  <td>WCH    <td>code normalization
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CSP_SPI_H
#define _CSP_SPI_H

/* Includes ------------------------------------------------------------------*/
#include "soc.h"

/// \struct csp_spi_t
/// \brief SPI reg description
typedef struct
{
	__IOM  uint32_t  CR0;		//0x0000	Control Register 0 			 
	__IOM  uint32_t  CR1;		//0x0004	Control Register 1 
	__IOM  uint32_t  DR; 		//0x0008	Receive/transmit  data register 
	__IM   uint32_t  SR;		//0x000C	Status register 
	__IOM  uint32_t  CPSR;		//0x0010	Clock prescale register         
	__IOM  uint32_t  IMCR;		//0x0014	Interrupt set/clear register    
	__IM   uint32_t  RISR;		//0x0018	Raw interrupt status register   
	__IM   uint32_t  MISR;		//0x001C	Mask interrupt status register  
	__OM   uint32_t  ICR;		//0x0020	Interrupt clear register 
    __IOM  uint32_t  DMACR;     //0x0024    Dma Control Register
	__OM   uint32_t  SRR;		//0x0028	Software Reset Register
} csp_spi_t;

/*****************************************************************************
************************** SPI Function defined *****************************
******************************************************************************/
#define SPI_RECV_TIMEOUT		(0x1FFF)
#define SPI_SEND_TIMEOUT		(0x1FFF)

//SPI Registers RST  Value
#define SPI_CR0_RST          	(0x00000000)    	//CR0 reset value     
#define SPI_CR1_RST          	(0x00000010)    	//CR1 reset value     
#define SPI_DR_RST          	(0x00000000)   	 	//DR reset value      
#define SPI_SR_RST          	(0x00000003)    	//SR reset value      
#define SPI_CPSR_RST          	(0x00000000)    	//CPSR reset value    
#define SPI_IMCR_RST          	(0x00000000)   	 	//IMCR reset value   
#define SPI_RISR_RST          	(0x00000008)    	//RISR reset value    
#define SPI_MISR_RST          	(0x00000000)    	//MISR reset value    
#define SPI_ICR_RST          	(0x00000000)    	//ICR reset value     

#define	SPI_RXTX_TIMEOUT		0x1ff
/*****************************************************************************
* SPICR0 : Control Register 0
******************************************************************************/
#define	SPI_DATA_LEN_POS	(0)							//Data Size Select    
#define	SPI_DATA_LEN_MSK	(0x0Ful << SPI_DATA_LEN_POS)
#define SPI_DATA_LEN(val)	(((val-1) & 0x0Ful) << SPI_DATA_LEN_POS)	
typedef enum
{
	SPI_LEN_4		= 3,            
	SPI_LEN_5,            
	SPI_LEN_6,            
	SPI_LEN_7,            
	SPI_LEN_8,  
	SPI_LEN_9,      
	SPI_LEN_10,            
	SPI_LEN_11,            
	SPI_LEN_12,            
	SPI_LEN_13,            
	SPI_LEN_14,  
	SPI_LEN_15,  
	SPI_LEN_16
}spi_data_len_e;

#define	SPI_RFR_POS		(4)							//Frame Format 		
#define	SPI_RFR_MSK		(0x03ul << SPI_RFR_POS)

#define SPI_WORK_MODE_POS	(6)							//SPICLK Polarity / Phase   
#define SPI_WORK_MODE_MSK	(0x03ul << SPI_WORK_MODE_POS)
typedef enum
{
	SPI_MODE_0	= (0x00ul),            
	SPI_MODE_1	= (0x02ul),  
    SPI_MODE_2	= (0x01ul),            
	SPI_MODE_3  = (0x03ul)            
}spi_work_mode_e;

#define SPI_SCR_POS		(8)							//Serial Clock Rate   			
#define SPI_SCR_MSK		(0xFFul << SPI_SCR_POS)						
#define SPI_SCR(val)	(((val) & 0xFFul) << SPI_SCR_POS)	

/******************************************************************************
* SPICR1 : Control Register 1
******************************************************************************/ 
#define	SPI_LBM_POS		(0)					//Loopback mode 				 				
#define	SPI_LBM_MSK		(0x01ul << SPI_LBM_POS)
typedef enum
{
	SPI_LBM_DIS		= 0,            
	SPI_LBM_EN   
}spi_lbm_e;

#define	SPI_SSE_POS		(1)	
#define	SPI_SSE_EN		(0x01ul << SPI_SSE_POS)							 				
#define	SPI_SSE_MSK		(0x01ul << SPI_SSE_POS)

#define	SPI_MODE_POS	(2)					//Master / Slave Mode Select  	
#define	SPI_MODE_MSK	(0x01ul << SPI_MODE_POS)
typedef enum
{
	SPI_MASTER	= 0,            
	SPI_SLAVE      
}spi_mode_e;

#define	SPI_SOD_POS		(3)					
#define	SPI_SOD_MSK		(0x01ul << SPI_SOD_POS)
typedef enum
{
	SPI_SOD_TXEN	= 0,            
	SPI_SOD_TXDIS      
}spi_sod_e;

#define	SPI_RXIFL_POS	(4)					//RX INT FIFO Level Select    
#define	SPI_RXIFL_MSK	(0x07ul << SPI_RXIFL_POS)
#define SPI_RXIFL(val)	(((val) & 0x07ul) << SPI_RXIFL_POS)
typedef enum
{
	SPI_RXFIFO_1_8	= (0x01ul),            
	SPI_RXFIFO_1_4  = (0x02ul),        
	SPI_RXFIFO_1_2  = (0x04ul)   
}spi_rxifl_e;

#define	SPI_LPMD_POS		(7)				
#define	SPI_LPMD_MSK		(0x01ul << SPI_LPMD_POS)
typedef enum
{
	SPI_LPMD_NORMAL	= 0,            
	SPI_LPMD_SINGLE      
}spi_lpmd_e;

#define	SPI_LPTXOE_POS		(8)					
#define	SPI_LPTXOE_MSK		(0x01ul << SPI_LPTXOE_POS)
typedef enum
{
	SPI_LPTXOE_DIS	= 0,            
	SPI_LPTXOE_EN      
}spi_lptxoe_e;

/******************************************************************************
* SPIDR : Data Register
******************************************************************************/
// SPIDR : Data Register
#define	SPI_DATA_POS		(0)				//Transmit/Receive FIFO 		 
#define	SPI_DATA_MSK		(0xFFFFul << SPI_RXFIFO_POS)

/******************************************************************************
* SPISR : Status Register
******************************************************************************/
// SPISR : Status Register
typedef enum
{
	SPI_TFE		= (0x01ul << 0), 			//Transmit FIFO Empty                    
	SPI_TNF     = (0x01ul << 1), 			//Transmit FIFO is not Full          
	SPI_RNE     = (0x01ul << 2),			//Receive is not Empty             
	SPI_RFF		= (0x01ul << 3),   			//Receive FIFO Full                    
	SPI_BSY     = (0x01ul << 4) 			//PrimeCell SPI Busy Flag            
}spi_sr_e;

/******************************************************************************
* CPSR : Clock prescale register
******************************************************************************/
// CPSR : Clock prescale register
#define	SPI_CPSDVSR_POS		(0)				//Clock Prescale Devisor		 
#define	SPI_CPSDVSR_MSK		(0xFFul << SPI_CPSDVSR_POS)
#define SPI_CPSDVSR(val)	(((val) & 0xFFul) << SPI_CPSDVSR_POS)

/******************************************************************************
* interrupt
******************************************************************************/
typedef enum
{
	SPI_INT_RX_OV	= (0x01ul << 0), 		//Receive Over flow Interrupt              
	SPI_INT_RXTO  	= (0x01ul << 1), 		//Receive Timeout Interrupt          
	SPI_INT_RXFIFO  = (0x01ul << 2),		//Receive FIFO Interrupt           
	SPI_INT_TXFIFO	= (0x01ul << 3),   		//Transmit FIFO interrupt 
	SPI_INT_ALL		= (0x0F),   			//ALL interrupt 
}spi_int_e;

/******************************************************************************
* DMACR : DMA Control register 
******************************************************************************/
#define	SPI_RDMA_EN_POS		(0)		
#define	SPI_RDMA_EN_MSK		(0x01ul << SPI_RDMA_EN_POS)	
typedef enum{
	SPI_RDMA_DIS		= 0,
	SPI_RDMA_EN
}spi_rdma_en_e;

#define	SPI_TDMA_EN_POS		(1)	
#define	SPI_TDMA_EN_MSK		(0x01ul << SPI_TDMA_EN_POS)	
typedef enum{
	SPI_TDMA_DIS		= 0,
	SPI_TDMA_EN
}spi_tdma_en_e;

#define	SPI_RDMA_MD_POS	(2)			 
#define	SPI_RDMA_MD_MSK	(0x01ul << SPI_RDMA_MD_POS)	
typedef enum{
	SPI_RDMA_FIFO_NSPACE = 0,
	SPI_RDMA_FIFO_TRG
}spi_rdma_md_e;

#define	SPI_TDMA_MD_POS	(3)			 
#define	SPI_TDMA_MD_MSK	(0x01ul << SPI_TDMA_MD_POS)	
typedef enum{
	SPI_TDMA_FIFO_NFULL		= 0,
	SPI_TDMA_FIF0_TRG
}spi_tdma_md_e;

/******************************************************************************
* SRR : Software Reset register 
******************************************************************************/
#define	SPI_SWRST_POS			(0)			 
#define	SPI_SWRST_MSK			(0x01ul << SPI_SWRST_POS)

#define	SPI_RXFIFO_RST_POS		(8)			 
#define SPI_RXFIFO_RST_MSK		(0x01ul << SPI_RXFIFO_RST_POS)

#define	SPI_TXFIFO_RST_POS		(9)			 
#define SPI_TXFIFO_RST_MSK		(0x01ul << SPI_TXFIFO_RST_POS)

/******************************************************************************
********************* SPI inline Functions Declaration **********************
******************************************************************************/
static inline void csp_spi_enable(csp_spi_t *ptSpiBase)
{
	ptSpiBase->CR1 |= SPI_SSE_EN;
}

static inline void csp_spi_disable(csp_spi_t *ptSpiBase)
{
	ptSpiBase->CR1 &= ~SPI_SSE_EN;
}

static inline uint16_t csp_spi_get_data(csp_spi_t *ptSpiBase)
{
	return (uint16_t)(ptSpiBase->DR);
}

static inline void csp_spi_set_data(csp_spi_t *ptSpiBase,uint16_t hwData)
{
	ptSpiBase->DR = hwData;
}

static inline void csp_spi_set_mode(csp_spi_t *ptSpiBase,spi_mode_e eMode)
{
	ptSpiBase->CR1 = (ptSpiBase->CR1 & (~SPI_MODE_MSK)) | (eMode << SPI_MODE_POS);
}

static inline uint8_t csp_spi_get__mode(csp_spi_t *ptSpiBase)
{
	return (uint8_t)((ptSpiBase->CR1 & SPI_MODE_MSK) >> SPI_MODE_POS);
}

static inline void csp_spi_set_work_mode(csp_spi_t *ptSpiBase, spi_work_mode_e eSpoSph)
{
	ptSpiBase->CR0 = (ptSpiBase->CR0 & (~SPI_WORK_MODE_MSK)) | (eSpoSph << SPI_WORK_MODE_POS);
}

static inline void csp_spi_set_data_size(csp_spi_t *ptSpiBase,spi_data_len_e eSize)
{
	ptSpiBase->CR0 = (ptSpiBase->CR0 & (~SPI_DATA_LEN_MSK)) | (eSize << SPI_DATA_LEN_POS);
}

static inline uint8_t csp_spi_get_data_size(csp_spi_t *ptSpiBase)
{
	return (uint8_t)(ptSpiBase->CR0 & SPI_DATA_LEN_MSK);
}

static inline void csp_spi_set_fifo(csp_spi_t *ptSpiBase,spi_rxifl_e eRxIfl)
{
	ptSpiBase->CR1  = (ptSpiBase->CR1 & (~SPI_RXIFL_MSK)) | (eRxIfl << SPI_RXIFL_POS);
}

static inline void csp_spi_set_loop_back_mode(csp_spi_t *ptSpiBase, spi_lbm_e eLbmSel)
{
	ptSpiBase->CR1  = (ptSpiBase->CR1 & (~SPI_LBM_MSK)) | (eLbmSel << SPI_LBM_POS);
}

static inline void csp_spi_set_single_wire_mode(csp_spi_t *ptSpiBase, spi_lpmd_e eLpmd, spi_lptxoe_e eLptxoe)
{
	ptSpiBase->CR1  = (ptSpiBase->CR1 & ~(SPI_LPMD_MSK | SPI_LPTXOE_MSK)) | (eLpmd << SPI_LPMD_POS) | (eLptxoe << SPI_LPTXOE_POS);
}

static inline uint32_t csp_spi_get_sr(csp_spi_t *ptSpiBase)
{
	return (uint32_t)(ptSpiBase->SR);
}

static inline uint8_t csp_spi_get_fifo(csp_spi_t *ptSpiBase)
{
	return ptSpiBase->CR1 & (SPI_RXIFL_MSK) ;
}

static inline void csp_spi_set_clk_div(csp_spi_t *ptSpiBase,uint8_t wScr,uint8_t wCpsdvsr)
{
	ptSpiBase->CR0 = (ptSpiBase->CR0 & (~SPI_SCR_MSK)) | SPI_SCR(wScr);
	ptSpiBase->CPSR  = wCpsdvsr;	
}

static inline void csp_spi_int_enable(csp_spi_t *ptSpiBase,spi_int_e eSpiInt)
{
	ptSpiBase->IMCR |= eSpiInt;
}

static inline void csp_spi_int_disable(csp_spi_t *ptSpiBase,spi_int_e eSpiInt)
{
	ptSpiBase->IMCR &= ~eSpiInt;
}

static inline uint32_t csp_spi_get_risr(csp_spi_t *ptSpiBase)
{
	return (ptSpiBase->RISR);
}

static inline uint32_t csp_spi_get_isr(csp_spi_t *ptSpiBase)
{
	return (ptSpiBase->MISR);
}

static inline void csp_spi_clr_isr(csp_spi_t *ptSpiBase, spi_int_e eSpiInt)
{
	ptSpiBase->ICR = eSpiInt;
}

static inline void csp_spi_sw_rst(csp_spi_t *ptSpiBase)
{
	ptSpiBase->SRR = SPI_SWRST_MSK;
}

static inline void csp_spi_rxfifo_sw_rst(csp_spi_t *ptSpiBase)
{
	ptSpiBase->SRR = SPI_RXFIFO_RST_MSK;	
}

static inline void	csp_spi_txfifo_sw_rst(csp_spi_t *ptSpiBase)		
{ 
	ptSpiBase->SRR = SPI_TXFIFO_RST_MSK;	
}

static inline void csp_spi_set_rxdma(csp_spi_t *ptSpiBase,  spi_rdma_md_e eRxDmaMode)
{
	ptSpiBase->DMACR = (ptSpiBase->DMACR & ~SPI_RDMA_MD_MSK) | (SPI_RDMA_EN << SPI_RDMA_EN_POS) | (eRxDmaMode << SPI_RDMA_MD_POS);
}

static inline void csp_spi_set_txdma(csp_spi_t *ptSpiBase, spi_rdma_md_e eTxDmaMode)	
{
	ptSpiBase->DMACR = (ptSpiBase->DMACR & ~SPI_TDMA_MD_MSK) | (SPI_TDMA_EN << SPI_TDMA_EN_POS) | (eTxDmaMode << SPI_TDMA_MD_POS);
}
#endif