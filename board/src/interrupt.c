/***********************************************************************//** 
 * \file  interrupt.c
 * \brief  source file for interrypt handlers
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-10 <td>V0.0  <td>WNN   <td>initial
 * </table>
 * *********************************************************************
*/

/* Includes ------------------------------------------------------------------*/

#include "soc.h"
#include "gpio.h"
//#include "ept.h"
#include "adc.h"
#include "lpt.h"
#include "pin.h"
#include "spi.h"
#include "board_config.h"
#include "csp.h"

#include "rtc.h"

#include "qspi.h"
#include "irq.h"
#include "cnta.h"

/* externs function--------------------------------------------------------*/
extern void tick_irqhandler(void);
extern void uart_irqhandler(csp_uart_t *ptUartBase,uint8_t byIdx);
extern void usart_irqhandler(csp_usart_t *ptUsartBase,uint8_t byIdx);
extern void can_irqhandler(csp_can_t *ptCanBase);
extern void dma_irqhandler(csp_dma_t *ptDmaBase);						//DMA
extern void gpta0_irqhandler(csp_gpta_t *ptGptaBase);
extern void gpio_irqhandler(uint8_t byExiNum);
extern void ifc_irqhandler(void);
extern void bt_irqhandler(csp_bt_t *ptBtBase);
extern void wwdt_irqhandler(csp_wwdt_t *ptWwdtBase);
extern void cmp_irqhandler(csp_cmp_t *ptCmpBase);
extern void adc_irqhandler(csp_adc_t *ptAdcBase);
extern void syscon_irqhandler(csp_syscon_t *ptSysconBase);
extern void led_irqhandler(csp_led_t *ptLedBase);


/* private function--------------------------------------------------------*/
/* extern variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

/*************************************************************/
//CORET Interrupt
//EntryParameter:NONE
//ReturnValue:NONE
/*************************************************************/
void coret_int_handler(void)
{
	// ISR content ...
	tick_irqhandler();
}

void syscon_int_handler(void) 
{
    // ISR content ...
	if(SYSCON ->MISR & IWDT_INT)
	{
		nop;
		csp_syscon_int_clr(SYSCON, IWDT_INT);
	}
	if(SYSCON ->MISR & (0x1<<8))
	{
		nop;
		csp_syscon_int_clr(SYSCON, 0x1<<8);
	}
	nop;	
}

void ifc_int_handler(void)
{
	// ISR content ...
	
}
void adc0_int_handler(void) 
{	
    // ISR content ...
	adc_irqhandler(ADC0);
}

void adc1_int_handler(void) 
{
	// ISR content ...
	adc_irqhandler(ADC1);
}

 void dma0_int_handler(void)
{
	// ISR content ...
	dma_irqhandler(DMA0);
	//csp_qspi_set_int(QSPI, QSPI_TXE_INT,false);
}

void dma1_int_handler(void) 
{	
	// ISR content ...
	dma_irqhandler(DMA1);	
}

void gpta0_int_handler(void)
{
	 // ISR content ...
}

void gpta1_int_handler(void) 
{
    // ISR content ...	
}


void gpta2_int_handler(void)
{
	 // ISR content ...	 
}

void gpta3_int_handler(void) 
{
    // ISR content ...	
}

void gptb0_int_handler(void) 
{
    // ISR content ...	
}

void gptb1_int_handler(void) 
{
    // ISR content ...	
}

void gptb2_int_handler(void) 
{
    // ISR content ...	
}

void gptb3_int_handler(void) 
{
    // ISR content ...
}

void gptb4_int_handler(void) 
{
    // ISR content ...	
}

void gptb5_int_handler(void) 
{
    // ISR content ...	
}

void dac0_int_handler(void) 
{
    // ISR content ...
}


void usart0_int_handler(void) 
{
	usart_irqhandler(USART0, 0);
    // ISR content ...
//	volatile uint32_t wIsr= csp_usart_get_isr(USART0);
//	volatile uint16_t hwRecv = 0;
//	
//	if(wIsr & US_RXRDY_INT)							//usart rx			
//	{
//		nop;
//		csp_usart_clr_isr(USART0,US_RXRDY_INT);		//clear interrupt status
//		hwRecv = csp_usart_get_data(USART0);
//		csp_usart_put_char(USART0, 0x01);
//	}
//	
//	if(wIsr & US_TXRDY_INT)							//usart tx			
//	{
//		nop;
//		csp_usart_clr_isr(USART0,US_TXRDY_INT);		//clear interrupt status
//	
//	}
//	
//	if(wIsr & US_TXRIS_INT)							//usart txfifo 		
//	{
//		nop;
//		csp_usart_clr_isr(USART0,US_TXRIS_INT);		//clear interrupt status
//	}
//	
//	if(wIsr & US_RXRIS_INT)							//usart rxfifo 		
//	{
//		nop;
//		csp_usart_clr_isr(USART0,US_RXRIS_INT);		//clear interrupt status
//		hwRecv = csp_usart_get_data(USART0);
//		csp_usart_put_char(USART0, 0x02);
//	}
//	
//	if(wIsr & US_RORRIS_INT)						//usart rxfifo over 		
//	{
//		nop;
//		csp_usart_clr_isr(USART0,US_RORRIS_INT);		//clear interrupt status
//		csp_usart_put_char(USART0, 0x03);
//	}
//	
//	
//	if(wIsr & US_TIMEOUT_INT)						//usart rx timeover			
//	{
//		nop;
//		csp_usart_cr_cmd(USART0, US_STTTO);			//enable receive timeover
//		csp_usart_clr_isr(USART0,US_TIMEOUT_INT);	//clear interrupt status
//		csp_usart_put_char(USART0, 0x04);
//	}
//	
//	if(wIsr & US_RXBRK_INT)						//usart break 
//	{
//		nop;
//		csp_usart_clr_isr(USART0,US_RXBRK_INT);		//clear interrupt status
//		csp_usart_put_char(USART0, 0x05);
//	}
//	
//	if(wIsr & US_PARE_INT)							//usart parity
//	{
//		nop;
//		csp_usart_clr_isr(USART0,US_PARE_INT);		//clear interrupt status
//		csp_usart_put_char(USART0, 0x06);
//	}
//
//	
//	if(wIsr & US_OVRE_INT)							//usart rx over
//	{
//		nop;						
//		csp_usart_clr_isr(USART0,US_OVRE_INT);		//clear interrupt status
//		csp_usart_put_char(USART0, 0x07);
//	}
//	
//	if(wIsr & US_IDLE_INT)							//usart rx over
//	{
//		nop;						
//		csp_usart_clr_isr(USART0,US_IDLE_INT);		//clear interrupt status
//		csp_usart_put_char(USART0, 0x08);
//	}
//	
	
}

void usart1_int_handler(void) 
{
    // ISR content ...
	usart_irqhandler(USART1, 1);
}

void uart0_int_handler(void) 
{
    // ISR content ...
	uart_irqhandler(UART0,0);
}

void uart1_int_handler(void) 
{
	uart_irqhandler(UART1,1);
}

void uart2_int_handler(void) 
{
    // ISR content ...
	uart_irqhandler(UART2,2);
}

void qspi_int_handler(void) 
{
    // ISR content ...
	//qspi_irqhandler(QSPI);
}

void sio0_int_handler(void) 
{
    // ISR content ...
}

void sio1_int_handler(void) 
{
    // ISR content ...
}

void i2c_int_handler(void) 
{
    // ISR content ...
}

void spi0_int_handler(void) 
{
   // ISR content ...
   spi_irqhandler(SPI0);
}

void spi1_int_handler(void) 
{
   // ISR content ...
   spi_irqhandler(SPI1);
}

void exi0_int_handler(void) 
{
    // ISR content ...
	gpio_irqhandler(0);
}
void exi1_int_handler(void) 
{
    // ISR content ...
	gpio_irqhandler(1);
}
void exi2_3_int_handler(void) 
{
    // ISR content ...
   gpio_irqhandler(2);
}
void exi4_9_int_handler(void) 
{
    // ISR content ...
	gpio_irqhandler(3);
}
void exi10_15_int_handler(void) 
{
    // ISR content ...
	gpio_irqhandler(4);
}

void can_int_handler(void) 
{
    // ISR content ...
	can_irqhandler(CAN0);
}

void cnta_int_handler(void)
{
	// ISR content ...
	cnta_irqhandler(CA0);
}

void lpt_int_handler(void)
{
	// ISR content ...
	lpt_irqhandler(LPT);
}

void wwdt_int_handler(void)
{
	// ISR content ...
}

void rtc_int_handler(void)
{
	// ISR content ...
}

void cmp0_int_handler(void) 
{
    // ISR content ...
}

void cmp1_int_handler(void) 
{
    // ISR content ...
}

void cmp2_int_handler(void) 
{
    // ISR content ...
}

void led_int_handler(void) 
{
    // ISR content ...
	led_irqhandler(LED);
}

void bt0_int_handler(void) 
{
    // ISR content ...
	bt_irqhandler(BT0);
}

void bt1_int_handler(void) 
{
    // ISR content ...
	bt_irqhandler(BT1);
}

void bt2_int_handler(void) 
{
    // ISR content ...
	bt_irqhandler(BT2);	
}

void bt3_int_handler(void) 
{
    // ISR content ...
	bt_irqhandler(BT3);	
}
/*************************************************************/
/*************************************************************/
/*************************************************************/
//void PriviledgeVioHandler(void) 
//{
//    // ISR content ...
//	while(1){};
//}
//
//void SystemDesPtr(void) 
//{
//    // ISR content ...
//	while(1){};
//}
//
//void MisalignedHandler(void) 
//{
//    // ISR content ...
//	while(1){};
//}
//
//void IllegalInstrHandler(void) 
//{
//    // ISR content ...
//	while(1){};
//}
//
//void AccessErrHandler(void) 
//{
//    // ISR content ...
//	while(1){};
//}
//
//void BreakPointHandler(void) 
//{
//    // ISR content ...
//	while(1){};
//}
//
//void UnrecExecpHandler(void) 
//{
//    // ISR content ...
//	while(1){};
//}
//
//void Trap0Handler(void) 
//{
//    // ISR content ...
//	while(1){};
//}
//
//void Trap1Handler(void) 
//{
//    // ISR content ...
//	while(1){};
//}
//
//void Trap2Handler(void) 
//{
//    // ISR content ...
//	while(1){};
//}
//
//void Trap3Handler(void) 
//{
//    // ISR content ...
//	while(1){};
//
//}
//
//void PendTrapHandler(void) 
//{
//    // ISR content ...
//	while(1){};
//}

void tspend_handler(void)
{
	while(1);
}
/******************* (C) COPYRIGHT 2020 APT Chip *****END OF FILE****/

