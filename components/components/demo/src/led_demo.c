/***********************************************************************//** 
 * \file  led_demo.c
 * \brief  LED示例代码

 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version    <th>Author  <th>Description
 * <tr><td> 2021-12-03 <td>V0.0  <td>WNN     <td>initial
 * <tr><td> 2023-3-21  <td>V0.1  <td>WCH     <td>initial
 * </table>
 * *********************************************************************
*/

/* include ----------------------------------------------------------------*/
#include "led.h"
#include "csp.h"
#include "pin.h"

/* externs function--------------------------------------------------------*/
/* private function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/
/* device instance ------------------------------------------------------*/
/* Private variablesr-----------------------------------------------------*/ 
uint8_t g_byLedData[4] = {0x06,0x5b,0x4f,0x66};//display code of 1,2,3,4 

void io_config(void)
{
	//SEG0~SEG7
	csi_pin_set_mux(PD0, PD0_LED_S0);
	csi_pin_set_mux(PD1, PD1_LED_S1);
	csi_pin_set_mux(PA0, PA0_LED_S2);
	csi_pin_set_mux(PA1, PA1_LED_S3);
	csi_pin_set_mux(PA2, PA2_LED_S4);
	csi_pin_set_mux(PA3, PA3_LED_S5);
	csi_pin_set_mux(PA5, PA5_LED_S6);
	csi_pin_set_mux(PA6, PA6_LED_S7);
//	csi_pin_set_mux(PC4, PC4_LED_S6);
//	csi_pin_set_mux(PC5, PC5_LED_S7);
	
	//COM0~COM9
	csi_pin_set_mux(PB3,  PB3_LED_C0);
	csi_pin_set_mux(PB4,  PB4_LED_C1);
	csi_pin_set_mux(PB6,  PB6_LED_C2);
	csi_pin_set_mux(PB7,  PB7_LED_C3);
	csi_pin_set_mux(PB8,  PB8_LED_C4);
	csi_pin_set_mux(PB9,  PB9_LED_C5);
	csi_pin_set_mux(PC10, PC10_LED_C6);
	csi_pin_set_mux(PC11, PC11_LED_C7);
	csi_pin_set_mux(PC12, PC12_LED_C8);
	csi_pin_set_mux(PD2,  PD2_LED_C9);
	
//	mdelay(3000);
//	csi_swd_unlock();//如果要使用SWD口(PA13/PA14)，前面一定要加delay函数，否则复位后将很难连上芯片。
//	csi_pin_set_mux(PA11, PA11_LED_C6);
//	csi_pin_set_mux(PA12, PA12_LED_C7);
//	csi_pin_set_mux(PA13, PA13_LED_C8);
//	csi_pin_set_mux(PA14,  PD2_LED_C9);
	
	//开启COM口强驱
	csi_pin_drive(PB3, GPIO_DRIVE_STRONG);
	csi_pin_drive(PB4, GPIO_DRIVE_STRONG);
	csi_pin_drive(PB6, GPIO_DRIVE_STRONG);
	csi_pin_drive(PB7, GPIO_DRIVE_STRONG);
}

/**
  \brief  LED示例代码
   * \brief  1、正常控制：依次点亮4个数码管，依次显示1，2，3，4
   * \brief  2、闪烁控制：关闭COM0，显示2，3，4
   * \brief  3、闪烁控制：关闭COM0/1，显示3，4
   * \brief  4、闪烁控制：关闭COM0/1/2，显示4
  \return      csi_error_t
 * 四位一体共阴数码管引脚图：
					  COM1  a   f COM2 COM3 b
						|	|	|	|	|	|
						|	|	|	|	|	|									
				|—————————————————|
				|  |——|  |——|  |——|  |——|  |
				|  |	|  |    |  |	|  |	|  |
				|  |——|  |——|  |——|  |——|  |
				|  |	|  |    |  |	|  |	|  |
				|  |——|. |——|. |——|. |——|. |		
				|—————————————————|
						|	|	|	|	|	|
						|	|	|	|	|	|
					    e   d   dp  c   g  COM4
*/
csi_error_t led_demo(void)
{	
	uint8_t byDisplayStatus=0;
	csi_led_config_t ptLedCfg;
	
	io_config();
	
	ptLedCfg.byClk = LED_PCLK_DIV8;
	ptLedCfg.hwComMask = 0x0f;			//COM0~3打开
	ptLedCfg.byBrt = LED_100;			//LED显示亮度50%
	ptLedCfg.byOnTime = 120;			//显示周期时间(单位：Tledclk)
	ptLedCfg.byBreakTime = 50;			//Non-Overlap时间(单位：Tledclk)
	ptLedCfg.byInt = LED_INTSRC_NONE;
	csi_led_init(LED, &ptLedCfg);			

	while(1)
	{
		switch(byDisplayStatus)
		{
			//display status 0:正常控制，依次显示1，2，3，4
			case 0:
				csi_led_lighton(LED);
				for(uint8_t i = 0; i < 4; i++)
				{
					csi_led_write_data(LED, i, g_byLedData[i]);
					mdelay(50);
				}
				csi_led_lightoff(LED);
				byDisplayStatus++;
				mdelay(1);
			break;
			
			//display status 1:闪烁控制，关闭COM0，依次显示2，3，4
			case 1:
				csi_led_lighton(LED);
				csi_led_blink_control(LED, LED_BLK_OFF,(0x01&LED_BLK_MSK));//disable COM0
				for(uint8_t i = 0; i < 4; i++)
				{
					csi_led_write_data(LED, i, g_byLedData[i]);
					mdelay(50);
				}
				csi_led_lightoff(LED);
				byDisplayStatus++;
				mdelay(1);
			break;
				
			//display status 2:闪烁控制，关闭COM0/1，依次显示3，4
			case 2:
				csi_led_lighton(LED);
				csi_led_blink_control(LED, LED_BLK_OFF,(0x02&LED_BLK_MSK));//disable COM1
				for(uint8_t i = 0; i < 4; i++)
				{
					csi_led_write_data(LED, i, g_byLedData[i]);
					mdelay(50);
				}
				csi_led_lightoff(LED);
				byDisplayStatus++;
				mdelay(1);
			break;
				
			//display status 3:闪烁控制，关闭COM0/1/2，显示4
			case 3:
				csi_led_lighton(LED);
				csi_led_blink_control(LED, LED_BLK_OFF,(0x04&LED_BLK_MSK));//disable COM2
				for(uint8_t i = 0; i < 4; i++)
				{
					csi_led_write_data(LED, i, g_byLedData[i]);
					mdelay(50);
				}
				csi_led_lightoff(LED);
				byDisplayStatus = 0;
				mdelay(1);
				csi_led_blink_control(LED, LED_BLK_ON,(0x07&LED_BLK_MSK));//enable COM0/1/2
			break;
			
			default:
			break;
		}
		//clear display buffer
		for(uint8_t i = 0;i<4;i++)
			csi_led_write_data(LED, i, 0x00);
	}
}

/** \brief led interrupt handle function
 * 
 *  \param[in] args: dev of led
 *  \return none
 */ 
__attribute ((weak))void led_irqhandler(csp_led_t *ptLedBase)
{
	csp_led_clr_int(ptLedBase, LED_INT_ICEND|LED_INT_IPEND);
}