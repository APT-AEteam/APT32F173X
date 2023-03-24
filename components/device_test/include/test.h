/***********************************************************************//** 
 * \file  test.h
 * \brief  test description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-3-24 <td>V0.0  <td>YYM   <td>initial
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _TEST_H
#define _TEST_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"

//gpta test

extern int gpta_timer_test(void);
extern int gpta_capture_sync_test(void);
extern int gpta_capture_sync_test1(void);
extern int gpta_pwm_test(void);
extern int gpta_pwm_waveform_test(void);
extern int gpta_reglk_test(void);

#endif