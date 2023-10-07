/***********************************************************************//** 
 * \file  iwdt_demo_callback.c
 * \brief  iwdt demo callback code
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-9-28 <td>V0.0  <td>GQQ     <td>initial,code normalization
 * </table>
 * *********************************************************************
*/

/* include ----------------------------------------------------------------*/
#include "csi_drv.h"
#include "board_config.h"

/* externs function--------------------------------------------------------*/
/* private function--------------------------------------------------------*/
/* externs variable------------------------------------------------------*/
/* Private variable------------------------------------------------------*/

#if (USE_IWDT_CALLBACK == 1)

/** \brief  user_iwdt_callback：iWDT中断回调函数
 * 
 * 	\brief	用户定义，支持EVI中断处理，使用csi标准库，中断发生时会自动调用用户注册的回调函
 * 			数，用户可在回调函数里做自己的处理，而不需要关注具体的底层中断处理。
 * 
 *  \param[in] ptSysBase: iWDT寄存器结构体指针，指向iWDT的基地址,也即SYSCON的基地址 
 *  \return none
 */ 
void user_iwdt_callback(csp_syscon_t * ptSysBase)
{
	//user code
	csi_iwdt_feed();
	
}

/** \brief	iwdt_int_callback_demo: iWDT中断服务函数，使用callback
 * 
 *  \brief 	iWDT的计时到400ms时，可以产生一个报警中断
 * 
 *  \param[in] none
 *  \return none
 */
 csi_error_t iwdt_int_callback_demo(void)
{
	csi_error_t iRet = CSI_OK;
	
	csi_iwdt_init(IWDT_TO_1024);			//初始化看门狗，溢出时间为1000ms(系统复位时间)
	csi_iwdt_int_enable(IWDT_ALARMTO_2_8);	//使能看门狗报警中断，报警时间为4/8溢出时间
	
	csi_iwdt_register_callback(SYSCON, user_iwdt_callback);	//注册中断回调函数
	csi_iwdt_open();						//打开看门狗

	while(1);
	
	return iRet;
}


#endif