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

/** \brief can send test
 * 
 *  \param[in] none
 *  \return none
 */
int can_send_demo(void)
{
	int iRet = 0;
	uint8_t	i = 0;
	uint8_t byData[4];
	//volatile uint32_t wData = 0;
	csi_can_config_t 	 tCanConfig;					//CAN init config
	csi_can_tx_config_t  tCanTxCfg;						//CAN TX config
	
	csi_pin_set_mux(PC4, PC4_CAN_RX);					//CAN RX管脚配置
	csi_pin_set_mux(PC5, PC5_CAN_TX);					//CAN TX管脚配置
	
	//CAN MR 寄存器配置,即初始化配置
	tCanConfig.byClkSrc 	= CAN_CLKSRC_PCLK;			//CAN CLK = PCLK		
	tCanConfig.wBaudRate 	= CAN_BDR_200K;				//500k
	tCanConfig.hwStaInter 	= 0xff1e;					//状态中断全部使能	
	tCanConfig.wChnlInter	= 0xffffffff;				//报文通道中断,Chnl1~32
	csi_can_init(CAN0, &tCanConfig);
	
	//CAN TX CONFIG
	for(i = CAN_CH1; i < CAN_CH3; i++)
	{
		//Ir
		tCanTxCfg.tIr.byIdmode		= CAN_STD_ID;		//报文ID模式，标准模式(11Bit)
		tCanTxCfg.tIr.byMsgDir		= CAN_MSG_SEND;		//报文方向
		tCanTxCfg.tIr.hwStdId 		= 0x700 + i;		//标准11BIT ID
		tCanTxCfg.tIr.wExtId		= 0x3FFFF;			//扩展ID,标准模式下忽略	
		//tx mcr
		tCanTxCfg.tMcr.bTxIeEn		= ENABLE;			//使能报文发送中断(通道源中断)
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
	
	csi_can_chnl_send(CAN0, CAN_CH1, 6);
	udelay(400);
	csi_can_chnl_send(CAN0, CAN_CH2, 7);
	udelay(400);
	
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
	tCanTxCfg.tIr.byMsgDir		= CAN_MSG_SEND;			//报文方向
	tCanTxCfg.tIr.hwStdId 		= 0x7e0;				//标准11BIT ID
	tCanTxCfg.tIr.wExtId		= 0x3FFFF;				//扩展ID,标准模式下忽略	
	csi_can_set_ir(CAN0, CAN_CH1, &tCanTxCfg.tIr);		//配置报文识别符
	
	
	for(i = 0; i< 8; i++)
	{
		
		csi_can_chnl_send(CAN0, CAN_CH1, 8);
		udelay(400);
		csi_can_chnl_send(CAN0, CAN_CH2, 8);
		udelay(400);
	}
	
	return iRet;
}

/** \brief can send test
 * 
 *  \param[in] none
 *  \return none
 */
int can_recv_demo(void)
{
	int iRet = 0;
	uint8_t	i = 0;
	
	csi_can_config_t 	 tCanConfig;			//CAN init config
	csi_can_rx_config_t  tCanRxCfg;				//CAN RX config	
	
//	csi_pin_set_mux(PC4, PC4_CAN_RX);			//CAN RX管脚配置
//	csi_pin_set_mux(PC5, PC5_CAN_TX);			//CAN TX管脚配置
	csi_pin_set_mux(PA11, PA11_CAN_RX);			//CAN RX管脚配置
	csi_pin_set_mux(PA12, PA12_CAN_TX);			//CAN TX管脚配置
	
	//CAN MR 寄存器配置,即初始化配置
	tCanConfig.byClkSrc 	= CAN_CLKSRC_PCLK;		//CAN CLK = PCLK		
	tCanConfig.wBaudRate 	= CAN_BDR_200K;			//500k
	tCanConfig.hwStaInter 	= CAN_INTSRC_STA_ALL;	//异常中断全部使能	
	tCanConfig.wChnlInter	= 0xFFFF;				//报文通道中断
	csi_can_init(CAN0, &tCanConfig);
	
	//CAN RX CONFIG
	for(i = CAN_CH1; i < CAN_CH4; i++)
	{
		//Ir
		tCanRxCfg.tIr.byIdmode		= CAN_STD_ID;		//报文ID模式，标准模式(11Bit)
		tCanRxCfg.tIr.byMsgDir		= CAN_MSG_RECV;		//报文方向
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
	
	csi_can_open(CAN0);
	
	nop;
	
	while(1)
	{
		
		nop;
		udelay(200);
		nop;
	}
	return iRet;
}

extern uint8_t apt_can_post_msg(uint32_t wMsg, csi_can_msg_mode_e eMsgMode);

/** \brief can receive interrupt handle function
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \return none
 */ 
__attribute__((weak)) void can_irqhandler(csp_can_t *ptCanBase)
{
	uint8_t byRecvPos	= 0;
	volatile uint32_t wIrVal  = 0;
	volatile uint32_t wStatus = 0;
	volatile uint16_t hwIntNum = csp_can_get_hpir(ptCanBase);		//get interrupt pointer							
	
	switch(hwIntNum)		//receive handle
	{
		case 0x00:			//end handle, 
			break;
		case 0x8000:		//status change interrupt handle
			//
			wStatus = csp_can_get_sr(ptCanBase) & CAN_MSG_STATUS_ALL;
			apt_can_post_msg(wStatus, 0);
			
			if(wStatus & CAN_ERWARNTR_INT)					//error passive warning transition interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_ERWARNTR_INT);	
			}
			//
			if(wStatus & CAN_ERPASSTR_INT)					//error passive transition interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_ERPASSTR_INT);
			}
			//
			if(wStatus & CAN_BUSOFFTR_INT)					//bus off transition interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_BUSOFFTR_INT);
			}
			//
			if(wStatus & CAN_ACTVT_INT)						//activity interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_ACTVT_INT);
			}
			//
			if(wStatus & CAN_RXOK_INT)						//successfully received a message interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_RXOK_INT);
			}
			//
			if(wStatus & CAN_TXOK_INT)						//successfully transmit a message interrupt.
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_TXOK_INT);
			}
			//
			if(wStatus & CAN_STUFF_INT)						//stuff error interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_STUFF_INT);
			}
			//
			if(wStatus & CAN_FORM_INT)						//form error interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_FORM_INT);
			}
			//
			if(wStatus & CAN_ACK_INT)						//acknowledge error interrupt.
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_ACK_INT);
			}
			//
			if(wStatus & CAN_BIT1_INT)						//bit to one error interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_BIT1_INT);
			}
			//
			if(wStatus & CAN_BIT0_INT)						//bit to zero error interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_BIT0_INT);
			}
			//
			if(wStatus & CAN_CRC_INT)						//CRC error interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_CRC_INT);
			}
			
			break;
		default:			
			//message channel handle
			csp_can_set_tmr(ptCanBase, hwIntNum, 1, CAN_AMCR_MSK | CAN_CLRIT_MSK | CAN_TRND_MSK);	//Write If1 command request, clear NAWDATA and ITPND flag
			while(csp_can_get_sr(ptCanBase) & CAN_BUSY1_S);											//If1 Busy?	
			wStatus = csp_can_get_mcr(ptCanBase);													//Read If1 message control reg, Read first and clean up NAWDATA and ITPND
			
			//receive msg
			if(wStatus & CAN_NEWDAT_MSK)															//NEWDAT flag == 1 receive msg
			{
				if(g_tCanTran.ptCanRecv)
				{
				
					byRecvPos = hwIntNum - g_tCanTran.byStrChnl;
					if((g_tCanTran.ptCanRecv + byRecvPos)->wRecvId & (0x01ul << 31))
					{
						if(byRecvPos < (g_tCanTran.byChTolNum -1))
						{
							byRecvPos++;
							apt_can_post_msg((0x01ul << hwIntNum), 1);
						}
						else
							return;
					}
					else
						apt_can_post_msg((0x01ul << (hwIntNum - 1)), 1);
					
					
					wIrVal = csi_can_get_ifx(ptCanBase, hwIntNum, CAN_IFX_IR);
					if(wIrVal & CAN_XTD_MSK)														
						wIrVal &= (CAN_EXTID_MSK | CAN_EXTID_MSK);									//extid
					else
						wIrVal = (wIrVal & CAN_BASEID_MSK) >> 18;									//stdid
					
					(g_tCanTran.ptCanRecv + byRecvPos)->wRecvId =	wIrVal | (0x01ul << 31);									//ID
					(g_tCanTran.ptCanRecv + byRecvPos)->wRecvData[0] = csi_can_get_ifx(ptCanBase, hwIntNum, CAN_IFX_DAR);		//DATA_A
					(g_tCanTran.ptCanRecv + byRecvPos)->wRecvData[1] = csi_can_get_ifx(ptCanBase, hwIntNum, CAN_IFX_DBR);		//DATA_B
					(g_tCanTran.ptCanRecv + byRecvPos)->byDataLen = wStatus & 0x0f;												//DATA LEN
					(g_tCanTran.ptCanRecv + byRecvPos)->byChnlNum = hwIntNum;													//Channel Num
				}
			}
			break;
	}
}