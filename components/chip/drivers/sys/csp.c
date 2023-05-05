/***********************************************************************//** 
 * \file  csp.c
 * \brief for debugging and using lower-level driver apis out of CSI
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-11-24 <td>V0.0  <td>WNN   <td>initial
 * <tr><td> 2021-5-13  <td>V0.0  <td>ZJY   <td>add *IWDT
 * </table>
 * *********************************************************************
*/

#include "csp.h"
#include "soc.h"

CLIC_Type  		*COREINT 	= (CLIC_Type 	*)(CLIC_BASE);
CORET_Type 		*CORETIMER  = (CORET_Type	*)(CORET_BASE);
csp_ifc_t 		*IFC   		= (csp_ifc_t 	*)(APB_IFC_BASE) ;
csp_syscon_t 	*SYSCON  	= (csp_syscon_t *)(APB_SYS_BASE);
csp_etb_t		*ETCB 		= (csp_etb_t 	*)(APB_ETCB_BASE); 
csp_dma_t		*DMA0 		= (csp_dma_t 	*)(APB_DMA0_BASE); 
csp_dma_t		*DMA1 		= (csp_dma_t 	*)(APB_DMA1_BASE); 
csp_adc_t 		*ADC0    	= (csp_adc_t 	*)(APB_ADC0_BASE);

#if	defined(IS_CHIP_1732) 
csp_adc_t 		*ADC1    	= (csp_adc_t 	*)(APB_ADC1_BASE);
#endif

csp_fvr_t 		*FVR    	= (csp_fvr_t 	*)(APB_FVR_BASE);
csp_gpio_t 		*GPIOA		= (csp_gpio_t	*)(APB_GPIOA_BASE);
csp_gpio_t 		*GPIOB		= (csp_gpio_t 	*)(APB_GPIOB_BASE);
csp_gpio_t 		*GPIOC		= (csp_gpio_t 	*)(APB_GPIOC_BASE);
csp_gpio_t 		*GPIOD		= (csp_gpio_t 	*)(APB_GPIOD_BASE);
csp_igrp_t		*GPIOGRP  	= (csp_igrp_t 	*)(APB_IGRP_BASE);
csp_usart_t 	*USART0   	= (csp_usart_t 	*)(APB_USART0_BASE);
csp_usart_t 	*USART1   	= (csp_usart_t 	*)(APB_USART1_BASE);
csp_uart_t 		*UART0   	= (csp_uart_t 	*)(APB_UART0_BASE);
csp_uart_t 		*UART1   	= (csp_uart_t 	*)(APB_UART1_BASE);
csp_uart_t 		*UART2   	= (csp_uart_t 	*)(APB_UART2_BASE);
csp_spi_t  		*SPI0     	= (csp_spi_t 	*)(APB_SPI0_BASE);
csp_spi_t  		*SPI1     	= (csp_spi_t 	*)(APB_SPI1_BASE);
csp_sio_t  		*SIO0     	= (csp_sio_t 	*)(APB_SIO0_BASE);
csp_sio_t  		*SIO1     	= (csp_sio_t 	*)(APB_SIO1_BASE);
csp_cmp_t  		*CMP0     	= (csp_cmp_t 	*)(APB_CMP0_BASE);
csp_cmp_t  		*CMP1     	= (csp_cmp_t 	*)(APB_CMP1_BASE);
csp_cmp_t  		*CMP2     	= (csp_cmp_t 	*)(APB_CMP2_BASE);
csp_i2c_t  		*I2C0     	= (csp_i2c_t 	*)(APB_I2C0_BASE);
csp_can_t 		*CAN0    	= (csp_can_t 	*)(APB_CAN_BASE);	
csp_cnta_t  	*CA0      	= (csp_cnta_t 	*)(APB_CNTA_BASE);
csp_gpta_t 		*GPTA0     	= (csp_gpta_t 	*)(APB_GPTA0_BASE);
csp_gpta_t 		*GPTA1     	= (csp_gpta_t 	*)(APB_GPTA1_BASE);
csp_gpta_t 		*GPTA2     	= (csp_gpta_t 	*)(APB_GPTA2_BASE);
csp_gpta_t 		*GPTA3     	= (csp_gpta_t 	*)(APB_GPTA3_BASE);
csp_gptb_t 		*GPTB0     	= (csp_gptb_t 	*)(APB_GPTB0_BASE);
csp_gptb_t 		*GPTB1     	= (csp_gptb_t 	*)(APB_GPTB1_BASE);
csp_gptb_t 		*GPTB2     	= (csp_gptb_t 	*)(APB_GPTB2_BASE);
csp_gptb_t 		*GPTB3     	= (csp_gptb_t 	*)(APB_GPTB3_BASE);
csp_gptb_t 		*GPTB4     	= (csp_gptb_t 	*)(APB_GPTB4_BASE);
csp_gptb_t 		*GPTB5     	= (csp_gptb_t 	*)(APB_GPTB5_BASE);

csp_lpt_t 		*LPT     	= (csp_lpt_t 	*)(APB_LPT_BASE) ;
csp_wwdt_t 		*WWDT      	= (csp_wwdt_t 	*)(APB_WWDT_BASE);
csp_led_t 		*LED      	= (csp_led_t 	*)(APB_LED_BASE);
csp_bt_t  		*BT0 	   	= (csp_bt_t 	*)(APB_BT0_BASE) ;
csp_bt_t  		*BT1 	   	= (csp_bt_t 	*)(APB_BT1_BASE) ;
csp_bt_t  		*BT2 	   	= (csp_bt_t 	*)(APB_BT2_BASE) ;
csp_bt_t  		*BT3 	   	= (csp_bt_t 	*)(APB_BT3_BASE) ;
csp_crc_t 		*CRC      	= (csp_crc_t    *)(APB_CRC_BASE);
csp_rtc_t 		*RTC      	= (csp_rtc_t 	*)(APB_RTC_BASE);
csp_iwdt_t 		*IWDT		= (csp_iwdt_t 	*)(APB_SYS_BASE);
csp_opa_t  		*OPA0 	   	= (csp_opa_t    *)(APB_OPA0_BASE);
csp_opa_t  		*OPA1 	   	= (csp_opa_t    *)(APB_OPA1_BASE);
csp_opa_t  		*OPA2 	   	= (csp_opa_t    *)(APB_OPA2_BASE);
csp_opa_t  		*OPA3 	   	= (csp_opa_t    *)(APB_OPA3_BASE);
csp_dac_t  		*DAC0 	   	= (csp_dac_t    *)(AHB_DAC_BASE);


csp_lin_t 		*LIN0   	= (csp_lin_t *)(APB_USART0_BASE);
csp_lin_t 		*LIN1   	= (csp_lin_t *)(APB_USART1_BASE);

