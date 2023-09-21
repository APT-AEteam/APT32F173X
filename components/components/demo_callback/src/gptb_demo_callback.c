/***********************************************************************//** 
 * \file  gptb_demo.c
 * \brief  TIMER_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-11  <td>V0.0  <td>ljy      <td>initial
 * <tr><td> 2023-5-8   <td>V0.1  <td>wangch   <td>modify
 * <tr><td> 2023-9-18  <td>V0.2  <td>wangch   <td>code normalization
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
static uint32_t s_wGptbCapBuff[4] = {0};

#if (USE_GPTB_CALLBACK == 1)		
/** \brief  user_capture_callback：GPTB捕获中断回调函数
 * 
 * 	\brief	用户定义，支持指CAPLD0/1/2/3四种中断处理，使用csi标准库，中断发生时会自动调用用户注册的回调函
 * 			数，用户可在回调函数里做自己的处理，而不需要关注具体的底层中断处理。
 * 
 *  \param[in]  ptGptbBase:	GPTBx寄存器结构体指针，指向GPTBx的基地址 
 *  \param[out] wIsr: 		GPTBx中断状态
 *  \return none
 */ 
void user_capture_callback(csp_gptb_t *ptGptbBase,  uint32_t wIsr)
{
	if(wIsr & GPTB_INTSRC_CAPLD0)
	{
		s_wGptbCapBuff[0] = csp_gptb_get_cmpa(GPTB0);
	}
}

/** \brief  user_pwm_callback：GPTB PWM pend中断回调函数
 * 
 * 	\brief	用户定义，支持多种中断处理, \ref csi_gptb_int_e。使用csi标准库，中断发生时会自动调用用户注册的回调函
 * 			数，用户可在回调函数里做自己的处理，而不需要关注具体的底层中断处理。
 * 
 *  \param[in]  ptGptbBase:	GPTBx寄存器结构体指针，指向GPTBx的基地址 
 *  \param[out] wIsr: 		GPTBx中断状态
 *  \return none
 */ 
void user_pwm_callback(csp_gptb_t *ptGptbBase,  uint32_t wIsr)
{
	if(wIsr & GPTB_INTSRC_PEND)
	{
		csi_gpio_toggle(GPIOA, PA6);		//PA6翻转
	}
}

/** \brief  user_emergency_callback：GPTB紧急模式中断回调函数
 * 
 * 	\brief	用户定义，支持多种中断处理， \ref csi_gptb_emint_e。使用csi标准库，中断发生时会自动调用用户注册的回调函
 * 			数，用户可在回调函数里做自己的处理，而不需要关注具体的底层中断处理。
 * 
 *  \param[in]  ptGptbBase:	GPTBx寄存器结构体指针，指向GPTBx的基地址 
 *  \param[out] wIsr: 		GPTBx中断状态
 *  \return none
 */ 
void user_emergency_callback(csp_gptb_t *ptGptbBase,  uint32_t wIsr)
{
	if(wIsr & GPTB_EM_INTSRC_EP0)
	{
		csi_gpio_toggle(GPIOA, PA5);		//PA5翻转
	}
}
#endif

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
int gptb_capture_callback_demo(void)
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
	tCapCfg.eWorkMode         	= GPTB_WORK_CAPTURE;                     //GPTB工作模式：捕获/波形输出	
	tCapCfg.eCountMode    		= GPTB_CNT_UP;                       //GPTB计数模式：递增/递减/递增递减	
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
	csi_gptb_register_callback(GPTB0,user_capture_callback);
	csi_gptb_set_sync(GPTB0, GPTB_SYNCIN0, GPTB_SYNC_CONT, GPTB_AUTO_REARM_ZRO);//使能SYNCIN0外部触发
    csi_gptb_set_sync(GPTB0, GPTB_SYNCIN3, GPTB_SYNC_CONT, GPTB_AUTO_REARM_ZRO);//使能SYNCIN3外部触发
	csi_gptb_start(GPTB0);
    while(1)
	{
		mdelay(1);
	}
	return iRet;
}

/** \brief GPTB 波形输出示例代码
 *   		-波形参数：10KHz，占空比50%
 *     		-开启周期结束中断，并在中断里面翻转IO(需要打开PA6 IO配置注释)；
 * 			-若不需要开启中断,需调用csi_gptb_int_disable接口函数，关闭周期结束中断。
 * 			-观察到PA6端口的频率为10KHz/2=5KHz
 *  \param[in] none
 *  \return error code
 */
int gptb_pwm_callback_demo(void)
{
	int iRet = 0;	
	csi_gptb_pwm_config_t 			tPwmCfg;						//GPTB捕获参数配置结构体	
	csi_gptb_pwm_channel_config_t   tPwmChannelCfg;	
//------------------------------------------------------------------------------------------------------------------------	
#if (USE_GUI == 0)		
	csi_gpio_set_mux(GPIOC, PC13, PC13_GPTB0_CHAX);					//初始化PC13为CHAX
	csi_gpio_set_mux(GPIOA, PA7, PA7_GPTB0_CHAY);					//初始化PA7为CHAY
	csi_gpio_set_mux(GPIOD, PD0, PD0_GPTB0_CHB);					//初始化PD0为CHBX
	csi_gpio_set_mux(GPIOA, PA6, PA6_OUTPUT);						//初始化PA6为输出	
#endif
//------------------------------------------------------------------------------------------------------------------------		
	tPwmCfg.eWorkMode       	 = GPTB_WORK_WAVE;                  //GPTB工作模式：捕获/波形输出
	tPwmCfg.eCountMode   		 = GPTB_CNT_UPDN;                   //GPTB计数模式：递增/递减/递增递减
	tPwmCfg.eRunMode    		 = GPTB_RUN_CONT;                   //GPTB运行模式：连续/一次性
	tPwmCfg.byDutyCycle 		 = 50;								//GPTB输出PWM占空比			
	tPwmCfg.wFreq 				 = 10000;							//GPTB输出PWM频率	
	csi_gptb_wave_init(GPTB0, &tPwmCfg);
//------------------------------------------------------------------------------------------------------------------------	
	tPwmChannelCfg.eActionZro    =   GPTB_ACT_LO;
	tPwmChannelCfg.eActionPrd    =   GPTB_ACT_NA;
	tPwmChannelCfg.eActionC1u    =   GPTB_ACT_HI;
	tPwmChannelCfg.eActionC1d    =   GPTB_ACT_LO;
	tPwmChannelCfg.eActionC2u    =   GPTB_ACT_NA;
	tPwmChannelCfg.eActionC2d    =   GPTB_ACT_NA;
	tPwmChannelCfg.eActionT1u    =   GPTB_ACT_NA;
	tPwmChannelCfg.eActionT1d    =   GPTB_ACT_NA;
	tPwmChannelCfg.eActionT2u    =   GPTB_ACT_NA;
	tPwmChannelCfg.eActionT2d    =   GPTB_ACT_NA;
	tPwmChannelCfg.eC1sel 		 =   GPTB_COMPA;
	tPwmChannelCfg.eC2sel 		 =   GPTB_COMPA;
	csi_gptb_pwm_channel_init(GPTB0, &tPwmChannelCfg,  GPTB_CHANNEL_1);
	csi_gptb_pwm_channel_init(GPTB0, &tPwmChannelCfg,  GPTB_CHANNEL_2);
//------------------------------------------------------------------------------------------------------------------------
	csi_gptb_int_enable(GPTB0, GPTB_INTSRC_PEND);
	csi_gptb_register_callback(GPTB0,user_pwm_callback);
	csi_gptb_start(GPTB0);

	while(1)
	{
		nop;
	}	
    return iRet;
}

/** \brief GPTB带死区延迟和锁止功能的互补波形输出：死区时间（上升沿500ns，下降沿200ns）
 *   		-波形基本参数：10KHz，50%
 *     		-设置死区模块，得到带死区延迟的互补波形
 * 			-设置紧急模块，当紧急输入脚为低电平时，强制拉低所有PWM波形
 * 			-开启EP0中断，并在中断里面翻转IO(需要打开PA5 IO配置注释)；
 * 			-若不需要开启中断,需调用csi_gptb_int_disable接口函数，关闭周期结束中断。
 * 			-观察PA5端口，在每次紧急中断产生时翻转
 *  \param[in] none
 *  \return error code
 */
int gptb_pwm_dz_em_callback_demo(void)
{
	int iRet = 0;	
	csi_gptb_pwm_config_t 			tPwmCfg;	
	csi_gptb_pwm_channel_config_t   tPwmChannelCfg;
	csi_gptb_deadzone_config_t  	tDeadZoneCfg;
	csi_gptb_emergency_config_t   	tEmCfg; 
////------------------------------------------------------------------------------------------------------------------------	
#if (USE_GUI == 0)		
	csi_gpio_set_mux(GPIOC,  PC13, PC13_GPTB0_CHAX);				//初始化PC13为CHAX
	csi_gpio_set_mux(GPIOA,  PA7,  PA7_GPTB0_CHAY);					//初始化PA7为CHAY
	csi_gpio_set_mux(GPIOD,  PD0,  PD0_GPTB0_CHB);					//初始化PD0为CHBX	
	csi_gpio_set_mux(GPIOA,  PA6,  PA6_EBI0);						//初始化PA6为EBI0
	csi_gpio_set_mux(GPIOA,  PA9,  PA9_EBI1);						//初始化PA9为EBI1
	csi_gpio_set_mux(GPIOB,  PB2,  PB2_EBI2);						//初始化PB2为EBI2
	csi_gpio_set_mux(GPIOA,  PA0,  PA0_EBI3);						//初始化PA0为EBI3
	csi_gpio_pull_mode(GPIOA,PA6,  GPIO_PULLUP);					//EBI0上拉使能
	csi_gpio_pull_mode(GPIOA,PA9,  GPIO_PULLUP);					//EBI1上拉使能
	csi_gpio_pull_mode(GPIOB,PB2,  GPIO_PULLUP);					//EBI2上拉使能
	csi_gpio_pull_mode(GPIOA,PA0,  GPIO_PULLUP);					//EBI3上拉使能
	csi_gpio_set_mux(GPIOA, PA5, PA5_OUTPUT);						//初始化PA5为输出
#endif
//------------------------------------------------------------------------------------------------------------------------		
	tPwmCfg.eWorkMode       		= GPTB_WORK_WAVE;               //GPTB工作模式：捕获/波形输出
	tPwmCfg.eCountMode   			= GPTB_CNT_UPDN;                //GPTB计数模式：递增/递减/递增递减
	tPwmCfg.eRunMode    			= GPTB_RUN_CONT;                //GPTB运行模式：连续/一次性
	tPwmCfg.byDutyCycle 			= 50;							//GPTB输出PWM占空比			
	tPwmCfg.wFreq 					= 10000;						//GPTB输出PWM频率
	csi_gptb_wave_init(GPTB0, &tPwmCfg);
//------------------------------------------------------------------------------------------------------------------------	
	tPwmChannelCfg.eActionZro    	= GPTB_ACT_LO;
	tPwmChannelCfg.eActionPrd    	= GPTB_ACT_NA;
	tPwmChannelCfg.eActionC1u    	= GPTB_ACT_HI;
	tPwmChannelCfg.eActionC1d    	= GPTB_ACT_LO;
	tPwmChannelCfg.eActionC2u    	= GPTB_ACT_NA;
	tPwmChannelCfg.eActionC2d    	= GPTB_ACT_NA;
	tPwmChannelCfg.eActionT1u    	= GPTB_ACT_NA;
	tPwmChannelCfg.eActionT1d    	= GPTB_ACT_NA;
	tPwmChannelCfg.eActionT2u    	= GPTB_ACT_NA;
	tPwmChannelCfg.eActionT2d    	= GPTB_ACT_NA;
	tPwmChannelCfg.eC1sel  		 	= GPTB_COMPA;
	tPwmChannelCfg.eC2sel  		 	= GPTB_COMPA;
	csi_gptb_pwm_channel_init(GPTB0, &tPwmChannelCfg,  GPTB_CHANNEL_1);
	csi_gptb_pwm_channel_init(GPTB0, &tPwmChannelCfg,  GPTB_CHANNEL_2);
//------------------------------------------------------------------------------------------------------------------------
	tDeadZoneCfg.eChxOutSel_S1S0    = GPTB_DBOUT_AR_BF;             //使能通道A的上升沿延时，使能通道B的下降沿延时
	tDeadZoneCfg.eChxPol_S3S2       = GPTB_DBPOL_B;                 //通道A和通道B延时输出电平是否反向
	tDeadZoneCfg.eChxInSel_S5S4     = GPTB_DBIN_AR_AF;              //PWMA作为上升沿和下降沿延时处理的输入信号
	tDeadZoneCfg.eChxDedb_S6        = GPTB_DEDB_AR_BF;       	    //不使用死区双沿
	tDeadZoneCfg.eChxOutSwap_S8S7   = GPTB_DBSWAP_OUTA_A_OUTB_B;    //OUTA=通道A输出，OUTB=通道B输出
	tDeadZoneCfg.eDclkSel           = GPTB_DCKSEL_DPSC;
	tDeadZoneCfg.hwDpsc             = 0;             				//FDBCLK = FPCLK / (DPSC+1)
	tDeadZoneCfg.wRisingEdgeTime    = 500;             				//上升沿延迟：ns
	tDeadZoneCfg.wFallingEdgeTime   = 200;             				//下降沿延迟：ns
    csi_gptb_deadzone_init(GPTB0, &tDeadZoneCfg, GPTB_CHANNEL_1);
//------------------------------------------------------------------------------------------------------------------------
	tEmCfg.eEp       				= GPTB_EP0;            			//EP通道选择：EP0/1/2/3
    tEmCfg.eEmSrc    				= GPTB_EMSRC_EBI0;              //EP输入源选择
    tEmCfg.eEmPol   				= GPTB_EMPOL_L;      			//EP输入源极性选择
	tEmCfg.eEpxLockMode  			= GPTB_LCKMD_SLCK;        		//EP锁止模式选择：软锁/硬锁
	tEmCfg.eOsrShdw   				= GPTB_OSR_SHADOW;       	 	//EMOSR shadow模式选择
	
	if(tEmCfg.eEpxLockMode == GPTB_LCKMD_SLCK)
		tEmCfg.eSlckClrMode = GPTB_SLCLRMD_ZRO;           			//软锁自动清除条件
	if(tEmCfg.eOsrShdw == GPTB_OSR_SHADOW)
		tEmCfg.eOsrLoadMode = GPTB_LD_EMOSR_PRD;					//EMOSR载入控制设置

    csi_gptb_emergency_init(GPTB0,&tEmCfg); 
	csi_gptb_set_emergency_out(GPTB0, GPTB_EMCOAX, GPTB_EMOUT_L);   //紧急状态下CHAX输出状态设置为低电平
	csi_gptb_set_emergency_out(GPTB0, GPTB_EMCOAY, GPTB_EMOUT_L);	//紧急状态下CHAY输出状态设置为低电平
	csi_gptb_set_emergency_out(GPTB0, GPTB_EMCOBX, GPTB_EMOUT_L);	//紧急状态下CHBX输出状态设置为低电平
	csi_gptb_emint_enable(GPTB0, GPTB_EM_INTSRC_EP0);      			//紧急状态输EP0中断使能
	csi_gptb_register_callback(GPTB0,user_emergency_callback);
//------------------------------------------------------------------------------------------------------------------------
	csi_gptb_start(GPTB0);//start  timer

	while(1)
	{
		nop;
	}	
	return iRet;
}