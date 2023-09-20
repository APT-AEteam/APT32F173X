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
static uint32_t s_wGptbCapBuff[4] = {0};

#if (USE_GPTB_CALLBACK == 0)		
/** \brief	gptbx_int_handler: GPTB中断服务函数
 * 
 *  \brief 	GPTB发生中断时会调用此函数，函数在interrupt.c里定义为弱(weak)属性，默认不做处理；用户用到GPTB中
 * 			断时，请重新定义此函数，在此函数中进行对应中断处理，也可直接在interrupt.c里的函数里进行处理
 * 
 *  \param[in] none
 *  \return none
 */
ATTRIBUTE_ISR  void gptb0_int_handler(void)
{
	//用户直接在中断服务接口函数里处理中断，建议客户使用此模式
	volatile uint32_t wIsr = csp_gptb_get_isr(GPTB0);
	
	if(wIsr & GPTB_INT_PEND)				//PEND interrupt
	{
		csp_gptb_clr_isr(GPTB0, GPTB_INT_PEND);
		csi_gpio_toggle(GPIOA, PA6);		//PA6翻转
	}
	if(wIsr & GPTB_INT_CAPLD0)				//CAPLD0 interrupt
	{
		csp_gptb_clr_isr(GPTB0, GPTB_INT_CAPLD0);
		s_wGptbCapBuff[0] = csp_gptb_get_cmpa(GPTB0);
	}
	if(wIsr & GPTB_INT_CAPLD1)				//CAPLD1 interrupt
	{
		csp_gptb_clr_isr(GPTB0, GPTB_INT_CAPLD1);
		s_wGptbCapBuff[0] = csp_gptb_get_cmpa(GPTB0);
		s_wGptbCapBuff[1] = csp_gptb_get_cmpb(GPTB0);
	}
	if(wIsr & GPTB_INT_CAPLD2)				//CAPLD2 interrupt
	{
		csp_gptb_clr_isr(GPTB0, GPTB_INT_CAPLD2);
		s_wGptbCapBuff[0] = csp_gptb_get_cmpa(GPTB0);
		s_wGptbCapBuff[1] = csp_gptb_get_cmpb(GPTB0);
		s_wGptbCapBuff[2] = csp_gptb_get_cmpaa(GPTB0);
	}
	if(wIsr & GPTB_INT_CAPLD3)				//CAPLD3 interrupt
	{
		csp_gptb_clr_isr(GPTB0, GPTB_INT_CAPLD3);
		s_wGptbCapBuff[0] = csp_gptb_get_cmpa(GPTB0);
		s_wGptbCapBuff[1] = csp_gptb_get_cmpb(GPTB0);
		s_wGptbCapBuff[2] = csp_gptb_get_cmpaa(GPTB0);
		s_wGptbCapBuff[3] = csp_gptb_get_cmpba(GPTB0);
	}
}
#endif

/** \brief	gptb_timer_demo: gptb做基本定时器功能，默认使用向上计数，PEND中断
 * 
 *  \brief	csi初始化使用(开启)周期结束中断，并在中断里面翻转IO(需要打开PA6 IO配置注释)；
 * 			若不需要开启中断,需调用csi_gptb_int_disable接口函数，关闭周期结束中断。
 * 			定时1000us，翻转一次IO口，故IO翻转周期为2000us=2ms，500Hz
 * 
 * @ 工作模式:	GPTB_RUN_CONT: 连续工作模式。计数结束，计数器重新开始计数，周期执行
 * 				GPTB_RUN_ONCE: 单次工作模式。计数结束，计数器停止工作
 * 
 *  \param[in] none
 *  \return error code
 */
int gptb_timer_demo(void)
{
	int iRet = 0;
	csi_gptb_time_config_t tTimConfig;
	
#if (USE_GUI == 0)		
	csi_gpio_set_mux(GPIOA, PA6, PA6_OUTPUT);	//初始化PA6为输出
#endif
	
	tTimConfig.wTimeVal  = 1000;				//GPTB定时值 = 1000us
	tTimConfig.eRunMode  = GPTB_RUN_CONT;		//GPTB计数器工作模式
	csi_gptb_timer_init(GPTB0, &tTimConfig);	//初始化GPTB	
	
	csi_gptb_start(GPTB0);                  	//启动定时器

	return iRet;	
}

/** \brief GPTB合并捕获示例代码，PA1输入2KHz/50% PWM波形，测试周期时间。
 *          - sync2 sync3不区分，实现4次捕获
 *   		- 捕获4次产生一次捕获中断，每一次捕获后，计数器值均进行重置
 *     		- 由PA1外部扩展口,下降沿触发外部事件1，经过ETCB  触发GPTB的SYNCIN3端口捕获
 * 			- 信号由PA1的高低电平切换产生（一直高或低电平意味着没有触发）
 *          - CMPA捕获的是第一次周期值，CMPB捕获的是第二次周期值，CMPAA捕获的是第三次周期值,CMPBA捕获的是第四次周期值
 *  实测捕获到的值均为0xCD14 = 52500，故周期时间为500us。
 * 	注意：当GPTB工作时钟不同时，所支持捕获的PWM最小输出频率也不同。如GPTB工作在105MHz时，所支持捕获的最小PWM频率为105MHz/65536=1602Hz。
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
	csi_etcb_config_t tEtcbConfig;									//ETCB参数配置结构体
	csi_gptb_capture_config_t tCapCfg;								//GPTB捕获参数配置结构体
//------------------------------------------------------------------------------------------------------------------------ 	
	csi_gpio_set_mux(GPIOA, PA1, PA1_INPUT);						//PA01 输入
	csi_gpio_pull_mode(GPIOA, PA1, GPIO_PULLUP);					//PA01 上拉使能
	csi_gpio_irq_mode(GPIOA, PA1,EXI_GRP1, GPIO_IRQ_FALLING_EDGE);	//PA01 下降沿产生中断
	csi_gpio_int_enable(GPIOA, PA1);								//使能PA01中断	
	csi_exi_set_evtrg(EXI_TRGOUT1, EXI_TRGSRC_GRP1, 1);				//EXI 触发配置
	csi_exi_evtrg_enable(EXI_TRGOUT1);								//使能 EXI_TRGOUT1触发输出
//------------------------------------------------------------------------------------------------------------------------		
	tEtcbConfig.eChType  = ETCB_ONE_TRG_ONE;  						//单个源触发单个目标
	tEtcbConfig.eSrcIp   = ETCB_EXI_TRGOUT1;  						//EXI_TRGOUT1作为触发源
	tEtcbConfig.eDstIp   = ETCB_GPTB0_SYNCIN3;  					//GPTB0 SYNCIN3作为目标事件
	tEtcbConfig.eTrgMode = ETCB_HARDWARE_TRG;
	ch = csi_etcb_ch_alloc(tEtcbConfig.eChType);					//自动获取空闲通道号,ch >= 0 获取成功	
	if(ch < 0)														//ch < 0,则获取通道号失败
		return -1;	
	iRet = csi_etcb_ch_init(ch, &tEtcbConfig);	
//------------------------------------------------------------------------------------------------------------------------	
	tCapCfg.eWorkMode         	= GPTB_CAPTURE;                     //GPTB工作模式：捕获/波形输出	
	tCapCfg.eCountMode    		= GPTB_UPCNT;                       //GPTB计数模式：递增/递减/递增递减	
	tCapCfg.eRunMode	    	= GPTB_RUN_CONT;        		    //GPTB运行模式：连续/一次性	
	tCapCfg.eCapMode      		= GPTB_CAP_MERGE;                	//GPTB捕获模式：合并/分离	
	tCapCfg.byCapStopWrap 		= 3;                                //GPTB捕获次数：0/1/2/3
	tCapCfg.byCapLdaret   		= 1;                                //CMPA捕获载入后计数器设置(1h：捕获载入后计数器值重置;0h：捕获载入后计数器值不重置)
	tCapCfg.byCapLdbret   		= 1;  								//CMPB捕获载入后计数器设置(1h：捕获载入后计数器值重置;0h：捕获载入后计数器值不重置)
	tCapCfg.byCapLdcret   		= 1;								//CMPC捕获载入后计数器设置(1h：捕获载入后计数器值重置;0h：捕获载入后计数器值不重置)
	tCapCfg.byCapLddret   		= 1;   								//CMPD捕获载入后计数器设置(1h：捕获载入后计数器值重置;0h：捕获载入后计数器值不重置)                        
	csi_gptb_capture_init(GPTB0, &tCapCfg);
//------------------------------------------------------------------------------------------------------------------------
	csi_gptb_int_enable(GPTB0, GPTB_INTSRC_CAPLD3);
    csi_gptb_set_sync(GPTB0, GPTB_SYNCIN3, GPTB_SYNC_CONT, GPTB_AUTO_REARM_ZRO);//使能SYNCIN3外部触发
	csi_gptb_start(GPTB0);
	
    while(1)
	{
		mdelay(1);
	}
	return iRet;
}

/** \brief GPTB 合并捕获示例代码，PA1输入2KHz/30% PWM波形，测试低电平时间
 *          - sync2 sync3不区分，实现1次捕获
 *   		- 捕获1次产生一次捕获中断，CMPA捕获载入后，计数器值进行重置
 *     		- 由PA1下降沿产生外部事件0，经过ETCB  触发sync0，重置和启动计数器
 *          - 由PA1外部扩展口，上升沿产生外部事件5，经过ETCB  触发sync3捕获，上升沿捕获值存放在CMPA中
 * 			- 信号由PA1的高低电平切换产生（一直高或低电平意味着没有触发）
 *          - CMPA捕获的是低电平时间
 * 实测CMPA捕获到的值为0x8F8C = 36748，故低电平时间为350us。
 * 注意：当GPTB工作时钟不同时，所支持捕获的PWM最小输出频率也不同。如GPTB工作在105MHz时，所支持捕获的最小PWM频率为105MHz/65536=1602Hz。
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
	csi_etcb_config_t tEtcbConfig;									//ETCB 参数配置结构体	
	csi_gptb_capture_config_t tCapCfg;								//GPTB捕获参数配置结构体
//------------------------------------------------------------------------------------------------------------------------	
	csi_gpio_set_mux(GPIOA, PA1, PA1_INPUT);						//PA01 输入
	csi_gpio_pull_mode(GPIOA, PA1, GPIO_PULLUP);					//PA01 上拉使能
	csi_gpio_irq_mode(GPIOA, PA1,EXI_GRP1, GPIO_IRQ_FALLING_EDGE);	//PA01 下降沿产生中断
	csi_exi_set_evtrg(EXI_TRGOUT1, EXI_TRGSRC_GRP1, 1);				//EXI 触发配置
	csi_exi_evtrg_enable(EXI_TRGOUT1);								//使能 EXI_TRGOUT1触发输出
	csi_gpio_irq_mode(GPIOA, PA1,EXI_GRP16, GPIO_IRQ_RISING_EDGE);	//PA01 上升沿产生中断
	csi_exi_set_evtrg(EXI_TRGOUT5, EXI_TRGSRC_GRP16, 1);			//EXI 触发配置
	csi_exi_evtrg_enable(EXI_TRGOUT5);								//使能 EXI_TRGOUT5触发输出
	csi_gpio_int_enable(GPIOA, PA1);								//使能PA01中断	
//------------------------------------------------------------------------------------------------------------------------
	tEtcbConfig.eChType  = ETCB_ONE_TRG_ONE;  						//单个源触发单个目标
	tEtcbConfig.eSrcIp   = ETCB_EXI_TRGOUT1 ;  						//EXI_TRGOUT1作为触发源
	tEtcbConfig.eDstIp   = ETCB_GPTB0_SYNCIN0;  					//GPTB0 SYNCIN0作为目标事件
	tEtcbConfig.eTrgMode = ETCB_HARDWARE_TRG;
	ch = csi_etcb_ch_alloc(tEtcbConfig.eChType);					//自动获取空闲通道号,ch >= 0 获取成功		
	if(ch < 0)														//ch < 0,则获取通道号失败
		return -1;
	iRet = csi_etcb_ch_init(ch, &tEtcbConfig);			
//------------------------------------------------------------------------------------------------------------------------		
	tEtcbConfig.eChType  = ETCB_ONE_TRG_ONE;  						//单个源触发单个目标
	tEtcbConfig.eSrcIp   = ETCB_EXI_TRGOUT5 ;  						//EXI_TRGOUT5作为触发源
	tEtcbConfig.eDstIp   = ETCB_GPTB0_SYNCIN3;  					//GPTB0 SYNCIN3作为目标事件
	tEtcbConfig.eTrgMode = ETCB_HARDWARE_TRG;
	ch = csi_etcb_ch_alloc(tEtcbConfig.eChType);					//自动获取空闲通道号,ch >= 0 获取成功		
	if(ch < 0)														//ch < 0,则获取通道号失败
		return -1;
	iRet = csi_etcb_ch_init(ch, &tEtcbConfig);	
//------------------------------------------------------------------------------------------------------------------------	
	tCapCfg.eWorkMode         	= GPTB_CAPTURE;                     //GPTB工作模式：捕获/波形输出	
	tCapCfg.eCountMode    		= GPTB_UPCNT;                       //GPTB计数模式：递增/递减/递增递减	
	tCapCfg.eRunMode	    	= GPTB_RUN_CONT;        		    //GPTB运行模式：连续/一次性	
	tCapCfg.eCapMode      		= GPTB_CAP_MERGE;                	//GPTB捕获模式：合并/分离	
	tCapCfg.byCapStopWrap 		= 0;                                //GPTB捕获次数：0/1/2/3
	tCapCfg.byCapLdaret   		= 1;                                //CMPA捕获载入后计数器设置(1h：捕获载入后计数器值重置;0h：捕获载入后计数器值不重置)
	tCapCfg.byCapLdbret   		= 0;  								//CMPB捕获载入后计数器设置(1h：捕获载入后计数器值重置;0h：捕获载入后计数器值不重置)
	tCapCfg.byCapLdcret   		= 0;								//CMPC捕获载入后计数器设置(1h：捕获载入后计数器值重置;0h：捕获载入后计数器值不重置)
	tCapCfg.byCapLddret   		= 0;   								//CMPD捕获载入后计数器设置(1h：捕获载入后计数器值重置;0h：捕获载入后计数器值不重置)                        
	csi_gptb_capture_init(GPTB0, &tCapCfg);
//------------------------------------------------------------------------------------------------------------------------
	csi_gptb_int_enable(GPTB0, GPTB_INTSRC_CAPLD0);
	csi_gptb_set_sync(GPTB0, GPTB_SYNCIN0, GPTB_SYNC_CONT, GPTB_AUTO_REARM_ZRO);//使能SYNCIN0外部触发
    csi_gptb_set_sync(GPTB0, GPTB_SYNCIN3, GPTB_SYNC_CONT, GPTB_AUTO_REARM_ZRO);//使能SYNCIN3外部触发
	csi_gptb_start(GPTB0);
    while(1)
	{
		mdelay(1);
	}
	return iRet;
}

/** \brief GPTB 区分捕获示例代码，PA1输入2KHz/30% PWM波形，测试高电平和周期时间，同时可计算出低电平时间
 *          - sync2 sync3区分，实现2次捕获
 *   		- 捕获2次产生一次捕获中断，CMPB捕获载入后，计数器值进行重置
 *     		- 由PA1下降沿产生外部事件1，经过ETCB  触发sync2捕获，捕获值存放在CMPA中
 *          - 由PA1外部扩展口，上升沿产生外部事件5，经过ETCB  触发sync3捕获，捕获值存放在CMPB中
 * 			- 信号由PA1的高低电平切换产生（一直高或低电平意味着没有触发）
 *          - CMPA捕获值为高电平时间，CMPB捕获值为周期时间，故低电平时间为 CMPB - CMPA。  
 * 实测CMPA捕获值为0x3D87 = 36748，故高电平时间150us；CMPB捕获值为0xCD14 = 52500，故周期时间为350us。
 * 注意：当GPTB工作时钟不同时，所支持捕获的PWM最小输出频率也不同。如GPTB工作在105MHz时，所支持捕获的最小PWM频率为105MHz/65536=1602Hz。
 *  \param[in] none
 *  \return error code
				—————          —————           —————         
				|        |          |        |           |        |    
				|        |          |        |           |        |        
PA1输入波形———        ——————         ——————         ———
			   CMPB      CMPA      CMPB      CMPA       CMPB      CMPA  
*/
int gptb_capture_sync_demo2(void)
{
	int iRet = 0;	
    volatile uint8_t ch;
	csi_etcb_config_t tEtcbConfig;									//ETCB 参数配置结构体	
	csi_gptb_capture_config_t tCapCfg;								//GPTB捕获参数配置结构体
//------------------------------------------------------------------------------------------------------------------------	
	csi_gpio_set_mux(GPIOA, PA1, PA1_INPUT);						//PA01 输入
	csi_gpio_pull_mode(GPIOA, PA1, GPIO_PULLUP);					//PA01 上拉使能
	csi_gpio_irq_mode(GPIOA, PA1,EXI_GRP1, GPIO_IRQ_FALLING_EDGE);	//PA01 下降沿产生中断
	csi_exi_set_evtrg(EXI_TRGOUT1, EXI_TRGSRC_GRP1, 1);				//EXI 触发配置
	csi_exi_evtrg_enable(EXI_TRGOUT1);								//使能 EXI_TRGOUT1触发输出
	csi_gpio_irq_mode(GPIOA, PA1,EXI_GRP16, GPIO_IRQ_RISING_EDGE);	//PA01 上升沿产生中断
	csi_exi_set_evtrg(EXI_TRGOUT5, EXI_TRGSRC_GRP16, 1);			//EXI 触发配置
	csi_exi_evtrg_enable(EXI_TRGOUT5);								//使能 EXI_TRGOUT5触发输出
	csi_gpio_int_enable(GPIOA, PA1);	
//------------------------------------------------------------------------------------------------------------------------	
	tEtcbConfig.eChType  = ETCB_ONE_TRG_ONE;  						//单个源触发单个目标
	tEtcbConfig.eSrcIp   = ETCB_EXI_TRGOUT1 ;  						//EXI_TRGOUT1作为触发源
	tEtcbConfig.eDstIp   = ETCB_GPTB0_SYNCIN2;  					//GPTB0 SYNCIN2作为目标事件
	tEtcbConfig.eTrgMode = ETCB_HARDWARE_TRG;
	ch = csi_etcb_ch_alloc(tEtcbConfig.eChType);					//自动获取空闲通道号,ch >= 0 获取成功		
	if(ch < 0)														//ch < 0,则获取通道号失败
		return -1;
	iRet = csi_etcb_ch_init(ch, &tEtcbConfig);	
//------------------------------------------------------------------------------------------------------------------------		
	tEtcbConfig.eChType  = ETCB_ONE_TRG_ONE;  						//单个源触发单个目标
	tEtcbConfig.eSrcIp   = ETCB_EXI_TRGOUT5 ;  						//EXI_TRGOUT5作为触发源
	tEtcbConfig.eDstIp   = ETCB_GPTB0_SYNCIN3;  					//GPTB0 SYNCIN3作为目标事件
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
	tCapCfg.byCapStopWrap 		= 1;                                //GPTB捕获次数：0/1/2/3
	tCapCfg.byCapLdaret   		= 0;                                //CMPA捕获载入后计数器设置(1h：捕获载入后计数器值重置;0h：捕获载入后计数器值不重置)
	tCapCfg.byCapLdbret   		= 1;  								//CMPB捕获载入后计数器设置(1h：捕获载入后计数器值重置;0h：捕获载入后计数器值不重置)
	tCapCfg.byCapLdcret   		= 0;								//CMPC捕获载入后计数器设置(1h：捕获载入后计数器值重置;0h：捕获载入后计数器值不重置)
	tCapCfg.byCapLddret   		= 0;   								//CMPD捕获载入后计数器设置(1h：捕获载入后计数器值重置;0h：捕获载入后计数器值不重置)                        
	csi_gptb_capture_init(GPTB0, &tCapCfg);
//------------------------------------------------------------------------------------------------------------------------
    csi_gptb_int_enable(GPTB0, GPTB_INTSRC_CAPLD1);
	csi_gptb_set_sync(GPTB0, GPTB_SYNCIN2, GPTB_SYNC_CONT, GPTB_AUTO_REARM_ZRO);//使能SYNCIN2外部触发
	csi_gptb_set_sync(GPTB0, GPTB_SYNCIN3, GPTB_SYNC_CONT, GPTB_AUTO_REARM_ZRO);//使能SYNCIN3外部触发
	csi_gptb_start(GPTB0);
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