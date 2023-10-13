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
#ifndef _DEMO_CALLBACK_H
#define _DEMO_CALLBACK_H

/* Includes ------------------------------------------------------------------*/
#include <drv/common.h>

//bt demo
extern int bt_timer_callback_demo(void);
extern int bt_pwm_callback_demo(void);

//iwdt demo
extern int iwdt_normal_demo(void);
extern int iwdt_irq_demo(void);
extern  csi_error_t iwdt_int_callback_demo(void);

//uart demo
extern int uart_send_int_callback_demo(void);
extern int uart_receive_int_callback_demo(void);

//usart demo
extern int usart_send_int_callback_demo(void);
extern int usart_receive_int_callback_demo(void);

extern int lin_send_int_callback_demo(void);
extern int lin_send_recv_int_callback_demo(void);

//adc demo
//normal mode(no interrupt)
extern int adc_continuous_callback_demo(void);

//sio demo
extern int sio_led_rgb_int_callback_demo(void);

//lpt demo
extern int lpt_timer_int_callback_demo(void);

//reliability demo
extern void lvd_demo(void);
extern void lvr_demo(void);
extern void memorycheck_demo(void);
extern void emcm_demo(void);
extern void syscon_cqcr_demo(void);

//wwdt demo
extern  csi_error_t wwdt_int_callback_demo(void);

//ifc demo
extern void ifc_read_demo();
extern void	ifc_dflash_page_program_demo();
extern void	ifc_dflash_page_parallel_program_demo();
extern void ifc_pflash_page_program_demo();
extern void ifc_program_demo();

//rtc_demo
extern void rtc_alarm_callback_demo(void);

//low power demo
extern void lp_lpt_wakeup_deepsleep_demo(void);
extern void lp_exi_wakeup_demo(void);

//iic demo
extern void iic_slave_int_callback_demo(void);

//cnta demo
extern int cnta_timer_int_callback_demo(void);

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
extern int cmp_base_int_callback_demo(void);

//gpta demo
extern int gpta_timer_int_callback_demo(void);
extern int gpta_capture_int_callback_demo(void);
extern int gpta_pwm_int_callback_demo(void);

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
extern csi_error_t led_callback_demo(void);

//opa demo
extern void opa_internal_gain_mode_demo(void);
extern void opa_external_gain_mode_demo(void);

//fvr demo
extern void fvr_output_demo(void);
extern void fvr_buf_demo(void);

//can
extern int can_send_int_callback_demo(void);
extern int can_receive_int_callback_demo(void);

//dac demo
extern int dac_callback_demo(void);


///  sram相关的示例代码
extern void sram_dsram24k_dsram8k_demo0(void);
extern void sram_dsram24k_dsram8k_demo1(void);
extern void sram_dsram24k_iram8k_demo(void);


///  wizard相关的示例代码
extern void lvr_wizard_demo(void);
extern int cqcr_wizard_demo(void);


extern void freertos_demo(void);

#endif