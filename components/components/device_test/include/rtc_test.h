/***********************************************************************//** 
 * \file  rtc test.h
 * \brief  rtc test description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-4-10 <td>V0.0  <td>CWL   <td>initial
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _RTC_TEST_H
#define _RTC_TEST_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"


extern void rtc_set_time_demo_test(void);
extern void rtc_alarm_demo_test(void);
extern void rtc_timer_demo_test(void);
extern void rtc_trgev_demo_test(void);

#endif	