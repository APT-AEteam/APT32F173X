
 
/***********************************************************************//** 
 * \file  soc.h
 * \brief  CSI Core Peripheral Access Layer Header File for
 *           CSKYSOC Device Series
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-13 <td>V0.0 <td>ZJY     <td>initial
 * </table>
 * *********************************************************************
*/  

#ifndef _SOC_H_
#define _SOC_H_

#include <stdint.h>
#include <csi_core.h>
#include <drv/clk.h>

#ifdef __cplusplus
extern "C" {
#endif


#ifndef ISOSC_VALU
#define ISOSC_VALUE			27000U
#endif

#ifndef EMOSC_32K_VALUE
#define EMOSC_32K_VALUE		32000U
#endif

#ifndef IMOSC_5M_VALUE
#define IMOSC_5M_VALUE		5556000U
#endif
#ifndef IMOSC_4M_VALUE
#define IMOSC_4M_VALUE      4194000U
#endif
#ifndef IMOSC_2M_VALUE
#define IMOSC_2M_VALUE      2094000U
#endif
#ifndef IMOSC_131K_VALUE
#define IMOSC_131K_VALUE	131000U
#endif

#ifndef HFOSC_24M_VALUE
#define HFOSC_24M_VALUE     24000000U
#endif
#ifndef HFOSC_12M_VALUE
#define HFOSC_12M_VALUE     12000000U
#endif
#ifndef HFOSC_6M_VALUE
#define HFOSC_6M_VALUE      6000000U
#endif
#ifndef HFOSC_3M_VALUE
#define HFOSC_3M_VALUE      3000000U
#endif

//UARTx id number
#define UART_IDX_NUM   		3
#define UART_RECV_MAX_LEN	128
//USARTx id number
#define USART_IDX_NUM   	2
#define USART_RECV_MAX_LEN	128

//DMA  id number
//max channel number
#define DMA_IDX_NUM			2
#define DMA_CH_MAX_NUM		6

//ETB dma start channel number
//ETB max channel  number
#define ETB_CH_DMA_STAR		ETB_CH20
#define ETB_CH_MAX_NUM		32

//FLASH
#define PFLASHBASE			0x00000000
#define PFLASHSIZE			0x00020000
#define PFLASHPAGESIZE		256
#define PFLASHPAGENUM		512
#define PFLASHLIMIT			(PFLASHBASE + PFLASHSIZE) 
#define USEROPTION_ADDR		0x000800C0
#define PROTECTION_ADDR		0x00080104
#define DFLASHBASE			0x10000000
#define DFLASHSIZE			0x00002000
#define DFLASHPAGESIZE		64
#define DFLASHPAGENUM		128
#define DFLASHLIMIT 		(DFLASHBASE + DFLASHSIZE) 

#define START_SRAM1_ADDR     0x20006000

typedef enum {
	
	NMI_EXPn                        = -2,      /* NMI Exception */
	SOFTWARE_IRQn					= 3U,
	CORET_IRQn						= 7U,
	SYSCON_IRQ_NUM			        = 16U,						 					
	IFC_IRQn						= 17U,
	ADC0_IRQn						= 18U,						 					
	ADC1_IRQn						= 19U,
	DMA0_IRQn						= 20U,
	DMA1_IRQn						= 21U,
	EXI0_IRQ_NUM					= 22U,
	EXI1_IRQ_NUM					= 23U,
	GPTA0_IRQn						= 24U,	
	GPTA1_IRQn						= 25U,
	GPTA2_IRQn						= 26U,
	GPTA3_IRQn						= 27U,			
	GPTB0_IRQn      				= 28U,
	GPTB1_IRQn      				= 29U,
	GPTB2_IRQn      				= 30U,
	GPTB3_IRQn      				= 31U,
	GPTB4_IRQn      				= 32U,
	GPTB5_IRQn      				= 33U,
	DAC0_IRQn     					= 34U,
	//DUMMY0_IRQn    				= 35U,
	USART0_IRQn    					= 36U,
	USART1_IRQn    					= 37U,
	UART0_IRQn      				= 38U,
	UART1_IRQn      				= 39U,
	UART2_IRQn      				= 40U,
	SIO0_IRQn						= 42U,
	SIO1_IRQn						= 43U,
	I2C_IRQn    					= 44U,
	SPI0_IRQn						= 45U,
	SPI1_IRQn						= 46U,
	CAN_IRQn      					= 47U,
	EXI2_IRQ_NUM    				= 48U,
	EXI3_IRQ_NUM    				= 49U,
	EXI4_IRQ_NUM    				= 50U,
	CNTA_IRQn						= 51U,
	LPT_IRQn						= 52U,
	WWDT_IRQn						= 53U,
	RTC_IRQn						= 54U,
	CMP0_IRQn      					= 55U,
	CMP1_IRQn      					= 56U,
	CMP2_IRQn     					= 57U,
	LED_IRQn     					= 58U,
	//DUMMY1_IRQn     				= 59U,
	BT0_IRQn      					= 60U,
	BT1_IRQn      					= 61U,
	BT2_IRQn      					= 62U,
	BT3_IRQn      					= 63U,
} irqn_type_e;

typedef enum {
	PA0			= 0U,
	PA1			= 1U,		
	PA2			= 2U,
    PA3			= 3U,
    PA4			= 4U,
    PA5			= 5U,
    PA6			= 6U,
    PA7			= 7U,
    PA8			= 8U,
    PA9			= 9U,
    PA10		= 10U,
    PA11		= 11U,
    PA12		= 12U,
    PA13		= 13U,
    PA14		= 14U,
    PA15		= 15U,
	
    PB0			= 16U,
    PB1		    = 17U,
    PB2		    = 18U,
    PB3		    = 19U,
    PB4		    = 20U,
    PB5		    = 21U,
	PB6			= 22U,
    PB7			= 23U,
    PB8		    = 24U,
    PB9	        = 25U,
    PB10		= 26U,
    PB11		= 27U,
    PB12		= 28U,
	PB13		= 29U,
	PB14		= 30U,
	PB15		= 31U,
	
	PC0		    = 32U,
	PC1		    = 33U,
	PC2		    = 34U,
	PC3		    = 35U,
	PC4		    = 36U,
	PC5		    = 37U,
	PC6		    = 38U,
	PC7		    = 39U,
	PC8		    = 40U,
	PC9		    = 41U,
	PC10		= 42U,
	PC11		= 43U,
	PC12		= 44U,
	PC13		= 45U,
	PC14		= 46U,
	PC15		= 47U,
	
	PD0		    = 48U,
	PD1		    = 49U,
	PD2		    = 50U,
	PD3		    = 51U,
	PD4		    = 52U,
	PD5		    = 53U,
	
} pin_name_e;


typedef enum {
	
	PA0_GPD				    =  0U,
	PA0_INPUT				=  1U,		//input
	PA0_OUTPUT				=  2U,		//output
	PA0_OUTPUT_MONI		    =  3U,		//output with monitor	
	PA0_SPI1_MISO		    =  4U,		
	PA0_GPTA0_CHA			=  5U,		
	PA0_GPTB3_CHAX	        =  6U,		
    PA0_EBI3			    =  7U,
    PA0_BT0_OUT             =  9U,
    PA0_LED_S2              =  10U,
	PA0_CPIN0P			    =  11U,
    PA0_OPA0P               =  12U,
	PA0_ADC_INA21            =  13U,
    
	
	PA1_GPD				    =  0U,
	PA1_INPUT				=  1U,		//input
	PA1_OUTPUT				=  2U,		//output
	PA1_OUTPUT_MONI		    =  3U,		//output with monitor
	PA1_SPI1_MOSI			=  4U,		
	PA1_GPTA0_CHB			=  5U,		
	PA1_GPTB4_CHAX		    =  6U,
    PA1_BT1_OUT             =  9U,
    PA1_LED_S3              =  10U,
    PA1_CPIN0N              =  11U,
	PA1_OPA0N			    =  12U,
    PA1_ADC_INA22            =  13U,
	
	
	PA2_GPD				    =  0U,
	PA2_INPUT				=  1U,		//input
	PA2_OUTPUT				=  2U,		//output
	PA2_OUTPUT_MONI		    =  3U,		//output with monitor
    PA2_USART1_TX			=  4U,
    PA2_GPTA1_CHA           =  5U,
    PA2_CLO                 =  6U,
    PA2_UART1_TX          	=  7U,
    PA2_BT2_OUT             =  9U,
	PA2_LED_S4              =  10U,
	PA2_UART0_TX            =  11U,
	PA2_OPA0X			    =  12U,
    PA2_ADC_INA23           =  13U,
	

	PA3_GPD				    =  0U,
	PA3_INPUT				=  1U,		//input
	PA3_OUTPUT				=  2U,		//output
	PA3_OUTPUT_MONI		    =  3U,		//output with monitor
	PA3_USART1_RX			=  4U,
    PA3_GPTA1_CHB           =  5U,
    PA3_GPTB5_CHAX          =  6U,
    PA3_UART1_RX            =  7U,
    PA3_BT3_OUT             =  9U,
	PA3_LED_S5              =  10U,
	PA3_UART0_RX            =  11U,
	PA3_OPA3P               =  12U,
	PA3_ADC_INB20           =  13U,
	
	
	PA4_GPD				    =  0U,
	PA4_INPUT				=  1U,		//input
	PA4_OUTPUT				=  2U,		//output
	PA4_OUTPUT_MONI		    =  3U,		//output with monitor
	PA4_SPI0_NSS			=  4U,
    PA4_USART1_CK           =  5U,
    PA4_DAC0_OUT            =  6U,
    PA4_LPT_OUT             =  7U,
	PA4_UART1_TX		    =  8U,
    PA4_UART0_TX            =  9U,	    	
	PA4_GROUP1              =  11U,
	PA4_OPA3N				=  12U,

	
	PA5_GPD				    =  0U,
	PA5_INPUT				=  1U,		//input
	PA5_OUTPUT				=  2U,		//output
	PA5_OUTPUT_MONI		    =  3U,		//output with monitor
	PA5_SPI0_SCK			=  4U,
    PA5_EBI3                =  5U,	
    PA5_LPT_IN              =  7U,
	PA5_UART1_RX			=  8U,
    PA5_UART0_RX            =  9U,
	PA5_LED_S6              =  10U,
	PA5_GROUP1              =  11U,
	PA5_OPA3X               =  12U,
	PA5_ADC_INB8            =  13U,
	
	
	PA6_GPD				    =  0U,
	PA6_INPUT				=  1U,		//input
	PA6_OUTPUT				=  2U,		//output
	PA6_OUTPUT_MONI		    =  3U,		//output with monitor
	PA6_SPI0_MISO			=  4U,
    PA6_GPTA2_CHA           =  5U,
    PA6_EBI0                =  6U,
	PA6_GPTB3_CHAX			=  8U,
	PA6_LED_S7              =  10U,
	PA6_GROUP1              =  11U,
	PA6_OPA2X               =  12U,
	PA6_ADC_INB9            =  13U,
	
	PA7_GPD				    =  0U,
	PA7_INPUT				=  1U,		//input
	PA7_OUTPUT				=  2U,		//output
	PA7_OUTPUT_MONI		    =  3U,		//output with monitor
	PA7_SPI0_MOSI			=  4U,
    PA7_GPTA2_CHB          	=  5U,
    PA7_GPTB3_CHAY          =  6U,
	PA7_GPTB4_CHAX			=  8U,
	PA7_GPTB0_CHAY          =  9U,
	PA7_GROUP1              =  11U,
	PA7_OPA2N               =  12U,
	PA7_ADC_INB10           =  13U,
	

	
	PA8_GPD				    =  0U,
	PA8_INPUT				=  1U,		//input
	PA8_OUTPUT				=  2U,		//output
	PA8_OUTPUT_MONI		    =  3U,		//output with monitor
    PA8_USART0_CK           =  5U,
    PA8_GPTB3_CHAX          =  6U,
	PA8_DAC0_OUT            =  7U,
	PA8_GPTB0_CHAX			=  8U,
	PA8_GPTB1_CHAX          =  9U,	
	PA8_CP0_OUT             =  11U,
	PA8_CPIN1P              =  12U,
	

	PA9_GPD				    =  0U,
	PA9_INPUT				=  1U,		//input
	PA9_OUTPUT				=  2U,		//output
	PA9_OUTPUT_MONI		    =  3U,		//output with monitor
	PA9_EBI1			    =  4U,
    PA9_USART0_TX           =  5U,
    PA9_GPTB4_CHAX          =  6U,
	PA9_GPTB1_CHAX			=  8U,
	PA9_GPTB2_CHAY          =  9U,	
	PA9_CP1_OUT             =  11U,
	PA9_CPIN1N              =  12U,
	PA9_ADC_INB4            =  13U,
	
	
	PA10_GPD				=  0U,
	PA10_INPUT				=  1U,		//input
	PA10_OUTPUT			    =  2U,		//output
	PA10_OUTPUT_MONI		=  3U,		//output with monitor
	PA10_CNTA_BUZ			=  4U,
    PA10_USART0_RX          =  5U,
    PA10_GPTB5_CHAX         =  6U,
    PA10_EBI0	  	        =  7U,
	PA10_GPTB2_CHAX		    =  8U,
	PA10_GPTB2_CHAX_1       =  9U,	
	PA10_CP2_OUT         =  11U,	
    PA10_ADC_INB5            =  13U,
	
	PA11_GPD				=  0U,
	PA11_INPUT				=  1U,		//input
	PA11_OUTPUT			    =  2U,		//output
	PA11_OUTPUT_MONI		=  3U,		//output with monitor
	PA11_CAN_RX			    =  4U,
    PA11_EBI0               =  5U,
    PA11_SPI0_NSS	  	    =  7U,
	PA11_GPTB3_CHAY			=  8U,
	PA11_GPTB0_CHAY         =  9U,
	PA11_LED_C6             =  10U,
	PA11_GPTA1_CHA          =  11U,	
	PA11_ADC_INB6           =  13U,
	

	PA12_GPD				=  0U,
	PA12_INPUT				=  1U,		//input
	PA12_OUTPUT			    =  2U,		//output
	PA12_OUTPUT_MONI		=  3U,		//output with monitor
	PA12_CAN_TX		        =  4U,
    PA12_UART1_TX           =  5U,
    PA12_UART0_TX           =  6U,
    PA12_SPI0_SCK	  	    =  7U,
	PA12_GPTB3_CHAX			=  8U,
	PA12_GPTB0_CHAX         =  9U,
	PA12_LED_C7             =  10U,
	PA12_GPTA2_CHA          =  11U,	
	PA12_ADC_INB7           =  13U,
	
	PA13_GPD				=  0U,
	PA13_INPUT				=  1U,		//input
	PA13_OUTPUT			    =  2U,		//output
	PA13_OUTPUT_MONI		=  3U,		//output with monitor
	PA13_SWDIO				=  4U,
    PA13_I2C_SCL            =  5U,
    PA13_UART1_RX           =  6U,
    PA13_UART0_RX	  	    =  7U,
	PA13_LED_C8             =  10U,

	
	PA14_GPD				=  0U,
	PA14_INPUT				=  1U,		
	PA14_OUTPUT			    =  2U,		
	PA14_OUTPUT_MONI		=  3U,		
	PA14_SWDCLK			    =  4U,
    PA14_I2C_SDA          	=  5U,
    PA14_UART1_TX           =  6U,
    PA14_UART0_TX		  	=  7U,
	PA14_LED_C9			    =  10U,
	
	PA15_GPD				=  0U,
	PA15_INPUT				=  1U,		
	PA15_OUTPUT			    =  2U,		
	PA15_OUTPUT_MONI		=  3U,		
	PA15_SPI0_NSS			=  4U,
    PA15_UART1_RX          	=  5U,
    PA15_UART0_RX           =  6U,
    PA15_SPI1_NSS		  	=  7U,
	PA15_EBI0			    =  8U,
	PA15_GROUP1             =  10U,
	PA15_CPIN7N             =  12U,
	PA15_ADC_INA6           =  13U,
   
	PB0_GPD				    =  0U,
	PB0_INPUT				=  1U,		//input
	PB0_OUTPUT				=  2U,		//output
	PB0_OUTPUT_MONI		    =  3U,		//output with monitor
	PB0_SIO0			    =  4U,
    PB0_GPTA3_CHA          	=  5U,
    PB0_GPTB4_CHAY          =  6U,
	PB0_GPTB5_CHAX			=  8U,
	PB0_GPTB1_CHAY          =  9U,
	PB0_GPTB0_CHAX          =  10U,
	PB0_GROUP1              =  11U,
	PB0_OPA2P               =  12U,
	PB0_ADC_INB11           =  13U,
	
	
	PB1_GPD				=  0U,
	PB1_INPUT			=  1U,		//input
	PB1_OUTPUT			=  2U,		//output
	PB1_OUTPUT_MONI		=  3U,		//output with monitor
	PB1_SIO1			=  4U,
    PB1_GPTA3_CHB       =  5U,
    PB1_GPTB5_CHAY      =  6U,
	PB1_LVDIN		    =  8U,
	PB1_GPTB2_CHAY      =  9U,
	PB1_GPTB1_CHAY      =  10U,
	PB1_GROUP1          =  11U,	
	PB1_CPIN3P          =  12U,
	PB1_ADC_INB12       =  13U,
	
	PB2_GPD				=  0U,
	PB2_INPUT			=  1U,		//input
	PB2_OUTPUT			=  2U,		//output
	PB2_OUTPUT_MONI		=  3U,		//output with monitor
	PB2_RTC_ALM			=  4U,
    PB2_USART0_CK       =  5U,
    PB2_EBI2            =  6U,
	PB2_CP0_OUT         =  11U,
	PB2_CPIN3N          =  12U,
	PB2_ADC_INB13       =  13U,
	
	PB3_GPD				=  0U,
	PB3_INPUT			=  1U,		//input
	PB3_OUTPUT			=  2U,		//output
	PB3_OUTPUT_MONI		=  3U,		//output with monitor
	PB3_SPI0_SCK        =  4U,
    PB3_GPTA2_CHB       =  5U,
    PB3_SPI1_SCK 	    =  7U,
	PB3_CLO			    =  8U,
	PB3_EBI1            =  9U,
	PB3_LED_C0          =  10U,
	PB3_OPA1P           =  12U,
	PB3_ADC_INA11       =  13U,
	
	
	PB4_GPD				    =  0U,
	PB4_INPUT				=  1U,		//input
	PB4_OUTPUT				=  2U,		//output
	PB4_OUTPUT_MONI		    =  3U,		//output with monitor
	PB4_SPI0_MISO		    =  4U,
    PB4_GPTA3_CHA          	=  5U,
    PB4_EBI3                =  6U,
    PB4_SPI1_MISO		  	=  7U,
	PB4_GPTB3_CHAX			=  8U,
	PB4_LED_C1              =  10U,
	PB4_OPA1N               =  12U,
	PB4_ADC_INA12           =  13U,
	
	
	PB5_GPD				=  0U,
	PB5_INPUT				=  1U,		//input
	PB5_OUTPUT				=  2U,		//output
	PB5_OUTPUT_MONI		    =  3U,		//output with monitor
	PB5_SPI0_MOSI			=  4U,
    PB5_GPTA3_CHB          	=  5U,
    PB5_LPT_IN              =  6U,
    PB5_SPI1_MOSI	  	    =  7U,
	PB5_GPTB4_CHAX		    =  8U,
	PB5_GROUP0              =  10U,
	PB5_OPA1X               =  12U,
	PB5_ADC_INA13           =  13U,
	
	
	PB6_GPD				    =  0U,
	PB6_INPUT				=  1U,		//input
	PB6_OUTPUT				=  2U,		//output
	PB6_OUTPUT_MONI		    =  3U,		//output with monitor
	PB6_UART1_TX			=  4U,
    PB6_GPTA1_CHA          	=  5U,
    PB6_I2C_SCL             =  6U,
    PB6_LPT_OUT	  	        =  7U,
	PB6_GPTB5_CHAX		    =  8U,
	PB6_GPTB3_CHAX          =  9U,
	PB6_LED_C2              =  10U,
	PB6_CPIN7P              =  12U,
	PB6_ADC_INA14           =  13U,
	
	PB7_GPD				    =  0U,
	PB7_INPUT				=  1U,		//input
	PB7_OUTPUT				=  2U,		//output
	PB7_OUTPUT_MONI		    =  3U,		//output with monitor
	PB7_UART1_RX			=  4U,
    PB7_GPTA1_CHB          	=  5U,
    PB7_I2C_SDA             =  6U,
	PB7_GPTB3_CHAY          =  9U,
	PB7_LED_C3              =  10U,
	PB7_CPIN8N              =  12U,
	PB7_FVROUT              =  13U,
	
	
	
	PB8_GPD				    =  0U,
	PB8_INPUT				=  1U,		//input
	PB8_OUTPUT				=  2U,		//output
	PB8_OUTPUT_MONI		    =  3U,		//output with monitor
	PB8_USART1_TX			=  4U,
    PB8_GPTA0_CHA          	=  5U,
    PB8_I2C_SCL             =  6U,
    PB8_CAN_RX	  	        =  7U,
	PB8_EBI1		        =  8U,
	PB8_GPTB4_CHAX          =  9U,
	PB8_LED_C4              =  10U,
	PB8_GROUP0              =  11U,
	PB8_CPIN9P              =  12U,
	PB8_BUF                 =  13U,
	
	
	PB9_GPD				    =  0U,
	PB9_INPUT				=  1U,		//input
	PB9_OUTPUT				=  2U,		//output
	PB9_OUTPUT_MONI		    =  3U,		//output with monitor
	PB9_USART1_RX			=  4U,
    PB9_GPTA0_CHB          	=  5U,
    PB9_I2C_SDA            =  6U,
    PB9_CAN_TX	  	       =  7U,
	PB9_EBI2		       =  8U,
	PB9_GPTB4_CHAY         =  9U,
	PB9_LED_C5             =  10U,
	PB9_GROUP0             =  11U,
	PB9_CPIN9N             =  12U,
	PB9_ADC_INA16          =  13U,
	
	PB10_GPD				=  0U,
	PB10_INPUT				=  1U,		//input
	PB10_OUTPUT				=  2U,		//output
	PB10_OUTPUT_MONI		=  3U,		//output with monitor
	PB10_GPTA1_CHA			=  4U,
    PB10_USART0_TX          =  5U,
    PB10_GPTA0_CHA          =  6U,
    PB10_I2C_SCL	  	    =  7U,
	PB10_UART0_RX		    =  8U,
	PB10_GROUP0              =  10U,
	PB10_CP1_OUT             =  11U,
	PB10_CPIN4P              =  12U,
	PB10_ADC_INB14           =  13U,
	
    PB11_GPD				=  0U,
	PB11_INPUT				=  1U,		//input
	PB11_OUTPUT				=  2U,		//output
	PB11_OUTPUT_MONI		=  3U,		//output with monitor
	PB11_GPTA1_CHB			=  4U,
    PB11_USART0_RX          =  5U,
    PB11_GPTA0_CHB          =  6U,
    PB11_I2C_SDA	  	    =  7U,
	PB11_UART0_TX		    =  8U,
	PB11_GROUP0             =  10U,
	PB11_CP2_OUT            =  11U,
	PB11_CPIN4N             =  12U,
	PB11_ADC_INB19          =  13U,
	
	PB12_GPD				=  0U,
	PB12_INPUT				=  1U,		//input
	PB12_OUTPUT				=  2U,		//output
	PB12_OUTPUT_MONI		=  3U,		//output with monitor
	PB12_SPI0_NSS			=  4U,
    PB12_BT3_OUT          	=  5U,
    PB12_EBI1               =  6U,
    PB12_USART0_CK	  	    =  7U,
	PB12_GROUP0              =  10U,
	PB12_CPIN5P              =  12U,
	PB12_ADC_INB0           =  13U,
	
	PB13_GPD				=  0U,
	PB13_INPUT				=  1U,		//input
	PB13_OUTPUT				=  2U,		//output
	PB13_OUTPUT_MONI		=  3U,		//output with monitor
	PB13_SPI0_SCK			=  4U,
    PB13_BT2_OUT          	=  5U,
    PB13_GPTB3_CHAY         =  6U,
    PB13_GPTB0_CHAY	  	    =  7U,
	PB13_GPTB0_CHAY_1         =  9U,
	PB13_CPIN5N             =  12U,
	PB13_ADC_INB1           =  13U,
	
	PB14_GPD				=  0U,
	PB14_INPUT				=  1U,		//input
	PB14_OUTPUT				=  2U,		//output
	PB14_OUTPUT_MONI		=  3U,		//output with monitor
	PB14_SPI0_MISO			=  4U,
    PB14_BT1_OUT          	=  5U,
    PB14_GPTB4_CHAY           =  6U,
    PB14_GPTB1_CHAY	  	    =  7U,
	PB14_GPTB0_CHAX         =  9U,
	PB14_CPIN6P             =  12U,
	PB14_ADC_INB2           =  13U,
	
	PB15_GPD				=  0U,
	PB15_INPUT				=  1U,		//input
	PB15_OUTPUT				=  2U,		//output
	PB15_OUTPUT_MONI		=  3U,		//output with monitor
	PB15_SPI0_MOSI			=  4U,
    PB15_BT0_OUT          	=  5U,
    PB15_GPTB5_CHAY         =  6U,
    PB15_GPTB2_CHAY	  	    =  7U,
	PB15_GPTB1_CHAY         =  9U,
	PB15_CPIN6N             =  12U,
	PB15_ADC_INB3           =  13U,
	
	PC0_GPD				    =  0U,
	PC0_INPUT				=  1U,		//input
	PC0_OUTPUT				=  2U,		//output
	PC0_OUTPUT_MONI		    =  3U,		//output with monitor
	PC0_LPT_IN			    =  4U,
    PC0_SIO0        	    =  5U,
    PC0_UART2_RX            =  6U,
	PC0_GPTB3_CHB           =  9U,
	PC0_CPIN11P             =  12U,
	PC0_ADC_INA17           =  13U,
	
	PC1_GPD				    =  0U,
	PC1_INPUT				=  1U,		//input
	PC1_OUTPUT				=  2U,		//output
	PC1_OUTPUT_MONI		    =  3U,		//output with monitor
	PC1_LPT_OUT			    =  4U,
    PC1_EBI0        	    =  5U,
    PC1_UART2_TX            =  6U,
	PC1_GPTB4_CHB           =  9U,
	PC1_CPIN12P             =  12U,
	PC1_ADC_INA18           =  13U,
	
	PC2_GPD				    =  0U,
	PC2_INPUT				=  1U,		//input
	PC2_OUTPUT				=  2U,		//output
	PC2_OUTPUT_MONI		    =  3U,		//output with monitor
	PC2_SPI1_NSS			=  4U,
    PC2_EBI1        	    =  5U,
	PC2_GPTB5_CHB           =  9U,
	PC2_CPIN13P             =  12U,
	PC2_ADC_INA19           =  13U,	
	
	PC3_GPD				    =  0U,
	PC3_INPUT				=  1U,		//input
	PC3_OUTPUT				=  2U,		//output
	PC3_OUTPUT_MONI		    =  3U,		//output with monitor
	PC3_SPI1_SCK		    =  4U,
    PC3_EBI2        	    =  5U,
	PC3_CPIN14P             =  12U,
	PC3_ADC_INA20           =  13U,

	PC4_GPD				    =  0U,
	PC4_INPUT				=  1U,		//input
	PC4_OUTPUT				=  2U,		//output
	PC4_OUTPUT_MONI		    =  3U,		//output with monitor
	PC4_I2C_SCL			    =  4U,
    PC4_UART0_RX       	    =  5U,
    PC4_CAN_RX              =  6U,
	PC4_LED_S6              =  10U,
	PC4_CPIN15P             =  12U,
	PC4_ADC_INB21           =  13U,
	
	PC5_GPD				    =  0U,
	PC5_INPUT				=  1U,		//input
	PC5_OUTPUT				=  2U,		//output
	PC5_OUTPUT_MONI		    =  3U,		//output with monitor
	PC5_I2C_SDA			    =  4U,
    PC5_UART0_TX        	=  5U,
    PC5_CAN_TX              =  6U,
	PC5_LED_S7              =  10U,
	PC5_ADC_INB22           =  13U,

	PC6_GPD				    =  0U,
	PC6_INPUT				=  1U,		//input
	PC6_OUTPUT				=  2U,		//output
	PC6_OUTPUT_MONI		    =  3U,		//output with monitor
	PC6_GPTA2_CHA		    =  4U,
    PC6_UART2_TX        	=  5U,
    PC6_EBI2                =  6U,
	PC6_ADC_INB15           =  13U,
	
	PC7_GPD				    =  0U,
	PC7_INPUT				=  1U,		//input
	PC7_OUTPUT				=  2U,		//output
	PC7_OUTPUT_MONI		    =  3U,		//output with monitor
	PC7_GPTA2_CHB			=  4U,
    PC7_UART2_RX        	=  5U,
    PC7_EBI3                =  6U,
	PC7_ADC_INB16           =  13U,
	
	PC8_GPD				    =  0U,
	PC8_INPUT				=  1U,		//input
	PC8_OUTPUT				=  2U,		//output
	PC8_OUTPUT_MONI		    =  3U,		//output with monitor
	PC8_GPTA3_CHA			=  4U,
    PC8_I2C_SCL        	    =  5U,
    PC8_UART1_TX            =  6U,
	PC8_ADC_INB17           =  13U,

	PC9_GPD				    =  0U,
	PC9_INPUT				=  1U,		//input
	PC9_OUTPUT				=  2U,		//output
	PC9_OUTPUT_MONI		    =  3U,		//output with monitor
	PC9_GPTA3_CHB		    =  4U,
    PC9_I2C_SDA        	    =  5U,
    PC9_UART1_RX            =  6U,
	PC9_ADC_INB18           =  13U,

	PC10_GPD				=  0U,
	PC10_INPUT				=  1U,		//input
	PC10_OUTPUT				=  2U,		//output
	PC10_OUTPUT_MONI		=  3U,		//output with monitor
	PC10_USART0_TX			=  4U,
    PC10_USART1_TX          =  5U,
    PC10_UART2_TX           =  6U,
    PC10_SPI1_SCK	  	    =  7U,
	PC10_I2C_SDA		    =  8U,
	PC10_BT0_OUT            =  9U,
	PC10_LED_C6             =  10U,
	PC10_ADC_INA7          =  13U,
	
	PC11_GPD				=  0U,
	PC11_INPUT				=  1U,		//input
	PC11_OUTPUT				=  2U,		//output
	PC11_OUTPUT_MONI		=  3U,		//output with monitor
	PC11_USART0_RX			=  4U,
    PC11_USART1_RX          =  5U,
    PC11_UART2_RX           =  6U,
    PC11_SPI1_MISO	  	    =  7U,
	PC11_I2C_SCL		    =  8U,
	PC11_BT1_OUT            =  9U,
	PC11_LED_C7             =  10U,
	PC11_ADC_INA8          =  13U,
	
	PC12_GPD				=  0U,
	PC12_INPUT				=  1U,		//input
	PC12_OUTPUT				=  2U,		//output
	PC12_OUTPUT_MONI		=  3U,		//output with monitor
	PC12_USART0_CK			=  4U,
    PC12_USART1_CK          =  5U,
    PC12_EBI2               =  6U,
    PC12_SPI1_MOSI	  	    =  7U,
	PC12_BT2_OUT            =  9U,
	PC12_LED_C8             =  10U,
	PC12_ADC_INA9          =  13U,
	
	PC13_GPD				=  0U,
	PC13_INPUT				=  1U,		//input
	PC13_OUTPUT				=  2U,		//output
	PC13_OUTPUT_MONI		=  3U,		//output with monitor
	PC13_RTC_ALM			=  4U,
    PC13_GPTB5_CHAX         =  5U,
    PC13_GPTB1_CHAX         =  6U,
    PC13_UART1_RX	  	    =  7U,
	PC13_GPTB0_CHAX         =  9U,
	PC13_UART0_RX           =  10U,
	PC13_CP0_OUT            =  11U,
	PC13_CPIN1P             =  12U,
	PC13_ADC_INA0           =  13U,
	
	PC14_GPD				=  0U,
	PC14_INPUT				=  1U,		//input
	PC14_OUTPUT				=  2U,		//output
	PC14_OUTPUT_MONI		=  3U,		//output with monitor
	PC14_SXIN			    =  4U,
    PC14_GPTB5_CHAY         =  5U,
    PC14_GPTB2_CHAY         =  6U,
    PC14_UART1_TX	  	    =  7U,
	PC14_GPTB1_CHAX         =  9U,
	PC14_UART0_TX           =  10U,
	PC14_CP1_OUT            =  11U,
	PC14_CPIN1N             =  12U,
	PC14_ADC_INA1           =  13U,
	
	PC15_GPD				=  0U,
	PC15_INPUT				=  1U,		//input
	PC15_OUTPUT				=  2U,		//output
	PC15_OUTPUT_MONI		=  3U,		//output with monitor
	PC15_SXOUT			    =  4U,
    PC15_EBI1               =  6U,
	PC15_GPTB2_CHAX         =  9U,
	PC15_CP2_OUT            =  11U,
	PC15_CPIN2P             =  12U,
	PC15_ADC_INA2           =  13U,
	
	PD0_GPD				    =  0U,
	PD0_INPUT				=  1U,		//input
	PD0_OUTPUT				=  2U,		//output
	PD0_OUTPUT_MONI		    =  3U,		//output with monitor
	PD0_XIN			        =  4U,
    PD0_GPTB5_CHAX          =  5U,
    PD0_EBI3                =  6U,
    PD0_UART2_RX	  	    =  7U,
	PD0_GPTB0_CHB           =  9U,
	PD0_LED_S0              =  10U,
	PD0_GROUP0              =  11U,
	PD0_CPIN10P             =  12U,
	PD0_ADC_INA3            =  13U,
	
	PD1_GPD				    =  0U,
	PD1_INPUT				=  1U,		//input
	PD1_OUTPUT				=  2U,		//output
	PD1_OUTPUT_MONI		    =  3U,		//output with monitor
	PD1_XOUT			    =  4U,
    PD1_SPI1_NSS            =  5U,
    PD1_EBI2                =  6U,
    PD1_UART2_TX	  	    =  7U,
	PD1_GPTB1_CHB           =  9U,
	PD1_LED_S1              =  10U,
	PD1_CLO                 =  11U,
	PD1_CPIN10N             =  12U,
	PD1_ADC_INA4            =  13U,
	
	PD2_GPD				    =  0U,
	PD2_INPUT				=  1U,		//input
	PD2_OUTPUT				=  2U,		//output
	PD2_OUTPUT_MONI		    =  3U,		//output with monitor
	PD2_SPI0_NSS		    =  4U,
    PD2_GPTA2_CHA           =  5U,
    PD2_EBI1                =  6U,
    PD2_SPI1_NSS	  	    =  7U,
	PD2_BT3_OUT             =  9U,
	PD2_LED_C9              =  10U,
	PD2_ADC_INA10           =  13U,
	
	PD3_GPD				    =  0U,
	PD3_INPUT				=  1U,		//input
	PD3_OUTPUT				=  2U,		//output
	PD3_OUTPUT_MONI		    =  3U,		//output with monitor
	PD3_USART1_CK           =  4U,
    PD3_SIO0                =  5U,
    PD3_EBI3                =  6U,
    PD3_CNTA_BUZ	  	    =  7U,
	PD3_CLO                 =  8U,
	PD3_SIO1                =  9U,
	PD3_GROUP0              =  10U,
	PD3_CPIN8P              =  12U,
	PD3_ADC_INA15           =  13U,
	
	PD4_GPD				    =  0U,
	PD4_INPUT				=  1U,		//input
	PD4_OUTPUT				=  2U,		//output
	PD4_OUTPUT_MONI		    =  3U,		//output with monitor
	PD4_CLO			        =  4U,
    PD4_SIO0                =  5U,
    PD4_GPTB1_CHAY          =  6U,
	PD4_EBI0                =  8U,
	PD4_GPTB2_CHAY          =  9U,
	PD4_GROUP1              =  11U,

	PD5_GPD				    =  0U,
	PD5_INPUT				=  1U,		//input
	PD5_OUTPUT				=  2U,		//output
	PD5_OUTPUT_MONI		    =  3U,		//output with monitor
	PD5_NRST			    =  4U,
    PD5_SPI1_SCK            =  5U,
    PD5_CNTA_BUZ            =  6U,
    PD5_BT2_OUT	  	        =  7U,
	PD5_GPTB2_CHB           =  9U,
	PD5_CPIN2N              =  12U,
	PD5_ADC_INA5            =  13U,
	
	PIN_GPD				    =  0U,
	PIN_INPUT				=  1U,		//input
	PIN_OUTPUT				=  2U,		//output
	PIN_OUTPUT_MONI			=  3U,		//output with monitor
	
	IOMAP					=  10U
} pin_func_e;

typedef enum{
	SWD_GRP0 = 0,
	SWD_GRP1,
	SWD_GRP2
}swd_grp_e;



#ifdef REMAP
	#define SRAMBASE		0x00000000
	#define SRAMSIZE 		0x00000800
	#define SRAMLIMIT		(SRAMBASE + SRAMSIZE) 
	#define MEMVECTORBASE	0x00000700
	#define MEMVECTORSIZE	(0x50 << 2)
#else
	#define SRAMBASE 		0x20000000
	#define SRAMSIZE 		0x0000C000
	#define SRAMLIMIT 		(SRAMBASE + SRAMSIZE) 
	#define MEMVECTORBASE 	0x20000F00
	#define MEMVECTORSIZE 	(0x50 << 2)
#endif

//APB for APT32F173
//--Peripheral Address Setting
#define APB_PERI_BASE		0x40000000UL 
#define APB_IFC_BASE    	(APB_PERI_BASE + 0x10000)
#define APB_SYS_BASE  	    (APB_PERI_BASE + 0x11000)
#define APB_ETCB_BASE    	(APB_PERI_BASE + 0x12000)
#define APB_ADC0_BASE    	(APB_PERI_BASE + 0x30000)
#define APB_ADC1_BASE    	(APB_PERI_BASE + 0x31000)
#define APB_FVR_BASE    	(APB_PERI_BASE + 0x32000)

#define APB_CNTA_BASE     	(APB_PERI_BASE + 0x50000)

#define APB_BT0_BASE       	(APB_PERI_BASE + 0x51000)
#define APB_BT1_BASE      	(APB_PERI_BASE + 0x52000)
#define APB_BT2_BASE       	(APB_PERI_BASE + 0x53000)
#define APB_BT3_BASE      	(APB_PERI_BASE + 0x54000)

#define APB_GPTA0_BASE    	(APB_PERI_BASE + 0x55000)
#define APB_GPTA1_BASE    	(APB_PERI_BASE + 0x56000)
#define APB_GPTA2_BASE    	(APB_PERI_BASE + 0x57000)
#define APB_GPTA3_BASE    	(APB_PERI_BASE + 0x58000)

#define APB_GPTB0_BASE   	(APB_PERI_BASE + 0x59000)
#define APB_GPTB1_BASE   	(APB_PERI_BASE + 0x5A000)
#define APB_GPTB2_BASE   	(APB_PERI_BASE + 0x5B000)
#define APB_GPTB3_BASE   	(APB_PERI_BASE + 0x5C000)
#define APB_GPTB4_BASE   	(APB_PERI_BASE + 0x5D000)
#define APB_GPTB5_BASE   	(APB_PERI_BASE + 0x5E000)

#define APB_RTC_BASE		(APB_PERI_BASE + 0x60000)
#define APB_LPT_BASE     	(APB_PERI_BASE + 0x61000)
#define APB_WWDT_BASE		(APB_PERI_BASE + 0x62000)
#define APB_LED_BASE		(APB_PERI_BASE + 0x70000)

#define APB_USART0_BASE   	(APB_PERI_BASE + 0x80000)
#define APB_USART1_BASE   	(APB_PERI_BASE + 0x81000)
#define APB_UART0_BASE  	(APB_PERI_BASE + 0x82000)
#define APB_UART1_BASE  	(APB_PERI_BASE + 0x83000)
#define APB_UART2_BASE  	(APB_PERI_BASE + 0x84000)

#define APB_SPI0_BASE  		(APB_PERI_BASE + 0x90000)
#define APB_SPI1_BASE  		(APB_PERI_BASE + 0x91000)

#define APB_I2C0_BASE   	(APB_PERI_BASE + 0xA0000)
#define APB_SIO0_BASE  		(APB_PERI_BASE + 0xB0000)
#define APB_SIO1_BASE  		(APB_PERI_BASE + 0xB1000)

#define APB_CMP0_BASE   	(APB_PERI_BASE + 0xD0000)
#define APB_CMP1_BASE  		(APB_PERI_BASE + 0xD1000)
#define APB_CMP2_BASE  		(APB_PERI_BASE + 0xD2000)

#define APB_OPA0_BASE   	(APB_PERI_BASE + 0xE0000)
#define APB_OPA1_BASE  		(APB_PERI_BASE + 0xE1000)
#define APB_OPA2_BASE  		(APB_PERI_BASE + 0xE2000)
#define APB_OPA3_BASE  		(APB_PERI_BASE + 0xE3000)

#define APB_CAN_BASE  		(APB_PERI_BASE + 0xF0000)
#define APB_STAMP_BASE  	(APB_PERI_BASE + 0xF1000)

#define APB_CRC_BASE  		(APB_PERI_BASE + 0x100000)
#define APB_DMA0_BASE  		(APB_PERI_BASE + 0x300000)
#define APB_DMA1_BASE  		(APB_PERI_BASE + 0x310000)

//
#define AHB_GPIO_BASE 		0x60000000
#define APB_GPIOA_BASE  	(AHB_GPIO_BASE + 0x0000) 	//A0  
#define APB_GPIOB_BASE  	(AHB_GPIO_BASE + 0x2000) 	//B0 
#define APB_GPIOC_BASE  	(AHB_GPIO_BASE + 0x4000) 	//A0  
#define APB_GPIOD_BASE  	(AHB_GPIO_BASE + 0x6000) 	//B0 

#define APB_IGRP_BASE		(AHB_GPIO_BASE + 0xF000)
//
#define AHB_DAC_BASE    	0x50200000
#define CORE_ADDR_BASE  	0xE0004000
#define CLIC_ADDR_BASE      0xE0800000 	


//--Interrupt Bit Position
#define	SOFTWARE_INT		(0x01ul<<3U)
#define	CORET_INT			(0x01ul<<7U)				 					
#define	SYSCON_INT			(0x01ul<<16U)
#define	IFC_INT				(0x01ul<<17U)
#define	ADC0_INT			(0x01ul<<18U)						 					
#define	ADC1_INT			(0x01ul<<19U)
#define	DMA0_INT			(0x01ul<<20U)
#define	DMA1_INT			(0x01ul<<21U)
#define	EXI0_INT			(0x01ul<<22U)
#define	EXI1_INT			(0x01ul<<23U)
#define	GPTA0_INT			(0x01ul<<24U)	
#define	GPTA1_INT			(0x01ul<<25U)
#define	GPTA2_INT			(0x01ul<<26U)
#define	GPTA3_INT			(0x01ul<<27U)			
#define	GPTB0_INT      		(0x01ul<<28U)
#define	GPTB1_INT      		(0x01ul<<29U)
#define	GPTB2_INT      		(0x01ul<<30U)
#define	GPTB3_INT      		(0x01ul<<31U)
#define	GPTB4_INT      		(0x01ul<<32U)
#define	GPTB5_INT      		(0x01ul<<33U)
#define	DAC0_INT     		(0x01ul<<34U)
//#define	DUMMY0_INT    	(0x01ul<<35U)
#define	USART0_INT    		(0x01ul<<36U)
#define	USART1_INT    		(0x01ul<<37U)
#define	UART0_INT      		(0x01ul<<38U)
#define	UART1_INT      		(0x01ul<<39U)
#define	UART2_INT      		(0x01ul<<40U)
//#define	DUMMY1_INT      (0x01ul<<41U)
#define	SIO0_INT			(0x01ul<<42U)
#define	SIO1_INT			(0x01ul<<43U)
#define	I2C_INT    			(0x01ul<<44U)
#define	SPI0_INT			(0x01ul<<45U)
#define	SPI1_INT			(0x01ul<<46U)
#define	CAN_INT      		(0x01ul<<47U)
#define	EXI2_INT    		(0x01ul<<48U)
#define	EXI3_INT    		(0x01ul<<49U)
#define	EXI4_INT    		(0x01ul<<50U)
#define	CNTA_INT			(0x01ul<<51U)
#define	LPT_INT				(0x01ul<<52U)
#define	WWDT_INT			(0x01ul<<53U)
#define	RTC_INT				(0x01ul<<54U)
#define	CMP0_INT      		(0x01ul<<55U)
#define	CMP1_INT      		(0x01ul<<56U)
#define	CMP2_INT     		(0x01ul<<57U)
#define	LED_INT     		(0x01ul<<58U)
//#define	DUMMY2_INT     	(0x01ul<<59U)
#define	BT0_INT      		(0x01ul<<60U)
#define	BT1_INT      		(0x01ul<<61U)
#define	BT2_INT      		(0x01ul<<62U)
#define	BT3_INT      		(0x01ul<<63U)


#define CONFIG_IRQ_NUM				32U
#define CK_CPU_ENALLNORMALIRQ		__enable_irq()
#define CK_CPU_DISALLNORMALIRQ		__disable_irq()

#define nop							asm ("nop")
#define NOP							asm ("nop")

//ISR Define for generating special interrupt related ASM (E906), with compile option -mistack

void sw_int_handler(void);				//CPU software interrupt
void coret_int_handler(void);			//CPU Systick interrupt 
void syscon_int_handler(void);			//System controller interrupt
void ifc_int_handler(void);				//Internal flash controller interrupt
void adc0_int_handler(void);			//ADC0 controller interrupt
void adc1_int_handler(void);			//ADC1 controller interrupt
void dma0_int_handler(void);			//DMA0 interrupt
void dma1_int_handler(void);			//DMA1 interrupt
void gpta0_int_handler(void);			//GPTA0 Interrupt
void gpta1_int_handler(void);			//GPTA1 Interrupt
void gpta2_int_handler(void);			//GPTA2 Interrupt
void gpta3_int_handler(void);			//GPTA3 Interrupt
void gptb0_int_handler(void);			//GPTB0 Interrupt
void gptb1_int_handler(void);			//GPTB1 Interrupt
void gptb2_int_handler(void);			//GPTB2 Interrupt
void gptb3_int_handler(void);			//GPTB3 Interrupt
void gptb4_int_handler(void);			//GPTB2 Interrupt
void gptb5_int_handler(void);			//GPTB3 Interrupt
void dac0_int_handler(void);			//DAC0 interrupt
void usart0_int_handler(void);			//USART0 interrupt
void usart1_int_handler(void);			//USART1 interrupt
void uart0_int_handler(void);			//UART0 interrupt
void uart1_int_handler(void);			//UART1 interrupt
void uart2_int_handler(void);			//UART2 interrupt
void sio0_int_handler(void);			//SIO0 interrupt
void sio1_int_handler(void);			//SIO1 interrupt
void i2c_int_handler(void);				//I2C  interrupt
void spi0_int_handler(void);			//SPI0 interrupt
void spi1_int_handler(void);			//SPI1 interrupt
void can_int_handler(void);				//CAN interrupt
void exi0_int_handler(void);			//External interrupt GROUP0, GROUP16 
void exi1_int_handler(void);			//External interrupt GROUP1, GROUP17 
void exi2_3_int_handler(void);			//External Interrupt GROUP2 ~ 3, GROUP18~19
void exi4_9_int_handler(void);			//External Interrupt GROUP4 ~ 9
void exi10_15_int_handler(void);		//External Interrupt GROUP10 ~ 15
void cnta_int_handler(void);			//COUNTER A interrupt
void lpt_int_handler(void);				//Low power timer interrupt
void wwdt_int_handler(void);			//Window watchdog timer interrupt
void rtc_int_handler(void);				//Real timer clock interrupt
void cmp0_int_handler(void);			//CMP0 interrupt
void cmp1_int_handler(void);			//CMP1 interrupt
void cmp2_int_handler(void);			//CMP2 interrupt
void led_int_handler(void);				//LED controller interrupt
void bt0_int_handler(void);				//BT0 interrupt
void bt1_int_handler(void);				//BT1interrupt
void bt2_int_handler(void);				//BT2 interrupt
void bt3_int_handler(void);				//BT3 interrupt


#ifdef __cplusplus
}
#endif

#endif  /* _SOC_H_ */
