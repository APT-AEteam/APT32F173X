/***********************************************************************//** 
 * \file  can_demo.c
 * \brief  CAN_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-07-19 <td>V0.0  <td>ZJY   <td>initial
 * <tr><td> 2021-09-27 <td>V0.1  <td>ZJY   <td>code normalization
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


#if (USE_CAN_CALLBACK == 0)	
	
static csi_can_recv_t s_tCanRecv[4];		//接收数据结构体缓存


/** \brief  can_int_handler: CAN中断服务函数
 * 
 *  \brief  CAN发生中断时会调用此函数，函数在interrupt.c里定义为弱(weak)属性，默认不做处理;用户用到中断
 * 			时，请重新定义此函数，在此函数中进行对应中断处理，也可直接在interrupt.c里的函数里进行处理。
 * 
 *  \brief  报文对象的中断，即报文通道中断(CH: 1~32)，序号越低优先级越高。
 * 			
 *  \param[in] none
 *  \return none
 */
ATTRIBUTE_ISR void can_int_handler(void) 
{
	volatile uint8_t byRecvPos = 0;
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
//			//
//			if(wStatus & CAN_INT_ERPASSTR)						//被动错误发送或者回复
//			{
//				csp_can_clr_isr(CAN0, CAN_INT_ERPASSTR);
//			}
//			......
			
			csp_can_clr_isr(CAN0, wStatus);						//清除所有中断	
					
			break;
		default:												//消息通道中断, CAN_CH1~CAN_CH32(channel: 1~32)		
			//读报文控制寄存器，读出后自动清除NAWDATA和ITPND位
			csp_can_set_tmr(CAN0, hwIntNum,1,CAN_AMCR_MSK | CAN_CLRIT_MSK | CAN_TRND_MSK);	//通过传输管理寄存器(TMR)写IF1命令请求，清除NAWDATA和ITPND标志 
			while(csp_can_get_sr(CAN0) & CAN_STA_BUSY1);									//查询IF1是否忙?
			wStatus = csp_can_get_mcr(CAN0);												//读报文控制寄存器(MCR)，读出后，自动清除MCR中的NAWDATA和ITPND标志
			
			if(wStatus & CAN_NEWDAT_MSK)													//如果NEWDAT标志 == 1，则为接收到完整报文的对应通道产生的中断
			{
				//获取接收数据存储的位置，即按照接收报文的通道号存储在接收BUF数组中，CH[3-6] -> BUF[0-2]
				//demo中将接收通道配置为CH3开始的通道，方便中断处理满足不同demo
				byRecvPos = hwIntNum - CAN_CH3;												//对应中断的通道号-接收报文的起始通道(请将接收报文配置在连续通道中)
				wIrVal = csi_can_get_ifx(CAN0, hwIntNum, CAN_IFX_IR);						//获取ID
				if(wIrVal & CAN_XTD_MSK)														
					wIrVal &= (CAN_EXTID_MSK | CAN_EXTID_MSK);								//扩展ID模式
				else
					wIrVal = (wIrVal & CAN_BASEID_MSK) >> 18;								//标准ID模式
				
				s_tCanRecv[byRecvPos].wRecvId =	wIrVal;										//ID
				s_tCanRecv[byRecvPos].byDataLen = wStatus & 0x0f;							//帧数据长度
				s_tCanRecv[byRecvPos].byChNum = hwIntNum;									//帧通道号
				s_tCanRecv[byRecvPos].wRecvData[0] = csi_can_get_ifx(CAN0, hwIntNum, CAN_IFX_DAR);		//DATA_A
				s_tCanRecv[byRecvPos].wRecvData[1] = csi_can_get_ifx(CAN0, hwIntNum, CAN_IFX_DBR);		//DATA_B
				
				//清除接收OK状态
				csp_can_clr_sr(CAN0, CAN_INT_RXOK);		
			}
			else 																			//如果NEWDAT标志 == 0，则为对应报文通道发送报文完成产生的中断
			{
				//用户添加处理
				//demo中将发送通道配置为CH1开始的通道，方便中断处理满足不同demo
				if(hwIntNum ==  CAN_CH1)
				{
					nop;
				}
				
				csp_can_clr_sr(CAN0, CAN_INT_TXOK);		//清除发送OK状态			
			}
			break;
	}
}

#endif

/** \brief	can_send_demo： CAN发送demo，不开启中断
 * 	\brief	CAN报文发送，报文数据可配置为1~8个字节；通过查询TXOK状态判断发送是否完成
 * 
 *  \param[in] none
 *  \return none
 */
int can_send_demo(void)
{
	int iRet = 0;
	uint8_t byData[4];
	uint32_t wSdDelay = 0x00;
	csi_can_config_t 	 tCanConfig;						//CAN 初始化结构体
	csi_can_tx_config_t  tCanTxConfig;						//CAN 发送报文配置结构体
	
#if (USE_GUI == 0)											//用户未选择图形化编程			
	csi_gpio_set_mux(GPIOA, PA11, PA11_CAN_RX);				//CAN RX管脚配置
	csi_gpio_set_mux(GPIOA, PA12, PA12_CAN_TX);				//CAN TX管脚配置
#endif

	//初始化配置
	tCanConfig.eClkSrc  = CAN_CLKSRC_PCLK;					//CAN CLK = PCLK		
	tCanConfig.wBaudRate = CAN_BDR_500K;					//500k
	tCanConfig.bAuReTran = DISABLE;							//自动重新传输关闭
	csi_can_init(CAN0, &tCanConfig);						//CAN初始化
	
	//CAN发送报文配置，每个报文配置在一个报文通道中(通道：CAN_CH1~CAN_CH32)
	//报文通道1配置(CAN_CH1)
	//id,识别符配置
	tCanTxConfig.tId.eIdMode	= CAN_ID_STD;				//报文ID模式，标准模式(11Bit)
	tCanTxConfig.tId.eMsgDir	= CAN_DIR_SEND;				//报文方向
	tCanTxConfig.tId.hwStdId 	= 0x701;					//标准11BIT ID
	tCanTxConfig.tId.wExtId		= 0x3FFFF;					//扩展ID,标准模式下忽略	
	//tx mcr 发送报文控制配置
	tCanTxConfig.tMc.bTxIeEn	= DISABLE;					//禁止报文发送中断(通道源中断)
	tCanTxConfig.tMc.bRmtEn		= DISABLE;					//远程应答帧禁止，即接收到远程帧，不请求发送应答
	tCanTxConfig.tMc.bTxReqEn	= DISABLE;					//报文对象请求发送禁止，
	tCanTxConfig.tMc.byDataLen	= 0x08;						//发送数据长度(数量)
	//data a
	tCanTxConfig.tDataA.bydata[0] 	= 0x11;					//数据A(低4字节数据配置)配置
	tCanTxConfig.tDataA.bydata[1] 	= 0x12;
	tCanTxConfig.tDataA.bydata[2] 	= 0x13;
	tCanTxConfig.tDataA.bydata[3] 	= 0x14;
	//data b
	tCanTxConfig.tDataB.bydata[0] 	= 0x21;					//数据B(高4字节数据配置)配置
	tCanTxConfig.tDataB.bydata[1] 	= 0x22;
	tCanTxConfig.tDataB.bydata[2] 	= 0x23;
	tCanTxConfig.tDataB.bydata[3] 	= 0x24;
	csi_can_set_msg_tx(CAN0, CAN_CH1, &tCanTxConfig);				//配置发送报文通道
	
	csi_can_start(CAN0);									//使能(打开)CAN模块
	
	//发送报文通道1的报文
	csi_can_msg_send(CAN0, CAN_CH1, 7);						//报文通道1中的报文，发送7字节数据	
	while(1)
	{
		if(csi_can_get_sr(CAN0) & CAN_STA_TXOK)				//可通过查询(TXOK状态)发送是否完成消息
		{
			csi_can_clr_sr(CAN0, CAN_STA_TXOK);				//清除消息
			wSdDelay = 0;
			break;
		}
		else 
		{	
			if(wSdDelay >= 0x3fff)							//发送状态查询超时处理
			{
				wSdDelay = 0;
				break;
			}
			else
				wSdDelay++;
		}
	}
	
	//报文配置完成后，可以单独更新报文中的元素(ID/dataA_B)
	//更新报文通道1中的报文数据
	byData[0] = 0x55;										//发送的数据
	byData[1] = 0x65;
	byData[2] = 0x75;
	byData[3] = 0x85;
	csi_can_set_adata(CAN0, CAN_CH1, byData);				//更新报文通道1中的低4字节数据
	
	byData[0] = 0x95;
	byData[1] = 0xA5;										//发送的数据
	byData[2] = 0xB5;
	byData[3] = 0xC5;
	csi_can_set_bdata(CAN0, CAN_CH1, byData);				//更新报文通道1中的高4字节数据
	
	//更新报文通道1中报文ID模式为扩展模式
	tCanTxConfig.tId.eIdMode	= CAN_ID_EXT;				//报文ID模式，扩展模式(29Bit)
	tCanTxConfig.tId.eMsgDir	= CAN_DIR_SEND;				//报文方向
	tCanTxConfig.tId.hwStdId 	= 0x7e0;					//标准11BIT ID
	tCanTxConfig.tId.wExtId		= 0x3FFFF;					//扩展ID,标准模式下忽略	
	csi_can_set_id(CAN0, CAN_CH1, &tCanTxConfig.tId);		//配置报文识别符
	
	//更新后的的报文发送
	csi_can_msg_send(CAN0, CAN_CH1, 8);						//报文通道1中的报文，发送8字节数据
	while(1)
	{
		if(csi_can_get_sr(CAN0) & CAN_STA_TXOK)				//可通过查询(TXOK状态)发送是否完成消息
		{
			csi_can_clr_sr(CAN0, CAN_STA_TXOK);				//清除消息
			wSdDelay = 0;
			break;
		}
		else 
		{
			if(wSdDelay >= 0x3fff)							//发送状态查询超时处理
			{
				wSdDelay = 0;
				break;
			}
			else
				wSdDelay++;
		}
	}
	
	return iRet;
}

/** \brief	can_send_int_demo CAN发送demo，开启中断
 * 
 * 	\brief	CAN报文发送使用中断(开启所有状态中断和对应发送报文通道中断)，报文数据长度可配置为1~8个字节；
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
	tCanConfig.eClkSrc 	= CAN_CLKSRC_PCLK;					//CAN CLK = PCLK		
	tCanConfig.wBaudRate 	= CAN_BDR_500K;					//500k
	tCanConfig.bAuReTran = DISABLE;							//自动重新传输关闭
	csi_can_init(CAN0, &tCanConfig);
	
	//id 识别符配置
	tCanTxConfig.tId.eIdMode		= CAN_ID_EXT;			//报文ID模式，扩展模式(11Bit)
	tCanTxConfig.tId.eMsgDir		= CAN_DIR_SEND;			//报文方向
	tCanTxConfig.tId.hwStdId 		= 0x701;				//标准11BIT ID
	tCanTxConfig.tId.wExtId			= 0x3FFFF;				//扩展ID,标准模式下忽略	
	
	//tx mcr 发送报文控制配置
	tCanTxConfig.tMc.bTxIeEn		= ENABLE;				//使能报文发送中断(通道源中断)
	tCanTxConfig.tMc.bRmtEn			= DISABLE;				//远程应答帧禁止，即接收到远程帧，不请求发送应答
	tCanTxConfig.tMc.bTxReqEn		= DISABLE;				//报文对象请求发送禁止，
	tCanTxConfig.tMc.byDataLen		= 0x08;					//发送数据长度(数量)
	
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
	csi_can_set_msg_tx(CAN0, CAN_CH1, &tCanTxConfig);		//配置发送报文

	csi_can_int_enable(CAN0, CAN_INTSRC_ERR_ALL);			//使能所有异常状态中断
	csi_can_ch_int_enable(CAN0, CAN_CH1);					//使能发送报文对应的报文通道中断
	csi_can_start(CAN0);									//使能(打开)CAN模块
		
	while(1)
	{
		csi_can_msg_send(CAN0, CAN_CH1, 8);					//报文通道1中的报文，发送8字节数据
		udelay(300);
	}
	
	return iRet;
}

/** \brief	can_receive_int_demo: CAN报文接收demo，开启报文通道中断
 * 
 * 	\brief	CAN报文接收使用中断(开启所有状态中断和对应发送报文通道中断)，在中断函数中处理报文接收
 * 
 *  \param[in] none
 *  \return none
 */
int can_receive_int_demo(void)
{
	int iRet = 0;
	csi_can_config_t 	 tCanConfig;						//CAN 初始化结构体
	csi_can_rx_config_t  tCanRxConfig;						//CAN 发送报文配置结构体

#if (USE_GUI == 0)											//用户未选择图形化编程			
	csi_gpio_set_mux(GPIOA, PA11, PA11_CAN_RX);				//CAN RX管脚配置
	csi_gpio_set_mux(GPIOA, PA12, PA12_CAN_TX);				//CAN TX管脚配置
#endif
	
	//CAN MR 寄存器配置,即初始化配置
	//初始化配置
	tCanConfig.eClkSrc  = CAN_CLKSRC_PCLK;					//CAN CLK = PCLK		
	tCanConfig.wBaudRate = CAN_BDR_500K;					//500k
	tCanConfig.bAuReTran = DISABLE;							//自动重新传输关闭
	csi_can_init(CAN0, &tCanConfig);						//CAN初始化
	
	//报文接收到CH3~CH4, ID为全匹配，匹配上的ID数据接收到对应的报文接收通道中
	//接收报文通道3配置(CAN_CH3)
	{
		//id,识别符配置
		tCanRxConfig.tId.eIdMode		= CAN_ID_STD;		//报文ID模式，标准模式(11Bit)
		tCanRxConfig.tId.eMsgDir		= CAN_DIR_RECV;		//报文方向
		tCanRxConfig.tId.hwStdId 		= 0x703;			//标准11BIT ID
		tCanRxConfig.tId.wExtId			= 0x3FFFF;			//扩展ID,标准模式下忽略	
		
		//rx mskr 接收报文mask配置(参与接收ID滤波)
		//报文控制配置中bMskEn不使能，此配置无意义
		//bMskEn使能，下面的配置：ID匹配范围为0x7F0~0x7FF
//		tCanRxConfig.tMsk.eIdMdMsk	 	= CAN_IDMSK_STD;	//ID模式(STD/EXT) MASK, 扩展位不参与接收滤波
//		tCanRxConfig.tMsk.eIdDirMsk 	= CAN_DIRMSK_DIS;	//报文方向位不参与接收滤波
//		tCanRxConfig.tMsk.hwStdIdMsk 	= 0x7F0;			//标准模式ID mask 
//		tCanRxConfig.tMsk.wExtIdMsk	 	= 0x00000;			//扩展模式ID mask, 标准模式下忽略
		
		//rx mcr 接收报文控制配置
		tCanRxConfig.tMc.bMskEn			= DISABLE;			//禁止过滤接收mask,即禁止接收滤波
		tCanRxConfig.tMc.bRxIeEn		= ENABLE;			//使能报文接收中断(通道源中断), 置位ITPAND; 若使用报文通道中断(源中断)，必须使能
		tCanRxConfig.tMc.bOverWrEn		= DISABLE;			//关闭报文覆盖模式	
		tCanRxConfig.tMc.byDataLen		= 0x08;				//报文数据长度，设置为最大，实际接收到的数据 <= 8
		csi_can_set_msg_rx(CAN0, CAN_CH3, &tCanRxConfig);	//配置接收报文
		
		csi_can_ch_int_enable(CAN0, CAN_CH3);				//使能对应报文接收通道中断
	}
	
	//接收报文通道4配置(CAN_CH4)
	{
		//id,识别符配置
		tCanRxConfig.tId.eIdMode		= CAN_ID_STD;		//报文ID模式，标准模式(11Bit)
		tCanRxConfig.tId.eMsgDir		= CAN_DIR_RECV;		//报文方向
		tCanRxConfig.tId.hwStdId 		= 0x704;			//标准11BIT ID
		tCanRxConfig.tId.wExtId			= 0x3FFFF;			//扩展ID,标准模式下忽略	
		
		//rx mskr 接收报文mask配置(参与接收ID滤波)
		//报文控制配置中bMskEn不使能，此配置无意义
		//bMskEn使能，下面的配置：ID匹配范围为0x7F0~0x7FF
//		tCanRxConfig.tMsk.eIdMdMsk	 	= CAN_IDMSK_STD;	//ID模式(STD/EXT) MASK, 扩展位不参与接收滤波
//		tCanRxConfig.tMsk.eIdDirMsk 	= CAN_DIRMSK_DIS;	//报文方向位不参与接收滤波
//		tCanRxConfig.tMsk.hwStdIdMsk 	= 0x7F0;			//标准模式ID mask 
//		tCanRxConfig.tMsk.wExtIdMsk	 	= 0x00000;			//扩展模式ID mask, 标准模式下忽略
		
		//rx mcr 接收报文控制配置
		tCanRxConfig.tMc.bMskEn			= DISABLE;			//禁止过滤接收mask,即禁止接收滤波
		tCanRxConfig.tMc.bRxIeEn		= ENABLE;			//使能报文接收中断(通道源中断), 置位ITPAND; 若使用报文通道中断(源中断)，必须使能
		tCanRxConfig.tMc.bOverWrEn		= DISABLE;			//关闭报文覆盖模式	
		tCanRxConfig.tMc.byDataLen		= 0x08;				//报文数据长度，设置为最大，实际接收到的数据 <= 8
		csi_can_set_msg_rx(CAN0, CAN_CH4, &tCanRxConfig);	//配置接收报文
		
		csi_can_ch_int_enable(CAN0, CAN_CH4);				//使能对应报文接收通道中断
	}
	
	csi_can_int_enable(CAN0, CAN_INTSRC_ERR_ALL);			//使能所有异常状态中断
	csi_can_start(CAN0);									//使能(打开)CAN模块
	
	while(1)
	{
		mdelay(10);
	}
	
	return iRet;
}

/** \brief	can_receive_fifo_init_demo: CAN报文接收FIFO缓存demo，接收使用中断，使用ID接收滤波
 * 
 * 	\brief	两个或者多个连续的报文通道配置为接收报文通道，可组成一个接收报文FIFO缓存，该配置中最小序号的报文通道作为FIFO
 * 			缓存区的第一个报文对象；接收FIFO中，报文对象的OVERWRITE需配置为0，最后一个报文对象的OVERWRITE位应配置为1；如
 * 			果接收到的报文识别符(ID)匹配到一个FIFO缓存，将会被接收到报文FIFO缓存中。
 * 
 *  \brief 	MCR寄存器中UMASK位使能时(bMskEn=ENABLE)，选择接收ID过滤模式,可选择扩展和标准模式，ID过滤根据BASEMASK和EXTMASK
 * 			的配置来处理，若他们中的某一位为1，那么对应的ID位必须匹配，某一位为零时，对应的ID位不过滤，即对应的ID位可以为
 * 			0或者1。
 * 
 *  \brief  CAN报文接收FIFO缓存的配置必须和ID滤波配合使用，即UMASK位使能时(bMskEn=ENABLE)，报文接收FIFO功能才能实现，若ID
 * 			不使用过滤匹配，则报文接收FIFO不可用。
 * 
 *  \param[in] none
 *  \return none
 */
int can_receive_fifo_init_demo(void)
{
	int iRet = 0;
	csi_can_config_t 	 tCanConfig;						//CAN 初始化结构体
	csi_can_rx_config_t  tCanRxConfig;						//CAN 发送报文配置结构体
	
#if (USE_GUI == 0)											//用户未选择图形化编程			
	csi_gpio_set_mux(GPIOA, PA11, PA11_CAN_RX);				//CAN RX管脚配置
	csi_gpio_set_mux(GPIOA, PA12, PA12_CAN_TX);				//CAN TX管脚配置
#endif
	
	//CAN MR 寄存器配置,即初始化配置
	tCanConfig.eClkSrc 	= CAN_CLKSRC_PCLK;					//CAN CLK = PCLK		
	tCanConfig.wBaudRate 	= CAN_BDR_500K;					//500k
	tCanConfig.bAuReTran = DISABLE;							//自动重新传输关闭
	csi_can_init(CAN0, &tCanConfig);
	
	//报文接收到CH3~CH5, ID为滤波匹配，在一个范围内ID匹配(0x7F0~0x7FF)，数据接收到对应的报文接收通道中
	//接收报文通道3配置(CAN_CH3)
	{
		//id,识别符配置
		tCanRxConfig.tId.eIdMode		= CAN_ID_STD;		//报文ID模式，标准模式(11Bit)
		tCanRxConfig.tId.eMsgDir		= CAN_DIR_RECV;		//报文方向
		tCanRxConfig.tId.hwStdId 		= 0x703;			//标准11BIT ID
		tCanRxConfig.tId.wExtId			= 0x3FFFF;			//扩展ID,标准模式下忽略	
		
		//rx mskr 接收报文mask配置(参与接收滤波),
		//ID匹配范围：0x700~0x70f，即11bit的ID高7位必须匹配
		tCanRxConfig.tMsk.eIdMdMsk	 	= CAN_IDMSK_STD;	//ID模式(STD/EXT) MASK, 扩展位不参与接收滤波
		tCanRxConfig.tMsk.eIdDirMsk 	= CAN_DIRMSK_DIS;	//报文方向位不参与接收滤波
		tCanRxConfig.tMsk.hwStdIdMsk 	= 0x7F0;			//标准模式ID mask,即ID的高7位必须匹配，低4位不必匹配
		tCanRxConfig.tMsk.wExtIdMsk	 	= 0x3FFFF;			//扩展模式ID mask, 标准模式下忽略
		
		//rx mcr 接收报文控制配置
		tCanRxConfig.tMc.bMskEn			= ENABLE;			//使能过滤接收mask, 即使能ID接收滤波,ID匹配在某个范围内
		tCanRxConfig.tMc.bRxIeEn		= ENABLE;			//使能报文接收中断(通道源中断), 置位ITPAND; 若使用报文通道中断(源中断)，必须使能
		tCanRxConfig.tMc.bOverWrEn		= DISABLE;			//关闭报文覆盖模式	
		tCanRxConfig.tMc.byDataLen		= 0x08;				//报文数据长度，设置为最大，实际接收到的数据 <= 8
		csi_can_set_msg_rx(CAN0, CAN_CH3, &tCanRxConfig);	//配置接收报文
		
		csi_can_ch_int_enable(CAN0, CAN_CH3);				//使能对应报文接收通道中断
	}
	
	//接收报文通道4配置(CAN_CH4)
	{
		//id,识别符配置
		tCanRxConfig.tId.eIdMode		= CAN_ID_STD;		//报文ID模式，标准模式(11Bit)
		tCanRxConfig.tId.eMsgDir		= CAN_DIR_RECV;		//报文方向
		tCanRxConfig.tId.hwStdId 		= 0x704;			//标准11BIT ID
		tCanRxConfig.tId.wExtId			= 0x3FFFF;			//扩展ID,标准模式下忽略	
		
		//rx mskr 接收报文mask配置(参与接收滤波),
		//ID匹配范围：0x700~0x70f，即11bit的ID高7位必须匹配
		tCanRxConfig.tMsk.eIdMdMsk	 	= CAN_IDMSK_STD;	//ID模式(STD/EXT) MASK, 扩展位不参与接收滤波
		tCanRxConfig.tMsk.eIdDirMsk 	= CAN_DIRMSK_DIS;	//报文方向位不参与接收滤波
		tCanRxConfig.tMsk.hwStdIdMsk 	= 0x7F0;			//标准模式ID mask,即ID的高7位必须匹配，低4位不必匹配
		tCanRxConfig.tMsk.wExtIdMsk	 	= 0x3FFFF;			//扩展模式ID mask, 标准模式下忽略
		
		//rx mcr 接收报文控制配置
		tCanRxConfig.tMc.bMskEn			= ENABLE;			//使能过滤接收mask, 即使能ID接收滤波,ID匹配在某个范围内
		tCanRxConfig.tMc.bRxIeEn		= ENABLE;			//使能报文接收中断(通道源中断), 置位ITPAND; 若使用报文通道中断(源中断)，必须使能
		tCanRxConfig.tMc.bOverWrEn		= DISABLE;			//关闭报文覆盖模式	
		tCanRxConfig.tMc.byDataLen		= 0x08;				//报文数据长度，设置为最大，实际接收到的数据 <= 8
		csi_can_set_msg_rx(CAN0, CAN_CH4, &tCanRxConfig);	//配置接收报文
		
		csi_can_ch_int_enable(CAN0, CAN_CH4);				//使能对应报文接收通道中断
	}
	
	//接收报文通道5配置(CAN_CH5)
	{
		//id,识别符配置
		tCanRxConfig.tId.eIdMode		= CAN_ID_STD;		//报文ID模式，标准模式(11Bit)
		tCanRxConfig.tId.eMsgDir		= CAN_DIR_RECV;		//报文方向
		tCanRxConfig.tId.hwStdId 		= 0x705;			//标准11BIT ID
		tCanRxConfig.tId.wExtId			= 0x3FFFF;			//扩展ID,标准模式下忽略	
		
		//rx mskr 接收报文mask配置(参与接收滤波),
		//ID匹配范围：0x700~0x70f，即11bit的ID高7位必须匹配
		tCanRxConfig.tMsk.eIdMdMsk	 	= CAN_IDMSK_STD;	//ID模式(STD/EXT) MASK, 扩展位不参与接收滤波
		tCanRxConfig.tMsk.eIdDirMsk 	= CAN_DIRMSK_DIS;	//报文方向位不参与接收滤波
		tCanRxConfig.tMsk.hwStdIdMsk 	= 0x7F0;			//标准模式ID mask,即ID的高7位必须匹配，低4位不必匹配
		tCanRxConfig.tMsk.wExtIdMsk	 	= 0x3FFFF;			//扩展模式ID mask, 标准模式下忽略
		
		//rx mcr 接收报文控制配置
		tCanRxConfig.tMc.bMskEn			= ENABLE;			//使能过滤接收mask, 即使能ID接收滤波,ID匹配在某个范围内
		tCanRxConfig.tMc.bRxIeEn		= ENABLE;			//使能报文接收中断(通道源中断), 置位ITPAND; 若使用报文通道中断(源中断)，必须使能
		tCanRxConfig.tMc.bOverWrEn		= ENABLE;			//使能报文覆盖模式，若连续的报文通道组成接收fifo，最后一个通道的报文覆盖必须开启
		tCanRxConfig.tMc.byDataLen		= 0x08;				//报文数据长度，设置为最大，实际接收到的数据 <= 8
		csi_can_set_msg_rx(CAN0, CAN_CH5, &tCanRxConfig);	//配置接收报文
		
		csi_can_ch_int_enable(CAN0, CAN_CH5);				//使能对应报文接收通道中断
	}
	
	csi_can_int_enable(CAN0, CAN_INTSRC_ERR_ALL);			//使能所有异常状态中断
	csi_can_start(CAN0);									//使能(打开)CAN模块
	
	while(1)
	{
		mdelay(10);
	}
	return iRet;
}


/** \brief	can_remote_frames_send_demo: CAN远程帧发送demo，使能报文接收通道中断和异常状态中断
 * 
 * 	\brief	发送远程帧：设备向网络中对应ID匹配的设备发起数据请求，对应的设备收到远程帧时，会发送对应数据帧做出应答；报
 * 			文对象为接收报文时，如果将该报文的TXRQST置1，那么将会发送具有该报文对象识别符(ID)的远程帧，如果有标识符(ID)
 * 			匹配的数值帧，则接收该报文；即将接收报文的TXRQST置1，将发送远程帧，发送远程帧和接收数据帧两者不冲突。
 *
 * 	\brief	注意：远程应答帧配置时，应选择通道号比接收报文通道号低的通道，以免造成远程帧无法正常应答；即CAN报文配置配置
 * 			时，发送报文对象配置在序号低的通道，接收报文对象配置在序号高的通道。			
 * 
 *  \param[in] none
 *  \return none
 */
int can_remote_frames_send_demo(void)
{
	int iRet = 0;
	csi_can_config_t 	 tCanConfig;						//CAN 初始化结构体
	csi_can_rx_config_t  tCanRxConfig;						//CAN 接收报文配置结构体

#if (USE_GUI == 0)											//用户未选择图形化编程			
	csi_gpio_set_mux(GPIOA, PA11, PA11_CAN_RX);				//CAN RX管脚配置
	csi_gpio_set_mux(GPIOA, PA12, PA12_CAN_TX);				//CAN TX管脚配置
#endif
	
	//CAN MR 寄存器配置,即初始化配置
	//初始化配置
	tCanConfig.eClkSrc  = CAN_CLKSRC_PCLK;					//CAN CLK = PCLK		
	tCanConfig.wBaudRate = CAN_BDR_500K;					//500k
	tCanConfig.bAuReTran = DISABLE;							//自动重新传输关闭
	csi_can_init(CAN0, &tCanConfig);						//CAN初始化
	
	//报文接收到CH3~CH4, ID为全匹配，匹配上的ID数据接收到对应的报文接收通道中
	//CH3~CH4可以作为远程帧的发送，即发送远程帧来请求数据
	//接收报文通道3配置(CAN_CH3)
	{
		//id,识别符配置
		tCanRxConfig.tId.eIdMode		= CAN_ID_STD;		//报文ID模式，标准模式(11Bit)
		tCanRxConfig.tId.eMsgDir		= CAN_DIR_RECV;		//报文方向
		tCanRxConfig.tId.hwStdId 		= 0x753;			//标准11BIT ID
		tCanRxConfig.tId.wExtId			= 0x3FFFF;			//扩展ID,标准模式下忽略	
		
		//rx mskr 接收报文mask配置(参与接收ID滤波)
		//报文控制配置中bMskEn不使能，此配置无意义
		//bMskEn使能，下面的配置：ID匹配范围为0x7F0~0x7FF
//		tCanRxConfig.tMsk.eIdMdMsk	 	= CAN_IDMSK_STD;	//ID模式(STD/EXT) MASK, 扩展位不参与接收滤波
//		tCanRxConfig.tMsk.eIdDirMsk 	= CAN_DIRMSK_DIS;	//报文方向位不参与接收滤波
//		tCanRxConfig.tMsk.hwStdIdMsk 	= 0x7F0;			//标准模式ID mask 
//		tCanRxConfig.tMsk.wExtIdMsk	 	= 0x00000;			//扩展模式ID mask, 标准模式下忽略
		
		//rx mcr 接收报文控制配置
		tCanRxConfig.tMc.bMskEn			= DISABLE;			//禁止过滤接收mask,即禁止接收滤波
		tCanRxConfig.tMc.bRxIeEn		= ENABLE;			//使能报文接收中断(通道源中断), 置位ITPAND; 若使用报文通道中断(源中断)，必须使能
		tCanRxConfig.tMc.bOverWrEn		= DISABLE;			//关闭报文覆盖模式	
		tCanRxConfig.tMc.byDataLen		= 0x08;				//报文数据长度，设置为最大，实际接收到的数据 <= 8
		csi_can_set_msg_rx(CAN0, CAN_CH3, &tCanRxConfig);	//配置接收报文
		
		csi_can_ch_int_enable(CAN0, CAN_CH3);				//使能对应报文接收通道中断
	}
	
	//接收报文通道3配置(CAN_CH4)
	{
		//id,识别符配置
		tCanRxConfig.tId.eIdMode		= CAN_ID_STD;		//报文ID模式，标准模式(11Bit)
		tCanRxConfig.tId.eMsgDir		= CAN_DIR_RECV;		//报文方向
		tCanRxConfig.tId.hwStdId 		= 0x754;			//标准11BIT ID
		tCanRxConfig.tId.wExtId			= 0x3FFFF;			//扩展ID,标准模式下忽略	
		
		//rx mskr 接收报文mask配置(参与接收ID滤波)
		//报文控制配置中bMskEn不使能，此配置无意义 
		//bMskEn使能，下面的配置：ID匹配范围为0x7F0~0x7FF
//		tCanRxConfig.tMsk.eIdMdMsk	 	= CAN_IDMSK_STD;	//ID模式(STD/EXT) MASK, 扩展位不参与接收滤波
//		tCanRxConfig.tMsk.eIdDirMsk 	= CAN_DIRMSK_DIS;	//报文方向位不参与接收滤波
//		tCanRxConfig.tMsk.hwStdIdMsk 	= 0x7F0;			//标准模式ID mask 
//		tCanRxConfig.tMsk.wExtIdMsk	 	= 0x00000;			//扩展模式ID mask, 标准模式下忽略
		
		//rx mcr 接收报文控制配置
		tCanRxConfig.tMc.bMskEn			= DISABLE;			//禁止过滤接收mask,即禁止接收滤波
		tCanRxConfig.tMc.bRxIeEn		= ENABLE;			//使能报文接收中断(通道源中断), 置位ITPAND; 若使用报文通道中断(源中断)，必须使能
		tCanRxConfig.tMc.bOverWrEn		= DISABLE;			//关闭报文覆盖模式	
		tCanRxConfig.tMc.byDataLen		= 0x08;				//报文数据长度，设置为最大，实际接收到的数据 <= 8
		csi_can_set_msg_rx(CAN0, CAN_CH4, &tCanRxConfig);	//配置接收报文
		
		csi_can_ch_int_enable(CAN0, CAN_CH4);				//使能对应报文接收通道中断
	}
	
	csi_can_int_enable(CAN0, CAN_INTSRC_ERR_ALL);			//使能所有异常状态中断
	csi_can_start(CAN0);									//使能(打开)CAN模块
	
	
	//远程帧(数据请求)，即发送远程帧；配置为接收报文通道的报文，通过csi_can_msg_send发送(TXRQST置位)，是为发送远程帧
	{
		csi_can_msg_send(CAN0, CAN_CH3, 8);				
		udelay(500);
		csi_can_msg_send(CAN0, CAN_CH4, 8);
		udelay(500);
	}
	
	while(1)
	{
		mdelay(10);
	}
	
	return iRet;
}


/** \brief	can_remote_frames_response_demo: CAN远程帧应答demo，不使用中断
 * 
 * 	\brief	远程帧应答(接收)：CAN扫描到一个远程帧的识别符(ID)与发送报文某个通道中相匹配，会把对应报文的TXRQST置位，自动
 * 			发送对应的数据帧对远程帧做出应答；将发送报文对象的bRmtEn参数使能，那么该报文通道接收到与该报文识别符(ID)匹配
 * 			的远程帧时，会主动置位该报文的TXRQST，发送数据帧对远程帧做出应答。
 *
 *  \brief	数据长度：远程应答帧的数据长度和接收到的远程数据请求帧中的数据长度无关，和发送帧的配置长度有关。	
 * 
 * 	\brief	注意：远程应答帧配置时，应选择通道号比接收报文通道号低的通道，以免造成远程帧无法正常应答；即CAN报文配置配置
 * 			时，发送报文对象配置在序号低的通道，接收报文对象配置在序号高的通道。
 *			
 * 
 *  \param[in] none
 *  \return none
 */
int can_remote_frames_response_demo(void)
{
	int iRet = 0;
	
	csi_can_config_t 	 tCanConfig;						//CAN 初始化结构体
	csi_can_tx_config_t  tCanTxConfig;						//CAN 发送报文配置结构体

#if (USE_GUI == 0)											//用户未选择图形化编程			
	csi_gpio_set_mux(GPIOA, PA11, PA11_CAN_RX);				//CAN RX管脚配置
	csi_gpio_set_mux(GPIOA, PA12, PA12_CAN_TX);				//CAN TX管脚配置
#endif
	
	//CAN MR 寄存器配置,即初始化配置
	//初始化配置
	tCanConfig.eClkSrc  = CAN_CLKSRC_PCLK;					//CAN CLK = PCLK		
	tCanConfig.wBaudRate = CAN_BDR_500K;					//500k
	tCanConfig.bAuReTran = DISABLE;							//自动重新传输关闭
	csi_can_init(CAN0, &tCanConfig);						//CAN初始化
	
	//CAN发送报文配置，每个报文配置在一个报文通道中(通道：CAN_CH1~CAN_CH32)
	//发送报文对象配置时，使能了远程帧功能，即接收到匹配ID的远程数据请求，自动发送数据
	//报文通道1配置(CAN_CH1)
	{
		//id,识别符配置
		tCanTxConfig.tId.eIdMode	= CAN_ID_STD;			//报文ID模式，标准模式(11Bit)
		tCanTxConfig.tId.eMsgDir	= CAN_DIR_SEND;			//报文方向
		tCanTxConfig.tId.hwStdId 	= 0x751;				//标准11BIT ID
		tCanTxConfig.tId.wExtId		= 0x3FFFF;				//扩展ID,标准模式下忽略	
		//tx mcr 发送报文控制配置
		tCanTxConfig.tMc.bTxIeEn	= DISABLE;				//禁止报文发送中断(通道源中断)
		tCanTxConfig.tMc.bRmtEn		= ENABLE;				//远程帧使能，即接收到远程帧数据请求，发送数据
		tCanTxConfig.tMc.bTxReqEn	= DISABLE;				//报文对象请求发送禁止，
		tCanTxConfig.tMc.byDataLen	= 0x07;					//发送数据长度(数量)
		//data a
		tCanTxConfig.tDataA.bydata[0] 	= 0x11;				//数据A(低4字节数据配置)配置
		tCanTxConfig.tDataA.bydata[1] 	= 0x12;
		tCanTxConfig.tDataA.bydata[2] 	= 0x13;
		tCanTxConfig.tDataA.bydata[3] 	= 0x14;
		//data b
		tCanTxConfig.tDataB.bydata[0] 	= 0x21;				//数据B(高4字节数据配置)配置
		tCanTxConfig.tDataB.bydata[1] 	= 0x22;
		tCanTxConfig.tDataB.bydata[2] 	= 0x23;
		tCanTxConfig.tDataB.bydata[3] 	= 0x24;
		csi_can_set_msg_tx(CAN0, CAN_CH1, &tCanTxConfig);	//配置发送报文通道
	}
	
	//报文通道2配置(CAN_CH2)
	{
		//id,识别符配置
		tCanTxConfig.tId.eIdMode	= CAN_ID_STD;			//报文ID模式，标准模式(11Bit)
		tCanTxConfig.tId.eMsgDir	= CAN_DIR_SEND;			//报文方向
		tCanTxConfig.tId.hwStdId 	= 0x752;				//标准11BIT ID
		tCanTxConfig.tId.wExtId		= 0x3FFFF;				//扩展ID,标准模式下忽略	
		//tx mcr 发送报文控制配置
		tCanTxConfig.tMc.bTxIeEn	= DISABLE;				//禁止报文发送中断(通道源中断)
		tCanTxConfig.tMc.bRmtEn		= ENABLE;				//远程帧使能，即接收到远程帧数据请求，发送数据
		tCanTxConfig.tMc.bTxReqEn	= DISABLE;				//报文对象请求发送禁止，
		tCanTxConfig.tMc.byDataLen	= 0x08;					//发送数据长度(数量)
		//data a
		tCanTxConfig.tDataA.bydata[0] 	= 0x31;				//数据A(低4字节数据配置)配置
		tCanTxConfig.tDataA.bydata[1] 	= 0x32;
		tCanTxConfig.tDataA.bydata[2] 	= 0x33;
		tCanTxConfig.tDataA.bydata[3] 	= 0x34;
		//data b
		tCanTxConfig.tDataB.bydata[0] 	= 0x41;				//数据B(高4字节数据配置)配置
		tCanTxConfig.tDataB.bydata[1] 	= 0x42;
		tCanTxConfig.tDataB.bydata[2] 	= 0x43;
		tCanTxConfig.tDataB.bydata[3] 	= 0x44;
		csi_can_set_msg_tx(CAN0, CAN_CH2, &tCanTxConfig);	//配置发送报文通道
	}
	
	csi_can_start(CAN0);									//使能(打开)CAN模块
	
	while(1)
	{
		mdelay(10);
	}
	
	return iRet;
}
