/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     sys_console.h
 * @brief    header file for acmp driver
 * @version  V1.0
 * @date     2019-12-19
 ******************************************************************************/

#ifndef _SYS_CONSOLE_H_
#define _SYS_CONSOLE_H_

#include "drv/gpio.h"
#include "drv/uart.h"
#include "drv/usart.h"

#ifdef __cplusplus
extern "C" {
#endif

//CONSOLE IP(UART/USART), select UART[0~2]/USART[0~1]
#define CONSOLE_UART			0				//select uart1
#define CONSOLE_USART   		1				//select usart1

//
#define console_putc(CTRL, UARTx, VAL)	(CTRL > 0) ?  csi_usart_putc(UARTx, VAL) : csi_uart_putc(UARTx, VAL)
#define console_getc(CTRL, UARTx)		(CTRL > 0) ?  csi_usart_getc(UARTx) : csi_usart_getc(UARTx)

typedef struct {
    pin_mask_e ePin;
    pin_func_e eFunc;
} console_port_t;

typedef struct {
	csp_gpio_t 		*ptGpiox;
    console_port_t 	tTx;
    console_port_t 	tRx;
    uint32_t 		baudrate;
    void 			*pUartx;
	uint8_t 		byIpType;
} sys_console_t;

extern sys_console_t g_tConsole;

int32_t console_init(sys_console_t *tHandle);

#ifdef __cplusplus
}
#endif

#endif /*_SYS_CONSOLE_H_*/

