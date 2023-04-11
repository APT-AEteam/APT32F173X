/***********************************************************************//** 
 * \file  spi test.h
 * \brief  spi test description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-4-10 <td>V0.0  <td>WCH   <td>initial
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _SPI_H
#define _SPI_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"

extern int spi_master_send_test(void);
extern int spi_master_send_int_test(void);
extern int spi_master_send_receive_test(void);
extern int spi_slave_send_receive_test(void); 
extern int spi_flash_read_write_test(void);
extern int spi_dma_send_test(void);
extern int spi_dma_send_receive_test(void);   

#endif