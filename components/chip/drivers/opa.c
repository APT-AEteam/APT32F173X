/***********************************************************************//** 
 * \file  opa.c
 * \brief opa csi driver
 * \copyright Copyright (C) 2015-2022 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2022-02-17 <td>V0.0  <td>XB    <td>initial
 * </table>
 * *********************************************************************
*/
#include <drv/opa.h>

/**
  \brief       initializes the resources needed for the OPA interface.
  \param[in]   ptOpaBase	pointer of opa register structure
  \param[in]   ptOpaCfg    pointer of opa parameter config structure
  \return      error code.
*/
csi_error_t csi_opa_init(csp_opa_t *ptOpaBase, csi_opa_config_t *ptOpaCfg)
{
	if(ptOpaBase == NULL)
		return CSI_ERROR;
		
	csi_clk_enable((uint32_t *)ptOpaBase);
	
	if(ptOpaCfg->byMode == OPA_INTERNAL_MODE)
	{
		csp_opa_ex_dis(ptOpaBase);
		if(ptOpaCfg->byInputMode == SINGEL)	
		{
			csp_opa_pgac_singel_int(ptOpaBase,ptOpaCfg->byInternalGain);
			csp_opa_dif_dis(ptOpaBase);
		}
		if(ptOpaCfg->byInputMode ==  DIFFERENCE)	
		{
			csp_opa_pgac_difference_int(ptOpaBase,ptOpaCfg->byInternalGain);
			csp_opa_dif_en(ptOpaBase);
		}
	}
	else
	{	
		csp_opa_ex_en(ptOpaBase);
		csp_opa_out_en(ptOpaBase);
	}
	if(ptOpaCfg->byOpaNegativeInput == ENABLE)
		csp_opa_in_en(ptOpaBase);
	else
		csp_opa_in_dis(ptOpaBase);
		
	if(ptOpaCfg->byOpaOutput == ENABLE)
		csp_opa_out_en(ptOpaBase);
	else
		csp_opa_out_dis(ptOpaBase);
		
	return CSI_OK;

}


/**
  \brief       start opa Hardware module 
  \param[in]   ptOpaBase	pointer of opa register structure
  \return      error code.
*/
void csi_opa_start(csp_opa_t *ptOpaBase)
{
	csp_opa_en(ptOpaBase);
}


/**
  \brief       stop opa Hardware module 
  \param[in]   ptOpaBase	pointer of opa register structure
  \return      error code.
*/
void csi_opa_stop(csp_opa_t *ptOpaBase)
{
	csp_opa_dis(ptOpaBase);	
}