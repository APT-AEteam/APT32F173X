/***********************************************************************//** 
 * \file  opa.c
 * \brief opa csi driver
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2022-02-17 <td>V0.0  <td>XB    <td>initial
 * </table>
 * *********************************************************************
*/
#include "opa.h"

/**
*  \brief       initializes the resources needed for the OPA interface.
*  \param[in]   ptOpaBase:	pointer of opa register structure
*  \param[in]   ptOpaCfg:    pointer of opa parameter config structure
* 			   	- eMode: Mode set, \ref csi_opa_mode_e
* 			   	- eSingleGain: Internal Single Gain set, \ref csi_opa_pgac_single_e
*  			   	- eDifferenceGain: Internal Difference Gain set, \ref csi_opa_pgac_difference_e
*   			- eInputMode: Input Mode set, \ref csi_opa_mode_input_e
* 				- bOpaNegativeInput: Opa Negative Input enable/disable
* 				- bOpaOutput: Opa Output enable/disable
  \return      error code.
*/
csi_error_t csi_opa_init(csp_opa_t *ptOpaBase, csi_opa_config_t *ptOpaCfg)
{
	if(ptOpaBase == NULL)
		return CSI_ERROR;
		
	csi_clk_enable((uint32_t *)ptOpaBase);
	
	if(ptOpaCfg->eMode == OPA_INTERNAL_MODE)
	{
		csp_opa_external_disable(ptOpaBase);
		if(ptOpaCfg->eInputMode == SINGEL)	
		{
			csp_opa_set_pgac_singel(ptOpaBase,(opa_pgac_single_e)ptOpaCfg->eSingleGain);
			csp_opa_difference_disable(ptOpaBase);
		}
		if(ptOpaCfg->eInputMode ==  DIFFERENCE)	
		{
			csp_opa_set_pgac_difference(ptOpaBase,(opa_pgac_difference_e)ptOpaCfg->eDifferenceGain);
			csp_opa_difference_enable(ptOpaBase);
		}
	}
	else
	{	
		csp_opa_external_enable(ptOpaBase);
		csp_opa_out_enable(ptOpaBase);
	}
	if(ptOpaCfg->bOpaNegativeInput == ENABLE)
		csp_opa_in_enable(ptOpaBase);
	else
		csp_opa_in_disable(ptOpaBase);
		
	if(ptOpaCfg->bOpaOutput == ENABLE)
		csp_opa_out_enable(ptOpaBase);
	else
		csp_opa_out_disable(ptOpaBase);
		
	return CSI_OK;

}

/**
  \brief       enable opa Hardware module 
  \param[in]   ptOpaBase:	pointer of opa register structure
  \return      error code.
*/
void csi_opa_enable(csp_opa_t *ptOpaBase)
{
	csp_opa_enable(ptOpaBase);
}

/**
  \brief       disable opa Hardware module 
  \param[in]   ptOpaBase:	pointer of opa register structure
  \return      error code.
*/
void csi_opa_disable(csp_opa_t *ptOpaBase)
{
	csp_opa_disable(ptOpaBase);	
}