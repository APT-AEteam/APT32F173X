/***********************************************************************//** 
 * \file  led.c
 * \brief  CRC driver
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-12-8 <td>V0.0  <td>WNN   <td>initial
 * <tr><td> 2023-3-21 <td>V0.1  <td>WCH  <td>modified
 * <tr><td> 2023-9-1  <td>V0.2  <td>WCH  <td>modified
 * </table>
 * *********************************************************************
*/
#include <drv/led.h>

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
csi_error_t csi_led_init(csp_led_t *ptLedBase, csi_led_config_t *ptLedCfg) 
{
	csi_clk_enable((uint32_t *)ptLedBase);	
	csi_led_set_bright(ptLedBase, ptLedCfg->byBrt);
	csp_led_set_clk(ptLedBase, ptLedCfg->byClkDiv);
	csp_led_set_commask(ptLedBase, (ptLedCfg->hwComMask & 0x3ff));
	if(ptLedCfg->hwOnTime > 2096 || ptLedCfg->hwBreakTime > 524)
		return CSI_ERROR;
	csp_led_set_dcomcnt(ptLedBase, (ptLedCfg->hwOnTime/8 - 7));
	csp_led_set_novcnt(ptLedBase, (ptLedCfg->hwBreakTime/2 -7));
	
	csi_irq_enable((uint32_t *)ptLedBase);
	
	return CSI_OK;
}

/** \brief   led bright control
 * 
 *  \param[in] ptLedBase: pointer of LED register structure
 *  \param[in] eBrt:	LED bright control \ref csi_led_brt_e
 *  \return  none  
 */
void csi_led_set_bright(csp_led_t *ptLedBase, csi_led_brt_e eBrt) 
{
	csp_led_set_brt(ptLedBase,(csp_led_brt_e)eBrt);
}

/** \brief LED interrupt enable control
 * 
 *  \param[in] ptLedBase: pointer of bt register structure
 *  \param[in] eIntSrc: led interrupt source \ref csi_led_intsrc_e
 *  \return none
 */ 
void csi_led_int_enable(csp_led_t *ptLedBase, csi_led_intsrc_e eIntSrc)
{
	csp_led_int_enable(ptLedBase, (csp_led_int_e)eIntSrc);
}

/** \brief LED interrupt disable control
 * 
 *  \param[in] ptLedBase: pointer of bt register structure
 *  \param[in] eIntSrc: led interrupt source \ref csi_led_intsrc_e
 *  \return none
 */ 
void csi_led_int_disable(csp_led_t *ptLedBase, csi_led_intsrc_e eIntSrc)
{
	csp_led_int_disable(ptLedBase, (csp_led_int_e)eIntSrc);
}

/** \brief   write led data
 * 
 *  \param[in] ptLedBase: pointer of LED register structure
 *  \param[in] byCom: to select which com
 *  \param[in] byData: SEG data for the specific com
 *  \return None
 */
void csi_led_write_data(csp_led_t *ptLedBase, uint8_t byCom, uint8_t byData)
{
	csp_led_set_setdata(ptLedBase, byCom, byData);
}

/** \brief   set led blink pattern
 * 
 * \param[in] ptLedBase: pointer of LED register structure
 * \param[in] hwOnMsk: on pattern
 * \return  None
 */
void csi_led_set_blink_pattern(csp_led_t *ptLedBase, uint16_t hwOnMsk)
{
	csp_led_com_on(ptLedBase, (hwOnMsk&LED_BLK_MSK));
	csp_led_com_off(ptLedBase, ~(hwOnMsk&LED_BLK_MSK));
}

/** \brief  led blink control
 * 
 * \param[in] ptLedBase: pointer of LED register structure
 * \param[in] eLedBlk: blink on or blink off \ref csi_led_intsrc_e
 * \param[in] hwOnMsk: on pattern
 * \return  None
 */
void csi_led_blink_control(csp_led_t *ptLedBase, csi_led_blk_e eLedBlk, uint16_t hwOnMsk)
{
	if(eLedBlk == LED_BLK_ON)
		csp_led_com_on(ptLedBase, (hwOnMsk & LED_BLK_MSK));
	else if(eLedBlk == LED_BLK_OFF)
		csp_led_com_off(ptLedBase, (hwOnMsk & LED_BLK_MSK));
}
/** \brief   led scan start
 * 
 * \param[in] ptLedBase: pointer of LED register structure
 * \return None
 */
void csi_led_light_on(csp_led_t *ptLedBase)
{
	csp_led_light_on(ptLedBase);
}
/** \brief led scan stop
 * 
 *  \param[in] ptLedBase: pointer of LED register structure
 *  \return None
 */
void csi_led_light_off(csp_led_t *ptLedBase)
{	
	csp_led_light_off(ptLedBase);
}