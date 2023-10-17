/***********************************************************************//** 
 * \file  wwdt_demo.c
 * \brief  wwdt demo code
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-9-14 <td>V0.0  <td>YT     <td>initial,code normalization
 * </table>
 * *********************************************************************
*/

/* include ----------------------------------------------------------------*/
#include "csi_drv.h"
#include "board_config.h"

/* externs function--------------------------------------------------------*/
/* private function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

#if (USE_WWDT_CALLBACK == 1)

/** \brief  user_wwdt_callback：WWDT中断回调函数
 * 
 * 	\brief	用户定义，支持EVI中断处理，使用csi标准库，中断发生时会自动调用用户注册的回调函
 * 			数，用户可在回调函数里做自己的处理，而不需要关注具体的底层中断处理。
 *  \brief	csi初始化里不开启中断，需要进中断函数，需要调用csi_wwdt_int_enable函数；demo默认打开中断
 * 
 *  \param[in] ptWwdtBase: WWDT寄存器结构体指针，指向WWDT的基地址 
 *  \return none
 */ 
void user_wwdt_callback(csp_wwdt_t * ptWwdtBase)
{
	//user code
	
}

/** \brief	wwdt_int_callback_demo: WWDT中断服务函数，使用callback
 * 
 *  \brief 	WWDT的计数器计数到0x80时，可以产生一个警告中断
 * 
 *  \param[in] none
 *  \return none
 */
 csi_error_t wwdt_int_callback_demo(void)
{
	volatile uint32_t temp;	
	
	csi_wwdt_init(80);							//设置timeout时间为80ms 时间设置过大 会返回错误
	csi_wwdt_debug_enable(WWDT);				//可以配置在debug模式下，wdt是否继续计时		
	csi_wwdt_set_window_time(40);				//设置窗口值为40ms
	csi_wwdt_int_enable( );                     //中断使能
	
	csi_wwdt_register_callback(WWDT, user_wwdt_callback);	//注册中断回调函数
	csi_wwdt_open();							//WWDT一旦使能，软件将不能停止
	
	while(1) {
		temp = csi_wwdt_get_remaining_time() ;
		if (temp< 30) {						//此处仅为示例喂狗操作。实际应用需要对代码运行时间有全局的了解。只有在窗口内喂狗，芯片才不会复位
//			csi_wwdt_feed();		     // 屏蔽此处代码，会进中断	
			if (csi_wwdt_get_remaining_time()  < 79) {
				return CSI_ERROR;
			}
		}			
	}
	
	return CSI_ERROR;
}


#endif