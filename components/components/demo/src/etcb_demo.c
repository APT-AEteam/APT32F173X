/***********************************************************************//** 
 * \file  etcb_demo.c
 * \brief  ETCB_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-13 <td>V0.0 <td>ZJY    <td>initial
 * <tr><td> 2021-7-28 <td>V0.1 <td>YYM    <td>modify
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

/** \brief	exi trg bt start：EXI通过ETCB触发BT启动的demo
 * 
 *  \brief	PB1外部下降沿触发BT0启动，对应VIC中断不开启，即不进入PB1的外部中断；若需要进入PB1外部中断，需
 * 			开启对应的VIC中断，调用csi_gpio_vic_irq_enable接口函数
 *  
 *  \param[in] none
 *  \return error code
 */
int exi_etcb_bt_start_demo(void)
{
	int iRet = 0;
	volatile uint8_t ch = 0;
	csi_etcb_config_t tEtbConfig;				               			//ETCB 参数配置结构体	
	csi_bt_time_config_t tTimConfig;									//BT 定时初始化参数结构体	

#if (USE_GUI == 0)			
	csi_gpio_set_mux(GPIOB, PB1, PB1_INPUT);							//PB1 配置为输入
	csi_gpio_pull_mode(GPIOB, PB1, GPIO_PULLUP);						//PB1 上拉
	csi_gpio_int_enable(GPIOB, PB1);									//PB1 中断使能	
	csi_gpio_irq_mode(GPIOB, PB1, EXI_GRP1, GPIO_IRQ_FALLING_EDGE);		//PB1 下降沿产生中断，选择中断组1
	csi_exi_set_evtrg(EXI_TRGOUT1, EXI_TRGSRC_GRP1, 0);					//EXI 触发输出配置
	csi_exi_evtrg_enable(EXI_TRGOUT1);									//使能 EXI_TRGOUT1触发输出
	
	csi_gpio_set_mux(GPIOA, PA6, PA6_OUTPUT);							//PA6 output ，并在BT0中断里面翻转IO
	csi_gpio_set_high(GPIOA, PA6);										//PA6 output high;		
#endif
	
	tTimConfig.wTimeVal = 1000;											//BT定时值 = 1000us
	tTimConfig.eRunMode  = BT_RUN_CONT;									//BT计数器工作模式
	csi_bt_timer_init(BT0,&tTimConfig);									//BT0 定时	
	csi_bt_set_sync(BT0, BT_SYNCIN0, BT_SYNC_CONT, BT_TRG_SYCAREARM);	//外部触发bt0启动(SYNCIN0)
	csi_bt_sync_enable(BT0, BT_SYNCIN0);								//BT0 同步输入2使能
	
	//ETCB 初始化
	tEtbConfig.eChType = ETCB_ONE_TRG_ONE;  								//单个源触发单个目标
	tEtbConfig.eSrcIp  = ETCB_EXI_TRGOUT1;  	    						//EXI_TRGOUT1作为触发源
	tEtbConfig.eDstIp =  ETCB_BT0_SYNCIN0;   	   						//BT0 同步输入作为目标事件
	tEtbConfig.eTrgMode = ETCB_HARDWARE_TRG;
	ch = csi_etcb_ch_alloc(tEtbConfig.eChType);	    					//自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    					//ch < 0,则获取通道号失败
	iRet = csi_etcb_ch_init(ch, &tEtbConfig);
	
	while(1)
	{
		nop;
	}
	return iRet;
}


/** \brief exi trg bt stop/start: EXI通过ETCB触发BT停止/启动demo
 * 
 *  \brief	PB1外部下降沿触发BT0停止/启动，当BT处于运行状态时才有效，即第一次触发关闭BT，再次触发开启BT，
 * 			依次类推。
 *
 *  \param[in] none
 *  \return error code
// */
int exi_etcb_bt_stop_demo(void)
{
	int iRet = 0;
	volatile uint8_t ch = 0;
	csi_etcb_config_t tEtbConfig;				               				//ETCB 参数配置结构体
	csi_bt_time_config_t tTimConfig;										//BT 定时初始化参数结构体				

#if (USE_GUI == 0)			
	csi_gpio_set_mux(GPIOB, PB1, PB1_INPUT);								//PB1 配置为输入
	csi_gpio_pull_mode(GPIOB, PB1, GPIO_PULLUP);							//PB1 上拉
	csi_gpio_int_enable(GPIOB, PB1);										//PB1 中断使能	
	csi_gpio_irq_mode(GPIOB, PB1, EXI_GRP1, GPIO_IRQ_FALLING_EDGE);			//PB1 下降沿产生中断，选择中断组1
	csi_exi_set_evtrg(EXI_TRGOUT1, EXI_TRGSRC_GRP1, 0);						//EXI 触发输出配置
	csi_exi_evtrg_enable(EXI_TRGOUT1);										//使能 EXI_TRGOUT1触发输出
	
	csi_gpio_set_mux(GPIOA, PA6, PA6_OUTPUT);								//PA6 output ，并在BT0中断里面翻转IO
	csi_gpio_set_high(GPIOA, PA6);											//PA6 output high;	
#endif	
	
	tTimConfig.wTimeVal = 1000;												//BT定时值 = 1000us
	tTimConfig.eRunMode  = BT_RUN_CONT;										//BT计数器工作模式
	csi_bt_timer_init(BT0,&tTimConfig);										//BT0 定时	
	csi_bt_set_sync(BT0, BT_SYNCIN1, BT_SYNC_CONT, BT_TRG_AUTOAREARM);		//外部触发BT0停止(SYNCIN1)，连续模式
	csi_bt_sync_enable(BT0, BT_SYNCIN1);									//BT0 同步输入1使能
	
	csi_bt_start(BT0);	    												//启动BT0
	
	tEtbConfig.eChType = ETCB_ONE_TRG_ONE;  								//单个源触发单个目标
	tEtbConfig.eSrcIp  = ETCB_EXI_TRGOUT1;  	    						//EXI_TRGOUT1作为触发源
	tEtbConfig.eDstIp =  ETCB_BT0_SYNCIN1;   	    						//BT0 同步输入作为目标事件
	tEtbConfig.eTrgMode = ETCB_HARDWARE_TRG;
	ch = csi_etcb_ch_alloc(tEtbConfig.eChType);	    						//自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    						//ch < 0,则获取通道号失败
	iRet = csi_etcb_ch_init(ch, &tEtbConfig);
	
	while(1)
	{
		nop;
	}
	return iRet;	
}

/** \brief	exi trg adc samp：EXI通过ETCB触发ADC采样的demo
 * 
 *  \brief	PB1外部下降沿触发ADC采样，对应VIC中断不开启，即不进入PB1的外部中断；若需要进入PB1外部中断，需
 * 			开启对应的VIC中断，调用csi_gpio_vic_irq_enable接口函数
 *  
 *  \param[in] none
 *  \return error code
 */
int exi_etcb_adc_samp_demo(void)
{
	int iRet = 0;	
	int iChnlNum = 1;
	volatile uint8_t ch = 0;											//设置ADC总通道数
	csi_etcb_config_t tEtcbConfig;				               			//ETCB 参数配置结构体	
	csi_adc_config_t tAdcConfig;										//ADC初始化参数结构体	

#if (USE_GUI == 0)			
	csi_gpio_set_mux(GPIOB, PB1, PB1_INPUT);							//PB1 配置为输入
	csi_gpio_pull_mode(GPIOB, PB1, GPIO_PULLUP);						//PB1 上拉
	csi_gpio_int_enable(GPIOB, PB1);									//PB1 中断使能	
	csi_gpio_irq_mode(GPIOB, PB1, EXI_GRP1, GPIO_IRQ_FALLING_EDGE);		//PB1 下降沿产生中断，选择中断组1
	csi_exi_set_evtrg(EXI_TRGOUT1, EXI_TRGSRC_GRP1, 0);					//EXI 触发输出配置
	csi_exi_evtrg_enable(EXI_TRGOUT1);									//使能 EXI_TRGOUT1触发输出
	csi_gpio_set_mux(GPIOC,PC13, PC13_ADC_INA0);						//ADC 输入通道配置
#endif

	tAdcConfig.byClkDiv = 0x02;									//ADC clk两分频：clk = pclk/2
	tAdcConfig.eClkSel = ADC_CLK_PCLK;							//ADC clk选择：PCLK
	tAdcConfig.bySampHold = 0x06;								//ADC 采样时间： time = 16 + 6 = 22(ADC clk周期)
	tAdcConfig.eRunMode = ADC_RUN_ONCE;							//ADC 转换模式： 单次转换；
	tAdcConfig.eVrefSrc = ADCVERF_VDD_VSS;						//ADC 参考电压： 系统VDD	
	csi_adc_init(ADC0, &tAdcConfig);							//初始化ADC参数配置	
	
	csi_adc_set_seq_num(ADC0,iChnlNum);													//配置ADC总采样通道个数
	csi_adc_set_seqx(ADC0,0,ADC_INA0,ADC_CV_COUNT_1,ADC_AVG_COF_1,ADCSYNC_IN0);			//配置ADC采样通道0，触发信号为ADCSYNC_IN0
	csi_adc_set_sync(ADC0, ADC_SYNCIN0, ADC_SYNC_CONT, 0); 		//选择ADC_SYNCEN0同步事件，同步事件发生后延迟0us@pclk=40Mhz启动ADC
	
	//ETCB 初始化
	tEtcbConfig.eChType = ETCB_ONE_TRG_ONE;  							//单个源触发单个目标
	tEtcbConfig.eSrcIp  = ETCB_EXI_TRGOUT1;  	    					//EXI_TRGOUT1作为触发源
	tEtcbConfig.eDstIp =  ETCB_ADC0_SYNCIN0;   	   						//ADC0 同步输入作为目标事件
	tEtcbConfig.eTrgMode = ETCB_HARDWARE_TRG;
	ch = csi_etcb_ch_alloc(tEtcbConfig.eChType);	    					//自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    					//ch < 0,则获取通道号失败
	iRet = csi_etcb_ch_init(ch, &tEtcbConfig);
	
	while(1)
	{
		nop;
	}
	return iRet;
}

/** \brief  exi trg gpta capture / GPTA sync2 sync3区分捕获示例代码，测试低电平和周期时间，同时可计算出高电平时间
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
int exi_etcb_gpta_capture_demo(void)
{
	int iRet = 0;	
    volatile uint8_t ch;
	csi_etcb_config_t tEtcbConfig;				                         //ETCB 参数配置结构体
	csi_gpta_capture_config_t tCapConfig;	                             //GPTA 捕获参数配置结构体
//------------------------------------------------------------------------------------------------------------------------	
	csi_gpio_set_mux(GPIOA, PA3, PA3_INPUT);		
	csi_gpio_pull_mode(GPIOA, PA3, GPIO_PULLUP);						//PA3 上拉
	csi_gpio_irq_mode(GPIOA, PA3, EXI_GRP3, GPIO_IRQ_RISING_EDGE);		//PA3 上升沿产生中断  GPIO_IRQ_FALLING_EDGE  GPIO_IRQ_RISING_EDGE
	csi_gpio_int_enable(GPIOA, PA3);	
	csi_exi_set_evtrg(EXI_TRGOUT0, EXI_TRGSRC_GRP3, 1);
	csi_exi_evtrg_enable(EXI_TRGOUT0);	
	csi_gpio_irq_mode(GPIOA, PA3, EXI_GRP16, GPIO_IRQ_FALLING_EDGE);     //PA3 下降沿产生中断，选择中断组16
	csi_gpio_int_enable(GPIOA, PA3);                                     //PA3 中断使能                                   
	csi_exi_set_evtrg(EXI_TRGOUT5, EXI_TRGSRC_GRP16, 1);	
    csi_exi_evtrg_enable(EXI_TRGOUT5);
//------------------------------------------------------------------------------------------------------------------------			
	tEtcbConfig.eChType  = ETCB_ONE_TRG_ONE;  	                         //单个源触发单个目标
	tEtcbConfig.eSrcIp   = ETCB_EXI_TRGOUT0 ;  	                         //...作为触发源
	tEtcbConfig.eDstIp   = ETCB_GPTA0_SYNCIN2;                           //GPTA0 同步输入2作为目标事件
	tEtcbConfig.eTrgMode = ETCB_HARDWARE_TRG;
	ch = csi_etcb_ch_alloc(tEtcbConfig.eChType);	                     //自动获取空闲通道号,ch >= 0 获取成功						//ch < 0,则获取通道号失败		
	if(ch < 0)														     //ch < 0,则获取通道号失败
	{
		return -1;
	}
	iRet = csi_etcb_ch_init(ch, &tEtcbConfig);	
//------------------------------------------------------------------------------------------------------------------------			
	tEtcbConfig.eChType  = ETCB_ONE_TRG_ONE;  	                         //单个源触发单个目标
	tEtcbConfig.eSrcIp   = ETCB_EXI_TRGOUT5 ;  	                         //...作为触发源
	tEtcbConfig.eDstIp   = ETCB_GPTA0_SYNCIN3;                           //GPTA0 同步输入3作为目标事件
	tEtcbConfig.eTrgMode = ETCB_HARDWARE_TRG;
	ch = csi_etcb_ch_alloc(tEtcbConfig.eChType);	                     //自动获取空闲通道号,ch >= 0 获取成功						//ch < 0,则获取通道号失败		
	if(ch < 0)														     //ch < 0,则获取通道号失败
	{
		return -1;
	}
	iRet = csi_etcb_ch_init(ch, &tEtcbConfig);		
//------------------------------------------------------------------------------------------------------------------------
	tCapConfig.eWorkMode         	= GPTA_WORK_CAPTURE;                //GPTA工作模式：捕获/波形输出	
	tCapConfig.eCountMode    		= GPTA_CNT_UP;                      //GPTA计数模式：递增/递减/递增递减	
	tCapConfig.eRunMode	    	    = GPTA_RUN_CONT;        		    //GPTA运行模式：连续/一次性	
	tCapConfig.eCapMode      		= GPTA_CAP_SEPARATE;                //GPTA捕获模式：合并/分离	
	tCapConfig.byCapStopWrap 		= 1;                                //GPTA捕获次数：0/1/2/3
	tCapConfig.byCapLdaret   		= 0;                                //CMPA捕获载入后计数器设置(1h：捕获载入后计数器值重置;0h：捕获载入后计数器值不重置)
	tCapConfig.byCapLdbret   		= 1;  								//CMPB捕获载入后计数器设置(1h：捕获载入后计数器值重置;0h：捕获载入后计数器值不重置)
	tCapConfig.byCapLdcret   		= 0;								//CMPC捕获载入后计数器设置(1h：捕获载入后计数器值重置;0h：捕获载入后计数器值不重置)
	tCapConfig.byCapLddret   		= 0;   								//CMPD捕获载入后计数器设置(1h：捕获载入后计数器值重置;0h：捕获载入后计数器值不重置)                        
	csi_gpta_capture_init(GPTA0, &tCapConfig);
//------------------------------------------------------------------------------------------------------------------------	
	csi_gpta_int_enable(GPTA0, GPTA_INTSRC_CAPLD1);	

    csi_gpta_set_sync(GPTA0, GPTA_SYNCIN2, GPTA_SYNC_CONT, GPTA_AUTO_REARM_ZRO);//使能SYNCIN2外部触发
	csi_gpta_sync_enable(GPTA0, GPTA_SYNCIN2);
	csi_gpta_set_sync(GPTA0, GPTA_SYNCIN3, GPTA_SYNC_CONT, GPTA_AUTO_REARM_ZRO);//使能SYNCIN3外部触发
	csi_gpta_sync_enable(GPTA0, GPTA_SYNCIN3);
	
	csi_gpta_start(GPTA0);
	
    while(1)
	{		
		mdelay(200);                        
		mdelay(200);
	}			
	return iRet;
};

/** \brief	exi trg gptb start：EXI通过ETCB触发GPTB启动输出波形（10KHz/50%）的demo，可通过观察CHAX/CHAY来判断GPTB是否启动
 * 
 *  \brief	PA1外部下降沿触发GPTB启动。默认VIC中断不开启，即不进入PA1的外部中断；若需要进入PA1外部中断，需
 * 			开启对应的VIC中断，调用csi_gpio_vic_irq_enable接口函数
 *  
 *  \param[in] none
 *  \return error code
 */
int exi_etcb_gptb_start_demo(void)
{
	int iRet = 0;	
	volatile uint8_t ch;
	csi_etcb_config_t 			tEtcbCfg;			//ETCB参数配置结构体	
	csi_gptb_pwm_config_t 		tPwmCfg;			//GPTB捕获参数配置结构体	
	csi_gptb_pwm_ch_config_t  	tPwmChCfg;			//GPTB PWM通道参数配置结构体
//------------------------------------------------------------------------------------------------------------------------	
#if (USE_GUI == 0)
	csi_gpio_set_mux(GPIOA, PA1, PA1_INPUT);						//PA01 输入
	csi_gpio_pull_mode(GPIOA, PA1, GPIO_PULLUP);					//PA01 上拉使能
	csi_gpio_irq_mode(GPIOA, PA1,EXI_GRP1, GPIO_IRQ_FALLING_EDGE);	//PA01 下降沿产生中断
	csi_exi_set_evtrg(EXI_TRGOUT1, EXI_TRGSRC_GRP1, 1);				//EXI 触发配置
	csi_exi_evtrg_enable(EXI_TRGOUT1);								//使能 EXI_TRGOUT1触发输出
	csi_gpio_int_enable(GPIOA, PA1);	
	
	csi_gpio_set_mux(GPIOC, PC13, PC13_GPTB0_CHAX);	//初始化PC13为CHAX
	csi_gpio_set_mux(GPIOA, PA7, PA7_GPTB0_CHAY);	//初始化PA7为CHAY
#endif
//------------------------------------------------------------------------------------------------------------------------	
	tEtcbCfg.eChType  = ETCB_ONE_TRG_ONE;  			//单个源触发单个目标
	tEtcbCfg.eSrcIp   = ETCB_EXI_TRGOUT1 ;  		//EXI_TRGOUT1作为触发源
	tEtcbCfg.eDstIp   = ETCB_GPTB0_SYNCIN0;  		//GPTB0 SYNCIN0作为目标事件
	tEtcbCfg.eTrgMode = ETCB_HARDWARE_TRG;
	ch = csi_etcb_ch_alloc(tEtcbCfg.eChType);		//自动获取空闲通道号,ch >= 0 获取成功		
	if(ch < 0)										//ch < 0,则获取通道号失败
		return -1;
	iRet = csi_etcb_ch_init(ch, &tEtcbCfg);		
//------------------------------------------------------------------------------------------------------------------------	
	tPwmCfg.eWorkMode       = GPTB_WORK_WAVE;       //GPTB工作模式：捕获/波形输出
	tPwmCfg.eCountMode   	= GPTB_CNT_UPDN;        //GPTB计数模式：递增/递减/递增递减
	tPwmCfg.eRunMode    	= GPTB_RUN_CONT;        //GPTB运行模式：连续/一次性
	tPwmCfg.byDutyCycle 	= 50;					//GPTB输出PWM占空比			
	tPwmCfg.wFreq 			= 10000;				//GPTB输出PWM频率	
	csi_gptb_pwm_init(GPTB0, &tPwmCfg);
//------------------------------------------------------------------------------------------------------------------------	
	tPwmChCfg.eActionZRO    =   GPTB_ACT_LO;		//CNT=ZRO时，波形输出低电平
	tPwmChCfg.eActionPRD    =   GPTB_ACT_NA;		//CNT=PRD时，波形输出不变
	tPwmChCfg.eActionC1U    =   GPTB_ACT_HI;		//CNT=C1U时，波形输出高电平
	tPwmChCfg.eActionC1D    =   GPTB_ACT_LO;		//CNT=C1D时，波形输出低电平
	tPwmChCfg.eActionC2U    =   GPTB_ACT_NA;		//CNT=C2U时，波形输出不变
	tPwmChCfg.eActionC2D    =   GPTB_ACT_NA;		//CNT=C2D时，波形输出不变
	tPwmChCfg.eActionT1U    =   GPTB_ACT_NA;		//CNT=T1U时，波形输出不变
	tPwmChCfg.eActionT1D    =   GPTB_ACT_NA;		//CNT=T1D时，波形输出不变
	tPwmChCfg.eActionT2U    =   GPTB_ACT_NA;		//CNT=T2U时，波形输出不变
	tPwmChCfg.eActionT2D    =   GPTB_ACT_NA;		//CNT=T2D时，波形输出不变													
	tPwmChCfg.eC1Sel 		=   GPTB_COMPA;			//C1选择CMPA为数据源		
	tPwmChCfg.eC2Sel 		=   GPTB_COMPA;			//C2选择CMPA为数据源
	csi_gptb_set_channel(GPTB0, &tPwmChCfg,  GPTB_CHANNEL_1);
//------------------------------------------------------------------------------------------------------------------------
	csi_gptb_set_sync(GPTB0, GPTB_SYNCIN0, GPTB_SYNC_CONT, GPTB_AUTO_REARM_ZRO);//设置SYNCIN0为连续触发，CNT=ZRO自动REARM
	csi_gptb_sync_enable(GPTB0, GPTB_SYNCIN0);		//使能SYNCIN0外部触发
	
	while(1)
	{
		nop;
	}
	
	return iRet;
}

/** \brief	bt trg gptb start：BT0的周期事件通过ETCB触发GPTB启动输出波形（10KHz/50%）的demo，可通过观察CHAX/CHAY来判断GPTB是否启动
 * 
 *  \brief	PA1外部下降沿触发GPTB启动。默认VIC中断不开启，即不进入PA1的外部中断；若需要进入PA1外部中断，需
 * 			开启对应的VIC中断，调用csi_gpio_vic_irq_enable接口函数
 *  
 *  \param[in] none
 *  \return error code
 */
int bt_etcb_gptb_start_demo(void)
{
	int iRet = 0;	
	volatile uint8_t ch;
	csi_etcb_config_t 			tEtcbCfg;			//ETCB参数配置结构体	
	csi_bt_time_config_t 		tTimConfig;			//BT 定时初始化参数结构体
	csi_gptb_pwm_config_t 		tPwmCfg;			//GPTB捕获参数配置结构体	
	csi_gptb_pwm_ch_config_t  	tPwmChCfg;			//GPTB PWM通道参数配置结构体
//------------------------------------------------------------------------------------------------------------------------	
#if (USE_GUI == 0)
	csi_gpio_set_mux(GPIOC, PC13, PC13_GPTB0_CHAX);	//初始化PC13为CHAX
	csi_gpio_set_mux(GPIOA, PA7, PA7_GPTB0_CHAY);	//初始化PA7为CHAY
#endif
//------------------------------------------------------------------------------------------------------------------------
	//BT0初始化
	tTimConfig.wTimeVal  = 10000;					//BT定时值 = 1000us
	tTimConfig.eRunMode  = BT_RUN_CONT;				//BT计数器工作模式
	csi_bt_timer_init(BT0,&tTimConfig);				//BT0 定时
	csi_bt_set_evtrg(BT0, BT_TRGSRC_PEND);			//BT0 PEND事件触发输出
	csi_bt_evtrg_enable(BT0);						//BT0 事件触发输出使能
	csi_bt_start(BT0);
//------------------------------------------------------------------------------------------------------------------------	
	tEtcbCfg.eChType  = ETCB_ONE_TRG_ONE;  			//单个源触发单个目标
	tEtcbCfg.eSrcIp   = ETCB_BT0_TRGOUT ;  			//EXI_TRGOUT1作为触发源
	tEtcbCfg.eDstIp   = ETCB_GPTB0_SYNCIN0;  		//GPTB0 SYNCIN0作为目标事件
	tEtcbCfg.eTrgMode = ETCB_HARDWARE_TRG;
	ch = csi_etcb_ch_alloc(tEtcbCfg.eChType);		//自动获取空闲通道号,ch >= 0 获取成功		
	if(ch < 0)										//ch < 0,则获取通道号失败
		return -1;
	iRet = csi_etcb_ch_init(ch, &tEtcbCfg);		
//------------------------------------------------------------------------------------------------------------------------	
	tPwmCfg.eWorkMode       = GPTB_WORK_WAVE;       //GPTB工作模式：捕获/波形输出
	tPwmCfg.eCountMode   	= GPTB_CNT_UPDN;        //GPTB计数模式：递增/递减/递增递减
	tPwmCfg.eRunMode    	= GPTB_RUN_CONT;        //GPTB运行模式：连续/一次性
	tPwmCfg.byDutyCycle 	= 50;					//GPTB输出PWM占空比			
	tPwmCfg.wFreq 			= 10000;				//GPTB输出PWM频率	
	csi_gptb_pwm_init(GPTB0, &tPwmCfg);
//------------------------------------------------------------------------------------------------------------------------	
	tPwmChCfg.eActionZRO    =   GPTB_ACT_LO;		//CNT=ZRO时，波形输出低电平
	tPwmChCfg.eActionPRD    =   GPTB_ACT_NA;		//CNT=PRD时，波形输出不变
	tPwmChCfg.eActionC1U    =   GPTB_ACT_HI;		//CNT=C1U时，波形输出高电平
	tPwmChCfg.eActionC1D    =   GPTB_ACT_LO;		//CNT=C1D时，波形输出低电平
	tPwmChCfg.eActionC2U    =   GPTB_ACT_NA;		//CNT=C2U时，波形输出不变
	tPwmChCfg.eActionC2D    =   GPTB_ACT_NA;		//CNT=C2D时，波形输出不变
	tPwmChCfg.eActionT1U    =   GPTB_ACT_NA;		//CNT=T1U时，波形输出不变
	tPwmChCfg.eActionT1D    =   GPTB_ACT_NA;		//CNT=T1D时，波形输出不变
	tPwmChCfg.eActionT2U    =   GPTB_ACT_NA;		//CNT=T2U时，波形输出不变
	tPwmChCfg.eActionT2D    =   GPTB_ACT_NA;		//CNT=T2D时，波形输出不变													
	tPwmChCfg.eC1Sel 		=   GPTB_COMPA;			//C1选择CMPA为数据源		
	tPwmChCfg.eC2Sel 		=   GPTB_COMPA;			//C2选择CMPA为数据源
	csi_gptb_set_channel(GPTB0, &tPwmChCfg,  GPTB_CHANNEL_1);
//------------------------------------------------------------------------------------------------------------------------
	csi_gptb_set_sync(GPTB0, GPTB_SYNCIN0, GPTB_SYNC_CONT, GPTB_AUTO_REARM_ZRO);//设置SYNCIN0为连续触发，CNT=ZRO自动REARM
	csi_gptb_sync_enable(GPTB0, GPTB_SYNCIN0);						//使能SYNCIN0外部触发

	while(1)
	{
		nop;
	}
	
	return iRet;
}

/** \brief	bt trg adc samp：BT0的周期事件通过ETCB触发ADC采样的demo
 * 
 *  \brief	BT的定时周期触发ADC采样，调用csi_gpio_vic_irq_enable接口函数
 *  
 *  \param[in] none
 *  \return error code
 */
int bt_etcb_adc_samp_demo(void)
{
	int iRet = 0;	
	int iChnlNum = 1;
	volatile uint8_t ch = 0;											//设置ADC总通道数
	csi_etcb_config_t tEtcbCfg;				               			//ETCB 参数配置结构体	
	csi_adc_config_t tAdcConfig;										//ADC初始化参数结构体	
	csi_bt_time_config_t 		tTimConfig;			//BT 定时初始化参数结构体

#if (USE_GUI == 0)			
	csi_gpio_set_mux(GPIOB, PB1, PB1_INPUT);							//PB1 配置为输入
	csi_gpio_pull_mode(GPIOB, PB1, GPIO_PULLUP);						//PB1 上拉
	csi_gpio_set_mux(GPIOC,PC13, PC13_ADC_INA0);						//ADC 输入通道配置
#endif

	//BT0初始化
	tTimConfig.wTimeVal  = 10000;					//BT定时值 = 1000us
	tTimConfig.eRunMode  = BT_RUN_CONT;				//BT计数器工作模式
	csi_bt_timer_init(BT0,&tTimConfig);				//BT0 定时
	csi_bt_set_evtrg(BT0, BT_TRGSRC_PEND);			//BT0 PEND事件触发输出
	csi_bt_evtrg_enable(BT0);						//BT0 事件触发输出使能
	csi_bt_start(BT0);
	
//------------------------------------------------------------------------------------------------------------------------		
	tAdcConfig.byClkDiv = 0x02;									//ADC clk两分频：clk = pclk/2
	tAdcConfig.eClkSel = ADC_CLK_PCLK;							//ADC clk选择：PCLK
	tAdcConfig.bySampHold = 0x06;								//ADC 采样时间： time = 16 + 6 = 22(ADC clk周期)
	tAdcConfig.eRunMode = ADC_RUN_ONCE;							//ADC 转换模式： 单次转换；
	tAdcConfig.eVrefSrc = ADCVERF_VDD_VSS;						//ADC 参考电压： 系统VDD	
	csi_adc_init(ADC0, &tAdcConfig);							//初始化ADC参数配置	
	
	csi_adc_set_seq_num(ADC0,iChnlNum);													//配置ADC总采样通道个数
	csi_adc_set_seqx(ADC0,0,ADC_INA0,ADC_CV_COUNT_1,ADC_AVG_COF_1,ADCSYNC_IN0);			//配置ADC采样通道0，触发信号为ADCSYNC_IN0
	csi_adc_set_sync(ADC0, ADC_SYNCIN0, ADC_SYNC_CONT, 0); 		//选择ADC_SYNCEN0同步事件，同步事件发生后延迟0us@pclk=40Mhz启动ADC
	
//------------------------------------------------------------------------------------------------------------------------	
	tEtcbCfg.eChType  = ETCB_ONE_TRG_ONE;  			//单个源触发单个目标
	tEtcbCfg.eSrcIp   = ETCB_BT0_TRGOUT ;  			//EXI_TRGOUT1作为触发源
	tEtcbCfg.eDstIp   = ETCB_ADC0_SYNCIN0;  		//GPTB0 SYNCIN0作为目标事件
	tEtcbCfg.eTrgMode = ETCB_HARDWARE_TRG;
	ch = csi_etcb_ch_alloc(tEtcbCfg.eChType);		//自动获取空闲通道号,ch >= 0 获取成功		
	if(ch < 0)										//ch < 0,则获取通道号失败
		return -1;
	iRet = csi_etcb_ch_init(ch, &tEtcbCfg);	
	
	while(1)
	{
		nop;
	}
	return iRet;
}