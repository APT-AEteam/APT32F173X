/***********************************************************************//** 
 * \file  cnta.h
 * \brief header file for cnta driver
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version   <th>Author  <th>Description
 * <tr><td> 2020-9-09 <td>V0.0  <td>ZJY     <td>initial
 * <tr><td> 2021-5-27 <td>V0.1  <td>LQ      <td>modify
 * <tr><td> 2023-09-07 <td>V0.1  <td>LHY    <td>modify
 * </table>
 * *********************************************************************
*/

#ifndef _DRV_CNTA_H_
#define _DRV_CNTA_H_

#include "csp.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \struct csi_cnta_timer_config_t
/// \brief  conta timer parameter configuration
typedef struct {
	csp_cnta_ckdiv_e eClkDiv;  //时钟分频
	csp_cnta_mode_e  eRunMode; //单次还是重复
	uint32_t	     wTime;	   //周期（us）
} csi_cnta_timer_config_t;

/// \struct csi_cnta_pwm_config_t
/// \brief  conta pwm parameter configuration
typedef struct {
	csp_cnta_ckdiv_e eClkDiv;    //时钟分频选择
	uint8_t		     byStartLevel;//计数器pwm初始电平
	uint8_t		     byStopLevel; //计数器pwm结束电平
	uint8_t		     byDutyCycle; //占空比(0-100)
	uint32_t	     wFreq;	     //频率（hz）	
} csi_cnta_pwm_config_t;

typedef enum {
	CNTA_INTSRC_NONE	= (0x00ul << 0),
	CNTA_INTSRC_PENDH	= (0x01ul << 0),
	CNTA_INTSRC_PENDL	= (0x01ul << 1),
	CNTA_INTSRC_PENDHL	= (0x03ul << 0),
}csi_cnta_intsrc_e;

typedef enum
{
	CNTA_POLAR_LOW	= 0,
    CNTA_POLAR_HIGH,       
	CNTA_STOP_LOW    = 0,	
    CNTA_STOP_HIGH   	    
}csi_cnta_pwmlev_t;

typedef enum
{
	CNTA_PWM_CARRIER		= 0,
    CNTA_PWM_ENVELOPE    
}csi_cnta_pwmout_t;

typedef enum
{
	CNTA_REM_STAT_LOW		= 0,
    CNTA_REM_STAT_HIGH    
}csi_cnta_remstat_t;

typedef enum
{
	CNTA_MATCH_CARRIERON_DIS	= 0,
	CNTA_MATCH_CARRIERON_CLR, 
	CNTA_MATCH_CARRIERON_SET       
}csi_cnta_tcmatch_e;

typedef enum
{
	CNTA_PEND_CARRIERON_DIS	= 0,
	CNTA_PEND_CARRIERON_CLR, 
	CNTA_PEND_CARRIERON_SET      
}csi_cnta_tcpend_e;

typedef enum
{
	CNTA_SW_DIS			= (0x00ul),
	CNTA_SW_EN			= (0x01ul)  
}csi_cnta_sw_updata_e;

typedef enum
{
	CNTA_HW_DIS				= (0x00ul),
	CNTA_HW_TCMATCH			= (0x01ul), 
	CNTA_HW_TCPEND			= (0x02ul),
    CNTA_HW_TCMATCH_TCPEND	= (0x03ul)   
}csi_cnta_hw_updata_e;


/// \struct csi_cnta_ctrl_t
/// \brief  cnta control handle, not open to users  
typedef struct 
{
    void(*callback)(csp_cnta_t *ptCntaBase);
} csi_cnta_ctrl_t;

extern csi_cnta_ctrl_t g_tCntaCtrl[CNTA_IDX];


/** \brief initialize cnta data structure
 * 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \param[in] ptContaTimerCfg:point of timer parameter config
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_cnta_timer_init(csp_cnta_t *ptCntaBase,csi_cnta_timer_config_t *ptContaTimerCfg);

/** \brief start cnta
 * 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \return none
 */ 
void csi_cnta_start(csp_cnta_t *ptCntaBase);

/**
  \brief       Stop cnta
  \param[in]   ptCntaBase    pointer of cnta register structure
  \return      None
*/
void csi_cnta_stop(csp_cnta_t *ptCntaBase);

/**
  \brief       Get cnta datah value
  \param[in]   ptCntaBase    pointer of cnta register structure
  \return      cnta datah load value
*/
uint32_t csi_cnta_get_datah_value(csp_cnta_t *cnta);

/** \brief get cnta datal load value
 * 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \return cnta datal load value
 */ 
uint32_t csi_cnta_get_datal_value(csp_cnta_t *ptCntaBase);

/** \brief cnta pwm init 
 * 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \param[in] ptContaPwmCfg:point of pwm parameter config
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_cnta_pwm_init(csp_cnta_t *ptCntaBase,csi_cnta_pwm_config_t *ptContaPwmCfg);

/** \brief updata cnta pwm freq para: (datah and datal value)
 * 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \param[in] hwDatah: rem ouput high time long 
 *  \param[in] hwDatal: rem ouput low time long  
 *  \param[in] eUpdata: data reg soft updata control
 *  \return none
 */
void csi_cnta_pwm_para_updata(csp_cnta_t *ptCntaBase, uint16_t hwDatah, uint16_t hwDatal, csi_cnta_sw_updata_e eUpdata);

/** \brief set cnta tc sync 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \param[in] tcpend_rem: carrieron control of tc pend end 
 *  \param[in] tcmatch_rem: carrieron control of tc pulse match 
 *  \param[in] hw_updata: cadata reg auto updata control
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_cnta_bt0_set_sync(csp_cnta_t *ptCntaBase, csi_cnta_tcpend_e tcpend_rem, csi_cnta_tcmatch_e tcmatch_rem,csi_cnta_hw_updata_e hw_updata);

/** \brief CNTA interrupt enable control
 * 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \param[in] eIntSrc: cnta interrupt source \ref csi_cnta_intsrc_e
 *  \return none
 */ 
void csi_cnta_int_enable(csp_cnta_t *ptCntaBase, csi_cnta_intsrc_e eIntSrc);

/** \brief CNTA interrupt disable control
 * 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \param[in] eIntSrc: cnta interrupt source \ref csi_cnta_intsrc_e
 *  \return none
 */ 
void csi_cnta_int_disable(csp_cnta_t *ptCntaBase, csi_cnta_intsrc_e eIntSrc);

/** \brief cnta interrupt handle weak function
 * 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \return none
 */ 
void csi_cnta_irqhandler(csp_cnta_t *ptCntaBase, uint8_t byIdx);

/** 
  \brief  	   register cnta interrupt callback function
  \param[in]   ptLedBase    pointer of cnta register structure
  \param[in]   callback		cnta interrupt handle function
  \return      error code \ref csi_error_t
 */ 
csi_error_t csi_cnta_register_callback(csp_cnta_t *ptCntaBase, void  *callback);




//---------------------------------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif /* _DRV_TIMER_H_ */