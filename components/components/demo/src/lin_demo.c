/***********************************************************************//** 
 * \file  lin_demo.c
 * \brief  LIN_EDMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-10-14 <td>V0.0 <td>ZJY     <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
//#include <string.h>
//#include "drv/usart.h"
#include "drv/gpio.h"
#include "drv/lin.h"


/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/

/* Private variablesr------------------------------------------------------*/

#if (USE_LIN_CALLBACK == 0)
/** \brief  usart1_int_handler：lin1中断处理函数
 * 
 *	\brief  LIN发生中断时会调用此函数，函数在interrupt.c里定义为弱(weak)属性，默认不做处理;用户用到中断
 * 			时，请重新定义此函数，在此函数中进行对应中断处理，也可直接在interrupt.c里的函数里进行处理。
 * 		
 * 			注意：LIN和USART都是用USART模块进行通信的，故对于同一个USART IP，要么只用USART功能，要么只用LIN功能，
 * 				 不能同时使用
 * 
 *  \param[out] ptUartBase: USARTx寄存器结构体指针，指向USARTx的基地址 
 *  \return none
 */
ATTRIBUTE_ISR void usart1_int_handler(void)
{
	uint32_t wLinSr = csp_usart_get_isr(LIN1) & 0x7f000000;
	
	if(wLinSr & LIN_INT_ERR)								//error
	{
		csp_usart_clr_isr(LIN1,LIN_INT_ERR_S);	//clear interrupt status
	}
	else
	{
		switch(wLinSr)	
		{
			case LIN_INT_WAKEUP:								//LIN Wake up Interrupt	
				nop;
				csp_usart_clr_isr(LIN1,LIN_INT_WAKEUP_S);	//clear interrupt status
				break;
			case LIN_INT_ENDMESS:								//Ended message Interrupt
				if(!(g_tLinCtrl[1].byWkStat & LIN_EVENT_ALLERR))	//no error
				{
					if(g_tLinCtrl[1].byWkMode == LIN_RECV)
					{
						if(g_tLinCtrl[1].byRxSize < 5)
						{
							for(uint8_t i = 0; i < g_tLinCtrl[1].byRxSize; i++)
							{
								g_tLinCtrl[1].pbyRxData[i] = (uint8_t)(csp_usart_lin_get_dfrr0(LIN1) >> (8 * i));
							}
						}
						else
						{
							for(uint8_t i = 0; i < (g_tLinCtrl[1].byRxSize - 4); i++)
							{
								g_tLinCtrl[1].pbyRxData[i+4] = (uint8_t)(csp_usart_lin_get_dfrr1(LIN1) >> (8 * i));
							}
							g_tLinCtrl[1].pbyRxData[0] = (uint8_t)csp_usart_lin_get_dfrr0(LIN1);
							g_tLinCtrl[1].pbyRxData[1] = (uint8_t)(csp_usart_lin_get_dfrr0(LIN1) >> 8);
							g_tLinCtrl[1].pbyRxData[2] = (uint8_t)(csp_usart_lin_get_dfrr0(LIN1) >> 16);
							g_tLinCtrl[1].pbyRxData[3] = (uint8_t)(csp_usart_lin_get_dfrr0(LIN1) >> 24);
						}
					}
	
					g_tLinCtrl[1].byWkStat = LIN_EVENT_ENDMESS;	
					
					
				}
					
				csp_usart_clr_isr(LIN1,LIN_INT_ENDMESS_S);	//clear interrupt status
				break;
			case LIN_INT_ENDHEADER:								//Ended header Interrupt
//				if(g_tLinTran.byWkMode == LIN_RECV)
//					csp_usart_cr_cmd(LIN1, LIN_STRESP);	

				csp_usart_clr_isr(LIN1,LIN_INT_ENDHEADER_S);	//clear interrupt status
				break;
			default:
				break;
		}
	}
}

#endif

/** \brief 	lin_send_demo
 * 			示例lin作为主机发送完整数据帧。成功发送一个完整的数据帧，会进行下一次发送。
 *  	  	帧头发送完成时，LIN_ENDHEADER_INT_S状态会被置起。整个数据帧发送完成时，
 * 		   	LIN_ENDMESS_INT_S会被置起。
 * 
 *  \param[in] none
 *  \return error code
 */
int lin_send_demo(void)
{
	int iRet = 0;
	uint8_t bySdBuf[8] = {0x45,0x46,3,4,5,6,7,8};
	csi_lin_config_t tLinCfg;							//lin 初始化参数配置结构体

#if (USE_GUI == 0)	
	csi_gpio_set_mux(GPIOC, PC10, PC10_USART1_TX);		//USART1 TX管脚配置	
	csi_gpio_set_mux(GPIOC, PC11, PC11_USART1_RX);		//USART1 RX管脚配置
	csi_gpio_pull_mode(GPIOC, PC11, GPIO_PULLUP);		//RX管脚上拉使能, 建议配置
#endif
	
	tLinCfg.byClkSrc	= LIN_CLKSRC_DIV1;				//时钟源clk = pclk
	tLinCfg.byLinVer	= LIN_VER1_2;					//LIN总线版本 支持lin1.2/lin2.0
	tLinCfg.bySyncBrk	= 13;							//同步间隔段位数，>=13
	tLinCfg.byCheck		= LIN_CHKSUM_CLASSIC;			//校验和选择,经典的
	tLinCfg.hwWkUpTime  = 0x3ad4;						//LIN2.0 唤醒时间
	tLinCfg.byLcp1[0]	= 0x4c;							//LCP0~8设置，超时限制，如果超时会产生无响应错误标志
	tLinCfg.byLcp1[1]	= 0x5b;
	tLinCfg.byLcp1[2]	= 0x68;
	tLinCfg.byLcp1[3]	= 0x77;
	tLinCfg.byLcp2[0]	= 0x84;
	tLinCfg.byLcp2[1]	= 0x92;
	tLinCfg.byLcp2[2]	= 0xa0;
	tLinCfg.byLcp2[3]	= 0xaf;
	tLinCfg.hwBaudRate	= 9600;							//速率 <= 20kpbs
	csi_lin_init(LIN1, &tLinCfg);						//初始化LIN
	
	csi_lin_int_enable(LIN1, LIN_INTSRC_USER);			//使能LIN所有中断		
	csi_lin_start(LIN1);

	mdelay(10);

	while(1)
	{
		csi_lin_send(LIN1, 0x0e, (void *)bySdBuf, 8);		//发送完整帧
		while(1)
		{
			if(csi_lin_get_msg(LIN1, ENABLE))
			{
				nop;
				break;
			}
		}
		nop;
		mdelay(10);
	}
	
	return iRet;
}


/** \brief 	lin_send_recv_demo
 * 			示例lin作为主机发送帧头，并等待接收应答数据。
 *  	  	帧头发送完成时，LIN_ENDHEADER_INT_S状态会被置起。整个数据帧接收完成时，
 * 		   	LIN_ENDMESS_INT_S会被置起。
 * 
 *  \param[in] none
 *  \return error code
 */
int lin_send_recv_demo(void)
{
	volatile int iRet = 0;
	uint8_t byReBuf[8] = {0};
	
	csi_lin_config_t tLinCfg;							//lin 初始化参数配置结构体
	
#if (USE_GUI == 0)		
	csi_gpio_set_mux(GPIOC, PC10, PC10_USART1_TX);		//USART1 TX管脚配置	
	csi_gpio_set_mux(GPIOC, PC11, PC11_USART1_RX);		//USART1 RX管脚配置
	csi_gpio_pull_mode(GPIOC, PC11, GPIO_PULLUP);		//RX管脚上拉使能, 建议配置
#endif
	
	tLinCfg.byClkSrc	= LIN_CLKSRC_DIV1;				//时钟源clk = pclk
	tLinCfg.byLinVer	= LIN_VER2_0;					//LIN总线版本 支持lin1.2/lin2.0
	tLinCfg.bySyncBrk	= 15;							//同步间隔段位数，>=13
	tLinCfg.byCheck		= LIN_CHKSUM_CLASSIC;			//校验和选择,经典的
	tLinCfg.hwWkUpTime  = 0x3ad4;						//LIN2.0 唤醒时间
	tLinCfg.byLcp1[0]	= 0x4c;							//LCP0~8设置，超时限制，如果超时会产生无响应错误标志
	tLinCfg.byLcp1[1]	= 0x5b;
	tLinCfg.byLcp1[2]	= 0x68;
	tLinCfg.byLcp1[3]	= 0x77;
	tLinCfg.byLcp2[0]	= 0x84;
	tLinCfg.byLcp2[1]	= 0x92;
	tLinCfg.byLcp2[2]	= 0xa0;
	tLinCfg.byLcp2[3]	= 0xaf;
	tLinCfg.hwBaudRate	= 9600;							//速率 <= 20kpbs
	csi_lin_init(LIN1, &tLinCfg);						//初始化LIN
	
	csi_lin_int_enable(LIN1, LIN_INTSRC_USER);			//使能LIN所有中断
	csi_lin_start(LIN1);

	mdelay(10);

	while(1)
	{
		iRet = csi_lin_send_recv(LIN1, 0x0e, (void *)byReBuf, 7);	//发送帧头，等待应答数据
			
		if(iRet == 7)		//接收完成
		{
			nop;
		}
		
		mdelay(10);
	}
	
	return iRet;
}

