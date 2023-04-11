/***********************************************************************//** 
 * \file  gpta_test.c
 * \brief  GPTA_TEST description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-3-21 <td>V1.0 <td>YYM     <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include <gpta.h>
#include <drv/pin.h>
#include "drv/etb.h"

//#include "test.h"
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

/** \brief gpta timer
 * 
 *  \param[in] none
 *  \return error code
 */
int gpta_timer_test(void)
{
	int iRet = 0;
	
	csi_gpta_timer_init(GPTA0, 10000);		//初始化GPTA0, 定时10000us； GPTA定时，默认采用向上计数，PEND中断
	
	csi_gpta_start(GPTA0);                  //启动定时器

	return iRet;	
}

/** \brief GPTA sync2 sync3合并捕获示例代码
 *          //sync2 sync3不区分，实现4次捕获
 *   		- 捕获4次产生一次捕获中断
 *     		- 由PA01触发外部事件1，经过ETCB  触发sync3 捕获
 * 			- 信号由PA01的高低电平切换产生（一直高电平意味着没有触发）
 *  \param[in] none
 *  \return error code
 */

int gpta_capture_sync_test(void)
{
	int iRet = 0;	
    volatile uint8_t ch;


	csi_pin_set_mux(PA1,PA1_INPUT);		
	csi_pin_pull_mode(PA1, GPIO_PULLUP);						//PA0 上拉
	csi_pin_irq_mode(PA1, EXI_GRP16, GPIO_IRQ_FALLING_EDGE);     //PA0 上升沿产生中断，选择中断组16
	csi_pin_irq_enable(PA1, ENABLE);                            //PA0 中断使能                              //PC02 中断使能        
	csi_exi_set_evtrg(5, TRGSRC_EXI16, 1);	 
	
//------------------------------------------------------------------------------------------------------------------------		
	csi_etb_config_t tEtbConfig;				//ETB 参数配置结构体	
	tEtbConfig.byChType  = ETB_ONE_TRG_ONE;  	//单个源触发单个目标
	tEtbConfig.bySrcIp   = ETB_EXI_TRGOUT5 ;  	//...作为触发源
	tEtbConfig.byDstIp   =  ETB_GPTA0_SYNCIN3;  //GPTB0 同步输入2作为目标事件
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	//自动获取空闲通道号,ch >= 0 获取成功						//ch < 0,则获取通道号失败		
	iRet = csi_etb_ch_config(ch, &tEtbConfig);	
//------------------------------------------------------------------------------------------------------------------------	
	csi_gpta_captureconfig_t tPwmCfg;								  
	tPwmCfg.byWorkmod       = GPTA_CAPTURE;                     //WAVE or CAPTURE    //计数或捕获	
	tPwmCfg.byCountingMode  = GPTA_UPCNT;                       //CNYMD  //计数方向
	tPwmCfg.byOneshotMode    = GPTA_OP_CONT; 
	tPwmCfg.byStartSrc      = GPTA_SYNC_START;				    //软件使能同步触发使能控制（RSSR中START控制位）//启动方式
	tPwmCfg.byPscld         = GPTA_LDPSCR_ZRO;                  //PSCR(分频)活动寄存器载入控制。活动寄存器在配置条件满足时，从影子寄存器载入更新值	
	tPwmCfg.byCaptureCapmd   = 0;                               //0:连续捕捉模式    1h：一次性捕捉模式
	tPwmCfg.byCaptureStopWrap=4-1;                              //Capture模式下，捕获事件计数器周期设置值
	tPwmCfg.byCaptureLdaret   =0;                                //CMPA捕捉载入后，计数器值计数状态控制位(1h：CMPA触发后，计数器值进行重置;0h：CMPA触发后，计数器值不进行重置)
	tPwmCfg.byCaptureLdbret   =0; 
	tPwmCfg.byCaptureLdaaret  =0;  
	tPwmCfg.byCaptureLdbaret  =1;  
	tPwmCfg.byCaptureCapmdSel = GPTA_CMPMD_SEL_NODIFF;
                                                        	
	tPwmCfg.wInt 		 =GPTA_INTSRC_CAPLD3;                   //interrupt//
		
	csi_gpta_capture_init(GPTA0, &tPwmCfg);

//------------------------------------------------------------------------------------------------------------------------
    csi_gpta_set_sync(GPTA0, GPTA_TRG_SYNCEN3, GPTA_TRG_CONTINU, GPTA_AUTO_REARM_ZRO);//使能SYNCIN2外部触发
	csi_gpta_start(GPTA0);//start  timer
    while(1){		
		  		      
		    mdelay(200);                        
		    	
		    mdelay(200);
	}			
	return iRet;
};

/** \brief GPTA sync2 sync3区分捕获示例代码
 *          //sync2 sync3区分，实现2次捕获
 *   		- 捕获2次产生一次捕获中断
 *     		- 由PA3产生外部事件3，经过ETCB  触发sync2 捕获
 *          - 由PA3外部扩展口产生外部事件5，经过ETCB  触发sync3 捕获
 * 			- 信号由PA01的高低电平切换产生（一直高电平意味着没有触发）
 *  \param[in] none
 *  \return error code
 */

int gpta_capture_sync_test1(void)
{
	int iRet = 0;	
    volatile uint8_t ch;
	
	csi_pin_set_mux(PA3,PA3_INPUT);		
	csi_pin_pull_mode(PA3, GPIO_PULLUP);						//PA0 上拉
	
	csi_pin_irq_mode(PA3,EXI_GRP3, GPIO_IRQ_RISING_EDGE);		//PA0 下降沿产生中断
	csi_pin_irq_enable(PA3, ENABLE);	
	csi_exi_set_evtrg(0, TRGSRC_EXI3, 1);	

	csi_pin_irq_mode(PA3, EXI_GRP16, GPIO_IRQ_FALLING_EDGE);     //PA0 上升沿产生中断，选择中断组16
	csi_pin_irq_enable(PA3, ENABLE);                            //PA0 中断使能                              //PC02 中断使能        
	csi_exi_set_evtrg(5, TRGSRC_EXI16, 1);	   
	
//------------------------------------------------------------------------------------------------------------------------		
	csi_etb_config_t tEtbConfig;				//ETB 参数配置结构体	
	tEtbConfig.byChType  = ETB_ONE_TRG_ONE;  	//单个源触发单个目标
	tEtbConfig.bySrcIp   = ETB_EXI_TRGOUT0 ;  	//...作为触发源
	tEtbConfig.byDstIp   =  ETB_GPTA0_SYNCIN2;  //GPTA0 同步输入2作为目标事件
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	//自动获取空闲通道号,ch >= 0 获取成功						//ch < 0,则获取通道号失败		
	iRet = csi_etb_ch_config(ch, &tEtbConfig);	
	
//------------------------------------------------------------------------------------------------------------------------		
	tEtbConfig.byChType  = ETB_ONE_TRG_ONE;  	//单个源触发单个目标
	tEtbConfig.bySrcIp   = ETB_EXI_TRGOUT5 ;  	//...作为触发源
	tEtbConfig.byDstIp   =  ETB_GPTA0_SYNCIN3;  //GPTA0 同步输入3作为目标事件
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	//自动获取空闲通道号,ch >= 0 获取成功						//ch < 0,则获取通道号失败		
	iRet = csi_etb_ch_config(ch, &tEtbConfig);		
	
//------------------------------------------------------------------------------------------------------------------------	
	csi_gpta_captureconfig_t tPwmCfg;								  
	tPwmCfg.byWorkmod       = GPTA_CAPTURE;                     //WAVE or CAPTURE    //计数或捕获	
	tPwmCfg.byCountingMode  = GPTA_UPCNT;                       //CNYMD  //计数方向
	tPwmCfg.byOneshotMode    = GPTA_OP_CONT; 
	tPwmCfg.byStartSrc      = GPTA_SYNC_START;				    //软件使能同步触发使能控制（RSSR中START控制位）//启动方式
	tPwmCfg.byPscld         = GPTA_LDPSCR_ZRO;                  //PSCR(分频)活动寄存器载入控制。活动寄存器在配置条件满足时，从影子寄存器载入更新值	
	tPwmCfg.byCaptureCapmd   = 0;                               //0:连续捕捉模式    1h：一次性捕捉模式
	tPwmCfg.byCaptureStopWrap=2-1;                              //Capture模式下，捕获事件计数器周期设置值
	tPwmCfg.byCaptureLdaret   =0;                                //CMPA捕捉载入后，计数器值计数状态控制位(1h：CMPA触发后，计数器值进行重置;0h：CMPA触发后，计数器值不进行重置)
	tPwmCfg.byCaptureLdbret   =1; 
	tPwmCfg.byCaptureLdaaret  =0;  
	tPwmCfg.byCaptureLdbaret  =0;  
	tPwmCfg.byCaptureCapmdSel = GPTA_CMPMD_SEL_DIFF;
                                                        	
	tPwmCfg.wInt 		 = GPTA_INTSRC_PEND;                   //interrupt//
		
	csi_gpta_capture_init(GPTA0, &tPwmCfg);

//------------------------------------------------------------------------------------------------------------------------
    csi_gpta_set_sync(GPTA0, GPTA_TRG_SYNCEN2, GPTA_TRG_CONTINU, GPTA_AUTO_REARM_ZRO);//使能SYNCIN2外部触发
	csi_gpta_set_sync(GPTA0, GPTA_TRG_SYNCEN3, GPTA_TRG_CONTINU, GPTA_AUTO_REARM_ZRO);//使能SYNCIN3外部触发
	csi_gpta_start(GPTA0);//start  timer
    while(1){		
		  		      
		    mdelay(200);                        
		    	
		    mdelay(200);
	}			
	return iRet;
};

/** \brief GPTA基本的波形输出示例代码
 *          PWMA在50%和20%之间切换
 *  \param[in] none
 *  \return error code
 */
int gpta_pwm_test(void)
{
	int iRet = 0;	
//------------------------------------------------------------------------------------------------------------------------	
	csi_pin_set_mux(PA0, PA0_GPTA0_CHA);						//
	csi_pin_set_mux(PA1, PA1_GPTA0_CHB);						//
//------------------------------------------------------------------------------------------------------------------------	
	csi_gpta_pwmconfig_t tPwmCfg;								  
	tPwmCfg.byWorkmod        = GPTA_WAVE;                        //WAVE  波形模式
	tPwmCfg.byCountingMode   = GPTA_UPCNT;                       //CNYMD  //计数方向
	tPwmCfg.byOneshotMode    = GPTA_OP_CONT;                     //OPM    //单次或连续(工作方式)
	tPwmCfg.byStartSrc       = GPTA_SYNC_START;					 //软件使能同步触发使能控制（RSSR中START控制位）//启动方式
	tPwmCfg.byPscld          = GPTA_LDPSCR_ZRO;                  //PSCR(分频)活动寄存器载入控制。活动寄存器在配置条件满足时，从影子寄存器载入更新值		
	tPwmCfg.byDutyCycle 	 = 50;								 //pwm ouput duty cycle//PWM初始值(X%)			
	tPwmCfg.wFreq 			 = 1000;							 //pwm ouput frequency	
	tPwmCfg.wInt 		 	 = GPTA_INTSRC_CBU;                     //interrupt
	csi_gpta_wave_init(GPTA0, &tPwmCfg);
	
//------------------------------------------------------------------------------------------------------------------------

	csi_gpta_pwmchannel_config_t  tEptchannelCfg;
	tEptchannelCfg.byActionZro    =   GPTA_LO;
	tEptchannelCfg.byActionPrd    =   GPTA_NA;
	tEptchannelCfg.byActionC1u    =   GPTA_HI;
	tEptchannelCfg.byActionC1d    =   GPTA_LO;
	tEptchannelCfg.byActionC2u    =   GPTA_HI;
	tEptchannelCfg.byActionC2d    =   GPTA_LO;
	tEptchannelCfg.byActionT1u    =   GPTA_LO;
	tEptchannelCfg.byActionT1d    =   GPTA_LO;
	tEptchannelCfg.byActionT2u    =   GPTA_NA;
	tEptchannelCfg.byActionT2d    =   GPTA_NA;
	tEptchannelCfg.byChoiceC1sel  =   GPTA_CMPA;
	tEptchannelCfg.byChoiceC2sel  =   GPTA_CMPA;	
	csi_gpta_channel_config(GPTA0, &tEptchannelCfg,  GPTA_CHANNEL_1);//channel
	tEptchannelCfg.byChoiceC1sel  =   GPTA_CMPB;
	tEptchannelCfg.byChoiceC2sel  =   GPTA_CMPB;
	csi_gpta_channel_config(GPTA0, &tEptchannelCfg,  GPTA_CHANNEL_2);

	csi_gpta_start(GPTA0);
//------------------------------------------------------------------------------------------------------------------------
	while(1){	
			csi_gpta_change_ch_duty(GPTA0,GPTA_CAMPA, 20);
	        csi_gpta_change_ch_duty(GPTA0,GPTA_CAMPB, 20);
		    mdelay(200);                        
			csi_gpta_change_ch_duty(GPTA0,GPTA_CAMPA, 50);
	        csi_gpta_change_ch_duty(GPTA0,GPTA_CAMPB, 50);
		    mdelay(200);	
	}	

    return iRet;
}

/** \brief GPTA波形强制输出demo
 *          包含一次性强制性输出和连续强制输出
 *  \param[in] none
 *  \return error code
 */
int gpta_pwm_waveform_test(void)
{
	int iRet = 0;	
//------------------------------------------------------------------------------------------------------------------------	
	csi_pin_set_mux(PA0, PA0_GPTA0_CHA);						//
	csi_pin_set_mux(PA1, PA1_GPTA0_CHB);						//
	
//------------------------------------------------------------------------------------------------------------------------	
	csi_gpta_pwmconfig_t tPwmCfg;								  
	tPwmCfg.byWorkmod        = GPTA_WAVE;                        //WAVE  波形模式
	tPwmCfg.byCountingMode   = GPTA_UPDNCNT;                     //CNYMD  //计数方向
	tPwmCfg.byOneshotMode    = GPTA_OP_CONT;                     //OPM    //单次或连续(工作方式)
	tPwmCfg.byStartSrc       = GPTA_SYNC_START;					 //软件使能同步触发使能控制（RSSR中START控制位）//启动方式
	tPwmCfg.byPscld          = GPTA_LDPSCR_ZRO;                  //PSCR(分频)活动寄存器载入控制。活动寄存器在配置条件满足时，从影子寄存器载入更新值		
	tPwmCfg.byDutyCycle 	 = 50;								 //pwm ouput duty cycle//PWM初始值(X%)			
	tPwmCfg.wFreq 			 = 1000;							 //pwm ouput frequency	
	tPwmCfg.wInt 		 	 = GPTA_INTSRC_CBU;                     //interrupt
	csi_gpta_wave_init(GPTA0, &tPwmCfg);
//------------------------------------------------------------------------------------------------------------------------
	
	csi_gpta_pwmchannel_config_t  tEptchannelCfg;
	tEptchannelCfg.byActionZro    =   GPTA_LO;
	tEptchannelCfg.byActionPrd    =   GPTA_NA;
	tEptchannelCfg.byActionC1u    =   GPTA_HI;
	tEptchannelCfg.byActionC1d    =   GPTA_LO;
	tEptchannelCfg.byActionC2u    =   GPTA_HI;
	tEptchannelCfg.byActionC2d    =   GPTA_LO;
	tEptchannelCfg.byActionT1u    =   GPTA_LO;
	tEptchannelCfg.byActionT1d    =   GPTA_LO;
	tEptchannelCfg.byActionT2u    =   GPTA_NA;
	tEptchannelCfg.byActionT2d    =   GPTA_NA;
	tEptchannelCfg.byChoiceC1sel  =   GPTA_CMPA;
	tEptchannelCfg.byChoiceC2sel  =   GPTA_CMPA;	
	csi_gpta_channel_config(GPTA0, &tEptchannelCfg,  GPTA_CHANNEL_1);//channel
	tEptchannelCfg.byChoiceC1sel  =   GPTA_CMPB;
	tEptchannelCfg.byChoiceC2sel  =   GPTA_CMPB;
	csi_gpta_channel_config(GPTA0, &tEptchannelCfg,  GPTA_CHANNEL_2);

//------------------------------------------------------------------------------------------------------------------------
	csi_gpta_start(GPTA0);
//------------------------------------------------------------------------------------------------------------------------
	 while(1){		
		  	csi_gpta_change_ch_duty(GPTA0,GPTA_CAMPA, 20);
	        csi_gpta_change_ch_duty(GPTA0,GPTA_CAMPB, 20);
		    mdelay(10);

            csi_gpta_onetimesoftware_output(GPTA0,GPTA_OSTSF1,GPTA_HI);
            mdelay(2);
			
		    csi_gpta_change_ch_duty(GPTA0,GPTA_CAMPA, 50);
	        csi_gpta_change_ch_duty(GPTA0,GPTA_CAMPB, 50);
		    mdelay(10);
			
			csi_gpta_aqcsfload_config(GPTA0, GPTA_AQCSF_NOW);
			csi_gpta_continuous_software_waveform(GPTA0, GPTA_CHANNEL_1, GPTA_AQCSF_L);
			mdelay(10);
			csi_gpta_continuous_software_waveform(GPTA0, GPTA_CHANNEL_1, GPTA_AQCSF_NONE);
	}			
    return iRet;
}

//================================================================================== 
//
//GPTA链接代码实例
//
//通过GPTA1链接GPTA0,实现波形的输出
//
//==================================================================================
int gpta_reglk_test(void)
{
	int iRet = 0;	
//------------------------------------------------------------------------------------------------------------------------	
	csi_pin_set_mux(PA0, PA0_GPTA0_CHA);						//
	csi_pin_set_mux(PA1, PA1_GPTA0_CHB);						//
	
//------------------------------------------------------------------------------------------------------------------------	
    csi_gpta_channel_cmpload_config(GPTA0, GPTA_CMPLD_SHDW, GPTA_LDCMP_ZRO ,GPTA_CAMPA);
	csi_gpta_channel_cmpload_config(GPTA0, GPTA_CMPLD_SHDW, GPTA_LDCMP_ZRO ,GPTA_CAMPB);
	csi_gpta_pwmconfig_t tPwmCfg;								  
	tPwmCfg.byWorkmod        = GPTA_WAVE;                        //WAVE  波形模式
	tPwmCfg.byCountingMode   = GPTA_UPCNT;                     //CNYMD  //计数方向
	tPwmCfg.byOneshotMode    = GPTA_OP_CONT;                     //OPM    //单次或连续(工作方式)
	tPwmCfg.byStartSrc       = GPTA_SYNC_START;					 //软件使能同步触发使能控制（RSSR中START控制位）//启动方式
	tPwmCfg.byPscld          = GPTA_LDPSCR_ZRO;                  //PSCR(分频)活动寄存器载入控制。活动寄存器在配置条件满足时，从影子寄存器载入更新值		
	tPwmCfg.byDutyCycle 	 = 50;								 //pwm ouput duty cycle//PWM初始值(X%)			
	tPwmCfg.wFreq 			 = 1000;							 //pwm ouput frequency	
	tPwmCfg.wInt 		 	 = GPTA_INT_CBU;                     //interrupt
	csi_gpta_wave_init(GPTA0, &tPwmCfg);
	

	csi_gpta_pwmchannel_config_t  tEptchannelCfg;
	tEptchannelCfg.byActionZro    =   GPTA_LO;
	tEptchannelCfg.byActionPrd    =   GPTA_NA;
	tEptchannelCfg.byActionC1u    =   GPTA_HI;
	tEptchannelCfg.byActionC1d    =   GPTA_LO;
	tEptchannelCfg.byActionC2u    =   GPTA_HI;
	tEptchannelCfg.byActionC2d    =   GPTA_LO;
	tEptchannelCfg.byActionT1u    =   GPTA_LO;
	tEptchannelCfg.byActionT1d    =   GPTA_LO;
	tEptchannelCfg.byActionT2u    =   GPTA_NA;
	tEptchannelCfg.byActionT2d    =   GPTA_NA;
	tEptchannelCfg.byChoiceC1sel  =   GPTA_CMPA;
	tEptchannelCfg.byChoiceC2sel  =   GPTA_CMPA;	
	csi_gpta_channel_config(GPTA0, &tEptchannelCfg,  GPTA_CHANNEL_1);//channel
	tEptchannelCfg.byChoiceC1sel  =   GPTA_CMPB;
	tEptchannelCfg.byChoiceC2sel  =   GPTA_CMPB;
	csi_gpta_channel_config(GPTA0, &tEptchannelCfg,  GPTA_CHANNEL_2);
	
	
//------------------------------------------------------------------------------------------------------------------------
    csi_gpta_feglk_config_t  FEGLKcfg;                                                                    
	FEGLKcfg.byPrdr	   = 2;                                                              //0x0 不链接, 0x1 EPT0 ,0x2 GPTA1
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
//------------------------------------------------------------------------------------------------------------------------
	
	csi_pin_set_mux(PA2, PA2_GPTA1_CHA);						//
	csi_pin_set_mux(PA3, PA3_GPTA1_CHB);						//
							  
	tPwmCfg.byWorkmod        = GPTA_WAVE;                        //WAVE  波形模式
	tPwmCfg.byCountingMode   = GPTA_UPCNT;                       //CNYMD  //计数方向
	tPwmCfg.byOneshotMode    = GPTA_OP_CONT;                     //OPM    //单次或连续(工作方式)
	tPwmCfg.byStartSrc       = GPTA_SYNC_START;					 //软件使能同步触发使能控制（RSSR中START控制位）//启动方式
	tPwmCfg.byPscld          = GPTA_LDPSCR_ZRO;                  //PSCR(分频)活动寄存器载入控制。活动寄存器在配置条件满足时，从影子寄存器载入更新值		
	tPwmCfg.byDutyCycle 	 = 50;								 //pwm ouput duty cycle//PWM初始值(X%)			
	tPwmCfg.wFreq 			 = 1000;							 //pwm ouput frequency	
	tPwmCfg.wInt 		 	 = GPTA_INTSRC_CBU;                     //interrupt
	csi_gpta_wave_init(GPTA1, &tPwmCfg);
	
//------------------------------------------------------------------------------------------------------------------------

	tEptchannelCfg.byActionZro    =   GPTA_LO;
	tEptchannelCfg.byActionPrd    =   GPTA_NA;
	tEptchannelCfg.byActionC1u    =   GPTA_HI;
	tEptchannelCfg.byActionC1d    =   GPTA_LO;
	tEptchannelCfg.byActionC2u    =   GPTA_HI;
	tEptchannelCfg.byActionC2d    =   GPTA_LO;
	tEptchannelCfg.byActionT1u    =   GPTA_LO;
	tEptchannelCfg.byActionT1d    =   GPTA_LO;
	tEptchannelCfg.byActionT2u    =   GPTA_NA;
	tEptchannelCfg.byActionT2d    =   GPTA_NA;
	tEptchannelCfg.byChoiceC1sel  =   GPTA_CMPA;
	tEptchannelCfg.byChoiceC2sel  =   GPTA_CMPA;	
	csi_gpta_channel_config(GPTA1, &tEptchannelCfg,  GPTA_CHANNEL_1);//channel
	tEptchannelCfg.byChoiceC1sel  =   GPTA_CMPB;
	tEptchannelCfg.byChoiceC2sel  =   GPTA_CMPB;
	csi_gpta_channel_config(GPTA1, &tEptchannelCfg,  GPTA_CHANNEL_2);

	csi_gpta_start(GPTA1);	
	while(1){	
			csi_gpta_change_ch_duty(GPTA1,GPTA_CAMPA, 20);
	        csi_gpta_change_ch_duty(GPTA1,GPTA_CAMPB, 20);
		    mdelay(200);                        
			csi_gpta_change_ch_duty(GPTA1,GPTA_CAMPA, 50);
	        csi_gpta_change_ch_duty(GPTA1,GPTA_CAMPB, 50);
		    mdelay(200);	
	}	
	return iRet;	
	
}