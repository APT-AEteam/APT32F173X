/***********************************************************************//** 
 * \file  can_test.h
 * \brief  test description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-04-19 <td>V0.0  <td>ZJY   <td>initial
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CAN_TEST_H
#define _CAN_TEST_H

/* Includes ------------------------------------------------------------------*/
#include <drv/can.h>

int can_send_test(csi_can_chnl_e eCanChnl, uint8_t byChnlNum);
int can_recv_test(csi_can_chnl_e eCanChnl, uint8_t byChnlNum, uint8_t byMskEn);
int can_remote_frames_test(void);
int can_basic_mode_test(void);
int can_lback_silent_mode_test(void);

#endif