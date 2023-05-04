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
#include "soc.h"
#include "sys_clk.h"
#include <drv/can.h>
#include <drv/pin.h>
#include <iostring.h>

#include "csp.h"

/* externs function--------------------------------------------------------*/

/* externs variablesr------------------------------------------------------*/

/* Private macro-----------------------------------------------------------*/


/* Private variablesr------------------------------------------------------*/

/** \brief can 发送测试
 * 
 *  \param[in] eCanChnl：起始通道，CAN_CH1~CAN_CH32 
 *  \param[in] byChnlNum：通道数，1~32
 *  \return iRet
 */
int can_send_test(csi_can_chnl_e eCanChnl, uint8_t byChnlNum)
{
	int iRet = 0;
	uint8_t	i = 0;
	uint8_t byData[4];
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
	tCanConfig.wBaudRate 	= CAN_BDR_200K;				//500k
	tCanConfig.bAuReTran 	= DISABLE;					//关闭自动重新传输(自动重发)
	tCanConfig.hwStaInter 	= CAN_INTSRC_STA_ALL;		//状态中断全部使能	
	tCanConfig.wChnlInter	= 0xffffffff;				//报文通道中断,Chnl1~32
	csi_can_init(CAN0, &tCanConfig);
	
	//CAN TX CONFIG
	for(i = eCanChnl; i < (eCanChnl + byChnlNum); i++)
	{
		//Ir
		tCanTxCfg.tIr.byIdmode		= CAN_STD_ID;		//报文ID模式，标准模式(11Bit)
		tCanTxCfg.tIr.byMsgDir		= CAN_MSG_SEND;		//报文方向
		tCanTxCfg.tIr.hwStdId 		= 0x700 + i;		//标准11BIT ID
		tCanTxCfg.tIr.wExtId		= 0x3FFFF;			//扩展ID,标准模式下忽略	
		//tx mcr
		tCanTxCfg.tMcr.bTxIeEn		= ENABLE;			//使能报文发送中断(通道源中断)，置位ITPAND
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
	
	for(i = eCanChnl; i < (eCanChnl + byChnlNum); i++)
	{
		csi_can_msg_valid_enable(CAN0, i, DISABLE);		//报文无效
		udelay(500);
	}
	
	//更新报文中数据(DAR/DBR)
	byData[0] = 0x50;									//发送的数据
	byData[1] = 0x60;
	byData[2] = 0x70;
	byData[3] = 0x80;
	csi_can_set_data_a(CAN0, eCanChnl, byData);			//updata DAR 寄存器
	
	byData[0] = 0x90;
	byData[1] = 0xA0;									//发送的数据
	byData[2] = 0xB0;
	byData[3] = 0xC0;
	csi_can_set_data_b(CAN0, eCanChnl+1, byData);		//updata DAR 寄存器
	
	tCanTxCfg.tIr.byIdmode		= CAN_EXT_ID;			//报文ID模式，标准模式(11Bit)
	tCanTxCfg.tIr.byMsgDir		= CAN_MSG_SEND;			//报文方向
	tCanTxCfg.tIr.hwStdId 		= 0x7e0;				//标准11BIT ID
	tCanTxCfg.tIr.wExtId		= 0x3FFFF;				//扩展ID,标准模式下忽略	
	csi_can_set_ir(CAN0, eCanChnl, &tCanTxCfg.tIr);		//配置报文识别符
	
	for(i = eCanChnl; i < (eCanChnl + byChnlNum); i++)
	{
		if(i%2)
			csi_can_chnl_send(CAN0, i, 6);
		else
			csi_can_chnl_send(CAN0, i, 5);
			
		udelay(500);
	}
	
	mdelay(5);
	
	for(i = eCanChnl; i < (eCanChnl + byChnlNum); i++)
	{
		csi_can_msg_valid_enable(CAN0, i, ENABLE);		//CH1报文有效
		udelay(500);
	}
	
	for(i = eCanChnl; i < (eCanChnl + byChnlNum); i++)
	{
		if(i%2)
			csi_can_chnl_send(CAN0, i, 7);
		else
			csi_can_chnl_send(CAN0, i, 6);
			
		mdelay(10);
	}
	
	//更新报文中数据(DAR/DBR)
	byData[0] = 0x55;									//发送的数据
	byData[1] = 0x65;
	byData[2] = 0x75;
	byData[3] = 0x85;
	csi_can_set_data_a(CAN0, eCanChnl, byData);			//updata DAR 寄存器
	
	byData[0] = 0x95;
	byData[1] = 0xA5;									//发送的数据
	byData[2] = 0xB5;
	byData[3] = 0xC5;
	csi_can_set_data_b(CAN0, eCanChnl+1, byData);		//updata DAR 寄存器
	
	tCanTxCfg.tIr.byIdmode		= CAN_EXT_ID;			//报文ID模式，标准模式(11Bit)
	tCanTxCfg.tIr.byMsgDir		= CAN_MSG_SEND;			//报文方向
	tCanTxCfg.tIr.hwStdId 		= 0x7e0;				//标准11BIT ID
	tCanTxCfg.tIr.wExtId		= 0x3FFFF;				//扩展ID,标准模式下忽略	
	csi_can_set_ir(CAN0, eCanChnl, &tCanTxCfg.tIr);		//配置报文识别符
	
	//while(1)
	{
		for(i = eCanChnl; i < (eCanChnl + byChnlNum); i++)
		{
			csi_can_chnl_send(CAN0, i, 8);
			udelay(500);
		}
		nop;
		mdelay(10);
	}
	
	return iRet;
}
 

/** \brief can 接收测试
 * 
 *  \param[in] eCanChnl：起始通道，CAN_CH1~CAN_CH32 
 *  \param[in] byChnlNum：通道数，1~32
 *  \param[in] byMskEn：ID接收过滤使能，0/1=使能/禁止
 *  \return iRet
 */
int can_recv_test(csi_can_chnl_e eCanChnl, uint8_t byChnlNum, uint8_t byMskEn)
{
	int iRet = 0;
	uint8_t	i = 0;
	
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
	tCanConfig.wBaudRate 	= CAN_BDR_200K;				//500k
	tCanConfig.bAuReTran 	= DISABLE;					//关闭自动重新传输(自动重发)
	tCanConfig.hwStaInter 	= CAN_INTSRC_STA_ALL;		//异常中断全部使能	
	tCanConfig.wChnlInter	= 0xFFFFFFFF;				//报文通道中断
	csi_can_init(CAN0, &tCanConfig);
	
	//CAN RX CONFIG
	for(i = eCanChnl; i < (eCanChnl + byChnlNum); i++)
	{
		//Ir
		tCanRxCfg.tIr.byIdmode		= CAN_STD_ID;		//报文ID模式，标准模式(11Bit)
		tCanRxCfg.tIr.byMsgDir		= CAN_MSG_RECV;		//报文方向
		tCanRxCfg.tIr.hwStdId		= 0x7F0+i-eCanChnl;	//标准11BIT ID
		tCanRxCfg.tIr.wExtId		= 0x3FFFF;			//扩展ID,标准模式下忽略
		
		//Mskr
		tCanRxCfg.tMskr.byIdMdMsk	= CAN_STD_ID_MSK;	//ID模式MASK，标准/扩展(STD/EXT)
		tCanRxCfg.tMskr.byIdDirMsk	= CAN_DIR_MSK_DIS;	//禁止报文方向MASK(MDIR MASK disable)，应用中不应使能
		tCanRxCfg.tMskr.hwStdIdMsk  = 0x7F0;			//标准模式ID mask 
		tCanRxCfg.tMskr.wExtIdMsk	= 0x0FFF0;			//扩展模式ID mask, 标准模式下忽略
		
		//rx mcr
		if(byMskEn)
			tCanRxCfg.tMcr.bMskEn	= ENABLE;			//禁止ID接收过滤mask
		else
			tCanRxCfg.tMcr.bMskEn	= DISABLE;			//使能ID接收过滤mask
			
		tCanRxCfg.tMcr.bRxIeEn		= ENABLE;			//使能报文接收中断(通道源中断)，置位ITPAND
		tCanRxCfg.tMcr.bOverWrEn	= DISABLE;			//关闭报文覆盖模式	
		tCanRxCfg.tMcr.byDataLen	= 0x08;				//数据长度
		
		if(i == (eCanChnl + byChnlNum - 1))
			tCanRxCfg.tMcr.bOverWrEn = ENABLE;			//最后一个报文 使能报文覆盖功能,配置为接收FIFO缓存
			
		csi_can_rx_config(CAN0, i, &tCanRxCfg);
	}
	
	csi_can_open(CAN0);
	
	while(1)
	{
		nop;
		mdelay(5);
		nop;
	}
	return iRet;
}
/** \brief can 远程帧测试，包括远程帧发送与接收
 * 
 *  \param[in] none
 *  \return none
 */
int can_remote_frames_test(void)
{
	int iRet = 0;
	uint8_t	i = 0;
	//uint32_t wTmrCmd = 0;
	volatile uint32_t wData = 0;
	
	csi_can_config_t 	 tCanConfig;					//CAN init config
	csi_can_rx_config_t  tCanRxCfg;						//CAN RX config	
	csi_can_tx_config_t  tCanTxCfg;						//CAN TX config	 
	
	csi_pin_set_mux(PA11, PA11_CAN_RX);					//CAN RX管脚配置
	csi_pin_set_mux(PA12, PA12_CAN_TX);					//CAN TX管脚配置
	
	//CAN MR 寄存器配置,即初始化配置
	tCanConfig.byClkSrc 	= CAN_CLKSRC_PCLK;			//CAN CLK = PCLK		
	tCanConfig.wBaudRate 	= CAN_BDR_200K;				//200k
	tCanConfig.bAuReTran 	= DISABLE;					//关闭自动重新传输(自动重发)
	tCanConfig.hwStaInter 	= CAN_INTSRC_STA_ALL;		//异常中断	
	tCanConfig.wChnlInter	= 0x0fff;					//报文通道中断
	csi_can_init(CAN0, &tCanConfig);
	
	//CAN RX CONFIG
	for(i = CAN_CH3; i < CAN_CH6; i++)					//CH3~CH5接收
	{
		//Ir
		tCanRxCfg.tIr.byIdmode		= CAN_STD_ID;		//报文ID模式，标准模式(11Bit)
		tCanRxCfg.tIr.byMsgDir		= CAN_MSG_RECV;		//报文方向
		tCanRxCfg.tIr.hwStdId		= 0x7F0+i-CAN_CH3;	//标准11BIT ID
		tCanRxCfg.tIr.wExtId		= 0x3FFFF;			//扩展ID,标准模式下忽略
		//Mskr
		tCanRxCfg.tMskr.byIdMdMsk	= CAN_EXT_ID_MSK;	//ID模式MASK，标准/扩展(STD/EXT)
		tCanRxCfg.tMskr.byIdDirMsk	= CAN_DIR_MSK_DIS;	//禁止报文方向MASK(MDIR MASK disable)，应用中不应使能
		tCanRxCfg.tMskr.hwStdIdMsk  = 0x7F0;			//标准模式ID mask 
		tCanRxCfg.tMskr.wExtIdMsk	= 0x3FFF0;			//扩展模式ID mask, 标准模式下忽略
		//rx mcr
		tCanRxCfg.tMcr.bMskEn		= ENABLE;			//使能过滤接收mask
		tCanRxCfg.tMcr.bRxIeEn		= ENABLE;			//RX中断使能，置位ITPAND
		tCanRxCfg.tMcr.bOverWrEn	= DISABLE;			//关闭报文覆盖模式	
		tCanRxCfg.tMcr.byDataLen	= 0x08;				//数据长度
		
		if(i == CAN_CH5)
		{
			tCanRxCfg.tMcr.bOverWrEn = ENABLE;			//最后一个报文 使能报文覆盖功能
		}
			
		csi_can_rx_config(CAN0, i, &tCanRxCfg);
	}
	
	//CAN TX CONFIG
	for(i = CAN_CH1; i < CAN_CH3; i++)					//CH1~2
	{
		//Ir
		tCanTxCfg.tIr.byIdmode		= CAN_STD_ID;		//报文ID模式，标准模式(11Bit)
		tCanTxCfg.tIr.byMsgDir		= CAN_MSG_SEND;		//报文方向
		tCanTxCfg.tIr.hwStdId 		= 0x7F0+i-CAN_CH1;	//标准11BIT ID
		tCanTxCfg.tIr.wExtId		= 0x3FFFF;			//扩展ID,标准模式下忽略	
		//tx mcr
		tCanTxCfg.tMcr.bTxIeEn		= DISABLE;			//使能报文发送ITPAND（报文发送通道中断）
		tCanTxCfg.tMcr.bRmtEn		= ENABLE;			//远程帧使能，即接收到远程帧数据请求，发送数据
		tCanTxCfg.tMcr.bTxReqEn		= DISABLE;			//报文对象请求发送禁止，
		tCanTxCfg.tMcr.byDataLen	= 0x08;				//发送数据数量
		//data a
		tCanTxCfg.tDataA.bydata[0] 	= i+0x11;			//数据A(低4字节数据配置)配置
		tCanTxCfg.tDataA.bydata[1] 	= i+0x12;
		tCanTxCfg.tDataA.bydata[2] 	= i+0x13;
		tCanTxCfg.tDataA.bydata[3] 	= i+0x14;
		//data b
		tCanTxCfg.tDataB.bydata[0] 	= i+0x21;			//数据A(高4字节数据配置)配置
		tCanTxCfg.tDataB.bydata[1] 	= i+0x22;
		tCanTxCfg.tDataB.bydata[2] 	= i+0x23;
		tCanTxCfg.tDataB.bydata[3] 	= i+0x24;

		csi_can_tx_config(CAN0, i, &tCanTxCfg);
	}
	
	
	//CAN RX CONFIG
	for(i = CAN_CH6; i < CAN_CH9; i++)					//CH6~CH8
	{
		//Ir
		tCanRxCfg.tIr.byIdmode		= CAN_STD_ID;		//报文ID模式，标准模式(11Bit)
		tCanRxCfg.tIr.byMsgDir		= CAN_MSG_RECV;		//报文方向
		tCanRxCfg.tIr.hwStdId		= 0x120+i-CAN_CH6;	//标准11BIT ID
		tCanRxCfg.tIr.wExtId		= 0x3FFFF;			//扩展ID,标准模式下忽略
		//Mskr
		tCanRxCfg.tMskr.byIdMdMsk	= CAN_EXT_ID_MSK;	//ID 模式(STD/EXT) MASK disable	
		tCanRxCfg.tMskr.byIdDirMsk	= CAN_DIR_MSK_DIS;	//报文方向 MASK disable
		tCanRxCfg.tMskr.hwStdIdMsk  = 0x7f0;			//标准模式ID mask 
		tCanRxCfg.tMskr.wExtIdMsk	= 0x3FFF0;			//扩展模式ID mask, 标准模式下忽略
		//rx mcr
		tCanRxCfg.tMcr.bMskEn		= ENABLE;			//使能过滤接收mask
		tCanRxCfg.tMcr.bRxIeEn		= ENABLE;			//RX中断使能，置位ITPAND
		tCanRxCfg.tMcr.bOverWrEn	= DISABLE;			//关闭报文覆盖模式	
		tCanRxCfg.tMcr.byDataLen	= 0x08;				//数据长度
	
		
		if(i == CAN_CH8)
		{
			tCanRxCfg.tMcr.bOverWrEn = ENABLE;			//最后一个报文 使能报文覆盖功能
		}
			
		csi_can_rx_config(CAN0, i, &tCanRxCfg);
		
		//wData = csi_can_get_ifx(CAN0,CAN_CH8, CAN_IFX_MCR);
	}
	
	//CAN TX CONFIG
	for(i = CAN_CH9; i < CAN_CH12; i++)					//CH9~CH10
	{
		//Ir
		tCanTxCfg.tIr.byIdmode		= CAN_STD_ID;		//报文ID模式，标准模式(11Bit)
		tCanTxCfg.tIr.byMsgDir		= CAN_MSG_SEND;		//报文方向
		tCanTxCfg.tIr.hwStdId 		= 0x120+i-CAN_CH9;	//标准11BIT ID
		tCanTxCfg.tIr.wExtId		= 0x3FFFF;			//扩展ID,标准模式下忽略	
		//tx mcr
		tCanTxCfg.tMcr.bTxIeEn		= DISABLE;			//使能报文发送ITPAND（报文发送通道中断）
		tCanTxCfg.tMcr.bRmtEn		= ENABLE;			//远程帧使能，即接收到远程帧数据请求，发送数据
		tCanTxCfg.tMcr.bTxReqEn		= DISABLE;			//报文对象请求发送禁止，
		tCanTxCfg.tMcr.byDataLen	= 0x08;				//发送数据数量
		//data a
		tCanTxCfg.tDataA.bydata[0] 	= i+0x31;			//数据A(低4字节数据配置)配置
		tCanTxCfg.tDataA.bydata[1] 	= i+0x32;
		tCanTxCfg.tDataA.bydata[2] 	= i+0x33;
		tCanTxCfg.tDataA.bydata[3] 	= i+0x34;
		//data b
		tCanTxCfg.tDataB.bydata[0] 	= i+0x41;			//数据A(高4字节数据配置)配置
		tCanTxCfg.tDataB.bydata[1] 	= i+0x42;
		tCanTxCfg.tDataB.bydata[2] 	= i+0x43;
		tCanTxCfg.tDataB.bydata[3] 	= i+0x44;

		csi_can_tx_config(CAN0, i, &tCanTxCfg);
	}
	
	csi_can_open(CAN0);
	
	
	//远程帧(数据请求)，即发送远程帧
	{
		csi_can_chnl_send(CAN0, CAN_CH6, 8);
		udelay(500);
		csi_can_chnl_send(CAN0, CAN_CH7, 8);
		udelay(500);
//		csi_can_chnl_send(CAN0, CAN_CH8, 8);
//		udelay(500);
		
	}
	
	wData = csi_can_get_ifx(CAN0,CAN_CH6, CAN_IFX_MCR);
	wData = csi_can_get_ifx(CAN0,CAN_CH7, CAN_IFX_MCR);
	wData = csi_can_get_ifx(CAN0,CAN_CH8, CAN_IFX_MCR);
	my_printf("Can Ch8 Mcr Val: %d \n", wData);
	
	//发送数据帧
	{
		csi_can_chnl_send(CAN0, CAN_CH9, 8);
		udelay(500);
		csi_can_chnl_send(CAN0, CAN_CH10, 8);
		udelay(500);
		csi_can_chnl_send(CAN0, CAN_CH11, 8);
	}
	
	//远程帧(数据回复),即回复
//	csi_can_chnl_send(CAN0, CAN_CH5, 8);
//	udelay(200);
//	wData = csi_can_get_ifx(CAN0,CAN_CH5, CAN_IFX_MCR);
//	
//	
//	csi_can_chnl_send(CAN0, CAN_CH4, 8);
//	udelay(200);
//	wData = csi_can_get_ifx(CAN0,CAN_CH4, CAN_IFX_MCR);
//	nop;
	
	while(1)
	{
		nop;
		udelay(500);
		nop;
	}
	
	return iRet;
	
}

/** \brief can 基础模式测试，CAN脱离报文RAM运行
 * 
 *  \param[in] none
 *  \return none
 */
int can_basic_mode_test(void)
{
	
	int iRet = 0;
	uint8_t i;
	csi_can_config_t 	 tCanConfig;					//CAN init config
	csi_can_tx_config_t  tCanTxCfg;						//CAN TX(If0) config	
	csi_can_rx_config_t  tCanRxCfg;						//CAN RX(If1) config	
		
//	csi_pin_set_mux(PA11, PA11_CAN_RX);					//CAN RX管脚配置
//	csi_pin_set_mux(PA12, PA12_CAN_TX);					//CAN TX管脚配置	
	csi_pin_set_mux(PB8, PB8_CAN_RX);					//CAN RX管脚配置
	csi_pin_set_mux(PB9, PB9_CAN_TX);					//CAN TX管脚配置
	
	//CAN MR 寄存器配置,即初始化配置
	tCanConfig.byClkSrc 	= CAN_CLKSRC_PCLK;			//CAN CLK = PCLK		
	tCanConfig.wBaudRate 	= CAN_BDR_200K;				//200k
	tCanConfig.bAuReTran 	= DISABLE;					//关闭自动重新传输(自动重发)
	tCanConfig.hwStaInter 	= CAN_INTSRC_STA_ALL;		//异常中断全部使能	
	tCanConfig.wChnlInter	= CAN_INTSRC_NONE;			//报文通道中断
	csi_can_init(CAN0, &tCanConfig);
	
	csp_can_test_mode(CAN0, CAN_MODE_BASIC);			//配置为基础模式
	
	//发送配置
	{
		tCanTxCfg.tIr.byIdmode		= CAN_STD_ID;		//报文ID模式，标准模式(11Bit)
		tCanTxCfg.tIr.byMsgDir		= CAN_MSG_SEND;		//报文方向
		tCanTxCfg.tIr.hwStdId 		= 0x7F2;			//标准11BIT ID
		tCanTxCfg.tIr.wExtId		= 0x3FFF0;			//扩展ID,标准模式下忽略	
		//tx mcr
		tCanTxCfg.tMcr.bTxIeEn		= DISABLE;			//使能报文发送ITPAND（报文发送通道中断）
		tCanTxCfg.tMcr.bRmtEn		= ENABLE;			//远程帧禁止，即接收到远程帧，不请求发送
		tCanTxCfg.tMcr.bTxReqEn		= DISABLE;			//报文对象请求发送禁止，
		tCanTxCfg.tMcr.byDataLen	= 0x08;				//发送数据数量
		//data a
		tCanTxCfg.tDataA.bydata[0] 	= 0x11;				//数据A(低4字节数据配置)配置
		tCanTxCfg.tDataA.bydata[1] 	= 0x12;
		tCanTxCfg.tDataA.bydata[2] 	= 0x13;
		tCanTxCfg.tDataA.bydata[3] 	= 0x14;
		//data b
		tCanTxCfg.tDataB.bydata[0] 	= 0x21;				//数据A(高4字节数据配置)配置
		tCanTxCfg.tDataB.bydata[1] 	= 0x22;
		tCanTxCfg.tDataB.bydata[2] 	= 0x23;
		tCanTxCfg.tDataB.bydata[3] 	= 0x24;
		
		csp_can_set_adata(CAN0, (uint8_t *)tCanTxCfg.tDataA.bydata);	
		csp_can_set_bdata(CAN0, (uint8_t *)tCanTxCfg.tDataB.bydata);	
		csp_can_ir_config(CAN0,  tCanTxCfg.tIr.wExtId, tCanTxCfg.tIr.hwStdId, CAN_MDIR_SEND, tCanTxCfg.tIr.byIdmode, CAN_MSGVAL_INVALID);
		csp_can_set_mcr_wr(CAN0, tCanTxCfg.tMcr.byDataLen, tCanTxCfg.tMcr.bTxReqEn, tCanTxCfg.tMcr.bTxIeEn, tCanTxCfg.tMcr.bRmtEn);
		
		//csi_can_tx_config(CAN0, CAN_CH1, &tCanTxCfg);
	}
	
	csi_can_open(CAN0);
	
	//发送
	for(i = 0; i < 15; i++)
	{
		csp_can_cr_cmd(CAN0, CAN_RQBTX);
		udelay(500);
	}
	
	//接收配置
	{
		tCanRxCfg.tIr.byIdmode		= CAN_STD_ID;		//报文ID模式，标准模式(11Bit)
		tCanRxCfg.tIr.byMsgDir		= CAN_MSG_RECV;		//报文方向
		tCanRxCfg.tIr.hwStdId		= 0x7F0;			//标准11BIT ID
		tCanRxCfg.tIr.wExtId		= 0x3FFFF;			//扩展ID,标准模式下忽略
		//Mskr
		tCanRxCfg.tMskr.byIdMdMsk	= CAN_EXT_ID_MSK;	//ID 模式(STD/EXT) MASK disable	
		tCanRxCfg.tMskr.byIdDirMsk	= CAN_DIR_MSK_DIS;	//报文方向 MASK disable
		tCanRxCfg.tMskr.hwStdIdMsk = 0x7F0;				//标准模式ID mask 
		tCanRxCfg.tMskr.wExtIdMsk	= 0x3FFF0;			//扩展模式ID mask, 标准模式下忽略
		//rx mcr
		tCanRxCfg.tMcr.bMskEn		= DISABLE;			//使能过滤接收mask
		tCanRxCfg.tMcr.bRxIeEn		= ENABLE;			//RX中断使能，置位ITPAND
		tCanRxCfg.tMcr.bOverWrEn	= DISABLE;			//关闭报文覆盖模式	
		tCanRxCfg.tMcr.byDataLen	= 0x08;				//数据长度

		csp_can_ir_config(CAN0, tCanRxCfg.tIr.wExtId, tCanRxCfg.tIr.hwStdId, CAN_MDIR_RECV, tCanRxCfg.tIr.byIdmode, CAN_MSGVAL_INVALID);
		csp_can_mskr_config(CAN0, tCanRxCfg.tMskr.wExtIdMsk, tCanRxCfg.tMskr.hwStdIdMsk, CAN_MMDIR_DIS, tCanRxCfg.tMskr.byIdMdMsk);
		csp_can_set_mcr_rd(CAN0, tCanRxCfg.tMcr.byDataLen, tCanRxCfg.tMcr.bOverWrEn, tCanRxCfg.tMcr.bRxIeEn, tCanRxCfg.tMcr.bMskEn);
		
		//csi_can_rx_config(CAN0, CAN_CH1, &tCanRxCfg);
	}

	csp_can_cr_cmd(CAN0, CAN_STSR);						
	
	while(1)
	{
		nop;
		mdelay(10);
		csp_can_cr_cmd(CAN0, CAN_STSR);
	}
	
	return iRet;
	
}

/** \brief can 静默和回环模式测试
 * 
 *  \param[in] none
 *  \return none
 */
int can_lback_silent_mode_test(void)
{
	int iRet = 0;
	uint8_t i = 0;
	
	csi_can_config_t 	 tCanConfig;					//CAN init config			
    csi_can_tx_config_t  tCanTxCfg;						//CAN TX config	 
	csi_can_rx_config_t  tCanRxCfg;						//CAN RX config		 
	
	csi_pin_set_mux(PA11, PA11_CAN_RX);					//CAN RX管脚配置
	csi_pin_set_mux(PA12, PA12_CAN_TX);					//CAN TX管脚配置
	
	//CAN MR 寄存器配置,即初始化配置
	tCanConfig.byClkSrc 	= CAN_CLKSRC_PCLK;			//CAN CLK = PCLK		
	tCanConfig.wBaudRate 	= CAN_BDR_200K;				//200k
	tCanConfig.bAuReTran 	= DISABLE;					//关闭自动重新传输(自动重发)
	tCanConfig.hwStaInter 	= CAN_INTSRC_STA_ALL;		//状态中断	
	tCanConfig.wChnlInter	= 0x0fff;					//报文通道中断
	csi_can_init(CAN0, &tCanConfig);
	
	//两个都使能，静默+回环模式
	csp_can_test_mode(CAN0, CAN_MODE_SILENT);			//静默模式
	csp_can_test_mode(CAN0, CAN_MODE_LBACK);			//回环模式
	
	//CAN TX CONFIG
	for(i = CAN_CH1; i < CAN_CH3; i++)					//CH1~2
	{
		//Ir
		tCanTxCfg.tIr.byIdmode		= CAN_STD_ID;		//报文ID模式，标准模式(11Bit)
		tCanTxCfg.tIr.byMsgDir		= CAN_MSG_SEND;		//报文方向
		tCanTxCfg.tIr.hwStdId 		= 0x7F0+i-CAN_CH1;	//标准11BIT ID
		tCanTxCfg.tIr.wExtId		= 0x3FFFF;			//扩展ID,标准模式下忽略	
		//tx mcr
		tCanTxCfg.tMcr.bTxIeEn		= ENABLE;			//使能报文发送ITPAND（报文发送通道中断）
		tCanTxCfg.tMcr.bRmtEn		= DISABLE;			//远程帧禁止
		tCanTxCfg.tMcr.bTxReqEn		= DISABLE;			//报文对象请求发送禁止，
		tCanTxCfg.tMcr.byDataLen	= 0x08;				//发送数据数量
		//data a
		tCanTxCfg.tDataA.bydata[0] 	= i+0x41;			//数据A(低4字节数据配置)配置
		tCanTxCfg.tDataA.bydata[1] 	= i+0x42;
		tCanTxCfg.tDataA.bydata[2] 	= i+0x43;
		tCanTxCfg.tDataA.bydata[3] 	= i+0x44;
		//data b
		tCanTxCfg.tDataB.bydata[0] 	= i+0x51;			//数据A(高4字节数据配置)配置
		tCanTxCfg.tDataB.bydata[1] 	= i+0x52;
		tCanTxCfg.tDataB.bydata[2] 	= i+0x53;
		tCanTxCfg.tDataB.bydata[3] 	= i+0x54;

		csi_can_tx_config(CAN0, i, &tCanTxCfg);
	}
	
	//CAN RX CONFIG
	for(i = CAN_CH3; i < CAN_CH6; i++)					//CH3~CH5接收
	{
		//Ir
		tCanRxCfg.tIr.byIdmode		= CAN_STD_ID;		//报文ID模式，标准模式(11Bit)
		tCanRxCfg.tIr.byMsgDir		= CAN_MSG_RECV;		//报文方向
		tCanRxCfg.tIr.hwStdId		= 0x7F0+i-CAN_CH3;	//标准11BIT ID
		tCanRxCfg.tIr.wExtId		= 0x3FFFF;			//扩展ID,标准模式下忽略
		//Mskr
		tCanRxCfg.tMskr.byIdMdMsk	= CAN_EXT_ID_MSK;	//ID模式MASK，标准/扩展(STD/EXT)
		tCanRxCfg.tMskr.byIdDirMsk	= CAN_DIR_MSK_DIS;	//禁止报文方向MASK(MDIR MASK disable)，应用中不应使能
		tCanRxCfg.tMskr.hwStdIdMsk  = 0x7F0;			//标准模式ID mask 
		tCanRxCfg.tMskr.wExtIdMsk	= 0x3FFF0;			//扩展模式ID mask, 标准模式下忽略
		//rx mcr
		tCanRxCfg.tMcr.bMskEn		= ENABLE;			//使能过滤接收mask
		tCanRxCfg.tMcr.bRxIeEn		= ENABLE;			//RX中断使能，置位ITPAND
		tCanRxCfg.tMcr.bOverWrEn	= DISABLE;			//关闭报文覆盖模式	
		tCanRxCfg.tMcr.byDataLen	= 0x08;				//数据长度
		
		if(i == CAN_CH5)
		{
			tCanRxCfg.tMcr.bOverWrEn = ENABLE;			//最后一个报文 使能报文覆盖功能
		}
			
		csi_can_rx_config(CAN0, i, &tCanRxCfg);
	}
	
	csi_can_open(CAN0);
	
	csi_can_chnl_send(CAN0, CAN_CH1, 8);
	udelay(500);
	csi_can_chnl_send(CAN0, CAN_CH2, 8);
	udelay(500);

	
	while(1)
	{
		
		nop;
		mdelay(10);
	}
	
	return iRet;
	
}

/** \brief can 测试中断处理函数
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \return none
 */ 
__attribute__((weak)) void can_irqhandler(csp_can_t *ptCanBase)
{
	volatile uint16_t hwMcrVal = 0x00;
	volatile uint16_t hwIntNum = csp_can_get_hpir(ptCanBase);		//get interrupt pointer							
	volatile uint32_t wData[2] = {0x00 ,0x00};
	
	switch(hwIntNum)		//receive handle
	{
		case 0x00:			//end handle, 
			break;
		case 0x8000:		//status change interrupt handle
			//
			if(csp_can_get_sr(ptCanBase) & CAN_ERWARNTR_INT)		//error passive warning transition interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_ERWARNTR_INT);	
			}
			//
			if(csp_can_get_sr(ptCanBase) & CAN_ERPASSTR_INT)		//error passive transition interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_ERPASSTR_INT);
			}
			//
			if(csp_can_get_sr(ptCanBase) & CAN_BUSOFFTR_INT)		//bus off transition interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_BUSOFFTR_INT);
			}
			//
			if(csp_can_get_sr(ptCanBase) & CAN_ACTVT_INT)			//activity interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_ACTVT_INT);
			}
			//
			if(csp_can_get_sr(ptCanBase) & CAN_RXOK_INT)			//successfully received a message interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_RXOK_INT);
			}
			//
			if(csp_can_get_sr(ptCanBase) & CAN_TXOK_INT)			//successfully transmit a message interrupt.
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_TXOK_INT);
			}
			//
			if(csp_can_get_sr(ptCanBase) & CAN_STUFF_INT)			//stuff error interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_STUFF_INT);
			}
			//
			if(csp_can_get_sr(ptCanBase) & CAN_FORM_INT)			//form error interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_FORM_INT);
			}
			//
			if(csp_can_get_sr(ptCanBase) & CAN_ACK_INT)				//acknowledge error interrupt.
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_ACK_INT);
			}
			//
			if(csp_can_get_sr(ptCanBase) & CAN_BIT1_INT)			//bit to one error interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_BIT1_INT);
			}
			//
			if(csp_can_get_sr(ptCanBase) & CAN_BIT0_INT)			//bit to zero error interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_BIT0_INT);
			}
			//
			if(csp_can_get_sr(ptCanBase) & CAN_CRC_INT)				//CRC error interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_CRC_INT);
			}
			
			break;
		default:			//message channel handle

			csp_can_set_tmr(ptCanBase, hwIntNum, 1, CAN_AMCR_MSK | CAN_CLRIT_MSK | CAN_TRND_MSK);	//Write If1 command request, clear NAWDATA and ITPND flag
			while(csp_can_get_sr(ptCanBase) & CAN_BUSY1_S);											//If1 Busy?	
			hwMcrVal = csp_can_get_mcr(ptCanBase);													//Read If1 message control reg, Read first and clean up NAWDATA and ITPND
			
			if(hwMcrVal & CAN_NEWDAT_MSK)		//NEWDAT flag = 1 ? receive msg
			{
				wData[0] = csi_can_get_ifx(ptCanBase, hwIntNum, CAN_IFX_DAR);			//DATA A
				wData[1] = csi_can_get_ifx(ptCanBase, hwIntNum, CAN_IFX_DBR);			//DATA B
				
				my_printf("Can Receive Data0: %d \n", wData[0]);
				my_printf("Can Receive Data1: %d \n", wData[0]);
				
				hwMcrVal = csi_can_get_ifx(ptCanBase, hwIntNum, CAN_IFX_MCR);			//MCR
				hwMcrVal = csi_can_get_ifx(ptCanBase, hwIntNum, CAN_IFX_MSKR);			//MSKR
				hwMcrVal = csi_can_get_ifx(ptCanBase, hwIntNum, CAN_IFX_IR);			//IR
				nop;

			}
			
			//csp_can_clr_isr(ptCanBase, CAN_RXOK_INT);		//clear RXOK flag
			nop;
			break;
		
	}
}