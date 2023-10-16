/***********************************************************************//** 
 * \file  demo.h
 * \brief  demo description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-12-11 <td>V0.0  <td>ZJY   <td>initial
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _DEMO_H
#define _DEMO_H

/* Includes ------------------------------------------------------------------*/
#include "drv/common.h"

//gpio demo
extern int gpio_ouput_demo(void);
extern int gpio_input_demo(void);
extern int gpio_irq_demo(void);

//bt demo
extern int bt_timer_demo(void);
extern int bt_pwm_demo(void);
extern int bt_sync_trg_count_demo(void);
extern int bt_trg_out_demo(void);

//iwdt demo
extern int iwdt_normal_demo(void);
extern int iwdt_irq_demo(void);

//etcb demo
extern int exi_etcb_bt_start_demo(void);
extern int exi_etcb_bt_stop_demo(void);
extern int exi_etcb_adc_samp_demo(void);
extern int exi_etcb_gpta_capture_demo(void);
extern int exi_etcb_gptb_start_demo();
extern int bt_etcb_gptb_start_demo();

//uart demo
extern int uart_send_demo(void);
extern int uart_send_int_demo(void);
extern int uart_receive_int_demo(void);
extern int uart_send_dma_demo(void);
extern int uart_receive_dma_demo(void);

//usart demo
extern int usart_send_demo(void);
extern int usart_send_int_demo(void);
extern int usart_receive_int_demo(void);
extern int usart_send_dma_demo(void);
extern int usart_receive_dma_demo(void);

extern int usart_send_int_callback_demo(void);
extern int usart_receive_int_callback_demo(void);

extern int lin_send_demo(void);
extern int lin_send_recv_demo(void);

//adc demo
//normal mode(no interrupt)
extern int adc_samp_oneshot_demo(void);
extern int adc_samp_continuous_demo(void);
//interrupt mode
extern int adc_samp_oneshot_int_demo(void);
extern int adc_samp_continuous_int_demo(void);

//sio demo
extern int sio_led_rgb_demo(void);
extern int sio_led_rgb_txdne_int_demo(void);
extern int sio_led_rgb_txbufempt_int_demo(void);
extern int sio_led_rgb_send_dma_demo(void);
//sio hdq
extern int sio_hdq_master_wrcmd_demo(void);
extern int sio_hdq_master_rdcmd_demo(void);
extern int sio_hdq_slave_demo(void);

//lpt demo
extern int lpt_timer_demo(void);
extern int lpt_pwm_demo(void);
extern int lpt_sync_trg_start_demo(void);
extern int lpt_trg_out_demo(void);
extern int lpt_soft_trg_out_demo(void);
extern int lpt_filter_demo(void);
extern int lpt_window_demo(void);

//reliability demo
extern void lvd_demo(void);
extern void lvr_demo(void);
extern void memorycheck_demo(void);
extern void emcm_demo(void);
extern void syscon_cqcr_demo(void);

//wwdt demo
extern csi_error_t wwdt_demo(void);

//ifc demo
extern void ifc_read_demo();
extern void	ifc_dflash_page_program_demo();
extern void	ifc_dflash_page_parallel_program_demo();
extern void ifc_pflash_page_program_demo();
extern void ifc_program_demo();

//rtc_demo
extern void rtc_set_time_demo(void);
extern void rtc_alarm_demo(void);
extern void rtc_timer_demo(void);
extern void rtc_etcb_bt_pwm_demo(void);

//low power demo
extern void lp_lpt_wakeup_demo(void);
extern void lp_exi_wakeup_demo(void);
extern void lp_lvd_wakeup_demo(void);
extern void lp_rtc_wakeup_demo(void);
extern void lp_iwdt_wakeup_demo(void);

//iic demo
extern void iic_master_eeprom_demo(void);
extern void iic_master_demo(void);
extern void iic_slave_demo(void);

//cnta demo
extern int cnta_timer_demo(void);
extern int cnta_pwm_demo(void);
extern int cnta_envelope_demo(void);

//spi demo
extern int spi_master_send_demo(void);
extern int spi_master_send_int_demo(void);
extern int spi_master_send_receive_demo(void); 
extern int spi_slave_receive_int_demo(void);
extern int spi_send_dma_demo();
extern int spi_receive_dma_demo();
extern int spi_flash_read_demo();    
extern int spi_flash_write_demo(); 

//cmp demo
extern int cmp_base_demo(void);
extern int cmp_dfcr_demo(void);
extern int cmp_wfcr_demo(void);

//gpta demo
extern int gpta_timer_demo(void);
extern int gpta_capture_demo(void);
extern int gpta_pwm_demo(void);
extern int gpta_pwm_waveform_demo(void);
extern int gpta_reglk_demo(void);

//gptb demo
extern int gptb_timer_demo();
extern int gptb_capture_demo();
extern int gptb_pwm_demo();
extern int gptb_pwm_dz_demo();
extern int gptb_pwm_dz_em_demo();
extern int gptb_reglk_demo();

//crc demo
extern csi_error_t crc32_demo(void);
extern csi_error_t crc16_ccitt_demo(void);
extern csi_error_t crc16_demo(void);
extern csi_error_t crc16_xmodem_demo(void);

//led
extern csi_error_t led_demo(void);

//opa demo
extern void opa_internal_gain_mode_demo(void);
extern void opa_external_gain_mode_demo(void);

//fvr demo
extern void fvr_output_demo(void);
extern void buf_output_demo(void);

//can
extern int can_send_demo(void);
extern int can_send_int_demo(void);
extern int can_receive_int_demo(void);
extern int can_receive_fifo_init_demo(void);
extern int can_remote_frames_send_demo(void);
extern int can_remote_frames_response_demo(void);

//dac demo
extern void dac_demo(void);	
extern void dac_sync_demo(void);	

///  sram相关的示例代码
extern void use_sram1_as_isram_demo(void);


///  wizard相关的示例代码
extern void lvr_wizard_demo(void);
extern int cqcr_wizard_demo(void);


extern void freertos_demo(void);

#endif