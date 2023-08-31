
/***********************************************************************//** 
 * \file  board_config.h
 * \brief  system clock management for cpu clock(HCLK) and peri clock(PCLK)
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-10 <td>V0.0  <td>WNN   <td>initial
 * </table>
 * *********************************************************************
*/

#ifndef _BOARD_CONFIG_H_
#define _BOARD_CONFIG_H_

#include <stdint.h>
#include "soc.h"
#include "sys_clk.h"

#ifdef __cplusplus
extern "C" {
#endif

//如果定义了DBG_PRINT2PC，串口对应脚和PC连上后，可以通过serial pane里的UART #x 看到串口数据
//#define DBG_PRINT2PC	

// Need to work with gcc_flash_dram16k_iram16k.ld or gcc_flash_dram24k_iram8k.ld
// Otherwise, the project will report an error
//#define CODE_REMAP_TO_IRAM

#ifndef EMOSC_VALUE
#define EMOSC_VALUE             24000000U
#endif

#ifndef ESOSC_VALUE
#define ESOSC_VALUE             32768U
#endif

#ifndef PLL_VALUE

#if	defined(IS_CHIP_1732) 
#define PLL_VALUE               105000000U
#else
#define PLL_VALUE               72000000U
#endif

#endif

/* example pin manager */

#define CONSOLE_IDX                     1
#define CONSOLE_TXD                     PA2
#define CONSOLE_RXD                     PA3
#define CONSOLE_TXD_FUNC                PA2_UART1_TX
#define CONSOLE_RXD_FUNC                PA3_UART1_RX

#define XIN_PIN							PD0
#define XOUT_PIN						PD1
#define XIN_PIN_FUNC					PD0
#define XOUT_PIN_FUNC					PD1


//csi interrupt user callback enable: 1/0 = ENABLE/DISABLE
//csi库函数中断支持回调函数，通过宏USE_XXX_CALLBACK来实现次功能选择，若使能需用
#define USE_BT_CALLBACK					1		//UART(0/1/2)
#define USE_UART_CALLBACK				1		//UART(0/1/2)

//csi Interrupt Handling Enable, 1/0(OPEN/CLOSE)
//系统中断处理配置默认是全部打开，对应的中断处理会调用处理函数，会占用的代码空间
//用户不需要用到某个中断时，可以将对应中断宏定义值设置为0可节约代码空间



__attribute__((weak)) void board_init(void);

#ifdef __cplusplus
}
#endif

#endif /* _BOARD_CONFIG_H_ */

