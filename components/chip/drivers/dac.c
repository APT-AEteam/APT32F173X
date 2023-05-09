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
//#include <csi_config.h>
#include <csp_dac.h>
#include <drv/irq.h>
#include <drv/clk.h>
#include <sys_clk.h>
#include <drv/dac.h>
#include "drv/gpio.h"
#include <drv/pin.h>
#include "csp.h"

/** \brief initialize dac data structure
 * 
 *  \param[in] ptDacBase: pointer of dac register structure
 *  \param[in] ptDacCfg: pointer of dca parameter config structure
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
	csi_pin_set_mux(PA4,PA4_DAC0_OUT);	
	csp_dac_buff_enable(DAC0,1);
	}
	if(ptDacCfg->byBufsel == DISABLE)
	{
		csi_pin_set_mux(PA8,PA8_DAC0_OUT);
		csi_pin_output_mode(PA8,GPIO_OPEN_DRAIN);
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
  \brief       dac interrupt set 
  \param[in]   ptDacBase	pointer of dac register structure
  \return      none
*/
void csi_dac_irq_enable(csp_dac_t *ptDacBase, csi_dac_irq_e eVal,bool eEnable)
{
	csp_dac_irq_enable(ptDacBase,(dac_irq_e)eVal,eEnable);
}

/**
  \brief       dac syncr set 
  \param[in]   ptDacBase	pointer of dac register structure
  \return      none
*/
void csi_dac_syncr_enable(csp_dac_t *ptDacBase, csi_dac_syncr_e eVal,bool eEnable)
{
	csp_dac_syncr_enable(ptDacBase,(dac_syncr_e)eVal,eEnable);
}

/**
  \brief       dac step value set 
  \param[in]   ptDacBase	pointer of dac register structure
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