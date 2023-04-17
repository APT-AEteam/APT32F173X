/***********************************************************************//** 
 * \file  syscon test.h
 * \brief  test description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-3-24 <td>V0.0  <td>YYM   <td>initial
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _SYSCON_TEST_H
#define _SYSCON_TEST_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"

typedef enum {
    SRC_IMOSC_INT = 0,
    SRC_EMOSC_INT,
    SRC_HFOSC_INT,
	SRC_PLL_INT,
	SRC_ISOSC_INT,
	SRC_ESOSC_INT,	
	SRC_SYCOSC_INT
} cclk_src_int_e;

//syscon test
extern void syscon_nmi_int_test(void);
extern void syscon_eft_led_test(void);
extern void syscon_fvr_test(void);
extern void syscon_emosc_rst_test(void);
extern void syscon_emosc_audo_imosc_test(void);
extern void syscon_emosc_cmk_int_test(void);
extern void syscon_osc5005_trim_test(void);
extern void syscon_rst_test(void);
extern void syscon_pllunlock_test(void);
extern void syscon_sram_test(void);
extern void syscon_reset_test(void);
extern void syscon_reset_srb_test(void);
extern void syscon_low_fre_testing(void);
extern void syscon_lvd_test(void);
extern void syscon_lvr_test(void);
extern void syscon_lvd_power_test(void);
extern void syscon_clk_int_test(cclk_src_int_e eSrc);
extern void syscon_em_fiten_test(void);
extern void syscon_iwdt_irq_test(void);
extern void syscon_lpt_wakeup_deepsleep_test(void);
extern void syscon_lpt_wakeup_sleep_testing(void);
extern void syscon_io_wakeup_sleep_test(void);
extern int syscon_low_temp_mclk_switch_test(void);
extern void syscon_deepsleep_lowtemp_test(void);
extern int syscon_evtrg_int_test(void);
extern int syscon_evswf_test(void);



#endif