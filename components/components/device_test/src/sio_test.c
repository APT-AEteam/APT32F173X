/***********************************************************************//** 
 * \file  sio_test.c
 * \brief  SIO_TEST description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-04-19 <td>V0.0 <td>ZJY     <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include <iostring.h>
#include <csi_drv.h>

#include "demo.h"

/* Private macro-----------------------------------------------------------*/
#define		HDQ_WR_CMD		(0x01 << 7)
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

//rgb 原始数据
static uint8_t	s_byDipData[24] = 
{
	//R		G		B
	0x03,	0x07,	0x0a,//rgb1
	0x04,	0x07,	0x05,//rgb2
	0x05,	0x07,	0x01,//rgb3
	0x06,	0x07,	0x02,//rgb4
	0x07,	0x07,	0x03,//rgb5
	0x08,	0x07,	0x04,//rgb6
	0x09,	0x07,	0x05,//rgb7
	0x0a,	0x07,	0x06 //rgb8
};


static uint32_t	s_wSioRxBuf[24];		//接收缓存


static uint32_t sio_led_data_conver(uint8_t byData)
{
	uint8_t i;
	uint32_t wData = 0xaaaa;
	for(i = 0; i < 8; i++)
	{
		if(byData & 0x80)
			wData |= (0x01 << (2*i));	
		byData = byData << 1;
	}

	return wData;
}

static void set_led_rgb_store(uint32_t *pwLeddData,uint16_t hwLedNum)
{
	//led
	*(pwLeddData+hwLedNum*3) = sio_led_data_conver(s_byDipData[hwLedNum*3+1]) ;		//G
	*(pwLeddData+hwLedNum*3+1) = sio_led_data_conver(s_byDipData[hwLedNum*3]);		//R		
	*(pwLeddData+hwLedNum*3+2) = sio_led_data_conver(s_byDipData[hwLedNum*3+2]);	//B	
}


/** \brief sio 发送测试，测试发送相关选项
 * 
 *  \param[in] ptSioBase: pointer of sio register structure; SIO0/SIO1
 *  \param[in] ePinName: pin mux of sio; SIO0: PB0/PC0/PD3/PD4; SIO1: PB1/PD3
 *  \return error code
 */
int sio_send_test(csp_sio_t *ptSioBase, pin_name_e ePinName)
{
	int iRet = 0;
	uint32_t byDipDataEnd[24];
	uint8_t i, byRecv;
	csi_sio_tx_config_t tSioTxCfg;
	
//	csi_pin_set_mux(PB0, PB0_SIO0);	
//	csi_pin_set_mux(PC0, PC0_SIO0);	
//	csi_pin_set_mux(PD3, PD3_SIO0);
//	csi_pin_set_mux(PD4, PD4_SIO0);
//	csi_pin_set_mux(PB1, PB1_SIO1);
//	csi_pin_set_mux(PD3, PD3_SIO1);
	
	if(((uint32_t)ptSioBase) == APB_SIO0_BASE)
	{
		switch(ePinName)
		{
			case PB0:
				csi_pin_set_mux(PB0, PB0_SIO0);	
				break;
			case PC0:
				csi_pin_set_mux(PC0, PC0_SIO0);	
				break;
			case PD3:
				csi_pin_set_mux(PD3, PD3_SIO0);
				break;
			case PD4:
				csi_pin_set_mux(PD4, PD4_SIO0);
				break;
			default:
				return -1;
		}
	}
	else if(((uint32_t)ptSioBase) == APB_SIO1_BASE)
	{
		if(ePinName == PB1)
			csi_pin_set_mux(PB1, PB1_SIO1);
		else if(ePinName == PD3)
			csi_pin_set_mux(PD3, PD3_SIO1);
		else
			return -1;
	}
	else
		return -1;
	
	//SIO TX 参数配置
	tSioTxCfg.byD0Len 		= 1;				//D0 对象序列长度(bit个数)，这里不用D0								
	tSioTxCfg.byD1Len 		= 1;				//D1 对象序列长度(bit个数)，这里不用D1
	tSioTxCfg.byDLLen 		= 4;				//DL 对象序列长度(bit个数)
	tSioTxCfg.byDHLen 		= 4;				//DH 对象序列长度(bit个数)
	tSioTxCfg.byDLLsq 		= 0x01;				//DL 对象序列具体定义: bit= 0  -     
												//					          | |___|					
	tSioTxCfg.byDHHsq 		= 0x07;				//DH 对象序列具体定义: bit= 1  ___
												//					          |   |_| 	
	tSioTxCfg.byTxBufLen 	= 8;				//发送数据缓存长度(bit个数 = 8)，txbuf 一次发送bit数量，len <= 16
	tSioTxCfg.byTxCnt 		= 24;				//SIO一次发送总的数据长度(bit个数 = 24)，byTxCnt >= byTxBufLen，byTxCnt < 256(最大32bytes)
	tSioTxCfg.byIdleLev 	= SIO_IDLE_L;		//SIO空闲时刻IO管脚输出电平
	tSioTxCfg.byTxDir 		= SIO_TXDIR_LSB;	//LSB->MSB, txbuf 数据按照bit[1:0]...[31:30]方式移出
	tSioTxCfg.wTxFreq 		= 500000;			//tx clk =0.5M, Ttxshift = 1/0.5 = 2us；发送每bit时间是2us
	tSioTxCfg.byInt			= SIO_INTSRC_NONE;	//不使用中断

	csi_sio_tx_init(ptSioBase, &tSioTxCfg);

	for(i = 0; i < 8; i++)
	{
		set_led_rgb_store(byDipDataEnd,i);			//数据转换
	}
	
	while(1)
	{
		byRecv = csi_uart_getc(UART1);
		if(byRecv)
		{ 
			csi_sio_send(ptSioBase, byDipDataEnd, 24);			//发送
			mdelay(10);
		}
	}
	return iRet;
}


/** \brief gpio 中断发送测试，测试包括发送中断 相关测试
 *  \param[in] ptSioBase: pointer of sio register structure; SIO0/SIO1
 *  \param[in] ePinName: pin mux of sio; SIO0: PB0/PC0/PD3/PD4; SIO1: PB1/PD3
 *  \param[in] eSioSendInt: tx interrupt; SIO_INTSRC_TXBUFEMPT/SIO_INTSRC_TXDNE
 *  \return error code
 */
int sio_send_int_test(csp_sio_t *ptSioBase, pin_name_e ePinName, csi_sio_intsrc_e eSioSendInt)
{
	int iRet = 0;
	uint32_t byDipDataEnd[24];
	uint8_t i, byRecv;
	csi_sio_tx_config_t tSioTxCfg;
	
//	csi_pin_set_mux(PB0, PB0_SIO0);	
//	csi_pin_set_mux(PC0, PC0_SIO0);	
//	csi_pin_set_mux(PD3, PD3_SIO0);
//	csi_pin_set_mux(PD4, PD4_SIO0);
//	csi_pin_set_mux(PB1, PB1_SIO1);
//	csi_pin_set_mux(PD3, PD3_SIO1);
	
	if(((uint32_t)ptSioBase) == APB_SIO0_BASE)
	{
		switch(ePinName)
		{
			case PB0:
				csi_pin_set_mux(PB0, PB0_SIO0);	
				break;
			case PC0:
				csi_pin_set_mux(PC0, PC0_SIO0);	
				break;
			case PD3:
				csi_pin_set_mux(PD3, PD3_SIO0);
				break;
			case PD4:
				csi_pin_set_mux(PD4, PD4_SIO0);
				break;
			default:
				return -1;
		}
	}
	else if(((uint32_t)ptSioBase) == APB_SIO1_BASE)
	{
		if(ePinName == PB1)
			csi_pin_set_mux(PB1, PB1_SIO1);
		else if(ePinName == PD3)
			csi_pin_set_mux(PD3, PD3_SIO1);
		else
			return -1;
	}
	else
		return -1;
	
	//SIO TX 参数配置
	tSioTxCfg.byD0Len 		= 1;					//D0 对象序列长度(bit个数)，这里不用D0								
	tSioTxCfg.byD1Len 		= 1;					//D1 对象序列长度(bit个数)，这里不用D1
	tSioTxCfg.byDLLen 		= 4;					//DL 对象序列长度(bit个数)
	tSioTxCfg.byDHLen 		= 4;					//DH 对象序列长度(bit个数)
	tSioTxCfg.byDLLsq 		= 0x01;					//DL 对象序列具体定义: bit= 0  -     
													//					          | |___|					
	tSioTxCfg.byDHHsq 		= 0x07;					//DH 对象序列具体定义: bit= 1  ___
													//					          |   |_| 	
	tSioTxCfg.byTxBufLen 	= 8;					//发送数据缓存长度(bit个数 = 8)，txbuf 一次发送bit数量，len <= 16
	tSioTxCfg.byTxCnt 		= 24;					//SIO一次发送总的数据长度(bit个数 = 24)，byTxCnt >= byTxBufLen，byTxCnt < 256(最大32bytes)
	tSioTxCfg.byIdleLev 	= SIO_IDLE_L;			//SIO空闲时刻IO管脚输出电平
	tSioTxCfg.byTxDir 		= SIO_TXDIR_LSB;		//LSB->MSB, txbuf 数据按照bit[1:0]...[31:30]方式移出
	tSioTxCfg.wTxFreq 		= 500000;				//tx clk =0.5m, Ttxshift = 1/0.5 = 2us；发送每bit时间是2us
	
	
	if(eSioSendInt == SIO_INTSRC_TXBUFEMPT)
	{
		tSioTxCfg.byInt		= SIO_INTSRC_TXBUFEMPT; //使用中断方式发送
	}
	else if(eSioSendInt	== SIO_INTSRC_TXDNE)
	{
		tSioTxCfg.byInt		=  SIO_INTSRC_NONE;		//不使用中断方式发送
		
		//开启中断，用来测试TXDNE中断
		csi_irq_enable((uint32_t*)ptSioBase);							
		csp_sio_int_enable(ptSioBase,SIO_INTSRC_TXDNE, ENABLE);       	
	}
	else
		return -1;
		

	csi_sio_tx_init(ptSioBase, &tSioTxCfg);
	
	for(i = 0; i < 8; i++)
	{
		set_led_rgb_store(byDipDataEnd,i);			//数据转换
	}
	
	while(1)
	{
		byRecv = csi_uart_getc(UART1);
		if(byRecv)
		{ 
			csi_sio_send(ptSioBase, byDipDataEnd, 3);			//发送
			mdelay(10);
		}
	}
	
	return iRet;
}

/** \brief sio 接收测试，包括接收中断，超时、blk中断和接收相关测试
 * 
 *  \param[in] ptSioBase: pointer of sio register structure; SIO0/SIO1
 *  \param[in] ePinName: pin mux of sio; SIO0: PB0/PC0/PD3/PD4; SIO1: PB1/PD3
 *  \param[in] eSioSendInt: tx interrupt; SIO_INTSRC_RXBUFFULL/SIO_INTSRC_RXDNE
 *  \return error code
 */
int sio_recv_test(csp_sio_t *ptSioBase, pin_name_e ePinName, csi_sio_intsrc_e eSioRecvInt)
{
	int iRet = 0;
	csi_sio_rx_config_t tSioRxCfg;
	uint32_t wLedRxBuf[24];
	uint32_t wRxBuf[8];
	
	if(((uint32_t)ptSioBase) == APB_SIO0_BASE)
	{
		switch(ePinName)
		{
			case PB0:
				csi_pin_set_mux(PB0, PB0_SIO0);	
				break;
			case PC0:
				csi_pin_set_mux(PC0, PC0_SIO0);	
				break;
			case PD3:
				csi_pin_set_mux(PD3, PD3_SIO0);
				break;
			case PD4:
				csi_pin_set_mux(PD4, PD4_SIO0);
				break;
			default:
				return -1;
		}
	}
	else if(((uint32_t)ptSioBase) == APB_SIO1_BASE)
	{
		if(ePinName == PB1)
			csi_pin_set_mux(PB1, PB1_SIO1);
		else if(ePinName == PD3)
			csi_pin_set_mux(PD3, PD3_SIO1);
		else
			return -1;
	}
	else
		return -1;	
		
		
	if((eSioRecvInt != SIO_INTSRC_RXDNE) && (eSioRecvInt != SIO_INTSRC_RXBUFFULL))
		return -1;
	
	//SIO RX 参数配置
	tSioRxCfg.byDebPerLen 	= 3;					//接收滤波周期	
	tSioRxCfg.byDebClkDiv 	= 2;					//接收滤波时钟分频
	tSioRxCfg.byTrgEdge 	= SIO_TRG_RISE;			//接收采样触发边沿，上升沿
	tSioRxCfg.byTrgMode		= SIO_TRGMD_DEB;		//接收采样触发模式，去抖后
	tSioRxCfg.byRxDir 	 	= SIO_RXDIR_LSB;		//接收数据方向， 数据按照bit0...31方式移入，根据不同应用移入数据
	tSioRxCfg.bySpMode		= SIO_SPMD_EDGE_EN;		//接收数据采样边沿对齐使能	
	tSioRxCfg.bySpExtra		= SIO_EXTRACT_HI;		//采样提取策略，(20H)HITHR; (BIT = 1)个数 > HITHR 提取H,否则提取L
	tSioRxCfg.byHithr		= 4;					//提取判定值, (BIT = 1)个数 > HITHR 提取H,否则提取L
	tSioRxCfg.byRxBufLen	= 8;					//发送数据缓存长度(bit个数 = 8)，rxbuf 一次接收bit数量，len <= 32				
	tSioRxCfg.byRxCnt		= 24;					//SIO一次接收总的数据长度(bit个数 = 24)，byRxCnt >= byRxBufLen，byRxCnt < 256(最大32bytes)				
	tSioRxCfg.wRxFreq		= 1000000;				//rx clk =1MHz, Trxsamp = 1/1 = 1us；每1us 采样一次
	tSioRxCfg.bySpBitLen	= 8;					//bit采样的长度，每个bit采样次数为8，总得采样时间 = 8*Trxsamp = 1us
	tSioRxCfg.byInt			= eSioRecvInt | 
			SIO_INTSRC_TIMEOUT | SIO_INTSRC_BREAK;	//接收采用RXBUFFULL中断，当接收到byRxBufLen个bit后，产生中断，读取到接收buf中，每次读取byRxBufLen(8)bit
													//接收采RXDNE中断，byRxCnt <= 32bit；接收byRxCnt(24)个bit，产生中断,读取数据到接收buf，每次读取byRxCnt(24)bit
	
	csi_sio_rx_init(ptSioBase, &tSioRxCfg);			//初始化SIO接收参数
	//csi_sio_timeout_rst(ptSioBase, 12, ENABLE);		//接收超时复位, timeout cnt > bySpBitLen
	//csi_sio_break_rst(ptSioBase,SIO_BKLEV_HIGH, 20, ENABLE);
	if(eSioRecvInt == SIO_INTSRC_RXDNE)
		csi_sio_set_buffer(s_wSioRxBuf, 8);			//设置接收数据buf和buf长度len >= (24个byRxBufLen)，将接收到的数据存放于用户定义的buffer中
	else if(eSioRecvInt == SIO_INTSRC_RXBUFFULL)
		csi_sio_set_buffer(s_wSioRxBuf, 24);
		
	while(1)
	{
		if(eSioRecvInt == SIO_INTSRC_RXDNE)
			iRet = csi_sio_receive(SIO0, wRxBuf, 8);		//接收数据, 失败返回0；成功返回用户设置的接收数据个数(8*byRxCnt)bit，byRxCnt bytes；
		else if(eSioRecvInt == SIO_INTSRC_RXBUFFULL)
			iRet = csi_sio_receive(SIO0, wLedRxBuf, 24);	//接收数据, 失败返回0；成功返回用户设置的接收数据个数(8*byRxCnt)bit，byRxCnt bytes；此函数非阻塞
		
		if(iRet > 0)								//接收到用户设置数据								
		{
			my_printf("SIO0 receive data len: %d \n", iRet);		
		}
	}
	return iRet;
}

/** \brief sio dma 发送
 * 
 *  \param[in] ptSioBase: pointer of sio register structure; SIO0/SIO1
 *  \return error code
 */
int sio_send_dma_test(csp_sio_t *ptSioBase)
{
	int ret = 0;
	uint8_t byRecv;
	uint8_t byCount = 0;
	uint32_t wDipDataEnd[24];
	uint16_t hwDmaSendData[24];
	
	csi_dma_ch_config_t tDmaConfig;				
	csi_etb_config_t 	tEtbConfig;	
	csi_sio_tx_config_t tSioTxCfg;
	
	if(((uint32_t)ptSioBase) == APB_SIO0_BASE)
		csi_pin_set_mux(PB0, PB0_SIO0);
	else if(((uint32_t)ptSioBase) == APB_SIO1_BASE)
		csi_pin_set_mux(PD3, PD3_SIO1);
	
	//dma config
	tDmaConfig.bySrcLinc 	= DMA_ADDR_CONSTANT;		//低位传输原地址固定不变
	tDmaConfig.bySrcHinc 	= DMA_ADDR_INC;				//高位传输原地址自增
	tDmaConfig.byDetLinc 	= DMA_ADDR_CONSTANT;		//低位传输目标地址固定不变
	tDmaConfig.byDetHinc 	= DMA_ADDR_CONSTANT;		//高位传输目标地址固定不变
	tDmaConfig.byDataWidth 	= DMA_DSIZE_16_BITS;		//传输数据宽度16bit
	tDmaConfig.byReload 	= DMA_RELOAD_DISABLE;		//禁止自动重载
	tDmaConfig.byTransMode 	= DMA_TRANS_ONCE;			//DMA服务模式(传输模式)，连续服务
	tDmaConfig.byTsizeMode  = DMA_TSIZE_ONE_DSIZE;		//传输数据大小，一个 DSIZE , 即DSIZE定义大小
	tDmaConfig.byReqMode	= DMA_REQ_HARDWARE;			//DMA请求模式，硬件触发
	tDmaConfig.wInt			= DMA_INTSRC_TCIT;			//使用TCIT中断
	
	//etb config
	tEtbConfig.byChType = ETB_ONE_TRG_ONE_DMA;			//单个源触发单个目标，DMA方式
	
	if(((uint32_t)ptSioBase) == APB_SIO0_BASE)
		tEtbConfig.bySrcIp 	= ETB_SIO0_TXSRC;			//SIO0 TXSRC作为触发源
	else if(((uint32_t)ptSioBase) == APB_SIO1_BASE)
		tEtbConfig.bySrcIp 	= ETB_SIO1_TXSRC;			//SIO1 TXSRC作为触发源
	
	tEtbConfig.byDstIp 	= ETB_DMA0_CH1;					//ETB DMA通道 作为目标实际
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;			//通道触发模式采样硬件触发
	
	csi_etb_init();										//使能ETB模块
	ret = csi_etb_ch_config(ETB_CH21, &tEtbConfig);		//初始化ETB
	if(ret < CSI_OK)
		return CSI_ERROR;
	ret = csi_dma_ch_init(DMA0, DMA_CH1, &tDmaConfig);	//初始化DMA

	//SIO TX 参数配置
	tSioTxCfg.byD0Len 		= 1;						//D0 对象序列长度(bit个数)，这里不用D0								
	tSioTxCfg.byD1Len 		= 1;						//D1 对象序列长度(bit个数)，这里不用D1
	tSioTxCfg.byDLLen 		= 4;						//DL 对象序列长度(bit个数)
	tSioTxCfg.byDHLen 		= 4;						//DH 对象序列长度(bit个数)
	tSioTxCfg.byDLLsq 		= 0x01;						//DL 对象序列具体定义: bit= 0  -     
														//					          | |___|					
	tSioTxCfg.byDHHsq 		= 0x07;						//DH 对象序列具体定义: bit= 1  ___
														//					          |   |_| 	
	tSioTxCfg.byTxBufLen 	= 8;						//发送数据缓存长度(bit个数 = 8)，txbuf 一次发送bit数量，len <= 16
	tSioTxCfg.byTxCnt 		= 8;						//SIO一次发送总的数据长度(bit个数 = 8)，byTxCnt >= byTxBufLen，byTxCnt < 256(最大32bytes)
	tSioTxCfg.byIdleLev 	= SIO_IDLE_L;				//SIO空闲时刻IO管脚输出电平
	tSioTxCfg.byTxDir 		= SIO_TXDIR_LSB;			//MSB->LSB, txbuf 数据按照bit[1:0]...[31:30]方式移出
	tSioTxCfg.wTxFreq 		= 500000;					//tx clk =4MHz, Ttxshift = 1/4 = 250ns；发送每bit时间是250ns
	tSioTxCfg.byInt			= SIO_INTSRC_NONE;			//不使用中断。
	
	csi_sio_tx_init(ptSioBase, &tSioTxCfg);
	
	for(byCount = 0; byCount < 8; byCount++ )
	{
		set_led_rgb_store(wDipDataEnd, byCount);		//24*16byte
	}
	
	for(uint8_t i = 0; i < 24; i++)
	{
		hwDmaSendData[i] = (uint16_t)wDipDataEnd[i];	//将数据转换为16bit的类型，dma发送
	}
	
	while(1)
	{
		byRecv = csi_uart_getc(UART1);
		if(byRecv)
		{ 
			csi_sio_send_dma(ptSioBase, DMA0, DMA_CH1, hwDmaSendData, 24);	
		}
		mdelay(10);
		if(csi_dma_get_msg(DMA0, DMA_CH1, ENABLE))		//获取发送完成消息，并清除消息
		{
			//添加用户代码
			nop;
		}
	}
	
	return ret;
}

/** \brief sio dma 接收
 * 
 *  \param[in] ptSioBase: pointer of sio register structure; SIO0/SIO1
 *  \return error code
 */
int sio_recv_dma_test(csp_sio_t *ptSioBase)
{
	int ret = 0;
	uint8_t byLedRxBuf[24];
	csi_dma_ch_config_t tDmaConfig;				
	csi_etb_config_t 	tEtbConfig;	
	csi_sio_rx_config_t tSioRxCfg;
	
	if(((uint32_t)ptSioBase) == APB_SIO0_BASE)
		csi_pin_set_mux(PB0, PB0_SIO0);
	else if(((uint32_t)ptSioBase) == APB_SIO1_BASE)
		csi_pin_set_mux(PD3, PD3_SIO1);
	
	
	//dma config
	tDmaConfig.bySrcLinc 	= DMA_ADDR_CONSTANT;		//低位传输原地址固定不变
	tDmaConfig.bySrcHinc 	= DMA_ADDR_CONSTANT;		//高位传输原地址固定不变
	tDmaConfig.byDetLinc 	= DMA_ADDR_CONSTANT;		//低位传输目标地址固定不变
	tDmaConfig.byDetHinc 	= DMA_ADDR_INC;				//高位传输目标地址自增
	tDmaConfig.byDataWidth 	= DMA_DSIZE_8_BITS;			//传输数据宽度8bit
	tDmaConfig.byReload 	= DMA_RELOAD_DISABLE;		//禁止自动重载
	tDmaConfig.byTransMode 	= DMA_TRANS_ONCE;			//DMA服务模式(传输模式)，连续服务
	tDmaConfig.byTsizeMode  = DMA_TSIZE_ONE_DSIZE;		//传输数据大小，一个 DSIZE , 即DSIZE定义大小
	tDmaConfig.byReqMode	= DMA_REQ_HARDWARE;			//DMA请求模式，硬件请求
	tDmaConfig.wInt			= DMA_INTSRC_TCIT;			//使用TCIT中断
	
	//etb config
	tEtbConfig.byChType = ETB_ONE_TRG_ONE_DMA;			//单个源触发单个目标，DMA方式
	
	if(((uint32_t)ptSioBase) == APB_SIO0_BASE)
		tEtbConfig.bySrcIp 	= ETB_SIO0_RXSRC;			//SIO0 RXSRC作为触发源
	else if(((uint32_t)ptSioBase) == APB_SIO1_BASE)
		tEtbConfig.bySrcIp 	= ETB_SIO1_RXSRC;			//SIO1 RXSRC作为触发源
	
	tEtbConfig.byDstIp 	= ETB_DMA1_CH2;					//ETB DMA通道 作为目标实际
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;			//通道触发模式采样硬件触发
	
	csi_etb_init();										//使能ETB模块
	ret = csi_etb_ch_config(ETB_CH20, &tEtbConfig);		//初始化ETB，DMA ETB CHANNEL > ETB_CH19_ID
	if(ret < CSI_OK)
		return CSI_ERROR;
		
	ret = csi_dma_ch_init(DMA1, DMA_CH2, &tDmaConfig);	//初始化DMA
	
	//SIO RX 参数配置
	tSioRxCfg.byDebPerLen 	= 3;					//接收滤波周期	
	tSioRxCfg.byDebClkDiv 	= 2;					//接收滤波时钟分频
	tSioRxCfg.byTrgEdge 	= SIO_TRG_RISE;			//接收采样触发边沿，上升沿
	tSioRxCfg.byTrgMode		= SIO_TRGMD_DEB;		//接收采样触发模式，去抖后
	tSioRxCfg.byRxDir 	 	= SIO_RXDIR_LSB;		//接收数据方向， 数据按照bit0...31方式移入，根据不同应用移入数据
	tSioRxCfg.bySpMode		= SIO_SPMD_EDGE_EN;		//接收数据采样边沿对齐使能	
	tSioRxCfg.bySpExtra		= SIO_EXTRACT_HI;		//采样提取策略，(20H)HITHR; (BIT = 1)个数 > HITHR 提取H,否则提取L
	tSioRxCfg.byHithr		= 4;					//提取判定值, (BIT = 1)个数 > HITHR 提取H,否则提取L
	tSioRxCfg.byRxBufLen	= 8;					//发送数据缓存长度(bit个数 = 8)，rxbuf 一次接收bit数量，len <= 32				
	tSioRxCfg.byRxCnt		= 8;					//SIO一次接收总的数据长度(bit个数 = 24)，byRxCnt >= byRxBufLen，byRxCnt < 256(最大32bytes)				
	tSioRxCfg.wRxFreq		= 1000000;				//rx clk =8MHz, Trxsamp = 1/8 = 125ns；每125ns 采样一次
	tSioRxCfg.bySpBitLen	= 8;					//bit采样的长度，每个bit采样次数为8，总得采样时间 = 8*Trxsamp = 1us
	tSioRxCfg.byInt			= SIO_INTSRC_NONE;		//接收采用RXBUFFULL中断，当接收到byRxBufLen个bit后，产生中断，读取到接收buf中，每次读取byRxBufLen(8)bit
	
	csi_sio_rx_init(ptSioBase, &tSioRxCfg);			//初始化SIO接收参数
	csi_sio_timeout_rst(ptSioBase, 20, ENABLE);		//接收超时复位, timeout cnt > bySpBitLen
	
	csi_sio_recv_dma(ptSioBase, DMA1, DMA_CH2, (void*)byLedRxBuf, 24);
	
	while(1)
	{
		if(csi_dma_get_msg(DMA1, DMA_CH2, ENABLE))	//获取接收完成消息，并清除消息
		{
			//添加用户代码
			nop;
		}							
		mdelay(10);
		nop;
	}
	return ret;
}
/** \brief sio interrupt handle 
 * 
 *  \param[in] ptSioBase: pointer of sio register structure
 *  \return none
 */
void sio_irqhandler(csp_sio_t *ptSioBase)
{
	volatile uint32_t wStatus = csp_sio_get_isr(ptSioBase) & 0x3f;
	
	switch(wStatus)
	{
		case SIO_RXBUFFULL:										
		case SIO_RXDNE:
			if(NULL == g_tSioTran.pwData || 0 == g_tSioTran.hwSize)
			{
				csp_sio_get_rxbuf(ptSioBase);
				g_tSioTran.byRxStat = SIO_STATE_ERROR;				//receive error
			}
			else
			{
				*(g_tSioTran.pwData + g_tSioTran.hwTranLen) = csp_sio_get_rxbuf(ptSioBase);	//receive data
				g_tSioTran.hwTranLen ++;
				if(g_tSioTran.hwTranLen >= g_tSioTran.hwSize)
				{
					g_tSioTran.byRxStat = SIO_STATE_FULL;			//receive buf full, g_tSioTran.hwTranLen = receive data len = receive buf len
					csp_sio_woke_rst(SIO0);
				}
			}
			csp_sio_clr_isr(ptSioBase, SIO_RXDNE | SIO_RXBUFFULL);
			break;
		case SIO_TIMEOUT:
			nop;
			csp_sio_clr_isr(ptSioBase, SIO_TIMEOUT);
			break;
		case SIO_BREAK:												//receive break interrupt ,reset receive module
			nop;
			csp_sio_clr_isr(ptSioBase, SIO_BREAK);
			break;
		case SIO_TXBUFEMPT:
			csp_sio_clr_isr(ptSioBase, SIO_TXBUFEMPT);
		 	SIO0->TXBUF = *(g_tSioTran.pwData);
			g_tSioTran.pwData++;
			g_tSioTran.hwTranLen++;
			if(g_tSioTran.hwTranLen >= g_tSioTran.hwSize)
			{
				csp_sio_int_enable(ptSioBase,SIO_INTSRC_TXBUFEMPT, DISABLE);
				g_tSioTran.hwTranLen = 0;
				g_tSioTran.byTxStat = SIO_STATE_IDLE;
			}
			break;
		case SIO_TXDNE:
			csp_sio_clr_isr(ptSioBase, SIO_TXDNE);
			break;
		default:
			csp_sio_clr_isr(ptSioBase, SIO_BREAK | SIO_RXDNE | SIO_RXBUFFULL | SIO_TIMEOUT);
			break;
	}
}

