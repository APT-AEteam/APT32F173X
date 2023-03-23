/***********************************************************************//** 
 * \file  opa_demo.c
 * \brief  LIN_TEST description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2022-03-18 <td>V0.0 <td>XB     <td>initial
 * </table>
 * *********************************************************************
*/

/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include <drv/opa.h>
#include "pin.h"
#include "iostring.h"

//#include "demo.h"

/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/
/********************************************************************
 * 内部增益模式接线图  Vout =  vp * GAIN ；
                |╲
            Vp  |  ╲
            ----|+   ╲
                |      ╲______Vout
             NC |      ╱   
			 ×-|-   ╱    
                |  ╱      
                |╱         
********************************************************************/
/** \brief opa internal gain mode test 
 *  \brief OPA内部增益模式测试
 * 
 *  \param[in] none
 *  \return error code
 */
void opa_internal_gain_mode_test(void)
{
	csi_opa_config_t ptOpaConfig_t;
	
	csi_pin_set_mux(PA0,PA0_OPA0P); //PIN14 输入正端
	csi_pin_set_mux(PA1,PA1_OPA0N); //PIN15 输入负端
	csi_pin_set_mux(PA2,PA2_OPA0X); //PIN16 输出
	csi_pin_set_mux(PB3,PB3_OPA1P); //PIN55 输入正端
	csi_pin_set_mux(PB4,PB4_OPA1N); //PIN56 输入负端
	csi_pin_set_mux(PB5,PB5_OPA1X); //PIN57 输出
	csi_pin_set_mux(PB0,PB0_OPA2P); //PIN26 输入正端
	csi_pin_set_mux(PA7,PA7_OPA2N); //PIN23 输入负端
	csi_pin_set_mux(PA6,PA6_OPA2X); //PIN22 输出
	csi_pin_set_mux(PA3,PA3_OPA3P); //PIN17 输入正端
	csi_pin_set_mux(PA4,PA4_OPA3N); //PIN20 输入负端
	csi_pin_set_mux(PA5,PA5_OPA3X); //PIN21 输出
	
	//OPA 参数配置
	ptOpaConfig_t.byMode = OPA_INTERNAL_MODE;
	ptOpaConfig_t.byInternalGain = OPA_PGAC_X2;
	ptOpaConfig_t.byInputMode = SINGEL;
	ptOpaConfig_t.byOpaNegativeInput = DISABLE;
	ptOpaConfig_t.byOpaOutput = ENABLE;
	
	csi_opa_init(OPA0,&ptOpaConfig_t);
	OPA0 ->CR |= 0x1<<19;
	csi_opa_start(OPA0);
	
	csi_opa_init(OPA1,&ptOpaConfig_t);
	csi_opa_start(OPA1);  
	
	csi_opa_init(OPA2,&ptOpaConfig_t);
	csi_opa_start(OPA2); 
	
	csi_opa_init(OPA3,&ptOpaConfig_t);
	csi_opa_start(OPA3); 
}

/********************************************************************
 * OPA外部增益模式使用接线示意
 * 场景① 外部增益模式接线图  
 * Vout =  vp * (1 + R2/R1)
 * gain =  1 + R2/R1
	 
					|╲
				Vp  |  ╲
				----|+   ╲
					|      ╲______Vout
					|      ╱  |
				Vn _|_   ╱    |
				  | |  ╱      |
			 R1   | |╱   R2   |
	GND___/\/\/\__|_____/\/\/\_| 		

 * 场景② 差分输入外部增益接线示意图
 * Vout =  (((R1+R2)/(R3+R4)) * (R4/R1) * V2) - ((R2/R1) * V1)
 * 当R1 = R3且R2 = R4时，Vout =（R2/R1）* (V2 - V1)
 * 
			 R1                R2 
	V1_____/\/\/\____________/\/\/\_
					|			   |
					|	|╲        |
					| N |  ╲      |
					|___|_   ╲    |
						|      ╲__|____Vout
			 R3		  P |      ╱  
	V2_____/\/\/\_______|+   ╱    
					|   |  ╱      
				R4  |   |╱     
	  GND___/\/\/\__|

 * 
********************************************************************/
/** \brief opa external gain mode test 
 *  \brief OPA外部增益模式测试
 * 
 *  \param[in] none
 *  \return error code
 */
 
void opa_external_gain_mode_test(void)
{
	csi_opa_config_t ptOpaConfig_t;
	

	csi_pin_set_mux(PA0,PA0_OPA0P); //PIN14 输入正端
	csi_pin_set_mux(PA1,PA1_OPA0N); //PIN15 输入负端
	csi_pin_set_mux(PA2,PA2_OPA0X); //PIN16 输出
	csi_pin_set_mux(PB3,PB3_OPA1P); //PIN55 输入正端
	csi_pin_set_mux(PB4,PB4_OPA1N); //PIN56 输入负端
	csi_pin_set_mux(PB5,PB5_OPA1X); //PIN57 输出
	csi_pin_set_mux(PB0,PB0_OPA2P); //PIN26 输入正端
	csi_pin_set_mux(PA7,PA7_OPA2N); //PIN23 输入负端
	csi_pin_set_mux(PA6,PA6_OPA2X); //PIN22 输出
	csi_pin_set_mux(PA3,PA3_OPA3P); //PIN17 输入正端
	csi_pin_set_mux(PA4,PA4_OPA3N); //PIN20 输入负端
	csi_pin_set_mux(PA5,PA5_OPA3X); //PIN21 输出
	//OPA 参数配置
	ptOpaConfig_t.byMode = OPA_EXTERNAL_MODE;
	ptOpaConfig_t.byOpaNegativeInput = ENABLE;
	ptOpaConfig_t.byOpaOutput = ENABLE;
	
	csi_opa_init(OPA0,&ptOpaConfig_t);
	csi_opa_start(OPA0);
	
	csi_opa_init(OPA1,&ptOpaConfig_t);
	csi_opa_start(OPA1);
	
	csi_opa_init(OPA2,&ptOpaConfig_t);
	csi_opa_start(OPA2);
	
	csi_opa_init(OPA3,&ptOpaConfig_t);
	csi_opa_start(OPA3);
}

