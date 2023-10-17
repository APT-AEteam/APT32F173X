/***********************************************************************//** 
 * \file  baord_config.c
 * \brief  board configuration 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
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
//	{SRC_HFOSC, HFOSC_12M_VALUE, SCLK_DIV1, PCLK_DIV1,HFOSC_12M_VALUE, HFOSC_12M_VALUE};
//	{SRC_HFOSC, HFOSC_6M_VALUE, SCLK_DIV1, PCLK_DIV1,HFOSC_6M_VALUE, HFOSC_6M_VALUE};
//	{SRC_HFOSC, HFOSC_3M_VALUE, SCLK_DIV1, PCLK_DIV1,HFOSC_3M_VALUE, HFOSC_3M_VALUE};
	{SRC_HF_PLL, PLLP_VALUE, SCLK_DIV1, PCLK_DIV1, PLLP_VALUE, PLLP_VALUE};
//	{SRC_EM_PLL, PLLP_VALUE, SCLK_DIV1, PCLK_DIV1, PLLP_VALUE, PLLP_VALUE};
//	{SRC_EMOSC, EMOSC_VALUE, SCLK_DIV2, PCLK_DIV2, EMOSC_VALUE/2, EMOSC_VALUE/4};
//	{SRC_IMOSC, IMOSC_5M_VALUE, SCLK_DIV1, PCLK_DIV1,IMOSC_5M_VALUE, IMOSC_5M_VALUE};
//	{SRC_IMOSC, IMOSC_4M_VALUE, SCLK_DIV1, PCLK_DIV1,IMOSC_4M_VALUE, IMOSC_4M_VALUE};
//	{SRC_IMOSC, IMOSC_2M_VALUE, SCLK_DIV1, PCLK_DIV1,IMOSC_2M_VALUE, IMOSC_2M_VALUE};
//	{SRC_IMOSC, IMOSC_131K_VALUE, SCLK_DIV1, PCLK_DIV1,IMOSC_131K_VALUE, IMOSC_131K_VALUE};
//	{SRC_ESOSC, ESOSC_VALUE, SCLK_DIV2, PCLK_DIV1,ESOSC_VALUE/2, ESOSC_VALUE/2};


//PLL CONFIG LIST
// PLLPCLK = Input_Freq /(g_tPllClkConfig.byDivM+1) * g_tPllClkConfig.byNul / (g_tPllClkConfig.byCkp_Div+1)
// PLLQCLK = Input_Freq /(g_tPllClkConfig.byDivM+1) * g_tPllClkConfig.byNul / g_tPllClkConfig.byCkq_Div 
//
// -------------------------------------------------------------------------------
// | INPUT Freq | byDivM | byNul | byCkp_Div | byCkq_Div | PLLp Freq | PLLq Freq |
// -------------------------------------------------------------------------------
// |   24Mhz    |    3   |   35  |     1     |     4     |  105Mhz   |  52.5Mhz  |
// |   12Mhz    |    1   |   35  |     1     |     4     |  105Mhz   |  52.5Mhz  |
// |   6Mhz     |    0   |   35  |     1     |     4     |  105Mhz   |  52.5Mhz  | 
// |-----------------------------------------------------------------------------|
// |   24Mhz    |    2   |   24  |     1     |     4     |   96Mhz   |   48Mhz   |
// |   24Mhz    |    2   |   36  |     2     |     6     |   96Mhz   |   48Mhz   |
// |   24Mhz    |    3   |   32  |     1     |     4     |   96Mhz   |   48Mhz   |
// |   24Mhz    |    3   |   48  |     2     |     6     |   96Mhz   |   48Mhz   |
// |   24Mhz    |    4   |   40  |     1     |     4     |   96Mhz   |   48Mhz   |
// |   24Mhz    |    4   |   60  |     2     |     6     |   96Mhz   |   48Mhz   |
// |   24Mhz    |    5   |   48  |     1     |     4     |   96Mhz   |   48Mhz   |
// |   24Mhz    |    5   |   72  |     2     |     6     |   96Mhz   |   48Mhz   |
// |-----------------------------------------------------------------------------|
// |   24Mhz    |    2   |   24  |     1     |     2     |   96Mhz   |   96Mhz   |
// |   24Mhz    |    3   |   32  |     1     |     2     |   96Mhz   |   96Mhz   |
// |   24Mhz    |    4   |   40  |     1     |     2     |   96Mhz   |   96Mhz   |
// |   24Mhz    |    5   |   48  |     1     |     2     |   96Mhz   |   96Mhz   |
// |-----------------------------------------------------------------------------|
// |   24Mhz    |    2   |   36  |     3     |     6     |   72Mhz   |   48Mhz   |
// |   24Mhz    |    3   |   48  |     3     |     6     |   72Mhz   |   48Mhz   |
// |   24Mhz    |    4   |   60  |     3     |     6     |   72Mhz   |   48Mhz   |
// |   24Mhz    |    5   |   72  |     3     |     6     |   72Mhz   |   48Mhz   |
// -------------------------------------------------------------------------------
//NOTE: 1.PLLq CAN be used as ADC source freq,MAX ADC source freq should be 96M.
//		2.1731 PLLp max Freq 72Mhz, 1732 PLLp max Freq 105Mhz

csi_pll_config_t g_tPllClkConfig = 
#if(IS_CHIP_1732 ==1) 
//	{PLL_SEL_HFOSC_24M, 3, 35, 1,CKQ_DIV4};       //PLLP = 105M  PLLQ=52.2M
	{PLL_SEL_HFOSC_12M, 1, 35, 1,CKQ_DIV4};
//	{PLL_SEL_HFOSC_6M,  0, 35, 1,CKQ_DIV4};
//	{PLL_SEL_EMOSC_24M, 3, 35, 1,CKQ_DIV4};	
//	{PLL_SEL_HFOSC_24M, 2, 24, 1,CKQ_DIV4};		  //PLLP = 96M  PLLQ=48M
//	{PLL_SEL_HFOSC_24M, 2, 36, 2,CKQ_DIV6};
//	{PLL_SEL_HFOSC_24M, 3, 32, 1,CKQ_DIV4};		  
//	{PLL_SEL_HFOSC_24M, 3, 48, 2,CKQ_DIV6};
//	{PLL_SEL_HFOSC_24M, 4, 40, 1,CKQ_DIV4};		  
//	{PLL_SEL_HFOSC_24M, 4, 60, 2,CKQ_DIV6};
//	{PLL_SEL_HFOSC_24M, 5, 48, 1,CKQ_DIV4};		  
//	{PLL_SEL_HFOSC_24M, 5, 72, 2,CKQ_DIV6};
//	{PLL_SEL_EMOSC_24M, 3, 32, 1,CKQ_DIV4};	
//	{PLL_SEL_HFOSC_24M, 2, 24, 1,CKQ_DIV2};		  //PLLP = 96M  PLLQ=96M
//	{PLL_SEL_HFOSC_24M, 3, 32, 1,CKQ_DIV2};
//	{PLL_SEL_HFOSC_24M, 4, 40, 1,CKQ_DIV2};
//	{PLL_SEL_HFOSC_24M, 5, 48, 1,CKQ_DIV2};
//	{PLL_SEL_EMOSC_24M, 3, 32, 1,CKQ_DIV2};
#endif
//	{PLL_SEL_HFOSC_24M, 2, 36, 3,CKQ_DIV6};		 //PLLP = 72M  PLLQ=96M
//	{PLL_SEL_HFOSC_24M, 3, 48, 3,CKQ_DIV6};
//	{PLL_SEL_HFOSC_24M, 4, 60, 3,CKQ_DIV6};
//	{PLL_SEL_HFOSC_24M, 5, 72, 3,CKQ_DIV6};
//	{PLL_SEL_EMOSC_24M, 2, 36, 3,CKQ_DIV6};	
	
	
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

