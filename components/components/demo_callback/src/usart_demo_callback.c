/***********************************************************************//** 
 * \file  usart_demo_callback.c
 * \brief  USART_CALLBACK_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-9-22 <td>V0.0 <td>GQQ      <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/

#include "csi_drv.h"
#include "board_config.h"



#if (USE_USART_CALLBACK == 1)	

/* externs function--------------------------------------------------------*/
/* externs variable------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variable------------------------------------------------------*/

uint8_t byRecvBuf[128]={0};			//receive buf
uint8_t bySendBuf[30]={1,2,3,4,5,6,7,8,9,21,22,23,24,25,26,27,28,29,30,10,11,12,13,14,15,16,17,18,19};



/** \brief  send_callback：串口中断发送回调函数
 * 
 * 	\brief	用户定义，使用csi标准库进行中断发送时，发送完毕后回自动调用用户注册的回调函数；用户可在回调
 * 			函数里做自己的处理，而不需要关注具体的底层中断处理。
 * 		
 *  \param[out] ptUartBase: USARTx寄存器结构体指针，指向USARTx的基地址 
 *  \return none
 */
static void	user_send_callback(csp_usart_t *ptUsartBase)
{
	//添加用户处理
	nop;
}


/**	\brief  usart_send_int_callback_demo：USART中断发送demo，使用callback
 * 
	\brief  USART数据中断方式发送，使用标准csi库函数，库函数中断支持回调函数，用户可注册发送中断回调函数发
 * 			送完毕后，中断函数会主动调用回调函数
 * 
 *  \param[in] none
 *  \return 0/-1
 */
int usart_send_int_callback_demo(void)
{
	int iRet = 0;
	uint8_t bySendBuf[30]={1,2,3,4,5,6,7,8,9,21,22,23,24,25,26,27,28,29,30,10,11,12,13,14,15,16,17,18,19};
	csi_usart_config_t tUsartConfig= {0};					//USART0 参数配置结构体
	
#if (USE_GUI == 0)		
	csi_gpio_set_mux(GPIOA, PA9, PA9_USART0_TX);				//USART0 TX管脚配置	
	csi_gpio_set_mux(GPIOA, PA10, PA10_USART0_RX);				//USART0 RX管脚配置
	csi_gpio_pull_mode(GPIOA, PA10, GPIO_PULLUP);				//RX管脚上拉使能, 建议配置
#endif
	
	tUsartConfig.eClkSrc 		= USART_CLKSRC_DIV1;		//clk = PCLK
	tUsartConfig.eMode			= USART_MODE_ASYNC;			//异步模式
	tUsartConfig.eDatabit 		= USART_DATA_BITS_8;		//字节长度，8bit
	tUsartConfig.eStopbit 		= USART_STOP_BITS_1;		//停止位，1个
	tUsartConfig.eParity		= USART_PARITY_EVEN;		//偶校验
	tUsartConfig.wBaudRate 		= 115200;					//波特率：115200
	csi_usart_init(USART0, &tUsartConfig);					//初始化串口
	
	csi_usart_register_callback(USART0, USART_CALLBACK_ID_SEND, user_send_callback);		//注册USART0发送中断回调函数
	csi_usart_start(USART0, USART_FUNC_RX_TX);		//开启USART的RX和TX功能，也可单独开启RX或者TX功能
	
	//使用中断方式发送，若要知道是否发送完成可使用回调函数
	while(1)
	{
		csi_usart_send_int(USART0,(void *)bySendBuf,28);		//采用中断方式
		mdelay(200);
	}
	
	return iRet;
}


/** \brief  user_receive_callback：USART接收中断回调函数
 * 
 * 	\brief 	用户定义,使用csi标准库进行中断接收时,接收支持指定长度和动态长度(一串字符)两种方式，两者中任
 * 			一方式完成都会调用回调函数；用户可在回调函数里做自己的处理，而不需要关注具体的底层中断处理。
 * 
 * @ USART_EVENT_RX_DNE：定长接收完成事件，用户通过接口函数csi_uart_receive_int指定接收长度和数据接收buf，
 * 						 数据会依次接收到buf中直到接收数据达到指定长度，接收结束自动调用回调函数，用户可在
 * 						 对应的回调函数中做处理。
 * 
 * @ USART_EVENT_RX_TO:	动态接收完成事件，用户通过接口函数csi_usart_receive_int将接收长度和数据接收buf传入，
 * 						接收长度要足够大，需超过实际使用中最长的协议长度；通过节超时作为区分不同字符串的依
 * 						据，字节超时时间初始化时可配置。
 * 
 * @ 注意：				实际使用中，两种接收方式请选择一种，若选择了指定长度，回调函数USART_EVENT_RX_TO事件
 * 						请不要做处理(接收清零)，否则会导致指定长度接收数据异常。
 * 
 *  \param[out] ptSioBase: 	USARTx寄存器结构体指针，指向USARTx的基地址 
 *  \param[out] eEvent: 	接收回调函数事件，支持定长和超时两种方式
 *  \param[out] pbyBuf: 	接收buf，指向接收数据缓存数组首地址
 *  \param[out] hwLen: 		接收长度，
 *  \return none
 */ 
static void user_receive_callback(csp_usart_t *ptUsartBase, csi_usart_event_e eEvent, uint8_t *pbyBuf, uint16_t *hwLen)
{
	volatile uint16_t hwRecvLen = *hwLen;								//获取接收长度
	
	switch(eEvent) 
	{
		case USART_EVENT_RX_DNE:											//指定长度接收
			//添加用户处理
//			csi_usart_send(ptUsartBase,(void *)pbyBuf, hwRecvLen);		//USART发送采用轮询方式
//			csi_usart_receive_int(USART0, pbyBuf, 16);					//重新开启接收
			break;
		case USART_EVENT_RX_TO:											//动态长度接收
			//添加用户处理
			*hwLen = 0;													//清除接收长度，准备下次接收；此模式必须执行清零操作
			csi_usart_send(ptUsartBase,(void *)pbyBuf, hwRecvLen);		//USART发送采用轮询方式
			break;
		default:
			break;
	}
}
/** \brief  usart_recv_int_callback_demo：USART中断接收demo，使用callback
 * 
    \brief  串口接收，使用中断方式接收数据，支持指定长度和动态长度(一串字符)两种方式，中断接收完毕后
 * 			均调用回调函数，csi库中断接收默认支持csi_uart_recv_callback回调函数；具体详细说明请参阅
 * 			回调函数处的介绍
 * 
 * @接收超时: 即接收字节超时，指接收数据时，两个字节之间的时间，超过设定值，产生超时中断；可用来判断
 * 			一串字符接收是否完成，也可用来配合接收FIFO中断触发点大于1时的接收，保证数据接收完整
 * 
 * @接收FIFO触发点: 接收FIFO中断触发点，支持1/2/4这三种模式，即FIFO收到1个字节还是多个字节才会触发RXFIFO中
 * 			断；初始化时选择4，即接收FIFO中收到>=4数据时才会触发中断，这样的优点是提高效率(节约中断资源)，
 * 			缺点是,可能导致接收不全，需要开启接收超时中断来完成未能触发RXFIFO中断数据的接收
 * 
 *  \param[in] none
 *  \return error code
 */
int usart_receive_int_callback_demo(void)
{
	int iRet = 0;
	csi_usart_config_t tUsartConfig= {0};							//USART0 参数配置结构体
	
#if (USE_GUI == 0)		
	csi_gpio_set_mux(GPIOA, PA9, PA9_USART0_TX);				//USART0 TX管脚配置	
	csi_gpio_set_mux(GPIOA, PA10, PA10_USART0_RX);				//USART0 RX管脚配置
	csi_gpio_pull_mode(GPIOA, PA10, GPIO_PULLUP);				//RX管脚上拉使能, 建议配置
#endif	

	tUsartConfig.eClkSrc 	= USART_CLKSRC_DIV1;				//clk = PCLK
	tUsartConfig.eMode		= USART_MODE_ASYNC;					//异步模式
	tUsartConfig.eDatabit 	= USART_DATA_BITS_8;				//字节长度，8bit
	tUsartConfig.eStopbit 	= USART_STOP_BITS_1;				//停止位，1个
	tUsartConfig.eParity	= USART_PARITY_EVEN;				//偶校验
	tUsartConfig.wBaudRate 	= 115200;							//波特率：115200
	tUsartConfig.hwRecvTo 	= 88;								//USART接收超时时间，单位：bit位周期，8个bytes(11bit*8=88, 115200波特率时=764us)，若参数初始化为0，则关闭超时功能
	tUsartConfig.eRxFifoTrg = USART_RXFIFOTRG_FOUR;				//USART的RXFIFO的中断触发点设置为4，即RXFIFO接收到数据>=4,才会触发RXFIFO中断；支持三种配置(1/2/4)，此参数若不被初始化，则配置为1	
	csi_usart_init(USART0, &tUsartConfig);						//初始化串口
	
	csi_usart_register_callback(USART0, USART_CALLBACK_ID_RECV, user_receive_callback);		//注册USART0接收中断回调函数
	csi_usart_start(USART0, USART_FUNC_RX_TX);					//开启USART的RX和TX功能，也可单独开启RX或者TX功能
	
	//指定长度接收
	//接收到16个字节数据时，会自动调用usart0_recv_callback，用户可在USART_EVENT_RX_DNE的事件中做处理
//	csi_usart_receive_int(USART0, byRecvBuf, 16);		//开启接收，指定接收数据长度和数据接收buf,并开启接收FIFO和接收超时中断
	
	
	//动态长度接收
	//接收到一串字符时，会自动调用usart0_recv_callback，用户可在USART_EVENT_RX_TO的事件中做处理
	csi_usart_receive_int(USART0, byRecvBuf, 128);	//开启接收，指定数据接收buf和最大接收数据长度，保证接收长度足够大；开启RXFIFO和接收超时中断
	
	while(1)
	{
		mdelay(10);
	}
	
	return iRet;
}


#endif