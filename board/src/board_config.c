/***********************************************************************//** 
 * \file  baord_config.c
 * \brief  board configuration 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-11-23 <td>V0.0  <td>WNN   <td>initial
 * </table>
 * *********************************************************************
*/

#include <sys_clk.h>
#include <can.h>
#include "board_config.h"
#include "sys_console.h"

/* Private macro------------------------------------------------------*/
/* externs function---------------------------------------------------*/
extern int32_t console_init(sys_console_t *handle);

/* externs variablesr-------------------------------------------------*/
/* variablesr---------------------------------------------------------*/
sys_console_t console;

/// system clock configuration parameters to define source, source freq(if selectable), sdiv and pdiv
csi_clk_config_t tClkConfig = 
	//{SRC_HFOSC, HFOSC_48M_VALUE, SCLK_DIV2, PCLK_DIV1, 5556000, 5556000};
	{SRC_PLL, PLL_105M_VALUE, SCLK_DIV2, PCLK_DIV1, 5556000, 5556000};
	//{SRC_EMOSC, EMOSC_VALUE, SCLK_DIV2, PCLK_DIV2, 5556000, 5556000};
	//{SRC_IMOSC, IMOSC_5M_VALUE, SCLK_DIV1, PCLK_DIV1,5556000, 5556000};
	//{SRC_HFOSC, HFOSC_48M_VALUE, SCLK_DIV2, PCLK_DIV1,5556000, 5556000};
	//{SRC_IMOSC, IMOSC_4M_VALUE, SCLK_DIV2, PCLK_DIV1,5556000, 5556000};
	//{SRC_ESOSC, ESOSC_VALUE, SCLK_DIV8, PCLK_DIV1,5556000, 5556000};

/// can bit timer parameters config, Standard baud rate
const csi_can_bittime_t  tBitTime[] = {
	
	
	//CAN clk source = pclk = 6MHz
	//Baudrate			SyncJump	PhaseSeg1	PhaseSeg2	BaudDiv
//	{CAN_BDR_200K, 		1, 			3,			4,			2		},			//Tq num = 10, T_bit = 5us	
//	{CAN_BDR_250K, 		1, 			4,			5,			1		},			//Tq num = 12, T_bit = 4us
//	{CAN_BDR_500K, 		2, 			4,			5,			0		},			//Tq num = 12, T_bit = 2us
//	{0, 				0, 			0,			0,			0		},	
	
	//CAN clk source = pclk = 24MHz
	//Baudrate			SyncJump	PhaseSeg1	PhaseSeg2	BaudDiv
	{CAN_BDR_200K, 		1, 			4,			5,			9		},			//Tq num = 12, T_bit = 5us	
	{CAN_BDR_250K, 		1, 			4,			5,			7		},			//Tq num = 12, T_bit = 4us
	{CAN_BDR_500K, 		2, 			2,			3,			5		},			//Tq num = 8,  T_bit = 2us
	{0, 				0, 			0,			0,			0		},	
};

/** \brief board initialize config; 
 * 
 *  \param[in] none
 *  \return none
 */ 
__attribute__((weak)) void board_init(void)		
{
    //console config for print
	console.uart_id = (uint32_t)CONSOLE_IDX;
    console.baudrate = 115200U;
    console.tx.pin = CONSOLE_TXD;   
    console.tx.func = CONSOLE_TXD_FUNC;
 
    console.rx.pin = CONSOLE_RXD;
    console.rx.func = CONSOLE_RXD_FUNC;
	console.uart = (csp_uart_t *)(APB_UART0_BASE + CONSOLE_IDX * 0x1000);
    console_init(&console);

}

