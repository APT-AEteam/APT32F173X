
/***********************************************************************//** 
 * \file  hwdiv.c
 * \brief  to replace div functions in stdio
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-20 <td>V0.0  <td>WNN   <td>initial
 * </table>
 * *********************************************************************
*/

#include "soc.h"
#include "csp_hwdiv.h"

#if 0
#define HWDIV_REG_BASE	(csp_hwdiv_t *)APB_HWD_BASE  

//!!!This function is to replace the div function in stdio.h
//!!!This function will be called AUTOMATICALLY when "/" is used.
int __divsi3(int wDividend, int wDivisor)
{
	uint32_t wPsr,wQt;

	wPsr = __get_MSTATUS();
	__disable_irq();
	
	csp_hwdiv_t * ptHwdivBase = (csp_hwdiv_t *)HWDIV_REG_BASE;
	ptHwdivBase->CR = 0;
	
	ptHwdivBase->DIVIDEND = wDividend;
	ptHwdivBase->DIVISOR = wDivisor;
	
	wQt = ptHwdivBase->QUOTIENT;
	__set_MSTATUS(wPsr);
	return (wQt);
}

//!!!This function is to replace the mod function in stdio.h
//!!!This function will be called AUTOMATICALLY when "%" is used.
int __modsi3(int wDividend, int wDivisor)
{
	uint32_t wPsr,wRm;

	wPsr = __get_MSTATUS();
	__disable_irq();
	
	csp_hwdiv_t * ptHwdivBase = (csp_hwdiv_t *)HWDIV_REG_BASE;
	ptHwdivBase->CR = 0;
	
	ptHwdivBase->DIVIDEND = wDividend;
	ptHwdivBase->DIVISOR = wDivisor;

	wRm = ptHwdivBase->REMAIN;
	__set_MSTATUS(wPsr);
	return (wRm);
}

//!!!This function is to replace the div function in stdio.h
//!!!This function will be called AUTOMATICALLY when "/" is used.
unsigned int __udivsi3(unsigned int wDividend, unsigned int wDivisor)
{
	uint32_t wPsr,wQt;

	wPsr = __get_MSTATUS();
	__disable_irq();
	
	csp_hwdiv_t * ptHwdivBase = (csp_hwdiv_t *)HWDIV_REG_BASE;
	ptHwdivBase->CR = 1;
	
	ptHwdivBase->DIVIDEND = wDividend;
	ptHwdivBase->DIVISOR = wDivisor;

	wQt = ptHwdivBase->QUOTIENT;
	__set_MSTATUS(wPsr);
	return (wQt);
}

//!!!This function is to replace the mod function in stdio.h
//!!!This function will be called AUTOMATICALLY when "%" is used.
unsigned int __umodsi3(unsigned int wDividend, unsigned int wDivisor)
{
	uint32_t wPsr,wRm;
	
	wPsr = __get_MSTATUS();
	__disable_irq();
	
	csp_hwdiv_t * ptHwdivBase = (csp_hwdiv_t *)HWDIV_REG_BASE;
	ptHwdivBase->CR = 1;
	
	ptHwdivBase->DIVIDEND = wDividend;
	ptHwdivBase->DIVISOR = wDivisor;

	wRm = ptHwdivBase->REMAIN;
	__set_MSTATUS(wPsr);
	return (wRm);
}

#endif