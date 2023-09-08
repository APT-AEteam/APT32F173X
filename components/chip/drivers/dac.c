/***********************************************************************//** 
 * \file  dac.c
 * \brief  csi dac driver
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-12 <td>V0.0  <td>ZJY   <td>initial
 * <tr><td> 2021-1-8  <td>V0.1  <td>WNN   <td>modify
 * </table>
 * *********************************************************************
*/
#include <drv/dac.h>
/* Private macro------------------------------------------------------*/
/* externs function---------------------------------------------------*/
/* externs variablesr-------------------------------------------------*/
/* Private variablesr-------------------------------------------------*/
csi_dac_ctrl_t g_tDacCtrl[DAC_IDX];
/** \brief get dac number 
 * 
 *  \param[in] ptDacBase: pointer of dac register structure
 *  \return dac number 0/1
 */ 
static uint8_t apt_get_dac_idx(csp_dac_t *ptDacBase)
{
	switch((uint32_t)ptDacBase)
	{
		case AHB_DAC_BASE:		//DAC0
			return 0;		

		default:
			return 0xff;		//error
	}
}
/** \brief  register dac interrupt callback function
 * 
 *  \param[in] ptDacBase: pointer of dac register structure
 *  \param[in] callback: dac interrupt handle function
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_dac_register_callback(csp_dac_t *ptDacBase, void  *callback)
{
	uint8_t byIdx = apt_get_dac_idx(ptDacBase);
	if(byIdx == 0xff)
		return CSI_ERROR;
		
	g_tDacCtrl[byIdx].callback = callback;
	
	return CSI_OK;
}
/** \brief dac interrupt handler function
 * 
 *  \param[in] ptDacBase: pointer of dac register structure
 *  \param[in] byIdx: dac idx(0)
 *  \return none
 */ 
void csi_dac_irqhandler(csp_dac_t *ptDacBase, uint8_t byIdx)
{
	uint8_t byIsr = csp_dac_get_isr(ptDacBase);
	g_tDacCtrl[byIdx].callback(ptDacBase, byIsr);
			
	csp_dac_clr_isr(ptDacBase, byIsr);
}

/** \brief initialize dac data structure
 * 
 *  \param[in] ptDacBase: pointer of dac register structure
 *  \param[in] ptDacCfg: pointer of dac parameter config structure
 *  \return error code \ref csi_error_t
 */ 
void csi_dac_init(csp_dac_t *ptDacBase, csi_dac_config_t *ptDacCfg)
{	
	csp_dac_clr_da(ptDacBase);//清除转换数据
	csp_dac_set_clk_div(ptDacBase,ptDacCfg->byClkDiv);//设置时钟16分频
	csp_dac_refsel_enable(ptDacBase,ptDacCfg->byRefsel);//关闭REF
	csp_dac_set_datar(ptDacBase,ptDacCfg->byDatarset);//在DATAR中写入数据
	
	if(ptDacCfg->byBufsel == ENABLE)
	{	
	csp_dac_buff_enable(DAC0,1);
	}
	if(ptDacCfg->byBufsel == DISABLE)
	{
	csp_dac_buff_enable(DAC0,0);
	}
}

/**
  \brief       enable dac power  
  \param[in]   ptDacBase	pointer of dac register structure
  \return      none
*/
void csi_dac_en(csp_dac_t *ptDacBase)
{
	csp_dac_powerdown_enable(ptDacBase, 1);//开启powerdown
}


/**
  \brief       disable dac power  
  \param[in]   ptDacBase	pointer of dac register structure
  \return      none
*/
void csi_dac_dis(csp_dac_t *ptDacBase)
{
	csp_dac_powerdown_enable(ptDacBase, 0);	
}

/**
  \brief       dac interrupt enable 
  \param[in]   ptDacBase:	pointer of dac register structure
  \param[in]   eVal: pointer of dac interrupt config structure
  \return      none
*/
void csi_dac_int_enable(csp_dac_t *ptDacBase, csi_dac_irq_e eVal)
{
	csp_dac_int_enable(ptDacBase,(dac_int_e)eVal);
}
/**
  \brief       dac interrupt disable 
  \param[in]   ptDacBase:	pointer of dac register structure
  \param[in]   eVal: pointer of dac interrupt config structure
  \return      none
*/
void csi_dac_int_disable(csp_dac_t *ptDacBase, csi_dac_irq_e eVal)
{
	csp_dac_int_disable(ptDacBase,(dac_int_e)eVal);
}
/**
  \brief       dac syncr set 
  \param[in]   ptDacBase	pointer of dac register structure
  \param[in]   eVal: pointer of dac sync config structure
  \param[in]   eEnable: enable/disable dac sync
  \return      none
*/
void csi_dac_syncr_enable(csp_dac_t *ptDacBase, csi_dac_syncr_e eVal,bool eEnable)
{
	csp_dac_syncr_enable(ptDacBase,(dac_sync_e)eVal,eEnable);
}

/**
  \brief       dac step value set 
  \param[in]   ptDacBase	pointer of dac register structure
  \param[in]   eDer	data of dac sync step
  \return      none
*/
void csi_dac_step_val(csp_dac_t *ptDacBase, uint16_t eDer)
{
	csp_dac_step_val(ptDacBase, eDer);
}

/**
  \brief       dac start 
  \param[in]   ptDacBase	pointer of dac register structure
  \return      none
*/
void csi_dac_start(csp_dac_t *ptDacBase)
{
	csp_dac_start(ptDacBase);
}