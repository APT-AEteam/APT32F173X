/***********************************************************************//** 
 * \file  iwdt_demo.c
 * \brief  IWDT_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-13 <td>V0.0 <td>ZJY    <td>initial
 * <tr><td> 2023-9-27 <td>V0.1  <td>GQQ   <td>code normalization
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/

#include "csi_drv.h"



/* externs function--------------------------------------------------------*/
/* externs variable------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variable------------------------------------------------------*/

#if(USE_IWDT_CALLBACK ==0)
	
/** \brief iwdt int handler
 * 			iwdt 中断处理函数。用户可以根据自己的需要编写中断处理函数
 * 			iwdt属于syscon的一部分，中断入口是syscon_int_handler。通过syscon的中断状态判断是否是iwdt中断。
 * 			故在调用iwdt中断处理函数前，一定要保证syscon_int_handler函数已经被调用。
 * 			
 * 			注意：syscon_int_handler 弱属性函数在irq.c中定义。syscon_int_handler 强属性函数在syscon_demo.c中定义
 * 
 *  \param[in] none
 *  \return error code
 */
void iwdt_int_handler(void)
{
	csp_syscon_clr_isr(SYSCON, IWDT_INT);
	csi_iwdt_feed();	
}
 
#endif

/** \brief iwdt normal mode
 * 
 *  \param[in] none
 *  \return error code
 */
int iwdt_normal_demo(void)
{
	int iRet = 0;

	csi_iwdt_init(IWDT_TO_1024);			//初始化看门狗，溢出时间为1000ms(系统复位时间)
	csi_iwdt_open();						//打开看门狗
	mdelay(5000);							//延时500ms
	csi_iwdt_feed();						//喂狗
	mdelay(500);
			
	return iRet;
}
/** \brief iwdt working with interrupt
 * 
 *  \param[in] none
 *  \return error code
 */
int iwdt_int_demo(void)
{
	int iRet = 0;

	csi_iwdt_init(IWDT_TO_1024);			//初始化看门狗，溢出时间为1000ms(系统复位时间)
	csi_iwdt_int_enable(IWDT_ALARMTO_2_8);	//使能看门狗报警中断，报警时间为4/8溢出时间
	csi_iwdt_open();						//打开看门狗
	//mdelay(700);

	return iRet;
}