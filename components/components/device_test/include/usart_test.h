/***********************************************************************//** 
 * \file  usart test.h
 * \brief  usart test description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-4-10 <td>V0.0  <td>CWL   <td>initial
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _USART_TEST_H
#define _USART_TEST_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"


extern int  usart_char_demo_test(void);
extern int  usart_send_demo_test(void);
extern int  usart_recv_demo_test(void);
extern int  usart_recv_dma_demo_test(void);
extern int  usart_send_dma_demo_test(void);
extern int  usart_send_int_demo_test(void);
extern int  usart_recv_rx_int_demo_test(void);
extern int  usart_recv_rxfifo_int_demo_test(void);

#endif