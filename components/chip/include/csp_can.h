/***********************************************************************//** 
 * \file  csp_can.h
 * \brief  CAN description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-7-08 <td>V0.0  <td>ZJY   <td>initial
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CSP_CAN_H
#define _CSP_CAN_H

/* Includes ------------------------------------------------------------------*/
#include <soc.h>

typedef struct
{
	__IOM  	uint32_t	TMR;		//0x0100	IF0/1 Transfer Manage Register
	__IOM  	uint32_t	DAR;		//0x0104	IF0/1 Data A Register
	__IOM  	uint32_t	DBR;		//0x0108	IF0/1 Data A Register
	__IOM  	uint32_t	MASKR;		//0x010C	IF0/1 Mask Register
	__IOM  	uint32_t	IR;			//0x0110	IF0/1 Identifier Register
	__IOM  	uint32_t	MCR;		//0x0114	IF0/1 Message Control Register
	__IM  	uint32_t	STPR;		//0x0118	IF0/1 Timestamp Register
	__IM  	uint32_t	RSVD5;		//0x11C
	
} can_if0_1_regs_t;

/// \struct csp_can_t
/// \brief CAN reg description
typedef struct
{
	__IM  	uint32_t  	RSVD0[20];  //0x0000~0x4C
    __OM	uint32_t 	ECR;		//0x0050	Clk Enable Register  
    __OM  	uint32_t 	DCR;        //0x0054 	Clk Disable Register              
    __IM  	uint32_t 	PMSR;       //0x0058 	Clk Status Register  
    __IM  	uint32_t  	RSVD1;      //0x005C 	   
    __OM  	uint32_t  	CR;       	//0x0060 	Control Register    
	__IOM  	uint32_t  	MR;       	//0x0064 	Mode Register  
	__IM  	uint32_t  	RSVD2;      //0x0068 	     
	__OM  	uint32_t  	CSR;       	//0x006C 	Clear Status Register  
	__IM  	uint32_t  	SR;       	//0x0070 	Status Register  
    __OM  	uint32_t  	IER;     	//0x0074	Interrupt Enable Register  
    __OM  	uint32_t  	IDR;		//0x0078	Interrupt Disable Register  
	__IM  	uint32_t	IMR;		//0x007C	Interrupt Mask Register 
	__IM  	uint32_t  	RSVD3;      //0x0080 
	__IM  	uint32_t  	ISSR;       //0x0084 	Source Interrupt Status Register 	
	__OM  	uint32_t  	SIER;     	//0x0088	Source Interrupt Enable Register  
    __OM  	uint32_t  	SIDR;		//0x008C	Source Interrupt Disable Register  
	__IM  	uint32_t	SIMR;		//0x0090	Source Interrupt Mask Register 
	__IM  	uint32_t	HPIR;		//0x0094	Highest Priority Interrupt Register 
	__IM  	uint32_t	ERCR;		//0x0098	Error count Register 
	__IM  	uint32_t	RSVD4[25];	//0x009C	
	can_if0_1_regs_t	IF[2];		//0x100~0x11c; 120~	138 IFx Operate Register
	__IM  	uint32_t	TRR;		//0x140		Transfer Request Register 
	__IM  	uint32_t	NDR;		//0x144		Channel x New Data Status
	__IM  	uint32_t	MVR;		//0x148		Channel x Message valid register
	__IOM  	uint32_t	TSTR;		//0x14C		Test register
} csp_can_t; 

/*****************************************************************************
************************** CAN Para Macro defined ***************************
******************************************************************************/
#define CAN_TMR_ALL_TRAN	0x1F00
#define CAN_TMR_TX_NORMAL	0x1F80		//AMCR/AIR/AMSKR/ADBR/ADAR/WR
#define CAN_TMR_RX_NORMAL	0x1C80		//AMCR/AIR/AMSKR/WR		


/******************************************************************************
* ECR, DCR, PMSR : CAN Clk enable/disable/status Register
******************************************************************************/										
#define	CAN_CLKEN_POS			(1)			 
#define	CAN_CLKEN_MSK			(0x01ul << CAN_CLKEN_POS)	

#define	CAN_DBGEN_POS			(31)			 
#define	CAN_DBGEN_MSK			(0x01ul << CAN_DBGEN_POS)	
/******************************************************************************
* CR : CAN Control Register
******************************************************************************/	
#define	CAN_SWRST_POS			(0)			 
#define	CAN_SWRST_MSK			(0x01ul << CAN_SWRST_POS)	

#define	CAN_CANEN_POS			(1)			 
#define	CAN_CANEN_MSK			(0x01ul << CAN_CANEN_POS)	

#define	CAN_CANDIS_POS			(2)			 
#define	CAN_CANDIS_MSK			(0x01ul << CAN_CANDIS_POS)	

#define	CAN_CCEN_POS			(3)			 
#define	CAN_CCEN_MSK			(0x01ul << CAN_CCEN_POS)	

#define	CAN_CCDIS_POS			(4)			 
#define	CAN_CCDIS_MSK			(0x01ul << CAN_CCDIS_POS)	

#define	CAN_RQBTX_POS			(8)			 
#define	CAN_RQBTX_MSK			(0x01ul << CAN_RQBTX_POS)
	
#define	CAN_ABBTX_POS			(9)			 
#define	CAN_ABBTX_MSK			(0x01ul << CAN_ABBTX_POS)	

#define	CAN_STSR_POS			(10)			 
#define	CAN_STSR_MSK			(0x01ul << CAN_STSR_POS)

typedef enum{
	CAN_SWRST			= CAN_SWRST_MSK,
	CAN_CANEN			= CAN_CANEN_MSK,
	CAN_CANDIS			= CAN_CANDIS_MSK,
	CAN_CCEN			= CAN_CCEN_MSK,
	CAN_CCDIS			= CAN_CCDIS_MSK,
	CAN_RQBTX			= CAN_RQBTX_MSK,
	CAN_ABBTX			= CAN_ABBTX_MSK,
	CAN_STSR			= CAN_STSR_MSK
}can_cr_e;
	
/******************************************************************************
* MR : CAN Mode Register
******************************************************************************/	
#define	CAN_BD_POS				(0)			 
#define	CAN_BD_MSK				(0x3FFul << CAN_BD_POS)	
#define	CAN_BD(val)				(((val) & 0x3FF) << CAN_BD_POS)

#define	CAN_CSSEL_POS			(10)			 
#define	CAN_CSSEL_MSK			(0x01ul << CAN_CSSEL_POS)	
typedef enum{
	CAN_CSSEL_PCLK		= 0,
	CAN_CSSEL_EMCLK	
}can_cssel_e;

#define	CAN_SJW_POS				(12)			 
#define	CAN_SJW_MSK				(0x03ul << CAN_SJW_POS)	
#define	CAN_SJW(val)			(((val) & 0x03) << CAN_SJW_POS)

#define	CAN_AR_POS				(14)			 
#define	CAN_AR_MSK				(0x01ul << CAN_AR_POS)	
typedef enum{
	CAN_AR_DIS			= 0,
	CAN_AR_EN	
}can_ar_e;

#define	CAN_PHSEG1_POS			(16)			 
#define	CAN_PHSEG1_MSK			(0x0Ful << CAN_PHSEG1_POS)	
#define	CAN_PHSEG1(val)			(((val) & 0x0F) << CAN_PHSEG1_POS)

#define	CAN_PHSEG2_POS			(20)			 
#define	CAN_PHSEG2_MSK			(0x07ul << CAN_PHSEG2_POS)	
#define	CAN_PHSEG2(val)			(((val) & 0x07) << CAN_PHSEG2_POS)

/******************************************************************************
* SR, CSR, IER, IDR, IMR: CAN Status/Interrupt Register
******************************************************************************/	
typedef enum{
	CAN_ISS_S			= (0x01ul << 0),		//Only Status
	CAN_ERWARNTR_INT	= (0x01ul << 1),		//Iterrupt and Status
	CAN_ERPASSTR_INT	= (0x01ul << 2),
	CAN_BUSOFFTR_INT	= (0x01ul << 3),
	CAN_ACTVT_INT		= (0x01ul << 4),
	CAN_RXOK_INT		= (0x01ul << 8),
	CAN_TXOK_INT		= (0x01ul << 9),
	CAN_STUFF_INT		= (0x01ul << 10),
	CAN_FORM_INT		= (0x01ul << 11),
	CAN_ACK_INT			= (0x01ul << 12),
	CAN_BIT1_INT		= (0x01ul << 13),		
	CAN_BIT0_INT		= (0x01ul << 14),
	CAN_CRC_INT			= (0x01ul << 15),
	CAN_CANENS_S		= (0x01ul << 16),		//Only Status
	CAN_ERWARN_S		= (0x01ul << 17),		
	CAN_ERPASS_S		= (0x01ul << 18),		
	CAN_BUSOFF_S		= (0x01ul << 19),		
	CAN_BUSY0_S			= (0x01ul << 20),
	CAN_BUSY1_S			= (0x01ul << 21),
	CAN_RS_S			= (0x01ul << 22),
	CAN_TS_S			= (0x01ul << 23),	
	CAN_CCENS_S			= (0x01ul << 24),
	CAN_BTXPD_S			= (0x01ul << 25),
	CAN_STA_ALL_INT		= 0xff1e
}can_int_e; 


/******************************************************************************
* ISSR, SIER, SIDR, SIMR: CAN Source Interrupt/Status Register
******************************************************************************/	
typedef enum{
	CAN_CH1_INT			= (0x01ul << 0),		//Iterrupt and Status		
	CAN_CH2_INT			= (0x01ul << 1),		
	CAN_CH3_INT			= (0x01ul << 2),
	CAN_CH4_INT			= (0x01ul << 3),
	CAN_CH5_INT			= (0x01ul << 4),
	CAN_CH6_INT			= (0x01ul << 5),			
	CAN_CH7_INT			= (0x01ul << 6),		
	CAN_CH8_INT			= (0x01ul << 7),
	CAN_CH9_INT			= (0x01ul << 8),
	CAN_CH10_INT		= (0x01ul << 9),
	CAN_CH11_INT		= (0x01ul << 10),
	CAN_CH12_INT		= (0x01ul << 11),
	CAN_CH13_INT		= (0x01ul << 12),
	CAN_CH14_INT		= (0x01ul << 13),		
	CAN_CH15_INT		= (0x01ul << 14),
	CAN_CH16_INT		= (0x01ul << 15),
	CAN_CH17_INT		= (0x01ul << 16),		
	CAN_CH18_INT		= (0x01ul << 17),		
	CAN_CH19_INT		= (0x01ul << 18),		
	CAN_CH20_INT		= (0x01ul << 19),		
	CAN_CH21_INT		= (0x01ul << 20),
	CAN_CH22_INT		= (0x01ul << 21),
	CAN_CH23_INT		= (0x01ul << 22),
	CAN_CH24_INT		= (0x01ul << 23),	
	CAN_CH25_INT		= (0x01ul << 24),
	CAN_CH26_INT		= (0x01ul << 25),
	CAN_CH27_INT		= (0x01ul << 26),
	CAN_CH28_INT		= (0x01ul << 27),
	CAN_CH29_INT		= (0x01ul << 28),
	CAN_CH30_INT		= (0x01ul << 29),	
	CAN_CH31_INT		= (0x01ul << 30),
	CAN_CH32_INT		= (0x01ul << 31),
	CAN_CHNL_ALL_INT	= 0xffffffff					
}can_sint_e; 

#define	CAN_CHNL_POS(num)	(num)	
#define	CAN_CHNL_INT(num)	(0x01 << CAN_CHNL_POS(num))

/******************************************************************************
* HPIR: CAN Highest Priority Interrupt Register
******************************************************************************/	
#define	CAN_INTID_POS		(0)	
#define	CAN_INTID_MSK		(0xFFFF << CAN_INTID_POS)

/******************************************************************************
* ERCR: CAN Error Counter Register
******************************************************************************/	
#define	CAN_REC_POS			(0)	
#define	CAN_REC_MSK			(0x7Ful << CAN_REC_POS)

#define	CAN_REP_POS			(7)	
#define	CAN_REP_MSK			(0x01ul << CAN_REP_POS)

#define	CAN_TEC_POS			(8)	
#define	CAN_TEC_MSK			(0xFFul << CAN_TEC_POS)

/******************************************************************************
* TMR: CAN Interface X Transfer Management Register
******************************************************************************/	
#define	CAN_NUMBER_POS		(0)	
#define	CAN_NUMBER_MSK		(0x3Ful << CAN_NUMBER_POS)
#define	CAN_NUMBER(val)		(((val) & 0x3f) << CAN_NUMBER_POS)

#define	CAN_WR_POS			(7)	
#define	CAN_WR_MSK			(0x01ul << CAN_WR_POS)
typedef enum{
	CAN_WR_READ			= 0,
	CAN_WR_WRITE	
}can_wr_e;

#define	CAN_ADAR_POS		(8)	
#define	CAN_ADAR_MSK		(0x01ul << CAN_ADAR_POS)
typedef enum{
	CAN_ADAR_CONST		= 0,
	CAN_ADAR_TRAN	
}can_adar_e;

#define	CAN_ADBR_POS		(9)	
#define	CAN_ADBR_MSK		(0x01ul << CAN_ADBR_POS)
typedef enum{
	CAN_ADBR_CONST		= 0,
	CAN_ADBR_TRAN	
}can_adbr_e;

#define	CAN_AMSKR_POS		(10)	
#define	CAN_AMSKR_MSK		(0x01ul << CAN_AMSKR_POS)
typedef enum{
	CAN_AMSKR_CONST		= 0,
	CAN_AMSKR_TRAN	
}can_amskr_e;

#define	CAN_AIR_POS			(11)	
#define	CAN_AIR_MSK			(0x01ul << CAN_AIR_POS)
typedef enum{
	CAN_AIR_CONST		= 0,
	CAN_AIR_TRAN	
}can_air_e;

#define	CAN_AMCR_POS			(12)	
#define	CAN_AMCR_MSK			(0x01ul << CAN_AMCR_POS)
typedef enum{
	CAN_AMCR_CONST		= 0,
	CAN_AMCR_TRAN	
}can_amcr_e;

#define	CAN_TRND_POS		(14)	
#define	CAN_TRND_MSK		(0x01ul << CAN_TRND_POS)
typedef enum{
	CAN_TRND_CONST		= 0,
	CAN_TRND_SET_CLR	
}can_trnd_e;

#define	CAN_CLRIT_POS		(15)	
#define	CAN_CLRIT_MSK		(0x01ul << CAN_CLRIT_POS)
typedef enum{
	CAN_CLRIT_CONST		= 0,
	CAN_CLRIT_CLR	
}can_clrit_e;

/******************************************************************************
* DAR, DBR: CAN Interface X Data A Register Register
******************************************************************************/	
#define	CAN_DR_DATA_POS(n)	(n*8)	
#define	CAN_DR_DATA_MSK(n)	(0xFFul << CAN_DR_DATA_POS(n))
#define	CAN_DR_DATA(val, n)	(((val) & 0xFF) << CAN_DR_DATA_POS(n))

/******************************************************************************
* MSKR: CAN Interface X Mask Register 
******************************************************************************/	
#define	CAN_EXTMASK_POS		(0)	
#define	CAN_EXTMASK_MSK		(0x3FFFFul << CAN_EXTMASK_POS)
#define	CAN_EXTMASK(val)	(((val) & 0x3FFFF) << CAN_EXTMASK_POS)

#define	CAN_BASEMASK_POS	(18)	
#define	CAN_BASEMASK_MSK	(0x7FFul << CAN_BASEMASK_POS)
#define	CAN_BASEMASK(val)	(((val) & 0x7FF) << CAN_BASEMASK_POS)

#define	CAN_MMDIR_POS		(30)	
#define	CAN_MMDIR_MSK		(0x01ul << CAN_MMDIR_POS)
typedef enum{
	CAN_MMDIR_DIS	= 0,
	CAN_MMDIR_EN	
}can_mmdir_e;

#define	CAN_MXTD_POS		(31)	
#define	CAN_MXTD_MSK		(0x01ul << CAN_MXTD_POS)
typedef enum{
	CAN_MXTD_DIS		= 0,
	CAN_MXTD_EN	
}can_mxtd_e;

/******************************************************************************
* IR: CAN Interface X Identifier Register 
******************************************************************************/	
#define	CAN_EXTID_POS		(0)	
#define	CAN_EXTID_MSK		(0x3FFFFul << CAN_EXTID_POS)
#define	CAN_EXTID(val)		(((val) & 0x3FFFF) << CAN_EXTID_POS)

#define	CAN_BASEID_POS		(18)	
#define	CAN_BASEID_MSK		(0x7FFul << CAN_BASEID_POS)
#define	CAN_BASEID(val)		(((val) & 0x7FF) << CAN_BASEID_POS)

#define	CAN_MDIR_POS		(29)	
#define	CAN_MDIR_MSK		(0x01ul << CAN_MDIR_POS)
typedef enum{
	CAN_MDIR_RECV	= 0,
	CAN_MDIR_SEND	
}can_mdir_e;

#define	CAN_XTD_POS			(30)	
#define	CAN_XTD_MSK			(0x01ul << CAN_XTD_POS)
typedef enum{
	CAN_XTD_BIT_11	= 0,
	CAN_XTD_BIT_29	
}can_xtd_e;

#define	CAN_MSGVAL_POS		(31)	
#define	CAN_MSGVAL_MSK		(0x01ul << CAN_MSGVAL_POS)
typedef enum{
	CAN_MSGVAL_INVALID	= 0,
	CAN_MSGVAL_VALID	
}can_msgval_e;

/******************************************************************************
* MCR: CAN nterface X Message Control Register 
******************************************************************************/	
#define	CAN_DLC_POS			(0)	
#define	CAN_DLC_MSK			(0x0Ful << CAN_DLC_POS)
#define	CAN_DLC(val)		(((val) & 0x0F) << CAN_DLC_POS)

#define	CAN_OVERWR_POS		(7)	
#define	CAN_OVERWR_MSK		(0x01ul << CAN_OVERWR_POS)
typedef enum{
	CAN_OVERWR_NORMAL	= 0,
	CAN_OVERWR_OVER	
}can_overwrite_e;

#define	CAN_TXREQST_POS		(8)	
#define	CAN_TXREQST_MSK		(0x01ul << CAN_TXREQST_POS)
typedef enum{
	CAN_TXREQST_DIS	= 0,
	CAN_TXREQST_EN	
}can_txreqst_e;

#define	CAN_RMTEN_POS		(9)	
#define	CAN_RMTEN_MSK		(0x01ul << CAN_RMTEN_POS)
typedef enum{
	CAN_RMTEN_CONST	= 0,
	CAN_RMTEN_SET	
}can_rmten_e;

#define	CAN_RXIE_POS		(10)	
#define	CAN_RXIE_MSK		(0x01ul << CAN_RXIE_POS)
typedef enum{
	CAN_RXIE_CONST	= 0,
	CAN_RXIE_SET	
}can_rxie_e;

#define	CAN_TXIE_POS		(11)	
#define	CAN_TXIE_MSK		(0x01ul << CAN_TXIE_POS)
typedef enum{
	CAN_TXIE_CONST	= 0,
	CAN_TXIE_SET	
}can_txie_e;

#define	CAN_UMASK_POS		(12)	
#define	CAN_UMASK_MSK		(0x01ul << CAN_UMASK_POS)
typedef enum{
	CAN_UMASK_NO	= 0,
	CAN_UMASK_USE	
}can_unmask_e;

#define	CAN_ITPND_POS		(13)	
#define	CAN_ITPND_MSK		(0x01ul << CAN_ITPND_POS)
typedef enum{
	CAN_ITPND_NONE	= 0,
	CAN_ITPND_OCCUR	
}can_itpnd_e;

#define	CAN_MSGLST_POS		(14)	
#define	CAN_MSGLST_MSK		(0x01ul << CAN_MSGLST_POS)
typedef enum{
	CAN_MSGLST_NONE	= 0,
	CAN_MSGLST_OCCUR	
}can_msglst_e;

#define	CAN_NEWDAT_POS		(15)	
#define	CAN_NEWDAT_MSK		(0x01ul << CAN_NEWDAT_POS)
typedef enum{
	CAN_NEWDAT_NONE	= 0,
	CAN_NEWDAT_OCCUR	
}can_newdat_e;

/******************************************************************************
* MCR: CAN Interface X Stamp Register
******************************************************************************/	
#define	CAN_STAMP_POS		(0)	
#define	CAN_STAMP_MSK		(0xFFFFFFFFul << CAN_STAMP_POS)
//#define	CAN_STAMP(val)		(((val) & 0xFFFFFFFF) << CAN_STAMP_POS)

/******************************************************************************
* TRR: CAN Transmission Request  Register
******************************************************************************/	
#define	CAN_TRR_POS(ch)		(ch)	
#define	CAN_TRR_MSK(ch)		(0x01ul << CAN_TRR_POS(ch))

/******************************************************************************
* NDR: CAN New Data Register
******************************************************************************/	
#define	CAN_NDR_POS(ch)		(ch)	
#define	CAN_NDR_MSK(ch)		(0x01ul << CAN_NDR_POS(ch))

/******************************************************************************
* MVR: CAN Message Valid Register
******************************************************************************/	
#define	CAN_MVR_POS(ch)		(ch)	
#define	CAN_MVR_MSK(ch)		(0x01ul << CAN_MVR_POS(ch))

/******************************************************************************
* TSTR: CAN Test Register
******************************************************************************/	
#define	CAN_BASIC_POS		(0)	
#define	CAN_BASIC_MSK		(0x01ul << CAN_BASIC_POS)

#define	CAN_SILENT_POS		(1)	
#define	CAN_SILENT_MSK		(0x01ul << CAN_SILENT_POS)

#define	CAN_LBACK_POS		(2)	
#define	CAN_LBACK_MSK		(0x01ul << CAN_LBACK_POS)

typedef enum{
	CAN_MODE_BASIC		= 0,
	CAN_MODE_SILENT,
	CAN_MODE_LBACK
}can_test_mode_e;

#define	CAN_TXPIN_POS		(3)	
#define	CAN_TXPIN_MSK		(0x03ul << CAN_TXPIN_POS)
typedef enum{
	CAN_TXPIN_CORE		= 0,
	CAN_TXPIN_SMP,
	CAN_TXPIN_DOMIN,
	CAN_TXPIN_RECES
}can_txpin_e;

#define	CAN_TXOPD_POS		(5)	
#define	CAN_TXOPD_MSK		(0x01ul << CAN_TXOPD_POS)

#define	CAN_RXPIN_POS		(6)	
#define	CAN_RXPIN_MSK		(0x01ul << CAN_RXPIN_POS)

#define	CAN_TSTKEY_POS		(16)	
#define	CAN_TSTKEY_MSK		(0xFFFFul << CAN_TSTKEY_POS)
#define	CAN_TSTKEY			0x0C75

/******************************************************************************
********************* CAN inline Functions Declaration **********************
******************************************************************************/
static inline void csp_can_clk_en(csp_can_t *ptCanBase)
{
	ptCanBase->ECR = CAN_CLKEN_MSK;
}
static inline void csp_can_dbg_en(csp_can_t *ptCanBase)
{
	ptCanBase->ECR = CAN_DBGEN_MSK;
}
//
static inline void csp_can_cr_cmd(csp_can_t *ptCanBase, can_cr_e eCrCmd)
{
	ptCanBase->CR = eCrCmd;
}
static inline void csp_can_soft_rst(csp_can_t *ptCanBase)
{
	ptCanBase->CR = CAN_SWRST_MSK;
}
static inline void csp_can_en(csp_can_t *ptCanBase)
{
	ptCanBase->CR = CAN_CANEN_MSK;
}
static inline void csp_can_dis(csp_can_t *ptCanBase)
{
	ptCanBase->CR = CAN_CANDIS_MSK;
}

static inline void csp_can_ccen(csp_can_t *ptCanBase)
{
	ptCanBase->CR = CAN_CCEN_MSK;
}
static inline void csp_can_ccdis(csp_can_t *ptCanBase)
{
	ptCanBase->CR = CAN_CCDIS_MSK;
}
static inline void csp_can_set_mode(csp_can_t *ptCanBase, uint16_t hwBrDiv, can_cssel_e eClkSrc, can_ar_e eAr, uint8_t bySjw, uint8_t byPhseg1,uint8_t byPhseg2)
{
	ptCanBase->MR = CAN_BD(hwBrDiv) | (eClkSrc << CAN_CSSEL_POS) | (eAr << CAN_AR_POS) | CAN_SJW(bySjw) | CAN_PHSEG1(byPhseg1) | CAN_PHSEG2(byPhseg2) ;
}
static inline void csp_can_set_ar(csp_can_t *ptCanBase, can_ar_e eAr)
{
	ptCanBase->MR = (ptCanBase->MR & ~(CAN_AR_MSK)) | (eAr << CAN_AR_POS);
}
//
static inline void csp_can_int_enable(csp_can_t *ptCanBase, can_int_e eCanInt, bool bEnable)
{
	if(bEnable)
		ptCanBase->IER = eCanInt;
	else
		ptCanBase->IDR = eCanInt;
}
static inline void csp_can_clr_isr(csp_can_t *ptCanBase, can_int_e eCanInt)
{
	ptCanBase->CSR = eCanInt;
}
static inline uint16_t csp_can_get_isr(csp_can_t *ptCanBase)
{
	return (uint16_t)(ptCanBase->SR & 0xffff);
}
static inline uint32_t csp_can_get_imr(csp_can_t *ptCanBase)
{
	return (uint16_t)(ptCanBase->IMR & 0xffff);;
}
static inline uint32_t csp_can_get_sr(csp_can_t *ptCanBase)
{
	return (uint32_t)(ptCanBase->SR);
}
//
static inline void csp_can_sint_enable(csp_can_t *ptCanBase, can_sint_e eCanSrcInt, bool bEnable)
{
	if(bEnable)
		ptCanBase->SIER = eCanSrcInt;
	else
		ptCanBase->SIDR = eCanSrcInt;
}
static inline uint32_t csp_can_get_sisr(csp_can_t *ptCanBase)
{
	return (uint32_t)(ptCanBase->ISSR);
}
static inline uint32_t csp_can_get_simr(csp_can_t *ptCanBase)
{
	return (uint32_t)(ptCanBase->SIMR);
}
//
static inline uint16_t csp_can_get_hpir(csp_can_t *ptCanBase)
{
	return (uint16_t)(ptCanBase->HPIR & 0xffff);
}
static inline uint8_t csp_can_get_rec(csp_can_t *ptCanBase)
{
	return (uint8_t)(ptCanBase->ERCR & CAN_REC_MSK);
}
static inline uint8_t csp_can_get_tec(csp_can_t *ptCanBase)
{
	return (uint8_t)((ptCanBase->ERCR & CAN_TEC_MSK) >> CAN_TEC_POS);
}
static inline uint8_t csp_can_get_rep(csp_can_t *ptCanBase)
{
	return (uint8_t)((ptCanBase->ERCR & CAN_REP_MSK) >> CAN_REP_POS);
}
//
static inline void csp_can_tmr_wr(csp_can_t *ptCanBase, uint8_t byNum, can_adbr_e eAdat, can_adbr_e eBdat, can_amskr_e eAmsk, can_air_e eAir, can_amcr_e eAmcr, can_trnd_e eTrnd)
{
	ptCanBase->IF[0].TMR = (CAN_WR_WRITE << CAN_WR_POS) | (eAdat << CAN_ADAR_POS) | (eBdat << CAN_ADBR_POS) | (eAmsk << CAN_AMSKR_POS) | 
				(eAir << CAN_AIR_POS) | (eAmcr << CAN_AMCR_POS) | (eTrnd << CAN_TRND_POS) | CAN_NUMBER(byNum); 
} 
static inline void csp_can_tmr_rd(csp_can_t *ptCanBase, uint8_t byNum, can_adbr_e eAdat, can_adbr_e eBdat, can_amskr_e eAmsk, can_air_e eAir, can_amcr_e eAmcr, can_trnd_e eTrnd, can_clrit_e eClrIt)
{
	ptCanBase->IF[0].TMR = (CAN_WR_READ << CAN_WR_POS) | (eAdat << CAN_ADAR_POS) | (eBdat << CAN_ADBR_POS) | (eAmsk << CAN_AMSKR_POS) | 
				(eAir << CAN_AIR_POS) | (eAmcr << CAN_AMCR_POS) | (eTrnd << CAN_TRND_POS) | (eClrIt << CAN_CLRIT_POS) | CAN_NUMBER(byNum); 
} 
static inline void csp_can_set_tmr(csp_can_t *ptCanBase, uint8_t byNum, uint8_t byIfx, uint32_t wCmdVal)
{
	ptCanBase->IF[byIfx].TMR = CAN_NUMBER(byNum) | wCmdVal; 
} 
static inline void csp_can_set_dar(csp_can_t *ptCanBase, uint32_t wAdata)
{
	ptCanBase->IF[0].DAR = wAdata;
}
static inline void csp_can_set_dbr(csp_can_t *ptCanBase, uint32_t wBdata)
{
	ptCanBase->IF[0].DBR = wBdata;
}
static inline void csp_can_set_adata(csp_can_t *ptCanBase, uint8_t *pbyData)
{
	ptCanBase->IF[0].DAR = (uint32_t)(pbyData[0] | (pbyData[1] << 8) | (pbyData[2] << 16) | (pbyData[3] << 24));
}
static inline void csp_can_set_bdata(csp_can_t *ptCanBase, uint8_t *pbyData)
{
	ptCanBase->IF[0].DBR = (uint32_t)(pbyData[0] | (pbyData[1] << 8) | (pbyData[2] << 16) | (pbyData[3] << 24));
}

 
static inline void csp_can_mskr_config(csp_can_t *ptCanBase, uint32_t wExtMsk, uint16_t hwStdMsk, can_mmdir_e eMmdir, can_mxtd_e eMxtd)
{
	ptCanBase->IF[0].MASKR =  CAN_EXTMASK(wExtMsk) | CAN_BASEMASK(hwStdMsk) | (eMmdir << CAN_MMDIR_POS) | (eMxtd << CAN_MXTD_POS);
} 
static inline void csp_can_set_mskr(csp_can_t *ptCanBase, uint32_t wValue)
{
	ptCanBase->IF[0].MASKR = wValue;
}
static inline void csp_can_ir_config(csp_can_t *ptCanBase, uint32_t wExtId, uint16_t hwStdId, can_mdir_e eMdir, can_xtd_e eXtd, can_msgval_e eMsgVal)
{
	ptCanBase->IF[0].IR =  CAN_EXTID(wExtId) | CAN_BASEID(hwStdId) | (eMdir << CAN_MDIR_POS) | (eXtd << CAN_XTD_POS) | (eMsgVal << CAN_MSGVAL_POS); 
} 
static inline void csp_can_set_ir(csp_can_t *ptCanBase, uint32_t wValue)
{
	ptCanBase->IF[0].IR = wValue;
}
static inline void csp_can_set_mcr(csp_can_t *ptCanBase, uint32_t wValue)
{
	ptCanBase->IF[0].MCR = wValue;
}
//
static inline void csp_can_set_mcr_wr(csp_can_t *ptCanBase, uint8_t byDataLen, can_txreqst_e eTxReq, can_txie_e eTxIe, can_rmten_e eRem)
{
	ptCanBase->IF[0].MCR = (CAN_DLC(byDataLen)) | (eTxReq << CAN_TXREQST_POS) | (eTxIe << CAN_TXIE_POS) | (eRem << CAN_RMTEN_POS);
} 
static inline void csp_can_set_mcr_rd(csp_can_t *ptCanBase, uint8_t byDataLen, can_overwrite_e eOverWr, can_rxie_e eRxIe, can_unmask_e eMsk)
{
	ptCanBase->IF[0].MCR = CAN_DLC(byDataLen) | (eOverWr << CAN_OVERWR_POS) | (eRxIe << CAN_RXIE_POS) | (eMsk << CAN_UMASK_POS);
}
// 
//static inline uint32_t csp_can_get_mcr(csp_can_t *ptCanBase, uint8_t byIfx)
//{
//	return (ptCanBase->IF[byIfx].MCR);
//}
//static inline uint32_t csp_can_get_ir(csp_can_t *ptCanBase, uint8_t byIfx)
//{
//	return (ptCanBase->IF[byIfx].IR);
//}
//static inline uint32_t csp_can_get_mskr(csp_can_t *ptCanBase, uint8_t byIfx)
//{
//	return (ptCanBase->IF[byIfx].MASKR);
//}
//
//static inline uint32_t csp_can_get_dbr(csp_can_t *ptCanBase, uint8_t byIfx)
//{
//	return (ptCanBase->IF[byIfx].DBR);
//}
//static inline uint32_t csp_can_get_dar(csp_can_t *ptCanBase, uint8_t byIfx)
//{
//	return (ptCanBase->IF[byIfx].DAR);
//}
//
//static inline uint32_t csp_can_get_stpr(csp_can_t *ptCanBase, uint8_t byIfx)
//{
//	return (uint32_t)((ptCanBase->IF[byIfx].STPR));
//}
static inline uint32_t csp_can_get_mcr(csp_can_t *ptCanBase)
{
	return (ptCanBase->IF[1].MCR);
}
static inline uint32_t csp_can_get_ir(csp_can_t *ptCanBase)
{
	return (ptCanBase->IF[1].IR);
}
static inline uint32_t csp_can_get_mskr(csp_can_t *ptCanBase)
{
	return (ptCanBase->IF[1].MASKR);
}

static inline uint32_t csp_can_get_dbr(csp_can_t *ptCanBase)
{
	return (ptCanBase->IF[1].DBR);
}
static inline uint32_t csp_can_get_dar(csp_can_t *ptCanBase)
{
	return (ptCanBase->IF[1].DAR);
}
static inline uint32_t csp_can_get_stpr(csp_can_t *ptCanBase)
{
	return (uint32_t)((ptCanBase->IF[1].STPR));
}
//
static inline uint32_t csp_can_get_trr(csp_can_t *ptCanBase)
{
	return (uint32_t)(ptCanBase->TRR);
}
static inline uint32_t csp_can_get_ndr(csp_can_t *ptCanBase)
{
	return (uint32_t)(ptCanBase->NDR);
}
static inline uint32_t csp_can_get_mvr(csp_can_t *ptCanBase)
{
	return (uint32_t)(ptCanBase->MVR);
}
//
static inline void csp_can_test_mode(csp_can_t *ptCanBase, can_test_mode_e eTestMode)
{
	ptCanBase->TSTR |= ((CAN_TSTKEY << CAN_TSTKEY_POS) | (0x01 << eTestMode));
}

static inline void csp_can_set_tstr(csp_can_t *ptCanBase, uint8_t byValue)
{
	ptCanBase->TSTR |= ((CAN_TSTKEY << CAN_TSTKEY_POS) | byValue);
}


#endif




