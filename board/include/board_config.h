
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

#if	(IS_CHIP_1732 == 1) 
#define PLLP_VALUE               105000000U //MAX freq
#define PLLQ_VALUE				 96000000U  //MAX freq

#else
#define PLLP_VALUE               72000000U	//MAX freq
#define PLLQ_VALUE				 48000000U  //MAX freq
#endif

#endif

//console
//可通过宏定义选择UART或者USART；UART支持UART0~2, USART支持USART0~1
//选择不同的UART/USART时，注意对应的TX/RX管脚需要配置正确
#define CONSOLE_TYPE			CONSOLE_USART			//CONSOLE_UART/CONSOLE_USART = UART/USART 
#define CONSOLE_IP				USART1					//UART0~2/USART0~1
#define CONSOLE_TXD             PA2						//GPIO of TX
#define CONSOLE_RXD             PA3						//GPIO of RX
#define CONSOLE_TXD_FUNC        PA2_USART1_TX
#define CONSOLE_RXD_FUNC        PA3_USART1_RX
#define CONSOLE_PORT     		GPIOA					//GPIO Port
#define CONSOLE_BAUD    		115200					//baudrate

//emosc pin
#define XIN_PORT				GPIOD
#define XIN_PIN					PD0
#define XIN_PIN_FUNC			PD0_XIN
#define XOUT_PORT				GPIOD
#define XOUT_PIN				PD1
#define XOUT_PIN_FUNC			PD1_XOUT

//esosc pin
#define SXIN_PORT				GPIOC
#define SXIN_PIN				PC14
#define SXIN_PIN_FUNC			PC14_SXIN
#define SXOUT_PORT				GPIOC
#define SXOUT_PIN				PC15
#define SXOUT_PIN_FUNC			PC15_SXOUT


//csi interrupt user callback enable: 1/0 = ENABLE/DISABLE
//csi库函数中断支持回调函数，通过宏USE_XXX_CALLBACK来实现次功能选择，若使能需用
#define USE_BT_CALLBACK			0		//BT(0/1/2/3)
#define USE_UART_CALLBACK		0		//UART(0/1/2)
#define USE_SIO_CALLBACK		0		//SIO(0/1)
#define USE_LED_CALLBACK		0		//LED(0)
#define USE_CNTA_CALLBACK		0		//CNTA(0)
#define USE_CMP_CALLBACK		0		//CMP(0/1/2)
#define USE_GPTA_CALLBACK		0		//GPTA(0/1/2/3)
#define USE_GPTB_CALLBACK		0		//GPTB(0/1/2/3/4/5)
#define USE_SPI_CALLBACK		0		//SPI(0/1)
#define USE_DAC_CALLBACK		0		//DAC(0)
#define USE_USART_CALLBACK		0		//USART(0/1)
#define USE_LIN_CALLBACK        0       //LIN(0/1) LIN和USART使用的都是USART模块，同一个USART,使用了普通的USART功能，LIN功能就不能使用了
#define USE_IIC_CALLBACK		0		//IIC(0)
#define USE_LPT_CALLBACK		0		//LPT(0)
#define USE_WWDT_CALLBACK		0		//WWDT(0)
#define USE_ADC_CALLBACK		0		//ADC(0/1)
#define USE_RTC_CALLBACK		0		//RTC
#define USE_CAN_CALLBACK		0		//CAN(0)
#define USE_IWDT_CALLBACK		0		//IWDT(0)


//csi GUI Handling Enable, 1/0(OPEN/CLOSE)
//可视化编程，通过宏USE_GUI来实现功能选择
#define USE_GUI					0	


//FreeRTOS Enable, 1/0(OPEN/CLOSE)
//freeRTOS，如果使用的话请打开此宏	
#define USE_KERNEL_FREERTOS  0 
 				

__attribute__((weak)) void board_init(void);

#ifdef __cplusplus
}
#endif

#endif /* _BOARD_CONFIG_H_ */

