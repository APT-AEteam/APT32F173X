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

#include "csi_drv.h"
#include "board_config.h"
#include <csi_config.h>

#ifdef CONFIG_KERNEL_FREERTOS
#include <csi_kernel.h>
#endif

			
/* externs function--------------------------------------------------------*/
extern void nmi_int_handler(void);
extern void usart_irqhandler(csp_usart_t *ptUsartBase,uint8_t byIdx);
extern void can_irqhandler(csp_can_t *ptCanBase);
extern void dma_irqhandler(csp_dma_t *ptDmaBase);						//DMA
extern void gpio_irqhandler(uint8_t byExiNum);
extern void ifc_irqhandler(void);
extern void bt_irqhandler0(csp_bt_t *ptBtBase);
extern void bt_irqhandler1(csp_bt_t *ptBtBase);
extern void bt_irqhandler2(csp_bt_t *ptBtBase);
extern void bt_irqhandler3(csp_bt_t *ptBtBase);
extern void wwdt_irqhandler(void);
extern void adc_irqhandler(csp_adc_t *ptAdcBase);
extern void syscon_irqhandler(csp_syscon_t *ptSysconBase);
extern void i2c_irqhandler(csp_iic_t *ptIicBase);

extern void sio_irqhandler(csp_sio_t *ptSioBase);
extern void rtc_irqhandler(csp_rtc_t *ptRtcBase);

extern void lpt_irqhandler(csp_lpt_t *ptLptBase);
extern void dac_irqhandler(csp_dac_t *ptDacBase);


/*************************************************************/
//CORET Interrupt
//EntryParameter:NONE
//ReturnValue:NONE
/*************************************************************/
ATTRIBUTE_ISR void nmi_int_handler(void)
{
	if(csp_syscon_get_isr(SYSCON) & LVD_INT)
	{
		nop;
		csp_syscon_clr_isr(SYSCON, LVD_INT);
	}
	if(csp_exi_get_isr(SYSCON) & EXI_STATUS_GRP0)
	{
		nop;
		//gpio_irqhandler(0);
	}
}

ATTRIBUTE_ISR void coret_int_handler(void)
{
	// ISR content ...
}

ATTRIBUTE_ISR void syscon_int_handler(void) 
{
    // ISR content ...

	if(csp_syscon_get_isr(SYSCON) & LVD_INT)
	{
		nop;
		csp_syscon_clr_isr(SYSCON, LVD_INT);
	}
	
	if(csp_syscon_get_isr(SYSCON) & IWDT_INT)
	{
		nop;
		//csi_pin_toggle(PA05);
		csp_syscon_clr_isr(SYSCON, IWDT_INT);
		//csi_iwdt_feed();
	}
}

ATTRIBUTE_ISR void ifc_int_handler(void)
{
	CSI_INTRPT_ENTER();
	ifc_irqhandler();	//only DFLASH paramode write would use IFC interrupt handler. DO NOT rewrite this funcionï¼
	CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR __attribute__((weak)) void adc0_int_handler(void) 
{	
#if	ADC0_INT_HANDLE_EN
	// ISR content ...
	CSI_INTRPT_ENTER();
	adc_irqhandler(ADC0); //this is a weak function defined in adc_demo.c, for better efficiency, we recommand user directly implement IRQ handler here without any function call.
	CSI_INTRPT_EXIT();
#endif	
}

ATTRIBUTE_ISR __attribute__((weak)) void adc1_int_handler(void) 
{
#if	ADC1_INT_HANDLE_EN
	// ISR content ...
	#if	defined(IS_CHIP_1732) 
		CSI_INTRPT_ENTER();	
		adc_irqhandler(ADC1);//this is a weak function defined in adc_demo.c, for better efficiency, we recommand user directly implement IRQ handler here without any function call.
		CSI_INTRPT_EXIT();
	#endif	
#endif	
}

ATTRIBUTE_ISR __attribute__((weak)) void dma0_int_handler(void)
{
    // ISR content ...
	CSI_INTRPT_ENTER();
	dma_irqhandler(DMA0);
	CSI_INTRPT_EXIT();
}


ATTRIBUTE_ISR __attribute__((weak)) void dma1_int_handler(void) 
{
    // ISR content ...
	CSI_INTRPT_ENTER();
	dma_irqhandler(DMA1);
	CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR __attribute__((weak)) void wwdt_int_handler(void)
{
	CSI_INTRPT_ENTER();
#if (USE_WWDT_CALLBACK==1)
	csi_wwdt_irqhandler(WWDT,0);
#else
	csp_wwdt_clr_isr(WWDT,csp_wwdt_get_isr(WWDT));
#endif
	CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR __attribute__((weak)) void gpta0_int_handler(void)
{
	// ISR content ...
	CSI_INTRPT_ENTER();
#if (USE_GPTA_CALLBACK == 1)
	csi_gpta_irqhandler(GPTA0, 0);
#else
	csp_gpta_clr_isr(GPTA0, csp_gpta_get_isr(GPTA0));	
#endif
	CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR __attribute__((weak)) void gpta1_int_handler(void) 
{
	// ISR content ...
	CSI_INTRPT_ENTER();
#if (USE_GPTA_CALLBACK == 1)
	csi_gpta_irqhandler(GPTA1, 1);
#else
	csp_gpta_clr_isr(GPTA1, csp_gpta_get_isr(GPTA1));	
#endif
	CSI_INTRPT_EXIT();	
}


ATTRIBUTE_ISR __attribute__((weak)) void gpta2_int_handler(void)
{
	// ISR content ...
	CSI_INTRPT_ENTER();
#if (USE_GPTA_CALLBACK == 1)
	csi_gpta_irqhandler(GPTA2, 2);
#else
	csp_gpta_clr_isr(GPTA2, csp_gpta_get_isr(GPTA2));	
#endif
	CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR __attribute__((weak)) void gpta3_int_handler(void) 
{
	// ISR content ...
	CSI_INTRPT_ENTER();
#if (USE_GPTA_CALLBACK == 1)
	csi_gpta_irqhandler(GPTA3, 3);
#else
	csp_gpta_clr_isr(GPTA3, csp_gpta_get_isr(GPTA3));	
#endif
	CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR __attribute__((weak)) void gptb0_int_handler(void) 
{
	// ISR content ...
	CSI_INTRPT_ENTER();
#if (USE_GPTB_CALLBACK == 1)
	csi_gptb_irqhandler(GPTB0, 0);
#else
	csp_gptb_clr_isr(GPTB0, csp_gptb_get_isr(GPTB0));
#endif
	CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR __attribute__((weak)) void gptb1_int_handler(void) 
{
	// ISR content ...
	CSI_INTRPT_ENTER();
#if (USE_GPTB_CALLBACK == 1)
	csi_gptb_irqhandler(GPTB1, 1);
#else
	csp_gptb_clr_isr(GPTB1, csp_gptb_get_isr(GPTB1));
#endif
	CSI_INTRPT_EXIT();	
}

ATTRIBUTE_ISR __attribute__((weak)) void gptb2_int_handler(void) 
{
	// ISR content ...
	CSI_INTRPT_ENTER();
#if (USE_GPTB_CALLBACK == 1)
	csi_gptb_irqhandler(GPTB2, 2);
#else
	csp_gptb_clr_isr(GPTB2, csp_gptb_get_isr(GPTB2));
#endif
	CSI_INTRPT_EXIT();	
}

ATTRIBUTE_ISR __attribute__((weak)) void gptb3_int_handler(void) 
{
	// ISR content ...
	CSI_INTRPT_ENTER();
#if (USE_GPTB_CALLBACK == 1)
	csi_gptb_irqhandler(GPTB3, 3);
#else
	csp_gptb_clr_isr(GPTB3, csp_gptb_get_isr(GPTB3));
#endif
	CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR __attribute__((weak)) void gptb4_int_handler(void) 
{
	// ISR content ...
	CSI_INTRPT_ENTER();
#if (USE_GPTB_CALLBACK == 1)
	csi_gptb_irqhandler(GPTB4, 4);
#else
	csp_gptb_clr_isr(GPTB4, csp_gptb_get_isr(GPTB4));
#endif
	CSI_INTRPT_EXIT();	
}

ATTRIBUTE_ISR __attribute__((weak)) void gptb5_int_handler(void) 
{
	// ISR content ...
	CSI_INTRPT_ENTER();
#if (USE_GPTB_CALLBACK == 1)
	csi_gptb_irqhandler(GPTB5, 5);
#else
	csp_gptb_clr_isr(GPTB5, csp_gptb_get_isr(GPTB5));
#endif
	CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR __attribute__((weak)) void dac0_int_handler(void) 
{
    // ISR content ...
	CSI_INTRPT_ENTER();
#if (USE_DAC_CALLBACK == 1)
	csi_dac_irqhandler(DAC0, 0);
#else
	csp_dac_clr_isr(DAC0, csp_dac_get_isr(DAC0))
#endif
	CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR __attribute__((weak)) void usart0_int_handler(void) 
{
	CSI_INTRPT_ENTER();
#if (USE_USART_CALLBACK == 1)
	csi_usart_irqhandler(USART0, 0);
#elif (USE_LIN_CALLBACK ==1)
	csi_lin_irqhandler(LIN0,0);
#else
	csp_usart_clr_isr(USART0, csp_usart_get_isr(USART0))
#endif
	CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR __attribute__((weak)) void usart1_int_handler(void) 
{
	CSI_INTRPT_ENTER();
#if (USE_USART_CALLBACK == 1)
	csi_usart_irqhandler(USART1, 1);
#elif (USE_LIN_CALLBACK ==1)
	csi_lin_irqhandler(LIN1,1);
#else
	csp_usart_clr_isr(USART1, csp_usart_get_isr(USART1))
#endif
	CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR __attribute__((weak)) void uart0_int_handler(void) 
{
    // ISR content ...
	CSI_INTRPT_ENTER();
#if (USE_UART_CALLBACK == 1)
	csi_uart_irqhandler(UART0, 0);
#else
	csp_uart_clr_isr(UART0, csp_uart_get_isr(UART0))
#endif
	CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR __attribute__((weak)) void uart1_int_handler(void) 
{
    // ISR content ...
	CSI_INTRPT_ENTER();
#if (USE_UART_CALLBACK == 1)
	csi_uart_irqhandler(UART1, 1);
#else
	csp_uart_clr_isr(UART1, csp_uart_get_isr(UART1))
#endif
	CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR __attribute__((weak)) void uart2_int_handler(void) 
{
    // ISR content ...
	CSI_INTRPT_ENTER();
#if (USE_UART_CALLBACK == 1)
	csi_uart_irqhandler(UART2, 2);
#else
	csp_uart_clr_isr(UART2, csp_uart_get_isr(UART2))
#endif
	CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR __attribute__((weak)) void sio0_int_handler(void) 
{
	CSI_INTRPT_ENTER();
#if (USE_SIO_CALLBACK == 1)
	csi_sio_irqhandler(SIO0,0);
#else
	csp_sio_clr_isr(SIO0, csp_sio_get_isr(SIO0));
#endif
	CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR __attribute__((weak)) void sio1_int_handler(void) 
{
	CSI_INTRPT_ENTER();
#if (USE_SIO_CALLBACK == 1)
	csi_sio_irqhandler(SIO1,0);
#else
	csp_sio_clr_isr(SIO1, csp_sio_get_isr(SIO1));
#endif
	CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR __attribute__((weak)) void i2c_int_handler(void) 
{
	CSI_INTRPT_ENTER();
#if (USE_I2C_CALLBACK==1)
	csi_iic_irqhandler(IIC0,0);
#else
	csp_i2c_clr_isr(IIC0, csp_i2c_get_isr(IIC0));
#endif
	CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR __attribute__((weak)) void spi0_int_handler(void) 
{
#if	SPI0_INT_HANDLE_EN
   // ISR content ...
	CSI_INTRPT_ENTER();
	spi_irqhandler(SPI0);//this is a weak function defined in spi_demo.c, for better efficiency, we recommand user directly implement IRQ handler here without any function call.
	CSI_INTRPT_EXIT();
#endif   
}

ATTRIBUTE_ISR __attribute__((weak)) void spi1_int_handler(void) 
{
#if	SPI1_INT_HANDLE_EN
   // ISR content ...
	CSI_INTRPT_ENTER();
	spi_irqhandler(SPI1);//this is a weak function defined in spi_demo.c, for better efficiency, we recommand user directly implement IRQ handler here without any function call.
	CSI_INTRPT_EXIT();
#endif
}

ATTRIBUTE_ISR __attribute__((weak)) void exi0_int_handler(void) 
{
    // ISR content ...
	CSI_INTRPT_ENTER();
	csp_exi_clr_isr(SYSCON, csp_exi_get_isr(SYSCON));				//clear interrput 
	CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR __attribute__((weak)) void exi1_int_handler(void) 
{
    // ISR content ...
	CSI_INTRPT_ENTER();
	csp_exi_clr_isr(SYSCON, csp_exi_get_isr(SYSCON));				//clear interrput 
	CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR __attribute__((weak)) void exi2_3_int_handler(void) 
{
    // ISR content ...
	CSI_INTRPT_ENTER();
	csp_exi_clr_isr(SYSCON, csp_exi_get_isr(SYSCON));				//clear interrput 
	CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR __attribute__((weak)) void exi4_9_int_handler(void) 
{
#if	EXI4_9_INT_HANDLE_EN
    // ISR content ...
	CSI_INTRPT_ENTER();
	gpio_irqhandler(3);  //this is a weak function defined in gpio_demo.c, for better efficiency, we recommand user directly implement IRQ handler here without any function call.
	CSI_INTRPT_EXIT();
#endif
}

ATTRIBUTE_ISR __attribute__((weak)) void exi10_15_int_handler(void) 
{
#if	EXI10_15_INT_HANDLE_EN
    // ISR content ...
	CSI_INTRPT_ENTER();
	gpio_irqhandler(4); //this is a weak function defined in gpio_demo.c, for better efficiency, we recommand user directly implement IRQ handler here without any function call.
	CSI_INTRPT_EXIT();
#endif
}

ATTRIBUTE_ISR void __attribute__((weak)) can_int_handler(void) 
{
#if	CAN_INT_HANDLE_EN
    // ISR content ...
	CSI_INTRPT_ENTER();
	can_irqhandler(CAN0); //this is a weak function defined in can_demo.c, for better efficiency, we recommand user directly implement IRQ handler here without any function call.
	CSI_INTRPT_EXIT();
#endif
}

ATTRIBUTE_ISR __attribute__((weak)) void cnta_int_handler(void)
{
	// ISR content ...
	CSI_INTRPT_ENTER();
#if (USE_CNTA_CALLBACK == 1)
	csi_cnta_irqhandler(CA0, 0);
#else
	csi_pin_toggle(PA10);
#endif
	CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR __attribute__((weak)) void lpt_int_handler(void)
{
	CSI_INTRPT_ENTER();
#if (USE_LPT_CALLBACK==1)
	csi_lpt_irqhandler(LPT,0);
#else
	csp_lpt_clr_isr(LPT,csp_lpt_get_isr(LPT));
#endif
	CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR __attribute__((weak)) void rtc_int_handler(void)
{
	CSI_INTRPT_ENTER();
#if (USE_RTC_CALLBACK == 1)
	
#else
	csp_rtc_clr_isr(RTC, csp_rtc_get_isr(RTC));
#endif
	CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR __attribute__((weak)) void cmp0_int_handler(void) 
{
	// ISR content ...
	CSI_INTRPT_ENTER();
#if (USE_CMP_CALLBACK == 1)
	csi_cmp_irqhandler(CMP0, 0);
#else
	csp_cmp_clr_isr(CMP0);
#endif
	CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR __attribute__((weak)) void cmp1_int_handler(void) 
{
	// ISR content ...
	CSI_INTRPT_ENTER();
#if (USE_CMP_CALLBACK == 1)
	csi_cmp_irqhandler(CMP1, 1);
#else
	csp_cmp_clr_isr(CMP1);
#endif
	CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR __attribute__((weak)) void cmp2_int_handler(void) 
{
	// ISR content ...
	CSI_INTRPT_ENTER();
#if (USE_CMP_CALLBACK == 1)
	csi_cmp_irqhandler(CMP2, 2);
#else
	csp_cmp_clr_isr(CMP2);
#endif
	CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR __attribute__((weak)) void led_int_handler(void) 
{
	// ISR content ...
	CSI_INTRPT_ENTER();
#if (USE_LED_CALLBACK == 1)
	csi_led_irqhandler(LED, 0);
#else
	csp_led_clr_isr(LED, csp_led_get_isr(LED));
#endif
	CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR  __attribute__((weak)) void bt0_int_handler(void) 
{
    // ISR content ...
	CSI_INTRPT_ENTER();
#if (USE_BT_CALLBACK == 1)
	csi_bt_irqhandler(BT0, 0);
#else
	csp_bt_clr_isr(BT0, csp_bt_get_isr(BT0));
#endif
	CSI_INTRPT_EXIT();	
}

ATTRIBUTE_ISR __attribute__((weak)) void bt1_int_handler(void) 
{
    // ISR content ...
	CSI_INTRPT_ENTER();
#if (USE_BT_CALLBACK == 1)
	csi_bt_irqhandler(BT1, 1);
#else
	csp_bt_clr_isr(BT1, csp_bt_get_isr(BT1));
#endif
	CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR __attribute__((weak)) void bt2_int_handler(void) 
{
    // ISR content ...
	CSI_INTRPT_ENTER();
#if (USE_BT_CALLBACK == 1)
	csi_bt_irqhandler(BT1, 2);
#else
	csp_bt_clr_isr(BT0, csp_bt_get_isr(BT0));
#endif
	
	CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR void __attribute__((weak)) bt3_int_handler(void) 
{
	// ISR content ...
	CSI_INTRPT_ENTER();
	csp_bt_clr_isr(BT3, csp_bt_get_isr(BT3));				//use for tick
	CSI_INTRPT_EXIT();	
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

