/***********************************************************************//** 

 * *********************************************************************
*/

#ifndef _GPTB_H_
#define _GPTB_H_

#include "stdint.h"
#include "common.h"
#include "csp_gptb.h"
#ifdef __cplusplus
extern "C" {
#endif
extern uint32_t g_gptb0_prd;
extern uint32_t g_gptb1_prd;

typedef struct csi_gptb_config csi_gptb_config_t;
struct csi_gptb_config {
    uint8_t		byWorkmod;          //Count or capture
	uint8_t     byCountingMode;    //csi_gptb_cntmd_e
	uint8_t     byOneshotMode;     //Single or continuous
	uint8_t     byStartSrc ;
	uint8_t     byPscld;
	uint8_t		byDutyCycle;		 //TIMER PWM OUTPUT duty cycle	
	uint8_t     byCaptureCapLden;
	uint8_t     byCaptureRearm;
	uint8_t     byCaptureCapmd;
	uint8_t     byCaptureStopWrap;
	uint8_t     byCaptureLdaret;
	uint8_t     byCaptureLdbret;
	uint8_t     byCaptureLdcret;
	uint8_t     byCaptureLddret;
	uint8_t     byBurst;
    uint8_t     byCgsrc;
	uint8_t     byCgflt;
	uint32_t	wFreq;				 //TIMER PWM OUTPUT frequency 
	uint32_t    wInt;
};

typedef struct csi_gptb_pwmconfig  csi_gptb_pwmconfig_t;
struct csi_gptb_pwmconfig {
	uint8_t		byWorkmod;          //Count or capture
	uint8_t     byCountingMode;    //csi_gptb_cntmd_e
	uint8_t     byOneshotMode;     //Single or continuous
	uint8_t     byStartSrc ;
	uint8_t     byPscld;
	uint8_t		byDutyCycle;		 //TIMER PWM OUTPUT duty cycle	
	uint32_t	wFreq;				 //TIMER PWM OUTPUT frequency 
	uint32_t    wInt;
};

typedef struct csi_gptb_captureconfig  csi_gptb_captureconfig_t;
struct csi_gptb_captureconfig {
	uint8_t		byWorkmod;          //Count or capture
	uint8_t     byCountingMode;     //csi_gptb_cntmd_e
	uint8_t     byOneshotMode;      //Single or continuous
	uint8_t     byStartSrc ;
	uint8_t     byPscld;
	uint8_t		byDutyCycle;		 //TIMER PWM OUTPUT duty cycle	
 	uint8_t     byCaptureCapLden;
	uint8_t		byCapSrcMode;        //CAPMODE_SEL
	uint8_t     byCaptureRearm;
	uint8_t     byCaptureCapmd;
	uint8_t     byCaptureStopWrap;
	uint8_t     byCaptureLdaret;
	uint8_t     byCaptureLdbret;
	uint8_t     byCaptureLdcret;
	uint8_t     byCaptureLddret;
	uint32_t    wInt;
};

typedef struct csi_gptb_pwmchannel_config      csi_gptb_pwmchannel_config_t;
struct csi_gptb_pwmchannel_config {
	
    uint8_t		byActionZro;          //
	uint8_t     byActionPrd;          //
	uint8_t     byActionC1u;          //
    uint8_t     byActionC1d;          //
	uint8_t     byActionC2u;          //
	uint8_t     byActionC2d;          //
	uint8_t     byActionT1u;          //
	uint8_t     byActionT1d;          //
	uint8_t     byActionT2u;          //
	uint8_t     byActionT2d;          //	
    uint8_t     byChoiceC1sel;
	uint8_t     byChoiceC2sel;
};

typedef struct csi_gptb_deadzone_config   csi_gptb_deadzone_config_t;
struct  csi_gptb_deadzone_config
{	
	uint8_t       byChxOuselS1S0      ;   //
	uint8_t       byChxPolarityS3S2   ;   //
	uint8_t       byChxInselS5S4      ;   //
	uint8_t       byChxOutSwapS8S7    ;   //		
	uint8_t       byDcksel;
	uint8_t       byChaDedb;
	uint8_t       byChbDedb;
	uint8_t       byChcDedb;
	uint16_t      hwDpsc;                 //
	uint32_t      wRisingEdgeTime ;  //
	uint32_t      wFallingEdgeTime;  //
};

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

typedef struct csi_gptb_emergency_config   csi_gptb_emergency_config_t;
struct csi_gptb_emergency_config
{
    uint8_t  byEpxInt ;
	uint8_t  byPolEbix;
    uint8_t	 byEpx;
	uint8_t  byEpxLckmd;
	uint8_t  byFltpace0;
	uint8_t  byFltpace1;
	uint8_t  byOrl0;
	uint8_t  byOrl1;
	uint8_t  byOsrshdw;
	uint8_t  byOsrldmd;
	uint8_t  bySlclrmd;
};

typedef struct  csi_gptb_event_trigger_config   csi_gptb_event_trigger_config_t;
struct  csi_gptb_event_trigger_config{
	uint8_t   byTrg0sel  ;
	uint8_t   byTrg1sel  ;
	uint8_t   byTrg0en   ;
	uint8_t   byTrg1en   ;
	uint8_t   byCnt0initfrc;
	uint8_t   byCnt1initfrc;
	uint8_t   byCnt0initen;
	uint8_t   byCnt1initen;
	uint8_t   byTrg0prd ;
	uint8_t   byTrg1prd ;
	uint8_t   byCnt0init;
	uint8_t   byCnt1init;
	
	uint8_t   byFltipscld;
	uint8_t   byFltckprs;
	uint8_t   bySrcsel;
	uint8_t   byBlkinv;
	uint8_t   byAlignmd;
	uint8_t   byCrossmd;
	uint16_t  hwOffset;
	uint16_t  hwWindow;
	
};

typedef struct csi_gptb_Global_load_control_config    csi_gptb_Global_load_control_config_t;
struct csi_gptb_Global_load_control_config{
	
	bool bGlden;
	bool bOstmd;
	uint8_t byGldprd;	
    uint8_t byGldmd;
	uint8_t byGldcnt;
};

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
}csi_gptb_Global_load_gldcfg_e;

/// \struct csi_gptb_filter_config_t
/// \brief  gptb sync trigger filter parameter configuration, open to users  
typedef struct {
	uint8_t		byFiltSrc;			//filter input signal source
	uint8_t		byWinInv;			//window inversion 
	uint8_t		byWinAlign;			//window alignment 
	uint8_t		byWinCross;			//window cross
	uint16_t	hwWinOffset;		//window offset
	uint16_t 	hwWinWidth;			//window width		
} csi_gptb_filter_config_t;

typedef struct {
	uint8_t		byPrdr;			
	uint8_t		byCmpa;			
	uint8_t		byCmpb;			
	uint8_t		byGld2;			
	uint8_t 	byRssr;		          
	uint8_t 	byEmslclr;
	uint8_t 	byEmhlclr;
    uint8_t 	byEmicr;
	uint8_t 	byEmfrcr;
    uint8_t 	byAqosf;
	uint8_t 	byAqcsf;		
} csi_gptb_reglk_config_t;

typedef enum
{
	GPTB_CAPTURE	= 0,		
    GPTB_WAVE 	= 1		
}csi_gptb_wave_e;

typedef enum {
	GPTB_UPCNT = 0,
	GPTB_DNCNT,
	GPTB_UPDNCNT
}csi_gptb_cntmd_e;


typedef enum{
	GPTB_OP_CONT = 0,
	GPTB_OP_OT,
}csi_gptb_opmd_e;

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
	GPTB_SW = 0,
	GPTB_SYNC
}csi_gptb_stmd_e;

typedef enum{
	GPTB_STP_HZ = 0,
	GPTB_STP_L
}csi_gptb_stopst_e;

typedef enum {
	GPTB_POLARITY_HIGH = 0,
	GPTB_POLARITY_LOW
}csi_gptb_polarity_t;


typedef enum{
	GPTB_CAP_CONT = 0,
	GPTB_CAP_OT
}csi_gptb_capmd_e;

typedef enum{
	GPTBINT_NONE        = 0x0,
	GPTBINT_TRGEV0 		= 0x1 << 0,
	GPTBINT_TRGEV1 		= 0x1 << 1,
	GPTBINT_TRGEV2 		= 0x1 << 2,
	GPTBINT_TRGEV3 		= 0x1 << 3,
	GPTBINT_CAPLD0 		= 0x1 << 4,
	GPTBINT_CAPLD1 		= 0x1 << 5,
	GPTBINT_CAPLD2 		= 0x1 << 6,
	GPTBINT_CAPLD3 		= 0x1 << 7,
	GPTBINT_CAU 		= 0x1 << 8,
	GPTBINT_CAD 		= 0x1 << 9,
	GPTBINT_CBU 		= 0x1 << 10,
	GPTBINT_CBD 		= 0x1 << 11,
	
	GPTBINT_PEND   		= 0x1 << 16,	
	GPTBINT_PRDMA  		= 0x1 << 17,
	GPTBINT_ZROMA  		= 0x1 << 18
}csi_gptb_int_e;

typedef enum{
	GPTB_TRGO_0_SEL = 0,
	GPTB_TRGO_1_SEL
}csi_gptb_syncrout_e;

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

/**
 * \enum     csi_ept_trgin_e
 * \brief    GPTB sync trigger input 
 */
typedef enum{
	GPTB_TRGIN_SYNCEN0	= 0,	//start	up or reset count			
	GPTB_TRGIN_SYNCEN1,			//reg updata				
	GPTB_TRGIN_SYNCEN2,			//capture				
	GPTB_TRGIN_SYNCEN3,			//			
	GPTB_TRGIN_SYNCEN4,			//	
	GPTB_TRGIN_SYNCEN5,			//
    GPTB_TRGIN_SYNCEN6					
}csi_gptb_trgin_e;

/**
 * \enum	csi_gptb_trgout_e
 * \brief   gptb event trigger out port
 */
typedef enum{
	GPTB_TRG_OUT0		= 0,	
	GPTB_TRG_OUT1,				
}csi_gptb_trgout_e;

typedef enum{
	GPTB_CNT0INIT		= 0,	//filter input syncin0
	GPTB_CNT1INIT,				//filter input syncin1		
}csi_gptb_cntinit_e;

typedef enum {
	GPTB_AQCSF_NONE=0,
	GPTB_AQCSF_L,
	GPTB_AQCSF_H,
	GPTB_AQCSF_NONE1
}csi_gptb_aqcsf_e;

typedef enum {
	GPTB_AQCSF_NOW=0,
	GPTB_AQCSF_ZRO,
	GPTB_AQCSF_PRD,
	GPTB_AQCSF_ZRO_PRD
}csi_gptb_aqosf_e;

/**
 * \enum     csi_gptb_trgmode_e
 * \brief    gptb sync trigger mode 
 */
typedef enum{
	GPTB_TRG_CONTINU		= 0,	
	GPTB_TRG_ONCE								
}csi_gptb_trgmode_e;

/**
 * \enum     csi_gptb_arearm_e
 * \brief    v hardware auto rearm 
 */
typedef enum{
	GPTB_AUTO_REARM_DIS 	= 0,	//disable auto rearm
	GPTB_AUTO_REARM_ZRO,			//CNT = ZRO auto rearm
	GPTB_AUTO_REARM_PRD,			//CNT = PRD auto rearm
	GPTB_AUTO_REARM_ZRO_PRD		    //CNT = PRD or PRD auto rearm
}csi_gptb_arearm_e;

//------------------------------------------------------------------------------------------------------------------------
typedef enum{
	GPTB_LDPSCR_ZRO = 0,
	GPTB_LDPSCR_PRD,
	GPTB_LDPSCR_ZROPRD
}csp_gptb_ldpscr_e;

typedef enum{
	GPTB_CAP_MERGE = 0,
	GPTB_CAP_SEPARATE
}csi_gptb_capsrcmd_e;

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
	GPTB_DB_TCLK = 0,
	GPTB_DB_DPSC
}csi_gptb_dbclksrc_e;

typedef enum {
	GPTB_DB_AR_BF = 0,
	GPTB_DB_BR_BF
}csi_gptb_dedb_e;

typedef enum {
	GPTB_DBOUT_DIS = 0,
	GPTB_DBOUT_BF,
	GPTB_DBOUT_AR,
	GPTB_DBOUT_AR_BF
}csi_gptb_db_outsel_e;

typedef enum {
	GPTB_DB_POL_DIS = 0,
	GPTB_DB_POL_A,
	GPTB_DB_POL_B,
	GPTB_DB_POL_AB
}csi_gptb_db_pol_e;

typedef enum {
	GPTB_DBCHAIN_AR_AF = 0,
	GPTB_DBCHAIN_BR_AF,
	GPTB_DBCHAIN_AR_BF,
	GPTB_DBCHAIN_BR_BF
}csi_gptb_dbcha_insel_e;

typedef enum {
	GPTB_DBCHAOUT_OUTA_A_OUTB_B = 0,
	GPTB_DBCHAOUT_OUTA_B_OUTB_B,
	GPTB_DBCHAOUT_OUTA_A_OUTB_A,
	GPTB_DBCHAOUT_OUTA_B_OUTB_A
}csi_gptb_dbcha_outswap_e;

typedef enum {
	GPTB_EMOUT_HZ,
	GPTB_EMOUT_H,
	GPTB_EMOUT_L,
	GPTB_EMOUT_NONE
}csi_gptb_emout_e;

typedef enum {
	GPTB_EBI0 = 1,
	GPTB_EBI1,
	GPTB_EBI2,
	GPTB_EBI3,
	GPTB_CMP0,
	GPTB_CMP1,
	GPTB_CMP2,
	GPTB_LVD,
	GPTB_ORL0 = 0xe,
	GPTB_ORL1,
}csi_gptb_ebi_e;

#define GPTB_ORLx_EBI0  1<<0
#define GPTB_ORLx_EBI1  1<<1
#define	GPTB_ORLx_EBI2  1<<2
#define	GPTB_ORLx_EBI3  1<<3
#define	GPTB_ORLx_CMP0  1<<4
#define	GPTB_ORLx_CMP1  1<<5
#define	GPTB_ORLx_CMP2  1<<6

typedef enum {
	GPTB_EBI_POL_H = 0,
	GPTB_EBI_POL_L
}csi_gptb_ebipol_e;

typedef enum{
	GPTB_STP_ST_HZ = 0,
	GPTB_STP_ST_LOW
}csi_gptb_stpst_e;

typedef enum{
	GPTB_EP0 = 0,
	GPTB_EP1,
	GPTB_EP2,
	GPTB_EP3,
}csi_gptb_ep_e;

typedef enum{
	GPTB_EP_DIS = 0,
	GPTB_EP_SLCK,
	GPTB_EP_HLCK,
	GPTB_EP_DISABLE
}csi_gptb_ep_lckmd_e;

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
	GPTB_EP_FLT0_DIS = 0,
	GPTB_EP_FLT0_2P,
	GPTB_EP_FLT0_4P,
	GPTB_EP_FLT0_6P,
	GPTB_EP_FLT0_8P,
	GPTB_EP_FLT0_16P,
	GPTB_EP_FLT0_32P,
	GPTB_EP_FLT0_64P
}csi_gptb_epflt0_e;
typedef enum{
	GPTB_EP_FLT1_DIS = 0,
	GPTB_EP_FLT1_2P,
	GPTB_EP_FLT1_4P,
	GPTB_EP_FLT1_6P,
	GPTB_EP_FLT1_8P,
	GPTB_EP_FLT1_16P,
	GPTB_EP_FLT1_32P,
	GPTB_EP_FLT1_64P
}csi_gptb_epflt1_e;

typedef enum
{
	GPTB_OSR_IMMEDIATE=0,
	GPTB_OSR_SHADOW
}csi_gptb_Osrshdw_e;

typedef enum {
	GPTB_EMINT_EP0 = 0x1,
	GPTB_EMINT_EP1 = 0x1 << 1,
	GPTB_EMINT_EP2 = 0x1 << 2,
	GPTB_EMINT_EP3 = 0x1 << 3,
	GPTB_EMINT_CPUF= 0x1 << 8,
	GPTB_EMINT_MEMF= 0x1 << 9,
	GPTB_EMINT_EOMF= 0x1 << 10
}csi_gptb_emint_e;

typedef enum {
	GPTB_NA = 0,
	GPTB_LO,
	GPTB_HI,
	GPTB_TG	
}csi_gptb_action_e;

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
//------------------------------------------------------------------------------------------------------------------------
/**
 \brief  Basic configuration
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptGptbPwmCfg   	refer to csi_gptb_config_t
 \return CSI_OK/CSI_ERROR
*/
csi_error_t csi_gptb_config_init(csp_gptb_t *ptGptbBase, csi_gptb_config_t *ptGptbPwmCfg);

 /**
 \brief  capture configuration
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptGptbPwmCfg   	refer to csi_gptb_captureconfig_t
 \return CSI_OK /CSI_ERROR
//*/
csi_error_t csi_gptb_capture_init(csp_gptb_t *ptGptbBase, csi_gptb_captureconfig_t *ptGptbPwmCfg);

 /**
 \brief  wave configuration
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptGptbPwmCfg   	refer to csi_gptb_pwmconfig_t
 \return CSI_OK /CSI_ERROR
*/
csi_error_t  csi_gptb_wave_init(csp_gptb_t *ptGptbBase, csi_gptb_pwmconfig_t *ptGptbPwmCfg);

/** \brief initialize gptb data structure
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] wTimeOut: the timeout for gptb, unit: us, 20us < wTimeOut < 3S
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_gptb_timer_init(csp_gptb_t *ptGptbBase, uint32_t wTimeOut);

/** \brief set gptb count mode
 * 
 *  \param[in] ptGptbBase: pointer of gpta register structure
 *  \param[in] eCntMode: gpta count mode, one pulse/continuous
 *  \return none
 */ 
void csi_gptb_count_mode(csp_gptb_t *ptGptbBase, csi_gptb_opmd_e eCntMode);

/** \brief enable/disable gptb burst 
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] byCgsrc:cgr src 
 *  \param[in] byCgflt:cfg flt
 *  \param[in] bEnable: ENABLE/DISABLE
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_burst_enable(csp_gptb_t *ptGptbBase,uint8_t byCgsrc,uint8_t byCgflt, bool bEnable);

 /**
 \brief  Channel configuration
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptPwmCfg   	    refer to csi_gptb_pwmchannel_config_t
 \param  channel        Channel label
 \return CSI_OK /CSI_ERROR
*/
csi_error_t csi_gptb_channel_config(csp_gptb_t *ptGptbBase, csi_gptb_pwmchannel_config_t *ptPwmCfg, csi_gptb_channel_e eChannel);

/** \brief Channel CMPLDR configuration
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eLdmd: refer to csi_gptb_ldmd_e
 *  \param[in] eShdwldmd: refer to csi_gptb_shdwldmd_e
 *  \param[in] eChannel: refer to csi_gptb_comp_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_channel_cmpload_config(csp_gptb_t *ptGptbBase, csi_gptb_ldmd_e eLdmd, csi_gptb_shdwldmd_e eShdwldmd ,csi_gptb_comp_e eChannel);

/**
 \brief  DeadZoneTime configuration 
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptCfg           refer to csi_ept_deadzone_config_t
 \return CSI_OK /CSI_ERROR
*/
csi_error_t csi_gptb_dz_config(csp_gptb_t *ptGptbBase, csi_gptb_deadzone_config_t *ptCfg);

 /**
 \brief  DeadZoneTime configuration loading 
 \param  ptGptbBase    	pointer of gptb register structure
 \param  eDbldr         refer to csi_gptb_dbldr_e
 \param  eDbldmd         refer to csi_gptb_dbldmd_e
 \param  eShdwdbldmd	       refer to csi_gptb_shdwdbldmd_e
 \return CSI_OK /CSI_ERROR
*/
csi_error_t csi_gptb_dbldrload_config(csp_gptb_t *ptGptbBase, csi_gptb_dbldr_e eDbldr,csi_gptb_dbldmd_e eDbldmd, csi_gptb_shdwdbldmd_e eShdwdbldmd);
   
 /**
 \brief  DeadZoneTime configuration 
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptCfg           refer to csi_gptb_deadzone_config_t
 \return CSI_OK /CSI_ERROR
*/
csi_error_t csi_gptb_dbcr_config(csp_gptb_t *ptGptbBase, csi_gptb_deadzone_config_t *ptCfg);

 /**
 \brief  channelmode configuration 
 \param  ptGptbBase    pointer of gptb register structure
 \param  ptCfg         refer to csi_gptb_deadzone_config_t
 \param  eChannel         refer to csi_gptb_channel_e
 \return CSI_OK /CSI_ERROR  
*/
csi_error_t csi_gptb_channelmode_config(csp_gptb_t *ptGptbBase,csi_gptb_deadzone_config_t *ptCfg,csi_gptb_channel_e eChannel);

/**
 \brief  State of emergency configuration 
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptCfg           refer to csi_gptb_emergency_config_t
 \return CSI_OK /CSI_ERROR
//*/
csi_error_t csi_gptb_emergency_cfg(csp_gptb_t *ptGptbBase, csi_gptb_emergency_config_t *ptCfg);
/**
 \brief  State of emergency configuration 
 \param  ptGptbBase    	pointer of gptb register structure
 \param  eOsrch         refer to csi_gptb_osrchx_e
 \param  eEmout         refer to csi_gptb_emout_e
 \return CSI_OK /CSI_ERROR
//*/
csi_error_t csi_gptb_emergency_pinxout(csp_gptb_t *ptGptbBase,csi_gptb_osrchx_e  eOsrch ,csi_gptb_emout_e eEmout);

/** \brief Channel AQLDR configuration
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eLdmd: refer to csi_gptb_ldmd_e
 *  \param[in] eShdwldmd: refer to csi_gptb_shdwldmd_e
 *  \param[in] eChannel: refer to csi_gptb_channel_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_channel_aqload_config(csp_gptb_t *ptGptbBase, csi_gptb_ldmd_e eLdmd, csi_gptb_shdwldmd_e eShdwldmd ,csi_gptb_channel_e eChannel);

/**
 \brief  gptb configuration Loading
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptGlobal       refer to csi_gptb_Global_load_control_config_t
 \return CSI_OK /CSI_ERROR
*/
csi_error_t csi_gptb_global_config(csp_gptb_t *ptGptbBase,csi_gptb_Global_load_control_config_t *ptGlobal);

/** \brief GLDCFG loading
 * 
 *  \param[in] ptGPTBBase of GPTB register structure
 *  \param[in] Glo:  csi_GPTB_Global_load_gldcfg  
 *  \param[in] bEnable ENABLE or DISABLE
 *  \return CSI_OK
 */
csi_error_t csi_gptb_gldcfg(csp_gptb_t *ptGptbBase ,csi_gptb_Global_load_gldcfg_e Glo,bool bEnable);

/**
 \brief  Software trigger loading
 \param  ptGptbBase    	pointer of gptb register structure
 \return CSI_OK
*/
csi_error_t csi_gptb_global_sw(csp_gptb_t *ptGptbBase);

/**
 \brief  rearm  loading
 \param  ptGptbBase    	pointer of gptb register structure
 \return CSI_OK
*/
csi_error_t csi_gptb_global_rearm(csp_gptb_t *ptGptbBase);

/** \brief start gptb
 *  \param ptGptbBase:  pointer of bt register structure
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_gptb_start(csp_gptb_t *ptGptbBase);

/**
 \brief SW stop GPTB counter
 \param ptGptbBase    pointer of gptb register structure
*/
void csi_gptb_swstop(csp_gptb_t *ptGptbBase);

/**
 \brief set GPTB start mode. 
 \param ptGptbBase    pointer of gptb register structure
 \return eMode 隆锚oGPTB_SW/GPTB_SYNC
*/
void csi_gptb_set_start_mode(csp_gptb_t *ptGptbBase, csi_gptb_stmd_e eMode);

/**
 \brief set GPTB operation mode
 \param ptGptbBase    pointer of gptb register structure
 \param eMode 	 GPTB_OP_CONT/GPTB_OP_OT
*/
void csi_gptb_set_os_mode(csp_gptb_t *ptGptbBase, csi_gptb_opmd_e eMode);

/**
 \brief set GPTB stop status
 \param ptGptbBase    pointer of gptb register structure
 \param eSt 	 GPTB_STP_HZ/GPTB_STP_LOW
*/
void csi_gptb_set_stop_st(csp_gptb_t *ptGptbBase, csi_gptb_stpst_e eSt);

/**
 \brief get counter period to calculate the duty cycle. 
 \param ptGptbBase    pointer of gptb register structure
 \return counter period (reg data)
*/
uint16_t csi_gptb_get_prdr(csp_gptb_t *ptGptbBase);

/** \brief  update gptb PRDR and CMPx reg value
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eComp: select which COMP to set(COMPA or COMPB)
 *  \param[in] hwPrdr: gptb PRDR reg  value
 *  \param[in] hwCmp: gptb COMP reg value
 *  \return none
 */
csi_error_t csi_gptb_prdr_cmp_update(csp_gptb_t *ptGptbBase,csi_gptb_comp_e eComp, uint16_t hwPrdr, uint16_t hwCmp);

/**
 \brief change gptb output dutycycle. 
 \param ptGptbBase   pointer of gptb register structure
 \param eCh          refer to csi_gptb_comp_e
 \param wDuty        duty of PWM:0%-100%
*/
csi_error_t csi_gptb_change_ch_duty(csp_gptb_t *ptGptbBase, csi_gptb_comp_e eCh, uint32_t wDuty);

/**
 \brief get harklock status
 \param ptEpt    pointer of gptb register structure
 \return uint8_t 0 or 1
*/
uint8_t csi_gptb_get_hdlck_st(csp_gptb_t *ptGptbBase);

/**
 \brief clear harklock status
 \param ptEpt   pointer of gptb register structure
 \return  eEp 	 external emergency input: csi_gptb_ep_e                 
*/
void csi_gptb_clr_hdlck(csp_gptb_t *ptGptbBase, csi_gptb_ep_e eEp);

/**
 \brief get 	 softlock status
 \param ptEpt    pointer of gptb register structure
 \return uint8_t 0 or 1
*/
uint8_t csi_gptb_get_sftlck_st(csp_gptb_t *ptGptbBase);

/**
 \brief clear softlock status
 \param ptEpt   pointer of gptb register structure
 \return  eEp 	 external emergency input: csi_gptb_ep_e
*/
void csi_gptb_clr_sftlck(csp_gptb_t *ptGptbBase, csi_gptb_ep_e eEp);

/** \brief software force emergency
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eEp: external emergency input: GPTB_EP0~3
 *  \return none
 */
void csi_gptb_force_em(csp_gptb_t *ptGptbBase, csi_gptb_ep_e eEp);

/**
  \brief       enable/disable gptb in debug mode
  \param[in]   ptEpt      pointer of gptb register structure
  \param[in]   bEnable		ENABLE/DISABLE
*/

void csi_gptb_debug_enable(csp_gptb_t *ptGptbBase, bool bEnable);

/**
  \brief       enable/disable gptb emergencyinterruption
  \param[in]   ptGptbBase       pointer of gptb register structure
  \param[in]   eEmint		       refer to csi_gptb_emint_e
*/
void csi_gptb_emint_en(csp_gptb_t *ptGptbBase, csi_gptb_emint_e eEmint);

/**
  \brief   enable/disable gptb out trigger 
  \param   ptGptbBase   pointer of gptb register structure
  \param   byCh			0/1
  \param   bEnable		ENABLE/DISABLE
*/
csi_error_t csi_gptb_evtrg_enable(csp_gptb_t *ptGptbBase, uint8_t byCh, bool bEnable);

/**
  \brief   One time software output 
  \param   ptGptbBase      pointer of gptb register structure 
  \param   byCh	         GPTB_OSTSFA/GPTB_OSTSFB		
  \param   eAction 		GPTB_LDAQCR_ZRO/GPTB_LDAQCR_PRD/GPTB_LDAQCR_ZROPRD
*/
csi_error_t csi_gptb_onetimesoftwareforce_output(csp_gptb_t *ptGptbBase, csi_gptb_channel_e eChannel, csi_gptb_action_e eAction);

/** \brief  Continuous software waveform loading control
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eLoadtime:    refer to csi_gptb_aqosf_e
 *  \return  none
 */
void csi_gptb_loading_method_aqcsf(csp_gptb_t *ptGptbBase, csi_gptb_aqosf_e eLoadtime);

/** \brief Continuous software waveform control
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] byCh        refer to csi_gptb_channel_e
 *  \param[in] eAction:    refer to  csi_gptb_aqosf_e
 *  \return  none
 */
csi_error_t csi_gptb_continuoussoftwareforce_output(csp_gptb_t *ptGptbBase, csi_gptb_channel_e eChannel, csi_gptb_aqcsf_e eAction);

/** \brief gptb  input  config  
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eInt:     refer to to csp_gptb_int_e
 *  \param[in] bEnable:  ENABLE/DISABLE
 *  \return CSI_OK;
 */
csi_error_t csi_gptb_int_enable(csp_gptb_t *ptGptbBase, csi_gptb_int_e eInt, bool bEnable);

/** \brief gptb sync input evtrg config  
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eTrgin: gptb sync evtrg input channel(0~6)
 *  \param[in] eTrgMode: gptb sync evtrg mode, continuous/once
 *  \param[in] eAutoRearm: refer to csi_gptb_arearm_e 
 *  \return none
 */
void csi_gptb_set_sync(csp_gptb_t *ptGptbBase, csi_gptb_trgin_e eTrgIn, csi_gptb_trgmode_e eTrgMode, csi_gptb_arearm_e eAutoRearm);

/** \brief gptb extsync input select
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eTrgin: gptb sync evtrg input channel(0~6)
 *  \param[in] byTrgChx: trgxsel channel(0~1)
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_set_extsync_chnl(csp_gptb_t *ptGptbBase, csi_gptb_trgin_e eTrgIn, csi_gptb_syncrout_e eTrgChx);

/** \brief gptb sync input filter config  
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] ptFilter: pointer of sync input filter parameter config structure
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_set_sync_filter(csp_gptb_t *ptGptbBase, csi_gptb_filter_config_t *ptFilter);

/** \brief rearm gptb sync evtrg  
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eTrgin: gptb sync evtrg input channel(0~6)
 *  \return none
 */
void csi_gptb_rearm_sync(csp_gptb_t *ptGptbBase,csi_gptb_trgin_e eTrgin);

/** \brief gptb evtrg output config
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] byTrgOut: evtrg out port(0~1)
 *  \param[in] eTrgSrc: evtrg source(1~15) 
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_set_evtrg(csp_gptb_t *ptGptbBase, csi_gptb_trgout_e eTrgOut, csi_gptb_trgsrc_e eTrgSrc);

/** \brief gptb evtrg cntxinit control
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] byCntChx: evtrg countinit channel(0~1)
 *  \param[in] byCntVal: evtrg cnt value(1~16)
 *  \param[in] byCntInitVal: evtrg cntxinit value(1~16)
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_set_evcntinit(csp_gptb_t *ptGptbBase, csi_gptb_cntinit_e eCntChx, uint8_t byCntVal, uint8_t byCntInitVal);

/**
 \brief  gptb configuration Loading
 \param  ptGptbBase    	pointer of gptb register structure
 \param  ptCfg           refer to csi_gptb_reglk_config_t
 \return CSI_OK /CSI_ERROR
*/
csi_error_t csi_gptb_reglk_config(csp_gptb_t *ptGptbBase,csi_gptb_reglk_config_t *ptGlobal);

#ifdef __cplusplus
}
#endif

#endif /* _DRV_PWM_H_ */