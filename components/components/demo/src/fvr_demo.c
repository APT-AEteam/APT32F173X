/***********************************************************************//** 
 * \file  fvr_demo.c
 * \brief  FVR_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-4-25 <td>V0.0 <td>XS    <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include "sys_clk.h"
#include <drv/fvr.h>
#include "pin.h"

#include "demo.h"
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

/** \brief 配置FVR电平，并通过管脚输出示例代码
 * 
 *  \param[in] none
 *  \return none
 */
void fvr_output_demo(void)
{
	csi_pin_set_mux(PB7, PB7_FVROUT);//设置PB7为FVR输出
	soc_clk_enable(FVR_SYS_CLK);//配置FVR时钟
	csi_fvr_lvl_set(FVR, LVL_4V);//设置FVR电平为4V
//	csi_fvr_cmpref_set(FVR,FVR_CMPREF_FVR);//设置CMP参考电平为FVR
//	csi_fvr_opacm(FVR);	//设置OPA CM电平为FVR
	csi_fvr_start(FVR);	
}

/** \brief 配置BUF(intervref)电平，并通过管脚输出
 * 
 *  \param[in] none
 *  \return none
 */
void fvr_buf_demo(void)
{
	soc_clk_enable(FVR_SYS_CLK);//配置FVR时钟
	csi_pin_set_mux(PD4, PD4_INPUT);//设置BUF输入管脚PD4
	csi_pin_set_mux(PB8, PB8_BUF);//设置BUF输出管脚PB8
	csi_fvr_buf_init(FVR,BUFLVL_INPUT);//设置buf的输入源
	csi_fvr_start(FVR);		
}