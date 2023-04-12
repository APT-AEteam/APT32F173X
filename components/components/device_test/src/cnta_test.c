#include "demo.h"
#include "pin.h"
#include <string.h>
#include <csi_drv.h>
#include "csp_cnta.h"
#include "cnta_test.h"

void cnta_test(void)
{
	cnta_timer_test(); 
	cnta_pwm_test();
	cnta_envelope_test();
}

void cnta_timer_test(void)
{
	csi_conta_timer_config_t tTimerCfg;
	
	csi_pin_set_mux(PA10, PA10_OUTPUT);         //配置输出
	csi_pin_set_high(PA10);					    //初始设置为高，在cnta的中断中翻转
	
	csi_clk_enable((uint32_t *)CA0);		    //cnta clk enable
	
	tTimerCfg.eClkDiv  = CNTA_CK_DIV8;
	tTimerCfg.wTime    = 2000;                     //1000us,if cnta clk is 3M,the timeout for timer((0.333us * 1) ->(0.333us * 65535): 0.3333us -> 21.845ms)
	tTimerCfg.eRunMode = CNTA_REPEAT_MODE;
	tTimerCfg.byInter  = CNTA_PENDL_INT;
	
	csi_cnta_timer_init(CA0,&tTimerCfg);
	csi_cnta_start(CA0);
	while(1)
	{
		nop;
	}
}


void cnta_pwm_test(void)
{
	csi_conta_pwm_config_t tPwmCfg;
	
	csi_pin_set_mux(PA5, PA5_OUTPUT);         //配置输出
	csi_pin_set_high(PA5);					    //初始设置为高，在cnta的中断中翻转
	
	tPwmCfg.eClkDiv      = CNTA_CK_DIV8;   //时钟8分频
	tPwmCfg.byStartLevel = POLAR_LOW;	   //开始极性低
	tPwmCfg.byStopLevel  = STOP_LOW;       //结束极性低
	tPwmCfg.byDutyCycle  = 60;             //占空比
	tPwmCfg.wFreq        = 38000;          //频率(hz)
	tPwmCfg.byInter      = CNTA_NONE_INT;  //无中断源
	
	//cnta作为pwm输出口
	csi_pin_set_mux(PA10,PA10_CNTA_BUZ);//set counter output pin	
	//csi_pin_set_mux(PD3,PD3_CNTA_BUZ);//set counter output pin	
	//csi_pin_set_mux(PD5,PD5_CNTA_BUZ);//set counter output pin
	
	csi_cnta_pwm_init(CA0,&tPwmCfg);
	//csi_cnta_bt0_sync(CA0, PEND_CARRIERON_SET, MATCH_CARRIERON_CLR,CNTA_HW_DIS);//结合bt0设置硬件自动打开或者关闭载波
	csi_cnta_start(CA0);
	while(1)
	{
		nop;
	}
}


void cnta_envelope_test(void)
{
	csi_conta_pwm_config_t tPwmCfg;
	csi_bt_pwm_config_t tBTPwmCfg;							//BT PWM输出参数初始化配置结构体
	
	csi_pin_set_mux(PA6, PA6_OUTPUT);		//PA6 output，并在BT_PEND_INT中断里翻转，用于查看CountA的状态
	csi_pin_set_high(PA6);					//PA6 output high;
	csi_pin_set_mux(PA5, PA5_OUTPUT);		//PA5 output，并在BT_CMP_INT中断里翻转，用于查看CountA的状态
	csi_pin_set_high(PA5);					//PA5 output high;
	
	//BT 初始化
	csi_pin_set_mux(PC11, PC11_BT1_OUT);					//PC11 作为BT1 PWM输出引脚
	//init timer pwm para config
	tBTPwmCfg.byIdleLevel = BT_PWM_IDLE_HIGH;					//PWM 输出空闲电平
	tBTPwmCfg.byStartLevel= BT_PWM_START_HIGH;				//PWM 输出起始电平
	tBTPwmCfg.byDutyCycle = 50;								//PWM 输出占空比(0 < DutyCycle < 100)		
	tBTPwmCfg.wFreq 		= 50;								//PWM 输出频率
	//tPwmCfg.byInter 	= BT_INTSRC_PEND | BT_INTSRC_CMP;	//PWM 中断配置(PEND and CMP)
	tBTPwmCfg.byInter		= BT_INTSRC_NONE;
	
	csi_bt_pwm_init(BT1, &tBTPwmCfg);							//初始化BT1 PWM输出
	csi_bt_start(BT1);										//启动BT1
	
//	csp_bt_set_cr(BT0, (BT_IMMEDIATE << BT_SHDW_POS) | (BT_CONTINUOUS << BT_OPM_POS) |		//bt work mode
//			(BT_PCLKDIV << BT_EXTCKM_POS) | (BT_CNTRLD_EN << BT_CNTRLD_POS) | BT_CLK_EN );
//	csp_bt_set_pscr(BT0,8);						//bt clk div	
//	csp_bt_set_prdr(BT0,5000);				    //bt prdr load value
//	csp_bt_set_cmp(BT0, 2500);				    //bt prdr load value
//	
//	csp_bt_int_set(BT0, BT_PEND_INT, ENABLE);  //enable PEND interrupt
//	csp_bt_int_set(BT0, BT_CMP_INT, ENABLE);	//enable CMP interrupt
//	
//	csi_irq_enable((uint32_t *)BT0);		    //enable bt irq
//	csi_bt_start(BT0);					        //启动BT
	
	
	//CountA 初始化
	tPwmCfg.eClkDiv      = CNTA_CK_DIV1;   //时钟8分频
	tPwmCfg.byStartLevel = POLAR_LOW;	   //开始极性低
	tPwmCfg.byStopLevel  = STOP_LOW;       //结束极性低
	tPwmCfg.byDutyCycle  = 60;             //占空比
	tPwmCfg.wFreq        = 380000;          //频率(hz)
	tPwmCfg.byInter      = CNTA_NONE_INT;  //无中断源
	
	//cnta作为pwm输出口
	csi_pin_set_mux(PA10,PA10_CNTA_BUZ);//set counter output pin	
	//csi_pin_set_mux(PD3,PD3_CNTA_BUZ);//set counter output pin	
	//csi_pin_set_mux(PD5,PD5_CNTA_BUZ);//set counter output pin
	
	csi_cnta_pwm_init(CA0,&tPwmCfg);
	
	// CACON.ENVELOPE测试
	csp_cnta_set_carrier(CA0, CNTA_CARRIER_EN, PWM_CARRIER, STOP_LOW, POLAR_LOW);  //载波输出	
//	csp_cnta_set_carrier(CA0, CNTA_CARRIER_EN, PWM_ENVELOPE, STOP_LOW, POLAR_LOW); //包络输出

	// CACON.BTPEND_REM_CON 和 CACONBTMATCH_REM_CON 测试
//	csp_cnta_set_sync(CA0, PEND_CARRIERON_CLR, MATCH_CARRIERON_SET,CNTA_HW_TCPEND);//BT脉冲匹配中断发生时，CARRIERON位会被硬件自动置位
	csp_cnta_set_sync(CA0, PEND_CARRIERON_SET, MATCH_CARRIERON_CLR,CNTA_HW_TCPEND);//BT周期结束中断发生时，CARRIERON位会被硬件自动置位
//	csp_cnta_set_sync(CA0, PEND_CARRIERON_DIS, MATCH_CARRIERON_DIS,CNTA_HW_TCPEND);//禁止CARRIERON的硬件自动触发
	csi_cnta_start(CA0);
	while(1)
	{
		nop;
	}
}


/*
void cnta_reset_test(void)
{
	csp_cnta_soft_rst(CA0);				//default init valu
	while(1)
	{
		NOP;
	}
}

void cnta_timer_test(void)
{
	csi_pin_set_mux(PA10, PA10_OUTPUT);         //配置输出
	csi_pin_set_high(PA10);					    //初始设置为高，在cnta的中断中翻转
	
	csi_clk_enable((uint32_t *)CA0);		    //cnta clk enable
    csp_cnta_soft_rst(CA0);				        //default init valu
	csp_cnta_set_ckdiv(CA0,CNTA_CK_DIV8,CNTA_REPEAT_MODE);	//cnta clk = pclk/eClkDiv
	apt_cnta_int_arrt_set(CLIC_INTATTR_TRIG_UP); 
	csp_cnta_set_datal(CA0, 5000);				//set data
	csp_cnta_set_datah(CA0, 5000);		    //set data
	csp_cnta_soft_updata(CA0);	
	csp_cnta_set_int(CA0,CNTA_PENDHL_INT, true); //set intrrupt
	
//	csp_cnta_set_datal(CA0, 5000);				//set data
//	csp_cnta_set_datah(CA0, 0);				//set data
//	csp_cnta_soft_updata(CA0);	
//	csp_cnta_set_int(CA0,CNTA_PENDL_INT, true); //set intrrupt
	
//	csp_cnta_set_datal(CA0, 5000);				//set data
//	csp_cnta_set_datah(CA0, 5000);				//set data
//	csp_cnta_soft_updata(CA0);	
//	csp_cnta_set_int(CA0,CNTA_PENDHL_INT, true); //set intrrupt
	
	csi_irq_enable((uint32_t *)CA0);			//enable cnta irq
	
	csi_cnta_start(CA0);
	
	while(1)
	{
		NOP;
	}
}


__attribute__((weak)) void cnta_test_irqhandler(csp_cnta_t *ptCntaBase)
{	
//	if(CA0->INTMASK & 0x01)
	csi_pin_toggle(PA10);
}


void cnta_pwm_test(void)
{
	csi_pin_set_mux(PA6, PA6_OUTPUT);		//PA06 output
	csi_pin_set_high(PA6);					//PA06 output high;
	
	csi_pin_set_mux(PA10,PA10_CNTA_BUZ); //set counter output pin
//	csi_pin_set_mux(PD5,PD5_CNTA_BUZ);   //set counter output pin
//	csi_pin_set_mux(PD3,PD3_CNTA_BUZ);   //set counter output pin

	csi_clk_enable((uint32_t *)CA0);		//cnta clk enable
    csp_cnta_soft_rst(CA0);				//default init valu
	csp_cnta_set_ckdiv(CA0,CNTA_CK_DIV8,CNTA_REPEAT_MODE);	//cnta clk = pclk/eClkDiv
	csp_cnta_set_carrier(CA0, CNTA_CARRIER_EN, PWM_CARRIER, STOP_HIGH, POLAR_LOW);
	apt_cnta_int_arrt_set(CLIC_INTATTR_TRIG_UP); 
	csp_cnta_set_datal(CA0, 5000);				//set data
    csp_cnta_set_datah(CA0, 5000);				//set data
	csp_cnta_soft_updata(CA0);	
	csp_cnta_set_int(CA0,CNTA_NONE_INT, true);//set intrrupt
	csi_irq_enable((uint32_t *)CA0);					//enable cnta irq
	
	
	csi_pin_set_low(PA6);					//PA06 output low;
	csi_cnta_start(CA0);
	while(1)
	{
		NOP;
		
		Delay_Nms(100);
//		csi_cnta_stop(CA0);                  //停止CountA
		CA0->CACON = CA0->CACON & 0xFDFFFFFF; // 关闭载波，可测试REM_STAT位
		csi_pin_set_high(PA6);  //PA06 output high;
		
		Delay_Nms(100);
//		csi_cnta_start(CA0);                  //启动CountA 
		CA0->CACON = CA0->CACON | 0x02000000; // 打开载波，可测试REM_STAT位
		csi_pin_set_low(PA6);   //PA06 output low;
	}
}


void cnta_bt_test(void)
{
	csi_pin_set_mux(PA6, PA6_OUTPUT);		//PA06 output
	csi_pin_set_high(PA6);					//PA06 output high;
	csi_pin_set_mux(PA5, PA5_OUTPUT);		//PA06 output
	csi_pin_set_high(PA5);					//PA06 output high;
	
	//BT 初始化
	csp_bt_set_cr(BT0, (BT_IMMEDIATE << BT_SHDW_POS) | (BT_CONTINUOUS << BT_OPM_POS) |		//bt work mode
			(BT_PCLKDIV << BT_EXTCKM_POS) | (BT_CNTRLD_EN << BT_CNTRLD_POS) | BT_CLK_EN );
	csp_bt_set_pscr(BT0,2);						//bt clk div	
	csp_bt_set_prdr(BT0,5000);				    //bt prdr load value
	csp_bt_set_cmp(BT0, 2500);				    //bt prdr load value
	
	csp_bt_int_enable(BT0, BT_PEND_INT, true);  //enable PEND interrupt
	csp_bt_int_enable(BT0, BT_CMP_INT, true);	//enable CMP interrupt
	
	csi_irq_enable((uint32_t *)BT0);		    //enable bt irq
	csi_bt_start(BT0);					        //启动BT
	
	//CountA 初始化
	csi_pin_set_mux(PA10,PA10_CNTA_BUZ); //set counter output pin
//	csi_pin_set_mux(PD5,PD5_CNTA_BUZ);   //set counter output pin
//	csi_pin_set_mux(PD3,PD3_CNTA_BUZ);   //set counter output pin

	csi_clk_enable((uint32_t *)CA0);		//cnta clk enable
    csp_cnta_soft_rst(CA0);				//default init valu
	csp_cnta_set_ckdiv(CA0,CNTA_CK_DIV1,CNTA_REPEAT_MODE);	//cnta clk = pclk/eClkDiv
	
	
	// CACON.ENVELOPE测试
	csp_cnta_set_carrier(CA0, CNTA_CARRIER_EN, PWM_CARRIER, STOP_LOW, POLAR_LOW); //载波输出	
//	csp_cnta_set_carrier(CA0, CNTA_CARRIER_EN, PWM_ENVELOPE, STOP_LOW, POLAR_LOW); //包络输出

	// CACON.BTPEND_REM_CON 和 CACONBTMATCH_REM_CON 测试
//	csp_cnta_set_sync(CA0, PEND_CARRIERON_CLR, MATCH_CARRIERON_SET,CNTA_HW_TCPEND);//BT脉冲匹配中断发生时，CARRIERON位会被硬件自动置位
	csp_cnta_set_sync(CA0, PEND_CARRIERON_SET, MATCH_CARRIERON_CLR,CNTA_HW_TCPEND);//BT周期结束中断发生时，CARRIERON位会被硬件自动置位
//	csp_cnta_set_sync(CA0, PEND_CARRIERON_DIS, MATCH_CARRIERON_DIS,CNTA_HW_TCPEND);//禁止CARRIERON的硬件自动触发
	
	
	csp_cnta_set_datal(CA0, 100);				//set data
    csp_cnta_set_datah(CA0, 100);				//set data
	csp_cnta_soft_updata(CA0);	
	csp_cnta_set_int(CA0,CNTA_NONE_INT, true);//set intrrupt
	csi_irq_enable((uint32_t *)CA0);					//enable cnta irq
	
	csi_cnta_start(CA0);
	while(1)
	{
		NOP;
	}
}

*/