
/*============================================================================
 * Name        : main.c
 * Author      : $(username)
 * Version     : 0.0.0
 * Copyright   : Your copyright notice
 * Description : Simple function in C, Ansi-style
 *============================================================================
 */
/* include ----------------------------------------------------------------*/
#include "csi_drv.h"
#include "board_config.h"
//
#include "demo.h"
#include "demo_callback.h"

void user_demo(void)
{

/// ***************************************************
///	GPIO相关示例代码
///	GPIO是比较特殊csi标准库驱动不支持中断callback	 
/// ***************************************************	
//	gpio_ouput_demo();
//	gpio_input_demo();
//	gpio_irq_demo();
	
/// ***************************************************
/// BT相关的示例代码
///	BT的csi标准库驱动支持中断callback
/// ***************************************************
#if (USE_BT_CALLBACK == 0)				//不使用callback
//	bt_timer_demo();
//	bt_pwm_demo();
//	bt_sync_trg_count_demo();
//	bt_trg_out_demo();
#else									//使用callback
//	bt_timer_callback_demo();
//	bt_pwm_callback_demo();
#endif

/// ***************************************************
///  SYSCON相关的示例代码
/// ***************************************************
//	lvd_demo();
//	lvr_demo();
//	memorycheck_demo();
//	emcm_demo();
//  syscon_cqcr_demo();

/// ***************************************************
///  NMI相关的示例代码
/// ***************************************************
//	lvd_trigger_nmi_demo();

/// ***************************************************
/// IWDT相关的示例代码
/// ***************************************************
#if (USE_IWDT_CALLBACK == 0)				//不使用callback
//	iwdt_normal_demo();
//	iwdt_irq_demo();
#else
//	iwdt_int_callback_demo();
#endif
/// ***************************************************
/// WWDT相关的示例代码
/// ***************************************************
//	wwdt_demo();
#if (USE_WWDT_CALLBACK == 0)				//不使用callback
//	wwdt_demo();
#else
//	 wwdt_int_callback_demo();
#endif
/// ***************************************************
///  flash相关的示例代码
/// ***************************************************	
//	ifc_read_demo();
//	ifc_dflash_page_program_demo();
//	ifc_dflash_page_parallel_program_demo();
//	ifc_pflash_page_program_demo();
//	ifc_program_demo();
/// ***************************************************
///  RTC相关的示例代码
/// ***************************************************
#if (USE_RTC_CALLBACK == 0)				//不使用callback
//	rtc_set_time_demo();
//	rtc_alarm_demo();
//	rtc_timer_demo();
//	rtc_etcb_bt_pwm_demo();
#else
//	rtc_alarm_callback_demo();
#endif
/// ***************************************************
///  低功耗相关的示例代码
/// ***************************************************
//	lp_exi_wakeup_demo();
//	lp_lpt_wakeup_demo();
//	lp_lvd_wakeup_demo();
//	lp_rtc_wakeup_demo();
//	lp_iwdt_wakeup_demo();
	
/// ***************************************************
///  LPT相关的示例代码
/// ***************************************************
#if (USE_LPT_CALLBACK == 0)			//不使用callback
//	lpt_timer_demo( );
//	lpt_pwm_demo( );
//	exi_etcb_lpt_start_demo( );
//	lpt_etcb_bt_start_demo( );
//  lpt_filter_demo( );
#else									//使用callback
//	lpt_timer_int_callback_demo( );
#endif

/// ***************************************************
///  Counter A相关的示例代码
/// ***************************************************
#if (USE_CNTA_CALLBACK == 0)			//不使用callback
//	cnta_timer_demo();
//	cnta_pwm_demo();
//	cnta_envelope_demo();
#else									//使用callback
//	cnta_timer_int_callback_demo();
#endif
	
/// ***************************************************
///  GPTA相关的示例代码
/// ***************************************************
#if (USE_GPTA_CALLBACK == 0)			//不使用callback
//	gpta_timer_demo();
//	gpta_capture_demo();
//	gpta_pwm_demo();
//	gpta_pwm_waveform_demo();
//	gpta_reglk_demo();
#else									//使用callback
//	gpta_timer_int_callback_demo();
//	gpta_capture_int_callback_demo();
//	gpta_pwm_int_callback_demo();
#endif

/// ***************************************************
///  GPTB相关的示例代码
/// ***************************************************
#if (USE_GPTB_CALLBACK == 0)			//不使用callback
//	gptb_timer_demo();
//	gptb_capture_demo();
//	gptb_pwm_demo();
//	gptb_pwm_dz_demo();
//	gptb_pwm_dz_em_demo();
//	gptb_reglk_demo();
#else									//使用callback
//	gptb_capture_int_callback_demo();
//	gptb_pwm_int_callback_demo();
//	gptb_pwm_dz_em_int_callback_demo();
#endif

/// ***************************************************
///  CMP相关的示例代码
/// ***************************************************
#if (USE_CMP_CALLBACK == 0)				//不使用callback
//	cmp_base_demo();
//	cmp_dfcr_demo();
//	cmp_wfcr_demo();
#else									//使用callback
//	cmp_base_int_callback_demo();
#endif

/// ***************************************************
///  ADC相关的示例代码
/// ***************************************************
#if (USE_ADC_CALLBACK == 0)				//不使用callback
//	adc_samp_oneshot_demo();
//	adc_samp_continuous_demo();
//	adc_samp_oneshot_int_demo();
//	adc_samp_continuous_int_demo();
#else									//使用callback
//	adc_continuous_callback_demo();
#endif

/// ***************************************************
///  CRC相关的示例代码
/// ***************************************************
//	crc32_demo();
//	crc16_ccitt_demo();
//	crc16_demo();
//	crc16_xmodem_demo();
	
/// ***************************************************
/// ETCB相关的示例代码
/// ETCB的demo是多个IP联动，相对较复杂
/// ***************************************************
	//EXI trigger BT 
//	exi_etcb_bt_start_demo();
//	exi_etcb_bt_stop_demo();
	//EXI trigger ADC
//	exi_etcb_adc_samp_demo();
	//EXI trigger gpta_capture
//	exi_etcb_gpta_capture_demo();
	//EXI/BT trigger GPTB start
//	exi_etcb_gptb_start_demo();
//	bt_etcb_gptb_start_demo();

/// ***************************************************
/// UART相关的示例代码
///	UART的csi标准库驱动支持中断callback
/// ***************************************************
#if (USE_UART_CALLBACK == 0)			//不使用callback
//	uart_send_demo();
//	uart_send_int_demo();
//	uart_receive_int_demo();
//	uart_send_dma_demo();
//	uart_receive_dma_demo();
#else									//使用callback
//	uart_send_int_callback_demo();
//	uart_receive_int_callback_demo();
#endif

/// ***************************************************
///  USART相关的示例代码
/// ***************************************************
#if (USE_USART_CALLBACK == 0)				//不使用callback
//	usart_send_demo();
//	usart_send_int_demo();
//	usart_recv_int_demo();
//	usart_send_dma_demo();  
//	usart_recv_dma_demo();
#elif(USE_USART_CALLBACK == 1)
//	usart_send_int_callback_demo();	
//	usart_receive_int_callback_demo();
#elif(USE_LIN_CALLBACK == 0)	
//	lin_send_demo();        
//	lin_send_recv_demo();
#elif(USE_LIN_CALLBACK == 1)
//	lin_send_recv_int_callback_demo();
#endif
/// ***************************************************
///  IIC相关的示例代码
/// ***************************************************
#if (USE_IIC_CALLBACK == 0)				//不使用callback
//	iic_master_eeprom_demo();
//	iic_master_demo();
//	iic_slave_demo();
//	iic_multi_slave_demo();
//	iic_dma_send_demo();
//	iic_dma_receive_demo();
#else									//使用callback
//	iic_slave_int_callback_demo();
//  iic_slave_state_callback_demo();
#endif

	
/// ***************************************************
/// SIO相关的示例代码
///	SIO的csi标准库驱动支持中断callback
/// ***************************************************
#if (USE_SIO_CALLBACK == 0)				//不使用callback
//	sio_led_rgb_demo();
//	sio_led_rgb_txbufempt_int_demo();
//	sio_led_rgb_txdne_int_demo();
//	sio_led_rgb_send_dma_demo();
//	sio_hdq_master_wrcmd_demo();
//	sio_hdq_master_rdcmd_demo();
//	sio_hdq_slave_demo();
#else									//使用callback
//	sio_led_rgb_int_callback_demo();
#endif

/// ***************************************************
/// CAN相关的示例代码
///	CAN的csi标准库驱动支持中断callback
/// ***************************************************
#if (USE_CAN_CALLBACK == 0)				//不使用callback
//	can_send_demo();
//	can_send_int_demo();
//	can_receive_int_demo();
//	can_receive_fifo_init_demo();
//	can_remote_frames_send_demo();
//	can_remote_frames_response_demo();
#else									//使用callback
//	can_send_int_callback_demo();
//	can_receive_int_callback_demo();
#endif

/// ***************************************************
///  SPI相关的示例代码
/// ***************************************************
#if (USE_SPI_CALLBACK == 0)			//不使用callback  
//	spi_master_send_demo();
//	spi_master_send_int_demo();
//	spi_master_send_receive_demo(); 
//	spi_slave_receive_int_demo(); 
//	spi_send_dma_demo();
//	spi_receive_dma_demo();
//	spi_flash_read_demo();    
//	spi_flash_write_demo(); 
#else									//使用callback
//	spi_master_send_int_callback_demo();
//	spi_slave_receive_int_callback_demo();
#endif
	
/// ***************************************************
///  OPA相关的示例代码
/// ***************************************************
//	opa_internal_gain_mode_demo();
//	opa_external_gain_mode_demo();

/// ***************************************************
///  FVR相关的示例代码
/// ***************************************************
//	fvr_output_demo();
//	buf_output_demo();

/// ***************************************************
///  DAC相关的示例代码
/// ***************************************************
#if (USE_DAC_CALLBACK == 0)				//不使用callback
//	dac_demo();
//	dac_sync_demo();
#else									//使用callback
//	dac_callback_demo();
#endif

///  LED相关的示例代码
/// ***************************************************
#if (USE_LED_CALLBACK == 0)			//不使用callback  
//	led_demo();
#else
//	led_callback_demo();			//使用callback
#endif

/// ***************************************************
///  sram相关的示例代码
/// ***************************************************
//	use_sram1_as_isram_demo();

/// ***************************************************
///  wizard相关的示例代码
/// ***************************************************
//  lvr_wizard_demo();
//  cqcr_wizard_demo();

/// ***************************************************
///  freertos相关的示例代码，
///  使用前，请将borad_config.h里面的USE_KERNEL_FREERTOS宏设置为1
/// ***************************************************
//	freertos_demo();
}

