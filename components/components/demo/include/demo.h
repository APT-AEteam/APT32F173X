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
#include "common.h"

//gpio port demo
extern int gpio_port_ouput_demo(void);
extern int gpio_port_input_demo(void);
extern int gpio_port_irq_demo(void);

//pin demo
extern int pin_output_demo(void);
extern int pin_input_demo(void);
extern int pin_irq_demo(void);

//bt demo
extern int bt_timer_demo(void);
extern int bt_pwm_demo(void);
extern int bt_sync_trg_start_demo(void);
extern int bt_sync_trg_count_demo(void);
extern int bt_sync_trg_stop_demo(void);
extern int bt_trg_out_demo(void);

//iwdt demo
extern int iwdt_normal_demo(void);
extern int iwdt_irq_demo(void);

//etcb demo

extern int etcb_one_trg_one_demo(void);
extern int etcb_one_trg_more_demo(void);

//uart demo
//uart send
extern int uart_char_demo(void);
extern int uart_send_demo(void);
extern int uart_send_int_demo(void);
//uart receive
extern int uart_receive_demo(void);
extern int uart_recv_int_demo(void);
extern int uart_recv_dynamic_demo(void);
extern int uart_recv_dynamic_int_demo(void);

//adc demo
//normal mode(no interrupt)
extern int adc_samp_oneshot_demo(void);
extern int adc_samp_continuous_demo(void);
//interrupt mode
extern int adc_samp_oneshot_int_demo(void);
extern int adc_samp_continuous_int_demo(void);

//sio demo
//sio led
extern int sio_led_rgb_demo(void);
extern int sio_led_rgb_recv_rxfull_demo(void);
extern int sio_led_rgb_recv_rxdone_demo(void);
//hdq
extern int sio_hdq_send_demo(void);
extern int sio_hdq_recv_wrcmd_demo(void);
extern int sio_hdq_send_recv_demo(void);
extern int sio_hdq_recv_rdcmd_demo(void);

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
extern void rtc_trgev_demo(void);

//low power demo
extern void lp_lpt_wakeup_deepsleep_demo(void);
extern void lp_exi_wakeup_demo(void);

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
extern int spi_slave_send_receive_demo(void);  
extern int spi_slave_receive_int_demo(void);
extern int spi_flash_read_write_demo(void);
extern int spi_etcb_dma_send_demo(void);
extern int spi_etcb_dma_send_receive_demo(void);

//cmp demo
extern int cmp_base_demo(void);
extern int cmp_dfcr_demo(void);
extern int cmp_wfcr_demo(void);

//gpta demo
extern int gpta_timer_demo(void);
extern int gpta_capture_sync_demo0(void);
extern int gpta_capture_sync_demo1(void);
extern int gpta_capture_sync_demo2(void);
extern int gpta_pwm_demo(void);
extern int gpta_pwm_waveform_demo(void);
extern int gpta_reglk_demo(void);

//gptb demo
extern int gptb_timer_demo(void);
extern int gptb_capture_demo(void);
extern int gptb_capture_sync_demo0(void);
extern int gptb_capture_sync_demo1(void);
extern int gptb_capture_sync_demo2(void);
extern int gptb_pwm_demo(void);
extern int gptb_pwm_dz_demo(void);
extern int gptb_pwm_dz_em_demo(void);

//crc demo
extern csi_error_t crc_demo();

//led
extern csi_error_t led_demo(void);

//opa demo
extern void opa_internal_gain_mode_demo(void);
extern void opa_external_gain_mode_demo(void);

//fvr demo
extern void fvr_output_demo(void);
extern void fvr_buf_demo(void);

//can
extern int can_send_demo(void);
extern int can_recv_demo(void);

//dac demo
extern void dac_demo(void);	


///  sram相关的示例代码
extern void sram_dsram24k_dsram8k_demo0(void);
extern void sram_dsram24k_dsram8k_demo1(void);
extern void sram_dsram24k_iram8k_demo(void);


///  wizard相关的示例代码
extern void lvr_wizard_demo(void);
extern int cqcr_wizard_demo(void);


extern void freertos_demo(void);

#endif