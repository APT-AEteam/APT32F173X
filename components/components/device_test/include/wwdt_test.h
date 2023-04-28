/***********************************************************************//** 
 * \file  wwdt test.h
 * \brief  wwdt test description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-4-19 <td>V0.0  <td>YT   <td>initial
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _WWDT_TEST_H
#define _WWDT_TEST_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"


csi_error_t wwdt_test(void);
csi_error_t wwdt_irq_test(void);
__attribute__((weak)) void wwdt_irqhandler(void);
#endif