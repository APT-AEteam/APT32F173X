/***********************************************************************//** 
 * \file  wwdt_demo.c
 * \brief  wwdt demo code
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-7-27 <td>V2.0 <td>WNN    <td>initial
 * <tr><td> 2023-3-15 <td>V0.1  <td>YT     <td>modify
 * <tr><td> 2023-9-14 <td>V0.2  <td>YT     <td>code normalization
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


#if (USE_WWDT_CALLBACK == 0)	

/** \brief	wwdt_int_handler: WWDT中断服务函数
 * 
 *  \brief 	WWDT发生中断时会调用此函数，函数在interrupt.c里定义为弱(weak)属性，默认不做处理；用户用到WWDT中
 * 			断时，请重新定义此函数，在此函数中进行对应中断处理，也可直接在interrupt.c里的函数里进行处理
 * 
 *  \param[in] none
 *  \return none
 */
 ATTRIBUTE_ISR void wwdt_int_handler(void)
 {
	 //用户直接在中断服务接口函数里处理中断，建议客户使用此模式
	 csp_wwdt_clr_isr(WWDT);
 }
 
 #endif
 
 /** \brief	wwdt_demo: WWDT 喂狗
 * 
 *  \brief	窗内喂狗不会引起复位，窗外喂狗会引起复位
 *  \brief	csi初始化里不开启中断，需要进中断函数，需要调用csi_wwdt_int_enable函数；demo默认打开中断
 *  \param[in] none
 *  \return error code
 */
 
csi_error_t wwdt_demo(void)
{
	volatile uint32_t temp;	
	
	csi_wwdt_init(80);							//设置timeout时间为80ms 时间设置过大 会返回错误
	csi_wwdt_debug_enable(WWDT);				//可以配置在debug模式下，wdt是否继续计时		
	csi_wwdt_set_window_time(40);				//设置窗口值为40ms
	csi_wwdt_int_enable( );                     //中断使能
	
	csi_wwdt_open();							//WWDT一旦使能，软件将不能停止
	
//	csi_wwdt_feed();							//如果在这里（窗口外）喂狗，将会引起芯片复位
	while(1) {
		temp = csi_wwdt_get_remaining_time() ;
		if (temp< 30) {						//此处仅为示例喂狗操作。实际应用需要对代码运行时间有全局的了解。只有在窗口内喂狗，芯片才不会复位
			csi_wwdt_feed();		        //屏蔽此处代码，会进中断	
			if (csi_wwdt_get_remaining_time()  < 79) {
				return CSI_ERROR;
			}
		}			
	}
	
	return CSI_ERROR;
	

	
	
	
	
}


