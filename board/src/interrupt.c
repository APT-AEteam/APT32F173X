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
#include "adc.h"
#include "lpt.h"
#include "pin.h"
#include "spi.h"
#include "board_config.h"
#include "csp.h"
#include "rtc.h"
#include "irq.h"
#include "cnta.h"
#include <drv/tick.h>

/* externs function--------------------------------------------------------*/
extern void nmi_int_handler(void);
extern void tick_irqhandler(void);
extern void uart_irqhandler(csp_uart_t *ptUartBase,uint8_t byIdx);
extern void usart_irqhandler(csp_usart_t *ptUsartBase,uint8_t byIdx);
extern void can_irqhandler(csp_can_t *ptCanBase);
extern void dma_irqhandler(csp_dma_t *ptDmaBase);						//DMA
extern void gpta_irqhandler(csp_gpta_t *ptGptaBase);
extern void gpio_irqhandler(uint8_t byExiNum);
extern void ifc_irqhandler(void);
extern void bt_irqhandler0(csp_bt_t *ptBtBase);
extern void bt_irqhandler1(csp_bt_t *ptBtBase);
extern void bt_irqhandler2(csp_bt_t *ptBtBase);
extern void bt_irqhandler3(csp_bt_t *ptBtBase);
extern void cnta_irqhandler(csp_cnta_t *ptCntaBase);
extern void wwdt_irqhandler(void);
extern void cmp_irqhandler(csp_cmp_t *ptCmpBase);
extern void adc_irqhandler(csp_adc_t *ptAdcBase);
extern void syscon_irqhandler(csp_syscon_t *ptSysconBase);
extern void led_irqhandler(csp_led_t *ptLedBase);
extern void i2c_irqhandler(csp_i2c_t *ptIicBase);

extern void gptb_irqhandler(csp_gptb_t *ptGptbBase);
extern void sio_irqhandler(csp_sio_t *ptSioBase);
extern void rtc_irqhandler(csp_rtc_t *ptRtcBase);

/* private function--------------------------------------------------------*/
/* extern variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

/*************************************************************/
//CORET Interrupt
//EntryParameter:NONE
//ReturnValue:NONE
/*************************************************************/
void nmi_int_handler(void)
{
	if(csp_syscon_get_int_st(SYSCON) & LVD_INT)
	{
		nop;
		csp_syscon_int_clr(SYSCON, LVD_INT);
	}
	if(csp_exi_port_get_isr(SYSCON) & STATUS_EXI0)
	{
		nop;
		gpio_irqhandler(0);
	}
}

void coret_int_handler(void)
{
	// ISR content ...
//	tick_irqhandler();
}

void syscon_int_handler(void) 
{
    // ISR content ...

	if(csp_syscon_get_int_st(SYSCON) & LVD_INT)
	{
		nop;
		csp_syscon_int_clr(SYSCON, LVD_INT);
	}
	
	if(csp_syscon_get_int_st(SYSCON) & IWDT_INT)
	{
		nop;
		//csi_pin_toggle(PA05);
		csp_syscon_int_clr(SYSCON, IWDT_INT);
		//csi_iwdt_feed();
	}
}

void ifc_int_handler(void)
{
#if	IFC_INT_HANDLE_EN
	// ISR content ...
	ifc_irqhandler();
#endif
	
}
void adc0_int_handler(void) 
{	
#if	ADC0_INT_HANDLE_EN
	// ISR content ...
	adc_irqhandler(ADC0);
#endif	
}

void adc1_int_handler(void) 
{
#if	ADC1_INT_HANDLE_EN
	// ISR content ...
	adc_irqhandler(ADC1);
#endif	
}

 void dma0_int_handler(void)
{
#if DMA0_INT_HANDLE_EN	
	// ISR content ...
	dma_irqhandler(DMA0);
#endif
}

void dma1_int_handler(void) 
{
#if DMA1_INT_HANDLE_EN	
	// ISR content ...
	dma_irqhandler(DMA1);
#endif	
}

void wwdt_int_handler(void)
{
#if WWDT_INT_HANDLE_EN
	 // ISR content ...
	wwdt_irqhandler();
	
#endif
}

void gpta0_int_handler(void)
{
#if GPTA0_INT_HANDLE_EN	
	 // ISR content ...
	gpta_irqhandler(GPTA0);
#endif
}

void gpta1_int_handler(void) 
{
#if GPTA1_INT_HANDLE_EN	
	 // ISR content ...
	gpta_irqhandler(GPTA1);
#endif	
}


void gpta2_int_handler(void)
{
#if GPTA2_INT_HANDLE_EN	
	 // ISR content ...
	gpta_irqhandler(GPTA2);
#endif
}

void gpta3_int_handler(void) 
{
#if GPTA3_INT_HANDLE_EN	
	gpta_irqhandler(GPTA3);
	 // ISR content ...
#endif
}

void gptb0_int_handler(void) 
{
#if GPTB0_INT_HANDLE_EN		
    // ISR content ...	
	gptb_irqhandler(GPTB0);
#endif
}

void gptb1_int_handler(void) 
{
#if GPTB1_INT_HANDLE_EN		
    // ISR content ...	
	gptb_irqhandler(GPTB1);
#endif	
}

void gptb2_int_handler(void) 
{
#if GPTB2_INT_HANDLE_EN		
    // ISR content ...	
	gptb_irqhandler(GPTB2);
#endif	
}

void gptb3_int_handler(void) 
{
#if GPTB3_INT_HANDLE_EN		
    // ISR content ...	
	gptb_irqhandler(GPTB3);
#endif
}

void gptb4_int_handler(void) 
{
#if GPTB4_INT_HANDLE_EN		
    // ISR content ...	
	gptb_irqhandler(GPTB4);
#endif	
}

void gptb5_int_handler(void) 
{
#if GPTB5_INT_HANDLE_EN		
    // ISR content ...	
	gptb_irqhandler(GPTB5);
#endif
}

void dac0_int_handler(void) 
{
#if DAC0_INT_HANDLE_EN		
    // ISR content ...
#endif
}


void usart0_int_handler(void) 
{
#if	USART0_INT_HANDLE_EN	
	usart_irqhandler(USART0, 0);
#endif	
}

void usart1_int_handler(void) 
{
#if	USART0_INT_HANDLE_EN	
    // ISR content ...
	usart_irqhandler(USART1, 1);
#endif
}

void uart0_int_handler(void) 
{
#if	UART0_INT_HANDLE_EN
    // ISR content ...
	uart_irqhandler(UART0, 0);

#endif
}

void uart1_int_handler(void) 
{
#if	UART1_INT_HANDLE_EN
    // ISR content ...
	uart_irqhandler(UART1, 1);

#endif
}
void uart2_int_handler(void) 
{
#if	UART2_INT_HANDLE_EN
    // ISR content ...
	uart_irqhandler(UART2, 2);

#endif
}


void sio0_int_handler(void) 
{
#if	SIO0_INT_HANDLE_EN
	sio_irqhandler(SIO0);
    // ISR content ...
#endif
}

void sio1_int_handler(void) 
{
#if	SIO1_INT_HANDLE_EN
	sio_irqhandler(SIO1);
    // ISR content ...
#endif
}

void i2c_int_handler(void) 
{
#if	I2C_INT_HANDLE_EN
    // ISR content ...
	i2c_irqhandler(I2C0);
	
#endif
}

void spi0_int_handler(void) 
{
#if	SPI0_INT_HANDLE_EN
   // ISR content ...
   spi_irqhandler(SPI0);
#endif   
}

void spi1_int_handler(void) 
{
#if	SPI1_INT_HANDLE_EN
   // ISR content ...
   spi_irqhandler(SPI1);
#endif
}

void exi0_int_handler(void) 
{
#if	EXI0_INT_HANDLE_EN
    // ISR content ...
	gpio_irqhandler(0);
	csi_pin_toggle(PD0);
#endif
}
void exi1_int_handler(void) 
{
#if	EXI1_INT_HANDLE_EN
    // ISR content ...
	gpio_irqhandler(1);
#endif
}
void exi2_3_int_handler(void) 
{
#if	EXI2_3_INT_HANDLE_EN
    // ISR content ...
	gpio_irqhandler(2);
#endif
}
void exi4_9_int_handler(void) 
{
#if	EXI4_9_INT_HANDLE_EN
    // ISR content ...
	gpio_irqhandler(3);
#endif
}
void exi10_15_int_handler(void) 
{
#if	EXI10_15_INT_HANDLE_EN
    // ISR content ...
	gpio_irqhandler(4);
#endif
}

void can_int_handler(void) 
{
#if	CAN_INT_HANDLE_EN
    // ISR content ...
	can_irqhandler(CAN0);
#endif
}

void cnta_int_handler(void)
{
#if	CNTA_INT_HANDLE_EN
	// ISR content ...
	cnta_irqhandler(CA0);
#endif
}

void lpt_int_handler(void)
{
#if	LPT_INT_HANDLE_EN
    // ISR content ...
	lpt_irqhandler(LPT);
#endif
}

void rtc_int_handler(void)
{
#if	RTC_INT_HANDLE_EN
	// ISR content ...
	rtc_irqhandler(RTC);
#endif
}

void cmp0_int_handler(void) 
{
#if	CMP0_INT_HANDLE_EN
    // ISR content ...
	cmp_irqhandler(CMP0);
#endif
}

void cmp1_int_handler(void) 
{
#if	CMP1_INT_HANDLE_EN
    // ISR content ...
	cmp_irqhandler(CMP1);
#endif
}

void cmp2_int_handler(void) 
{
#if	CMP2_INT_HANDLE_EN
    // ISR content ...
	cmp_irqhandler(CMP2);
#endif
}

void led_int_handler(void) 
{
#if	LED_INT_HANDLE_EN
    // ISR content ...
	led_irqhandler(LED);
#endif
}

void bt0_int_handler(void) 
{
#if	BT0_INT_HANDLE_EN
    // ISR content ...
	bt_irqhandler0(BT0);
#endif
}

void bt1_int_handler(void) 
{
#if	BT1_INT_HANDLE_EN
    // ISR content ...
	bt_irqhandler1(BT1);
#endif
}

void bt2_int_handler(void) 
{
#if	BT2_INT_HANDLE_EN
    // ISR content ...
	bt_irqhandler2(BT2);	
#endif
}

void bt3_int_handler(void) 
{
#if	BT3_INT_HANDLE_EN
//    // ISR content ...
	csi_tick_increase();
	bt_irqhandler3(BT3);   //BT3 is for systick!!!
#endif	
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

void sw_int_handler(void)
{
	while(1);
}
/******************* (C) COPYRIGHT 2020 APT Chip *****END OF FILE****/

