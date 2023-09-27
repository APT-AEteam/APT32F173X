/***********************************************************************//** 
 * \file  gpta_demo.c
 * \brief  TIMER_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-11 <td>V0.0 <td>ljy     <td>initial
 * <tr><td> 2023-3-21 <td>V1.0 <td>YYM     <td>initial
 * <tr><td> 2023-9-07 <td>V2.0 <td>LHY     <td>code normalization
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
		csi_gpio_toggle(GPIOA, PA6);
	    csp_gpta_clr_isr(GPTA0, GPTA_INT_PEND);
	}
	
	if((wMisr & GPTA_INT_CAPLD1) == GPTA_INT_CAPLD1)
	{		
     	s_wGptaCapBuff[0]=csp_gpta_get_cmpa(GPTA0);
		s_wGptaCapBuff[1]=csp_gpta_get_cmpb(GPTA0);
		csp_gpta_clr_isr(GPTA0, GPTA_INT_CAPLD1);			
	}
	
    if((wMisr & GPTA_INT_CBU) == GPTA_INT_CBU)
	{	
	    csp_gpta_clr_isr(GPTA0, GPTA_INT_CBU);
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
	csi_gpta_time_config_t tTimConfig;
	
#if (USE_GUI == 0)		
	csi_gpio_set_mux(GPIOA, PA6, PA6_OUTPUT);	//初始化PA6为输出
#endif
	
	tTimConfig.wTimeVal  = 10000;				//GPTA定时值 = 1000us
	tTimConfig.eRunMode  = GPTA_RUN_CONT;		//GPTA计数器工作模式
	csi_gpta_timer_init(GPTA0, &tTimConfig);    //初始化GPTA0, 定时10000us； GPTA定时，默认采用向上计数，PEND中断
	
	csi_gpta_start(GPTA0);                      //启动定时器

	return iRet;	
}

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

int gpta_capture_demo(void)
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

/** \brief GPTA基本的波形输出示例代码
 *          PWM在50%和20%之间切换
 *  \param[in] none
 *  \return error code
 */
int gpta_pwm_demo(void)
{
	int iRet = 0;
	csi_gpta_pwm_config_t tPwmConfig;
	csi_gpta_pwm_channel_config_t  tGptachannelConfig;
//------------------------------------------------------------------------------------------------------------------------	
#if (USE_GUI == 0)	
	csi_gpio_set_mux(GPIOA, PA0, PA0_GPTA0_CHA);										
#endif
//------------------------------------------------------------------------------------------------------------------------		
	tPwmConfig.eWorkMode       	= GPTA_WORK_WAVE;                  //GPTA工作模式：捕获/波形输出
	tPwmConfig.eCountMode   	= GPTA_CNT_UPDOWN;                 //GPTA计数模式：递增/递减/递增递减
	tPwmConfig.eRunMode    		= GPTA_RUN_CONT;                   //GPTA运行模式：连续/一次性
	tPwmConfig.byDutyCycle 		= 50;							   //GPTA输出PWM占空比			
	tPwmConfig.wFreq 			= 10000;						   //GPTA输出PWM频率	
	csi_gpta_pwm_init(GPTA0, &tPwmConfig);
//------------------------------------------------------------------------------------------------------------------------	
	tGptachannelConfig.eActionZro    =   GPTA_ACT_LO;
	tGptachannelConfig.eActionPrd    =   GPTA_ACT_NA;
	tGptachannelConfig.eActionC1u    =   GPTA_ACT_HI;
	tGptachannelConfig.eActionC1d    =   GPTA_ACT_LO;
	tGptachannelConfig.eActionC2u    =   GPTA_ACT_NA;
	tGptachannelConfig.eActionC2d    =   GPTA_ACT_NA;
	tGptachannelConfig.eActionT1u    =   GPTA_ACT_NA;
	tGptachannelConfig.eActionT1d    =   GPTA_ACT_NA;
	tGptachannelConfig.eActionT2u    =   GPTA_ACT_NA;
	tGptachannelConfig.eActionT2d    =   GPTA_ACT_NA;
	tGptachannelConfig.eC1sel        =   GPTA_COMPA;
	tGptachannelConfig.eC2sel        =   GPTA_COMPA;	
	csi_gpta_set_channel(GPTA0, &tGptachannelConfig,  GPTA_CHANNEL_1);
//------------------------------------------------------------------------------------------------------------------------
	csi_gpta_start(GPTA0);

	while(1)
	{	
		csi_gpta_update_ch_duty(GPTA0,GPTA_COMPA, 20);
		mdelay(200);                        
		csi_gpta_update_ch_duty(GPTA0,GPTA_COMPA, 50);
		mdelay(200);	
	}	

    return iRet;
}

/** \brief GPTA波形强制输出demo
 *         包含一次性强制性输出和连续强制输出
 *  \param[in] none
 *  \return error code
 */
int gpta_pwm_waveform_demo(void)
{
	int iRet = 0;	
	csi_gpta_pwm_config_t tPwmConfig;
	csi_gpta_pwm_channel_config_t  tGptachannelConfig;
//------------------------------------------------------------------------------------------------------------------------	
#if (USE_GUI == 0)	
	csi_gpio_set_mux(GPIOA, PA0, PA0_GPTA0_CHA);						
#endif	
//------------------------------------------------------------------------------------------------------------------------	
	tPwmConfig.eWorkMode       	= GPTA_WORK_WAVE;                  //GPTA工作模式：捕获/波形输出
	tPwmConfig.eCountMode   	= GPTA_CNT_UPDOWN;                 //GPTA计数模式：递增/递减/递增递减
	tPwmConfig.eRunMode    		= GPTA_RUN_CONT;                   //GPTA运行模式：连续/一次性
	tPwmConfig.byDutyCycle 		= 50;							   //GPTA输出PWM占空比			
	tPwmConfig.wFreq 			= 1000;						       //GPTA输出PWM频率	
	csi_gpta_pwm_init(GPTA0, &tPwmConfig);
//------------------------------------------------------------------------------------------------------------------------	
	tGptachannelConfig.eActionZro    =   GPTA_ACT_LO;
	tGptachannelConfig.eActionPrd    =   GPTA_ACT_NA;
	tGptachannelConfig.eActionC1u    =   GPTA_ACT_HI;
	tGptachannelConfig.eActionC1d    =   GPTA_ACT_LO;
	tGptachannelConfig.eActionC2u    =   GPTA_ACT_NA;
	tGptachannelConfig.eActionC2d    =   GPTA_ACT_NA;
	tGptachannelConfig.eActionT1u    =   GPTA_ACT_NA;
	tGptachannelConfig.eActionT1d    =   GPTA_ACT_NA;
	tGptachannelConfig.eActionT2u    =   GPTA_ACT_NA;
	tGptachannelConfig.eActionT2d    =   GPTA_ACT_NA;
	tGptachannelConfig.eC1sel        =   GPTA_COMPA;
	tGptachannelConfig.eC2sel        =   GPTA_COMPA;	
	csi_gpta_set_channel(GPTA0, &tGptachannelConfig,  GPTA_CHANNEL_1);
//------------------------------------------------------------------------------------------------------------------------	
	csi_gpta_start(GPTA0);

	 while(1)
	{		
		csi_gpta_update_ch_duty(GPTA0,GPTA_COMPA, 20);
		mdelay(10);

		csi_gpta_set_aqosf(GPTA0,GPTA_CHANNEL_1,GPTA_ACT_HI);
		mdelay(2);
		
		csi_gpta_update_ch_duty(GPTA0,GPTA_COMPA, 50);
		mdelay(10);
		
		csi_gpta_set_aqcsf_loadmode(GPTA0, GPTA_AQCSF_LOAD_NOW);
		csi_gpta_set_aqcsf(GPTA0, GPTA_CHANNEL_1, GPTA_AQCSF_L);
		mdelay(10);
		csi_gpta_set_aqcsf(GPTA0, GPTA_CHANNEL_1, GPTA_AQCSF_NONE);
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
	csi_gpta_pwm_config_t tPwmConfig;
	csi_gpta_pwm_channel_config_t  tGptachannelConfig;
	csi_gpta_reglk_config_t  tReglkConfig;                     // 0x0 不链接, 0x1 GPTA0 ,0x2 GPTA1 0x3 GPTA2 ,0x4 GPTA3       
//------------------------------------------------------------------------------------------------------------------------	
#if (USE_GUI == 0)		
	csi_gpio_set_mux(GPIOA, PA0, PA0_GPTA0_CHA);	           //GPTA0 CHA输出
	csi_gpio_set_mux(GPIOA, PA2, PA2_GPTA1_CHA);	           //GPTA1 CHA输出		
#endif	
//------------------------------------------------------------------------------------------------------------------------	
    csi_gpta_set_cmpload(GPTA0, GPTA_SHDW_SHADOW, GPTA_LDMD_ZRO ,GPTA_COMPA);
//------------------------------------------------------------------------------------------------------------------------	
	tPwmConfig.eWorkMode       	= GPTA_WORK_WAVE;              //GPTA工作模式：捕获/波形输出
	tPwmConfig.eCountMode   	= GPTA_CNT_UP;                 //GPTA计数模式：递增/递减/递增递减
	tPwmConfig.eRunMode    		= GPTA_RUN_CONT;               //GPTA运行模式：连续/一次性
	tPwmConfig.byDutyCycle 		= 50;						   //GPTA输出PWM占空比			
	tPwmConfig.wFreq 			= 1000;						   //GPTA输出PWM频率	
	csi_gpta_pwm_init(GPTA0, &tPwmConfig);

	tGptachannelConfig.eActionZro    =   GPTA_ACT_LO;
	tGptachannelConfig.eActionPrd    =   GPTA_ACT_NA;
	tGptachannelConfig.eActionC1u    =   GPTA_ACT_HI;
	tGptachannelConfig.eActionC1d    =   GPTA_ACT_LO;
	tGptachannelConfig.eActionC2u    =   GPTA_ACT_NA;
	tGptachannelConfig.eActionC2d    =   GPTA_ACT_NA;
	tGptachannelConfig.eActionT1u    =   GPTA_ACT_NA;
	tGptachannelConfig.eActionT1d    =   GPTA_ACT_NA;
	tGptachannelConfig.eActionT2u    =   GPTA_ACT_NA;
	tGptachannelConfig.eActionT2d    =   GPTA_ACT_NA;
	tGptachannelConfig.eC1sel        =   GPTA_COMPA;
	tGptachannelConfig.eC2sel        =   GPTA_COMPA;	
	csi_gpta_set_channel(GPTA0, &tGptachannelConfig,  GPTA_CHANNEL_1);
//------------------------------------------------------------------------------------------------------------------------		
	tPwmConfig.eWorkMode       	= GPTA_WORK_WAVE;              //GPTA工作模式：捕获/波形输出
	tPwmConfig.eCountMode   	= GPTA_CNT_UP;                 //GPTA计数模式：递增/递减/递增递减
	tPwmConfig.eRunMode    		= GPTA_RUN_CONT;               //GPTA运行模式：连续/一次性
	tPwmConfig.byDutyCycle 		= 50;						   //GPTA输出PWM占空比			
	tPwmConfig.wFreq 			= 1000;						   //GPTA输出PWM频率
	csi_gpta_pwm_init(GPTA1, &tPwmConfig);
	
	tGptachannelConfig.eActionZro    =   GPTA_ACT_LO;
	tGptachannelConfig.eActionPrd    =   GPTA_ACT_NA;
	tGptachannelConfig.eActionC1u    =   GPTA_ACT_HI;
	tGptachannelConfig.eActionC1d    =   GPTA_ACT_LO;
	tGptachannelConfig.eActionC2u    =   GPTA_ACT_NA;
	tGptachannelConfig.eActionC2d    =   GPTA_ACT_NA;
	tGptachannelConfig.eActionT1u    =   GPTA_ACT_NA;
	tGptachannelConfig.eActionT1d    =   GPTA_ACT_NA;
	tGptachannelConfig.eActionT2u    =   GPTA_ACT_NA;
	tGptachannelConfig.eActionT2d    =   GPTA_ACT_NA;
	tGptachannelConfig.eC1sel        =   GPTA_COMPA;
	tGptachannelConfig.eC2sel        =   GPTA_COMPA;
	csi_gpta_set_channel(GPTA1, &tGptachannelConfig,  GPTA_CHANNEL_1);//channel
//------------------------------------------------------------------------------------------------------------------------	
	tReglkConfig.byPrdr	  = 2;                                                             
	tReglkConfig.byRssr    = 2;																                   
	tReglkConfig.byCmpa    = 2;																					
	tReglkConfig.byCmpb    = 2;																					
	tReglkConfig.byGld2    = 0;																					
	tReglkConfig.byEmslclr = 2;																					
	tReglkConfig.byEmhlclr = 2;																					
	tReglkConfig.byEmicr   = 2;																					 
	tReglkConfig.byEmfrcr  = 2;																					 
	tReglkConfig.byAqosf   = 2;																					
	tReglkConfig.byAqcsf   = 2;  																				 
    csi_gpta_set_reglk(GPTA0, &tReglkConfig);
//------------------------------------------------------------------------------------------------------------------------	
	csi_gpta_start(GPTA1);	
	while(1)
	{	
		csi_gpta_update_ch_duty(GPTA1,GPTA_COMPA, 20);
		mdelay(200);                        
		csi_gpta_update_ch_duty(GPTA1,GPTA_COMPA, 50);
		mdelay(200);	
	}	
	return iRet;	
}

