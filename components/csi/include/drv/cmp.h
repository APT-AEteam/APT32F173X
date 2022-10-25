/***********************************************************************//** 
 * \file  cmp.h
 * \brief header file for cmp driver
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-10-14 <td>V0.0  <td>YYM   <td>modify
 * </table>
 * *********************************************************************
*/

#ifndef _DRV_CMP_H_
#define _DRV_CMP_H_

#include <drv/common.h>
#include "csp.h"

#ifdef __cplusplus
extern "C" {
#endif

#define	CMP_CLK_POS			(0)
#define	CMP_CLK_MSK			(0x01ul << CMP_CLK_POS)

#define CMP_SOFTRESET       (0X01<<7)

#define CMP_INT_MSK         (0X01)


#define	CMP_ETCB_POS			(7)
#define	CMP_ETCB_MSK			(0x01ul << CMP_ETCB_POS)


typedef enum
{	
	PHYST_0mv			=	(0X00<<1),
	PHYST_5mv			=	(0X01<<1),
	PHYST_10mv			=	(0X02<<1),
	PHYST_15mv			=	(0X03<<1),
	PHYST_20mv			=	(0X04<<1),
	PHYST_25mv			=	(0X05<<1),
	PHYST_40mv			=	(0X06<<1),
	PHYST_60mv			=	(0X07<<1)
}cmp_cr_phystsel_e;

typedef enum
{	
	NHYST_0mv			=	(0X00<<18),
	NHYST_5mv			=	(0X01<<18),
	NHYST_10mv			=	(0X02<<18),
	NHYST_15mv			=	(0X03<<18),
	NHYST_20mv			=	(0X04<<18),
	NHYST_25mv			=	(0X05<<18),
	NHYST_40mv			=	(0X06<<18),
	NHYST_60mv			=	(0X07<<18)
}cmp_cr_nhystsel_e;


typedef enum
{
	POLARITY_0			=	(0X00<<6),
	POLARITY_1			=	(0X01<<6),
}cmp_cr_polarity_e;

typedef enum
{
	EVE_SEL_FALL		=	(0X00<<8),
	EVE_SEL_RISE		=	(0X01<<8),
	EVE_SEL_FALL_RISE	=	(0X02<<8),
}cmp_cr_eve_sel_e;

typedef enum
{
	DIS1_FLTEN			=	(0X00<<10),
	EN1_FLTEN			=	(0X01<<10),
}cmp_cr_dflt1en_e;

typedef enum
{
	DIS2_FLTEN			=	(0X00<<11),
	EN2_FLTEN			=	(0X01<<11),
}cmp_cr_dflt2en_e;

typedef enum
{
	CPOS_0				=	(0X00<<23),
	CPOS_1				=	(0X01<<23)
}cmp_cr_cpos_e;

typedef enum
{
	DEPTH_N_8        =   0,
	DEPTH_N_16       =   1,
	DEPTH_N_32       =   2,
	DEPTH_N_64       =   3,
	DEPTH_N_128      =   4,
	DEPTH_N_256      =   5,
	DEPTH_N_512      =   6,
	DEPTH_N_1024     =   7
}cmp_dfcr_depth_e;


typedef enum
{
	LTEN_DISENABLE         = (0x00<<12),
	LTEN_ENABLE            = (0x01<<12)
	
}cmp_wflten_e;


typedef enum
{
	ALIGN_DISALLOW_LAST   = (0x00<<13),
	ALIGN_ALLOW_LAST      = (0x01<<13)
	
}cmp_wfalign_e;


typedef enum
{
	OSET_NONE0     =  (0x00<<14),
	OSET_HIGH      =  (0x01<<14),
	OSET_LOW       =  (0x02<<14),
	OSET_NONE1     =  (0x03<<14)
	
}cmp_wfoset_e;


/** \brief cmp reg clk enable
 * 
 *  \param[in] ptCmpBase: pointer of CMP reg structure.
 *  \param[in] bEnable: enable/disable cmp clk
 *  \return none
 */ 
void csi_cmp_clk_enable(csp_cmp_t *ptCmpBase,bool bEnable);

/** \brief cmp reg reset software
 * 
 *  \param[in] ptCmpBase: pointer of CMP reg structure.
 *  \return none
 */ 
void csi_cmp_software_reset(csp_cmp_t *ptCmpBase);

/** \brief cmp enable
 * 
 *  \param[in] ptCmpBase: pointer of CMP reg structure.
 *  \return none
 */
void csi_cmp_enable(csp_cmp_t *ptCmpBase);

/** \brief cmp disable
 * 
 *  \param[in] ptCmpBase: pointer of CMP reg structure.
 *  \return none
 */
void csi_cmp_disable(csp_cmp_t *ptCmpBase);

/** \brief cmp out read
 * 
 *  \param[in] ptCmpBase: pointer of CMP reg structure.
 *  \return out value
 */ 
uint8_t csi_cmp_out_read(csp_cmp_t *ptCmpBase);

/** \brief cmp cr reg config
 * 
 *  \param[in] ptCmpBase: pointer of CMP reg structure.
 *  \param[in] ePhystsel: phystsel
 *  \param[in] eNhystsel: nhystsel
 *  \param[in] ePolarity: Polarity
 *  \param[in] eEveSel:   Eve Sel
 *  \param[in] eDflt1en:  Dflt1 enable
 *  \param[in] eDflt2en:  Dflt2 enable
 *  \param[in] eCpos:     cmp out sel
 *  \return none
 */
void csi_cmp_cr_config(csp_cmp_t *ptCmpBase , cmp_cr_phystsel_e ePhystsel , cmp_cr_nhystsel_e eNhystsel ,cmp_cr_polarity_e ePolarity , 
						cmp_cr_eve_sel_e eEveSel , cmp_cr_dflt1en_e eDflt1en, cmp_cr_dflt2en_e eDflt2en , cmp_cr_cpos_e eCpos);
						
/** \brief cmp cr reg of wfcr set
 * 
 *  \param[in] ptCmpBase: pointer of CMP reg structure.
 *  \param[in] eWlten:  wfcr enable 
 *  \param[in] eWfalign: wfcr align 
 *  \param[in] eWfoset: wfcr oset
 *  \return none
 */
void csi_cmp_cr_wf_set(csp_cmp_t *ptCmpBase ,cmp_wflten_e eWlten, cmp_wfalign_e eWfalign , cmp_wfoset_e eWfoset);

/** \brief cmp dfcr1 reg config
 * 
 *  \param[in] ptCmpBase: pointer of CMP reg structure.
 *  \param[in] eDepth: depth
 *  \param[in] byDivn: divn
 *  \param[in] byDivm: divm
 *  \return none
 */
void csi_cmp_dfcr1_config(csp_cmp_t *ptCmpBase , cmp_dfcr_depth_e eDepth, uint8_t byDivn,uint8_t byDivm);

/** \brief cmp dfcr2 reg config
 * 
 *  \param[in] ptCmpBase: pointer of CMP reg structure.
 *  \param[in] eDepth: depth
 *  \param[in] byDivn: divn
 *  \param[in] byDivm: divm
 *  \return none
 */
void csi_cmp_dfcr2_config(csp_cmp_t *ptCmpBase , cmp_dfcr_depth_e eDepth, uint8_t byDivn,uint8_t byDivm);

/** \brief cmp wfcr reg config
 * 
 *  \param[in] ptCmpBase: pointer of CMP reg structure.
 *  \param[in] dwWcnt: wcnt
 *  \param[in] byClkDiv: clk div
 *  \param[in] byDcnt: delay cnt
 *  \return none
 */
void csi_cmp_wfcr_config(csp_cmp_t *ptCmpBase,uint16_t dwWcnt,uint8_t byClkDiv,uint8_t byDcnt);

/** \brief cmp inpcr reg set
 * 
 *  \param[in] ptCmpBase: pointer of CMP reg structure.
 *  \param[in] byNsel: n- sel
 *  \param[in] byPsel: p+ sel
 *  \return none
 */
void csi_cmp_inpcr_config(csp_cmp_t *ptCmpBase,uint8_t byNsel,uint8_t byPsel);

/** \brief cmp int enable
 * 
 *  \param[in] ptCmpBase: pointer of CMP reg structure.
 *  \param[in] bEnable: enable/disable cmp int
 *  \return none
 */
void csi_cmp_int_enable(csp_cmp_t *ptCmpBase,bool bEnable);

/** \brief cmp int clear
 * 
 *  \param[in] ptCmpBase: pointer of CMP reg structure.
 *  \return none
 */
void csi_cmp_int_clear(csp_cmp_t *ptCmpBase);

/** \brief cmp evtrg enable
 * 
 *  \param[in] ptCmpBase: pointer of CMP reg structure.
 *  \param[in] bEnable: enable/disable etcb out
 *  \return none
 */
void csi_cmp_set_evtrg(csp_cmp_t *ptCmpBase, bool bEnable);


void csi_cmp_speed(csp_cmp_t *ptCmpBase);
#ifdef __cplusplus
}
#endif

#endif /* _DRV_TIMER_H_ */