/***********************************************************************//** 
 * \file  csp_iic.h
 * \brief  IIC description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-9-01 <td>V0.0  <td>ZJY   <td>initial
 * <tr><td> 2021-1-21 <td>V0.1  <td>ZJY   <td> modified in 110
 * <tr><td> 2021-1-22 <td>V0.2  <td>WNN   <td> move to 102
 * <tr><td> 2023-3-2 <td>V0.3   <td>YT    <td> move to 173
 * <tr><td> 2023-9-14 <td>V0.4  <td>YT    <td>code normalization
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CSP_IIC_H
#define _CSP_IIC_H

/* Includes ------------------------------------------------------------------*/
#include <soc.h>

/// \struct csp_iic_t
/// \brief IIC reg description     
typedef struct
{
    __IOM  uint32_t  CR;		//0x0000	IIC Control                           	 
    __IOM  uint32_t  TADDR;		//0x0004	IIC Target Address                     	 
    __IOM  uint32_t  SADDR;     //0x0008	IIC Slave Address                         
    __IOM  uint32_t  RSVD1;     //0x000C 
    __IOM  uint32_t  DATA_CMD;  //0x0010	IIC Rx/Tx Data Buffer and Command         
    __IOM  uint32_t  SS_SCLH;   //0x0014	IIC Standard Speed SCL High Count         
    __IOM  uint32_t  SS_SCLL;   //0x0018	IIC Standard Speed SCL Low  Count         
    __IOM  uint32_t  FS_SCLH;   //0x001C	IIC Fast mode and Fast Plus SCL High Count
    __IOM  uint32_t  FS_SCLL;   //0x0020	IIC Fast mode and Fast Plus SCL Low Count
    __IOM  uint32_t  RSVD2;		//0x0024                 
    __IOM  uint32_t  RSVD3;     //0x0028          	 
    __IOM  uint32_t  RX_FLSEL;  //0x002C 	IIC Receive FIFO Threshold            	 
    __IOM  uint32_t  TX_FLSEL;  //0x0030	IIC Transmit FIFO Threshold            	 
    __IM  uint32_t  RX_FL;     //0x0034	IIC Receive  FIFO Level               	 
    __IM  uint32_t  TX_FL;     //0x0038	IIC Transmit FIFO Level                	 
    __IOM  uint32_t  IICENABLE; //0x003C 	IIC Enable                            	 
    __IM   uint32_t  STATUS;    //0x0040 	IIC Status                            	 
    __IOM  uint32_t  RSVD4;     //0x0044              	 
    __IOM  uint32_t  SDA_TSETUP;//0x0048	IIC SDA Setup Time                    	 
    __IOM  uint32_t  SDA_THOLD; //0x004C 	IIC SDA hold time length              	 
    __IOM  uint32_t  SPKLEN;    //0x0050 	IIC SS and FS Spike Suppression Limit 	 
    __IOM  uint32_t  RSVD5;		//0x0054
	__IM   uint32_t  MISR;      //0x0058 	IIC Interrupt Status           	 
    __IOM  uint32_t  IMCR;     	//0x005C 	IIC Interrupt Enable                  	 
    __IM   uint32_t  RISR;      //0x0060 	IIC Raw Interrupt Status            	     
    __OM   uint32_t  ICR;       //0x0064 	IIC Interrupt Status Clear                   	 
    __IOM  uint32_t  RSVD6;		//0x0068
    __IOM  uint32_t  SCL_TOUT; 	//0x006C 	IIC SCL Stuck at Low Timeout          	 
    __IOM  uint32_t  SDA_TOUT; 	//0x0070 	IIC SDA Stuck at Low Timeout          	 
    __IM   uint32_t  TX_ABRT; 	//0x0074 	IIC Transmit Abort Status             	 
    __IOM  uint32_t  GCALL;     //0x0078 	IIC ACK General Call                  	 
	__IOM  uint32_t  NACK; 		//0x007C 	IIC Generate SLV_DATA_NACK
	__IOM  uint32_t  DMACR; 	//0x0080 	DMA Control
    __IOM  uint32_t  RSVD7;		//0x0084	
    __IOM  uint32_t  RSVD8;		//0x0088	
    __IOM  uint32_t  SRR; 		//0x008C 	Soft Reset Control   
    __IOM  uint32_t  SADDRQUAL; //0x0090 	IIC Slave Address Qualifier
    __IM  uint32_t  ADDRMON; 	//0x0094 	IIC Slave Address Monitor
	
} csp_iic_t;

/*****************************************************************************
************************** ssp Function defined *****************************
******************************************************************************/
#define IIC_RECV_TIMEOUT		(10000U)
#define IIC_SEND_TIMEOUT		(10000U)

typedef enum
{
	AADR_8BIT		= 0U,
    AADR_16BIT			         
}iic_addr_size_e;

/******************************************************************************
* CR : IIC Control Register 
******************************************************************************/
#define	IIC_MODE_POS		(0)			//IIC Mode: Master/Slave           
#define	IIC_MODE_MSK		(0x41ul << IIC_MODE_POS)
typedef enum
{
	IIC_MASTER			= (0x41ul),
    IIC_SLAVE			= (0x00ul)           
}iic_mode_e;

#define	IIC_SPEED_POS		(1)			//IIC Speed Mode: Standard/Fast    
#define	IIC_SPEED_MSK		(0x03ul << IIC_SPEED_POS)
typedef enum
{
	IIC_SPEED_STAN		= (0x01ul),
	IIC_SPEED_FAST		= (0x02ul)          
}iic_speed_e;

#define	IIC_ADDR_BIT_SLAVE_POS		(3)	//IIC Addr Bit Num                 
#define	IIC_ADDR_BIT_SLAVE_MSK		(0x01ul << IIC_ADDR_BIT_SLAVE_POS)
#define	IIC_ADDR_BIT_MASTER_POS		(4)
#define	IIC_ADDR_BIT_MASTER_MSK		(0x01ul << IIC_ADDR_BIT_MASTER_POS)
typedef enum
{
	IIC_ADDR_7BIT	= 0,
	IIC_ADDR_10BIT 
}iic_addr_mode_e;

#define	IIC_RESTART_POS		(5)			//IIC Restart Enable  
#define	IIC_RESTART_MSK		(0x01ul << IIC_RESTART_POS)
typedef enum
{
	IIC_RESTART_DIS		= 0,
	IIC_RESTART_EN
}iic_restart_e;

#define	IIC_STOPDET_POS		(7)			//IIC STOP DET
#define	IIC_STOPDET_MSK		(0x01ul << IIC_STOPDET_POS)
typedef enum
{
	IIC_STOPDET_ALS		= 0,
	IIC_STOPDET_IFADD
}iic_stop_det_e;

#define	IIC_TX_EMPTY_POS	(8)			//IIC TX_EMPTY Control
#define	IIC_TX_EMPTY_MSK	(0x01ul << IIC_TX_EMPTY_POS)
typedef enum
{
	IIC_TX_EMPTY_CTRL	= 0,
	IIC_TX_EMPTY_DONE
}iic_tx_empty_e;

#define	IIC_RXFULL_HLD_POS	(9)			//IIC Rx Full Hold Ctr
#define	IIC_RXFULL_HLD_MSK	(0x01ul << IIC_RXFULL_HLD_POS)
typedef enum
{
	IIC_RXFULL_HLD_NULL	= 0,
	IIC_RXFULL_HLD_DONE
}iic_rxfull_hld_e;

#define	IIC_BUS_CLR_POS		(11)		//IIC Enable Bus Clr 
#define	IIC_BUS_CLR_MSK		(0x01ul << IIC_BUS_CLR_POS)
typedef enum
{
	IIC_BUS_CLR_DIS		= 0,
	IIC_BUS_CLR_EN
}iic_bus_clr_e;
/******************************************************************************
* TADDR : IIC Target Address 
******************************************************************************/
#define	IIC_TADDR_POS		(0)
#define	IIC_TADDR_MSK		(0xFFFul << IIC_TADDR_POS)
typedef enum
{
	TADDR_NORMAL_0		= (0x00ul << 10),
	TADDR_NORMAL_1		= (0x01ul << 10),
	TADDR_GCALL			= (0x02ul << 10),
	TADDR_STARTB		= (0x03ul << 10)
}iic_taddr_special_e;
/******************************************************************************
* SADDR : IIC Slave Address 
******************************************************************************/
#define	IIC_SADDR_POS		(0)
#define	IIC_SADDR_MSK		(0x3FFul << IIC_SADDR_POS)

/******************************************************************************
* DATA_CMD : IIC Data and Command Register 
******************************************************************************/
#define	IIC_CMD_POS		(8)
#define	IIC_CMD_MSK		(0x07ul << IIC_CMD_POS)
typedef enum
{
	IIC_CMD_WRITE      	= (0x00ul << 8),	//IIC Read Command  
	IIC_CMD_READ		= (0x01ul << 8),	//IIC Write Command   
	IIC_CMD_STOP        = (0x01ul << 9),	//IIC Stop after this byte 
	IIC_CMD_NONESTOP    = (0x00ul << 9),	//IIC None Stop  
	IIC_CMD_RESTART0    = (0x00ul << 10),	//IIC Restart Mode0  
	IIC_CMD_RESTART1    = (0x01ul << 10)	//IIC Restart Mode1 
}iic_cmd_e;

#define IIC_DATA_POS       (0)  
#define IIC_DATA_MSK       (0xFFul << IIC_DATA_POS) 
#define IIC_DATA(val)      (((val) & 0xFFul) << IIC_DATA_POS)   

/*****************************************************************************
* ENABLE : IIC Enable Register
******************************************************************************/                     
#define IIC_ENABLE_POS      (0)   			//IIC Enable/Disable          
#define IIC_ENABLE_MSK      (0x01ul << IIC_ENABLE_POS)   			
typedef enum
{
	IIC_DISABLE			= 0,
	IIC_ENABLE
}iic_enable_e;

#define IIC_ABORT_POS		(1)   			//IIC Abort Transfer          
#define IIC_ABORT_MSK       (0x01ul << IIC_ENABLE_POS)   			
typedef enum
{
	IIC_ABORT_DIS		= 0,
	IIC_ABORT_EN
}iic_abort_e;

#define IIC_RECOVER_POS		(3)   			//IIC Enable Stuck Recovery    
#define IIC_RECOVER_MSK     (0x01ul << IIC_RECOVER_POS)   			
typedef enum
{
	IIC_RECOVER_DIS		= 0,
	IIC_RECOVER_EN
	
}iic_recover_e;

/*****************************************************************************
* STATUS : IIC STATUS Register
******************************************************************************/                     
typedef enum
{
	IIC_BUSY		= (0x01ul << 0),   //IIC Activity                	
	IIC_TFNF        = (0x01ul << 1),   //IIC Transmit FIFO Not Full 	    
	IIC_TFE         = (0x01ul << 2),   //IIC Transmit FIFO Empty    	    
	IIC_RFNE        = (0x01ul << 3),   //IIC Receive FIFO Not Empty      
	IIC_RFF         = (0x01ul << 4),   //IIC Receive FIFO Full           
	IIC_MST_BUSY    = (0x01ul << 5),   //IIC Master FSM Activity         
	IIC_SLV_BUSY    = (0x01ul << 6),   //IIC Slave  FSM Activity         
	IIC_REC_FAIL    = (0x01ul << 11),  //IIC Recovery FAIL               
	IIC_ENABLE_SR   = (0x01ul << 12),  //IIC Enable Status               
	IIC_ABORT_SR    = (0x01ul << 13),  //IIC Abort  Status               
	IIC_SDA_REC_SR  = (0x01ul << 15)   //IIC SDA Recover Status          
}iic_status_e;

/*****************************************************************************
* RISR/MISR/IMSCR/ICR : IIC Interrupt Mask/Status Register
******************************************************************************/  
typedef enum
{
	IIC_INT_RX_UNDER		= (0x01ul << 0),   //IIC Interrupt Status  
	IIC_INT_RX_OVER         = (0x01ul << 1),   //IIC Interrupt Status   
	IIC_INT_RX_FULL        = (0x01ul << 2),   //IIC Interrupt Status         
	I2_INT_TX_OVER          = (0x01ul << 3),   //IIC Interrupt Status         
	IIC_INT_TX_EMPTY        = (0x01ul << 4),   //IIC Interrupt Status         
	IIC_INT_RD_REQ          = (0x01ul << 5),   //IIC Interrupt Status         
	IIC_INT_TX_ABRT         = (0x01ul << 6),   //IIC Interrupt Status         
	IIC_INT_RX_DONE         = (0x01ul << 7),   //IIC Interrupt Status         
	IIC_INT_BUSY      	 	= (0x01ul << 8),   //IIC Interrupt Status         
	IIC_INT_STOP_DET        = (0x01ul << 9),   //IIC Interrupt Status         
	IIC_INT_START_DET       = (0x01ul <<10),   //IIC Interrupt Status         
	IIC_INT_GEN_CALL        = (0x01ul <<11),   //IIC Interrupt Status         
	IIC_INT_RESTART_DET     = (0x01ul <<12),   //IIC Interrupt Status               
	IIC_INT_SCL_SLOW        = (0x01ul <<14),    //IIC Interrupt Status 
	IIC_INT_ALL            = (0xFFFF<<0)		
}iic_int_e;

/*****************************************************************************
* SS_SCLH : Standard Speed SCL High Count  
******************************************************************************/  
#define	IIC_SS_SCLH_POS		(0)
#define	IIC_SS_SCLH_MSK		(0xFFFFul << IIC_SS_SCLH_POS)

/*****************************************************************************
* SS_SCLL : Standard Speed SCL Low  Count  
******************************************************************************/ 
#define	IIC_SS_SCLL_POS		(0)
#define	IIC_SS_SCLL_MSK		(0xFFFFul << IIC_SS_SCLL_POS)

/*****************************************************************************
* FS_SCLH : Fast mode and Fast Plus SCL High Count  
******************************************************************************/  
#define	IIC_FS_SCLH_POS		(0)
#define	IIC_FS_SCLH_MSK		(0xFFFFul << IIC_FS_SCLH_POS)

/*****************************************************************************
* FS_SCLL : Fast mode and Fast Plus SCL Low  Count  
******************************************************************************/ 
#define	IIC_FS_SCLL_POS		(0)
#define	IIC_FS_SCLL_MSK		(0xFFFFul << IIC_FS_SCLL_POS)

/*****************************************************************************
* RX_FLSEL : IIC Receive FIFO Threshold
******************************************************************************/  
#define	IIC_RX_FLSEL_POS	(0)
#define	IIC_RX_FLSEL_MSK	(0x07ul << IIC_RX_FLSEL_POS)

/*****************************************************************************
* TX_FLSEL : IIC Transmit FIFO Threshold    
******************************************************************************/  
#define	IIC_TX_FLSEL_POS	(0)
#define	IIC_TX_FLSEL_MSK	(0x07ul << IIC_TX_FLSEL_POS)

/*****************************************************************************
* RX_FL : IIC Receive  FIFO Level  
******************************************************************************/ 
#define	IIC_RX_FL_POS		(0)
#define	IIC_RX_FL_MSK		(0x0Ful << IIC_RX_FL_POS) 
/*****************************************************************************
* TX_FL : IIC Transmit FIFO Level 
******************************************************************************/  
#define	IIC_TX_FL_POS		(0)
#define	IIC_TX_FL_MSK		(0x0Ful << IIC_TX_FL_POS) 

/*****************************************************************************
* SDA_TSETUP : IIC SDA Setup Time   
******************************************************************************/  
#define	IIC_SETUP_POS		(0)
#define	IIC_SETUP_MSK		(0xFFul << IIC_SETUP_POS)

/*****************************************************************************
* SDA_THOLD : IIC SDA hold time length    
******************************************************************************/ 
#define IIC_TXHOLD_POS		(0)   			
#define IIC_TXHOLD_MSK      (0xFFFFul << IIC_TXHOLD_POS)   			

#define IIC_RXHOLD_POS		(16)   			
#define IIC_RXHOLD_MSK      (0x00FFul << IIC_RXHOLD_POS)  

/*****************************************************************************
* SPKLEN : IIC SS and FS Spike Suppression Limit   
******************************************************************************/  
#define	IIC_SPKLEN_POS		(0)
#define	IIC_SPKLEN_MSK		(0xFFul << IIC_SPKLEN_POS)

/*****************************************************************************
* SCL/SDA TOUT : IIC SCL/SDA Stuck Time Out
******************************************************************************/   
#define IIC_SCL_TOUT_POS    (0)
#define IIC_SCL_TOUT_MSK    (0xFFFFFFFFul << IIC_SCL_TOUT_POS) 
#define IIC_SDA_TOUT_POS    (0)
#define IIC_SDA_TOUT_MSK    (0xFFFFFFFFul << IIC_SDA_TOUT_POS)    
#define IIC_SCL_TOUT(val)   (((val) & 0xFFFFFFFFul) << 0) //SCL Stuck Time Out 
#define IIC_SDA_TOUT(val)   (((val) & 0xFFFFFFFFul) << 0) //SDA Stuck Time Out 

/*****************************************************************************
* TX_ABRT : IIC Transmit Abort Status
******************************************************************************/                     
typedef enum
{
	TX_ABRT_7ADDR_NACK		= (0x01ul << 0),
	TX_ABRT_10ADDR1_NACK	= (0x01ul << 1),
	TX_ABRT_10ADDR2_NACK	= (0x01ul << 2),
	TX_ABRT_TXDATA_NACK		= (0x01ul << 3),
	TX_ABRT_GCALL_NACK		= (0x01ul << 4),
	TX_ABRT_GCALL_READ		= (0x01ul << 5),
	TX_ABRT_SBYTE_ACK		= (0x01ul << 7),
	TX_ABRT_SBYTE_NRSTRT	= (0x01ul << 9),
	TX_ABRT_10B_RD_NRSTRT	= (0x01ul << 10),
	TX_ABRT_MASTER_DIS		= (0x01ul << 11),
	TX_ABRT_ARB_LOST		= (0x01ul << 12),
	TX_ABRT_SLVFLUSH_TX		= (0x01ul << 13),
	TX_ABRT_SLV_ARBLOST		= (0x01ul << 14),
	TX_ABRT_SLVRD_INTX		= (0x01ul << 15),
	TX_ABRT_USER_ABRT		= (0x01ul << 16),
	TX_ABRT_SDA_S_LOW		= (0x01ul << 17)
}iic_tx_abrt_e;

/*****************************************************************************
* GCALL : IIC General Call Register
******************************************************************************/                     
#define IIC_ACK_GCALL_POS	(0) 			//Uses ACK to answer General Call    			
#define IIC_ACK_GCALL_MSK	(0x01ul << IIC_ACK_GCALL_POS)   			
typedef enum
{
	IIC_ACK_GCALL_DIS	= 0,
	IIC_ACK_GCALL_EN
}iic_ack_gcall_e;

/*****************************************************************************
* NACK : IIC Slave NACK Control Register
******************************************************************************/                     
//#define IIC_NACK_DATA     (0x01ul << 0)   //Generate a NACK After received byte 
//#define IIC_NACK_NORMAL   (0x00ul << 0)   //Generate NACK/ACK as normal         
#define IIC_NACK_POS		(0) 			//Uses ACK to answer General Call    			
#define IIC_NACK_MSK		(0x01ul << IIC_NACK_POS)   			
typedef enum
{
	IIC_NACK_NORMAL	= 0,
	IIC_NACK_DATA
}iic_nack_e;

/*****************************************************************************
* DMACR : IIC DMA Control Register
******************************************************************************/ 
#define	IIC_RDMA_EN_POS		(0)		
#define	IIC_RDMA_EN_MSK		(0x01ul << IIC_RDMA_EN_POS)	
typedef enum{
	IIC_RDMA_DIS		= 0,
	IIC_RDMA_EN
}iic_rdma_en_e;

#define	IIC_TDMA_EN_POS		(1)	
#define	IIC_TDMA_EN_MSK		(0x01ul << IIC_TDMA_EN_POS)	
typedef enum{
	IIC_TDMA_DIS		= 0,
	IIC_TDMA_EN
}iic_tdma_en_e;

#define	IIC_RDMA_SEL_POS	(2)			 
#define	IIC_RDMA_SEL_MSK	(0x01ul << IIC_RDMA_SEL_POS)	
typedef enum{
	IIC_RDMA_FIFO_NSPACE = 0,
	IIC_RDMA_FIFO_RX_FLSEL
}iic_rdma_sel_e;

#define	IIC_TDMA_SEL_POS	(3)			 
#define	IIC_TDMA_SEL_MSK	(0x01ul << IIC_TDMA_SEL_POS)	
typedef enum{
	IIC_TDMA_FIFO_NFULL		= 0,
	IIC_TDMA_FIF0_TX_FLSEL
}iic_tdma_sel_e;

/*****************************************************************************
* SRR : IIC Soft Reset Register
******************************************************************************/ 
#define	IIC_SWRST_R_POS		(0)		
#define	IIC_SWRST_R_MSK		(0x01ul << IIC_SWRST_R_POS)	
typedef enum{
	IIC_SWRST_R_NO_AVAIL = 0,
	IIC_SWRST_R
}iic_swrst_r_e;

#define	IIC_SWRST_C_POS		(1)		
#define	IIC_SWRST_C_MSK		(0x01ul << IIC_SWRST_C_POS)	
typedef enum{
	IIC_SWRST_C_NO_AVAIL = 0,
	IIC_SWRST_C
}iic_swrst_c_e;

#define	IIC_SWRST_F_POS		(2)		
#define	IIC_SWRST_F_MSK		(0x01ul << IIC_SWRST_F_POS)	
typedef enum{
	IIC_SWRST_F_NO_AVAIL = 0,
	IIC_SWRST_F
}iic_swrst_f_e;


/*****************************************************************************
* SADDRQUAL : IIC Slave Address Qualifier Register
******************************************************************************/
#define	IIC_SLV_QUAL_POS		(0)		
#define	IIC_SLV_QUAL_MSK		(0x3FFul << IIC_SLV_QUAL_POS)

#define	IIC_QUAL_MODE_POS		(10)		
#define	IIC_QUAL_MODE_MSK		(0x01ul << IIC_QUAL_MODE_POS)	
typedef enum
{
	IIC_QUAL_MASK	= 0,
	IIC_QUAL_EXTEND    
}iic_qual_e;
/******************************************************************************
********************* IIC inline Functions Declaration ************************
******************************************************************************/
static inline void csp_iic_enable(csp_iic_t *ptIicBase)
{
	ptIicBase->IICENABLE |= IIC_ENABLE;
}
static inline void csp_iic_disable(csp_iic_t *ptIicBase)
{
	ptIicBase->IICENABLE = IIC_DISABLE;
}
static inline void csp_iic_set_speed(csp_iic_t *ptIicBase, iic_speed_e eSpeed)
{
	ptIicBase->CR = (ptIicBase->CR & (~IIC_SPEED_MSK)) | (eSpeed << IIC_SPEED_POS);
}
static inline void csp_iic_set_cr(csp_iic_t *ptIicBase, uint32_t wCr)
{
	ptIicBase->CR = (ptIicBase->CR&0XFFFFF000)| wCr;
}

static inline void csp_iic_set_saddr(csp_iic_t *ptIicBase, uint32_t wSaddr)
{
	ptIicBase->SADDR = (wSaddr & IIC_SADDR_MSK);  
}
static inline void csp_iic_set_taddr(csp_iic_t *ptIicBase, uint32_t wTAddr)
{
	ptIicBase->TADDR = (wTAddr & IIC_TADDR_MSK);
}
//
static inline void csp_iic_set_data_cmd(csp_iic_t *ptIicBase, uint16_t hwDataCmd)
{
	ptIicBase->DATA_CMD = hwDataCmd;
}
static inline uint8_t csp_iic_get_data(csp_iic_t *ptIicBase)
{
	return (uint8_t)(ptIicBase->DATA_CMD);
}
//
static inline void csp_iic_set_sda_tsetup(csp_iic_t *ptIicBase, uint8_t bySetTime)
{
	ptIicBase->SDA_TSETUP = bySetTime;
}
static inline void csp_iic_set_sda_thold(csp_iic_t *ptIicBase, uint16_t hwTxHold, uint8_t byRxHold)
{
	ptIicBase->SDA_THOLD = hwTxHold | (byRxHold << IIC_RXHOLD_POS);
}
//
static inline void csp_iic_set_spklen(csp_iic_t *ptIicBase, uint8_t bySpkLen)
{
	ptIicBase->SPKLEN = bySpkLen;
}
//
static inline void csp_iic_set_rx_flsel(csp_iic_t *ptIicBase, uint8_t byRxFL)
{
	ptIicBase->RX_FLSEL = (byRxFL & IIC_RX_FLSEL_MSK); 
}
static inline void csp_iic_set_tx_flsel(csp_iic_t *ptIicBase,uint8_t byTxFL)
{
	ptIicBase->TX_FLSEL = (byTxFL & IIC_TX_FLSEL_MSK); 
}
static inline uint8_t csp_iic_get_rx_fifo_num(csp_iic_t *ptIicBase)
{
	return (uint8_t)(ptIicBase->RX_FL & IIC_RX_FL_MSK);
}
static inline uint8_t csp_iic_get_tx_fifo_num(csp_iic_t *ptIicBase)
{
	return (uint8_t)(ptIicBase->TX_FL & IIC_TX_FL_MSK);
}

static inline void csp_iic_set_ss_sclh(csp_iic_t *ptIicBase, uint32_t wSsSclH)
{
	ptIicBase->SS_SCLH = wSsSclH; 
}
static inline void csp_iic_set_ss_scll(csp_iic_t *ptIicBase, uint32_t wSsSclL)
{
	ptIicBase->SS_SCLL = wSsSclL; 
}
static inline void csp_iic_set_fs_sclh(csp_iic_t *ptIicBase, uint32_t wFsSclH)
{
	ptIicBase->FS_SCLH = wFsSclH; 
}
static inline void csp_iic_set_fs_scll(csp_iic_t *ptIicBase, uint32_t wFsSclL)
{
	ptIicBase->FS_SCLL = wFsSclL; 
}

//
static inline void csp_iic_set_scl_tout(csp_iic_t *ptIicBase, uint32_t wSclTout)
{
	ptIicBase->SCL_TOUT = wSclTout; 
}
static inline void csp_iic_set_sda_tout(csp_iic_t *ptIicBase, uint32_t wSdaTout)
{
	ptIicBase->SDA_TOUT = wSdaTout; 
}
//
static inline void csp_iic_set_ack_gcall(csp_iic_t *ptIicBase, iic_ack_gcall_e eAckGcall)
{
	ptIicBase->GCALL = (ptIicBase->GCALL & ~IIC_ACK_GCALL_MSK) | eAckGcall;
}
static inline void csp_iic_set_nack(csp_iic_t *ptIicBase, iic_nack_e eNack)
{
	ptIicBase->NACK = (ptIicBase->NACK  & ~IIC_NACK_MSK) | eNack;
}


static inline uint16_t csp_iic_get_sr(csp_iic_t *ptIicBase)
{
	return (uint16_t)ptIicBase->STATUS;
}
static inline uint16_t csp_iic_get_risr(csp_iic_t *ptIicBase)
{
	return (uint16_t)ptIicBase->RISR;
}
static inline uint16_t csp_iic_get_isr(csp_iic_t *ptIicBase)
{
	return (uint16_t)ptIicBase->MISR;
}

static inline uint32_t csp_iic_get_iicenable(csp_iic_t *ptIicBase)
{
	return ptIicBase->IICENABLE;
}

static inline uint32_t csp_iic_get_tx_abrt(csp_iic_t *ptIicBase)
{
	return ptIicBase->TX_ABRT;
}

static inline void csp_iic_clr_isr(csp_iic_t *ptIicBase,iic_int_e eIntNum)
{
	ptIicBase->ICR = eIntNum;
}

static inline void csp_iic_int_enable(csp_iic_t *ptIicBase,iic_int_e eIntNum)
{
	ptIicBase->IMCR |= eIntNum;
}
static inline void csp_iic_int_disable(csp_iic_t *ptIicBase,iic_int_e eIntNum)
{
	ptIicBase->IMCR &= (~eIntNum);
}

static inline void csp_iic_restart_enable(csp_iic_t *ptIicBase)
{
	ptIicBase->CR |= (IIC_RESTART_EN << IIC_RESTART_POS);
}
static inline void csp_iic_restart_disable(csp_iic_t *ptIicBase)
{
	ptIicBase->CR &= ~IIC_RESTART_MSK;
}

static inline void csp_iic_busclr_enable(csp_iic_t *ptIicBase)
{
	ptIicBase->CR |= (IIC_BUS_CLR_EN << IIC_BUS_CLR_POS);
}
static inline void csp_iic_busclr_disable(csp_iic_t *ptIicBase)
{
	ptIicBase->CR &= IIC_BUS_CLR_MSK;
}

static inline void csp_iic_abort_enable(csp_iic_t *ptIicBase)
{
	ptIicBase->IICENABLE |= (IIC_ABORT_EN << IIC_ABORT_POS);
}
static inline void csp_iic_recover_enable(csp_iic_t *ptIicBase)
{
	ptIicBase->IICENABLE |= (IIC_RECOVER_EN << IIC_RECOVER_POS);
}
static inline void csp_iic_recover_disable(csp_iic_t *ptIicBase)
{
	ptIicBase->IICENABLE = (ptIicBase->IICENABLE & ~IIC_RECOVER_MSK);
}
static inline void csp_iic_stop(csp_iic_t *ptIicBase)
{
	ptIicBase->DATA_CMD |= IIC_CMD_STOP;
}

static inline void csp_iic_set_receive_dma(csp_iic_t *ptIicBase, iic_rdma_en_e eRxDmaEn, iic_rdma_sel_e eRxDmaSel) 
{
	ptIicBase->DMACR = (ptIicBase->DMACR & ~(IIC_RDMA_EN_MSK | IIC_RDMA_SEL_MSK)) | (eRxDmaEn << IIC_RDMA_EN_POS) | (eRxDmaSel << IIC_RDMA_SEL_POS);
}

static inline void csp_iic_set_send_dma(csp_iic_t *ptIicBase, iic_tdma_en_e eTxDmaEn, iic_tdma_sel_e eTxDmaSel) 
{
	ptIicBase->DMACR = (ptIicBase->DMACR & ~(IIC_TDMA_EN_MSK | IIC_TDMA_SEL_MSK)) | (eTxDmaEn << IIC_TDMA_EN_POS) | (eTxDmaSel << IIC_TDMA_SEL_POS);
}

static inline void csp_iic_reg_sw_rst(csp_iic_t *ptIicBase)
{
	ptIicBase->SRR |= (IIC_SWRST_R<<IIC_SWRST_R_POS);
}

static inline void csp_iic_logic_sw_rst(csp_iic_t *ptIicBase)
{
	ptIicBase->SRR |= (IIC_SWRST_C<<IIC_SWRST_C_POS);
}

static inline void csp_iic_fifo_sw_rst(csp_iic_t *ptIicBase)
{
	ptIicBase->SRR |= (IIC_SWRST_F<<IIC_SWRST_F_POS);
}

static inline void csp_iic_set_qualmode(csp_iic_t *ptIicBase, iic_qual_e eQualmode)
{
	ptIicBase->SADDRQUAL =(ptIicBase->SADDRQUAL  & ~IIC_QUAL_MODE_MSK) | (eQualmode<<IIC_QUAL_MODE_POS);
}

static inline void csp_iic_set_slvqual(csp_iic_t *ptIicBase, uint32_t wSlvqual)
{
	ptIicBase->SADDRQUAL = (ptIicBase->SADDRQUAL  & ~IIC_SLV_QUAL_MSK) | (wSlvqual<<IIC_SLV_QUAL_POS);
}

static inline uint32_t csp_iic_get_monaddr(csp_iic_t *ptIicBase)
{
	return ptIicBase->ADDRMON;
}



#endif

