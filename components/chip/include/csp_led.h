/***********************************************************************//** 
 * \file  csp_led.h
 * \brief  LED description and static inline void functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-20 <td>V0.0  <td>ZJY   <td>initial
 * <tr><td> 2020-12-3 <td>V0.1  <td>WNN   <td>modiied
 * <tr><td> 2023-3-21 <td>V0.1  <td>WCH   <td>modified
 * <tr><td> 2023-9-1  <td>V0.2  <td>WCH   <td>code normalization
 * 
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CSP_LED_H
#define _CSP_LED_H

/* Includes ------------------------------------------------------------------*/
#include <soc.h>

/// \struct csp_led_t
/// \brief LED Reg Description
typedef volatile struct
{
    __IOM uint32_t      CR;				//0x0000		Control Register                 
    __IOM uint32_t      BRIGHT;       	//0x0004   		Brightness Control Register      
    __IM  uint32_t      RISR;         	//0x0008   		Raw Interrupt Status Register    
    __IOM uint32_t      IMCR;         	//0x000C   		Interrupt Masking Control Reg.   
    __IM  uint32_t      MISR;         	//0x0010    	Masked Interrupt Status Register 
    __OM  uint32_t      ICR;          	//0x0014   		Interrupt Clear Register         
    __IOM uint32_t      RSVD;			//0x0018 		Reserved
    __IOM uint32_t      TIMCR;        	//0x001C    	Timing Control Register          
    __OM  uint32_t      BLKER;       	//0x0020    	Blink Enable Control Register    
    __OM  uint32_t      BLKDR;        	//0x0024   		Blink Disable Control Register   
    __IM  uint32_t      BLKST;        	//0x0028    	Blink Status Control Register    
    __IOM uint32_t      SEGDATA[10];    //0x002C~0x0050 Segment Data
 } csp_led_t;
 
/******************************************************************************
* LED Regiser
******************************************************************************/ 
///CR
#define LED_COM_EN_POS                  16                                     /*!< LED CR: COM_EN Position */
#define LED_COM_EN_MSK                  (0x3FFUL << LED_COM_EN_POS)            /*!< LED CR: COM_EN Mask */

#define LED_LEDCLK_POS                  1                                      /*!< LED CR: LEDCLK Position */
#define LED_LEDCLK_MSK                  (0x7UL << LED_LEDCLK_POS)              /*!< LED CR: LEDCLK Mask */
typedef enum{
	LED_PCLK_D4 = 0,
	LED_PCLK_D8,
	LED_PCLK_D16,
	LED_PCLK_D32,
	LED_PCLK_D64,
	LED_PCLK_D128,
	LED_PCLK_D256,
}led_clkdiv_e;

#define LED_LIGHTON_POS                 0                                       /*!< LED CR: LIGHTON Position */
#define LED_LIGHTON_MSK                 (0x1UL << LED_LIGHTON_POS)              /*!< LED CR: LIGHTON Mask */

///BRIGHT
#define LED_BRT_POS                 	0                                       /*!< LED BRIGHT: BRT Position */
#define LED_BRT_MSK                 	(0x7UL << LED_BRT_POS)                  /*!< LED BRIGHT: BRT Mask */
typedef enum {
	LED_100 = 0,
	LED_87,
	LED_75,
	LED_62,
	LED_50,
	LED_37,
	LED_25,
	LED_12
}led_brt_e;

///RISR/IMCR/MISR/ICR
typedef enum {
	LED_INT_ICEND = (0x01ul << 0),
	LED_INT_IPEND = (0x01ul << 1),
	LED_INT_ALL   = 0x0Ful
}led_int_e;

///TIMCR
#define LED_NOVCNT_POS               	8                                        /*!< LED TIMCR: NOVCNT Position */
#define LED_NOVCNT_MSK               	(0xFFUL << LED_NOVCNT_POS)               /*!< LED TIMCR: NOVCNT Mask */

#define LED_DCOMCNT_POS              	0                                        /*!< LED TIMCR: DCOMCNT Position */
#define LED_DCOMCNT_MSK              	(0xFFUL << LED_DCOMCNT_POS)              /*!< LED TIMCR: DCOMCNT Mask */
 
///BLKER/BLKDR/BLKST
#define LED_BLK_MSK						(0x3ff)

///SEGDATA0~9
#define LED_SEGDATA_POS(n)              n                                        /*!< LED SEGDATn: Position */
#define LED_SEGDATA_MSK(n)              (0x1UL << LED_SEGDAT0_SEG7_POS)          /*!< LED SEGDATn: Mask */

/******************************************************************************
********************* LED inline Functions**********************
******************************************************************************/
//on/off
static inline void csp_led_light_on(csp_led_t *ptLedBase) 					 
{
	ptLedBase -> CR = (ptLedBase -> CR & (~LED_LIGHTON_MSK)) | (0x1 << LED_LIGHTON_POS);
}

static inline void csp_led_light_off(csp_led_t *ptLedBase) 	
{
	ptLedBase -> CR &= ~LED_LIGHTON_MSK;
}		

static inline void csp_led_com_on(csp_led_t *ptLedBase,uint16_t hwVal)	
{
	ptLedBase -> BLKDR = hwVal;
}			 

static inline void csp_led_com_off(csp_led_t *ptLedBase,uint16_t hwVal)	
{
	ptLedBase -> BLKER = hwVal;
}		  	 

//write reg
static inline void csp_led_set_clk(csp_led_t *ptLedBase, led_clkdiv_e eClk) 		
{
	ptLedBase -> CR = (ptLedBase -> CR & (~LED_LEDCLK_MSK)) | (eClk << LED_LEDCLK_POS);
}	
 
static inline void csp_led_set_brt(csp_led_t *ptLedBase, led_brt_e eBrt)
{
	ptLedBase -> BRIGHT = eBrt;
}  	

static inline void csp_led_set_commask(csp_led_t *ptLedBase,uint8_t byComMsk) 	 
{
	ptLedBase -> CR = (ptLedBase -> CR & (~LED_COM_EN_MSK)) | (byComMsk << LED_COM_EN_POS);
}

static inline void csp_led_set_dcomcnt(csp_led_t *ptLedBase,uint8_t byVal)	
{
	ptLedBase -> TIMCR = (ptLedBase -> TIMCR & (~LED_DCOMCNT_MSK)) | (byVal << LED_DCOMCNT_POS);
}	 

static inline void csp_led_set_novcnt(csp_led_t *ptLedBase,uint8_t byVal)   
{
	ptLedBase -> TIMCR = (ptLedBase -> TIMCR & (~LED_NOVCNT_MSK)) | (byVal << LED_NOVCNT_POS);
}  	 

static inline void csp_led_set_data(csp_led_t *ptLedBase,uint8_t byCom,uint8_t byData)
{
	ptLedBase -> SEGDATA[byCom] = byData;
}

//read reg
static inline uint16_t csp_led_get_blink_status(csp_led_t *ptLedBase)
{
	return ptLedBase -> BLKST;
}			 

//interrupt
static inline void csp_led_int_enable(csp_led_t *ptLedBase,led_int_e eInt)
{
	ptLedBase -> IMCR |= eInt;
}	

static inline void csp_led_int_disable(csp_led_t *ptLedBase,led_int_e eInt)
{
	ptLedBase -> IMCR &= ~eInt;
}	

static inline uint32_t csp_led_get_risr(csp_led_t *ptLedBase)
{
	return ptLedBase -> RISR;
}

static inline uint32_t csp_led_get_isr(csp_led_t *ptLedBase)
{
	return ptLedBase -> MISR;
}

static inline void csp_led_clr_isr(csp_led_t *ptLedBase,led_int_e eInt)		
{
	ptLedBase -> ICR  =  eInt;
}	
#endif