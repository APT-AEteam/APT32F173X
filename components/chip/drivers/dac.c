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
//#include "csp_dac.h"
#include <drv/irq.h>
#include <drv/clk.h>
#include <sys_clk.h>
#include "csp.h"
/** \brief dac interrupt handle function
 * 
 *  \param[in] ptAdcBase: pointer of dac register structure
 *  \return none
 */ 
__attribute__((weak)) void dac_irqhandler(csp_dac_t *ptAdcBase)
{
	
	
	

}







