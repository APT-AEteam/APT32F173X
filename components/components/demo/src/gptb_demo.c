/***********************************************************************//** 
 * \file  gptb_demo.c
 * \brief  TIMER_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-11 <td>V0.0 <td>ljy     <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include <drv/gptb.h>
#include <drv/pin.h>


/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/


/** \brief gptb
 * 
 *  \param[in] none
 *  \return error code
 */
//int gptb_demo(void)
//{
//	int iRet = 0;	
////------------------------------------------------------------------------------------------------------------------------	
//	csi_pin_set_mux(PA8,  PA8_GPTB0_CHAX);						//
//	csi_pin_set_mux(PB13, PB13_GPTB0_CHAY);						//
//	csi_pin_set_mux(PD0,  PD0_GPTB0_CHB);
//	
////	csi_pin_set_mux(PA07,PA07_EPI0);
////	csi_pin_set_mux(PA013,PA013_EPI1);
////	csi_pin_set_mux(PB03, PB03_EPI2);
////	csi_pin_set_mux(PB02, PB02_EPI3);
////------------------------------------------------------------------------------------------------------------------------	
//	csi_gptb_config_t tPwmCfg;								  
//	tPwmCfg.byWorkmod       = GPTB_WAVE;                        //WAVE or CAPTURE    //计数或捕获	
//	tPwmCfg.byCountingMode  = GPTB_UPCNT;                       //CNYMD  //计数方向
//	tPwmCfg.byOneshotMode   = GPTB_OP_CONT;                     //OPM    //单次或连续(工作方式)
//	tPwmCfg.byStartSrc      = GPTB_SYNC_START;				    //软件使能同步触发使能控制（RSSR中START控制位）//启动方式
//	tPwmCfg.byPscld         = GPTB_LDPSCR_ZRO;                  //PSCR(分频)活动寄存器载入控制。活动寄存器在配置条件满足时，从影子寄存器载入更新值
//	
//	if(tPwmCfg.byWorkmod==GPTB_WAVE)                            //工作在波形模式
//	{
//	    tPwmCfg.byDutyCycle 	 = 50;								//pwm ouput duty cycle//PWM初始值			
//	    tPwmCfg.wFreq 			 = 10000;							//pwm ouput frequency
//		
//	}
//	else{ tPwmCfg.wFreq 		 = 10000;                         //捕获频率大概范围
//		tPwmCfg.byCaptureCapLden = true ;                         //CMPA和CMPB在捕捉事件载入使能
//		tPwmCfg.byCaptureRearm   = true ;                         //1:重置捕捉计数器
//		tPwmCfg.byCaptureCapmd   = 0;                             //0:连续捕捉模式    1h：一次性捕捉模式
//		tPwmCfg.byCaptureStopWrap=4-1;                            //Capture模式下，捕获事件计数器周期设置值
//		tPwmCfg.byCaptureLdaret  =0;                              //CMPA捕捉载入后，计数器值计数状态控制位(1h：CMPA触发后，计数器值进行重置;0h：CMPA触发后，计数器值不进行重置)
//		tPwmCfg.byCaptureLdbret  =0;                              
//		tPwmCfg.byCaptureLdcret  =0;                              
//		tPwmCfg.byCaptureLddret  =0;                              
//	}
//	
//	tPwmCfg.byBurst   = false ;                                    //使能群脉冲模式
//	tPwmCfg.byCgsrc   = GPTB_CGSRC_TIOA;                           //CHAX作为CG的输入源
//	tPwmCfg.byCgflt   = GPTB_CGFLT_2;                              //门控输入数字滤波控制
//		
//	tPwmCfg.byInter 		 = GPTB_INT_TRGEV0;                    //interrupt
//	csi_gptb_config_init(GPTB0, &tPwmCfg);//EPT0
//
////------------------------------------------------------------------------------------------------------------------------
//	csi_gptb_pwmchannel_config_t  tEptchannelCfg;
//	tEptchannelCfg.byActionZro    =   LO;
//	tEptchannelCfg.byActionPrd    =   NA;
//	tEptchannelCfg.byActionCau    =   HI;
//	tEptchannelCfg.byActionCad    =   LO;
//	tEptchannelCfg.byActionCbu    =   NA;
//	tEptchannelCfg.byActionCbd    =   NA;
//	tEptchannelCfg.byActionT1u    =   LO;
//	tEptchannelCfg.byActionT1d    =   LO;
//	tEptchannelCfg.byActionT2u    =   NA;
//	tEptchannelCfg.byActionT2d    =   NA;
//	tEptchannelCfg.byChoiceCasel  =   GPTB_CMPA;
//	tEptchannelCfg.byChoiceCbsel  =   GPTB_CMPA;	
//	csi_gptb_channel_config(GPTB0, &tEptchannelCfg,  GPTB_CHANNEL_A);//channel
//	csi_gptb_channel_config(GPTB0, &tEptchannelCfg,  GPTB_CHANNEL_B);
//	
//    //csp_gptb_set_aqtscr(GPTB0,GPTB_T1,EP1);//波形输出T事件选择
////------------------------------------------------------------------------------------------------------------------------	
//
//	csi_gptb_dbldrload_config(GPTB0,GPTB_DBCR,GPTB_SHDW_IMMEDIATE,GPTB_LD_ZRO);
//	
////------------------------------------------------------------------------------------------------------------------------	
//	csi_gptb_deadzone_config_t  tGptbDeadZoneTime;
//	tGptbDeadZoneTime.byDcksel               = DB_DPSC;         //
//	tGptbDeadZoneTime.hwDpsc                 =  0;              //FDBCLK = FHCLK / (DPSC+1)
//	tGptbDeadZoneTime.hwRisingEdgereGister = 24;                //NS/(1/(48000000/(DPSC+1))*10^9) // 500NS/(1000/48) = 24;
//	tGptbDeadZoneTime.hwFallingEdgereGister= 24;                //下降沿
//	tGptbDeadZoneTime.byChaDedb             = DB_AR_BF;         //不使用死区双沿
//	tGptbDeadZoneTime.byChbDedb             = DB_AR_BF;
//	tGptbDeadZoneTime.byChcDedb             = DB_AR_BF;
//	csi_gptb_dbcr_config(GPTB0,&tGptbDeadZoneTime);
//	
//	tGptbDeadZoneTime.byChxOuselS1S0      = DBOUT_AR_BF;              //使能通道A的上升沿延时，使能通道B的下降沿延时
//	tGptbDeadZoneTime.byChxPolarityS3S2   = DB_POL_B;                 //通道A和通道B延时输出电平是否反向
//	tGptbDeadZoneTime.byChxInselS5S4      = DBCHAIN_AR_AF;            //PWMA作为上升沿和下降沿延时处理的输入信号
//	tGptbDeadZoneTime.byChxOutSwapS8S7    = DBCHAOUT_OUTA_B_OUTB_A;   //OUTA=通道A输出，OUTB=通道B输出
//    csi_gptb_channelmode_config(GPTB0,&tGptbDeadZoneTime,GPTB_CHANNEL_A);
//	
////------------------------------------------------------------------------------------------------------------------------
//    csi_gptb_emergency_config_t   tEptEmergencyCfg;           //紧急状态输入
//    tEptEmergencyCfg.byEpxInt    = EBI1 ;                     //EPx选择外部IO端口（EBI0~EBI4）
//    tEptEmergencyCfg.byPolEbix   = EBI_POL_H;                 //EBIx的输入有效极性选择控制
//	tEptEmergencyCfg.byEpx       = EP1;                       //使能EPx
//	tEptEmergencyCfg.byEpxLckmd  = EP_HLCK;                   //使能 锁
//    tEptEmergencyCfg.byFltpace0  = EPFLT0_2P;                 //EP0、EP1、EP2和EP3的数字去抖滤波检查周期数
//	tEptEmergencyCfg.byFltpace1  = EPFLT1_2P;                 //EP4、EP5、EP6和EP7的数字去抖滤波检查周期数
//	if(tEptEmergencyCfg.byEpxInt ==ORL0){tEptEmergencyCfg.byOrl0 = ORLx_EP0 |ORLx_EP1|ORLx_EP2;}
//	if(tEptEmergencyCfg.byEpxInt ==ORL1){tEptEmergencyCfg.byOrl1 = ORLx_EP4 |ORLx_EP5|ORLx_EP6;}
//	csi_gptb_emergency_cfg(GPTB0,&tEptEmergencyCfg);
//	
//	csi_gptb_emergency_pinxout(GPTB0,GPTB_EMCOAX,EM_OUT_L);
//	csi_gptb_emergency_pinxout(GPTB0,GPTB_EMCOBX,EM_OUT_L);	
//	csi_gptb_emergency_pinxout(GPTB0,GPTB_EMCOAY,EM_OUT_L);
//	
////	csi_gptb_emergency_interruption_en(GPTB0,GPTB_INT_EP1);      //紧急状态输入中断使能
//	
////------------------------------------------------------------------------------------------------------------------------	
//
////------------------------------------------------------------------------------------------------------------------------
//
////  csi_gptb_Global_load_control_config_t  Globaldemo;
////	Globaldemo.bGlden    =   DISABLE;                           //全局的Shadow到Active寄存器载入控制使能
////	Globaldemo.byGldmd   =   GPTB_LDGLD_ZRO_PRD_LD_SYNC;        //全局载入触发事件选择
////	Globaldemo.bOstmd    =   GPTB_LDMD_ANYTIME;                 //One Shot 载入模式使能控制位
////	Globaldemo.bGldprd   =   0 ;                                //全局载入触发周期选择。可以选择N次触发条件满足后，才进行一次全局载入。	                       
////	csi_gptb_global_config(GPTB0,&Globaldemo);
////	csi_gptb_global_rearm(GPTB0) ;                              //重置ONE SHOT模式。ONE SHOT模式下，一次触发后，需要重置模式才允许再次触发
////	csi_gptb_global_sw(GPTB0) ;                                 //软件产生一次GLD触发
//
////------------------------------------------------------------------------------------------------------------------------	
//	csi_gptb_start(GPTB0);//start  timer
//    while(1){
////		    if((csp_gptb_get_emHdlck(GPTB0)&GPTB_INT_EP1)==GPTB_INT_EP1)
////				{
////					mdelay(10);
////					csp_gptb_clr_emHdlck(GPTB0,EP1);
////				}
////            csp_gptb_evtrg_soft(GPTB0,0);			
//		    csi_gptb_change_ch_duty(GPTB0,GPTB_CH_A, 25);		      
//		    mdelay(200);                        
//			//csi_gptb_Continuous_software_waveform(GPTB0,GPTB_CHANNEL_A,EM_AQCSF_L);//
//			//mdelay(200);
//			//csi_gptb_Continuous_software_waveform(GPTB0,GPTB_CHANNEL_A,EM_AQCSF_NONE);//持续性软件波形控制寄存器
//		    csi_gptb_change_ch_duty(GPTB0,GPTB_CH_A, 50);	
//		    mdelay(200);
//	}			
//	return iRet;
//};



int gptb_demo1(void)
{
	int iRet = 0;	
//------------------------------------------------------------------------------------------------------------------------	
	csi_pin_set_mux(PA8,  PA8_GPTB0_CHAX);						//
	csi_pin_set_mux(PB13, PB13_GPTB0_CHAY);						//
	csi_pin_set_mux(PD0,  PD0_GPTB0_CHB);	

//------------------------------------------------------------------------------------------------------------------------	
	csi_gptb_captureconfig_t tPwmCfg;								  
		tPwmCfg.byWorkmod       = GPTB_CAPTURE;                     //WAVE or CAPTURE    //计数或捕获	
		tPwmCfg.byCountingMode  = GPTB_UPCNT;                       //CNYMD  //计数方向
		tPwmCfg.byOneshotMode   = GPTB_OP_CONT;                     //OPM    //单次或连续(工作方式)
		tPwmCfg.byStartSrc      = GPTB_SYNC_START;				    //软件使能同步触发使能控制（RSSR中START控制位）//启动方式
	    tPwmCfg.byPscld         = GPTB_LDPSCR_ZRO;                  //PSCR(分频)活动寄存器载入控制。活动寄存器在配置条件满足时，从影子寄存器载入更新值	
		tPwmCfg.byCaptureCapmd  = 0;                                //0:连续捕捉模式    1h：一次性捕捉模式
		tPwmCfg.byCapSrcMode    = GPTB_SEPARATE_CAP;                //0:GPTB_MERGE_CAP合并触发信号------1：GPTB_SEPARATE_CAP分离触发信号
		tPwmCfg.byCaptureStopWrap=2-1;                              //Capture模式下，捕获事件计数器周期设置值
		tPwmCfg.byCaptureLdaret  =0;                                //CMPA捕捉载入后，计数器值计数状态控制位(1h：CMPA触发后，计数器值进行重置;0h：CMPA触发后，计数器值不进行重置)
		tPwmCfg.byCaptureLdbret  =1;                              
                             	 
	    tPwmCfg.byInter 		 =GPTB_INT_CAPLD1;                   //interrupt
	csi_gptb_capture_init(GPTB0, &tPwmCfg);

//------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------	

	csi_gptb_set_sync (GPTB0, GPTB_TRGIN_SYNCEN2, GPTB_TRG_CONTINU,GPTB_AUTO_REARM_ZRO);
	csi_gptb_set_sync (GPTB0, GPTB_TRGIN_SYNCEN3, GPTB_TRG_CONTINU,GPTB_AUTO_REARM_ZRO);

	csi_gptb_start(GPTB0);//start  timer
	
	return iRet;
};
void apt_gptb_irqhandler(void)
  {  	
	  if(((csp_gptb_get_misr(GPTB0) & GPTBINT_CAPLD1))==GPTBINT_CAPLD1)
		{		
//		  csi_pin_set_high(PA0);
		  csp_gptb_clr_int(GPTB0, GPTBINT_CAPLD1);
//		  csi_pin_set_low(PA0); 		
		} 
  }
int gptb_demo12(void)
{
	int iRet = 0;	
////------------------------------------------------------------------------------------------------------------------------	
	csi_pin_set_mux(PB15, PB15_GPTB5_CHAY);						//
	csi_pin_set_mux(PB14, PB14_GPTB4_CHAY);						//
	csi_pin_set_mux(PB13, PB13_GPTB3_CHAY);						   	
	csi_pin_set_mux(PA8, PA8_GPTB3_CHAX);						//	
	csi_pin_set_mux(PA9, PA9_GPTB4_CHAX);						//
	csi_pin_set_mux(PA10,PA10_GPTB5_CHAX);
	
//	csi_pin_set_mux(PD0, PD0_GPTB0_CHB);	
//	csi_pin_set_mux(PD1, PD1_GPTB1_CHB);
//	csi_pin_set_mux(PD5, PD5_GPTB2_CHB);
//	csi_pin_set_mux(PC0, PC0_GPTB3_CHB);
//	csi_pin_set_mux(PC1, PC1_GPTB4_CHB);
//	csi_pin_set_mux(PC2, PC2_GPTB5_CHB);
	
//	csi_pin_set_mux(PD4,   PD4_GPTB1_CHAY);
//	csi_pin_set_mux(PC13, PC13_GPTB0_CHAX);
//	csi_pin_set_mux(PC14, PC14_GPTB1_CHAX);
//	csi_pin_set_mux(PC15, PC15_GPTB2_CHAX);
//	csi_pin_set_mux(PA0,  PA0_GPTB3_CHAX);	
//	csi_pin_set_mux(PA1,  PA1_GPTB4_CHAX);
//	csi_pin_set_mux(PA3,  PA3_GPTB5_CHAX);
//	csi_pin_set_mux(PA6,  PA6_GPTB3_CHAX);
//	csi_pin_set_mux(PA7,  PA7_GPTB4_CHAX);//50
//	csi_pin_set_mux(PB0,  PB0_GPTB5_CHAX);
//	csi_pin_set_mux(PB1,  PB1_GPTB5_CHAY);
//	csi_pin_set_mux(PB13, PB13_GPTB0_CHAY);
//	csi_pin_set_mux(PB14, PB14_GPTB1_CHAY);
//	csi_pin_set_mux(PB15, PB15_GPTB2_CHAY);
//	csi_pin_set_mux(PA8,  PA8_GPTB0_CHAX);
//	csi_pin_set_mux(PA9,  PA9_GPTB1_CHAX);
//	csi_pin_set_mux(PA10, PA10_GPTB2_CHAX);
//	csi_pin_set_mux(PA11, PA11_GPTB3_CHAY);
//	csi_pin_set_mux(PA12, PA12_GPTB3_CHAX);
//	csi_pin_set_mux(PB4,  PB4_GPTB3_CHAX);
//	csi_pin_set_mux(PB5,  PB5_GPTB4_CHAX);
//	csi_pin_set_mux(PB6,  PB6_GPTB5_CHAX);
	
//	csi_pin_set_mux(PD4,   PD4_GPTB2_CHAY);
//	csi_pin_set_mux(PC13, PC13_GPTB5_CHAX);
//	csi_pin_set_mux(PC14, PC14_GPTB5_CHAY);
//	csi_pin_set_mux(PD0,   PD0_GPTB5_CHAX);
//	csi_pin_set_mux(PA7,  PA7_GPTB3_CHAY);
//	csi_pin_set_mux(PB0,  PB0_GPTB4_CHAY);
//	csi_pin_set_mux(PB1,  PB1_GPTB2_CHAY);
//	csi_pin_set_mux(PB13, PB13_GPTB0_CHAY);
//	csi_pin_set_mux(PB14, PB14_GPTB0_CHAX);
//	csi_pin_set_mux(PB15, PB15_GPTB1_CHAY);
//	csi_pin_set_mux(PA8,  PA8_GPTB1_CHAX);
//	csi_pin_set_mux(PA9,  PA9_GPTB2_CHAY);
//	csi_pin_set_mux(PA10, PA10_GPTB2_CHAX);
//	csi_pin_set_mux(PA11, PA11_GPTB0_CHAY);
//	csi_pin_set_mux(PA12, PA12_GPTB0_CHAX);
//	csi_pin_set_mux(PB6,  PB6_GPTB3_CHAX);
//	csi_pin_set_mux(PB7,  PB7_GPTB3_CHAY);
//	csi_pin_set_mux(PB8,  PB8_GPTB4_CHAX);
//	csi_pin_set_mux(PB9,  PB9_GPTB4_CHAY);
	csi_pin_set_mux(PD7,   PD7_GPTB5_CHAX);
	csi_pin_set_mux(PD8,   PD8_GPTB5_CHAY);
	
//	csi_pin_set_mux(PC13, PC13_GPTB1_CHAX);
//	csi_pin_set_mux(PC14, PC14_GPTB2_CHAY);
//	csi_pin_set_mux(PA7,  PA7_GPTB0_CHAY);
//	csi_pin_set_mux(PB0,  PB0_GPTB1_CHAY);
//	csi_pin_set_mux(PB1,  PB1_GPTB1_CHAY);
//	csi_pin_set_mux(PD7,   PD7_GPTB0_CHAY);
//	csi_pin_set_mux(PD8,   PD8_GPTB0_CHAX);
	
//	csi_pin_set_mux(PB0,   PB0_GPTB0_CHAX);
//	csi_pin_set_mux(PD8,   PD8_GPTB1_CHAY);
	
////------------------------------------------------------------------------------------------------------------------------		
    csi_gptb_pwmconfig_t tPwmCfg;								  
	tPwmCfg.byWorkmod        = GPTB_WAVE;                       //WAVE  波形模式
	tPwmCfg.byCountingMode   = GPTB_UPDNCNT;                    //CNYMD  //计数方向
	tPwmCfg.byOneshotMode    = GPTB_OP_CONT;                    //OPM    //单次或连续(工作方式)
	tPwmCfg.byStartSrc       = GPTB_SYNC_START;					//软件使能同步触发使能控制（RSSR中START控制位）//启动方式
	tPwmCfg.byPscld          = GPTB_LDPSCR_ZRO;                 //PSCR(分频)活动寄存器载入控制。活动寄存器在配置条件满足时，从影子寄存器载入更新值		
	tPwmCfg.byDutyCycle 	 = 90;								//pwm ouput duty cycle//PWM初始值			
	tPwmCfg.wFreq 			 = 10000;							//pwm ouput frequency	
	tPwmCfg.byInter 		 = GPTBINT_N;                       //interrupt
	csi_gptb_wave_init(GPTB0, &tPwmCfg);
	
	tPwmCfg.byDutyCycle 	 = 80;
    csi_gptb_wave_init(GPTB1, &tPwmCfg);
	
	tPwmCfg.byDutyCycle 	 = 70;
    csi_gptb_wave_init(GPTB2, &tPwmCfg);
	
	tPwmCfg.byDutyCycle 	 = 60;
    csi_gptb_wave_init(GPTB3, &tPwmCfg);
	
	tPwmCfg.byDutyCycle 	 = 50;
    csi_gptb_wave_init(GPTB4, &tPwmCfg);
	tPwmCfg.byDutyCycle 	 = 40;
    csi_gptb_wave_init(GPTB5, &tPwmCfg);
	
////------------------------------------------------------------------------------------------------------------------------	
	csi_gptb_pwmchannel_config_t  channel;
	channel.byActionZro    =   LO;
	channel.byActionPrd    =   NA;
	channel.byActionCau    =   HI;
	channel.byActionCad    =   LO;
	channel.byActionCbu    =   NA;
	channel.byActionCbd    =   NA;
	channel.byActionT1u    =   LO;
	channel.byActionT1d    =   LO;
	channel.byActionT2u    =   NA;
	channel.byActionT2d    =   NA;
	channel.byChoiceCasel  =   GPTB_CMPA;
	channel.byChoiceCbsel  =   GPTB_CMPA;

	csi_gptb_channel_config(GPTB0, &channel,  GPTB_CHANNEL_A);//channel
	csi_gptb_channel_config(GPTB0, &channel,  GPTB_CHANNEL_B);
	csi_gptb_channel_config(GPTB1, &channel,  GPTB_CHANNEL_A);//channel
	csi_gptb_channel_config(GPTB1, &channel,  GPTB_CHANNEL_B);
	csi_gptb_channel_config(GPTB2, &channel,  GPTB_CHANNEL_A);//channel
	csi_gptb_channel_config(GPTB2, &channel,  GPTB_CHANNEL_B);	
	csi_gptb_channel_config(GPTB3, &channel,  GPTB_CHANNEL_A);//channel
	csi_gptb_channel_config(GPTB3, &channel,  GPTB_CHANNEL_B);	
	csi_gptb_channel_config(GPTB4, &channel,  GPTB_CHANNEL_A);//channel
	csi_gptb_channel_config(GPTB4, &channel,  GPTB_CHANNEL_B);	
    csi_gptb_channel_config(GPTB5, &channel,  GPTB_CHANNEL_A);//channel
	csi_gptb_channel_config(GPTB5, &channel,  GPTB_CHANNEL_B);
	
////------------------------------------------------------------------------------------------------------------------------	
//    csi_gptb_deadzone_config_t  tGptbDeadZoneTime;
//	tGptbDeadZoneTime.byDcksel               = DB_DPSC;         //
//	tGptbDeadZoneTime.hwDpsc                 =  0;              //FDBCLK = FHCLK / (DPSC+1)
//	tGptbDeadZoneTime.hwRisingEdgereGister   = 500;             //上升沿-ns
//	tGptbDeadZoneTime.hwFallingEdgereGister  = 200;             //下降沿-ns
//	tGptbDeadZoneTime.byChaDedb              = DB_AR_BF;        //不使用死区双沿
//	tGptbDeadZoneTime.byChbDedb              = DB_AR_BF;
//	tGptbDeadZoneTime.byChcDedb              = DB_AR_BF;
//	csi_gptb_dz_config(GPTB3,&tGptbDeadZoneTime);
//	csi_gptb_dz_config(GPTB4,&tGptbDeadZoneTime);
//	csi_gptb_dz_config(GPTB5,&tGptbDeadZoneTime);
//	
//	tGptbDeadZoneTime.byChxOuselS1S0      = DBOUT_AR_BF;              //使能通道A的上升沿延时，使能通道B的下降沿延时
//	tGptbDeadZoneTime.byChxPolarityS3S2   = DB_POL_B;                 //通道A和通道B延时输出电平是否反向
//	tGptbDeadZoneTime.byChxInselS5S4      = DBCHAIN_AR_AF;            //PWMA作为上升沿和下降沿延时处理的输入信号
//	tGptbDeadZoneTime.byChxOutSwapS8S7    = DBCHAOUT_OUTA_B_OUTB_A;   //OUTA=通道A输出，OUTB=通道B输出
//    csi_gptb_channelmode_config(GPTB3,&tGptbDeadZoneTime,GPTB_CHANNEL_A);
//	csi_gptb_channelmode_config(GPTB4,&tGptbDeadZoneTime,GPTB_CHANNEL_A);
//	csi_gptb_channelmode_config(GPTB5,&tGptbDeadZoneTime,GPTB_CHANNEL_A);
////------------------------------------------------------------------------------------------------------------------------
//    csi_gptb_emergency_config_t   tEptEmergencyCfg;           //紧急状态输入
//    tEptEmergencyCfg.byEpxInt    = EBI1 ;                     //EPx选择外部IO端口（EBI0~EBI4）
//    tEptEmergencyCfg.byPolEbix   = EBI_POL_L;//EBI_POL_H;     //EBIx的输入有效极性选择控制
//	tEptEmergencyCfg.byEpx       = EP1;                       //使能EPx
//	tEptEmergencyCfg.byEpxLckmd  = EP_HLCK;                   //使能 锁
//    tEptEmergencyCfg.byFltpace0  = EPFLT0_2P;                 //EP0、EP1、EP2和EP3的数字去抖滤波检查周期数
//	tEptEmergencyCfg.byFltpace1  = EPFLT1_2P;                 //EP4、EP5、EP6和EP7的数字去抖滤波检查周期数
//	if(tEptEmergencyCfg.byEpxInt ==ORL0){tEptEmergencyCfg.byOrl0 = ORLx_EP0 |ORLx_EP1|ORLx_EP2;}
//	if(tEptEmergencyCfg.byEpxInt ==ORL1){tEptEmergencyCfg.byOrl1 = ORLx_EP4 |ORLx_EP5|ORLx_EP6;}
//	csi_gptb_emergency_cfg(GPTB3,&tEptEmergencyCfg);
//	csi_gptb_emergency_cfg(GPTB4,&tEptEmergencyCfg);
//	csi_gptb_emergency_cfg(GPTB5,&tEptEmergencyCfg);
//	
//	csi_gptb_emergency_pinxout(GPTB3,GPTBCHAX,EM_OUT_L);       //紧急状态下输出状态设置（注意mos/igbt的驱动电平）
//	csi_gptb_emergency_pinxout(GPTB3,GPTBCHAY,EM_OUT_L);
//	csi_gptb_emergency_pinxout(GPTB4,GPTBCHAX,EM_OUT_L);       //
//	csi_gptb_emergency_pinxout(GPTB4,GPTBCHAY,EM_OUT_L);
//	csi_gptb_emergency_pinxout(GPTB5,GPTBCHAX,EM_OUT_L);       //
//	csi_gptb_emergency_pinxout(GPTB5,GPTBCHAY,EM_OUT_L);

//	csi_gptb_emergency_interruption_en(GPTB3,EM_INT_EP1);      //紧急状态输入中断使能

////------------------------------------------------------------------------------------------------------------------------
//    csi_gptb_Global_load_control_config_t  Globaldemo;
//	Globaldemo.bGlden    =   DISABLE;                           //全局的Shadow到Active寄存器载入控制使能
//	Globaldemo.byGldmd   =   GPTB_LDGLD_ZRO_PRD_LD_SYNC;        //全局载入触发事件选择
//	Globaldemo.bOstmd    =   GPTB_LDMD_ANYTIME;                 //One Shot 载入模式使能控制位
//	Globaldemo.bGldprd   =   0 ;                                //全局载入触发周期选择。可以选择N次触发条件满足后，才进行一次全局载入。
//	if(Globaldemo.bGlden==ENABLE)
//	{Globaldemo.bPrdr    =ENABLE;
//     Globaldemo.bCmpa    =ENABLE;
//     Globaldemo.bCmpb    =ENABLE;
//     Globaldemo.bAqcra   =ENABLE;
//	 Globaldemo.bAqcrb   =ENABLE;
//	 Globaldemo.bAqcsf   =ENABLE;	 
//	}					   
//	csi_gptb_global_config(GPTB3,&Globaldemo);
//	csi_gptb_global_rearm(GPTB3) ;                              //重置ONE SHOT模式。ONE SHOT模式下，一次触发后，需要重置模式才允许再次触发
//	csi_gptb_global_sw(GPTB3) ;                                 //软件产生一次GLD触发
////------------------------------------------------------------------------------------------------------------------------
//    csi_gptb_set_sync(        GPTB3, GPTB_TRGIN_SYNCEN3, GPTB_TRG_CONTINU, GPTB_AUTO_REARM_ZRO_PRD);  //使能触发输入
//	csi_gptb_set_extsync_chnl(GPTB3, GPTB_TRGIN_SYNCEN3, GPTB_TRGO_0_SEL);                            //配置触发3到输出0
//	csi_gptb_set_evtrg(GPTB3, GPTB_TRG_OUT0, GPTB_TRGSRC_EX);                                         //触发3通过GPTB_TRG_OUT0输出
////------------------------------------------------------------------------------------------------------------------------
//    csi_gptb_feglk_config_t  FEGLKcfg;                                                                    //
//	FEGLKcfg.byPrdr	   = 8;                                                                                 //0x0  不链接
//	FEGLKcfg.byRssr    = 8;																                    //0x1  GPTA0
//	FEGLKcfg.byCmpa    = 0;																					//0x2  GPTA1
//	FEGLKcfg.byCmpb    = 0;																					//0x3  GPTA2
//	FEGLKcfg.byGld2    = 0;																					//0x4  GPTA3
//	FEGLKcfg.byEmslclr = 8;																					//0x5  GPTB0
//	FEGLKcfg.byEmhlclr = 8;																					//0x6  GPTB1
//	FEGLKcfg.byEmicr   = 0;																					//0x7  GPTB2
//	FEGLKcfg.byEmfrcr  = 0;																					//0x8  GPTB3
//	FEGLKcfg.byAqosf   = 8;																					//0x9  GPTB4
//	FEGLKcfg.byAqcsf   = 0;  																				//0xa  GPTB5
//    csi_gptb_feglk_config(GPTB4,&FEGLKcfg);
//	csi_gptb_feglk_config(GPTB5,&FEGLKcfg);                                                                 //GPTB4,5 由GPTB3控制
////------------------------------------------------------------------------------------------------------------------------
	csi_gptb_start(GPTB0);//start  timer
	csi_gptb_start(GPTB1);//start  timer
	csi_gptb_start(GPTB2);
	csi_gptb_start(GPTB3);//start  timer
	csi_gptb_start(GPTB4);//start  timer
	csi_gptb_start(GPTB5);//start  timer
////------------------------------------------------------------------------------------------------------------------------	
	while(1){	
//		    csi_gptb_change_ch_duty(GPTB3,GPTB_CH_A, 25);
//		    csi_gptb_change_ch_duty(GPTB4,GPTB_CH_A, 25);
//            csi_gptb_change_ch_duty(GPTB5,GPTB_CH_A, 25);
		    mdelay(200);                        
//		    csi_gptb_change_ch_duty(GPTB3,GPTB_CH_A, 50);
//            csi_gptb_change_ch_duty(GPTB4,GPTB_CH_A, 50);
//			csi_gptb_change_ch_duty(GPTB5,GPTB_CH_A, 50);	
		    mdelay(200);	
	}
	return iRet;
}


