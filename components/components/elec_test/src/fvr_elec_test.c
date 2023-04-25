/***********************************************************************//** 
 * \file  fvr_eled_test.c
 * \brief  FVR_TEST description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-4-25 <td>V0.0 <td>XS     <td>initial
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

/** \brief FVR内部参考电压测试
 *  \return error code
 */
void fvr_elec_test(void)
{
	csi_pin_set_mux(PB7, PB7_FVROUT);//设置PB7为FVR输出
	soc_clk_enable(FVR_SYS_CLK);//配置FVR时钟
	csi_fvr_lvl_set(FVR, FVR_LVL_4V);//设置FVR内部电压为4V
	csi_fvr_start(FVR);	
}
/** \brief 内部INTVREF参考电压测试
 *  \return error code
 */
void buf_elec_test(void)
{
	soc_clk_enable(FVR_SYS_CLK);//配置FVR时钟
	csi_pin_set_mux(PB8, PB8_BUF);//设置BUF输出管脚PB8
	csi_fvr_buf_init(FVR,FVR_BUFLVL_1V);//设置BUF为内部INTVREF
	csi_fvr_start(FVR);		
}