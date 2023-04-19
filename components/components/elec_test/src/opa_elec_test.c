/***********************************************************************//** 
 * \file  opa_elec_test.c
 * \brief  OPA电气参数测试代码
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date      <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-4-19 <td>V0.0     <td>WCH     <td>initial
 * </table>
 * *********************************************************************
*/

/* include ----------------------------------------------------------------*/
#include <drv/gpio.h>
#include <drv/opa.h>
#include "pin.h"

/** \brief OPA IO初始化
 *	OPA0/1/2/3引脚初始化
 *  \param[in] none
 *  \return none
 */
void opa_io_init(void)
{
	csi_pin_set_mux(PA0,PA0_OPA0P); //OPA0输入正端
	csi_pin_set_mux(PA1,PA1_OPA0N); //OPA0输入负端
	csi_pin_set_mux(PA2,PA2_OPA0X); //POPA0输出
	
	csi_pin_set_mux(PB3,PB3_OPA1P); //OPA1输入正端
	csi_pin_set_mux(PB4,PB4_OPA1N); //OPA1输入负端
	csi_pin_set_mux(PB5,PB5_OPA1X); //OPA1输出
	
	csi_pin_set_mux(PB0,PB0_OPA2P); //OPA2输入正端
	csi_pin_set_mux(PA7,PA7_OPA2N); //OPA2输入负端
	csi_pin_set_mux(PA6,PA6_OPA2X); //OPA2输出
	
	csi_pin_set_mux(PA3,PA3_OPA3P); //OPA3输入正端
	csi_pin_set_mux(PA4,PA4_OPA3N); //OPA3输入负端
	csi_pin_set_mux(PA5,PA5_OPA3X); //OPA3输出
}

/** \brief OPA工作电压测试
 *	1、OPA设置为单端输入，1倍增益。
 *  2、P端输入1KHz方波，方波最高电压3V，最低电压0V。
 *  3、调节VDD，记录OPA正常复制方波可达到的电压高点Vmax和电压低点Vmin。
 *  \param[in] none
 *  \return none
 */
void opa_operate_voltage_test(void)
{
	opa_io_init();
	
	csi_opa_config_t ptOpaConfig_t;
	//OPA 参数配置
	ptOpaConfig_t.byMode				= OPA_INTERNAL_MODE;	//内部增益模式
	ptOpaConfig_t.byInternalGain 		= OPA_PGAC_X2;			//两倍增益（在OPAx_CR的19位置1情况下失效）
	ptOpaConfig_t.byInputMode 			= SINGEL;				//单端输入
	ptOpaConfig_t.byOpaNegativeInput 	= DISABLE;				//关闭负端使能
	ptOpaConfig_t.byOpaOutput 			= ENABLE;				//开启放大器输出
	
	csi_opa_init(OPA0,&ptOpaConfig_t);
	OPA0 ->CR |= 0x1<<19;//输出1倍增益
	csi_opa_start(OPA0);
	
	csi_opa_init(OPA1,&ptOpaConfig_t);
	OPA1 ->CR |= 0x1<<19;//输出1倍增益
	csi_opa_start(OPA1);  
	
	csi_opa_init(OPA2,&ptOpaConfig_t);
	OPA2 ->CR |= 0x1<<19;//输出1倍增益
	csi_opa_start(OPA2); 
	
	csi_opa_init(OPA3,&ptOpaConfig_t);
	OPA3 ->CR |= 0x1<<19;//输出1倍增益
	csi_opa_start(OPA3); 
}

/** \brief OPA输入失调电压测试
 *	1、OPA设置为单端输入，1倍增益。
 *	2、OPA正端输入1V时，记录输出电压值。
 *	3、VOFF = Vout - Vin
 *  \param[in] none
 *  \return none
 */
void opa_input_offset_voltage_test(void)
{
	opa_io_init();
	
	csi_opa_config_t ptOpaConfig_t;
	//OPA 参数配置
	ptOpaConfig_t.byMode 				= OPA_INTERNAL_MODE;//内部增益模式
	ptOpaConfig_t.byInternalGain 		= OPA_PGAC_X2;		//两倍增益（在OPAx_CR的19位置1情况下失效）
	ptOpaConfig_t.byInputMode 			= SINGEL;			//单端输入
	ptOpaConfig_t.byOpaNegativeInput 	= DISABLE;			//关闭负端使能
	ptOpaConfig_t.byOpaOutput 			= ENABLE;			//开启放大器输出
	
	csi_opa_init(OPA0,&ptOpaConfig_t);
	OPA0 ->CR |= 0x1<<19;//输出1倍增益
	csi_opa_start(OPA0);
	
	csi_opa_init(OPA1,&ptOpaConfig_t);
	OPA1 ->CR |= 0x1<<19;//输出1倍增益
	csi_opa_start(OPA1);  
	
	csi_opa_init(OPA2,&ptOpaConfig_t);
	OPA2 ->CR |= 0x1<<19;//输出1倍增益
	csi_opa_start(OPA2); 
	
	csi_opa_init(OPA3,&ptOpaConfig_t);
	OPA3 ->CR |= 0x1<<19;//输出1倍增益
	csi_opa_start(OPA3); 
}

/** \brief OPA输入共模电压测试
 *	最大值测试：
 *	1、OPA设置为单端输入，1倍增益
 *	2、OPA正端输入为4V和4.8V时，记录运放输出电压
 *	最小值测试：
 *	1、OPA设置为单端输入，1倍增益
 *	2、正端输入为0.2V和0.4V时，记录运放输出电压
 *	3、测最小值的时候，功耗太大会导致参考地和测量地有压差，所以测量时应尽量降低功耗
 *  \param[in] none
 *  \return none
 */
void opa_input_common_mode_voltage_test(void)
{
	opa_io_init();
	
	csi_opa_config_t ptOpaConfig_t;
	//OPA 参数配置
	ptOpaConfig_t.byMode 				= OPA_INTERNAL_MODE;//内部增益模式
	ptOpaConfig_t.byInternalGain 		= OPA_PGAC_X2;		//两倍增益（在OPAx_CR的19位置1情况下失效）
	ptOpaConfig_t.byInputMode 			= SINGEL;			//单端输入
	ptOpaConfig_t.byOpaNegativeInput 	= DISABLE;			//关闭负端使能
	ptOpaConfig_t.byOpaOutput 			= ENABLE;			//开启放大器输出
	
	csi_opa_init(OPA0,&ptOpaConfig_t);
	OPA0 ->CR |= 0x1<<19;//输出1倍增益
	csi_opa_start(OPA0);
	
	csi_opa_init(OPA1,&ptOpaConfig_t);
	OPA1 ->CR |= 0x1<<19;//输出1倍增益
	csi_opa_start(OPA1);  
	
	csi_opa_init(OPA2,&ptOpaConfig_t);
	OPA2 ->CR |= 0x1<<19;//输出1倍增益
	csi_opa_start(OPA2); 
	
	csi_opa_init(OPA3,&ptOpaConfig_t);
	OPA3 ->CR |= 0x1<<19;//输出1倍增益
	csi_opa_start(OPA3); 
}

/** \brief OPA输出电压范围测试
 *	1、测试输出电压0.2V：配置运放为内部增益X1，调节P端输入电压，使输出分别为0.2V和0.2V+20mV时的输入电压值；
 *	2、测试输出电压为4.8V：配置运放为内部增益X1，调节P端输入电压，使输出分别为4.8V和4.8V-20mV时的输入电压值；
 *	3、斜率=输出/输入
 *  \param[in] none
 *  \return none
 */
void opa_output_voltage_test(void)
{
	opa_io_init();
	
	csi_opa_config_t ptOpaConfig_t;
	//OPA 参数配置
	ptOpaConfig_t.byMode 				= OPA_INTERNAL_MODE;//内部增益模式
	ptOpaConfig_t.byInternalGain 		= OPA_PGAC_X2;		//两倍增益（在OPAx_CR的19位置1情况下失效）
	ptOpaConfig_t.byInputMode 			= SINGEL;			//单端输入
	ptOpaConfig_t.byOpaNegativeInput 	= DISABLE;			//关闭负端使能
	ptOpaConfig_t.byOpaOutput 			= ENABLE;			//开启放大器输出
	
	csi_opa_init(OPA0,&ptOpaConfig_t);
	OPA0 ->CR |= 0x1<<19;//输出1倍增益
	csi_opa_start(OPA0);
	
	csi_opa_init(OPA1,&ptOpaConfig_t);
	OPA1 ->CR |= 0x1<<19;//输出1倍增益
	csi_opa_start(OPA1);  
	
	csi_opa_init(OPA2,&ptOpaConfig_t);
	OPA2 ->CR |= 0x1<<19;//输出1倍增益
	csi_opa_start(OPA2); 
	
	csi_opa_init(OPA3,&ptOpaConfig_t);
	OPA3 ->CR |= 0x1<<19;//输出1倍增益
	csi_opa_start(OPA3); 
}

/** \brief OPA输出电流测试
 *	1、配置好运放，外部增益，运放N端和输出端短接，运放P端给1.5V电压，输出端接负载
 *	2、记录输出电压；调整负载，记录输出电压低于1.5V-0.002V时的电流值
 *  \param[in] none
 *  \return none
 */
void opa_output_current_test(void)
{
	opa_io_init();
	
	csi_opa_config_t ptOpaConfig_t;
	//OPA 参数配置
	ptOpaConfig_t.byMode 				= OPA_EXTERNAL_MODE;//外部增益模式
	ptOpaConfig_t.byOpaNegativeInput 	= ENABLE;			//开启负端使能
	ptOpaConfig_t.byOpaOutput 			= ENABLE;			//开启放大器输出
	
	csi_opa_init(OPA0,&ptOpaConfig_t);
	csi_opa_start(OPA0);
	
	csi_opa_init(OPA1,&ptOpaConfig_t);
	csi_opa_start(OPA1);
	
	csi_opa_init(OPA2,&ptOpaConfig_t);
	csi_opa_start(OPA2);
	
	csi_opa_init(OPA3,&ptOpaConfig_t);
	csi_opa_start(OPA3); 
}

/** \brief OPA内部增益测试
 *	1、单端模式：配置好运放，正端输入合适的电压值，负端不用接，记录运放输出电压值（Vout = Vp+ * Gain）
 *	2、差分模式：配置好运放，正端输入合适的电压值，负端接地，记录运放输出电压值（Vout = （Vp+ - Vp-）* Gain + 2.5）
 *	注：为保证测试结果准确性，测试时应尽量降低功耗，比如使用131K作为主时钟。
 *  \param[in] none
 *  \return none
 */
void opa_internal_gain_test(void)
{
	opa_io_init();
	
	csi_opa_config_t ptOpaConfig_t;
	//OPA 参数配置
	ptOpaConfig_t.byMode 				= OPA_INTERNAL_MODE;//内部增益模式
	ptOpaConfig_t.byInternalGain 		= OPA_PGAC_X2;		//两倍增益（在OPAx_CR的19位置1情况下失效）
	ptOpaConfig_t.byInputMode 			= SINGEL;			//单端输入
	ptOpaConfig_t.byOpaNegativeInput 	= DISABLE;			//关闭负端使能
	ptOpaConfig_t.byOpaOutput 			= ENABLE;			//开启放大器输出
	
	csi_opa_init(OPA0,&ptOpaConfig_t);
	csi_opa_start(OPA0);
	
	csi_opa_init(OPA1,&ptOpaConfig_t);
	csi_opa_start(OPA1);  
	
	csi_opa_init(OPA2,&ptOpaConfig_t);
	csi_opa_start(OPA2); 
	
	csi_opa_init(OPA3,&ptOpaConfig_t);
	csi_opa_start(OPA3); 
}