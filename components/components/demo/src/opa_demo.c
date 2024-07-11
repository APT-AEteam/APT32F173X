/***********************************************************************//** 
 * \file  opa_demo.c
 * \brief  OPA_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  		<th>Version  <th>Author  <th>Description
 * <tr><td> 2023-3-17   <td>V0.0 	 <td>xs      <td>initial
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

/** \brief opa_internal_gain_mode_demo:OPA内部增益模式
 *  \brief 
 * 
 *  \param[in] none
 *  \return error code
 */
void opa_internal_gain_mode_demo(void)
{
	csi_opa_config_t tOpaConfig= {0};
#if (USE_GUI == 0)
	csi_gpio_set_mux(GPIOA,PA0,PA0_OPA0P); 		//PIN14 输入正端
	csi_gpio_set_mux(GPIOA,PA1,PA1_OPA0N); 		//PIN15 输入负端
	csi_gpio_set_mux(GPIOA,PA2,PA2_OPA0X); 		//PIN16 输出
#endif	
	//OPA 参数配置
	tOpaConfig.eMode = OPA_INTERNAL_MODE;		//内部增益模式
	tOpaConfig.eInputMode = OPA_SINGLE;				//单端输入
	tOpaConfig.eSingleGain = OPA_SINGLE_GAIN_X2;//两倍增益
	tOpaConfig.bOpaNInput = DISABLE;		//关闭负端使能
	tOpaConfig.bOpaOutput = ENABLE;				//开启放大器输出
	csi_opa_init(OPA0,&tOpaConfig);
	
	csi_opa_enable(OPA0);
}


/********************************************************************/
/** \brief opa_external_gain_mode_demo:OPA外部增益模式
 *  \brief 
 * 
 *  \param[in] none
 *  \return error code
 */
 
void opa_external_gain_mode_demo(void)
{
	csi_opa_config_t tOpaConfig= {0};
	
#if (USE_GUI == 0)	
	csi_gpio_set_mux(GPIOA,PA0,PA0_OPA0P); 		//PIN14 输入正端
	csi_gpio_set_mux(GPIOA,PA1,PA1_OPA0N); 		//PIN15 输入负端
	csi_gpio_set_mux(GPIOA,PA2,PA2_OPA0X); 		//PIN16 输出
#endif
	//OPA 参数配置
	tOpaConfig.eMode = OPA_EXTERNAL_MODE;		//外部增益模式
	tOpaConfig.bOpaNInput = ENABLE;				//开启负端使能
	tOpaConfig.bOpaOutput = ENABLE;				//开启放大器输出
	csi_opa_init(OPA0,&tOpaConfig);
	
	csi_opa_enable(OPA0);
}

