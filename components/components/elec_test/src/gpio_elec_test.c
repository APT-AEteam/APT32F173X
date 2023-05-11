/***********************************************************************//** 
 * \file  gpio_elec_test.c
 * \brief  GPIO电气参数测试代码
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date      <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-4-10 <td>V0.0     <td>WCH     <td>initial
 * </table>
 * *********************************************************************
*/

/* include ----------------------------------------------------------------*/
#include <drv/gpio.h>
#include <drv/gptb.h>
#include "drv/etb.h"
#include "pin.h"
#include "demo.h"

/** \brief GPIO VIH/VIL测试，非TTL模式
 *	VIL：
 *  1、VDD设置为5V,将目标IO设为输入。
 *  2、目标IO接可调电源，调节可调电源输出，记录可以使指示IO稳定输出低的最高电平，即为VIL。
 *	VIH：
 *  1、VDD设置为5V,将目标IO设为输入。
 *  2、目标IO接可调电源，调节可调电源输出，记录可以使指示IO稳定输出高的最低电平，即为VIH。
 *  \param[in] none
 *  \return none
 */
void gpio_vih_vil_test_nottl_mode(void)
{
	csi_pin_set_mux(PD2,PD2_INPUT);   //目标IO
	csi_pin_set_mux(PA1,PA1_OUTPUT);  //指示IO		
	
	while(1)
	{
		if(csi_pin_read(PD2))               
			csi_pin_set_high(PA1);    
		else                                 
			csi_pin_set_low(PA1);
	}
}

/** \brief GPIO VIH/VIL测试，TTL1模式
 *	VIL：
 *  1、VDD设置为5V,将目标IO设为输入，并设置为TTL1模式。
 *  2、目标IO接可调电源，调节可调电源输出，记录可以使指示IO稳定输出低的最高电平，即为VIL。
 *	VIH：
 *  1、VDD设置为5V,将目标IO设为输入，并设置为TTL1模式。
 *  2、目标IO接可调电源，调节可调电源输出，记录可以使指示IO稳定输出高的最低电平，即为VIH。
 *  \param[in] none
 *  \return none
 */
void gpio_vih_vil_test_ttl1_mode(void)
{
	csi_pin_set_mux(PB6,PB6_INPUT);			 //目标IO1
	csi_pin_input_mode(PB6,GPIO_INPUT_TTL1); //目标IO1设置为TTL1模式
	csi_pin_set_mux(PC8,PC8_INPUT);			 //目标IO2
	csi_pin_input_mode(PC8,GPIO_INPUT_TTL1); //目标IO2设置为TTL1模式
	csi_pin_set_mux(PA1,PA1_OUTPUT);		 //指示IO1
	csi_pin_set_mux(PA2,PA2_OUTPUT);		 //指示IO2			
	
	while(1)
	{
		if(csi_pin_read(PB6))               
			csi_pin_set_high(PA1);    
		else                                 
			csi_pin_set_low(PA1);
			
		if(csi_pin_read(PC8))               
			csi_pin_set_high(PA2);    
		else                                 
			csi_pin_set_low(PA2);
	}
}

/** \brief GPIO VIH/VIL测试，TTL2模式
 *	VIL：
 *  1、VDD设置为5V,将目标IO设为输入，并设置为TTL2模式。
 *  2、目标IO接可调电源，调节可调电源输出，记录可以使指示IO稳定输出低的最高电平，即为VIL。
 *	VIH：
 *  1、VDD设置为5V,将目标IO设为输入，并设置为TTL2模式。
 *  2、目标IO接可调电源，调节可调电源输出，记录可以使指示IO稳定输出高的最低电平，即为VIH。
 *  \param[in] none
 *  \return none
 */
void gpio_vih_vil_test_ttl2_mode(void)
{
	csi_pin_set_mux(PC4,PC4_INPUT);				//目标IO1
	csi_pin_input_mode(PC4,GPIO_INPUT_TTL2);	//目标IO1设置为TTL2模式
	csi_pin_set_mux(PC10,PC10_INPUT);			//目标IO2
	csi_pin_input_mode(PC10,GPIO_INPUT_TTL2);	//目标IO2设置为TTL2模式
	csi_pin_set_mux(PA1,PA1_OUTPUT);			//指示IO1	
	csi_pin_set_mux(PA2,PA2_OUTPUT);			//指示IO2		
	
	while(1)
	{
		if(csi_pin_read(PC4))               
			csi_pin_set_high(PA1);    
		else                                 
			csi_pin_set_low(PA1);
			
		if(csi_pin_read(PC10))               
			csi_pin_set_high(PA2);    
		else                                 
			csi_pin_set_low(PA2);
	}
}

/** \brief GPIO VOH测试
 *  1、将VDD设为5V，将目标IO设为输出高，打开强驱。
 *  2、目标IO接入电子负载和电压表，调节电子负载，直到电流显示为15mA。
 *  3、记录此时电压表的电压。
 *  \param[in] none
 *  \return none
 */
void gpio_voh_test(void)
{
	//PA0/1/2/3/5/6、PC4/5、PD0/1为LED_S功能，默认0是高驱，1是恒流
	csi_pin_set_mux(PA0,PA0_OUTPUT);
	csi_pin_drive(PA0, GPIO_DRIVE_LV0);//PA0带LED_S功能，默认0是高驱，1是恒流
	csi_pin_set_high(PA0);
	
	csi_pin_set_mux(PB1,PB1_OUTPUT);
	csi_pin_drive(PB1, GPIO_DRIVE_LV1);
	csi_pin_set_high(PB1);
	
	csi_pin_set_mux(PC10,PC10_OUTPUT);
	csi_pin_drive(PC10, GPIO_DRIVE_LV1);
	csi_pin_set_high(PC10);
	
	csi_pin_set_mux(PD2,PD2_OUTPUT);
	csi_pin_drive(PD2, GPIO_DRIVE_LV1);
	csi_pin_set_high(PD2);
}

/** \brief GPIO VOL测试
 *  1、VDD设为5V，将目标IO设为输出低，打开强驱。
 *  2、目标IO接电压表，外部5V电源经电子负载接入目标IO。
 *  3、调节电子负载，直到电流显示为15mA(普通IO) / 120mA(HSIO)。
 *  4、记录此时电压表的电压。
 *  \param[in] none
 *  \return none
 */
void gpio_vol_test(void)
{
	//PA0/1/2/3/5/6、PC4/5、PD0/1为LED_S功能，默认0是高驱，1是恒流
	csi_pin_set_mux(PA0,PA0_OUTPUT);	//普通IO
	csi_pin_drive(PA0, GPIO_DRIVE_LV0); //PA0带LED_S功能，默认0是高驱，1是恒流
	csi_pin_set_low(PA0);
	
	csi_pin_set_mux(PB1,PB1_OUTPUT);	//普通IO
	csi_pin_drive(PB1, GPIO_DRIVE_LV1);
	csi_pin_set_low(PB1);
	
	csi_pin_set_mux(PC10,PC10_OUTPUT);	//HSIO
	csi_pin_drive(PC10, GPIO_DRIVE_LV1);
	csi_pin_set_low(PC10);
	
	csi_pin_set_mux(PD2,PD2_OUTPUT);	//HSIO
	csi_pin_drive(PD2, GPIO_DRIVE_LV1);
	csi_pin_set_low(PD2);
}

/** \brief GPIO RPU测试
 *  RPU:将目标IO设为高阻态，打开上拉电阻。设置VDD为5V,记录电流，即为IPU。上拉电阻RPU = VDD/IPU。
 *  \param[in] none
 *  \return none
 */
void gpio_rpu_test(void)
{	
	csi_pin_set_mux(PA0,PA0_GPD);
	csi_pin_set_mux(PB1,PB1_GPD);
	csi_pin_set_mux(PC10,PC10_GPD);
	csi_pin_set_mux(PD2,PD2_GPD);
	
	//打开上拉电阻
	csi_pin_pull_mode(PA0,GPIO_PULLUP);
	csi_pin_pull_mode(PB1,GPIO_PULLUP);
	csi_pin_pull_mode(PC10,GPIO_PULLUP);
	csi_pin_pull_mode(PD2,GPIO_PULLUP);
}

/** \brief GPIO RPD测试
 *  RPD:将目标IO设为高阻态，打开下拉电阻。设置VDD为5V,记录电流，即为IPD。上拉电阻RPD = VDD/IPD。
 *  \param[in] none
 *  \return none
 */
void gpio_rpd_test(void)
{	
	csi_pin_set_mux(PA0,PA0_GPD);
	csi_pin_set_mux(PB1,PB1_GPD);
	csi_pin_set_mux(PC10,PC10_GPD);
	csi_pin_set_mux(PD2,PD2_GPD);

	//打开下拉电阻
	csi_pin_pull_mode(PA0,GPIO_PULLDOWN);
	csi_pin_pull_mode(PB1,GPIO_PULLDOWN);
	csi_pin_pull_mode(PC10,GPIO_PULLDOWN);
	csi_pin_pull_mode(PD2,GPIO_PULLDOWN);
}

/** \brief GPIO输出频率测试
 *  通过CLO功能把系统频率分频后通过CLO引脚输出，输出波形未明显变形判定有效。
 *  \param[in] none
 *  \return none
 */
void gpio_iofout_test(void)
{	
	csi_pin_set_mux(PD3, PD3_CLO);
	csi_clo_config(CLO_HFCLK,CLO_DIV2);
}

/** \brief GPIO输入频率测试
 *  1、通过波形发生器产生波形，接入IO，波形参数10MHZ，占空比50%。
 *  2、输入波形经过ETCB触发GPTB捕获，GPTB时钟52.5MHZ，捕获模式设置为合并捕获，捕获两次，第一次捕获后复位计数值，第二次得到的值即为周期值，用GPTB时钟除以周期值得到输入频率。
 *  3、实测捕获到的周期值为4，故输入频率为 52.5MHZ / (4 + 1) = 10.5MHZ
 *  \param[in] none
 *  \return error code
 */
void gpio_iofin_test(void)
{
//------------------------------------------------------------------------------------------------------------------------	
	csi_pin_set_mux(PA1,PA1_INPUT);		
	csi_pin_pull_mode(PA1, GPIO_PULLUP);						//PA01 上拉
	csi_pin_irq_mode(PA1,EXI_GRP1, GPIO_IRQ_FALLING_EDGE);		//PA01 下降沿产生中断
	csi_pin_irq_enable(PA1, ENABLE);							//使能GPIO中断	
	csi_pin_vic_irq_enable(EXI_GRP1, ENABLE);
	csi_exi_set_evtrg(1, TRGSRC_EXI1, 1);		
//------------------------------------------------------------------------------------------------------------------------		
	csi_etb_config_t tEtbConfig;				//ETB 参数配置结构体	
	volatile uint8_t ch;
	tEtbConfig.byChType  = ETB_ONE_TRG_ONE;  	//单个源触发单个目标
	tEtbConfig.bySrcIp   = ETB_EXI_TRGOUT1 ;  	//...作为触发源
	tEtbConfig.byDstIp   = ETB_GPTB0_SYNCIN2;   //GPTB0 同步输入2作为目标事件
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	//自动获取空闲通道号,ch >= 0 获取成功						//ch < 0,则获取通道号失败		
	csi_etb_ch_config(ch, &tEtbConfig);	
//------------------------------------------------------------------------------------------------------------------------	
	csi_gptb_captureconfig_t tPwmCfg;								  
	tPwmCfg.byWorkmod         = GPTB_CAPTURE;                     //WAVE or CAPTURE    //计数或捕获	
	tPwmCfg.byCountingMode    = GPTB_UPCNT;                       //CNYMD  //计数方向
	tPwmCfg.byStartSrc        = GPTB_SYNC_START;				  //软件使能同步触发使能控制（RSSR中START控制位）//启动方式
	tPwmCfg.byPscld           = GPTB_LDPSCR_ZRO;                  //PSCR(分频)活动寄存器载入控制。活动寄存器在配置条件满足时，从影子寄存器载入更新值	
	tPwmCfg.byCaptureCapmd    = GPTB_OP_CONT;        		      //0:连续捕捉模式    1h：一次性捕捉模式
	tPwmCfg.byCapSrcMode      = GPTB_MERGE_CAP;                   //0:GPTB_MERGE_CAP合并触发信号 1：GPTB_SEPARATE_CAP分离触发信号
	tPwmCfg.byCaptureStopWrap = 2;                                //Capture模式下，捕获事件计数器周期设置值
	tPwmCfg.byCaptureLdaret   = 1;                                //CMPA捕捉载入后，计数器值计数状态控制位(1h：CMPA触发后，计数器值进行重置;0h：CMPA触发后，计数器值不进行重置)
	tPwmCfg.byCaptureLdbret   = 0;  
	tPwmCfg.byCaptureLdcret   = 0;
	tPwmCfg.byCaptureLddret   = 0;                            
	tPwmCfg.wInt 		  = GPTB_INT_CAPLD1;                  //interrupt
	csi_gptb_capture_init(GPTB0, &tPwmCfg);
	
	csi_gptb_set_sync (GPTB0, GPTB_TRGIN_SYNCEN2, GPTB_TRG_CONTINU,GPTB_AUTO_REARM_ZRO);
	
	csi_gptb_start(GPTB0);//start  timer
}

/** \brief GPIO ILIH/ILIL测试
 *  ILIH:V1为5V,将目标IO设为输入,测量IO口与VDD之间的电流。
 *  ILIL:V1为5V,将目标IO设为输入,测量IO口与GND之间的电流。
 *  \param[in] none
 *  \return none
 */
void gpio_ilih_ilil_test(void)
{
	csi_pin_set_mux(PA0,PA0_INPUT); 
	csi_pin_set_mux(PB1,PB1_INPUT);
	csi_pin_set_mux(PC10,PC10_INPUT);
}

/** \brief GPIO 恒流模式测试(只有LED_S脚有恒流模式)
 *  1、目标IO设置成恒流输出模式，输出高。
 *  2、目标IO口加电压3V，记录相应的IO口电流。
 *  \param[in] none
 *  \return none
 */
void gpio_ioc_test(void)
{	
	//PA0/1/2/3/5/6、PC4/5、PD0/1为LED_S功能，默认0是高驱，1是恒流
	csi_pin_set_mux(PA0,PA0_OUTPUT);
	csi_pin_drive(PA0, GPIO_DRIVE_LV1);
	csi_pin_set_high(PA0);
	
	csi_pin_set_mux(PA5,PA5_OUTPUT);
	csi_pin_drive(PA5, GPIO_DRIVE_LV1);
	csi_pin_set_high(PA5);
	
	csi_pin_set_mux(PC4,PC4_OUTPUT);
	csi_pin_drive(PC4, GPIO_DRIVE_LV1);
	csi_pin_set_high(PC4);
	
	csi_pin_set_mux(PD0,PD0_OUTPUT);
	csi_pin_drive(PD0, GPIO_DRIVE_LV1);
	csi_pin_set_high(PD0);
}