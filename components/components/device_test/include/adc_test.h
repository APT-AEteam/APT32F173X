/***********************************************************************//** 
 * \file  adc test.h
 * \brief  adc test description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-5-8 <td>V0.0  <td>XS   <td>initial
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _ADC_TEST_H
#define _ADC_TEST_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"

extern int adc_samp_oneshot_test(void);
extern int adc_samp_continuous_test(void);
extern int adc_samp_oneshot_int_test(void);
extern int adc_samp_continuous_int_test(void);
extern int adc_samp_continuous_cmp_cross(void);
extern int adc_samp_oneshot_cmp_always(void);
extern int etcb_one_trg_one_test(void);
extern int etcb_one_trg_more_test(void);
extern int etcb_event_trigger_out_test(void);

#endif