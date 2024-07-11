/***********************************************************************//** 
 * \file  fvr_demo.c
 * \brief  FVR_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-4-25 <td>V0.0 <td>XS    <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include "csi_drv.h"
#include "board_config.h"

/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

/** \brief 配置FVR电平，并通过管脚输出
 *  \brief 由于FVR可以同时提供给ADC,OPA和CMP使用，因此在使用FVR前需确保模块间配置不会冲突
 * 
 *  \param[in] none
 *  \return none
 */
void fvr_output_demo(void)
{
	csi_fvr_config_t tFvrConfig= {0};
	
#if (USE_GUI == 0)		
	csi_gpio_set_mux(GPIOB,PB7, PB7_FVROUT);				//设置PB7为FVR输出
#endif	

	tFvrConfig.eFvrLevel = FVR_LEVEL_1V;					//设置FVR输出电压
	tFvrConfig.eCmpRef = FVR_COMPARER_REF_FVR;				//设置CMP的参考电压为FVR
	tFvrConfig.eOpaCm = FVR_OPACM_HALFFVR;					//设置OPA的基准电压为1/2FVR	
	csi_fvr_init(FVR,&tFvrConfig);
	
	csi_fvr_enable(FVR);									//使能FVR
}

/** \brief 配置BUF(intervref)电平，并通过管脚输出
 * 
 *  \param[in] none
 *  \return none
 */
void buf_output_demo(void)
{
#if (USE_GUI == 0)	
	csi_gpio_set_mux(GPIOD,PD4, PD4_INPUT);					//设置BUF输入管脚PD4
	csi_gpio_set_mux(GPIOB,PB8, PB8_BUF);					//设置BUF输出管脚PB8
#endif

	csi_set_buf(FVR,FVR_BUF_LEVEL_EXTERNAL);				//设置buf的输出为外部管脚输入
	csi_fvr_enable(FVR);		
}