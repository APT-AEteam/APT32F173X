/***********************************************************************//** 
 * \file  lvd_r_elec_test.c
 * \brief  LVD_R_ELEC_TEST description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-4-19 <td>V0.0 <td>ZJY     <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include <iostring.h>
#include <csi_drv.h>

/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/


/** \brief lvr 测试: 芯片正常工作后，逐渐调低供电电压，记录芯片停止工作时的电压，即为Vthrf，
 *   再逐渐调高供电，记录芯片恢复工作时的电压，即为VR，VLVR = VR- Vthr
 * 
 *  \param[in] eLvrLev: LVR_19~LVR_40，参考csi_lvr_level_e定义
 *  \return none
 */
void lvr_test(csi_lvr_level_e eLvrLev)
{
	csi_pin_set_mux(PD0, PD0_OUTPUT);	 
	csi_pin_set_high(PD0);	

	csi_lvr_enable(eLvrLev);						//设置LVR的触发电压	
	
	while(1)
	{
		mdelay(10);
		csi_pin_toggle(PD0);			
	}
}

/** \brief lvd 测试: 芯片正常工作后，逐渐调低供电电压，记录LVD标志位置起时的电压，即为Vthdf，再逐渐调高供电，
 *  记录LVD标志位清零时的电压，即为VR，VLVD = VR- Vthdf
 * 
 *  \param[in] eLvrLev: LVD_21~LVD_39和LVDIN_10，参考csi_lvd_level_e定义
 *  \return none
 */
void lvd_test(csi_lvd_level_e eLvdLev)
{
	csi_pin_set_mux(PD0, PD0_OUTPUT);	 
	csi_pin_set_high(PD0);							//PD0 OUTPUT H
	
	csi_pin_set_mux(PC0, PC0_OUTPUT);	
	csi_pin_set_high(PC0);							//PC0 OUTPUT H，可以将PC0的IO翻转放到LVD中断处理函数里，作为中断进入的标记
	
	csi_pin_set_mux(PB1, PB1_LVDIN);				//LVD外部1V0的输入IO管脚配置
	csi_lvd_int_enable(LVD_INTFR, eLvdLev);  		//电压下降到低于或者升高到超过 LVDLVL(both)时，触发中断

	while(1)
	{
		//LVD测试时用来双重IO的电平指示，LVD中断(PC0翻转)和LVD的FLAG(PD0电平指示)，都能指示对应状态
		if(csi_lvd_flag())
		{
			csi_pin_set_low(PD0);	 	   			//当前电压低于INTLVL设置的检测阈值,PD0低
		}
		else
		{
			csi_pin_set_high(PD0);    				//当前电压高于INTLVL设置的检测阈值,PD0高
		}
	}
}

/** \brief 低压复位检测工作电流测试: 配置好相关的参数(5V供电，LVD阀值3V9)，打开/关闭LVD, 然后进入deepsleep模式，记录电流
 * 
 *  \param[in] bLvdEn: ENABLE/DISABLE
 *  \return none
 */
void icc_test(bool bLvdEn)
{
	csi_pin_set_mux(PD0, PD0_OUTPUT);	 
	csi_pin_set_high(PD0);							//PD0 OUTPUT H
	
	csi_lvd_int_enable(LVD_INTFR,LVD_39);  			//LVD ON/中断使能/LVD阀值
	
	if(!bLvdEn)
		csi_lvd_disable();							//LVD OFF
	
	//延时，防止deepsleep下代码烧录不进去
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
		
	csi_pm_enter_sleep(PM_MODE_DEEPSLEEP);			//进入DEEPSLEEP，PD0高
	
	while(1)
	{
		mdelay(10);
		csi_pin_toggle(PD0);			
	}
}

/** \brief lvr deepsleep测试: 芯片正常工作进入deepsleep后(看电流变化/PD0状态)，逐渐调低供电电压，记录芯片停止工作时的电压，即为Vthrf，
 *   再逐渐调高供电，记录芯片恢复工作时的电压，即为VR，VLVR = VR- Vthr；
 * 
 *  \param[in] eLvrLev: LVR_19~LVR_40，参考csi_lvr_level_e定义
 *  \return none
 */
void lvr_deepsleep_test(csi_lvr_level_e eLvrLev)
{
	csi_pin_set_mux(PD0, PD0_OUTPUT);	 
	csi_pin_set_high(PD0);	

	csi_lvr_enable(eLvrLev);						//设置LVR的触发电压	
	
	//延时，防止deepsleep下代码烧录不进去
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	
	csi_pm_enter_sleep(PM_MODE_DEEPSLEEP);			//进入deepsleep，PD0高
	
	while(1)
	{
		mdelay(10);
		csi_pin_toggle(PD0);			
	}
}

/** \brief lvd 测试: 设置DEEPSLEEP时的唤醒源为LVD，LVD的检测中断极性分别设置为当电压下降到低于或者升高到超过 LVDLVL(both)时触发中断；
 *  芯片正常工作进入deepsleep后，调整供电电压，分别记录上升沿和下降沿唤醒时的的电压值
 * 
 *  \param[in] eLvrLev: LVD_21~LVD_39和LVDIN_10，参考csi_lvd_level_e定义
 *  \return none
 */
void lvd_deepsleep_test(csi_lvd_level_e eLvdLev)
{
	csi_pin_set_mux(PD0, PD0_OUTPUT);	 
	csi_pin_set_high(PD0);							//PD0 OUTPUT H
	
	csi_pin_set_mux(PC0, PC0_OUTPUT);	
	csi_pin_set_high(PC0);							//PC0 OUTPUT H，将PC0的IO翻转放到LVD中断处理函数里，作为中断进入的标记
	
	csi_pin_set_mux(PB1, PB1_LVDIN);				//LVD外部1V0的输入IO管脚配置
	csi_lvd_int_enable(LVD_INTFR, eLvdLev);  		//电压下降到低于或者升高到超过 LVDLVL(both)时，触发中断
	csi_pm_config_wakeup_source(WKUP_LVD, ENABLE);	//配置LVD唤醒deepsleep
	
	//延时，防止deepsleep下代码烧录不进去
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	mdelay(200);
	csi_pin_toggle(PD0);
	
	while(1)
	{
		csi_pm_enter_sleep(PM_MODE_DEEPSLEEP);		//进入deepsleep
		
		mdelay(10);									//唤醒后
		csi_pin_set_low(PD0);
		mdelay(10);
		csi_pin_set_high(PD0);
		mdelay(10);
		csi_pin_set_low(PD0);
		mdelay(10);
		csi_pin_set_high(PD0);
		mdelay(10);
		csi_pin_set_low(PD0);
		mdelay(10);
		csi_pin_set_high(PD0);
	}
}
/** \brief lvd测试中断处理函数：里面添加了IO翻转和中断状态清除。
 *   测试时如需要LVD中断，可在interrupt.c的函数syscon_int_handler中加入同样处理或者直接调用此函数
 *   
 *  \param[in] none
 *  \return none
 */
void lvd_elec_test_int_handle(void)
{
	if(csp_syscon_get_int_st(SYSCON) & LVD_INT)
	{
		csi_pin_toggle(PC0);
		csp_syscon_int_clr(SYSCON, LVD_INT);
	}
}
