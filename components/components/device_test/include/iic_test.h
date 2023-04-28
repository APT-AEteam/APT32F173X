
/***********************************************************************//** 
 * \file  iic test.h
 * \brief  iic test description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-4-19 <td>V0.0  <td>YT   <td>initial
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _IIC_TEST_H
#define _IIC_TEST_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"



void iic_master_iomap_demo(void);
void iic_master_test(void);
void iic_slave_test(void);
void iic_multi_slave_address_test(void);
void iic_dma_tx_test(void);
void iic_dma_rx_test(void);

__attribute__((weak)) void i2c_irqhandler(csp_i2c_t *ptIicBase);

#endif