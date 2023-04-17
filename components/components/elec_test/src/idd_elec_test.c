/***********************************************************************//** 
 * \file  ifc_elec_test.c
 * \brief  IDD电气参数测试代码
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-4-6 <td>V0.0  <td>YT   <td>initial
 * </table>
 * *********************************************************************
*/
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
/* Private variablesr------------------------------------------------------*/


/**
 \brief 测试IDD1
  *   主函数中，当选择131.072KHZ时，通过csp_eflash_lpmd_enable(SYSCON, DISABLE)函数，使能EFlash_LPMD或禁止EFlash_LPMD。 
  * \return      void
*/
void idd1_elec_test(void)
{
	
	// 测131.072 kHz时，用到下面三行函数
//	csp_eflash_lpmd_enable(SYSCON, ENABLE);
//	mdelay(4000);
//	csp_eflash_lpmd_enable(SYSCON, DISABLE);
	
	
	// 所有IO输出高(SWD除外)
	csi_pin_set_mux(PA0, PA0_OUTPUT);		//PA0 output
	csi_pin_set_high(PA0);					//PA0 output high;

	csi_pin_set_mux(PA1, PA1_OUTPUT);		//PA1 output
	csi_pin_set_high(PA1);	
	
	csi_pin_set_mux(PA2, PA2_OUTPUT);		//PA2 output
	csi_pin_set_high(PA2);					//PA2 output high;

	csi_pin_set_mux(PA3, PA3_OUTPUT);		//PA3 output
	csi_pin_set_high(PA3);	
	
	csi_pin_set_mux(PA4, PA4_OUTPUT);		//PA4 output
	csi_pin_set_high(PA4);					//PA4 output high;

	csi_pin_set_mux(PA5, PA5_OUTPUT);		//PA5 output
	csi_pin_set_high(PA5);	
	
	csi_pin_set_mux(PA6, PA6_OUTPUT);		//PA6 output
	csi_pin_set_high(PA6);					//PA6 output high;

	csi_pin_set_mux(PA7, PA7_OUTPUT);		//PA7 output
	csi_pin_set_high(PA7);	
	
	csi_pin_set_mux(PA8, PA8_OUTPUT);		//PA8 output
	csi_pin_set_high(PA8);					//PA8 output high;

	csi_pin_set_mux(PA9, PA9_OUTPUT);		//PA9 output
	csi_pin_set_high(PA9);	
	
	csi_pin_set_mux(PA10, PA10_OUTPUT);		//PA10 output
	csi_pin_set_high(PA10);					//PA10 output high;

	csi_pin_set_mux(PA11, PA11_OUTPUT);		//PA11 output
	csi_pin_set_high(PA11);	
	
	csi_pin_set_mux(PA12, PA12_OUTPUT);		//PA12 output
	csi_pin_set_high(PA12);					//PA12 output high;

//	csi_pin_set_mux(PA13, PA13_OUTPUT);		//PA13 output,swd
//	csi_pin_set_high(PA13);	
	
	csi_pin_set_mux(PA14, PA14_OUTPUT);		//PA14 output
	csi_pin_set_high(PA14);					//PA14 output high;

	csi_pin_set_mux(PA15, PA15_OUTPUT);		//PA15 output
	csi_pin_set_high(PA15);	


	csi_pin_set_mux(PB0, PB0_OUTPUT);		//PB0 output
	csi_pin_set_high(PB0);					//PB0 output high;

	csi_pin_set_mux(PB1, PB1_OUTPUT);		//PB1 output
	csi_pin_set_high(PB1);	
	
	csi_pin_set_mux(PB2, PB2_OUTPUT);		//PB2 output
	csi_pin_set_high(PB2);					//PB2 output high;

	csi_pin_set_mux(PB3, PB3_OUTPUT);		//PB3 output
	csi_pin_set_high(PB3);	
	
	csi_pin_set_mux(PB4, PB4_OUTPUT);		//PB4 output
	csi_pin_set_high(PB4);					//PB4 output high;

	csi_pin_set_mux(PB5, PB5_OUTPUT);		//PB5 output
	csi_pin_set_high(PB5);	
	
	csi_pin_set_mux(PB6, PB6_OUTPUT);		//PB6 output
	csi_pin_set_high(PB6);					//PB6 output high;

	csi_pin_set_mux(PB7, PB7_OUTPUT);		//PB7 output
	csi_pin_set_high(PB7);	
	
	csi_pin_set_mux(PB8, PB8_OUTPUT);		//PB8 output
	csi_pin_set_high(PB8);					//PB8 output high;

	csi_pin_set_mux(PB9, PB9_OUTPUT);		//PB9 output
	csi_pin_set_high(PB9);	
	
	csi_pin_set_mux(PB10, PB10_OUTPUT);		//PB10 output
	csi_pin_set_high(PB10);					//PB10 output high;

	csi_pin_set_mux(PB11, PB11_OUTPUT);		//PB11 output
	csi_pin_set_high(PB11);	
	
	csi_pin_set_mux(PB12, PB12_OUTPUT);		//PB12 output
	csi_pin_set_high(PB12);					//PB12 output high;

	csi_pin_set_mux(PB13, PB13_OUTPUT);		//PB13 output
	csi_pin_set_high(PB13);	
	
	csi_pin_set_mux(PB14, PB14_OUTPUT);		//PB14 output
	csi_pin_set_high(PB14);					//PB14 output high;

	csi_pin_set_mux(PB15, PB15_OUTPUT);		//PB15 output
	csi_pin_set_high(PB15);	


	csi_pin_set_mux(PC0, PC0_OUTPUT);		//PC0 output
	csi_pin_set_high(PC0);					//PC0 output high;

	csi_pin_set_mux(PC1, PC1_OUTPUT);		//PC1 output
	csi_pin_set_high(PC1);	
	
	csi_pin_set_mux(PC2, PC2_OUTPUT);		//PC2 output
	csi_pin_set_high(PC2);					//PC2 output high;

	csi_pin_set_mux(PC3, PC3_OUTPUT);		//PC3 output
	csi_pin_set_high(PC3);	
	
	csi_pin_set_mux(PC4, PC4_OUTPUT);		//PC4 output
	csi_pin_set_high(PC4);					//PC4 output high;

	csi_pin_set_mux(PC5, PC5_OUTPUT);		//PC5 output
	csi_pin_set_high(PC5);	
	
	csi_pin_set_mux(PC6, PC6_OUTPUT);		//PC6 output
	csi_pin_set_high(PC6);					//PC6 output high;

	csi_pin_set_mux(PC7, PC7_OUTPUT);		//PC7 output
	csi_pin_set_high(PC7);	
	
	csi_pin_set_mux(PC8, PC8_OUTPUT);		//PC8 output
	csi_pin_set_high(PC8);					//PC8 output high;

	csi_pin_set_mux(PC9, PC9_OUTPUT);		//PC9 output
	csi_pin_set_high(PC9);	
	
	csi_pin_set_mux(PC10, PC10_OUTPUT);		//PC10 output
	csi_pin_set_high(PC10);					//PC10 output high;

	csi_pin_set_mux(PC11, PC11_OUTPUT);		//PC11 output
	csi_pin_set_high(PC11);	
	
	csi_pin_set_mux(PC12, PC12_OUTPUT);		//PC12 output
	csi_pin_set_high(PC12);					//PC12 output high;

	csi_pin_set_mux(PC13, PC13_OUTPUT);		//PC13 output
	csi_pin_set_high(PC13);	
	
	csi_pin_set_mux(PC14, PC14_OUTPUT);		//PC14 output
	csi_pin_set_high(PC14);					//PC14 output high;

	csi_pin_set_mux(PC15, PC15_OUTPUT);		//PC15 output
	csi_pin_set_high(PC15);		
	

	csi_pin_set_mux(PD0, PD0_OUTPUT);		//PD0 output
	csi_pin_set_high(PD0);					//PD0 output high;

	csi_pin_set_mux(PD1, PD1_OUTPUT);		//PD1 output
	csi_pin_set_high(PD1);	
	
	csi_pin_set_mux(PD2, PD2_OUTPUT);		//PD2 output
	csi_pin_set_high(PD2);					//PD2 output high;

	csi_pin_set_mux(PD3, PD3_OUTPUT);		//PD3 output
	csi_pin_set_high(PD3);	
	
	csi_pin_set_mux(PD4, PD4_OUTPUT);		//PD4 output
	csi_pin_set_high(PD4);					//PD4 output high;

	csi_pin_set_mux(PD5, PD5_OUTPUT);		//PD5 output
	csi_pin_set_high(PD5);	

	while(1)
	{
		
	}
}




/**
 \brief 测试IDD2
  *    
  * \return      void
*/
void idd2_elec_test(void)
{
	
	
	// 所有IO输出高(SWD除外)
	csi_pin_set_mux(PA0, PA0_OUTPUT);		//PA0 output
	csi_pin_set_high(PA0);					//PA0 output high;

	csi_pin_set_mux(PA1, PA1_OUTPUT);		//PA1 output
	csi_pin_set_high(PA1);	
	
	csi_pin_set_mux(PA2, PA2_OUTPUT);		//PA2 output
	csi_pin_set_high(PA2);					//PA2 output high;

	csi_pin_set_mux(PA3, PA3_OUTPUT);		//PA3 output
	csi_pin_set_high(PA3);	
	
	csi_pin_set_mux(PA4, PA4_OUTPUT);		//PA4 output
	csi_pin_set_high(PA4);					//PA4 output high;

	csi_pin_set_mux(PA5, PA5_OUTPUT);		//PA5 output
	csi_pin_set_high(PA5);	
	
	csi_pin_set_mux(PA6, PA6_OUTPUT);		//PA6 output
	csi_pin_set_high(PA6);					//PA6 output high;

	csi_pin_set_mux(PA7, PA7_OUTPUT);		//PA7 output
	csi_pin_set_high(PA7);	
	
	csi_pin_set_mux(PA8, PA8_OUTPUT);		//PA8 output
	csi_pin_set_high(PA8);					//PA8 output high;

	csi_pin_set_mux(PA9, PA9_OUTPUT);		//PA9 output
	csi_pin_set_high(PA9);	
	
	csi_pin_set_mux(PA10, PA10_OUTPUT);		//PA10 output
	csi_pin_set_high(PA10);					//PA10 output high;

	csi_pin_set_mux(PA11, PA11_OUTPUT);		//PA11 output
	csi_pin_set_high(PA11);	
	
	csi_pin_set_mux(PA12, PA12_OUTPUT);		//PA12 output
	csi_pin_set_high(PA12);					//PA12 output high;

//	csi_pin_set_mux(PA13, PA13_OUTPUT);		//PA13 output,swd
//	csi_pin_set_high(PA13);	
	
	csi_pin_set_mux(PA14, PA14_OUTPUT);		//PA14 output
	csi_pin_set_high(PA14);					//PA14 output high;

	csi_pin_set_mux(PA15, PA15_OUTPUT);		//PA15 output
	csi_pin_set_high(PA15);	


	csi_pin_set_mux(PB0, PB0_OUTPUT);		//PB0 output
	csi_pin_set_high(PB0);					//PB0 output high;

	csi_pin_set_mux(PB1, PB1_OUTPUT);		//PB1 output
	csi_pin_set_high(PB1);	
	
	csi_pin_set_mux(PB2, PB2_OUTPUT);		//PB2 output
	csi_pin_set_high(PB2);					//PB2 output high;

	csi_pin_set_mux(PB3, PB3_OUTPUT);		//PB3 output
	csi_pin_set_high(PB3);	
	
	csi_pin_set_mux(PB4, PB4_OUTPUT);		//PB4 output
	csi_pin_set_high(PB4);					//PB4 output high;

	csi_pin_set_mux(PB5, PB5_OUTPUT);		//PB5 output
	csi_pin_set_high(PB5);	
	
	csi_pin_set_mux(PB6, PB6_OUTPUT);		//PB6 output
	csi_pin_set_high(PB6);					//PB6 output high;

	csi_pin_set_mux(PB7, PB7_OUTPUT);		//PB7 output
	csi_pin_set_high(PB7);	
	
	csi_pin_set_mux(PB8, PB8_OUTPUT);		//PB8 output
	csi_pin_set_high(PB8);					//PB8 output high;

	csi_pin_set_mux(PB9, PB9_OUTPUT);		//PB9 output
	csi_pin_set_high(PB9);	
	
	csi_pin_set_mux(PB10, PB10_OUTPUT);		//PB10 output
	csi_pin_set_high(PB10);					//PB10 output high;

	csi_pin_set_mux(PB11, PB11_OUTPUT);		//PB11 output
	csi_pin_set_high(PB11);	
	
	csi_pin_set_mux(PB12, PB12_OUTPUT);		//PB12 output
	csi_pin_set_high(PB12);					//PB12 output high;

	csi_pin_set_mux(PB13, PB13_OUTPUT);		//PB13 output
	csi_pin_set_high(PB13);	
	
	csi_pin_set_mux(PB14, PB14_OUTPUT);		//PB14 output
	csi_pin_set_high(PB14);					//PB14 output high;

	csi_pin_set_mux(PB15, PB15_OUTPUT);		//PB15 output
	csi_pin_set_high(PB15);	


	csi_pin_set_mux(PC0, PC0_OUTPUT);		//PC0 output
	csi_pin_set_high(PC0);					//PC0 output high;

	csi_pin_set_mux(PC1, PC1_OUTPUT);		//PC1 output
	csi_pin_set_high(PC1);	
	
	csi_pin_set_mux(PC2, PC2_OUTPUT);		//PC2 output
	csi_pin_set_high(PC2);					//PC2 output high;

	csi_pin_set_mux(PC3, PC3_OUTPUT);		//PC3 output
	csi_pin_set_high(PC3);	
	
	csi_pin_set_mux(PC4, PC4_OUTPUT);		//PC4 output
	csi_pin_set_high(PC4);					//PC4 output high;

	csi_pin_set_mux(PC5, PC5_OUTPUT);		//PC5 output
	csi_pin_set_high(PC5);	
	
	csi_pin_set_mux(PC6, PC6_OUTPUT);		//PC6 output
	csi_pin_set_high(PC6);					//PC6 output high;

	csi_pin_set_mux(PC7, PC7_OUTPUT);		//PC7 output
	csi_pin_set_high(PC7);	
	
	csi_pin_set_mux(PC8, PC8_OUTPUT);		//PC8 output
	csi_pin_set_high(PC8);					//PC8 output high;

	csi_pin_set_mux(PC9, PC9_OUTPUT);		//PC9 output
	csi_pin_set_high(PC9);	
	
	csi_pin_set_mux(PC10, PC10_OUTPUT);		//PC10 output
	csi_pin_set_high(PC10);					//PC10 output high;

	csi_pin_set_mux(PC11, PC11_OUTPUT);		//PC11 output
	csi_pin_set_high(PC11);	
	
	csi_pin_set_mux(PC12, PC12_OUTPUT);		//PC12 output
	csi_pin_set_high(PC12);					//PC12 output high;

	csi_pin_set_mux(PC13, PC13_OUTPUT);		//PC13 output
	csi_pin_set_high(PC13);	
	
	csi_pin_set_mux(PC14, PC14_OUTPUT);		//PC14 output
	csi_pin_set_high(PC14);					//PC14 output high;

	csi_pin_set_mux(PC15, PC15_OUTPUT);		//PC15 output
	csi_pin_set_high(PC15);		
	

	csi_pin_set_mux(PD0, PD0_OUTPUT);		//PD0 output
	csi_pin_set_high(PD0);					//PD0 output high;

	csi_pin_set_mux(PD1, PD1_OUTPUT);		//PD1 output
	csi_pin_set_high(PD1);	
	
	csi_pin_set_mux(PD2, PD2_OUTPUT);		//PD2 output
	csi_pin_set_high(PD2);					//PD2 output high;

	csi_pin_set_mux(PD3, PD3_OUTPUT);		//PD3 output
	csi_pin_set_high(PD3);	
	
	csi_pin_set_mux(PD4, PD4_OUTPUT);		//PD4 output
	csi_pin_set_high(PD4);					//PD4 output high;

	csi_pin_set_mux(PD5, PD5_OUTPUT);		//PD5 output
	csi_pin_set_high(PD5);	
	
	// 解决OSC5005(24M)时，进入sleep后，没有HF频率输出的问题
	//SYSCON_PWRCR寄存器 SLP配置部分需要配置成4。（使能SLEEP模式下的VOS，VDDCORE选择VCref(BGR 1.0V) x 1.2,电压升高）
//	SYSCON->PWRKEY = 0xa67a6CC7;
//	SYSCON->PWRCR= (SYSCON->PWRCR & (~((0x0Ful << 0)))) | (2 << 0);   //VOSEN
//	SYSCON->PWRCR= (SYSCON->PWRCR & (~((0x0Ful << 16)))) | (4 << 16);   //SLP_CFG
	

	
	csi_pm_clk_enable(SP_IDLE_PCLK, ENABLE);   //使能或禁止 SLEEP 模式下的 PCLK(缺省使能)
	SYSCON->PCDR0=0xffffffff;    // 禁止相应模块PCLK时钟
	SYSCON->PCDR1=0xffffffff;
	
//	SYSCON->PCER0=0xffffffff;    // 禁止相应模块PCLK时钟
//	SYSCON->PCER1=0xffffffff;
	
	
	
	csi_pm_enter_sleep(PM_MODE_SLEEP);
	
}




/**
 \brief 测试IDD3
  *    
  * \return      void
*/
void idd3_elec_test(void)
{
	//所有IO高阻态
	csi_pin_set_mux(PA0, PA0_GPD);		//PA0 output
	csi_pin_set_mux(PA1, PA1_GPD);		//PA1 output
	csi_pin_set_mux(PA2, PA2_GPD);		//PA2 output
	csi_pin_set_mux(PA3, PA3_GPD);		//PA3 output
	csi_pin_set_mux(PA4, PA4_GPD);		//PA4 output
	csi_pin_set_mux(PA5, PA5_GPD);		//PA5 output
	csi_pin_set_mux(PA6, PA6_GPD);		//PA6 output
	csi_pin_set_mux(PA7, PA7_GPD);		//PA7 output
	csi_pin_set_mux(PA8, PA8_GPD);		//PA8 output
	csi_pin_set_mux(PA9, PA9_GPD);		//PA9 output
	csi_pin_set_mux(PA10, PA10_GPD);		//PA10 output
	csi_pin_set_mux(PA11, PA11_GPD);		//PA11 output
	csi_pin_set_mux(PA12, PA12_GPD);		//PA12 output
	csi_pin_set_mux(PA13, PA13_GPD);		//PA13 output
	csi_pin_set_mux(PA14, PA14_GPD);		//PA14 output
	csi_pin_set_mux(PA15, PA15_GPD);		//PA15 output
	csi_pin_set_mux(PB0, PB0_GPD);		//PB0 output
	csi_pin_set_mux(PB1, PB1_GPD);		//PB1 output
	csi_pin_set_mux(PB2, PB2_GPD);		//PB2 output
	csi_pin_set_mux(PB3, PB3_GPD);		//PB3 output
	csi_pin_set_mux(PB4, PB4_GPD);		//PB4 output
	csi_pin_set_mux(PB5, PB5_GPD);		//PB5 output
	csi_pin_set_mux(PB6, PB6_GPD);		//PB6 output
	csi_pin_set_mux(PB7, PB7_GPD);		//PB7 output
	csi_pin_set_mux(PB8, PB8_GPD);		//PB8 output
	csi_pin_set_mux(PB9, PB9_GPD);		//PB9 output
	csi_pin_set_mux(PB10, PB10_GPD);		//PB10 output
	csi_pin_set_mux(PB11, PB11_GPD);		//PB11 output
	csi_pin_set_mux(PB12, PB12_GPD);		//PB12 output
	csi_pin_set_mux(PB13, PB13_GPD);		//PB13 output
	csi_pin_set_mux(PB14, PB14_GPD);		//PB14 output
	csi_pin_set_mux(PB15, PB15_GPD);		//PB15 output
	csi_pin_set_mux(PC0, PC0_GPD);		//PC0 output
	csi_pin_set_mux(PC1, PC1_GPD);		//PC1 output
	csi_pin_set_mux(PC2, PC2_GPD);		//PC2 output
	csi_pin_set_mux(PC3, PC3_GPD);		//PC3 output
	csi_pin_set_mux(PC4, PC4_GPD);		//PC4 output
	csi_pin_set_mux(PC5, PC5_GPD);		//PC5 output;	
	csi_pin_set_mux(PC6, PC6_GPD);		//PC6 output
	csi_pin_set_mux(PC7, PC7_GPD);		//PC7 output
	csi_pin_set_mux(PC8, PC8_GPD);		//PC8 output
	csi_pin_set_mux(PC9, PC9_GPD);		//PC9 output
	csi_pin_set_mux(PC10, PC10_GPD);		//PC10 output
	csi_pin_set_mux(PC11, PC11_GPD);		//PC11 output
	csi_pin_set_mux(PC12, PC12_GPD);		//PC12 output
	csi_pin_set_mux(PC13, PC13_GPD);		//PC13 output
	csi_pin_set_mux(PC14, PC14_GPD);		//PC14 output
	csi_pin_set_mux(PC15, PC15_GPD);		//PC15 output
	csi_pin_set_mux(PD0, PD0_GPD);		//PD0 output
	csi_pin_set_mux(PD1, PD1_GPD);		//PD1 output
	csi_pin_set_mux(PD2, PD2_GPD);		//PD2 output
	csi_pin_set_mux(PD3, PD3_GPD);		//PD3 output
	csi_pin_set_mux(PD4, PD4_GPD);		//PD4 output
	csi_pin_set_mux(PD5, PD5_GPD);		//PD5 output
	
	
	csi_pm_clk_enable(DP_ISOSC, DISABLE);
	csi_pm_enter_sleep(PM_MODE_DEEPSLEEP);
	
	
}