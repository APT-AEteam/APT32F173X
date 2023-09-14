
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


#ifdef __cplusplus
extern "C" {
#endif

//如果定义了DBG_PRINT2PC，串口对应脚和PC连上后，可以通过serial pane里的UART #x 看到串口数据
//#define DBG_PRINT2PC	

// Need to work with gcc_flash_dram16k_iram16k.ld or gcc_flash_dram24k_iram8k.ld
// Otherwise, the project will report an error
//#define USE_SRAM1_8K_AS_IRAM    1 
//#define USE_SRAM1_16K_AS_IRAM    1 


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

#define CONSOLE_IDX				1
#define CONSOLE_TXD             PA2
#define CONSOLE_RXD             PA3
#define CONSOLE_TXD_FUNC        PA2_UART1_TX
#define CONSOLE_RXD_FUNC        PA3_UART1_RX
#define CONSOLE_PORT     		(GPIOA)

#define XIN_PIN					PD0
#define XOUT_PIN				PD1
#define XIN_PIN_FUNC			PD0
#define XOUT_PIN_FUNC			PD1


//csi interrupt user callback enable: 1/0 = ENABLE/DISABLE
//csi库函数中断支持回调函数，通过宏USE_XXX_CALLBACK来实现次功能选择，若使能需用
#define USE_BT_CALLBACK			1		//BT(0/1/2/3)
#define USE_UART_CALLBACK		1		//UART(0/1/2)
#define USE_LED_CALLBACK		1		//LED(0)
#define USE_CNTA_CALLBACK		1		//CNTA(0)
#define USE_CMP_CALLBACK		1		//CMP(0/1/2)
#define USE_GPTA_CALLBACK		1		//GPTA(0/1/2/3)
#define USE_DAC_CALLBACK		1		//DAC(0)
#define USE_USART_CALLBACK		1		//USART(0/1)
#define USE_I2C_CALLBACK		1		//IIC(0)
#define USE_LPT_CALLBACK		1		//LPT(0)
#define USE_WWDT_CALLBACK		1		//WWDT(0)

//csi GUI Handling Enable, 1/0(OPEN/CLOSE)
//可视化编程，通过宏USE_GUI来实现功能选择
#define USE_GUI					0						

__attribute__((weak)) void board_init(void);

#ifdef __cplusplus
}
#endif

#endif /* _BOARD_CONFIG_H_ */

