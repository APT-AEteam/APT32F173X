/***********************************************************************//** 
 * \file  dac_demo.c
 * \brief  FVR_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-3-17 <td>V0.0 <td>dac     <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include "sys_clk.h"
#include <drv/dac.h>
#include "pin.h"

#include "demo.h"
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/
void delay(uint16_t val)
{bool t;uint32_t vha;
    vha=val;t=1;
	while(t){if(vha)vha--;
			 else{
				 t=0;
				 }
	}
}
uint16_t i;
void dac_demo(void)	
{
	csp_dac_clr_da(DAC0);//清除转换数据
	csp_dac_set_clk_div(DAC0,16);//设置时钟16分频
	csp_dac_buff_enable(DAC0,1);//使能BUFF信号
	csp_dac_refsel_enable(DAC0,0);//关闭REF
	csp_dac_powerdown_enable(DAC0, 1);//开启powerdown
	csp_dac_set_datar(DAC0,34952);//在DATAR中写入数据
//	csp_dac_set_datal(DAC0,65535);
	csp_dac_irq_enable(DAC0, DAC_EOC,1);//使能EOC中断
	csp_dac_irq_enable(DAC0, DAC_WRERR,1);
	csp_dac_irq_enable(DAC0, DAC_SYNCERR,1);
	
	csp_dac_syncr_enable(DAC0, DAC_SYNCIN0,true);//开启DAC_SYNCIN0触发
	csp_dac_syncr_enable(DAC0, DAC_SYNCIN1,true);
	csp_dac_syncr_enable(DAC0, DAC_SYNCIN2,true);
	csp_dac_step_val(DAC0, 409);//设置触发增减值
	
	csp_dac_start(DAC0);

//	}
}