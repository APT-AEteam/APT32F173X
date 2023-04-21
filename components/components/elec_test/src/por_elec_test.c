/***********************************************************************//** 
 * \file  por_elec_test.c
 * \brief  por电气参数测试代码
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-4-6 <td>V0.0  <td>YYM   <td>initial
 * </table>
 * *********************************************************************
*/


/* include ----------------------------------------------------------------*/
#include "reliability.h"
#include "pin.h"
#include "cmp.h"

/** \brief POR 工作电压测试
 *	
 * 主程序IO toggle，正常上电后，逐渐减少供电，IO口不翻转，记录芯片停止工作的电压值，即为falling。
 * 再逐渐调高供电，IO口翻转，记录芯片重新开始工作的电压值，即为rising。
 * 测量工具：示波器、电压源(KEYSIGHT E36106B)
 * 
 *  \param[in] none
 *  \return none
 */
int syscon_por_test(void)
{
	csi_lvd_disable();	
	csi_pin_set_mux(PA5, PA5_OUTPUT); 
	csi_pin_set_high(PA5);	
	while(1)
	{
		mdelay(10);
		csi_pin_toggle(PA5);			
	}	
}

/** \brief POR上电时间测试
 *	
 * 启动代码开始时PB2置高，POR后，测量电压0~1.64V时间间隔，测量电压1.64V到PB2的上升沿时间间隔
 * IO口置高汇编代码，需要放在启动代码的最前面，这样可最大程度上规避，代码造成的测量误差。
 * 
 * 测量工具：示波器、电压源(KEYSIGHT E36106B)
 * 
 */
 
 /*
   在 startup.S中加入以下汇编代码
    * 
    //set PB2 out
   la a1,0x60002000
   la a2,0x200
   sw a2,(a1)

   //set PB2 high
   la a1,0x6000200c
   la a2,0x04
   sw a2,(a1)
 */