/***********************************************************************//** 
 * \file  timer_demo.c
 * \brief  TIMER_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-11 <td>V0.0 <td>ZJY     <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include <drv/bt.h>
#include <drv/pin.h>
#include <drv/etb.h>
#include "demo.h"
#include <csi_drv.h>

/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

/** \brief bt timer:BT0做基本定时器功能，周期结束产生中断，并在中断里面翻转IO（需要打开PA6IO配置注释）
 * 
 *  \param[in] none
 *  \return error code
 */
int bt_timer_demo(void)
{
	int iRet = 0;
	
//	csi_pin_set_mux(PA6, PA6_OUTPUT);	//PA6 output ，并在BT中断里面翻转IO
//	csi_pin_set_high(PA6);				//PA6 output high;
	
	csi_bt_timer_init(BT0, 1000);		//初始化BT0:BT0定时1000 us，默认采用PEND中断
	csi_bt_start(BT0);					//启动BT0定时器
	
	while(1)
	{
		NOP;
	}
	return iRet;
}

/** \brief bt pwm ouput:BT1 PWM输出
 *  
 *  \param[in] none
 *  \return error code
 */
int bt_pwm_demo(void)
{
	int iRet = 0;
	csi_bt_pwm_config_t tPwmCfg;							//BT PWM输出参数初始化配置结构体
	
//	csi_pin_set_mux(PA1,  PA1_BT1_OUT);					    //PA1  作为BT1 PWM输出引脚
//	csi_pin_set_mux(PB14, PB14_BT1_OUT);					//PB14 作为BT1 PWM输出引脚
	csi_pin_set_mux(PC11, PC11_BT1_OUT);					//PC11 作为BT1 PWM输出引脚
	
	//init timer pwm para config
	tPwmCfg.byIdleLevel = BT_PWM_IDLE_HIGH;					//PWM 输出空闲电平
	tPwmCfg.byStartLevel= BT_PWM_START_HIGH;				//PWM 输出起始电平
	tPwmCfg.byDutyCycle = 90;								//PWM 输出占空比(0 < DutyCycle < 100)		
	tPwmCfg.wFreq 		= 50;								//PWM 输出频率
	//tPwmCfg.byInt   	= BT_INTSRC_PEND | BT_INTSRC_CMP;	//PWM 中断配置(PEND and CMP)
	tPwmCfg.byInt  		= BT_INTSRC_NONE;
	
	csi_bt_pwm_init(BT1, &tPwmCfg);							//初始化BT1 PWM输出
	csi_bt_start(BT1);										//启动BT1
	
	while(1)
	{
		NOP;
	}
	
	return iRet;
}

/** \brief bt sync trg start ：PB1外部下降沿中断触发BT0工作
 *  
 *  \param[in] none
 *  \return error code
 */
int bt_sync_trg_start_demo(void)
{
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				               			//ETB 参数配置结构体		
	
	csi_pin_set_mux(PB1, PB1_INPUT);									//PB1 配置为输入
	csi_pin_pull_mode(PB1, GPIO_PULLUP);								//PB1 上拉
	csi_pin_irq_mode(PB1, EXI_GRP1, GPIO_IRQ_FALLING_EDGE);			    //PB1 下降沿产生中断，选择中断组1
	csi_pin_irq_enable(PB1,ENABLE);										//PB1 中断使能	
	csi_exi_set_evtrg(1, TRGSRC_EXI1, 0);						        //EXI1 触发EXI_TRGOUT1
	
//	csi_pin_set_mux(PA6, PA6_OUTPUT);									//PA6 output ，并在BT0中断里面翻转IO
//	csi_pin_set_high(PA6);												//PA6 output high;		
	
	csi_bt_timer_init(BT0,1000);					//BT0 定时
	csi_bt_set_sync(BT0, BT_TRGIN_SYNCEN0, BT_TRG_CONTINU, BT_TRG_SYCAREARM);	//外部触发bt0启动(SYNCIN0)
	
//	csi_bt_timer_init(BT1,2000);					//BT1 定时
//	csi_bt_set_sync(BT1, BT_TRGIN_SYNCEN0, BT_TRG_ONCE, BT_TRG_AUTOAREARM);	//外部触发bt1启动(SYNCIN0)
	
//	csi_bt_timer_init(BT2,3000);					//BT2 定时
//	csi_bt_set_sync(BT2, BT_TRGIN_SYNCEN0, BT_TRG_ONCE, BT_TRG_AUTOAREARM);	//外部触发bt2启动(SYNCIN0)
	
//	csi_bt_timer_init(BT3,4000);					//BT3 定时
//	csi_bt_set_sync(BT3, BT_TRGIN_SYNCEN0, BT_TRG_ONCE, BT_TRG_AUTOAREARM);	//外部触发bt3启动(SYNCIN0)
	
	// ETCB 初始化
	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  		//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_EXI_TRGOUT1;  	    //EXI_TRGOUT1作为触发源
	tEtbConfig.byDstIp =  ETB_BT0_SYNCIN0;   	    //BT0 同步输入作为目标事件
//	tEtbConfig.byDstIp =  ETB_BT1_SYNCIN0;   	    //BT1 同步输入作为目标事件
//	tEtbConfig.byDstIp =  ETB_BT2_SYNCIN0;   	    //BT2 同步输入作为目标事件
//	tEtbConfig.byDstIp =  ETB_BT3_SYNCIN0;   	    //BT3 同步输入作为目标事件	
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
	while(1)
	{
		NOP;
	}
	return iRet;
}

/** \brief bt sync trg count: BT0 输出PWM触发EXI脚PB1，再通过BT1同步输入端口，触发BT1计数值加1
 *                            BT0的PWM输出脚PA0要和PB1脚连接在一起
 *  \param[in] none
 *  \return error code
 */
int bt_sync_trg_count_demo(void)
{
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				            //ETB 参数配置结构体
	csi_bt_pwm_config_t tPwmCfg;							//BT PWM输出参数初始化配置结构体
	
	csi_pin_set_mux(PB1, PB1_INPUT);						//PB1 配置为输入
	csi_pin_pull_mode(PB1, GPIO_PULLUP);					//PB1 上拉
	csi_pin_irq_mode(PB1, EXI_GRP1, GPIO_IRQ_FALLING_EDGE);	//PB1 下降沿产生中断，选择中断组1
	csi_pin_irq_enable(PB1,ENABLE);				//PB1 中断使能
	csi_exi_set_evtrg(1, TRGSRC_EXI1, 1);					//EXI1(PB1) 触发EXI_TRGOUT1(PB1用EXI1触发输出)
	
//	csi_pin_set_mux(PA6, PA6_OUTPUT);		                //PA6 output ，并在BT中断里面翻转IO
//	csi_pin_set_high(PA6);					                //PA6 output high;		

	//BT1 初始化
	csi_bt_timer_init(BT1,10);								//BT1定时
	csi_bt_set_sync(BT1, BT_TRGIN_SYNCEN2, BT_TRG_CONTINU, BT_TRG_AUTOAREARM);		//外部触发BT1计数(SYNCIN2)
	
	// ETCB 初始化
	tEtbConfig.byChType  = ETB_ONE_TRG_ONE;  		//单个源触发单个目标
	tEtbConfig.bySrcIp   = ETB_EXI_TRGOUT1;  	    //EXI_TRGOUT1作为触发源
	tEtbConfig.byDstIp   = ETB_BT1_SYNCIN2;   	    //BT1 同步输入作为目标事件
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
	
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	            //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								   		    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
	// BT0 初始化
	csi_pin_set_mux(PA0,  PA0_BT0_OUT);					    //PA0  作为BT0 PWM输出引脚
	//init timer pwm para config
	tPwmCfg.byIdleLevel = BT_PWM_IDLE_HIGH;					//PWM 输出空闲电平
	tPwmCfg.byStartLevel= BT_PWM_START_HIGH;				//PWM 输出起始电平
	tPwmCfg.byDutyCycle = 50;								//PWM 输出占空比(0 < DutyCycle < 100)		
	tPwmCfg.wFreq 		= 1000;								//PWM 输出频率
	//tPwmCfg.byInt   	= BT_INTSRC_PEND | BT_INTSRC_CMP;	//PWM 中断配置(PEND and CMP)
	tPwmCfg.byInt  		= BT_INTSRC_NONE;	
	csi_bt_pwm_init(BT0, &tPwmCfg);							//初始化BT0 PWM输出
	
	csi_bt_start(BT0);	     //启动BT0
	csi_bt_start(BT1);	     //启动BT1	
	
	while(1)
	{
		NOP;
	}
	return iRet;
}

/** \brief bt sync trg stop:外部中断触发BT0~BT3 关闭
 *  
 *  \param[in] none
 *  \return error code
// */
int bt_sync_trg_stop_demo(void)
{
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				               			//ETB 参数配置结构体		
	
	csi_pin_set_mux(PB1, PB1_INPUT);									//PB1 配置为输入
	csi_pin_pull_mode(PB1, GPIO_PULLUP);								//PB1 上拉
	csi_pin_irq_mode(PB1, EXI_GRP1, GPIO_IRQ_FALLING_EDGE);			    //PB1 下降沿产生中断，选择中断组1
	csi_pin_irq_enable(PB1,ENABLE);							            //PB1 中断使能	
	csi_exi_set_evtrg(1, TRGSRC_EXI1, 0);						        //EXI1 触发EXI_TRGOUT0
	
	
//	csi_pin_set_mux(PA6, PA6_OUTPUT);		//PA6 output ，并在BT中断里面翻转IO
//	csi_pin_set_high(PA6);					//PA6 output high;		
	
	csi_bt_timer_init(BT0,1000);									//BT0 定时
	csi_bt_set_sync(BT0, BT_TRGIN_SYNCEN1, BT_TRG_ONCE, BT_TRG_AUTOAREARM);	//外部触发BT0启动(SYNCIN1)，单次模式
	
//	csi_bt_timer_init(BT1,2000);									//BT1 定时
//	csi_bt_set_sync(BT1, BT_TRGIN_SYNCEN1, BT_TRG_ONCE, BT_TRG_AUTOAREARM);	//外部触发BT1启动(SYNCIN1)
	
//	csi_bt_timer_init(BT2,3000);									//BT2 定时
//	csi_bt_set_sync(BT2, BT_TRGIN_SYNCEN1, BT_TRG_ONCE, BT_TRG_AUTOAREARM);	//外部触发BT2启动(SYNCIN1)
	
//	csi_bt_timer_init(BT3,4000);									//BT3 定时
//	csi_bt_set_sync(BT3, BT_TRGIN_SYNCEN1, BT_TRG_CONTINU, BT_TRG_AUTOAREARM);	//外部触发BT3启动(SYNCIN1)
		
	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  		//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_EXI_TRGOUT1;  	    //EXI_TRGOUT1作为触发源
	tEtbConfig.byDstIp =  ETB_BT0_SYNCIN1;   	    //BT0 同步输入作为目标事件
//	tEtbConfig.byDstIp =  ETB_BT1_SYNCIN1;   	    //BT1 同步输入作为目标事件
//	tEtbConfig.byDstIp =  ETB_BT2_SYNCIN1;   	    //BT2 同步输入作为目标事件
//	tEtbConfig.byDstIp =  ETB_BT3_SYNCIN1;   	    //BT3 同步输入作为目标事件
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
   
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
	csi_bt_start(BT0);	     //启动BT0
//	csi_bt_start(BT1);	     //启动BT1
//	csi_bt_start(BT2);	     //启动BT2
//	csi_bt_start(BT3);	     //启动BT3
	while(1)
	{
		NOP;
	}
	return iRet;	
}

/** \brief bt evtrg out: 用BT0定时触发另外BT1 PWM输出，当BT0定时周期到时触发BT1输出PWM
 *  \param[in] none
 *  \return error code
 */
int bt_trg_out_demo(void)  
{
	bool byFlag = 0;
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				               		//ETB 参数配置结构体		
	csi_bt_pwm_config_t tPwmCfg;									//BT PWM输出参数初始化配置结构体
	
//	csi_pin_set_mux(PA6, PA6_OUTPUT);								//PA6 output ，并在BT中断里面翻转IO
//	csi_pin_set_low(PA6);											//PA6 output high;	

	csi_bt_timer_init(BT0,10000);									//BT0 定时
	csi_bt_set_evtrg(BT0, BT_TRGSRC_PEND,ENABLE);					//BT0 PEND事件触发输出
	csi_irq_enable((uint32_t *)BT0);								//enable BT0 irq
	
	csi_pin_set_mux(PC11, PC11_BT1_OUT);					//PC11 作为BT1 PWM输出引脚
	//init timer pwm para config
	tPwmCfg.byIdleLevel = BT_PWM_IDLE_LOW;					//PWM 输出空闲电平
	tPwmCfg.byStartLevel= BT_PWM_START_HIGH;				//PWM 输出起始电平
	tPwmCfg.byDutyCycle = 80;								//PWM 输出占空比(0 < DutyCycle < 100)		
	tPwmCfg.wFreq 		= 1000;								//PWM 输出频率
	//tPwmCfg.byInt   	= BT_INTSRC_PEND | BT_INTSRC_CMP;	//PWM 中断配置(PEND and CMP)
	tPwmCfg.byInt  		= BT_INTSRC_NONE;
	csi_bt_pwm_init(BT1, &tPwmCfg);							//初始化BT1 PWM输出
	csi_bt_set_sync(BT1, BT_TRGIN_SYNCEN0, BT_TRG_ONCE, BT_TRG_AUTOAREARM);	//外部触发bt启动(SYNCIN0)

	tEtbConfig.byChType  = ETB_ONE_TRG_ONE;  //单个源触发单个目标
	tEtbConfig.bySrcIp   = ETB_BT0_TRGOUT;   //BT0_TRGOUT作为触发源
	tEtbConfig.byDstIp   = ETB_BT1_SYNCIN0;  //BT1 同步输入作为目标事件
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG; //硬件触发模式
   
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
	while(1)
	{
		NOP;
		if(byFlag == 0)             //启动一次
		{
			byFlag = 1;
			csi_pin_set_high(PA6);  //拉高PA6，并在BT0周期中断翻转
			csi_bt_start(BT0);      //启动BT0，当周期结束中断时产生触发信号触发BT1工作
//			csi_bt_soft_evtrg(BT0);
		}
	}
	return iRet;
}

/** \brief bt_irqhandler: BT0 中断处理示例函数
 *  \param[in] csp_bt_t
 *  \return error code
 */
__attribute__((weak)) void bt_irqhandler0(csp_bt_t *ptBtBase)
{
    // ISR content ...
	volatile uint32_t wMisr = csp_bt_get_isr(ptBtBase);
	
	if(wMisr & BT_PEND_INT)					//PEND interrupt
	{
		csp_bt_clr_isr(ptBtBase, BT_PEND_INT);
//		csi_pin_toggle(PA6);				//PA06 toggle	
	}
	if(wMisr & BT_CMP_INT)					//CMP interrupt
	{
		csp_bt_clr_isr(ptBtBase, BT_CMP_INT);
//		csi_pin_set_low(PA5);
//		csi_pin_set_high(PA6);	
	}
	if(wMisr & BT_EVTRG_INT)				//EVTRG interrupt
	{
//		csi_pin_set_low(PA5);
		csp_bt_clr_isr(ptBtBase, BT_EVTRG_INT);
	}
}

/** \brief bt_irqhandler: BT1 中断处理示例函数
 *  \param[in] csp_bt_t
 *  \return error code
 */
__attribute__((weak)) void bt_irqhandler1(csp_bt_t *ptBtBase)
{
    // ISR content ...
	volatile uint32_t wMisr = csp_bt_get_isr(ptBtBase);
	
	if(wMisr & BT_PEND_INT)					//PEND interrupt
	{
		csp_bt_clr_isr(ptBtBase, BT_PEND_INT);
//		csi_pin_toggle(PA6);				//PA06 toggle	
	}
	
	if(wMisr & BT_CMP_INT)					//CMP interrupt
	{
		csp_bt_clr_isr(ptBtBase, BT_CMP_INT);
//		csi_pin_set_low(PA5);
//		csi_pin_set_high(PA6);	
	}
	if(wMisr & BT_EVTRG_INT)				//EVTRG interrupt
	{
//		csi_pin_set_low(PA5);
		csp_bt_clr_isr(ptBtBase, BT_EVTRG_INT);
	}
}

/** \brief bt_irqhandler: BT2 中断处理示例函数
 *  \param[in] csp_bt_t
 *  \return error code
 */
__attribute__((weak)) void bt_irqhandler2(csp_bt_t *ptBtBase)
{
    // ISR content ...
	volatile uint32_t wMisr = csp_bt_get_isr(ptBtBase);
	
	if(wMisr & BT_PEND_INT)					//PEND interrupt
	{
		csp_bt_clr_isr(ptBtBase, BT_PEND_INT);
//		csi_pin_toggle(PA6);				//PA06 toggle	
	}
	if(wMisr & BT_CMP_INT)					//CMP interrupt
	{
		csp_bt_clr_isr(ptBtBase, BT_CMP_INT);
//		csi_pin_set_low(PA5);
//		csi_pin_set_high(PA6);	
	}
	if(wMisr & BT_EVTRG_INT)				//EVTRG interrupt
	{
//		csi_pin_set_low(PA5);
		csp_bt_clr_isr(ptBtBase, BT_EVTRG_INT);
	}
}
