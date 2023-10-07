/***********************************************************************//** 
 * \file  lin_demo_callback.c
 * \brief  LIN_CALLBACK_EDMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-9-22 <td>V0.0 <td>GQQ     <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/

#include "csi_drv.h"
#include "iostring.h"
#include "board_config.h"

/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/

/* Private variable------------------------------------------------------*/

#if (USE_LIN_CALLBACK == 1)
	


/** \brief user_receive_callback：lin主机中断发送并等待应答的回调函数
 * 
 * 	\brief	用户定义，使用csi标准库进行中断发送时，发送完毕后回自动调用用户注册的回调函数；用户可在回调
 * 			函数里做自己的处理，而不需要关注具体的底层中断处理。
 * 		
 *  \param[out] ptLinBase: Linx寄存器结构体指针，指向USARTx的基地址 
 *  \param[out] eEvent: lin事件 \ref  csi_lin_event_e
 *  \param[out] pbyBuf: 接收数据缓存指针
 *  \param[out] hwLen: 接收数据的长度
 *  \return none
 */
void user_receive_callback(csp_lin_t *ptLinBase,csi_lin_event_e eEvent, uint8_t *pbyBuf, uint8_t *hwLen)
{
	uint8_t byRecvLen = *hwLen;
	
	switch (eEvent) 
	{
		case LIN_EVENT_ENDHEADER:
		//添加用户程序
		break;
		case LIN_EVENT_ENDMESS:
		//添加用户程序
			if(byRecvLen == 7)
			{
				my_printf("Recieve length is correct!");
			}
		break;
		default: 
		break;
	}
}

/** \brief lin_send_recv_int_callback_demo：示例lin主机发送帧头，等待应答数据
 *  		帧头发送完成的时候会进入回调函数，在条件LIN_EVENT_ENDHEADER下添加相应的用户代码
 *			接收到完整的应答数据时，会进入条件LIN_EVENT_ENDMESS下执行，用户可在该条件语句下
 *			添加相应的代码
 *  \param[in] none
 *  \return error code
 */
int lin_send_recv_int_callback_demo(void)
{
	volatile int iRet = 0;
	uint8_t byReBuf[8] = {0};
	
	csi_lin_config_t tLinCfg;							//lin 初始化参数配置结构体
	
#if (USE_GUI == 0)		
	csi_gpio_set_mux(GPIOB, PB10, PB10_USART0_TX);		//USART0 TX管脚配置	
	csi_gpio_set_mux(GPIOB, PB11, PB11_USART0_RX);		//USART0 RX管脚配置
	csi_gpio_pull_mode(GPIOB, PB11, GPIO_PULLUP);		//RX管脚上拉使能, 建议配置
#endif
	
	tLinCfg.eClkSrc		= LIN_CLKSRC_DIV1;				//时钟源clk = pclk
	tLinCfg.eLinVer		= LIN_VER2_0;					//LIN总线版本 支持lin1.2/lin2.0
	tLinCfg.bySyncBrk	= 15;							//同步间隔段位数，>=13
	tLinCfg.eCheck		= LIN_CHKSUM_CLASSIC;			//校验和选择,经典的
	tLinCfg.hwWkUpTime  = 0x3ad4;						//LIN2.0 唤醒时间
	tLinCfg.hwBaudRate	= 9600;							//速率 <= 20kpbs
	csi_lin_init(LIN0, &tLinCfg);						//初始化LIN
	
	csi_lin_int_enable(LIN0, LIN_INTSRC_USER);			//使能LIN所有中断
	csi_lin_register_callback(LIN0, LIN_CALLBACK_ID_RECV, user_receive_callback);
	csi_lin_start(LIN0);

	mdelay(10);

	while(1)
	{
		iRet = csi_lin_send_recv(LIN0, 0x0e, (void *)byReBuf, 7);	//发送帧头，等待应答数据
		
		mdelay(10);
	}
	
	return iRet;
}

#endif