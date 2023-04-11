/***********************************************************************//** 
 * \file  gptb test.h
 * \brief  gptb test description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-4-10 <td>V0.0  <td>WCH   <td>initial
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _GPTB_TEST_H
#define _GPTB_TEST_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"

extern int gptb_capture_test(void);
extern int gptb_pwm_test(void);
extern int gptb_pwm_dz_test(void);
extern int gptb_pwm_dz_em_test(void);

#endif