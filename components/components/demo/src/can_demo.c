/***********************************************************************//** 
 * \file  can_test.c
 * \brief  CAN_TEST description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-07-19 <td>V0.0  <td>ZJY   <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include <csi_drv.h>

/* externs function--------------------------------------------------------*/

/* externs variablesr------------------------------------------------------*/

/* Private macro-----------------------------------------------------------*/

/* Private variablesr------------------------------------------------------*/
static uint8_t s_byRecvMode	= 0;		//接收模式选择，接收中断处理中用；0/1 = normal/fifo



/** \brief can 发送demo，开启状态中断，禁止报文通道中断，可通过TXOK状态中断判断发送是否完成
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
	
	//volatile uint32_t wData = 0;
	csi_can_config_t 	 tCanConfig;					//CAN init config
	csi_can_tx_config_t  tCanTxCfg;						//CAN TX config
	
//	csi_pin_set_mux(PC4, PC4_CAN_RX);					//CAN RX管脚配置
//	csi_pin_set_mux(PC5, PC5_CAN_TX);					//CAN TX管脚配置
//	csi_pin_set_mux(PA11, PA11_CAN_RX);					//CAN RX管脚配置
//	csi_pin_set_mux(PA12, PA12_CAN_TX);					//CAN TX管脚配置
	csi_pin_set_mux(PB8, PB8_CAN_RX);					//CAN RX管脚配置
	csi_pin_set_mux(PB9, PB9_CAN_TX);					//CAN TX管脚配置
	
	//CAN MR 寄存器配置,即初始化配置
	tCanConfig.byClkSrc 	= CAN_CLKSRC_PCLK;			//CAN CLK = PCLK		
	tCanConfig.wBaudRate 	= CAN_BDR_500K;				//500k
	tCanConfig.hwStaInter 	= 0xff1e;					//状态中断全部使能	
	tCanConfig.wChnlInter	= 0xffffffff;				//报文通道中断,Chnl1~32
	csi_can_init(CAN0, &tCanConfig);
	
	//CAN TX CONFIG
	for(i = CAN_CH1; i < CAN_CH3; i++)
	{
		//Ir
		tCanTxCfg.tIr.byIdmode		= CAN_STD_ID;		//报文ID模式，标准模式(11Bit)
		tCanTxCfg.tIr.byMsgDir		= CAN_DIR_SEND;		//报文方向
		tCanTxCfg.tIr.hwStdId 		= 0x700 + i;		//标准11BIT ID
		tCanTxCfg.tIr.wExtId		= 0x3FFFF;			//扩展ID,标准模式下忽略	
		//tx mcr
		tCanTxCfg.tMcr.bTxIeEn		= DISABLE;			//禁止报文发送中断(通道源中断)
		tCanTxCfg.tMcr.bRmtEn		= DISABLE;			//远程应答帧禁止，即接收到远程帧，不请求发送应答
		tCanTxCfg.tMcr.bTxReqEn		= DISABLE;			//报文对象请求发送禁止，
		tCanTxCfg.tMcr.byDataLen	= 0x08;				//发送数据数量
		//data a
		tCanTxCfg.tDataA.bydata[0] 	= i+0x11;			//数据A(低4字节数据配置)配置
		tCanTxCfg.tDataA.bydata[1] 	= i+0x12;
		tCanTxCfg.tDataA.bydata[2] 	= i+0x13;
		tCanTxCfg.tDataA.bydata[3] 	= i+0x14;
		//data b
		tCanTxCfg.tDataB.bydata[0] 	= i+0x21;			//数据B(高4字节数据配置)配置
		tCanTxCfg.tDataB.bydata[1] 	= i+0x22;
		tCanTxCfg.tDataB.bydata[2] 	= i+0x23;
		tCanTxCfg.tDataB.bydata[3] 	= i+0x24;

		csi_can_tx_config(CAN0, i, &tCanTxCfg);
	}
	
	csi_can_open(CAN0);
	
	//通道1发送
	csi_can_chnl_send(CAN0, CAN_CH1, 6);
	while(1)
	{
		if(csi_can_get_status_msg() & CAN_MSG_TXOK)		//可通过查询(TXOK状态)发送是否完成消息
		{
			csi_can_clr_status_msg(CAN_MSG_TXOK);		//清除消息
			wSdDelay = 0;
			break;
		}
		else 
		{
			if(wSdDelay >= 0x3fff)
			{
				wSdDelay = 0;
				break;
			}
			else
				wSdDelay++;
		}
	}
	
	//通道2发送
	csi_can_chnl_send(CAN0, CAN_CH2, 7);
	while(1)
	{
		if(csi_can_get_status_msg() & CAN_MSG_TXOK)		//可通过查询(TXOK状态)发送是否完成消息
		{
			csi_can_clr_status_msg(CAN_MSG_TXOK);		//清除消息
			wSdDelay = 0;
			break;
		}
		else 
		{
			if(wSdDelay >= 0x3fff)
			{
				wSdDelay = 0;
				break;
			}
			else
				wSdDelay++;
		}
	}
	
	//更新报文中数据(DAR/DBR)
	byData[0] = 0x55;									//发送的数据
	byData[1] = 0x65;
	byData[2] = 0x75;
	byData[3] = 0x85;
	csi_can_set_data_a(CAN0, CAN_CH1, byData);			//updata DAR 寄存器
	
	byData[0] = 0x95;
	byData[1] = 0xA5;									//发送的数据
	byData[2] = 0xB5;
	byData[3] = 0xC5;
	csi_can_set_data_b(CAN0, CAN_CH2, byData);			//updata DAR 寄存器
	
	tCanTxCfg.tIr.byIdmode		= CAN_STD_ID;			//报文ID模式，标准模式(11Bit)
	tCanTxCfg.tIr.byMsgDir		= CAN_DIR_SEND;			//报文方向
	tCanTxCfg.tIr.hwStdId 		= 0x7e0;				//标准11BIT ID
	tCanTxCfg.tIr.wExtId		= 0x3FFFF;				//扩展ID,标准模式下忽略	
	csi_can_set_ir(CAN0, CAN_CH1, &tCanTxCfg.tIr);		//配置报文识别符
	
	
	for(i = 0; i< 8; i++)
	{
		if(i%2)
			csi_can_chnl_send(CAN0, CAN_CH1, 8);
		else
			csi_can_chnl_send(CAN0, CAN_CH2, 8);
		
		//
		while(1)
		{
			if(csi_can_get_status_msg() & CAN_MSG_TXOK)		//可通过查询(TXOK状态)发送是否完成消息
			{
				csi_can_clr_status_msg(CAN_MSG_TXOK);		//清除消息
				wSdDelay = 0;
				break;
			}
			else 
			{
				if(wSdDelay >= 0x3fff)
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

/** \brief can 发送demo，开启状态中断和报文通道中断，可通过具体的报文通道发送中断判断发送完成
 * 
 *  \param[in] none
 *  \return none
 */
int can_send_int_demo(void)
{
	int iRet = 0;
	uint8_t	byChNum, i = 0;
	uint32_t wSdDelay = 0x00;
	
	//volatile uint32_t wData = 0;
	csi_can_config_t 	 tCanConfig;					//CAN init config
	csi_can_tx_config_t  tCanTxCfg;						//CAN TX config
	
//	csi_pin_set_mux(PC4, PC4_CAN_RX);					//CAN RX管脚配置
//	csi_pin_set_mux(PC5, PC5_CAN_TX);					//CAN TX管脚配置
//	csi_pin_set_mux(PA11, PA11_CAN_RX);					//CAN RX管脚配置
//	csi_pin_set_mux(PA12, PA12_CAN_TX);					//CAN TX管脚配置
	csi_pin_set_mux(PB8, PB8_CAN_RX);					//CAN RX管脚配置
	csi_pin_set_mux(PB9, PB9_CAN_TX);					//CAN TX管脚配置
	
	//CAN MR 寄存器配置,即初始化配置
	tCanConfig.byClkSrc 	= CAN_CLKSRC_PCLK;			//CAN CLK = PCLK		
	tCanConfig.wBaudRate 	= CAN_BDR_500K;				//500k
	tCanConfig.hwStaInter 	= 0xff1e;					//状态中断全部使能	
	tCanConfig.wChnlInter	= 0xffffffff;				//报文通道中断,Chnl1~32
	csi_can_init(CAN0, &tCanConfig);
	
	//CAN TX CONFIG
	//CH1
	//Ir
	tCanTxCfg.tIr.byIdmode		= CAN_STD_ID;		//报文ID模式，标准模式(11Bit)
	tCanTxCfg.tIr.byMsgDir		= CAN_DIR_SEND;		//报文方向
	tCanTxCfg.tIr.hwStdId 		= 0x700;			//标准11BIT ID
	tCanTxCfg.tIr.wExtId		= 0x3FFFF;			//扩展ID,标准模式下忽略	
	//tx mcr
	tCanTxCfg.tMcr.bTxIeEn		= ENABLE;			//使能报文发送中断(通道源中断)
	tCanTxCfg.tMcr.bRmtEn		= DISABLE;			//远程应答帧禁止，即接收到远程帧，不请求发送应答
	tCanTxCfg.tMcr.bTxReqEn		= DISABLE;			//报文对象请求发送禁止，
	tCanTxCfg.tMcr.byDataLen	= 0x08;				//发送数据数量
	//data a
	tCanTxCfg.tDataA.bydata[0] 	= 0x11;				//数据A(低4字节数据配置)配置
	tCanTxCfg.tDataA.bydata[1] 	= 0x12;
	tCanTxCfg.tDataA.bydata[2] 	= 0x13;
	tCanTxCfg.tDataA.bydata[3] 	= 0x14;
	//data b
	tCanTxCfg.tDataB.bydata[0] 	= 0x21;				//数据B(高4字节数据配置)配置
	tCanTxCfg.tDataB.bydata[1] 	= 0x22;
	tCanTxCfg.tDataB.bydata[2] 	= 0x23;
	tCanTxCfg.tDataB.bydata[3] 	= 0x24;
	csi_can_tx_config(CAN0, CAN_CH1, &tCanTxCfg);
	
	//CH2
	//Ir
	tCanTxCfg.tIr.byIdmode		= CAN_EXT_ID;		//报文ID模式，扩展模式(29Bit)
	tCanTxCfg.tIr.byMsgDir		= CAN_DIR_SEND;		//报文方向
	tCanTxCfg.tIr.hwStdId 		= 0x703;			//标准11BIT ID
	tCanTxCfg.tIr.wExtId		= 0x2FFFF;			//扩展ID,标准模式下忽略	
	//tx mcr
	tCanTxCfg.tMcr.bTxIeEn		= ENABLE;			//使能报文发送中断(通道源中断)
	tCanTxCfg.tMcr.bRmtEn		= DISABLE;			//远程应答帧禁止，即接收到远程帧，不请求发送应答
	tCanTxCfg.tMcr.bTxReqEn		= DISABLE;			//报文对象请求发送禁止，
	tCanTxCfg.tMcr.byDataLen	= 0x08;				//发送数据数量
	//data a
	tCanTxCfg.tDataA.bydata[0] 	= 0x13;				//数据A(低4字节数据配置)配置
	tCanTxCfg.tDataA.bydata[1] 	= 0x14;
	tCanTxCfg.tDataA.bydata[2] 	= 0x15;
	tCanTxCfg.tDataA.bydata[3] 	= 0x16;
	//data b
	tCanTxCfg.tDataB.bydata[0] 	= 0x23;				//数据B(高4字节数据配置)配置
	tCanTxCfg.tDataB.bydata[1] 	= 0x24;
	tCanTxCfg.tDataB.bydata[2] 	= 0x25;
	tCanTxCfg.tDataB.bydata[3] 	= 0x26;
	csi_can_tx_config(CAN0, CAN_CH2, &tCanTxCfg);
	
	csi_can_open(CAN0);
	
	for(i = 0; i< 8; i++)
	{
		if(i%2)
			byChNum = CAN_CH1;
		else
			byChNum = CAN_CH2;
			
		csi_can_chnl_send(CAN0, byChNum, 8);
		
		//
		while(1)
		{
			if(csi_can_get_msg(CAN_MSG_SEND, byChNum) && (csi_can_get_status_msg() & CAN_MSG_TXOK))		//可通过查询发送通道中断、状态中断的消息
			{
				csi_can_clr_msg(CAN_MSG_SEND, byChNum);				//清除通道中断消息
				csi_can_clr_status_msg(CAN_MSG_TXOK);				//清除状态中断消息	
				wSdDelay = 0;
				break;
			}
			else 
			{
				if(wSdDelay >= 0x3fff)
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
	
//	csi_pin_set_mux(PC4, PC4_CAN_RX);					//CAN RX管脚配置
//	csi_pin_set_mux(PC5, PC5_CAN_TX);					//CAN TX管脚配置
//	csi_pin_set_mux(PA11, PA11_CAN_RX);					//CAN RX管脚配置
//	csi_pin_set_mux(PA12, PA12_CAN_TX);					//CAN TX管脚配置
	csi_pin_set_mux(PB8, PB8_CAN_RX);					//CAN RX管脚配置
	csi_pin_set_mux(PB9, PB9_CAN_TX);					//CAN TX管脚配置
	
	//CAN MR 寄存器配置,即初始化配置
	tCanConfig.byClkSrc 	= CAN_CLKSRC_PCLK;			//CAN CLK = PCLK		
	tCanConfig.wBaudRate 	= CAN_BDR_500K;				//500k
	tCanConfig.hwStaInter 	= CAN_INTSRC_STA_ALL;		//异常中断全部使能	
	tCanConfig.wChnlInter	= 0xFFFF;					//报文通道中断
	csi_can_init(CAN0, &tCanConfig);
	
	//CAN RX CONFIG
	//报文接收到CHI~CH3,ID为全匹配，匹配上的ID数据接收到对应的报文接收通道中
	for(i = CAN_CH1; i < CAN_CH4; i++)
	{
		//Ir
		tCanRxCfg.tIr.byIdmode		= CAN_STD_ID;		//报文ID模式，标准模式(11Bit)
		tCanRxCfg.tIr.byMsgDir		= CAN_DIR_RECV;		//报文方向
		tCanRxCfg.tIr.hwStdId		= 0x7F0+i;			//标准11BIT ID
		tCanRxCfg.tIr.wExtId		= 0x3FFFF;			//扩展ID,标准模式下忽略
		//Mskr
		tCanRxCfg.tMskr.byIdMdMsk	= CAN_STD_ID_MSK;	//ID模式(STD/EXT) MASK
		tCanRxCfg.tMskr.byIdDirMsk	= CAN_DIR_MSK_DIS;	//报文方向 MASK disable
		tCanRxCfg.tMskr.hwStdIdMsk  = 0x7F0;			//标准模式ID mask 
		tCanRxCfg.tMskr.wExtIdMsk	= 0x00000;			//扩展模式ID mask, 标准模式下忽略
		//rx mcr
		tCanRxCfg.tMcr.bMskEn		= DISABLE;			//禁止过滤接收mask
		tCanRxCfg.tMcr.bRxIeEn		= ENABLE;			//使能报文接收中断(通道源中断)，置位ITPAND
		tCanRxCfg.tMcr.bOverWrEn	= DISABLE;			//关闭报文覆盖模式	
		tCanRxCfg.tMcr.byDataLen	= 0x08;				//数据长度
		
		csi_can_rx_config(CAN0, i, &tCanRxCfg);
	}
	
	csi_can_recv_init(&tCanRecv[0], CAN_CH1, 3);		//接收数据结构体
	csi_can_open(CAN0);
	
	
	while(1)
	{
		
		for(i = 1; i < 3; i++)
		{
			if(csi_can_get_recv_msg() & CAN_MSG_CH(i))
			{
				iRet = csi_can_chnl_read(CAN0,byRxBuf, i);
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
	
//	csi_pin_set_mux(PC4, PC4_CAN_RX);					//CAN RX管脚配置
//	csi_pin_set_mux(PC5, PC5_CAN_TX);					//CAN TX管脚配置
//	csi_pin_set_mux(PA11, PA11_CAN_RX);					//CAN RX管脚配置
//	csi_pin_set_mux(PA12, PA12_CAN_TX);					//CAN TX管脚配置
	csi_pin_set_mux(PB8, PB8_CAN_RX);					//CAN RX管脚配置
	csi_pin_set_mux(PB9, PB9_CAN_TX);					//CAN TX管脚配置
	
	//CAN MR 寄存器配置,即初始化配置
	tCanConfig.byClkSrc 	= CAN_CLKSRC_PCLK;			//CAN CLK = PCLK		
	tCanConfig.wBaudRate 	= CAN_BDR_500K;				//500k
	tCanConfig.hwStaInter 	= CAN_INTSRC_STA_ALL;		//异常中断全部使能	
	tCanConfig.wChnlInter	= 0xFFFF;					//报文通道中断
	csi_can_init(CAN0, &tCanConfig);
	
	//CAN RX CONFIG
	for(i = CAN_CH1; i < CAN_CH4; i++)
	{
		//Ir
		tCanRxCfg.tIr.byIdmode		= CAN_STD_ID;		//报文ID模式，标准模式(11Bit)
		tCanRxCfg.tIr.byMsgDir		= CAN_DIR_RECV;		//报文方向
		tCanRxCfg.tIr.hwStdId		= 0x7F0+i;			//标准11BIT ID
		tCanRxCfg.tIr.wExtId		= 0x3FFFF;			//扩展ID,标准模式下忽略
		//Mskr
		tCanRxCfg.tMskr.byIdMdMsk	= CAN_STD_ID_MSK;	//ID模式(STD/EXT) MASK
		tCanRxCfg.tMskr.byIdDirMsk	= CAN_DIR_MSK_DIS;	//报文方向 MASK disable
		tCanRxCfg.tMskr.hwStdIdMsk  = 0x7F0;			//标准模式ID mask 
		tCanRxCfg.tMskr.wExtIdMsk	= 0x00000;			//扩展模式ID mask, 标准模式下忽略
		//rx mcr
		tCanRxCfg.tMcr.bMskEn		= DISABLE;			//使能过滤接收mask
		tCanRxCfg.tMcr.bRxIeEn		= ENABLE;			//使能报文接收中断(通道源中断)，置位ITPAND
		tCanRxCfg.tMcr.bOverWrEn	= DISABLE;			//关闭报文覆盖模式	
		tCanRxCfg.tMcr.byDataLen	= 0x08;				//数据长度
		
		if(i == CAN_CH3)
			tCanRxCfg.tMcr.bOverWrEn = ENABLE;			//最后一个报文 使能报文覆盖功能
			
		csi_can_rx_config(CAN0, i, &tCanRxCfg);
	}
	
	csi_can_recv_init(&tCanRecv[0], CAN_CH1, 3);		//接收数据结构体
	csi_can_open(CAN0);
	
	nop;
	
	while(1)
	{
		
		for(i = 1; i < 3; i++)
		{
			if(csi_can_get_recv_msg() & CAN_MSG_CH(i))
			{
				iRet = csi_can_chnl_read(CAN0,byRxBuf, i);
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
//__attribute__((weak)) 
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
			
			if(wStatus & CAN_ERWARNTR_INT)						//error passive warning transition interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_ERWARNTR_INT);	
			}
			//
			if(wStatus & CAN_ERPASSTR_INT)						//error passive transition interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_ERPASSTR_INT);
			}
			//
			if(wStatus & CAN_BUSOFFTR_INT)						//bus off transition interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_BUSOFFTR_INT);
			}
			//
			if(wStatus & CAN_ACTVT_INT)							//activity interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_ACTVT_INT);
			}
			//
			if(wStatus & CAN_RXOK_INT)							//successfully received a message interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_RXOK_INT);
			}
			//
			if(wStatus & CAN_TXOK_INT)							//successfully transmit a message interrupt.
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_TXOK_INT);
			}
			//
			if(wStatus & CAN_STUFF_INT)							//stuff error interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_STUFF_INT);
			}
			//
			if(wStatus & CAN_FORM_INT)							//form error interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_FORM_INT);
			}
			//
			if(wStatus & CAN_ACK_INT)							//acknowledge error interrupt.
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_ACK_INT);
			}
			//
			if(wStatus & CAN_BIT1_INT)							//bit to one error interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_BIT1_INT);
			}
			//
			if(wStatus & CAN_BIT0_INT)							//bit to zero error interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_BIT0_INT);
			}
			//
			if(wStatus & CAN_CRC_INT)							//CRC error interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_CRC_INT);
			}
			
			break;
		default:			
			//消息通道中断
			csp_can_set_tmr(ptCanBase, hwIntNum, 1, CAN_AMCR_MSK | CAN_CLRIT_MSK | CAN_TRND_MSK);	//Write If1 command request, clear NAWDATA and ITPND flag
			while(csp_can_get_sr(ptCanBase) & CAN_BUSY1_S);											//If1 Busy?	
			wStatus = csp_can_get_mcr(ptCanBase);													//Read If1 message control reg, Read first and clean up NAWDATA and ITPND
			
			if(wStatus & CAN_NEWDAT_MSK)															//NEWDAT flag == 1 receive msg
			{
				//receive message
				if(g_tCanTran.ptCanRecv)
				{
					byRecvPos = hwIntNum - g_tCanTran.byStrChnl;
					if(s_byRecvMode == 0)
					{
						//不使用FIFO缓存，数据接收报文不覆盖
						if((g_tCanTran.ptCanRecv + byRecvPos)->wRecvId != 0)			//判断接收BUF中ID是否为0(读的时候会清除)，BUF中有数据则不读入到BUF
							return;
					}
					else 
					{
						//判断接收BUF通道中是否有数据，有的话把数据接收到BUF下一个通道中，直到BUF中最后一个通道，
						//若BUF中都有数据，则覆盖到最后一个BUF通道中
						while(1)
						{
							if((g_tCanTran.ptCanRecv + byRecvPos)->wRecvId != 0)		//接收BUF通道中有数据
							{
								if(byRecvPos < (g_tCanTran.byChTolNum -1))
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
					
					(g_tCanTran.ptCanRecv + byRecvPos)->wRecvId =	wIrVal;				//ID
					(g_tCanTran.ptCanRecv + byRecvPos)->wRecvData[0] = csi_can_get_ifx(ptCanBase, hwIntNum, CAN_IFX_DAR);		//DATA_A
					(g_tCanTran.ptCanRecv + byRecvPos)->wRecvData[1] = csi_can_get_ifx(ptCanBase, hwIntNum, CAN_IFX_DBR);		//DATA_B
					(g_tCanTran.ptCanRecv + byRecvPos)->byDataLen = wStatus & 0x0f;												//DATA LEN
					(g_tCanTran.ptCanRecv + byRecvPos)->byChnlNum = hwIntNum;													//Channel Num
					
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