/*********************************************************************** 
 * \file  csp_gptb.h
 * \brief  headfile for GPTB in csp level
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-6-17 <td>V0.0  <td>ljy   <td>initial
 * <tr><td> 2023-9-18 <td>V0.1  <td>wch   <td>code normalization
 * </table>
 * *********************************************************************
*/

#ifndef _CSP_GPTB_H
#define _CSP_GPTB_H

#include "soc.h"

/// \struct csp_gptb_t
/// \brief GPTB reg description
typedef struct
{
	__IOM uint32_t CEDR;			//0x0000	ID & Clock Enable/Disable Register                                              
	__IOM uint32_t RSSR;            //0x0004    RESET/START/STOP Register                                         
	__IOM uint32_t PSCR;            //0x0008    Counter Clock Prescaler Register                                          
	__IOM uint32_t CR;              //0x000c    General Control Register                                          
	__IOM uint32_t SYNCR;           //0x0010    Synchronization Control Register                                         
    __IOM uint32_t GLDCR;           //0x0014    Global Load Control Register                                         
    __IOM uint32_t GLDCFG;          //0x0018    Global Load Configuration Register                                        
    __IOM uint32_t GLDCR2;          //0x001c                                             
    __IOM uint32_t RESERVED;
    __IOM uint32_t PRDR;            //0x0024	Period Register                                             
    __IOM uint32_t RESERVED1;                                              
    __IOM uint32_t CMPA;            //0x002c	Compare A Register	                                             
    __IOM uint32_t CMPB;            //0x0030    Compare B Register                                         
    __IOM uint32_t RESERVED2[2];
    __IOM uint32_t CMPLDR;          //0x003c    Compare Data Load Control Register                                         
    __IOM uint32_t CNT;             //0x0040    Counter Register                                          
    __IOM uint32_t AQLDR;           //0x0044	Action Qualifier Load Control Register                                             
    __IOM uint32_t AQCR1;           //0x0048    Action Qualifier Control Register 1                                         
    __IOM uint32_t AQCR2;           //0x004c    Action Qualifier Control Register 2                                         
    __IOM uint32_t RESERVED3[3];
    __IOM uint32_t AQOSF;           //0x005c    Action Qualifier One Shot Force Register                                         
    __IOM uint32_t AQCSF;           //0x0060    Action Qualifier Continuous Force Register                                         
    __IOM uint32_t DBLDR;           //0x0064                                             
    __IOM uint32_t DBCR;            //0x0068                                             
    __IOM uint32_t DPSCR;           //0x006c                                             
    __IOM uint32_t DBDTR;           //0x0070                                             
    __IOM uint32_t DBDTF;           //0x0074                                             
    __IOM uint32_t RESERVED4;
    __IOM uint32_t EMSRC;           //0x007c                                             
    __IOM uint32_t EMSRC2;          //0x0080                                             
    __IOM uint32_t EMPOL;           //0x0084                                             
    __IOM uint32_t EMECR;           //0x0088                                             
    __IOM uint32_t EMOSR;           //0x008c                                             
    __IOM uint32_t RESERVED5;
    __IM  uint32_t EMSLSR;          //0x0094                                             
    __OM  uint32_t EMSLCLR;         //0x0098                                             
    __IM  uint32_t EMHLSR;          //0x009c                                             
    __OM  uint32_t EMHLCLR;         //0x00a0                                             
    __IOM uint32_t EMFRCR;			//0x00a4
    __IM  uint32_t EMRISR;          //0x00a8                                             
    __IM  uint32_t EMMISR;          //0x00ac                                             
    __IOM uint32_t EMIMCR;          //0x00b0                                             
    __IOM uint32_t EMICR;           //0x00b4                                             
    __IOM uint32_t TRGFTCR;         //0x00b8	Digital Compare Filter Control Register                                             
    __IOM uint32_t TRGFTWR;         //0x00bc    Digital Compare Filter Window Register                                         
    __IOM uint32_t EVTRG;           //0x00c0    Event Generation Control Register                                         
    __IOM uint32_t EVPS;            //0x00c4	Event Counter Prescaler                                          
    __IOM uint32_t EVCNTINIT;       //0x00c8                                             
    __IOM uint32_t EVSWF;           //0x00cc 	Event Counter Load Control Register                                            
    __IOM uint32_t RISR;            //0x00d0    Raw Interrupt Status Register                                         
    __IOM uint32_t MISR;            //0x00d4    Masked Interrupt Status Register                                         
    __IOM uint32_t IMCR;            //0x00d8    Interrupt Masking Control Register                                         
    __IOM uint32_t ICR;             //0x00dc    Interrupt Clear Register                                         
    __IOM uint32_t REGLK;           //0x00e0                                             
    __IOM uint32_t REGLK2;          //0x00e4                                             
    __IOM uint32_t REGPROT;         //0x00e8   
    __IM uint32_t  RESERVED6[467-3];
	__IM uint32_t  CMPAA;	        //0x082C	cmpa active reg for capture mode
    __IM uint32_t  CMPBA;	        //0x0830	cmpb active reg for capture mode
} csp_gptb_t;

///CEDR
#define GPTB_CLKEN          (1ul)
#define GPTB_DBGEN_POS		(1)
#define GPTB_DBGEN          (1ul << 1)

#define GPTB_CKSRC_POS		(3)
#define GPTB_CKSRC_MSK		(1ul << GPTB_CKSRC_POS)
#define GPTB_CKSRC_SYNCIN4  (1ul << GPTB_CKSRC_POS)
#define GPTB_CKSRC_PCLK		(0ul << GPTB_CKSRC_POS)
typedef enum
{
	GPTB_CLK_SRC_PCLK	= 0,		
    GPTB_CLK_SRC_SYNCIN4 = 1		
}gptb_clksrc_e;

#define GPTB_START_POS		(6)
#define GPTB_START_MSK		(0x1ul << GPTB_START_POS)
#define GPTB_START_SHD      (0ul << GPTB_START_POS)
#define GPTB_START_IMMD     (1ul << GPTB_START_POS)
#define GPTB_FLTPRS_POS		(8)
#define GPTB_FLTPRS_MSK		(0xfful << GPTB_FLTPRS_POS)

///RSSR
#define GPTB_START		(0x1ul)
#define GPTB_RESET_POS	(12)
#define GPTB_RESET_MSK	(0xf << GPTB_RESET_POS)
#define GPTB_RESET		(0x5ul << GPTB_RESET_POS)

///CR
#define GPTB_CNTMD_POS  (0)
#define GPTB_CNTMD_MSK	(0x3ul << GPTB_CNTMD_POS)
typedef enum{
	GPTB_CNTMD_UP = 0,
	GPTB_CNTMD_DN,
	GPTB_CNTMD_UPDN
}gptb_cntmd_e;

#define GPTB_STARTMODE_POS	(2)
#define GPTB_STARTMODE_MSK 	(0x1ul << GPTB_STARTMODE_POS)
typedef enum{
	GPTB_SW_START = 0,
	GPTB_SYNC_START
}gptb_startmode_e;

#define GPTB_STPST_POS		(3)
#define GPTB_STPST_MSK 		(0x1 << GPTB_STPST_POS)
typedef enum{
	GPTB_STPST_HZ = 0,
	GPTB_STPST_LOW
}gptb_stpst_e;

#define GPTB_PRDLD_POS		(4)
#define GPTB_PRDLD_MSK		(0x3ul << GPTB_PRDLD_POS)
typedef enum {
	GPTB_LDPRDR_PEND = 0,
	GPTB_LDPRDR_LOAD_SYNC,
	GPTB_LDPRDR_PEND_LOAD_SYNC,
	GPTB_LDPRDR_IMMD
}gptb_ldprdr_e;

#define GPTB_RUNMD_POS		(6)
#define GPTB_RUNMD_MSK		(0x1 << GPTB_RUNMD_POS)
typedef enum{
	GPTB_RUNMD_CONT = 0,
	GPTB_RUNMD_ONCE,
}gptb_runmd_e;

#define GPTB_CAPLD_EN   	(1ul << 8)
#define GPTB_CAPLD_POS		(8)
#define GPTB_CAPLD_MSK		(0x1 << GPTB_CAPLD_POS)

#define GPTB_BURST_EN   	(1ul << 9)
#define GPTB_BURST_POS      (9)
#define GPTB_BURST_MSK      (1ul << GPTB_BURST_POS)

#define GPTB_FLT_INIT       (1ul << 10)
#define GPTB_FLT_INIT_POS   (10)
#define GPTB_FLT_INIT_MSK   (1ul << GPTB_FLT_INIT_POS)

#define GPTB_CGSRC_POS		(11)
#define GPTB_CGSRC_MSK		(0x3 << GPTB_CGSRC_POS)
typedef enum {
	GPTB_SRC_CHAX = 0,
	GPTB_SRC_CHBX,
}gptb_cgsrc_e;

#define GPTB_CGFLT_POS		(13)
#define GPTB_CGFLT_MSK		(0x7 << GPTB_CGFLT_POS)
typedef enum {
    GPTB_FLT_BP = 0,
	GPTB_FLT_2,
	GPTB_FLT_4,
	GPTB_FLT_6,
	GPTB_FLT_8,
	GPTB_FLT_16,
	GPTB_FLT_32,
	GPTB_FLT_64
}gptb_cgflt_e;

#define GPTB_PSCLD_POS		(16)
#define GPTB_PSCLD_MSK		(0x3 << GPTB_PSCLD_POS)

#define GPTB_MODE_POS		(18)
#define GPTB_MODE_MSK     	(1ul << GPTB_MODE_POS)
typedef enum{
	GPTB_MODE_CAPTURE = 0,
	GPTB_MODE_WAVE
}gptb_workmode_e;

#define CAPMODE_SEL_POS  	(19)
typedef enum{
	GPTB_MERGE_CAP = 0,
	GPTB_SEPARATE_CAP
}gptb_capmd_e;

#define GPTB_CAPMD_POS  	(21)
#define GPTB_CAPMD_MSK		(0x1 << GPTB_CAPMD_POS)

#define GPTB_CAPREARM   	(1ul << 20)

#define GPTB_STOPWRAP_POS	(22)
#define GPTB_STOPWRAP_MSK	(0x3 << GPTB_STOPWRAP_POS)

#define GPTB_CMPA_RST_POS	(24)
#define GPTB_CMPA_RST_MSK	(0x1 << GPTB_CMPA_RST_POS) 	
#define GPTB_CMPB_RST_POS	(25)
#define GPTB_CMPB_RST_MSK	(0x1 << GPTB_CMPB_RST_POS)
#define GPTB_CMPC_RST_POS	(26)
#define GPTB_CMPC_RST_MSK	(0x1 << GPTB_CMPC_RST_POS) 	
#define GPTB_CMPD_RST_POS	(27)
#define GPTB_CMPD_RST_MSK	(0x1 << GPTB_CMPD_RST_POS)
	
///SYNCR Regiser
#define GPTB_SYNC_POS(ch)	(ch)
#define GPTB_SYNC_MSK(ch)	(1<<ch)
#define GPTB_SYNCEN(ch)		(1<<ch)
typedef enum{
	GPTB_SYNCIN_0	= 0,			
	GPTB_SYNCIN_1,						
	GPTB_SYNCIN_2,						
	GPTB_SYNCIN_3,			
	GPTB_SYNCIN_4,
	GPTB_SYNCIN_5,
    GPTB_SYNCIN_6		
}gptb_syncin_e;

#define GPTB_OSTMD_POS(ch)	(ch+8)
#define GPTB_OSTMD_MSK(ch)	(1<<(GPTB_OSTMD_POS(ch)))

typedef enum {
	GPTB_OSTMD_CONT = 0,
	GPTB_OSTMD_OS
}gptb_syncmd_e;

#define GPTB_REARM_POS(ch)	(ch+15)
#define GPTB_REARM_MSK(ch)	(1<<(ch+15))
#define GPTB_REARM(ch) 		(1<<(ch+15))

#define GPTB_TXREARM0_POS	(22)
#define GPTB_TXREARM0_MSK	(0x3 << GPTB_TXREARM0_POS)
typedef enum{
	GPTB_TXREARM0_DIS =0,
	GPTB_TX1REARM0,
	GPTB_TX2REARM0,
	GPTB_TX1_2REARM0
}gptb_txrearm0_e;

#define GPTB_TRGO0SEL_POS	(24)
#define GPTB_TRGO0SEL_MSK	(0x7<<GPTB_TRGO0SEL_POS)

#define GPTB_TRGO1SEL_POS	(27)
#define GPTB_TRGO1SEL_MSK	(0x7<<GPTB_TRGO1SEL_POS)

#define GPTB_AREARM_POS		(30)
#define GPTB_AREARM_MSK		(0x3<<GPTB_AREARM_POS)
typedef enum{
	GPTB_AREARM_DIS = 0,
	GPTB_AREARM_ZRO,
	GPTB_AREARM_PRD,
	GPTB_AREARM_ZRO_PRD
}gptb_arearm_e;

///GLDCR Regiser
#define GPTB_GLDEN_POS		(0)
#define GPTB_GLDEN_MSK		(0x1)
#define GPTB_GLDMD_POS		(1)
#define GPTB_GLDMD_MSK		(0xf << GPTB_GLDMD_POS)

#define GPTB_GLDCR_OSTMD_POS 	(5)
#define GPTB_GLDCR_OSTMD_MSK	(0x1 << GPTB_GLDCR_OSTMD_POS)

#define GPTB_GLDPRD_POS		(7)
#define GPTB_GLDPRD_MSK		(0x7 << GPTB_GLDPRD_POS)

#define GPTB_GLDCNT_POS		(10)
#define GPTB_GLDCNT_MSK		(0x7 << GPTB_GLDCNT_POS)

///GLDCFG
#define GPTB_LD_PRDR_POS 	(0)
#define GPTB_LD_PRDR_MSK	(0x1 << GPTB_LD_PRDR_POS)
#define GPTB_LD_CMPA_POS 	(1)
#define GPTB_LD_CMPA_MSK	(0x1 << GPTB_LD_CMPA_POS)
#define GPTB_LD_CMPB_POS 	(2)
#define GPTB_LD_CMPB_MSK	(0x1 << GPTB_LD_CMPB_POS)
#define GPTB_LD_DBDTR_POS 	(5)
#define GPTB_LD_DBDTR_MSK	(0x1 << GPTB_LD_DBDTR_POS)
#define GPTB_LD_DBDTF_POS 	(6)
#define GPTB_LD_DBDTF_MSK	(0x1 << GPTB_LD_DBDTF_POS)
#define GPTB_LD_DBCR_POS 	(7)
#define GPTB_LD_DBCR_MSK	(0x1 << GPTB_LD_DBCR_POS)
#define GPTB_LD_AQCR1_POS 	(8)
#define GPTB_LD_AQCR1_MSK	(0x1 << GPTB_LD_AQCR1_POS)
#define GPTB_LD_AQCR2_POS 	(9)
#define GPTB_LD_AQCR2_MSK	(0x1 << GPTB_LD_AQCR2_POS)
#define GPTB_LD_AQCSF_POS 	(12)
#define GPTB_LD_AQCSF_MSK	(0x1 << GPTB_LD_AQCSF_POS)
#define GPTB_LD_EMOSR_POS 	(13)
#define GPTB_LD_EMOSR_MSK	(0x1 << GPTB_LD_EMOSR_POS)
typedef enum {	
	GPTB_GLD_PRDR  = 0,
	GPTB_GLD_CMPA  ,
	GPTB_GLD_CMPB  ,
	GPTB_GLD_DBDTR = 5,
	GPTB_GLD_DBDTF,
	GPTB_GLD_DBCR,
	GPTB_GLD_AQCR1,
	GPTB_GLD_AQCR2,
	GPTB_GLD_AQCSF = 12,
	GPTB_GLD_EMOSR
}gptb_gldcfg_e;

typedef enum{
	GPTB_LD_NOTGLD = 0,
	GPTB_LD_GLD
}gptb_gldmd_e;

///GLDCR2
#define GPTB_OSREARM_EN	(0x1)
#define GPTB_SW_GLD		(0x2)

///PHSR
#define GPTB_PHSR_POS	(0)
#define GPTB_PHSR_MSK	(0xffff)
#define GPTB_PHSDIR_POS	(31)
#define GPTB_PHSDIR_MSK	(0x1 << GPTB_PHSDIR_POS)
typedef enum{
	GPTB_DIR_DN = 0,
	GPTB_DIR_UP
}gptb_phsdir_e;

//CMPA/B
#define GPTB_CMPDATA_MSK	(0xffff)	
#define GPTB_CMPDATA_OVWRT  (0x1 << 31)
typedef enum{
    GPTB_CMPA=0,
	GPTB_CMPB,
}gptb_comp_e;

//CMPLDR
#define GPTB_CMP_LD_POS(n)	(n)
#define GPTB_CMP_LD_MSK(n)  (0x1 << GPTB_CMP_LD_POS(n))

#define GPTB_CMPA_LD_POS	(0)
#define GPTB_CMPA_LD_MSK    (0x1 << GPTB_CMPA_LD_POS)
#define GPTB_CMPB_LD_POS	(1)
#define GPTB_CMPB_LD_MSK    (0x1 << GPTB_CMPB_LD_POS)

typedef enum {
	GPTB_LDMD_SHDW = 0,
	GPTB_LDMD_IMM
}gptb_ldmd_e;

#define GPTB_CMP_LDTIME_POS(n)	(4+ ((n)*3))
#define GPTB_CMP_LDTIME_MSK(n)	(0x7 << GPTB_CMP_LDTIME_POS(n))

#define GPTB_CMPA_LDTIME_POS	(4)
#define GPTB_CMPA_LDTIME_MSK	(0x7 << GPTB_CMPA_LDTIME_POS)
#define GPTB_CMPB_LDTIME_POS	(7)
#define GPTB_CMPB_LDTIME_MSK	(0x7 << GPTB_CMPB_LDTIME_POS)

typedef enum{
	GPTB_LDCMP_NEVER= 0,
	GPTB_LDCMP_ZRO ,
	GPTB_LDCMP_PRD,

	GPTB_LDCMP_LD_SYNC=4,
}gptb_shdwldmd_e;

#define GPTB_SHDWAFULL 	(0x1 <<20)
#define GPTB_SHDWBFULL 	(0x1 <<21)

//AQLDR
typedef enum{
    GPTB_CHANNEL1=1,
	GPTB_CHANNEL2,
}gptb_channel_e;

#define GPTB_AQCR1_SHDWEN_POS	(0)
#define GPTB_AQCR1_SHDWEN_MSK	(0x1)
#define GPTB_AQCR2_SHDWEN_POS	(1)
#define GPTB_AQCR2_SHDWEN_MSK	(0x1 << GPTB_AQCR2_SHDWEN_POS)

#define GPTB_LDAMD_POS		(2)
#define GPTB_LDAMD_MSK		(0x7 << GPTB_LDAMD_POS)
#define GPTB_LDBMD_POS		(5)
#define GPTB_LDBMD_MSK		(0x7 << GPTB_LDBMD_POS)

//AQCR1/2
#define GPTB_ACT_ZRO_POS	(0)
#define GPTB_ACT_ZRO_MSK	(0x3 << GPTB_ACT_ZRO_POS)
#define GPTB_ACT_PRD_POS	(2)
#define GPTB_ACT_PRD_MSK	(0x3 << GPTB_ACT_PRD_POS)
#define GPTB_ACT_C1U_POS	(4)
#define GPTB_ACT_C1U_MSK	(0x3 << GPTB_ACT_C1U_POS)
#define GPTB_ACT_C1D_POS	(6)
#define GPTB_ACT_C1D_MSK	(0x3 << GPTB_ACT_C1D_POS)
#define GPTB_ACT_C2U_POS	(8)
#define GPTB_ACT_C2U_MSK	(0x3 << GPTB_ACT_C2U_POS)
#define GPTB_ACT_C2D_POS	(10)
#define GPTB_ACT_C2D_MSK	(0x3 << GPTB_ACT_C2D_POS)
#define GPTB_ACT_T1U_POS	(12)
#define GPTB_ACT_T1U_MSK	(0x3 << GPTB_ACT_T1U_POS)
#define GPTB_ACT_T1D_POS	(14)
#define GPTB_ACT_T1D_MSK	(0x3 << GPTB_ACT_T1D_POS)
#define GPTB_ACT_T2U_POS	(16)
#define GPTB_ACT_T2U_MSK	(0x3 << GPTB_ACT_T2U_POS)
#define GPTB_ACT_T2D_POS	(18)
#define GPTB_ACT_T2D_MSK	(0x3 << GPTB_ACT_T2D_POS)

#define GPTB_C1SEL_POS	(20)
#define GPTB_C1SEL_MSK	(0x3<<GPTB_C1SEL_POS)
#define GPTB_C2SEL_POS	(22)
#define GPTB_C2SEL_MSK	(0x3<<GPTB_C2SEL_POS)

//AQOSF
#define GPTB_OSTSFA		(1)
#define GPTB_ACTA_POS	(1)
#define GPTB_ACTA_MSK	(0x3 << GPTB_ACTA_POS)
#define GPTB_OSTSFB		(0x1 << 4)
#define GPTB_ACTB_POS	(5)
#define GPTB_ACTB_MSK	(0x3 << GPTB_ACTB_POS)
#define GPTB_AQCSF_LDMD_POS (16)
#define GPTB_AQCSF_LDMD_MSK (0x3 << GPTB_AQCSF_LDMD_POS)	
typedef enum {
	GPTB_NA = 0,
	GPTB_LO,
	GPTB_HI,
	GPTB_TG	
}gptb_action_e;

//AQCSF
#define GPTB_CSFA_POS	(0)
#define GPTB_CSFA_MSK	(0x3)
#define GPTB_CSFB_POS	(2)
#define GPTB_CSFB_MSK	(0x3 << GPTB_CSFB_POS)
typedef enum {
	GPTB_LOAD_NOW=0,
	GPTB_LOAD_ZRO,
	GPTB_LOAD_PRD,
	GPTB_LOAD_ZRO_PRD
}gptb_aqcsf_ldmd_e;

typedef enum {
	GPTB_NONE=0,
	GPTB_L,
	GPTB_H,
	GPTB_NONE1
}gptb_aqcsf_act_e;

//DBLDR  
#define GPTB_DBLDR_SHDWCR_POS              0                                              /*!< GPTB DBLDR: SHDWCR Position */
#define GPTB_DBLDR_SHDWCR_MSK              (0x1UL << GPTB_DBLDR_SHDWCR_POS)               /*!< GPTB DBLDR: SHDWCR Mask */
#define GPTB_DBLDR_LDCRMODE_POS            1                                              /*!< GPTB DBLDR: LDCRMODE Position */
#define GPTB_DBLDR_LDCRMODE_MSK            (0x3UL << GPTB_DBLDR_LDCRMODE_POS)             /*!< GPTB DBLDR: LDCRMODE Mask */
#define GPTB_DBLDR_SHDWDTR_POS             3                                              /*!< GPTB DBLDR: SHDWDTR Position */
#define GPTB_DBLDR_SHDWDTR_MSK             (0x1UL << GPTB_DBLDR_SHDWDTR_POS)              /*!< GPTB DBLDR: SHDWDTR Mask */
#define GPTB_DBLDR_LDDTRMD_POS             4                                              /*!< GPTB DBLDR: LDDTRMD Position */
#define GPTB_DBLDR_LDDTRMD_MSK             (0x3UL << GPTB_DBLDR_LDDTRMD_POS)              /*!< GPTB DBLDR: LDDTRMD Mask */
#define GPTB_DBLDR_SHDWDTF_POS             6                                              /*!< GPTB DBLDR: SHDWDTF Position */
#define GPTB_DBLDR_SHDWDTF_MSK             (0x1UL << GPTB_DBLDR_SHDWDTF_POS)              /*!< GPTB DBLDR: SHDWDTF Mask */
#define GPTB_DBLDR_LDDTFMD_POS             7                                              /*!< GPTB DBLDR: LDDTFMD Position */
#define GPTB_DBLDR_LDDTFMD_MSK             (0x3UL << GPTB_DBLDR_LDDTFMD_POS)              /*!< GPTB DBLDR: LDDTFMD Mask */
#define GPTB_DBLDR_SHDWPSC_POS             9                                              /*!< GPTB DBLDR: SHDWPSC Position */
#define GPTB_DBLDR_SHDWPSC_MSK             (0x1UL << GPTB_DBLDR_SHDWPSC_POS)              /*!< GPTB DBLDR: SHDWPSC Mask */
#define GPTB_DBLDR_LDPSCMD_POS             10                                             /*!< GPTB DBLDR: LDPSCMD Position */
#define GPTB_DBLDR_LDPSCMD_MSK             (0x3UL << GPTB_DBLDR_LDPSCMD_POS)              /*!< GPTB DBLDR: LDPSCMD Mask */
typedef enum{
	GPTB_DBLDR_NEVER= 0,
	GPTB_DBLDR_ZRO ,
	GPTB_DBLDR_PRD,
	GPTB_DBLDR_ZRO_PRD,
}gptb_dbldr_e;

typedef enum{
	GPTB_DB_SHDW_IMMEDIATE =0,
	GPTB_DB_SHDW_SHADOW      
}gptb_dbldmd_e;

typedef enum{
	GPTB_DB_LDMD_NEVER = 0,
	GPTB_DB_LDMD_ZRO,
	GPTB_DB_LDMD_PRD,
	GPTB_DB_LDMD_ZRO_PRD	
}gptb_shdwdbldmd_e;

//DBCR
#define GPTB_CHA_OUTSEL_POS			(0)
#define GPTB_CHA_OUTSEL_MSK			(0x3) 
#define GPTB_CHA_POL_POS			(2)
#define GPTB_CHA_POL_MSK			(0x3 << GPTB_CHA_POL_POS)
#define GPTB_CHA_INSEL_POS			(4)
#define GPTB_CHA_INSEL_MSK			(0x3 << GPTB_CHA_INSEL_POS)
#define GPTB_CHA_OUTSWAP_POS		(6)
#define GPTB_CHA_OUTSWAP_MSK		(0x3 << GPTB_CHA_OUTSWAP_POS)
#define GPTB_DCKSEL_POS      		(24)                                        /*!< GPTB DBCR: DCKSEL Position */
#define GPTB_DCKSEL_MSK     		(0x1 << GPTB_DCKSEL_POS)               		/*!< GPTB DBCR: DCKSEL Mask */
#define GPTB_CHA_DEDB_POS			(25)
#define GPTB_CHA_DEDB_MSK			(0x1 << GPTB_CHA_DEDB_POS)

//DPSCR
#define GPTB_DPSCR_DPSC_POS          0                                          /*!< GPTB DPSCR: DPSC Position */
#define GPTB_DPSCR_DPSC_MSK          (0xFFFFUL << GPTB_DPSCR_DPSC_POS)          /*!< GPTB DPSCR: DPSC Mask */

//DBDTR
#define GPTB_DBDTR_DTR_POS           0                                          /*!< GPTB DBDTR: DTR Position */
#define GPTB_DBDTR_DTR_MSK           (0xFFFFUL << GPTB_DBDTR_DTR_POS)           /*!< GPTB DBDTR: DTR Mask */

//DBDTF
#define GPTB_DBDTF_DTF_POS           0                                          /*!< GPTB DBDTF: DTF Position */
#define GPTB_DBDTF_DTF_MSK           (0xFFFFUL << GPTB_DBDTF_DTF_POS)           /*!< GPTB DBDTF: DTF Mask */

//EMSRC
#define GPTB_SEL_POS_EP(n)	((n) << 2)
#define GPTB_SEL_MSK_EP(n)	(0xf << GPTB_SEL_POS_EP(n))
typedef enum{
	GPTB_EP_0 = 0,
	GPTB_EP_1,
	GPTB_EP_2,
	GPTB_EP_3,
}gptb_ep_e;

//EMSRC2
#define GPTB_ORL0_POS	(0)
#define GPTB_ORL0_MSK	(0xff)
#define GPTB_ORL1_POS	(16)
#define GPTB_ORL1_MSK	(0xff << GPTB_ORL1_POS)
#define GPTB_ORL0_NO_EPI(n)		(0x1<< n)
#define GPTB_ORL1_NO_EPI(n)		(0x1<<(n+16))
#define GPTB_EPPACE0_POS	(8)
#define GPTB_EPPACE0_MSK	(0xf << GPTB_EPPACE0_POS)
#define GPTB_EPPACE1_POS	(12)
#define GPTB_EPPACE1_MSK	(0xf << GPTB_EPPACE1_POS)

///EMPOL
#define GPTB_POL_POS_EBI(n)	(n)
#define GPTB_POL_MSK_EBI(n)	(0x1 << GPTB_POL_POS_EBI(n))

//EMECR
#define GPTB_EMECR_MEM_FAULT_POS           30                                             /*!< GPTB EMECR: MEM_FAULT Position */
#define GPTB_EMECR_MEM_FAULT_MSK           (0x1UL << GPTB_EMECR_MEM_FAULT_POS)            /*!< GPTB EMECR: MEM_FAULT Mask */
#define GPTB_EMECR_EOM_FAULT_POS           29                                             /*!< GPTB EMECR: EOM_FAULT Position */
#define GPTB_EMECR_EOM_FAULT_MSK           (0x1UL << GPTB_EMECR_EOM_FAULT_POS)            /*!< GPTB EMECR: EOM_FAULT Mask */
#define GPTB_EMECR_CPU_FAULT_POS           28                                             /*!< GPTB EMECR: CPU_FAULT Position */
#define GPTB_EMECR_CPU_FAULT_MSK           (0x1UL << GPTB_EMECR_CPU_FAULT_POS)            /*!< GPTB EMECR: CPU_FAULT Mask */
#define GPTB_EMECR_EMASYNC_POS             26                                             /*!< GPTB EMECR: EMASYNC Position */
#define GPTB_EMECR_EMASYNC_MSK             (0x1UL << GPTB_EMECR_EMASYNC_POS)              /*!< GPTB EMECR: EMASYNC Mask */
#define GPTB_EMECR_SLCLRMD_POS             24                                             /*!< GPTB EMECR: SLCLRMD Position */
#define GPTB_EMECR_SLCLRMD_MSK             (0x3UL << GPTB_EMECR_SLCLRMD_POS)              /*!< GPTB EMECR: SLCLRMD Mask */
#define GPTB_EMECR_OSRLDMD_POS             22                                             /*!< GPTB EMECR: OSRLDMD Position */
#define GPTB_EMECR_OSRLDMD_MSK             (0x3UL << GPTB_EMECR_OSRLDMD_POS)              /*!< GPTB EMECR: OSRLDMD Mask */
#define GPTB_EMECR_OSRSHDW_POS             21                                             /*!< GPTB EMECR: OSRSHDW Position */
#define GPTB_EMECR_OSRSHDW_MSK             (0x1UL << GPTB_EMECR_OSRSHDW_POS)              /*!< GPTB EMECR: OSRSHDW Mask */

#define GPTB_LCKMD_POS_EP(n)	((n) << 1)
#define GPTB_LCKMD_MSK_EP(n)	(0x3 << GPTB_LCKMD_POS_EP(n))

typedef enum {
	GPTB_EM_INT_EP0  = 0x1,
	GPTB_EM_INT_EP1  = 0x1 << 1,
	GPTB_EM_INT_EP2  = 0x1 << 2,
	GPTB_EM_INT_EP3  = 0x1 << 3,
	GPTB_EM_INT_CPUF = 0x1 << 8,
	GPTB_EM_INT_MEMF = 0x1 << 9,
	GPTB_EM_INT_EOMF = 0x1 << 10,
	GPTB_EM_INT_ALL  = 0xFFFF
}gptb_emint_e;

#define GPTB_EMOSR_SHDWEN_POS (21)
#define GPTB_EMSOR_SHDWEN_MSK  (0x1 << GPTB_EMOSR_SHDWEN_POS)

#define GPTB_OSRLDMD_POS      22                                            
#define GPTB_OSRLDMD_MSK     (0x3UL << GPTB_OSRLDMD_POS)              

#define GPTB_SLCK_CLRMD_POS	(24)
#define GPTB_SLCK_CLRMD_MSK	(0x3 << GPTB_SLCK_CLRMD_POS)

//EMOSR
#define GPTB_EMOSR_EM_COAY_POS             8                                              /*!< GPTB EMOSR: EM_COAY Position */
#define GPTB_EMOSR_EM_COAY_MSK             (0x3UL << GPTB_EMOSR_EM_COAY_POS)              /*!< GPTB EMOSR: EM_COAY Mask */
#define GPTB_EMOSR_EM_COBX_POS             2                                              /*!< GPTB EMOSR: EM_COBX Position */
#define GPTB_EMOSR_EM_COBX_MSK             (0x3UL << GPTB_EMOSR_EM_COBX_POS)              /*!< GPTB EMOSR: EM_COBX Mask */
#define GPTB_EMOSR_EM_COAX_POS             0                                              /*!< GPTB EMOSR: EM_COAX Position */
#define GPTB_EMOSR_EM_COAX_MSK             (0x3UL << GPTB_EMOSR_EM_COAX_POS)              /*!< GPTB EMOSR: EM_COAX Mask */

//EMSLSR
#define GPTB_EMSLSR_EP3_POS                3                                              /*!< GPTB EMSLSR: EP3 Position */
#define GPTB_EMSLSR_EP3_MSK                (0x1UL << GPTB_EMSLSR_EP3_POS)                 /*!< GPTB EMSLSR: EP3 Mask */
#define GPTB_EMSLSR_EP2_POS                2                                              /*!< GPTB EMSLSR: EP2 Position */
#define GPTB_EMSLSR_EP2_MSK                (0x1UL << GPTB_EMSLSR_EP2_POS)                 /*!< GPTB EMSLSR: EP2 Mask */
#define GPTB_EMSLSR_EP1_POS                1                                              /*!< GPTB EMSLSR: EP1 Position */
#define GPTB_EMSLSR_EP1_MSK                (0x1UL << GPTB_EMSLSR_EP1_POS)                 /*!< GPTB EMSLSR: EP1 Mask */
#define GPTB_EMSLSR_EP0_POS                0                                              /*!< GPTB EMSLSR: EP0 Position */
#define GPTB_EMSLSR_EP0_MSK                (0x1UL << GPTB_EMSLSR_EP0_POS)                 /*!< GPTB EMSLSR: EP0 Mask */
//EMSLCLR
#define GPTB_EMSLCLR_EP3_POS               3                                              /*!< GPTB EMSLCLR: EP3 Position */
#define GPTB_EMSLCLR_EP3_MSK               (0x1UL << GPTB_EMSLCLR_EP3_POS)                /*!< GPTB EMSLCLR: EP3 Mask */
#define GPTB_EMSLCLR_EP2_POS               2                                              /*!< GPTB EMSLCLR: EP2 Position */
#define GPTB_EMSLCLR_EP2_MSK               (0x1UL << GPTB_EMSLCLR_EP2_POS)                /*!< GPTB EMSLCLR: EP2 Mask */
#define GPTB_EMSLCLR_EP1_POS               1                                              /*!< GPTB EMSLCLR: EP1 Position */
#define GPTB_EMSLCLR_EP1_MSK               (0x1UL << GPTB_EMSLCLR_EP1_POS)                /*!< GPTB EMSLCLR: EP1 Mask */
#define GPTB_EMSLCLR_EP0_POS               0                                              /*!< GPTB EMSLCLR: EP0 Position */
#define GPTB_EMSLCLR_EP0_MSK               (0x1UL << GPTB_EMSLCLR_EP0_POS)                /*!< GPTB EMSLCLR: EP0 Mask */
//EMHLSR
#define GPTB_EMHLSR_EOM_FAULT_POS          10                                             /*!< GPTB EMHLSR: EOM_FAULT Position */
#define GPTB_EMHLSR_EOM_FAULT_MSK          (0x1UL << GPTB_EMHLSR_EOM_FAULT_POS)           /*!< GPTB EMHLSR: EOM_FAULT Mask */
#define GPTB_EMHLSR_MEM_FAULT_POS          9                                              /*!< GPTB EMHLSR: MEM_FAULT Position */
#define GPTB_EMHLSR_MEM_FAULT_MSK          (0x1UL << GPTB_EMHLSR_MEM_FAULT_POS)           /*!< GPTB EMHLSR: MEM_FAULT Mask */
#define GPTB_EMHLSR_CPU_FAULT_POS          8                                              /*!< GPTB EMHLSR: CPU_FAULT Position */
#define GPTB_EMHLSR_CPU_FAULT_MSK          (0x1UL << GPTB_EMHLSR_CPU_FAULT_POS)           /*!< GPTB EMHLSR: CPU_FAULT Mask */
#define GPTB_EMHLSR_EP3_POS                3                                              /*!< GPTB EMHLSR: EP3 Position */
#define GPTB_EMHLSR_EP3_MSK                (0x1UL << GPTB_EMHLSR_EP3_POS)                 /*!< GPTB EMHLSR: EP3 Mask */
#define GPTB_EMHLSR_EP2_POS                2                                              /*!< GPTB EMHLSR: EP2 Position */
#define GPTB_EMHLSR_EP2_MSK                (0x1UL << GPTB_EMHLSR_EP2_POS)                 /*!< GPTB EMHLSR: EP2 Mask */
#define GPTB_EMHLSR_EP1_POS                1                                              /*!< GPTB EMHLSR: EP1 Position */
#define GPTB_EMHLSR_EP1_MSK                (0x1UL << GPTB_EMHLSR_EP1_POS)                 /*!< GPTB EMHLSR: EP1 Mask */
#define GPTB_EMHLSR_EP0_POS                0                                              /*!< GPTB EMHLSR: EP0 Position */
#define GPTB_EMHLSR_EP0_MSK                (0x1UL << GPTB_EMHLSR_EP0_POS)                 /*!< GPTB EMHLSR: EP0 Mask */
//EMHLCLR
#define GPTB_EMHLCLR_MEM_FAULT_POS         10                                             /*!< GPTB EMHLCLR: MEM_FAULT Position */
#define GPTB_EMHLCLR_MEM_FAULT_MSK         (0x1UL << GPTB_EMHLCLR_MEM_FAULT_POS)          /*!< GPTB EMHLCLR: MEM_FAULT Mask */
#define GPTB_EMHLCLR_EOM_FAULT_POS         9                                              /*!< GPTB EMHLCLR: EOM_FAULT Position */
#define GPTB_EMHLCLR_EOM_FAULT_MSK         (0x1UL << GPTB_EMHLCLR_EOM_FAULT_POS)          /*!< GPTB EMHLCLR: EOM_FAULT Mask */
#define GPTB_EMHLCLR_CPU_FAULT_POS         8                                              /*!< GPTB EMHLCLR: CPU_FAULT Position */
#define GPTB_EMHLCLR_CPU_FAULT_MSK         (0x1UL << GPTB_EMHLCLR_CPU_FAULT_POS)          /*!< GPTB EMHLCLR: CPU_FAULT Mask */
#define GPTB_EMHLCLR_EP3_POS               3                                              /*!< GPTB EMHLCLR: EP3 Position */
#define GPTB_EMHLCLR_EP3_MSK               (0x1UL << GPTB_EMHLCLR_EP3_POS)                /*!< GPTB EMHLCLR: EP3 Mask */
#define GPTB_EMHLCLR_EP2_POS               2                                              /*!< GPTB EMHLCLR: EP2 Position */
#define GPTB_EMHLCLR_EP2_MSK               (0x1UL << GPTB_EMHLCLR_EP2_POS)                /*!< GPTB EMHLCLR: EP2 Mask */
#define GPTB_EMHLCLR_EP1_POS               1                                              /*!< GPTB EMHLCLR: EP1 Position */
#define GPTB_EMHLCLR_EP1_MSK               (0x1UL << GPTB_EMHLCLR_EP1_POS)                /*!< GPTB EMHLCLR: EP1 Mask */
#define GPTB_EMHLCLR_EP0_POS               0                                              /*!< GPTB EMHLCLR: EP0 Position */
#define GPTB_EMHLCLR_EP0_MSK               (0x1UL << GPTB_EMHLCLR_EP0_POS)                /*!< GPTB EMHLCLR: EP0 Mask */
//_EMRISR
#define GPTB_EMRISR_MEM_FAULT_POS          10                                             /*!< GPTB EMRISR: MEM_FAULT Position */
#define GPTB_EMRISR_MEM_FAULT_MSK          (0x1UL << GPTB_EMRISR_MEM_FAULT_POS)           /*!< GPTB EMRISR: MEM_FAULT Mask */
#define GPTB_EMRISR_EOM_FAULT_POS          9                                              /*!< GPTB EMRISR: EOM_FAULT Position */
#define GPTB_EMRISR_EOM_FAULT_MSK          (0x1UL << GPTB_EMRISR_EOM_FAULT_POS)           /*!< GPTB EMRISR: EOM_FAULT Mask */
#define GPTB_EMRISR_CPU_FAULT_POS          8                                              /*!< GPTB EMRISR: CPU_FAULT Position */
#define GPTB_EMRISR_CPU_FAULT_MSK          (0x1UL << GPTB_EMRISR_CPU_FAULT_POS)           /*!< GPTB EMRISR: CPU_FAULT Mask */
#define GPTB_EMRISR_EP3_POS                3                                              /*!< GPTB EMRISR: EP3 Position */
#define GPTB_EMRISR_EP3_MSK                (0x1UL << GPTB_EMRISR_EP3_POS)                 /*!< GPTB EMRISR: EP3 Mask */
#define GPTB_EMRISR_EP2_POS                2                                              /*!< GPTB EMRISR: EP2 Position */
#define GPTB_EMRISR_EP2_MSK                (0x1UL << GPTB_EMRISR_EP2_POS)                 /*!< GPTB EMRISR: EP2 Mask */
#define GPTB_EMRISR_EP1_POS                1                                              /*!< GPTB EMRISR: EP1 Position */
#define GPTB_EMRISR_EP1_MSK                (0x1UL << GPTB_EMRISR_EP1_POS)                 /*!< GPTB EMRISR: EP1 Mask */
#define GPTB_EMRISR_EP0_POS                0                                              /*!< GPTB EMRISR: EP0 Position */
#define GPTB_EMRISR_EP0_MSK                (0x1UL << GPTB_EMRISR_EP0_POS)                 /*!< GPTB EMRISR: EP0 Mask */
//EMMISR
#define GPTB_EMMISR_EOM_FAULT_POS          10                                             /*!< GPTB EMMISR: EOM_FAULT Position */
#define GPTB_EMMISR_EOM_FAULT_MSK          (0x1UL << GPTB_EMMISR_EOM_FAULT_POS)           /*!< GPTB EMMISR: EOM_FAULT Mask */
#define GPTB_EMMISR_MEM_FAULT_POS          9                                              /*!< GPTB EMMISR: MEM_FAULT Position */
#define GPTB_EMMISR_MEM_FAULT_MSK          (0x1UL << GPTB_EMMISR_MEM_FAULT_POS)           /*!< GPTB EMMISR: MEM_FAULT Mask */
#define GPTB_EMMISR_CPU_FAULT_POS          8                                              /*!< GPTB EMMISR: CPU_FAULT Position */
#define GPTB_EMMISR_CPU_FAULT_MSK          (0x1UL << GPTB_EMMISR_CPU_FAULT_POS)           /*!< GPTB EMMISR: CPU_FAULT Mask */
#define GPTB_EMMISR_EP3_POS                3                                              /*!< GPTB EMMISR: EP3 Position */
#define GPTB_EMMISR_EP3_MSK                (0x1UL << GPTB_EMMISR_EP3_POS)                 /*!< GPTB EMMISR: EP3 Mask */
#define GPTB_EMMISR_EP2_POS                2                                              /*!< GPTB EMMISR: EP2 Position */
#define GPTB_EMMISR_EP2_MSK                (0x1UL << GPTB_EMMISR_EP2_POS)                 /*!< GPTB EMMISR: EP2 Mask */
#define GPTB_EMMISR_EP1_POS                1                                              /*!< GPTB EMMISR: EP1 Position */
#define GPTB_EMMISR_EP1_MSK                (0x1UL << GPTB_EMMISR_EP1_POS)                 /*!< GPTB EMMISR: EP1 Mask */
#define GPTB_EMMISR_EP0_POS                0                                              /*!< GPTB EMMISR: EP0 Position */
#define GPTB_EMMISR_EP0_MSK                (0x1UL << GPTB_EMMISR_EP0_POS)                 /*!< GPTB EMMISR: EP0 Mask */
//_EMIMCR
#define GPTB_EMIMCR_MEM_POS                10                                             /*!< GPTB EMIMCR: MEM Position */
#define GPTB_EMIMCR_MEM_MSK                (0x1UL << GPTB_EMIMCR_MEM_POS)                 /*!< GPTB EMIMCR: MEM Mask */
#define GPTB_EMIMCR_EOM_POS                9                                              /*!< GPTB EMIMCR: EOM Position */
#define GPTB_EMIMCR_EOM_MSK                (0x1UL << GPTB_EMIMCR_EOM_POS)                 /*!< GPTB EMIMCR: EOM Mask */
#define GPTB_EMIMCR_CPU_FAULT_POS          8                                              /*!< GPTB EMIMCR: CPU_FAULT Position */
#define GPTB_EMIMCR_CPU_FAULT_MSK          (0x1UL << GPTB_EMIMCR_CPU_FAULT_POS)           /*!< GPTB EMIMCR: CPU_FAULT Mask */
#define GPTB_EMIMCR_EP3_POS                3                                              /*!< GPTB EMIMCR: EP3 Position */
#define GPTB_EMIMCR_EP3_MSK                (0x1UL << GPTB_EMIMCR_EP3_POS)                 /*!< GPTB EMIMCR: EP3 Mask */
#define GPTB_EMIMCR_EP2_POS                2                                              /*!< GPTB EMIMCR: EP2 Position */
#define GPTB_EMIMCR_EP2_MSK                (0x1UL << GPTB_EMIMCR_EP2_POS)                 /*!< GPTB EMIMCR: EP2 Mask */
#define GPTB_EMIMCR_EP1_POS                1                                              /*!< GPTB EMIMCR: EP1 Position */
#define GPTB_EMIMCR_EP1_MSK                (0x1UL << GPTB_EMIMCR_EP1_POS)                 /*!< GPTB EMIMCR: EP1 Mask */
#define GPTB_EMIMCR_EP0_POS                0                                              /*!< GPTB EMIMCR: EP0 Position */
#define GPTB_EMIMCR_EP0_MSK                (0x1UL << GPTB_EMIMCR_EP0_POS)                 /*!< GPTB EMIMCR: EP0 Mask */
//EMICR
#define GPTB_EMICR_MEM_POS                 10                                             /*!< GPTB EMICR: MEM Position */
#define GPTB_EMICR_MEM_MSK                 (0x1UL << GPTB_EMICR_MEM_POS)                  /*!< GPTB EMICR: MEM Mask */
#define GPTB_EMICR_EOM_FAULT_POS           9                                              /*!< GPTB EMICR: EOM_FAULT Position */
#define GPTB_EMICR_EOM_FAULT_MSK           (0x1UL << GPTB_EMICR_EOM_FAULT_POS)            /*!< GPTB EMICR: EOM_FAULT Mask */
#define GPTB_EMICR_CPU_FAULT_POS           8                                              /*!< GPTB EMICR: CPU_FAULT Position */
#define GPTB_EMICR_CPU_FAULT_MSK           (0x1UL << GPTB_EMICR_CPU_FAULT_POS)            /*!< GPTB EMICR: CPU_FAULT Mask */
#define GPTB_EMICR_EP3_POS                 3                                              /*!< GPTB EMICR: EP3 Position */
#define GPTB_EMICR_EP3_MSK                 (0x1UL << GPTB_EMICR_EP3_POS)                  /*!< GPTB EMICR: EP3 Mask */
#define GPTB_EMICR_EP2_POS                 2                                              /*!< GPTB EMICR: EP2 Position */
#define GPTB_EMICR_EP2_MSK                 (0x1UL << GPTB_EMICR_EP2_POS)                  /*!< GPTB EMICR: EP2 Mask */
#define GPTB_EMICR_EP1_POS                 1                                              /*!< GPTB EMICR: EP1 Position */
#define GPTB_EMICR_EP1_MSK                 (0x1UL << GPTB_EMICR_EP1_POS)                  /*!< GPTB EMICR: EP1 Mask */
#define GPTB_EMICR_EP0_POS                 0                                              /*!< GPTB EMICR: EP0 Position */
#define GPTB_EMICR_EP0_MSK                 (0x1UL << GPTB_EMICR_EP0_POS)                  /*!< GPTB EMICR: EP0 Mask */

///TRGFTCR
#define GPTB_FLTSRC_POS	    (0)
#define GPTB_FLTSRC_MSK	    (0x7)
#define GPTB_FLTBLKINV_POS  (4)
#define GPTB_FLTBLKINV_MSK	(0x1 << GPTB_FLTBLKINV_POS)
#define GPTB_ALIGNMD_POS	(5)
#define GPTB_ALIGNMD_MSK	(0x3 << GPTB_ALIGNMD_POS)

#define GPTB_CROSSMD_POS	(7)
#define GPTB_CROSSMD_MSK	(0x1 << GPTB_CROSSMD_POS)

///TRGFTWR
#define GPTB_FLT_OFFSET_POS	(0)
#define GPTB_FLT_OFFSET_MSK	(0xffff)
#define GPTB_FLT_WDW_POS	(16)
#define GPTB_FLT_WDW_MSK	(0xffff << GPTB_FLT_WDW_POS)

///EVTRG
#define GPTB_SEL_POS_TRG(n)	(n << 2)
#define GPTB_SEL_MSK_TRG(n)	(0xf << GPTB_SEL_POS_TRG(n))
typedef enum{
	GPTB_TRG01_DIS = 0,
	GPTB_TRG01_ZRO,
	GPTB_TRG01_PRD,
	GPTB_TRG01_ZRO_PRD,
	GPTB_TRG01_CMPA_R,
	GPTB_TRG01_CMPA_F,
	GPTB_TRG01_CMPB_R,
	GPTB_TRG01_CMPB_F,
	
	GPTB_TRG01_SYNC = 0xc,
	GPTB_TRG01_EP0,
	GPTB_TRG01_EP1,
	GPTB_TRG01_EP2
}gptb_trgsrc_e;

#define GPTB_INITEN_POS_CNT(n)	(16+n)
#define GPTB_INITEN_MSK_CNT(n)	(1 <<(16+n))
#define GPTB_OUTEN_POS_TRG(n)	(20+n)
#define GPTB_OUTEN_MSK_TRG(n)	(1 <<(20+n))
typedef enum{
	GPTB_TRGEV0		= 0,	
	GPTB_TRGEV1,				
}gptb_trgev_e;

#define GPTB_SWTRG_EV(n)		(n+24)

///EVPS
#define GPTB_PRD_POS_EV(n)	(n*4)
#define GPTB_PRD_MSK_EV(n)	(1 << (n*4))
#define GPTB_CNT_POS_EV(n)	(16 + n*4)	
#define GPTB_CNT_MSKS_EV(n)	(1 <<(16 + n*4))

///EVSWF
#define GPTB_SWF_EV(n)		(0x1 << n)

///EVCNTINIT
#define GPTB_CNT_INIT_POS_EV(n)		(n<<2)
#define GPTB_CNT_INIT_MSK_EV(n) 	(0xf << GPTB_CNT_INIT_POS_EV(n))
#define GPTB_CNT_INIT(val, n)   	((0xf & val) << GPTB_CNT_INIT_POS_EV(n))

//REGLK 
#define GPTB_PRDR_POS	(0)
#define GPTB_PRDR_MSK   (0xf << GPTB_PRDR_POS)
#define GPTB_CMPA_POS	(4)
#define GPTB_CMPA_MSK   (0xf << GPTB_CMPA_POS)
#define GPTB_CMPB_POS	(8)
#define GPTB_CMPB_MSK   (0xf << GPTB_CMPB_POS)
#define GPTB_GLD2_POS	(20)
#define GPTB_GLD2_MSK   (0xf << GPTB_GLD2_POS)	
#define GPTB_RSSR_POS	(24)
#define GPTB_RSSR_MSK   (0xf << GPTB_RSSR_POS)	

//REGLK2 
#define GPTB_EMSLCLR_POS   (0)
#define GPTB_EMSLCLR_MSK   (0xf << GPTB_EMSLCLR_POS)
#define GPTB_EMHLCLR_POS   (4)
#define GPTB_EMHLCLR_MSK   (0xf << GPTB_EMHLCLR_POS)
#define GPTB_EMICR_POS	   (8)
#define GPTB_EMICR_MSK     (0xf << GPTB_EMICR_POS)
#define GPTB_EMFRCR_POS	   (12)
#define GPTB_EMFRCR_MSK    (0xf << GPTB_EMFRCR_POS)	
#define GPTB_AQOSF_POS	   (16)
#define GPTB_AQOSF_MSK     (0xf << GPTB_AQOSF_POS)
#define GPTB_AQCSF_POS	   (20)
#define GPTB_AQCSF_MSK     (0xf << GPTB_AQCSF_POS)
	
/******************************************************************************
/// Interrupt Related
******************************************************************************/
typedef enum{
	GPTB_INT_TRGEV0 = 1 << 0,
	GPTB_INT_TRGEV1 = 1 << 1,
	GPTB_INT_TRGEV2 = 1 << 2,
	GPTB_INT_TRGEV3 = 1 << 3,
	GPTB_INT_CAPLD0 = 1 << 4,
	GPTB_INT_CAPLD1 = 1 << 5,
	GPTB_INT_CAPLD2 = 1 << 6,
	GPTB_INT_CAPLD3 = 1 << 7,
	GPTB_INT_CAU    = 1 << 8,
	GPTB_INT_CAD    = 1 << 9,
	GPTB_INT_CBU    = 1 << 10,
	GPTB_INT_CBD    = 1 << 11,
	GPTB_INT_PEND   = 1 << 16,	
	GPTB_INT_PRDMA  = 1 << 17,
	GPTB_INT_ZROMA  = 1 << 18,
	GPTB_INT_ALL    = 0xFFFFFFFF
}gptb_int_e;

#define GPTB_INT_EV(n)		(0x1 << n)
#define GPTB_INT_CAPLD(n)	(0x1 << (n+4))

///REGPROT
#define GPTB_REGPROT			(0xa55a << 16 | 0xc73a)

/******************************************************************************
********************* GPTBx Macro Definition Declaration **********************
******************************************************************************/
static inline void csp_gptb_wr_key(csp_gptb_t *ptGptbBase)
{
	ptGptbBase -> REGPROT = GPTB_REGPROT;
}

static inline void csp_gptb_clk_enable(csp_gptb_t *ptGptbBase)
{
	ptGptbBase -> CEDR |=  GPTB_CLKEN;
}
 
static inline void csp_gptb_dbg_enable(csp_gptb_t *ptGptbBase)
{
	ptGptbBase -> CEDR |= GPTB_DBGEN;
}

static inline void csp_gptb_dbg_disable(csp_gptb_t *ptGptbBase)
{
	ptGptbBase -> CEDR &= ~GPTB_DBGEN;
}

static inline void csp_gptb_set_clksrc(csp_gptb_t *ptGptbBase, gptb_clksrc_e eClkSrc)
{
	ptGptbBase -> CEDR = (ptGptbBase -> CEDR & (~GPTB_CKSRC_MSK)) | (eClkSrc << GPTB_CKSRC_POS);
}

static inline void csp_gptb_sw_rst(csp_gptb_t *ptGptbBase)
{  
	ptGptbBase -> REGPROT = GPTB_REGPROT;
	ptGptbBase -> RSSR |= GPTB_RESET;
}

static inline void csp_gptb_start(csp_gptb_t *ptGptbBase)
{
	ptGptbBase->RSSR |= GPTB_START ;
} 

static inline void csp_gptb_stop(csp_gptb_t *ptGptbBase)
{
	ptGptbBase->RSSR &= ~ GPTB_START ;
}

static inline void csp_gptb_set_pscr(csp_gptb_t *ptGptbBase, uint16_t hwVal)
{
	ptGptbBase -> PSCR = hwVal;
}

static inline void csp_gptb_set_cr(csp_gptb_t *ptGptbBase, uint32_t wCr)
{
	ptGptbBase->CR = wCr;
}

static inline void csp_gptb_set_work_mode(csp_gptb_t *ptGptbBase, gptb_workmode_e eWorkMode)
{
	ptGptbBase -> CR = (ptGptbBase -> CR &(~GPTB_MODE_MSK)) | eWorkMode << GPTB_MODE_POS;
}

static inline void csp_gptb_set_count_mode(csp_gptb_t *ptGptbBase, gptb_cntmd_e eCntMode)
{
	ptGptbBase -> CR = (ptGptbBase->CR & (~GPTB_CNTMD_MSK)) | (eCntMode << GPTB_CNTMD_POS);
}

static inline uint8_t csp_gptb_get_count_mode(csp_gptb_t *ptGptbBase)
{
	return (ptGptbBase->CR & GPTB_CNTMD_MSK);
}

static inline void csp_gptb_set_stopwrap(csp_gptb_t *ptGptbBase, uint8_t byCnt)
{
	ptGptbBase -> CR = (ptGptbBase -> CR & ~(GPTB_STOPWRAP_MSK)) | (byCnt << GPTB_STOPWRAP_POS);
}

static inline void csp_gptb_burst_enable(csp_gptb_t *ptGptbBase)
{
	ptGptbBase -> CR |= GPTB_BURST_EN;
}

static inline void csp_gptb_burst_disable(csp_gptb_t *ptGptbBase)
{
	ptGptbBase -> CR &= ~GPTB_BURST_EN;
}

static inline void csp_gptb_set_cgsrc(csp_gptb_t *ptGptbBase, gptb_cgsrc_e eCgSrc)
{
	ptGptbBase -> CR = (ptGptbBase -> CR & (~GPTB_CGSRC_MSK)) | (eCgSrc << GPTB_CGSRC_POS);
}

static inline void csp_gptb_flt_enable(csp_gptb_t *ptGptbBase)
{
	ptGptbBase -> CR |= GPTB_FLT_INIT;
}

static inline void csp_gptb_set_flt(csp_gptb_t *ptGptbBase, gptb_cgflt_e eCgFlt)
{
	ptGptbBase -> CR = (ptGptbBase -> CR & (~GPTB_CGFLT_MSK)) | (eCgFlt << GPTB_CGFLT_POS);
}

static inline void csp_gptb_set_start_mode(csp_gptb_t *ptGptbBase, gptb_startmode_e eStartMode)
{
	ptGptbBase -> CR = (ptGptbBase->CR & (~GPTB_STARTMODE_MSK)) | (eStartMode << GPTB_STARTMODE_POS);
}

static inline void csp_gptb_set_stop_status(csp_gptb_t *ptGptbBase, gptb_stpst_e eStpst) 
{
	ptGptbBase -> CR = (ptGptbBase->CR & (~GPTB_STPST_MSK))| (eStpst << GPTB_STPST_POS);
}

static inline void csp_gptb_set_run_mode(csp_gptb_t *ptGptbBase, gptb_runmd_e eRunmd)
{
	ptGptbBase -> CR = (ptGptbBase->CR & (~GPTB_RUNMD_MSK))| (eRunmd << GPTB_RUNMD_POS);
}

static inline void csp_gptb_capld_enable(csp_gptb_t *ptGptbBase)
{
	ptGptbBase -> CR |= GPTB_CAPLD_EN;
}

static inline void csp_gptb_capld_disable(csp_gptb_t *ptGptbBase)
{
	ptGptbBase -> CR &= ~GPTB_CAPLD_EN;
}

static inline void csp_gptb_set_prdld_mode(csp_gptb_t *ptGptbBase, gptb_ldprdr_e ePrdrld)
{
	ptGptbBase -> CR = (ptGptbBase->CR & ~(GPTB_PRDLD_MSK)) | (ePrdrld << GPTB_PRDLD_POS);
}

static inline void csp_gptb_sync_enable(csp_gptb_t *ptGptbBase, gptb_syncin_e eSyncIn)
{   
	ptGptbBase -> REGPROT = GPTB_REGPROT;
	ptGptbBase -> SYNCR |= GPTB_SYNCEN(eSyncIn);
}

static inline void csp_gptb_sync_disable(csp_gptb_t *ptGptbBase, gptb_syncin_e eSyncIn)
{   
	ptGptbBase -> REGPROT = GPTB_REGPROT;
	ptGptbBase -> SYNCR &= ~GPTB_SYNCEN(eSyncIn);
}

static inline void csp_gptb_set_sync_mode(csp_gptb_t *ptGptbBase, gptb_syncin_e eSyncIn, gptb_syncmd_e eMode)
{   
	ptGptbBase -> REGPROT = GPTB_REGPROT;
	ptGptbBase -> SYNCR = (ptGptbBase -> SYNCR & ~(GPTB_OSTMD_MSK(eSyncIn)) ) | eMode << GPTB_OSTMD_POS(eSyncIn);
}

static inline void csp_gptb_set_auto_rearm(csp_gptb_t *ptGptbBase, gptb_arearm_e eMode)
{   
	ptGptbBase -> REGPROT = GPTB_REGPROT;
	ptGptbBase -> SYNCR = (ptGptbBase -> SYNCR & ~(GPTB_AREARM_MSK)) | eMode << GPTB_AREARM_POS;
}

static inline void csp_gptb_sync_trg0sel(csp_gptb_t *ptGptbBase, gptb_syncin_e eSyncIn)
{   
	ptGptbBase -> REGPROT = GPTB_REGPROT;
	ptGptbBase -> SYNCR   =((ptGptbBase -> SYNCR & ~GPTB_TRGO0SEL_MSK) | (eSyncIn << GPTB_TRGO0SEL_POS));
}

static inline void csp_gptb_sync_trg1sel(csp_gptb_t *ptGptbBase, gptb_syncin_e eSyncIn)
{   
	ptGptbBase -> REGPROT = GPTB_REGPROT;
	ptGptbBase -> SYNCR   = (ptGptbBase -> SYNCR & ~GPTB_TRGO1SEL_MSK) | (eSyncIn << GPTB_TRGO1SEL_POS);
}

static inline void csp_gptb_sync_rearm(csp_gptb_t *ptGptbBase, gptb_syncin_e eSyncIn)
{   
	ptGptbBase -> REGPROT = GPTB_REGPROT;
	ptGptbBase -> SYNCR  |= GPTB_REARM(eSyncIn);
}

static inline void csp_gptb_set_gldcr(csp_gptb_t *ptGptbBase, uint32_t wVal)
{
	ptGptbBase -> GLDCR   =  wVal ;
}

static inline void  csp_gptb_set_gldcfg(csp_gptb_t *ptGptbBase, uint16_t hwMask, uint16_t hwVal)
{
	ptGptbBase -> GLDCFG = (ptGptbBase -> GLDCFG & ~(hwMask)) | hwVal;
}

static inline void csp_gptb_set_gldcr2(csp_gptb_t *ptGptbBase, uint32_t wVal)
{    
	ptGptbBase -> REGPROT = GPTB_REGPROT;
	ptGptbBase -> GLDCR2   |=  wVal ;
}

static inline void csp_gptb_set_prdr(csp_gptb_t *ptGptbBase, uint16_t hwVal)
{
	ptGptbBase -> PRDR = hwVal;
}

static inline uint16_t csp_gptb_get_prdr(csp_gptb_t *ptGptbBase)
{
	return (ptGptbBase -> PRDR);
}

static inline void csp_gptb_set_cmpa(csp_gptb_t *ptGptbBase, uint16_t hwVal)
{
	ptGptbBase -> CMPA = hwVal;
}

static inline uint16_t csp_gptb_get_cmpa(csp_gptb_t *ptGptbBase)
{
	return (ptGptbBase -> CMPA);
}

static inline void csp_gptb_set_cmpb(csp_gptb_t *ptGptbBase, uint16_t hwVal)
{
	ptGptbBase -> CMPB = hwVal;
}

static inline uint16_t csp_gptb_get_cmpb(csp_gptb_t *ptGptbBase)
{
	return (ptGptbBase -> CMPB);
}

static inline uint16_t csp_gptb_get_cmpaa(csp_gptb_t *ptGptbBase)
{
	return (ptGptbBase -> CMPAA);
}

static inline uint16_t csp_gptb_get_cmpba(csp_gptb_t *ptGptbBase)
{
	return (ptGptbBase -> CMPBA);
}

static inline void csp_gptb_set_cmpldr(csp_gptb_t *ptGptbBase, gptb_ldmd_e eLdmd, gptb_shdwldmd_e eShdwldmd ,gptb_comp_e eChannel)
{
	if(eChannel == GPTB_CMPA)
	{
		ptGptbBase -> CMPLDR = (ptGptbBase -> CMPLDR & ~(GPTB_CMPA_LD_MSK))     | (eLdmd     << GPTB_CMPA_LD_POS);
		ptGptbBase -> CMPLDR = (ptGptbBase -> CMPLDR & ~(GPTB_CMPA_LDTIME_MSK)) | (eShdwldmd << GPTB_CMPA_LDTIME_POS);
	}	
	else if(eChannel == GPTB_CMPB)
	{
		ptGptbBase -> CMPLDR = (ptGptbBase -> CMPLDR & ~(GPTB_CMPB_LD_MSK))     | (eLdmd     << GPTB_CMPB_LD_POS);
		ptGptbBase -> CMPLDR = (ptGptbBase -> CMPLDR & ~(GPTB_CMPB_LDTIME_MSK)) | (eShdwldmd << GPTB_CMPB_LDTIME_POS);
	}
}

static inline void csp_gptb_set_aqldr(csp_gptb_t *ptGptbBase, gptb_ldmd_e eLdmd, gptb_shdwldmd_e eShdwldmd ,gptb_channel_e eChannel)
{
	if(eChannel == GPTB_CHANNEL1)
	{
		ptGptbBase -> AQLDR = (ptGptbBase -> AQLDR &~(GPTB_AQCR1_SHDWEN_MSK)) | (eLdmd     << GPTB_AQCR1_SHDWEN_POS);
		ptGptbBase -> AQLDR = (ptGptbBase -> AQLDR &~(GPTB_LDAMD_MSK))		  | (eShdwldmd << GPTB_LDAMD_POS);
	}
	else if(eChannel == GPTB_CHANNEL2)
	{
		ptGptbBase -> AQLDR = (ptGptbBase -> AQLDR &~(GPTB_AQCR2_SHDWEN_MSK)) | (eLdmd 	   << GPTB_AQCR2_SHDWEN_POS);
		ptGptbBase -> AQLDR = (ptGptbBase -> AQLDR &~(GPTB_LDBMD_MSK))		  | (eShdwldmd << GPTB_LDBMD_POS);
	}
}

static inline void csp_gptb_set_aqcr1(csp_gptb_t *ptGptbBase, uint32_t wVal)
{
	ptGptbBase -> AQCR1 = wVal;
}

static inline void csp_gptb_set_aqcr2(csp_gptb_t *ptGptbBase, uint32_t wVal)
{
	ptGptbBase -> AQCR2 = wVal;
}

static inline void csp_gptb_set_aqosf(csp_gptb_t *ptGptbBase, gptb_channel_e eChannel, gptb_action_e eAction)
{
	if(eChannel == GPTB_CHANNEL1)
	{
		ptGptbBase -> AQOSF |= GPTB_OSTSFA;
		ptGptbBase -> AQOSF  = (ptGptbBase -> AQOSF & ~(GPTB_ACTA_MSK)) | ((eAction & 0x03) << GPTB_ACTA_POS);
	}
	else if(eChannel == GPTB_CHANNEL2)
	{
		ptGptbBase -> AQOSF |= GPTB_OSTSFB;
		ptGptbBase -> AQOSF  = (ptGptbBase -> AQOSF & ~(GPTB_ACTB_MSK)) | ((eAction & 0x03) << GPTB_ACTB_POS);
	}
}

static inline void csp_gptb_set_aqcsf_load_mode(csp_gptb_t *ptGptbBase, gptb_aqcsf_ldmd_e eLoadMode)
{
	ptGptbBase ->AQOSF = (ptGptbBase -> AQOSF & ~(GPTB_AQCSF_LDMD_MSK)) | ((eLoadMode & 0x03) << GPTB_AQCSF_LDMD_POS);
}

static inline void csp_gptb_set_aqcsf(csp_gptb_t *ptGptbBase, gptb_channel_e eChannel, gptb_aqcsf_act_e eAction)
{
	if(eChannel == GPTB_CHANNEL1)
		ptGptbBase -> AQCSF = (ptGptbBase ->AQCSF & ~(0x03)) | (eAction & 0x03);
	else if(eChannel == GPTB_CHANNEL2)
		ptGptbBase -> AQCSF = (ptGptbBase ->AQCSF & ~(0x0c)) | ((eAction & 0x03) << 2);
}

static inline uint32_t csp_gptb_get_dbldr(csp_gptb_t *ptGptbBase)
{
	return ( ptGptbBase -> DBLDR);
}

static inline void csp_gptb_set_dbldr(csp_gptb_t *ptGptbBase, uint32_t wVal)
{
	ptGptbBase -> DBLDR = wVal;
}

static inline void csp_gptb_set_dbcr(csp_gptb_t *ptGptbBase, uint32_t wVal)
{
	ptGptbBase -> DBCR = wVal;
}

static inline uint32_t csp_gptb_get_dbcr(csp_gptb_t *ptGptbBase)
{
	return ( ptGptbBase -> DBCR);	
}

static inline void csp_gptb_set_dpscr(csp_gptb_t *ptGptbBase, uint16_t hwVal)
{
	ptGptbBase ->DPSCR = hwVal;
}

static inline void csp_gptb_set_dbdtr(csp_gptb_t *ptGptbBase, uint16_t hwVal)
{
	ptGptbBase ->DBDTR = hwVal;
}

static inline void csp_gptb_set_dbdtf(csp_gptb_t *ptGptbBase, uint16_t hwVal)
{
	ptGptbBase ->DBDTF = hwVal;
}

static inline void csp_gptb_set_emsrc(csp_gptb_t *ptGptbBase,uint32_t wVal)
{
	ptGptbBase -> REGPROT = GPTB_REGPROT;
	ptGptbBase -> EMSRC = wVal;
}

static inline uint32_t csp_gptb_get_emsrc(csp_gptb_t *ptGptbBase )
{	
	return ( ptGptbBase -> EMSRC );
}

static inline void csp_gptb_set_emsrc2(csp_gptb_t *ptGptbBase,uint32_t wVal)
{
	ptGptbBase -> REGPROT = GPTB_REGPROT;
	ptGptbBase -> EMSRC2  = wVal;
}

static inline uint32_t csp_gptb_get_emsrc2(csp_gptb_t *ptGptbBase )
{	
	return ( ptGptbBase -> EMSRC2 );
}

static inline void csp_gptb_set_empol(csp_gptb_t *ptGptbBase, uint32_t wVal)
{
	ptGptbBase -> REGPROT = GPTB_REGPROT;
	ptGptbBase -> EMPOL = wVal;
}

static inline uint32_t csp_gptb_get_empol(csp_gptb_t *ptGptbBase )
{	
	return ( ptGptbBase -> EMPOL );
}

static inline  uint32_t csp_gptb_get_emecr(csp_gptb_t *ptGptbBase)				
{
	return (ptGptbBase->EMECR);
}

static inline void csp_gptb_set_emecr(csp_gptb_t *ptGptbBase,uint32_t wVal)
{
	ptGptbBase -> REGPROT = GPTB_REGPROT;
	ptGptbBase -> EMECR = wVal;
}

static inline void csp_gptb_set_emosr(csp_gptb_t *ptGptbBase,uint32_t wVal)
{
	ptGptbBase -> REGPROT = GPTB_REGPROT;
	ptGptbBase -> EMOSR = wVal;
}

static inline  uint32_t csp_gptb_get_emosr(csp_gptb_t *ptGptbBase)				
{
	return (ptGptbBase->EMOSR);
}

static inline void csp_gptb_emint_enable(csp_gptb_t *ptGptbBase, gptb_emint_e eEmint)
{
	ptGptbBase -> EMIMCR |=  eEmint;
}

static inline void csp_gptb_emint_disable(csp_gptb_t *ptGptbBase, gptb_emint_e eEmint)
{
	ptGptbBase -> EMIMCR &=  ~eEmint;
}

static inline uint32_t csp_gptb_get_emrisr(csp_gptb_t *ptGptbBase)
{
	return (ptGptbBase -> EMRISR);
}

static inline uint32_t csp_gptb_get_emisr(csp_gptb_t *ptGptbBase)
{
	return (ptGptbBase -> EMMISR);
}

static inline void csp_gptb_clr_emisr(csp_gptb_t *ptGptbBase, gptb_emint_e eInt)
{
	ptGptbBase -> EMICR = eInt;
}

static inline uint16_t csp_gptb_get_emhdlck(csp_gptb_t *ptGptbBase)				
{
	return (ptGptbBase ->EMHLSR &0x70f);
}

static inline void csp_gptb_clr_emhdlck(csp_gptb_t *ptGptbBase, gptb_ep_e eEp)
{
	ptGptbBase -> EMHLCLR = 0x01<< eEp;
}

static inline  uint16_t csp_gptb_get_emsdlck(csp_gptb_t *ptGptbBase)				
{
	return (ptGptbBase->EMSLSR&0x0f);
}

static inline void csp_gptb_clr_emsdlck(csp_gptb_t *ptGptbBase, gptb_ep_e eEp)
{
	ptGptbBase -> EMSLCLR = 0x1 << eEp;
}

static inline void csp_gptb_force_em(csp_gptb_t *ptGptbBase, gptb_ep_e eEp)
{
	ptGptbBase -> REGPROT = GPTB_REGPROT;
	ptGptbBase -> EMFRCR  = 0x1 << eEp;
}

static inline void csp_gptb_set_trgftcr(csp_gptb_t *ptGptbBase, uint32_t wPrd)
{
	ptGptbBase ->  TRGFTCR = wPrd;
}

static inline void csp_gptb_set_trgftwr(csp_gptb_t *ptGptbBase, uint32_t wPrd)
{
	ptGptbBase ->  TRGFTWR = wPrd;
}

static inline void csp_gptb_evtrg_cntiniten_enable(csp_gptb_t *ptGptbBase, gptb_trgev_e eTrgEv)
{
	ptGptbBase -> EVTRG |= 1 << GPTB_INITEN_POS_CNT(eTrgEv);
}

static inline void csp_gptb_set_evtrg_prd(csp_gptb_t *ptGptbBase, gptb_trgev_e eTrgEv, uint8_t byPrd)
{
	ptGptbBase -> EVPS = (ptGptbBase -> EVPS & (~GPTB_PRD_MSK_EV(eTrgEv))) | (byPrd << GPTB_PRD_POS_EV(eTrgEv));
}

static inline uint32_t csp_gptb_get_evtrg_cnt(csp_gptb_t *ptGptbBase, gptb_trgev_e eTrgEv)
{
	return (uint32_t) (ptGptbBase -> EVPS & GPTB_CNT_MSKS_EV(eTrgEv) >> GPTB_CNT_POS_EV(eTrgEv));
}

static inline void csp_gptb_sw_trg(csp_gptb_t *ptGptbBase, gptb_trgev_e eTrgEv)
{
	ptGptbBase -> EVSWF |= 0x1 << eTrgEv; 
}

static inline void csp_gptb_set_evcntinit(csp_gptb_t *ptGptbBase, gptb_trgev_e eTrgEv, uint8_t byVal)
{
	ptGptbBase -> EVCNTINIT = (ptGptbBase -> EVCNTINIT & ~GPTB_CNT_INIT_MSK_EV(eTrgEv)) | GPTB_CNT_INIT(byVal,eTrgEv);
}

static inline void csp_gptb_evtrg_enable(csp_gptb_t *ptGptbBase, gptb_trgev_e eTrgEv)
{
	ptGptbBase -> EVTRG |= (1 << GPTB_OUTEN_POS_TRG(eTrgEv));
}

static inline void csp_gptb_set_evtrg_src(csp_gptb_t *ptGptbBase,  gptb_trgev_e eTrgEv, gptb_trgsrc_e eSrc) 
{
	ptGptbBase -> EVTRG = (ptGptbBase -> EVTRG & (~GPTB_SEL_MSK_TRG(eTrgEv))) | (eSrc << GPTB_SEL_POS_TRG(eTrgEv));
}

static inline void csp_gptb_int_enable(csp_gptb_t *ptGptbBase, gptb_int_e eInt)
{
	ptGptbBase -> IMCR |= eInt;
}

static inline void csp_gptb_int_disable(csp_gptb_t *ptGptbBase, gptb_int_e eInt)
{
	ptGptbBase -> IMCR &= ~eInt;
}

static inline uint32_t csp_gptb_get_risr(csp_gptb_t *ptGptbBase)
{
	return (ptGptbBase -> RISR);
}

static inline uint32_t csp_gptb_get_isr(csp_gptb_t *ptGptbBase)
{
	return (ptGptbBase -> MISR);
}

static inline void csp_gptb_clr_isr(csp_gptb_t *ptGptbBase, gptb_int_e eInt)
{
	ptGptbBase -> ICR = eInt;
}

static inline void csp_gptb_set_reglk(csp_gptb_t *ptGptbBase, uint32_t wRegLk)
{
	ptGptbBase -> REGLK = wRegLk;
}

static inline void csp_gptb_set_reglk2(csp_gptb_t *ptGptbBase, uint32_t wRegLk)
{
	ptGptbBase -> REGLK2 = wRegLk;
}
#endif   /* csp_gptb_h */