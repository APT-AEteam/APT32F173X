/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     timer.h
 * @brief    header file for timer driver
 * @version  V1.0
 * @date     27. Apr 2020
 * @model    timer
 ******************************************************************************/

#ifndef _DRV_BT_H_
#define _DRV_BT_H_

#include "csp.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \enum     csi_bt_pwmlev_e
 * \brief    BT pwm output level 
 */
typedef enum
{
	BT_PWM_START_LOW	= 0,		//PWM start level is low 
    BT_PWM_START_HIGH 	= 1,		//PWM start level is high        
	BT_PWM_IDLE_LOW  	= 0,		//PWM idle level is low 
    BT_PWM_IDLE_HIGH	= 1 		//PWM idle level is high   	    
}csi_bt_pwmlev_e;

/**
 * \enum     csi_bt_workmode_e
 * \brief    BT count mode  
 */
typedef enum{
	BT_CNT_CONTINU	= 0,			//continuous count mode	
	BT_CNT_ONCE 					//once count mode	
}csi_bt_wkmode_e;

/**
 * \enum     csi_bt_syncin_e
 * \brief    BT sync trigger input 
 */
typedef enum{
	BT_SYNCIN0	= 0,		//BT sync trigger input0		
	BT_SYNCIN1,				//BT sync trigger input1	
	BT_SYNCIN2				//BT sync trigger input2		
}csi_bt_syncin_e;


/**
 * \enum     csi_bt_trgout_e
 * \brief    BT trigger out
 */
typedef enum{
	BT_TRGOUT0	= 0,		//BT trigger out0		
}csi_bt_trgout_e;

/**
 * \enum     csi_bt_trgmode_e
 * \brief    BT sync trigger mode 
 */
typedef enum{
	BT_TRG_CONTINU	= 0,			//BT continuous trigger mode 
	BT_TRG_ONCE					
}csi_bt_trgmode_e;					//BT once trigger mode 

/**
 * \enum     csi_bt_arearm_e
 * \brief    BT sync arearm mode 
 */
typedef enum{
	BT_TRG_AUTOAREARM	= 1,			//x1：计数器周期结束时，自动AREARM
	BT_TRG_SYCAREARM	                //1x：SYNCIN[1]时，自动AREARM			
}csi_bt_arearm_e;					    //BT  sync arearm mode 


/**
 * \enum     csi_bt_trgsrc_e
 * \brief    BT event trigger source
 */
typedef enum
{
	BT_TRGSRC_DIS	= 0,  			//BT none trigger	       
	BT_TRGSRC_PEND,					//BT PEND as trigger event	
	BT_TRGSRC_CMP,					//BT CMP as trigger event		
	BT_TRGSRC_OVF					//BT CMP as trigger event	         
}csi_bt_trgsrc_e;

/**
 * \enum     csi_bt_intsrc_e
 * \brief    BT interrupt source 
 */
typedef enum
{
	BT_INTSRC_NONE   =	(0x00ul << 0), 		//NONE interrupt
	BT_INTSRC_PEND   =	(0x01ul << 0),		//PEND interrupt
	BT_INTSRC_CMP    =	(0x01ul << 1),		//CMP interrupt   
	BT_INTSRC_OVF    =	(0x01ul << 2), 		//OVF interrupt
	BT_INTSRC_EVTRG  =	(0x01ul << 3)		//EVTRG interrupt
}csi_bt_intsrc_e;

/**
 * \enum     csi_bt_event_e
 * \brief    BT callback event 
 */
typedef enum
{
	BT_EVENT_PEND   =	0, 			//PEND
	BT_EVENT_CMP,
//	BT_EVENT_OVF,   
	BT_EVENT_EVTRG
}csi_bt_event_e;


/// \struct csi_bt_time_config_t
/// \brief  bt timer parameter configuration, open to users  
typedef struct {
	uint32_t			wTimeVal;			//TIMER Timing Value
	csi_bt_wkmode_e		eWkMode;			//TIMER WorkMode: continuous/once
} csi_bt_time_config_t;

/// \struct csi_bt_pwm_config_t
/// \brief  bt pwm parameter configuration, open to users  
typedef struct {
	csi_bt_pwmlev_e	eIdleLevel;			//TIMER PWM OUTPUT idel level
	csi_bt_pwmlev_e	eStartLevel;		//TIMER PWM OUTPUT start Level
	csi_bt_wkmode_e	eWkMode;			//TIMER WorkMode: continuous/once
	uint8_t			byDutyCycle;		//TIMER PWM OUTPUT duty cycle
	uint32_t		wFreq;				//TIMER PWM OUTPUT frequency
} csi_bt_pwm_config_t;


/// \struct csi_bt_ctrl_t
/// \brief  bt control handle, not open to users  
typedef struct 
{
    void(*callback)(csp_bt_t *ptBtBase, uint8_t byIsr);
} csi_bt_ctrl_t;

extern csi_bt_ctrl_t g_tBtCtrl[BT_IDX];

/** 
  \brief  	   register bt interrupt callback function
  \param[in]   ptBtBase		pointer of bt register structure
  \param[in]   callback		bt interrupt handle function
  \return      error code \ref csi_error_t
 */ 
csi_error_t csi_bt_register_callback(csp_bt_t *ptBtBase, void  *callback);

/** 
  \brief  	   bt interrupt handler function
  \param[in]   ptBtBase		pointer of bt register structure
  \param[in]   byIdx		bt idx(0/1/2/3)
  \return 	   none
 */ 
void csi_bt_irqhandler(csp_bt_t *ptBtBase, uint8_t byIdx);

/** 
  \brief initialize bt data structure
  \param[in] ptBtBase		pointer of bt register structure
  \param[in] ptBtTimCfg		ptBtTimCfg: pointer of bt timing parameter config structure
				- wTimeVal	timing value, unit: us
				- eWkMode	bt count work mode, \ref csi_bt_wkmode_e
   \return error code \ref csi_error_t
*/ 
csi_error_t csi_bt_timer_init(csp_bt_t *ptBtBase, csi_bt_time_config_t *ptBtTimCfg);

/** 
  \brief 	   set work mode
  \param[in]   ptBtBase		pointer of bt register structure
  \param[in]   eCntMode		bt count mode, one pulse/continuous
  \return 	   none
 */ 
void csi_bt_count_mode(csp_bt_t *ptBtBase, csi_bt_wkmode_e eWkMode);

/** 
  \brief 	   start bt
  \param[in]   ptBtBase		pointer of bt register structure
  \return      none
 */ 
void csi_bt_start(csp_bt_t *ptBtBase);

/** 
  \brief       stop bt
  \param[in]   ptBtBase		pointer of bt register structure
  \return      none
 */ 
void csi_bt_stop(csp_bt_t *ptBtBase);

/** 
  \brief 	   enable/disable bt interrupt
  \param[in]   ptBtBase	 	pointer of bt register structure
  \param[in]   eIntSrc		bt interrupt source
  \return 	   none
 */ 
void csi_bt_int_enable(csp_bt_t *ptBtBase, csi_bt_intsrc_e eIntSrc);

/** 
  \brief 	   isable bt interrupt
  \param[in]   ptBtBase	 	pointer of bt register structure
  \param[in]   eIntSrc		bt interrupt source
  \return 	   none
 */ 
void csi_bt_int_disable(csp_bt_t *ptBtBase, csi_bt_intsrc_e eIntSrc);

/**
  \brief       Get bt remaining value
  \param[in]   ptBtBase		pointer of bt register structure
  \return      timer		remaining value
*/
uint32_t csi_bt_get_remaining_value(csp_bt_t *ptBtBase);

/**
  \brief       Get bt load value
  \param[in]   ptBtBase		pointer of bt register structure
  \return      timer		load value
*/
uint32_t csi_bt_get_load_value(csp_bt_t *ptBtBase);

/**
  \brief       Check bt is running
  \param[in]   ptBtBase		pointer of bt register structure
  \return      true->running, false->stopped
*/
bool csi_bt_is_running(csp_bt_t *ptBtBase);

/** 
  \brief bt pwm init 
  \param[in] ptBtBase		pointer of bt register structure
  \param[in] ptBtPwmCfg		pointer of bt pwm parameter config structure
				- eIdleLevel	pwm output level of bt idel, \ref csi_bt_pwmlev_e
				- eStartLevel	pwm output level of bt start, \ref csi_bt_wkmode_e
				- eWkMode		bt count work mode, \ref csi_bt_pwmlev_e
				- byDutyCycle	pwm duty cycle, 0~100
				- wFreq: pwm frequency
  \return error code \ref csi_error_t
 */ 
csi_error_t csi_bt_pwm_init(csp_bt_t *ptBtBase, csi_bt_pwm_config_t *ptBtPwmCfg);

/** 
  \brief  	   updata bt para and cmp reg value
  \param[in]   ptBtBase		pointer of bt register structure
  \param[in]   hwPrdr		bt prdr reg value
  \param[in]   hwCmp  		bt cmp reg value
  \return      none
 */
void csi_bt_prdr_cmp_updata(csp_bt_t *ptBtBase, uint16_t hwPrdr, uint16_t hwCmp);

/** 
  \brief  	   updata bt pwm duty cycle
  \param[in]   ptBtBase		pointer of bt register structure
  \param[in]   byDutyCycle	duty cycle(0 -> 100)
  \return 	   none
 */
void csi_bt_pwm_duty_cycle_updata(csp_bt_t *ptBtBase, uint8_t byDutyCycle); 

/** 
  \brief  	   updata bt pwm freq and duty cycle
  \param[in]   ptBtBase		pointer of bt register structure
  \param[in]   wfreq  	  	pwm frequency  
  \param[in]   byDutyCycle	pwm duty cycle(0 -> 100)
  \return 	   none
 */
void csi_bt_pwm_updata(csp_bt_t *ptBtBase, uint32_t wfreq, uint8_t byDutyCycle); 

/** 
  \brief 	   bt sync evtrg config  
  \param[in]   ptBtBase		pointer of bt register structure
  \param[in]   eTrgin		bt evtrg input channel, \ref csi_bt_syncin_e 
  \param[in]   eTrgMode 	bt evtrg mode, \ref csi_bt_trgmode_e 
  \param[in]   bAutoRearm 	auto rearm, \ref csi_bt_trgmode_e  
  \return      none
*/
csi_error_t csi_bt_set_sync(csp_bt_t *ptBtBase,csi_bt_syncin_e eTrgin, csi_bt_trgmode_e eTrgMode, csi_bt_arearm_e bAutoRearm);

/** 
  \brief bt sync enable
  \param[in] ptBtBase: pointer of bt register structure
  \param[in] eSyncIn: bt sync evtrg input channel, \ref csi_bt_syncin_e
  \return none
 */
void csi_bt_sync_enable(csp_bt_t *ptBtBase, csi_bt_syncin_e eSyncIn);

/** 
  \brief bt sync disable
  \param[in] ptBtBase: pointer of bt register structure
  \param[in] eSyncIn: bt sync evtrg input channel, \ref csi_bt_syncin_e
  \return none
 */
void csi_bt_sync_disable(csp_bt_t *ptBtBase, csi_bt_syncin_e eSyncIn);

/** 
  \brief 	   restart bt sync evtrg 
  \param[in]   ptBtBase		pointer of bt register structure
  \param[in]   eTrgin		bt evtrg input channel, \ref csi_bt_syncin_e 
  \return      none
 */
void csi_bt_rearm_sync(csp_bt_t *ptBtBase,csi_bt_syncin_e eTrgin);

/** 
  \brief 	   bt evtrg output config
  \param[in]   ptBtBase		pointer of bt register structure
  \param[in]   eTrgSrc 		bt evtrg source, \ref csi_bt_trgsrc_e 
  \return 	   error code \ref csi_error_t
 */
csi_error_t csi_bt_set_evtrg(csp_bt_t *ptBtBase, csi_bt_trgsrc_e eTrgSrc);

/** 
  \brief bt evtrg output enable
  \param[in] ptBtBase: pointer of bt register structure
  \return none
 */
void csi_bt_evtrg_enable(csp_bt_t *ptBtBase);

/** 
  \brief bt evtrg output disable
  \param[in] ptBtBase: pointer of bt register structure
  \return none
 */
void csi_bt_evtrg_disable(csp_bt_t *ptBtBase);

/** 
  \brief bt software generates a trigger event
  \param[in] ptBtBase:pointer of bt register structure
  \return error code \ref csi_error_t
 */
void csi_bt_soft_evtrg(csp_bt_t *ptBtBase);

/** \brief     start bt by sync event
  \param[in]   ptBtBase		pointer of bt register structure
  \param[in]   wTimeOut 	the timeout for bt, unit: us
  \return      none
 */ 
//void csi_bt_start_sync(csp_bt_t *ptBtBase, uint32_t wTimeOut);

#ifdef __cplusplus
}
#endif

#endif /* _DRV_TIMER_H_ */
