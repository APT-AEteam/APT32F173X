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
#ifndef _LVD_R_ELEC_TEST_H
#define _LVD_R_ELEC_TEST_H
/* Includes ------------------------------------------------------------------*/
#include <drv/reliability.h>

void lvr_test(csi_lvr_level_e eLvrLev);
void lvd_test(csi_lvd_level_e eLvdLev);
void icc_test(bool bLvdEn);
void lvr_deepsleep_test(csi_lvr_level_e eLvrLev);
void lvd_deepsleep_test(csi_lvd_level_e eLvdLev);

#endif