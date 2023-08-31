/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */


/******************************************************************************
 * @file     sys_console.c
 * @brief    CSI Source File for console
 * @version  V1.0
 * @date     2019-12-19
 ******************************************************************************/
#include <drv/pin.h>
#include "sys_console.h"

extern void csi_uart_putc(csp_uart_t *ptUartBase, uint8_t byData);

int32_t console_init(sys_console_t *handle)
{
    int32_t ret = 0;
	
	csi_uart_config_t tUartConfig;
	
    csi_pin_set_mux(handle->tx.pin, handle->tx.func);
    csi_pin_set_mux(handle->rx.pin, handle->rx.func);
	csi_pin_pull_mode(handle->rx.pin, GPIO_PULLUP);
	
	tUartConfig.eParity = UART_PARITY_NONE;			//no parity
	tUartConfig.wBaudRate = handle->baudrate;		//115200
	
    ret = csi_uart_init(handle->uart, &tUartConfig);
	
	if(ret < 0)
		return -1;
	
	csi_uart_start(handle->uart, UART_FUNC_RX_TX);
	
    return ret;
}

int fputc(int ch, FILE *stream)
{
    (void)stream;

    if (ch == '\n') {
        csi_uart_putc(g_tConsole.uart, (uint8_t)'\r');
    }

    csi_uart_putc(g_tConsole.uart, (uint8_t)ch);

    return 0;
}

int fgetc(FILE *stream)
{
    uint8_t ch;
    (void)stream;

    ch = csi_uart_getc(g_tConsole.uart);

    return (int)ch;
}
