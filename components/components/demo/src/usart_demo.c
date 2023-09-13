/***********************************************************************//** 
 * \file  usart_demo.c
 * \brief  USART_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-6-30 <td>V0.0 <td>ZJY     <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include <drv/usart.h>
#include <drv/pin.h>


#include "demo.h"

/* externs function--------------------------------------------------------*/
/* externs variable------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variable------------------------------------------------------*/

uint8_t byRecvBuf[64] = {0};					//接收缓存
uint8_t bySendBuf[30]={1,2,3,4,5,6,7,8,9,21,22,23,24,25,26,27,28,29,30,10,11,12,13,14,15,16,17,18,19};	

static uint16_t hwSendLen = 0;	

/** \brief  usart0_int_handler: USART中断服务函数
 * 
    \brief  USART发生中断时会调用此函数，函数在interrupt.c里定义为弱(weak)属性，默认不做处理;用户用到中断
 * 			时，请重新定义此函数，在此函数中进行对应中断处理，也可直接在interrupt.c里的函数里进行处理。
 * 
 *  \param[in] none
 *  \return none
 */
ATTRIBUTE_ISR void usart0_int_handler(void) 
{
	//TXFIFO中断
	if(csp_usart_get_isr(USART0)&US_TXFIFO_INT)				//TXFIFO中断
	{
		//发送16字节数据，发送完毕关闭中断(停止发送)
		if(hwSendLen < 16)
			csp_usart_set_data(USART0, bySendBuf[hwSendLen++]);
		else
		{
			hwSendLen = 0;
			csi_usart_int_disable(USART0, USART_INTSRC_TXFIFO);	//关闭中断	
		}
	}
	
	//RXFIFO中断
	if(csp_usart_get_isr(USART0)&US_RXFIFO_INT)				//RXFIFO中断
	{
		//接收1字节数据
		byRecvBuf[0] = csp_usart_get_data(USART0);				//接收数据,RXFIFO中断状态不需要专门清除，读数据时自动清除
	}
	
	//接收超时中断
	if(csp_usart_get_isr(USART0)& US_RXTO_INT)				//接收超时中断
	{
		//添加用户处理(若开启此中断)
		csp_usart_clr_isr(USART0, US_RXTO_INT);				//清除接收超时中断状态
		csp_usart_cr_cmd(USART0,US_STTTO);					//使能接收超时
	}
}

/** \brief usart_send_demo: USART0 轮询发送18个字节数据
 *  	 
 *  \param[in] none
 *  \return error code
 */
 
int usart_send_demo(void)
{
	int iRet = 0;
	csi_usart_config_t tUsartCfg;						//USART0 参数配置结构体
	
#if !defined(USE_GUI)	

	csi_gpio_set_mux(GPIOA, PA9, PA9_USART0_TX);		//USART0 TX管脚配置	
	csi_gpio_set_mux(GPIOA, PA10, PA10_USART0_RX);		//USART0 RX管脚配置
	csi_gpio_pull_mode(GPIOA, PA10, GPIO_PULLUP);		//RX管脚上拉使能, 建议配置
	
#endif	
	tUsartCfg.byClkSrc 		= USART_CLKSRC_DIV1;		//clk = PCLK
	tUsartCfg.byMode		= USART_MODE_ASYNC;			//异步模式
	tUsartCfg.byDatabit 	= USART_DATA_BITS_8;		//字节长度，8bit
	tUsartCfg.byStopbit 	= USART_STOP_BITS_1;		//停止位，1个
	tUsartCfg.byParity		= USART_PARITY_EVEN;		//偶校验
	tUsartCfg.wBaudRate 	= 115200;					//波特率：115200

	
	csi_usart_init(USART0, &tUsartCfg);					//初始化串口	
	csi_usart_start(USART0, USART_FUNC_RX_TX);			//开启USART的RX和TX功能，也可单独开启RX或者TX功能
	
	while(1)
	{

		if(csi_usart_send(USART0,(void *)bySendBuf,18) != 18)
			return -1;
		
		mdelay(100);
	}

	return iRet;
}


/** \brief usart_send_int_demo : USART中断发送数据
 * 
 *  \brief  使用TXFIFO中断发送，用户直接在中断服务函数(usart0_int_handler)里处理；TXFIFO中断，即TXFIFO空
 * 			产生中断，开启时若TXFIFO中无数据，则产生中断 
 * 
 *  \param[in] none
 *  \return error code
 */
int usart_send_int_demo(void)
{
	int iRet = 0;
	csi_usart_config_t tUsartCfg;						//USART0 参数配置结构体
	
	
#if !defined(USE_GUI)

	csi_gpio_set_mux(GPIOA, PA9, PA9_USART0_TX);		//USART0 TX管脚配置	
	csi_gpio_set_mux(GPIOA, PA10, PA10_USART0_RX);		//USART0 RX管脚配置
	csi_gpio_pull_mode(GPIOA, PA10, GPIO_PULLUP);		//RX管脚上拉使能, 建议配置
	
#endif	

	tUsartCfg.byClkSrc 		= USART_CLKSRC_DIV1;		//clk = PCLK
	tUsartCfg.byMode		= USART_MODE_ASYNC;			//异步模式
	tUsartCfg.byDatabit 	= USART_DATA_BITS_8;		//字节长度，8bit
	tUsartCfg.byStopbit 	= USART_STOP_BITS_1;		//停止位，1个
	tUsartCfg.byParity		= USART_PARITY_EVEN;		//偶校验
	tUsartCfg.wBaudRate 	= 115200;					//波特率：115200
	
	csi_usart_init(USART0, &tUsartCfg);					//初始化串口	
	csi_usart_start(USART0, USART_FUNC_RX_TX);			//开启USART的RX和TX功能，也可单独开启RX或者TX功能

	csi_usart_int_enable(USART0, USART_INTSRC_TXFIFO);  //使能TXFIFO中断
	
	while(1)
	{
		mdelay(100);
	}


	return iRet;	
}



/** \brief  usart_recv_int_demo：usart中断接收数据
 * 
 *  \brief  使用RXFIFO中断接收，用户直接在中断服务函数(usart0_int_handler)里处理
 * 
 * @ 接收超时:	即接收字节超时，指接收数据时，两个字节之间的时间，超过设定值，产生超时中断；
 * 
 * @ eRxFifoTrg: 接收FIFO触发点 \ref csi_usart_rxfifo_trg_e
 * 
 * 					USART_RXFIFOTRG_ONE  - FIFO收到1个字节数据触发中断
 * 					USART_RXFIFOTRG_TWO  - FIFO收到2个字节数据触发中断
 * 					USART_RXFIFOTRG_FOUR - FIFO收到4个字节数据触发中断
 * 
 * 				Notes:接收FIFO触发点设置为2/4时，一定要开启接收超时中断，否则可能会因为数据长度不能达到触发点而收不到数据
 * 
 *  \param[in] none
 *  \return error code
 */
int usart_recv_int_demo(void)
{
	int iRet = 0;
	csi_usart_config_t tUsartCfg;							//USART0 参数配置结构体
	
	
#if !defined(USE_GUI)
	
	csi_gpio_set_mux(GPIOA, PA9, PA9_USART0_TX);			//USART0 TX管脚配置	
	csi_gpio_set_mux(GPIOA, PA10, PA10_USART0_RX);			//USART0 RX管脚配置
	csi_gpio_pull_mode(GPIOA, PA10, GPIO_PULLUP);			//RX管脚上拉使能, 建议配置
	
#endif	

	tUsartCfg.byClkSrc 		= USART_CLKSRC_DIV1;			//clk = PCLK
	tUsartCfg.byMode		= USART_MODE_ASYNC;				//异步模式
	tUsartCfg.byDatabit 	= USART_DATA_BITS_8;			//字节长度，8bit
	tUsartCfg.byStopbit 	= USART_STOP_BITS_1;			//停止位，1个
	tUsartCfg.byParity		= USART_PARITY_EVEN;			//偶校验
	tUsartCfg.wBaudRate 	= 115200;						//波特率：115200
	tUsartCfg.hwRecvTo		= 88;							//USART接收超时时间，单位：bit位周期，8个bytes(11bit*8=88, 115200波特率时=764us)
	tUsartCfg.eRxFifoTrg    = USART_RXFIFOTRG_ONE;
	
	csi_usart_init(USART0, &tUsartCfg);						//初始化串口
	csi_usart_start(USART0, USART_FUNC_RX_TX);				//开启USART的RX和TX功能，也可单独开启RX或者TX功能
	
	csi_usart_int_enable(USART0, USART_INTSRC_RXFIFO);  	//使能RXFIFO中断
	//csi_usart_int_enable(USART0, USART_INTSRC_TIMEOUT);	//开启字节接收超时中断
	
	while(1)
	{
		mdelay(100);
	}
	
	return iRet;
}

/** \brief USART通过DMA发送数据
 *  	 
 * 
 *  \param[in] none
 *  \return error code
 */
int usart_send_dma_demo(void)
{
	int iRet = 0;
	csi_usart_config_t tUsartCfg;						//USART0 参数配置结构体
	csi_dma_ch_config_t tDmaConfig;						//DMA 参数配置结构体			
	csi_etcb_config_t 	tEtbConfig;						//ETCB 参数配置结构体
	
#if !defined(USE_GUI)

	csi_gpio_set_mux(GPIOA, PA9, PA9_USART0_TX);		//USART0 TX管脚配置	
	csi_gpio_set_mux(GPIOA, PA10, PA10_USART0_RX);		//USART0 RX管脚配置
	csi_gpio_pull_mode(GPIOA, PA10, GPIO_PULLUP);		//RX管脚上拉使能, 建议配置

#endif	

	//usart 参数配置
	tUsartCfg.byClkSrc 		= USART_CLKSRC_DIV1;		//clk = PCLK
	tUsartCfg.byMode		= USART_MODE_ASYNC;			//异步模式
	tUsartCfg.byDatabit 	= USART_DATA_BITS_8;		//字节长度，8bit
	tUsartCfg.byStopbit 	= USART_STOP_BITS_1;		//停止位，1个
	tUsartCfg.byParity		= USART_PARITY_EVEN;		//偶校验
	tUsartCfg.wBaudRate 	= 115200;					//波特率：115200
	
	csi_usart_init(USART0, &tUsartCfg);					//初始化串口
	csi_usart_start(USART0, USART_FUNC_RX_TX);			//开启USART的RX和TX功能，也可单独开启RX或者TX功能
	

	//dma 参数配置
	tDmaConfig.eSrcLinc 	= DMA_ADDR_CONSTANT;		//低位传输原地址固定不变
	tDmaConfig.eSrcHinc 	= DMA_ADDR_INC;				//高位传输原地址自增
	tDmaConfig.eDetLinc 	= DMA_ADDR_CONSTANT;		//低位传输目标地址固定不变
	tDmaConfig.eDetHinc 	= DMA_ADDR_CONSTANT;		//高位传输目标地址固定不变
	tDmaConfig.eDataWidth 	= DMA_DSIZE_8_BITS;			//传输数据宽度8bit
	tDmaConfig.eReload 		= DMA_RELOAD_DISABLE;		//禁止自动重载
	tDmaConfig.eTransMode 	= DMA_TRANS_CONTINU;		//DMA服务模式(传输模式)，连续服务
	tDmaConfig.eTsizeMode  	= DMA_TSIZE_ONE_DSIZE;		//传输数据大小，一个 DSIZE , 即DSIZE定义大小
	tDmaConfig.eReqMode		= DMA_REQ_HARDWARE;			//DMA请求模式，软件请求（软件触发）
	
	csi_dma_ch_init(DMA0, DMA_CH0, &tDmaConfig);	    //初始化DMA
	
	csi_dma_int_enable(DMA0, DMA_CH0, DMA_INTSRC_LTCIT);
	csi_usart_set_txdma(USART0, USDMA_TX_FIF0_TRG);
	
	
	//etcb 参数配置
	csi_etcb_init();										//使能ETB模块
		
	tEtbConfig.eChType = ETB_ONE_TRG_ONE_DMA;			//单个源触发单个目标，DMA方式
	tEtbConfig.eSrcIp 	= ETB_USART0_TXSRC;				//UART TXSRC作为触发源
	tEtbConfig.eDstIp 	= ETB_DMA0_CH0 + DMA_CH0;		//ETB DMA通道 作为目标实际
	tEtbConfig.eTrgMode = ETB_HARDWARE_TRG;				//通道触发模式采样硬件触发
	
	iRet = csi_etcb_ch_config(ETB_CH20, &tEtbConfig);	//初始化ETB，DMA ETB CHANNEL > ETB_CH19_ID

	while(1)
	{
		csi_usart_send_dma(USART0,(void *)bySendBuf, DMA_CH0, 31);	//采用DMA方式发送
		
		mdelay(100);
		if(csi_dma_get_msg(DMA0,DMA_CH0, ENABLE))		//获取发送完成消息，并清除消息
		{
			//添加用户代码
			nop;
		}
		mdelay(10);
	}
	
	return iRet;
}

/** \brief usart dma receive data
 *  	 - USART通过DMA接收数据
 * 
 *  \param[in] none
 *  \return error code
 */
int usart_recv_dma_demo(void)
{
	int iRet = 0;
	csi_usart_config_t tUsartCfg;						//USART0 参数配置结构体
	csi_dma_ch_config_t tDmaConfig;						//DMA 参数配置结构体
	csi_etcb_config_t 	tEtbConfig;						//ETCB 参数配置结构体
	
	
#if !defined(USE_GUI)

	csi_gpio_set_mux(GPIOA, PA9, PA9_USART0_TX);		//USART0 TX管脚配置	
	csi_gpio_set_mux(GPIOA, PA10, PA10_USART0_RX);		//USART0 RX管脚配置
	csi_gpio_pull_mode(GPIOA, PA10, GPIO_PULLUP);		//RX管脚上拉使能, 建议配置
		
#endif	

	//usart 参数配置
	tUsartCfg.byClkSrc 		= USART_CLKSRC_DIV1;		//clk = PCLK
	tUsartCfg.byMode		= USART_MODE_ASYNC;			//异步模式
	tUsartCfg.byDatabit 	= USART_DATA_BITS_8;		//字节长度，8bit
	tUsartCfg.byStopbit 	= USART_STOP_BITS_1;		//停止位，1个
	tUsartCfg.byParity		= USART_PARITY_EVEN;		//偶校验
	tUsartCfg.wBaudRate 	= 115200;					//波特率：115200
	
	csi_usart_init(USART0, &tUsartCfg);					//初始化串口
	csi_usart_start(USART0, USART_FUNC_RX_TX);			//开启USART的RX和TX功能，也可单独开启RX或者TX功能
	
	
	
	//dma 参数配置
	tDmaConfig.eSrcLinc 	= DMA_ADDR_CONSTANT;		//低位传输原地址固定不变
	tDmaConfig.eSrcHinc 	= DMA_ADDR_CONSTANT;		//高位传输原地址固定不变
	tDmaConfig.eDetLinc 	= DMA_ADDR_CONSTANT;		//低位传输目标地址固定不变
	tDmaConfig.eDetHinc 	= DMA_ADDR_INC;				//高位传输目标地址自增
	tDmaConfig.eDataWidth 	= DMA_DSIZE_8_BITS;			//传输数据宽度8bit
	tDmaConfig.eReload 		= DMA_RELOAD_DISABLE;			//自动重载
	tDmaConfig.eTransMode 	= DMA_TRANS_CONTINU;		//DMA服务模式(传输模式)，连续服务
	tDmaConfig.eTsizeMode  	= DMA_TSIZE_ONE_DSIZE;		//传输数据大小，一个 DSIZE , 即DSIZE定义大小
	tDmaConfig.eReqMode		= DMA_REQ_HARDWARE;				//DMA请求模式，硬件请求
	csi_dma_ch_init(DMA0, DMA_CH3, &tDmaConfig);		//初始化DMA
	
	csi_dma_int_enable(DMA0, DMA_CH3, DMA_INTSRC_LTCIT);
	csi_usart_set_rxdma(USART0, USDMA_RX_FIFO_NSPACE);
	
	
	//etcb 参数配置
	csi_etcb_init();										//使能ETB模块
	
	tEtbConfig.eChType = ETB_ONE_TRG_ONE_DMA;			//单个源触发单个目标，DMA方式
	tEtbConfig.eSrcIp 	= ETB_USART0_RXSRC;				//UART TXSRC作为触发源
	tEtbConfig.eDstIp 	= ETB_DMA0_CH0 + DMA_CH3;		//ETB DMA通道 作为目标实际
	tEtbConfig.eTrgMode = ETB_HARDWARE_TRG;				//通道触发模式采样硬件触发
										
	csi_etcb_ch_config(ETB_CH20, &tEtbConfig);			//初始化ETB，DMA ETB CHANNEL > ETB_CH19_ID


	csi_usart_recv_dma(USART0,(void*)byRecvBuf, DMA_CH3, 25);	//DMA接收
	
	while(1)
	{
		if(csi_dma_get_msg(DMA0,DMA_CH3, ENABLE))		//获取接收完成消息，并清除消息
		{
			//添加用户代码
			csi_usart_send(USART0, (void*)byRecvBuf, 25);
			
			break;
		}
		mdelay(10);
	}
	
	return iRet;
}


