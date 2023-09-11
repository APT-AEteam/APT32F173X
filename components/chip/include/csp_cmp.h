/***********************************************************************//** 
 * \file  csp_cmp.h
 * \brief CMP description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-10-14 <td>V0.0  <td>YYM   <td>modify
 * <tr><td> 2023-09-07 <td>V0.1  <td>LHY    <td>modify
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CSP_CMP_H
#define _CSP_CMP_H

/* Includes ------------------------------------------------------------------*/
#include <soc.h>

/// \struct csp_cmp_t
/// \brief CMP reg description   

typedef struct
{
   __IOM uint32_t  CEDR;  		//0x0000	Clock control & ID
   __IOM uint32_t  CR;		    //0x0004	Start & Stop Ctrl
   __IOM uint32_t  DFCR1;		//0x0008	Digital filter 1 control register 
   __IOM uint32_t  DFCR2;  		//0x000C	Digital filter 2 control register 
   __IOM uint32_t  WFCR;		//0x0010	captures the window control register
   __IOM uint32_t  INPCR;   	//0x0014	Input control register
   __IM  uint32_t  RISR;       	//0x0018    Interrupt RISR
   __IOM uint32_t  IMCR;       	//0x001c    Interrupt IMCR
   __IM  uint32_t  MISR;       	//0x0020    Interrupt MISR
   __OM  uint32_t  ICR;        	//0x0024    Interrupt clear
} csp_cmp_t;


#define CMP_INT_MSK             (0x01)


/******************************************************************************
* CEDR : Clock control & ID
******************************************************************************/
#define	CMP_IDCODE_POS			(8)
#define	CMP_IDCODE_MSK			(0xFFFFFFul << CMP_CLK_POS)

#define	CMP_SWRST_POS           (7)
#define CMP_SOFTRESET           (0x01<<CMP_SWRST_POS)

#define	CMP_CLK_POS			    (0)
#define	CMP_CLK_MSK			    (0x01ul << CMP_CLK_POS)


/******************************************************************************
* CR : Start & Stop Ctrl
******************************************************************************/
#define	CMP_CMPEN_POS	        (0)
#define	CMP_CMPEN_MSK		    (0x01ul << CMP_CMPEN_POS)
typedef enum
{
	CMP_DIS			   = 0x00,
	CMP_EN,				
}csp_cmp_cmpen_e;

#define	CMP_PHYSTPOL_POS	    (1)
#define	CMP_PHYSTPOL_MSK		(0x07ul << CMP_PHYSTPOL_POS)
typedef enum
{
	PHYST_0mv		   = 0x00,
	PHYST_10mv,
	PHYST_15mv,
	PHYST_25mv,
	PHYST_35mv,
	PHYST_45mv,
	PHYST_55mv,	
	PHYST_65mv				
}csp_cmp_phystpol_e;

#define	CMP_POLARITY_POS		(6)
#define	CMP_POLARITY_MSK		(0x01ul << CMP_POLARITY_POS)
typedef enum
{
    CMP_OUT_DIRECT     = 0x00,
	CMP_OUT_UNDIRECT            
}csp_cmp_polarity_e;

#define	CMP_SYNCOE_POS		    (7)
#define	CMP_SYNCOE_MSK		    (0x01ul << CMP_SYNCOE_POS)
typedef enum
{
	SYNCOE_DIS		   = 0x00,
	SYNCOE_EN,				
}csp_cmp_syncoe_e;

#define	CMP_EVE_SEL_POS		    (8)
#define	CMP_EVE_SEL_MSK		    (0x03ul << CMP_EVE_SEL_POS)
typedef enum
{
	EVE_SEL_DOWN	   = 0x00,
	EVE_SEL_UP,
	EVE_SEL_DOWN_UP,
	EVE_SEL_UP1
			
}csp_cmp_eve_sel_e;

#define	CMP_DFLT1EN_POS		    (10)
#define	CMP_DFLT1EN_MSK		    (0x01ul << CMP_DFLT1EN_POS)

#define	CMP_DFLT2EN_POS		    (11)
#define	CMP_DFLT2EN_MSK		    (0x01ul << CMP_DFLT2EN_POS)

#define	CMP_WFLTEN_POS		    (12)
#define	CMP_WFLTEN_MSK		    (0x01ul << CMP_WFLTEN_POS)

#define	CMP_WFALIGN_POS		    (13)
#define	CMP_WFALIGN_MSK		    (0x01ul << CMP_WFALIGN_POS)
typedef enum
{
	ALIGN_DISALLOW_LAST    = 0x00,
	ALIGN_ALLOW_LAST             
	
}csp_cmp_wfalign_e;

#define	CMP_WFOSET_POS		    (14)
#define	CMP_WFOSET_MSK		    (0x03ul << CMP_WFOSET_POS)
typedef enum
{
	WFOSET_DIS			= 0x00,
	WFOSET_HIGH,
	WFOSET_LOW,
	WFOSET_DIS1
}csp_cmp_wfoset_e;

#define	CMP_SPEEDUP_POS		    (16)
#define	CMP_SPEEDUP_MSK		    (0x01ul << CMP_SPEEDUP_POS)
typedef enum
{
	SPEEDUP_NORMAL      = 0x00,
	SPEEDUP_FAST                 
	
}csp_cmp_speedup_e;

#define	CMP_DOUBLEI_POS		    (17)
#define	CMP_DOUBLEI_MSK		    (0x01ul << CMP_DOUBLEI_POS)
typedef enum
{
	DOUBLEI_NORMAL      = 0x00,
	DOUBLEI_DOUBLE               
	
}csp_cmp_doublei_e;

#define	CMP_NHYSTPOL_POS	    (18)
#define	CMP_NHYSTPOL_MSK		(0x07ul << CMP_NHYSTPOL_POS	)
typedef enum
{
	NHYST_0mv		    = 0x00,
	NHYST_10mv,
	NHYST_15mv,
	NHYST_25mv,
	NHYST_35mv,
	NHYST_45mv,
	NHYST_55mv,	
	NHYST_65mv				
}csp_cmp_nhystpol_e;

#define	CMP_CPOSEL_POS		    (23)
#define	CMP_CPOSEL_MSK		    (0x01ul << CMP_CPOSEL_POS)
typedef enum
{
	CPOS_DIRECT		    = 0x00,
	CPOS_1				         
}csp_cmp_cr_cpos_e;

#define	CMP_CPOUT0_POS		    (24)
#define	CMP_CPOUT0_MSK		    (0x01ul << CMP_CPOUT0_POS)

#define	CMP_CPOUT1_POS		    (25)
#define	CMP_CPOUT1_MSK		    (0x01ul << CMP_CPOUT1_POS)

#define	CMP_CPOUT2_POS		    (26)
#define	CMP_CPOUT2_MSK		    (0x01ul << CMP_CPOUT2_POS)

#define	CMP_CPOUT3_POS		    (27)
#define	CMP_CPOUT3_MSK		    (0x01ul << CMP_CPOUT3_POS)


/******************************************************************************
* DFCR1 : Digital filter 1 control register 
******************************************************************************/
//CMP_DFCR1 register
#define	CMP_DFCR1_DEPTH_POS		    (0)
#define	CMP_DFCR1_DEPTH_MSK		    (0x03ul << CMP_DFCR1_DEPTH_POS)
typedef enum
{
	DEPTH1_8        =   0,
	DEPTH1_16       =   1,
	DEPTH1_32       =   2,
	DEPTH1_64       =   3,
	DEPTH1_128      =   4,
	DEPTH1_256      =   5,
	DEPTH1_512      =   6,
	DEPTH1_1024     =   7
}csp_dfcr1_depth_e;

#define	CMP_DFCR1_DIVN_POS		    (16)
#define	CMP_DFCR1_DIVN_MSK		    (0x1ful << CMP_DFCR1_DIVN_POS)

#define	CMP_DFCR1_DIVM_POS		    (8)
#define	CMP_DFCR1_DIVM_MSK		    (0xfful << CMP_DFCR1_DIVM_POS)

/******************************************************************************
* DFCR2 : Digital filter 2 control register 
******************************************************************************/
//CMP_DFCR2 register
#define	CMP_DFCR2_DEPTH_POS		    (0)
#define	CMP_DFCR2_DEPTH_MSK		    (0x03ul << CMP_DFCR2_DEPTH_POS)
typedef enum
{
	DEPTH2_16       =   0,
	DEPTH2_32       =   1,
	DEPTH2_64       =   2,
	DEPTH2_128      =   3,
	DEPTH2_256      =   4,
	DEPTH2_512      =   5,
	DEPTH2_1024     =   6,
	DEPTH2_2048     =   7
}csp_dfcr2_depth_e;

#define	CMP_DFCR2_DIVN_POS		    (16)
#define	CMP_DFCR2_DIVN_MSK		    (0x1ful << CMP_DFCR2_DIVN_POS)

#define	CMP_DFCR2_DIVM_POS		    (8)
#define	CMP_DFCR2_DIVM_MSK		    (0xfful << CMP_DFCR2_DIVM_POS)

/******************************************************************************
* WFCR : captures the window control register 
******************************************************************************/
//CMP_WFCR register
#define	CMP_WFCR_WCNT_POS		    (0)
#define	CMP_WFCR_WCNT_MSK		    (0x3fful << CMP_WFCR_WCNT_POS)

#define	CMP_WFCR_CLKDIV_POS		    (10)
#define	CMP_WFCR_CLKDIV_MSK		    (0x3ful << CMP_WFCR_CLKDIV_POS)

#define	CMP_WFCR_DCNT_POS		    (16)
#define	CMP_WFCR_DCNT_MSK		    (0xfful << CMP_WFCR_DCNT_POS)

/******************************************************************************
* INPCR : captures the window control register 
******************************************************************************/
//CMP_INPCR register
#define	CMP_INPCR_NSEL_POS		    (0)
#define	CMP_INPCR_NSEL_MSK		    (0xful << CMP_INPCR_NSEL_POS)
typedef enum
{
	NSEL_CP0       =  0,
	NSEL_CP1       =  1,
	NSEL_CP2       =  2,
	NSEL_CP3       =  3,	
	NSEL_CP4       =  4,
	NSEL_CP5       =  5,
	NSEL_1VBUF     =  7
}csp_nsel_e;

#define	CMP_INPCR_PSEL_POS		    (8)
#define	CMP_INPCR_PSEL_MSK		    (0xful << CMP_INPCR_PSEL_POS)
typedef enum
{
	PSEL_CP0       = 0,
	PSEL_CP1       = 1,
	PSEL_CP2       = 2,
	PSEL_CP3       = 3,	
	PSEL_CP4       = 4,
	PSEL_CP5       = 5,
	PSEL_CP6       = 6,
	PSEL_CP7       = 7
}csp_psel_e;

#define	CMP_N_INPCR_FITLER_ENABLE_POS		    (16)
#define	CMP_N_INPCR_FITLER_ENABLE_MSK		    (0x1ul << CMP_N_INPCR_FITLER_ENABLE_POS)

/******************************************************************************
* RISR : Interrupt RISR
******************************************************************************/
//CMP_RISR register
#define	CMP_RISR_EDGEDET0_POS		     (0)
#define	CMP_RISR_EDGEDET0_MSK		     (0x1ul << CMP_RISR_EDGEDET0_POS)

#define	CMP_RISR_EDGEDET1_POS		     (1)
#define	CMP_RISR_EDGEDET1_MSK		     (0x1ul << CMP_RISR_EDGEDET1_POS)

#define	CMP_RISR_EDGEDET2_POS		     (2)
#define	CMP_RISR_EDGEDET2_MSK		     (0x1ul << CMP_RISR_EDGEDET2_POS)

/******************************************************************************
* IMCR : Interrupt IMCR
******************************************************************************/
//CMP_IMCR register
#define	CMP_IMCR_POS		        (0)
#define	CMP_IMCR_MSK		        (0x1ul << CMP_IMCR_POS)
typedef enum
{
	CMP_NONE_INT      = 0, 
	CMP_EDGEDET_INT   	
}
csp_cmp_int_e;

/******************************************************************************
* MISR : Interrupt MISR
******************************************************************************/
//CMP_MISR register
#define	CMP_MISR_EDGEDET0_POS		     (0)
#define	CMP_MISR_EDGEDET0_MSK		     (0x1ul << CMP_MISR_EDGEDET0_POS)

#define	CMP_MISR_EDGEDET1_POS		     (1)
#define	CMP_MISR_EDGEDET1_MSK		     (0x1ul << CMP_MISR_EDGEDET1_POS)

#define	CMP_MISR_EDGEDET2_POS		     (2)
#define	CMP_MISR_EDGEDET2_MSK		     (0x1ul << CMP_MISR_EDGEDET2_POS)

/******************************************************************************
* ICR : Interrupt ICR
******************************************************************************/
//CMP_ICR register
#define	CMP_ICR_EDGEDET_POS		         (0)
#define	CMP_ICR_EDGEDET_MSK		         (0x1ul << CMP_ICR_EDGEDET_POS)


/******************************************************************************
********************** CMP inline Functions Declaration ***********************
******************************************************************************/
static inline void csp_cmp_clk_enable(csp_cmp_t *ptCmpBase)
{
	ptCmpBase->CEDR |=  CMP_CLK_MSK;	
}
 
static inline void csp_cmp_clk_disable(csp_cmp_t *ptCmpBase)
{
	ptCmpBase->CEDR &= ~CMP_CLK_MSK;
}

static inline void  csp_cmp_software_reset(csp_cmp_t *ptCmpBase)
{
	ptCmpBase->CEDR |= CMP_SOFTRESET;
}

static inline void csp_cmp_start(csp_cmp_t *ptCmpBase)
{
	ptCmpBase->CR |= CMP_CMPEN_MSK;
}

static inline void csp_cmp_stop(csp_cmp_t *ptCmpBase)
{
	ptCmpBase->CR &= ~CMP_CMPEN_MSK;	
}

static inline void  csp_cmp_hystpol(csp_cmp_t *ptCmpBase , csp_cmp_phystpol_e ePhystpol,csp_cmp_phystpol_e eNhystpol)
{
	ptCmpBase->CR = (ptCmpBase->CR&~(CMP_PHYSTPOL_MSK|CMP_NHYSTPOL_MSK))|(ePhystpol<<CMP_PHYSTPOL_POS)|(eNhystpol<<CMP_NHYSTPOL_POS	);
}

static inline void  csp_cmp_polarity(csp_cmp_t *ptCmpBase , csp_cmp_polarity_e ePolarity)
{
	ptCmpBase->CR = (ptCmpBase->CR&~(CMP_POLARITY_MSK))|(ePolarity<<CMP_POLARITY_POS);
}

static inline void  csp_cmp_evtrg(csp_cmp_t *ptCmpBase , csp_cmp_eve_sel_e eEveSel)
{
	ptCmpBase->CR = (ptCmpBase->CR&~(CMP_SYNCOE_MSK|CMP_EVE_SEL_MSK))|(eEveSel<<CMP_EVE_SEL_POS);
}

static inline void csp_cmp_syncoe_enable(csp_cmp_t *ptCmpBase)
{
	ptCmpBase->CR |= CMP_SYNCOE_MSK;
}

static inline void csp_cmp_syncoe_disable(csp_cmp_t *ptCmpBase)
{
	ptCmpBase->CR &= ~CMP_SYNCOE_MSK;
}


static inline void  csp_cmp_dflt1_enable(csp_cmp_t *ptCmpBase)
{
	ptCmpBase->CR = (ptCmpBase->CR&~CMP_DFLT1EN_MSK)|(1<<CMP_DFLT1EN_POS);
}

static inline void  csp_cmp_dflt1_disable(csp_cmp_t *ptCmpBase)
{
	ptCmpBase->CR = (ptCmpBase->CR&~CMP_DFLT1EN_MSK)|(0<<CMP_DFLT1EN_POS);
}

static inline void  csp_cmp_dflt2_enable(csp_cmp_t *ptCmpBase)
{
	ptCmpBase->CR = (ptCmpBase->CR&~CMP_DFLT2EN_MSK)|(1<<CMP_DFLT2EN_POS);
}

static inline void  csp_cmp_dflt2_disable(csp_cmp_t *ptCmpBase)
{
	ptCmpBase->CR = (ptCmpBase->CR&~CMP_DFLT2EN_MSK)|(0<<CMP_DFLT2EN_POS);
}

static inline void  csp_cmp_set_wf(csp_cmp_t *ptCmpBase, csp_cmp_wfalign_e eWfalign, csp_cmp_wfoset_e eWfoset, bool bEnable)
{
	ptCmpBase->CR = (ptCmpBase->CR&~(CMP_WFLTEN_MSK|CMP_WFALIGN_MSK|CMP_WFOSET_MSK))|(bEnable<<CMP_WFLTEN_POS)|(eWfalign<<CMP_WFALIGN_POS)|(eWfoset<<CMP_WFOSET_POS);
}

static inline void  csp_cmp_speedup(csp_cmp_t *ptCmpBase , csp_cmp_speedup_e eSpeedup)
{
	ptCmpBase->CR = (ptCmpBase->CR&~(CMP_SPEEDUP_MSK))|(eSpeedup<<CMP_SPEEDUP_POS);
}

static inline void  csp_cmp_doublei(csp_cmp_t *ptCmpBase , csp_cmp_doublei_e eDoublei)
{
	ptCmpBase->CR = (ptCmpBase->CR&~(CMP_DOUBLEI_MSK))|(eDoublei<<CMP_DOUBLEI_POS);
}

static inline void  csp_cmp_out(csp_cmp_t *ptCmpBase , csp_cmp_cr_cpos_e ePos_Sel)
{
	ptCmpBase->CR = (ptCmpBase->CR&~(CMP_CPOSEL_MSK))|(ePos_Sel<<CMP_CPOSEL_POS);
}

static inline uint8_t csp_cmp_get_out0(csp_cmp_t *ptCmpBase)
{
	return (uint8_t)((ptCmpBase->CR & CMP_CPOUT0_MSK) >> CMP_CPOUT0_POS);
}

static inline uint8_t csp_cmp_get_out1(csp_cmp_t *ptCmpBase)
{
	return (uint8_t)((ptCmpBase->CR & CMP_CPOUT1_MSK) >> CMP_CPOUT1_POS);
}

static inline uint8_t csp_cmp_get_out2(csp_cmp_t *ptCmpBase)
{
	return (uint8_t)((ptCmpBase->CR & CMP_CPOUT2_MSK) >> CMP_CPOUT2_POS);
}

static inline uint8_t csp_cmp_get_out3(csp_cmp_t *ptCmpBase)
{
	return (uint8_t)((ptCmpBase->CR & CMP_CPOUT3_MSK) >> CMP_CPOUT3_POS);
}

static inline void  csp_cmp_dfcr1(csp_cmp_t *ptCmpBase , csp_dfcr1_depth_e eDepth,uint8_t byDivn,uint8_t byDivm)
{
	ptCmpBase->DFCR1 = (ptCmpBase->DFCR1&~(CMP_DFCR1_DEPTH_MSK|CMP_DFCR1_DIVN_MSK|CMP_DFCR1_DIVM_MSK))|(eDepth<<CMP_DFCR1_DEPTH_POS)|(byDivn<<CMP_DFCR1_DIVN_POS)|(byDivm<<CMP_DFCR1_DIVM_POS);
}

static inline void  csp_cmp_dfcr2(csp_cmp_t *ptCmpBase , csp_dfcr2_depth_e eDepth,uint8_t byDivn,uint8_t byDivm)
{
	ptCmpBase->DFCR2 = (ptCmpBase->DFCR2&~(CMP_DFCR2_DEPTH_MSK|CMP_DFCR2_DIVN_MSK|CMP_DFCR2_DIVM_MSK))|(eDepth<<CMP_DFCR2_DEPTH_POS)|(byDivn<<CMP_DFCR2_DIVN_POS)|(byDivm<<CMP_DFCR2_DIVM_POS);
}

static inline void  csp_cmp_wfcr(csp_cmp_t *ptCmpBase, uint16_t hwWcnt,uint8_t byClkDiv,uint8_t byDcnt)
{
	ptCmpBase->WFCR = (ptCmpBase->WFCR&~(CMP_WFCR_WCNT_MSK|CMP_WFCR_CLKDIV_MSK|CMP_WFCR_DCNT_MSK))|(hwWcnt<<CMP_WFCR_WCNT_POS)|(byClkDiv<<CMP_WFCR_CLKDIV_POS)|(byDcnt<<CMP_WFCR_DCNT_POS);
}

static inline void  csp_cmp_inpcr(csp_cmp_t *ptCmpBase , csp_nsel_e eNsel, csp_psel_e ePsel)
{
	ptCmpBase->INPCR = (ptCmpBase->INPCR&~(CMP_INPCR_NSEL_MSK|CMP_INPCR_PSEL_MSK))|(eNsel<<CMP_INPCR_NSEL_POS)|(ePsel<<CMP_INPCR_PSEL_POS);
}

static inline void  csp_cmp_n_inpcr_fitler_enable(csp_cmp_t *ptCmpBase)
{
	ptCmpBase->INPCR |= CMP_N_INPCR_FITLER_ENABLE_MSK; 	
}

static inline void  csp_cmp_n_inpcr_fitler_disable(csp_cmp_t *ptCmpBase)
{
	ptCmpBase->INPCR &= ~CMP_N_INPCR_FITLER_ENABLE_MSK; 	
}

static inline uint8_t csp_cmp_get_risr(csp_cmp_t *ptCmpBase)
{
	return (uint8_t)((ptCmpBase-> RISR)&0x03);
}

static inline void csp_cmp_int_enable(csp_cmp_t *ptCmpBase, csp_cmp_int_e eCmpInt)
{
	ptCmpBase->IMCR |= eCmpInt; 
}

static inline void csp_cmp_int_disable(csp_cmp_t *ptCmpBase, csp_cmp_int_e eCmpInt)
{
	ptCmpBase->IMCR &= ~eCmpInt; 
}

//static inline void csp_cmp_int_clear(csp_cmp_t *ptCmpBase,csp_cmp_int_e eCmpInt)
//{
//	ptCmpBase->ICR|= eCmpInt;
//}

//static inline  void csp_cmp_edgedet_int_enable(csp_cmp_t *ptCmpBase,bool bEnable)
//{
//	ptCmpBase->IMCR = (ptCmpBase->IMCR & ~CMP_INT_MSK) | bEnable;
//}

static inline uint32_t csp_cmp_get_imcr(csp_cmp_t *ptCmpBase)
{
	return (uint32_t)(ptCmpBase-> IMCR);
}

static inline uint32_t csp_cmp_get_misr(csp_cmp_t *ptCmpBase)
{
	return (uint32_t)(ptCmpBase-> MISR);
}

//static inline void csp_cmp_edgedet_int_clear(csp_cmp_t *ptCmpBase)
//{
//	ptCmpBase->ICR|= 0x01;
//}

static inline void csp_cmp_clr_isr(csp_cmp_t *ptCmpBase, csp_cmp_int_e eInt)
{
	ptCmpBase -> ICR =  eInt;
}

#endif



