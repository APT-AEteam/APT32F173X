/***********************************************************************//** 
 * \file  reliability_demo.c
 * \brief  reliability demo code, including LVR/LVD, RSR, emcm and memory check
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-27 <td>V2.0 <td>WNN    <td>initial
 * </table>
 * *********************************************************************
*/

/* include ----------------------------------------------------------------*/
#include "csi_drv.h"
#include "iostring.h"
#include "board_config.h"

/* externs function--------------------------------------------------------*/
/* private function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

/** \brief 函数示例LVD的配置方法
 * 
 *  \param[in] none
 *  \return error code
 */
void lvd_demo(void)
{
	uint8_t byLevel;
	
	csi_lvd_int_enable(LVD_INTF,LVD_30);  //VDD掉电到3.0V即触发LVD中断
	byLevel = csi_get_lvdlevel();
	
	my_printf("lvd level: %d\n", byLevel); //执行board_init()对串口进行配置后才有打印
	while(1)
	{
		if(csi_lvd_flag())
		{
			my_printf("vdd low lvd level\n");   //当前电压低于 INTLVL 设置的检测阈值
		}
		else
		{
			my_printf("vdd high lvd level\n");    //当前电压高于 INTLVL 设置的检测阈值
		}
		
	}	
}

/** \brief 函数示例LVR的配置方法,及复位源查询方式
 * 
 *  \param[in] none
 *  \return error code
 */
void lvr_demo(void)
{
	uint8_t byLevel;
	uint8_t byRstSrc;	

	csi_lvr_enable(LVR_31);				  	//VDD掉电到2.8V，芯片复位	
	byLevel = csi_get_lvrlevel();
	my_printf("lvr level: %d\n", byLevel);	//执行board_init()对串口进行配置后才有打印
	byRstSrc = csi_get_rst_reason(); 		//查询复位源，值为csi_rst_rsr_e枚举量之一
	my_printf("rst source: %d\n", byRstSrc);
	while(1);
	
}

/** \brief 内存检查的使用方法
 * 
 *  \param[in] none
 *  \return error code
 */
void memorycheck_demo(void)
{	
	csi_flashcheck_set_times(10);		//开启flashcheck功能，检查错误次数上限 10
	
	csi_flashcheck_rst();				//错误到达上限，芯片复位	
	csi_flashcheck_disable();			//关闭flashcheck功能	
		
	csi_sramcheck_set_times( 8);		//开启fsram check功能，检查错误次数上限 8	
	csi_sramcheck_rst();				//错误到达上限，芯片复位 
	
	csi_sramcheck_disable();			//关闭sram check功能
	
}

/** \brief 外部主时钟监测的使用方法。必须外接外部主晶振。
 * 
 *  \param[in] none
 *  \return error code
 */
void emcm_demo(void)
{
	csi_gpio_set_mux(XIN_PORT,XIN_PIN, XIN_PIN_FUNC);
	csi_gpio_set_mux(XOUT_PORT,XOUT_PIN, XOUT_PIN_FUNC);
	
	csi_emosc_enable(8000000);			//使能外部晶振驱动电路,输入频率参数，以调整内部增益
	csi_emcm_2_imosc_int();				//一旦检测到外部晶振失常，系统时钟切到IMOSC，并触发中断。注意：
	csi_emcm_rst();						//一旦检测到外部晶振失常，系统复位。
	csi_emcm_disable();					//取消对外部晶振的检测。
}


/** \brief 外部副时钟监测的使用方法。必须外接外部副晶振。
 * 
 *  \param[in] none
 *  \return error code
 */
void escm_demo(void)
{
	csi_gpio_set_mux(SXIN_PORT,SXIN_PIN, SXIN_PIN_FUNC);
	csi_gpio_set_mux(SXOUT_PORT,SXOUT_PIN, SXOUT_PIN_FUNC);
	
	csi_esosc_enable();			 //使能外部晶振驱动电路,输入频率参数，以调整内部增益
	
	csi_escm_2_imosc_int();				//一旦检测到外部晶振失常，系统时钟切到IMOSC，并触发中断。注意：
	csi_escm_rst();						//一旦检测到外部晶振失常，系统复位。
	csi_escm_disable();					//取消对外部晶振的检测。
	
}

/** \brief cqcr测试demo 必须外接晶振。
 *   参考时钟选择 EM,主时钟选择EM  {SRC_EMOSC, EMOSC_VALUE, SCLK_DIV2, PCLK_DIV2, EMOSC_VALUE/2, EMOSC_VALUE/4};
 *   注意EM管脚需要配置  参考时钟配置为0x3ff，即EM 24M的计数值
 *   如果源时钟选择IM 5.556M 获取到的csi_get_cqsr()值大概为0x1149.	
 *  \param[in] none
 *  \return error code
 */
void syscon_cqcr_demo(void)
{
	uint32_t wCqsrValue = 0;
	csi_set_cqcr(CQCR_REFSEL_EM,CQCR_SRCSEL_IM,0x3ff);   // 参考时钟选择 EM,  源时钟选择控 IM

	wCqsrValue = csi_get_cqsr();
	my_printf("cqsr value =%d",wCqsrValue);
		
}