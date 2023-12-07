/***********************************************************************//** 
 * \file  csp_adc.h
 * \brief  ADC description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-14 <td>V0.0  <td>ZJY   <td>initial
 * <tr><td> 2021-1-8  <td>V0.1  <td>WNN   <td>modify(enum pattern, add)
 * <tr><td> 2023-9-19 <td>V0.2  <td>XS    <td>fix bug,code normalization
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CSP_ADC_H
#define _CSP_ADC_H

/* Includes ------------------------------------------------------------------*/
#include "soc.h"

/// \struct csp_adc_t
/// \brief ADC Reg Description
 typedef struct
 {
    __OM   uint32_t  ECR;		//0x0000 Clock Enable Register             
    __OM   uint32_t  DCR;       //0x0004 Clock Disable Register            
    __IM   uint32_t  PMSR;      //0x0008 Power Management Status Register  
    __IM   uint32_t  RSVD0;
    __IOM  uint32_t  CR;        //0x0010 Control Register                  
    __IOM  uint32_t  MR;        //0x0014 Mode Register                     
    __IOM  uint32_t  SHR;		//0x0018 Sample Hold Period Register
    __OM   uint32_t  CSR;       //0x001C Clear Status Register             
    __IM   uint32_t  SR;        //0x0020 Status Register                   
    __IOM  uint32_t  IMCR;      //0x0024 Interrupt Enable Register         
    __OM   uint32_t  RSVD1;     //0x0028 Interrupt Disable Register        
    __IM   uint32_t  MISR;      //0x002C Interrupt Enable Status Register           
    __IOM  uint32_t  SEQ[16];   //0x0030 Conversion Mode Register 0~11     
    __IOM  uint32_t  PRI;       //0x0070 Conversion Priority Register      
    __IOM  uint32_t  TDL0;      //0x0074 Trigger Delay control Register    
    __IOM  uint32_t  TDL1;      //0x0078 Trigger Delay control Register    
    __IOM  uint32_t  SYNCR;     //0x007C Sync Control Register             
    __IM   uint32_t  RSVD2[2];         
    __IOM  uint32_t  EVTRG;     //0x0088 Event Trigger Control  Register   
    __IOM  uint32_t  EVPS;      //0x008C Event Prescale Register           
    __IOM  uint32_t  EVSWF;     //0x0090 Event Softtrig Register           
    __IOM  uint32_t  RSVD3[27];
    __IM   uint32_t  DR[16];    //0x0100 Convert Data Register             
    __IOM  uint32_t  CMP0;      //0x0140 Comparison Data Register          
    __IOM  uint32_t  CMP1;      //0x0144 Comparison Data Register          
	__IOM  uint32_t  DRMASK;	//0x0148 Disable Conversion Data Updata Register
 } csp_adc_t;
 
/******************************************************************************
* TS : Temperature sensor Registers
******************************************************************************/
#define CALIBRATION_TS1  *((uint32_t *)(0x00080100))
#define CALIBRATION_IR1  *((uint32_t *)(0x00080104))
#define CALIBRATION_TS2  *((uint32_t *)(0x00080108))
#define CALIBRATION_IR2  *((uint32_t *)(0x0008010c))

/******************************************************************************
* ECR, DCR, PMSR : ADC Power Management Registers
******************************************************************************/
#define ADC_CLKEN			(0x01ul << 1) 
#define ADC_IPIDCODE_MSK  	(0x3FFFFFFul << 4)
#define ADC_CLKSEL_POS		(16)       			
#define ADC_CLKSEL_MSK		(0X3ul << 16)       
#define ADC_DBGEN          	(0x01ul << 31)

typedef enum
{
	ADC_HFOSC		= (0),            
	ADC_PLLQ       	= (1),            
	ADC_PCLK   		= (2),            
}adc_clksel_e;
/******************************************************************************
* CR : ADC Control Register
******************************************************************************/
#define ADC_SWRST			(0x1 << 0)            
#define ADC_ADCEN     		(0x1 << 1)            
#define ADC_ADCDIS   		(0x1 << 2)            
#define ADC_START     		(0x1 << 3)           
#define ADC_STOP      		(0x1 << 4)  
#define ADC_SWTRG     		(0x1 << 5)

#define ADC_VREF_POS 		(6) 
#define ADC_VREF_MSK 		(0x03ul << ADC_VREF_POS) 
typedef enum{
	ADC_VERF_VDD_VSS 		= 	(0x00ul),
	ADC_VREF_VREFP_VSS		= 	(0x02ul),
	ADC_VREF_INTVREF_VSS	= 	(0x03ul)
}adc_vref_e;

#define ADC_BIT_POS 		(31) 
#define ADC_BIT_MSK 		(0x01ul << ADC_BIT_POS) 
typedef enum{
	ADC_12BIT			= 	(0x00ul),
	ADC_10BIT			= 	(0x01ul)
}adc_resolution_e;

/******************************************************************************
* MR : ADC Mode Register
******************************************************************************/
#define ADC_PRLVAL_POS      (0) 
#define ADC_PRLVAL_MASK     (0x1Ful << ADC_PRLVAL_POS)  
#define ADC_NBRCH_POS       (10)      			    
#define ADC_NBRCH_MSK       (0xFul << ADC_NBRCH_POS) 
#define ADC_NBRCH(ch)       (((ch-1) & 0xFul) << ADC_NBRCH_POS) 
#define ADC_NBRCMP0_POS		(16)
#define ADC_NBRCMP0_MASK	(0xFul << ADC_NBRCMP0_POS)
#define ADC_NBRCMP1_POS		(22)
#define ADC_NBRCMP1_MASK	(0xFul << ADC_NBRCMP1_POS)
#define ADC_NBRCMP0(val)    (((val) & 0xFul) << ADC_NBRCMP0_POS) 
#define ADC_NBRCMP1(val)    (((val) & 0xFul) << ADC_NBRCMP1_POS) 
 
#define ADC_CMPOS_POS		(29)
#define ADC_CMPOS_MSK		(0x01ul << ADC_CMPOS_POS)
typedef enum{
	ADC_CMP_ALWAYS = 0,
	ADC_CMP_ACROSS
}adc_cmpos_e;

#define ADC_CV_POS 			(30) 
#define ADC_CV_MSK 			(0x03ul << ADC_CV_POS) 
typedef enum{
	ADC_RUN_ONESHOT			=   (0x00ul),
	ADC_RUN_CONTINUE		= 	(0x01ul),
	ADC_RUN_WAITSYNC		= 	(0x02ul),
}adc_runmode_e;

/******************************************************************************
* SHR : ADC Sample hode Registers 
******************************************************************************/
#define ADC_SHR_POS	 		(0)
#define ADC_SHR_MSK	 		(0xFFul << ADC_SHR_POS)

/******************************************************************************
* CSR, SR, IMCR, MISR : ADC Status Registers and Interrupt Registers
******************************************************************************/
#define ADC_SEQ_POS			(16)
#define ADC_SEQ_MSK 		(0xffff << ADC_SEQ_POS)
#define ADC_SEQ(val)   		(0x01ul << (val + ADC_SEQ_POS))    

#define ADC_SEQ_IDX_POS		(10)
#define ADC_SEQ_IDX_MSK 	(0x0F << ADC_SEQ_IDX_POS)

typedef enum{
	ADC_EOC				= 	(0x01uL << 0),
	ADC_READY			= 	(0x01uL << 1),
	ADC_OVR				= 	(0x01uL << 2),
	ADC_CMP0H			= 	(0x01uL << 4),
	ADC_CMP0L			= 	(0x01uL << 5),
	ADC_CMP1H			= 	(0x01uL << 6),
	ADC_CMP1L			= 	(0x01uL << 7),
	ADC_ADCENS    		= 	(0x01uL << 8),  		
	ADC_CTCVS     		= 	(0x01uL << 9), 				
	ADC_SEQ0			= 	(0x01uL << 16),			
	ADC_SEQ1  			= 	(0x01uL << 17),     
	ADC_SEQ2  			= 	(0x01uL << 18),     
	ADC_SEQ3  			= 	(0x01uL << 19),     
	ADC_SEQ4  			= 	(0x01uL << 20),     
	ADC_SEQ5  			= 	(0x01uL << 21),     
	ADC_SEQ6 	 		= 	(0x01uL << 22),     
	ADC_SEQ7  			= 	(0x01uL << 23),     
	ADC_SEQ8  			= 	(0x01uL << 24),     
	ADC_SEQ9  			= 	(0x01uL << 25),     
	ADC_SEQ10  			= 	(0x01uL << 26),     
	ADC_SEQ11   		= 	(0x01uL << 27),     
	ADC_SEQ12   		= 	(0x01uL << 28),     
	ADC_SEQ13   		= 	(0x01uL << 29),     
	ADC_SEQ14   		= 	(0x01uL << 30),    
	ADC_SEQ15   		= 	(0x01uL << 31),
	ADC_SR_ALL//
}adc_sr_e;

typedef enum{
	ADC_INT_EOC			= 	(0x01uL << 0),
	ADC_INT_READY		= 	(0x01uL << 1),
	ADC_INT_OVR			= 	(0x01uL << 2),
	ADC_INT_CMP0H		= 	(0x01uL << 4),
	ADC_INT_CMP0L		= 	(0x01uL << 5),
	ADC_INT_CMP1H		= 	(0x01uL << 6),
	ADC_INT_CMP1L		= 	(0x01uL << 7),	
	ADC_INT_SEQ0		= 	(0x01uL << 16),			
	ADC_INT_SEQ1  		= 	(0x01uL << 17),     
	ADC_INT_SEQ2  		= 	(0x01uL << 18),     
	ADC_INT_SEQ3  		= 	(0x01uL << 19),     
	ADC_INT_SEQ4  		= 	(0x01uL << 20),     
	ADC_INT_SEQ5  		= 	(0x01uL << 21),     
	ADC_INT_SEQ6 	 	= 	(0x01uL << 22),     
	ADC_INT_SEQ7  		= 	(0x01uL << 23),     
	ADC_INT_SEQ8  		= 	(0x01uL << 24),     
	ADC_INT_SEQ9  		= 	(0x01uL << 25),     
	ADC_INT_SEQ10  		= 	(0x01uL << 26),     
	ADC_INT_SEQ11   	= 	(0x01uL << 27),     
	ADC_INT_SEQ12   	= 	(0x01uL << 28),     
	ADC_INT_SEQ13   	= 	(0x01uL << 29),     
	ADC_INT_SEQ14   	= 	(0x01uL << 30),     
	ADC_INT_SEQ15   	= 	(0x01uL << 31),
	ADC_INT_ALL			=	(0XFFFFFFFF<<0)
}adc_int_e;

/******************************************************************************
* SEQx : ADC Conversion Mode Register
******************************************************************************/
#define ADC_AIN_POS		(0)
#define ADC_AIN_MSK	   	(0x7Ful << ADC_AIN_POS)
typedef enum{
	ADC_ADCINA0		= 0ul,
	ADC_ADCINA1,
	ADC_ADCINA2,
	ADC_ADCINA3,
	ADC_ADCINA4,
	ADC_ADCINA5,
	ADC_ADCINA6,
	ADC_ADCINA7,
	ADC_ADCINA8,
	ADC_ADCINA9,
	ADC_ADCINA10,
	ADC_ADCINA11,
	ADC_ADCINA12,
	ADC_ADCINA13,
	ADC_ADCINA14,
	ADC_ADCINA15,
	ADC_ADCINA16,
	ADC_ADCINA17,
	ADC_ADCINA18,
	ADC_ADCINA19,
	ADC_ADCINA20,
	ADC_ADCINA21,
	ADC_ADCINA22,
	ADC_ADCINA23,
	ADC_ADCINB0		= 0x40ul,
	ADC_ADCINB1,
	ADC_ADCINB2,
	ADC_ADCINB3,
	ADC_ADCINB4,
	ADC_ADCINB5,
	ADC_ADCINB6,
	ADC_ADCINB7,
	ADC_ADCINB8,
	ADC_ADCINB9,
	ADC_ADCINB10,
	ADC_ADCINB11,
	ADC_ADCINB12,
	ADC_ADCINB13,
	ADC_ADCINB14,
	ADC_ADCINB15,
	ADC_ADCINB16,
	ADC_ADCINB17,
	ADC_ADCINB18,
	ADC_ADCINB19,
	ADC_ADCINB20,
	ADC_ADCINB21,
	ADC_ADCINB22,
	ADC_ADCIN_OPA0X     	= 0x79ul,
	ADC_ADCIN_OPA1X,
	ADC_ADCIN_OPA2X,
	ADC_ADCIN_OPA3X,
	ADC_ADCIN_INTVREF,
	ADC_ADCIN_1_5VDD,
	ADC_ADCIN_VSS
}adc_channel_e;

#define ADC_CVCNT_P0S	(8)
#define ADC_CVCNT_MSK 	(0x0ful << ADC_CVCNT_P0S)
typedef enum{
	ADC_CVCNT_1			= (0x00ul ),
	ADC_CVCNT_2			= (0x01ul ),
	ADC_CVCNT_4			= (0x02ul ),
	ADC_CVCNT_8			= (0x03ul ),
	ADC_CVCNT_16		= (0x04ul ),
	ADC_CVCNT_32		= (0x05ul ),
	ADC_CVCNT_64		= (0x06ul ),
	ADC_CVCNT_128		= (0x07ul ),
	ADC_CVCNT_256		= (0x08ul ),
	ADC_CVCNT_512		= (0x09ul )
}adc_cnt_e;

#define ADC_AVGEN_POS 	(12)
#define ADC_AVGEN_MSK   (0x01ul << ADC_AVGEN_POS)  

#define ADC_AVGSEL_POS	(13)
#define ADC_AVGSEL_MSK	(0x0ful << ADC_AVGSEL_POS)
typedef enum{
	ADC_AVGSEL_1		= (0x00ul ),
	ADC_AVGSEL_2		= (0x01ul ),
	ADC_AVGSEL_4		= (0x02ul ),
	ADC_AVGSEL_8		= (0x03ul ),
	ADC_AVGSEL_16		= (0x04ul ),
	ADC_AVGSEL_32		= (0x05ul ),
	ADC_AVGSEL_64		= (0x06ul ),
	ADC_AVGSEL_128		= (0x07ul ),
	ADC_AVGSEL_256		= (0x08ul ),
	ADC_AVGSEL_512		= (0x09ul )
}adc_avg_e;

#define ADC_SYNCSRC_POS		(17)
#define ADC_SYNCSRC_MSK		(0x07ul << ADC_SYNCSRC_POS)
typedef enum{
	ADC_SYNC_NONE		= 0,
	ADC_SYNC_SWTGR		,
	ADC_SYNC_SRC0		,
	ADC_SYNC_SRC1		,
	ADC_SYNC_SRC2		,
	ADC_SYNC_SRC3		,
	ADC_SYNC_SRC4		,
	ADC_SYNC_SRC5		
}adc_sync_source_e;

#define ADC_DRMASKEN_POS	(20)
#define ADC_DRMASKEN_MSK	(0x0Ful << ADC_DRMASKEN_POS)
#define ADC_DRMASKOFF_POS	(24)
#define ADC_DRMASKOFF_MSK	(0x0Ful << ADC_DRMASKOFF_POS)

/******************************************************************************
* DR : ADC Convert Data Register
******************************************************************************/
#define ADC_DATA_POS      	(0)    
#define ADC_DATA_MASK     	(0xFFFul << ADC_DATA_POS)   

/******************************************************************************
* TDLx : ADC Trigger Delay Control Register
******************************************************************************/
#define ADC_TDL_SOC0_3_MSK	(0xff)
#define ADC_TDL_SOC1_4_MSK	(0xff00)
#define ADC_TDL_SOC2_5_MSK	(0xff0000)
#define ADC_SOC0_TDL(val)   (((val) & 0xff) << 0)
#define ADC_SOC1_TDL(val)   (((val) & 0xff) << 8)
#define ADC_SOC2_TDL(val)   (((val) & 0xff) << 16)
#define ADC_SOC3_TDL(val)   (((val) & 0xff) << 0)
#define ADC_SOC4_TDL(val)   (((val) & 0xff) << 8)
#define ADC_SOC5_TDL(val)   (((val) & 0xff) << 16)

/******************************************************************************
* SYNCR
******************************************************************************/
#define ADC_SYNC_POS(n)		(0 + n)
#define ADC_SYNC_MSK(n)		(0x01ul << ADC_SYNC_POS(n))
typedef enum{
	ADC_SYNCEN0			= 0,
	ADC_SYNCEN1,		
	ADC_SYNCEN2,	
	ADC_SYNCEN3,	
	ADC_SYNCEN4,		
	ADC_SYNCEN5,
}adc_sync_in_e;

#define ADC_OSTMD_POS(n)	(8 + n)
#define ADC_OSTMD_MSK(n)	(0x01ul << ADC_OSTMD_POS(n))
typedef enum{
	ADC_OSTMD_CONTINUOUS = 0,
    ADC_OSTMD_ONCE   				
}adc_ostmd_e;

#define ADC_REARM_POS(n)	(16 + n)
#define ADC_REARM_MSK(n) 	(0x01 << ADC_REARM_POS(n))
#define ADC_REARM_EN		(0x01ul)
/******************************************************************************
* EVTRG 
******************************************************************************/
typedef enum{
	ADC_TRGSRC_NONE			= 0,
	ADC_TRGSRC_EOC,		
	ADC_TRGSRC_READY,	
	ADC_TRGSRC_OVR,	
	ADC_TRGSRC_CMP0H,		
	ADC_TRGSRC_CMP0L,		
	ADC_TRGSRC_CMP1H,		
	ADC_TRGSRC_CMP1L,	
	ADC_TRGSRC_SEQEND0,	
	ADC_TRGSRC_SEQEND1,
	ADC_TRGSRC_SEQEND2,	
	ADC_TRGSRC_SEQEND3,
	ADC_TRGSRC_SEQEND4,	
	ADC_TRGSRC_SEQEND5,
	ADC_TRGSRC_SEQEND6,	
	ADC_TRGSRC_SEQEND7,
	ADC_TRGSRC_SEQEND8,	
	ADC_TRGSRC_SEQEND9,
	ADC_TRGSRC_SEQEND10,	
	ADC_TRGSRC_SEQEND11,
	ADC_TRGSRC_SEQEND12,	
	ADC_TRGSRC_SEQEND13,
	ADC_TRGSRC_SEQEND14,	
	ADC_TRGSRC_SEQEND15
}adc_evtrg_src_e;

typedef enum{
	ADC_TRG0_SEL			= 0,
	ADC_TRG1_SEL		
}adc_evtrg_out_e;

#define ADC_TRGSRC0_POS			(0)
#define ADC_TRGSRC0_MSK			(0x1Ful << ADC_TRGSRC0_POS)
#define ADC_TRGSRC1_POS			(8)
#define ADC_TRGSRC1_MSK			(0x1Ful << ADC_TRGSRC1_POS)
#define ADC_TRGSRC_POS(n)		(8 * (n))
#define ADC_TRGSRC_MSK(n)		(0x1Ful << ADC_TRGSRC_POS(n))

#define ADC_EV_SEQ_END(val)  	(0x8 + val)
#define ADC_TRGSRC0(val)		(((val) & 0x1Ful) << 0) 
#define ADC_TRGSRC1(val)  		(((val) & 0x1Ful) << 8)

#define ADC_TRG0OE       		(0x01ul <<20)
#define ADC_TRG1OE       		(0x01ul <<21)
#define ADC_TRGOE_POS(n)      	(20 + (n))
#define ADC_TRGOE_MSK(n) 		(0x01ul << ADC_TRGOE_POS(n)) 
/******************************************************************************
* EPVS  
******************************************************************************/
#define ADC_EV0PRD_POS			(0)
#define ADC_EV0PRD_MSK			(0xf << ADC_EV0PRD_POS)
#define ADC_EV1PRD_POS			(4)
#define ADC_EV1PRD_MSK			(0xf << ADC_EV1PRD_POS)
#define ADC_EVPRD_POS(n)		(4*(n))
#define ADC_EVPRD_MSK(n)		(0xf << ADC_EVPRD_POS(n))

#define ADC_EV0CNT_POS			(16)
#define ADC_EV0CNT_MSK			(0xf << ADC_EV0CNT_POS)
#define ADC_EV1CNT_POS			(20)
#define ADC_EV1CNT_MSK			(0xf << ADC_EV1CNT_POS)

#define ADC_TRGEV0PRD(val) 		(((val) & 0xFul) << 0)
#define ADC_TRGEV1PRD(val)   	(((val) & 0xFul) << 4)
/******************************************************************************
* EVSWF  
******************************************************************************/
#define ADC_EV0SWF      		(0x01ul << 0)       
#define ADC_EV1SWF				(0x01ul << 1) 

/******************************************************************************
********************* ADC inline Functions Declaration **********************
******************************************************************************/
static inline void csp_adc_clk_enable(csp_adc_t *ptAdcBase)
{
	ptAdcBase->ECR = ADC_CLKEN;	
}

static inline void csp_adc_clk_disable(csp_adc_t *ptAdcBase)
{
	ptAdcBase->DCR = ADC_CLKEN;	
}

static inline void csp_adc_set_clksrc(csp_adc_t *ptAdcBase,adc_clksel_e eClkSel)
{
	ptAdcBase->ECR = (ptAdcBase->ECR & (~ ADC_CLKSEL_MSK)) | ((eClkSel<<ADC_CLKSEL_POS));	
}

static inline void csp_adc_dbg_enable(csp_adc_t *ptAdcBase)
{
	ptAdcBase->ECR = ADC_DBGEN;
}

static inline void csp_adc_dbg_disable(csp_adc_t *ptAdcBase)
{
	ptAdcBase->DCR = ADC_DBGEN;
}

static inline void csp_adc_set_runmode(csp_adc_t *ptAdcBase, adc_runmode_e eRunMode)
{
	ptAdcBase->MR = (ptAdcBase->MR & (~ADC_CV_MSK)) | (eRunMode <<ADC_CV_POS) ;
}

static inline void csp_adc_set_resolution(csp_adc_t *ptAdcBase, adc_resolution_e eBitNum)
{
	ptAdcBase->CR |= (ptAdcBase->CR & (~ADC_BIT_MSK)) | (eBitNum << ADC_BIT_POS);
}

static inline void csp_adc_set_clkdiv(csp_adc_t *ptAdcBase, uint8_t byDiv)
{
	ptAdcBase->MR = (ptAdcBase->MR & (~ADC_PRLVAL_MASK)) | byDiv;
}

static inline void csp_adc_set_vref(csp_adc_t *ptAdcBase, adc_vref_e eVrefSel)
{
	ptAdcBase->CR =  (ptAdcBase->CR & (~ADC_VREF_MSK)) | (eVrefSel<<ADC_VREF_POS);
}

static inline void csp_adc_set_seq_num(csp_adc_t *ptAdcBase, uint8_t bySeqNum)
{
	ptAdcBase->MR = (ptAdcBase->MR & (~ADC_NBRCH_MSK)) |  ADC_NBRCH(bySeqNum);
}

static inline void csp_adc_set_seqx(csp_adc_t *ptAdcBase, uint8_t bySeqNum, adc_channel_e eAdcChnl, 
				adc_cnt_e eCvCnt,adc_avg_e eAvgSel, bool bAvgctrl, adc_sync_source_e eSyncSrc)
{
	ptAdcBase->SEQ[bySeqNum] = eAdcChnl | (eCvCnt<<ADC_CVCNT_P0S) | (eAvgSel <<ADC_AVGSEL_POS) | 
				(bAvgctrl<<ADC_AVGEN_POS) | (eSyncSrc << ADC_SYNCSRC_POS);
}

static inline void csp_adc_set_cmp0(csp_adc_t *ptAdcBase, uint32_t wCmpData, uint8_t byCmpChnl)
{
	ptAdcBase->CMP0 = wCmpData;
	ptAdcBase->MR = (ptAdcBase->MR & (~ADC_NBRCMP0_MASK)) | ADC_NBRCMP0(byCmpChnl);
} 

static inline void csp_adc_set_cmp1(csp_adc_t *ptAdcBase, uint32_t wCmpData, uint8_t byCmpChnl)
{
	ptAdcBase->CMP1 = wCmpData;
	ptAdcBase->MR = (ptAdcBase->MR & (~ADC_NBRCMP1_MASK)) | ADC_NBRCMP1(byCmpChnl);
} 

//irq
static inline uint8_t csp_adc_get_seq_idx(csp_adc_t *ptAdcBase)
{
	return (uint8_t)((ptAdcBase->SR & ADC_SEQ_IDX_MSK) >> ADC_SEQ_IDX_POS);
}

static inline uint32_t csp_adc_get_sr(csp_adc_t *ptAdcBase)
{
	return (uint32_t)(ptAdcBase->SR);
}

static inline void csp_adc_clr_sr(csp_adc_t *ptAdcBase,adc_sr_e eAdcSr)
{
	ptAdcBase->CSR =  eAdcSr;
}

static inline uint32_t csp_adc_get_isr(csp_adc_t *ptAdcBase)
{
	return (ptAdcBase->MISR);
}

static inline void csp_adc_int_enable(csp_adc_t *ptAdcBase, adc_int_e eInt)
{
	ptAdcBase->IMCR |= eInt; 
}

static inline void csp_adc_int_disable(csp_adc_t *ptAdcBase, adc_int_e eInt)
{
	ptAdcBase->IMCR &= ~eInt; 
}

//cr control: start/stop/en/dis/swtrg
static inline void csp_adc_start(csp_adc_t *ptAdcBase)//在DEFINE写
{
	ptAdcBase->CR |= ADC_START;
}

static inline void csp_adc_stop(csp_adc_t *ptAdcBase)
{
	ptAdcBase->CR |= ADC_STOP;
}

static inline void csp_adc_enable(csp_adc_t *ptAdcBase)
{
	ptAdcBase->CR |= ADC_ADCEN;
}

static inline void csp_adc_disable(csp_adc_t *ptAdcBase)
{
	ptAdcBase->CR |= ADC_ADCDIS;
}

static inline void csp_adc_sw_start(csp_adc_t *ptAdcBase)
{
	ptAdcBase->CR |= ADC_SWTRG;
}

static inline void csp_adc_sw_rst(csp_adc_t *ptAdcBase)
{
	ptAdcBase->CR |= ADC_SWRST;
}

static inline void csp_adc_set_samp_hold(csp_adc_t *ptAdcBase, uint16_t hwSampHold)
{
	ptAdcBase->SHR = (hwSampHold & 0xff);
}

static inline void csp_adc_set_pri(csp_adc_t *ptAdcBase, uint8_t byPri)
{
	ptAdcBase->PRI = (byPri & 0x0f);
}

static inline void csp_adc_set_cmp_mode(csp_adc_t *ptAdcBase, adc_cmpos_e wCmpMode)
{
	ptAdcBase->MR = (ptAdcBase->MR & (~ADC_CMPOS_MSK)) | (wCmpMode << ADC_CMPOS_POS);
} 

static inline uint16_t csp_adc_get_data(csp_adc_t *ptAdcBase, uint8_t byChnl)
{
	return (ptAdcBase->DR[byChnl] & ADC_DATA_MASK);
} 
//syncr
static inline void csp_adc_sync_enable(csp_adc_t *ptAdcBase, adc_sync_in_e eSyncIn)
{
	ptAdcBase->SYNCR |= ADC_SYNC_MSK(eSyncIn);
}

static inline void csp_adc_sync_disable(csp_adc_t *ptAdcBase,  adc_sync_in_e eSyncIn)
{
	ptAdcBase->SYNCR &= ~ADC_SYNC_MSK(eSyncIn);
}

static inline void csp_adc_set_sync_mode(csp_adc_t *ptAdcBase, adc_sync_in_e eSyncIn,adc_ostmd_e eOstMd)
{
	ptAdcBase->SYNCR = ptAdcBase->SYNCR & (~ADC_OSTMD_MSK(eSyncIn));
	ptAdcBase->SYNCR |= ((0x01ul << eSyncIn) | (eOstMd << (8 + eSyncIn)));
}

static inline void csp_adc_sync_rearm(csp_adc_t *ptAdcBase,  adc_sync_in_e eSyncIn)
{
	ptAdcBase->SYNCR |= (0x1 << (eSyncIn + ADC_REARM_POS(eSyncIn)));
}

static inline void csp_adc_set_evtrg_prd(csp_adc_t *ptAdcBase,adc_evtrg_out_e eEvtOut,uint8_t byPeriod)
{
	ptAdcBase->EVPS |= (ptAdcBase->EVPS & ~ADC_EVPRD_MSK(eEvtOut))| (byPeriod << ADC_EVPRD_POS(eEvtOut));
}

static inline void csp_adc_set_evtrg_src(csp_adc_t *ptAdcBase,  adc_evtrg_src_e eEvtSrc ,adc_evtrg_out_e eEvtOut)
{
	ptAdcBase->EVTRG = (ptAdcBase->EVTRG & ~ADC_TRGSRC_MSK(eEvtOut)) | eEvtSrc<<(ADC_TRGSRC_POS(eEvtOut));
}

static inline void csp_adc_evtrg_enable(csp_adc_t *ptAdcBase ,adc_evtrg_out_e eEvtOut)
{
	ptAdcBase->EVTRG |=  0x1<<ADC_TRGOE_POS(eEvtOut);
}

static inline void csp_adc_evtrg_disable(csp_adc_t *ptAdcBase ,adc_evtrg_out_e eEvtOut)
{
	ptAdcBase->EVTRG &=  ~(0x1<<ADC_TRGOE_POS(eEvtOut));
}

static inline void csp_adc_sw_evtrg(csp_adc_t *ptAdcBase,adc_evtrg_out_e eEvtOut)			
{
	ptAdcBase->EVSWF = (0X1 << eEvtOut);
}

static inline void csp_adc_drmask_enable(csp_adc_t *ptAdcBase,uint16_t hwDrNum)			
{
	ptAdcBase->DRMASK |= (0X1 << hwDrNum);
}

static inline void csp_adc_drmask_disable(csp_adc_t *ptAdcBase,uint16_t hwDrNum)			
{
	ptAdcBase->DRMASK &=  ~(0X1 << hwDrNum);
}

static inline void csp_adc_trg_delay0(csp_adc_t *ptAdcBase,adc_sync_in_e eSyncIn,uint16_t byDelay)
{
	ptAdcBase->TDL0 = (ptAdcBase->TDL0 & ~(0xFFul << (eSyncIn * 8))) | byDelay;
}

static inline void csp_adc_trg_delay1(csp_adc_t *ptAdcBase,adc_sync_in_e eSyncIn,uint16_t byDelay)
{
	ptAdcBase->TDL1 = (ptAdcBase->TDL1 & ~(0xFFul << ((eSyncIn - 3)  * 8))) | byDelay;
}

#endif

