/***********************************************************************//** 
 * \file  csp_led.h
 * \brief  LED description and static inline void functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-20 <td>V0.0  <td>ZJY   <td>initial
 * <tr><td> 2020-12-3 <td>V0.1  <td>WNN   <td>modiied
 * <tr><td> 2023-3-21 <td>V0.1  <td>WCH   <td>modified
 * <tr><td> 2023-9-1  <td>V0.2  <td>WCH   <td>modified
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
}csp_led_ledclk_e;

#define LED_LIGHTON_POS                 0                                       /*!< LED CR: LIGHTON Position */
#define LED_LIGHTON_MSK                 (0x1UL << LED_LIGHTON_POS)              /*!< LED CR: LIGHTON Mask */

///BRIGHT
#define LED_BRT_POS                 	0                                       /*!< LED BRIGHT: BRT Position */
#define LED_BRT_MSK                 	(0x7UL << LED_BRT_POS)                  /*!< LED BRIGHT: BRT Mask */
typedef enum {
	COM100 = 0,
	COM87,
	COM75,
	COM62,
	COM50,
	COM37,
	COM25,
	COM12
}csp_led_brt_e;

///RISR/IMCR/MISR/ICR
typedef enum {
	LED_INT_ICEND = 1,
	LED_INT_IPEND
}csp_led_int_e;

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
********************* LEDx Macro Definition Declaration **********************
******************************************************************************/
//on/off
#define csp_led_light_on(LEDx) 			((LEDx) -> CR = ((LEDx) -> CR & (~LED_LIGHTON_MSK)) | (0x1 << LED_LIGHTON_POS))
#define csp_led_light_off(LEDx) 		((LEDx) -> CR = (LEDx) -> CR & (~LED_LIGHTON_MSK))
#define csp_led_com_on(LEDx, hwVal)		((LEDx) -> BLKDR = hwVal)
#define csp_led_com_off(LEDx,hwVal)		((LEDx) -> BLKER = hwVal)

//write reg
#define csp_led_set_clk(LEDx, eClk) 		((LEDx) -> CR = ((LEDx) -> CR & (~LED_LEDCLK_MSK)) | (eClk << LED_LEDCLK_POS))
#define csp_led_set_brt(LEDx, eBrt)  	    ((LEDx) -> BRIGHT = eBrt)
#define csp_led_set_commask(LEDx, byComMsk) ((LEDx) -> CR = ((LEDx) -> CR & (~LED_COM_EN_MSK)) | (byComMsk << LED_COM_EN_POS))

#define csp_led_set_dcomcnt(LEDx, byVal)	((LEDx) -> TIMCR = ((LEDx) -> TIMCR & (~LED_DCOMCNT_MSK)) | (byVal << LED_DCOMCNT_POS))
#define csp_led_set_novcnt(LEDx, byVal)     ((LEDx) -> TIMCR = ((LEDx) -> TIMCR & (~LED_NOVCNT_MSK)) | (byVal << LED_NOVCNT_POS))
#define csp_led_set_setdata(LEDx, byCom, byData) ((LEDx) -> SEGDATA[byCom] = byData)

//read reg
#define csp_led_get_com_st(LEDx)  	((LEDx) -> BLKST)

//interrupt
#define csp_led_int_enable(LEDx, eInt)	((LEDx) -> IMCR |= eInt)
#define csp_led_int_disable(LEDx, eInt)	((LEDx) -> IMCR &= ~eInt)
#define csp_led_clr_int(LEDx, eInt)		((LEDx) -> ICR  =  eInt)
#define csp_led_get_misr(LEDx)			((LEDx) -> MISR)

#endif