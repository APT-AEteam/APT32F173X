/***********************************************************************//** 
 * \file  reliability.c
 * \brief  reliability driver, including LVR/LVD, RSR, EMCM and memory check
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-27 <td>V2.0 <td>WNN    <td>initial
 * <tr><td> 2023-9-22 <td>V0.1  <td>GQQ   <td>fix bug,code normalization
 * </table>
 * *********************************************************************
*/

#include "csp.h"
#include "reliability.h"

/// ************************************************************************
///						for LVD module
///*************************************************************************
/** \brief lvd int enable  
 * 
 *  \param[in] ePol: lvd falling/rising/both 
 *  \param[in] eLvl: lvd level
 *  \return none
 */
void csi_lvd_int_enable(csi_lvd_pol_e ePol, csi_lvd_level_e eLvl)
{
	
	csp_lvd_set_level(SYSCON, (lvd_level_e)eLvl);	
	csp_lvd_set_int_pol(SYSCON, (lvdint_pol_e)ePol);
	
	csp_syscon_clr_isr(SYSCON,LVD_INT);  //clear int before enable 
	csp_syscon_int_enable(SYSCON, LVD_INT);			
	csp_lvd_lvr_enable(SYSCON);						
}

/** \brief lvd  disable  
 * 
 *  \return  none
 */
void csi_lvd_disable(void)
{
	csp_lvd_lvr_disable(SYSCON);
}

/** \brief lvd  flag status 
 * 
 *  \return flag
 */
uint32_t csi_lvd_flag(void)
{
	return csp_lvd_get_flag(SYSCON);
}

/** \brief Enable LVR
 * 
 *  \param[in] eLvl LVR level
 *  \return error code \ref csi_error_t
 */
void csi_lvr_enable(csi_lvr_level_e eLvl)
{
	
	csp_lvr_set_level(SYSCON, (lvr_level_e)eLvl);
	csp_lvr_rst_enable(SYSCON);	
	csp_lvd_lvr_enable(SYSCON);
}

/** \brief Disable LVR
 * 
 *  \return none
 */
void csi_lvr_disable(void)
{
	csp_lvd_lvr_disable(SYSCON);
}

/** \brief Get lvd level 
 *  \pasram[in] none
 *  \return lvd level
 */
uint32_t csi_get_lvdlevel(void)
{		
	uint32_t ret = 24;
	switch ((csp_lvd_get_intlvl(SYSCON)) >> 8)
	{
		case (0):
			ret = 24;
			break;
		case (1):
			ret = 21;
			break;
		case (2):
			ret = 27;
			break;
		case (3):
			ret = 30;
			break;
		case (4):
			ret = 33;
			break;
		case (5):
			ret = 36;
			break;
		case (6):
			ret = 39;
			break;
		case (7):
			ret = 10;
			break;
		default :
			break;	
	}
	return ret;
	
}

/** \brief Get lvr level 
 * 
 *  \param[in] none
 *  \return lvd level
 */
uint32_t csi_get_lvrlevel(void)
{
	uint32_t ret = 19;
	switch (csp_lvr_get_lvl(SYSCON) >> 12)
	{
		case (0):
			ret = 19;
			break;
		case (1):
			ret = 22;
			break;
		case (2):
			ret = 25;
			break;
		case (3):
			ret = 28;
			break;
		case (4):
			ret = 31;
			break;
		case (5):
			ret = 34;
			break;
		case (6):
			ret = 37;
			break;
		case (7):
			ret = 40;
			break;
		default :
			break;	
	}
	return ret;
}

/// ************************************************************************
///						for Reset Source Check
///*************************************************************************

/** \brief get chip reset reason
 * 
 *  \param[in] none
 *  \return RSR REG val
 */
uint16_t csi_get_rst_reason(void)
{
	return csp_rst_rd_st(SYSCON);
}
/** \brief clr chip reset reason
 * 
 *  \param[in] hwRstSrc: reset reason
 *  \return none
 */
void csi_clr_rst_reason(uint16_t hwRstSrc)
{
	csp_rst_clr_rsr(SYSCON, hwRstSrc);
}

/** \brief chip software reset
 * 
 *  \param[in] none
 *  \return none
 */
void csi_sys_sw_rst(void)
{
	csp_sw_rst(SYSCON, SYS_SWRST);
}
/// ************************************************************************
///						for user reg operate 
///*************************************************************************
/** \brief user reg write 
 * 
 *  \param[in] eUreg: UREG0~2
 *  \param[in] wValue: value of write
 *  \return error code
 */
csi_error_t csi_ureg_write(csi_user_reg_e eUreg, uint32_t wValue)
{
	
	if(eUreg > USER_REG2)
		return CSI_ERROR;
	else
		csp_set_ureg(SYSCON, eUreg, wValue);
		
	return CSI_OK;
}
/** \brief user reg read
 * 
 *  \param[in] eUreg: UREG0~2
 *  \return value of read
 */
uint32_t csi_ureg_read(csi_user_reg_e eUreg)
{
	if(eUreg > USER_REG2)
		return 0;
	else
		return csp_get_ureg(SYSCON, eUreg);
}
/// ************************************************************************
///						for sram check error
///*************************************************************************
/** \brief set sramcheck fail time.
 * 
 *  \param[in] wVal: times
 *  \return none
 */
void csi_sramcheck_set_times(uint16_t hwVal)
{
	csp_sramcheck_set_times(SYSCON, hwVal);
}

/** \brief set chip to reset when sramcheck fail times > preset value
 * 
 *  \param[in] none
 *  \return none
*/
void csi_sramcheck_rst(void)
{
	csp_sramcheck_rst(SYSCON);
	csp_sramcheck_enable(SYSCON);
}

/** \brief claim INT when sramcheck fail times > preset value
 * 
 *  \param[in]  none
 *  \return  none
 */
void csi_sramcheck_int(void)
{
	csp_syscon_clr_isr(SYSCON,RAM_INT_ERR);
	csp_sramcheck_int_enable(SYSCON);
//	csi_vic_enable_irq(SYSCON_IRQ_NUM);
	csp_sramcheck_enable(SYSCON);
}

/** \brief  disable sramcheck funtion
 * 
 *  \param[in]  none
 *  \return  none
 */
void csi_sramcheck_disable(void)
{
	csp_sramcheck_disable(SYSCON);
}

/// ************************************************************************
///						for flash check error
///*************************************************************************
/** \brief set flashcheck fail time.
 * 
 *  \param[in] wVal: times
 *  \return none
 */
void csi_flashcheck_set_times(uint32_t wVal)
{	
	csp_flashcheck_set_times(SYSCON, wVal);
}

/** \brief set chip to reset when flashcheck times > preset value
 * 
 *  \param[in]  none
 *  \return  none
 */
void csi_flashcheck_rst(void)
{
	csp_flashcheck_enable(SYSCON);
}

/** \brief disable flashcheck funtion
 * 
 *  \param[in]  none
 *  \return  none
 */
void csi_flashcheck_disable(void)
{
	csp_flashcheck_disable(SYSCON);
}

/// ************************************************************************
///						for EM clock monitor
///*************************************************************************

/** \brief claim INT and switch sysclk to IMOSC when EMOSC failure detected
 * 
 *  \param[in]  none
 *  \return  none
 */
void csi_emcm_2_imosc_int(void)
{

	csi_imosc_enable(0);
	csp_emcm_enable(SYSCON);
	csp_emcm_rst_disable(SYSCON);
	
	csp_syscon_clr_isr(SYSCON, EMFAIL_INT);
	csp_syscon_int_enable(SYSCON, EMFAIL_INT);
//	csi_vic_enable_irq(SYSCON_IRQ_NUM);
}

/** \brief rest chip when EMOSC failure detected
 * 
 *  \param[in]  none
 *  \return  none
 */
void csi_emcm_rst(void)
{	
	csp_emcm_enable(SYSCON);
	csp_emcm_rst_enable(SYSCON);
}

/** \brief disable EMOSC monitor funtion
 * 
 *  \param[in]  none
 *  \return  none
 */
void csi_emcm_disable(void)
{
	csp_emcm_disable(SYSCON);
	
}

/// ************************************************************************
///						for ES clock monitor
///*************************************************************************

/** \brief claim INT and switch sysclk to IMOSC when ESOSC failure detected
 * 
 *  \param[in]  none
 *  \return  none
 */
void csi_escm_2_imosc_int(void)
{

	csi_imosc_enable(0);  //0 - 5MHz   1 - 4MHz  2 - 2MHz  3 - 131KHz
	csp_escm_enable(SYSCON);
	csp_escm_rst_disable(SYSCON);
	
	csp_syscon_clr_isr(SYSCON, ESFAIL_INT);
	csp_syscon_int_enable(SYSCON, ESFAIL_INT);
//	csi_vic_enable_irq(SYSCON_IRQ_NUM);
}

/** \brief rest chip when EMOSC failure detected
 * 
 *  \param[in]  none
 *  \return  none
 */
void csi_escm_rst(void)
{	
	csp_escm_enable(SYSCON);
	csp_escm_rst_enable(SYSCON);
}

/** \brief disable EMOSC monitor funtion
 * 
 *  \param[in]  none
 *  \return  none
 */
void csi_escm_disable(void)
{
	csp_escm_disable(SYSCON);
}

/// *********************************************************************************
///  For SWD Lock function: To switch SWD pins to other AF, should unlock SWD first 
///**********************************************************************************

/** \brief SWD Lock
 * 
 *  \param[in]  none
 *  \return  none
 */
void csi_swd_lock(void)
{
	csp_set_swd_lock(SYSCON);
}

/** \brief SWD Unlock
 * 
 *  \param[in]  none
 *  \return  none
 */
void csi_swd_unlock(void)
{
	csp_set_swd_unlock(SYSCON);
}

/** \brief cmos auto trim
 * 
 *  
 * 
 *  \param none
 *  \return csi_error_t.
 */
csi_error_t csi_cmos_autotrim(void)
{
	uint32_t wTrimValue;
	
	SYSCON->TRIMUREG = CMRTRIM_CTL_MSK;
	while((SYSCON->TRIMUREG & CMRTRIM_CTL_MSK) == CMRTRIM_CTL_MSK);
	wTrimValue = SYSCON->TRIMUREG & CMRTRIM_VULUE_MSK;
	AUTOTRIM_KEY_UREG = AUTOTRIM_KEY;
	__ISB();	
	AUTOTRIM_TRIM_UREG = (AUTOTRIM_TRIM_UREG &0xff00ffff)|(wTrimValue<<16);

	SYSCON->TRIMUREG = CMRTRIML_CTL_MSK;
	while((SYSCON->TRIMUREG & CMRTRIML_CTL_MSK) == CMRTRIML_CTL_MSK);
	wTrimValue = ((SYSCON->TRIMUREG & CMRTRIML_VULUE_MSK)>> CMRTRIML_VULUE_POS);
	AUTOTRIM_KEY_UREG = AUTOTRIM_KEY; 
	__ISB();
	AUTOTRIM_TRIM_UREG = (AUTOTRIM_TRIM_UREG &0x00ffffff)|(wTrimValue<<24);	
	
	return CSI_OK;
}

/** \brief cqcr enable
 * 
 *  \param none
 *  \return none
 */
void csi_cqcr_enable(void)
{
	csp_cqcr_enable(SYSCON);
}

/** \brief cqcr disable
 * 
 *  \param none
 *  \return none
 */
void csi_cqcr_disable(void)
{
	csp_cqcr_disable(SYSCON);
}

/** \brief cqcr ref sel
 * 
 *  \param[in] eRefSel csi_cqcr_refsel_e
 *  \param[in] eSrcSel csi_cqcr_srcsel_e
 *  \param[in] wVal 
 *  \return none
 */
void csi_set_cqcr(csi_cqcr_refsel_e eRefSel,csi_cqcr_srcsel_e eSrcSel,uint32_t wVal)
{
	csp_cqcr_set_ref_sel(SYSCON,(cqcr_refsel_e)eRefSel);
	csp_cqcr_set_src_sel(SYSCON,(cqcr_srcsel_e)eSrcSel);
	csp_cqcr_set_value(SYSCON,wVal);
	csp_cqcr_enable(SYSCON);
}

/** \brief get cqcr 
 * 
 *  \return cqcr value
 */
uint32_t csi_get_cqsr(void)
{
	while(csp_cqcr_get_status(SYSCON));
	return csp_get_cqsr(SYSCON);
}

/** \brief sram set
 * 
 *  \param[in] eSramBlk csi_sram1_func_e    SRAM_24KRAM0_8KRAM1_CTRL or SRAM_16KRAM0_16KRAM1_CTRL
 *  \param[in] eSram1Func csi_sram1_func_e  SRAM1_DSARM_CTRL or SRAM1_ISRAM_CTRL
 *  \return none
 */
void csi_set_sram(csi_sram_blk_e eSramBlk,csi_sram1_func_e eSram1Func)
{
	csp_sram_blk_ctrl(SYSCON, (sram_blk_e)eSramBlk);	
	csp_sram1_func_ctrl(SYSCON, (sram1_func_e)eSram1Func);
}