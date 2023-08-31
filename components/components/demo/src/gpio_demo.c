/***********************************************************************//** 
 * \file  gpio_demo.c
 * \brief  GPIO_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-4-25 <td>V0.0 <td>ZJY     <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include "drv/gpio.h"

/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
#define		PINMASK_PA00		(0x01ul << 0)	
#define		PINMASK_PA01		(0x01ul << 1)
#define		PINMASK_PA02		(0x01ul << 2)	
#define		PINMASK_PA05		(0x01ul << 5)
#define		PINMASK_PA09		(0x01ul << 9)	
#define		PINMASK_PA010		(0x01ul << 10)
#define		PINMASK_PA013		(0x01ul << 13)

#define		PINMASK_PB01		(0x01ul << 1)
#define		PINMASK_PB02		(0x01ul << 2)
#define		PINMASK_PB05		(0x01ul << 5)
/* Private variablesr------------------------------------------------------*/

/** \brief gpio port output demo 
 * 
 *  \param[in] none
 *  \return error code
 */
int gpio_ouput_demo(void)
{
	int iRet = 0;

	csi_gpio_set_mux(GPIOA, PA2, PA2_OUTPUT);			//PA2输出
	csi_gpio_set_high(GPIOA, PA2);						//输出高
	mdelay(100);
	csi_gpio_set_low(GPIOA, PA2);						//输出低
	mdelay(100);
	csi_gpio_set_high(GPIOA, PA2);						//输出高
	mdelay(100);
	
	csi_gpio_output_mode(GPIOA, PA2, GPIO_OPEN_DRAIN);	//GPIOA 端口配置为开漏输出
	csi_gpio_set_high(GPIOA, PA2);						//输出高
	mdelay(100);
	csi_gpio_set_low(GPIOA, PA2);						//输出低
	mdelay(100);
	csi_gpio_set_high(GPIOA, PA2);						//输出高
	mdelay(100);
	
	csi_gpio_output_mode(GPIOA, PA2, GPIO_PUSH_PULL);	//GPIOA 端口配置为推挽输出
	csi_gpio_set_high(GPIOA, PA2);						//输出高
	mdelay(100);
	csi_gpio_set_low(GPIOA, PA2);						//输出低
	mdelay(100);
	csi_gpio_set_high(GPIOA, PA2);						//输出高
	mdelay(100);
	
	
	return iRet;
}

/** \brief gpio port output demo 
 * 
 *  \param[in] none
 *  \return error code
 */
int gpio_input_demo(void)
{
	int iRet = 0;
	
	csi_gpio_set_mux(GPIOA, PA0, PA0_INPUT);			//PA0配置为输入
	csi_gpio_pull_mode(GPIOA, PA0, GPIO_PULLNONE);		//无上下拉
	mdelay(100);
	
	csi_gpio_pull_mode(GPIOA, PA0, GPIO_PULLUP);		//上拉
	mdelay(100);

	csi_gpio_pull_mode(GPIOA, PA0, GPIO_PULLDOWN);		//下拉
	mdelay(100);

	return iRet;
}

 /** \brief gpio port interrupt
 * 
 *  \param[in] none
 *  \return error code
 */
 
int gpio_irq_demo(void)
{
	int iRet = 0;
	
	csi_gpio_set_mux(GPIOA, PA0, PA0_INPUT);						//PA0配置为输入
	csi_gpio_pull_mode(GPIOA, PA0, GPIO_PULLUP);					//上拉
	csi_gpio_irq_enable(GPIOA,PA0);									//使能PA0端口对应外部中断
	csi_gpio_irq_mode(GPIOA, PA0, EXI_GRP0,GPIO_IRQ_FALLING_EDGE);	//下降沿
	csi_gpio_vic_irq_enable(EXI_GRP0, ENABLE);						//GPIOA端口对应VIC中断使能
	
	return iRet;
}

/** \brief  exi0_int_handler: EXI0 中断服务函数,支持EXI GROUP0和GROUP16
 * 
 *  \brief 	支持EXI的中断GROUP0和GROUP16，两者中任一产生中断，会调用该函数；函数在interrupt.c里定义为弱(weak)
 * 			属性，默认处理仅清除状态，使用EXI中断时，请用户重定义此函数，完成对应的中断处理
 * 			
 * 
 *  \param[in] byExiNum: exi interrupt vector num, 0~4
 *  \return none
 */ 
ATTRIBUTE_ISR  void exi0_int_handler(void) 
{
	volatile uint32_t wExiSta = csp_exi_get_isr(SYSCON); 		//get interrupt status
	
	if(wExiSta & STATUS_EXI0)				//exi group0
	{
		//用户添加处理
		nop;
	}
	
	if(STATUS_EXI0 & STATUS_EXI16)			//exigroup16			
	{
		//用户添加处理
		nop;
	}
	csp_exi_clr_isr(SYSCON, wExiSta);		//clear interrput 
}
