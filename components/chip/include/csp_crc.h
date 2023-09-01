/***********************************************************************//** 
 * \file  csp_crc.h
 * \brief  CRC description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-25 <td>V0.0  <td>WNN   <td>initial
 * </table>
 * *********************************************************************
*/


#ifndef _CSP_CRC_H
#define _CSP_CRC_H

#include "csi_core.h"
#include "stdint.h"
#include "stdbool.h"
#include "csp_common.h"

typedef struct
{
    __IM   uint32_t RSVD;
	__IOM  uint32_t CEDR;            /**< Clock Enable/Disable Register	     */
   __OM  uint32_t SRR;             /**< Software Reset Register	     */
   __IOM uint32_t CR;              /**< Control Register	             */
   __IOM uint32_t SEED;            /**< Seed Value Register	             */
   __OM  uint32_t DATAIN;          /**< Data in Value Register	             */
   __IM  uint32_t DATAOUT;         /**< Data out Value Register	     */
} csp_crc_t;

/******************************************************************************
************************* CRC Registers Definition ****************************
******************************************************************************/



///CEDR : Clock Enable/Disable Register
#define CRC_CLKEN          (0x01ul << 0)         /**< IP Clock             */


///SRR : Software Reset Register
#define CRC_SWRST          (0x01ul << 31)         /**< Software Reset         */


///CR : CRC Control Register
#define CRC_XORIN_POS	(0ul)
#define CRC_XORIN_MSK	(0x1)
#define CRC_XOROUT_POS	(1ul)
#define CRC_XOROUT_MSK	(0x1 << CRC_XOROUT_POS)
#define CRC_REFIN_POS	(2ul)
#define CRC_REFIN_MSK	(0x1 << CRC_REFIN_POS)
#define CRC_REFOUT_POS	(3ul)
#define CRC_REFOUT_MSK	(0x1 << CRC_REFOUT_POS)
#define CRC_POLY_POS	(4ul)
#define CRC_POLY_MSK	(0x3 << CRC_POLY_POS)

#define csp_crc_clk_enable(ptCrcBase)				(ptCrcBase -> CEDR = (ptCrcBase -> CEDR & (~CRC_CLKEN)) | 1)

#define csp_crc_clk_disable(ptCrcBase)				(ptCrcBase -> CEDR = (ptCrcBase -> CEDR & (~CRC_CLKEN)) | 0)

#define csp_crc_rst(ptCrcBase)						(ptCrcBase -> SRR = CRC_SWRST)

#define csp_crc_xorin_enable(ptCrcBase)				(ptCrcBase -> CR = (ptCrcBase -> CR & (~CRC_XORIN_MSK))|(1 << CRC_XORIN_POS))

#define csp_crc_xorin_disable(ptCrcBase)			(ptCrcBase -> CR = (ptCrcBase -> CR & (~CRC_XORIN_MSK))|(0 << CRC_XORIN_POS))

#define csp_crc_xorout_enable(ptCrcBase)			(ptCrcBase -> CR = (ptCrcBase -> CR & (~CRC_XOROUT_MSK))|(1 << CRC_XOROUT_POS))

#define csp_crc_xorout_disable(ptCrcBase)			(ptCrcBase -> CR = (ptCrcBase -> CR & (~CRC_XOROUT_MSK))|(0 << CRC_XOROUT_POS))

#define csp_crc_refin_enable(ptCrcBase)				(ptCrcBase -> CR = (ptCrcBase -> CR & (~CRC_REFIN_MSK))|(1 << CRC_REFIN_POS))

#define csp_crc_refin_disable(ptCrcBase)			(ptCrcBase -> CR = (ptCrcBase -> CR & (~CRC_REFIN_MSK))|(0 << CRC_REFIN_POS))

#define csp_crc_refout_enable(ptCrcBase)			(ptCrcBase -> CR = (ptCrcBase -> CR & (~CRC_REFOUT_MSK)) | (1 << CRC_REFOUT_POS))

#define csp_crc_refout_disable(ptCrcBase)			(ptCrcBase -> CR = (ptCrcBase -> CR & (~CRC_REFOUT_MSK)) | (0 << CRC_REFOUT_POS))

#define csp_crc_set_poly(ptCrcBase,byPoly)			(ptCrcBase -> CR = (ptCrcBase -> CR & (~CRC_POLY_MSK))|(byPoly << CRC_POLY_POS))

#define csp_crc_set_seed(ptCrcBase,wSeed)			(ptCrcBase -> SEED = wSeed)

#define csp_crc_get_result(ptCrcBase)				(ptCrcBase->DATAOUT)

#endif /* CSP_CRC_H */




