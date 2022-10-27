
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

//extern system_clk_config_t g_tSystemClkConfig[];


/* example pin manager */

#define CONSOLE_IDX                     1
#define CONSOLE_TXD                     PC14
#define CONSOLE_RXD                     PC13
#define CONSOLE_TXD_FUNC                PC14_UART1_TX
#define CONSOLE_RXD_FUNC                PC13_UART1_RX

#define XIN_PIN							PA3
#define XOUT_PIN						PA4
#define XIN_PIN_FUNC					PA3
#define XOUT_PIN_FUNC					PA4


#define EXI_PIN                      	PA9
#define EXI_PIN_FUNC                 	PIN_FUNC_INPUT

#define SPI_IDX                 		0
#define SPI_MOSI_PIN            		PA14            
#define SPI_MISO_PIN            		PA15            
#define SPI_NSS_PIN             		PB5             
#define SPI_SCK_PIN             		PB4             
#define SPI_MOSI_PIN_FUNC       		PA14
#define SPI_MISO_PIN_FUNC       		PA15
#define SPI_NSS_PIN_FUNC        		PB5
#define SPI_SCK_PIN_FUNC				PB4

#define IIC_IDX		            		0
#define IIC_SDA_PIN             		PA13        
#define IIC_SCL_PIN             		PB0        
#define IIC_SDA_PIN_FUNC        		PA13
#define IIC_SCL_PIN_FUNC        		PB0

#define GPIO_IDX						1
#define GPIO_PIN                		PB2
#define GPIO_PIN_MSK	     			0x4
#define GPIO_PIN_FUNC           		PIN_FUNC_OUTPUT


#define EPT_IDX                 		0
#define EPT_PWMCHAY_PIN              	PA12
#define EPT_PWMCHAX_PIN					PA10
#define EPT_PWMCHAX_PIN_FUNC            PA10
#define EPT_PWMCHAY_PIN_FUNC			PA12
#define EPT_EBI_PIN						PB2
#define EPT_EBI							PB2

#define ADC_IDX							0
#define ADC_PIN							PA11
#define ADC_PIN_FUNC					PA11
#define ADC_CH							ADCIN12
#define ADC_PIN1						PA13
#define ADC_PIN1_FUNC					PA13
#define ADC_CH1							ADCIN14


#define TCH_IDX							0
#define TCH_PIN1						PB3
#define TCH_PIN2						PA8
#define	TCH_PIN1_FUNC					PB3
#define TCH_PIN2_FUNC					PA8_

#define SIO_IDX					        0
#define SIO_PIN					        PA2
#define SIO_PIN_FUNC					PA2

//csi Interrupt Handling Enable, 1/0(OPEN/CLOSE)
//系统中断处理配置默认是全部打开，对应的中断处理会调用处理函数，会占用的代码空间
//用户不需要用到某个中断时，可以将对应中断宏定义值设置为0可节约代码空间

#define	IFC_INT_HANDLE_EN				1		//IFC 
#define	ADC0_INT_HANDLE_EN				1		//ADC0
#define	ADC1_INT_HANDLE_EN				1		//ADC1
#define	DMA0_INT_HANDLE_EN				1		//DMA0
#define	DMA1_INT_HANDLE_EN				1		//DMA1
#define	WWDT_INT_HANDLE_EN				1		//WWDT
#define	GPTA0_INT_HANDLE_EN				1		//GPTA0
#define	GPTA1_INT_HANDLE_EN				1		//GPTA1
#define	GPTA2_INT_HANDLE_EN				1		//GPTA2
#define	GPTA3_INT_HANDLE_EN				1		//GPTA3
#define	GPTB0_INT_HANDLE_EN             1       //GPTB0
#define	GPTB1_INT_HANDLE_EN             1       //GPTB1
#define	GPTB2_INT_HANDLE_EN             1       //GPTB2
#define	GPTB3_INT_HANDLE_EN             1       //GPTB3
#define	GPTB4_INT_HANDLE_EN             1       //GPTB4
#define	GPTB5_INT_HANDLE_EN             1       //GPTB5
#define	DAC0_INT_HANDLE_EN	            1       //DAC0
#define	RTC_INT_HANDLE_EN				1		//RTC
#define	UART0_INT_HANDLE_EN				1		//UART0
#define	UART1_INT_HANDLE_EN				1		//UART1
#define	UART2_INT_HANDLE_EN				1		//UART2
#define	USART0_INT_HANDLE_EN			1		//USART0
#define	USART1_INT_HANDLE_EN			1		//USART1
#define	SIO0_INT_HANDLE_EN				1		//SIO0
#define	SIO1_INT_HANDLE_EN				1		//SIO1
#define	I2C_INT_HANDLE_EN				1		//I2C
#define	SPI0_INT_HANDLE_EN				1		//SPI0
#define	SPI1_INT_HANDLE_EN				1		//SPI1
#define	QSPI_INT_HANDLE_EN				1		//QSPI
#define	EXI0_INT_HANDLE_EN				1		//EXI0(GPIO external interrupt), EXI GROUP0 or GROUP16
#define	EXI1_INT_HANDLE_EN				1		//EXI1(GPIO external interrupt), EXI GROUP1 or GROUP17
#define	EXI2_3_INT_HANDLE_EN			1		//EXI2~3(GPIO external interrupt), EXI GROUP2~3 or GROUP18~19
#define	EXI4_9_INT_HANDLE_EN			1		//EXI4~9(GPIO external interrupt), EXI GROUP4~9 
#define	EXI10_15_INT_HANDLE_EN			1		//EXI10~15(GPIO external interrupt), EXI GROUP10~15 
#define	CNTA_INT_HANDLE_EN				1		//CNTA
#define	CAN_INT_HANDLE_EN				1		//CAN
#define	LPT_INT_HANDLE_EN				1		//LPT
#define	BT0_INT_HANDLE_EN				1		//BT0
#define	BT1_INT_HANDLE_EN				1		//BT1
#define	BT2_INT_HANDLE_EN				1		//BT2
#define	BT3_INT_HANDLE_EN				1		//BT3,用作系统时钟，默认开启。
#define	CMP0_INT_HANDLE_EN				1		//CMP0
#define	CMP1_INT_HANDLE_EN				1		//CMP1
#define	CMP2_INT_HANDLE_EN				1		//CMP2
#define	LED_INT_HANDLE_EN				1		//LED
#define	TKEY_INT_HANDLE_EN				1		//TOUCH


__attribute__((weak)) void board_init(void);

#ifdef __cplusplus
}
#endif

#endif /* _BOARD_CONFIG_H_ */

