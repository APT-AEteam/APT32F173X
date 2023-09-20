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

#include "sys_clk.h"
#include "drv/can.h"
#include "board_config.h"
#include "sys_console.h"

/* Private macro------------------------------------------------------*/
/* externs function---------------------------------------------------*/
extern int32_t console_init(sys_console_t *handle);

/* externs variablesr-------------------------------------------------*/
/* variablesr---------------------------------------------------------*/
sys_console_t g_tConsole;


/* system clock configuration parameters to define source, source freq(if selectable), sdiv and pdiv
 * 
 * such as: g_tClkConfig.eSdiv = SCLK_DIV1, g_tClkConfig.wSclk = g_tClkConfig.wFreq / 1
 *          g_tClkConfig.ePdiv = SCLK_DIV4, g_tClkConfig.wPclk = g_tClkConfig.wSclk / 4 
 * 
 */
csi_clk_config_t g_tClkConfig = 
//	{SRC_HFOSC, HFOSC_24M_VALUE, SCLK_DIV1, PCLK_DIV1, HFOSC_24M_VALUE, HFOSC_24M_VALUE};
	//{SRC_HFOSC, HFOSC_12M_VALUE, SCLK_DIV1, PCLK_DIV1,HFOSC_12M_VALUE, HFOSC_12M_VALUE};
	//{SRC_HFOSC, HFOSC_6M_VALUE, SCLK_DIV1, PCLK_DIV1,HFOSC_6M_VALUE, HFOSC_6M_VALUE};
	//{SRC_HFOSC, HFOSC_3M_VALUE, SCLK_DIV1, PCLK_DIV1,HFOSC_3M_VALUE, HFOSC_3M_VALUE};
//	{SRC_AUTO_HF_PLL, PLL_VALUE, SCLK_DIV1, PCLK_DIV1, PLL_VALUE, PLL_VALUE};
//	{SRC_AUTO_EM_PLL, PLL_VALUE, SCLK_DIV1, PCLK_DIV1, PLL_VALUE, PLL_VALUE};
//	{SRC_MANUAL_HF_PLL, PLL_VALUE, SCLK_DIV1, PCLK_DIV1, PLL_VALUE, PLL_VALUE};
	//{SRC_MANUAL_EM_PLL, PLL_VALUE, SCLK_DIV1, PCLK_DIV1, PLL_VALUE, PLL_VALUE};
	//{SRC_EMOSC, EMOSC_VALUE, SCLK_DIV2, PCLK_DIV2, EMOSC_VALUE/2, EMOSC_VALUE/4};
	{SRC_IMOSC, IMOSC_5M_VALUE, SCLK_DIV1, PCLK_DIV1,IMOSC_5M_VALUE, IMOSC_5M_VALUE};
	//{SRC_IMOSC, IMOSC_4M_VALUE, SCLK_DIV1, PCLK_DIV1,IMOSC_4M_VALUE, IMOSC_4M_VALUE};
	//{SRC_IMOSC, IMOSC_2M_VALUE, SCLK_DIV1, PCLK_DIV1,IMOSC_2M_VALUE, IMOSC_2M_VALUE};
	//{SRC_IMOSC, IMOSC_131K_VALUE, SCLK_DIV1, PCLK_DIV1,IMOSC_131K_VALUE, IMOSC_131K_VALUE};
	//{SRC_ESOSC, ESOSC_VALUE, SCLK_DIV2, PCLK_DIV1,ESOSC_VALUE/2, ESOSC_VALUE/2};

/*  PLL CONFIG
 * 
 * PLLPCLK = 24 /(g_tPllClkConfig.byDivM+1) * g_tPllClkConfig.byNul / (g_tPllClkConfig.byCkp_Div+1)
 * g_tPllClkConfig.byCkq_Div= CKQ_DIV4, PLLQCLK = 24 /(g_tPllClkConfig.byDivM+1) * g_tPllClkConfig.byNul / 4  
 * 
 */  
csi_pll_config_t g_tPllClkConfig = 
//	{PLL_SEL_HFOSC_24M, 3, 35, 1,CKQ_DIV4};       //PLLPCLK = 24 /(3+1) *35 / (1+1)  PLLQCLK = 24 /(3+1) *35 / 4 
//	{PLL_SEL_HFOSC_12M, 1, 35, 1,CKQ_DIV4};
	{PLL_SEL_HFOSC_6M,  1, 70, 1,CKQ_DIV4};
	//{PLL_SEL_EMOSC_24M, 3, 35, 1,CKQ_DIV4};	
	
/// can bit timer parameters config, Standard baud rate
const csi_can_bittime_t  g_tBitTime[] = {
	
	
	//CAN clk source = pclk = 6MHz
	//Baudrate			SyncJump	PhaseSeg1	PhaseSeg2	BaudDiv
//	{CAN_BDR_200K, 		1, 			4,			3,			2		},			//Tq num = 10, T_bit = 5us	
//	{CAN_BDR_250K, 		1, 			5,			4,			1		},			//Tq num = 12, T_bit = 4us
//	{CAN_BDR_500K, 		1, 			5,			4,			0		},			//Tq num = 12, T_bit = 2us
//	{CAN_BDR_1000K, 	1, 			2,			1,			0		},			//Tq num = 6,  T_bit = 2us
//	{0, 				0, 			0,			0,			0		},	
	
	//CAN clk source = pclk = 24MHz
	//Baudrate			SyncJump	PhaseSeg1	PhaseSeg2	BaudDiv
//	{CAN_BDR_200K, 		1, 			5,			5,			9		},			//Tq num = 12, T_bit = 5us	
//	{CAN_BDR_250K, 		1, 			5,			4,			7		},			//Tq num = 12, T_bit = 4us
//	{CAN_BDR_500K, 		1, 			3,			2,			5		},			//Tq num = 8,  T_bit = 2us
//	{CAN_BDR_1000K, 	1, 			3,			2,			2		},			//Tq num = 8,  T_bit = 1us
//	{0, 				0, 			0,			0,			0		},	
	
		//CAN clk source = pclk = 48MHz
	//Baudrate			SyncJump	PhaseSeg1	PhaseSeg2	BaudDiv
//	{CAN_BDR_200K, 		1, 			7,			5,			15		},			//Tq num = 15, T_bit = 5us	
//	{CAN_BDR_250K, 		1, 			5,			4,			15		},			//Tq num = 12, T_bit = 4us
//	{CAN_BDR_500K, 		1, 			3,			2,			11		},			//Tq num = 8,  T_bit = 2us
//	{CAN_BDR_1000K, 	1, 			3,			2,			5		},			//Tq num = 8, T_bit = 1us
//	{0, 				0, 			0,			0,			0		},	
	
	//CAN clk source = pclk = 96MHz
	//Baudrate			SyncJump	PhaseSeg1	PhaseSeg2	BaudDiv
//	{CAN_BDR_200K, 		1, 			7,			5,			31		},			//Tq num = 15, T_bit = 5us	
//	{CAN_BDR_250K, 		1, 			5,			4,			31		},			//Tq num = 12, T_bit = 4us
//	{CAN_BDR_500K, 		1, 			3,			2,			23		},			//Tq num = 8,  T_bit = 2us
//	{CAN_BDR_1000K, 	1, 			3,			2,			11		},			//Tq num = 8, T_bit = 1us
//	{0, 				0, 			0,			0,			0		},	
	
	//CAN clk source = pclk = 105MHz
	//Baudrate			SyncJump	PhaseSeg1	PhaseSeg2	BaudDiv
	{CAN_BDR_200K, 		1, 			7,			5,			34		},			//Tq num = 15, T_bit = 5us	
	{CAN_BDR_250K, 		1, 			5,			4,			34		},			//Tq num = 12, T_bit = 4us
	{CAN_BDR_500K, 		1, 			6,			5,			14		},			//Tq num = 14,  T_bit = 2us
	{CAN_BDR_1000K, 	1, 			3,			1,			14		},			//Tq num = 8, T_bit = 1us
	{0, 				0, 			0,			0,			0		},	
};

/** \brief board initialize config; 
 * 
 *  \param[in] none
 *  \return none
 */ 
__attribute__((weak)) void board_init(void)		
{
    //g_tConsole config for print
	g_tConsole.uart_id 	= (uint32_t)CONSOLE_IDX;
    g_tConsole.baudrate = 115200U;
    g_tConsole.tx.pin	= CONSOLE_TXD;   
    g_tConsole.tx.func 	= CONSOLE_TXD_FUNC;
    g_tConsole.rx.pin 	= CONSOLE_RXD;
    g_tConsole.rx.func 	= CONSOLE_RXD_FUNC;
	g_tConsole.gpiox 	= CONSOLE_PORT;
	g_tConsole.uart 	= (csp_uart_t *)(APB_UART0_BASE + CONSOLE_IDX * 0x1000);
    console_init(&g_tConsole);
}

