/***********************************************************************//** 
 * \file  reliability.h
 * \brief  reliability driver head file, including LVR/LVD, RSR, emcm and memory check
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-27 <td>V2.0 <td>WNN    <td>initial
 * </table>
 * *********************************************************************
*/

#ifndef _DRV_LVD_H_
#define _DRV_LVD_H_

#include <drv/common.h>

/// ************************************************************************
///						for LVD module
///*************************************************************************


typedef enum{
	LVD_21 = 0,
	LVD_24,
	LVD_27,
	LVD_30,
	LVD_33,
	LVD_36,
	LVD_39,
	LVDIN_10			//1.0V
}csi_lvd_level_e;

typedef enum {
	LVR_19 = 0,
	LVR_22,
	LVR_25,
	LVR_28,
	LVR_31,
	LVR_34,
	LVR_37,
	LVR_40
}csi_lvr_level_e;

typedef enum {
	LVD_INTF = 1,
	LVD_INTR,
	LVD_INTFR
}csi_lvd_pol_e;

typedef enum{
	CQCR_REFSEL_EM,
	CQCR_REFSEL_ES,	
}csi_cqcr_refsel_e;

typedef enum{
	CQCR_SRCSEL_IM,
	CQCR_SRCSEL_ES,
	CQCR_SRCSEL_IS,
	CQCR_SRCSEL_HF
}csi_cqcr_srcsel_e;


typedef enum {
	SRAM1_DSARM_CTRL = 0,
	SRAM1_ISRAM_CTRL
}csi_sram1_func_e;

typedef enum {
	SRAM_24KRAM0_8KRAM1_CTRL = 0,
	SRAM_16KRAM0_16KRAM1_CTRL
}csi_sram_blk_e;

#define AUTOTRIM_KEY_UREG   (*(unsigned int *)(0x400111FC)) 
#define AUTOTRIM_TRIM_UREG  (*(unsigned int *)(0x40011204))
#define AUTOTRIM_KEY         0x00006996 

/**
  \brief       lvd  int enable  
  \param       ePol      lvd falling/rising/both 
  \param       eLvl      lvd level
  \return      none
*/
void csi_lvd_int_enable(csi_lvd_pol_e ePol, csi_lvd_level_e eLvl);

/**
  \brief       lvd disable  
  \return      none
*/
void csi_lvd_disable(void);

/**
  \brief       lvd  flag status 
  \return      flag
*/
uint32_t csi_lvd_flag(void);

/**
  \brief       Enable LVR
  \param       eLvl LVR level
  \return      error code \ref csi_error_t
*/
void csi_lvr_enable(csi_lvr_level_e eLvl);

/**
  \brief       Disable LVR
  \return      none
*/
void csi_lvr_disable(void);


/**
  \brief       low voltage reset enable 
  \pasram[in]   ptRlblty
  \return      elvl    lvr level
*/
void csi_lvr_enable(csi_lvr_level_e eLvl);


/**
  \brief       Disable LVR
  \return      error code \ref csi_error_t
*/
void csi_lvr_disable(void);

/**
  \brief       Get lvd level 
  \pasram[in]   none
  \return      lvd    lvd level
*/
uint32_t csi_get_lvdlevel(void);

/**
  \brief       Get lvr level 
  \param       none
  \return      lvd    lvd level
*/
uint32_t csi_get_lvrlevel(void);
/// ************************************************************************
///						for Reset Source check
///*************************************************************************

typedef enum{
	RST_SRC_POR			= (0x01ul << 0),
	RST_SRC_LVD			= (0x01ul << 1),
	RST_SRC_EXT			= (0x01ul << 2),
	RST_SRC_IWDT		= (0x01ul << 4),
	RST_SRC_EMCM		= (0x01ul << 6),
	RST_SRC_CPU			= (0x01ul << 7),
	RST_SRC_SW			= (0x01ul << 8),
	RST_SRC_CPUFAULT	= (0x01ul << 9),
	RST_SRC_PLLUNLOCK	= (0x01ul << 10),
	RST_SRC_RAMERR 		= (0x01ul << 11),
	RST_SRC_EFLERR		= (0x01ul << 12),
	RST_SRC_WWDT		= (0x01ul << 13),
	RST_SRC_CPU1RST		= (0x01ul << 14),
	RST_SRC_SW1RST		= (0x01ul << 15)
}csi_rsr_src_e;


typedef enum{
	USER_REG0			= 0,
	USER_REG1,
	USER_REG2,
}csi_user_reg_e;

/**
  \brief       get chip reset reason
  \param       none
  \return      RSR REG value 
*/
uint16_t csi_get_rst_reason(void);

/**
  \brief       clr chip reset reason
  \param       hwRstSrc: reset reason
  \return      none
*/
void csi_clr_rst_reason(uint16_t hwRstSrc);

/**
  \brief       chip software reset
  \param       none
  \return      none
*/
void csi_sys_swrst(void);

/**
  \brief       user reg write 
  \param       eUreg, UREG0~2
  \param       value of write
  \return      error code
*/
csi_error_t csi_ureg_write(csi_user_reg_e eUreg, uint32_t wValue);

/**
  \brief       user reg read
  \param       eUreg, UREG0~2
  \return      value of read
*/
uint32_t csi_ureg_read(csi_user_reg_e eUreg);

/// ************************************************************************
///						for sram error check
///*************************************************************************
/**
  \brief       set sramcheck fail time.
  \param       wVal times
  \return      none
*/
void csi_sramcheck_set_times(uint16_t hwVal);

/**
  \brief       set chip to reset when sramcheck times > preset value
  \param       none
  \return      none
*/
void csi_sramcheck_rst(void);

/**
  \brief       claim INT when sramcheck times > preset value
  \param       none
  \return      none
*/
void csi_sramcheck_int(void);

/**
  \brief       disable sramcheck funtion
  \param       none
  \return      none
*/
void csi_sramcheck_disable(void);

/// ************************************************************************
///						for flash error check
///*************************************************************************
/**
  \brief       set flashcheck fail time.
  \param       wVal times
  \return      error code \ref csi_error_t
*/
void csi_flashcheck_set_times(uint32_t wVal);

/**
  \brief       set chip to reset when flashcheck times > preset value
  \param       none
  \return      none
*/
void csi_flashcheck_rst(void);

/**
  \brief       disable flashcheck funtion
  \param       none
  \return      none
*/
void csi_flashcheck_disable(void);

/// ************************************************************************
///						for EM clock monitor
///*************************************************************************
/**
  \brief       claim INT and switch sysclk to IMOSC when EMOSC failure detected
  \return      none
*/
void csi_emcm_2_imosc_int(void);

/**
  \brief      rest chip when EMOSC failure detected
  \param       none
  \return      none
*/
void csi_emcm_rst(void);


/**
  \brief       disable EMOSC monitor funtion
  \param       none
  \return      none
*/
void csi_emcm_disable(void);

/// ************************************************************************
///						for ES clock monitor
///*************************************************************************
/**
  \brief       claim INT and switch sysclk to IMOSC when ESOSC failure detected
  \return      none
*/
void csi_escm_2_imosc_int(void);

/**
  \brief      rest chip when ESOSC failure detected
  \param       none
  \return      none
*/
void csi_escm_rst(void);


/**
  \brief       disable ESOSC monitor funtion
  \param       none
  \return      none
*/
void csi_escm_disable(void);

/// ************************************************************************
///						for RSR
///*************************************************************************
/**
  \brief       get chip reset reason
  \param       none
  \return      csi_rsr_e 
*/
uint16_t csi_get_rst_reason(void);


/// *********************************************************************************
///  For SWD Lock function: To switch SWD pins to other AF, should unlock SWD first 
///**********************************************************************************

/**
  \brief       SWD Lock
  \return      none
*/
void csi_swd_lock(void);
/**
  \brief       SWD Unlock
  \return      none
*/
void csi_swd_unlock(void);

/** \brief cmos auto trim
 * 
 *  
 * 
 *  \param none
 *  \return csi_error_t.
 */
csi_error_t csi_cmos_autotrim(void);

/** \brief cqcr enable
 * 
 *  \param none
 *  \return none
 */
void csi_cqcr_enable(void);


/** \brief cqcr disable
 * 
 *  \param none
 *  \return none
 */
void csi_cqcr_disable(void);

/** \brief cqcr ref sel
 * 
 *  \param[in] eRefSel csi_cqcr_refsel_e
 *  \param[in] eSrcSel csi_cqcr_srcsel_e
 *  \param[in] wVal 
 *  \return none
 */
void csi_set_cqcr(csi_cqcr_refsel_e eRefSel,csi_cqcr_srcsel_e eSrcSel,uint32_t wVal);

/** \brief get cqcr 
 * 
 *  \return cqcr value
 */
uint32_t csi_get_cqsr(void);

/** \brief sram set
 * 
 *  \param[in] eSramBlk csi_sram1_func_e    SRAM_24KRAM0_8KRAM1_CTRL or SRAM_16KRAM0_16KRAM1_CTRL
 *  \param[in] eSram1Func csi_sram1_func_e  SRAM1_DSARM_CTRL or SRAM1_ISRAM_CTRL
 *  \return none
 */
void csi_set_sram(csi_sram_blk_e eSramBlk,csi_sram1_func_e eSram1Func);

#endif /* _DRV_RLBLTY_H_ */