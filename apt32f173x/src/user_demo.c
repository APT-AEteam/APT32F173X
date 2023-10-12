
/*============================================================================
 * Name        : main.c
 * Author      : $(username)
 * Version     : 0.0.0
 * Copyright   : Your copyright notice
 * Description : Simple function in C, Ansi-style
 *============================================================================
 */
/* include ----------------------------------------------------------------*/
#include "demo.h"
#include "pin.h"
#include "demo_callback.h"

void user_demo(void)
{

/// ***************************************************
///  GPIO相关示例代码
///  gpio_xxx适用于多个pin同时配置和操作的情况
///  pin_xxx适用于单独对一个pin进行配置和操作的情况
/// ***************************************************	
//	gpio_port_ouput_demo();
//	gpio_port_input_demo();
//	gpio_port_irq_demo();
//	pin_output_demo();
//	pin_input_demo();
//	pin_irq_demo();
//  pin_ioremap_demo();
	
/// ***************************************************
///  通过PA02输出系统时钟，主要用于调试。
///  同时示例管脚功能配置的方法。
/// ***************************************************	
//	csi_gpio_set_mux(GPIOA, PA2, PA2_CLO);
//	csi_clo_config(CLO_HCLK, CLO_DIV16);
	
/// ***************************************************
///  系统可靠性相关的示例代码
/// ***************************************************
//	lvd_demo();
//	lvr_demo();
//	memorycheck_demo();
//	emcm_demo();
//  syscon_cqcr_demo();

/// ***************************************************
/// IWDT相关的示例代码
/// ***************************************************
//	iwdt_normal_demo();
//	iwdt_irq_demo();

//	iwdt_int_callback_demo();

/// ***************************************************
/// WWDT相关的示例代码
/// ***************************************************
//	wwdt_demo();
	
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
//	rtc_set_time_demo();
//	rtc_alarm_demo();
//	rtc_timer_demo();
//	rtc_etcb_bt_pwm_demo();

//	rtc_alarm_callback_demo();
/// ***************************************************
///  低功耗相关的示例代码
/// ***************************************************
//	lp_exi_wakeup_demo();
//	lp_lpt_wakeup_demo();
//	lp_lvd_wakeup_demo();
//	lp_rtc_wakeup_demo();
//	lp_iwdt_wakeup_demo();
	
/// ***************************************************
///  BT相关的示例代码
/// ***************************************************
//	bt_timer_demo();
//	bt_pwm_demo();
//	bt_sync_trg_start_demo();
//	bt_sync_trg_count_demo();
//	bt_sync_trg_stop_demo();
//	bt_trg_out_demo();

/// ***************************************************
///  LPT相关的示例代码
/// ***************************************************
//	lpt_timer_demo();
//	lpt_pwm_demo();
//	lpt_sync_trg_start_demo();
//	lpt_trg_out_demo();
//  lpt_filter_demo();
//  lpt_window_demo();

/// ***************************************************
///  Counter A相关的示例代码
/// ***************************************************
//	cnta_timer_demo();
//	cnta_pwm_demo();
//	cnta_envelope_demo();
	
/// ***************************************************
///  GPTA相关的示例代码
/// ***************************************************
//	gpta_timer_demo();
//	gpta_capture_demo();
//	gpta_pwm_demo();
//	gpta_pwm_waveform_demo();
//	gpta_reglk_demo();

/// ***************************************************
///  GPTB相关的示例代码
/// ***************************************************
//	gptb_capture_demo();
//	gptb_capture_sync_demo0();
//	gptb_capture_sync_demo1();
//	gptb_capture_sync_demo2();
//	gptb_pwm_demo();
//	gptb_pwm_dz_demo();
//	gptb_pwm_dz_em_demo();

/// ***************************************************
///  CMP相关的示例代码
/// ***************************************************
//	cmp_base_demo();
//	cmp_dfcr_demo();
//	cmp_wfcr_demo();

/// ***************************************************
///  ADC相关的示例代码
/// ***************************************************
//	adc_samp_oneshot_demo();
//	adc_samp_continuous_demo();
//	adc_samp_oneshot_int_demo();
//	adc_samp_continuous_int_demo();
	
/// ***************************************************
///  CRC相关的示例代码
/// ***************************************************
//	crc_demo();
	
/// ***************************************************
///  ETCB相关的示例代码
/// ***************************************************
//  etcb_one_trg_one_demo();
//	etcb_one_trg_more_demo();

/// ***************************************************
///  UART相关的示例代码
/// ***************************************************
//	uart_char_demo();
//	uart_send_demo();
//	uart_send_int_demo();
//	uart_receive_demo();
//	uart_recv_int_demo();
//	uart_recv_dynamic_demo();
//	uart_recv_dynamic_int_demo();
//	uart_send_dma_demo();
//	uart_recv_dma_demo();

/// ***************************************************
///  USART相关的示例代码
/// ***************************************************

//	usart_send_demo();
//	usart_send_int_demo();
//	usart_recv_int_demo();
//	usart_send_dma_demo();  
//	usart_recv_dma_demo();
//

//	usart_send_int_callback_demo();	
//	usart_receive_int_callback_demo();
	
//	lin_send_demo();        
//	lin_send_recv_demo();

//	lin_send_int_callback_demo();
//	lin_send_recv_int_callback_demo();

/// ***************************************************
///  IIC相关的示例代码
/// ***************************************************
//	iic_master_demo();
//	iic_master_eeprom_demo( );
//	iic_slave_demo();
//  iic_multi_slave_address_demo( );
//  iic_dma_tx_demo();
//  iic_dma_rx_demo();
	
/// ***************************************************
///  SIO相关的示例代码
/// ***************************************************
//	sio_led_rgb_demo();
//	sio_led_rgb_recv_rxfull_demo();
//	sio_led_rgb_recv_rxdone_demo();
//	sio_hdq_send_demo();
//	sio_hdq_recv_wrcmd_demo();
//	sio_hdq_send_recv_demo();
//	sio_hdq_recv_rdcmd_demo();
	
/// ***************************************************
///  SPI相关的示例代码
/// ***************************************************
//  spi_master_send_demo();
//  spi_master_send_int_demo();
//  spi_master_send_receive_demo(); 
//  spi_slave_send_receive_demo();  
//  spi_slave_receive_int_demo(); 
//  spi_etcb_dma_send_demo();
//  spi_etcb_dma_send_receive_demo();
//  spi_flash_read_write_demo();          
	
/// ***************************************************
///  OPA相关的示例代码
/// ***************************************************
//	opa_internal_gain_mode_demo();
//	opa_external_gain_mode_demo();

/// ***************************************************
///  FVR相关的示例代码
/// ***************************************************
//	fvr_output_demo();
//	fvr_buf_demo();

/// ***************************************************
///  DAC相关的示例代码
/// ***************************************************
//	dac_demo();

///  LED相关的示例代码
/// ***************************************************
//	led_demo();

/// ***************************************************
///  sram相关的示例代码
/// ***************************************************
//	use_sram1_as_isram_demo();
//	sram_dsram24k_dsram8k_demo1();
//	sram_dsram24k_iram8k_demo();

/// ***************************************************
///  wizard相关的示例代码
/// ***************************************************
//  lvr_wizard_demo();
//  cqcr_wizard_demo();

/// ***************************************************
///  freertos相关的示例代码，
///  使用前，请将borad_config.h里面的CONFIG_KERNEL_FREERTOS宏设置为1
/// ***************************************************
//	freertos_demo();
}

