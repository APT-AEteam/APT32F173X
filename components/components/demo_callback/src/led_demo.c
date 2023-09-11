/***********************************************************************//** 
 * \file  led_demo.c
 * \brief  LED  callback功能示例代码

 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date       <th>Version    <th>Author  <th>Description
 * <tr><td> 2023-9-4   <td>V0.0       <td>WCH     <td>initial
 * </table>
 * *********************************************************************
*/

/* include ----------------------------------------------------------------*/
#include "drv/led.h"
#include "drv/gpio.h"
#include "board_config.h"

/* externs function--------------------------------------------------------*/
/* private function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/
/* device instance --------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/ 
uint8_t g_byLedData[4] = {0x06,0x5b,0x4f,0x66};//数码管编码：1,2,3,4 

/** \brief  user_led_callback：led中断回调函数
 * 
 * 	\brief	用户定义，支持ICPEND/IPEND两种中断处理，使用csi标准库，中断发生时会自动调用用户注册的回调函
 * 			数，用户可在回调函数里做自己的处理，而不需要关注具体的底层中断处理。
 * 
 *  \param[out] ptLedBase: 	LEDx寄存器结构体指针，指向LEDx的基地址 
 *  \param[out] byIsr: 		LEDx中断状态
 *  \return none
 */ 
static void user_led_callback(csp_led_t *ptLedBase, uint8_t byIsr)
{
	csp_led_clr_isr(LED, csp_led_get_isr(LED));
}

/** \brief  apt_io_config: LED相关IO配置，包括SEG脚与COM脚的配置
 * 
 * 注意：
 * 如果要使用SWD口作COM功能使用(PA13/PA14)，前面一定要加delay函数，否则复位后将很难连上芯片，见如下使用示例：
 * 
 * mdelay(3000);
 * csi_swd_unlock();
 * 
 * \param[in] none
 * \return none
 */
void led_io_config(void)
{
#if (USE_GUI == 0)
	//SEG0~SEG7
	csi_gpio_set_mux(GPIOD, PD0, PD0_LED_S0);
	csi_gpio_set_mux(GPIOD, PD1, PD1_LED_S1);
	csi_gpio_set_mux(GPIOA, PA0, PA0_LED_S2);
	csi_gpio_set_mux(GPIOA, PA1, PA1_LED_S3);
	csi_gpio_set_mux(GPIOA, PA2, PA2_LED_S4);
	csi_gpio_set_mux(GPIOA, PA3, PA3_LED_S5);
	csi_gpio_set_mux(GPIOA, PA5, PA5_LED_S6);
	csi_gpio_set_mux(GPIOA, PA6, PA6_LED_S7);
	
	//COM0~COM3
	csi_gpio_set_mux(GPIOB, PB3, PB3_LED_C0);
	csi_gpio_set_mux(GPIOB, PB4, PB4_LED_C1);
	csi_gpio_set_mux(GPIOB, PB6, PB6_LED_C2);
	csi_gpio_set_mux(GPIOB, PB7, PB7_LED_C3);

	//开启COM口强驱
	csi_gpio_drive(GPIOB, PB3, GPIO_DRIVE_STRONG);
	csi_gpio_drive(GPIOB, PB4, GPIO_DRIVE_STRONG);
	csi_gpio_drive(GPIOB, PB6, GPIO_DRIVE_STRONG);
	csi_gpio_drive(GPIOB, PB7, GPIO_DRIVE_STRONG);
#endif
}

/**
* \brief led_demo： LED示例代码
* \brief 代码功能说明：
* \brief 1、正常控制：依次点亮4个数码管，依次显示1，2，3，4
* \brief 2、闪烁控制：关闭COM0，显示2，3，4
* \brief 3、闪烁控制：关闭COM0/1，显示3，4
* \brief 4、闪烁控制：关闭COM0/1/2，显示4
* \return  error code
* 四位一体共阴数码管(SM420564W32U3A/AB092716)引脚图：
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
int led_callback_demo(void)
{	
	int iRet = 0;
	uint8_t byDisplayStatus=0;
	
	led_io_config();
	
	csi_led_config_t ptLedCfg;
	ptLedCfg.eClkDiv 	  = LED_PCLK_DIV8;				//LED时钟为系统时钟8分频
	ptLedCfg.hwComMask 	  = 0x0f;						//COM0~3打开
	ptLedCfg.eBrt 		  = LED_100;					//LED显示亮度50%
	ptLedCfg.hwOnTime 	  = 120;						//显示周期时间(单位：Tledclk)
	ptLedCfg.hwBreakTime  = 50;							//Non-Overlap时间(单位：Tledclk)
	csi_led_init(LED, &ptLedCfg);		
	
	csi_led_int_enable(LED, LED_INTSRC_ICEND);			//若需使用中断，请调该接口使能对应中断，这里使用LED_INTSRC_ICEND中断
	csi_led_register_callback(LED, user_led_callback);	//注册中断回调函数
	
	csi_led_light_on(LED);								//开启LED自动扫描
	
	while(1)
	{
		switch(byDisplayStatus)
		{
			//display status 0:正常控制，依次显示1，2，3，4
			case 0:
				for(uint8_t i = 0; i < 4; i++)
				{
					csi_led_write_data(LED, i, g_byLedData[i]);
					mdelay(5);
				}
				byDisplayStatus++;
				mdelay(1000);
			break;
			
			//display status 1:闪烁控制，关闭COM0，依次显示2，3，4
			case 1:
				csi_led_blink_control(LED, LED_BLK_OFF,(0x01&LED_BLK_MSK));//disable COM0
				for(uint8_t i = 0; i < 4; i++)
				{
					csi_led_write_data(LED, i, g_byLedData[i]);
					mdelay(5);
				}
				byDisplayStatus++;
				mdelay(1000);
			break;
				
			//display status 2:闪烁控制，关闭COM0/1，依次显示3，4
			case 2:
				csi_led_blink_control(LED, LED_BLK_OFF,(0x02&LED_BLK_MSK));//disable COM1
				for(uint8_t i = 0; i < 4; i++)
				{
					csi_led_write_data(LED, i, g_byLedData[i]);
					mdelay(5);
				}
				byDisplayStatus++;
				mdelay(1000);
			break;
				
			//display status 3:闪烁控制，关闭COM0/1/2，显示4
			case 3:
				csi_led_blink_control(LED, LED_BLK_OFF,(0x04&LED_BLK_MSK));//disable COM2
				for(uint8_t i = 0; i < 4; i++)
				{
					csi_led_write_data(LED, i, g_byLedData[i]);
					mdelay(5);
				}
				byDisplayStatus = 0;
				mdelay(1000);
				csi_led_blink_control(LED, LED_BLK_ON,(0x07&LED_BLK_MSK));//enable COM0/1/2
			break;
			
			default:
			break;
		}
		//clear display buffer
		for(uint8_t i = 0;i<4;i++)
			csi_led_write_data(LED, i, 0x00);
	}
	return iRet;
}