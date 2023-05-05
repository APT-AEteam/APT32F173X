/***********************************************************************//** 
 * \file  dac.h
 * \brief dac csi driver head file
 * \copyright Copyright (C) 2015-2022 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-02-28 <td>V0.0  <td>XS    <td>initial
 * </table>
 * *********************************************************************
*/
#ifndef _DRV_DAC_H_
#define _DRV_DAC_H_

#include <drv/common.h>
#include "csp.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
	uint8_t				byClkDiv;		//adc clkdiv, adc clk = PCLK/(2*byClkDiv)
	uint8_t				byRefsel;		//adc sample hold period, sample time = (bySmpHold + 16) clk period
	uint16_t			byDatarset;		//adc conversion  mode, continuous/one shot
	uint8_t				byBufsel;		//adc reference voltage
} csi_dac_config_t;

typedef enum{
	EOC			    = (0x01uL << 0),
	WRERR			= (0x01uL << 1),
	SYNCERR			= (0x01uL << 2),
	
}csi_dac_irq_e;

typedef enum{
	SYNCIN0			= (0x01uL << 0),
	SYNCIN1			= (0x01uL << 1),
	SYNCIN2			= (0x01uL << 2),
	
}csi_dac_syncr_e;
/** \brief initialize dac data structure
 * 
 *  \param[in] ptDacBase: pointer of dac register structure
 *  \param[in] ptDacCfg: pointer of dca parameter config structure
 *  \return error code \ref csi_error_t
 */ 
void csi_dac_init(csp_dac_t *ptDacBase, csi_dac_config_t *ptDacCfg);

/**
  \brief       enable dac power  
  \param[in]   ptDacBase	pointer of dac register structure
  \return      none
*/
void csi_dac_en(csp_dac_t *ptDacBase);

/**
  \brief       disable dac power  
  \param[in]   ptDacBase	pointer of dac register structure
  \return      none
*/
void csi_dac_dis(csp_dac_t *ptDacBase);

/**
  \brief       dac interrupt set 
  \param[in]   ptDacBase	pointer of dac register structure
  \return      none
*/
void csi_dac_irq_enable(csp_dac_t *ptDacBase, csi_dac_irq_e byVal,bool bEnable);

/**
  \brief       dac syncr set 
  \param[in]   ptDacBase	pointer of dac register structure
  \return      none
*/
void csi_dac_syncr_enable(csp_dac_t *ptDacBase, csi_dac_syncr_e byVal,bool bEnable);

/**
  \brief       dac step value set 
  \param[in]   ptDacBase	pointer of dac register structure
  \return      none
*/
void csi_dac_step_val(csp_dac_t *ptDacBase, uint16_t byDer);

/**
  \brief       dac start 
  \param[in]   ptDacBase	pointer of dac register structure
  \return      none
*/
void csi_dac_start(csp_dac_t *ptDacBase);

#endif