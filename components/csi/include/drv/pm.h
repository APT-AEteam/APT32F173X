/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     drv/pm.h
 * @brief    header File for pm manage
 * @version  V1.0
 * @date     16. Mar 2020
 ******************************************************************************/

#ifndef _PM_H_
#define _PM_H_

#include <stdint.h>
#include <drv/common.h>
#include <soc.h>
#include <csi_core.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_USER_PM
typedef struct{
	void (*prepare_to_sleep)(void );
	void (*wkup_frm_sleep)(void );
	void (*prepare_to_deepsleep)(void );
	void (*wkup_frm_deepsleep)(void );
}pm_core_t;
#endif

typedef enum {
	
	WKUP_IWDT = 8,
	WKUP_RTC,
	WKUP_LPT,
	WKUP_LVD
	
} csi_wakeup_src_e;

typedef enum {
	SP_IDLE_PCLK	= (0x01ul << 8),	///< IDLE_PCLK for sleep
	SP_IDLE_HCLK 	= (0x01ul << 9),	///< DLE_HCLK for sleep		
	DP_ISOSC 		= (0x01ul << 12),	///< ISOSC for deepsleep				
	DP_IMOSC	 	= (0x01ul << 13),	///< ISOSC for deepsleep	
	DP_ESOSC		= (0x01ul << 14),	///< ISOSC for deepsleep					
	DP_EMOSC		= (0x01ul << 15)	///< ISOSC for deepsleep
} csi_pm_clk_e;


typedef enum {
	PM_MODE_LPRUN		= 0,	///< LowPower Running mode
	PM_MODE_SLEEP,				///< Sleep mode		
	PM_MODE_DEEPSLEEP,			///< Normal DeepSleep mode					
} csi_pm_mode_e;


/**
  \brief       SoC enter low-power mode, each chip's implementation is different
               called by csi_pm_enter_sleep
  \param[in]   mode        low-power mode
  \return      error code
*/
csi_error_t csi_pm_enter_sleep(csi_pm_mode_e eMode);

/**
  \brief       SoC the wakeup source.
  \param[in]   wakeup_num  wakeup source num
  \param[in]   enable      flag control the wakeup source is enable or not
  \return      error code
*/
csi_error_t csi_pm_config_wakeup_source(csi_wakeup_src_e eWkupSrc, bool bEnable);

/**
  \brief       deep sleep mode, osc enable/disable.
  \param[in]   eSrc			\ref csi_pm_clk_e
  \param[in]   enable  		enable/disable deep sleep osc
  \return      error code
*/
void csi_pm_clk_enable(csi_pm_clk_e eOsc, bool bEnable);

/**
  \brief       clear wkalv int status
  \param[in]   eWkAlv		WKUP_ALV0 - WKUP_ALV_ALL
  \return      none
*/
void csi_pm_clr_wkint(uint8_t byWkInt);

/**
  \brief       get wkalv int status
  \param[in]   eWkAlv: WKUP_ALV0~WKUP_ALV_ALL
  \return      none
*/
uint8_t csi_pm_get_wkint(void);

#ifdef __cplusplus
}
#endif

#endif /* _CSI_PM_H_ */
