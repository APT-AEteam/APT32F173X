/***********************************************************************//** 
 * \file  lpt_demo.c
 * \brief  LPT_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-27 <td>V0.0 <td>YYM     <td>initial
 * <tr><td> 2023-3-15 <td>V0.1  <td>YT     <td>modify
 * <tr><td> 2023-9-14 <td>V0.1  <td>YT     <td>modify
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

#if (USE_LPT_CALLBACK == 0)	

/** \brief  lpt_int_handler: LPT中断服务函数
 * 
 *  \brief  lpt发生中断时会调用此函数，函数在interrupt.c里定义为弱(weak)属性，默认不做处理;用户用到中断
 * 			时，请重新定义此函数，在此函数中进行对应中断处理，也可直接在interrupt.c里的函数里进行处理。
 * 
 *  \param[in] none
 *  \return none
 */
ATTRIBUTE_ISR void lpt_int_handler(void)
{
	//用户直接在中断服务接口函数里处理中断，建议客户使用此模式
	volatile uint32_t wMisr = csp_lpt_get_isr(LPT);
	
	if(wMisr & LPT_INT_TRGEV)					//TRGEV interrupt
	{
		csp_lpt_clr_isr(LPT, LPT_INT_TRGEV);
	}
	if(wMisr & LPT_INT_MATCH)					//MATCH interrupt
	{
		csp_lpt_clr_isr(LPT, LPT_INT_MATCH);	
	}
	if(wMisr & LPT_INT_PEND)				//PEND interrupt
	{
		csp_lpt_clr_isr(LPT, LPT_INT_PEND);
	}
}

#endif

/** \brief lpt timer
 * 	\brief csi初始化里不开启中断，需要进中断函数，需调用csi_lpt_int_enable接口，demo默认开启pend中断
 *  \param[in] none
 *  \return error code
 */
int lpt_timer_demo(void)
{
	int iRet = 0;

	csi_lpt_time_config_t tTimConfig;
	tTimConfig.wTimeVal = 200;					//LPT定时值 = 200us
	tTimConfig.eRunMode  = LPT_CNT_CONT;		//LPT计数器工作模式，连续
	tTimConfig.eClksrc=LPT_CLK_PCLK_DIV4;  		//LPT时钟源  
	csi_lpt_timer_init(LPT,&tTimConfig);        //初始化lpt 
	
	csi_lpt_int_enable(LPT,LPT_INTSRC_PEND);	 //使能LPT PEND中断  
	csi_lpt_start(LPT);	                        //启动lpt
	
	return iRet;	
}


/** \brief lpt pwm ouput
 * 
 *  \param[in] none
 *  \return error code
 */
int lpt_pwm_demo(void)
{
	int iRet = 0;
	
	csi_lpt_pwm_config_t tLptPwmCfg;  	
	
#if (USE_GUI == 0)	
	csi_gpio_set_mux(GPIOB, PB6, PB6_LPT_OUT);                   //将PB6设为LPT_OUT
#endif

	tLptPwmCfg.eClkSrc = LPT_CLK_PCLK_DIV4;                     //PWM 时钟选择
	tLptPwmCfg.eStartPol = LPT_PWM_START_HIGH;                   //初始低电平
	tLptPwmCfg.eIdlePol  = LPT_PWM_IDLE_LOW;                    //停止时highZ
	tLptPwmCfg.byDutyCycle = 40;                                 //PWM 输出占空比(0~100)	
	tLptPwmCfg.wFreq = 2000;                                     //PWM 输出频率

	if(csi_lpt_pwm_init(LPT, &tLptPwmCfg) == CSI_OK){            //初始化lpt
		csi_lpt_start(LPT);                                      //启动lpt
	}	
	
	return iRet;	
}

/** \brief lpt sync trg count
 *  外部下降沿中断触发lpt
 * \brief csi初始化里不开启中断，需要进中断函数，需调用csi_lpt_int_enable接口，demo默认开启pend中断
 *  \param[in] none
 *  \return error code
 */
int exi_etcb_lpt_start_demo(void)
{
	int iRet = 0;
	volatile uint8_t ch;
	csi_etcb_config_t tEtbConfig;				               			//ETB 参数配置结构体
	
#if (USE_GUI == 0)	
	csi_gpio_set_mux(GPIOB, PB0, PB0_INPUT);							//PB0 配置为输入
	csi_gpio_pull_mode(GPIOB, PB0, GPIO_PULLUP);						//PB0 上拉
	csi_gpio_irq_mode(GPIOB, PB0, EXI_GRP0, GPIO_IRQ_FALLING_EDGE);		//PB0 下降沿产生中断，选择中断组16
	csi_gpio_int_enable(GPIOB, PB0);									//PB0 中断使能
#endif 

	csi_exi_set_evtrg(EXI_TRGOUT0, EXI_TRGSRC_GRP0, 0);						//EXI0(PB00) 触发EXI_TRGOUT0(PB00用EXI0触发输出)
	csi_exi_evtrg_enable(EXI_TRGOUT0);										//使能 EXI_TRGOUT0触发输出
	
	
	csi_lpt_time_config_t tTimConfig;
	tTimConfig.wTimeVal = 5;					//LPT定时值 = 5ms
	tTimConfig.eRunMode  = LPT_CNT_CONT;		//LPT计数器工作模式
	tTimConfig.eClksrc=LPT_CLK_PCLK_DIV4;  		//LPT时钟源  
	csi_lpt_timer_init(LPT,&tTimConfig);        //初始化lpt 

	csi_lpt_int_enable(LPT,LPT_INTSRC_PEND);	 //使能LPT PEND中断
	
	csi_lpt_set_sync(LPT, LPT_TRG_SYNCIN0, LPT_SYNC_ONCE, ENABLE);
	csi_lpt_sync_enable(LPT);
	csi_lpt_debug_enable(LPT);
	
	csp_lpt_set_evtrg(LPT, LPT_TRGSRC0_ZRO);  
	
	tEtbConfig.eChType = ETCB_ONE_TRG_ONE;  		//单个源触发单个目标
	tEtbConfig.eSrcIp  = ETCB_EXI_TRGOUT0;  	    //EXI_TRGOUT5作为触发源
	tEtbConfig.eDstIp =  ETCB_LPT_SYNCIN;   	    //LPT同步输入作为目标事件
	tEtbConfig.eTrgMode = ETCB_HARDWARE_TRG;
	
	ch = csi_etcb_ch_alloc(tEtbConfig.eChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etcb_ch_init(ch, &tEtbConfig);

	return iRet;
	
}


/** \brief lpt trg out
 *  lpt通过etcb触发bt
 * \brief csi初始化里不开启中断，需要进中断函数，需调用csi_lpt_int_enable接口，demo默认开启pend中断
 *  \param[in] none
 *  \return error code
 */
int lpt_etcb_bt_start_demo(void)
{
	int iRet = 0;
	volatile uint8_t ch;
	csi_etcb_config_t tEtbConfig;				//ETB 参数配置结构体
	
	csi_lpt_time_config_t tTimConfig;
	tTimConfig.wTimeVal = 50;					//LPT定时值 = 50ms
	tTimConfig.eRunMode  = LPT_CNT_CONT;		//LPT计数器工作模式
	tTimConfig.eClksrc=LPT_CLK_PCLK_DIV4;  		//LPT时钟源  
	csi_lpt_timer_init(LPT,&tTimConfig);        //初始化lpt 
	
	csi_lpt_int_enable(LPT,LPT_INTSRC_PEND);	 //使能LPT PEND中断
	
	csi_lpt_set_evtrg(LPT, LPT_TRGOUT, LPT_TRGSRC_PRD, 10);
	csi_lpt_trg_enable(LPT);

	csp_lpt_set_evtrg(LPT, LPT_TRGSRC0_ZRO);  
	
	csp_lpt_set_trgprd(LPT,0xf);
	csp_lpt_set_trgcnt(LPT,0xf);

	csi_bt_time_config_t tTim2Config;
	tTim2Config.wTimeVal = 1000;				//BT定时值，单位：us；即定时值 = 1000us
	tTim2Config.eRunMode  = BT_RUN_CONT;		//BT计数器工作模式：连续/单次
	
	csi_bt_timer_init(BT1,&tTim2Config);						//BT定时
	csi_bt_set_sync(BT1, BT_SYNCIN0, BT_SYNC_ONCE, DISABLE);  	//外部触发bt启动(SYNCIN0)
	csi_bt_sync_enable(BT1, BT_SYNCIN0);                        // BT1同步输入0使能

	tEtbConfig.eChType = ETCB_ONE_TRG_ONE;  				    //单个源触发单个目标
	tEtbConfig.eSrcIp  = ETCB_LPT_TRGOUT0;  	    			//LPT作为触发源
	tEtbConfig.eDstIp =  ETCB_BT1_SYNCIN0;   	    			//BT1同步输入0 作为目标事件
	tEtbConfig.eTrgMode = ETCB_HARDWARE_TRG;
	
	
	ch = csi_etcb_ch_alloc(tEtbConfig.eChType);	    				//自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    				//ch < 0,则获取通道号失败
	iRet = csi_etcb_ch_init(ch, &tEtbConfig);
	
	csi_lpt_start(LPT);	                            				//启动lpt,触发BT1启动
	
	return iRet;
}


/** \brief lpt external clock with filter
 *  \brief csi初始化里不开启中断，需要进中断函数，需调用csi_lpt_int_enable接口，demo默认开启pend中断
 *  \param[in] none
 *  \return error code
 */
int lpt_filter_demo(void)
{
	int iRet = 0;
#if (USE_GUI == 0)	
	csi_gpio_set_mux(GPIOA,PA5, PA5_LPT_IN);                              //将PB5设为LPT_IN
#endif
	
	csi_lpt_time_config_t tTimConfig;
	tTimConfig.wTimeVal = 1;					//LPT定时值 = 1ms
	tTimConfig.eRunMode  = LPT_CNT_CONT;		//LPT计数器工作模式
	tTimConfig.eClksrc=LPT_CLK_IN_FALL;  		//LPT时钟源  
	csi_lpt_timer_init(LPT,&tTimConfig);        //初始化lpt 
	
	csi_lpt_int_enable(LPT,LPT_INTSRC_PEND);	 //使能LPT PEND中断
	
	csp_lpt_flt_init(LPT);    // CR[FLTIPSCLD]=1,数字滤波器计数器初始化，计数器值被初始化为CEDR[FLTCKPRS]中的设置值
	csp_lpt_set_filter(LPT, 3, LPT_FLTDEB_02);     // CEDR[FLTCKPRS]=3、CR[FLTDEB] =LPT_FLTDEB_02
	csi_lpt_start(LPT);	                             //启动lpt
	return iRet;	
}





