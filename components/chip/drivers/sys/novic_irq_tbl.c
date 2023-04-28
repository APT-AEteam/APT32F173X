/*
 * Copyright (C) 2016 YunOS Project. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//#include <csi_config.h>
#include <soc.h>

extern void Default_Handler(void);

void (*g_irqvector[64])(void);
void (*g_nmivector)(void);

void irq_vectors_init(void)
{
    int i;

    for (i = 0; i < 64; i++) {
        g_irqvector[i] = Default_Handler;
    }
	
//#ifdef	CONFIG_IRQ_LOOKUP		//Table lookup method for interrupt processing 
	//init all irq table
    g_irqvector[CORET_IRQn] 	= coret_int_handler;
	g_irqvector[IFC_IRQn] 		= syscon_int_handler;
	g_irqvector[ADC0_IRQn] 		= adc0_int_handler;
	g_irqvector[ADC1_IRQn] 		= adc1_int_handler;
	g_irqvector[DMA0_IRQn] 		= dma0_int_handler;
	g_irqvector[DMA1_IRQn] 		= dma1_int_handler;
	
	g_irqvector[GPTA0_IRQn] 	= gpta0_int_handler;
	g_irqvector[GPTA1_IRQn] 	= gpta1_int_handler;
	g_irqvector[GPTA2_IRQn] 	= gpta2_int_handler;
	g_irqvector[GPTA3_IRQn] 	= gpta3_int_handler;
	g_irqvector[GPTB0_IRQn] 	= gptb0_int_handler;
	g_irqvector[GPTB1_IRQn] 	= gptb1_int_handler;
	g_irqvector[GPTB2_IRQn] 	= gptb2_int_handler;
	g_irqvector[GPTB3_IRQn] 	= gptb3_int_handler;
	g_irqvector[GPTB4_IRQn] 	= gptb4_int_handler;
	g_irqvector[GPTB5_IRQn] 	= gptb5_int_handler;
	g_irqvector[DAC0_IRQn] 		= dac0_int_handler;
	
	g_irqvector[USART0_IRQn] 	= usart0_int_handler;
	g_irqvector[USART1_IRQn] 	= usart1_int_handler;
	g_irqvector[UART0_IRQn] 	= uart0_int_handler;
	g_irqvector[UART1_IRQn] 	= uart1_int_handler;
	g_irqvector[UART2_IRQn] 	= uart2_int_handler;
	g_irqvector[SIO0_IRQn] 		= sio0_int_handler;
	g_irqvector[SIO1_IRQn] 		= sio1_int_handler;
	g_irqvector[I2C_IRQn] 		= i2c_int_handler;
	g_irqvector[SPI0_IRQn] 		= spi0_int_handler;
	g_irqvector[SPI1_IRQn] 		= spi0_int_handler;
	g_irqvector[CAN_IRQn] 		= can_int_handler;
	
	g_irqvector[EXI0_IRQ_NUM] 		= exi0_int_handler;
	g_irqvector[EXI1_IRQ_NUM] 		= exi1_int_handler;
	g_irqvector[EXI2_IRQ_NUM] 		= exi2_3_int_handler;
	g_irqvector[EXI3_IRQ_NUM] 		= exi4_9_int_handler;
	g_irqvector[EXI4_IRQ_NUM] 		= exi10_15_int_handler;
	
	g_irqvector[CNTA_IRQn] 		= cnta_int_handler;
	g_irqvector[LPT_IRQn] 		= lpt_int_handler;
	g_irqvector[WWDT_IRQn] 		= wwdt_int_handler;
	g_irqvector[RTC_IRQn] 		= rtc_int_handler;
	g_irqvector[CMP0_IRQn] 		= cmp0_int_handler;
	g_irqvector[CMP1_IRQn] 		= cmp1_int_handler;
	g_irqvector[CMP2_IRQn] 		= cmp2_int_handler;
	g_irqvector[LED_IRQn] 		= led_int_handler;
	g_irqvector[BT0_IRQn] 		= bt0_int_handler;
	g_irqvector[BT1_IRQn] 		= bt1_int_handler;
	g_irqvector[BT2_IRQn] 		= bt2_int_handler;
	g_irqvector[BT3_IRQn] 		= bt3_int_handler;

//#endif

}
