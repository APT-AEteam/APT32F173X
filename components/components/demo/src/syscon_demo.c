/***********************************************************************//** 
 * \file  reliability_demo.c
 * \brief  reliability demo code, including LVR/LVD, RSR, emcm and memory check
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-27 <td>V2.0 <td>WNN    <td>initial
 * <tr><td> 2023-9-22 <td>V0.1  <td>GQQ   <td>fix bug,code normalization
 * </table>
 * *********************************************************************
*/

/* include ----------------------------------------------------------------*/
#include "csi_drv.h"
#include "iostring.h"
#include "board_config.h"

/* externs function--------------------------------------------------------*/
extern void iwdt_int_handler(void);
/* private function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/


/** \brief syscon int handler
 * 
 *  \brief 	syscon发生中断时会调用此函数，函数在irq.c里定义为弱(weak)属性，默认仅作部分中断状态清除处理；用户用到syscon中
 * 			断时，建议将此函数剪切到interrupt.c中，在此函数中进行对应中断处理。
 * 
 * 			注意：iwdt中断的入口也是这个函数。如果不用可以删除。
 *  \param[in] none
 *  \return none
 */ 
ATTRIBUTE_ISR void syscon_int_handler(void) 
{

	
	if(csp_syscon_get_isr(SYSCON) & LVD_INT) //LVD INT
	{
		csp_syscon_clr_isr(SYSCON, LVD_INT);
	}
	
	if(csp_syscon_get_isr(SYSCON) & IWDT_INT) //IWDT INT
	{
#if (USE_IWDT_CALLBACK == 1)
	
		csi_iwdt_irqhandler(SYSCON,0);
#else
		iwdt_int_handler();
#endif
		
	}
	if(csp_syscon_get_isr(SYSCON) & EMFAIL_INT) //EMOSC FAIL INT
	{
		csp_syscon_clr_isr(SYSCON, EMFAIL_INT);
	}
	if(csp_syscon_get_isr(SYSCON) & ESFAIL_INT) //ESOSC FAIL INT
	{
		csp_syscon_clr_isr(SYSCON, ESFAIL_INT);
	}

}


/** \brief lvd_demo：函数示例LVD的配置方法
 * 			-配置为VDD掉电到3.0V触发LVD中断
 * 
 *  \param[in] none
 *  \return error code
 */
void lvd_demo(void)
{
	uint8_t byLevel;
	
	csi_set_lvd(LVD_INTF,LVD_30);  			//VDD掉电到3.0V即触发LVD中断
	csi_lvd_lvr_enable();
	
	byLevel = csi_get_lvd_level();
	my_printf("lvd level: %d\n", byLevel); 			//执行board_init()对串口进行配置后才有打印
	
	while(1)
	{
		if(csi_lvd_get_flag())
		{
			printf("vdd low lvd level\n");		//当前电压低于 INTLVL 设置的检测阈值
		}
		else
		{
			my_printf("vdd high lvd level\n");   	//当前电压高于 INTLVL 设置的检测阈值
		}
		
	}	
}

/** \brief lvr_demo:函数示例LVR的配置方法,及复位源查询方式
 * 			-配置LVR，使VDD掉电到3.1V产生LVR复位，并打印复位源
 * 
 *  \param[in] none
 *  \return error code
 */
void lvr_demo(void)
{
	uint8_t byLevel;
	uint8_t byRstSrc;	

	csi_set_lvr(LVR_31);				  	//VDD掉电到3.1V，芯片复位	
	csi_lvd_lvr_enable();
	
	byLevel = csi_get_lvr_level();
	my_printf("lvr level: %d\n", byLevel);	//执行board_init()对串口进行配置后才有打印
	
	byRstSrc = csi_get_rst_reason(); 		//查询复位源，值为csi_rst_rsr_e枚举量之一
	my_printf("rst source: %d\n", byRstSrc);
	
	while(1);
	
}

/** \brief memorycheck_demo：内存检查的使用方法
 * 			-配置内存校验错误次数,达到最大次数后芯片复位，并关闭内存校验
 * 
 *  \param[in] none
 *  \return error code
 */
void memorycheck_demo(void)
{	
	//flash
	csi_flashcheck_set_time(10);		//开启flashcheck功能，检查错误次数上限 10
	csi_flashcheck_rst();				//错误到达上限，芯片复位	
	
	csi_flashcheck_disable();			//关闭flashcheck功能	
	
	//sram	
	csi_sramcheck_set_time( 8);		//开启fsram check功能，检查错误次数上限 8	
	csi_sramcheck_rst();				//错误到达上限，芯片复位 
	
	csi_sramcheck_disable();			//关闭sram check功能
	
}

/** \brief emcm_demo：外部主时钟监测的使用方法。
 * 			-配置外部主时钟监测，并开启中断和复位功能
 * 
 * 	\brief 注意： 必须外接外部主晶振。
 * 
 *  \param[in] none
 *  \return error code
 */
void emcm_demo(void)
{
#if (USE_GUI==0)
	csi_gpio_set_mux(XIN_PORT,XIN_PIN, XIN_PIN_FUNC);
	csi_gpio_set_mux(XOUT_PORT,XOUT_PIN, XOUT_PIN_FUNC);
#endif
	
	csi_emosc_enable(8000000);			//使能外部晶振驱动电路,输入频率参数，以调整内部增益
	csi_emcm_switch_imosc_int();				//一旦检测到外部晶振失常，系统时钟切到IMOSC，并触发中断。注意：
	csi_emcm_rst();						//一旦检测到外部晶振失常，系统复位。
	csi_emcm_disable();					//取消对外部晶振的检测。
}


/** \brief escm_demo：外部副时钟监测的使用方法。
 * 			-配置外部副时钟监测，并开启中断和复位功能
 * 
 * 	\brief 注意： 必须外接外部副晶振。
 *  
 *  \param[in] none
 *  \return error code
 */
void escm_demo(void)
{
#if (USE_GUI==0)
	csi_gpio_set_mux(SXIN_PORT,SXIN_PIN, SXIN_PIN_FUNC);
	csi_gpio_set_mux(SXOUT_PORT,SXOUT_PIN, SXOUT_PIN_FUNC);
#endif	
	csi_esosc_enable();			 		//使能外部晶振驱动电路,输入频率参数，以调整内部增益
	
	csi_escm_2_imosc_int();				//一旦检测到外部晶振失常，系统时钟切到IMOSC，并触发中断。注意：
	csi_escm_rst();						//一旦检测到外部晶振失常，系统复位。
	csi_escm_disable();					//取消对外部晶振的检测。
	
}

/** \brief cqcr测试demo 必须外接晶振。
 * 
 *   参考时钟选择 EM,源时钟选择EM 时配置为：{SRC_EMOSC, EMOSC_VALUE, SCLK_DIV2, PCLK_DIV2, EMOSC_VALUE/2, EMOSC_VALUE/4};
 *   注意EM管脚需要配置  参考时钟配置为0x3ff，即EM 24M的计数值
 *   如果源时钟选择IM 5.556M 获取到的csi_get_cqsr()值大概为0x1149.	
 *  \param[in] none
 *  \return none
 */
void syscon_cqcr_demo(void)
{
	uint32_t wCqsrValue = 0;
	csi_set_cqcr(CQCR_REFSEL_EM,CQCR_SRCSEL_IM,0x3ff);   // 参考时钟选择 EM,  源时钟选择控 IM

	wCqsrValue = csi_get_cqsr();
	my_printf("cqsr value =%d",wCqsrValue);
		
}


/**  \brief PMP demo示例前8K地址区域只能执行，不可读写。
  *  注意： 1、需要配合连接文件gcc_flash_pmp.ld 使用。
  * 		2、需要在代码中打断点，单步运行查看结果是否和注释一致。
  *  		3、一旦使能保护，只能通过复位解除。
  *  \param[in] none
  *  \return    none
*/
void pmp_demo(void)
{
	volatile uint32_t wTestData, wDataRdOut[10];
	
	mpu_region_attr_t tPmpConfig= {0};
	tPmpConfig.r = 0;
	tPmpConfig.w = 0;
	tPmpConfig.x = 1;
	tPmpConfig.a = ADDRESS_MATCHING_NAPOT;
	tPmpConfig.l = 1;
	//设置[0x0 - 0x1FFF]为保护区间：加锁，只能执行，不能读写。此时memory区显示正常值
	csi_mpu_config_region(0, 0x0, REGION_SIZE_8KB, tPmpConfig, ENABLE);
	//保护区间memory区不再显示正常值
	
	
	//csi_ifc_read（0x2000）正常，csi_ifc_read（0x400）会进入异常
	while(wTestData --)	{
		nop;
		csi_ifc_read(IFC, 0x2000, (uint32_t *)wDataRdOut, 10);
		csi_ifc_read(IFC, 0x400, (uint32_t *)wDataRdOut, 10);
	}

	while(1);
}