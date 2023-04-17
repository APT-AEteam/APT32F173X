/***********************************************************************//** 
 * \file  syscon_test.c
 * \brief  SYSCON_TEST description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-3-21 <td>V1.0 <td>YYM     <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include "reliability.h"
#include "pin.h"
#include "sys_clk.h"
#include <drv/iwdt.h>
#include <drv/lpt.h>
#include <gpta.h>
#include "drv/etb.h"
#include <cmp.h>
#include <gpta.h>
#include <bt.h>
#include <ifc.h>
#include <irq.h>
#include <iostring.h>
#include "syscon_test.h"
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

//================================================================================== 
//
//NMI中断测试
//nmi中断测试，中断后，会进入NMI_Handler处理函数中
//

void syscon_nmi_int_test(void)
{
	csp_nmi_int_enable(SYSCON, NMI_EXI0_INT);
	
	csi_pin_set_mux(PA0, PA0_INPUT);                          //PB02 配置为输入
	csi_pin_pull_mode(PA0, GPIO_PULLUP);                       //PB02 上拉
	csi_pin_irq_mode(PA0, EXI_GRP0, GPIO_IRQ_FALLING_EDGE);    //PB02 下降沿产生中断，选择中断组2
	csi_pin_irq_enable(PA0, ENABLE);                           //PB02 中断使能        
	csi_pin_vic_irq_enable(EXI_GRP0, ENABLE);                   //VIC中断使能，选择中断组2
	__disable_excp_irq();                                     //在enable的情况下，也会进入NMI处理函数

}

//================================================================================== 
//
//闪灯程序，可进行EFT状态显示测试等
//
//
//==================================================================================
void syscon_eft_led_test(void)
{
	csi_pin_set_mux(PA6, PA6_OUTPUT);		//PA06 output
	csi_pin_set_high(PA6);					//PA06 output high;
	
	csi_pin_set_mux(PA1, PA1_OUTPUT);		//PA06 output
	csi_pin_set_low(PA1);					//PA06 output high;
	
	csi_pin_set_mux(PA2, PA2_OUTPUT);		//PA06 output
	csi_pin_set_high(PA2);					//PA06 output high;	
	while(1)
	{
		mdelay(200);						//delay 100ms
		csi_pin_toggle(PA1);				//PA06 toggle
		csi_pin_toggle(PA2);				//PA06 toggle				
	}		
}

//================================================================================== 
//
//FVR测试,BUF 1v输出
//
//==================================================================================
void syscon_fvr_test(void)
{
	csi_pin_set_mux(PB8, PB8_BUF);
	csi_clk_enable((uint32_t *)FVR);	
	*(unsigned int *)(0x40032000) = 0x5a110000;
}

//================================================================================== 
//
//在测试的过程中，系统时钟选择EM clk,{SRC_EMOSC, EMOSC_VALUE, SCLK_DIV1, PCLK_DIV1, 5556000, 5556000};
//EM时钟测试，失效后，会进行复位，程序指示灯不闪烁
//
//==================================================================================
void syscon_emosc_rst_test(void)
{
	SYSCON->GCER |= (1<< 18);
	SYSCON->GCER |= (1<< 19);    //使能EM失效后，进行复位	
	SYSCON->OPT1 |= (1<< 21);   //EMCKM的检测时间间隔设置，检测周期基于当前IMOSC的频率设置,1h：14个周期
	syscon_eft_led_test();
}
//================================================================================== 
//
//在测试的过程中，系统时钟选择EM clk,{SRC_EMOSC, EMOSC_VALUE, SCLK_DIV1, PCLK_DIV1, 5556000, 5556000};
//EM时钟测试。失效后，会切换到IM时钟，程序指示灯还在闪烁
//
//==================================================================================
void syscon_emosc_audo_imosc_test(void)
{
	SYSCON->GCER |= (1<< 18);
	SYSCON->GCDR |= (1<< 19);  //禁止EM失效后，进行复位，会切换到IM时钟   
	syscon_eft_led_test();
}
//================================================================================== 
//
//在测试的过程中，系统时钟选择EM clk,{SRC_EMOSC, EMOSC_VALUE, SCLK_DIV1, PCLK_DIV1, 5556000, 5556000};
//EM时钟测试，失效中断测试，EM失效后，会进入中断函数syscon_int_handler
//
//==================================================================================
void syscon_emosc_cmk_int_test(void)
{
	SYSCON->GCER |= (1<< 18);
	SYSCON->GCDR |= (1<< 19);  //禁止EM失效后，进行复位，会切换到IM时钟   
	csp_syscon_int_enable(SYSCON, EMFAIL_INT, ENABLE);	
	csi_vic_enable_irq(SYSCON_IRQ_NUM);
	syscon_eft_led_test();	
}

//================================================================================== 
//
//osc5005时钟测试
//时钟选择{SRC_HFOSC, HFOSC_48M_VALUE, SCLK_DIV4, PCLK_DIV1, 5556000, 5556000};
//切换到osc5005时钟，为HFOSC时钟的1/2，即24M
//==================================================================================

void syscon_osc5005_trim_test(void)
{
	*(unsigned int *)(0x40011000+0x1FC) = 0x00006996;	
	//*(unsigned int *)0x40011278         = 0x05025047;	  //0x5000000为切换到osc5005模式，为HFOSC时钟的1/2，0x47为trim值 	 TSEL=b'10
	*(unsigned int *)0x40011278         = 0x05035047;	  //0x5000000为切换到osc5005模式，为HFOSC时钟的1/2，0x47为trim值 	 TSEL=b'11
	csi_clo_config(CLO_HFCLK, CLO_DIV4, PA2);             
	
}
 //================================================================================== 
//
//EXTRST 外部复位管脚触发的硬件 RESET 信号（低电平有效）。此复位只有在外部复位脚有效时可用（通过User Option配置）。
//
//代码下载完成后，会存在一直复位的现象，此时将PD5拉高，芯片将不会一直复位
//测试过程中，RESET脚拉低，指示IO口不翻转，一直进行复位，RESET脚拉高，指示IO口翻转
//==================================================================================
void syscon_rst_test(void)
{
	csi_ifc_wr_useroption(IFC, 0x05);
}

 //================================================================================== 
//
//pll失锁测试
//PLL失锁产生的系统复位
//使用EM时钟为PLL源时钟，测试过程中，拔掉EM时钟，会产生PLL失锁系统复位
//
//==================================================================================
void syscon_pllunlock_test(void)
{
	csi_pin_set_mux(PD0, PD0_XIN);
	csi_pin_set_mux(PD1, PD1_XOUT);
	
//	SYSCON->OSTR |=  (1<<EM_FLTSEL_POS);      //外部振荡器滤波范围选择，把SYSCON_OSTR的第26，27位设置成0x01（10ns）
//	SYSCON->OSTR |=  (1<<EM_FLTEN_POS);      //1：需要先打开滤波然后使能外部时钟，即SYSCON_OSTR寄存器的第25位使能

	csi_emosc_enable(EMOSC_VALUE); //EMOSC_VALUE
	
	IFC->MR |= HIGH_SPEED | PF_WAIT7;

	csp_pll_clk_sel(SYSCON, PLL_CLK_SEL_EMOSC);
	csp_pll_set_div_m(SYSCON, 3);
	csp_pll_set_nul(SYSCON, 35);
	csp_pll_set_ckp_div(SYSCON, 1);
	SYSCON->PLLCR |=  (1<<15);                    //PLL 失锁复位控制位 1h：在PLL失锁时，产生系统复位。
	csp_pll_clk_enable(SYSCON, ENABLE);
	csi_pll_enable();
}

//================================================================================== 
//
//sram测试
//
//==================================================================================
//link部分修改
/*
MEMORY
{
	ROM(RX)   : ORIGIN = 0x00000000,  LENGTH = 128K
	RAM1(RWX)  : ORIGIN = 0x20006000,  LENGTH = 8K	
	RAM(RWX)  : ORIGIN = 0x20000000,  LENGTH = 24K
	ROM1(RX)  : ORIGIN = 0x40000,  LENGTH = 8K	
}

    .func :
   {
	   . = ALIGN(0x4) ;	
        *(func)
		. = ALIGN(0x200) ;	
    } >ROM1 AT>RAM1

*/

__attribute__((section("func")))void sram_isram_testing(void)
{
	while (1)
	{
	    NOP;
	    NOP;
	    mdelay(100);
		csi_clo_config(CLO_HCLK, CLO_DIV2, PA2);
		csi_pin_set_mux(PA6, PA6_OUTPUT);		//PA06 output
		csi_pin_set_high(PA6);					//PA06 output high;
		NOP;
		NOP;
	}	
}
void syscon_sram_test(void)
{
//	csp_sram1_func_ctrl(SYSCON,SRAM1_ISRAM);
	sram_isram_testing();
}

//================================================================================== 
//
//系统复位测试
//注意添加相应的延迟,以防下次上电CDK连接困难的问题
//
//==================================================================================
void syscon_reset_test(void)
{
	mdelay(1000);
	mdelay(1000);
	mdelay(1000);
	csp_set_swrst(SYSCON, SYS_SWRST);
}

//================================================================================== 
//
//系统复位测试，跳转到4K对齐地址
//注意添加相应的延迟,以防下次上电CDK连接困难的问题
//系统复位后，跳转到0x4000地址
//==================================================================================
void syscon_reset_srb_test(void)
{
	mdelay(1000);
	mdelay(1000);
	mdelay(1000);
	SYSCON->BRPKEY =  0x9559; 
	SYSCON->SRBREG =  1;            //复位地址跳转到0x(SBR<<12)，只支持4K对齐
	csp_set_swrst(SYSCON, SYS_SWRST);
}

//================================================================================== 
//
//低频率输出测试，需要加延迟，防止下次CDK连接困难
//
//
//==================================================================================
void syscon_fre_testing(void)
{
	mdelay(1000);
	mdelay(1000);
	//csi_clo_config(CLO_HFCLK, CLO_DIV4, PB3);	
	csi_clo_config(CLO_IMCLK, CLO_DIV1, PB3);	
	//csi_clo_config(CLO_ISCLK, CLO_DIV1, PB3);	
	//csi_clo_config(CLO_PLL_PCLK, CLO_DIV16, PB3);	
	
}

//================================================================================== 
//
//LVD测试 调试模式下，中断不是很好测试，可在中断函数中，加入电平变化函数
//
//==================================================================================

void syscon_lvd_test(void)
{
	csi_lvd_int_enable(LVD_INTF,LVD_21);  //VDD掉电到3.0V即触发LVD中断
	
	while(1)
	{
		if(csi_lvd_flag())
		{
			csi_pin_set_high(PA6);	
		}
		else
		{
			csi_pin_set_low(PA6);	
		}
		
	}	
}

//================================================================================== 
//
//LVR测试 
//
//==================================================================================
void syscon_lvr_test(void)
{
	csi_lvr_enable(LVR_31);      //VDD掉电到3.1V，芯片复位 
	
	while(1)
	{
		mdelay(100);
		csi_pin_toggle(PA6);	
	}	
}

//================================================================================== 
//
//power测试，测试的过程中关掉LVD
//
//==================================================================================
void syscon_lvd_power_test()
{
	csi_lvd_disable();	
}

//================================================================================== 
//
//时钟中断测试
//
//==================================================================================

void syscon_clk_int_test(cclk_src_int_e eSrc)
{
	switch(eSrc)
	{
		case (SRC_IMOSC_INT): 
			csp_syscon_int_enable(SYSCON, IMOSC_ST_INT, ENABLE);	
			csi_vic_enable_irq(SYSCON_IRQ_NUM);	
			csp_syscon_int_enable(SYSCON, HFOSC_ST_INT, ENABLE);	
			csi_vic_enable_irq(SYSCON_IRQ_NUM);	
			csi_imosc_enable(0);
		break;
		case (SRC_EMOSC_INT): 
			csp_syscon_int_enable(SYSCON, EMOSC_ST_INT, ENABLE);	
			csi_vic_enable_irq(SYSCON_IRQ_NUM);	
			csi_pin_set_mux(PD0, PD0_XIN);
			csi_pin_set_mux(PD1, PD1_XOUT);
			csi_emosc_enable(24000000);
		break;
		case (SRC_HFOSC_INT): 
			csp_syscon_int_enable(SYSCON, HFOSC_ST_INT, ENABLE);	
			csi_vic_enable_irq(SYSCON_IRQ_NUM);	
			csi_hfosc_enable(3);
		break;	
		case (SRC_PLL_INT): 
			csp_syscon_int_enable(SYSCON, PLL_ST_INT, ENABLE);	
			csi_vic_enable_irq(SYSCON_IRQ_NUM);	
			
			csi_hfosc_enable(1);           // HFOSC_24M_VALUE
			IFC->MR |= HIGH_SPEED | PF_WAIT7;
			
			csp_pll_clk_sel(SYSCON, PLL_CLK_SEL_HFOSC);
			csp_pll_set_div_m(SYSCON, 3);
			csp_pll_set_nul(SYSCON, 35);
			csp_pll_set_ckp_div(SYSCON, 1);
			csp_pll_clk_enable(SYSCON, ENABLE);
			csi_pll_enable();
		break;
		case (SRC_ISOSC_INT): 
			csp_syscon_int_enable(SYSCON, ISOSC_ST_INT, ENABLE);	
			csi_vic_enable_irq(SYSCON_IRQ_NUM);	
			
			csi_isosc_enable();
		break;
		case (SRC_ESOSC_INT):
			csp_syscon_int_enable(SYSCON, ESOSC_ST_INT, ENABLE);	
			csi_vic_enable_irq(SYSCON_IRQ_NUM);	
			
 			csi_pin_set_mux(PC14, PC14_SXIN);
			csi_pin_set_mux(PC15, PC15_SXOUT);   // Config pins before use ESOSC 
			csi_esosc_enable(32768); 
		break;	
		case (SRC_SYCOSC_INT): 
			csp_syscon_int_enable(SYSCON, SYSTICK_ST_INT, ENABLE);	
			csi_vic_enable_irq(SYSCON_IRQ_NUM);	
			csi_hfosc_enable(3);
			csp_set_clksrc(SYSCON, 2);
		break;	
	}
}

//================================================================================== 
//
//EM 时钟滤波功能测试
//将其他时钟切换到IM时钟，如果EM时间打开，先关掉，然后配置EM时钟，最后切换EM时钟为SCLK。
//==================================================================================

void syscon_em_fiten_test(void)
{
	
	csp_set_clksrc(SYSCON, SRC_IMOSC);
	
	if(csp_get_gcsr(SYSCON) & EMOSC)
	csi_emosc_disable();
	
	SYSCON->OSTR |=  (1<<EM_FLTSEL_POS);      //外部振荡器滤波范围选择，把SYSCON_OSTR的第26，27位设置成0x01（10ns）
	SYSCON->OSTR |=  (1<<EM_FLTEN_POS);      //1：需要先打开滤波然后使能外部时钟，即SYSCON_OSTR寄存器的第25位使能	
	csi_pin_set_mux(PD0, PD0_XIN);
	csi_pin_set_mux(PD1, PD1_XOUT);
	csi_emosc_enable(24000000);	
	
	csp_set_sdiv(SYSCON, 1);             //SCLK 1分频
	csp_set_clksrc(SYSCON, SRC_EMOSC);
	
	csp_eflash_lpmd_enable(SYSCON, 0);   
	
	csp_set_pdiv(SYSCON, 1);	            //PCLK 1分频
		
}

//================================================================================== 
//
//iwdt 中断测试
//在中断中加入IO口翻转功能，可以测量出报警中断的时间
//==================================================================================

void syscon_iwdt_irq_test(void)
{

	csi_iwdt_init(IWDT_TO_1024);					//初始化看门狗，溢出时间为1000ms(系统复位时间)
	csi_iwdt_irq_enable(IWDT_ALARMTO_2_8, ENABLE);	//使能看门狗报警中断，报警时间为4/8溢出时间
	csi_iwdt_open();								//打开看门狗

}

//================================================================================== 
//
//deepsleep测试
//通过LPT唤醒DEEPSLEEP模式
//
//==================================================================================

void syscon_lpt_wakeup_deepsleep_test(void)
{
	
//	SYSCON->PWRKEY = 0xA67A6CC7;
//	SYSCON->PWRCR = (SYSCON->PWRCR & (~(0x0f<<24))) | 0x4000002;

	csi_cmos_autotrim();
	csi_pm_mode_e ePmMode = PM_MODE_DEEPSLEEP;		  //PM_MODE_DEEPSLEEP  PM_MODE_SLEEP
	
	csi_pin_set_mux(PA5,PA5_OUTPUT);				//PA05 OUTPUT
	mdelay(1000);
	
	csi_pin_toggle(PA5);
	delay_ums(200);
	csi_pin_toggle(PA5);
	delay_ums(200);
	csi_pin_toggle(PA5);
	delay_ums(200);
	csi_pin_toggle(PA5);
	delay_ums(200);
	csi_pin_toggle(PA5);
	delay_ums(200);
	csi_pin_toggle(PA5);
	delay_ums(200);
	csi_pin_toggle(PA5);
	delay_ums(200);
	csi_pin_toggle(PA5);
	delay_ums(200);

#ifdef CONFIG_USER_PM	
	csi_pm_attach_callback(ePmMode, prepare_lp, wkup_lp);	//需要在工程设置compiler tab下加入define CONFIG_USER_PM=1;
#endif
//	mdelay(1000);
//	mdelay(1000);
	csi_pm_config_wakeup_source(WKUP_LPT, ENABLE);			//配置唤醒源
//	csi_pm_clk_enable(DP_ISOSC, ENABLE);					//SNOOZE模式下时钟开启/关闭
	csi_pm_clk_enable(DP_IMOSC, ENABLE);
//	csi_pm_clk_enable(DP_ESOSC, ENABLE);
//	csi_pm_clk_enable(DP_EMOSC, ENABLE);

	//lpt初始化配置
	csi_lpt_timer_init(LPT,LPT_CLK_ISCLK, 500);       		//初始化lpt,选用内部超低功耗时钟,定时500ms,默认采用PEND中断
	csi_lpt_start(LPT);	
	delay_ums(200);

	switch(ePmMode)
	{
		case PM_MODE_SLEEP:
	//		my_printf("Enter Sleep Mode\n");
			break;
		case PM_MODE_DEEPSLEEP:
	//		my_printf("Enter Deep-Sleep mode\n");
			break;
		default:
			break;
	}
	
	while(1) 
	{
		csi_pin_set_high(PA5);
		csi_pm_enter_sleep(ePmMode);
	//	my_printf("Wakeup From Deep-Sleep Mode...\n");
		csi_pin_set_low(PA5);
	//	mdelay(100);	
	}
}

//================================================================================== 
//
//sleep测试
//通过LPT唤醒SLEEP模式
//需要关掉BT3
//==================================================================================

void syscon_lpt_wakeup_sleep_test(void)
{
	
	SYSCON->PWRKEY = 0xA67A6CC7;
	SYSCON->PWRCR = (SYSCON->PWRCR & (~(0x0f<<16))) | 0x40002;

//	csp_sleep_vos_config(SYSCON, BGR_1V,VCREF_12);
//	csp_sleep_vos_config_enable(SYSCON);
//	csp_sleep_vos_enable(SYSCON, ENABLE);
	
	csi_pm_mode_e ePmMode = PM_MODE_SLEEP;		  //PM_MODE_DEEPSLEEP  PM_MODE_SLEEP
	
	csi_pin_set_mux(PA5,PA5_OUTPUT);				//PA05 OUTPUT
//	mdelay(2000);
	
	csi_pin_toggle(PA5);
	delay_ums(200);
	csi_pin_toggle(PA5);
	delay_ums(200);
	csi_pin_toggle(PA5);
	delay_ums(200);
	csi_pin_toggle(PA5);
	delay_ums(200);
	csi_pin_toggle(PA5);
	delay_ums(200);
	csi_pin_toggle(PA5);
	delay_ums(200);
	csi_pin_toggle(PA5);
	delay_ums(200);
	csi_pin_toggle(PA5);
	delay_ums(200);

#ifdef CONFIG_USER_PM	
	csi_pm_attach_callback(ePmMode, prepare_lp, wkup_lp);	//需要在工程设置compiler tab下加入define CONFIG_USER_PM=1;
#endif
	mdelay(1000);
	mdelay(1000);
	csi_pm_config_wakeup_source(WKUP_LPT, ENABLE);			//配置唤醒源
//	csi_pm_clk_enable(DP_ISOSC, ENABLE);					//SNOOZE模式下时钟开启/关闭
//	csi_pm_clk_enable(DP_IMOSC, ENABLE);
//	csi_pm_clk_enable(DP_ESOSC, ENABLE);
//	csi_pm_clk_enable(DP_EMOSC, ENABLE);

//	csi_pm_clk_enable(SP_IDLE_PCLK, DISABLE);					//sleep模式下关闭PCLK
//	csi_pm_clk_enable(SP_IDLE_HCLK, DISABLE);					//sleep模式下关闭HCLK
	
	//lpt初始化配置
	csi_lpt_timer_init(LPT,LPT_CLK_ISCLK, 50);       		//初始化lpt,选用内部超低功耗时钟,定时500ms,默认采用PEND中断
	csi_lpt_start(LPT);	
	delay_ums(200);
	csi_bt_stop(BT3);
	switch(ePmMode)
	{
		case PM_MODE_SLEEP:
	//		my_printf("Enter Sleep Mode\n");
			break;
		case PM_MODE_DEEPSLEEP:
	//		my_printf("Enter Deep-Sleep mode\n");
			break;
		default:
			break;
	}
	
	while(1) 
	{
		//csi_bt_stop(BT3);
		csi_pin_set_high(PA5);
		csi_pm_enter_sleep(ePmMode);
		//csi_bt_start(BT3);
		//csi_irq_enable(BT3);
	//	my_printf("Wakeup From Deep-Sleep Mode...\n");
		csi_pin_set_low(PA5);
		nop;nop;nop;nop;nop;nop;
		//mdelay(100);	
	}
}

/** \brief 通过LPT唤醒DEEPSLEEP模式
 * 
 *  \param  none
 *  \return none
 */
void syscon_io_wakeup_sleep_test(void)
{
	csi_pm_mode_e ePmMode = PM_MODE_SLEEP;			//PM_MODE_SLEEP/PM_MODE_DEEPSLEEP/PM_MODE_SNOOZE/PM_MODE_SHUTDOWN
	uint16_t hwRstSrc = csi_get_rst_reason();
	
	if(hwRstSrc)									//获取并打印复位信息
	{
		my_printf("System Reset Source = 0x%x \n", hwRstSrc);
		csi_clr_rst_reason(hwRstSrc);				//清除复位信息
	}
	
	csi_pin_set_mux(PB2,PB2_OUTPUT);				//PB02 OUTPUT
	
	csi_pin_toggle(PB2);
	mdelay(200);
	csi_pin_toggle(PB2);
	mdelay(200);
	csi_pin_toggle(PB2);
	mdelay(200);
	csi_pin_toggle(PB2);
	mdelay(200);
	csi_pin_toggle(PB2);
	mdelay(200);
	csi_pin_toggle(PB2);
	mdelay(200);
	csi_pin_toggle(PB2);
	mdelay(200);
	csi_pin_toggle(PB2);
	mdelay(200);
	
#ifdef CONFIG_USER_PM	
	csi_pm_attach_callback(ePmMode, prepare_lp, wkup_lp);	//需要在工程设置compiler tab下加入define CONFIG_USER_PM=1;
#endif

	csi_pin_set_mux(PB1,PB1_INPUT);								//PB1 输入							
	csi_pin_pull_mode(PB1, GPIO_PULLUP);						//PB1 上拉
	csi_pin_irq_mode(PB1,EXI_GRP1, GPIO_IRQ_FALLING_EDGE);		//PB1 下降沿产生中断
	csi_pin_irq_enable(PB1, ENABLE);							//PB1 中断使能，选择中断组1
	csi_pin_vic_irq_enable(EXI_GRP1, ENABLE);					//PB1 VIC中断使能，选择中断组1	
	csi_vic_get_pending_irq(EXI1_IRQ_NUM);

	csi_pm_clk_enable(SP_IDLE_PCLK, DISABLE);					//sleep模式下关闭PCLK
	csi_pm_clk_enable(SP_IDLE_HCLK, DISABLE);					//sleep模式下关闭HCLK
//	csi_pm_clk_enable(DP_ISOSC, ENABLE);
//	csi_pm_clk_enable(DP_IMOSC, ENABLE);
//	csi_pm_clk_enable(DP_ESOSC, ENABLE);
//	csi_pm_clk_enable(DP_EMOSC, ENABLE);
	
	mdelay(10);
	csi_bt_stop(BT3);
	switch(ePmMode)
	{
		case PM_MODE_SLEEP:
			//my_printf("Enter Sleep Mode\n");
			break;
		case PM_MODE_DEEPSLEEP:
			//my_printf("Enter Deep-Sleep mode\n");
			break;
		default:
			break;
	}
	
	while(1) 
	{
		csi_pin_set_high(PB2);
		
		csi_pm_enter_sleep(ePmMode);
		//csi_iwdt_feed();
		//mdelay(100);
		csi_pin_set_low(PB2);
		
		//mdelay(100);
		//my_printf("Wakeup From Sleep Mode...\n");
	}
}


//================================================================================== 
//
//时钟切换测试
//
//==================================================================================
int syscon_low_temp_mclk_switch_test(void)
{
	
	uint16_t hwCnt = 0;
	my_printf("Enter Main Clk Switch...\n");
	my_printf("Repeat Count is = %d \n", hwCnt);
	
	while(hwCnt < 1000)
	{
		//EM SWITCH IM
		tClkConfig.eClkSrc = SRC_IMOSC;
		tClkConfig.wFreq = IMOSC_4M_VALUE;
		tClkConfig.eSdiv = SCLK_DIV1;
		tClkConfig.ePdiv = PCLK_DIV1;
		csi_sysclk_config(tClkConfig);
		mdelay(40);
		
		//IM SWITCH HF
		tClkConfig.eClkSrc = SRC_HFOSC;
		tClkConfig.wFreq = HFOSC_24M_VALUE;
		tClkConfig.eSdiv = SCLK_DIV1;
		tClkConfig.ePdiv = PCLK_DIV1;
		csi_sysclk_config(tClkConfig);
		mdelay(40);
		
		//HF SWITCH IM
		tClkConfig.eClkSrc = SRC_IMOSC;
		tClkConfig.wFreq = IMOSC_5M_VALUE;
		tClkConfig.eSdiv = SCLK_DIV1;
		tClkConfig.ePdiv = PCLK_DIV1;
		csi_sysclk_config(tClkConfig);
		mdelay(40);
		
		//IM SWITCH HF
		tClkConfig.eClkSrc = SRC_HFOSC;
		tClkConfig.wFreq = HFOSC_24M_VALUE;
		tClkConfig.eSdiv = SCLK_DIV1;
		tClkConfig.ePdiv = PCLK_DIV1;
		csi_sysclk_config(tClkConfig);
		mdelay(40);
		
		//HF SWITCH IM
		tClkConfig.eClkSrc = SRC_IMOSC;
		tClkConfig.wFreq = IMOSC_4M_VALUE;
		tClkConfig.eSdiv = SCLK_DIV1;
		tClkConfig.ePdiv = PCLK_DIV1;
		csi_sysclk_config(tClkConfig);
		mdelay(40);
		
	#if 1	
		//IM SWITCH EM
		csi_pin_set_mux(PD0, PD0_XIN);
		csi_pin_set_mux(PD1, PD1_XOUT);
		
		tClkConfig.eClkSrc = SRC_EMOSC;
		tClkConfig.wFreq = 240000000;
		tClkConfig.eSdiv = SCLK_DIV1;
		tClkConfig.ePdiv = PCLK_DIV1;
		csi_sysclk_config(tClkConfig);
		mdelay(40);
		
		//EM SWITCH IM
		tClkConfig.eClkSrc = SRC_IMOSC;
		tClkConfig.wFreq = IMOSC_4M_VALUE;
		tClkConfig.eSdiv = SCLK_DIV1;
		tClkConfig.ePdiv = PCLK_DIV1;
		csi_sysclk_config(tClkConfig);
		mdelay(40);
	#endif
	
		//IM SWITCH HF
		tClkConfig.eClkSrc = SRC_HFOSC;
		tClkConfig.wFreq = HFOSC_24M_VALUE;
		tClkConfig.eSdiv = SCLK_DIV1;
		tClkConfig.ePdiv = PCLK_DIV1;
		csi_sysclk_config(tClkConfig);
		mdelay(40);
		
		//HF SWITCH IM
		tClkConfig.eClkSrc = SRC_IMOSC;
		tClkConfig.wFreq = IMOSC_4M_VALUE;
		tClkConfig.eSdiv = SCLK_DIV1;
		tClkConfig.ePdiv = PCLK_DIV1;
		csi_sysclk_config(tClkConfig);
		mdelay(40);
		
		//IM SWITCH EM
		csi_pin_set_mux(PD0, PD0_XIN);
		csi_pin_set_mux(PD1, PD1_XOUT);
		
		tClkConfig.eClkSrc = SRC_EMOSC;
		tClkConfig.wFreq = 240000000;
		tClkConfig.eSdiv = SCLK_DIV1;
		tClkConfig.ePdiv = PCLK_DIV1;
		csi_sysclk_config(tClkConfig);
		mdelay(40);		
		
		hwCnt++; 
		my_printf("Repeat Count is = %d \n", hwCnt);
		mdelay(2);	
		
	}
	
	return 0;
}

//================================================================================== 
//
//低温下deepsleep 1000次测试
//打开看门狗,记得在syscon_irqhandler函数中（在reliability.c中）喂狗
//
//==================================================================================
void syscon_deepsleep_lowtemp_test(void)
{ 
	 uint32_t wTimes = 0;
	 uint8_t  byFlag = 0;
	 
	 mdelay(1000);
	 
	 csi_pin_set_mux(PA6,PA6_OUTPUT);
	 csi_pin_set_high(PA6);
	 
	 SYSCON -> GCER = (0x01 << IMOSC_STP);
	 csi_iwdt_init(IWDT_TO_4096);     //初始化看门狗，溢出时间为1000ms(系统复位时间)
	 csi_iwdt_irq_enable(IWDT_ALARMTO_2_8, ENABLE); //使能看门狗报警中断，报警时间为4/8溢出时间
	 csi_iwdt_open();        //打开看门狗
	 
	 csi_pm_config_wakeup_source(WKUP_IWDT, ENABLE);
	 
	 while(1) 
	 { 
	  my_printf("deepsleep\n");
	  mdelay(1);
	  csi_pm_enter_sleep(PM_MODE_DEEPSLEEP);
	  wTimes=wTimes+1;
	  my_printf("wakeup:%d\n\n",wTimes);
	  csi_pin_toggle(PA6);
	  
	  if(1000 <= wTimes)
	  {
	   byFlag = 1;
	   break;
	  }
	 }
	 my_printf("deepsleep test_flag=%d (0:fail   1:successful)  times=%d",byFlag,wTimes);
	 while(1)
	 {
	  mdelay(100);
	  csi_pin_toggle(PA6);
	 }
}

//================================================================================== 
//
//硬件触发，需要触发三次，  TRGEV1CNT = 2 测试寄存器 SYSCON_EVTRG  SYSCON_EVPS
//EXI1 		 --> BT_SYNC0 
//EV1TRG  事件触发输出Event1 触发的中断。
//测试寄存器： SYSCON_IMER SYSCON_IMDR SYSCON_IMCR  SYSCON_ICR SYSCON_RISR SYSCON_MISR SYSCON_RSR
//SYSCON_IAR 对应位写入1，即可触发相应的中断
//==================================================================================
int syscon_evtrg_int_test(void)
{
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				                //ETB 参数配置结构体	
	
	csi_pin_set_mux(PA1,PA1_INPUT);		
	csi_pin_pull_mode(PA1, GPIO_PULLUP);						//PA01 上拉
	csi_pin_irq_mode(PA1,EXI_GRP1, GPIO_IRQ_FALLING_EDGE);		//PA01 下降沿产生中断	
	csi_pin_irq_enable(PA1, ENABLE);
	csi_exi_set_evtrg(1, TRGSRC_EXI1, 3);                        //三次产生一次触发

	csp_syscon_int_enable(SYSCON, EV1TRG_INT, ENABLE);	
	csi_vic_enable_irq(SYSCON_IRQ_NUM);		

	csi_bt_start_sync(BT0, 200);
	csi_bt_set_sync(BT0,BT_TRGIN_SYNCEN0, BT_TRG_ONCE, ENABLE);  
	
	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  		//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_EXI_TRGOUT1 ;  	    //EXI1 触发输出0作为触发源
	tEtbConfig.bySrcIp1 = ETB_SRC_NOT_USE;          //无触发源2      
	tEtbConfig.bySrcIp2 = ETB_SRC_NOT_USE;          //无触发源3
	tEtbConfig.byDstIp =  ETB_BT0_SYNCIN0;   	    //BT0 同步输入作为目标事件
	tEtbConfig.byDstIp1 = ETB_DST_NOT_USE;          //无目标事件2
	tEtbConfig.byDstIp2 = ETB_DST_NOT_USE;          //无目标事件3
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
   
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
	return iRet;
}

//================================================================================== 
//
//软件产生一次触发 
//
//测试寄存器：SYSCON_EVTRG SYSCON_EVSWF
//
//==================================================================================
int syscon_evswf_test(void)
{
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				                //ETB 参数配置结构体	
	
	csi_exi_set_evtrg(1, TRGSRC_EXI1, 1);
	
	csi_bt_start_sync(BT0, 200);
	csi_bt_set_sync(BT0,BT_TRGIN_SYNCEN0, BT_TRG_ONCE, ENABLE);  
	
	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  		//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_EXI_TRGOUT1 ;  	    //EXI1 触发输出0作为触发源
	tEtbConfig.bySrcIp1 = ETB_SRC_NOT_USE;          //无触发源2      
	tEtbConfig.bySrcIp2 = ETB_SRC_NOT_USE;          //无触发源3
	tEtbConfig.byDstIp =  ETB_BT0_SYNCIN0;   	    //BT0 同步输入作为目标事件
	tEtbConfig.byDstIp1 = ETB_DST_NOT_USE;          //无目标事件2
	tEtbConfig.byDstIp2 = ETB_DST_NOT_USE;          //无目标事件3
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
   
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
	SYSCON -> EVSWF |= 0x1 << 1; 	
	
	return iRet;
}

