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


/* system clock configuration parameters to define source, source freq(if selectable), sdiv and pdiv
 * 
 * such as: tClkConfig.eSdiv = SCLK_DIV1, tClkConfig.wSclk = tClkConfig.wFreq / 1
 *          tClkConfig.ePdiv = SCLK_DIV4, tClkConfig.wPclk = tClkConfig.wSclk / 4 
 * 
 */
csi_clk_config_t tClkConfig = 
	//{SRC_HFOSC, HFOSC_24M_VALUE, SCLK_DIV1, PCLK_DIV1, HFOSC_24M_VALUE, HFOSC_24M_VALUE};
	//{SRC_HFOSC, HFOSC_12M_VALUE, SCLK_DIV1, PCLK_DIV1,HFOSC_12M_VALUE, HFOSC_12M_VALUE};
	//{SRC_HFOSC, HFOSC_6M_VALUE, SCLK_DIV1, PCLK_DIV1,HFOSC_6M_VALUE, HFOSC_6M_VALUE};
	//{SRC_HFOSC, HFOSC_3M_VALUE, SCLK_DIV1, PCLK_DIV1,HFOSC_3M_VALUE, HFOSC_3M_VALUE};
	{SRC_AUTO_HF_PLL, PLL_VALUE, SCLK_DIV2, PCLK_DIV1, PLL_VALUE/2, PLL_VALUE/2};
	//{SRC_AUTO_EM_PLL, PLL_VALUE, SCLK_DIV2, PCLK_DIV1, PLL_VALUE/2, PLL_VALUE/2};
	//{SRC_MANUAL_PLL, PLL_VALUE, SCLK_DIV2, PCLK_DIV1, PLL_VALUE/2, PLL_VALUE/2};
	//{SRC_EMOSC, EMOSC_VALUE, SCLK_DIV2, PCLK_DIV2, EMOSC_VALUE/2, EMOSC_VALUE/4};
	//{SRC_IMOSC, IMOSC_5M_VALUE, SCLK_DIV1, PCLK_DIV1,IMOSC_5M_VALUE, IMOSC_5M_VALUE};
	//{SRC_IMOSC, IMOSC_4M_VALUE, SCLK_DIV1, PCLK_DIV1,IMOSC_4M_VALUE, IMOSC_4M_VALUE};
	//{SRC_IMOSC, IMOSC_2M_VALUE, SCLK_DIV1, PCLK_DIV1,IMOSC_2M_VALUE, IMOSC_2M_VALUE};
	//{SRC_IMOSC, IMOSC_131K_VALUE, SCLK_DIV1, PCLK_DIV1,IMOSC_131K_VALUE, IMOSC_131K_VALUE};
	//{SRC_ESOSC, ESOSC_VALUE, SCLK_DIV2, PCLK_DIV1,ESOSC_VALUE/2, ESOSC_VALUE/2};

// PLL MANUAL CONFIG
//PLLPCLK = 24 /(tPllClkConfig.byDivM+1) * tPllClkConfig.byNul / (tPllClkConfig.byCkp_Div+1)
//PLLQCLK = 24 /(tPllClkConfig.byDivM+1) * tPllClkConfig.byNul / (tPllClkConfig.byCkq_Div)
csi_pll_manual_config_t tPllClkConfig = 
	{PLL_SEL_HFOSC_24M, 3, 35, 1,CKQ_DIV4};
	//{PLL_SEL_HFOSC_12M, 1, 35, 1,CKQ_DIV4};
	//{PLL_SEL_HFOSC_6M,  1, 70, 1,CKQ_DIV4};
	//{PLL_SEL_EMOSC_24M, 3, 35, 1,CKQ_DIV4};	
	
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

