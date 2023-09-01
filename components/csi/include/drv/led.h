/***********************************************************************//** 
 * \file  led.h
 * \brief  LED  
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-12-8  <td>V0.0  <td>WNN   <td>initial
 * <tr><td> 2023-3-21  <td>V0.1  <td>WCH   <td>modified
 * <tr><td> 2023-9-1   <td>V0.2  <td>WCH   <td>modified
 * </table>
 * *********************************************************************
*/

#ifndef _LED_H_
#define _LED_H_

#include "csp.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	LED_BLK_ON,
	LED_BLK_OFF
}csi_led_blk_e;

typedef enum {
	LED_100 = 0,
	LED_87,
	LED_75,
	LED_62,
	LED_50,
	LED_37,
	LED_25,
	LED_12
}csi_led_brt_e;

typedef enum {
	LED_PCLK_DIV4 = 0,
	LED_PCLK_DIV8,
	LED_PCLK_DIV16,
	LED_PCLK_DIV32,
	LED_PCLK_DIV64,
	LED_PCLK_DIV128,
	LED_PCLK_DIV256,
}csi_led_ledclk_e;

typedef enum {
	LED_INTSRC_NONE = 0,
	LED_INTSRC_ICEND = 0x1<<0,
	LED_INTSRC_IPEND = 0x1<<1,
}csi_led_intsrc_e;

typedef struct csi_led_config {
	uint8_t 	byClkDiv;			//clk configure
	uint8_t 	byBrt;			//brightness configure
	uint16_t 	hwComMask;		//COM enable
	uint16_t 	hwOnTime;		//scanning timing: COM on cycles(range:56~2096).Tcom = byOnTime * Tledclk, needs to be a multiple of 8, otherwise the timing will NOT be accurate
	uint16_t 	hwBreakTime;	//scanning timing: cycles between COMs(range:14~524).Tbreak = byBreakTime * Tledclk.
}csi_led_config_t;


 /** \brief initialize uart parameter structure
 * 
 *  \param[in] ptLedBase: pointer of led register structure
 *  \param[in] ptLedCfg: pointer of led parameter config structure
 * 			   - byClkDiv: LED Clock divider
 * 			   - byBrt: LED brightness control
 * 			   - hwComMask: COM port enable mask
 * 			   - hwOnTime: scanning timing: COM on cycles(range:56~2096).Tcom = byOnTime * Tledclk, needs to be a multiple of 8, otherwise the timing will NOT be accurate
 * 			   - hwBreakTime: scanning timing: cycles between COMs(range:14~524).Tbreak = byBreakTime * Tledclk.
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_led_init(csp_led_t *ptLedBase, csi_led_config_t *tLedCfg);

/** \brief   led bright control
 * 
 *  \param[in] ptLedBase: pointer of LED register structure
 *  \param[in] eBrt:	LED bright control \ref csi_led_brt_e
 *  \return  none  
 */
void csi_led_set_bright(csp_led_t *ptLedBase, csi_led_brt_e eBrt);

/** \brief LED interrupt enable control
 * 
 *  \param[in] ptLedBase: pointer of bt register structure
 *  \param[in] eIntSrc: led interrupt source \ref csi_led_intsrc_e
 *  \return none
 */
void csi_led_int_enable(csp_led_t *ptLedBase, csi_led_intsrc_e eIntSrc);

/** \brief LED interrupt disable control
 * 
 *  \param[in] ptLedBase: pointer of bt register structure
 *  \param[in] eIntSrc: led interrupt source \ref csi_led_intsrc_e
 *  \return none
 */ 
void csi_led_int_disable(csp_led_t *ptLedBase, csi_led_intsrc_e eIntSrc);
 
/**
  \brief   	   write led data
  \param[in]   ptLedBase    pointer of LED register structure
  \param[in]   byCom		to select which com
  \param[in]   byData		SEG data for the specific com
  \return  	   None
*/
void csi_led_write_data(csp_led_t *ptLedBase, uint8_t byCom, uint8_t byData);

/** \brief   set led blink pattern
 * 
 * \param[in] ptLedBase: pointer of LED register structure
 * \param[in] hwOnMsk: on pattern
 * \return  None
 */
void csi_led_set_blink_pattern(csp_led_t *ptLedBase, uint16_t hwOnMsk);

/** \brief  led blink control
 * 
 * \param[in] ptLedBase: pointer of LED register structure
 * \param[in] eLedBlk: blink on or blink off
 * \param[in] hwOnMsk: on pattern
 * \return  None
 */
void csi_led_blink_control(csp_led_t *ptLedBase, csi_led_blk_e eLedBlk, uint16_t hwOnMsk);

/**
  \brief   	   led start
  \param[in]   ptLedBase    pointer of LED register structure
  \return  	   None
*/
void csi_led_light_on(csp_led_t *ptLedBase);

/**
  \brief   	   led stop
  \param[in]   ptLedBase    pointer of LED register structure
  \return  	   None
*/
void csi_led_light_off(csp_led_t *ptLedBase);



#ifdef __cplusplus
}
#endif

#endif /* _LED_H_ */