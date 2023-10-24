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
static uint8_t byLvFlag =0;

/** \brief nmi int handler
 * 
 *  \brief 	nmi被触发时会调用此函数，函数在irq.c里定义为弱(weak)属性，默认仅作部分中断状态清除处理；用户用到nmi中
 * 			断时，建议将此函数剪切到interrupt.c中，在此函数中进行对应中断处理。
 * 
 *  \param[in] none
 *  \return none
 */ 
ATTRIBUTE_ISR void nmi_int_handler(void) 
{
	//user code
	if(csp_syscon_get_isr(SYSCON) & LVD_INT)
	{
		byLvFlag = 1;
		csp_syscon_clr_isr(SYSCON, LVD_INT);
	}
}


/** \brief use_lvd_as_nmi_demo：
 * 			配置lvd中断为NMI触发源的示例函数。
 * 			
 * 			LVD/内存校验错误/EXI0/外部时钟异常都可以触发NMI
 * 
 *  \param[in] none
 *  \return error code
 */
void lvd_trigger_nmi_demo(void)
{
	
	csi_set_lvd(LVD_INTF,LVD_30);  					//VDD掉电到3.0V即触发LVD中断
	csi_lvd_lvr_enable();
	
	csi_nmi_int_enable(NMI_SRC_LVDINT);				//配置NMI的触发源为LVD中断
	
	while(1)
	{
		//user code
		if(byLvFlag)
		{
			my_printf("VDD is lower than 3.0V");
			byLvFlag = 0;
		}
	}
	
}

