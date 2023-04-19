
/***********************************************************************//** 
 * \file  opa_elec_test.h
 * \brief  opa electrical test description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2022-4-19 <td>V0.0  <td>WCH   <td>initial
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _OPA_ELEC_TEST_H
#define _OPA_ELEC_TEST_H

extern void opa_io_init(void);
extern void opa_operate_voltage_test(void);
extern void opa_input_offset_voltage_test(void);
extern void opa_input_common_mode_voltage_test(void);
extern void opa_output_voltage_test(void);
extern void opa_output_current_test(void);
extern void opa_internal_gain_test(void);

#endif