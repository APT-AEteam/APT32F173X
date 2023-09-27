/***********************************************************************//** 
 * \file  can_demo.c
 * \brief  CAN_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-07-19 <td>V0.0  <td>ZJY   <td>initial
 * <tr><td> 2021-09-25 <td>V0.1  <td>ZJY   <td>code normalization
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
static csi_can_recv_t s_tCanRecv[4];		//接收数据结构体缓存
static uint8_t s_byRecvMode	= 0;			//接收模式选择，接收中断处理中用；0/1 = normal/fifo

#if (USE_CAN_CALLBACK == 1)	

/** \brief  can_int_handler: CAN中断服务函数
 * 
    \brief  CAN发生中断时会调用此函数，函数在interrupt.c里定义为弱(weak)属性，默认不做处理;用户用到中断
 * 			时，请重新定义此函数，在此函数中进行对应中断处理，也可直接在interrupt.c里的函数里进行处理。
 * 
 *  \param[in] none
 *  \return none
 */
ATTRIBUTE_ISR void can_int_handler(void) 
{
	volatile uint32_t wIrVal, wStatus = 0;
	volatile uint16_t hwIntNum = csp_can_get_hpir(CAN0);		//获取中断信息，状态中断或者是报文通道中断
	
	switch(hwIntNum)		
	{
		case 0x00:												//无中断,不做处理 
			break;
		case 0x8000:											//状态中断，包含报文发送/接收OK状态和其他10个异常中断
			wStatus = csp_can_get_sr(CAN0) & CAN_INT_ALL;	
			
			//用户添加处理，处理对应的异常，
//			if(wStatus & CAN_INT_ERWARNTR)						//被动错误警告
//			{
//				csp_can_clr_isr(CAN0, CAN_INT_ERWARNTR);	
//			}
//			///
//			if(wStatus & CAN_INT_ERPASSTR)						//被动错误发送或者回复
//			{
//				csp_can_clr_isr(CAN0, CAN_INT_ERPASSTR);
//			}
//			......
			
			csp_can_clr_isr(CAN0, wStatus);						//清除所有中断	
					
			break;
		default:												//消息通道中断			
			
			csp_can_set_tmr(CAN0, hwIntNum,1,CAN_AMCR_MSK | CAN_CLRIT_MSK | CAN_TRND_MSK);	//通过传输管理寄存器(TMR)写IF1命令请求，清除NAWDATA和ITPND标志 
			while(csp_can_get_sr(CAN0) & CAN_STA_BUSY1);									//查询IF1是否忙?
			wStatus = csp_can_get_mcr(CAN0);												//读报文控制寄存器(MCR)，读出后，自动清除MCR中的NAWDATA和ITPND标志
			
			if(wStatus & CAN_NEWDAT_MSK)													//如果NEWDAT标志 == 1，则为接收到完整报文的对应通道产生的中断
			{
				
			}
			else 																			//如果NEWDAT标志 == 0，则为对应报文通道发送报文完成产生的中断
			{
				//用户添加处理
				nop;
			}
			break;
	}
	
}

#endif

/** \brief	can_send_demo： CAN发送demo，不开启中断
 * 	\brief	CAN报文发送，报文数据可配置为1~8个字节；通过TXOK状态中断判断发送是否完成
 * 
 *  \param[in] none
 *  \return none
 */
int can_send_demo(void)
{
	int iRet = 0;
	uint8_t	i = 0;
	uint8_t byData[4];
	uint32_t wSdDelay = 0x00;
	
	csi_can_config_t 	 tCanConfig;						//CAN 初始化结构体
	csi_can_tx_config_t  tCanTxConfig;						//CAN 发送报文配置结构体
	
#if (USE_GUI == 0)											//用户未选择图形化编程			
	csi_gpio_set_mux(GPIOA, PA11, PA11_CAN_RX);				//CAN RX管脚配置
	csi_gpio_set_mux(GPIOA, PA12, PA12_CAN_TX);				//CAN TX管脚配置
#endif

	//初始化配置
	tCanConfig.byClkSrc  = CAN_CLKSRC_PCLK;					//CAN CLK = PCLK		
	tCanConfig.wBaudRate = CAN_BDR_500K;					//500k
	tCanConfig.bAuReTran = DISABLE;							//自动重新传输关闭
	csi_can_init(CAN0, &tCanConfig);						//CAN初始化
	
	//CAN 发送报文配置，每个报文配置在一个报文通道中(CAN_CH1~CAN_CH32)
	for(i = CAN_CH1; i < CAN_CH3; i++)
	{
		//id,识别符配置
		tCanTxConfig.tId.eIdMode	= CAN_ID_STD;			//报文ID模式，标准模式(11Bit)
		tCanTxConfig.tId.eMsgDir	= CAN_DIR_SEND;			//报文方向
		tCanTxConfig.tId.hwStdId 	= 0x700 + i;			//标准11BIT ID
		tCanTxConfig.tId.wExtId		= 0x3FFFF;				//扩展ID,标准模式下忽略	
		
		//tx mcr 发送报文控制配置
		tCanTxConfig.tMc.bTxIeEn	= DISABLE;				//禁止报文发送中断(通道源中断)
		tCanTxConfig.tMc.bRmtEn		= DISABLE;				//远程应答帧禁止，即接收到远程帧，不请求发送应答
		tCanTxConfig.tMc.bTxReqEn	= DISABLE;				//报文对象请求发送禁止，
		tCanTxConfig.tMc.byDataLen	= 0x08;					//发送数据数量
		
		//data a
		tCanTxConfig.tDataA.bydata[0] 	= i+0x11;			//数据A(低4字节数据配置)配置
		tCanTxConfig.tDataA.bydata[1] 	= i+0x12;
		tCanTxConfig.tDataA.bydata[2] 	= i+0x13;
		tCanTxConfig.tDataA.bydata[3] 	= i+0x14;
		
		//data b
		tCanTxConfig.tDataB.bydata[0] 	= i+0x21;			//数据B(高4字节数据配置)配置
		tCanTxConfig.tDataB.bydata[1] 	= i+0x22;
		tCanTxConfig.tDataB.bydata[2] 	= i+0x23;
		tCanTxConfig.tDataB.bydata[3] 	= i+0x24;
		csi_can_set_tx_msg(CAN0, i, &tCanTxConfig);			//配置发送报文通道
	}
	
	csi_can_start(CAN0);									//使能(打开)CAN模块
	
	//发送报文通道1和报文通道2的报文
	for(i = 0; i< 2; i++)
	{
		if(i%2)
			csi_can_msg_send(CAN0, CAN_CH1, 7);				//报文通道1中的报文，发送7字节数据	
		else
			csi_can_msg_send(CAN0, CAN_CH2, 6);				//报文通道2中的报文，发送6字节数据	
		
		//
		while(1)
		{
			if(csi_can_get_sr(CAN0) & CAN_STA_TXOK)			//可通过查询(TXOK状态)发送是否完成消息
			{
				csi_can_clr_sr(CAN0, CAN_STA_TXOK);			//清除消息
				wSdDelay = 0;
				break;
			}
			else 
			{
				if(wSdDelay >= 0x3fff)						//发送状态查询超时处理
				{
					wSdDelay = 0;
					break;
				}
				else
					wSdDelay++;
			}
		}
	}
	
	//更新报文通道1/2中的报文数据
	byData[0] = 0x55;										//发送的数据
	byData[1] = 0x65;
	byData[2] = 0x75;
	byData[3] = 0x85;
	csi_can_set_bdata(CAN0, CAN_CH1, byData);				//更新报文通道1中的低4字节数据
	
	byData[0] = 0x95;
	byData[1] = 0xA5;										//发送的数据
	byData[2] = 0xB5;
	byData[3] = 0xC5;
	csi_can_set_bdata(CAN0, CAN_CH2, byData);				//更新报文通道1中的高4字节数据
	
	//更新报文通道1中报文ID模式为扩展模式
	tCanTxConfig.tId.eIdMode	= CAN_ID_EXT;				//报文ID模式，扩展模式(11Bit)
	tCanTxConfig.tId.eMsgDir	= CAN_DIR_SEND;				//报文方向
	tCanTxConfig.tId.hwStdId 	= 0x7e0;					//标准11BIT ID
	tCanTxConfig.tId.wExtId		= 0x3FFFF;					//扩展ID,标准模式下忽略	
	csi_can_set_id(CAN0, CAN_CH1, &tCanTxConfig.tId);		//配置报文识别符
	
	
	for(i = 0; i< 2; i++)
	{
		if(i%2)
			csi_can_msg_send(CAN0, CAN_CH1, 8);
		else
			csi_can_msg_send(CAN0, CAN_CH2, 8);
		
		//
		while(1)
		{
			if(csi_can_get_sr(CAN0) & CAN_STA_TXOK)			//可通过查询(TXOK状态)发送是否完成消息
			{
				csi_can_clr_sr(CAN0, CAN_STA_TXOK);			//清除消息
				wSdDelay = 0;
				break;
			}
			else 
			{
				if(wSdDelay >= 0x3fff)						//发送状态查询超时处理
				{
					wSdDelay = 0;
					break;
				}
				else
					wSdDelay++;
			}
		}
		udelay(400);
	}
	
	return iRet;
}

/** \brief	can_send_int_demo CAN发送demo，开启中断
 * 	\brief	CAN报文发送使用中断(开启所有状态中断和对应发送报文通道中断)，报文数据可配置为1~8个字节；
 * 
 *  \param[in] none
 *  \return none
 */
int can_send_int_demo(void)
{
	int iRet = 0;
	csi_can_config_t 	 tCanConfig;						//CAN init config
	csi_can_tx_config_t  tCanTxConfig;						//CAN TX config

#if (USE_GUI == 0)											//用户未选择图形化编程			
	csi_gpio_set_mux(GPIOA, PA11, PA11_CAN_RX);				//CAN RX管脚配置
	csi_gpio_set_mux(GPIOA, PA12, PA12_CAN_TX);				//CAN TX管脚配置
#endif
	
	//CAN MR 寄存器配置,即初始化配置
	tCanConfig.byClkSrc 	= CAN_CLKSRC_PCLK;				//CAN CLK = PCLK		
	tCanConfig.wBaudRate 	= CAN_BDR_1000K;				//1000k
	tCanConfig.bAuReTran = DISABLE;							//自动重新传输关闭
	csi_can_init(CAN0, &tCanConfig);
	
	//id,识别符配置
	tCanTxConfig.tId.eIdMode		= CAN_ID_EXT;			//报文ID模式，扩展模式(11Bit)
	tCanTxConfig.tId.eMsgDir		= CAN_DIR_SEND;			//报文方向
	tCanTxConfig.tId.hwStdId 		= 0x700;				//标准11BIT ID
	tCanTxConfig.tId.wExtId			= 0x3FFFF;					//扩展ID,标准模式下忽略	
	
	//tx mcr 发送报文控制配置
	tCanTxConfig.tMc.bTxIeEn		= ENABLE;				//使能报文发送中断(通道源中断)
	tCanTxConfig.tMc.bRmtEn			= DISABLE;				//远程应答帧禁止，即接收到远程帧，不请求发送应答
	tCanTxConfig.tMc.bTxReqEn		= DISABLE;				//报文对象请求发送禁止，
	tCanTxConfig.tMc.byDataLen		= 0x08;					//发送数据数量
	
	//data a
	tCanTxConfig.tDataA.bydata[0] 	= 0x21;					//数据A(低4字节数据配置)配置
	tCanTxConfig.tDataA.bydata[1] 	= 0x22;
	tCanTxConfig.tDataA.bydata[2] 	= 0x23;
	tCanTxConfig.tDataA.bydata[3] 	= 0x24;
	
	//data b
	tCanTxConfig.tDataB.bydata[0] 	= 0x31;					//数据B(高4字节数据配置)配置
	tCanTxConfig.tDataB.bydata[1] 	= 0x32;
	tCanTxConfig.tDataB.bydata[2] 	= 0x33;
	tCanTxConfig.tDataB.bydata[3] 	= 0x34;
	csi_can_set_tx_msg(CAN0, CAN_CH1, &tCanTxConfig);		//配置发送报文

	csi_can_int_enable(CAN0, CAN_INTSRC_ALL);				//使能所有状态中断
	csi_can_ch_int_enable(CAN0, CAN_CH1);					//使能发送报文对应的报文通道中断
	csi_can_start(CAN0);									//使能(打开)CAN模块
		
	//
	while(1)
	{
		csi_can_msg_send(CAN0, CAN_CH1, 8);					//报文通道1中的报文，发送8字节数据
		udelay(300);
	}
	
	return iRet;
}

/** \brief can 接收demo，接收报文通道不配置为FIFO缓存的报文接收，接收过滤掩码不使能(ID不过滤全匹配)；
 * 		   接收报文根据报文匹配的ID接收到对应的报文通道中
 * 
 *  \param[in] none
 *  \return none
 */
int can_recv_demo(void)
{
	int iRet = 0;
	uint8_t	i = 0;
	uint8_t byRxBuf[16];								//接收数据
	csi_can_recv_t		 tCanRecv[3];					//接收数据结构体缓存，用户定义大小
	csi_can_config_t 	 tCanConfig;					//CAN init config
	csi_can_rx_config_t  tCanRxCfg;						//CAN RX config	

#if (USE_GUI == 0)										//用户未选择图形化编程		

	csi_pin_set_mux(PB8, PB8_CAN_RX);					//CAN RX管脚配置
	csi_pin_set_mux(PB9, PB9_CAN_TX);					//CAN TX管脚配置
#endif
	
	//CAN MR 寄存器配置,即初始化配置
	tCanConfig.byClkSrc 	= CAN_CLKSRC_PCLK;			//CAN CLK = PCLK		
	tCanConfig.wBaudRate 	= CAN_BDR_500K;				//500k
	tCanConfig.hwStaInter 	= CAN_INTSRC_ALL;		//异常中断全部使能	
	tCanConfig.wChnlInter	= 0xFFFF;					//报文通道中断
	csi_can_init(CAN0, &tCanConfig);
	
	//CAN RX CONFIG
	//报文接收到CHI~CH3,ID为全匹配，匹配上的ID数据接收到对应的报文接收通道中
	for(i = CAN_CH1; i < CAN_CH4; i++)
	{
		//Ir
		tCanRxCfg.tId.eIdMode		= CAN_ID_STD;		//报文ID模式，标准模式(11Bit)
		tCanRxCfg.tId.eMsgDir		= CAN_DIR_RECV;		//报文方向
		tCanRxCfg.tId.hwStdId		= 0x7F0+i;			//标准11BIT ID
		tCanRxCfg.tId.wExtId		= 0x3FFFF;			//扩展ID,标准模式下忽略
		//Mskr
		tCanRxCfg.tMsk.eIdMdMsk		= CAN_IDMSK_STD;	//ID模式(STD/EXT) MASK
		tCanRxCfg.tMsk.eIdDirMsk	= CAN_DIRMSK_DIS;	//报文方向 MASK disable
		tCanRxCfg.tMsk.hwStdIdMsk  = 0x7F0;			//标准模式ID mask 
		tCanRxCfg.tMsk.wExtIdMsk	= 0x00000;			//扩展模式ID mask, 标准模式下忽略
		//rx mcr
		tCanRxCfg.tMc.bMskEn		= DISABLE;			//禁止过滤接收mask
		tCanRxCfg.tMc.bRxIeEn		= ENABLE;			//使能报文接收中断(通道源中断)，置位ITPAND
		tCanRxCfg.tMc.bOverWrEn	= DISABLE;			//关闭报文覆盖模式	
		tCanRxCfg.tMc.byDataLen	= 0x08;				//数据长度
		
		csi_can_set_rx_msg(CAN0, i, &tCanRxCfg);
	}
	
	csi_can_recv_init(&tCanRecv[0], CAN_CH1, 3);		//接收数据结构体
	csi_can_start(CAN0);
	
	
	while(1)
	{
		
		for(i = 1; i < 3; i++)
		{
			if(csi_can_get_recv_msg() & CAN_MSG_CH(i))
			{
				iRet = csi_can_ch_read(CAN0,byRxBuf, i);
			}
			mdelay(1);
		}
	}
	return iRet;
}

/** \brief can 接收demo，接收报文通道不配置为FIFO缓存的报文接收，接收过滤掩码不使能(ID不过滤全匹配)；
 * 		   接收报文根据报文匹配的ID接收到对应的报文通道中
 * 
 *  \param[in] none
 *  \return none
 */
int can_recv_fifo_demo(void)
{
	int iRet = 0;
	uint8_t	i = 0;
	uint8_t byRxBuf[16];								//接收数据
	csi_can_recv_t		 tCanRecv[3];					//接收数据结构体缓存，用户定义大小
	
	csi_can_config_t 	 tCanConfig;					//CAN init config
	csi_can_rx_config_t  tCanRxCfg;						//CAN RX config	
	
#if !defined(USE_GUI)									//用户未选择图形化编程		
//	csi_pin_set_mux(PC4, PC4_CAN_RX);					//CAN RX管脚配置
//	csi_pin_set_mux(PC5, PC5_CAN_TX);					//CAN TX管脚配置
//	csi_pin_set_mux(PA11, PA11_CAN_RX);					//CAN RX管脚配置
//	csi_pin_set_mux(PA12, PA12_CAN_TX);					//CAN TX管脚配置
	csi_pin_set_mux(PB8, PB8_CAN_RX);					//CAN RX管脚配置
	csi_pin_set_mux(PB9, PB9_CAN_TX);					//CAN TX管脚配置
#endif
	
	//CAN MR 寄存器配置,即初始化配置
	tCanConfig.byClkSrc 	= CAN_CLKSRC_PCLK;			//CAN CLK = PCLK		
	tCanConfig.wBaudRate 	= CAN_BDR_500K;				//500k
	tCanConfig.hwStaInter 	= CAN_INTSRC_ALL;			//异常中断全部使能	
	tCanConfig.wChnlInter	= 0xFFFF;					//报文通道中断
	csi_can_init(CAN0, &tCanConfig);
	
	//CAN RX CONFIG
	for(i = CAN_CH1; i < CAN_CH4; i++)
	{
		//Ir
		tCanRxCfg.tId.eIdMode		= CAN_ID_STD;		//报文ID模式，标准模式(11Bit)
		tCanRxCfg.tId.eMsgDir		= CAN_DIR_RECV;		//报文方向
		tCanRxCfg.tId.hwStdId		= 0x7F0+i;			//标准11BIT ID
		tCanRxCfg.tId.wExtId		= 0x3FFFF;			//扩展ID,标准模式下忽略
		//Mskr
		tCanRxCfg.tMsk.eIdMdMsk		= CAN_IDMSK_STD;	//ID模式(STD/EXT) MASK
		tCanRxCfg.tMsk.eIdDirMsk	= CAN_DIRMSK_DIS;	//报文方向 MASK disable
		tCanRxCfg.tMsk.hwStdIdMsk  = 0x7F0;			//标准模式ID mask 
		tCanRxCfg.tMsk.wExtIdMsk	= 0x00000;			//扩展模式ID mask, 标准模式下忽略
		//rx mcr
		tCanRxCfg.tMc.bMskEn		= DISABLE;			//使能过滤接收mask
		tCanRxCfg.tMc.bRxIeEn		= ENABLE;			//使能报文接收中断(通道源中断)，置位ITPAND
		tCanRxCfg.tMc.bOverWrEn	= DISABLE;			//关闭报文覆盖模式	
		tCanRxCfg.tMc.byDataLen	= 0x08;				//数据长度
		
		if(i == CAN_CH3)
			tCanRxCfg.tMc.bOverWrEn = ENABLE;			//最后一个报文 使能报文覆盖功能
			
		csi_can_set_rx_msg(CAN0, i, &tCanRxCfg);
	}
	
	csi_can_recv_init(&tCanRecv[0], CAN_CH1, 3);		//接收数据结构体
	csi_can_start(CAN0);
	
	nop;
	
	while(1)
	{
		
		for(i = 1; i < 3; i++)
		{
			if(csi_can_get_recv_msg() & CAN_MSG_CH(i))
			{
				iRet = csi_can_ch_read(CAN0,byRxBuf, i);
			}
			mdelay(1);
		}
	}
	return iRet;
}

extern uint8_t apt_can_post_msg(uint32_t wMsg, csi_can_msg_mode_e eMsgMode);

/** \brief can receive interrupt handle function
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \return none
 */ 
void can_irqhandler(csp_can_t *ptCanBase)
{
	uint8_t byRecvPos	= 0;
	volatile uint32_t wIrVal  = 0;
	volatile uint32_t wStatus = 0;
	volatile uint16_t hwIntNum = csp_can_get_hpir(ptCanBase);	//get interrupt pointer							
	
	switch(hwIntNum)		//receive handle
	{
		case 0x00:			//end handle, 
			break;
		case 0x8000:		//status change interrupt handle
			//状态中断
			wStatus = csp_can_get_sr(ptCanBase) & 0xff1e;		
			csi_can_post_msg(CAN_MSG_STATUS, wStatus);			//发送(置位)状态消息，供查询应用程序查询					
			
			if(wStatus & CAN_INT_ERWARNTR)						//error passive warning transition interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_INT_ERWARNTR);	
			}
			//
			if(wStatus & CAN_INT_ERPASSTR)						//error passive transition interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_INT_ERPASSTR);
			}
			//
			if(wStatus & CAN_INT_BUSOFFTR)						//bus off transition interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_INT_BUSOFFTR);
			}
			//
			if(wStatus & CAN_INT_ACTVT)							//activity interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_INT_ACTVT);
			}
			//
			if(wStatus & CAN_INT_RXOK)							//successfully received a message interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_INT_RXOK);
			}
			//
			if(wStatus & CAN_INT_TXOK)							//successfully transmit a message interrupt.
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_INT_TXOK);
			}
			//
			if(wStatus & CAN_INT_STUFF)							//stuff error interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_INT_STUFF);
			}
			//
			if(wStatus & CAN_INT_FORM)							//form error interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_INT_FORM);
			}
			//
			if(wStatus & CAN_INT_ACK)							//acknowledge error interrupt.
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_INT_ACK);
			}
			//
			if(wStatus & CAN_INT_BIT1)							//bit to one error interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_INT_BIT1);
			}
			//
			if(wStatus & CAN_INT_BIT0)							//bit to zero error interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_INT_BIT0);
			}
			//
			if(wStatus & CAN_INT_CRC)							//CRC error interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_INT_CRC);
			}
			
			break;
		default:			
			//消息通道中断
			csp_can_set_tmr(ptCanBase, hwIntNum, 1, CAN_AMCR_MSK | CAN_CLRIT_MSK | CAN_TRND_MSK);	//Write If1 command request, clear NAWDATA and ITPND flag
			while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY1);										//If1 Busy?	
			wStatus = csp_can_get_mcr(ptCanBase);													//Read If1 message control reg, Read first and clean up NAWDATA and ITPND
			
			if(wStatus & CAN_NEWDAT_MSK)															//NEWDAT flag == 1 receive msg
			{
				//receive message
				if(g_tCanCtrl[0].ptCanRecv)
				{
					byRecvPos = hwIntNum - g_tCanCtrl[0].byStrCh;
					if(s_byRecvMode == 0)
					{
						//不使用FIFO缓存，数据接收报文不覆盖
						if((g_tCanCtrl[0].ptCanRecv + byRecvPos)->wRecvId != 0)			//判断接收BUF中ID是否为0(读的时候会清除)，BUF中有数据则不读入到BUF
							return;
					}
					else 
					{
						//判断接收BUF通道中是否有数据，有的话把数据接收到BUF下一个通道中，直到BUF中最后一个通道，
						//若BUF中都有数据，则覆盖到最后一个BUF通道中
						while(1)
						{
							if((g_tCanCtrl[0].ptCanRecv + byRecvPos)->wRecvId != 0)		//接收BUF通道中有数据
							{
								if(byRecvPos < (g_tCanCtrl[0].byTolChNum -1))
									byRecvPos++;
								else
									 break;
							}
							else 
								break;
						}
					}
					
				
					wIrVal = csi_can_get_ifx(ptCanBase, hwIntNum, CAN_IFX_IR);
					if(wIrVal & CAN_XTD_MSK)														
						wIrVal &= (CAN_EXTID_MSK | CAN_EXTID_MSK);						//extid
					else
						wIrVal = (wIrVal & CAN_BASEID_MSK) >> 18;						//stdid
					
					(g_tCanCtrl[0].ptCanRecv + byRecvPos)->wRecvId =	wIrVal;				//ID
					(g_tCanCtrl[0].ptCanRecv + byRecvPos)->wRecvData[0] = csi_can_get_ifx(ptCanBase, hwIntNum, CAN_IFX_DAR);		//DATA_A
					(g_tCanCtrl[0].ptCanRecv + byRecvPos)->wRecvData[1] = csi_can_get_ifx(ptCanBase, hwIntNum, CAN_IFX_DBR);		//DATA_B
					(g_tCanCtrl[0].ptCanRecv + byRecvPos)->byDataLen = wStatus & 0x0f;												//DATA LEN
					(g_tCanCtrl[0].ptCanRecv + byRecvPos)->byChNum = hwIntNum;													//Channel Num
					
					csi_can_post_msg(CAN_MSG_RECV, (0x01ul << byRecvPos));				//发送接收数据消息
				}
			}
			else 
			{
				//message send complete
				csi_can_post_msg((0x01ul << (hwIntNum -1)), CAN_MSG_SEND);			//发送通道发送成功消息	
			}
			break;
	}
}