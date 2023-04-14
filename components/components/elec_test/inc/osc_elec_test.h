/***********************************************************************//** 
 * \file  osc_elec_test.h
 * \brief  osc电气参数测试代码
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-4-6 <td>V0.0  <td>YYM   <td>initial
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _OSC_ELEC_TEST_H
#define _OSC_ELEC_TEST_H

extern void osc_deepsleep_imosc_test(void)
extern void osc_deepsleep_isosc_test(void)
extern void osc_sleep_imosc_test(void);
extern void osc_sleep_hfosc_test(void);
extern void osc_sleep_isosc_test(void);
extern void osc_sleep_pll_test(void);

#endif