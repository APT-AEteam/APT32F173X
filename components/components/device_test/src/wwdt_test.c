/***********************************************************************//** 
 * \file  wwdt_test.c
 * \brief  wwdt测试代码 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-4-19 <td>V0.0 <td>YT    <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include "wwdt.h"
#include "iostring.h"
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/



/** \brief wwdt 复位测试代码
 * 在窗外喂狗，会导致复位。
 * 断点打到system_init()，如果复位，会返回改断点处，
 * 并且，WWDT寄存器恢复初始化设置，需要重新wwdt init后才能进行寄存器配置。
 *  \param[in] none
 *  \return error code
 */
csi_error_t wwdt_test(void)
{
	volatile uint32_t temp1, temp2, temp3;	
	
	csi_wwdt_init(80);							//设置timeout时间为80ms 时间设置过大 会返回错误
	csi_wwdt_debug_enable(ENABLE);				//可以配置在debug模式下，wdt是否继续计时		
	csi_wwdt_set_window_time(40);				//设置窗口值为40ms
	
	csi_wwdt_open();							//WWDT一旦使能，软件将不能停止
	mdelay(2);
	temp1 = csi_wwdt_get_remaining_time();
	if (temp1 < 77 || temp1 >79)
		return CSI_ERROR;
	
//	csi_wwdt_feed();							//如果在这里（窗口外）喂狗，将会引起芯片复位
	
	mdelay(62);
	temp2 = csi_wwdt_get_remaining_time(); 		//读取剩余时间
	if (temp2 < 14 || temp2 >17)
		return CSI_ERROR;
	
	while(1) {
		temp3 = csi_wwdt_get_remaining_time() ;
		if (temp3< 30) {						//此处仅为示例喂狗操作。实际应用需要对代码运行时间有全局的了解。只有在窗口内喂狗，芯片才不会复位
			csi_wwdt_feed();			
			if (csi_wwdt_get_remaining_time()  < 79) {
				return CSI_ERROR;
			}
		}			
	}
	
	return CSI_ERROR;
}



/** \brief wwdt 中断测试代码
 * 要进入WWDT中断，需要对WWDT中断使能，
 * 然后，窗外喂狗，CNT会计数到0x80，一旦变成0x80，就会进入中断。
 *  \param[in] none
 *  \return error code
 */
csi_error_t wwdt_irq_test(void)
{
	volatile uint32_t temp1, temp2, temp3;	
	
	csi_wwdt_init(80);							//设置timeout时间为80ms 时间设置过大 会返回错误
	csi_wwdt_debug_enable(ENABLE);				//可以配置在debug模式下，wdt是否继续计时		
	csi_wwdt_set_window_time(40);				//设置窗口值为40ms
	csi_wwdt_irq_enable(1);                     // 中断使能
	csi_wwdt_open();							//WWDT一旦使能，软件将不能停止
	mdelay(2);
	temp1 = csi_wwdt_get_remaining_time();
	if (temp1 < 77 || temp1 >79)
		return CSI_ERROR;
	
	//csi_wwdt_feed();							//如果在这里（窗口外）喂狗，将会引起芯片复位
	
	mdelay(62);
	temp2 = csi_wwdt_get_remaining_time(); 		//读取剩余时间
	if (temp2 < 14 || temp2 >17)
		return CSI_ERROR;
	
	while(1) {
		temp3 = csi_wwdt_get_remaining_time() ;
		if (temp3< 30) {						//此处仅为示例喂狗操作。实际应用需要对代码运行时间有全局的了解。只有在窗口内喂狗，芯片才不会复位
//			csi_wwdt_feed();			
			if (csi_wwdt_get_remaining_time()  < 79) {
				return CSI_ERROR;
			}
		}			
	}
	
	return CSI_ERROR;
}

