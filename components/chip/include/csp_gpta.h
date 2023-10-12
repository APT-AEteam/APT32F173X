/***********************************************************************//** 
 * \file  csp_gpta.h
 * \brief  headfile for GPTA in csp level
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-9-24 <td>V0.0  <td>WNN   <td>initial
 * <tr><td> 2023-9-07 <td>V1.0  <td>LHY   <td>code normalization
 * </table>
 * *********************************************************************
*/

#ifndef _CSP_GPTA_H
#define _CSP_GPTA_H

#include "soc.h"

typedef struct
{
   __IOM  uint32_t	CEDR;  		      //0x0000	  Clock control & ID
   __IOM  uint32_t	RSSR;		      //0x0004	  Start & Stop Ctrl
   __IOM  uint32_t	PSCR;		      //0x0008	  Clock prescaler
   __IOM  uint32_t	CR;  		      //0x000C	  Control register
   __IOM  uint32_t	SYNCR;		      //0x0010	  Synchronization control reg
   __IOM  uint32_t	GLDCR;		      //0x0014	  Global load control reg
   __IOM  uint32_t	GLDCFG; 	      //0x0018	  Global load config
   __IOM  uint32_t	GLDCR2;		      //0x001C	  Global load control reg2
   __IOM  uint32_t	RSVD0;            //0x0020
   __IOM  uint32_t	PRDR;   	      //0x0024	  Period reg
   __IOM  uint32_t	PHSR;     	      //0x0028	  Phase control reg
   __IOM  uint32_t	CMPA;  		      //0x002C	  Compare Value A
   __IOM  uint32_t	CMPB;   	      //0x0030	  Compare Value B
   __IOM  uint32_t	RSVD1[2];
   __IOM  uint32_t	CMPLDR;	          //0x003C	  Cmp reg load control
   __IOM  uint32_t	CNT;      	      //0x0040	  Counter reg
   __IOM  uint32_t	AQLDR; 		      //0x0044	  AQ reg load control
   __IOM  uint32_t	AQCR1;            //0x0048	  Action qualify of ch-1
   __IOM  uint32_t	AQCR2;    	      //0x004C	  Action qualify of ch-2
   __IM   uint32_t	RSVD2[3];
   __IOM  uint32_t	AQOSF; 		      //0x005C	  AQ output one-shot software forcing
   __IOM  uint32_t	AQCSF;            //0x0060	  AQ output conti-software forcing
   __IM   uint32_t	RSVD3[21];
   __IOM  uint32_t  TRGFTCR;          //0x00B8    Trigger Filter control reg
   __IOM  uint32_t  TRGFTWR;          //0x00BC    Trigger filter window
   __IOM  uint32_t  EVTRG;            //0x00C0    Event trigger setting
   __IOM  uint32_t  EVPS;             //0x00C4    Event presaler
   __IOM  uint32_t  EVCNTINIT;        //0x00C8    Event software forcing
   __IOM  uint32_t	EVSWF;		      //0x00CC
   __IM   uint32_t  RISR;             //0x00D0    Interrupt RISR
   __IM   uint32_t  MISR;             //0x00D4    Interrupt MISR
   __IOM  uint32_t  IMCR;             //0x00D8    Interrupt IMCR
   __OM   uint32_t  ICR;              //0x00DC    Interrupt clear
   __IOM  uint32_t  REGLK;            //0x00e0                                             
   __IOM  uint32_t  REGLK2;           //0x00e4    
   __OM   uint32_t 	REGPROT;	      //0x00E8	  Protection Register
   __IM   uint32_t	RESERVED6[467-3];  
   __IM   uint32_t 	CMPAA;	          //0x082C	  cmpa active reg for capture mode
   __IM   uint32_t 	CMPBA;	          //0x0830	  cmpb active reg for capture mode
} csp_gpta_t;


/******************************************************************************
************************** GPTA Registers Definition ****************************
******************************************************************************/

/******************************************************************************
* GPTA Regiser
******************************************************************************/
typedef enum 
{	GPTA_CHa	= 0,
	GPTA_CHb,
}gpta_chtype_e;

/******************************************************************************
* CEDR : Clock control & ID
******************************************************************************/
#define GPTA_CLKEN_POS           (0)
#define GPTA_CLKEN_MSK           (0x1ul << GPTA_CLKEN_POS)
#define GPTA_DBGEN_POS		     (1)
#define GPTA_DBGEN_MSK           (0x1ul << GPTA_DBGEN_POS)
#define GPTA_CSS_POS		     (3)
#define GPTA_CSS_MSK		     (0x1ul << GPTA_CSS_POS)
#define GPTA_STOPMD_POS		     (6)
#define GPTA_STOPMD_MSK		     (0x1ul << GPTA_STOPMD_POS)
typedef enum
{
	GPTA_STOP_SHDW = 0,
	GPTA_STOP_IMMEDIATE
}gpta_stopmode_e;
#define GPTA_FLTPRS_POS		     (8)
#define GPTA_FLTPRS_MSK		     (0xfful << GPTA_FLTPRS_POS)

/******************************************************************************
* RSSR : Start & Stop Ctrl
******************************************************************************/
#define GPTA_START_POS		     (0)
#define GPTA_START_MSK           (0x1ul  << GPTA_START_POS)
#define GPTA_RESET_POS	         (12)
#define GPTA_RESET_MSK	         (0xful  << GPTA_RESET_POS)
#define GPTA_RESET_VALUE         (0x5ul  << GPTA_RESET_POS)

/******************************************************************************
* CR : Control register
******************************************************************************/
#define GPTA_CNTMD_POS           (0)
#define GPTA_CNTMD_MSK	         (0x3ul  << GPTA_CNTMD_POS)
typedef enum
{
	GPTA_CNTMD_UP = 0,
	GPTA_CNTMD_DOWN,
	GPTA_CNTMD_UP_DOWN
}gpta_cntmd_e;
#define GPTA_STARTMODE_POS	     (2)
#define GPTA_STARTMODE_MSK         (0x1ul << GPTA_STARTMODE_POS)
typedef enum
{
	GPTA_SW_START = 0,
	GPTA_SYNC_START
}gpta_startmode_e;
#define GPTA_STPST_POS	         (3)
#define GPTA_STPST_MSK           (0x1 << GPTA_STPST_POS)
typedef enum
{
	GPTA_STPST_HZ = 0,
	GPTA_STPST_LOW
}gpta_stpst_e;
#define GPTA_PRDLD_POS	         (4)
#define GPTA_PRDLD_MSK		     (0x3ul << GPTA_PRDLD_POS)
typedef enum 
{
	GPTA_LDPRDR_PEND = 0,
	GPTA_LDPRDR_LOAD_SYNC,
	GPTA_LDPRDR_PEND_LOAD_SYNC,
	GPTA_LDPRDR_IMMD
}gpta_ldprdr_e;
#define GPTA_RUNMODE_POS	     (6)
#define GPTA_RUNMODE_MSK	     (0x1 << GPTA_RUNMODE_POS)
typedef enum
{
	GPTA_RUNMODE_CONT = 0,
	GPTA_RUNMODE_ONCE,
}gpta_runmode_e;
#define GPTA_PHSEN_POS		     (7)
#define GPTA_PHSEN_MSK		     (0x1 << GPTA_PHSEN_POS)
#define GPTA_CAPLD_POS	         (8)
#define GPTA_CAPLD_MSK	         (0x1 << GPTA_CAPLD_POS)
#define GPTA_BURST_POS           (9)
#define GPTA_BURST_MSK           (1ul << GPTA_BURST_POS)
#define GPTA_FLT_INIT_POS        (10)
#define GPTA_FLT_INIT_MSK        (1ul << GPTA_FLT_INIT_POS)
#define GPTA_CGSRC_POS	         (11)
#define GPTA_CGSRC_MSK	         (0x3 << GPTA_CGSRC_POS)
typedef enum 
{
	GPTA_CGSRC_TIOA = 0,
	GPTA_CGSRC_TIOB,
	GPTA_CGSRC_TIN
}gpta_cgsrc_e;
#define GPTA_CGFLT_POS	         (13)
#define GPTA_CGFLT_MSK	         (0x7 << GPTA_CGFLT_POS)
typedef enum 
{
	GPTA_FLT_BP = 0,
	GPTA_FLT_2,
	GPTA_FLT_4,
	GPTA_FLT_6,
	GPTA_FLT_8,
	GPTA_FLT_16,
	GPTA_FLT_32,
	GPTA_FLT_64
}gpta_cgflt_e;
#define GPTA_PSCLD_POS	         (16)
#define GPTA_PSCLD_MSK	         (0x3 << GPTA_PSCLD_POS)
#define GPTA_MODE_POS	         (18)
#define GPTA_MODE_MSK            (1ul << GPTA_MODE_POS)
typedef enum
{
	GPTA_MODE_CAPTURE = 0,
	GPTA_MODE_WAVE
}gpta_workmode_e;
#define GPTA_CAPMD_SEL_POS       (19)
#define GPTA_CAPMD_SEL_MSK       (1ul << GPTA_CAPMD_SEL_POS)
typedef enum
{
	GPTA_MERGE_CAP = 0,
	GPTA_SEPARATE_CAP
}gpta_capmd_e;

#define GPTA_CAPREARM_POS        (20)
#define GPTA_CAPREARM_MSK        (1ul << GPTA_CAPREARM_POS)
#define GPTA_CAPMD_POS           (21)
#define GPTA_CAPMD_MSK	         (0x1 << GPTA_CAPMD_POS)
#define GPTA_STOPWRAP_POS	     (22)
#define GPTA_STOPWRAP_MSK	     (0x3 << GPTA_STOPWRAP_POS)
#define GPTA_CMPA_RST_POS	     (24)
#define GPTA_CMPA_RST_MSK	     (0x1 << GPTA_CMPA_RST_POS) 	
#define GPTA_CMPB_RST_POS	     (25)
#define GPTA_CMPB_RST_MSK	     (0x1 << GPTA_CMPB_RST_POS) 
#define GPTA_CMPAA_RST_POS	     (26)
#define GPTA_CMPAA_RST_MSK	     (0x1 << GPTA_CMPAA_RST_POS) 
#define GPTA_CMPBA_RST_POS	     (27)
#define GPTA_CMPBA_RST_MSK	     (0x1 << GPTA_CMPBA_RST_POS) 

/******************************************************************************
* SYNCR : Synchronization control reg
******************************************************************************/
#define GPTA_SYNC_POS(ch)	     (ch)
#define GPTA_SYNC_MSK(ch)	     (0x1 <<(GPTA_SYNC_POS(ch)))
typedef enum
{
	GPTA_SYNCIN_0	= 0,			
	GPTA_SYNCIN_1,						
	GPTA_SYNCIN_2,						
	GPTA_SYNCIN_3,			
	GPTA_SYNCIN_4,
	GPTA_SYNCIN_5,
    GPTA_SYNCIN_6		
}gpta_syncin_e;
#define GPTA_OSTMD_POS(ch)	     (ch+8)
#define GPTA_OSTMD_MSK(ch)	     (0x1 <<(GPTA_OSTMD_POS(ch)))
typedef enum 
{
	GPTA_OSTMD_CONT = 0,
	GPTA_OSTMD_ONCE
}gpta_syncmd_e;
#define GPTA_REARM_POS(ch)	     (ch+15)
#define GPTA_REARM_MSK(ch)	     (0x1 <<(GPTA_REARM_POS(ch)))
#define GPTA_TXREARM0_POS	     (22)
#define GPTA_TXREARM0_MSK	     (0x3 << GPTA_TXREARM0_POS)
typedef enum
{
	GPTA_TXREARM0_DIS =0,
	GPTA_TX1REARM0,
	GPTA_TX2REARM0,
	GPTA_TX1_2REARM0
}gpta_txrearm0_e;
#define GPTA_TRGO0SEL_POS	     (24)
#define GPTA_TRGO0SEL_MSK	     (0x7 << GPTA_TRGO0SEL_POS)
#define GPTA_TRGO1SEL_POS	     (27)
#define GPTA_TRGO1SEL_MSK	     (0x7 << GPTA_TRGO1SEL_POS)
#define GPTA_AREARM_POS		     (30)
#define GPTA_AREARM_MSK		     (0x3 << GPTA_AREARM_POS)
typedef enum
{
	GPTA_AREARM_DIS = 0,
	GPTA_AREARM_ZRO,
	GPTA_AREARM_PRD,
	GPTA_AREARM_ZRO_PRD
}gpta_arearm_e;

/******************************************************************************
* GLDCR : Global load control reg
******************************************************************************/
#define GPTA_GLDEN_POS	         (0)
#define GPTA_GLDEN_MSK	         (0x1 << GPTA_GLDEN_POS)
#define GPTA_GLDMD_POS	         (1)
#define GPTA_GLDMD_MSK	         (0xf << GPTA_GLDMD_POS)
#define GPTA_GLDCR_OSTMD_POS     (5)
#define GPTA_GLDCR_OSTMD_MSK     (0x1 << GPTA_GLDCR_OSTMD_POS)
#define GPTA_GLDPRD_POS	         (7)
#define GPTA_GLDPRD_MSK	         (0x7 << GPTA_GLDPRD_POS)
#define GPTA_GLDCNT_POS	         (10)
#define GPTA_GLDCNT_MSK	         (0x7 << GPTA_GLDCNT_POS)

/******************************************************************************
* CLDCFG : Global load config
******************************************************************************/
#define GPTA_LD_PRDR_POS         (0)
#define GPTA_LD_PRDR_MSK	     (0x1 << GPTA_LD_PRDR_POS)
#define GPTA_LD_CMPA_POS         (1)
#define GPTA_LD_CMPA_MSK	     (0x1 << GPTA_LD_CMPA_POS)
#define GPTA_LD_CMPB_POS         (2)
#define GPTA_LD_CMPB_MSK	     (0x1 << GPTA_LD_CMPB_POS)
#define GPTA_LD_AQCR1_POS        (8)
#define GPTA_LD_AQCR1_MSK	     (0x1 << GPTA_LD_AQCR1_POS)
#define GPTA_LD_AQCR2_POS        (9)
#define GPTA_LD_AQCR2_MSK	     (0x1 << GPTA_LD_AQCR2_POS)
#define GPTA_LD_AQCSF_POS        (12)
#define GPTA_LD_AQCSF_MSK	     (0x1 << GPTA_LD_AQCSF_POS)
typedef enum
{
	GPTA_LD_NOTGLD = 0,
	GPTA_LD_GLD
}gpta_gldmd_e;

/******************************************************************************
* GLDCR2 : Global load control reg2
******************************************************************************/
#define GPTA_OSREARM_EN	         (0x1)
#define GPTA_SW_GLD		         (0x2)

/******************************************************************************
* PHSR : Phase control reg
******************************************************************************/
#define GPTA_PHSR_POS	         (0)
#define GPTA_PHSR_MSK	         (0xffff << GPTA_PHSR_POS)
#define GPTA_PHSDIR_POS	         (31)
#define GPTA_PHSDIR_MSK	         (0x1 << GPTA_PHSDIR_POS)
typedef enum
{
	GPTA_DIR_DOWN = 0,
	GPTA_DIR_UP
}gpta_phsdir_e;

/******************************************************************************
* CMPA/B : Compare Value A/B
******************************************************************************/
#define GPTA_CMPDATA_MSK	     (0xffff)	
#define GPTA_CMPDATA_OVWRT       (0x1 << 31)
typedef enum
{
    GPTA_CMPA=0,
	GPTA_CMPB,
}gpta_comp_e;

/******************************************************************************
* CMPLDR : Cmp reg load control
******************************************************************************/
#define GPTA_CMPA_LD_POS	     (0)
#define GPTA_CMPA_LD_MSK         (0x1 << GPTA_CMPA_LD_POS)
#define GPTA_CMPB_LD_POS	     (1)
#define GPTA_CMPB_LD_MSK         (0x1 << GPTA_CMPB_LD_POS)
typedef enum 
{
	GPTA_CMPLD_SHDW = 0,
	GPTA_CMPLD_IMMEDIATE
}gpta_ldmd_e;
#define GPTA_CMPA_LDTIME_POS     (4)
#define GPTA_CMPA_LDTIME_MSK     (0x7 << GPTA_CMPA_LDTIME_POS)
#define GPTA_CMPB_LDTIME_POS     (7)
#define GPTA_CMPB_LDTIME_MSK     (0x7 << GPTA_CMPB_LDTIME_POS)
typedef enum
{
	GPTA_LDCMP_NEVER = 0,
	GPTA_LDCMP_ZRO,
	GPTA_LDCMP_PRD,
	GPTA_LDCMP_LD_SYNC = 4,
}gpta_shdwldmd_e;
#define GPTA_SHDWAFULL 	         (0x1 <<20)
#define GPTA_SHDWBFULL 	         (0x1 <<21)

/******************************************************************************
* AQLDR : AQ reg load control
******************************************************************************/
typedef enum
{
    GPTA_CHANNEL1=1,
	GPTA_CHANNEL2,
}gpta_channel_e;

#define GPTA_AQCR1_SHDWEN_POS	 (0)
#define GPTA_AQCR1_SHDWEN_MSK	 (0x1 << GPTA_AQCR1_SHDWEN_POS)
#define GPTA_AQCR2_SHDWEN_POS	 (1)
#define GPTA_AQCR2_SHDWEN_MSK	 (0x1 << GPTA_AQCR2_SHDWEN_POS)
#define GPTA_LDAMD_POS		     (2)
#define GPTA_LDAMD_MSK		     (0x7 << GPTA_LDAMD_POS)
#define GPTA_LDBMD_POS		     (5)
#define GPTA_LDBMD_MSK		     (0x7 << GPTA_LDBMD_POS)

/******************************************************************************
* AQCR1/2 : Action qualify of ch-1/2
******************************************************************************/
#define GPTA_ACT_ZRO_POS	     (0)
#define GPTA_ACT_ZRO_MSK	     (0x3 << GPTA_ACT_ZRO_POS)
#define GPTA_ACT_PRD_POS	     (2)
#define GPTA_ACT_PRD_MSK	     (0x3 << GPTA_ACT_PRD_POS)
#define GPTA_ACT_C1U_POS	     (4)
#define GPTA_ACT_C1U_MSK	     (0x3 << GPTA_ACT_C1U_POS)
#define GPTA_ACT_C1D_POS	     (6)
#define GPTA_ACT_C1D_MSK	     (0x3 << GPTA_ACT_C1D_POS)
#define GPTA_ACT_C2U_POS	     (8)
#define GPTA_ACT_C2U_MSK	     (0x3 << GPTA_ACT_C2U_POS)
#define GPTA_ACT_C2D_POS	     (10)
#define GPTA_ACT_C2D_MSK	     (0x3 << GPTA_ACT_C2D_POS)
#define GPTA_ACT_T1U_POS	     (12)
#define GPTA_ACT_T1U_MSK	     (0x3 << GPTA_ACT_T1U_POS)
#define GPTA_ACT_T1D_POS	     (14)
#define GPTA_ACT_T1D_MSK	     (0x3 << GPTA_ACT_T1D_POS)
#define GPTA_ACT_T2U_POS	     (16)
#define GPTA_ACT_T2U_MSK	     (0x3 << GPTA_ACT_T2U_POS)
#define GPTA_ACT_T2D_POS	     (18)
#define GPTA_ACT_T2D_MSK	     (0x3 << GPTA_ACT_T2D_POS)
#define GPTA_C1SEL_POS	         (20)
#define GPTA_C1SEL_MSK	         (0x3 << GPTA_C1SEL_POS)
#define GPTA_C2SEL_POS	         (22)
#define GPTA_C2SEL_MSK	         (0x3 << GPTA_C2SEL_POS)

/******************************************************************************
* AQOSF : AQ output one-shot software forcing
******************************************************************************/
#define GPTA_OSTSF1_POS          (0)
#define GPTA_OSTSF1_MSK		     (0x1 << GPTA_OSTSF1_POS)
#define GPTA_ACT1_POS	         (1)
#define GPTA_ACT1_MSK	         (0x3 << GPTA_ACT1_POS)
#define GPTA_OSTSF2_POS          (4)
#define GPTA_OSTSF2_MSK	         (0x1 << GPTA_OSTSF2_POS)
#define GPTA_ACT2_POS	         (5)
#define GPTA_ACT2_MSK	         (0x3 << GPTA_ACT2_POS)
#define GPTA_AQCSF_LDMD_POS      (16)
#define GPTA_AQCSF_LDMD_MSK      (0x3 << GPTA_AQCSF_LDMD_POS)
typedef enum 
{
	GPTA_NA = 0,
	GPTA_LO,
	GPTA_HI,
	GPTA_TG	
}gpta_action_e;

/******************************************************************************
* AQCSF : AQ output conti-software forcing
******************************************************************************/
#define GPTA_CSF1_POS	         (0)
#define GPTA_CSF1_MSK	         (0x3 << GPTA_CSF1_POS)
#define GPTA_CSF2_POS	         (2)
#define GPTA_CSF2_MSK	         (0x3 << GPTA_CSF2_POS)
//typedef enum
//{
//	GPTA_LDOSF_NEA= 0,
//	GPTA_LDOSF_LOW ,
//	GPTA_LDOSF_HIGH,
//    GPTA_LDOSF_NEA1	
//}gpta_ldcsf_e;

typedef enum 
{
	GPTA_LOAD_NOW=0,
	GPTA_LOAD_ZRO,
	GPTA_LOAD_PRD,
	GPTA_LOAD_ZRO_PRD
}gpta_aqcsf_ldmd_e;

typedef enum 
{
	GPTA_NONE=0,
	GPTA_L,
	GPTA_H,
	GPTA_NONE1
}gpta_aqcsf_act_e;

/******************************************************************************
* TRGFTCR : Trigger Filter control reg
******************************************************************************/
#define GPTA_FLTSRC_POS	        (0)
#define GPTA_FLTSRC_MSK	        (0x7 << GPTA_FLTSRC_POS)
#define GPTA_FLTBLKINV_POS      (4)
#define GPTA_FLTBLKINV_MSK	    (0x1 << GPTA_FLTBLKINV_POS)
#define GPTA_ALIGNMD_POS	    (5)
#define GPTA_ALIGNMD_MSK	    (0x3 << GPTA_ALIGNMD_POS)
#define GPTA_CROSSMD_POS	    (7)
#define GPTA_CROSSMD_MSK	    (0x1 << GPTA_CROSSMD_POS)

/******************************************************************************
* TRGFTWR : Trigger filter window
******************************************************************************/
#define GPTA_FLT_OFFSET_POS	    (0)
#define GPTA_FLT_OFFSET_MSK	    (0xffff << GPTA_FLT_OFFSET_POS)
#define GPTA_FLT_WDW_POS	    (16)
#define GPTA_FLT_WDW_MSK	    (0xffff << GPTA_FLT_WDW_POS)

/******************************************************************************
* EVTRG : Event trigger setting
******************************************************************************/
#define GPTA_SEL_POS_TRG(n)	(n << 2)
#define GPTA_SEL_MSK_TRG(n)	(0xf << GPTA_SEL_POS_TRG(n))
typedef enum
{
	GPTA_TRG01_DIS = 0,
	GPTA_TRG01_ZRO,
	GPTA_TRG01_PRD,
	GPTA_TRG01_ZRO_PRD,
	GPTA_TRG01_CMPA_R,
	GPTA_TRG01_CMPA_F,
	GPTA_TRG01_CMPB_R,
	GPTA_TRG01_CMPB_F,
	GPTA_TRG01_SYNC = 0xc
}gpta_trgsrc_e;
#define GPTA_INITEN_POS_CNT(n)	(16+n)
#define GPTA_INITEN_MSK_CNT(n)	(0x1 << GPTA_INITEN_POS_CNT(n))
#define GPTA_OUTEN_POS_TRG(n)	(20+n)
#define GPTA_OUTEN_MSK_TRG(n)	(0x1 << GPTA_OUTEN_POS_TRG(n))
typedef enum
{
	GPTA_TRGEV0		= 0,	
	GPTA_TRGEV1,				
}gpta_trgev_e;
#define GPTA_SWTRG_EV(n)	    (n+24)

/******************************************************************************
* EVPS : Event presaler
******************************************************************************/
#define GPTA_PRD_POS_EV(n)	    (n * 4)
#define GPTA_PRD_MSK_EV(n)	    (0x1 << GPTA_PRD_POS_EV(n))
#define GPTA_CNT_POS_EV(n)	    (16 + n * 4)	
#define GPTA_CNT_MSK_EV(n)	    (0x1 << GPTA_CNT_POS_EV(n))

/******************************************************************************
* EVCNTINIT : Event software forcing
******************************************************************************/
#define GPTA_CNT_INIT_POS_EV(n) (n << 2)
#define GPTA_CNT_INIT_MSK_EV(n) (0x1 << GPTA_CNT_INIT_POS_EV(n))	
#define GPTA_CNT_INIT(val, n)   ((0xf & val) << GPTA_CNT_INIT_POS_EV(n))
	
/******************************************************************************
* EVSWF 
******************************************************************************/
#define GPTA_SWF_EV(n)	        (0x1 << n)

/******************************************************************************
* REGLK 
******************************************************************************/
#define GPTA_PRDR_POS	        (0)
#define GPTA_PRDR_MSK           (0xf << GPTA_PRDR_POS)
#define GPTA_CMPA_POS	        (4)
#define GPTA_CMPA_MSK           (0xf << GPTA_CMPA_POS)
#define GPTA_CMPB_POS	        (8)
#define GPTA_CMPB_MSK           (0xf << GPTA_CMPB_POS)
#define GPTA_GLD2_POS	        (20)
#define GPTA_GLD2_MSK           (0xf << GPTA_GLD2_POS)	
#define GPTA_RSSR_POS	        (24)
#define GPTA_RSSR_MSK           (0xf << GPTA_RSSR_POS)	

/******************************************************************************
* REGLK2 
******************************************************************************/
#define GPTA_EMSLCLR_POS        (0)
#define GPTA_EMSLCLR_MSK        (0xf << GPTA_EMSLCLR_POS)
#define GPTA_EMHLCLR_POS        (4)
#define GPTA_EMHLCLR_MSK        (0xf << GPTA_EMHLCLR_POS)
#define GPTA_EMICR_POS	        (8)
#define GPTA_EMICR_MSK          (0xf << GPTA_EMICR_POS)
#define GPTA_EMFRCR_POS	        (12)
#define GPTA_EMFRCR_MSK         (0xf << GPTA_EMFRCR_POS)	
#define GPTA_AQOSF_POS	        (16)
#define GPTA_AQOSF_MSK          (0xf << GPTA_AQOSF_POS)
#define GPTA_AQCSF_POS	        (20)
#define GPTA_AQCSF_MSK          (0xf << GPTA_AQCSF_POS)

/******************************************************************************
* Interrupt Related
******************************************************************************/
typedef enum
{
	GPTA_INT_TRGEV0 = 0x1 << 0,
	GPTA_INT_TRGEV1 = 0x1 << 1,
	GPTA_INT_CAPLD0 = 0x1 << 4,
	GPTA_INT_CAPLD1 = 0x1 << 5,
	GPTA_INT_CAPLD2 = 0x1 << 6,
	GPTA_INT_CAPLD3 = 0x1 << 7,
	GPTA_INT_CAU    = 0x1 << 8,
	GPTA_INT_CAD    = 0x1 << 9,
	GPTA_INT_CBU    = 0x1 << 10,
	GPTA_INT_CBD    = 0x1 << 11,
	GPTA_INT_PEND   = 0x1 << 16,	
	GPTA_INT_ALL    = 0x1FFFF
}gpta_int_e;

#define GPTA_INT_EV(n)		    (0x1 << n)
#define GPTA_INT_CAPLD(n)	    (0x1 << (n+4))

///REGPROT
#define GPTA_REGPROT	        (0xa55a << 16 | 0xc73a)

/*****************************************************************************
 * static inline functions
 ****************************************************************************/
static inline void csp_gpta_wr_key(csp_gpta_t *ptGptaBase)
{
	ptGptaBase -> REGPROT = GPTA_REGPROT;
}

//CEDR 
static inline void csp_gpta_clken(csp_gpta_t *ptGptaBase)
{
	ptGptaBase -> CEDR |=  GPTA_CLKEN_MSK;
}

static inline void csp_gpta_start(csp_gpta_t *ptGptaBase)
{
	ptGptaBase->RSSR |= GPTA_START_MSK ;
} 

static inline void csp_gpta_stop(csp_gpta_t *ptGptaBase)
{
	ptGptaBase->RSSR &= ~GPTA_START_MSK ;
}
 
static inline void csp_gpta_dbg_enable(csp_gpta_t *ptGptaBase)
{
	ptGptaBase -> CEDR |= GPTA_DBGEN_MSK;
}

static inline void csp_gpta_dbg_disable(csp_gpta_t *ptGptaBase)
{
	ptGptaBase -> CEDR &= ~GPTA_DBGEN_MSK;
}

static inline void csp_gpta_set_clksrc(csp_gpta_t *ptGptaBase, uint8_t byVal)
{
	ptGptaBase -> CEDR = (ptGptaBase -> CEDR & (~GPTA_CSS_MSK)) | (byVal << GPTA_CSS_POS);
}

//RSSR 
static inline void csp_gpta_sw_rst(csp_gpta_t *ptGptaBase)
{
	ptGptaBase -> REGPROT = GPTA_REGPROT;
	ptGptaBase->RSSR |= GPTA_RESET_VALUE;
}

//PRDR 
static inline void csp_gpta_set_prdr(csp_gpta_t *ptGptaBase, uint16_t hwVal)
{
	ptGptaBase -> PRDR = hwVal;
}

static inline uint16_t csp_gpta_get_prdr(csp_gpta_t *ptGptaBase)
{
	return (ptGptaBase -> PRDR);
}

//PSCR 
static inline void csp_gpta_set_pscr(csp_gpta_t *ptGptaBase, uint16_t hwVal)
{
	ptGptaBase -> PSCR = hwVal;
}

//cr  
static inline void csp_gpta_set_cr(csp_gpta_t *ptGptaBase,uint32_t wVal)
{
	ptGptaBase -> CR = wVal;
}

static inline void csp_gpta_set_mode(csp_gpta_t *ptGptaBase, gpta_workmode_e eMode)
{
	ptGptaBase -> CR = (ptGptaBase -> CR &(~GPTA_MODE_MSK)) | eMode << GPTA_MODE_POS;
}

static inline void csp_gpta_set_cntmd(csp_gpta_t *ptGptaBase, gpta_cntmd_e eMode)
{
	ptGptaBase -> CR = (ptGptaBase->CR & (~GPTA_CNTMD_MSK)) | (eMode << GPTA_CNTMD_POS);
}

static inline uint8_t csp_gpta_get_cntmd(csp_gpta_t *ptGptaBase)
{
	return (ptGptaBase->CR & GPTA_CNTMD_MSK);
}

static inline void csp_gpta_set_stopwrap(csp_gpta_t *ptGptaBase, uint8_t byTime)
{
	ptGptaBase -> CR = (ptGptaBase -> CR & ~(GPTA_STOPWRAP_MSK)) | (byTime << GPTA_STOPWRAP_POS);
}

static inline void csp_gpta_burst_enable(csp_gpta_t *ptGptaBase)
{
	ptGptaBase -> CR |= GPTA_BURST_MSK;
}

static inline void csp_gpta_burst_disable(csp_gpta_t *ptGptaBase)
{
	ptGptaBase -> CR &= ~GPTA_BURST_MSK;
}

static inline void csp_gpta_set_cgsrc(csp_gpta_t *ptGptaBase, uint8_t byCgSrc)
{
	ptGptaBase -> CR = (ptGptaBase -> CR & (~GPTA_CGSRC_MSK)) | (byCgSrc << GPTA_CGSRC_POS);
}

static inline void csp_gpta_set_flt(csp_gpta_t *ptGptaBase,uint8_t byCgFlt)
{
	ptGptaBase -> CR = (ptGptaBase -> CR & (~GPTA_CGFLT_MSK)) | (byCgFlt << GPTA_CGFLT_POS);
}

static inline void csp_gpta_flt_enable(csp_gpta_t *ptGptaBase)
{
	ptGptaBase -> CR |= GPTA_FLT_INIT_MSK;
}

static inline void csp_gpta_flt_disable(csp_gpta_t *ptGptaBase)
{
	ptGptaBase -> CR &= ~GPTA_FLT_INIT_MSK;
}

static inline void csp_gpta_set_ldprdr(csp_gpta_t *ptGptaBase, gpta_ldprdr_e eLdprdr)
{
	ptGptaBase -> CR = (ptGptaBase->CR & ~(GPTA_PRDLD_MSK)) | (eLdprdr << GPTA_PRDLD_POS);
}

static inline void csp_gpta_set_start_mode(csp_gpta_t *ptGptaBase, gpta_startmode_e eStartMode)
{
	ptGptaBase -> CR = (ptGptaBase->CR & (~GPTA_STARTMODE_MSK))| (eStartMode <<GPTA_STARTMODE_POS);
}

static inline void csp_gpta_set_stop_st(csp_gpta_t *ptGptaBase, gpta_stpst_e eVal)
{
	ptGptaBase -> CR = (ptGptaBase->CR & (~GPTA_STPST_MSK))| (eVal <<GPTA_STPST_POS);
}

static inline void csp_gpta_set_runmode(csp_gpta_t *ptGptaBase, gpta_runmode_e eRunModel)
{
	ptGptaBase -> CR = (ptGptaBase->CR & (~GPTA_RUNMODE_MSK))| (eRunModel <<GPTA_RUNMODE_POS);
}

static inline void csp_gpta_capld_enable(csp_gpta_t *ptGptaBase)
{
	ptGptaBase -> CR |= GPTA_CAPLD_MSK;
}

static inline void csp_gpta_capld_disable(csp_gpta_t *ptGptaBase)
{
	ptGptaBase -> CR &= ~GPTA_CAPLD_MSK;
}

static inline void csp_gpta_phsen_enable(csp_gpta_t *ptGptaBase)
{
	ptGptaBase -> CR |= GPTA_PHSEN_MSK;
}

static inline void csp_gpta_phsen_disable(csp_gpta_t *ptGptaBase)
{
	ptGptaBase -> CR &= ~GPTA_PHSEN_MSK;
}

//syncr 
static inline void csp_gpta_sync_enable(csp_gpta_t *ptGptaBase, gpta_syncin_e eSyncIn)
{  
	ptGptaBase -> REGPROT = GPTA_REGPROT;
	ptGptaBase -> SYNCR |= GPTA_SYNC_MSK(eSyncIn);
}

static inline void csp_gpta_sync_disable(csp_gpta_t *ptGptaBase, gpta_syncin_e eSyncIn)
{   
	ptGptaBase -> REGPROT = GPTA_REGPROT;
	ptGptaBase -> SYNCR &= ~GPTA_SYNC_MSK(eSyncIn);
}

static inline void csp_gpta_set_sync_mode(csp_gpta_t *ptGptaBase, gpta_syncin_e eSyncIn, gpta_syncmd_e eMode)
{   
	ptGptaBase -> REGPROT = GPTA_REGPROT;
	ptGptaBase -> SYNCR = (ptGptaBase -> SYNCR & ~(GPTA_OSTMD_MSK(eSyncIn)) ) | eMode << GPTA_OSTMD_POS(eSyncIn);
}

static inline void csp_gpta_set_auto_rearm(csp_gpta_t *ptGptaBase,  gpta_arearm_e eHWMode)
{   
	ptGptaBase -> REGPROT = GPTA_REGPROT;
	ptGptaBase -> SYNCR = (ptGptaBase -> SYNCR & ~(GPTA_AREARM_MSK)) | eHWMode << GPTA_AREARM_POS;
}

static inline void csp_gpta_sync_trg0sel(csp_gpta_t *ptGptaBase, gpta_syncin_e eSyncIn)
{   
	ptGptaBase -> REGPROT = GPTA_REGPROT;
	ptGptaBase -> SYNCR = (uint32_t)((ptGptaBase -> SYNCR & ~GPTA_TRGO0SEL_MSK) | (eSyncIn << GPTA_TRGO0SEL_POS));
}

static inline void csp_gpta_sync_trg1sel(csp_gpta_t *ptGptaBase, gpta_syncin_e eSyncIn)
{   
	ptGptaBase -> REGPROT = GPTA_REGPROT;;
	ptGptaBase -> SYNCR = (uint32_t)((ptGptaBase -> SYNCR & ~GPTA_TRGO1SEL_MSK) | (eSyncIn << GPTA_TRGO1SEL_POS));
}

static inline void csp_gpta_sync_rearm(csp_gpta_t *ptGptaBase, gpta_syncin_e eSyncIn)
{   
	ptGptaBase -> REGPROT = GPTA_REGPROT;;
	ptGptaBase -> SYNCR = ptGptaBase -> SYNCR | GPTA_REARM_MSK(eSyncIn);
}

//CMPA/B 
static inline void csp_gpta_set_cmpa(csp_gpta_t *ptGptaBase, uint16_t hwVal)
{
	ptGptaBase -> CMPA = hwVal;
}

static inline uint16_t csp_gpta_get_cmpa(csp_gpta_t *ptGptaBase)
{
	return (ptGptaBase -> CMPA);
}

static inline uint16_t csp_gpta_get_cmpaa(csp_gpta_t *ptGptaBase)
{
	return (ptGptaBase -> CMPAA);
}

static inline void csp_gpta_set_cmpb(csp_gpta_t *ptGptaBase, uint16_t hwVal)
{
	ptGptaBase -> CMPB = hwVal;
}

static inline uint16_t csp_gpta_get_cmpb(csp_gpta_t *ptGptaBase)
{
	return (ptGptaBase -> CMPB);
}

static inline uint16_t csp_gpta_get_cmpba(csp_gpta_t *ptGptaBase)
{
	return (ptGptaBase -> CMPBA);
}


//PHSR 
static inline void csp_gpta_set_phsr(csp_gpta_t *ptGptaBase, uint16_t hwVal)
{
	ptGptaBase -> PHSR = hwVal;
}

static inline uint16_t csp_gpta_get_phsr(csp_gpta_t *ptGptaBase)
{
	return (ptGptaBase -> PHSR);
}


//AQCR1/2 
static inline void csp_gpta_set_aqcr1(csp_gpta_t *ptGptaBase, uint32_t wVal)
{
	ptGptaBase -> AQCR1 = wVal;
}

static inline void csp_gpta_set_aqcr2(csp_gpta_t *ptGptaBase, uint32_t wVal)
{
	ptGptaBase -> AQCR2 = wVal;
}

//aqosf
static inline void csp_gpta_set_aqosf(csp_gpta_t *ptGptaBase, gpta_channel_e eChannel, gpta_action_e eAction)
{
	if(eChannel == GPTA_CHANNEL1)
	{
		ptGptaBase -> AQOSF |= GPTA_OSTSF1_MSK;
		ptGptaBase -> AQOSF  = (ptGptaBase -> AQOSF & ~(GPTA_ACT1_MSK)) | ((eAction & 0x03) << GPTA_ACT1_POS);
	}
	else if(eChannel == GPTA_CHANNEL2)
	{
		ptGptaBase -> AQOSF |= GPTA_OSTSF2_MSK;
		ptGptaBase -> AQOSF  = (ptGptaBase -> AQOSF & ~(GPTA_ACT2_MSK)) | ((eAction & 0x03) << GPTA_ACT2_POS);
	}
}

//aqcsf 
static inline void csp_gpta_set_aqcsf_ldmd(csp_gpta_t *ptGptaBase, gpta_aqcsf_ldmd_e eLoadMode)
{
	ptGptaBase ->AQOSF = (ptGptaBase -> AQOSF & ~(GPTA_AQCSF_LDMD_MSK)) | ((eLoadMode & 0x03) << GPTA_AQCSF_LDMD_POS);
}

static inline void csp_gpta_set_aqcsf(csp_gpta_t *ptGptaBase, gpta_channel_e eChannel, gpta_aqcsf_act_e eAction)
{
	if(eChannel == GPTA_CHANNEL1)
	{
		ptGptaBase -> AQCSF = (ptGptaBase ->AQCSF & ~(0x03)) | (eAction & 0x03);
	}
	else if(eChannel == GPTA_CHANNEL2)
	{
		ptGptaBase -> AQCSF = (ptGptaBase ->AQCSF & ~(0x0c)) | ((eAction & 0x03) << 2);
	}
}

//aqldr
static inline void csp_gpta_set_aqldr(csp_gpta_t *ptGptaBase, gpta_ldmd_e eLdmd, gpta_shdwldmd_e eShdwldmd ,gpta_channel_e eChannel)
{
	if(eChannel == GPTA_CHANNEL1)
	{
		ptGptaBase -> AQLDR = (ptGptaBase -> AQLDR &~(GPTA_AQCR1_SHDWEN_MSK)) | (eLdmd     << GPTA_AQCR1_SHDWEN_POS);
		ptGptaBase -> AQLDR = (ptGptaBase -> AQLDR &~(GPTA_LDAMD_MSK))		  | (eShdwldmd << GPTA_LDAMD_POS);
	}
	else if(eChannel == GPTA_CHANNEL2)
	{
		ptGptaBase -> AQLDR = (ptGptaBase -> AQLDR &~(GPTA_AQCR2_SHDWEN_MSK)) | (eLdmd 	   << GPTA_AQCR2_SHDWEN_POS);
		ptGptaBase -> AQLDR = (ptGptaBase -> AQLDR &~(GPTA_LDBMD_MSK))		  | (eShdwldmd << GPTA_LDBMD_POS);
	}
}

//GLDCFG
static inline void  csp_gpta_set_gldcfg(csp_gpta_t *ptGptaBase, uint16_t hwMask, uint16_t hwVal)
{
	ptGptaBase -> GLDCFG = (ptGptaBase -> GLDCFG & ~(hwMask)) | hwVal;
}

//GLDCR 
static inline void csp_gpta_set_gldcr(csp_gpta_t *ptGptaBase, uint32_t wVal)
{
	ptGptaBase -> GLDCR   =  wVal ;
}

static inline void csp_gpta_set_gldcr2(csp_gpta_t *ptGptaBase, uint32_t wVal)
{   ptGptaBase -> REGPROT = GPTA_REGPROT;
	ptGptaBase -> GLDCR2   |=  wVal ;
}


//EVTRG  
static inline void csp_gpta_evtrg_cntxiniten_enable(csp_gpta_t *ptGptaBase, gpta_trgev_e eTrgEv)///
{
	ptGptaBase -> EVTRG |= GPTA_INITEN_MSK_CNT(eTrgEv);
}

static inline void csp_gpta_evtrg_cntxiniten_disable(csp_gpta_t *ptGptaBase, gpta_trgev_e eTrgEv)///
{
	ptGptaBase -> EVTRG &= ~GPTA_INITEN_MSK_CNT(eTrgEv);
}

static inline void csp_gpta_evtrg_enable(csp_gpta_t *ptGptaBase, gpta_trgev_e eTrgEv)
{
	ptGptaBase -> EVTRG |= GPTA_OUTEN_MSK_TRG(eTrgEv);
}

static inline void csp_gpta_evtrg_disable(csp_gpta_t *ptGptaBase, gpta_trgev_e eTrgEv)
{
	ptGptaBase -> EVTRG &= ~GPTA_OUTEN_MSK_TRG(eTrgEv);
}

static inline void csp_gpta_set_evtrgsel(csp_gpta_t *ptGptaBase, gpta_trgev_e eTrgEv, gpta_trgsrc_e eSrc)
{
	ptGptaBase -> EVTRG = (ptGptaBase -> EVTRG & (~GPTA_SEL_MSK_TRG(eTrgEv))) | (eSrc << GPTA_SEL_POS_TRG(eTrgEv));
}

//CMPLDR
static inline void csp_gpta_set_cmpldr(csp_gpta_t *ptGptaBase, gpta_ldmd_e eLdmd, gpta_shdwldmd_e eShdwldmd ,gpta_comp_e eChannel)
{
	if(eChannel == GPTA_CMPA)
	{
		ptGptaBase -> CMPLDR = (ptGptaBase -> CMPLDR & ~(GPTA_CMPA_LD_MSK))     | (eLdmd     << GPTA_CMPA_LD_POS);
		ptGptaBase -> CMPLDR = (ptGptaBase -> CMPLDR & ~(GPTA_CMPA_LDTIME_MSK)) | (eShdwldmd << GPTA_CMPA_LDTIME_POS);
	}	
	else if(eChannel == GPTA_CMPB)
	{
		ptGptaBase -> CMPLDR = (ptGptaBase -> CMPLDR & ~(GPTA_CMPB_LD_MSK))     | (eLdmd     << GPTA_CMPB_LD_POS);
		ptGptaBase -> CMPLDR = (ptGptaBase -> CMPLDR & ~(GPTA_CMPB_LDTIME_MSK)) | (eShdwldmd << GPTA_CMPB_LDTIME_POS);
	}
}

//TRGFTCR 
static inline void csp_gpta_set_trgftcr(csp_gpta_t *ptGptaBase, uint32_t wPrd)
{
	ptGptaBase ->  TRGFTCR = wPrd;
}

//TRGFTWR  
static inline void csp_gpta_set_trgftwr(csp_gpta_t *ptGptaBase, uint32_t wPrd)
{
	ptGptaBase ->  TRGFTWR = wPrd;
}

//EVPS  
static inline void csp_gpta_set_evtrg_prd(csp_gpta_t *ptGptaBase, uint8_t byCh, uint8_t byPrd)
{
	ptGptaBase ->  EVPS = ( ptGptaBase ->  EVPS & ~GPTA_PRD_MSK_EV(byCh)) | (byPrd << GPTA_PRD_POS_EV(byCh));
}

static inline uint32_t csp_gpta_get_evtrg_cnt(csp_gpta_t *ptGptaBase, gpta_trgev_e eTrgEv)
{
	return (uint32_t) (ptGptaBase -> EVPS & GPTA_CNT_MSK_EV(eTrgEv) >> GPTA_CNT_POS_EV(eTrgEv));
}

//EVCNTINIT  
static inline void csp_gpta_set_evcntinit(csp_gpta_t *ptGptaBase, uint8_t byCh, uint8_t byVal)
{
	ptGptaBase -> EVCNTINIT = (ptGptaBase -> EVCNTINIT & ~GPTA_CNT_INIT_MSK_EV(byCh)) | GPTA_CNT_INIT(byVal,byCh);
}

//EVSWF  
static inline void csp_gpta_sw_trg(csp_gpta_t *ptGptaBase, uint8_t byCh)
{
	ptGptaBase -> EVSWF |= 0x1 << byCh; 
}

//REGLK/2 
static inline void csp_gpta_set_reglk(csp_gpta_t *ptGptaBase, uint32_t wRegLk)
{
	ptGptaBase -> REGLK = wRegLk;
}

static inline void csp_gpta_set_reglk2(csp_gpta_t *ptGptaBase, uint32_t wRegLk)
{
	ptGptaBase -> REGLK2 = wRegLk;
}

//interrupt
static inline void csp_gpta_int_enable(csp_gpta_t *ptGptaBase, gpta_int_e eInt)
{
	ptGptaBase -> IMCR |= eInt;
}

static inline void csp_gpta_int_disable(csp_gpta_t *ptGptaBase, gpta_int_e eInt)
{
	ptGptaBase -> IMCR &= ~eInt;	
}

static inline uint32_t csp_gpta_get_risr(csp_gpta_t *ptGptaBase)
{
	return (ptGptaBase -> RISR);
}

static inline uint32_t csp_gpta_get_isr(csp_gpta_t *ptGptaBase)
{
	return (ptGptaBase -> MISR);
}

static inline void csp_gpta_clr_isr(csp_gpta_t *ptGptaBase, gpta_int_e eInt)
{
	ptGptaBase -> ICR = eInt;
}



#endif   /* CSP_GPTA_H */


