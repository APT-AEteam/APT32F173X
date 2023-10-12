/***********************************************************************//** 
 * \file  interrupt.c
 * \brief  source file for interrypt handlers
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-10 <td>V0.0  <td>WNN    <td>initial
 * <tr><td> 2020-10-11 <td>V0.1 <td>ZJY    <td>code normalization 
 * </table>
 * *********************************************************************
*/

/* Includes ---------------------------------------------------------------*/

#include "csi_drv.h"
#include "board_config.h"

			
/* externs function--------------------------------------------------------*/
extern void nmi_int_handler(void);
extern void ifc_irqhandler(void);


/** \brief nmi interrupt service functions
 * 
 *  \param[in] none
 *  \return none
 */ 
ATTRIBUTE_ISR void nmi_int_handler(void)
{
	// ISR content ...
	CSI_INTRPT_ENTER();
	
	if(csp_syscon_get_isr(SYSCON) & LVD_INT)
	{
		csp_syscon_clr_isr(SYSCON, LVD_INT);
	}
	if(csp_exi_get_isr(SYSCON) & EXI_STATUS_GRP0)
	{
		nop;
	}
	CSI_INTRPT_EXIT();
}
/** \brief coret interrupt service functions
 * 
 *  \param[in] none
 *  \return none
 */ 
ATTRIBUTE_ISR void coret_int_handler(void)
{
	// ISR content ...
	CSI_INTRPT_ENTER();
	
	CSI_INTRPT_EXIT();
}

/** \brief syscon interrupt service functions
 * 
 *  \param[in] none
 *  \return none
 */ 
ATTRIBUTE_ISR void syscon_int_handler(void) 
{
    // ISR content ...
	CSI_INTRPT_ENTER();
	
	if(csp_syscon_get_isr(SYSCON) & LVD_INT) //LVD INT
	{
		csp_syscon_clr_isr(SYSCON, LVD_INT);
	}
	
	if(csp_syscon_get_isr(SYSCON) & IWDT_INT) //IWDT INT
	{
#if (USE_IWDT_CALLBACK == 1)
	
		csi_iwdt_irqhandler(SYSCON,0);
#else
		csp_syscon_clr_isr(SYSCON, IWDT_INT);
		//csi_iwdt_feed();
#endif
		
	}
	if(csp_syscon_get_isr(SYSCON) & EMFAIL_INT) //EMOSC FAIL INT
	{
		csp_syscon_clr_isr(SYSCON, EMFAIL_INT);
	}
	if(csp_syscon_get_isr(SYSCON) & ESFAIL_INT) //ESOSC FAIL INT
	{
		csp_syscon_clr_isr(SYSCON, ESFAIL_INT);
	}
	
	CSI_INTRPT_EXIT();
}

/** \brief ifc interrupt service functions
 * 
 *  \param[in] none
 *  \return none
 */ 
ATTRIBUTE_ISR void ifc_int_handler(void)
{
	CSI_INTRPT_ENTER();
	ifc_irqhandler();	//only DFLASH paramode write would use IFC interrupt handler. DO NOT rewrite this function
	CSI_INTRPT_EXIT();
}

/** \brief adc interrupt service functions
 * 
 *  \param[in] none
 *  \return none
 */ 
ATTRIBUTE_ISR __attribute__((weak)) void adc0_int_handler(void) 
{	
    //ISR content ...
	CSI_INTRPT_ENTER();
#if (USE_ADC_CALLBACK == 1)
	csi_adc_irqhandler(ADC0, 0);
#else
	csp_adc_clr_sr(ADC0, csp_adc_get_isr(ADC0));
#endif
	CSI_INTRPT_EXIT();	
}

ATTRIBUTE_ISR __attribute__((weak)) void adc1_int_handler(void) 
{
    //ISR content ...
	CSI_INTRPT_ENTER();
#if	(IS_CHIP_1732==1)
#if (USE_ADC_CALLBACK == 1)
	csi_adc_irqhandler(ADC1, 1);
#else
	csp_adc_clr_sr(ADC1, csp_adc_get_isr(ADC1));
#endif
#endif
	CSI_INTRPT_EXIT();	
}

/** \brief dma interrupt service functions
 * 
 *  \param[in] none
 *  \return none
 */ 
ATTRIBUTE_ISR __attribute__((weak)) void dma0_int_handler(void)
{
    // ISR content ...
	CSI_INTRPT_ENTER();
	csi_dma_irqhandler(DMA0);
	CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR __attribute__((weak)) void dma1_int_handler(void) 
{
    // ISR content ...
	CSI_INTRPT_ENTER();
	csi_dma_irqhandler(DMA1);
	CSI_INTRPT_EXIT();
}

/** \brief wwdt interrupt service functions
 * 
 *  \param[in] none
 *  \return none
 */ 
ATTRIBUTE_ISR __attribute__((weak)) void wwdt_int_handler(void)
{
	CSI_INTRPT_ENTER();
#if (USE_WWDT_CALLBACK==1)
	csi_wwdt_irqhandler(WWDT,0);
#else
	csp_wwdt_clr_isr(WWDT);
#endif
	CSI_INTRPT_EXIT();
}

/** \brief gpta(0~3) interrupt service functions
 * 
 *  \param[in] none
 *  \return none
 */ 
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

/** \brief gptb(0~5) interrupt service functions
 * 
 *  \param[in] none
 *  \return none
 */ 
ATTRIBUTE_ISR __attribute__((weak)) void gptb0_int_handler(void) 
{
	// ISR content ...
	CSI_INTRPT_ENTER();
#if (USE_GPTB_CALLBACK == 1)
	csi_gptb_irqhandler(GPTB0, 0);
#else
	csp_gptb_clr_isr(GPTB0, csp_gptb_get_isr(GPTB0));
	csp_gptb_clr_emisr(GPTB0, csp_gptb_get_emisr(GPTB0));
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
	csp_gptb_clr_emisr(GPTB1, csp_gptb_get_emisr(GPTB1));
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
	csp_gptb_clr_emisr(GPTB2, csp_gptb_get_emisr(GPTB2));
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
	csp_gptb_clr_emisr(GPTB3, csp_gptb_get_emisr(GPTB3));
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
	csp_gptb_clr_emisr(GPTB4, csp_gptb_get_emisr(GPTB4));
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
	csp_gptb_clr_emisr(GPTB5, csp_gptb_get_emisr(GPTB5));
#endif
	CSI_INTRPT_EXIT();
}

/** \brief dac interrupt service functions
 * 
 *  \param[in] none
 *  \return none
 */ 
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

/** \brief usart(0~1) interrupt service functions
 * 
 *  \param[in] none
 *  \return none
 */ 
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

/** \brief uart(0~2) interrupt service functions
 * 
 *  \param[in] none
 *  \return none
 */ 
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

/** \brief sio(0~1) interrupt service functions
 * 
 *  \param[in] none
 *  \return none
 */ 
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

/** \brief iic interrupt service functions
 * 
 *  \param[in] none
 *  \return none
 */ 
ATTRIBUTE_ISR __attribute__((weak)) void iic_int_handler(void) 
{
	CSI_INTRPT_ENTER();
#if (USE_IIC_CALLBACK==1)
	csi_iic_irqhandler(IIC0,0);
#else
	csp_iic_clr_isr(IIC0, csp_iic_get_isr(IIC0));
#endif
	CSI_INTRPT_EXIT();
}

/** \brief spi(0~1) interrupt service functions
 * 
 *  \param[in] none
 *  \return none
 */ 
ATTRIBUTE_ISR __attribute__((weak)) void spi0_int_handler(void) 
{
	// ISR content ...
	CSI_INTRPT_ENTER();
#if (USE_SPI_CALLBACK == 1)
	csi_spi_irqhandler(SPI0, 0);
#else
	csp_spi_clr_isr(SPI0, csp_spi_get_isr(SPI0));
#endif
	CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR __attribute__((weak)) void spi1_int_handler(void) 
{
	// ISR content ...
	CSI_INTRPT_ENTER();
#if (USE_SPI_CALLBACK == 1)
	csi_spi_irqhandler(SPI1, 0);
#else
	csp_spi_clr_isr(SPI1, csp_spi_get_isr(SPI1));
#endif
	CSI_INTRPT_EXIT();
}

/** \brief exi(0~15) interrupt service functions
 * 
 *  \param[in] none
 *  \return none
 */ 
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
  // ISR content ...
	CSI_INTRPT_ENTER();
	csp_exi_clr_isr(SYSCON, csp_exi_get_isr(SYSCON));				//clear interrput 
	CSI_INTRPT_EXIT();
}

ATTRIBUTE_ISR __attribute__((weak)) void exi10_15_int_handler(void) 
{
  // ISR content ...
	CSI_INTRPT_ENTER();
	csp_exi_clr_isr(SYSCON, csp_exi_get_isr(SYSCON));				//clear interrput 
	CSI_INTRPT_EXIT();
}

/** \brief can interrupt service functions
 * 
 *  \param[in] none
 *  \return none
 */ 
ATTRIBUTE_ISR void __attribute__((weak)) can_int_handler(void) 
{
    // ISR content ...
	CSI_INTRPT_ENTER();
#if (USE_CAN_CALLBACK == 1)
	csi_can_irqhandler(CAN0, 0);
#else
	csp_can_clr_isr(CAN0, csp_can_get_isr(CAN0));
#endif
	CSI_INTRPT_EXIT();
}

/** \brief cnta interrupt service functions
 * 
 *  \param[in] none
 *  \return none
 */ 
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

/** \brief lpt interrupt service functions
 * 
 *  \param[in] none
 *  \return none
 */ 
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

/** \brief rtc interrupt service functions
 * 
 *  \param[in] none
 *  \return none
 */ 
ATTRIBUTE_ISR __attribute__((weak)) void rtc_int_handler(void)
{
	CSI_INTRPT_ENTER();
#if (USE_RTC_CALLBACK == 1)
	csi_rtc_irqhandler(RTC,0);
#else
	csp_rtc_clr_isr(RTC, csp_rtc_get_isr(RTC));
#endif
	CSI_INTRPT_EXIT();
}

/** \brief cmp(0~2) interrupt service functions
 * 
 *  \param[in] none
 *  \return none
 */ 
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


/** \brief led interrupt service functions
 * 
 *  \param[in] none
 *  \return none
 */ 
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

/** \brief bt(0~3) interrupt service functions
 * 
 *  \param[in] none
 *  \return none
 */ 
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
/********* Exception interrupt service function **************/
/*************************************************************/
void sw_int_handler(void)
{
	while(1);
}

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

/******************* (C) COPYRIGHT 2023 APT Chip *****END OF FILE****/

