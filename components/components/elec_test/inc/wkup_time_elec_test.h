/***********************************************************************//** 
 * \file  lvd_r_elec_test.h
 * \brief  test description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-02-21 <td>V0.0  <td>ZJY   <td>initial
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _WKUP_TIME_ELEC_TEST_H
#define _WKUP_TIME_ELEC_TEST_H

/* Includes ------------------------------------------------------------------*/

void sleep_wkup_time_test(uint8_t bySlpRcvTim);
void deepsleep_wkup_time_test(uint32_t wSclkFreq, uint8_t byDspRcvTim);

#endif