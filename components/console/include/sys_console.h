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

//CONSOLE的端口选择，可选择UART[0~2]/USART[0~1]
#define CONSOLE_UART			0
#define CONSOLE_USART   		1

typedef struct {
    pin_name_e ePin;
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

int32_t console_init(sys_console_t *handle);

#ifdef __cplusplus
}
#endif

#endif /*_SYS_CONSOLE_H_*/

