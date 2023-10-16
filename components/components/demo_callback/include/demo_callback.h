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

extern int lin_send_recv_int_callback_demo(void);

//adc demo
//normal mode(no interrupt)
extern int adc_continuous_callback_demo(void);

//sio demo
extern int sio_led_rgb_int_callback_demo(void);

//lpt demo
extern int lpt_timer_int_callback_demo(void);

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


//iic demo
extern void iic_slave_int_callback_demo(void);

//cnta demo
extern int cnta_timer_int_callback_demo(void);

//spi demo
extern int spi_master_send_int_callback_demo();
extern int spi_slave_receive_int_callback_demo();

//cmp demo
extern int cmp_base_int_callback_demo(void);

//gpta demo
extern int gpta_timer_int_callback_demo(void);
extern int gpta_capture_int_callback_demo(void);
extern int gpta_pwm_int_callback_demo(void);

//gptb demo
extern int gptb_capture_int_callback_demo();
extern int gptb_pwm_int_callback_demo();
extern int gptb_pwm_dz_em_int_callback_demo();


//led
extern int led_callback_demo(void);

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


extern void freertos_demo(void);

#endif