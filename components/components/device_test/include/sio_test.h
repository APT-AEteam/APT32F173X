/***********************************************************************//** 
 * \file  sio_test.h
 * \brief  test description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-04-19 <td>V0.0  <td>ZJY   <td>initial
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _SIO_TEST_H
#define _SIO_TEST_H

/* Includes ------------------------------------------------------------------*/
#include <drv/sio.h>
#include <drv/gpio.h>

//sio
int sio_send_test(csp_sio_t *ptSioBase, pin_name_e ePinName);
int sio_send_int_test(csp_sio_t *ptSioBase, pin_name_e ePinName, csi_sio_intsrc_e eSioSendInt);
int sio_recv_test(csp_sio_t *ptSioBase, pin_name_e ePinName, csi_sio_intsrc_e eSioRecvInt);
int sio_send_dma_test(csp_sio_t *ptSioBase);
int sio_recv_dma_test(csp_sio_t *ptSioBase);

#endif