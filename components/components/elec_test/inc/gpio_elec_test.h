
/***********************************************************************//** 
 * \file  gpio_elec_test.h
 * \brief  gpio electrical test description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2022-4-10 <td>V0.0  <td>WCH   <td>initial
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _GPIO_ELEC_TEST_H
#define _GPIO_ELEC_TEST_H

extern void gpio_vih_vil_test_nottl_mode(void);
extern void gpio_vih_vil_test_ttl1_mode(void);
extern void gpio_vih_vil_test_ttl2_mode(void);
extern void gpio_voh_test(void);
extern void gpio_vol_test(void);
extern void gpio_rpu_test(void);
extern void gpio_rpd_test(void);
extern void gpio_iofout_test(void);
extern void gpio_iofin_test(void);
extern void gpio_ilih_ilil_test(void);
extern void gpio_ioc_test(void);

#endif