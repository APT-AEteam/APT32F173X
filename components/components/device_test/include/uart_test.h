/***********************************************************************//** 
 * \file  uart test.h
 * \brief  uart test description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-4-10 <td>V0.0  <td>CWL   <td>initial
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _UART_TEST_H
#define _UART_TEST_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"


extern void  uart_char_demo_test(void);
extern void  uart_receive_demo_test(void);
extern void  uart_recv_dma_demo_test(void);
extern void  uart_recv_dynamic_demo_test(void);
extern void  uart_recv_dynamic_int_demo_test(void);
extern void  uart_recv_int_demo_test(void);
extern void  uart_send_demo_test(void);
extern void  uart_send_dma_demo_test(void);
extern void  uart_send_int_demo_test_test(void);
extern void usart_test_irqhandler(csp_usart_t *ptUsartBase,uint8_t byIdx);

#endif