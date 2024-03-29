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
#include "soc.h"
#include "csp_syscon.h"

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
    GPIO_INT0 	= 0,
    GPIO_INT1 	= 1,
    GPIO_INT2 	= 2,
    GPIO_INT3 	= 3,
    GPIO_INT4 	= 4,
    GPIO_INT5 	= 5,
    GPIO_INT6 	= 6,
    GPIO_INT7 	= 7,
    GPIO_INT8 	= 8,
    GPIO_INT9 	= 9,
    GPIO_INT10 	= 10,
    GPIO_INT11 	= 11,
    GPIO_INT12 	= 12,
    GPIO_INT13 	= 13,
	GPIO_INT14	= 14,
	GPIO_INT15	= 15,
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
static inline void csp_gpio_pullnone_enable(csp_gpio_t *ptGpioBase, uint8_t byPinNum)	
{
	ptGpioBase->PUDR &= ~(0x03 << (byPinNum*2));
}
static inline void csp_gpio_pullup_enable(csp_gpio_t *ptGpioBase, uint8_t byPinNum)	
{ 
	ptGpioBase->PUDR = (ptGpioBase->PUDR & ~(0x03 << (byPinNum*2))) | (0x01 << (byPinNum*2));
}
static inline void csp_gpio_pulldown_enable(csp_gpio_t *ptGpioBase, uint8_t byPinNum)
{
	ptGpioBase->PUDR = (ptGpioBase->PUDR & ~(0x03 << (byPinNum*2))) | (0x02 << (byPinNum*2));
}

/*************************************************************************
 * @brief  gpio exi set 
****************************************************************************/
static inline void csp_gpio_int_enable(csp_gpio_t *ptGpioBase, uint16_t hwPinMsk)	
{
	ptGpioBase->IEER = hwPinMsk;
}
static inline void csp_gpio_int_disable(csp_gpio_t *ptGpioBase, uint16_t hwPinMsk)	
{
	ptGpioBase->IEDR = hwPinMsk;
}
static inline uint32_t csp_exi_get_isr(csp_syscon_t *ptSysconBase)			
{
	return ptSysconBase->EXICR;
}
static inline void csp_exi_clr_isr(csp_syscon_t *ptSysconBase, uint32_t wValue)		
{
	ptSysconBase->EXICR = wValue;
}
static inline void csp_exi_int_enable(csp_syscon_t *ptSysconBase, gpio_igrp_e eExiGrp)	
{
	ptSysconBase->EXIER = (0x01ul << eExiGrp);
}
static inline void csp_exi_int_disable(csp_syscon_t *ptSysconBase, gpio_igrp_e eExiGrp)
{
	ptSysconBase->EXIDR = (0x01ul << eExiGrp);
}

/*************************************************************************
 * @brief  gpio filtering
****************************************************************************/
static inline void csp_gpio_flt_enable(csp_gpio_t *ptGpioBase, uint16_t hwPinMsk, bool bEnable)	
{
	if(bEnable)
		ptGpioBase->FLTEN = hwPinMsk;
	else
		ptGpioBase->FLTEN &= ~hwPinMsk;
}

/*************************************************************************
 * @brief  gpio output mode set 
****************************************************************************/
static inline void csp_gpio_opendrain_enable(csp_gpio_t *ptGpioBase, uint16_t hwPinMsk)
{
	ptGpioBase->OMCR |= hwPinMsk;
}
static inline void csp_gpio_opendrain_disable(csp_gpio_t *ptGpioBase, uint16_t hwPinMsk)		
{
	ptGpioBase->OMCR &= ~hwPinMsk;
}

/*************************************************************************
 * @brief  gpio pin coms/ttl/driver/speed/
****************************************************************************/
static inline void csp_gpio_ttl_enable(csp_gpio_t *ptGpioBase, uint8_t byPinNum)
{
	ptGpioBase->DSCR |= (0x02ul << (2*byPinNum));	
}
static inline void csp_gpio_cmos_enable(csp_gpio_t *ptGpioBase, uint8_t byPinNum)
{
	ptGpioBase->DSCR &= ~(0x02ul << (2*byPinNum));
}

static inline void csp_gpio_set_ttl1(csp_gpio_t *ptGpioBase, uint8_t byPinNum)	
{
	ptGpioBase->OMCR &= ~(0x01 << (byPinNum+16));	
}
static inline void csp_gpio_set_ttl2(csp_gpio_t *ptGpioBase, uint8_t byPinNum)
{
	ptGpioBase->OMCR |= (0x01ul << (byPinNum+16));
}

static inline void csp_gpio_set_drive(csp_gpio_t *ptGpioBase, uint8_t byPinNum, uint8_t byDrive)
{
	if(byDrive)
		ptGpioBase->DSCR |= (0x01ul << 2*byPinNum);
	else
		ptGpioBase->DSCR &= ~(0x01ul << 2*byPinNum);
}
static inline void csp_gpio_set_speed(csp_gpio_t *ptGpioBase, uint8_t byPinNum, uint8_t bySpeed)
{
	if(bySpeed)
		ptGpioBase->DSCR |= (0x02ul << 2*byPinNum);
	else
		ptGpioBase->DSCR &= ~(0x02ul << 2*byPinNum);
	
}
/*************************************************************************
 * @brief  gpio output or input or status handle
****************************************************************************/
static inline void csp_gpio_set_wodr(csp_gpio_t *ptGpioBase, uint16_t hwVal)			
{
	ptGpioBase->WODR = hwVal;
}
static inline void csp_gpio_set_high(csp_gpio_t *ptGpioBase, uint16_t hwPinMsk)		
{
	ptGpioBase->SODR = hwPinMsk;
}
static inline void csp_gpio_set_low(csp_gpio_t *ptGpioBase, uint16_t hwPinMsk)		
{
	ptGpioBase->CODR = hwPinMsk;
}
//read gpio
static inline uint16_t csp_gpio_read_input(csp_gpio_t *ptGpioBase)			
{
	return (uint16_t)ptGpioBase->PSDR;
}
static inline uint16_t csp_gpio_read_output(csp_gpio_t *ptGpioBase)				
{
	return (uint16_t)ptGpioBase->ODSR;
}

//exi trigger 

static inline void csp_exi_set_evtrg0_3(csp_syscon_t *ptSysconBase, uint8_t byTrgOut, uint8_t byTrgSrc)
{
	ptSysconBase->EVTRG = (ptSysconBase->EVTRG & ~(TRG_SRC0_3_MSK(byTrgOut))) | (byTrgSrc << TRG_SRC0_3_POS(byTrgOut));
}
static inline void csp_exi_set_evtrg4_5(csp_syscon_t *ptSysconBase, uint8_t byTrgOut, uint8_t byTrgSrc)
{
	ptSysconBase->EVTRG = (ptSysconBase->EVTRG & ~(TRG_SRC4_5_MSK(byTrgOut))) | ((byTrgSrc-16) << TRG_SRC4_5_POS(byTrgOut));
}
static inline void csp_exi_clr_evtrg_cnt(csp_syscon_t *ptSysconBase, uint8_t byTrgOut)
{
	ptSysconBase->EVTRG |= TRG_EVCNT_CLR_MSK(byTrgOut);								//clear TRG EVxCNT
}

static inline void csp_exi_set_evtrg_cnt(csp_syscon_t *ptSysconBase, uint8_t byTrgOut, uint8_t byTrgPrd)
{
	ptSysconBase->EVPS = (ptSysconBase->EVPS & ~(TRG_EVPRD_MSK(byTrgOut))) | ((byTrgPrd - 1) << TRG_EVPRD_POS(byTrgOut));
}

static inline void csp_exi_evtrg_enable(csp_syscon_t *ptSysconBase, uint8_t byTrgOut)		
{
	ptSysconBase->EVTRG |= ENDIS_ESYNC_MSK(byTrgOut); 
}
static inline void csp_exi_evtrg_disable(csp_syscon_t *ptSysconBase, uint8_t byTrgOut)		
{
	ptSysconBase->EVTRG &= ~ENDIS_ESYNC_MSK(byTrgOut); 
}
static inline void csp_exi_sw_evtrg(csp_syscon_t *ptSysconBase, uint8_t byTrgOut)		
{
	ptSysconBase->EVSWF = (0x01ul << byTrgOut);
}


#endif   
