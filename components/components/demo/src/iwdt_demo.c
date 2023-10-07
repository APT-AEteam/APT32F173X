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