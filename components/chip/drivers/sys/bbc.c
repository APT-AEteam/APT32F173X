
/***********************************************************************//** 
 * \file  bbc.c
 * \brief  bit banding
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-6-30 <td>V0.0 <td>WNN   <td>initial
 * </table>
 * *********************************************************************
*/


#include "csp.h"

void csi_bbc_write(uint32_t wAddr, uint8_t byBit, bool bVal)
{
	uint32_t wBase, wAddrBit;
	
	wBase = (wAddr & 0xf0000000) + 0x8000000;
	wAddrBit = wBase | (wAddr << 5) | byBit;
	
	*((uint8_t *) wAddrBit) = bVal;
	

	
}