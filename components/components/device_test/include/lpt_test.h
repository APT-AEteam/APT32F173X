/***********************************************************************//** 
 * \file  lpt test.h
 * \brief  lpt test description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-4-19 <td>V0.0  <td>YT   <td>initial
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _LPT_TEST_H
#define _LPT_TEST_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"

int lpt_pwm_test(void);
int lpt_sync_trg_start_test(void);
int lpt_trg_out_test(void);
int lpt_soft_trg_out_test(void);
int lpt_filter_test(void);
int lpt_window_test(void);
int lpt_prdld_test(void);
int lpt_pscld_test(void);
int lpt_cmpld_test(void);

__attribute__((weak)) void lpt_irqhandler(csp_lpt_t *ptLptBase);

#endif