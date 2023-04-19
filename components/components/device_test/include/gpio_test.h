/***********************************************************************//** 
 * \file  test.h
 * \brief  test description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-02-21 <td>V0.0  <td>ZJY   <td>initial
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _GPIO_TEST_H
#define _GPIO_TEST_H

/* Includes ------------------------------------------------------------------*/
#include <drv/pin.h>
#include <drv/gpio.h>

//gpio 
int gpio_mux_test(void);
int gpio_ouput_test(void);
int gpio_input_test(void);
int gpio_interrupt_test(csp_gpio_t *pGpioPort, csi_gpio_irq_mode_e eTrgEdge);
int gpio_ex_interrupt_test(pin_name_e ePInName);
int gpio_exi_flt_test(pin_name_e ePinName, csi_exi_grp_e eExiGrp, csi_exi_flttm_e eFltTime);
int gpio_ioremap_test(void);

#endif