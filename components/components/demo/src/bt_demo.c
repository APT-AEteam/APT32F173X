/***********************************************************************//** 
 * \file  timer_demo.c
 * \brief  TIMER_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-7-12 <td>V0.0 <td>ZJY     <td>initial
 * <tr><td> 2023-9-19 <td>V0.1 <td>ZJY     <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include "csi_drv.h"
#include "board_config.h"

/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/

/* Private variablesr------------------------------------------------------*/

				
#if (USE_BT_CALLBACK == 0)		
	
/** \brief	btx_int_handler: BT中断服务函数
 * 
 *  \brief 	BT发生中断时会调用此函数，函数在interrupt.c里定义为弱(weak)属性，默认不做处理；用户用到BT中
 * 			断时，请重新定义此函数，在此函数中进行对应中断处理，也可直接在interrupt.c里的函数里进行处理
 * 
 *  \param[in] none
 *  \return none
 */
ATTRIBUTE_ISR  void bt1_int_handler(void)
{
	//用户直接在中断服务接口函数里处理中断，建议客户使用此模式
	volatile uint32_t byIsr = csp_bt_get_isr(BT1);
	
	if(byIsr & BT_INT_PEND)					//PEND 中断
	{
		csp_bt_clr_isr(BT1, BT_INT_PEND);
		csi_gpio_toggle(GPIOA, PA6);		//PA6翻转
	}
	
	if(byIsr & BT_INT_CMP)					//CMP 中断
	{
		csp_bt_clr_isr(BT1, BT_INT_CMP);	
	}
	
	if(byIsr & BT_INT_OVF)					//OVF 中断
	{
		csp_bt_clr_isr(BT1, BT_INT_OVF);
	}
	
	if(byIsr & BT_INT_EVTRG)				//EVTRG 中断
	{
		csp_bt_clr_isr(BT1, BT_INT_EVTRG);
	}
}

#endif

/** \brief	bt_timer_demo: BT做基本定时器功能，默认使用PEND中断
 * 
 *  \brief	csi初始化里不开启中断，需要进中断函数，需调用csi_bt_int_enable接口；demo默认使用PEND中断，并在中断
 * 			里面翻转IO(需要打开PA6 IO配置注释)；若不需要开启中断，调用csi_bt_int_disable接口，关闭周期结束中断
 * 
 * @ 工作模式:	BT_RUN_CONT: 连续工作模式, 指计数结束，计数器重新开始计数，周期执行
 * 				BT_RUN_ONCE: 单次工作模式，指计数结束，计数器停止工作
 * 
 *  \param[in] none
 *  \return error code
 */
int bt_timer_demo(void)
{
	int iRet = 0;
	csi_bt_time_config_t tTimConfig= {0};
	
#if (USE_GUI == 0)		
	csi_gpio_set_mux(GPIOA,PA6, PA6_OUTPUT);	//初始化PA6为输出
#endif
	
	tTimConfig.wTimeVal = 1000;					//BT定时值 = 1000us
	tTimConfig.eRunMode  = BT_RUN_CONT;			//BT计数器工作模式
	csi_bt_timer_init(BT1, &tTimConfig);		//初始化BT	
	
	csi_bt_int_enable(BT1,BT_INTSRC_PEND);		//使能BT PEND中断
	csi_bt_start(BT1);							//启动BT定时器,定时默认需使用PEND中断
		
	while(1)
	{
		nop;
	}
	return iRet;
}

/** \brief	bt_pwm_demo: BT做PWM输出demo，默认不开启中断
 * 
 * @ 工作模式:	BT_RUN_CONT: 连续工作模式, 指计数结束，计数器重新开始计数，周期执行
 * 				BT_RUN_ONCE: 单次工作模式，指计数结束，计数器停止工作
 *  
 *  \param[in] none
 *  \return error code
 */
int bt_pwm_demo(void)
{
	int iRet = 0;
	csi_bt_pwm_config_t tPwmConfig= {0};					//BT PWM输出参数初始化配置结构体

#if (USE_GUI == 0)		
	csi_gpio_set_mux(GPIOC, PC11, PC11_BT1_OUT);	//PC11 作为BT1 PWM输出引脚
#endif
	
	//init timer pwm para config
	tPwmConfig.eIdleLevel 	= BT_PWM_IDLE_HIGH;		//PWM 输出空闲电平
	tPwmConfig.eStartLevel	= BT_PWM_START_HIGH;	//PWM 输出起始电平
	tPwmConfig.eRunMode  	= BT_RUN_CONT;			//BT计数器工作模式：连续/单次
	tPwmConfig.byDutyCycle 	= 90;					//PWM 输出占空比(0 < DutyCycle < 100)		
	tPwmConfig.wFreq 		= 50;					//PWM 输出频率
	csi_bt_pwm_init(BT1, &tPwmConfig);				//初始化BT1 PWM输出
	
	csi_bt_start(BT1);								//启动BT1
	
	while(1)
	{
		nop;
	}
	
	return iRet;
}

#if (USE_BT_CALLBACK == 0)	
	
/** \brief	btx_int_handler: BT中断服务函数
 * 
 *  \brief 	BT发生中断时会调用此函数，函数在interrupt.c里定义为弱(weak)属性，默认不做处理；用户用到BT中
 * 			断时，请重新定义此函数，在此函数中进行对应中断处理，也可直接在interrupt.c里的函数里进行处理
 * 
 *  \param[in] none
 *  \return none
 */
ATTRIBUTE_ISR  void bt0_int_handler(void)
{
	//用户直接在中断服务接口函数里处理中断，建议客户使用此模式
	volatile uint8_t byIsr = csp_bt_get_isr(BT0);
	
	if(byIsr & BT_INT_PEND)					//PEND 中断
	{
		csp_bt_clr_isr(BT0, BT_INT_PEND);
		csi_gpio_toggle(GPIOA, PA6);		//PA6翻转
	}
	
	if(byIsr & BT_INT_CMP)					//CMP 中断
	{
		csp_bt_clr_isr(BT0, BT_INT_CMP);	
	}
	
	if(byIsr & BT_INT_OVF)					//OVF 中断
	{
		csp_bt_clr_isr(BT0, BT_INT_OVF);
	}
	
	if(byIsr & BT_INT_EVTRG)				//EVTRG 中断
	{
		csp_bt_clr_isr(BT0, BT_INT_EVTRG);
	}
}
#endif

/** \brief	bt sync trg count: EXI同步触发BT计数值加1，即同步触发信号作为BT的计数时钟节拍
 * 
 *  \brief	EXI通过BT1同步输入端口2触发BT1计数值加1，即用PB01的下降沿通过ETCB触发BT1计数；开启BT0的PWM输出，将
 * 			PWM信号接入PB01,作为PB01的驱动源，即PA0作为输出信号 连接到输入管脚PB01。
 * 
 *  \param[in] none
 *  \return error code
 */
int bt_sync_trg_count_demo(void)
{
	int iRet = 0;
	volatile uint8_t ch = 0;
	csi_etcb_config_t tEtbConfig= {0};				            				//ETCB 参数配置结构体
	csi_bt_pwm_config_t tPwmCfg= {0};											//BT PWM输出参数初始化配置结构体
	csi_bt_time_config_t tTimConfig= {0};										//BT 定时初始化参数结构体	

#if (USE_GUI == 0)			
	csi_gpio_set_mux(GPIOB, PB1, PB1_INPUT);								//PB1 配置为输入
	csi_gpio_pull_mode(GPIOB, PB1, GPIO_PULLUP);							//PB1 上拉
	csi_gpio_int_enable(GPIOB, PB1);										//PB1 中断使能	
	csi_gpio_irq_mode(GPIOB, PB1, EXI_GRP1, GPIO_IRQ_FALLING_EDGE);			//PB1 下降沿产生中断，选择中断组1
	csi_exi_set_evtrg(EXI_TRGOUT1, EXI_TRGSRC_GRP1, 0);						//EXI 触发配置
	csi_exi_evtrg_enable(EXI_TRGOUT1);										//使能 EXI_TRGOUT1触发输出
	
	csi_gpio_set_mux(GPIOA, PA0, PA0_BT0_OUT);								//PA0  作为BT0 PWM输出引脚
	csi_gpio_set_mux(GPIOA, PA6, PA6_OUTPUT);								//PA6 output ，并在BT0中断里面翻转IO
	csi_gpio_set_high(GPIOA, PA6);											//PA6 output high;	
#endif	
	
	//BT1 初始化
	tTimConfig.wTimeVal = 100;												//BT定时值 = 100us
	tTimConfig.eRunMode  = BT_RUN_CONT;										//BT计数器工作模式
	csi_bt_timer_init(BT1,&tTimConfig);										//BT1 定时	
	csi_bt_set_sync(BT1, BT_SYNCIN2, BT_SYNC_CONT, BT_TRG_AUTOAREARM);		//外部触发BT1计数(SYNCIN2)
	csi_bt_sync_enable(BT1, BT_SYNCIN2);									//BT1 同步输入2使能
	
	csi_bt_start(BT1);	    			 									//启动BT1
	
	// ETCB 初始化
	tEtbConfig.eChType  = ETCB_ONE_TRG_ONE;  		//单个源触发单个目标
	tEtbConfig.eSrcIp   = ETCB_EXI_TRGOUT1;  	    //EXI_TRGOUT1作为触发源
	tEtbConfig.eDstIp   = ETCB_BT1_SYNCIN2;   	    //BT1 同步输入作为目标事件
	tEtbConfig.eTrgMode = ETCB_HARDWARE_TRG;
	ch = csi_etcb_ch_alloc(tEtbConfig.eChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								   	//ch < 0,则获取通道号失败
	iRet = csi_etcb_ch_init(ch, &tEtbConfig);
	
	//BT0 PWM初始化
	tPwmCfg.eIdleLevel = BT_PWM_IDLE_HIGH;			//PWM 输出空闲电平
	tPwmCfg.eStartLevel= BT_PWM_START_HIGH;			//PWM 输出起始电平
	tPwmCfg.byDutyCycle = 50;						//PWM 输出占空比(0 < DutyCycle < 100)		
	tPwmCfg.wFreq 		= 1000;						//PWM 输出频率
	csi_bt_pwm_init(BT0, &tPwmCfg);					//初始化BT0 PWM输出
	
	csi_bt_start(BT0);	     						//启动BT0
	
	while(1)
	{
		nop;
	}
	return iRet;
}



/** \brief	bt evtrg out: BT0 触发输出demo
 * 
 *  \brief	用BT0定时触发 BT1 PWM输出，当BT0定时周期到时触发BT1输出PWM
 * 		
 *  \param[in] none
 *  \return error code
 */
int bt_trg_out_demo(void)  
{
	int iRet = 0;
	volatile uint8_t ch = 0;
	csi_etcb_config_t tEtbConfig= {0};				            //ETCB 参数配置结构体		
	csi_bt_pwm_config_t tPwmCfg= {0};							//BT PWM输出参数初始化配置结构体
	csi_bt_time_config_t tTimConfig= {0};						//BT 定时初始化参数结构体	
	
#if (USE_GUI == 0)	
	csi_gpio_set_mux(GPIOC,PC11, PC11_BT1_OUT);				//PC11 作为BT1 PWM输出引脚
	
	csi_gpio_set_mux(GPIOA, PA6, PA6_OUTPUT);				//PA6 output ，并在BT中断里面翻转IO
	csi_gpio_set_low(GPIOA, PA6);							//PA6 output high;	
#endif
	//BT0初始化
	tTimConfig.wTimeVal = 10000;							//BT定时值 = 1000us
	tTimConfig.eRunMode  = BT_RUN_CONT;						//BT计数器工作模式
	csi_bt_timer_init(BT0,&tTimConfig);						//BT0 定时
	csi_bt_set_evtrg(BT0, BT_TRGSRC_PEND);					//BT0 PEND事件触发输出
	csi_bt_evtrg_enable(BT0);								//BT0 事件触发输出使能
	
	//BT1 PWM初始化 
	tPwmCfg.eIdleLevel = BT_PWM_IDLE_LOW;					//PWM 输出空闲电平
	tPwmCfg.eStartLevel= BT_PWM_START_HIGH;					//PWM 输出起始电平
	tPwmCfg.byDutyCycle = 60;								//PWM 输出占空比(0 < DutyCycle < 100)		
	tPwmCfg.wFreq 		= 1000;								//PWM 输出频率
	csi_bt_pwm_init(BT1, &tPwmCfg);							//初始化BT1 PWM输出
	
	//外部触发bt启动(SYNCIN0)
	csi_bt_set_sync(BT1, BT_SYNCIN0, BT_SYNC_ONCE, BT_TRG_AUTOAREARM);	
	csi_bt_sync_enable(BT1, BT_SYNCIN0);					//BT1 同步输入2使能
	
	//ETCB初始化
	tEtbConfig.eChType  = ETCB_ONE_TRG_ONE;  				//单个源触发单个目标
	tEtbConfig.eSrcIp   = ETCB_BT0_TRGOUT;   				//BT0_TRGOUT作为触发源
	tEtbConfig.eDstIp   = ETCB_BT1_SYNCIN0;  				//BT1 同步输入作为目标事件
	tEtbConfig.eTrgMode = ETCB_HARDWARE_TRG; 				//硬件触发模式
	ch = csi_etcb_ch_alloc(tEtbConfig.eChType);	   			//自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    		//ch < 0,则获取通道号失败
	iRet = csi_etcb_ch_init(ch, &tEtbConfig);
	
	csi_bt_start(BT0);										//启动BT0					
	
	while(1)
	{
		nop;
	}
	return iRet;
}

