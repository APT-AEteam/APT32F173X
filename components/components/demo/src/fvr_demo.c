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

void fvr_output_demo(void)//FVR电平通过PB7输出
{
	csi_pin_set_mux(PB7, PB7_FVROUT);//设置PB7为FVR输出
	soc_clk_enable(FVR_SYS_CLK);//配置FVR时钟
	csi_fvr_lvl_set(FVR, FVR_LVL_4V);//设置FVR电平为4V
//	csi_fvr_cmpref_set(FVR,FVR_CMPREF_FVR);//设置CMP参考电平为FVR
//	csi_fvr_opacm(FVR);	//设置OPA CM电平为FVR
	csi_fvr_start(FVR);	
}


void fvr_buf_demo(void)//设置FVR的BUF(intervref)功能
{
	csi_pin_set_mux(PB7, PB7_FVROUT);//设置PB7为FVR输出
	soc_clk_enable(FVR_SYS_CLK);//配置FVR时钟
	csi_fvr_lvl_set(FVR, FVR_LVL_4V);//设置FVR电平为4V
	csi_pin_set_mux(PD4, PD4_INPUT);//设置BUF输入管脚PD4
	csi_pin_set_mux(PB8, PB8_BUF);//设置BUF输出管脚PB8
	csi_fvr_buf_init(FVR,FVR_BUFLVL_INPUT);//设置buf的输入源
	csi_fvr_start(FVR);		
}