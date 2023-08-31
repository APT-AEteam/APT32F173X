/***********************************************************************//** 
 * \file  csp_gpio.h
 * \brief  GPIO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-12 <td>V0.0  <td>ZJY   <td>initial
 * <tr><td> 2020-12-20 <td>V0.1  <td>ZJY   <td>modify for 110
 * <tr><td> 2021-1-12 <td>V0.2  <td>ZJY   <td> adapt for 102x/003
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CSP_GPIO_H
#define _CSP_GPIO_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <sys_clk.h>
#include "csp_common.h"

#define GPIO_RESET_VALUE  	(0x00000000)

/// \struct csp_gpio_t
/// \brief GPIO Reg Description
typedef struct
{
	__IOM  uint32_t  CONLR;		//0x0000	Control Low Register                
    __IOM  uint32_t  CONHR;     //0x0004	Control High Register                
    __OM   uint32_t  WODR;      //0x0008	Write Output Data Register                         
    __OM   uint32_t  SODR;      //0x000C	Set Output Data (bit-wise) Register  
    __OM   uint32_t  CODR;      //0x0010	Clear Output Data (bit-wise) Register
    __IM   uint32_t  ODSR;      //0x0014	Output Data Status Register          
    __IM   uint32_t  PSDR;      //0x0018	Pin Data Status Register                           
    __OM   uint32_t  FLTEN;     //0x001C    Filtering Enable Control Register
    __IOM  uint32_t  PUDR;      //0x0020	IO Pullup_Pulldown Register          
    __IOM  uint32_t  DSCR;      //0x0024	Output Driving Strength Register     
    __IOM  uint32_t  OMCR;      //0x0028	Slew-rate, Open-Drain Control          
    __IOM  uint32_t  IECR;      //0x002C	EXI Enable Control Register                     
	__OM   uint32_t  IEER;		//0x0030	EXI Enable Eet Register    
	__OM   uint32_t  IEDR;		//0x0034	EXI Enable Clear Register
} csp_gpio_t; 


typedef volatile struct
{
	__IOM  uint32_t  IGRPL;     //0x0000 	EXI Group Config Register L                  
    __IOM  uint32_t  IGRPH;	    //0x0004	EXI Group Config Register H                   
	__IOM  uint32_t  IGREX;		//0x0008	EXI Group Expand Config Register  
    __IOM  uint32_t  IO_CLKEN;	//0x000C 	Gpio Group Clk Enable Control Register
} csp_igrp_t; 


/******************************************************************************
* PIN_NUM: GPIO PIN Number
******************************************************************************/
/*
typedef enum
{
  PIN_NUM0    = 0,  	// Pin 0 selected 
  PIN_NUM1    = 1,  	// Pin 1 selected 
  PIN_NUM2    = 2,  	// Pin 2 selected 
  PIN_NUM3    = 3,   	// Pin 3 selected 
  PIN_NUM4    = 4,  	// Pin 4 selected 
  PIN_NUM5    = 5,  	// Pin 5 selected 
  PIN_NUM6    = 6,  	// Pin 6 selected 
  PIN_NUM7    = 7,  	// Pin 7 selected 
  PIN_NUM8    = 8,  	// Pin 8 selected 
  PIN_NUM9    = 9,  	// Pin 9 selected 
  PIN_NUM10   = 10,  	// Pin 10 selected 
  PIN_NUM11   = 11,   	// Pin 11 selected 
  PIN_NUM12   = 12,  	// Pin 12 selected 
  PIN_NUM13   = 13,  	// Pin 13 selected 
  PIN_NUM14   = 14,  	// Pin 14 selected 
  PIN_NUM15   = 15,  	// Pin 15 selected 
}gpio_pin_e;
*/
/******************************************************************************
* ODSR: PDSR: Output Data and Pin Data Status Register 
******************************************************************************/
typedef enum
{
    LOWBYTE	 = 0,
    HIGHBYTE = 1,
}gpio_byte_e;

typedef enum
{
	PIN_LOW	 = 0,
    PIN_HIGH = 1
}gpio_pin_state_e;

/******************************************************************************
* CONLR: CONHR: GPIO MUX Control  Register 
******************************************************************************/
typedef enum
{
    INPUT	= 1,
    OUTPUT	= 0,
}gpio_dir_e;
/******************************************************************************
* PUDR: DSCR: IO PULL AND Output Driving Strength Register 
******************************************************************************/


/******************************************************************************
* IECR: IEER: IEDR: GPIO EXI Control Register 
******************************************************************************/
typedef enum
{
    EXI0 	= 0,
    EXI1 	= 1,
    EXI2 	= 2,
    EXI3 	= 3,
    EXI4 	= 4,
    EXI5 	= 5,
    EXI6 	= 6,
    EXI7 	= 7,
    EXI8 	= 8,
    EXI9 	= 9,
    EXI10 	= 10,
    EXI11 	= 11,
    EXI12 	= 12,
    EXI13 	= 13,
	EXI14	= 14,
	EXI15	= 15,
	//EXI16	= 16,
	//EXI17	= 17,
	//EXI18	= 18,
	//EXI19	= 19
}gpio_int_e;
/******************************************************************************
* IGRPL: IGRPH: IGRPH: Group Config Register
******************************************************************************/
typedef enum
{
    GRP_GPIOA 	= 0,
    GRP_GPIOB 	= 2,
	GRP_GPIOC 	= 4,
	GRP_GPIOD 	= 6,
}gpio_group_e;


typedef enum
{
	EXI_IGRP0		= (uint32_t)(0),						
	EXI_IGRP1		= (uint32_t)(1),
	EXI_IGRP2		= (uint32_t)(2),
	EXI_IGRP3		= (uint32_t)(3),
	EXI_IGRP4		= (uint32_t)(4),
	EXI_IGRP5		= (uint32_t)(5),
	EXI_IGRP6		= (uint32_t)(6),
	EXI_IGRP7		= (uint32_t)(7),
	EXI_IGRP8		= (uint32_t)(8),
	EXI_IGRP9		= (uint32_t)(9),
	EXI_IGRP10		= (uint32_t)(10),
	EXI_IGRP11		= (uint32_t)(11),
	EXI_IGRP12		= (uint32_t)(12),
	EXI_IGRP13		= (uint32_t)(13),
	EXI_IGRP14		= (uint32_t)(14),
	EXI_IGRP15		= (uint32_t)(15),
	EXI_IGRP16		= (uint32_t)(16),
	EXI_IGRP17		= (uint32_t)(17),
	EXI_IGRP18		= (uint32_t)(18),
	EXI_IGRP19		= (uint32_t)(19)
}gpio_igrp_e;

/******************************************************************************
* GPIO EXI: 
******************************************************************************/
typedef enum
{
	EXI0_IRQ_MAP	= 0x01,			
	EXI1_IRQ_MAP	= 0x02,	
	EXI2_IRQ_MAP	= 0x0c,
	EXI3_IRQ_MAP	= 0x3f0,
	EXI4_IRQ_MAP	= 0xfc00
}exi_map_e;

typedef enum
{
	EXI_EDGE_IRT	= 0,
	EXI_EDGE_IFT	= 1,	
	EXI_EDGE_BOTH	= 2
}exi_trigger_e;

/******************************************************************************
********************* gpio inline Functions Declaration ***********************
******************************************************************************/
/*************************************************************************
 * @brief  gpio pull set 
****************************************************************************/
#define csp_gpio_pullnone(GPIOx, byPinNum)	((GPIOx)->PUDR = (GPIOx)->PUDR & ~(0x03 << (byPinNum*2)))
#define csp_gpio_pullup(GPIOx, byPinNum)	((GPIOx)->PUDR = ((GPIOx)->PUDR & ~(0x03 << (byPinNum*2))) | (0x01 << (byPinNum*2)))
#define csp_gpio_pulldown(GPIOx, byPinNum)	((GPIOx)->PUDR = ((GPIOx)->PUDR & ~(0x03 << (byPinNum*2))) | (0x02 << (byPinNum*2)))

/*************************************************************************
 * @brief  gpio exi set 
****************************************************************************/
#define csp_gpio_int_enable(GPIOx, hwPinMsk)	((GPIOx)->IEER = hwPinMsk)
#define csp_gpio_int_disable(GPIOx, hwPinMsk)	((GPIOx)->IEDR = hwPinMsk)
#define csp_exi_get_isr(SYSCON)					((SYSCON)->EXICR)
#define csp_exi_clr_isr(SYSCON, eIsr)			((SYSCON)->EXICR = eIsr)
#define csp_exi_int_enable(SYSCON, eExiGrp)		((SYSCON)->EXIER = (0x01ul << eExiGrp))
#define csp_exi_int_disable(SYSCON, eExiGrp)	((SYSCON)->EXIDR = (0x01ul << eExiGrp))

/*************************************************************************
 * @brief  gpio filtering
****************************************************************************/
#define csp_gpio_flt_enable(GPIOx, hwPinMsk, bEnable)	(bEnable > 0 ? ((GPIOx)->FLTEN = hwPinMsk) : ((GPIOx)->FLTEN &= ~hwPinMsk))  

/*************************************************************************
 * @brief  gpio output mode set 
****************************************************************************/
#define csp_gpio_opendrain_enable(GPIOx, hwPinMsk)		((GPIOx)->OMCR |= hwPinMsk)
#define csp_gpio_opendrain_disable(GPIOx, hwPinMsk)		((GPIOx)->OMCR &= ~hwPinMsk)

/*************************************************************************
 * @brief  gpio pin coms/ttl/driver/speed/
****************************************************************************/
#define csp_gpio_ccm_ttl(GPIOx, byPinNum)	((GPIOx)->DSCR |= (0x02ul << (2*byPinNum)))	
#define csp_gpio_ccm_cmos(GPIOx, byPinNum)	((GPIOx)->DSCR &= ~(0x02ul << (2*byPinNum)))

#define csp_gpio_set_ttl1(GPIOx, byPinNum)	((GPIOx)->OMCR &= ~(0x01 << (byPinNum+16)))	
#define csp_gpio_set_ttl2(GPIOx, byPinNum)	((GPIOx)->OMCR |= (0x01ul << (byPinNum+16)))

#define csp_gpio_set_drive(GPIOx, byPinNum, byDrive)	(byDrive == 0 ?  ((GPIOx)->DSCR &= ~(0x01ul << 2*byPinNum)) : \
														((GPIOx)->DSCR |= (0x01ul << 2*byPinNum)))
														
#define csp_gpio_set_speed(GPIOx, byPinNum, bySpeed)	(bySpeed == 0 ?  ((GPIOx)->DSCR &= ~(0x02ul << 2*byPinNum)) : \
														((GPIOx)->DSCR |= (0x02ul << 2*byPinNum)))
		
/*************************************************************************
 * @brief  gpio output or input or status handle
****************************************************************************/
#define csp_gpio_set_wodr(GPIOx, wVal)			((GPIOx)->WODR = wVal)
#define csp_gpio_set_high(GPIOx, hwPinMsk)		((GPIOx)->SODR = hwPinMsk)
#define csp_gpio_set_low(GPIOx, hwPinMsk)		((GPIOx)->CODR = hwPinMsk)

#define csp_gpio_read_input(GPIOx)				((GPIOx)->PSDR)
#define csp_gpio_read_output(GPIOx)				((GPIOx)->ODSR)


#endif   // apt32f173_gpio_H */

/******************* (C) COPYRIGHT 2019 APT Chip *****END OF FILE****/


