/***********************************************************************//** 
 * \file  gptb_demo.c
 * \brief  TIMER_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-11 <td>V0.0 <td>ljy     <td>initial
 * <tr><td> 2023-5-8  <td>V0.1 <td>wangch  <td>modify
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include "csi_drv.h"
#include "board_config.h"

/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

/** \brief GPTB基本定时功能
 * 
 *  \param[in] none
 *  \return error code
 */
int gptb_timer_demo(void)
{
	int iRet = 0;
	
	csi_gptb_timer_init(GPTB0, 10000);		//初始化GPTB0, 定时10000us； GPTB定时，默认采用向上计数，PEND中断
	
	csi_gptb_start(GPTB0);                  //启动定时器

	return iRet;	
}

/** \brief GPTB捕获示例代码
 *   		- 捕获2次产生一次捕获中断
 *     		- 由PA01触发外部事件1，经过ETCB  触发sync2或sync3捕获
 * 			- 信号由PA01的高低电平切换产生（一直高电平意味着没有触发）
 * 			- GPTB支持分离捕获和合并捕获两种模式：
 * 			- 分离捕获：区分sync2和sync3的捕获事件：sync2捕获对应capld0事件;sync3捕获对应capld1事件
 * 			- 合并捕获：合并sync2和sync3的捕获事件，对应capld0~3四个事件
 *  \param[in] none
 *  \return error code
 */

int gptb_capture_demo(void)
{
	int iRet = 0;	
	volatile uint8_t ch = 0;
	csi_etcb_config_t tEtcbConfig;				//ETCB 参数配置结构体
	csi_gptb_capture_config_t tCapCfg;
//------------------------------------------------------------------------------------------------------------------------	
	csi_gpio_set_mux(GPIOA, PA1, PA1_INPUT);	
	csi_gpio_pull_mode(GPIOA, PA1, GPIO_PULLUP);					//PA01 上拉使能
	csi_gpio_irq_mode(GPIOA, PA1,EXI_GRP1, GPIO_IRQ_FALLING_EDGE);	//PA01 下降沿产生中断
	csi_gpio_int_enable(GPIOA, PA1);								//使能PA01中断	
	csi_exi_set_evtrg(EXI_TRGOUT1, EXI_TRGSRC_GRP1, 1);		
	csi_exi_evtrg_enable(EXI_TRGOUT1);
//------------------------------------------------------------------------------------------------------------------------		
	tEtcbConfig.eChType  = ETCB_ONE_TRG_ONE;  						//单个源触发单个目标
	tEtcbConfig.eSrcIp   = ETCB_EXI_TRGOUT1 ;  						//EXI_TRGOUT1作为触发源
	tEtcbConfig.eDstIp   = ETCB_GPTB0_SYNCIN2;   					//GPTB0同步输入2作为目标事件
	tEtcbConfig.eTrgMode = ETCB_HARDWARE_TRG;
	ch = csi_etcb_ch_alloc(tEtcbConfig.eChType);					//自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)														//ch < 0,则获取通道号失败
		return -1;		
	iRet = csi_etcb_ch_init(ch, &tEtcbConfig);	
//------------------------------------------------------------------------------------------------------------------------	
	tCapCfg.eWorkMode         	= GPTB_CAPTURE;                     //GPTB工作模式：捕获/波形输出	
	tCapCfg.eCountMode    		= GPTB_UPCNT;                       //GPTB计数模式：递增/递减/递增递减	
	tCapCfg.eRunMode	    	= GPTB_RUN_CONT;        		    //GPTB运行模式：连续/一次性	
	tCapCfg.eCapMode      		= GPTB_CAP_SEPARATE;                //GPTB捕获模式：合并/分离	
	tCapCfg.byCapStopWrap 		= 2;                                //GPTB捕获次数：0/1/2/3
	tCapCfg.byCapLdaret   		= 0;                                //CMPA捕获载入后计数器设置(1h：捕获载入后计数器值重置;0h：捕获载入后计数器值不重置)
	tCapCfg.byCapLdbret   		= 0;  								//CMPB捕获载入后计数器设置(1h：捕获载入后计数器值重置;0h：捕获载入后计数器值不重置)
	tCapCfg.byCapLdcret   		= 0;								//CMPC捕获载入后计数器设置(1h：捕获载入后计数器值重置;0h：捕获载入后计数器值不重置)
	tCapCfg.byCapLddret   		= 0;   								//CMPD捕获载入后计数器设置(1h：捕获载入后计数器值重置;0h：捕获载入后计数器值不重置)                        
	csi_gptb_capture_init(GPTB0, &tCapCfg);
	
	csi_gptb_set_sync(GPTB0, GPTB_TRGIN_SYNCEN2, GPTB_TRG_CONTINU,GPTB_AUTO_REARM_ZRO);
	csi_gptb_start(GPTB0);
	
	return iRet;
}

/** \brief GPTB sync2 sync3合并捕获示例代码，测试周期时间
 *          - sync2 sync3不区分，实现4次捕获
 *   		- 捕获4次产生一次捕获中断，ldbarst捕获后，计数器进行重置
 *     		- 由PA1外部扩展口,下降沿触发外部事件5，经过ETCB  触发sync3 捕获
 * 			- 信号由PA1的高低电平切换产生（一直高或低电平意味着没有触发）
 *          - CMPA捕获的是第一次周期值，CMPB捕获的是第二次周期值，CMPAA捕获的是第三次周期值,CMPBA捕获的是第四次周期值
 *  \param[in] none
 *  \return error code
 
 PA1输入波形 ——          —————          —————           —————          —————
				|          |        |          |        |           |         |         |        |
				|          |        |          |        |           |         |         |        |
				——————        ——————         ——————          —————        ————
				CMPA                CMPB                 CMPAA                CMPBA               CMPA   
*/
int gptb_capture_sync_demo0(void)
{
	int iRet = 0;
    volatile uint8_t ch;
	
//------------------------------------------------------------------------------------------------------------------------
	csi_pin_set_mux(PA1,PA1_INPUT);		
	csi_pin_pull_mode(PA1, GPIO_PULLUP);						 //PA1 上拉
	csi_pin_irq_mode(PA1, EXI_GRP16, GPIO_IRQ_FALLING_EDGE);     //PA1 下降沿产生中断，选择中断组16
	csi_pin_irq_enable(PA1, ENABLE);                            //PA1 中断使能                                    
	csi_exi_set_evtrg(5, EXI_TRGSRC_GRP16, 1);	 
//------------------------------------------------------------------------------------------------------------------------		
	csi_etcb_config_t tEtbConfig;				//ETCB 参数配置结构体	
	tEtbConfig.eChType  = ETCB_ONE_TRG_ONE;  	//单个源触发单个目标
	tEtbConfig.eSrcIp   = ETCB_EXI_TRGOUT5 ;  	//...作为触发源
	tEtbConfig.eDstIp   = ETCB_GPTB0_SYNCIN3;  //GPTB0 同步输入2作为目标事件
	tEtbConfig.eTrgMode = ETCB_HARDWARE_TRG;
	
	ch = csi_etcb_ch_alloc(tEtbConfig.eChType);	//自动获取空闲通道号,ch >= 0 获取成功						//ch < 0,则获取通道号失败		
	iRet = csi_etcb_ch_init(ch, &tEtbConfig);	
//------------------------------------------------------------------------------------------------------------------------	
	csi_gptb_capture_config_t tCapCfg;								  
	tCapCfg.eWorkMode         	= GPTB_CAPTURE;                     //GPTB工作模式：捕获/波形输出	
	tCapCfg.eCountMode    		= GPTB_UPCNT;                       //GPTB计数模式：递增/递减/递增递减	
	tCapCfg.eRunMode	    	= GPTB_RUN_CONT;        		    //GPTB运行模式：连续/一次性	
	tCapCfg.eCapMode      		= GPTB_MERGE_CAP;                	//GPTB捕获模式：合并/分离	
	tCapCfg.byCapStopWrap 		= 3;                                //GPTB捕获次数：0/1/2/3
	tCapCfg.byCapLdaret   		= 0;                                //CMPA捕获载入后计数器设置(1h：捕获载入后计数器值重置;0h：捕获载入后计数器值不重置)
	tCapCfg.byCapLdbret   		= 0;  								//CMPB捕获载入后计数器设置(1h：捕获载入后计数器值重置;0h：捕获载入后计数器值不重置)
	tCapCfg.byCapLdcret   		= 0;								//CMPC捕获载入后计数器设置(1h：捕获载入后计数器值重置;0h：捕获载入后计数器值不重置)
	tCapCfg.byCapLddret   		= 1;   								//CMPD捕获载入后计数器设置(1h：捕获载入后计数器值重置;0h：捕获载入后计数器值不重置)                        
	csi_gptb_capture_init(GPTB0, &tCapCfg);
//------------------------------------------------------------------------------------------------------------------------
    csi_gptb_set_sync(GPTB0, GPTB_TRGIN_SYNCEN3, GPTB_TRG_CONTINU, GPTB_AUTO_REARM_ZRO);//使能SYNCIN3外部触发
	csi_gptb_start(GPTB0);//start  timer
    while(1)
	{
		mdelay(1);
	}
	return iRet;
}

/** \brief GPTB sync2 sync3合并捕获示例代码，测试低电平时间
 *          //sync2 sync3不区分，实现1次捕获
 *   		- 捕获1次产生一次捕获中断，ldarst捕获后，计数器进行重置
 *     		- 由PA1下降沿产生外部事件0，经过ETCB  触发sync0，重置和启动计数器
 *          - 由PA1外部扩展口，上升沿产生外部事件5，经过ETCB  触发sync3 捕获，上升沿捕获值存放在CMPA中
 * 			- 信号由PA1的高低电平切换产生（一直高或低电平意味着没有触发）
 *          - CMPA捕获的是下降沿时间
 *  \param[in] none
 *  \return error code
 * 
 PA1输入波形——          —————          —————           ———
               |          |        |          |        |           |        
	           |          |        |          |        |           |        
               ——————        ——————         ——————          
               RESET      CMPA     RESET     CMPA      RESET       CMPA               
*/
int gptb_capture_sync_demo1(void)
{
	int iRet = 0;	
    volatile uint8_t ch;
	
//------------------------------------------------------------------------------------------------------------------------
	csi_pin_set_mux(PA1,PA1_INPUT);		
	csi_pin_pull_mode(PA1, GPIO_PULLUP);						//PA1 上拉
	
	csi_pin_irq_mode(PA1,EXI_GRP1, GPIO_IRQ_FALLING_EDGE);		//PA1 下降沿产生中断，选择中断组1 
	csi_exi_set_evtrg(0, EXI_TRGSRC_GRP1, 1);					//产生外部事件0
	
	csi_pin_irq_mode(PA1, EXI_GRP16, GPIO_IRQ_RISING_EDGE);     //PA1 上升沿产生中断，选择中断组16                                                            
	csi_exi_set_evtrg(5, EXI_TRGSRC_GRP16, 1);	 				//产生外部事件5
	csi_pin_irq_enable(PA1, ENABLE);							//PA1 中断使能 
//------------------------------------------------------------------------------------------------------------------------
	csi_etcb_config_t tEtbConfig;		
	//ETCB 参数配置结构体	
	tEtbConfig.eChType  = ETCB_ONE_TRG_ONE;  	//单个源触发单个目标
	tEtbConfig.eSrcIp   = ETCB_EXI_TRGOUT0 ;  	//...作为触发源
	tEtbConfig.eDstIp   = ETCB_GPTB0_SYNCIN0;  //GPTB0 同步输入0作为目标事件
	tEtbConfig.eTrgMode = ETCB_HARDWARE_TRG;
	
	ch = csi_etcb_ch_alloc(tEtbConfig.eChType);	//自动获取空闲通道号,ch >= 0 获取成功						//ch < 0,则获取通道号失败		
	iRet = csi_etcb_ch_init(ch, &tEtbConfig);			
//------------------------------------------------------------------------------------------------------------------------		
	tEtbConfig.eChType  = ETCB_ONE_TRG_ONE;  	//单个源触发单个目标
	tEtbConfig.eSrcIp   = ETCB_EXI_TRGOUT5 ;  	//...作为触发源
	tEtbConfig.eDstIp   = ETCB_GPTB0_SYNCIN3;  //GPTB0 同步输入2作为目标事件
	tEtbConfig.eTrgMode = ETCB_HARDWARE_TRG;
	
	ch = csi_etcb_ch_alloc(tEtbConfig.eChType);	//自动获取空闲通道号,ch >= 0 获取成功						//ch < 0,则获取通道号失败		
	iRet = csi_etcb_ch_init(ch, &tEtbConfig);	
//------------------------------------------------------------------------------------------------------------------------	
	csi_gptb_capture_config_t tCapCfg;								  
	tCapCfg.eWorkMode         	= GPTB_CAPTURE;                     //GPTB工作模式：捕获/波形输出	
	tCapCfg.eCountMode    		= GPTB_UPCNT;                       //GPTB计数模式：递增/递减/递增递减	
	tCapCfg.eRunMode	    	= GPTB_RUN_CONT;        		    //GPTB运行模式：连续/一次性	
	tCapCfg.eCapMode      		= GPTB_MERGE_CAP;                	//GPTB捕获模式：合并/分离	
	tCapCfg.byCapStopWrap 		= 1;                                //GPTB捕获次数：0/1/2/3
	tCapCfg.byCapLdaret   		= 1;                                //CMPA捕获载入后计数器设置(1h：捕获载入后计数器值重置;0h：捕获载入后计数器值不重置)
	tCapCfg.byCapLdbret   		= 0;  								//CMPB捕获载入后计数器设置(1h：捕获载入后计数器值重置;0h：捕获载入后计数器值不重置)
	tCapCfg.byCapLdcret   		= 0;								//CMPC捕获载入后计数器设置(1h：捕获载入后计数器值重置;0h：捕获载入后计数器值不重置)
	tCapCfg.byCapLddret   		= 0;   								//CMPD捕获载入后计数器设置(1h：捕获载入后计数器值重置;0h：捕获载入后计数器值不重置)                        
	csi_gptb_capture_init(GPTB0, &tCapCfg);
//------------------------------------------------------------------------------------------------------------------------
	csi_gptb_set_sync(GPTB0, GPTB_TRGIN_SYNCEN0, GPTB_TRG_CONTINU, GPTB_AUTO_REARM_ZRO);//使能SYNCIN0外部触发
    csi_gptb_set_sync(GPTB0, GPTB_TRGIN_SYNCEN3, GPTB_TRG_CONTINU, GPTB_AUTO_REARM_ZRO);//使能SYNCIN3外部触发
	csi_gptb_start(GPTB0);//start  timer
    while(1)
	{
		mdelay(1);
	}
	return iRet;
}

/** \brief GPTB sync2 sync3区分捕获示例代码，测试低电平和周期时间，同时可计算出高电平时间
 *          //sync2 sync3区分，实现2次捕获
 *   		- 捕获2次产生一次捕获中断，ldbrst捕获后，计数器进行重置
 *     		- 由PA3产生外部事件0，经过ETCB  触发sync2 上升沿捕获，上升沿捕获值存放在CMPA中
 *          - 由PA3外部扩展口,产生外部事件5，经过ETCB  触发sync3 下降沿捕获，下降沿捕获值存放在CMPB中
 * 			- 信号由PA3的高低电平切换产生（一直高或低电平意味着没有触发）
 *          - 下降沿时间为CMPA，周期时间为CMPB，上升沿时间为 CMPB - CMPA。  
 *  \param[in] none
 *  \return error code
 * 
				 —————          —————           —————         
				 |        |          |        |           |        |    
				 |        |          |        |           |        |        
 PA3输入波形———        ——————         ——————         ———
			    CMPA      CMPB      CMPA      CMPB       CMPA      CMPB  
*/
int gptb_capture_sync_demo2(void)
{
	int iRet = 0;	
    volatile uint8_t ch;

//------------------------------------------------------------------------------------------------------------------------	
	csi_pin_set_mux(PA3,PA3_INPUT);		
	csi_pin_pull_mode(PA3, GPIO_PULLUP);						//PA3 上拉
	
	csi_pin_irq_mode(PA3,EXI_GRP3, GPIO_IRQ_RISING_EDGE);		//PA3 上升沿产生中断，选择中断组3
	csi_exi_set_evtrg(0, EXI_TRGSRC_GRP3, 1);					//产生外部事件0

	csi_pin_irq_mode(PA3, EXI_GRP16, GPIO_IRQ_FALLING_EDGE);    //PA3 下降沿产生中断，选择中断组16                                   
	csi_exi_set_evtrg(5, EXI_TRGSRC_GRP16, 1);	   				//产生外部事件5
	
	csi_pin_irq_enable(PA3, ENABLE);
//------------------------------------------------------------------------------------------------------------------------		
	csi_etcb_config_t tEtbConfig;				//ETCB 参数配置结构体	
	tEtbConfig.eChType  = ETCB_ONE_TRG_ONE;  	//单个源触发单个目标
	tEtbConfig.eSrcIp   = ETCB_EXI_TRGOUT0 ;  	//...作为触发源
	tEtbConfig.eDstIp   = ETCB_GPTB0_SYNCIN2;  //GPTB0 同步输入2作为目标事件
	tEtbConfig.eTrgMode = ETCB_HARDWARE_TRG;
	
	ch = csi_etcb_ch_alloc(tEtbConfig.eChType);	//自动获取空闲通道号,ch >= 0 获取成功						//ch < 0,则获取通道号失败		
	iRet = csi_etcb_ch_init(ch, &tEtbConfig);	
//------------------------------------------------------------------------------------------------------------------------		
	tEtbConfig.eChType  = ETCB_ONE_TRG_ONE;  	//单个源触发单个目标
	tEtbConfig.eSrcIp   = ETCB_EXI_TRGOUT5 ;  	//...作为触发源
	tEtbConfig.eDstIp   = ETCB_GPTB0_SYNCIN3;  //GPTB0 同步输入3作为目标事件
	tEtbConfig.eTrgMode = ETCB_HARDWARE_TRG;
	
	ch = csi_etcb_ch_alloc(tEtbConfig.eChType);	//自动获取空闲通道号,ch >= 0 获取成功						//ch < 0,则获取通道号失败		
	iRet = csi_etcb_ch_init(ch, &tEtbConfig);		
//------------------------------------------------------------------------------------------------------------------------	
	csi_gptb_capture_config_t tCapCfg;								  

	tCapCfg.eWorkMode         	= GPTB_CAPTURE;                     //GPTB工作模式：捕获/波形输出	
	tCapCfg.eCountMode    		= GPTB_UPCNT;                       //GPTB计数模式：递增/递减/递增递减	
	tCapCfg.eRunMode	    	= GPTB_RUN_CONT;        		    //GPTB运行模式：连续/一次性	
	tCapCfg.eCapMode      		= GPTB_SEPARATE_CAP;                //GPTB捕获模式：合并/分离	
	tCapCfg.byCapStopWrap 		= 2;                                //GPTB捕获次数：0/1/2/3
	tCapCfg.byCapLdaret   		= 0;                                //CMPA捕获载入后计数器设置(1h：捕获载入后计数器值重置;0h：捕获载入后计数器值不重置)
	tCapCfg.byCapLdbret   		= 1;  								//CMPB捕获载入后计数器设置(1h：捕获载入后计数器值重置;0h：捕获载入后计数器值不重置)
	tCapCfg.byCapLdcret   		= 0;								//CMPC捕获载入后计数器设置(1h：捕获载入后计数器值重置;0h：捕获载入后计数器值不重置)
	tCapCfg.byCapLddret   		= 0;   								//CMPD捕获载入后计数器设置(1h：捕获载入后计数器值重置;0h：捕获载入后计数器值不重置)                        
	csi_gptb_capture_init(GPTB0, &tCapCfg);
//------------------------------------------------------------------------------------------------------------------------
    csi_gptb_set_sync(GPTB0, GPTB_TRGIN_SYNCEN2, GPTB_TRG_CONTINU, GPTB_AUTO_REARM_ZRO);//使能SYNCIN2外部触发
	csi_gptb_set_sync(GPTB0, GPTB_TRGIN_SYNCEN3, GPTB_TRG_CONTINU, GPTB_AUTO_REARM_ZRO);//使能SYNCIN3外部触发
	csi_gptb_start(GPTB0);//start  timer
    while(1)
	{
		mdelay(1);
	}
	return iRet;	
}

/** \brief GPTB 波形输出示例代码
 *   		-20kHZ，占空比50%   输出波形
 *     		-可通过以下两种方式灵活调整PWM参数
 * 			--csi_gptb_change_ch_duty：修改PWM占空比
 *			--csi_gptb_prdr_cmp_update：修改PWM周期寄存器和比较寄存器的值
 *  \param[in] none
 *  \return error code
 */
int gptb_pwm_demo(void)
{
	int iRet = 0;	
////------------------------------------------------------------------------------------------------------------------------	
#if !defined(USE_GUI)
	csi_pin_set_mux(PC13, PC13_GPTB0_CHAX);	
	csi_pin_set_mux(PA7, PA7_GPTB0_CHAY); 
	csi_pin_set_mux(PD0, PD0_GPTB0_CHB);	
#endif
//------------------------------------------------------------------------------------------------------------------------		
    csi_gptb_pwm_config_t tPwmCfg;								  
	tPwmCfg.byWorkmod        = GPTB_WAVE;                       //WAVE  波形模式
	tPwmCfg.byCountingMode   = GPTB_UPDNCNT;                    //CNYMD  //计数方向
	tPwmCfg.byOneshotMode    = GPTB_RUN_CONT;                    //OPM    //单次或连续(工作方式)
	tPwmCfg.byStartSrc       = GPTB_SYNC;					//软件使能同步触发使能控制（RSSR中START控制位）//启动方式
	tPwmCfg.byPscld          = GPTB_LDPSCR_ZRO;                 //PSCR(分频)活动寄存器载入控制。活动寄存器在配置条件满足时，从影子寄存器载入更新值		
	tPwmCfg.byDutyCycle 	 = 50;								//pwm ouput duty cycle//PWM初始值			
	tPwmCfg.wFreq 			 = 50000;							//pwm ouput frequency	
//	tPwmCfg.wInt 		 	 = GPTBINT_NONE;                       //interrupt
	csi_gptb_wave_init(GPTB0, &tPwmCfg);

////------------------------------------------------------------------------------------------------------------------------	
	csi_gptb_pwm_channel_config_t  tPwmchannelCfg;
	tPwmchannelCfg.byActionZro    =   GPTB_LO;
	tPwmchannelCfg.byActionPrd    =   GPTB_NA;
	tPwmchannelCfg.byActionC1u    =   GPTB_HI;
	tPwmchannelCfg.byActionC1d    =   GPTB_LO;
	tPwmchannelCfg.byActionC2u    =   GPTB_NA;
	tPwmchannelCfg.byActionC2d    =   GPTB_NA;
	tPwmchannelCfg.byActionT1u    =   GPTB_NA;
	tPwmchannelCfg.byActionT1d    =   GPTB_NA;
	tPwmchannelCfg.byActionT2u    =   GPTB_NA;
	tPwmchannelCfg.byActionT2d    =   GPTB_NA;
	tPwmchannelCfg.byChoiceC1sel  =   GPTB_COMPA;
	tPwmchannelCfg.byChoiceC2sel  =   GPTB_COMPA;

	csi_gptb_channel_config(GPTB0, &tPwmchannelCfg,  GPTB_CHANNEL_1);//channel
	csi_gptb_channel_config(GPTB0, &tPwmchannelCfg,  GPTB_CHANNEL_2);

	csi_gptb_start(GPTB0);//start  timer

	while(1)
	{
		csi_gptb_change_ch_duty(GPTB0,GPTB_COMPA, 20);				  	//修改PWM1占空比为20%
		csi_gptb_change_ch_duty(GPTB0,GPTB_COMPB, 20);					//修改PWM2占空比为20%
			
		csi_gptb_prdr_cmp_update(GPTB0,GPTB_COMPA,1200,800);			//修改PWM1周期为1200，比较值为800
		csi_gptb_prdr_cmp_update(GPTB0,GPTB_COMPB,1200,800);			//修改PWM2周期为1200，比较值为800

		mdelay(1);	
	
		csi_gptb_change_ch_duty(GPTB0,GPTB_COMPA, 50);					//修改PWM1占空比为50%
		csi_gptb_change_ch_duty(GPTB0,GPTB_COMPB, 50);					//修改PWM2占空比为50%
			
		csi_gptb_prdr_cmp_update(GPTB0,GPTB_COMPA,1200,1000);			//修改PWM1周期为1200，比较值为1000
		csi_gptb_prdr_cmp_update(GPTB0,GPTB_COMPB,1200,1000);			//修改PWM2周期为1200，比较值为1000
		mdelay(1);
	}	
    return iRet;
}


/** \brief GPTB波形输出+互补对称波形+死区时间（上升沿500ns+下降沿200ns）
 *   		-10kHZ   
 *     		-PWM在80%和40%之间切换
 * 			-此为电机FOC基本波形设置
 *  \param[in] none
 *  \return error code
 */
int gptb_pwm_dz_demo(void)
{
//------------------------------------------------------------------------------------------------------------------------	
#if !defined(USE_GUI)
	csi_pin_set_mux(PC13, PC13_GPTB0_CHAX);	
	csi_pin_set_mux(PA7, PA7_GPTB0_CHAY); 
	csi_pin_set_mux(PD0, PD0_GPTB0_CHB);
#endif	
//------------------------------------------------------------------------------------------------------------------------		
    csi_gptb_pwm_config_t tPwmCfg;								  
	tPwmCfg.byWorkmod        = GPTB_WAVE;                       //WAVE  波形模式
	tPwmCfg.byCountingMode   = GPTB_UPDNCNT;                    //CNYMD  //计数方向
	tPwmCfg.byOneshotMode    = GPTB_RUN_CONT;                    //OPM    //单次或连续(工作方式)
	tPwmCfg.byStartSrc       = GPTB_SW;   				//软件使能同步触发使能控制（RSSR中START控制位）//启动方式
	tPwmCfg.byPscld          = GPTB_LDPSCR_ZRO;                 //PSCR(分频)活动寄存器载入控制。活动寄存器在配置条件满足时，从影子寄存器载入更新值		
	tPwmCfg.byDutyCycle 	 = 50;								//pwm ouput duty cycle//PWM初始值			
	tPwmCfg.wFreq 			 = 10000;							//pwm ouput frequency	
//	tPwmCfg.wInt 			 = GPTBINT_NONE;                       //interrupt
	csi_gptb_wave_init(GPTB0, &tPwmCfg);
//------------------------------------------------------------------------------------------------------------------------	
	csi_gptb_pwm_channel_config_t  tPwmchannelCfg;
	tPwmchannelCfg.byActionZro    =   GPTB_HI;
	tPwmchannelCfg.byActionPrd    =   GPTB_NA;
	tPwmchannelCfg.byActionC1u    =   GPTB_LO;
	tPwmchannelCfg.byActionC1d    =   GPTB_HI;
	tPwmchannelCfg.byActionC2u    =   GPTB_NA;
	tPwmchannelCfg.byActionC2d    =   GPTB_NA;
	tPwmchannelCfg.byActionT1u    =   GPTB_LO;
	tPwmchannelCfg.byActionT1d    =   GPTB_LO;
	tPwmchannelCfg.byActionT2u    =   GPTB_NA;
	tPwmchannelCfg.byActionT2d    =   GPTB_NA;
	tPwmchannelCfg.byChoiceC1sel  =   GPTB_COMPA;
	tPwmchannelCfg.byChoiceC2sel  =   GPTB_COMPB;

	csi_gptb_channel_config(GPTB0, &tPwmchannelCfg,  GPTB_CHANNEL_1);
	csi_gptb_channel_config(GPTB0, &tPwmchannelCfg,  GPTB_CHANNEL_2);
//------------------------------------------------------------------------------------------------------------------------	
    csi_gptb_deadzone_config_t  tGptbDeadZoneTime;
	tGptbDeadZoneTime.byDcksel               = GPTB_DB_DPSC;
	tGptbDeadZoneTime.hwDpsc                 = 0;              //FDBCLK = FHCLK / (DPSC+1)
	tGptbDeadZoneTime.wRisingEdgeTime        = 500;             //上升沿-ns
	tGptbDeadZoneTime.wFallingEdgeTime       = 200;             //下降沿-ns
	tGptbDeadZoneTime.byChaDedb              = GPTB_DB_AR_BF;       //不使用死区双沿
	csi_gptb_dz_config(GPTB0,&tGptbDeadZoneTime);
	
	tGptbDeadZoneTime.byChxOuselS1S0      = GPTB_DBOUT_AR_BF;              //使能通道A的上升沿延时，使能通道B的下降沿延时
	tGptbDeadZoneTime.byChxPolarityS3S2   = GPTB_DB_POL_B;                 //通道A和通道B延时输出电平是否反向
	tGptbDeadZoneTime.byChxInselS5S4      = GPTB_DBCHAIN_AR_AF;            //PWMA作为上升沿和下降沿延时处理的输入信号
	tGptbDeadZoneTime.byChxOutSwapS8S7    = GPTB_DBCHAOUT_OUTA_A_OUTB_B;   //OUTA=通道A输出，OUTB=通道B输出
    csi_gptb_channelmode_config(GPTB0,&tGptbDeadZoneTime,GPTB_CHANNEL_1);
//------------------------------------------------------------------------------------------------------------------------
	csi_gptb_start(GPTB0);//start  timer	
	while(1)
	{
		csi_gptb_change_ch_duty(GPTB0,GPTB_COMPA, 80);
		csi_gptb_change_ch_duty(GPTB0,GPTB_COMPB, 80);
		mdelay(200);                        
		csi_gptb_change_ch_duty(GPTB0,GPTB_COMPA, 40);
		csi_gptb_change_ch_duty(GPTB0,GPTB_COMPB, 40);
		mdelay(200);
	}
}

/** \brief GPTB波形输出+互补对称波形+死区时间（上升沿500ns+下降沿200ns）+软锁止/硬锁止
 *   		-10kHZ   
 *     		-PWM在80%和20%之间切换
 * 			-此为电机FOC基本波形设置
 *          -EBIx的输入需上拉或下拉，也可开启内部的上下拉
 *  \param[in] none
 *  \return error code
 */
int gptb_pwm_dz_em_demo(void)
{
	int iRet = 0;	
////------------------------------------------------------------------------------------------------------------------------	
#if !defined(USE_GUI)
	csi_pin_set_mux(PC13, PC13_GPTB0_CHAX);	
	csi_pin_set_mux(PA7, PA7_GPTB0_CHAY);
	csi_pin_set_mux(PD0, PD0_GPTB0_CHB);	
	
	csi_pin_set_mux(PA6,PA6_EBI0);csi_pin_pull_mode(PA6, GPIO_PULLUP);
	csi_pin_set_mux(PA9,PA9_EBI1);csi_pin_pull_mode(PA9, GPIO_PULLUP);
	csi_pin_set_mux(PB2,PB2_EBI2);csi_pin_pull_mode(PB2, GPIO_PULLUP);
	csi_pin_set_mux(PA0,PA0_EBI3);csi_pin_pull_mode(PA0, GPIO_PULLUP);
#endif
//------------------------------------------------------------------------------------------------------------------------		
    csi_gptb_pwm_config_t tPwmCfg;								  
	tPwmCfg.byWorkmod        = GPTB_WAVE;                       //WAVE  波形模式
	tPwmCfg.byCountingMode   = GPTB_UPDNCNT;                    //CNYMD  //计数方向
	tPwmCfg.byOneshotMode    = GPTB_RUN_CONT;                    //OPM    //单次或连续(工作方式)
	tPwmCfg.byStartSrc       = GPTB_SYNC;					//软件使能同步触发使能控制（RSSR中START控制位）//启动方式
	tPwmCfg.byPscld          = GPTB_LDPSCR_ZRO;                 //PSCR(分频)活动寄存器载入控制。活动寄存器在配置条件满足时，从影子寄存器载入更新值		
	tPwmCfg.byDutyCycle 	 = 50;								//pwm ouput duty cycle//PWM初始值			
	tPwmCfg.wFreq 			 = 10000;							//pwm ouput frequency	
//	tPwmCfg.wInt 		     = GPTBINT_NONE;                       //interrupt
	csi_gptb_wave_init(GPTB0, &tPwmCfg);	
////------------------------------------------------------------------------------------------------------------------------	
	csi_gptb_pwm_channel_config_t  tPwmchannelCfg;
	tPwmchannelCfg.byActionZro    =   GPTB_LO;
	tPwmchannelCfg.byActionPrd    =   GPTB_NA;
	tPwmchannelCfg.byActionC1u    =   GPTB_HI;
	tPwmchannelCfg.byActionC1d    =   GPTB_LO;
	tPwmchannelCfg.byActionC2u    =   GPTB_NA;
	tPwmchannelCfg.byActionC2d    =   GPTB_NA;
	tPwmchannelCfg.byActionT1u    =   GPTB_NA;
	tPwmchannelCfg.byActionT1d    =   GPTB_NA;
	tPwmchannelCfg.byActionT2u    =   GPTB_NA;
	tPwmchannelCfg.byActionT2d    =   GPTB_NA;
	tPwmchannelCfg.byChoiceC1sel  =   GPTB_COMPA;
	tPwmchannelCfg.byChoiceC2sel  =   GPTB_COMPA;

	csi_gptb_channel_config(GPTB0, &tPwmchannelCfg,  GPTB_CHANNEL_1);//channel
	csi_gptb_channel_config(GPTB0, &tPwmchannelCfg,  GPTB_CHANNEL_2);
////------------------------------------------------------------------------------------------------------------------------	
	csi_gptb_deadzone_config_t  tGptbDeadZoneTime;
	tGptbDeadZoneTime.byDcksel               = GPTB_DB_DPSC;
	tGptbDeadZoneTime.hwDpsc                 = 0;              //FDBCLK = FHCLK / (DPSC+1)
	tGptbDeadZoneTime.wRisingEdgeTime        = 500;             //上升沿-ns
	tGptbDeadZoneTime.wFallingEdgeTime       = 200;             //下降沿-ns
	tGptbDeadZoneTime.byChaDedb              = GPTB_DBOUT_AR_BF;       //不使用死区双沿
	csi_gptb_dz_config(GPTB0,&tGptbDeadZoneTime);
	
	tGptbDeadZoneTime.byChxOuselS1S0      = GPTB_DBOUT_AR_BF;              //使能通道A的上升沿延时，使能通道B的下降沿延时
	tGptbDeadZoneTime.byChxPolarityS3S2   = GPTB_DB_POL_B;                 //通道A和通道B延时输出电平是否反向
	tGptbDeadZoneTime.byChxInselS5S4      = GPTB_DBCHAIN_AR_AF;            //PWMA作为上升沿和下降沿延时处理的输入信号
	tGptbDeadZoneTime.byChxOutSwapS8S7    = GPTB_DBCHAOUT_OUTA_A_OUTB_B;   //OUTA=通道A输出，OUTB=通道B输出
    csi_gptb_channelmode_config(GPTB0,&tGptbDeadZoneTime,GPTB_CHANNEL_1);
//------------------------------------------------------------------------------------------------------------------------
    csi_gptb_emergency_config_t   tGptbEmergencyCfg;           
    tGptbEmergencyCfg.byEpxInt    = GPTB_EBI0;              //EPx选择外部IO端口（EBI0~EBI4）
    tGptbEmergencyCfg.byPolEbix   = GPTB_EBI_POL_L;      //EBIx的输入有效极性选择控制
	tGptbEmergencyCfg.byEpx       = GPTB_EP0;            //使能EPx
	tGptbEmergencyCfg.byEpxLckmd  = GPTB_EP_SLCK;        //使能 锁
    tGptbEmergencyCfg.byFltpace0  = GPTB_EP_FLT0_4P;      //EP0、EP1、EP2和EP3的数字去抖滤波检查周期数
	tGptbEmergencyCfg.byFltpace1  = GPTB_EP_FLT1_2P;      //EP4、EP5、EP6和EP7的数字去抖滤波检查周期数
	tGptbEmergencyCfg.byOsrshdw   = GPTB_OSR_SHADOW;       	 //锁止端口状态载入方式
	
	if(tGptbEmergencyCfg.byEpxLckmd == GPTB_EP_SLCK)
	{
		tGptbEmergencyCfg.bySlclrmd = GPTB_SLCLRMD_ZRO;           //软锁自动清除状态
	}
	if(tGptbEmergencyCfg.byOsrshdw == GPTB_OSR_SHADOW)
    {  
		tGptbEmergencyCfg.byOsrldmd = GPTB_LD_EMOSR_PRD;
	}
	
    csi_gptb_emergency_cfg(GPTB0,&tGptbEmergencyCfg); 
	csi_gptb_emergency_pinxout(GPTB0,GPTB_EMCOAX,GPTB_EMOUT_L);       //紧急状态下输出状态设置（注意mos/igbt的驱动电平）
	csi_gptb_emergency_pinxout(GPTB0,GPTB_EMCOAY,GPTB_EMOUT_L);
	csi_gptb_emergency_pinxout(GPTB0,GPTB_EMCOBX,GPTB_EMOUT_L);
	csi_gptb_emint_enable(GPTB0,GPTB_EMINT_EP0);      //紧急状态输入中断使能

	csi_gptb_start(GPTB0);//start  timer

	while(1)
	{
		csi_gptb_change_ch_duty(GPTB0,GPTB_COMPA, 20);
		csi_gptb_change_ch_duty(GPTB0,GPTB_COMPB, 20);
		mdelay(1);
		csi_gptb_change_ch_duty(GPTB0,GPTB_COMPA, 80);
		csi_gptb_change_ch_duty(GPTB0,GPTB_COMPB, 80);
		mdelay(1);
	}	
	return iRet;
}

static uint32_t s_wGptbCapBuff[4] = {0};

/** \brief gptb0 interrupt handle weak function
 *  \param[in] none
 *  \return    none
 */
__attribute__((weak)) void gptb_irqhandler(csp_gptb_t *ptGptbBase)
{
	volatile uint32_t wEMMisr = csp_gptb_get_emisr(ptGptbBase);
	volatile uint32_t wMisr   = csp_gptb_get_isr(ptGptbBase);	
	
	//GPTB emergency interrupt
	if(wEMMisr > 0)
	{
		if((wEMMisr & GPTB_EM_INT_EP0) == GPTB_EM_INT_EP0)
		{
			csp_gptb_clr_emisr(ptGptbBase, GPTB_EM_INT_EP0);
		}
		if((wEMMisr & GPTB_EM_INT_EP1) == GPTB_EM_INT_EP1)
		{
			csp_gptb_clr_emisr(ptGptbBase, GPTB_EM_INT_EP1);
		}
		if((wEMMisr & GPTB_EM_INT_EP2) == GPTB_EM_INT_EP2)
		{
			csp_gptb_clr_emisr(ptGptbBase, GPTB_EM_INT_EP2);
		}
		if((wEMMisr & GPTB_EM_INT_EP3) == GPTB_EM_INT_EP3)
		{
			csp_gptb_clr_emisr(ptGptbBase, GPTB_EM_INT_EP3);
		}	
	}

	//GPTB interrupt
	if(wMisr > 0)
	{
		if((wMisr & GPTB_INT_TRGEV0) == GPTB_INT_TRGEV0)
		{
			csp_gptb_clr_isr(ptGptbBase, GPTB_INT_TRGEV0);
		}
		if((wMisr & GPTB_INT_TRGEV1) == GPTB_INT_TRGEV1)
		{
			csp_gptb_clr_isr(ptGptbBase, GPTB_INT_TRGEV1);
		}
		if((wMisr & GPTB_INT_CAPLD0) == GPTB_INT_CAPLD0)
		{
			csp_gptb_clr_isr(ptGptbBase, GPTB_INT_CAPLD0);
			s_wGptbCapBuff[0]=csp_gptb_get_cmpa(ptGptbBase);
		}
		if((wMisr & GPTB_INT_CAPLD1) == GPTB_INT_CAPLD1)
		{
			csp_gptb_clr_isr(ptGptbBase, GPTB_INT_CAPLD1);
			s_wGptbCapBuff[0]=csp_gptb_get_cmpa(ptGptbBase);
			s_wGptbCapBuff[1]=csp_gptb_get_cmpb(ptGptbBase);
		}
		if((wMisr & GPTB_INT_CAPLD2) == GPTB_INT_CAPLD2)
		{
			csp_gptb_clr_isr(ptGptbBase, GPTB_INT_CAPLD2);
			s_wGptbCapBuff[0]=csp_gptb_get_cmpa(ptGptbBase);
			s_wGptbCapBuff[1]=csp_gptb_get_cmpb(ptGptbBase);
			s_wGptbCapBuff[2]=csp_gptb_get_cmpaa(ptGptbBase);
		}
		if((wMisr & GPTB_INT_CAPLD3) == GPTB_INT_CAPLD3)
		{
			csp_gptb_clr_isr(ptGptbBase, GPTB_INT_CAPLD3);
			s_wGptbCapBuff[0]=csp_gptb_get_cmpa(ptGptbBase);
			s_wGptbCapBuff[1]=csp_gptb_get_cmpb(ptGptbBase);
			s_wGptbCapBuff[2]=csp_gptb_get_cmpaa(ptGptbBase);
			s_wGptbCapBuff[3]=csp_gptb_get_cmpba(ptGptbBase);
		}
		if((wMisr & GPTB_INT_CAU) == GPTB_INT_CAU)
		{
			csp_gptb_clr_isr(ptGptbBase, GPTB_INT_CAU);
		}
		if((wMisr & GPTB_INT_CAD) == GPTB_INT_CAD)
		{
			csp_gptb_clr_isr(ptGptbBase, GPTB_INT_CAD);
		}
		if((wMisr & GPTB_INT_CBU) == GPTB_INT_CBU)
		{
			csp_gptb_clr_isr(ptGptbBase, GPTB_INT_CBU);
		}
		if((wMisr & GPTB_INT_CBD) == GPTB_INT_CBD)
		{
			csp_gptb_clr_isr(ptGptbBase, GPTB_INT_CBD);
		}
		if((wMisr & GPTB_INT_PEND) == GPTB_INT_PEND)
		{	
			csp_gptb_clr_isr(ptGptbBase, GPTB_INT_PEND);
		}
		if((wMisr & GPTB_INT_PRDMA) == GPTB_INT_PRDMA)
		{
			csp_gptb_clr_isr(ptGptbBase, GPTB_INT_PRDMA);
		}
		if((wMisr & GPTB_INT_ZROMA) == GPTB_INT_ZROMA)
		{
			csp_gptb_clr_isr(ptGptbBase, GPTB_INT_ZROMA);
		}
	}
}