/***********************************************************************//** 
 * \file  gpio_test.c
 * \brief  GPIO_TEST description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-4-25 <td>V0.0 <td>ZJY     <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include <iostring.h>
#include <csi_drv.h>

/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

/** \brief gpio 复用功能测试，测试写入的值是否正确,可遍历所有IO和复用功能 
 * 
 *  \param[in] none
 *  \return error code
 */
int gpio_mux_test(void)
{
	int iRet = 0;
	uint8_t i, j, k =0;
	uint8_t byPinName = PA0, byPinNum = 16;
	
	for(k = 0; k < 4; k++)							//PORTx =(A/B/C/D)				
	{
		switch(k)
		{
			case 0:
				byPinName = PA0;
				byPinNum = PA0 +16;
				break;
			case 1:
				byPinName = PB0;
				byPinNum = PB0 +16;
				break;
			case 2:
				byPinName = PC0;
				byPinNum = PC0 +16;
				break;
			case 3:
				byPinName = PD0;
				byPinNum = PD0 + 6;
				break;
			default:
				break;
		}
		
		for(i = 4; i < 14; i++)						//PIN 复用功能
		{
			for(j = byPinName; j < byPinNum; j++)	//ALL PIN of PORTx =(A/B/C/D)
			{
				if((j != 13) && (j != 14))
					csi_pin_set_mux(j ,i);
				
			}
				
			mdelay(10);
		}
		
		mdelay(10);
	}
	
	return iRet;
}

/** \brief gpio output 测试，测所有IO输出功能，包括输出模式，驱动能力，速度；验证寄存器配置
 * 
 *  \param[in] none
 *  \return error code
 */
int gpio_ouput_test(void)
{
	int iRet = 0;
	uint8_t i;
	uint32_t wStatus = 0, wPinMask = 0xffff;		
	csp_gpio_t *pGpioPort = GPIOA;
	
	for(i = 0; i < 4; i++)
	{
		switch(i)
		{
			case 0:
				pGpioPort = GPIOA;
				wPinMask = 0xffff;
				break;
			case 1:
				pGpioPort = GPIOB;
				wPinMask = 0xffff;
				break;
			case 2:
				pGpioPort = GPIOC;
				wPinMask = 0xffff;
				break;
			case 3:
				pGpioPort = GPIOD;
				wPinMask = 0x3f;
				break;
			default:
				break;
		}
		
		//正常输出模式(推挽)
		csi_gpio_port_dir(pGpioPort, wPinMask, GPIO_DIR_OUTPUT);		//端口配置为输出
		csi_gpio_port_drive(pGpioPort, wPinMask, GPIO_DRIVE_LV0);		//弱驱
		csi_gpio_port_set_high(pGpioPort, wPinMask);					//输出高
		wStatus = csp_gpio_read_output_port(pGpioPort);					//输出状态寄存器
		while(wStatus != wPinMask);
		
		mdelay(100);
		csi_gpio_port_speed(pGpioPort, wPinMask, GPIO_SPEED_LV0);		//慢速
		csi_gpio_port_set_low(pGpioPort, wPinMask);						//输出低
		wStatus = csp_gpio_read_output_port(pGpioPort);					//输出状态寄存器
		while(wStatus != 0);
		
		mdelay(100);
		csi_gpio_port_speed(pGpioPort, wPinMask, GPIO_SPEED_LV1);		//快速
		csi_gpio_port_set_high(pGpioPort, wPinMask);					//输出高
		mdelay(100);
		
		//开漏输出
		csi_gpio_port_output_mode(pGpioPort, wPinMask, GPIO_OPEN_DRAIN);//端口配置为开漏输出
		csi_gpio_port_set_high(pGpioPort, wPinMask);					//输出高
		mdelay(100);
		csi_gpio_port_set_low(pGpioPort, wPinMask);						//输出低
		mdelay(100);
		
		//推挽输出
		csi_gpio_port_output_mode(pGpioPort, wPinMask, GPIO_PUSH_PULL);	//端口配置为推挽输出
		csi_gpio_port_drive(pGpioPort, wPinMask, GPIO_DRIVE_LV1);		//强驱
		pGpioPort->SODR = wPinMask;
		mdelay(10);
		wStatus = csp_gpio_read_output_port(pGpioPort);					//输出状态寄存器
		while(wStatus != wPinMask);
		
		mdelay(100);
		//csi_gpio_port_set_low(pGpioPort, wPinMask);					//输出低
		pGpioPort->CODR = wPinMask;
		mdelay(10);
		wStatus = csp_gpio_read_output_port(pGpioPort);					//输出状态寄存器
		while(wStatus != 0);
		
		
		mdelay(100);
		csi_gpio_port_set_high(pGpioPort, wPinMask);					//输出高
		mdelay(100);
		
	}
	
	return iRet;
}

/** \brief gpio pinput测试，测所有IO输入功能，包括上拉、COMS/TTL1/TTL2模式，验证寄存器配置
 *  \param[in] none
 *  \return error code
 */
int gpio_input_test(void)
{
	int iRet = 0;
	uint8_t i;
	uint32_t wStatus = 0, wPinMask = 0xffff;		
	csp_gpio_t *pGpioPort = GPIOA;
	
	for(i = 0; i < 4; i++)
	{
		switch(i)
		{
			case 0:
				pGpioPort = GPIOA;
				wPinMask = 0xffff;
				break;
			case 1:
				pGpioPort = GPIOB;
				wPinMask = 0xffff;
				break;
			case 2:
				pGpioPort = GPIOC;
				wPinMask = 0xffff;
				break;
			case 3:
				pGpioPort = GPIOD;
				wPinMask = 0x3f;
				break;
			default:
				break;
		}
		
		csi_gpio_port_dir(pGpioPort, wPinMask, GPIO_DIR_INPUT);				//GPIOA0 端口配置为输入
		csi_gpio_port_pull_mode(pGpioPort, wPinMask, GPIO_PULLNONE);		//无上下拉
		csi_gpio_port_input_mode(pGpioPort, wPinMask, GPIO_INPUT_CMOS);		//input COMS 模式
		mdelay(100);
		
		csi_gpio_port_pull_mode(pGpioPort, wPinMask, GPIO_PULLUP);			//上拉
		csi_gpio_port_input_mode(pGpioPort, wPinMask, GPIO_INPUT_TTL1);		//input TTL1 模式
		mdelay(100);
		wStatus = csi_gpio_port_read(pGpioPort,wPinMask);
		while(wStatus != wPinMask);
		
		csi_gpio_port_pull_mode(pGpioPort, wPinMask, GPIO_PULLDOWN);		//下拉
		mdelay(100);
		wStatus = csi_gpio_port_read(pGpioPort,wPinMask);
		if(pGpioPort == GPIOA)
			wStatus = wStatus & 0x9fff;										//PA13/PA14 = SWDIO/SWCLK
		while(wStatus != 0);
		
		csi_gpio_port_input_mode(pGpioPort, wPinMask, GPIO_INPUT_TTL2);		//input TTL2 模式
		csi_gpio_port_pull_mode(pGpioPort, wPinMask, GPIO_PULLUP);			//上拉
		
		mdelay(100);
	}
	
	
	return iRet;
}

/** \brief gpio interrupt 测试，测所有IO中断
 * 
 *  \param[in] pGpioPort: Port of gpio, GPIOA/GPIOB/GPIOC/GPIOD
 *  \param[in] eTrgEdge: Rising/Falling/both
 *  \return error code
 */
int gpio_interrupt_test(csp_gpio_t *pGpioPort, csi_gpio_irq_mode_e eTrgEdge)
{
	int iRet = 0;
	uint32_t wPinMask = 0xffff;
	csi_gpio_pull_mode_e ePullMode = GPIO_PULLUP;
	
	if(pGpioPort == GPIOA)
		wPinMask = 0x9fff;											//PA13/PA14 = SWDIO/SWCLK
	else if(pGpioPort == GPIOD)
		wPinMask = 0x3f;
	else
		wPinMask = 0xffff;
		
	
	if(eTrgEdge == GPIO_IRQ_RISING_EDGE)							//边沿
		ePullMode = GPIO_PULLDOWN;
	else
		ePullMode = GPIO_PULLUP;
	
	csi_gpio_port_dir(pGpioPort, wPinMask, GPIO_DIR_INPUT);			//端口配置为输入
	csi_gpio_port_pull_mode(pGpioPort, wPinMask, ePullMode);		//上下拉配置
	csi_gpio_port_irq_mode(pGpioPort,wPinMask,eTrgEdge);			//边沿配置
	csi_gpio_port_irq_enable(pGpioPort,wPinMask,ENABLE);			//使能端口对应外部中断
	csi_gpio_port_vic_irq_enable(wPinMask, ENABLE);		
	
	return iRet;
}

/** \brief gpio extend interrupt 测试，测扩展中断(GRP16~19)
 * 
 *  \param[in] pGpioPort: Port of gpio, GPIOA/GPIOB/GPIOC/GPIOD
 *  \param[in] eTrgEdge: Rising/Falling/both
 *  \return error code
 */
int gpio_ex_interrupt_test(pin_name_e ePinName)
{
	int iRet = 0;
	csi_exi_grp_e eExiGrp = 0xff;
	
	if(ePinName > PC15)
		eExiGrp = EXI_GRP19;			
	else if(ePinName > PB15)
		eExiGrp = EXI_GRP18;				
	else if(ePinName > PA15)
		eExiGrp = EXI_GRP17;				
	else
		eExiGrp = EXI_GRP16;				
	
	csi_pin_set_mux(ePinName, PA5_INPUT);						//PIN 配置为输入
	csi_pin_pull_mode(ePinName, GPIO_PULLUP);					//PIN 上拉
	csi_pin_irq_enable(ePinName, ENABLE);						//PIN 中断使能	
	csi_pin_irq_mode(ePinName, eExiGrp, GPIO_IRQ_FALLING_EDGE);	//PIN 下降沿产生中断
	
	csi_pin_vic_irq_enable(eExiGrp, ENABLE);					//使能VIC中断
		
	
	return iRet;
}

/** \brief gpio interrupt 滤波测试，GRP0~3 和GRP16~19
 * 
 *  \param[in] pGpioPort: Port of gpio, GPIOA/GPIOB/GPIOC/GPIOD
 *  \param[in] eTrgEdge: Rising/Falling/both
 *  \return error code
 */
int gpio_exi_flt_test(pin_name_e ePinName, csi_exi_grp_e eExiGrp, csi_exi_flttm_e eFltTime)
{
	int iRet = 0;
	
	if((eExiGrp > EXI_GRP3) && (eExiGrp < EXI_GRP15))
		return -1;
	
	csi_exi_set_flt(eExiGrp, eFltTime);							//EXI 滤波
	
	csi_pin_set_mux(ePinName, PA5_INPUT);						//PIN 配置为输入
	csi_pin_pull_mode(ePinName, GPIO_PULLUP);					//PIN 上拉
	csi_pin_irq_enable(ePinName, ENABLE);						//PIN 中断使能	
	csi_pin_irq_mode(ePinName, eExiGrp, GPIO_IRQ_FALLING_EDGE);	//PIN 下降沿产生中断
	csi_pin_vic_irq_enable(eExiGrp, ENABLE);					//使能VIC中断
	
	return iRet;
}


/** \brief gpio 管脚remap配置
 * 		 - 将相应的IO配置为group0/group1的功能
 *  \param[in] none
 *  \return error code
 */
int gpio_ioremap_test(void)
{
	int iRet = 0;

	
	//IOMAP GROUP0
	csi_pin_set_iomap(PB10, IOMAP0_I2C_SCL);
	csi_pin_set_iomap(PB11, IOMAP0_I2C_SDA);
	csi_pin_set_iomap(PB12, IOMAP0_I2C_SCL);
	csi_pin_set_iomap(PA15, IOMAP0_USART0_TX);
	csi_pin_set_iomap(PB5, IOMAP0_SPI_NSS);
	csi_pin_set_iomap(PD3, IOMAP0_USART0_TX);
	csi_pin_set_iomap(PB8, IOMAP0_SPI_MISO);
	csi_pin_set_iomap(PB9, IOMAP0_SPI_NSS);
	
	//IOMAP GROUP1
	csi_pin_set_iomap(PD4, IOMAP1_USART0_SCK);
	csi_pin_set_iomap(PD0, IOMAP1_USART0_TX);
	csi_pin_set_iomap(PA4, IOMAP1_USART0_SCK);
	csi_pin_set_iomap(PA5, IOMAP1_UART2_TX);
	csi_pin_set_iomap(PA6, IOMAP1_USART0_TX);
	csi_pin_set_iomap(PA7, IOMAP1_CMP0_OUT);
	csi_pin_set_iomap(PB0, IOMAP1_GPTA1_CHA);
	csi_pin_set_iomap(PB1, IOMAP1_CMP0_OUT);
	
	return iRet;
}

/** \brief gpio interrupt handle function
 * 
 *  \param[in] byExiNum: exi interrupt vector num, 0~4
 *  \return none
 */ 
void gpio_irqhandler(uint8_t byExiNum)
{
	volatile uint32_t wExiSta; 
	wExiSta = csp_exi_port_get_isr(SYSCON);
	
    switch(byExiNum)
	{
		case 0:
			switch(wExiSta)								//EXI0
			{
				case STATUS_EXI0:						//int group0
					break;
				case STATUS_EXI16:						//int group16
					break;
			}
			break;
		case 1:
			switch(wExiSta)								//EXI1							
			{
				case STATUS_EXI1:						//int group1
					break;
				case STATUS_EXI17:						//int group17
					break;
			}
			break;
		case 2:
			switch(wExiSta)								//EXI2-3
			{
				case STATUS_EXI2:						//int group2
					break;
				case STATUS_EXI3:						//int group3
					break;
				case STATUS_EXI18:						//int group18
					break;
				case STATUS_EXI19:						//int group19
					break;
			}
			break;
		case 3:
			switch(wExiSta)								//EXI4-9
			{
				case STATUS_EXI4:						//int group4
					break;
				case STATUS_EXI5:						//int group5
					break;
				case STATUS_EXI6:						//int group6
					break;
				case STATUS_EXI7:						//int group7
					break;
				case STATUS_EXI8:						//int group8
					break;
				case STATUS_EXI9:						//int group9
					break;
			}
			break;
		case 4:
			switch(wExiSta)								//EXI10-15
			{
				case STATUS_EXI10:						//int grou10
					break;
				case STATUS_EXI11:						//int group11
					break;
				case STATUS_EXI12:						//int group12
					break;
				case STATUS_EXI13:						//int group13
					break;
				case STATUS_EXI14:						//int group14
					break;
				case STATUS_EXI15:						//int group15
					break;
			}
			break;
		default:
			break;
		
	}
	csp_exi_port_clr_isr(SYSCON, wExiSta);		//clear interrput 
}