/***********************************************************************//** 
 * \file  opa.h
 * \brief opa csi driver head file
 * \copyright Copyright (C) 2015-2022 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2022-02-17 <td>V0.0  <td>XB    <td>initial
 * </table>
 * *********************************************************************
*/
#ifndef _DRV_OPA_H_
#define _DRV_OPA_H_

#include <drv/common.h>
#include <drv/ringbuf.h>
#include "csp.h"

#ifdef __cplusplus
extern "C" {
#endif

 /**
 * \enum     csi_opa_mode_input_e
 * \brief    opa input mode
 */
typedef enum {
    SINGEL = 0,
    DIFFERENCE
}csi_opa_mode_input_e;

/**
 * \enum     csi_opa_mode_e
 * \brief    opa mode
 */
typedef enum {
    OPA_EXTERNAL_MODE = 0,
	OPA_INTERNAL_MODE ,
}csi_opa_mode_e;


/// \struct csi_opa_config_t
/// \brief  opa parameter configuration, open to users  
typedef struct {
	uint8_t           	byMode;					//Mode set
	uint8_t            	byInternalGain;			//Internal Gain set
	uint8_t            	byInputMode;			//Input Mode set
	uint8_t				byOpaNegativeInput;		//Opa Negative Input set
	uint8_t				byOpaOutput;			//Opa Output set
} csi_opa_config_t;


/**
  \brief       initializes the resources needed for the OPA interface.
  \param[in]   ptOpaBase	pointer of opa register structure
  \param[in]   ptOpaCfg    pointer of opa parameter config structure
  \return      error code.
*/
csi_error_t csi_opa_init(csp_opa_t *ptOpaBase, csi_opa_config_t *ptOpaCfg);


/**
  \brief       start opa Hardware module 
  \param[in]   ptOpaBase	pointer of opa register structure
  \return      error code.
*/
void csi_opa_start(csp_opa_t *ptOpaBase);


/**
  \brief       stop opa Hardware module 
  \param[in]   ptOpaBase	pointer of opa register structure
  \return      error code.
*/
void csi_opa_stop(csp_opa_t *ptOpaBase);

#ifdef __cplusplus
}
#endif

#endif /* _DRV_UART_H_ */
