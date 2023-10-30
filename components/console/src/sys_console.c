/***********************************************************************//** 
 * \file  uart.c
 * \brief  csi uart driver
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author	<th>Description
 * <tr><td> 2020-8-12 <td>V0.0 <td>ZJY   	<td>initial
 * <tr><td> 2023-9-21 <td>V0.1 <td>ZJY   	<td>code normalization 
 * </table>
 * *********************************************************************
*/
#include "sys_console.h"

/* externs function---------------------------------------------------*/
/* Private macro------------------------------------------------------*/

/* Private variablesr-------------------------------------------------*/


int32_t console_init(sys_console_t *tHandle)
{
    int32_t ret = 0;
	
	csi_gpio_set_mux(tHandle->ptGpiox, tHandle->tTx.ePin, tHandle->tTx.eFunc);
	csi_gpio_set_mux(tHandle->ptGpiox, tHandle->tRx.ePin, tHandle->tRx.eFunc);
	csi_gpio_pull_mode(tHandle->ptGpiox, tHandle->tRx.ePin, GPIO_PULLUP);
	
	if(tHandle->byIpType == CONSOLE_UART)									//UART
	{
		csi_uart_config_t 		tUartConfig;
		
		tUartConfig.eParity 	= UART_PARITY_NONE;							//no parity
		tUartConfig.wBaudRate 	= tHandle->baudrate;						//115200
		ret = csi_uart_init((csp_uart_t *)tHandle->pUartx, &tUartConfig);
		if(ret < 0)
			return -1;
			
		csi_uart_start((csp_uart_t *)tHandle->pUartx, UART_FUNC_RX_TX);
	}
	else if(tHandle->byIpType == CONSOLE_USART)								//USART
	{
		csi_usart_config_t 		tUsartCfg;									//USART0 参数配置结构体
		
		tUsartCfg.eClkSrc 		= USART_CLKSRC_DIV1;						//clk = PCLK
		tUsartCfg.eMode			= USART_MODE_ASYNC;							//异步模式
		tUsartCfg.eDatabit 		= USART_DATA_BITS_8;						//字节长度，8bit
		tUsartCfg.eStopbit 		= USART_STOP_BITS_1;						//停止位，1个
		tUsartCfg.eParity		= USART_PARITY_NONE;						//无校验
		tUsartCfg.wBaudRate 	= 115200;									//波特率：115200
		ret = csi_usart_init((csp_usart_t *)tHandle->pUartx, &tUsartCfg);	//初始化串口	
		if(ret < 0)
			return -1;
			
		csi_usart_start((csp_usart_t *)tHandle->pUartx, USART_FUNC_RX_TX);	//开启USART的RX和TX功能，也可单独开启RX或者TX功能
	}
	else 
		return -1;
	
    return ret;
}

int fputc(int ch, FILE *stream)
{
    (void)stream;
	
	if (ch == '\n') 
		console_putc(g_tConsole.byIpType, g_tConsole.pUartx, (uint8_t)'\r');

	console_putc(g_tConsole.byIpType, g_tConsole.pUartx, (uint8_t)ch);

    return 0;
}

int fgetc(FILE *stream)
{
    uint8_t ch;
    (void)stream;

	ch = console_getc(g_tConsole.byIpType, g_tConsole.pUartx);
	
    return (int)ch;
}
