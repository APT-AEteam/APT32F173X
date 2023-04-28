/***********************************************************************//** 
 * \file  mul_div_latency_test.c
 * \brief  乘除法延迟测试代码 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-4-28 <td>V0.0 <td>YT    <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include <csi_drv.h>
#include <iostring.h>
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

volatile uint16_t cnt1;   // 跑乘除法之前时刻的BT3[CNT]
volatile uint16_t cnt2;   // 跑完乘除法时刻的BT3[CNT]
volatile uint16_t cnt3;   // latency, cnt3=cnt2-cnt1

//注意：Optimization、时钟源、sclk、pclk、BT3CLK都对延迟有影响，要对比不同测试代码延时，需要统一这些配置


/** \brief 单精度浮点乘法延迟测试代码 
 *  跑一万次
 * 跑完代码后如果出现BT3 OVF=1的情况，需要调整BT3的寄存器配置，来防止计数溢出的出现
 * 方法1：BT3[PRDR]调大，比如：BT3->PRDR=0XFFff; 
 * 方法2：BT3[PSCR]调大,比如:BT3->PSCR = 0x4;
 * 方法3：SYSCON  PCLKDIV调大，比如：SYSCON->PCLKCR = PCLK_KEY|(PCLK_DIV2<<8);
 *  \param[in] none
 *  \return error code
 */
uint16_t fmul_test(void)
{
	volatile float x1;
	volatile float x2=4.2f;
	volatile float x3=3.3f;
	cnt1=csp_bt_get_cnt(BT3);
	
// 跑一万次
	volatile int32_t i=10000;
    cnt1=csp_bt_get_cnt(BT3);
    while(i>0)
	{
		x1=x2*x3;  	 	     
		i--;
	}
	cnt2=csp_bt_get_cnt(BT3);
	cnt3=cnt2-cnt1;		
	
	return cnt3;
	
}


/** \brief 16*32位整数乘法延迟测试代码 
 *  跑一万次
 * 跑完代码后如果出现BT3 OVF=1的情况，需要调整BT3的寄存器配置，来防止计数溢出的出现
 * 方法1：BT3[PRDR]调大，比如：BT3->PRDR=0XFFff; 
 * 方法2：BT3[PSCR]调大,比如:BT3->PSCR = 0x4;
 * 方法3：SYSCON  PCLKDIV调大，比如：SYSCON->PCLKCR = PCLK_KEY|(PCLK_DIV2<<8);
 *  \param[in] none
 *  \return error code
 */
uint16_t mul16_32_test(void)
{
	volatile int32_t x7;
	volatile int16_t x8=3;
	volatile int32_t x9=4;
	cnt1=csp_bt_get_cnt(BT3);
	
// 跑一万次
	volatile int32_t i=10000;
    cnt1=csp_bt_get_cnt(BT3);
    while(i>0)
	{
		x7=x8*x9;	  	 	     
		i--;
	}
	cnt2=csp_bt_get_cnt(BT3);
	cnt3=cnt2-cnt1;		
	
	return cnt3;
	
}


/** \brief 32*32位整数乘法延迟测试代码 
 *  跑一万次
 * 跑完代码后如果出现BT3 OVF=1的情况，需要调整BT3的寄存器配置，来防止计数溢出的出现
 * 方法1：BT3[PRDR]调大，比如：BT3->PRDR=0XFFff; 
 * 方法2：BT3[PSCR]调大,比如:BT3->PSCR = 0x4;
 * 方法3：SYSCON  PCLKDIV调大，比如：SYSCON->PCLKCR = PCLK_KEY|(PCLK_DIV2<<8);
 *  \param[in] none
 *  \return error code
 */
uint16_t mul32_32_test(void)
{
	volatile int32_t x7;
	volatile int32_t x8=3;
	volatile int32_t x9=4;
	cnt1=csp_bt_get_cnt(BT3);
	
// 跑一万次
	volatile int32_t i=10000;
    cnt1=csp_bt_get_cnt(BT3);
    while(i>0)
	{
		x7=x8*x9;	  	 	     
		i--;
	}
	cnt2=csp_bt_get_cnt(BT3);
	cnt3=cnt2-cnt1;		
	
	return cnt3;
	
}

/** \brief 64*64位整数乘法延迟测试代码 
 *  跑一万次
 * 跑完代码后如果出现BT3 OVF=1的情况，需要调整BT3的寄存器配置，来防止计数溢出的出现
 * 方法1：BT3[PRDR]调大，比如：BT3->PRDR=0XFFff; 
 * 方法2：BT3[PSCR]调大,比如:BT3->PSCR = 0x4;
 * 方法3：SYSCON  PCLKDIV调大，比如：SYSCON->PCLKCR = PCLK_KEY|(PCLK_DIV2<<8);
 *  \param[in] none
 *  \return error code
 */
uint16_t mul64_64_test(void)
{
	volatile int64_t x4;
	volatile int64_t x5=0xf0ffffffffffffff;
	volatile int64_t x6=0xffffffffff000fff;
	cnt1=csp_bt_get_cnt(BT3);
	
// 跑一万次
	volatile int32_t i=10000;
    cnt1=csp_bt_get_cnt(BT3);
    while(i>0)
	{
		x4=x5*x6;	  	 	     
		i--;
	}
	cnt2=csp_bt_get_cnt(BT3);
	cnt3=cnt2-cnt1;		
	
	return cnt3;
	
}

/** \brief 单精度浮点除法延迟测试代码 
 *  跑一万次
 * 跑完代码后如果出现BT3 OVF=1的情况，需要调整BT3的寄存器配置，来防止计数溢出的出现
 * 方法1：BT3[PRDR]调大，比如：BT3->PRDR=0XFFff; 
 * 方法2：BT3[PSCR]调大,比如:BT3->PSCR = 0x4;
 * 方法3：SYSCON  PCLKDIV调大，比如：SYSCON->PCLKCR = PCLK_KEY|(PCLK_DIV2<<8);
 *  \param[in] none
 *  \return error code
 */
uint16_t fdiv_test(void)
{
	volatile float x10;
	volatile float x11=4.2f;
	volatile float x12=1.3f;
	cnt1=csp_bt_get_cnt(BT3);
	
// 跑一万次
	volatile int32_t i=10000;
    cnt1=csp_bt_get_cnt(BT3);
    while(i>0)
	{
		x10=x11/x12; 	  	 	     
		i--;
	}
	cnt2=csp_bt_get_cnt(BT3);
	cnt3=cnt2-cnt1;		
	
	return cnt3;
	
}


/** \brief 16/32位整数除法延迟测试代码 
 *  跑一万次
 * 跑完代码后如果出现BT3 OVF=1的情况，需要调整BT3的寄存器配置，来防止计数溢出的出现
 * 方法1：BT3[PRDR]调大，比如：BT3->PRDR=0XFFff; 
 * 方法2：BT3[PSCR]调大,比如:BT3->PSCR = 0x4;
 * 方法3：SYSCON  PCLKDIV调大，比如：SYSCON->PCLKCR = PCLK_KEY|(PCLK_DIV2<<8);
 *  \param[in] none
 *  \return error code
 */
uint16_t div16_32_test(void)
{
	volatile int32_t x16;
	volatile int16_t x17=3;
	volatile int32_t x18=4;
	cnt1=csp_bt_get_cnt(BT3);
	
// 跑一万次
	volatile int32_t i=10000;
    cnt1=csp_bt_get_cnt(BT3);
    while(i>0)
	{
		x16=x17/x18;  	 	     
		i--;
	}
	cnt2=csp_bt_get_cnt(BT3);
	cnt3=cnt2-cnt1;		
	
	return cnt3;
	
}

/** \brief 32/32位整数除法延迟测试代码 
 *  跑一万次
 * 跑完代码后如果出现BT3 OVF=1的情况，需要调整BT3的寄存器配置，来防止计数溢出的出现
 * 方法1：BT3[PRDR]调大，比如：BT3->PRDR=0XFFff; 
 * 方法2：BT3[PSCR]调大,比如:BT3->PSCR = 0x4;
 * 方法3：SYSCON  PCLKDIV调大，比如：SYSCON->PCLKCR = PCLK_KEY|(PCLK_DIV2<<8);
 *  \param[in] none
 *  \return error code
 */
uint16_t div32_32_test(void)
{
	volatile int32_t x16;
	volatile int32_t x17=3;
	volatile int32_t x18=4;
	cnt1=csp_bt_get_cnt(BT3);
	
// 跑一万次
	volatile int32_t i=10000;
    cnt1=csp_bt_get_cnt(BT3);
    while(i>0)
	{
		x16=x17/x18;  	 	     
		i--;
	}
	cnt2=csp_bt_get_cnt(BT3);
	cnt3=cnt2-cnt1;		
	
	return cnt3;
	
}

