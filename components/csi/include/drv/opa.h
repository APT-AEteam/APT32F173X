/***********************************************************************//** 
 * \file  opa.h
 * \brief opa csi driver head file
 * \copyright Copyright (C) 2015-2022 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2022-02-17 <td>V0.0  <td>XB    <td>initial
 * <tr><td> 2023-9-11  <td>V0.1  <td>xs    <td>fix bug,code normalization
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

 /**
 * \enum     csi_opa_pgac_single_e
 * \brief    opa single gain
 */
typedef enum{
	OPA_SINGLE_GAIN_X2			= 0,
	OPA_SINGLE_GAIN_X4,
	OPA_SINGLE_GAIN_X8,
	OPA_SINGLE_GAIN_X16,
	OPA_SINGLE_GAIN_X21,
	OPA_SINGLE_GAIN_X32,
	OPA_SINGLE_GAIN_X42,
	OPA_SINGLE_GAIN_X64
}csi_opa_pgac_single_e;

 /**
 * \enum     csi_opa_pgac_difference_e
 * \brief    opa difference gain 
 */
typedef enum{
	OPA_DIFFERENCE_GAIN_X1		= 0,
	OPA_DIFFERENCE_GAIN_X2,
	OPA_DIFFERENCE_GAIN_X3,
	OPA_DIFFERENCE_GAIN_X5,
	OPA_DIFFERENCE_GAIN_X7,
	OPA_DIFFERENCE_GAIN_X9,
	OPA_DIFFERENCE_GAIN_X11,
	OPA_DIFFERENCE_GAIN_X19
}csi_opa_pgac_difference_e;

/// \struct csi_opa_config_t
/// \brief  opa parameter configuration, open to users  
typedef struct {
	csi_opa_mode_e           	eMode;				//Mode set
	csi_opa_pgac_single_e       eSingleGain;		//Internal Single Gain set
	csi_opa_pgac_difference_e	eDifferenceGain;	//Internal Difference Gain set
	csi_opa_mode_input_e        eInputMode;			//Input Mode set
	bool						bOpaNegativeInput;	//Opa Negative Input enable/disable
	bool						bOpaOutput;			//Opa Output enable/disable
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
void csi_opa_enable(csp_opa_t *ptOpaBase);


/**
  \brief       stop opa Hardware module 
  \param[in]   ptOpaBase	pointer of opa register structure
  \return      error code.
*/
void csi_opa_disable(csp_opa_t *ptOpaBase);

#ifdef __cplusplus
}
#endif

#endif /* _DRV_UART_H_ */
