/***********************************************************************//** 
 * \file  gpta.h
 * \brief  Enhanced timer
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-9-24 <td>V0.0  <td>WNN   <td>initial
 * <tr><td> 2023-9-07 <td>V1.0  <td>LHY   <td>code normalization
 * </table>
 * *********************************************************************
*/

#ifndef _GPTA_H_
#define _GPTA_H_

#include "stdint.h"
#include "common.h"
#include "csp.h"
#ifdef __cplusplus
extern "C" {
#endif

/*---------------------Enumrate definition--------------------------------*/
typedef enum
{
	GPTA_WORK_CAPTURE = 0,		
    GPTA_WORK_WAVE 	  = 1		
}csi_gpta_workmode_e;

typedef enum
{
	GPTA_RUN_CONT	= 0,			//continuous count mode	
	GPTA_RUN_ONCE 					//once count mode	
}csi_gpta_runmode_e;

typedef enum 
{
	GPTA_CNT_UP = 0,
	GPTA_CNT_DOWN,
	GPTA_CNT_UPDOWN
}csi_gpta_cntmd_e;

typedef enum
{
	GPTA_STOPST_HZ = 0,
	GPTA_STPOST_LOW
}csi_gpta_stopst_e;

typedef enum 
{	
	GPTA_PRDR_A=0,	
	GPTA_CMPA_A,
	GPTA_CMPB_A,
	GPTA_AQCRA_A=8,
	GPTA_AQCRB_A,
	GPTA_AQCSF_A=12,
}csi_gpta_gldcfg_e;

typedef enum
{
	GPTA_SHDW_SHADOW    = 0,    
	GPTA_SHDW_IMMEDIATE 
}csi_gpta_ldmd_e;

typedef enum
{
	GPTA_LDMD_NEVER = 0,
	GPTA_LDMD_ZRO,
	GPTA_LDMD_PRD,
	GPTA_LDMD_LD_SYNC = 4,
}csi_gpta_shdwldmd_e;

typedef enum
{
	GPTA_GLD_DIS = 0,
	GPTA_GLD_EN,
}csi_gpta_gld_en_e;

typedef enum 
{
	GPTA_GLDMD_ZRO = 0,
	GPTA_GLDMD_PRD,
	GPTA_GLDMD_ZRO_PRD,
	GPTA_GLDMD_ZRO_LD_SYNC,
	GPTA_GLDMD_PRD_LD_SYNC,
	GPTA_GLDMD_ZRO_PRD_LD_SYNC,
	GPTA_GLDMD_SW = 0xf
}csi_gpta_gldmd_e;

typedef enum
{
	GPTA_GLD_OSTMD_ANYTIME = 0,
	GPTA_GLD_OSTMD_ONESHOT,
}csi_gpta_gld_ostmd_e;

typedef enum
{
	GPTA_LD_SHDW = 0,
	GPTA_LD_IMMEDIATE	
}csi_gpta_aqldmd_e;

typedef enum
{
	GPTA_TRG_EV0		= 0,	
	GPTA_TRG_EV1,				
}csi_gpta_trgev_e;

typedef enum 
{
	GPTA_AQCSF_NONE=0,
	GPTA_AQCSF_L,
	GPTA_AQCSF_H,
	GPTA_AQCSF_NONE1
}csi_gpta_aqcsf_act_e;

typedef enum 
{
	GPTA_AQCSF_LOAD_NOW=0,
	GPTA_AQCSF_LOAD_ZRO,
	GPTA_AQCSF_LOAD_PRD,
	GPTA_AQCSF_LOAD_ZRO_PRD
}csi_gpta_aqcsf_ldmd_e;

typedef enum
{
	GPTA_AUTO_REARM_DIS = 0,	//disable auto rearm
	GPTA_AUTO_REARM_ZRO,		//CNT = ZRO auto rearm
	GPTA_AUTO_REARM_PRD,		//CNT = PRD auto rearm
	GPTA_AUTO_REARM_ZRO_PRD		//CNT = PRD or PRD auto rearm
}csi_gpta_arearm_e;

typedef enum
{
	GPTA_SYNCIN0	= 0,			
	GPTA_SYNCIN1,						
	GPTA_SYNCIN2,						
	GPTA_SYNCIN3,			
	GPTA_SYNCIN4,
	GPTA_SYNCIN5,
    GPTA_SYNCIN6		
}csi_gpta_syncin_e;

typedef enum
{
	GPTA_SYNC_CONT		= 0,	
	GPTA_SYNC_ONCE								
}csi_gpta_syncmode_e;

typedef enum
{
	GPTA_CNT0INIT		= 0,	//filter input syncin0
	GPTA_CNT1INIT,				//filter input syncin1		
}csi_gpta_cntinit_e;

typedef enum
{
    GPTA_CHANNEL_1=1,
	GPTA_CHANNEL_2
}csi_gpta_channel_e;

typedef enum
{
    GPTA_COMPA=0,
	GPTA_COMPB,
}csi_gpta_comp_e;

typedef enum 
{
	GPTA_CGSRC_CHAX = 0,
	GPTA_CGSRC_CHBX,
}csi_gpta_cgsrc_e;

typedef enum 
{
    GPTA_CGFLT_BP = 0,
	GPTA_CGFLT_2,
	GPTA_CGFLT_4,
	GPTA_CGFLT_6,
	GPTA_CGFLT_8,
	GPTA_CGFLT_16,
	GPTA_CGFLT_32,
	GPTA_CGFLT_64
}csi_gpta_cgflt_e;

typedef enum 
{
	GPTA_SW = 0,
	GPTA_SYNC
}csi_gpta_startmd_e;

typedef enum
{
	GPTA_LDPSCR_ZRO = 0,
	GPTA_LDPSCR_PRD,
	GPTA_LDPSCR_ZROPRD
}csi_gpta_ldpscr_e;

typedef enum
{
	GPTA_CAP_MERGE = 0,
	GPTA_CAP_SEPARATE
}csi_gpta_capmode_e;

typedef enum 
{
	GPTA_ACT_NA = 0,
	GPTA_ACT_LO,
	GPTA_ACT_HI,
	GPTA_ACT_TG	
}csi_gpta_action_e;

typedef enum
{
	GPTA_INTSRC_NONE   = 0x0,
	GPTA_INTSRC_TRGEV0 = 0x1,
	GPTA_INTSRC_TRGEV1 = 0x2,
	GPTA_INTSRC_CAPLD0 = 0x1 << 4,
	GPTA_INTSRC_CAPLD1 = 0x1 << 5,
	GPTA_INTSRC_CAPLD2 = 0x1 << 6,
	GPTA_INTSRC_CAPLD3 = 0x1 << 7,
	GPTA_INTSRC_CAU    = 0x1 << 8,
	GPTA_INTSRC_CAD    = 0x1 << 9,
	GPTA_INTSRC_CBU    = 0x1 << 10,
	GPTA_INTSRC_CBD    = 0x1 << 11,	
	GPTA_INTSRC_PEND   = 0x1 << 16,
	GPTA_INTSRC_ALL    = 0x1FFFF,	
}csi_gpta_intsrc_e;

typedef enum
{
	GPTA_TRGOUT0_SEL = 0,
	GPTA_TRGOUT1_SEL
}csi_gpta_trgout_e;

typedef enum
{
	GPTA_TRG0SRC_DIS = 0,
	GPTA_TRGSRC_ZRO,
	GPTA_TRGSRC_PRD,
	GPTA_TRGSRC_ZRO_PRD,
	GPTA_TRGSRC_CMPA_R,
	GPTA_TRGSRC_CMPA_F,
	GPTA_TRGSRC_CMPB_R,
	GPTA_TRGSRC_CMPB_F,
	
	GPTA_TRGSRC_SYNC = 0xc,
}csi_gpta_trgsrc_e;

typedef enum
{
	GPTA_FLTSRC_DISABLE	= 0,
	GPTA_FLTSRC_SYNCIN0,			
	GPTA_FLTSRC_SYNCIN1,						
	GPTA_FLTSRC_SYNCIN2,						
	GPTA_FLTSRC_SYNCIN3,			
	GPTA_FLTSRC_SYNCIN4,
	GPTA_FLTSRC_SYNCIN5,
    GPTA_FLTSRC_SYNCIN6		
}csi_gpta_fltsrc_e;

typedef enum
{
	GPTA_WININV_DIS =0,
	GPTA_WININV_EN,
}csi_gpta_wininv_e;

typedef enum
{
	GPTA_ALIGNMD_PRD =0,
	GPTA_ALIGNMD_ZRO,
	GPTA_ALIGNMD_ZRO_PRD,
	GPTA_ALIGNMD_T1
}csi_gpta_alignmd_e;

typedef enum
{
	GPTA_CROSSMD_DIS =0,
	GPTA_CROSSMD_EN,
}csi_gpta_crossmd_e;

/*---------------------Struct variables--------------------------------*/
typedef struct 
{
    void(*callback)(csp_gpta_t *ptGptaBase, uint32_t wIsr);
} csi_gpta_ctrl_t;

extern csi_gpta_ctrl_t g_tGptaCtrl[GPTA_IDX];

typedef struct 
{
	uint32_t				  wTimeVal;		   //GPTA Timing Value
    csi_gpta_runmode_e  	  eRunMode;        //GPTA run mode: continuous/once
}csi_gpta_time_config_t;

typedef struct  {
	csi_gpta_workmode_e		  eWorkMode;       //GPTA work mode:CAPTURE/WAVE
	csi_gpta_cntmd_e     	  eCountMode;      //GPTA count mode:UP/DOWN/UPDOWN
	csi_gpta_runmode_e     	  eRunMode;        //GPTA run mode: continuous/once
	csi_gpta_capmode_e		  eCapMode;        //GPTA capture mode:SEPERATE/MERGE
	uint8_t     			  byCapStopWrap;   //GPTA capture counts
	uint8_t     			  byCapLdaret;	   //GPTA when CMPA load,counter value reset or not
	uint8_t     			  byCapLdbret;	   //GPTA when CMPB load,counter value reset or not
	uint8_t     			  byCapLdcret;	   //GPTA when CMPAA load,counter value reset or not
	uint8_t     			  byCapLddret;	   //GPTA when CMPBA load,counter value reset or not
}csi_gpta_capture_config_t;

typedef struct 
{
	csi_gpta_workmode_e		  eWorkMode;       //GPTA work mode:CAPTURE/WAVE
	csi_gpta_cntmd_e     	  eCountMode;      //GPTA count mode:UP/DOWN/UPDOWN
	csi_gpta_runmode_e     	  eRunMode;        //GPTA run mode: continuous/once
	uint8_t					  byDutyCycle;	   //GPTA PWM OUTPUT duty cycle	
	uint32_t				  wFreq;		   //GPTA PWM OUTPUT frequency 
}csi_gpta_pwm_config_t;

typedef struct 
{
    csi_gpta_action_e		  eActionZro;      //ZRO event,wave action config:NA/LOW/HIGH/TOGGLE
	csi_gpta_action_e     	  eActionPrd;      //PRD event,wave action config:NA/LOW/HIGH/TOGGLE
	csi_gpta_action_e     	  eActionC1u;      //C1U event,wave action config:NA/LOW/HIGH/TOGGLE
    csi_gpta_action_e      	  eActionC1d;      //C1D event,wave action config:NA/LOW/HIGH/TOGGLE
	csi_gpta_action_e     	  eActionC2u;      //C2U event,wave action config:NA/LOW/HIGH/TOGGLE
	csi_gpta_action_e     	  eActionC2d;      //C2D event,wave action config:NA/LOW/HIGH/TOGGLE
	csi_gpta_action_e     	  eActionT1u;      //T1U event,wave action config:NA/LOW/HIGH/TOGGLE
	csi_gpta_action_e     	  eActionT1d;      //T1D event,wave action config:NA/LOW/HIGH/TOGGLE
	csi_gpta_action_e     	  eActionT2u;      //T2U event,wave action config:NA/LOW/HIGH/TOGGLE
	csi_gpta_action_e     	  eActionT2d;      //T2D event,wave action config:NA/LOW/HIGH/TOGGLE
    csi_gpta_comp_e    	 	  eC1sel;		   //C1 selection:CMPA/CMPB
	csi_gpta_comp_e     	  eC2sel;		   //C1 selection:CMPA/CMPB
}csi_gpta_pwm_channel_config_t;

typedef struct 
{
	csi_gpta_gld_en_e 		  eGldEnable;	   //Global load enable control
	csi_gpta_gldmd_e 		  eGldMode;		   //Global load mode
	csi_gpta_gld_ostmd_e 	  eGldOstMode;	   //Global oneshot control
	uint8_t 				  byGldprd;		   //Global load period select
	uint8_t 				  byGldcnt;		   //Global event count
}csi_gpta_global_load_config_t;

typedef struct 
{
	csi_gpta_fltsrc_e		  eFltSrc;			//Filter input signal source
	csi_gpta_wininv_e		  eWinInv;			//Filter window inversion control
	csi_gpta_alignmd_e		  eAlignMode;		//Filter window alignment mode control
	csi_gpta_crossmd_e		  eCrossMode;		//Filter window cross mode control
	uint16_t				  hwWinOffset;		//Filter window offset config
	uint16_t 				  hwWinWidth;		//Filter window width config	
}csi_gpta_filter_config_t;

typedef struct 
{
	uint8_t		byPrdr;							//REGLK control:PRDR	
	uint8_t		byCmpa;							//REGLK control:CMPA
	uint8_t		byCmpb;							//REGLK control:CMPB	
	uint8_t		byGld2;							//REGLK control:GLDCR2
	uint8_t 	byRssr;		    				//REGLK control:RSSR     
	uint8_t 	byEmslclr;						//REGLK control:EMSLCLR
	uint8_t 	byEmhlclr;						//REGLK control:EMHLCLR
    uint8_t 	byEmicr;						//REGLK control:EMICR
	uint8_t 	byEmfrcr;						//REGLK control:EMFRCR
    uint8_t 	byAqosf;						//REGLK control:AQOSF
	uint8_t 	byAqcsf;						//REGLK control:AQCSF
} csi_gpta_reglk_config_t;


/** \brief capture configuration
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] ptGptaPwmCfg: refer to csi_ept_captureconfig_t
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gpta_capture_init(csp_gpta_t *ptGptaBase, csi_gpta_capture_config_t *ptGptaPwmCfg);

/** \brief wave configuration
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] ptGptaPwmCfg: refer to csi_ept_pwmconfig_t
 *  \return error code \ref csi_error_t
 */
csi_error_t  csi_gpta_pwm_init(csp_gpta_t *ptGptaBase, csi_gpta_pwm_config_t *ptGptaPwmCfg);

/** \brief initialize gpta data structure
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] ptGptaTimCfg of gpta timing parameter config structure
 * 			   	- wTimeVal: timing value, unit: us
 * 			   	- eRunMode: gpta run mode, \ref csi_gpta_runmode_e
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_gpta_timer_init(csp_gpta_t *ptGptaBase,  csi_gpta_time_config_t *ptGptaTimCfg);

/** \brief set gpta count mode
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] eRunMode: gpta runmode, once/continuous
 *  \return none
 */ 
void csi_gpta_set_runmode(csp_gpta_t *ptGptaBase, csi_gpta_runmode_e eRunMode);

/** \brief set gpta burst 
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] byCgsrc \ref csi_gpta_cgsrc_e 
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gpta_set_burst(csp_gpta_t *ptGptaBase,csi_gpta_cgsrc_e eCgsrc);

/** \brief set gpta cgfilter 
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure 
 *  \param[in] eCgflt \ref csi_gpta_cgflt_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gpta_set_cgflt(csp_gpta_t *ptGptaBase,csi_gpta_cgflt_e eCgflt);

/** \brief Channel CMPLDR configuration
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] eLdmd: \ref csi_gpta_ldmd_e
 *  \param[in] eShowmdld: \ref csi_gpta_shdwldmd_e
 *  \param[in] eChannel: \ref csi_gpta_camp_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gpta_set_cmpload(csp_gpta_t *ptGptaBase, csi_gpta_ldmd_e eLdmd, csi_gpta_shdwldmd_e eShowmdld ,csi_gpta_comp_e eChannel);

/** \brief Set Channel
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] ptPwmCfg: \ref csi_gpta_pwm_channel_config_t
 *  \param[in] eChannel:  \ref csi_gpta_channel_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gpta_set_channel(csp_gpta_t *ptGptaBase, csi_gpta_pwm_channel_config_t *ptPwmCfg, csi_gpta_channel_e eChannel);

/** \brief Set Channel AQLDR 
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] eLoadMode: \ref csi_gpta_ldmd_e
 *  \param[in] eShdwLoadMode: \ref csi_gpta_shdwldmd_e
 *  \param[in] eChannel: \ref csi_gpta_channel_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gpta_channel_set_aqload(csp_gpta_t *ptGptaBase, csi_gpta_ldmd_e eLoadMode, csi_gpta_shdwldmd_e eShdwLoadMode ,csi_gpta_channel_e eChannel);

/** \brief  set gpta gldcr
*
*   \param[in]  ptGptaBase: pointer of gpta register structure
*   \param[in]  ptGlobal: refer to csi_gpta_global_load_config_t
*   \return error code \ref csi_error_t
*/
csi_error_t csi_gpta_set_gldcr(csp_gpta_t *ptGptaBase, csi_gpta_global_load_config_t *ptGldCfg);

/** \brief CLDCFG loading
 * 
 *  \param[in] ptGptaBase of gpta register structure
 *  \param[in] eGloCfg:  \ref csi_gpta_gldcfg_e  
 *  \param[in] bEnable：ENABLE or DISABLE
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gpta_set_gldcfg(csp_gpta_t *ptGptaBase, csi_gpta_gldcfg_e eGloCfg, bool bEnable);

/** \brief Software trigger loading
 * 
 *  \param[in] ptGptaBase pointer of gpta register structure
 *  \return none
*/
void csi_gpta_global_sw(csp_gpta_t *ptGptaBase);

/** \brief rearm  loading
 * 
 *  \param[in] ptGptaBase pointer of gpta register structure
 *  \return none
*/
void csi_gpta_global_rearm(csp_gpta_t *ptGptaBase);

/** \brief start gpta
 * 
 *  \param[in] ptGptaBase pointer of gpta register structure
 *  \return none
*/
void csi_gpta_start(csp_gpta_t *ptGptaBase);

/** \brief  stop gpta
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \return none
*/
void csi_gpta_stop(csp_gpta_t *ptGptaBase);

/** \brief set gpta start mode. 
 * 
 *  \param[in] ptGptaBase £ºpointer of gpta register structure
 *  \param[in] eMode \ref csi_gpta_startmd_e
 *  \return none
 */
void csi_gpta_set_start_mode(csp_gpta_t *ptGptaBase, csi_gpta_startmd_e eMode);

/** \brief set gpta stop status
 * 
 *  \param[in] ptGptaBase :   pointer of gpta register structure
 *  \param[in] eStopSt 	 csi_gpta_stopst_e
 *  \return none
 */
void csi_gpta_set_stop_status(csp_gpta_t *ptGptaBase, csi_gpta_stopst_e eStopSt);

/** \brief get counter period to calculate the duty cycle. 
 * 
 *  \param[in] ptGptaBase  :  pointer of gpta register structure
 *  \return counter period (reg data)
 */
uint16_t csi_gpta_get_prdr(csp_gpta_t *ptGptaBase);

/** \brief  set gpta phsr  
 * 
 *  \param[in] ptGptaBase ：pointer of gpta register structure
 *  \param[in] bwPhsr  :    phsr value
 *  \return none
 */
void csi_gpta_set_phsr(csp_gpta_t *ptGptaBase, uint16_t bwPhsr);

/** \brief enable gpta phsr  
 * 
 *  \param[in] ptGptaBase ：pointer of gpta register structure
 *  \return none
 */
void csi_gpta_phsr_enable(csp_gpta_t *ptGptaBase);

/** \brief disable gpta phsr  
 * 
 *  \param[in] ptGptaBase ：pointer of gpta register structure
 *  \return none
 */
void csi_gpta_phsr_disable(csp_gpta_t *ptGptaBase);

/** \brief  update gpta PRDR reg value
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] hwPrdr: gpta PRDR reg  value
 *  \return none
 */
csi_error_t csi_gpta_prdr_update(csp_gpta_t *ptGptaBase, uint16_t hwPrdr);

/** \brief  update gpta CMPx reg value
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] eComp: \ref  csi_gpta_comp_e.select which COMP to set(COMPA or COMPB)
 *  \param[in] hwCmp: gpta COMP reg value
 *  \return none
 */
csi_error_t csi_gpta_cmp_update(csp_gpta_t *ptGptaBase,csi_gpta_comp_e eComp, uint16_t hwCmp);

/** \brief change gpta output dutycycle. 
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] eCh: \ref csi_gpta_comp_e
 *	\param[in] wDuty : cmpx data to be set directly
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gpta_update_ch_duty(csp_gpta_t *ptGptaBase, csi_gpta_comp_e eCh, uint32_t wDuty);

/** \brief enable gpta in debug mode
 * 
 *  \param[in]  ptGptaBase      pointer of gpta register structure
 *  \return none
 */
void csi_gpta_debug_enable(csp_gpta_t *ptGptaBase);

/** \brief disable gpta in debug mode
 * 
 *  \param[in]  ptGptaBase      pointer of gpta register structure
 *  \return none
 */
void csi_gpta_debug_disable(csp_gpta_t *ptGptaBase);

/** \brief enable gpta out trigger 
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] eCh: \ref csi_gpta_trgev_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gpta_evtrg_enable(csp_gpta_t *ptGptaBase, csi_gpta_trgev_e eCh);

/** \brief disable gpta out trigger 
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] eCh: \ref csi_gpta_trgev_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gpta_evtrg_disable(csp_gpta_t *ptGptaBase, csi_gpta_trgev_e eCh);

/**
  \brief   One time software output 
  \param   ptGptaBase    pointer of gpta register structure 
  \param   eChannel	    \refer to csi_gpta_channel_e  		
  \param   eAction 		\refer to csi_gpta_action_e
*/
csi_error_t csi_gpta_set_aqosf(csp_gpta_t *ptGptaBase, csi_gpta_channel_e eChannel, csi_gpta_action_e eAction);

/** \brief  Continuous software waveform loading control
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] eLoadMode:  \refer to csi_gpta_aqcsf_ldmd_e
 *  \return  none
 */
void csi_gpta_set_aqcsf_loadmode(csp_gpta_t *ptGptaBase, csi_gpta_aqcsf_ldmd_e eLoadMode);

/** \brief Continuous software waveform control
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] eChannel    \refer to csi_gpta_channel_e
 *  \param[in] eAction:    \refer to  csi_gpta_aqcsf_act_e
 *  \return  none
 */
csi_error_t csi_gpta_set_aqcsf(csp_gpta_t *ptGptaBase, csi_gpta_channel_e eChannel, csi_gpta_aqcsf_act_e eAction);

/** \brief gpta software trigger
 *  \param[in] ptGptaBase:pointer of gpta register structure
 *  \param[in] eTgrOut: \refer csi_gpta_trgev_e
 *  \return none
 */
void csi_gpta_sw_trg(csp_gpta_t *ptGptaBase, csi_gpta_trgev_e eTgrOut);

/** \brief gpta interrupt enable control 
 *  \param[in] ptGptaBase:pointer of gpta register structure
 *  \param[in] eInt:  \refer to to csi_gpta_intsrc_e
 *  \return CSI_OK;
 */
void csi_gpta_int_enable(csp_gpta_t *ptGptaBase, csi_gpta_intsrc_e eInt);

/** \brief gpta interrupt disable control
 *  \param[in] ptGptaBase:pointer of gpta register structure
 *  \param[in] eInt: \refer csi_gpta_intsrc_e
 *  \return CSI_OK;
 */
void csi_gpta_int_disable(csp_gpta_t *ptGptaBase, csi_gpta_intsrc_e eInt);

/** \brief clear gpta int
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] eInt: \refer csi_gpta_intsrc_e
 *  \return none
 */
void csi_gpta_clr_isr(csp_gpta_t *ptGptaBase, csi_gpta_intsrc_e eInt);

/** \brief gpta sync input evtrg config  
 * 
 *  \param[in] ptGptaBase:pointer of gpta register structure
 *  \param[in] eSyncIn: \refer csi_gpta_syncin_e, gpta sync evtrg input channel(0~5)
 *  \param[in] eTrgMode: \refer csi_gpta_syncmode_e, gpta sync evtrg mode, continuous/once
 *  \param[in] eAutoRearm: \refer csi_gpta_arearm_e 
 *  \return none
 */
void csi_gpta_set_sync(csp_gpta_t *ptGptaBase, csi_gpta_syncin_e eSyncIn, csi_gpta_syncmode_e eSyncMode, csi_gpta_arearm_e eAutoRearm);

/** \brief gpta sync enable  
 * 
 *  \param[in] ptGptaBase:pointer of gpta register structure
 *  \param[in] eSyncIn: \refer csi_gpta_syncin_e, gpta sync evtrg input channel(0~5)
 *  \return none
 */
void csi_gpta_sync_enable(csp_gpta_t *ptGptaBase, csi_gpta_syncin_e eSyncIn);

/** \brief gpta sync disable  
 * 
 *  \param[in] ptGptaBase:pointer of gpta register structure
 *  \param[in] eSyncIn: \refer csi_gpta_syncin_e, gpta sync evtrg input channel(0~5)
 *  \return none
 */
void csi_gpta_sync_disable(csp_gpta_t *ptGptaBase, csi_gpta_syncin_e eSyncIn);

/** \brief gpta extsync input select
 * 
 *  \param[in] ptGptaBase:pointer of gpta register structure
 *  \param[in] eSyncIn: \ref csi_gpta_syncin_e
 *  \param[in] eTrgChx: \ref csi_gpta_trgout_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gpta_set_extsync_chnl(csp_gpta_t *ptGptaBase, csi_gpta_syncin_e eSyncIn, csi_gpta_trgout_e eTrgChx);

/** \brief gpta sync input filter config  
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] ptFilter: pointer of sync input filter parameter config structure
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gpta_set_sync_filter(csp_gpta_t *ptGptaBase, csi_gpta_filter_config_t *ptFilterCfg);

/** \brief rearm gpta sync evtrg  
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] eSyncIn: \ref csi_gpta_syncin_e, gpta sync evtrg input channel(0~5)
 *  \return none
 */
void csi_gpta_sync_rearm(csp_gpta_t *ptGptaBase,csi_gpta_syncin_e eSyncIn);

/** \brief gpta evtrg output config
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] eTrgOut: \ref csi_gpta_trgev_e, evtrg out port(0~1)
 *  \param[in] eTrgSrc: \ref csi_gpta_trgsrc_e(1~15) 
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gpta_set_evtrg(csp_gpta_t *ptGptaBase, csi_gpta_trgev_e eTrgOut, csi_gpta_trgsrc_e eTrgSrc);

/** \brief gpta evtrg cntxinit control
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] byCntChx: evtrg countinit channel(0~3)
 *  \param[in] byCntVal: evtrg cnt value
 *  \param[in] byCntInitVal: evtrg cntxinit value
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gpta_set_evcntinit(csp_gpta_t *ptGptaBase, uint8_t byCntChx, uint8_t byCntVal, uint8_t byCntInitVal);

/**
 \brief  gpta configuration Loading
 \param  ptGptaBase    	pointer of gpta register structure
 \param  ptGlobal           refer to csi_gpta_feglk_config_t
 \return CSI_OK /CSI_ERROR
*/
csi_error_t csi_gpta_set_reglk(csp_gpta_t *ptGptaBase,csi_gpta_reglk_config_t *ptGlobal);

/** \brief gpta interrupt handle weak function
 * 
 *  \param[in] ptGptaBase: pointer of cnta register structure
 *  \return none
 */ 
void csi_gpta_irqhandler(csp_gpta_t *ptGptaBase, uint8_t byIdx);

/** 
  \brief       register gpta interrupt callback function
  \param[in]   ptGptaBase    pointer of cnta register structure
  \param[in]   callback  gpta interrupt handle function
  \return      error code \ref csi_error_t
 */ 
csi_error_t csi_gpta_register_callback(csp_gpta_t *ptGptaBase, void  *callback);

#ifdef __cplusplus
}
#endif

#endif /* _DRV_PWM_H_ */