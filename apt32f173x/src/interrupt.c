/***********************************************************************//** 
 * \file  main.c
 * \brief  MAIN description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  		<th>Version <th>Author	<th>Description
 * <tr><td> 2021-10-6  <td>V0.0  	<td>GQQ  	<td>initial
 * <tr><td> 2021-10-19  <td>V0.1  	<td>ZJY  	<td>code normalization
 * </table>
 * *********************************************************************
*/

/* 本工程中断处理的weak函数都在irq.c中。
 * 在xxx_demo.c中，我们对相应IP的中断处理函数进行了重写。故调用xxx_demo.c的时候，用到的是xxx_demo.c中的中断处理函数。
 * 为防止重复重写中断处理函数,出现错误，建议将用到的中断从相应的xxx_demo.c中剪切到该文件。
 */
 
/* Includes ---------------------------------------------------------------*/
#include "csi_drv.h"
#include "board_config.h"

/* Private macro-----------------------------------------------------------*/
/* externs function--------------------------------------------------------*/
/* externs variable--------------------------------------------------------*/
/* Private variable--------------------------------------------------------*/

