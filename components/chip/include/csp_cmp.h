/***********************************************************************//** 
 * \file  csp_cmp.h
 * \brief CMP description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-10-14 <td>V0.0  <td>YYM   <td>modify
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CSP_CMP_H
#define _CSP_CMP_H

/* Includes ------------------------------------------------------------------*/
#include <soc.h>

/// \struct csp_cmp_t
/// \brief CMP reg description   
typedef struct
{
   __IOM uint32_t  CEDR;  		//0x0000	Clock control & ID
   __IOM uint32_t  CR;		    //0x0004	Start & Stop Ctrl
   __IOM uint32_t  DFCR1;		//0x0008	Clock prescaler
   __IOM uint32_t  DFCR2;  		//0x000C	Control register
   __IOM uint32_t  WFCR;		//0x0010	Synchronization control reg
   __IOM uint32_t  INPCR;   	//0x0014	Period reg
   __IM  uint32_t  RISR;       	//0x0034    Interrupt RISR
   __IOM uint32_t  IMCR;       	//0x003C    Interrupt IMCR
   __IM  uint32_t  MISR;       	//0x0038    Interrupt MISR
   __OM  uint32_t  ICR;        	//0x0040    Interrupt clear
} csp_cmp_t;



#endif

