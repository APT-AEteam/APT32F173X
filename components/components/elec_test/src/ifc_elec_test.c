/***********************************************************************//** 
 * \file  ifc_elec_test.c
 * \brief  Flash电气参数测试代码
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-4-6 <td>V0.0  <td>WNN   <td>initial
 * </table>
 * *********************************************************************
*/


/* include ----------------------------------------------------------------*/
#include "ifc.h"
#include "reliability.h"
#include "iwdt.h"
#include "pin.h"

/**
 \brief 测试Flash Endurance
  * 
  * \return      void
*/
#define FLASHERROR csi_pin_set_low(PA0)

void eflash_endurance(void)
{
	csi_rsr_src_e eRstSrc;
	uint32_t i,wTime1,wTime;
	uint32_t wFillData[64] = {0xffff0000};
	uint32_t wTimeData[1] = {0};
	csi_error_t tError;
	
	for(i=1;i<64;i++) 
	{
		wFillData[i] = wFillData[i-1]+1;
	}
	
	csi_iwdt_close();
	csi_pin_set_mux(PA0, PA0_OUTPUT);
	
	eRstSrc = csi_get_rst_reason();
	if ((eRstSrc != RST_SRC_POR) && (eRstSrc != RST_SRC_CPU)) {
		FLASHERROR;
		while(1);
	}
	
	
	wTime1 = *(uint32_t *)(0x10000000);
	//wTime2 = *(uint32_t *)(0x10000040);
	
	wTime = wTime1;
	
	
	while(1) {
		for(i = 0; i<DFLASHPAGENUM-2; i++)
		{
			tError = csi_ifc_program(IFC, 0x10000080+DFLASH_PAGE_SZ*4*i, wFillData, 16);	
			if (g_bFlashCheckPass == 0 )	{						//第一种判断方法
				FLASHERROR;
				while(1);
			}
			if (tError == CSI_ERROR )	{						//第一种判断方法
				FLASHERROR;
				while(1);
			}
		}
		
		for(i = 32; i<PFLASHPAGENUM-32; i++) //从8K开始向上测试
		{
			tError = csi_ifc_program(IFC, PFLASH_PAGE_SZ*4*i, wFillData, 64);	
			if (g_bFlashCheckPass == 0 )	{						//第一种判断方法
				FLASHERROR;
				while(1);
			}
			if (tError == CSI_ERROR )	{						//第一种判断方法
				FLASHERROR;
				while(1);
			}
		}
		wTime++;
		if (wTime%10 == 0) {
			
			wTimeData[0] = wTime;
			csi_ifc_program(IFC, 0x10000000,wTimeData,1);
			csi_ifc_program(IFC, 0x10000040,wTimeData,1);
			csi_pin_toggle(PA0);
			
		}
	}
}