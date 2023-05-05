/***********************************************************************//** 
 * \file  dac elec test.h
 * \brief  dac elec test description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-4-28 <td>V0.0  <td>XS   <td>initial
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _DAC_ELEC_TEST_H
#define _DAC_ELEC_TEST_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"


extern void dac_maxcov_test(void);
extern void dac_current_test(void);
extern void dac_inl_test(void);
extern void dac_offset_test(void);


#endif