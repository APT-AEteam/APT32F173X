/***********************************************************************//** 
 * \file  gptb.h
 * \brief  GPTB  
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-6-17 <td>V0.0  <td>ljy   <td>initial
 * <tr><td> 2023-9-18 <td>V0.1  <td>wch   <td>code normalization
 * </table>
 * *********************************************************************
*/

#ifndef _DRV_GPTB_H_
#define _DRV_GPTB_H_

#include "csp.h"

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------Enumrate definition--------------------------------*/
typedef enum
{
	GPTB_WORK_CAPTURE	= 0,		
    GPTB_WORK_WAVE 		= 1		
}csi_gptb_work_mode_e;

typedef enum
{
	GPTB_CLKSRC_PCLK	= 0,		
    GPTB_CLKSRC_SYNCIN4 = 1		
}csi_gptb_clksrc_e;

typedef enum{
	GPTB_RUN_CONT	= 0,			//continuous count mode	
	GPTB_RUN_ONCE 					//once count mode	
}csi_gptb_run_mode_e;

typedef enum {
	GPTB_CNT_UP 		= 0,
	GPTB_CNT_DN,
	GPTB_CNT_UPDN
}csi_gptb_cnt_mode_e;

typedef enum{
	GPTB_PHSDIR_DN = 0,
	GPTB_PHSDIR_UP
}csi_gptb_phsdir_e;

typedef enum{
	GPTB_STP_ST_HZ = 0,
	GPTB_STP_ST_LOW
}csi_gptb_stpst_e;

typedef enum{
	GPTB_CAP_MERGE = 0,
	GPTB_CAP_SEPARATE
}csi_gptb_cap_mode_e;

typedef enum{
	GPTB_CHAX = 0x1,
	GPTB_CHAY,
	GPTB_CHBX,
}csi_gptb_chx_e;

typedef enum{
	GPTB_EMCOAX =0,
	GPTB_EMCOBX,
	GPTB_EMCOAY
}csi_gptb_osrchx_e;

typedef enum {
	GPTB_CGSRC_CHAX = 0,
	GPTB_CGSRC_CHBX,
}csi_gptb_cgsrc_e;

typedef enum {
    GPTB_CGFLT_BP = 0,
	GPTB_CGFLT_2,
	GPTB_CGFLT_4,
	GPTB_CGFLT_6,
	GPTB_CGFLT_8,
	GPTB_CGFLT_16,
	GPTB_CGFLT_32,
	GPTB_CGFLT_64
}csi_gptb_cgflt_e;

typedef enum{
    GPTB_COMPA=0,
	GPTB_COMPB,
}csi_gptb_comp_e;

typedef enum{
    GPTB_CHANNEL_1=1,
	GPTB_CHANNEL_2,
}csi_gptb_channel_e;

typedef enum{
    GPTB_DBCR =0,
	GPTB_DBDTR,
	GPTB_DBDTF,
	GPTB_DCKPSC,		
}csi_gptb_dbldr_e;

typedef enum {
	GPTB_CH_A = 0,
	GPTB_CH_B,
}csi_gptb_chtype_e;

typedef enum {
	GPTB_STMD_START 		= 0,
	GPTB_STMD_START_SYNC
}csi_gptb_start_mode_e;

typedef enum{
	GPTB_STP_HZ = 0,
	GPTB_STP_L
}csi_gptb_stopst_e;

typedef enum{
	GPTB_INTSRC_TRGEV0 		= 0x1 << 0,
	GPTB_INTSRC_TRGEV1 		= 0x1 << 1,
	GPTB_INTSRC_TRGEV2 		= 0x1 << 2,
	GPTB_INTSRC_TRGEV3 		= 0x1 << 3,
	GPTB_INTSRC_CAPLD0 		= 0x1 << 4,
	GPTB_INTSRC_CAPLD1 		= 0x1 << 5,
	GPTB_INTSRC_CAPLD2 		= 0x1 << 6,
	GPTB_INTSRC_CAPLD3 		= 0x1 << 7,
	GPTB_INTSRC_CAU 		= 0x1 << 8,
	GPTB_INTSRC_CAD 		= 0x1 << 9,
	GPTB_INTSRC_CBU 		= 0x1 << 10,
	GPTB_INTSRC_CBD 		= 0x1 << 11,
	
	GPTB_INTSRC_PEND   		= 0x1 << 16,	
	GPTB_INTSRC_PRDMA  		= 0x1 << 17,
	GPTB_INTSRC_ZROMA  		= 0x1 << 18,
	GPTB_INTSRC_ALL  		= 0xFFFFFFFF
}csi_gptb_int_e;

typedef enum{
	GPTB_TRGOUT0_SEL = 0,
	GPTB_TRGOUT1_SEL
}csi_gptb_trgout_e;

typedef enum{
	GPTB_FILT_DIS		= 0,	//filter disable
	GPTB_FILT_SYNCIN0,			//filter input syncin0
	GPTB_FILT_SYNCIN1,			//filter input syncin1	
	GPTB_FILT_SYNCIN2,			//filter input syncin2
	GPTB_FILT_SYNCIN3,			//filter input syncin3	
	GPTB_FILT_SYNCIN4,			//filter input syncin4
	GPTB_FILT_SYNCIN5,			//filter input syncin5	
	GPTB_FILT_SYNCIN6,			//filter input syncin6	
}csi_gptb_filter_insrc_e;

typedef enum{
	GPTB_SYNCIN0	= 0,			
	GPTB_SYNCIN1,						
	GPTB_SYNCIN2,						
	GPTB_SYNCIN3,			
	GPTB_SYNCIN4,
	GPTB_SYNCIN5,
    GPTB_SYNCIN6		
}csi_gptb_syncin_e;

typedef enum{
	GPTB_TRG_EV0		= 0,	
	GPTB_TRG_EV1,				
}csi_gptb_trgev_e;

typedef enum{
	GPTB_CNT0INIT		= 0,	//filter input syncin0
	GPTB_CNT1INIT,				//filter input syncin1		
}csi_gptb_cntinit_e;

typedef enum {
	GPTB_AQCSF_NONE=0,
	GPTB_AQCSF_L,
	GPTB_AQCSF_H,
	GPTB_AQCSF_NONE1
}csi_gptb_aqcsf_act_e;

typedef enum {
	GPTB_AQCSF_LOAD_NOW=0,
	GPTB_AQCSF_LOAD_ZRO,
	GPTB_AQCSF_LOAD_PRD,
	GPTB_AQCSF_LOAD_ZRO_PRD
}csi_gptb_aqcsf_ldmd_e;

typedef enum{
	GPTB_SYNC_CONT		= 0,	
	GPTB_SYNC_ONCE								
}csi_gptb_syncmode_e;

typedef enum{
	GPTB_AUTO_REARM_DIS 	= 0,	//disable auto rearm
	GPTB_AUTO_REARM_ZRO,			//CNT = ZRO auto rearm
	GPTB_AUTO_REARM_PRD,			//CNT = PRD auto rearm
	GPTB_AUTO_REARM_ZRO_PRD		    //CNT = PRD or PRD auto rearm
}csi_gptb_arearm_e;

typedef enum{
	GPTB_LDPSCR_ZRO = 0,
	GPTB_LDPSCR_PRD,
	GPTB_LDPSCR_ZROPRD
}csp_gptb_ldpscr_e;

typedef enum{
	GPTB_SHADOW    = 0,    
	GPTB_IMMEDIATE 
}csi_gptb_ldmd_e;

typedef enum{
	GPTB_LDMD_NEVER = 0,
	GPTB_LDMD_ZRO,
	GPTB_LDMD_PRD,
	GPTB_LDMD_LD_SYNC = 4,
}csi_gptb_shdwldmd_e;

typedef enum{
	GPTB_GLD_DIS = 0,
	GPTB_GLD_EN,
}csi_gptb_gld_en_e;

typedef enum {
	GPTB_GLDMD_ZRO = 0,
	GPTB_GLDMD_PRD,
	GPTB_GLDMD_ZRO_PRD,
	GPTB_GLDMD_ZRO_LD_SYNC,
	GPTB_GLDMD_PRD_LD_SYNC,
	GPTB_GLDMD_ZRO_PRD_LD_SYNC,
	GPTB_GLDMD_SW = 0xf
}csi_gptb_gldmd_e;

typedef enum{
	GPTB_GLD_OSTMD_ANYTIME = 0,
	GPTB_GLD_OSTMD_ONESHOT,
}csi_gptb_gld_ostmd_e;

typedef enum{
	GPTB_DB_IMMEDIATE =0,
	GPTB_DB_SHADOW      
}csi_gptb_dbldmd_e;

typedef enum{
	GPTB_DBLDMD_NEVER = 0,
	GPTB_DBLDMD_ZRO,
	GPTB_DBLDMD_PRD,
	GPTB_DBLDMD_ZRO_PRD	
}csi_gptb_shdwdbldmd_e;

typedef enum {
	GPTB_DCKSEL_TCLK = 0,
	GPTB_DCKSEL_DPSC
}csi_gptb_dbclksrc_e;

typedef enum {
	GPTB_DEDB_AR_BF = 0,
	GPTB_DEDB_BR_BF
}csi_gptb_dedb_e;

typedef enum {
	GPTB_DBOUT_DIS = 0,
	GPTB_DBOUT_BF,
	GPTB_DBOUT_AR,
	GPTB_DBOUT_AR_BF
}csi_gptb_db_outsel_e;

typedef enum {
	GPTB_DBPOL_DIS = 0,
	GPTB_DBPOL_A,
	GPTB_DBPOL_B,
	GPTB_DBPOL_AB
}csi_gptb_db_pol_e;

typedef enum {
	GPTB_DBIN_AR_AF = 0,
	GPTB_DBIN_BR_AF,
	GPTB_DBIN_AR_BF,
	GPTB_DBIN_BR_BF
}csi_gptb_dbcha_insel_e;

typedef enum {
	GPTB_DBSWAP_OUTA_A_OUTB_B = 0,
	GPTB_DBSWAP_OUTA_B_OUTB_B,
	GPTB_DBSWAP_OUTA_A_OUTB_A,
	GPTB_DBSWAP_OUTA_B_OUTB_A
}csi_gptb_dbcha_outswap_e;

typedef enum{	
	GPTB_SLCLRMD_ZRO =0,
	GPTB_SLCLRMD_PRD,
	GPTB_SLCLRMD_ZRO_PRD,
	GPTB_SLCLRMD_SW 
}csi_gptb_slclrmd_e;

typedef enum{
	GPTB_LD_EMOSR_NEVER = 0,
	GPTB_LD_EMOSR_ZRO,
	GPTB_LD_EMOSR_PRD,
	GPTB_LD_EMOSR_ZRO_PRD	
}csi_gptb_ldemosr_e;

typedef enum{
	GPTB_EP0 = 0,
	GPTB_EP1,
	GPTB_EP2,
	GPTB_EP3,
}csi_gptb_ep_e;

typedef enum {
	GPTB_EMPOL_H = 0,
	GPTB_EMPOL_L
}csi_gptb_empol_e;

typedef enum {
	GPTB_EMOUT_HZ,
	GPTB_EMOUT_H,
	GPTB_EMOUT_L,
	GPTB_EMOUT_NONE
}csi_gptb_emout_e;

typedef enum {
	GPTB_EMSRC_EBI0 = 1,
	GPTB_EMSRC_EBI1,
	GPTB_EMSRC_EBI2,
	GPTB_EMSRC_EBI3,
	GPTB_EMSRC_CMP0,
	GPTB_EMSRC_CMP1,
	GPTB_EMSRC_CMP2,
	GPTB_EMSRC_LVD,
	GPTB_EMSRC_ORL0 = 0xe,
	GPTB_EMSRC_ORL1,
}csi_gptb_emsrc_e;

#define GPTB_ORLx_EBI0  1<<0
#define GPTB_ORLx_EBI1  1<<1
#define	GPTB_ORLx_EBI2  1<<2
#define	GPTB_ORLx_EBI3  1<<3
#define	GPTB_ORLx_CMP0  1<<4
#define	GPTB_ORLx_CMP1  1<<5
#define	GPTB_ORLx_CMP2  1<<6

typedef enum{
	GPTB_LCKMD_DIS = 0,
	GPTB_LCKMD_SLCK,
	GPTB_LCKMD_HLCK,
	GPTB_LCKMD_DISABLE
}csi_gptb_ep_lckmd_e;

typedef enum{
	GPTB_EP_FLT_DIS = 0,
	GPTB_EP_FLT_2P,
	GPTB_EP_FLT_4P,
	GPTB_EP_FLT_6P,
	GPTB_EP_FLT_8P,
	GPTB_EP_FLT_16P,
	GPTB_EP_FLT_32P,
	GPTB_EP_FLT_64P
}csi_gptb_epflt_e;

typedef enum
{
	GPTB_OSR_IMMEDIATE=0,
	GPTB_OSR_SHADOW
}csi_gptb_osrshdw_e;

typedef enum {
	GPTB_EM_INTSRC_EP0  = 0x1,
	GPTB_EM_INTSRC_EP1  = 0x1 << 1,
	GPTB_EM_INTSRCT_EP2 = 0x1 << 2,
	GPTB_EM_INTSRC_EP3  = 0x1 << 3,
	GPTB_EM_INTSRC_CPUF = 0x1 << 8,
	GPTB_EM_INTSRC_MEMF = 0x1 << 9,
	GPTB_EM_INTSRC_EOMF = 0x1 << 10,
	GPTB_EM_INTSRC_ALL  = 0xFFFF
}csi_gptb_emint_e;

typedef enum {
	GPTB_ACT_NA = 0,
	GPTB_ACT_LO,
	GPTB_ACT_HI,
	GPTB_ACT_TG	
}csi_gptb_action_e;

typedef enum{
	GPTB_FLTSRC_DISABLE	= 0,
	GPTB_FLTSRC_SYNCIN0,			
	GPTB_FLTSRC_SYNCIN1,						
	GPTB_FLTSRC_SYNCIN2,						
	GPTB_FLTSRC_SYNCIN3,			
	GPTB_FLTSRC_SYNCIN4,
	GPTB_FLTSRC_SYNCIN5,
    GPTB_FLTSRC_SYNCIN6		
}csi_gptb_fltsrc_e;

typedef enum{
	GPTB_WININV_DIS =0,
	GPTB_WININV_EN,
}csi_gptb_wininv_e;

typedef enum{
	GPTB_ALIGNMD_PRD =0,
	GPTB_ALIGNMD_ZRO,
	GPTB_ALIGNMD_ZRO_PRD,
	GPTB_ALIGNMD_T1
}csi_gptb_alignmd_e;

typedef enum{
	GPTB_CROSSMD_DIS =0,
	GPTB_CROSSMD_EN,
}csi_gptb_crossmd_e;

typedef enum{
	GPTB_TRG0SRC_DIS = 0,
	GPTB_TRGSRC_ZRO,
	GPTB_TRGSRC_PRD,
	GPTB_TRGSRC_ZRO_PRD,
	GPTB_TRGSRC_CMPA_R,
	GPTB_TRGSRC_CMPA_F,
	GPTB_TRGSRC_CMPB_R,
	GPTB_TRGSRC_CMPB_F,
	
	GPTB_TRGSRC_SYNC = 0xc,
	GPTB_TRGSRC_EP0,
	GPTB_TRGSRC_EP1,
	GPTB_TRGSRC_EP2
}csi_gptb_trgsrc_e;

typedef enum {	
	GPTB_GLO_PRDR  = 0,
	GPTB_GLO_CMPA  ,
	GPTB_GLO_CMPB  ,
	GPTB_GLO_DBDTR = 5,
	GPTB_GLO_DBDTF,
	GPTB_GLO_DBCR,
	GPTB_GLO_AQCR1,
	GPTB_GLO_AQCR2,
	GPTB_GLO_AQCSF = 12,
	GPTB_GLO_EMOSR
}csi_gptb_gldcfg_e;

/*---------------------Struct variables--------------------------------*/
typedef struct {
	uint32_t				  wTimeVal;		   //GPTB Timing Value
    csi_gptb_run_mode_e  	  eRunMode;        //GPTB run mode: continuous/once
}csi_gptb_time_config_t;

typedef struct {
	csi_gptb_work_mode_e	  eWorkMode;       //GPTB work mode:CAPTURE/WAVE
	csi_gptb_cnt_mode_e       eCountMode;      //GPTBcount mode:UP/DOWN/UPDOWN
	csi_gptb_run_mode_e       eRunMode;        //GPTB run mode: continuous/once
	uint8_t					  byDutyCycle;	   //GPTB PWM OUTPUT duty cycle	
	uint32_t				  wFreq;		   //GPTB PWM OUTPUT frequency 
}csi_gptb_pwm_config_t;

typedef struct  {
	csi_gptb_work_mode_e	  eWorkMode;       //GPTB work mode:CAPTURE/WAVE
	csi_gptb_cnt_mode_e       eCountMode;      //GPTB count mode:UP/DOWN/UPDOWN
	csi_gptb_run_mode_e    	  eRunMode;        //GPTB run mode: continuous/once
	csi_gptb_cap_mode_e		  eCapMode;        //GPTB capture mode:SEPERATE/MERGE
	uint8_t     			  byCapStopWrap;   //GPTB capture counts
	bool     			  	  bCapLdaRst;	   //GPTB when CMPA load,counter value reset or not
	bool     			  	  bCapLdbRst;	   //GPTB when CMPB load,counter value reset or not
	bool     			      bCapLdaaRst;	   //GPTB when CMPAA load,counter value reset or not
	bool     			 	  bCapLdbaRst;	   //GPTB when CMPBA load,counter value reset or not
}csi_gptb_capture_config_t;

typedef struct {
    csi_gptb_action_e		  eActionZRO;      //ZRO event,wave action config:NA/LOW/HIGH/TOGGLE
	csi_gptb_action_e     	  eActionPRD;      //PRD event,wave action config:NA/LOW/HIGH/TOGGLE
	csi_gptb_action_e     	  eActionC1U;      //C1U event,wave action config:NA/LOW/HIGH/TOGGLE
    csi_gptb_action_e      	  eActionC1D;      //C1D event,wave action config:NA/LOW/HIGH/TOGGLE
	csi_gptb_action_e     	  eActionC2U;      //C2U event,wave action config:NA/LOW/HIGH/TOGGLE
	csi_gptb_action_e     	  eActionC2D;      //C2D event,wave action config:NA/LOW/HIGH/TOGGLE
	csi_gptb_action_e     	  eActionT1U;      //T1U event,wave action config:NA/LOW/HIGH/TOGGLE
	csi_gptb_action_e     	  eActionT1D;      //T1D event,wave action config:NA/LOW/HIGH/TOGGLE
	csi_gptb_action_e     	  eActionT2U;      //T2U event,wave action config:NA/LOW/HIGH/TOGGLE
	csi_gptb_action_e     	  eActionT2D;      //T2D event,wave action config:NA/LOW/HIGH/TOGGLE
    csi_gptb_comp_e    	 	  eC1Sel;		   //C1 selection:CMPA/CMPB
	csi_gptb_comp_e     	  eC2Sel;		   //C1 selection:CMPA/CMPB
}csi_gptb_pwm_ch_config_t;

typedef struct {	
	csi_gptb_db_outsel_e      eChxOutSel_S1S0; //DeadZone switch S0S1 config:DeadZone output Source Select
	csi_gptb_db_pol_e         eChxPol_S3S2;    //DeadZone switch S2S3 config:DeadZone output polarity Control
	csi_gptb_dbcha_insel_e    eChxInSel_S5S4;  //DeadZone switch S4S5 config:DeadZone delay module intput Select
	csi_gptb_dbcha_outswap_e  eChxOutSwap_S8S7;//DeadZone switch S7S8 config:DeadZone output Swap Control	
	csi_gptb_dbclksrc_e       eDclkSel;		   //DeadZone Clock Source
	csi_gptb_dedb_e       	  eChxDedb_S6;	   //DeadZone switch S6 config:DeadZone output Select
	uint16_t      			  hwDpsc;          //DeadZone Clock Divider
	uint32_t      			  wRisingEdgeTime; //DeadZone rising edge delay time,unit:ns
	uint32_t      			  wFallingEdgeTime;//DeadZone falling edge delay time,unit:ns
}csi_gptb_deadzone_config_t;

typedef struct {
	csi_gptb_ep_e	  		  eEp;			   //Emergency EP channel select:EP 0/1/2/3
    csi_gptb_emsrc_e  		  eEmSrc;		   //Emergency source:EBIx/CMPx
	csi_gptb_empol_e 		  eEmPol;		   //Emergency polarity:LOW/HIGH
	csi_gptb_epflt_e  		  eFltPace0;	   //Emergency filter0 pace config
	csi_gptb_epflt_e   		  eFltPace1;	   //Emergency filter1 pace config
	csi_gptb_ep_lckmd_e  	  eEpxLockMode;	   //Emergency EP lock mode:SLCK/HLCK
	csi_gptb_slclrmd_e  	  eSlckClrMode;	   //Emergency SLCK clear mode
	csi_gptb_osrshdw_e  	  eOsrShdw;		   //Emergency EMOSR shadow mode
	csi_gptb_ldemosr_e  	  eOsrLoadMode;	   //Emergency EMSOR Shadow load mode
    uint8_t  				  byORL0;		   //Emergency ORL0 source
	uint8_t  				  byORL1;		   //Emergency ORL1 source
}csi_gptb_emergency_config_t;

typedef struct {
	csi_gptb_gld_en_e 		  eGldEnable;	   //Global load enable control
	csi_gptb_gldmd_e 		  eGldMode;		   //Global load mode
	csi_gptb_gld_ostmd_e 	  eGldOstMode;	   //Global oneshot control
	uint8_t 				  byGldprd;		   //Global load period select
	uint8_t 				  byGldcnt;		   //Global event count
}csi_gptb_global_load_config_t;

typedef struct {
	csi_gptb_fltsrc_e		  eFltSrc;			//Filter input signal source
	csi_gptb_wininv_e		  eWinInv;			//Filter window inversion control
	csi_gptb_alignmd_e		  eAlignMode;		//Filter window alignment mode control
	csi_gptb_crossmd_e		  eCrossMode;		//Filter window cross mode control
	uint16_t				  hwWinOffset;		//Filter window offset config
	uint16_t 				  hwWinWidth;		//Filter window width config	
}csi_gptb_filter_config_t;

typedef struct {
	uint8_t					  byPrdr;			//REGLK control:PRDR	
	uint8_t					  byCmpa;			//REGLK control:CMPA		
	uint8_t					  byCmpb;			//REGLK control:CMPB		
	uint8_t					  byGld2;			//REGLK control:GLDCR2		
	uint8_t 				  byRssr;			//REGLK control:RSSR	          
	uint8_t 				  byEmslclr;		//REGLK control:EMSLCLR
	uint8_t 				  byEmhlclr;		//REGLK control:EMHLCLR
    uint8_t 				  byEmicr;			//REGLK control:EMICR
	uint8_t 				  byEmfrcr;			//REGLK control:EMFRCR
    uint8_t 				  byAqosf;			//REGLK control:AQOSF
	uint8_t 				  byAqcsf;			//REGLK control:AQCSF
}csi_gptb_reglk_config_t;

/// \struct csi_gptb_ctrl_t
/// \brief  gptb control handle, not open to users  
typedef struct {		
	void(*callback)(csp_gptb_t *ptGptbBase, uint32_t wIsr, uint16_t hwEmIsr);
} csi_gptb_ctrl_t;

//----------------------------------Functions declaration-------------------------------------
/** \brief gptb interrupt handle  function
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] byIdx: gptb idx(0/1/2/3/4/5)
 *  \return    none
 */
void csi_gptb_irqhandler(csp_gptb_t *ptGptbBase, uint8_t byIdx);

/** \brief  register gptb interrupt callback function
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] callback: gptb callback handle function
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_gptb_register_callback(csp_gptb_t *ptGptbBase, void  *callback);

/**
 \brief  gptb capture configuration
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptGptbCapCfg   \refer csi_gptb_capture_config_t
  * 		- eWorkMode:GPTB work mode \refer csi_gptb_workmode_e
  * 		- eCountMode:GPTB counting mode \refer csi_gptb_cntmd_e
  * 		- eRunMode:GPTB run mode \refer csi_gptb_runmode_e
  * 		- eCapMode:GPTB capture mode \refer csi_gptb_capmode_e  		
  * 		- byCapStopWrap:GPTB capture counts
  * 		- byCapLdaret:CMPA load,counter value reset or not
  * 		- byCapLdbret:CMPB load,counter value reset or not
  * 		- byCapLdcret:CMPAA load,counter value reset or not
  * 		- byCapLddret:CMPBA load,counter value reset or not
 \return none
*/
void csi_gptb_capture_init(csp_gptb_t *ptGptbBase, csi_gptb_capture_config_t *ptGptbCapCfg);

/**
 \brief  gptb pwm configuration
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptGptbPwmCfg   \refer csi_gptb_capture_config_t
  * 		- eWorkMode:GPTB work mode \refer csi_gptb_workmode_e
  * 		- eCountMode:GPTB counting mode \refer csi_gptb_cntmd_e
  * 		- eRunMode:GPTB run mode \refer csi_gptb_runmode_e
  * 		- byDutyCycle:PWM duty cycle  		
  * 		- wFreq:PWM frequency
 \return error code \ref csi_error_t
*/
csi_error_t csi_gptb_pwm_init(csp_gptb_t *ptGptbBase, csi_gptb_pwm_config_t *ptGptbPwmCfg);

/** \brief gptb timer initialize
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] ptGptbTimCfg of gptb timing parameter config structure
 * 			   	- wTimeVal: timing value, unit: us
 * 			   	- eRunMode: gptb run mode, \ref csi_gptb_runmode_e
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_gptb_timer_init(csp_gptb_t *ptGptbBase, csi_gptb_time_config_t *ptGptbTimCfg);

/** \brief set gptb running mode
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eRunMode: gptb running mode, one pulse/continuous, \ref csi_gptb_runmode_e
 *  \return none
 */ 
void csi_gptb_set_run_mode(csp_gptb_t *ptGptbBase, csi_gptb_run_mode_e eRunMode);

/** \brief gptb cgfilter config
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure 
 *  \param[in] eCgflt \ref csi_gptb_cgflt_e
 *  \return none
 */
void csi_gptb_set_cgflt(csp_gptb_t *ptGptbBase,csi_gptb_cgflt_e eCgflt);

/** \brief gptb burst config
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] byCgsrc \ref csi_gptb_cgsrc_e 
 *  \return none
 */
void csi_gptb_set_burst(csp_gptb_t *ptGptbBase,csi_gptb_cgsrc_e eCgsrc);

 /**
 \brief  PWM Channel configuration
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptPwmChCfg   	\refer to csi_gptb_pwm_channel_config_t
 \param  channel        \refer to csi_gptb_channel_e
 \return none
*/
csi_error_t csi_gptb_set_channel(csp_gptb_t *ptGptbBase, csi_gptb_pwm_ch_config_t *ptPwmChCfg, csi_gptb_channel_e eChannel); 
 
/**
 \brief  gptb deadzone init
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptCfg   \refer csi_gptb_deadzone_config_t
  * 		-eChxOutSel_S1S0:DeadZone switch S0S1 config:DeadZone output Source Select. \refer csi_gptb_db_outsel_e
  * 		-eChxPol_S3S2:DeadZone switch S2S3 config:DeadZone output polarity Control. \refer csi_gptb_db_pol_e
  * 		-eChxInSel_S5S4:DeadZone switch S4S5 config:DeadZone delay module intput Select. \refer csi_gptb_dbcha_insel_e
  * 		-eChxOutSwap_S8S7:DeadZone switch S7S8 config:DeadZone output Swap Control. \refer csi_gptb_dbcha_outswap_e
  * 		-eDclkSel:DeadZone Clock Source. \refer csi_gptb_dbclksrc_e
  * 		-eChxDedb_S6:DeadZone switch S6 config:DeadZone output Select. \refer csi_gptb_dedb_e
  * 		-hwDpsc:DeadZone Clock Divider
  * 		-wRisingEdgeTime:DeadZone rising edge delay time,unit:ns
  * 		-wFallingEdgeTime:DeadZone falling edge delay time,unit:ns
 \param  eChannel   \refer csi_gptb_channel_e
 \return CSI_OK /CSI_ERROR
*/
csi_error_t csi_gptb_deadzone_init(csp_gptb_t *ptGptbBase, csi_gptb_deadzone_config_t *ptDeadzoneCfg, csi_gptb_channel_e eChannel);

/**
 \brief  gptb emergency init
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptEmCfg   \refer csi_gptb_emergency_config_t
  * 		-eEp:Emergency EP channel select:EP 0/1/2/3. \refer csi_gptb_ep_e
  * 		-eEmSrc:Emergency source:EBIx/CMPx. \refer csi_gptb_emsrc_e
  * 		-eEmPol:Emergency polarity:LOW/HIGH. \refer csi_gptb_empol_e
  * 		-eFltPace0:Emergency filter0 pace config. \refer csi_gptb_epflt_e
  * 		-eFltPace1:Emergency filter1 pace config. \refer csi_gptb_epflt_e
  * 		-eEpxLockMode:Emergency EP lock mode:SLCK/HLCK. \refer csi_gptb_ep_lckmd_e
  * 		-eSlckClrMode:Emergency SLCK clear mode. \refer csi_gptb_slclrmd_e
  * 		-eOsrShdw:Emergency EMOSR shadow mode. \refer csi_gptb_osrshdw_e
  * 		-eOsrLoadMode:Emergency EMSOR Shadow load mode. \refer csi_gptb_ldemosr_e
  * 		-byORL0:Emergency ORL0 source
  * 		-byORL1:Emergency ORL1 source
 \return CSI_OK /CSI_ERROR
*/
csi_error_t csi_gptb_emergency_init(csp_gptb_t *ptGptbBase, csi_gptb_emergency_config_t *ptEmCfg);

/**
 \brief  State of emergency configuration 
 \param  ptGptbBase    	pointer of gptb register structure
 \param  eOsrch         \refer to csi_gptb_osrchx_e
 \param  eEmout         \refer to csi_gptb_emout_e
 \return CSI_OK /CSI_ERROR
*/
csi_error_t csi_gptb_set_emergency_out(csp_gptb_t *ptGptbBase, csi_gptb_osrchx_e eOsrch, csi_gptb_emout_e eEmOut);

/** \brief Channel CMPLDR configuration
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eLoadMode: 		\ref to csi_gptb_ldmd_e
 *  \param[in] eShdwLoadMode:   \ref to csi_gptb_shdwldmd_e
 *  \param[in] eChannel: 		\ref to csi_gptb_comp_e
 *  \return error code \ref csi_error_t
 */
void csi_gptb_set_cmpload(csp_gptb_t *ptGptbBase, csi_gptb_ldmd_e eLoadMode, csi_gptb_shdwldmd_e eShdwLoadMode,csi_gptb_comp_e eCmp);

/** \brief Channel AQLDR configuration
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eLoadMode: 	  \ref to csi_gptb_ldmd_e
 *  \param[in] eShdwLoadMode: \ref to csi_gptb_shdwldmd_e
 *  \param[in] eChannel:      \ref to csi_gptb_channel_e
 *  \return error code \ref csi_error_t
 */
void csi_gptb_set_aqload(csp_gptb_t *ptGptbBase, csi_gptb_ldmd_e eLoadMode, csi_gptb_shdwldmd_e eShdwLoadMode ,csi_gptb_channel_e eChannel);

/**
 \brief  DeadZoneTime configuration loading 
 \param  ptGptbBase    	pointer of gptb register structure
 \param  eDbldr         \ref to csi_gptb_dbldr_e
 \param  eDbldmd        \ref to csi_gptb_dbldmd_e
 \param  eShdwdbldmd	\ref to csi_gptb_shdwdbldmd_e
 \return CSI_OK /CSI_ERROR
*/
csi_error_t csi_gptb_set_dbload(csp_gptb_t *ptGptbBase, csi_gptb_dbldr_e eDbldr,csi_gptb_dbldmd_e eDbldmd, csi_gptb_shdwdbldmd_e eShdwdbldmd);

/**
 \brief  gptb global load config
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptGldCfg       \refer csi_gptb_global_load_config_t
  * 		- eGldEnable:Global load enable control \refer csi_gptb_gld_en_e
  * 		- eGldMode:Global load mode \refer csi_gptb_gldmd_e
  * 		- eGldOstMode:Global oneshot control \refer csi_gptb_gld_ostmd_e
  * 		- byGldprd:Global load period select   		
  * 		- byGldcnt:Global event count
 \return CSI_OK /CSI_ERROR
*/
void csi_gptb_set_gldcr(csp_gptb_t *ptGptbBase, csi_gptb_global_load_config_t *ptGldCfg);

/** \brief GLDCFG loading
 * 
 *  \param[in] ptGPTBBase of GPTB register structure
 *  \param[in] Glo:  \ref csi_gptb_gldcfg_e
 *  \param[in] bEnable ENABLE or DISABLE
 *  \return CSI_OK
 */
void csi_gptb_set_gldcfg(csp_gptb_t *ptGptbBase ,csi_gptb_gldcfg_e eGloCfg, bool bEnable);

/**
 \brief  Software global load
 \param  ptGptbBase    	pointer of gptb register structure
 \return CSI_OK
*/
void csi_gptb_sw_global_load(csp_gptb_t *ptGptbBase) ;

/**
 \brief  rearm  loading
 \param  ptGptbBase    	pointer of gptb register structure
 \return CSI_OK
*/
void csi_gptb_global_rearm(csp_gptb_t *ptGptbBase);

/** \brief start GPTB counter
 *  \param ptGptbBase:  pointer of gptb register structure
 *  \return none
 */ 
void csi_gptb_start(csp_gptb_t *ptGptbBase);

/**
 \brief stop GPTB counter
 \param ptGptbBase  pointer of gptb register structure
 \return none
*/
void csi_gptb_stop(csp_gptb_t *ptGptbBase);

/**
 \brief GPTB clock source select
 \param ptGptbBase pointer of gptb register structure
 \param eClkSrc \ref to csi_gptb_clksrc_e
 \return none
*/
void csi_gptb_set_clksrc(csp_gptb_t *ptGptbBase,csi_gptb_clksrc_e eClkSrc);

/**
 \brief set GPTB start mode. 
 \param ptGptbBase    pointer of gptb register structure
 \param eMode \ref csi_gptb_start_mode_e
 \return none
*/
void csi_gptb_set_start_mode(csp_gptb_t *ptGptbBase, csi_gptb_start_mode_e eMode);

/**
 \brief set GPTB pwm stop status
 \param ptGptbBase    pointer of gptb register structure
 \param eStpSt \ref csi_gptb_stpst_e
 \return none
*/
void csi_gptb_set_stop_status(csp_gptb_t *ptGptbBase, csi_gptb_stpst_e eStpSt);

/**
 \brief get counter period to calculate the duty cycle. 
 \param ptGptbBase    pointer of gptb register structure
 \return counter period (reg data)
*/
uint16_t csi_gptb_get_prdr(csp_gptb_t *ptGptbBase);

/** \brief  update gptb PRDR reg value
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] hwPrdr: gptb PRDR reg  value
 *  \return none
 */
void csi_gptb_prdr_update(csp_gptb_t *ptGptbBase, uint16_t hwPrdr);

/** \brief  update gptb CMPx reg value
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eComp: \ref  csi_gptb_comp_e.select which COMP to set(COMPA or COMPB)
 *  \param[in] hwCmp: gptb COMP reg value
 *  \return none
 */
void csi_gptb_cmp_update(csp_gptb_t *ptGptbBase,csi_gptb_comp_e eComp, uint16_t hwCmp);

/** \brief  update gptb pwm freq and duty cycle
 * 
 *  \param[in] ptBtBase: pointer of gptb register structure
 *  \param[in] wFreq: pwm frequency  
 *  \param[in] byDutyCycle: pwm duty cycle(0 -> 100)
 *  \param[in] eComp: \refer to csi_gptb_comp_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_pwm_update(csp_gptb_t *ptGptbBase, uint32_t wFreq, uint8_t byDutyCycle, csi_gptb_comp_e eComp);

/**
 \brief get harklock status
 \param ptGptbBase    pointer of gptb register structure
 \return  0 or 1
*/
bool csi_gptb_get_hdlck_status(csp_gptb_t *ptGptbBase);

/**
 \brief clear harklock status
 \param ptGptbBase   pointer of gptb register structure
 \return  eEp: emergency channel:  \refer to csi_gptb_ep_e                 
*/
void csi_gptb_clr_hdlck(csp_gptb_t *ptGptbBase, csi_gptb_ep_e eEp);

/**
 \brief get softlock status
 \param ptGptbBase  pointer of gptb register structure
 \return  0 or 1
*/
bool csi_gptb_get_sftlck_status(csp_gptb_t *ptGptbBase);

/**
 \brief clear softlock status
 \param ptGptbBase  pointer of gptb register structure
 \return eEp: emergency channel:  \refer to csi_gptb_ep_e
*/
void csi_gptb_clr_sftlck(csp_gptb_t *ptGptbBase, csi_gptb_ep_e eEp);

/** \brief software force emergency
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eEp: emergency channel:  \refer to csi_gptb_ep_e
 *  \return none
 */
void csi_gptb_force_em(csp_gptb_t *ptGptbBase, csi_gptb_ep_e eEp);

/**
  \brief       enable gptb in debug mode
  \param[in]   ptGptbBase  pointer of gptb register structure
*/
void csi_gptb_debug_enable(csp_gptb_t *ptGptbBase);

/**
  \brief       disable gptb in debug mode
  \param[in]   ptGptbBase pointer of gptb register structure
*/
void csi_gptb_debug_disable(csp_gptb_t *ptGptbBase);

/**
  \brief       enable gptb emergency interrupt
  \param[in]   ptGptbBase  pointer of gptb register structure
  \param[in]   eEmint	\refer to csi_gptb_emint_e
*/
void csi_gptb_emint_enable(csp_gptb_t *ptGptbBase, csi_gptb_emint_e eEmint);

/**
  \brief       disable gptb emergency interrupt
  \param[in]   ptGptbBase   pointer of gptb register structure
  \param[in]   eEmint		\refer to csi_gptb_emint_e
*/
void csi_gptb_emint_disable(csp_gptb_t *ptGptbBase, csi_gptb_emint_e eEmint);

/**
  \brief   enable/disable gptb out trigger 
  \param   ptGptbBase   pointer of gptb register structure
  \param   eTrgEv		\refer to csi_gptb_trgev_e
  \return CSI_OK /CSI_ERROR
*/
csi_error_t csi_gptb_evtrg_enable(csp_gptb_t *ptGptbBase, csi_gptb_trgev_e eTrgEv);

/**
  \brief   Onetime software waveform control 
  \param   ptGptbBase    pointer of gptb register structure 
  \param   eChannel	    \refer to csi_gptb_channel_e  		
  \param   eAction 		\refer to csi_gptb_action_e
  \return  none
*/
void csi_gptb_set_aqosf(csp_gptb_t *ptGptbBase, csi_gptb_channel_e eChannel, csi_gptb_action_e eAction);

/** \brief  Continuous software waveform loading control
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eLoadMode:  \refer to csi_gptb_aqcsf_ldmd_e
 *  \return  none
 */
void csi_gptb_set_aqcsf_loadmode(csp_gptb_t *ptGptbBase, csi_gptb_aqcsf_ldmd_e eLoadMode);

/** \brief Continuous software waveform control
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] byCh        \refer to csi_gptb_channel_e
 *  \param[in] eAction:    \refer to  csi_gptb_aqcsf_e
 *  \return  none
 */
void csi_gptb_set_aqcsf(csp_gptb_t *ptGptbBase, csi_gptb_channel_e eChannel, csi_gptb_aqcsf_act_e eAction);

/** \brief gptb interrupt clear  
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eInt:       \ref csi_gptb_int_e
 *  \return none;
 */
void csi_gptb_clr_isr(csp_gptb_t *ptGptbBase, csi_gptb_int_e eInt);

/** \brief gptb interrupt enable  
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eInt:       \ref csi_gptb_int_e
 *  \return none;
 */
void csi_gptb_int_enable(csp_gptb_t *ptGptbBase, csi_gptb_int_e eInt);

/** \brief gptb interrupt disable   
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eInt:       \ref csi_gptb_int_e
 *  \return none;
 */
void csi_gptb_int_disable(csp_gptb_t *ptGptbBase, csi_gptb_int_e eInt);

/** \brief gptb sync input evtrg config  
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eSyncIn:    \refer to csi_gptb_syncin_e
 *  \param[in] eSyncMode:  \refer to csi_gptb_syncmode_e
 *  \param[in] eAutoRearm: \refer to csi_gptb_arearm_e 
 *  \return none
 */
void csi_gptb_set_sync(csp_gptb_t *ptGptbBase, csi_gptb_syncin_e eSyncIn, csi_gptb_syncmode_e eSyncMode, csi_gptb_arearm_e eAutoRearm);

/** \brief gptb sync enable
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eSyncIn: gptb sync input channel, \ref csi_gptb_syncin_e
 *  \return none
 */
void csi_gptb_sync_enable(csp_gptb_t *ptGptbBase, csi_gptb_syncin_e eSyncIn);

/** \brief gptb sync->trgout select
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eSyncIn:  \refer to csi_gptb_syncin_e
 *  \param[in] eTrgOut: \refer to csi_gptb_trgout_e
 *  \return error code \ref csi_error_t
 */
void csi_gptb_set_extsync_bypass(csp_gptb_t *ptGptbBase, csi_gptb_syncin_e eSyncIn, csi_gptb_trgout_e eTrgOut);

/** \brief gptb sync input filter config  
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] ptFilter: pointer of sync input filter parameter config structure
 * 				- eFltSrc:Filter input signal source \refer csi_gptb_fltsrc_e
 * 				- eWinInv:Filter window inversion control \refer csi_gptb_wininv_e
 * 				- eAlignMode:Filter window alignment mode control \refer csi_gptb_alignmd_e
 * 				- eCrossMode:Filter window cross mode control \refer csi_gptb_crossmd_e  		
 * 				- hwWinOffset:Filter window offset config
 * 				- hwWinWidth:Filter window width config
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_set_sync_filter(csp_gptb_t *ptGptbBase, csi_gptb_filter_config_t *ptFilterCfg);

/** \brief rearm gptb sync rearm  
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eSyncin: gptb sync evtrg input channel(0~6), \ref to csi_gptb_syncin_e
 *  \return none
 */
void csi_gptb_sync_rearm(csp_gptb_t *ptGptbBase,csi_gptb_syncin_e eSyncin);

/** \brief gptb evtrg output config
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eTrgEv: \ref to csi_gptb_trgev_e
 *  \param[in] eTrgSrc: \ref to csi_gptb_trgsrc_e 
 *  \return error code \ref csi_error_t
 */
void csi_gptb_set_evtrg(csp_gptb_t *ptGptbBase, csi_gptb_trgev_e eTrgEv, csi_gptb_trgsrc_e eTrgSrc);

/** \brief gptb evtrg cntxinit control
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eCntChx: evtrg countinit channel(0~1)   \ref to csi_gptb_cntinit_e
 *  \param[in] byCntVal: evtrg cnt value(1~16)
 *  \param[in] byCntInitVal: evtrg cntxinit value(1~16)
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_set_evcntinit(csp_gptb_t *ptGptbBase, csi_gptb_cntinit_e eCntChx, uint8_t byCntVal, uint8_t byCntInitVal);

/**
 \brief  gptb reg link configuration
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptGlobal       \refer csi_gptb_reglk_config_t
 \return none
*/
void csi_gptb_set_reglk(csp_gptb_t *ptGptbBase,csi_gptb_reglk_config_t *ptGlobal);

#ifdef __cplusplus
}
#endif

#endif /* _DRV_GPTB_H_ */