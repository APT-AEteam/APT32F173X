/***********************************************************************//** 
 * \file  sio_demo.c
 * \brief  SIO_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-11 <td>V0.0 <td>ZJY     <td>initial
 * <tr><td> 2023-9-19 <td>V0.1 <td>ZJY     <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include "csi_drv.h"
#include "board_config.h"

/* Private macro-----------------------------------------------------------*/
#define		HDQ_WR_CMD		(0x01 << 7)		//HDQ 写命令
#define		HDQ_WR_MODE		1				//HDQ 命令demo控制宏， 1/0(写/读命令处理，中断中)

/* Private function--------------------------------------------------------*/
// rgb led display
static void led_rgb_display(uint8_t *byColData, uint16_t hwLedNum);
static void set_led_rgb_store(uint32_t *pwLeddData,uint8_t *pbyDisp, uint16_t hwLedNum);

//ti hdq transfer
static uint32_t sio_hdq_addr_conversion(uint8_t byAddr);
static uint32_t sio_data_conversion(uint8_t byTxData);

/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

//rgb 原始数据
static uint8_t	s_byDipData[24] = 
{
	//R		G		B
	0x03,	0x07,	0x0a,	//rgb1
	0x04,	0x07,	0x05,	//rgb2
	0x05,	0x07,	0x01,	//rgb3
	0x06,	0x07,	0x02,	//rgb4
	0x07,	0x07,	0x03,	//rgb5
	0x08,	0x07,	0x04,	//rgb6
	0x09,	0x07,	0x05,	//rgb7
	0x0A,	0x07,	0x06 	//rgb8
};


static uint32_t 	s_wSioTxbuf[24];
static uint32_t 	s_wTxData = 0;
static uint8_t		s_byHdqDneFlg = 0;

#if (USE_SIO_CALLBACK == 0)	
	
static uint32_t		s_wSioRxBuf[24];		//接收缓存	
static uint16_t s_hwSendLen = 0;	
static uint16_t s_hwRecvLen = 0;



/** \brief  siox_int_handler: SIO中断服务函数
 * 
 *  \brief  SIO发生中断时会调用此函数，函数在interrupt.c里定义为弱(weak)属性，默认不做处理;用户用到中断
 * 			时，请重新定义此函数，在此函数中进行对应中断处理，也可直接在interrupt.c里的函数里进行处理。
 * 
 *  \param[in] none
 *  \return none
 */
ATTRIBUTE_ISR void sio0_int_handler(void) 
{
	uint8_t i, byIsr = csp_sio_get_isr(SIO0);

	//发送24个byte RGB数据, sio_led_rgb_txbufempt_int_demo使用
	if(byIsr & SIO_INT_TXBUFEMPT)			
	{
		csp_sio_clr_isr(SIO0, SIO_INT_TXBUFEMPT);
		if(s_hwSendLen < 24)
			csp_sio_set_txbuf(SIO0, s_wSioTxbuf[s_hwSendLen ++]);	
		else
		{
			csp_sio_int_disable(SIO0, SIO_INT_TXBUFEMPT);					
			s_hwSendLen = 0;
		}
	}
	
	//发送24个byte RGB数据, sio_led_rgb_txdne_int_demo使用
	if(byIsr & SIO_INT_TXDNE)			
	{
		s_hwSendLen++ ;
		csp_sio_clr_isr(SIO0, SIO_INT_TXDNE);
		if(s_hwSendLen < 8)
		{
			for(i = 0; i < 3; i++)
			{
				csp_sio_set_txbuf(SIO0, s_wSioTxbuf[3*s_hwSendLen + i]);
				while(!(csp_sio_get_risr(SIO0) & SIO_INT_TXBUFEMPT));
			}	
		}
		else
		{
			s_hwSendLen = 0;
		}
	}
	
	//从机接收数据，HDQ的demo使用，为区分主从机的接收，从机接收数据用TXDNE
	if(byIsr & SIO_INT_RXBUFFULL)
	{
		csp_sio_clr_isr(SIO0, SIO_INT_RXBUFFULL);
		
		s_wSioRxBuf[s_hwRecvLen ++] = (csp_sio_get_rxbuf(SIO0) >> 24);
	
		if((s_wSioRxBuf[0]) & HDQ_WR_CMD) 						//从机接收主机写数据命令
		{
			if(s_hwRecvLen >= 2)								//地址和数据两个数据接收完成
			{
				s_wSioRxBuf[0] &= ~HDQ_WR_CMD;					//去掉第7位命令位
				
				csp_sio_logic_sw_rst(SIO0);						//软件复位SIO的处理逻辑
				s_byHdqDneFlg = 1;								//完成标志位置1,
				s_hwRecvLen = 0;								//接收长度清零
				//用户添加处理,也可根据s_byHdqDneFlg在主程序中处理
			}
		}
		else 													//从机接收到主机读命令
		{
			s_hwRecvLen = 0;
			if(s_wSioRxBuf[0] == 0x68)							//判断地址				
			{
				csp_sio_set_wkmode(SIO0,SIO_MODE_TX);			//设置SIO为发送模式
				csi_sio_send(SIO0, &s_wTxData, 1);				//发送数据
				csp_sio_set_wkmode(SIO0,SIO_MODE_RX);			//设置SIO为接收模式
				csp_sio_logic_sw_rst(SIO0);						//软件复位SIO的处理逻辑
				s_byHdqDneFlg = 1;								//完成标志位置1,
			}
		}
	}
	
	//主机接收数据，HDQ的demo使用，为区分主从机的接收，主机接收数据用RXDNE
	if(byIsr & SIO_INT_RXDNE)
	{
		csp_sio_clr_isr(SIO0, SIO_INT_RXDNE);
		s_wSioRxBuf[0] = (csp_sio_get_rxbuf(SIO0) >> 24);
		
		if(s_wSioRxBuf[0] == 0x55)							//0x55为从机返回的数据						
			s_byHdqDneFlg = 1;								//完成标志位置1,
	}
	//break,HDQ的demo使用
	if(byIsr & SIO_INT_BREAK)
	{
		csp_sio_clr_isr(SIO0, SIO_INT_BREAK);
	}
}
#endif

/**	\brief 	sio_led_rgb_demo: SIO驱动RGB三色LED的demo
 * 
 *	\brief  sio 驱动RGB LED(ws2812), RGB DATA = 24bit; 驱动数据输出排列方式:GRB; 采用非中断方式
 * 
 *  \param[in] none
 *  \return error code
 */
int sio_led_rgb_demo(void)
{
	int iRet = 0;
	csi_sio_tx_config_t tSioTxCfg= {0};
	
#if (USE_GUI == 0)								//用户未选择图形化编程	
	//配置为SIO模式
	csi_gpio_set_mux(GPIOB, PB0, PB0_SIO0);	
#endif
	
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
	tSioTxCfg.eIdleLev 		= SIO_IDLE_LOW;		//SIO空闲时刻IO管脚输出电平
	tSioTxCfg.eTxDir 		= SIO_TXDIR_LSB;	//LSB->MSB, txbuf 数据按照bit[1:0]...[31:30]方式移出
	tSioTxCfg.wTxFreq 		= 4000000;			//tx clk =4MHz, Ttxshift = 1/4 = 250ns；发送每bit(1/0)是250ns*4 = 1us
	csi_sio_tx_init(SIO0, &tSioTxCfg);
	
	while(1)
	{
		led_rgb_display(s_byDipData, 8);
		mdelay(5);
	}
	
	return iRet;
}

/** \brief 	sio_led_rgb_txbufempt_int_demo：SIO驱动RGB三色LED的demo, 使用TXBUFEMPT中断
 * 
 *  \brief	sio 驱动RGB LED(ws2812), RGB DATA = 24bit; 驱动数据输出排列方式:GRB; 采用中断方式(TXBUFEMPT)
 * 
 *  \param[in] none
 *  \return error code
 */
int sio_led_rgb_txbufempt_int_demo(void)
{
	int iRet = 0;
	uint8_t byCount = 0;
	csi_sio_tx_config_t tSioTxCfg= {0};

#if (USE_GUI == 0)									//用户未选择图形化编程	
	csi_gpio_set_mux(GPIOB, PB0, PB0_SIO0);	
#endif

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
	tSioTxCfg.byTxCnt 		= 8;					//SIO一次发送总的数据长度(bit个数 = 8)，byTxCnt >= byTxBufLen，byTxCnt < 256(最大32bytes)
	tSioTxCfg.eIdleLev 		= SIO_IDLE_LOW;			//SIO空闲时刻IO管脚输出电平
	tSioTxCfg.eTxDir 		= SIO_TXDIR_LSB;		//MSB->LSB, txbuf 数据按照bit[1:0]...[31:30]方式移出
	tSioTxCfg.wTxFreq 		= 4000000;				//tx clk =4MHz, Ttxshift = 1/4 = 250ns；发送每bit(1/0)是250ns*4 = 1us
	csi_sio_tx_init(SIO0, &tSioTxCfg);				//初始化SIO发送配置
	
	//数据转换，RGB数据转换为SIO发送数据
	for(byCount = 0; byCount < 8; byCount++ )
	{
		set_led_rgb_store(s_wSioTxbuf, s_byDipData, byCount);
	}
	
	csi_sio_int_enable(SIO0, SIO_INTSRC_TXBUFEMPT);	//使能TXBUFEMPT中断, 中断函数中发送数据
	
	while(1)
	{
		mdelay(20);
	}
	
	return iRet;
}

/** \brief 	sio_led_rgb_txdne_int_demo：SIO驱动RGB三色LED的demo,使用TXDNE中断
 * 
 *  \brief	sio 驱动RGB LED(ws2812), RGB DATA = 24bit; 驱动数据输出排列方式:GRB; 采用中断方式(TXBUFEMPT)
 * 
 *  \param[in] none
 *  \return error code
 */
int sio_led_rgb_txdne_int_demo(void)
{
	int iRet = 0;
	uint8_t byCount = 0;
	csi_sio_tx_config_t tSioTxCfg= {0};

#if (USE_GUI == 0)									//用户未选择图形化编程	
	csi_gpio_set_mux(GPIOB, PB0, PB0_SIO0);	
#endif

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
	tSioTxCfg.byTxCnt 		= 24;					//SIO一次发送总的数据长度(bit个数 = 8)，byTxCnt >= byTxBufLen，byTxCnt < 256(最大32bytes)
	tSioTxCfg.eIdleLev 		= SIO_IDLE_LOW;			//SIO空闲时刻IO管脚输出电平
	tSioTxCfg.eTxDir 		= SIO_TXDIR_LSB;		//MSB->LSB, txbuf 数据按照bit[1:0]...[31:30]方式移出
	tSioTxCfg.wTxFreq 		= 4000000;				//tx clk =4MHz, Ttxshift = 1/4 = 250ns；发送每bit(1/0)是250ns*4 = 1us
	csi_sio_tx_init(SIO0, &tSioTxCfg);				//初始化SIO发送配置
	
	//数据转换，RGB数据转换为SIO发送数据
	for(byCount = 0; byCount < 8; byCount++ )
	{
		set_led_rgb_store(s_wSioTxbuf, s_byDipData, byCount);
	}
	
	csi_sio_int_enable(SIO0, SIO_INTSRC_TXDNE);		//使能TXBUFEMPT中断, 中断函数中发送数据
	csi_sio_send(SIO0, s_wSioTxbuf, 3);				//发送24个bit，产生TXDNE中断
	
	while(1)
	{
		mdelay(20);
	}
	
	return iRet;
}

/** \brief	sio_led_rgb_send_dma_demo: DMA方式SIO驱动RGB三色LED的demo
 * 
 *  \brief	sio 驱动RGB LED(ws2812), RGB DATA = 24bit; 驱动数据输出排列方式:GRB;采样DMA发送方式 
 * 
 *  \param[in] none
 *  \return error code
 */
int sio_led_rgb_send_dma_demo(void)
{
	int iRet = 0;
	uint8_t byCount = 0;
	uint32_t wDipDataEnd[24]= {0};
	uint16_t hwDmaSendData[24]= {0};
	
	csi_dma_ch_config_t tDmaConfig= {0};				
	csi_etcb_config_t 	tEtbConfig= {0};	
	csi_sio_tx_config_t tSioTxCfg= {0};

#if (USE_GUI == 0)										//用户未选择图形化编程	
	csi_gpio_set_mux(GPIOB, PB0, PB0_SIO0);	
#endif

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
	tSioTxCfg.eIdleLev 		= SIO_IDLE_LOW;				//SIO空闲时刻IO管脚输出电平
	tSioTxCfg.eTxDir 		= SIO_TXDIR_LSB;			//MSB->LSB, txbuf 数据按照bit[1:0]...[31:30]方式移出
	tSioTxCfg.wTxFreq 		= 4000000;					//tx clk =4MHz, Ttxshift = 1/4 = 250ns；发送每bit(1/0)是250ns*4 = 1us
	csi_sio_tx_init(SIO0, &tSioTxCfg);					//初始化SIO发送配置
	
	csi_sio_send_dma_enable(SIO0);						//SIO发送DMA使能
	
	for(byCount = 0; byCount < 8; byCount++ )			//RGB数据转换为SIO发送数据格式
	{
		set_led_rgb_store(wDipDataEnd, s_byDipData, byCount);	
	}
	
	for(uint8_t i = 0; i < 24; i++)
	{
		hwDmaSendData[i] = (uint16_t)wDipDataEnd[i];	//将数据转换为16bit的类型，dma发送数据宽度为16bit
	}
	
	//DMA配置初始化
	tDmaConfig.eSrcLinc 	= DMA_ADDR_CONSTANT;		//低位传输原地址固定不变
	tDmaConfig.eSrcHinc 	= DMA_ADDR_INC;				//高位传输原地址自增
	tDmaConfig.eDetLinc 	= DMA_ADDR_CONSTANT;		//低位传输目标地址固定不变
	tDmaConfig.eDetHinc 	= DMA_ADDR_CONSTANT;		//高位传输目标地址固定不变
	tDmaConfig.eDataWidth 	= DMA_DSIZE_16_BITS;		//传输数据宽度16bit
	tDmaConfig.eReload 		= DMA_RELOAD_DISABLE;		//禁止自动重载
	tDmaConfig.eRunMode 	= DMA_RUN_ONCE;			//DMA服务模式(传输模式)，连续服务
	tDmaConfig.eTsizeMode  	= DMA_TSIZE_ONE_DSIZE;		//传输数据大小，一个 DSIZE , 即DSIZE定义大小
	tDmaConfig.eReqMode		= DMA_REQ_HARDWARE;			//DMA请求模式，硬件触发
	iRet = csi_dma_ch_init(DMA0, DMA_CH1, &tDmaConfig);	//初始化DMA
	
	csi_dma_int_enable(DMA0, DMA_CH1, DMA_INTSRC_TCIT);	//使能DMA_CH2的TCIT中断
	
	//ETCB 配置初始化
	tEtbConfig.eChType 	= ETCB_ONE_TRG_ONE_DMA;			//单个源触发单个目标，DMA方式
	tEtbConfig.eSrcIp 	= ETCB_SIO0_TXSRC;				//SIO TXSRC作为触发源
	tEtbConfig.eDstIp 	= ETCB_DMA0_CH1;				//ETCB DMA通道 作为目标实际
	tEtbConfig.eTrgMode = ETCB_HARDWARE_TRG;			//通道触发模式采样硬件触发
	iRet = csi_etcb_ch_init(ETCB_CH21, &tEtbConfig);	//初始化ETCB
	if(iRet < CSI_OK)
		return CSI_ERROR;
	
	while(1)
	{
		csi_sio_send_dma(SIO0, DMA0, DMA_CH1, hwDmaSendData, 24);	
		
		mdelay(5);
		if(csi_dma_get_msg(DMA0, DMA_CH1, ENABLE))		//获取发送完成消息，并清除消息
		{
			//添加用户代码
			nop;
		}
	}
	
	return iRet;
}

/** \brief 	sio_hdq_master_wrcmd_demo: HDQ协议主机写数据命令demo，非中断方式
 * 
 *  \brief 	sio 实现TI HDQ单线通讯协议，主机发送数据；数据传输方式LSB, 低7位是地址，最高位是R/W(0/1)控制位；一次传输两个
 * 			数据，具体HDQ协议可百度查询；HDQ的第一个数据包含Break、addr+rw、Trsps，即第一个数据的bit数 = Break+(addr+rw)
 * 			+Trsps = 2 + 8 + 1 = 11bit; 第二个数据为1个byte，即8bit；总的一次要传输的bit数 = 19bit
 * 
 *  \param[in] none
 *  \return error code
 */
int sio_hdq_master_wrcmd_demo(void)
{
	int iRet = 0;
	csi_sio_tx_config_t tHdqTxCfg= {0};
	
	uint32_t wSendBuf[2];		
	uint8_t byHdqData[2] = {0x68, 0x55};
	
#if (USE_GUI == 0)									//用户未选择图形化编程	
	csi_gpio_set_mux(GPIOB, PB0, PB0_SIO0);	
#endif
	
	//SIO TX 参数配置
	tHdqTxCfg.byD0Len 		= 5;					//D0 对象序列长度(bit个数)，5个， break: 		__							
	tHdqTxCfg.byD1Len 		= 2;					//D1 对象序列长度(bit个数)，2个			 |_____|  |
	tHdqTxCfg.byDLLen 		= 5;					//DL 对象序列长度(bit个数)，5个
	tHdqTxCfg.byDHLen 		= 5;					//DH 对象序列长度(bit个数)，5个
	tHdqTxCfg.byDLLsq 		= 0x18;					//DL 对象序列具体定义: bit= 0 	   __      
													//					          |___|  |					
	tHdqTxCfg.byDHHsq 		= 0x1E;					//DH 对象序列具体定义: bit= 1 	 ____ 
													//					          |_|	 |
	tHdqTxCfg.byTxBufLen 	= 11;					//发送数据缓存长度(bit个数 = 11)，txbuf 一次发送bit数量，len <= 16
	tHdqTxCfg.byTxCnt 		= 19;					//SIO一次发送总的数据长度(bit个数 = 19)，byTxCnt >= byTxBufLen，byTxCnt < 256(最大32bytes)
	tHdqTxCfg.eIdleLev 		= SIO_IDLE_HIGH;		//SIO空闲时刻IO管脚输出电平
	tHdqTxCfg.eTxDir 		= SIO_TXDIR_LSB;		//MSB->LSB, txbuf 数据按照bit[1:0]...[31:30]方式移出
	tHdqTxCfg.wTxFreq 		= 25000;				//tx clk =25kHz, Ttxshift = 1/4 = 40us；发送每bit时间是40us
	csi_sio_tx_init(SIO0, &tHdqTxCfg);
	
	//发送写命令
	wSendBuf[0] = sio_hdq_addr_conversion(byHdqData[0] | HDQ_WR_CMD);	//第一个字节：地址+命令(低7位是地址，第8位是读写(0/1)控制位);
	wSendBuf[1] = sio_data_conversion(byHdqData[1]);					//第二个字节：数据字节
	
	while(1)
	{
		csi_sio_send(SIO0, wSendBuf, 2);			//用UART1的接收控制发送，即UART1接收到数据，SIO发送数据
		mdelay(5);
	}
	
	return iRet;
}



/** \brief 	sio_hdq_master_rdcmd_demo: HDQ协议主机读取数据命令demo，接收使用RXDNE中断(也可用RXBUFFULL)
 * 
 *  \brief 	SIO 实现TI HDQ单线通讯协议，主机读取数据(发送命令+收数据)；数据传输方式LSB, 低7位是地址，最高位是R/W(0/1)控制位；
 * 			两个数据（命令+数据），发送命令，接收1个byte数据。此demo和对应的sio_hdq_slave_receive_rdcmd_demo配合使用
 * 
 *  \param[in] none
 *  \return error code
 */
int sio_hdq_master_rdcmd_demo(void)
{
	int iRet = 0;
	csi_sio_tx_config_t tHdqTxCfg= {0};
	csi_sio_rx_config_t tHdqRxCfg= {0};
	
	uint32_t wRecvStart = 0;
	uint32_t wSendBuf[2]= {0};		
	uint8_t byHdqData[2] = {0x68, 0x55};
	
#if (USE_GUI == 0)										//用户未选择图形化编程	
	csi_gpio_set_mux(GPIOB, PB0, PB0_SIO0);	
#endif
	
	//SIO RX 参数配置
	tHdqRxCfg.byDebLen 	= 3;							//接收滤波周期	
	tHdqRxCfg.byDebClkDiv 	= 4;						//接收滤波时钟分频
	tHdqRxCfg.eTrgEdge 		= SIO_TRG_FALL;				//接收采样触发边沿，下降沿
	tHdqRxCfg.eTrgMode		= SIO_TRGMODE_DEB;			//接收采样触发模式，去抖后
	tHdqRxCfg.eRxDir 	 	= SIO_RXDIR_MSB;			//接收数据方向， 数据按照bit0...31方式移入，根据不同应用移入数据
	tHdqRxCfg.eSampMode		= SIO_SAMPMODE_EDGE;		//接收数据采样边沿对齐使能	
	tHdqRxCfg.eSampExtra	= SIO_EXTRACT_HI;			//采样提取策略，(20H)HITHR; (BIT = 1)个数 > HITHR 提取H,否则提取L
	tHdqRxCfg.byHiThres		= 13;						//提取判定值, (BIT = 1)个数 > HITHR 提取H,否则提取L
	tHdqRxCfg.byRxBufLen	= 8;						//发送数据缓存长度(bit个数 = 8)，rxbuf 一次接收bit数量，len <= 32				
	tHdqRxCfg.byRxCnt		= 8;						//SIO一次接收总的数据长度(bit个数 = 8)，byRxCnt >= byRxBufLen，byRxCnt < 256(最大32bytes)	
	tHdqRxCfg.bySampBitLen	= 19;						//bit采样的长度，每个bit采样次数为19，总得采样时间 = 19*Trxsamp = 190us			
	tHdqRxCfg.wRxFreq		= 100000;					//rx clk =100kHz, Trxsamp = 1/100kHz = 10us；每10us采样一次
	
	csi_sio_rx_init(SIO0, &tHdqRxCfg);					//初始化SIO接收参数
	
	csi_sio_set_break(SIO0, SIO_BKLEV_LOW, 19, ENABLE);	//检测接收break
	csi_sio_int_enable(SIO0, SIO_INTSRC_RXDNE);			//接收RXDNE中断，byRxCnt <= 32bit；接收byRxCnt(8)个bit，产生中断,读取数据到接收buf，每次读取byRxCnt(8)bit
	
	//SIO TX 参数配置
	tHdqTxCfg.byD0Len 		= 5;						//D0 对象序列长度(bit个数)，5个， break: 		__							
	tHdqTxCfg.byD1Len 		= 2;						//D1 对象序列长度(bit个数)，2个			 |_____|  |
	tHdqTxCfg.byDLLen 		= 5;						//DL 对象序列长度(bit个数)，5个
	tHdqTxCfg.byDHLen 		= 5;						//DH 对象序列长度(bit个数)，5个
	tHdqTxCfg.byDLLsq 		= 0x18;						//DL 对象序列具体定义: bit= 0 	   __      
														//					          |___|  |					
	tHdqTxCfg.byDHHsq 		= 0x1E;						//DH 对象序列具体定义: bit= 1 	 ____ 
														//					          |_|	 |
	tHdqTxCfg.byTxBufLen 	= 11;						//发送数据缓存长度(bit个数 = 11)，txbuf 一次发送bit数量，len <= 16
	tHdqTxCfg.byTxCnt 		= 11;						//SIO一次发送总的数据长度(bit个数 = 11)，byTxCnt >= byTxBufLen，byTxCnt < 256(最大32bytes)
	tHdqTxCfg.eIdleLev 		= SIO_IDLE_HIGH;			//SIO空闲时刻IO管脚输出电平
	tHdqTxCfg.eTxDir 		= SIO_TXDIR_LSB;			//MSB->LSB, txbuf 数据按照bit[1:0]...[31:30]方式移出
	tHdqTxCfg.wTxFreq 		= 25000;					//tx clk =25kHz, Ttxshift = 1/4 = 40us；发送每bit时间是40us
	csi_sio_tx_init(SIO0, &tHdqTxCfg);
	
	//发送读命令
	wSendBuf[0] = sio_hdq_addr_conversion(byHdqData[0]);//第一个字节：地址+命令(低7位是地址，第8位是读写(0/1)控制位);
	
	while(1)
	{
		
		csi_sio_send(SIO0, wSendBuf, 1);				//发送读数据命令
		csi_sio_set_wkmode(SIO0,SIO_RECV);				//设置SIO为接收模式,即进入接收模式
		
		wRecvStart = csi_tick_get_ms();					//获取时间			
		do{
			if(s_byHdqDneFlg)							//主机接收数据完成，标志位在中断里置位		
			{
				s_byHdqDneFlg = 0;
				csi_sio_set_wkmode(SIO0,SIO_SEND);		//设置SIO为发送模式
				//添加用户处理
				
				break;
			}
			//等待接收数据，超出两秒，则退出接收模式
			if((csi_tick_get_ms()- wRecvStart) >= 2000)	//超时处理，若超出2s 接收不到用户需要的数据则退出接收模式
			{
				csi_sio_set_wkmode(SIO0,SIO_SEND);		//设置SIO为发送模式
				break;
			}
			
		}while(1);
	}
	
	return iRet;
}

/** \brief 	sio_hdq_slave_demo: HDQ协议从机demo，接收主机的读写命令，接收使用RXBUFFULL中断
 * 
 *  \brief 	SIO 实现TI HDQ单线通讯协议，从机接收主机读数据命令；数据传输方式LSB, 低7位是地址，最高位是R/W(0/1)控制位；
 * 			从机收到1字节命令，返回（发送）1字节数据。此demo和对应的sio_hdq_master_rdcmd_demo配合使用
 * 
 *  \param[in] none
 *  \return error code
 */
int sio_hdq_slave_demo(void)
{
	int iRet = 0;
	csi_sio_rx_config_t tHdqRxCfg= {0};
	csi_sio_tx_config_t tHdqTxCfg= {0};
	uint8_t byTxChar = 0x55;
	
#if (USE_GUI == 0)												//用户未选择图形化编程	
	csi_gpio_set_mux(GPIOB, PB0, PB0_SIO0);	
#endif
	
	//SIO TX 参数配置
	tHdqTxCfg.byD0Len 		= 5;								//D0 对象序列长度(bit个数)，5个， break: 		__							
	tHdqTxCfg.byD1Len 		= 2;								//D1 对象序列长度(bit个数)，2个			 |_____|  |
	tHdqTxCfg.byDLLen 		= 5;								//DL 对象序列长度(bit个数)，5个
	tHdqTxCfg.byDHLen 		= 5;								//DH 对象序列长度(bit个数)，5个
	tHdqTxCfg.byDLLsq 		= 0x18;								//DL 对象序列具体定义: bit= 0 	   __      
																//					          |___|  |					
	tHdqTxCfg.byDHHsq 		= 0x1E;								//DH 对象序列具体定义: bit= 1 	 ____ 
																//					          |_|	 |
	tHdqTxCfg.byTxBufLen 	= 8;								//发送数据缓存长度(bit个数 = 8)，txbuf 一次发送bit数量，len <= 16
	tHdqTxCfg.byTxCnt 		= 8;								//SIO一次发送总的数据长度(bit个数 = 8)，byTxCnt >= byTxBufLen，byTxCnt < 256(最大32bytes)
	tHdqTxCfg.eIdleLev 		= SIO_IDLE_HIGH;					//SIO空闲时刻IO管脚输出电平
	tHdqTxCfg.eTxDir 		= SIO_TXDIR_LSB;					//MSB->LSB, txbuf 数据按照bit[1:0]...[31:30]方式移出
	tHdqTxCfg.wTxFreq 		= 25000;							//tx clk =25kHz, Ttxshift = 1/4 = 40us；发送每bit时间是40us
	csi_sio_tx_init(SIO0, &tHdqTxCfg);							//初始化SIO发送参数
	
	s_wTxData = sio_data_conversion(byTxChar);					//发送数据转换，数据发送在中断函数里执行
	
	//SIO RX 参数配置
	tHdqRxCfg.byDebLen 	= 3;									//接收滤波周期	
	tHdqRxCfg.byDebClkDiv 	= 4;								//接收滤波时钟分频
	tHdqRxCfg.eTrgEdge 		= SIO_TRG_FALL;						//接收采样触发边沿，下降沿
	tHdqRxCfg.eTrgMode		= SIO_TRGMODE_DEB;					//接收采样触发模式，去抖后
	tHdqRxCfg.eRxDir 	 	= SIO_RXDIR_MSB;					//接收数据方向， 数据按照bit0...31方式移入，根据不同应用移入数据
	tHdqRxCfg.eSampMode		= SIO_SAMPMODE_EDGE;				//接收数据采样边沿对齐使能	
	tHdqRxCfg.eSampExtra	= SIO_EXTRACT_HI;					//采样提取策略，(20H)HITHR; (BIT = 1)个数 > HITHR 提取H,否则提取L
	tHdqRxCfg.byHiThres		= 13;								//提取判定值, (BIT = 1)个数 > HITHR 提取H,否则提取L
	tHdqRxCfg.byRxBufLen	= 8;								//发送数据缓存长度(bit个数 = 8)，rxbuf 一次接收bit数量，len <= 32				
	tHdqRxCfg.byRxCnt		= 16;								//SIO一次接收总的数据长度(bit个数 = 16)，byRxCnt >= byRxBufLen，byRxCnt < 256(最大32bytes)	
	tHdqRxCfg.bySampBitLen	= 19;								//bit采样的长度，每个bit采样次数为19，总得采样时间 = 19*Trxsamp = 190us			
	tHdqRxCfg.wRxFreq		= 100000;							//rx clk =100kHz, Trxsamp = 1/100kHz = 10us；每10us采样一次
	csi_sio_rx_init(SIO0, &tHdqRxCfg);							//初始化SIO接收参数
	
	csi_sio_set_break(SIO0, SIO_BKLEV_LOW, 19, ENABLE);			//接收检测break, HDQ协议起始需要break总线
	csi_sio_int_enable(SIO0, SIO_INTSRC_BREAK);					//使能BREAK中断
	csi_sio_int_enable(SIO0, SIO_INTSRC_RXBUFFULL);				//使能RXBUFFULL中断，接收byRxBufLen(8)个bit，产生中断,读取数据到接收buf，每次读取byRxBufLen(8)bit
	
	
	while(1)
	{
		if(s_byHdqDneFlg)										//数据完成状态标志，置位时标识从机接收数据完成或者从机发送数据完成								
		{
			s_byHdqDneFlg = 0;
			//添加用户处理
		}
		mdelay(5);
	}
	
	return iRet;
}

/** \brief 	sio_led_data_conver: 数据转换为SIO格式
 * 
 *  \param[in] byData：需要转换的数据
 *  \return  SIO格式数据
 */
static uint32_t sio_led_data_conversion(uint8_t byData)
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

/** \brief 	set_led_rgb_store:  RGB LED const数组中数据转换装载
 * 
 *  \param[in] pwLeddData：转换后的数据buf指针
 *  \param[in] pbyDisp：需要转换的RGB LED 数组数据
 *  \param[in] hwLedNum：转换的数量
 *  \return  无
 */
static void set_led_rgb_store(uint32_t *pwLeddData, uint8_t *pbyDisp, uint16_t hwLedNum)
{
	//led
	*(pwLeddData+hwLedNum*3) = sio_led_data_conversion(pbyDisp[hwLedNum*3+1]) ;		//G
	*(pwLeddData+hwLedNum*3+1) = sio_led_data_conversion(pbyDisp[hwLedNum*3]);		//R		
	*(pwLeddData+hwLedNum*3+2) = sio_led_data_conversion(pbyDisp[hwLedNum*3+2]);	//B	
}

/** \brief 	led_rgb_display:  RGB LED数据显示，发送数据(驱动LED)
 * 
 *  \param[in] pwLeddData：要显示的RGB LED 数据buf指针
 *  \param[in] hwLedNum：LED数量
 *  \return  无
 */
static void led_rgb_display(uint8_t *byColData, uint16_t hwLedNum)
{
	uint16_t i;
	uint32_t wRgbData[24]= {0};
	for(i = 0; i < hwLedNum; i++)
	{
		set_led_rgb_store(wRgbData,byColData, i);
	}
	for(i = 0; i < hwLedNum; i++)
	{
		csi_sio_send(SIO0, &wRgbData[3*i],3);
	}
}

/** \brief 	sio_hdq_addr_conver: SIO HDQ地址数据格式转换
 * 
 *  \param[in] byAddr：需要转换的地址
 *  \return  SIO格式数据
 */
static uint32_t sio_hdq_addr_conversion(uint8_t byAddr)
{
	uint8_t i;
	volatile uint32_t wDout = 0x00;
	
	wDout |=  SIO_TXBUF_D0 << 0;			//t(B)	break
	wDout |=  SIO_TXBUF_D1 << 2 ;			//t(BR) 
	
	for(i = 0; i < 8; i++)
	{
		if(byAddr & 0x01)
			wDout |=  (SIO_TXBUF_DH << ((i+2) *2));
		else
			wDout |=  (SIO_TXBUF_DL << ((i+2) *2));
			
		byAddr = byAddr >> 1;
	}
	
	wDout |=  SIO_TXBUF_D1 << 20 ;		//t(RSPS)
	
	return wDout;
}

/** \brief 	sio_data_conver: SIO通用数据格式转换
 * 
 *  \param[in] byTxData：需要转换的数据
 *  \return  SIO格式数据
 */
static uint32_t sio_data_conversion(uint8_t byTxData)
{
	uint8_t i;
	volatile uint32_t wDout = 0x00;
	
	for(i = 0; i < 8; i++)
	{
		if(byTxData & 0x01)
			wDout |=  (SIO_TXBUF_DH << (i*2));
		else
			wDout |=  (SIO_TXBUF_DL << (i*2));
			
		byTxData = byTxData >> 1;
			
	}
	return wDout;
	
}
