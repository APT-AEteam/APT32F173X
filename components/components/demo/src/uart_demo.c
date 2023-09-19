/***********************************************************************//** 
 * \file  uart_demo.c
 * \brief  UART_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-9-12 <td>V0.0 <td>ZJY     <td>initial
 * <tr><td> 2023-9-19 <td>V0.1 <td>ZJY     <td>initial
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
static uint8_t s_byRecvBuf[64]={0};			//receive buf
static uint8_t s_bySendBuf[30]={1,2,3,4,5,6,7,8,9,21,22,23,24,25,26,27,28,29,30,10,11,12,13,14,15,16,17,18,19};	


#if (USE_BT_CALLBACK == 0)	
	
static uint16_t hwSendLen = 0;	

/** \brief  uartx_int_handler: UART中断服务函数
 * 
    \brief  UART发生中断时会调用此函数，函数在interrupt.c里定义为弱(weak)属性，默认不做处理;用户用到中断
 * 			时，请重新定义此函数，在此函数中进行对应中断处理，也可直接在interrupt.c里的函数里进行处理。
 * 
 *  \param[in] none
 *  \return none
 */
ATTRIBUTE_ISR void uart1_int_handler(void) 
{
	//TXFIFO中断
	if(csp_uart_get_isr(UART1)&UART_INT_TXFIFO_S)				//TXFIFO中断
	{
		//发送16字节数据，发送完毕关闭中断(停止发送)
		if(hwSendLen < 16)
			csp_uart_set_data(UART1, s_bySendBuf[hwSendLen++]);
		else
		{
			hwSendLen = 0;
			csp_uart_int_disable(UART1, UART_INT_RXFIFO);		//关闭rxfifo/中断	
		}
	}
	
	//RXFIFO中断
	if(csp_uart_get_isr(UART1)&UART_INT_RXFIFO_S)				//RXFIFO中断
	{
		//添加用户处理
		s_byRecvBuf[0] = csp_uart_get_data(UART1);				//接收数据,RXFIFO中断状态不需要专门清除，读数据时自动清除
	}
	
	//接收超时中断
	if(csp_uart_get_isr(UART1)& UART_INT_RXTO_S)				//接收超时中断
	{
		//添加用户处理(若开启此中断)
		csp_uart_clr_isr(UART1, UART_INT_RXTO_S);				//清除接收超时中断状态
		csp_uart_rto_enable(UART1);								//使能接收超时
	}
}
#endif

/**	\brief	uart_send_demo： UART发送demo，不使用中断
 * 
 *  \param[in] none
 *  \return error code
 */
int uart_send_demo(void)
{
	int iRet = 0;
	csi_uart_config_t tUartConfig;					//UART1 参数配置结构体
	
#if (USE_GUI == 0)	
	csi_gpio_set_mux(GPIOA, PA4, PA4_UART1_TX);		//TX	
	csi_gpio_set_mux(GPIOA, PA5, PA5_UART1_RX);		//RX
	csi_gpio_pull_mode(GPIOA, PA5, GPIO_PULLUP);	//RX管脚上拉使能, 建议配置
#endif	
	tUartConfig.eParity 	= UART_PARITY_ODD;		//校验位，奇校验
	tUartConfig.wBaudRate 	= 115200;				//波特率，115200
	csi_uart_init(UART1, &tUartConfig);				//初始化串口
	
	csi_uart_start(UART1, UART_FUNC_RX_TX);			//开启UART的RX和TX功能，也可单独开启RX或者TX功能
	
	while(1)
	{
		//不使用中断发送，发送接口返回发送数据长度；是否判断返回长度，由用户根据实际应用决定
		if (csi_uart_send(UART1,(void *)s_bySendBuf, 26) != 26)
		{	
			return -1;
		}
		mdelay(100);
	}
	
	return iRet;
}

/** \brief	uart_send_int_demo：UART数据发送demo，使用中断
 * 
 *  \brief  使用TXFIFO中断发送，用户直接在中断服务函数(uart1_int_handler)里处理；TXFIFO中断，即TXFIFO空
 * 			产生中断，开启时若TXFIFO中无数据，则产生中断
 * 
 *  \param[in] none
 *  \return error code
 */
int uart_send_int_demo(void)
{
	int iRet = 0;
	csi_uart_config_t tUartConfig;					//UART1 参数配置结构体
	
#if (USE_GUI == 0)	
	csi_gpio_set_mux(GPIOA, PA4, PA4_UART1_TX);		//TX	
	csi_gpio_set_mux(GPIOA, PA5, PA5_UART1_RX);		//RX
	csi_gpio_pull_mode(GPIOA, PA5, GPIO_PULLUP);	//RX管脚上拉使能, 建议配置
#endif	

	tUartConfig.eParity 	= UART_PARITY_ODD;		//校验位，奇校验
	tUartConfig.wBaudRate 	= 115200;				//波特率，115200
	csi_uart_init(UART1, &tUartConfig);				//初始化串口
	
	csi_uart_int_enable(UART1, UART_INTSRC_TXFIFO);	//开启TXFIFO中断
	csi_uart_start(UART1, UART_FUNC_RX_TX);			//开启UART的RX和TX功能，也可单独开启RX或者TX功能
	
	//使用中断方式发送，发送在UART1的中断服务函数(uart1_int_handler)里处理
	while(1)
	{
		mdelay(10);
	}
	
	return iRet;
}

/** \brief  uart_recv_int_demo：串口接收,使用中断模式
 * 
 *  \brief  使用RXFIFO中断接收，用户直接在中断服务函数(uart1_int_handler)里处理
 * 
 * @ 接收超时:	即接收字节超时，指接收数据时，两个字节之间的时间，超过设定值，产生超时中断；
 * 
 * @ 接收fifo触发点:接收FIFO中断触发点，支持1/2/4这三种模式，即FIFO收到1个字节还是多个字节才会触发RXFIFO中
 * 				断；初始化时选择1，即FIFO中有数据则产生中断。
 * 				
 * 
 *  \param[in] none
 *  \return error code
 */
int uart_recv_int_demo(void)
{
	int iRet = 0;
	csi_uart_config_t tUartConfig;					//UART1 参数配置结构体
	
#if (USE_GUI == 0)	
	csi_gpio_set_mux(GPIOA, PA4, PA4_UART1_TX);		//TX	
	csi_gpio_set_mux(GPIOA, PA5, PA5_UART1_RX);		//RX
	csi_gpio_pull_mode(GPIOA,PA5,GPIO_PULLUP);		//RX管脚上拉使能, 建议配置
#endif	

	tUartConfig.eParity 	= UART_PARITY_ODD;		//校验位，奇校验
	tUartConfig.wBaudRate 	= 115200;				//波特率，115200
	tUartConfig.hwRecvTo 	= 88;					//UART接收超时时间，单位：bit位周期，8个bytes(11bit*8=88, 115200波特率时=764us)
	tUartConfig.eRxFifoTrg  = UART_RXFIFOTRG_ONE;	//UART的RXFIFO的中断触发点设置为1，即RXFIFO接收到数据,就会触发RXFIFO中断；支持三种配置(1/2/4)
	csi_uart_init(UART1, &tUartConfig);				//初始化串口
	
	csi_uart_int_enable(UART1, UART_INTSRC_RXFIFO);	//开启RXFIFO中断，字节接收超时中断不开启(用户可根据具体处理需要是否开启）
	//csi_uart_int_enable(UART1, UART_INTSRC_RXTO);	//开启字节接收超时中断
	csi_uart_start(UART1, UART_FUNC_RX_TX);			//开启UART的RX和TX功能，也可单独开启RX或者TX功能
	
	
	
	//接收在用户在中断服务函数(uart1_int_handler)里处理
	while(1)
	{
		mdelay(10);
	}
	return iRet;
}

/** \brief	uart_send_dma_demo: UART通过DMA发送数据demo
 *  \brief	UART DMA发送数据，需使用ETCB模块，配置对应触发源和触发目标
 * 
 *  \param[in] none
 *  \return error code
 */
int uart_send_dma_demo(void)
{
	int iRet = 0;
	csi_uart_config_t tUartConfig;								//UART1 参数配置结构体
	csi_dma_ch_config_t tDmaConfig;								//DMA 通道配置结构体			
	csi_etcb_config_t 	tEtbConfig;								//ETCB 配置结构体
	
#if (USE_GUI == 0)		
	csi_gpio_set_mux(GPIOA,PA4, PA4_UART1_TX);					//TX	
	csi_gpio_set_mux(GPIOA, PA5, PA5_UART1_RX);					//RX
	csi_gpio_pull_mode(GPIOA, PA5,GPIO_PULLUP);					//RX管脚上拉使能, 建议配置
#endif	
	
	//UART 初始化
	tUartConfig.eParity = UART_PARITY_ODD;						//校验位，奇校验
	tUartConfig.wBaudRate = 115200;								//波特率，115200
	csi_uart_init(UART1, &tUartConfig);							//初始化串口
	
	csi_uart_set_txdma(UART1, UART_DMA_TXFIFO_NFULL);			//设置TX DMA工作模式并使能
	csi_uart_start(UART1, UART_FUNC_RX_TX);						//开启UART的RX和TX功能，也可单独开启RX或者TX功能
	
	//DMA配置初始化
	tDmaConfig.eSrcLinc 	= DMA_ADDR_CONSTANT;				//低位传输原地址固定不变
	tDmaConfig.eSrcHinc 	= DMA_ADDR_INC;						//高位传输原地址自增
	tDmaConfig.eDetLinc 	= DMA_ADDR_CONSTANT;				//低位传输目标地址固定不变
	tDmaConfig.eDetHinc 	= DMA_ADDR_CONSTANT;				//高位传输目标地址固定不变
	tDmaConfig.eDataWidth 	= DMA_DSIZE_8_BITS;					//传输数据宽度8bit
	tDmaConfig.eReload 		= DMA_RELOAD_DISABLE;				//禁止自动重载
	tDmaConfig.eTransMode 	= DMA_TRANS_CONT;					//DMA服务模式(传输模式)，连续服务
	tDmaConfig.eTsizeMode   = DMA_TSIZE_ONE_DSIZE;				//传输数据大小，一个 DSIZE , 即DSIZE定义大小
	tDmaConfig.eReqMode		= DMA_REQ_HARDWARE;					//DMA请求模式，硬件请求
	csi_dma_ch_init(DMA0, DMA_CH1, &tDmaConfig);				//初始化DMA0，选择CH1
	
	csi_dma_int_enable(DMA0, DMA_CH0,DMA_INTSRC_TCIT);			//使用TCIT中断
	
	//ETCB 配置初始化
	tEtbConfig.eChType 	= ETCB_ONE_TRG_ONE_DMA;					//单个源触发单个目标，DMA方式
	tEtbConfig.eSrcIp 	= ETCB_UART1_TXSRC;						//UART TXSRC作为触发源
	tEtbConfig.eDstIp 	= ETCB_DMA0_CH1;						//ETCB DMA通道 作为目标实际
	tEtbConfig.eTrgMode = ETCB_HARDWARE_TRG;					//通道触发模式采样硬件触发
	iRet = csi_etcb_ch_init(ETCB_CH20, &tEtbConfig);			//初始化ETCB，DMA ETCB CHANNEL > ETCB_CH19
	if(iRet < CSI_OK)
		return CSI_ERROR;
	
	while(1)
	{
		csi_uart_send_dma(UART1, DMA0, DMA_CH1, (void *)s_bySendBuf, 26);	
		mdelay(50);
		
		if(csi_dma_get_msg(DMA0, DMA_CH1, ENABLE))				//获取发送完成消息，并清除消息
		{
			//添加用户代码
			nop;
		}
	}
	
	return iRet;
}
/** \brief	uart_receive_dma_demo: UART通过DMA接收数据demo
 *  \brief	UART DMA接收数据，需使用ETCB模块，配置对应触发源和触发目标
 * 
 *  \param[in] none
 *  \return error code
 */
int uart_receive_dma_demo(void)
{
	int iRet = 0;
	csi_uart_config_t tUartConfig;								//UART1 参数配置结构体
	csi_dma_ch_config_t tDmaConfig;								//DMA 通道配置结构体			
	csi_etcb_config_t 	tEtbConfig;								//ETCB 配置结构体
	
#if (USE_GUI == 0)		
	csi_gpio_set_mux(GPIOA,PA4, PA4_UART1_TX);					//TX	
	csi_gpio_set_mux(GPIOA, PA5, PA5_UART1_RX);					//RX
	csi_gpio_pull_mode(GPIOA, PA5,GPIO_PULLUP);					//RX管脚上拉使能, 建议配置
#endif	
	tUartConfig.eParity = UART_PARITY_ODD;						//校验位，奇校验
	tUartConfig.wBaudRate = 115200;								//波特率，115200
	csi_uart_init(UART1, &tUartConfig);							//初始化串口
	
	csi_uart_set_rxdma(UART1, UART_DMA_RXFIFO_NSPACE);			//设置RX DMA工作模式并使能	
	csi_uart_start(UART1, UART_FUNC_RX_TX);						//开启UART的RX和TX功能，也可单独开启RX或者TX功能

	//DMA配置初始化
	tDmaConfig.eSrcLinc 	= DMA_ADDR_CONSTANT;				//低位传输原地址固定不变
	tDmaConfig.eSrcHinc 	= DMA_ADDR_CONSTANT;				//高位传输原地址固定不变
	tDmaConfig.eDetLinc 	= DMA_ADDR_CONSTANT;				//低位传输目标地址固定不变
	tDmaConfig.eDetHinc 	= DMA_ADDR_INC;						//高位传输目标地址自增
	tDmaConfig.eDataWidth 	= DMA_DSIZE_8_BITS;					//传输数据宽度8bit
	tDmaConfig.eReload 		= DMA_RELOAD_ENABLE;				//禁止自动重载
	tDmaConfig.eTransMode 	= DMA_TRANS_CONT;				//DMA服务模式(传输模式)，连续服务
	tDmaConfig.eTsizeMode  = DMA_TSIZE_ONE_DSIZE;				//传输数据大小，一个 DSIZE , 即DSIZE定义大小
	tDmaConfig.eReqMode		= DMA_REQ_HARDWARE;					//DMA请求模式，硬件请求
	csi_dma_ch_init(DMA0, DMA_CH0, &tDmaConfig);				//初始化DMA,选择CH0
	
	csi_dma_int_enable(DMA0, DMA_CH0,DMA_INTSRC_TCIT);			//使用TCIT中断
	
	//ETCB 配置初始化
	tEtbConfig.eChType 	= ETCB_ONE_TRG_ONE_DMA;					//单个源触发单个目标，DMA方式
	tEtbConfig.eSrcIp 	= ETCB_UART1_RXSRC;						//UART RXSRC作为触发源
	tEtbConfig.eDstIp 	= ETCB_DMA0_CH0;						//ETCB DMA通道 作为目标实际
	tEtbConfig.eTrgMode = ETCB_HARDWARE_TRG;					//通道触发模式采样硬件触发
	iRet = csi_etcb_ch_init(ETCB_CH21, &tEtbConfig);			//初始化ETCB，DMA ETCB CHANNEL > ETCB_CH19
	if(iRet < CSI_OK)
		return CSI_ERROR;
	
	csi_uart_recv_dma(UART1, DMA0, DMA_CH0, (void*)s_byRecvBuf,22);
	
	while(1)
	{
		if(csi_dma_get_msg(DMA0,DMA_CH0, ENABLE))				//获取接收完成消息，并清除消息
		{
			//添加用户代码
			csi_uart_send(UART1, (void*)s_byRecvBuf, 22);
			nop;
		}							
		mdelay(10);
		nop;
	}
	
	return iRet;
}

