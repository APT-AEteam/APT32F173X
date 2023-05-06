/***********************************************************************//** 
 * \file  adc elec test.h
 * \brief  adc elec test description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-5-4 <td>V0.0  <td>XS   <td>initial
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _ADC_ELEC_TEST_H
#define _ADC_ELEC_TEST_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"


extern void adc_work_current_test(void);
extern void adc_work_voltage_test(void);
extern void adc_accuracy_test(void);
extern void adc_speed_test(void);
extern void adc_offset_test(void);
extern void adc_rain_test(void);
extern void adc_inl_test(void);

#endif