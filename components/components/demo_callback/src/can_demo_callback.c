/***********************************************************************//** 
 * \file  can_demo_callback.c
 * \brief  CAN_DEMO_CALLBACK description and static inline functions at register level 
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

#if (USE_CAN_CALLBACK == 1)	
	
static csi_can_recv_t s_tCanRecv[3];		//接收数据结构体缓存,接收三个报文
	
/** \brief  user_send_callback：CAN报文发送完成回调函数
 * 
 * 	\brief	用户定义，使用csi标准库进行发送时，发送完毕后回自动调用用户注册的回调函数；用户可在回调
 * 			函数里做自己的处理，而不需要关注具体的底层中断。
 * 		
 *  \param[in] ptCanBase: 	CANx寄存器结构体指针，指向CANx的基地址 
 *  \param[in] byCh: 	  	对应报文的的通道号，1~32
 *  \return none
 */
static void	user_send_callback(csp_can_t *ptCanBase, uint8_t byCh)
{
	//添加用户处理
	if(byCh == CAN_CH1)		//报文通道1中报文发送成功
	{
		nop;
	}
	
	if(byCh == CAN_CH2)		//报文通道2中报文发送成功
	{
		nop;
	}
}

/** \brief  user_receive_callback：CAN报文接收回调函数
 * 
 * 	\brief	用户定义，使用csi标准库进行发送时，接收完毕后回自动调用用户注册的回调函数；用户可在回调
 * 			函数里做自己的处理，而不需要关注具体的底层中断。
 * 		
 *  \param[in]  ptCanBase: 	CANx寄存器结构体指针，指向CANx的基地址 
 *  \param[out] ptRecv:	 	指向报文接收数据buf，即s_tCanRecv数组中的某个； CH3~CH5报文接收到s_tCanRecv[0~3]
 *  \return none
 */
static void	user_receive_callback(csp_can_t *ptCanBase, csi_can_recv_t *ptRecv)
{
	//用户添加处理
	if(ptRecv->byChNum == CAN_CH3)
	{
		nop;
	}
	
	if(ptRecv->byChNum == CAN_CH4)
	{
		nop;
	}
}

/** \brief  user_err_callback：CAN报文接收发送异常状态回调函数
 * 
 * 	\brief	用户定义，使用csi标准库进行传输时，若传输过程中有异常发生，会自动调用用户注册的回调函数；
 * 			用户可在回调函数里做自己的处理，而不需要关注具体的底层中断。
 * 		
 *  \param[in]  ptCanBase: 	CANx寄存器结构体指针，指向CANx的基地址 
 *  \param[out] wIsr:	 	CAN报文接收/发送(异常)状态
 *  \return none
 */
static void	user_err_callback(csp_can_t *ptCanBase, uint32_t wIsr)
{
	//用户添加处理，处理对应的异常，共支持10个异常中断处理
	if(wIsr & CAN_INTSRC_ERWARNTR)				//被动错误警告
	{
		nop;	
	}
	//
	if(wIsr & CAN_INTSRC_ERPASSTR)				//被动错误发送或者回复
	{
		nop;
	}
//	if(wIsr & CAN_INTSRC_BUSOFFTR)				//总线关闭事件
//	{
//		nop;
//	}
//	......

}

/** \brief	can_send_int_callback_demo： CAN报文发送demo，使用callback
 * 
 * 	\brief	CAN报文发送使用中断(开启所有状态中断和对应发送报文通道中断)，使用标准csi库函数，库函数中断支持回调函数，
 * 			用户可注册报文通道发送完毕中断的回调函数，用户可在回调函数里做自己的处理，不必关注底层的中断。
 * 
 *  \param[in] none
 *  \return none
 */
int can_send_int_callback_demo(void)
{
	int iRet = 0;
	csi_can_config_t 	 tCanConfig= {0};						//CAN 初始化结构体
	csi_can_tx_config_t  tCanTxConfig= {0};						//CAN 发送报文配置结构体
	
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
	//报文通道2配置(CAN_CH1)
	{
		//id,识别符配置
		tCanTxConfig.tId.eIdMode	= CAN_ID_STD;			//报文ID模式，标准模式(11Bit)
		tCanTxConfig.tId.eMsgDir	= CAN_DIR_SEND;			//报文方向
		tCanTxConfig.tId.hwStdId 	= 0x701;				//标准11BIT ID
		tCanTxConfig.tId.wExtId		= 0x3FFFF;				//扩展ID,标准模式下忽略	
		//tx mcr 发送报文控制配置
		tCanTxConfig.tMc.bTxIeEn	= ENABLE;				//使能报文中断，若使用报文通道中断(源中断)，必须使能
		tCanTxConfig.tMc.bRmtEn		= DISABLE;				//远程应答帧禁止，即接收到远程帧，不请求发送应答
		tCanTxConfig.tMc.bTxReqEn	= DISABLE;				//报文对象请求发送禁止，
		tCanTxConfig.tMc.byDataLen	= 0x08;					//报文发送数据数量设置为最大，实际发送数据以发送接口中传入为准
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
		csi_can_set_msg_tx(CAN0, CAN_CH1, &tCanTxConfig);	//配置发送报文
	}
	
	//报文通道2配置(CAN_CH2)
	{
		//id,识别符配置
		tCanTxConfig.tId.eIdMode	= CAN_ID_EXT;			//报文ID模式，扩展模式(29Bit)
		tCanTxConfig.tId.eMsgDir	= CAN_DIR_SEND;			//报文方向
		tCanTxConfig.tId.hwStdId 	= 0x702;				//标准11BIT ID
		tCanTxConfig.tId.wExtId		= 0x3FFFF;				//扩展18BIT ID,标准模式下忽略	
		//tx mcr 发送报文控制配置
		tCanTxConfig.tMc.bTxIeEn	= ENABLE;				//使能报文中断，若使用报文通道中断(源中断)，必须使能
		tCanTxConfig.tMc.bRmtEn		= DISABLE;				//远程应答帧禁止，即接收到远程帧，不请求发送应答
		tCanTxConfig.tMc.bTxReqEn	= DISABLE;				//报文对象请求发送禁止，
		tCanTxConfig.tMc.byDataLen	= 0x08;					//报文发送数据数量设置为最大，实际发送数据以发送接口中传入为准
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
		csi_can_set_msg_tx(CAN0, CAN_CH2, &tCanTxConfig);	//配置发送报文
	}
	
	//注册发送完成中断用户回调函数
	csi_can_register_callback(CAN0, CAN_CALLBACK_SEND, user_send_callback);
	//注册传输异常中断户回调函数
	csi_can_register_callback(CAN0, CAN_CALLBACK_ERR, user_err_callback);
	csi_can_start(CAN0);									//使能(打开)CAN模块
	
	csi_can_msg_send_int(CAN0, CAN_CH1, 8);					//发送报文通道1中的报文，开启所有异常状态中断和对应报文通道中断
	udelay(300);											//多个报文发送时，报文之间需要加几百微秒延时
	csi_can_msg_send_int(CAN0, CAN_CH2, 8);					//发送报文通道2中的报文，开启所有异常状态中断和对应报文通道中断

	return iRet;
}

/** \brief	can_receive_int_callback_demo: CAN报文发接收demo，使用callback
 * 
 * 	\brief	CAN报文发送使用中断(开启所有状态中断和对应发送报文通道中断)，使用标准csi库函数，库函数中断支持回调函数，
 * 			用户可注册报文通道发送完毕中断的回调函数，用户可在回调函数里做自己的处理，不必关注底层的中断。
 * 
 *  \param[in] none
 *  \return none
 */
int can_receive_int_callback_demo(void)
{
	int iRet = 0;
	csi_can_config_t 	 tCanConfig= {0};						//CAN 初始化结构体
	csi_can_rx_config_t  tCanRxConfig= {0};						//CAN 发送报文配置结构体
	
#if (USE_GUI == 0)											//用户未选择图形化编程			
	csi_gpio_set_mux(GPIOA, PA11, PA11_CAN_RX);				//CAN RX管脚配置
	csi_gpio_set_mux(GPIOA, PA12, PA12_CAN_TX);				//CAN TX管脚配置
#endif

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
	}
	
	//接收报文通道3配置(CAN_CH4)
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
	}
	
	//注册接收完成中断用户回调函数
	csi_can_register_callback(CAN0, CAN_CALLBACK_RECV, user_receive_callback);
	//注册传输异常中断户回调函数
	csi_can_register_callback(CAN0, CAN_CALLBACK_ERR, user_err_callback);
	csi_can_msg_receive_int(CAN0, s_tCanRecv, CAN_CH3, 2);	//接收报文数据到s_tCanRecv数组，开启所有异常状态中断和对应报文接收通道中断
	csi_can_start(CAN0);									//使能(打开)CAN模块
	
	while(1)
	{
		mdelay(10);
	}
	
	return iRet;
}

#endif
