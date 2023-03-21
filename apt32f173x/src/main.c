/***********************************************************************//** 
 * \file  main.c
 * \brief  MAIN description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  		<th>Version <th>Author	<th>Description
 * <tr><td> 2021-02-21  <td>V0.0  	<td>ZJY   	<td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include <csi_drv.h>
#include <iostring.h>

#include "demo.h"
/* Private macro-----------------------------------------------------------*/

/* externs function--------------------------------------------------------*/
extern void system_init(void);
extern void board_init(void);
extern void user_demo(void);

/* externs variablesr------------------------------------------------------*/

int etcb_dj_test(void)
{
	int iRet = 0;
	volatile uint8_t ch;	
	
	csi_etb_config_t tEtbConfig;				//ETB 参数配置结构体	
	tEtbConfig.byChType  = ETB_ONE_TRG_ONE;  	//单个源触发单个目标
	tEtbConfig.bySrcIp   = ETB_BT0_TRGOUT;//ETB_BT0_TRGOUT ;  	//...作为触发源
	tEtbConfig.byDstIp   = ETB_DAC0_SYNCIN0;//ETB_ADC0_SYNCIN0;  //ADC0_SYNCIN0作为目标事件
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	//自动获取空闲通道号,ch >= 0 获取成功						//ch < 0,则获取通道号失败		
	iRet = csi_etb_ch_config(ch, &tEtbConfig);	

	ETCB ->CFG_CHX[0] |=0x1<<1;
	return iRet;
}
/* Private variablesr------------------------------------------------------*/

int main()
{
    system_init();
	board_init();
//	csi_clo_config(CLO_HFCLK, CLO_DIV1, PA2);	//PC11 作为BT1 PWM输出引脚
//	csi_pin_set_mux(PA8, PA8_DAC0_OUT);	
	csi_pin_set_mux(PA4, PA4_DAC0_OUT);		
//	csi_pin_set_mux(PA6, PA6_OUTPUT);		//PA06 output
//	csi_pin_set_high(PA6);					//PA06 output high;
	
//	my_printf("Hello World~~~~~~~\n");		//print message
	
//	user_demo();							//demo	
	etcb_dj_test();
			
	dac_demo();


	while(1)
	{	
	delay_ums(100);
	ETCB->SOFTTRIG |= 0x8;//软件触发ETCB
//	csi_pin_toggle(PA6);				//PA06 toggle
	}
	
    return 0;
}
