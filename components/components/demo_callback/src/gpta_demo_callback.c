/***********************************************************************//** 
 * \file  gpta_demo.c
 * \brief  TIMER_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-9-24 <td>V0.0 <td>LHY     <td>code normalization
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include "csi_drv.h"
#include "board_config.h"

#if (USE_GPTA_CALLBACK == 1)
//#include "demo.h"
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/
static uint32_t s_wGptaCapCallbackBuff[4] = {0};

/** \brief  gpta0_timer_callback：gpta0中断回调函数
 * 
 *  \brief  用户定义，使用csi标准库，中断发生时会自动调用用户注册的回调函
 *          数，用户可在回调函数里做自己的处理，而不需要关注具体的底层中断处理。
 * 
 *  \param[out] ptGptaBase: GPTA寄存器结构体指针，指向GPTA的基地址 
 *  \return none
 */ 
void gpta0_timer_callback(csp_gpta_t *ptGptaBase, uint32_t wIsr)
{
	if((wIsr & GPTA_INT_PEND) == GPTA_INT_PEND)
	{	
		csi_gpio_toggle(GPIOA, PA6);         //IO翻转
	}
}

/** \brief  user_capture_callback：GPTA捕获中断回调函数
 * 
 * 	\brief	用户定义，支持指CAPLD0/1/2/3四种中断处理，使用csi标准库，中断发生时会自动调用用户注册的回调函
 * 			数，用户可在回调函数里做自己的处理，而不需要关注具体的底层中断处理。
 * 
 *  \param[in]  ptGptaBase:	GPTAx寄存器结构体指针，指向GPTBx的基地址 
 *  \param[out] wIsr: 		GPTAx中断状态
 *  \return none
 */ 
void user_capture_callback(csp_gpta_t *ptGptaBase,  uint32_t wIsr)
{
	if(wIsr & GPTA_INTSRC_CAPLD1)
	{
		s_wGptaCapCallbackBuff[0] = csp_gpta_get_cmpa(GPTA0);
		s_wGptaCapCallbackBuff[1] = csp_gpta_get_cmpb(GPTA0);
	}
}


/** \brief  user_pwm_callback：GPTA PWM pend中断回调函数
 * 
 * 	\brief	用户定义，支持多种中断处理, \ref csi_gpta_int_e。使用csi标准库，中断发生时会自动调用用户注册的回调函
 * 			数，用户可在回调函数里做自己的处理，而不需要关注具体的底层中断处理。
 * 
 *  \param[in]  ptGptaBase:	GPTAx寄存器结构体指针，指向GPTBx的基地址 
 *  \param[out] wIsr: 		GPTAx中断状态
 *  \return none
 */ 
void user_pwm_callback(csp_gpta_t *ptGptaBase,  uint32_t wIsr)
{
	if((wIsr & GPTA_INT_PEND) == GPTA_INT_PEND)
	{
		//usercode
	}
}


/** \brief gpta timer
 * 
 *  \param[in] none
 *  \return error code
 */
int gpta_timer_int_callback_demo(void)
{
	int iRet = 0;
	csi_gpta_time_config_t tTimConfig= {0};
	
#if (USE_GUI == 0)		
	csi_gpio_set_mux(GPIOA, PA6, PA6_OUTPUT);	             //初始化PA6为输出,在中断中翻转电平
#endif
	
	tTimConfig.wTimeVal  = 10000;				             //GPTA定时值 = 1000us
	tTimConfig.eRunMode  = GPTA_RUN_CONT;		             //GPTA计数器工作模式
	csi_gpta_timer_init(GPTA0, &tTimConfig);                 //初始化GPTA0
	
	csp_gpta_int_enable(GPTA0, GPTA_INT_PEND);	             //使能GPTA0 PEND中断
	csi_gpta_register_callback(GPTA0, gpta0_timer_callback); //注册中断回调函数
	
	csi_gpta_start(GPTA0);                                   //启动GPTA0

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

int gpta_capture_int_callback_demo(void)
{
	int iRet = 0;	
    volatile uint8_t ch;
	csi_etcb_config_t tEtcbConfig= {0};				                         //ETCB 参数配置结构体
	csi_gpta_capture_config_t tCapConfig= {0};	                             //GPTA 捕获参数配置结构体
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
	csi_gpta_register_callback(GPTA0, user_capture_callback);	                 //注册中断回调函数
	
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
}

/** \brief GPTA基本的波形输出示例代码
 *          PWM在50%和20%之间切换
 *  \param[in] none
 *  \return error code
 */
int gpta_pwm_int_callback_demo(void)
{
	int iRet = 0;
	csi_gpta_pwm_config_t tPwmConfig= {0};
	csi_gpta_pwm_channel_config_t  tGptachannelConfig= {0};
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
	tGptachannelConfig.eActionZro    =   GPTA_ACT_LO;             //CNT=ZRO时，波形输出低电平
	tGptachannelConfig.eActionPrd    =   GPTA_ACT_NA;             //CNT=PRD时，波形输出不变
	tGptachannelConfig.eActionC1u    =   GPTA_ACT_HI;             //CNT=PRD时，波形输出高电平
	tGptachannelConfig.eActionC1d    =   GPTA_ACT_LO;             //CNT=PRD时，波形输出低电平
	tGptachannelConfig.eActionC2u    =   GPTA_ACT_NA;             //CNT=PRD时，波形输出不变
	tGptachannelConfig.eActionC2d    =   GPTA_ACT_NA;             //CNT=PRD时，波形输出不变
	tGptachannelConfig.eActionT1u    =   GPTA_ACT_NA;             //CNT=PRD时，波形输出不变
	tGptachannelConfig.eActionT1d    =   GPTA_ACT_NA;             //CNT=PRD时，波形输出不变
	tGptachannelConfig.eActionT2u    =   GPTA_ACT_NA;             //CNT=PRD时，波形输出不变
	tGptachannelConfig.eActionT2d    =   GPTA_ACT_NA;             //CNT=PRD时，波形输出不变
	tGptachannelConfig.eC1sel        =   GPTA_COMPA;              //C1选择CMPA为数据源
	tGptachannelConfig.eC2sel        =   GPTA_COMPA;	          //C2选择CMPA为数据源
	csi_gpta_set_channel(GPTA0, &tGptachannelConfig,  GPTA_CHANNEL_1);
//------------------------------------------------------------------------------------------------------------------------
	csi_gpta_int_enable(GPTA0, GPTA_INTSRC_PEND);
	csi_gpta_register_callback(GPTA0,user_pwm_callback);
	
	csi_gpta_start(GPTA0);

	while(1)
	{	
		csi_gpta_update_ch_duty(GPTA0,GPTA_COMPA, 20);            //修改占空比20%
		mdelay(200);                        
		csi_gpta_update_ch_duty(GPTA0,GPTA_COMPA, 50);            //修改占空比50%
		mdelay(200);	
	}	

    return iRet;
}



#endif