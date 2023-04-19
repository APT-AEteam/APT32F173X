/***********************************************************************//** 
 * \file  bt test.h
 * \brief  bt test description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-4-10 <td>V0.0  <td>CWL   <td>initial
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _BT_TEST_H
#define _BT_TEST_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"


extern void bt_timer_test(void);
extern void bt_pwm_test(void);
extern int bt_sync_start_test(void);
extern int bt_sync_count_test(void);
extern int bt_sync_stop_test(void);
extern int bt_sync_startstop_test(void);
extern int bt_evtrg_test(void);
extern int bt_syncmd_start_test(void);

#endif