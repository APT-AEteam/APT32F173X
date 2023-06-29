/***********************************************************************//** 
 * \file  opa_demo.c
 * \brief  LIN_TEST description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-03-18 <td>V0.0 <td>XS     <td>initial
 * </table>
 * *********************************************************************
*/

/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include <drv/opa.h>
#include "pin.h"
#include "iostring.h"

#include "demo.h"

/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

/** \brief opa internal gain mode test 
 *  \brief OPA内部增益模式测试
 * 
 *  \param[in] none
 *  \return error code
 */
void opa_internal_gain_mode_demo(void)
{
	csi_opa_config_t tOpaConfig;
	
	csi_pin_set_mux(PA0,PA0_OPA0P); //PIN14 输入正端
	csi_pin_set_mux(PA1,PA1_OPA0N); //PIN15 输入负端
	csi_pin_set_mux(PA2,PA2_OPA0X); //PIN16 输出
//	csi_pin_set_mux(PB3,PB3_OPA1P); //PIN55 输入正端
//	csi_pin_set_mux(PB4,PB4_OPA1N); //PIN56 输入负端
//	csi_pin_set_mux(PB5,PB5_OPA1X); //PIN57 输出
//	csi_pin_set_mux(PB0,PB0_OPA2P); //PIN26 输入正端
//	csi_pin_set_mux(PA7,PA7_OPA2N); //PIN23 输入负端
//	csi_pin_set_mux(PA6,PA6_OPA2X); //PIN22 输出
	csi_pin_set_mux(PA3,PA3_OPA3P); //PIN17 输入正端
	csi_pin_set_mux(PA4,PA4_OPA3N); //PIN20 输入负端
	csi_pin_set_mux(PA5,PA5_OPA3X); //PIN21 输出
	
	//OPA 参数配置
	tOpaConfig.byMode = OPA_INTERNAL_MODE;//内部增益模式
	tOpaConfig.byInternalGain = OPA_PGAC_X2;//两倍增益
	tOpaConfig.byInputMode = SINGEL;//单端输入
	tOpaConfig.byOpaNegativeInput = DISABLE;//关闭负端使能
	tOpaConfig.byOpaOutput = ENABLE;//开启放大器输出
	
	csi_opa_init(OPA0,&tOpaConfig);
	csi_opa_start(OPA0);
	
//	csi_opa_init(OPA1,&tOpaConfig);
//	csi_opa_start(OPA1);  
//	
//	csi_opa_init(OPA2,&tOpaConfig);
//	csi_opa_start(OPA2); 
//	
	csi_opa_init(OPA3,&tOpaConfig);
	csi_opa_start(OPA3); 
}


/********************************************************************/
/** \brief opa external gain mode test 
 *  \brief OPA外部增益模式测试
 * 
 *  \param[in] none
 *  \return error code
 */
 
void opa_external_gain_mode_demo(void)
{
	csi_opa_config_t tOpaConfig;
	

	csi_pin_set_mux(PA0,PA0_OPA0P); //PIN14 输入正端
	csi_pin_set_mux(PA1,PA1_OPA0N); //PIN15 输入负端
	csi_pin_set_mux(PA2,PA2_OPA0X); //PIN16 输出
//	csi_pin_set_mux(PB3,PB3_OPA1P); //PIN55 输入正端
//	csi_pin_set_mux(PB4,PB4_OPA1N); //PIN56 输入负端
//	csi_pin_set_mux(PB5,PB5_OPA1X); //PIN57 输出
//	csi_pin_set_mux(PB0,PB0_OPA2P); //PIN26 输入正端
//	csi_pin_set_mux(PA7,PA7_OPA2N); //PIN23 输入负端
//	csi_pin_set_mux(PA6,PA6_OPA2X); //PIN22 输出
	csi_pin_set_mux(PA3,PA3_OPA3P); //PIN17 输入正端
	csi_pin_set_mux(PA4,PA4_OPA3N); //PIN20 输入负端
	csi_pin_set_mux(PA5,PA5_OPA3X); //PIN21 输出
	//OPA 参数配置
	tOpaConfig.byMode = OPA_EXTERNAL_MODE;
	tOpaConfig.byOpaNegativeInput = DISABLE;
	tOpaConfig.byOpaOutput = ENABLE;
	
	csi_opa_init(OPA0,&tOpaConfig);
	csi_opa_start(OPA0);
	
//	csi_opa_init(OPA1,&tOpaConfig);
//	csi_opa_start(OPA1);
//	
//	csi_opa_init(OPA2,&tOpaConfig);
//	csi_opa_start(OPA2);
	
	csi_opa_init(OPA3,&tOpaConfig);
	csi_opa_start(OPA3);
}

