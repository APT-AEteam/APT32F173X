/***********************************************************************//** 
 * \file  gpta_demo.c
 * \brief  TIMER_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-11 <td>V0.0 <td>ljy     <td>initial
 * <tr><td> 2023-3-21 <td>V1.0 <td>YYM     <td>initial
 * <tr><td> 2023-9-07 <td>V1.1 <td>LHY     <td>modify
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include "csi_drv.h"
#include "board_config.h"

//#include "demo.h"
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

#if (USE_GPTA_CALLBACK == 0)
static uint32_t s_wGptaCapBuff[4] = {0};
/** \brief gpta0_int_handler: gpta中断服务函数
 * 
 *  \brief GPTA发生中断时会调用此函数，函数在interrupt.c里定义为弱(weak)属性，默认不做处理；用户用到GPTA中
 *         断时，请重新定义此函数，在此函数中进行对应中断处理，也可直接在interrupt.c里的函数里进行处理
 * 
 *  \param[in] none
 *  \return none
 */
ATTRIBUTE_ISR  void gpta0_int_handler(void)
{
	//用户直接在中断服务接口函数里处理中断，建议客户使用此模式
	volatile uint32_t wMisr = csp_gpta_get_isr(GPTA0);
	
	if((wMisr & GPTA_INT_PEND) == GPTA_INT_PEND)
	{	
	    csp_gpta_clr_isr(GPTA0, GPTA_INT_PEND);
	}
	if((wMisr & GPTA_INT_TRGEV0) == GPTA_INT_TRGEV0)
	{		
	    csp_gpta_clr_isr(GPTA0, GPTA_INT_TRGEV0);
	}
	if((wMisr & GPTA_INT_TRGEV1) == GPTA_INT_TRGEV1)
	{		
	    csp_gpta_clr_isr(GPTA0, GPTA_INT_TRGEV1);
	   	
	}
    if((wMisr & GPTA_INT_CAPLD0) == GPTA_INT_CAPLD0)
	{		
		s_wGptaCapBuff[0]=csp_gpta_get_cmpa(GPTA0);
		csp_gpta_clr_isr(GPTA0, GPTA_INT_CAPLD0);			
	}
	if((wMisr & GPTA_INT_CAPLD1) == GPTA_INT_CAPLD1)
	{		
     	s_wGptaCapBuff[0]=csp_gpta_get_cmpa(GPTA0);
		s_wGptaCapBuff[1]=csp_gpta_get_cmpb(GPTA0);
		csp_gpta_clr_isr(GPTA0, GPTA_INT_CAPLD1);			
	}
    if((wMisr & GPTA_INT_CAPLD2) == GPTA_INT_CAPLD2)
	{		
     	s_wGptaCapBuff[0]=csp_gpta_get_cmpa(GPTA0);
		s_wGptaCapBuff[1]=csp_gpta_get_cmpb(GPTA0);
		s_wGptaCapBuff[2]=csp_gpta_get_cmpaa(GPTA0);
		csp_gpta_clr_isr(GPTA0, GPTA_INT_CAPLD2);			
	}
	if((wMisr & GPTA_INT_CAPLD3) == GPTA_INT_CAPLD3)
	{		
     	s_wGptaCapBuff[0]=csp_gpta_get_cmpa(GPTA0);
		s_wGptaCapBuff[1]=csp_gpta_get_cmpb(GPTA0);
		s_wGptaCapBuff[2]=csp_gpta_get_cmpaa(GPTA0);
		s_wGptaCapBuff[3]=csp_gpta_get_cmpba(GPTA0);
		csp_gpta_clr_isr(GPTA0, GPTA_INT_CAPLD3);			
	}	
	
    if((wMisr & GPTA_INT_CBU) == GPTA_INT_CBU)
	{	
	    csp_gpta_clr_isr(GPTA0, GPTA_INT_CBU);
	}
	
	
}

/** \brief gpta1_int_handler: gpta中断服务函数
 * 
 *  \brief GPTA发生中断时会调用此函数，函数在interrupt.c里定义为弱(weak)属性，默认不做处理；用户用到GPTA中
 *         断时，请重新定义此函数，在此函数中进行对应中断处理，也可直接在interrupt.c里的函数里进行处理
 * 
 *  \param[in] none
 *  \return none
 */
ATTRIBUTE_ISR  void gpta1_int_handler(void)
{
//用户直接在中断服务接口函数里处理中断，建议客户使用此模式
	volatile uint32_t wMisr = csp_gpta_get_isr(GPTA1);
	
	if((wMisr & GPTA_INT_PEND) == GPTA_INT_PEND)
	{	
	    csp_gpta_clr_isr(GPTA1, GPTA_INT_PEND);
	}
	if((wMisr & GPTA_INT_TRGEV0) == GPTA_INT_TRGEV0)
	{		
	    csp_gpta_clr_isr(GPTA1, GPTA_INT_TRGEV0);
	}
	if((wMisr & GPTA_INT_TRGEV1) == GPTA_INT_TRGEV1)
	{		
	    csp_gpta_clr_isr(GPTA1, GPTA_INT_TRGEV1);
	   	
	}
    if((wMisr & GPTA_INT_CAPLD0) == GPTA_INT_CAPLD0)
	{		
		s_wGptaCapBuff[0]=csp_gpta_get_cmpa(GPTA1);
		csp_gpta_clr_isr(GPTA1, GPTA_INT_CAPLD0);			
	}
	if((wMisr & GPTA_INT_CAPLD1) == GPTA_INT_CAPLD1)
	{		
     	s_wGptaCapBuff[0]=csp_gpta_get_cmpa(GPTA1);
		s_wGptaCapBuff[1]=csp_gpta_get_cmpb(GPTA1);
		csp_gpta_clr_isr(GPTA1, GPTA_INT_CAPLD1);			
	}
    if((wMisr & GPTA_INT_CAPLD2) == GPTA_INT_CAPLD2)
	{		
     	s_wGptaCapBuff[0]=csp_gpta_get_cmpa(GPTA1);
		s_wGptaCapBuff[1]=csp_gpta_get_cmpb(GPTA1);
		s_wGptaCapBuff[2]=csp_gpta_get_cmpaa(GPTA1);
		csp_gpta_clr_isr(GPTA1, GPTA_INT_CAPLD2);			
	}
	if((wMisr & GPTA_INT_CAPLD3) == GPTA_INT_CAPLD3)
	{		
     	s_wGptaCapBuff[0]=csp_gpta_get_cmpa(GPTA1);
		s_wGptaCapBuff[1]=csp_gpta_get_cmpb(GPTA1);
		s_wGptaCapBuff[2]=csp_gpta_get_cmpaa(GPTA1);
		s_wGptaCapBuff[3]=csp_gpta_get_cmpba(GPTA1);
		csp_gpta_clr_isr(GPTA1, GPTA_INT_CAPLD3);			
	}	
	
    if((wMisr & GPTA_INT_CBU) == GPTA_INT_CBU)
	{	
	    csp_gpta_clr_isr(GPTA1, GPTA_INT_CBU);
	}
}

#endif



/** \brief gpta timer
 * 
 *  \param[in] none
 *  \return error code
 */
int gpta_timer_demo(void)
{
	int iRet = 0;
	
	csi_gpta_timer_init(GPTA0, 10000);		//初始化GPTA0, 定时10000us； GPTA定时，默认采用向上计数，PEND中断
	
	csi_gpta_start(GPTA0);                  //启动定时器

	return iRet;	
}

/** \brief GPTA sync2 sync3合并捕获示例代码，测试周期时间
 *          //sync2 sync3不区分，实现4次捕获
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

int gpta_capture_sync_demo0(void)
{
	int iRet = 0;
    volatile uint8_t ch;

	csi_gpio_set_mux(GPIOA, PA1, PA1_INPUT);		
	csi_gpio_pull_mode(GPIOA, PA1, GPIO_PULLUP);				    //PA1 上拉
	csi_gpio_irq_mode(GPIOA, PA1, EXI_GRP16, GPIO_IRQ_FALLING_EDGE);//PA1 下降沿产生中断，选择中断组16
	csi_gpio_int_enable(GPIOA,PA1);                                 //PA1 中断使能                                    
	csi_exi_set_evtrg(EXI_TRGOUT5, EXI_TRGSRC_GRP16, 1);	 
	csi_exi_evtrg_enable(EXI_TRGOUT5);		 
			
	csi_etcb_config_t tEtcbConfig;				                    //ETCB 参数配置结构体	
	tEtcbConfig.eChType  = ETCB_ONE_TRG_ONE;  	                    //单个源触发单个目标
	tEtcbConfig.eSrcIp   = ETCB_EXI_TRGOUT5 ;  	                    //...作为触发源
	tEtcbConfig.eDstIp   = ETCB_GPTA0_SYNCIN3;                      //GPTB0 同步输入2作为目标事件
	tEtcbConfig.eTrgMode = ETCB_HARDWARE_TRG;
	ch = csi_etcb_ch_alloc(tEtcbConfig.eChType);                    //自动获取空闲通道号,ch >= 0 获取成功						//ch < 0,则获取通道号失败		
	if(ch < 0)
	{
		return -1;
	}
	iRet = csi_etcb_ch_init(ch, &tEtcbConfig);	
	
	csi_gpta_captureconfig_t tPwmCfg;								  
	tPwmCfg.byWorkmod         = GPTA_CAPTURE;                       //WAVE or CAPTURE    //计数或捕获	
	tPwmCfg.byCountingMode    = GPTA_UPCNT;                         //CNYMD  //计数方向
	tPwmCfg.byOneshotMode     = GPTA_OP_CONT; 
	tPwmCfg.byStartSrc        = GPTA_SYNC;				            //软件使能同步触发使能控制（RSSR中START控制位）//启动方式
	tPwmCfg.byPscld           = GPTA_LDPSCR_ZRO;                    //PSCR(分频)活动寄存器载入控制。活动寄存器在配置条件满足时，从影子寄存器载入更新值	
	tPwmCfg.byCaptureCapmd    = 0;                                  //0:连续捕捉模式    1h：一次性捕捉模式
	tPwmCfg.byCaptureStopWrap = 4-1;                                //Capture模式下，捕获事件计数器周期设置值
	tPwmCfg.byCaptureLdaret   = 0;                                  //CMPA捕捉载入后，计数器值计数状态控制位(1h：CMPA触发后，计数器值进行重置;0h：CMPA触发后，计数器值不进行重置)
	tPwmCfg.byCaptureLdbret   = 0; 
	tPwmCfg.byCaptureLdaaret  = 0;  
	tPwmCfg.byCaptureLdbaret  = 1;  
	tPwmCfg.byCaptureCapmdSel = GPTA_CMPMD_SEL_NODIFF;
	csi_gpta_capture_init(GPTA0, &tPwmCfg);
                                                        	
	csi_gpta_int_enable(GPTA0, GPTA_INTSRC_CAPLD3);	
    csi_gpta_set_sync(GPTA0, GPTA_TRG_SYNCEN3, GPTA_TRG_CONT, GPTA_AUTO_REARM_ZRO);//使能SYNCIN2外部触发
	
	csi_gpta_start(GPTA0);//start  timer
	
    while(1)
	{	      
		mdelay(200);                        
		mdelay(200);
	}			
	return iRet;
};

/** \brief GPTA sync2 sync3合并捕获示例代码，测试低电平时间
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

int gpta_capture_sync_demo1(void)
{
	int iRet = 0;	
    volatile uint8_t ch;

	csi_gpio_set_mux(GPIOA, PA1, PA1_INPUT);		
	csi_gpio_pull_mode(GPIOA, PA1, GPIO_PULLUP);						//PA1 上拉
	
	csi_gpio_irq_mode(GPIOA, PA1,EXI_GRP1, GPIO_IRQ_FALLING_EDGE);		//PA1 下降沿产生中断 
	csi_gpio_int_enable(GPIOA, PA1);	
	csi_exi_set_evtrg(EXI_TRGOUT0, EXI_TRGSRC_GRP1, 1);
	csi_exi_evtrg_enable(EXI_TRGOUT0);		
	
	csi_gpio_irq_mode(GPIOA, PA1, EXI_GRP16, GPIO_IRQ_RISING_EDGE);     //PA1 上升沿产生中断，选择中断组16
	csi_gpio_int_enable(GPIOA, PA1);                                    //PA1 中断使能                                    
	csi_exi_set_evtrg(EXI_TRGOUT5, EXI_TRGSRC_GRP16, 1);	 
	csi_exi_evtrg_enable(EXI_TRGOUT5);
	
	csi_etcb_config_t tEtcbConfig;                                      //ETCB 参数配置结构体
	tEtcbConfig.eChType  = ETCB_ONE_TRG_ONE;  	                        //单个源触发单个目标
	tEtcbConfig.eSrcIp   = ETCB_EXI_TRGOUT0 ;                       	//...作为触发源
	tEtcbConfig.eDstIp   = ETCB_GPTA0_SYNCIN0;                          //GPTA0 同步输入0作为目标事件
	tEtcbConfig.eTrgMode = ETCB_HARDWARE_TRG;
	ch = csi_etcb_ch_alloc(tEtcbConfig.eChType);	                    //自动获取空闲通道号,ch >= 0 获取成功						//ch < 0,则获取通道号失败		
	iRet = csi_etcb_ch_init(ch, &tEtcbConfig);			
	
	tEtcbConfig.eChType  = ETCB_ONE_TRG_ONE;  	                        //单个源触发单个目标
	tEtcbConfig.eSrcIp   = ETCB_EXI_TRGOUT5 ;  	                        //...作为触发源
	tEtcbConfig.eDstIp   = ETCB_GPTA0_SYNCIN3;                          //GPTB0 同步输入2作为目标事件
	tEtcbConfig.eTrgMode = ETCB_HARDWARE_TRG;
	ch = csi_etcb_ch_alloc(tEtcbConfig.eChType);	                    //自动获取空闲通道号,ch >= 0 获取成功						//ch < 0,则获取通道号失败		
	iRet = csi_etcb_ch_init(ch, &tEtcbConfig);	
	
	csi_gpta_captureconfig_t tPwmCfg;								    //ETCB 参数配置结构体
	tPwmCfg.byWorkmod         = GPTA_CAPTURE;                           //WAVE or CAPTURE    //计数或捕获	
	tPwmCfg.byCountingMode    = GPTA_UPCNT;                             //CNYMD  //计数方向
	tPwmCfg.byOneshotMode     = GPTA_OP_CONT; 
	tPwmCfg.byStartSrc        = GPTA_SYNC;				                //软件使能同步触发使能控制（RSSR中START控制位）//启动方式
	tPwmCfg.byPscld           = GPTA_LDPSCR_ZRO;                        //PSCR(分频)活动寄存器载入控制。活动寄存器在配置条件满足时，从影子寄存器载入更新值	
	tPwmCfg.byCaptureCapmd    = 0;                                      //0:连续捕捉模式    1h：一次性捕捉模式
	tPwmCfg.byCaptureStopWrap = 1-1;                                    //Capture模式下，捕获事件计数器周期设置值
	tPwmCfg.byCaptureLdaret   = 1;                                      //CMPA捕捉载入后，计数器值计数状态控制位(1h：CMPA触发后，计数器值进行重置;0h：CMPA触发后，计数器值不进行重置)
	tPwmCfg.byCaptureLdbret   = 0; 
	tPwmCfg.byCaptureLdaaret  = 0;  
	tPwmCfg.byCaptureLdbaret  = 0;  
	tPwmCfg.byCaptureCapmdSel = GPTA_CMPMD_SEL_NODIFF;
	csi_gpta_capture_init(GPTA0, &tPwmCfg);
	
	csi_gpta_int_enable(GPTA0, GPTA_INTSRC_CAPLD0);		
	
	csp_gpta_set_phsr(GPTA0, 0x00);
	csp_gpta_phsen_enable(GPTA0);
	
	csi_gpta_set_sync(GPTA0, GPTA_TRG_SYNCEN0, GPTA_TRG_CONT, GPTA_AUTO_REARM_ZRO);//使能SYNCIN0外部触发
    csi_gpta_set_sync(GPTA0, GPTA_TRG_SYNCEN3, GPTA_TRG_CONT, GPTA_AUTO_REARM_ZRO);//使能SYNCIN2外部触发
	
	csi_gpta_start(GPTA0);//start  timer
	
    while(1)
	{
		mdelay(200);                        
		mdelay(200);
	}			
	return iRet;
};

/** \brief GPTA sync2 sync3区分捕获示例代码，测试低电平和周期时间，同时可计算出高电平时间
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

int gpta_capture_sync_demo2(void)
{
	int iRet = 0;	
    volatile uint8_t ch;
	
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
			
	csi_etcb_config_t tEtcbConfig;				                         //ETCB 参数配置结构体	
	tEtcbConfig.eChType  = ETCB_ONE_TRG_ONE;  	                         //单个源触发单个目标
	tEtcbConfig.eSrcIp   = ETCB_EXI_TRGOUT0 ;  	                         //...作为触发源
	tEtcbConfig.eDstIp   = ETCB_GPTA0_SYNCIN2;                           //GPTA0 同步输入2作为目标事件
	tEtcbConfig.eTrgMode = ETCB_HARDWARE_TRG;
	ch = csi_etcb_ch_alloc(tEtcbConfig.eChType);	                     //自动获取空闲通道号,ch >= 0 获取成功						//ch < 0,则获取通道号失败		
	iRet = csi_etcb_ch_init(ch, &tEtcbConfig);	
			
	tEtcbConfig.eChType  = ETCB_ONE_TRG_ONE;  	                         //单个源触发单个目标
	tEtcbConfig.eSrcIp   = ETCB_EXI_TRGOUT5 ;  	                         //...作为触发源
	tEtcbConfig.eDstIp   = ETCB_GPTA0_SYNCIN3;                           //GPTA0 同步输入3作为目标事件
	tEtcbConfig.eTrgMode = ETCB_HARDWARE_TRG;
	ch = csi_etcb_ch_alloc(tEtcbConfig.eChType);	                     //自动获取空闲通道号,ch >= 0 获取成功						//ch < 0,则获取通道号失败		
	iRet = csi_etcb_ch_init(ch, &tEtcbConfig);		
		
	csi_gpta_captureconfig_t tPwmCfg;								  
	tPwmCfg.byWorkmod         = GPTA_CAPTURE;                            //WAVE or CAPTURE    //计数或捕获	
	tPwmCfg.byCountingMode    = GPTA_UPCNT;                              //CNYMD  //计数方向
	tPwmCfg.byOneshotMode     = GPTA_OP_CONT; 
	tPwmCfg.byStartSrc        = GPTA_SYNC;				                 //软件使能同步触发使能控制（RSSR中START控制位）//启动方式
	tPwmCfg.byPscld           = GPTA_LDPSCR_ZRO;                         //PSCR(分频)活动寄存器载入控制。活动寄存器在配置条件满足时，从影子寄存器载入更新值	
	tPwmCfg.byCaptureCapmd    = 0;                                       //0:连续捕捉模式    1h：一次性捕捉模式
	tPwmCfg.byCaptureStopWrap = 2-1;                                     //Capture模式下，捕获事件计数器周期设置值
	tPwmCfg.byCaptureLdaret   = 0;                                       //CMPA捕捉载入后，计数器值计数状态控制位(1h：CMPA触发后，计数器值进行重置;0h：CMPA触发后，计数器值不进行重置)
	tPwmCfg.byCaptureLdbret   = 1; 
	tPwmCfg.byCaptureLdaaret  = 0;  
	tPwmCfg.byCaptureLdbaret  = 0;  
	tPwmCfg.byCaptureCapmdSel = GPTA_CMPMD_SEL_DIFF;
	csi_gpta_capture_init(GPTA0, &tPwmCfg);
	
	csi_gpta_int_enable(GPTA0, GPTA_INTSRC_CAPLD1);	

    csi_gpta_set_sync(GPTA0, GPTA_TRG_SYNCEN2, GPTA_TRG_CONT, GPTA_AUTO_REARM_ZRO);//使能SYNCIN2外部触发
	csi_gpta_set_sync(GPTA0, GPTA_TRG_SYNCEN3, GPTA_TRG_CONT, GPTA_AUTO_REARM_ZRO);//使能SYNCIN3外部触发
	
	csi_gpta_start(GPTA0);//start  timer
	
    while(1)
	{		
		mdelay(200);                        
		mdelay(200);
	}			
	return iRet;
};

/** \brief GPTA基本的波形输出示例代码
 *          PWM在50%和20%之间切换
 *  \param[in] none
 *  \return error code
 */
int gpta_pwm_demo(void)
{
	int iRet = 0;	
	
#if (USE_GUI == 0)	
	csi_gpio_set_mux(GPIOA, PA0, PA0_GPTA0_CHA);				
	csi_gpio_set_mux(GPIOA, PA1, PA1_GPTA0_CHB);						
#endif
	
	csi_gpta_pwmconfig_t tPwmCfg;								  
	tPwmCfg.byWorkmod        = GPTA_WAVE;                        //WAVE  波形模式
	tPwmCfg.byCountingMode   = GPTA_UPCNT;                       //CNYMD  //计数方向
	tPwmCfg.byOneshotMode    = GPTA_OP_CONT;                     //OPM    //单次或连续(工作方式)
	tPwmCfg.byStartSrc       = GPTA_SYNC;					     //软件使能同步触发使能控制（RSSR中START控制位）//启动方式
	tPwmCfg.byPscld          = GPTA_LDPSCR_ZRO;                  //PSCR(分频)活动寄存器载入控制。活动寄存器在配置条件满足时，从影子寄存器载入更新值		
	tPwmCfg.byDutyCycle 	 = 50;								 //pwm ouput duty cycle//PWM初始值(X%)			
	tPwmCfg.wFreq 			 = 1000;							 //pwm ouput frequency	
	csi_gpta_wave_init(GPTA0, &tPwmCfg);
	
	csi_gpta_int_enable(GPTA0, GPTA_INTSRC_CBU);

	csi_gpta_pwmchannel_config_t  tGptachannelCfg;
	tGptachannelCfg.byActionZro    =   GPTA_LO;
	tGptachannelCfg.byActionPrd    =   GPTA_NA;
	tGptachannelCfg.byActionC1u    =   GPTA_HI;
	tGptachannelCfg.byActionC1d    =   GPTA_LO;
	tGptachannelCfg.byActionC2u    =   GPTA_HI;
	tGptachannelCfg.byActionC2d    =   GPTA_LO;
	tGptachannelCfg.byActionT1u    =   GPTA_LO;
	tGptachannelCfg.byActionT1d    =   GPTA_LO;
	tGptachannelCfg.byActionT2u    =   GPTA_NA;
	tGptachannelCfg.byActionT2d    =   GPTA_NA;
	tGptachannelCfg.byChoiceC1sel  =   GPTA_CMPA;
	tGptachannelCfg.byChoiceC2sel  =   GPTA_CMPA;	
	csi_gpta_set_channel(GPTA0, &tGptachannelCfg,  GPTA_CHANNEL_1);//channel
	tGptachannelCfg.byChoiceC1sel  =   GPTA_CMPB;
	tGptachannelCfg.byChoiceC2sel  =   GPTA_CMPB;
	csi_gpta_set_channel(GPTA0, &tGptachannelCfg,  GPTA_CHANNEL_2);

	csi_gpta_start(GPTA0);

	while(1)
	{	
		csi_gpta_change_ch_duty(GPTA0,GPTA_COMPA, 20);
		csi_gpta_change_ch_duty(GPTA0,GPTA_COMPB, 20);
		mdelay(200);                        
		csi_gpta_change_ch_duty(GPTA0,GPTA_COMPA, 50);
		csi_gpta_change_ch_duty(GPTA0,GPTA_COMPB, 50);
		mdelay(200);	
	}	

    return iRet;
}

/** \brief GPTA波形强制输出demo
 *          包含一次性强制性输出和连续强制输出
 *  \param[in] none
 *  \return error code
 */
int gpta_pwm_waveform_demo(void)
{
	int iRet = 0;	
	
#if (USE_GUI == 0)	
	csi_gpio_set_mux(GPIOA, PA0, PA0_GPTA0_CHA);			
	csi_gpio_set_mux(GPIOA, PA1, PA1_GPTA0_CHB);				
#endif	
	
	csi_gpta_pwmconfig_t tPwmCfg;								  
	tPwmCfg.byWorkmod        = GPTA_WAVE;                    //WAVE  波形模式
	tPwmCfg.byCountingMode   = GPTA_UPDOWNCNT;                 //CNYMD  //计数方向
	tPwmCfg.byOneshotMode    = GPTA_OP_CONT;                 //OPM    //单次或连续(工作方式)
	tPwmCfg.byStartSrc       = GPTA_SYNC;					 //软件使能同步触发使能控制（RSSR中START控制位）//启动方式
	tPwmCfg.byPscld          = GPTA_LDPSCR_ZRO;              //PSCR(分频)活动寄存器载入控制。活动寄存器在配置条件满足时，从影子寄存器载入更新值		
	tPwmCfg.byDutyCycle 	 = 50;						     //pwm ouput duty cycle//PWM初始值(X%)			
	tPwmCfg.wFreq 			 = 1000;					     //pwm ouput frequency	
	csi_gpta_wave_init(GPTA0, &tPwmCfg);
	
	csi_gpta_int_enable(GPTA0, GPTA_INTSRC_CBU);
	
	csi_gpta_pwmchannel_config_t  tGptachannelCfg;
	tGptachannelCfg.byActionZro    =   GPTA_LO;
	tGptachannelCfg.byActionPrd    =   GPTA_NA;
	tGptachannelCfg.byActionC1u    =   GPTA_HI;
	tGptachannelCfg.byActionC1d    =   GPTA_LO;
	tGptachannelCfg.byActionC2u    =   GPTA_HI;
	tGptachannelCfg.byActionC2d    =   GPTA_LO;
	tGptachannelCfg.byActionT1u    =   GPTA_LO;
	tGptachannelCfg.byActionT1d    =   GPTA_LO;
	tGptachannelCfg.byActionT2u    =   GPTA_NA;
	tGptachannelCfg.byActionT2d    =   GPTA_NA;
	tGptachannelCfg.byChoiceC1sel  =   GPTA_CMPA;
	tGptachannelCfg.byChoiceC2sel  =   GPTA_CMPA;	
	csi_gpta_set_channel(GPTA0, &tGptachannelCfg,  GPTA_CHANNEL_1);//channel
	
	tGptachannelCfg.byChoiceC1sel  =   GPTA_CMPB;
	tGptachannelCfg.byChoiceC2sel  =   GPTA_CMPB;
	csi_gpta_set_channel(GPTA0, &tGptachannelCfg,  GPTA_CHANNEL_2);

	csi_gpta_start(GPTA0);

	 while(1)
	{		
		csi_gpta_change_ch_duty(GPTA0,GPTA_COMPA, 20);
		csi_gpta_change_ch_duty(GPTA0,GPTA_COMPB, 20);
		mdelay(10);

		csi_gpta_onetimesoftware_output(GPTA0,GPTA_OSTSF1_MSK,GPTA_HI);
		mdelay(2);
		
		csi_gpta_change_ch_duty(GPTA0,GPTA_COMPA, 50);
		csi_gpta_change_ch_duty(GPTA0,GPTA_COMPB, 50);
		mdelay(10);
		
		csi_gpta_set_aqcsfload(GPTA0, GPTA_AQCSF_NOW);
		csi_gpta_continuous_software_waveform(GPTA0, GPTA_CHANNEL_1, GPTA_AQCSF_L);
		mdelay(10);
		csi_gpta_continuous_software_waveform(GPTA0, GPTA_CHANNEL_1, GPTA_AQCSF_NONE);
	}			
    return iRet;
}

/** \brief GPTA链接代码实例
 *         通过GPTA1链接GPTA0,实现波形的输出
 *         0x0 不链接, 0x1 GPTA0 ,0x2 GPTA1 0x3 GPTA2 ,0x4 GPTA3 
 *         0x5 GPTB0 ,0x6 GPTB1,0x7 GPTB2 ,0x8 GPTB3 0x9 GPTB4 ,0xA GPTB5
 *  \param[in] none
 *  \return error code
 */
int gpta_reglk_demo(void)
{
	int iRet = 0;	
	
#if (USE_GUI == 0)		
	csi_gpio_set_mux(GPIOA, PA0, PA0_GPTA0_CHA);		
	csi_gpio_set_mux(GPIOA, PA1, PA1_GPTA0_CHB);		
#endif	
	
    csi_gpta_channel_set_cmpload(GPTA0, GPTA_SHDW_SHADOW, GPTA_LDMD_ZRO ,GPTA_COMPA);
	csi_gpta_channel_set_cmpload(GPTA0, GPTA_SHDW_SHADOW, GPTA_LDMD_ZRO ,GPTA_COMPB);
	csi_gpta_pwmconfig_t tPwmCfg;								  
	tPwmCfg.byWorkmod        = GPTA_WAVE;                    //WAVE  波形模式
	tPwmCfg.byCountingMode   = GPTA_UPCNT;                   //CNYMD  //计数方向
	tPwmCfg.byOneshotMode    = GPTA_OP_CONT;                 //OPM    //单次或连续(工作方式)
	tPwmCfg.byStartSrc       = GPTA_SYNC;					 //软件使能同步触发使能控制（RSSR中START控制位）//启动方式
	tPwmCfg.byPscld          = GPTA_LDPSCR_ZRO;              //PSCR(分频)活动寄存器载入控制。活动寄存器在配置条件满足时，从影子寄存器载入更新值		
	tPwmCfg.byDutyCycle 	 = 50;						     //pwm ouput duty cycle//PWM初始值(X%)			
	tPwmCfg.wFreq 			 = 1000;					     //pwm ouput frequency	
	csi_gpta_wave_init(GPTA0, &tPwmCfg);
	
	csi_gpta_int_enable(GPTA0, GPTA_INTSRC_CBU);

	csi_gpta_pwmchannel_config_t  tGptachannelCfg;
	tGptachannelCfg.byActionZro    =   GPTA_LO;
	tGptachannelCfg.byActionPrd    =   GPTA_NA;
	tGptachannelCfg.byActionC1u    =   GPTA_HI;
	tGptachannelCfg.byActionC1d    =   GPTA_LO;
	tGptachannelCfg.byActionC2u    =   GPTA_HI;
	tGptachannelCfg.byActionC2d    =   GPTA_LO;
	tGptachannelCfg.byActionT1u    =   GPTA_LO;
	tGptachannelCfg.byActionT1d    =   GPTA_LO;
	tGptachannelCfg.byActionT2u    =   GPTA_NA;
	tGptachannelCfg.byActionT2d    =   GPTA_NA;
	tGptachannelCfg.byChoiceC1sel  =   GPTA_CMPA;
	tGptachannelCfg.byChoiceC2sel  =   GPTA_CMPA;	
	csi_gpta_set_channel(GPTA0, &tGptachannelCfg,  GPTA_CHANNEL_1);//channel
	
	tGptachannelCfg.byChoiceC1sel  =   GPTA_CMPB;
	tGptachannelCfg.byChoiceC2sel  =   GPTA_CMPB;
	csi_gpta_set_channel(GPTA0, &tGptachannelCfg,  GPTA_CHANNEL_2);
		
	// 0x0 不链接, 0x1 GPTA0 ,0x2 GPTA1 0x3 GPTA2 ,0x4 GPTA3 
    csi_gpta_feglk_config_t  FEGLKcfg;                                                                    
	FEGLKcfg.byPrdr	   = 2;                                                             
	FEGLKcfg.byRssr    = 2;																                   
	FEGLKcfg.byCmpa    = 2;																					
	FEGLKcfg.byCmpb    = 2;																					
	FEGLKcfg.byGld2    = 0;																					
	FEGLKcfg.byEmslclr = 2;																					
	FEGLKcfg.byEmhlclr = 2;																					
	FEGLKcfg.byEmicr   = 2;																					 
	FEGLKcfg.byEmfrcr  = 2;																					 
	FEGLKcfg.byAqosf   = 2;																					
	FEGLKcfg.byAqcsf   = 2;  																				 
    csi_gpta_reglk_config(GPTA0,&FEGLKcfg);

	csi_gpio_set_mux(GPIOA, PA2, PA2_GPTA1_CHA);						//
	csi_gpio_set_mux(GPIOA, PA3, PA3_GPTA1_CHB);						//
							  
	tPwmCfg.byWorkmod        = GPTA_WAVE;                     //WAVE  波形模式
	tPwmCfg.byCountingMode   = GPTA_UPCNT;                    //CNYMD  //计数方向
	tPwmCfg.byOneshotMode    = GPTA_OP_CONT;                  //OPM    //单次或连续(工作方式)
	tPwmCfg.byStartSrc       = GPTA_SYNC;					  //软件使能同步触发使能控制（RSSR中START控制位）//启动方式
	tPwmCfg.byPscld          = GPTA_LDPSCR_ZRO;               //PSCR(分频)活动寄存器载入控制。活动寄存器在配置条件满足时，从影子寄存器载入更新值		
	tPwmCfg.byDutyCycle 	 = 50;						      //pwm ouput duty cycle//PWM初始值(X%)			
	tPwmCfg.wFreq 			 = 1000;					      //pwm ouput frequency	
	csi_gpta_wave_init(GPTA1, &tPwmCfg);
	
	csi_gpta_int_enable(GPTA1, GPTA_INTSRC_CBU);

	tGptachannelCfg.byActionZro    =   GPTA_LO;
	tGptachannelCfg.byActionPrd    =   GPTA_NA;
	tGptachannelCfg.byActionC1u    =   GPTA_HI;
	tGptachannelCfg.byActionC1d    =   GPTA_LO;
	tGptachannelCfg.byActionC2u    =   GPTA_HI;
	tGptachannelCfg.byActionC2d    =   GPTA_LO;
	tGptachannelCfg.byActionT1u    =   GPTA_LO;
	tGptachannelCfg.byActionT1d    =   GPTA_LO;
	tGptachannelCfg.byActionT2u    =   GPTA_NA;
	tGptachannelCfg.byActionT2d    =   GPTA_NA;
	tGptachannelCfg.byChoiceC1sel  =   GPTA_CMPA;
	tGptachannelCfg.byChoiceC2sel  =   GPTA_CMPA;	
	csi_gpta_set_channel(GPTA1, &tGptachannelCfg,  GPTA_CHANNEL_1);//channel
	
	tGptachannelCfg.byChoiceC1sel  =   GPTA_CMPB;
	tGptachannelCfg.byChoiceC2sel  =   GPTA_CMPB;
	csi_gpta_set_channel(GPTA1, &tGptachannelCfg,  GPTA_CHANNEL_2);

	csi_gpta_start(GPTA1);	
	while(1)
	{	
		csi_gpta_change_ch_duty(GPTA1,GPTA_COMPA, 20);
		csi_gpta_change_ch_duty(GPTA1,GPTA_COMPB, 20);
		mdelay(200);                        
		csi_gpta_change_ch_duty(GPTA1,GPTA_COMPA, 50);
		csi_gpta_change_ch_duty(GPTA1,GPTA_COMPB, 50);
		mdelay(200);	
	}	
	return iRet;	
}

