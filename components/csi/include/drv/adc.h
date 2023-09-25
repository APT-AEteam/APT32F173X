/***********************************************************************//** 
 * \file  adc.h
 * \brief  csi adc driver head file
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-12 <td>V0.0  <td>ZJY   <td>initial
 * <tr><td> 2021-1-8  <td>V0.1  <td>WNN   <td>modify
 * </table>
 * *********************************************************************
*/
#ifndef _DRV_ADC_H_
#define _DRV_ADC_H_

#include "csp.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \enum	csi_adc_runmode_e
 * \brief   adc conversion mode
 */
typedef enum
{
	ADC_RUN_ONCE		= 0,	//one shot mode
	ADC_RUN_CONT,			    //continuous mode	
    ADC_RUN_WAIT                //wait for sync mode
}csi_adc_runmode_e;
/**
 * \enum	csi_adc_clksel_e
 * \brief   adc clk select
 */
typedef enum
{
	ADC_CLK_HFOSC		= 0,		//HFOSC
	ADC_CLK_PLLQ,			        //PLLQ	
    ADC_CLK_PCLK                    //PCLK
}csi_adc_clksel_e;
/**
 * \enum	csi_adc_state_e
 * \brief   adc working status
 */
typedef enum{
	ADC_STATE_IDLE			= 0,		//idle		
	ADC_STATE_DOING,					//working
	ADC_STATE_DONE						//complete	
}csi_adc_state_e;

/**
 * \enum	csi_adc_vref_e
 * \brief   adc reference voltage 
 */
typedef enum{
	ADCVERF_VDD_VSS 		= (0x00ul),
	ADCVREF_FVR_VSS			= (0x02ul),
	ADCVREF_VREFP_VSS		= (0x02ul),
	ADCVREF_INTVREF_VSS		= (0x03ul)
}csi_adc_vref_e;

/**
 * \enum	csi_adc_cv_cnt_e
 * \brief   adc continuous repeat sample count
 */
typedef enum{
	ADC_CV_COUNT_1			= 0,		
	ADC_CV_COUNT_2,
	ADC_CV_COUNT_4,
	ADC_CV_COUNT_8,				
	ADC_CV_COUNT_16,
	ADC_CV_COUNT_32,
	ADC_CV_COUNT_64,
	ADC_CV_COUNT_128,					
	ADC_CV_COUNT_256,
	ADC_CV_COUNT_512,
}csi_adc_cv_cnt_e;

/**
 * \enum	csi_adc_avg_cof_e
 * \brief   adc average coefficient 
 */
typedef enum{
	ADC_AVG_COF_1			= 0,		
	ADC_AVG_COF_2,
	ADC_AVG_COF_4,
	ADC_AVG_COF_8,				
	ADC_AVG_COF_16,
	ADC_AVG_COF_32,
	ADC_AVG_COF_64,
	ADC_AVG_COF_128,				
	ADC_AVG_COF_256,
	ADC_AVG_COF_512,
}csi_adc_avg_cof_e;

/**
 * \enum	csi_adc_ch_e
 * \brief   adc input channel
 */
typedef enum{
	ADC_INA0		= 0ul,
	ADC_INA1,
	ADC_INA2,
	ADC_INA3,
	ADC_INA4,
	ADC_INA5,
	ADC_INA6,
	ADC_INA7,
	ADC_INA8,
	ADC_INA9,
	ADC_INA10,
	ADC_INA11,
	ADC_INA12,
	ADC_INA13,
	ADC_INA14,
	ADC_INA15,
	ADC_INA16,
	ADC_INA17,
	ADC_INA18,
	ADC_INA19,
	ADC_INA20,
	ADC_INA21,
	ADC_INA22,
	ADC_INA23,
	ADC_INB0   = 0x40ul,
	ADC_INB1,
	ADC_INB2,
	ADC_INB3,
	ADC_INB4,
	ADC_INB5,
	ADC_INB6,
	ADC_INB7,
	ADC_INB8,
	ADC_INB9,
	ADC_INB10,
	ADC_INB11,
	ADC_INB12,
	ADC_INB13,
	ADC_INB14,
	ADC_INB15,
	ADC_INB16,
	ADC_INB17,
	ADC_INB18,
	ADC_INB19,
	ADC_INB20,
	ADC_INB21,
	ADC_INB22,
	ADC_OPA0X   =0x79ul,
	ADC_OPA1X,
	ADC_OPA2X,
	ADC_OPA3X,
	ADCIN_INTVREF	= 0x7Dul,
	ADCIN_1_5VDD,
	ADCIN_VSS
}csi_adc_ch_e;

/**
 * \enum	csi_adc_cmp_dir_e
 * \brief   adc cmp dir
 */
typedef enum {
	ADC_CMP_H = 0,
	ADC_CMP_L,
}csi_adc_cmp_dir_e;

/**
 * \enum	csi_adc_syncsrc_e
 * \brief   adc sync trigger source; config seqx
 */
typedef enum {
	ADCSYNC_NONE		= 0,
	ADCSYNC_SWTGR,
	ADCSYNC_IN0	,
	ADCSYNC_IN1	,
	ADCSYNC_IN2	,
	ADCSYNC_IN3	,
	ADCSYNC_IN4	,
	ADCSYNC_IN5		
}csi_adc_syncsrc_e;

/**
 * \enum	csi_adc_syncin_e
 * \brief   adc sync trigger in
 */
typedef enum{
	ADC_SYNCIN0			= 0,
	ADC_SYNCIN1,		
	ADC_SYNCIN2,	
	ADC_SYNCIN3,	
	ADC_SYNCIN4,		
	ADC_SYNCIN5  	
}csi_adc_syncin_e;

/**
 * \enum	csi_adc_syncmode_e
 * \brief   adc sync trigger mode
 */
typedef enum{
	ADC_SYNC_CONT		= 0,	//continuous sync mode
	ADC_SYNC_ONCE				//once sync mode				
}csi_adc_syncmode_e;

/**
 * \enum	csi_adc_trgsrc_e
 * \brief   adc event trigger out source
 */
typedef enum{
	ADC_TRGSOURCE_NONE		= 0,
	ADC_TRGSOURCE_EOC,		
	ADC_TRGSOURCE_READY,	
	ADC_TRGSOURCE_OVR,	
	ADC_TRGSOURCE_CMP0H,		
	ADC_TRGSOURCE_CMP0L,		
	ADC_TRGSOURCE_CMP1H,		
	ADC_TRGSOURCE_CMP1L,	
	ADC_TRGSOURCE_SEQEND0,	
	ADC_TRGSOURCE_SEQEND1,
	ADC_TRGSOURCE_SEQEND2,	
	ADC_TRGSOURCE_SEQEND3,
	ADC_TRGSOURCE_SEQEND4,	
	ADC_TRGSOURCE_SEQEND5,
	ADC_TRGSOURCE_SEQEND6,	
	ADC_TRGSOURCE_SEQEND7,
	ADC_TRGSOURCE_SEQEND8,	
	ADC_TRGSOURCE_SEQEND9,
	ADC_TRGSOURCE_SEQEND10,	
	ADC_TRGSOURCE_SEQEND11,
	ADC_TRGSOURCE_SEQEND12,	
	ADC_TRGSOURCE_SEQEND13,
	ADC_TRGSOURCE_SEQEND14,	
	ADC_TRGSOURCE_SEQEND15
}csi_adc_trgsrc_e;

/**
 * \enum	csi_adc_trgout_e
 * \brief   adc event trigger out port
 */
typedef enum{
	ADC_TRGOUT0		= 0,	//trigger out0
	ADC_TRGOUT1				//trigger out1			
}csi_adc_trgout_e;

/**
 * \enum	csi_adc_intsrc_e
 * \brief   adc interrupt source
 */
typedef enum{
	ADC_INTSRC_EOC		= (0x01uL << 0),
	ADC_INTSRC_READY	= (0x01uL << 1),
	ADC_INTSRC_OVR		= (0x01uL << 2),
	ADC_INTSRC_CMP0H	= (0x01uL << 4),
	ADC_INTSRC_CMP0L	= (0x01uL << 5),
	ADC_INTSRC_CMP1H	= (0x01uL << 6),
	ADC_INTSRC_CMP1L	= (0x01uL << 7),
	
	ADC_INTSRC_SEQ0		= (0x01uL << 16),		//SEQX0-15     
	ADC_INTSRC_SEQ1  	= (0x01uL << 17),     
	ADC_INTSRC_SEQ2  	= (0x01uL << 18),     
	ADC_INTSRC_SEQ3  	= (0x01uL << 19),     
	ADC_INTSRC_SEQ4  	= (0x01uL << 20),     
	ADC_INTSRC_SEQ5  	= (0x01uL << 21),     
	ADC_INTSRC_SEQ6 	= (0x01uL << 22),     
	ADC_INTSRC_SEQ7  	= (0x01uL << 23),     
	ADC_INTSRC_SEQ8  	= (0x01uL << 24),     
	ADC_INTSRC_SEQ9  	= (0x01uL << 25),     
	ADC_INTSRC_SEQ10  	= (0x01uL << 26),     
	ADC_INTSRC_SEQ11   	= (0x01uL << 27),     
	ADC_INTSRC_SEQ12   	= (0x01uL << 28),     
	ADC_INTSRC_SEQ13   	= (0x01uL << 29),     
	ADC_INTSRC_SEQ14   	= (0x01uL << 30),     
	ADC_INTSRC_SEQ15  	= (0x01uL << 31),
	ADC_INTSRC_ALL  	= (0xffffffffuL << 0)
}csi_adc_intsrc_e;

typedef struct {
	uint8_t						byClkDiv;		//adc clkdiv, adc clk = PCLK/(2*byClkDiv),clkdiv<62
	uint8_t						bySampHold;		//adc sample hold period, sample time = (bySmpHold + 16) clk period
	csi_adc_clksel_e			eClkSel;		//adc clk select
	csi_adc_runmode_e			eRunMode;		//adc run  mode, continuous/one shot/wait
	csi_adc_vref_e				eVrefSrc;		//adc reference voltage
} csi_adc_config_t;

/// \struct csi_adc_ctrl_t
/// \brief  adc control handle, not open to users  
typedef struct 
{
    void(*callback)(csp_adc_t *ptAdcBase, uint8_t byIsr);
} csi_adc_ctrl_t;

/** \brief  register adc interrupt callback function
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] callback: adc interrupt handle function
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_adc_register_callback(csp_adc_t *ptAdcBase, void  *callback);

/** \brief adc interrupt handler function
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] byIdx: acd idx(0/1)
 *  \return none
 */ 
void csi_adc_irqhandler(csp_adc_t *ptAdcBase, uint8_t byIdx);

/** \brief initialize adc data structure
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] ptAdcCfg: pointer of adc parameter config structure
 * 				- byClkDiv: adc clkdiv, adc clk = PCLK/(2*byClkDiv)
 * 				- eClksel: adc clk select, \ref csi_adc_clksel_e
 *  			- bySampHold: adc sample hold period,unit: us
 *  			- bySampHold: adc sample hold period,unit: us 
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_adc_init(csp_adc_t *ptAdcBase, csi_adc_config_t *ptAdcCfg);

/** \brief config adc sample sequence
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] byChNum: channel number of sequence
 *  \param[in] eInChnl: adc input channel , \ref csi_adc_ch_e
 * 	\param[in] eCvCnt: continuous repeat sample count , \ref csi_adc_cv_cnt_e
 * 	\param[in] eAvgCof: average coefficient , \ref csi_adc_avg_cof_e
 * 	\param[in] eSyncSrc: sync source, \ref csi_adc_syncsrc_e
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_adc_set_seqx(csp_adc_t *ptAdcBase,uint8_t byChNum,csi_adc_ch_e eInChnl,csi_adc_cv_cnt_e eCvCnt,csi_adc_avg_cof_e eAvgCof,csi_adc_syncsrc_e eSyncSrc);

/** \brief config adc sequence total number
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] byChNum: channel number of sequence
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_adc_set_seq_num(csp_adc_t *ptAdcBase,uint8_t byChNum);

/** \brief start adc 
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_adc_start(csp_adc_t *ptAdcBase);

/** \brief stop adc 
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \return error code \ref csi_error_t
 */  
csi_error_t csi_adc_stop(csp_adc_t *ptAdcBase);

/** \brief set adc run mode, continue/one shot/wait
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] eRunMode: conversion mode, continuous/one shot/wait
 *  \return none
 */
void csi_adc_set_runmode(csp_adc_t *ptAdcBase, csi_adc_runmode_e eRunMode);

/** \brief set adc conversion sequence priority
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] byPri: conversion priority
 *  \return none
 */
void csi_adc_set_pri(csp_adc_t *ptAdcBase, uint8_t byPri);

/** \brief get adc value of sequence channel
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] byChIdx: channel id number of sequence
 *  \return error code \ref csi_error_t or adc value
 */
int16_t csi_adc_read_channel(csp_adc_t *ptAdcBase, uint8_t byChIdx);

/** \brief set adc vref
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] eVrefSrc: source of adc reference voltage
 *  \return none
 */  
void csi_adc_set_vref(csp_adc_t *ptAdcBase, csi_adc_vref_e eVrefSrc);

 /** \brief adc cmp0 config
 * 
 *  \param[in] ptAdcBase: pointer of ADC reg structure.
 *  \param[in] byCmpChnl: cmp0 channel (adc seqx)
 *  \param[in] wCmpData: cmp0 data
 *  \param[in] eDir: direction(High/Low)
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_adc_set_cmp0(csp_adc_t *ptAdcBase, uint8_t byCmpChnl, uint32_t wCmpData, csi_adc_cmp_dir_e eDir);

 /** \brief adc cmp1 config
 * 
 *  \param[in] ptAdcBase: pointer of ADC reg structure.
 *  \param[in] byCmpChnl: cmp1 channel (adc seqx)
 *  \param[in] wCmpData: cmp1 data
 *  \param[in] eDir: direction(High/Low)
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_adc_set_cmp1(csp_adc_t *ptAdcBase, uint8_t byCmpChnl, uint32_t wCmpData, csi_adc_cmp_dir_e eDir);

/** \brief set adc sync 
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] eSyncIn: sync(0~5) of adc input channels
 *  \param[in] eSyncMode: adc sync mode, continuous/once
 *  \param[in] byDelay: adc input delay, delay timer =  (trg_delay+1)*4 PCLK
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_adc_set_sync(csp_adc_t *ptAdcBase, csi_adc_syncin_e eSyncIn, csi_adc_syncmode_e eSyncMode, uint8_t byDelay);

/** \brief rearm adc sync
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] eSyncIn: adc sync evtrg input channel(0~5)
 *  \return none
 */
void csi_adc_sync_rearm(csp_adc_t *ptAdcBase, csi_adc_syncin_e eSyncIn);

/** \brief enable adc sync
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] eSyncIn: adc sync evtrg input channel(0~5)
 *  \return none
 */
void csi_adc_sync_enable(csp_adc_t *ptAdcBase, csi_adc_syncin_e eSyncIn);

/** \brief disable adc sync
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] eSyncIn: adc sync evtrg input channel(0~5)
 *  \return none
 */
void csi_adc_sync_disable(csp_adc_t *ptAdcBase, csi_adc_syncin_e eSyncIn);

/** \brief set adc evtrg output
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] byTrgOut: adc evtrg out port (0~1)
 *  \param[in] eTrgSrc: adc evtrg source(0~23) 
 *  \param[in] byPeriod: The event triggers the count 
 *  \return error code \ref csi_error_t
 */
csi_error_t  csi_adc_set_evtrg(csp_adc_t *ptAdcBase, csi_adc_trgsrc_e eTrgSrc, csi_adc_trgout_e eTrgOut, uint8_t byPeriod);

/** \brief  adc evtrg enable
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] byTrgOut: adc evtrg out port (0~1)
 *  \return error code \ref csi_error_t
 */
void  csi_adc_evtrg_enable(csp_adc_t *ptAdcBase,csi_adc_trgout_e eTrgOut);

/** \brief  adc evtrg disable
 * 
 *  \param[in] ptAdcBase: pointer of adc register structure
 *  \param[in] byTrgOut: adc evtrg out port (0~1)
 *  \return error code \ref csi_error_t
 */
void  csi_adc_evtrg_disable(csp_adc_t *ptAdcBase,csi_adc_trgout_e eTrgOut);

/** \brief enable adc INT status
 * 
 *  \param[in] ptAdcBase: ADC handle to operate
 *  \param[in] eIntSrc:  INT
 */
void csi_adc_int_enable(csp_adc_t *ptAdcBase, csi_adc_intsrc_e eIntSrc);

/** \brief disable adc INT status
 * 
 *  \param[in] ptAdcBase: ADC handle to operate
 *  \param[in] eIntSrc:  INT
 */
void csi_adc_int_disable(csp_adc_t *ptAdcBase, csi_adc_intsrc_e eIntSrc);

/** \brief select adc clock
 * 
 *  \param[in] ptAdcBase: ADC handle to operate
 *  \param[in] eClksel:  ADC clock select
 */
void csi_adc_set_clksrc(csp_adc_t *ptAdcBase,csi_adc_clksel_e eClksel);

#ifdef __cplusplus
}
#endif

#endif /* _DRV_ADC_H_ */
