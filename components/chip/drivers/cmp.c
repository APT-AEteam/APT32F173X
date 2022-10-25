/***********************************************************************//** 
 * \file  cmp.c
 * \brief  csi cmp driver
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-10-14 <td>V0.0  <td>YYM   <td>modify
 * </table>
 * *********************************************************************
*/
#include <sys_clk.h>
#include <drv/lpt.h>
#include <drv/irq.h>
#include <drv/gpio.h>
#include <drv/pin.h>
#include <drv/tick.h>
#include "csp_syscon.h"
#include "csp_cmp.h"
#include "cmp.h"
#include "board_config.h"

/* Private macro-----------------------------------------------------------*/
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/
/** \brief cmp reg clk enable
 * 
 *  \param[in] ptCmpBase: pointer of CMP reg structure.
 *  \param[in] bEnable: enable/disable cmp clk
 *  \return none
 */ 
void csi_cmp_clk_enable(csp_cmp_t *ptCmpBase,bool bEnable)
{
	ptCmpBase->CEDR = (ptCmpBase->CEDR & ~CMP_CLK_MSK) | (bEnable << CMP_CLK_POS);	
}

/** \brief cmp reg reset software
 * 
 *  \param[in] ptCmpBase: pointer of CMP reg structure.
 *  \return none
 */ 
void csi_cmp_software_reset(csp_cmp_t *ptCmpBase)
{
	ptCmpBase->CEDR = CMP_SOFTRESET;

}

/** \brief cmp out read
 * 
 *  \param[in] ptCmpBase: pointer of CMP reg structure.
 *  \return out value
 */ 
uint8_t csi_cmp_out_read(csp_cmp_t *ptCmpBase)
{
	uint32_t wValue;
	wValue=ptCmpBase->CR&0xf000000;
	wValue=wValue>>24;
	return (uint8_t)wValue;
}

/** \brief cmp enable
 * 
 *  \param[in] ptCmpBase: pointer of CMP reg structure.
 *  \return none
 */
void csi_cmp_enable(csp_cmp_t *ptCmpBase)
{
	ptCmpBase->CR|=0x01;;
}

/** \brief cmp disable
 * 
 *  \param[in] ptCmpBase: pointer of CMP reg structure.
 *  \return none
 */
void csi_cmp_disable(csp_cmp_t *ptCmpBase)
{
	ptCmpBase->CR&=0xfffffffe;	
}

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
						cmp_cr_eve_sel_e eEveSel , cmp_cr_dflt1en_e eDflt1en, cmp_cr_dflt2en_e eDflt2en , cmp_cr_cpos_e eCpos)
{
	ptCmpBase->CR&=0xff600031;
	
	ptCmpBase->CR|=ePhystsel|eNhystsel|ePolarity|eEveSel|eDflt1en|eDflt2en|eCpos;
}

void csi_cmp_speed(csp_cmp_t *ptCmpBase)
{
	ptCmpBase->CR|= 0x300000;
}

/** \brief cmp cr reg of wfcr set
 * 
 *  \param[in] ptCmpBase: pointer of CMP reg structure.
 *  \param[in] eWlten:  wfcr enable 
 *  \param[in] eWfalign: wfcr align 
 *  \param[in] eWfoset: wfcr oset
 *  \return none
 */
void csi_cmp_cr_wf_set(csp_cmp_t *ptCmpBase ,cmp_wflten_e eWlten, cmp_wfalign_e eWfalign , cmp_wfoset_e eWfoset)
{
	ptCmpBase->CR&=0xffff0fff;
	ptCmpBase->CR|=eWlten|eWfalign|eWfoset;
}

/** \brief cmp dfcr1 reg config
 * 
 *  \param[in] ptCmpBase: pointer of CMP reg structure.
 *  \param[in] eDepth: depth
 *  \param[in] byDivn: divn
 *  \param[in] byDivm: divm
 *  \return none
 */
void csi_cmp_dfcr1_config(csp_cmp_t *ptCmpBase , cmp_dfcr_depth_e eDepth, uint8_t byDivn,uint8_t byDivm)
{
	ptCmpBase->DFCR1&=0xffff0000;
	ptCmpBase->DFCR1|=eDepth|(byDivn<<3)|(byDivm<<8);
}

/** \brief cmp dfcr2 reg config
 * 
 *  \param[in] ptCmpBase: pointer of CMP reg structure.
 *  \param[in] eDepth: depth
 *  \param[in] byDivn: divn
 *  \param[in] byDivm: divm
 *  \return none
 */
void csi_cmp_dfcr2_config(csp_cmp_t *ptCmpBase , cmp_dfcr_depth_e eDepth, uint8_t byDivn,uint8_t byDivm)
{
	ptCmpBase->DFCR2&=0xffff0000;
	ptCmpBase->DFCR2|=eDepth|(byDivn<<3)|(byDivm<<8);
}

/** \brief cmp wfcr reg config
 * 
 *  \param[in] ptCmpBase: pointer of CMP reg structure.
 *  \param[in] dwWcnt: wcnt
 *  \param[in] byClkDiv: clk div
 *  \param[in] byDcnt: delay cnt
 *  \return none
 */
void csi_cmp_wfcr_config(csp_cmp_t *ptCmpBase,uint16_t dwWcnt,uint8_t byClkDiv,uint8_t byDcnt)
{
	ptCmpBase->WFCR&=0x0;
	ptCmpBase->WFCR|=dwWcnt|(byClkDiv<<10)|(byDcnt<<16);
}

/** \brief cmp inpcr reg set
 * 
 *  \param[in] ptCmpBase: pointer of CMP reg structure.
 *  \param[in] byNsel: n- sel
 *  \param[in] byPsel: p+ sel
 *  \return none
 */
void csi_cmp_inpcr_config(csp_cmp_t *ptCmpBase,uint8_t byNsel,uint8_t byPsel)
{
	ptCmpBase->INPCR&=0x0;
	ptCmpBase->INPCR|=byNsel|(byPsel<<8)|0x10000;
}

/** \brief cmp int enable
 * 
 *  \param[in] ptCmpBase: pointer of CMP reg structure.
 *  \param[in] bEnable: enable/disable cmp int
 *  \return none
 */
void csi_cmp_int_enable(csp_cmp_t *ptCmpBase,bool bEnable)
{
	ptCmpBase->IMCR|= (ptCmpBase->IMCR & ~CMP_INT_MSK) | bEnable;
}

/** \brief cmp int clear
 * 
 *  \param[in] ptCmpBase: pointer of CMP reg structure.
 *  \return none
 */
void csi_cmp_int_clear(csp_cmp_t *ptCmpBase)
{
	ptCmpBase->ICR|= 0x01;
}

/** \brief cmp evtrg enable
 * 
 *  \param[in] ptCmpBase: pointer of CMP reg structure.
 *  \param[in] bEnable: enable/disable etcb out
 *  \return none
 */
void csi_cmp_set_evtrg(csp_cmp_t *ptCmpBase, bool bEnable)
{
	ptCmpBase->CR = (ptCmpBase->CR & ~CMP_ETCB_MSK) | (bEnable << CMP_ETCB_POS);	
	
}
