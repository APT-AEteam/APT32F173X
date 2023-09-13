/***********************************************************************//** 
 * \file  gpio.c
 * \brief  csi gpio driver
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-13 <td>V0.0 <td>ZJY     <td>initial
 * </table>
 * *********************************************************************
*/
#include "drv/gpio.h"


/** \brief get gpio pin number 
 * 
 *  \param[in] ePinFunc: pointer of gpio register structure
 *  \return gpio port total number(16/6) or error(0xff)
 */ 
static uint8_t apt_get_gpio_num_low(pin_name_e ePinName)
{
	uint16_t byPinNum = 0;
	
	switch((uint8_t)ePinName)
	{
		case 0x01:
			break;
		case 0x02:
		case 0x04:
			byPinNum = ePinName >> 1;
			break;
		case 0x08:
		case 0x10:
			byPinNum = (ePinName >> 3) + 2;
			break;
		case 0x20:
		case 0x40:
			byPinNum = (ePinName >> 5) + 4;
			break;
		case 0x80:
			byPinNum =7;
			break; 
		default:
			byPinNum = 0;
			break;
	}
	
	return byPinNum;
}

/** \brief get gpio pin number 
 * 
 *  \param[in] ePinFunc: pointer of gpio register structure
 *  \return gpio port total number(16/6) or error(0xff)
 */ 
static uint8_t apt_get_gpio_num(pin_name_e ePinName)
{
	uint8_t byPinNum = 0;
	
	if(ePinName > 0x80)
		byPinNum = apt_get_gpio_num_low(ePinName >> 8) + 8;
	else
		byPinNum = apt_get_gpio_num_low((uint8_t)ePinName);
		
	return byPinNum;
}

/** \brief set gpio interrupt group
 * 
 *  \param[in] ptGpioBase: pointer of gpio register structure
 *  \param[in] byPinNum: pin0~15
 *  \param[in] eExiGrp:	EXI_IGRP0 ~ EXI_IGRP19
 *  \return none
 */ 
static int apt_gpio_intgroup_set(csp_gpio_t *ptGpioBase, uint8_t byPinNum, gpio_igrp_e eExiGrp)
{
	uint32_t byMaskShift,byMask;
	gpio_group_e eIoGroup = GRP_GPIOA;
	
	switch((uint32_t)ptGpioBase)
	{
		case APB_GPIOA_BASE:
			eIoGroup = GRP_GPIOA;
			break;
		case APB_GPIOB_BASE:
			eIoGroup = GRP_GPIOB;
			break;
		case APB_GPIOC_BASE:
			eIoGroup = GRP_GPIOC;
			break;
		case APB_GPIOD_BASE:
			eIoGroup = GRP_GPIOD;
			break;
		default:
			return CSI_ERROR;
	}
	
	if(eExiGrp < EXI_IGRP16)
	{
		if(byPinNum < 8)
		{
			byMaskShift = (byPinNum << 2);
			byMask = ~(0x0Ful << byMaskShift);
			GPIOGRP->IGRPL = ((GPIOGRP->IGRPL) & byMask) | (eIoGroup << byMaskShift);
		}
		else if(byPinNum < 16)
		{
			byMaskShift = ((byPinNum-8) << 2);
			byMask = ~(0x0Ful << byMaskShift);
			GPIOGRP->IGRPH = ((GPIOGRP->IGRPH) & byMask) | (eIoGroup << byMaskShift);
		}
		else
			return CSI_ERROR;
	}
	else 
	{
		switch(eExiGrp)
		{
			case EXI_IGRP16:
				if(GRP_GPIOA != eIoGroup)
					return CSI_ERROR;
					
				break;
			case EXI_IGRP17:
				if(GRP_GPIOB != eIoGroup)
					return CSI_ERROR;
					
				break;
			case EXI_IGRP18:
				if(GRP_GPIOC != eIoGroup)
					return CSI_ERROR;
					
				break;
			case EXI_IGRP19:
				if(GRP_GPIOD != eIoGroup)
					return CSI_ERROR;
					
				break;
			default:
				return CSI_ERROR;
		}
		
		byMaskShift = (eExiGrp - EXI_IGRP16) << 2;
		byMask = ~(0x0Ful << byMaskShift);
		GPIOGRP->IGREX = ((GPIOGRP->IGREX) & byMask) | (byPinNum << byMaskShift);
	}
	
	return CSI_OK;
}

/** \brief set gpio exi interrupt trigger 
 * 
 *  \param[in] ptSysconBase: pionter of SYSCON reg structure.
 *  \param[in] eExiGrp: EXI_IGRP0~EXI_IGRP19
 *  \param[in] eGpioTrg: EXI_IRT,EXI_IFT,
 *  \return none
 */ 
static void apt_exi_trg_edge_set(csp_syscon_t *ptSysconBase, gpio_igrp_e eExiGrp, exi_trigger_e eGpioTrg)
{
	uint32_t wPinMsak = (0x01ul << eExiGrp);
	
	ptSysconBase->EXIRT &= (~wPinMsak);					//trig edg
	ptSysconBase->EXIFT &= (~wPinMsak);
	
	switch(eGpioTrg)
	{
		case EXI_EDGE_IRT:
			ptSysconBase->EXIRT |= wPinMsak;
			ptSysconBase->EXIFT &= ~wPinMsak;
			break;
		case EXI_EDGE_IFT:
			ptSysconBase->EXIFT |= wPinMsak;
			ptSysconBase->EXIRT &= ~wPinMsak;
			break;
		case EXI_EDGE_BOTH:
			ptSysconBase->EXIRT |= wPinMsak;
			ptSysconBase->EXIFT |= wPinMsak;
			break;
		default:
			break;
	}
}
/** \brief set gpio iomap function
 * 
 *  \param[in] ePinName: gpio pin name
 *  \param[in] eIoMap: gpio pin remap function
 *  \param[in] byGrp: iomap group0/group1, 0/1
 *  \return none
 */  
static void apt_iomap_handle(pin_name_e ePinName, csi_gpio_iomap_e eIoMap, uint8_t byGrp)
{
	uint8_t i,j;
	volatile uint8_t wFlag = 0x00;
	uint32_t *pwIoMap = NULL;
	
	if(byGrp == 0)
		pwIoMap = (uint32_t *)&SYSCON->IOMAP0;
	else
	{
		pwIoMap = (uint32_t *)&SYSCON->IOMAP1;
		eIoMap = eIoMap - 8;
	}
	
	for(i = 0; i < ePinName; i++)
	{
		if((((*pwIoMap) >> 4*i) & 0x0f) == eIoMap)
		{
			for(j = 0; j < ePinName; j++)
			{
				switch(((*pwIoMap) >> 4*j) & 0x0f) 
				{
					case IOMAP0_I2C_SCL:
						wFlag |= (0x01 << IOMAP0_I2C_SCL);
						break;
					case IOMAP0_I2C_SDA:
						wFlag |= (0x01 << IOMAP0_I2C_SDA);
						break;
					case IOMAP0_USART0_TX:
						wFlag |= (0x01 << IOMAP0_USART0_TX);
						break;
					case IOMAP0_USART0_RX:
						wFlag |= (0x01 << IOMAP0_USART0_RX);
						break;
					case IOMAP0_SPI0_NSS:
						wFlag |= (0x01 << IOMAP0_SPI0_NSS);
						break;
					case IOMAP0_SPI0_SCK:
						wFlag |= (0x01 << IOMAP0_SPI0_SCK);
						break;
					case IOMAP0_SPI0_MISO:
						wFlag |= (0x01 << IOMAP0_SPI0_MISO);
						break;
					case IOMAP0_SPI0_MOSI:
						wFlag |= (0x01 << IOMAP0_SPI0_MOSI);
						break;
				}
			}
			
			for(j = 0; j < 8; j++)
			{
				if(((wFlag & 0x01) == 0) && (j != eIoMap))
				{
					break;
				}
				wFlag = (wFlag >> 1);
			}
			
			*pwIoMap = ((*pwIoMap) & ~(0x0F << 4*i)) | (j << 4*i);			//no select
		}
	}
}
/** \brief set gpio mux function
 * 
 *  \param[in] ptGpioBase: pointer of gpio register structure
 *  \param[in] ePinName: gpio pin name, \ref pin_name_e
 *  \param[in] ePinFunc: gpio pin function, \ref pin_func_e
 *  \return enone
 */  
void csi_gpio_set_mux(csp_gpio_t *ptGpioBase, pin_name_e ePinName, pin_func_e ePinFunc)
{
	uint8_t byPinNum = apt_get_gpio_num(ePinName);

	if(ePinName < 0x100)
		ptGpioBase->CONLR =(ptGpioBase->CONLR & ~(0xF << 4*byPinNum)) | (ePinFunc << 4*byPinNum);
	else
		ptGpioBase->CONHR =(ptGpioBase->CONHR & ~(0xF << 4*(byPinNum - 8))) | (ePinFunc << 4*(byPinNum - 8));
	
}
/** \brief get gpio mux function
 * 
 *  \param[in] ptGpioBase: pointer of gpio register structure
 *  \param[in] ePinName: gpio pin name, \ref pin_name_e
 *  \return value of gpio mux
 */ 
pin_func_e csi_gpio_get_mux(csp_gpio_t *ptGpioBase, pin_name_e ePinName)
{
    uint8_t ret = 0x0f;
	uint8_t byPinNum = apt_get_gpio_num(ePinName);
		
	if(ePinName < 8)
		ret = (((ptGpioBase->CONLR) >> 4 * byPinNum) & 0x0Ful);
	else
		ret = (((ptGpioBase->CONHR) >> 4 * (byPinNum - 8)) & 0x0Ful);
		
    return (pin_func_e)ret;
}
/** \brief set gpio iomap function
 * 
 *  \param[in] ePinName: gpio pin name
 *  \param[in] eIoMap: gpio pin remap function
 *  \return error code \ref csi_error_t
 */  
csi_error_t csi_pin_set_iomap(csp_gpio_t *ptGpioBase, pin_name_e ePinName, csi_gpio_iomap_e eIoMap)
{
	uint8_t byIoMap = 0x0b;
	uint8_t byPinNum = apt_get_gpio_num(ePinName);
	
	if(eIoMap > IOMAP1_CMP0_OUT)
		return CSI_ERROR;
	
	switch((uint32_t)ptGpioBase)
	{
		case APB_GPIOA_BASE:
			if((byPinNum < 7) &&(byPinNum > 3))				//pa3~pa8, iomap1
			{
				apt_iomap_handle((byPinNum-2), eIoMap, 1);
				SYSCON->IOMAP1 = (SYSCON->IOMAP1 & ~(0x0F << 4 * (byPinNum-2))) | ((eIoMap-8) << (4 * (byPinNum-2)));
			}
			else if(byPinNum == 15)							//pa15, iomap0
			{
				apt_iomap_handle((byPinNum-12), eIoMap, 0);
				SYSCON->IOMAP0 = (SYSCON->IOMAP0 & ~(0x0F << 4 * (byPinNum-12))) | (eIoMap << (4 * (byPinNum-12)));
				byIoMap = 0x0a;
			}
			else
				return CSI_ERROR;
				
			break;
		case APB_GPIOB_BASE:
			if((byPinNum == 0) || (byPinNum == 1))			//pb0~pb1, iomap1
			{
				apt_iomap_handle((byPinNum+6), eIoMap, 1);
				SYSCON->IOMAP1 = (SYSCON->IOMAP1 & ~(0x0F << 4 * (byPinNum+6))) | ((eIoMap-8) << (4 * (byPinNum+6)));	
			}
			else if(byPinNum == 5)							//pb5, iomap0
			{
				apt_iomap_handle((byPinNum-1), eIoMap, 0);
				SYSCON->IOMAP0 = (SYSCON->IOMAP0 & ~(0x0F << 4 * (byPinNum-1))) | (eIoMap << (4 * (byPinNum-1)));
				byIoMap = 0x0a;
			}
			else if((byPinNum == 8) || (byPinNum == 9))		//pb8~9, iomap0
			{
				apt_iomap_handle((byPinNum-2), eIoMap, 0);
				SYSCON->IOMAP0 = (SYSCON->IOMAP0 & ~(0x0F << 4 * (byPinNum-2))) | (eIoMap << (4 * (byPinNum-2)));
				byIoMap = 0x0b;
			}
			else if((byPinNum < 13) &&(byPinNum > 9))		//pb10~pb12, iomap0
			{
				apt_iomap_handle((byPinNum-10), eIoMap, 0);
				SYSCON->IOMAP0 = (SYSCON->IOMAP0 & ~(0x0F << 4 * (byPinNum-10))) | (eIoMap << (4 * (byPinNum-10)));
				byIoMap = 0x0a;
			}
			else
				return CSI_ERROR;
		
			break;
		case APB_GPIOD_BASE:
			if(byPinNum == 0)
			{
				apt_iomap_handle((byPinNum+1), eIoMap, 1);	//pd3, iomap1
				SYSCON->IOMAP1 = (SYSCON->IOMAP1 & ~(0x0F << 4 * (byPinNum+1))) | ((eIoMap-8) << (4 * (byPinNum+1)));	
			}
			else if(byPinNum == 3)							//pd3, iomap0
			{
				apt_iomap_handle((byPinNum+2), eIoMap, 0);
				SYSCON->IOMAP0 = (SYSCON->IOMAP0 & ~(0x0F << 4 * (byPinNum+2))) | (eIoMap << (4 * (byPinNum+2)));
				byIoMap = 0x0a;
			}
			else if(byPinNum == 4)							//pd4, iomap1
			{
				apt_iomap_handle((byPinNum-4), eIoMap, 1);
				SYSCON->IOMAP1 = (SYSCON->IOMAP1 & ~(0x0F << 4 * (byPinNum-4))) | ((eIoMap-8) << (4 * (byPinNum-4)));				
			}
			else 
				return CSI_ERROR;
				
			break;
		default:
			return CSI_ERROR;
	}
	
	if(ePinName < 0x100)
		ptGpioBase->CONLR =(ptGpioBase->CONLR & ~(0xF << 4*byPinNum)) | (byIoMap << 4*byPinNum);
	else
		ptGpioBase->CONHR =(ptGpioBase->CONHR & ~(0xF << 4*(byPinNum - 8))) | (byIoMap << 4*(byPinNum - 8));
		
	return CSI_OK;
}
/** \brief set gpio pull mode
 * 
 *  \param[in] ptGpioBase: pointer of gpio register structure
 *  \param[in] ePinName: gpio pin name, \ref pin_name_e
 *  \param[in] ePullMode: gpio pull mode, \ref csi_gpio_pull_mode_e
 *  \return error code \ref csi_error_t
 */  
csi_error_t csi_gpio_pull_mode(csp_gpio_t *ptGpioBase, pin_name_e ePinName, csi_gpio_pull_mode_e ePullMode)
{
	csi_error_t ret = CSI_OK;
	uint16_t byPinNum = 0;
	
	byPinNum = apt_get_gpio_num(ePinName);
		
	switch(ePullMode)
	{
		case GPIO_PULLNONE:
			csp_gpio_pullnone_enable(ptGpioBase, byPinNum);		//pull none
			break;
		case GPIO_PULLUP:
			csp_gpio_pullup_enable(ptGpioBase, byPinNum);			//pull up
			break;
		case GPIO_PULLDOWN:
			csp_gpio_pulldown_enable(ptGpioBase, byPinNum);		//pull down
			break;
		default:
			ret = CSI_ERROR;
			break;
	}
	
    return ret;
}
/** \brief set gpio input mode
 *
 *  \param[in] ptGpioBase: pointer of gpio register structure
 *  \param[in] ePinName: gpio pin name, \ref pin_name_e
 *  \param[in] eInputMode: input mode, \ref csi_gpio_input_mode_e 
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_gpio_input_mode(csp_gpio_t *ptGpioBase, pin_name_e ePinName, csi_gpio_input_mode_e eInputMode)
{

	csi_error_t ret = CSI_OK;
	uint16_t byPinNum = 0;
	
	byPinNum = apt_get_gpio_num(ePinName);
	
	switch (eInputMode)
	{
		case (GPIO_INPUT_TTL2):	
			csp_gpio_ttl_enable(ptGpioBase, byPinNum);
			csp_gpio_set_ttl2(ptGpioBase, byPinNum);
			break;
		case (GPIO_INPUT_TTL1): 
			csp_gpio_ttl_enable(ptGpioBase, byPinNum);
			csp_gpio_set_ttl1(ptGpioBase, byPinNum);
			break;
		case (GPIO_INPUT_CMOS):	csp_gpio_cmos_enable(ptGpioBase, byPinNum);
			break;
		default:
			ret = CSI_ERROR;
			break;
	}
		return ret;
}
/** \brief set gpio output mode
 * 
 *  \param[in] ptGpioBase: pointer of gpio register structure
 *  \param[in] ePinName: gpio pin name, \ref pin_name_e 
 *  \param[in] eOutMode: output mode, \ref csi_gpio_output_mode_e 
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_gpio_output_mode(csp_gpio_t *ptGpioBase, pin_name_e ePinName, csi_gpio_output_mode_e eOutMode)
{
	csi_error_t ret = CSI_OK;

	switch(eOutMode)
	{
		case GPIO_PUSH_PULL:
			csp_gpio_opendrain_disable(ptGpioBase, ePinName);	//push-pull mode
			break;
		case GPIO_OPEN_DRAIN:
			csp_gpio_opendrain_enable(ptGpioBase, ePinName);	//open drain mode 
			break;
		default:
			ret = CSI_ERROR;
			break;
	}
	
	return ret; 
}
/** \brief set gpio speed
 * 
 *  \param[in] ptGpioBase: pointer of gpio register structure
 *  \param[in] ePinName: gpio pin name, \ref pin_name_e
 *  \param[in] eSpeed: gpio pin speed, \ref csi_gpio_speed_e
 *  \return none
 */  
void csi_gpio_speed(csp_gpio_t *ptGpioBase, pin_name_e ePinName, csi_gpio_speed_e eSpeed)
{
	uint8_t byPinNum = 0;
	
	byPinNum = apt_get_gpio_num(ePinName);
	csp_gpio_set_speed(ptGpioBase, byPinNum, (uint8_t)eSpeed);
}

/** \brief set gpio drive level
 * 
 *  \param[in] ptGpioBase: pointer of gpio register structure
 *  \param[in] ePinName: gpio pin name, \ref pin_name_e
 *  \param[in] eDrive: gpio pin drive level, \ref csi_gpio_drive_e
 *  \return error code \ref csi_error_t
 */ 
void csi_gpio_drive(csp_gpio_t *ptGpioBase, pin_name_e ePinName, csi_gpio_drive_e eDrive)
{
	uint8_t byPinNum = 0;
	
	byPinNum = apt_get_gpio_num(ePinName);
	csp_gpio_set_drive(ptGpioBase, byPinNum, (uint8_t)eDrive);
		
}
/** \brief enable gpio input filtering
 * 
 *  \param[in] ptGpioBase: pointer of gpio register structure
 *  \param[in] ePinName: gpio pin name, \ref pin_name_e
 *  \param[in] bEnable:  ENABLE/DISABLE
 *  \return none
 */ 
void csi_gpio_input_filter(csp_gpio_t *ptGpioBase, pin_name_e ePinName, bool bEnable)
{
	csp_gpio_flt_enable(ptGpioBase, ePinName, bEnable);
}
/** \brief get gpio num
 * 
 *  \param[in] ePinName: gpio pin name, \ref pin_name_e
 *  \return pin num
 */ 
uint8_t csi_gpio_get_num(pin_name_e ePinName)
{
    return apt_get_gpio_num(ePinName);
}

/** \brief  gpio pin set high(output = 1)
 *
 *  \param[in] ptGpioBase: pointer of gpio register structure 
 *  \param[in] ePinName: gpio pin name, \ref pin_name_e
 *  \return none
 */
void csi_gpio_set_high(csp_gpio_t *ptGpioBase, pin_name_e ePinName)
{
	csp_gpio_set_high(ptGpioBase, (uint16_t)ePinName);
}

/** \brief   gpio pin set low(output = 0)
 *
 *  \param[in] ptGpioBase: pointer of gpio register structure 
 *  \param[in] ePinName: gpio pin name, \ref pin_name_e
 *  \return none
 */
void csi_gpio_set_low(csp_gpio_t *ptGpioBase, pin_name_e ePinName)
{
	csp_gpio_set_low(ptGpioBase, (uint16_t)ePinName);
}
/** \brief  gpio toggle
 * 
 *  \param[in] ptGpioBase: pointer of gpio register structure 
 *  \param[in] ePinName: gpio pin name, \ref pin_name_e
 *  \return none
 */
void csi_gpio_toggle(csp_gpio_t *ptGpioBase, pin_name_e ePinName)
{
	if(ptGpioBase->ODSR & ePinName)
		ptGpioBase->CODR = ePinName;
	else
		ptGpioBase->SODR = ePinName;
}
/** \brief get the value of selected gpio 
 * 
 *  \param[in] ptGpioBase: pointer of gpio register structure 
 *  \param[in] ePinName: gpio pin name, \ref pin_name_e
 *  \return According to the bit mask, the corresponding pin status is obtained
*/
bool csi_gpio_read(csp_gpio_t *ptGpioBase, pin_name_e ePinName)
{
	if(csp_gpio_read_input(ptGpioBase) & ePinName)
		return true;
	else
		return false;
}
/** \brief  write gpio port ouput value
 * 
 *  \param[in] ptGpioBase: pointer of gpio register structure
 *  \param[in] wPinMask: pin mask, mask of write pin
 *  \param[in] hwVal: Value of write
 *  \return none
 */ 
void  csi_gpio_write_port(csp_gpio_t *ptGpioBase, uint16_t hwPinMask, uint16_t hwVal)
{
	uint16_t hwState = csp_gpio_read_output(ptGpioBase);
	csp_gpio_set_wodr(ptGpioBase, ((hwState & ~hwPinMask) | hwVal));
}
/** \brief config gpio irq mode(assign exi group)
 * 
 *  \param[in] ptGpioBase: pointer of gpio register structure 
 *  \param[in] ePinName: pin name, \ref pin_name_e
 *  \param[in] eExiGrp: exi group, \ref csi_exi_grp_e
 *  \param[in] eTrgEdge: edge mode, \ref csi_gpio_irq_mode_e
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_gpio_irq_mode(csp_gpio_t *ptGpioBase, pin_name_e ePinName, csi_exi_grp_e eExiGrp, csi_gpio_irq_mode_e eTrgEdge)
{
	csi_error_t ret = CSI_OK;
	uint8_t byPinNum = 0;
	
	byPinNum = apt_get_gpio_num(ePinName);
	
	ret = apt_gpio_intgroup_set(ptGpioBase, byPinNum,(gpio_igrp_e)eExiGrp);					//interrupt group
	if(ret < 0)
		return CSI_ERROR;
	
	if(eTrgEdge >  GPIO_IRQ_BOTH_EDGE)
		return CSI_ERROR;
	else
		apt_exi_trg_edge_set(SYSCON,(gpio_igrp_e)eExiGrp, (exi_trigger_e)eTrgEdge);			//interrupt edge
	
	csp_exi_clr_isr(SYSCON,(0x01ul << eExiGrp));		
	csp_exi_int_enable(SYSCON,(0x01ul << eExiGrp));				//EXI interrupt enable
	
	
	return ret;
}

/** \brief gpio interrupt enable
 *
 *  \param[in] ptGpioBase: pointer of gpio register structure  
 *  \param[in] ePinName: pin name, \ref pin_name_e
 *  \return error code \ref csi_error_t
 */ 
void csi_gpio_int_enable(csp_gpio_t *ptGpioBase, pin_name_e ePinName)
{
	csp_gpio_int_enable(ptGpioBase, ePinName);
}

/** \brief gpio interrupt disable
 *
 *  \param[in] ptGpioBase: pointer of gpio register structure  
 *  \param[in] ePinName: pin name, \ref pin_name_e
 *  \return error code \ref csi_error_t
 */ 
void csi_gpio_int_disable(csp_gpio_t *ptGpioBase,pin_name_e ePinName)
{
	csp_gpio_int_disable(ptGpioBase, ePinName);
}

/** \brief gpio vic irq enable
 * 
 *  \param[in] eExiGrp: exi group(exi line), \ref csi_exi_grp_e
 *  \param[in] bEnable: ENABLE OR DISABLE
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_gpio_vic_irq_enable(csi_exi_grp_e eExiGrp, bool bEnable)
{
	uint32_t byIrqNum = EXI0_IRQ_NUM;
	
	switch(eExiGrp)
	{
		case EXI_GRP0:
		case EXI_GRP16:
			byIrqNum = EXI0_IRQ_NUM;
			break;
		case EXI_GRP1:
		case EXI_GRP17:
			byIrqNum = EXI1_IRQ_NUM;
			break;
		case EXI_GRP2:
		case EXI_GRP3:
		case EXI_GRP18:
		case EXI_GRP19:
			byIrqNum = EXI2_IRQ_NUM;
			break;
		default:
			if(eExiGrp < EXI_GRP10)				//group4->9
				byIrqNum = EXI3_IRQ_NUM;
			else if(eExiGrp < EXI_GRP16)		//group10->15
				byIrqNum = EXI4_IRQ_NUM;
			else
				return CSI_ERROR;
				
			break;
	}
	
	if(bEnable)
		csi_vic_enable_irq(byIrqNum);
	else
		csi_vic_disable_irq(byIrqNum);
	
	return CSI_OK;
}

/** \brief clear exi status
 *
 *  \param[in] ptGpioBase: pointer of gpio register structure  
 *  \param[in] eExiStatus: exi status, \ref csi_exi_status_e
 *  \return error code \ref csi_error_t
 */ 
void csi_exi_clr_isr(csi_exi_status_e eExiStatus)
{
	csp_exi_clr_isr(SYSCON, eExiStatus);
}

/** \brief  set exi as trigger event(EV0~5) 
 *  \param[in] eTrgOut: output event select \ref csi_exi_trgout_e
 *  \param[in] eExiTrgSrc: event source \ref csi_exi_trgsrc_e
 *  \param[in] byTrgPrd: accumulated EXI events to output trigger; 0~16
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_exi_set_evtrg(csi_exi_trgout_e eTrgOut, csi_exi_trgsrc_e eExiTrgSrc, uint8_t byTrgPrd)
{
	
	byTrgPrd &= 0xf;
	//set evtrg source 
	if ((eTrgOut < 4) && (eExiTrgSrc < 16))	
	{
		csp_exi_set_evtrg0_3(SYSCON, eTrgOut, eExiTrgSrc);
		if(byTrgPrd)		//set evtrg period
		{
			csp_exi_clr_evtrg_cnt(SYSCON, eTrgOut);					//clear TRG EVxCNT
			csp_exi_set_evtrg_cnt(SYSCON, eTrgOut, byTrgPrd);
		}
	}
	else if (eTrgOut < 6 && eExiTrgSrc > 15) 
		csp_exi_set_evtrg4_5(SYSCON, eTrgOut, eExiTrgSrc);
	else
		return CSI_ERROR;
	
	return CSI_OK;
}

/** \brief exi evtrg output enable
 * 
 *  \param[in] eTrgOut: output event select \ref csi_exi_trgout_e
 *  \param[in] bEnable: ENABLE/DISABLE
 *  \return none
 */
void csi_exi_evtrg_enable(csi_exi_trgout_e eTrgOut)
{
	csp_exi_evtrg_enable(SYSCON, eTrgOut);
}
/** \brief exi evtrg output disable
 * 
 *  \param[in] eTrgOut: output event select \ref csi_exi_trgout_e
 *  \param[in] bEnable: ENABLE/DISABLE
 *  \return none
 */
void csi_exi_evtrg_disable(csi_exi_trgout_e eTrgOut)
{
	csp_exi_evtrg_enable(SYSCON, eTrgOut);
}

/** \brief  exi software trigger event 
 *  \param[in] eTrgOut: output event select \ref csi_exi_trgout_e
 *  \return none
 */ 
void csi_exi_soft_evtrg(csi_exi_trgout_e eTrgOut)
{
	csp_exi_soft_evtrg(SYSCON, eTrgOut);
}
