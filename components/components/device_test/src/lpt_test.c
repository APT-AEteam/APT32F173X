/***********************************************************************//** 
 * \file  lpt_test.c
 * \brief  lpt测试代码 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-4-19 <td>V0.0 <td>YT    <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include <lpt.h>
#include <pin.h>
#include "demo.h"
#include <etb.h>
#include <bt.h>
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/



/** \brief lpt 输出pwm测试代码
 * 
 *  \param[in] none
 *  \return error code
 */
int lpt_pwm_test(void)
{
	int iRet = 0;
	
	csi_lpt_pwm_config_t tLptPwmCfg;  	
	csi_pin_set_mux(PB6, PB6_LPT_OUT);	                           //将PB6设为LPT_OUT

	tLptPwmCfg.byClksrc = LPT_CLK_PCLK_DIV4;                          //PWM 时钟选择
	tLptPwmCfg.byStartpol = LPT_PWM_START_LOW;                    //初始低电平
	tLptPwmCfg.byIdlepol  = LPT_PWM_IDLE_LOW;                     //停止时highZ
	tLptPwmCfg.byCycle = 30;                                     //PWM 输出占空比(0~100)	
	tLptPwmCfg.wFreq = 1000;                                     //PWM 输出频率
	tLptPwmCfg.byInter 	= LPT_NONE_INT;	                         //PWM 配置无中断
	
	if(csi_lpt_pwm_init(LPT, &tLptPwmCfg) == CSI_OK){            //初始化lpt

		csi_lpt_start(LPT);                                      //启动lpt
	}	
	return iRet;	
}

/** \brief lpt CR[PRDLD]寄存器测试代码
 * PRDLD=1, PRDR活动寄存器更新发生在周期结束时。PRDR改变时的周期和之前的一致。
 * PRDLD=0, 立即更新，所有对PRDR操作直接作用于活动寄存器。PRDR改变时的周期和之后的一致。
 *  \param[in] none
 *  \return error code
 */
int lpt_prdld_test(void)
{
	int iRet = 0;
	
	csi_lpt_pwm_config_t tLptPwmCfg;  	
	csi_pin_set_mux(PB6, PB6_LPT_OUT);	                           //将PB6设为LPT_OUT
	csi_pin_set_mux(PB5, PB5_LPT_IN);                              //将PB5设为LPT_IN
	tLptPwmCfg.byClksrc = LPT_CLK_EMCLK;                          //PWM 时钟选择
	tLptPwmCfg.byStartpol = LPT_PWM_START_LOW;                    //初始低电平
	tLptPwmCfg.byIdlepol  = LPT_PWM_IDLE_LOW;                     //停止时highZ
	tLptPwmCfg.byCycle = 30;                                     //PWM 输出占空比(0~100)	
	tLptPwmCfg.wFreq = 1000;                                     //PWM 输出频率
	tLptPwmCfg.byInter 	= LPT_NONE_INT;	                         //PWM 配置无中断
	if(csi_lpt_pwm_init(LPT, &tLptPwmCfg) == CSI_OK){            //初始化lpt
		LPT->CR = (LPT->CR &~(1<<4));
		csi_lpt_start(LPT);                                      //启动lpt
	}	
	while(1)
	{
		LPT->PRDR = 15000;
		
		csi_pin_toggle(PA6);				//PA06 toggle
		mdelay(100);
		
		LPT->PRDR = 12000;
		
		csi_pin_toggle(PA6);				//PA06 toggle
		mdelay(100);
	}
	return iRet;	
}


/** \brief lpt CR[PSCLD]寄存器测试代码
 * CR[PSCLD]=1，PSCR活动寄存器更新发生在周期结束，启动或者外部触发时。周期结束时，PSCR活动寄存器更新，信号频率发生变化。
 * CR[PSCLD]=0，PSCR活动寄存器更新发生在启动或者外部触发时。周期结束时，PSCR活动寄存器更新，信号频率没有发生变化。
 *  \param[in] none
 *  \return error code
 */
int lpt_pscld_test(void)
{
	int iRet = 0;
	
	csi_lpt_pwm_config_t tLptPwmCfg;  	
	csi_pin_set_mux(PB6, PB6_LPT_OUT);	                           //将PB6设为LPT_OUT
//	tLptPwmCfg.byClksrc =LPT_CLK_IN_FALL;                          //PWM 时钟选择，外部时钟
	tLptPwmCfg.byClksrc = LPT_CLK_EMCLK;                          //PWM 时钟选择
	tLptPwmCfg.byStartpol = LPT_PWM_START_LOW;                    //初始低电平
	tLptPwmCfg.byIdlepol  = LPT_PWM_IDLE_LOW;                     //停止时highZ
	tLptPwmCfg.byCycle = 30;                                     //PWM 输出占空比(0~100)	
	tLptPwmCfg.wFreq = 1000;                                     //PWM 输出频率
	tLptPwmCfg.byInter 	= LPT_NONE_INT;	                         //PWM 配置无中断
	if(csi_lpt_pwm_init(LPT, &tLptPwmCfg) == CSI_OK){            //初始化lpt
		LPT->CR = (LPT->CR &~(1<<16));  // CR[PSCLD]=0
		csi_lpt_start(LPT);                                      //启动lpt
	}	
	while(1)
	{
		LPT->PSCR=0x0;
		csi_pin_toggle(PA6);				//PA06 toggle
		mdelay(100);
		
		LPT->PSCR=0x3;
		csi_pin_toggle(PA6);				//PA06 toggle
		mdelay(100);
	}
	return iRet;	
}


/** \brief lpt CR[CMPLD]寄存器测试代码
 * CMPLD=0, 立即更新，所有对CMP操作直接作用于活动寄存器
 * CMPLD=1，CMP活动寄存器更新发生在周期结束时
 *  \param[in] none
 *  \return error code
 */
int lpt_cmpld_test(void)
{
	int iRet = 0;
	csi_lpt_pwm_config_t tLptPwmCfg;  	
	csi_pin_set_mux(PB6, PB6_LPT_OUT);	                           //将PB6设为LPT_OUT
	csi_pin_set_mux(PB5, PB5_LPT_IN);                              //将PB5设为LPT_IN
	tLptPwmCfg.byClksrc = LPT_CLK_PCLK_DIV4;                          //PWM 时钟选择
	tLptPwmCfg.byStartpol = LPT_PWM_START_LOW;                    //初始低电平
	tLptPwmCfg.byIdlepol  = LPT_PWM_IDLE_LOW;                     //停止时highZ
	tLptPwmCfg.byCycle = 30;                                     //PWM 输出占空比(0~100)	
	tLptPwmCfg.wFreq = 1000;                                     //PWM 输出频率
	tLptPwmCfg.byInter 	= LPT_NONE_INT;	                         //PWM 配置无中断
	if(csi_lpt_pwm_init(LPT, &tLptPwmCfg) == CSI_OK){            //初始化lpt
	
		LPT->CR = (LPT->CR &~(1<<17));  // CR[CMPLD]=0
		csi_lpt_start(LPT);                                      //启动lpt
	}	
	while(1)
	{
		LPT->CMP=0x00000e10;
		csi_pin_toggle(PA6);				//PA06 toggle
		mdelay(100);
		LPT->CMP=0x00001e10;
		csi_pin_toggle(PA6);				//PA06 toggle
		mdelay(100);
	}
	return iRet;	
}




/** \brief lpt 同步触发测试代码
 * 通过BT，产生下拉触发波形，触发PIN(PB0)，产生etcb触发。
 * 将BT输出连接PB00，接收触发信号。
 * 会进入LPT中断，并且REARMx置1(已经检测到触发，不允许后续触发)，
 * 这是因为OSTMDX=1(一次性触发模式，在一次触发事件被检测到后，改通道不允许后续的触发事件通过，
 * 直到被软件重置REARM后才允许新的触发事件通过)。PEND和MATCH 中断置1。AREARM=1，在周期结束时，自动REARM，REARM=0。
 *  \param[in] none
 *  \return error code
 */
int lpt_sync_trg_start_test(void)
{
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				               			//ETB 参数配置结构体

	csi_pin_set_mux(PB0, PB0_INPUT);									//PB0 配置为输入
	csi_pin_pull_mode(PB0, GPIO_PULLUP);								//PB0 上拉
	csi_pin_irq_mode(PB0, EXI_GRP0, GPIO_IRQ_FALLING_EDGE);			//PB0 下降沿产生中断，选择中断组16
	csi_pin_irq_enable(PB0,ENABLE);									//PB0 中断使能
 
	csi_exi_set_evtrg(0, TRGSRC_EXI0, 0);						//EXI0(PB00) 触发EXI_TRGOUT0(PB00用EXI0触发输出)

	csi_lpt_timer_init(LPT,LPT_CLK_PCLK_DIV4,5);       					//初始化lpt
	csi_lpt_set_sync(LPT, LPT_TRG_SYNCIN0, LPT_SYNC_ONCE, ENABLE);
	
	csp_lpt_debug_enable(LPT,ENABLE);
//	csp_lpt_trg_enable(LPT,ENABLE);      // 测试EVTRG TRGEV0,产生触发信号，进中断
	csp_lpt_set_evtrg(LPT, LPT_TRGSRC0_ZRO);  
	
	
	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  		//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_EXI_TRGOUT0;  	    //EXI_TRGOUT5作为触发源
	tEtbConfig.byDstIp =  ETB_LPT_SYNCIN;   	    //LPT同步输入作为目标事件
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
	
	csi_etb_init();
	
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);

	return iRet;
	
}


/** \brief lpt 触发输出测试代码
 *  在csi_lpt_timer_init函数中使能TRGEV_INT和PEND_INT,
 *  断点打在lpt_irqhandler(LPT)处，每进一次PEND中断，TRGEV0CNT+1,
 *  当TRGEV0CNT=TRGEV0PRD时，产生TRGEV0中断 
 *  \param[in] none
 *  \return error code
 */
int lpt_trg_out_test(void)
{
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				    				//ETB 参数配置结构体

	csi_lpt_timer_init(LPT,LPT_CLK_PCLK_DIV4,50);   				//初始化lpt,选用内部超低功耗时钟,定时50ms,默认采用PEND中断
	csi_lpt_set_evtrg(LPT, LPT_TRGOUT, LPT_TRGSRC_PRD, 10);
	
	csp_lpt_debug_enable(LPT,ENABLE);
	csp_lpt_trg_enable(LPT,ENABLE);      // 测试EVTRG TRGEV0,产生触发信号，进中断
	csp_lpt_set_evtrg(LPT, LPT_TRGSRC0_ZRO);  
	
	csp_lpt_set_trgprd(LPT,0xf);
	csp_lpt_set_trgcnt(LPT,0xf);
	csi_bt_timer_init(BT1,5000);									//BT定时1ms
	csi_bt_set_sync(BT1, BT_TRGIN_SYNCEN0, BT_TRG_ONCE, DISABLE);		//外部触发bt启动(SYNCIN0)

	
	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  						//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_LPT_TRGOUT0;  	    					//LPT作为触发源
	tEtbConfig.byDstIp =  ETB_BT1_SYNCIN0;   	    				//BT1同步输入0 作为目标事件
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
	
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    				//自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    				//ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
	csi_lpt_start(LPT);	                            				//启动lpt,触发BT1启动
	
	return iRet;
}

/** \brief lpt 软件触发测试代码
 *  LPT_EVSWF写1，触发BT1启动。
 *  
 *  \param[in] none
 *  \return error code
 */
int lpt_soft_trg_out_test(void)
{
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				    				//ETB 参数配置结构体
	
	csi_bt_timer_init(BT1,1000);									//BT定时1ms
	csi_bt_set_sync(BT1,BT_TRGIN_SYNCEN0, BT_TRG_ONCE, DISABLE);		//外部触发bt启动(SYNCIN0)
	
	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  						//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_LPT_TRGOUT0;  	    					//LPT作为触发源
	tEtbConfig.byDstIp =  ETB_BT1_SYNCIN0;   	    				//BT1同步输入0 作为目标事件
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
	
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    				//自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    				//ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
	csi_lpt_soft_evtrg(LPT);	                            		//lpt软件触发, 触发BT1启动
	
	return iRet;
}



/** \brief lpt 带滤波功能的外部时钟测试代码
 *  csi_lpt_timer_init函数中wLptClk = apt_get_lpt_clk(eClk);  改成  wLptClk = 2000000;
 *  波形发生器产生2 mHz信号，接入PA5
 *  现象：可以进PEND中断
 *  \param[in] none
 *  \return error code
 */
int lpt_filter_test(void)
{
	int iRet = 0;
	csi_pin_set_mux(PA5, PA5_LPT_IN);                              //将PB5设为LPT_IN
	csi_lpt_timer_init(LPT,LPT_CLK_IN_FALL,1);       //初始化lpt,选用内部超低功耗时钟,默认采用PEND中断
	csp_lpt_flt_init(LPT);    // CR[FLTIPSCLD]=1,数字滤波器计数器初始化，计数器值被初始化为CEDR[FLTCKPRS]中的设置值
	csp_lpt_set_filter(LPT, 3, LPT_FLTDEB_02);     // CEDR[FLTCKPRS]=3、CR[FLTDEB] =LPT_FLTDEB_02
	csi_lpt_start(LPT);	                             //启动lpt
	return iRet;	
}



/** \brief lpt 触发事件滤波窗测试代码
 *  
 *  \param[in] none
 *  \return error code
 */
int lpt_window_test(void)
{
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				                //ETB 参数配置结构体	
	csi_lpt_pwm_config_t tLptPwmCfg;  	

	csi_pin_set_mux(PA5,PA5_OUTPUT);	
	csi_pin_set_mux(PA2,PA2_OUTPUT);	

	csi_bt_timer_init(BT0, 1000);	//初始化BT0, 定时2000us； BT定时，默认采用PEND中断
	csi_bt_start(BT0);					//启动定时器  
	csi_bt_set_evtrg(BT0, 0, BT_TRGSRC_PEND);   

	csi_pin_set_mux(PB6, PB6_LPT_OUT);	                           //将PB6设为LPT_OUT
	tLptPwmCfg.byClksrc = LPT_CLK_PCLK_DIV4;                //PWM 时钟选择
	tLptPwmCfg.byStartpol = LPT_PWM_START_HIGH;              //初始低电平
	tLptPwmCfg.byIdlepol  = LPT_PWM_IDLE_LOW;                //停止时highZ
	tLptPwmCfg.byCycle = 1;                            //PWM 输出占空比(0~100)	
	tLptPwmCfg.wFreq = 100;                           //PWM 输出频率   10ms
	tLptPwmCfg.byInter 	= LPT_NONE_INT;	   	

	csi_lpt_pwm_init(LPT, &tLptPwmCfg); 
	csi_lpt_set_sync(LPT, 0, LPT_SYNC_CONT, ENABLE);

	csi_lpt_set_sync_window(LPT, ENABLE, DISABLE, 50, 50);

	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  		//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_BT0_TRGOUT ;  	    //BT0 触发输出0作为触发源
	tEtbConfig.bySrcIp1 = 0xff;      
	tEtbConfig.bySrcIp2 = 0xff;
	tEtbConfig.byDstIp =  ETB_LPT_SYNCIN;   	    //LPT0 同步输入作为目标事件
	tEtbConfig.byDstIp1 = 0xff;
	tEtbConfig.byDstIp2 = 0xff;
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;

	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);	

	return iRet;
}

__attribute__((weak)) void lpt_irqhandler(csp_lpt_t *ptLptBase)
{
	csi_pin_toggle(PA1);
	csp_lpt_clr_all_int(LPT);
}