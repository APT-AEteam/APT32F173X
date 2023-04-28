#include "demo.h"
#include "pin.h"
#include <string.h>
#include <csi_drv.h>
#include "bt_test.h"

static void Delay_Nms(uint32_t ms);

static void Delay_Nms(uint32_t ms)
{
	uint32_t i,j;
	
	for(i=0;i<ms;i++)
	{
		for(j=0;j<740;j++)
		{
			NOP;
		}
	}
}

/** \brief BT所有功能测试总入口函数
 *  \param[in] none
 *  \return error code
 */
void bt_test(void)
{	
	bt_timer_test();
//	bt_pwm_test();
//	bt_sync_start_test();
//	bt_sync_count_test();
//	bt_sync_stop_test();
//	bt_sync_startstop_test();
//	bt_evtrg_test();
//	bt_syncmd_start_test();
}

/** \brief bt pwm out: BT0~BT3 定时
 *  \param[in] none
 *  \return error code
 */
void bt_timer_test(void)
{		
	csi_pin_set_mux(PA6, PA6_OUTPUT);		//PA6 output ，并在BT中断里面翻转IO
	csi_pin_set_high(PA6);					//PA6 output high;
	csi_pin_set_mux(PA5, PA5_OUTPUT);		//PA5 output ，并在BT中断里面翻转IO
	csi_pin_set_high(PA5);					//PA5 output high;
	
	csi_bt_timer_init(BT0, 30000);		//初始化BT0, BT定时，默认采用PEND中断
	csi_bt_start(BT0);					//启动定时器
	
//	csi_bt_timer_init(BT1, 40000);		//初始化BT1, BT定时，默认采用PEND中断
//	csi_bt_start(BT1);					//启动定时器
	
//	csi_bt_timer_init(BT2, 50000);		//初始化BT2，BT定时，默认采用PEND中断
//	csi_bt_start(BT2);					//启动定时器
	
//	csi_bt_timer_init(BT3, 60000);		//初始化BT3, BT定时，默认采用PEND中断
//	csi_bt_start(BT3);					//启动定时器
	
	while(1)
	{
		NOP;
	}
}

/** \brief bt pwm out: BT0~BT3 PWM 输出
 *  \param[in] none
 *  \return error code
 */
void bt_pwm_test(void)
{
	bool bSyncmd_Flag = 0;
	csi_bt_pwm_config_t tPwmCfg;							//BT PWM输出参数初始化配置结构体
	
	csi_pin_set_mux(PA6, PA6_OUTPUT);		//PA6 output ，并在BT中断里面翻转IO
	csi_pin_set_high(PA6);					//PA6 output high;
	
//	csi_pin_set_mux(PA0,  PA0_BT0_OUT);					    //PA0  作为BT0 PWM输出引脚
//	csi_pin_set_mux(PB15, PB15_BT0_OUT);					//PB15 作为BT0 PWM输出引脚
//	csi_pin_set_mux(PC10, PC10_BT0_OUT);					//PC10 作为BT0 PWM输出引脚
	
//	csi_pin_set_mux(PA1,  PA1_BT1_OUT);					    //PA1  作为BT1 PWM输出引脚
//	csi_pin_set_mux(PB14, PB14_BT1_OUT);					//PB14 作为BT1 PWM输出引脚
	csi_pin_set_mux(PC11, PC11_BT1_OUT);					//PC11 作为BT1 PWM输出引脚
	

//	csi_pin_set_mux(PA2,  PA2_BT2_OUT);					    //PA2  作为BT2 PWM输出引脚
//	csi_pin_set_mux(PB13, PB13_BT2_OUT);					//PB13 作为BT2 PWM输出引脚
//	csi_pin_set_mux(PC12, PC12_BT2_OUT);					//PC12 作为BT2 PWM输出引脚
//	csi_pin_set_mux(PD5,  PD5_BT2_OUT);					    //PD5  作为BT2 PWM输出引脚
	
//	csi_pin_set_mux(PA3,  PA3_BT3_OUT);					    //PA3  作为BT3 PWM输出引脚
//	csi_pin_set_mux(PB12, PB12_BT3_OUT);					//PB12 作为BT3 PWM输出引脚
//	csi_pin_set_mux(PD2, PD2_BT3_OUT);					    //PD2 作为BT3 PWM输出引脚
	
	//init timer pwm para config
	tPwmCfg.byIdleLevel = BT_PWM_IDLE_HIGH;					//PWM 输出空闲电平
	tPwmCfg.byStartLevel= BT_PWM_START_HIGH;				//PWM 输出起始电平
	tPwmCfg.byDutyCycle = 50;								//PWM 输出占空比(0 < DutyCycle < 100)		
	tPwmCfg.wFreq 		= 50;								//PWM 输出频率
	//tPwmCfg.byInter 	= BT_INTSRC_PEND | BT_INTSRC_CMP;	//PWM 中断配置(PEND and CMP)
	tPwmCfg.byInter		= BT_INTSRC_NONE;
	
//	csi_bt_pwm_init(BT0, &tPwmCfg);							//初始化BT0 PWM输出
//	csi_bt_start(BT0);										//启动BT0
	
	csi_bt_pwm_init(BT1, &tPwmCfg);							//初始化BT1 PWM输出
	csi_bt_start(BT1);										//启动BT1
	
//	csi_bt_pwm_init(BT2, &tPwmCfg);							//初始化BT2 PWM输出
//	csi_bt_start(BT2);										//启动BT2
//	
//	csi_bt_pwm_init(BT3, &tPwmCfg);							//初始化BT3 PWM输出
//	csi_bt_start(BT3);										//启动BT3
	while(1)
	{
		NOP;
		bSyncmd_Flag ^= 1;
		if(bSyncmd_Flag == 0)
		{
			BT1->CMP = 1000;
			Delay_Nms(500);
			csi_pin_set_high(PA6);
		}
		else
		{
			BT1->CMP = 500;
			Delay_Nms(500);
			csi_pin_set_low(PA6);
		}
	}
}

/** \brief bt sync trg start: 外部中断触发BT0~BT3 启动
 *  \param[in] none
 *  \return error code
 */
int bt_sync_start_test(void)
{
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				               			//ETB 参数配置结构体		
	
	csi_pin_set_mux(PB1, PB1_INPUT);									//PB1 配置为输入
	csi_pin_pull_mode(PB1, GPIO_PULLUP);								//PB1 上拉
	csi_pin_irq_mode(PB1, EXI_GRP1, GPIO_IRQ_FALLING_EDGE);			    //PB1 下降沿产生中断，选择中断组1
	csi_pin_irq_enable(PB1, ENABLE);							//PB1 中断使能	
	csi_exi_set_evtrg(1, TRGSRC_EXI1, 0);						        //EXI1 触发EXI_TRGOUT0
	
	
	csi_pin_set_mux(PA6, PA6_OUTPUT);		//PA6 output ，并在BT中断里面翻转IO
	csi_pin_set_high(PA6);					//PA6 output high;
	csi_pin_set_mux(PA5, PA5_OUTPUT);		//PA5 output ，并在BT中断里面翻转IO
	csi_pin_set_high(PA5);			
	
	csi_bt_timer_init(BT0,1000);										//BT 定时
	csi_bt_set_sync(BT0, BT_TRGIN_SYNCEN0, BT_TRG_ONCE, BT_TRG_SYCAREARM);	//外部触发bt启动(SYNCIN0)
	
//	csi_bt_timer_init(BT1,2000);										//BT 定时
//	csi_bt_set_sync(BT1, BT_TRGIN_SYNCEN0, BT_TRG_ONCE, BT_TRG_AUTOAREARM);	//外部触发bt启动(SYNCIN0)
	
//	csi_bt_timer_init(BT2,3000);										//BT 定时
//	csi_bt_set_sync(BT2, BT_TRGIN_SYNCEN0, BT_TRG_ONCE, BT_TRG_AUTOAREARM);	//外部触发bt启动(SYNCIN0)
	
//	csi_bt_timer_init(BT3,4000);										//BT 定时
//	csi_bt_set_sync(BT3, BT_TRGIN_SYNCEN0, BT_TRG_ONCE, BT_TRG_AUTOAREARM);	//外部触发bt启动(SYNCIN0)
		
	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  		//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_EXI_TRGOUT1;  	    //EXI_TRGOUT1作为触发源
	
	tEtbConfig.byDstIp =  ETB_BT0_SYNCIN0;   	    //BT0 同步输入作为目标事件
//	tEtbConfig.byDstIp =  ETB_BT1_SYNCIN0;   	    //BT1 同步输入作为目标事件
//	tEtbConfig.byDstIp =  ETB_BT2_SYNCIN0;   	    //BT2 同步输入作为目标事件
//	tEtbConfig.byDstIp =  ETB_BT3_SYNCIN0;   	    //BT3 同步输入作为目标事件
	
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
   
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
//	csi_bt_start(BT0);
//	csi_etb_ch_start(ch);
	while(1)
	{
		NOP;
		BT0->PRDR = 3000;
		if(BT0->CR & 0x10000000)
		{
			Delay_Nms(200);
			csi_bt_stop(BT0);
			BT0->CR |= 0x10000000;//清除一次方可再触发，测试REARMx和OSTMDx位
		}
		
//		if(BT1->CR & 0x10000000)
//		{
//			Delay_Nms(200);
//			csi_bt_stop(BT1);
//			BT1->CR |= 0x10000000;//清除一次方可再触发，测试REARMx和OSTMDx位
//		}
		
//		if(BT2->CR & 0x10000000)
//		{
//			Delay_Nms(200);
//			csi_bt_stop(BT2);
//			BT2->CR |= 0x10000000;//清除一次方可再触发，测试REARMx和OSTMDx位
//		}
		
//		if(BT3->CR & 0x10000000)
//		{
//			Delay_Nms(200);
//			csi_bt_stop(BT3);
//			BT3->CR |= 0x10000000;//清除一次方可再触发，测试REARMx和OSTMDx位
//		}
	}
	return iRet;
}


/** \brief bt sync trg count: BT0~BT3选一个做PWM输出，触发另外一个BT Count
 * Example:BT1 输出PWM触发EXI，再通过BT0同步输入端口1触发BT0计数值加1
 *  \param[in] none
 *  \return error code
 */
int bt_sync_count_test(void)
{
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				            //ETB 参数配置结构体
	csi_bt_pwm_config_t tPwmCfg;							//BT PWM输出参数初始化配置结构体
	
	
	
	csi_pin_set_mux(PB1, PB1_INPUT);									//PB1 配置为输入
	csi_pin_pull_mode(PB1, GPIO_PULLUP);								//PB1 上拉
	csi_pin_irq_mode(PB1, EXI_GRP1, GPIO_IRQ_FALLING_EDGE);			    //PB1 下降沿产生中断，选择中断组1
	csi_pin_irq_enable(PB1,ENABLE);							//PB1 中断使能
	csi_exi_set_evtrg(1, TRGSRC_EXI1, 1);					            //EXI1(PB1) 触发EXI_TRGOUT1(PB1用EXI1触发输出)
	
	csi_pin_set_mux(PA6, PA6_OUTPUT);		                            //PA6 output ，并在BT中断里面翻转IO
	csi_pin_set_high(PA6);					                            //PA6 output high;		
	
	//BT0 初始化
//	csi_bt_timer_init(BT0,5);											    //BT定时
//	csi_bt_set_sync(BT0, BT_TRGIN_SYNCEN2, BT_TRG_CONTINU, BT_TRG_AUTOAREARM);		//外部触发bt0计数(SYNCIN1)

	//BT1 初始化
//	csi_bt_timer_init(BT1,5);											    //BT定时
//	csi_bt_set_sync(BT1, BT_TRGIN_SYNCEN2, BT_TRG_CONTINU, BT_TRG_AUTOAREARM);		//外部触发bt1计数(SYNCIN1)
	
	//BT2 初始化
//	csi_bt_timer_init(BT2,5);											    //BT定时
//	csi_bt_set_sync(BT2, BT_TRGIN_SYNCEN2, BT_TRG_CONTINU, BT_TRG_AUTOAREARM);		//外部触发bt2计数(SYNCIN1)
	
	//BT3 初始化
	csi_bt_timer_init(BT1,500);											    //BT定时
	csi_bt_set_sync(BT1, BT_TRGIN_SYNCEN2, BT_TRG_CONTINU, BT_TRG_AUTOAREARM);		//外部触发bt3计数(SYNCIN1)
	
	// ETCB 初始化
	tEtbConfig.byChType  = ETB_ONE_TRG_ONE;  		//单个源触发单个目标
	tEtbConfig.bySrcIp   = ETB_EXI_TRGOUT1;  	    //EXI_TRGOUT1作为触发源
//	tEtbConfig.byDstIp   = ETB_BT0_SYNCIN2;   	    //BT0 同步输入作为目标事件
	tEtbConfig.byDstIp   = ETB_BT1_SYNCIN2;   	    //BT1 同步输入作为目标事件
//	tEtbConfig.byDstIp   = ETB_BT2_SYNCIN2;   	    //BT2 同步输入作为目标事件
//	tEtbConfig.byDstIp   = ETB_BT3_SYNCIN2;   	    //BT3 同步输入作为目标事件
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
	
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
	csi_pin_set_mux(PA0,  PA0_BT0_OUT);					    //PA0  作为BT0 PWM输出引脚
	//init timer pwm para config
	tPwmCfg.byIdleLevel = BT_PWM_IDLE_HIGH;					//PWM 输出空闲电平
	tPwmCfg.byStartLevel= BT_PWM_START_HIGH;				//PWM 输出起始电平
	tPwmCfg.byDutyCycle = 50;								//PWM 输出占空比(0 < DutyCycle < 100)		
	tPwmCfg.wFreq 		= 50;								//PWM 输出频率
	//tPwmCfg.byInter 	= BT_INTSRC_PEND | BT_INTSRC_CMP;	//PWM 中断配置(PEND and CMP)
	tPwmCfg.byInter		= BT_INTSRC_NONE;	
	csi_bt_pwm_init(BT0, &tPwmCfg);							//初始化BT0 PWM输出
	csi_bt_start(BT0);						
	
//	csi_pin_set_mux(PC11, PC11_BT1_OUT);					//PC11 作为BT1 PWM输出引脚
//	//init timer pwm para config
//	tPwmCfg.byIdleLevel = BT_PWM_IDLE_HIGH;					//PWM 输出空闲电平
//	tPwmCfg.byStartLevel= BT_PWM_START_HIGH;				//PWM 输出起始电平
//	tPwmCfg.byDutyCycle = 50;								//PWM 输出占空比(0 < DutyCycle < 100)		
//	tPwmCfg.wFreq 		= 50;								//PWM 输出频率
//	//tPwmCfg.byInter 	= BT_INTSRC_PEND | BT_INTSRC_CMP;	//PWM 中断配置(PEND and CMP)
//	tPwmCfg.byInter		= BT_INTSRC_NONE;	
//	csi_bt_pwm_init(BT1, &tPwmCfg);							//初始化BT1 PWM输出
//	csi_bt_start(BT1);										//启动BT1
	
//	csi_bt_start(BT0);
	csi_bt_start(BT1);
//	csi_bt_start(BT2);
//	csi_bt_start(BT3);
	
	while(1)
	{
//		csi_etb_ch_swtrg(0x08);
		NOP;
		Delay_Nms(100);
//		BT0->CR |= (1 << 30);
	}
	return iRet;
}


/** \brief bt sync trg stop: 外部中断触发BT0~BT3 关闭
 *  \param[in] none
 *  \return error code
 */
int bt_sync_stop_test(void)
{
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				               			//ETB 参数配置结构体		
	
	csi_pin_set_mux(PB1, PB1_INPUT);									//PB1 配置为输入
	csi_pin_pull_mode(PB1, GPIO_PULLUP);								//PB1 上拉
	csi_pin_irq_mode(PB1, EXI_GRP1, GPIO_IRQ_FALLING_EDGE);			    //PB1 下降沿产生中断，选择中断组1
	csi_pin_irq_enable(PB1, ENABLE);							//PB1 中断使能	
	csi_exi_set_evtrg(1, TRGSRC_EXI1, 0);						        //EXI1 触发EXI_TRGOUT0
	
	
	csi_pin_set_mux(PA6, PA6_OUTPUT);		//PA6 output ，并在BT中断里面翻转IO
	csi_pin_set_high(PA6);					//PA6 output high;		
	
//	csi_bt_timer_init(BT0,1000);									//BT 定时
//	csi_bt_set_sync(BT0, BT_TRGIN_SYNCEN1, BT_TRG_ONCE, BT_TRG_AUTOAREARM);	//外部触发bt启动(SYNCIN0)
	
//	csi_bt_timer_init(BT1,2000);									//BT 定时
//	csi_bt_set_sync(BT1, BT_TRGIN_SYNCEN1, BT_TRG_ONCE, BT_TRG_AUTOAREARM);	//外部触发bt启动(SYNCIN0)
	
//	csi_bt_timer_init(BT2,3000);										//BT 定时
//	csi_bt_set_sync(BT2, BT_TRGIN_SYNCEN1, BT_TRG_ONCE, BT_TRG_AUTOAREARM);	//外部触发bt启动(SYNCIN0)
	
	csi_bt_timer_init(BT3,4000);										//BT 定时
	csi_bt_set_sync(BT3, BT_TRGIN_SYNCEN1, BT_TRG_CONTINU, BT_TRG_AUTOAREARM);	//外部触发bt启动(SYNCIN0)
		
	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  		//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_EXI_TRGOUT1;  	    //EXI_TRGOUT1作为触发源
//	tEtbConfig.byDstIp =  ETB_BT0_SYNCIN1;   	    //BT0 同步输入作为目标事件
//	tEtbConfig.byDstIp =  ETB_BT1_SYNCIN1;   	    //BT1 同步输入作为目标事件
//	tEtbConfig.byDstIp =  ETB_BT2_SYNCIN1;   	    //BT2 同步输入作为目标事件
	tEtbConfig.byDstIp =  ETB_BT3_SYNCIN1;   	    //BT3 同步输入作为目标事件
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
   
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
//	csi_bt_start(BT0);
//	csi_bt_start(BT1);
//	csi_bt_start(BT2);
	csi_bt_start(BT3);
//	Delay_Nms(100);
//	csi_bt_stop(BT3);
//	csi_etb_ch_start(ch);
	while(1)
	{
		NOP;
//		if(BT0->CR & 0x20000000)
//		{
//			Delay_Nms(100);
//			csi_bt_start(BT0);
//			BT0->CR |= 0x20000000;//清除一次方可再触发，测试REARMx和OSTMDx位
//		}
		
//		if(BT1->CR & 0x20000000)
//		{
//			Delay_Nms(100);
//			csi_bt_start(BT1);
//			BT1->CR |= 0x20000000;//清除一次方可再触发，测试REARMx和OSTMDx位
//		}
		
//		if(BT2->CR & 0x20000000)
//		{
//			Delay_Nms(100);
//			csi_bt_start(BT2);
//			BT2->CR |= 0x20000000;//清除一次方可再触发，测试REARMx和OSTMDx位
//		}
		
//		if(BT3->CR & 0x20000000)
//		{
//			Delay_Nms(100);
//			csi_bt_start(BT3);
//			BT3->CR |= 0x20000000;//清除一次方可再触发，测试REARMx和OSTMDx位
//		}
	}
	return iRet;
}


/** \brief bt sync start and stop: 用两个外部中断触发BT0开启和关闭，测试AREARM0和AREARM1位
 *  \param[in] none
 *  \return error code
 */
int bt_sync_startstop_test(void)
{
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				               			//ETB 参数配置结构体		
	
	//Start 按键
	csi_pin_set_mux(PB1, PB1_INPUT);									//PB1 配置为输入
	csi_pin_pull_mode(PB1, GPIO_PULLUP);								//PB1 上拉
	csi_pin_irq_mode(PB1, EXI_GRP1, GPIO_IRQ_FALLING_EDGE);			    //PB1 下降沿产生中断，选择中断组1
	csi_pin_irq_enable(PB1, ENABLE);							//PB1 中断使能	
	csi_exi_set_evtrg(1, TRGSRC_EXI1, 0);						        //EXI1 触发EXI_TRGOUT1
	
	//Stop 按键
	csi_pin_set_mux(PB2, PB2_INPUT);									//PB2 配置为输入
	csi_pin_pull_mode(PB2, GPIO_PULLUP);								//PB2 上拉
	csi_pin_irq_mode(PB2, EXI_GRP2, GPIO_IRQ_FALLING_EDGE);			    //PB2 下降沿产生中断，选择中断组1
	csi_pin_irq_enable(PB2, ENABLE);							//PB2 中断使能	
	csi_exi_set_evtrg(2, TRGSRC_EXI2, 0);						        //EXI2 触发EXI_TRGOUT2
	
	
	csi_pin_set_mux(PA6, PA6_OUTPUT);		//PA6 output ，并在BT中断里面翻转IO
	csi_pin_set_high(PA6);					//PA6 output high;
	
	csi_bt_timer_init(BT0,1000);										    //BT 定时
	csi_bt_set_sync(BT0, BT_TRGIN_SYNCEN0, BT_TRG_ONCE, BT_TRG_SYCAREARM);	//外部触发bt启动(SYNCIN0)		
	BT0->CR |= (1<<9) | (2<<20) | (1<<25);
	
	
	//Start
	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  		//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_EXI_TRGOUT1;  	    //EXI_TRGOUT1作为触发源
	tEtbConfig.byDstIp =  ETB_BT0_SYNCIN0;   	    //BT0 同步输入作为目标事件	
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
	//Stop
	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  		//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_EXI_TRGOUT2;  	    //EXI_TRGOUT1作为触发源
	tEtbConfig.byDstIp =  ETB_BT0_SYNCIN1;   	    //BT0 同步输入作为目标事件	
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
	while(1)
	{
		NOP;
	}
	return iRet;
}


/** \brief bt evtrg out: 用一个BT触发另外一个BT工作
 *  \param[in] none
 *  \return error code
 */
int bt_evtrg_test(void)
{
	bool byFlag = 0;
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				               		//ETB 参数配置结构体		
	
	csi_pin_set_mux(PA6, PA6_OUTPUT);								//PA6 output ，并在BT中断里面翻转IO
	csi_pin_set_low(PA6);											//PA6 output high;
	csi_pin_set_mux(PA5, PA5_OUTPUT);								//PA5 output 
	csi_pin_set_low(PA5);		

//	csi_bt_timer_init(BT0,10000);									//BT 定时
////	BT0->CR |= (1<<16);                                         //用来测试BT_TRGSRC_OVF触发
//	csi_bt_set_evtrg(BT0, BT_TRGSRC_PEND,ENABLE);					//BT1 PEND事件触发输出
//	csp_bt_int_set(BT0, BT_PEND_INT, DISABLE);					    //disable PEND interrupt
//	csp_bt_int_set(BT0, BT_EVTRG_INT, ENABLE);					    //enable EVTRG interrupt
//	csi_irq_enable((uint32_t *)BT0);								//enable bt1 irq
	
//	csi_bt_timer_init(BT1,10000);									//BT 定时
////	BT1->CR |= (1<<16);                                         //用来测试BT_TRGSRC_OVF触发
//	csi_bt_set_evtrg(BT1, BT_TRGSRC_PEND,ENABLE);					//BT1 PEND事件触发输出
//	csp_bt_int_set(BT1, BT_PEND_INT, DISABLE);						//disable PEND interrupt
//	csp_bt_int_set(BT1, BT_EVTRG_INT, ENABLE);						//enable EVTRG interrupt
//	csi_irq_enable((uint32_t *)BT1);								//enable bt1 irq

//	csi_bt_timer_init(BT2,10000);									//BT 定时
////	BT2->CR |= (1<<16);                                         //用来测试BT_TRGSRC_OVF触发
//	csi_bt_set_evtrg(BT2, BT_TRGSRC_CMP,ENABLE);					//BT1 PEND事件触发输出
//	csp_bt_int_set(BT2, BT_CMP_INT, DISABLE);						//disable PEND interrupt
//	csp_bt_int_set(BT2, BT_EVTRG_INT, ENABLE);						//enable EVTRG interrupt
//	csi_irq_enable((uint32_t *)BT2);								//enable bt1 irq
	
	csi_bt_timer_init(BT3,10000);									//BT 定时
//	BT3->CR |= (1<<16);                                             //用来测试BT_TRGSRC_OVF触发
	csi_bt_set_evtrg(BT3, BT_TRGSRC_PEND,ENABLE);					//BT1 PEND事件触发输出
	csp_bt_int_set(BT3, BT_PEND_INT, DISABLE);						//disable PEND interrupt
	csp_bt_int_set(BT3, BT_EVTRG_INT, ENABLE);						//enable EVTRG interrupt
	csi_irq_enable((uint32_t *)BT3);								//enable bt1 irq
//	BT3->CR |= (1<<4);											    //用来测试ICR、MISR寄存器EVTRG interrupt置位情况
	
	csi_bt_timer_init(BT0,1000);										//BT 定时
	csi_bt_set_sync(BT0, BT_TRGIN_SYNCEN0, BT_TRG_ONCE, BT_TRG_SYCAREARM);	//外部触发bt启动(SYNCIN0)
	
//	csi_bt_timer_init(BT1,2000);										//BT 定时
//	csi_bt_set_sync(BT1, BT_TRGIN_SYNCEN0, BT_TRG_ONCE, BT_TRG_AUTOAREARM);	//外部触发bt启动(SYNCIN0)
	
//	csi_bt_timer_init(BT2,3000);										//BT 定时
//	csi_bt_set_sync(BT2, BT_TRGIN_SYNCEN0, BT_TRG_ONCE, BT_TRG_AUTOAREARM);	//外部触发bt启动(SYNCIN0)
	
//	csi_bt_timer_init(BT3,4000);										//BT 定时
//	csi_bt_set_sync(BT3, BT_TRGIN_SYNCEN0, BT_TRG_ONCE, BT_TRG_AUTOAREARM);	//外部触发bt启动(SYNCIN0)
		
	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  		//单个源触发单个目标
//	tEtbConfig.bySrcIp  = ETB_BT0_TRGOUT;  	        //BT0_TRGOUT作为触发源
//	tEtbConfig.bySrcIp  = ETB_BT1_TRGOUT;  	        //BT1_TRGOUT作为触发源
//	tEtbConfig.bySrcIp  = ETB_BT2_TRGOUT;  	        //BT2_TRGOUT作为触发源
	tEtbConfig.bySrcIp  = ETB_BT3_TRGOUT;  	        //BT3_TRGOUT作为触发源
	
	tEtbConfig.byDstIp =  ETB_BT0_SYNCIN0;   	    //BT0 同步输入作为目标事件
//	tEtbConfig.byDstIp =  ETB_BT1_SYNCIN0;   	    //BT1 同步输入作为目标事件
//	tEtbConfig.byDstIp =  ETB_BT2_SYNCIN0;   	    //BT2 同步输入作为目标事件
//	tEtbConfig.byDstIp =  ETB_BT3_SYNCIN0;   	    //BT3 同步输入作为目标事件
	
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
   
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
	while(1)
	{
		NOP;
		if(byFlag == 0) //启动一次
		{
			byFlag = 1;
			csi_pin_set_high(PA5);	
//			csi_bt_start(BT0);
//			csi_bt_soft_evtrg(BT0);
			
//			csi_bt_start(BT1);
//			csi_bt_soft_evtrg(BT1);

//			csi_bt_start(BT2);
//			csi_bt_soft_evtrg(BT2);

			csi_bt_start(BT3);
//			csi_bt_soft_evtrg(BT3);
		}
	}
	return iRet;
}

/** \brief bt_syncmd_start_test: 软件触发BT事件
 *  \param[in] none
 *  \return error code
 */
int bt_syncmd_start_test(void)
{
	bool byExiFlag = 0;
	
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				               			//ETB 参数配置结构体		
	csi_bt_pwm_config_t tPwmCfg;										//BT PWM输出参数初始化配置结构体
	
	
//	csi_pin_set_mux(PB1, PB1_INPUT);									//PB1 配置为输入
//	csi_pin_pull_mode(PB1, GPIO_PULLUP);								//PB1 上拉
//	csi_pin_irq_mode(PB1, EXI_GRP1, GPIO_IRQ_FALLING_EDGE);			    //PB1 下降沿产生中断，选择中断组1
//	csi_pin_irq_enable(PB1,EXI_GRP1, ENABLE);							//PB1 中断使能	
//	csi_exi_set_evtrg(1, TRGSRC_EXI1, 0);						        //EXI1 触发EXI_TRGOUT0
	
	
	csi_pin_set_mux(PA6, PA6_OUTPUT);		//PA6 output ，并在BT中断里面翻转IO
	csi_pin_set_high(PA6);					//PA6 output high;
	csi_pin_set_mux(PA5, PA5_OUTPUT);		//PA5 output ，并在BT中断里面翻转IO
	csi_pin_set_low(PA5);		
	
	csi_bt_timer_init(BT0,10000);									//BT 定时
//	BT0->CR |= (1<<16);                                         //用来测试BT_TRGSRC_OVF触发
	csi_bt_set_evtrg(BT0, BT_TRGSRC_PEND,ENABLE);					//BT1 PEND事件触发输出
	csp_bt_int_set(BT0, BT_PEND_INT, DISABLE);					    //disable PEND interrupt
	csp_bt_int_set(BT0, BT_EVTRG_INT, ENABLE);					    //enable EVTRG interrupt
	csi_irq_enable((uint32_t *)BT0);								//enable bt1 irq

	csi_pin_set_mux(PA1,  PA1_BT1_OUT);					    //PA1  作为BT1 PWM输出引脚
	//init timer pwm para config
	tPwmCfg.byIdleLevel = BT_PWM_IDLE_HIGH;					//PWM 输出空闲电平
	tPwmCfg.byStartLevel= BT_PWM_START_HIGH;				//PWM 输出起始电平
	tPwmCfg.byDutyCycle = 50;								//PWM 输出占空比(0 < DutyCycle < 100)		
	tPwmCfg.wFreq 		= 50;								//PWM 输出频率
	//tPwmCfg.byInter 	= BT_INTSRC_PEND | BT_INTSRC_CMP;	//PWM 中断配置(PEND and CMP)
	tPwmCfg.byInter		= BT_INTSRC_NONE;
	
	csi_bt_pwm_init(BT1, &tPwmCfg);							//初始化BT1 PWM输出
	csi_bt_set_sync(BT1, BT_TRGIN_SYNCEN0, BT_TRG_CONTINU, BT_TRG_SYCAREARM);	//外部触发bt启动(SYNCIN0)
//	BT0->CR |= (1<<15) ;
//	BT0->CR |=  0x06;
	
	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  		//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_BT0_TRGOUT;  	    	//EXI_TRGOUT1作为触发源
	tEtbConfig.byDstIp =  ETB_BT1_SYNCIN0;   	    //BT0 同步输入作为目标事件
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
   
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
	csi_bt_start(BT1);
//	csi_etb_ch_start(ch);
	while(1)
	{
		NOP;
		byExiFlag ^= 1;	
		if(byExiFlag == 0)
		{
			
			csi_pin_set_high(PA5);
//			BT1->PRDR =  1000;
			BT1->CMP  =  1000;
			csi_bt_soft_evtrg(BT0);
			csi_pin_set_low(PA5);
			Delay_Nms(300);
		}
		else
		{
			
			csi_pin_set_high(PA5);
			BT1->CMP =  5000; 
			csi_bt_soft_evtrg(BT0);
			csi_pin_set_low(PA5);
			Delay_Nms(300);
		}
	}
	return iRet;
}



/** \brief bt_irqhandler: BT0 中断处理示例函数
 *  \param[in] csp_bt_t
 *  \return error code
 */
__attribute__((weak)) void bt_irqhandler0(csp_bt_t *ptBtBase)
{
    // ISR content ...
	volatile uint32_t wMisr = csp_bt_get_isr(ptBtBase);
	
	if(wMisr & BT_PEND_INT)					//PEND interrupt
	{
		csp_bt_clr_isr(ptBtBase, BT_PEND_INT);
//		csi_pin_toggle(PA6);				//PA06 toggle	
	}
	if(wMisr & BT_CMP_INT)					//CMP interrupt
	{
		csp_bt_clr_isr(ptBtBase, BT_CMP_INT);
//		csi_pin_set_low(PA5);
//		csi_pin_set_high(PA6);	
	}
	if(wMisr & BT_EVTRG_INT)				//EVTRG interrupt
	{
//		csi_pin_set_low(PA5);
		csp_bt_clr_isr(ptBtBase, BT_EVTRG_INT);
	}
}

/** \brief bt_irqhandler: BT1 中断处理示例函数
 *  \param[in] csp_bt_t
 *  \return error code
 */
__attribute__((weak)) void bt_irqhandler1(csp_bt_t *ptBtBase)
{
    // ISR content ...
	volatile uint32_t wMisr = csp_bt_get_isr(ptBtBase);
	
	if(wMisr & BT_PEND_INT)					//PEND interrupt
	{
		csp_bt_clr_isr(ptBtBase, BT_PEND_INT);
//		csi_pin_toggle(PA6);				//PA06 toggle	
	}
	
	if(wMisr & BT_CMP_INT)					//CMP interrupt
	{
		csp_bt_clr_isr(ptBtBase, BT_CMP_INT);
//		csi_pin_set_low(PA5);
//		csi_pin_set_high(PA6);	
	}
	if(wMisr & BT_EVTRG_INT)				//EVTRG interrupt
	{
//		csi_pin_set_low(PA5);
		csp_bt_clr_isr(ptBtBase, BT_EVTRG_INT);
	}
}

/** \brief bt_irqhandler: BT2 中断处理示例函数
 *  \param[in] csp_bt_t
 *  \return error code
 */
__attribute__((weak)) void bt_irqhandler2(csp_bt_t *ptBtBase)
{
    // ISR content ...
	volatile uint32_t wMisr = csp_bt_get_isr(ptBtBase);
	
	if(wMisr & BT_PEND_INT)					//PEND interrupt
	{
		csp_bt_clr_isr(ptBtBase, BT_PEND_INT);
//		csi_pin_toggle(PA6);				//PA06 toggle	
	}
	if(wMisr & BT_CMP_INT)					//CMP interrupt
	{
		csp_bt_clr_isr(ptBtBase, BT_CMP_INT);
//		csi_pin_set_low(PA5);
//		csi_pin_set_high(PA6);	
	}
	if(wMisr & BT_EVTRG_INT)				//EVTRG interrupt
	{
//		csi_pin_set_low(PA5);
		csp_bt_clr_isr(ptBtBase, BT_EVTRG_INT);
	}
}

