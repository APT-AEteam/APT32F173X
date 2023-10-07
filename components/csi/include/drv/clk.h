
/***********************************************************************//** 
 * \file  clk.h
 * \brief  head  file of clk.c
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-10 <td>V0.0  <td>WNN   <td>initial
 * <tr><td> 2021-1-8 <td>V0.1  <td>WNN   <td> add declaration of csi_xxosc_enable()
 * </table>
 * *********************************************************************
*/

#ifndef _DRV_CLK_H_
#define _DRV_CLK_H_


#include "drv/common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	EMFLT_5NS = 0,
	EMFLT_10NS,
	EMFLT_15NS,
	EMFLT_20NS,
}csi_em_fltsel_e;

typedef struct {
	uint32_t wRegBase;
    uint32_t wModule;
} csi_clkmap_t;

void csi_clk_enable(uint32_t *pIpBase);
void csi_clk_disable(uint32_t *pIpBase);

/** \brief emosc filter enable
 * 
 * 
 *  \param[in] eFltSel: filter select \ref csi_em_fltsel_e
 *  \return none
 */
void csi_emosc_flt_enable(csi_em_fltsel_e eFltSel);

/** \brief emosc enable
 * 
 *  enable external main oscillator in SYSCON
 * 
 *  \param none
 *  \return csi_error_t
 */
csi_error_t csi_emosc_enable(uint32_t wFreq);

/** \brief emosc disable
 * 
 *  disable external main oscillator in SYSCON
 * 
 *  \param none
 *  \return csi_error_t.
 */
csi_error_t csi_emosc_disable(void);

/** \brief esosc enable
 * 
 *  enable external sub oscillator in SYSCON
 * 
 *  \param[in] none
 *  \return csi_error_t
 */
csi_error_t csi_esosc_enable(void);

/** \brief esosc disable
 * 
 *  disable external sub oscillator in SYSCON
 * 
 *  \param[in] none
 *  \return none
 */
void csi_esosc_disable(void);

/** \brief imosc enable
 * 
 *  enable internal main oscillator in SYSCON
 * 
 *  \param byFre. 	0 - 5MHz
 * 					1 - 4MHz
 * 					2 - 2MHz
					3 - 131KHz
 *  \return csi_error_t
 */
csi_error_t csi_imosc_enable(uint8_t byFre);

/** \brief imosc disable
 * 
 *  disable internal main oscillator in SYSCON
 * 
 *  \param[in] none
 *  \return none.
 */
void csi_imosc_disable(void);

/** \brief hfosc enable
 * 
 *  enable high frequency oscillator in SYSCON
 *  * 
 *  \param byFre. 	0 - 48MHz
 * 					1 - 24MHz
 * 					2 - 12MHz
 *					3 - 6MHz
 *  \return csi_error_t.
 */
csi_error_t csi_hfosc_enable(uint8_t byFre);

 /** \brief hfosc disable
 * 
 *  disable high frequency oscillator in SYSCON
 * 
 *  \param none
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_hfosc_disable(void);

/** \brief isosc enable
 * 
 *  enable internal sub oscillator in SYSCON
 * 
 *  \param none
 *  \return csi_error_t
 */
csi_error_t csi_isosc_enable(void);

/** \brief isosc disable
 * 
 *  disable internal sub oscillator in SYSCON
 * 
 *  \param none
 *  \return csi_error_t.
 */
csi_error_t csi_isosc_disable(void);

/** \brief pll enable
 * 
 *  enable internal sub pll in SYSCON
 * 
 *  \param none
 *  \return csi_error_t
 */
csi_error_t csi_pll_enable(void);

/** \brief pll disable
 * 
 *  disable internal sub pll in SYSCON
 * 
 *  \param none
 *  \return none.
 */
void csi_pll_disable(void);

#ifdef __cplusplus
}
#endif

#endif /* _CSI_CLK_H_ */

