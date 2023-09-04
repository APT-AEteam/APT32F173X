/***********************************************************************//** 
 * \file  csp_syscon.h
 * \brief  SYSCON description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-10 <td>V0.0  <td>WNN   <td>initial
 * </table>
 * *********************************************************************
*/

#include <stdint.h>
#include "csp_common.h"
#include <csi_core.h>
#include <soc.h>

#ifndef _CSP_SYSCON_H_
#define _CSP_SYSCON_H_



/// \struct csp_syscon_t
/// \brief SYSCON reg description
typedef volatile struct {                   			/*!< SYSCON Structure                         */
	volatile unsigned int IDCCR;                        /*!< 0x000: Identification & System Controller Clock Control Register */
	volatile unsigned int GCER;                         /*!< 0x004: System Controller General Control Enable Register */
	volatile unsigned int GCDR;                         /*!< 0x008: System Controller General Control Disable Register */
	volatile unsigned int GCSR;                         /*!< 0x00C: System Controller General Control Status Register */
	volatile unsigned int CKST;                         /*!< 0x010*/
	volatile unsigned int RAMCHK;                       /*!< 0x014*/
	volatile unsigned int EFLCHK;                       /*!< 0x018*/
	volatile unsigned int SCLKCR;                       /*!< 0x01C: System Controller System Clock Selection & Division Register */
	volatile unsigned int PCLKCR;                       /*!< 0x020: System Controller Peripheral Clock Selection & Division Register */
	volatile unsigned int EXICFG;                       /*!< 0x024*/
	volatile unsigned int PCER0;                        /*!< 0x028: System Controller Peripheral Clock Enable Register */
	volatile unsigned int PCDR0;                        /*!< 0x02C: System Controller Peripheral Clock Disable Register */
	volatile unsigned int PCSR0;                        /*!< 0x030: System Controller Peripheral Clock Status Register */
	volatile unsigned int PCER1;                        /*!< 0x034: System Controller Peripheral Clock Enable Register */
	volatile unsigned int PCDR1;                        /*!< 0x038: System Controller Peripheral Clock Disable Register */
	volatile unsigned int PCSR1;                        /*!< 0x03C: System Controller Peripheral Clock Status Register */
	volatile unsigned int OSTR;                         /*!< 0x040: System Controller External OSC Stable Time Control Register */
	volatile unsigned int PLLCR;                        /*!< 0x044: System Controller PLL Stable Time Control Register */
	volatile unsigned int _RSVD0;                       /*!< 0x048: System Controller PLL PMS Value Control Register */
	volatile unsigned int LVDCR;                        /*!< 0x04C: System Controller LVD Control Register */
	volatile unsigned int CLCR;                         /*!< 0x050: System Controller IMOSC Fine Adjustment Register*/
	volatile unsigned int PWRCR;                        /*!< 0x054: System Controller Power Control Register */
	volatile unsigned int PWRKEY;                       /*!< 0x058: System Controller Power Control Register */
	volatile unsigned int _RSVD1;                       /*!< 0x05C: */
	volatile unsigned int _RSVD2;                       /*!< 0x060: */
	volatile unsigned int OPT1;                         /*!< 0x064: System Controller OSC Trim Control Register */
	volatile unsigned int OPT0;                         /*!< 0x068: System Controller Protection Control Register */
	volatile unsigned int WKCR;                         /*!< 0x06C: System Controller Clock Quality Check Control Register */
	volatile unsigned int _RSVD3;                       /*!< 0x070: System Controller Clock Quality Check Control Register */
	volatile unsigned int IMER;                         /*!< 0x074: System Controller Interrupt Enable Register */
	volatile unsigned int IMDR;                         /*!< 0x078: System Controller Interrupt Disable Register */
	volatile unsigned int IMCR;                         /*!< 0x07C: System Controller Interrupt Mask Register */
	volatile unsigned int IAR;                          /*!< 0x080: System Controller Interrupt Active Register */
	volatile unsigned int ICR;                          /*!< 0x084: System Controller Clear Status Register */
	volatile unsigned int RISR;                         /*!< 0x088: System Controller Raw Interrupt Status Register */
	volatile unsigned int MISR;                         /*!< 0x08C: System Controller Raw Interrupt Status Register */
	volatile unsigned int RSR;                          /*!< 0x090: System Controller Raw Interrupt Status Register */
	volatile unsigned int EXIRT;                        /*!< 0x094: System Controller Reset Status Register */
	volatile unsigned int EXIFT;                        /*!< 0x098: System Controller External Interrupt Mode 1 (Positive Edge) Register */
	volatile unsigned int EXIER;                        /*!< 0x09C: System Controller External Interrupt Mode 2 (Negative Edge) Register */
	volatile unsigned int EXIDR;                        /*!< 0x0A0: System Controller External Interrupt Enable Register */
	volatile unsigned int EXIMR;                        /*!< 0x0A4: System Controller External Interrupt Disable Register */
	volatile unsigned int EXIAR;                        /*!< 0x0A8: System Controller External Interrupt Mask Register */
	volatile unsigned int EXICR;                        /*!< 0x0AC: System Controller External Interrupt Active Register */
	volatile unsigned int EXIRS;                        /*!< 0x0B0: System Controller External Interrupt Clear Status Register */
	volatile unsigned int IWDCR;                        /*!< 0x0B4: System Controller Independent Watchdog Control Register */
	volatile unsigned int IWDCNT;                       /*!< 0x0B8: SystCem Controller Independent Watchdog Counter Value Register */
	volatile unsigned int IWDEDR;                       /*!< 0x0BC: System Controller Independent Watchdog Enable/disable Register*/
	volatile unsigned int IOMAP0;                       /*!< 0x0C0: Customer Information Content mirror of 1st byte*/
	volatile unsigned int IOMAP1;                       /*!< 0x0C4: Customer Information Content mirror of 1st byte*/
	volatile unsigned int _RSVD4;                    	/*!< 0x0C8: Customer Information Content mirror of 1st byte*/
	volatile unsigned int _RSVD5;                        /*!< 0x0CC: Customer Information Content mirror of 1st byte*/
	volatile unsigned int FINF0;                        /*!< 0x0D0: Customer Information Content mirror of 1st byte*/
	volatile unsigned int FINF1;                        /*!< 0x0D4: Customer Information Content mirror of 1st byte*/
	volatile unsigned int FINF2;                        /*!< 0x0D8: Customer Information Content mirror of 1st byte*/
	volatile unsigned int _RSVD6;                       /*!< 0x0DC: Customer Information Content mirror of 1st byte*/
	volatile unsigned int ERRINF;                       /*!< 0x0E0:*/
	volatile unsigned int UID0 ;                        /*!< 0x0E4: Customer Information Content mirror of 1st byte*/
	volatile unsigned int UID1 ;                        /*!< 0x0E8: Customer Information Content mirror of 1st byte*/
	volatile unsigned int UID2 ;                        /*!< 0x0EC: Customer Information Content mirror of 1st byte*/
	volatile unsigned int PWROPT;                       /*!< 0x0F0: Power recovery timmming control */
	volatile unsigned int EVTRG;                       	/*!< 0x0F4: Trigger gen                                    */
	volatile unsigned int EVPS;                      	/*!< 0x0F8: Trigger prs                                    */
	volatile unsigned int EVSWF;                       	/*!< 0x0FC: Trigger software force                         */
	volatile unsigned int UREG0;                      	/*!< 0x100: User defined reg0                              */
	volatile unsigned int UREG1;                       	/*!< 0x104: User defined reg1                              */
	volatile unsigned int UREG2;                       	/*!< 0x108: User defined reg0                              */
	volatile unsigned int TRIMUREG;                       /*!< 0x10C: User defined reg0                              */
	volatile unsigned int _RSVD7[2];					
	volatile unsigned int CQCR;                        	/*!< 0x118: Clock Quality check control reg                */
	volatile unsigned int CQSR;						   	/*!< 0x11C: Clock Qualifty check control reg  			   */
	volatile unsigned int _RSVD8[2];
	volatile unsigned int DBGCR;						/*!< 0x128: Debug Control Register			   */
	volatile unsigned int BRPKEY;						/*!< 0x12c: SRBREG, ERBREG  protect reg			   */	
	volatile unsigned int SRBREG;						/*!< 0x130: SRBREG Register			   */	
	volatile unsigned int ERBREG;						/*!< 0x134: ERBREG Register			   */	
} csp_syscon_t; 



/// IDCCR reg content
#define SYSCON_CLKEN		(0x01ul)
#define SYSCON_SWRST_POS 	(5)
typedef enum{
	SYS_SWRST	= 0x04ul,
	CPU_SWRST 	= 0x05ul
}sw_rst_e;
#define SYSCON_IDKEY		(0xE11Eul << 16)

/// GCER/GCDR/GCSR/CKST reg content
#define ISOSC 		(0x01ul)
#define IMOSC 		(0x01ul << 1)
#define ESOSC 		(0x01ul << 2)
#define EMOSC 		(0x01ul << 3)
#define HFOSC 		(0x01ul << 4)
#define PLL 		(0x01ul << 5)
#define SYSCLK		(0x01ul << 8)
#define SYSTICK 	(0x01ul << 11)

typedef enum{
	PCLK_IDLE = 8,
	HCLK_IDLE,
	ISOSC_STP = 12, 
	IMOSC_STP,
	ESOSC_STP,
	EMOSC_STP
}clk_pm_e;

#define ES_CM_EN 	(0x01ul<<16)
#define ES_CMRST 	(0x01ul<<17)

#define EM_CM_EN 	(0x01ul<<18)
#define EM_CMRST 	(0x01ul<<19)


#define SYSCLK_STABLE 	(0x01ul<<8)

///RAMCHK
#define RAMCHK_POS	(24)
#define RAMCHK_MSK	(0xff << RAMCHK_POS)
#define RAMCHK_EN	(0x5a << RAMCHK_POS)
#define RAMCHK_DIS	(0x00 << RAMCHK_POS)
#define RAMCHK_RST_POS	(16)
#define RAMCHK_RST_MSK (0xff << RAMCHK_RST_POS)
#define RAMCHK_RST	(0x5a << RAMCHK_RST_POS)
#define RAMCHK_INT	(0x00 << RAMCHK_RST_POS)
#define RAMCHK_TIMES_MSK	(0xffff)

///EFLCHK
#define EFLCHK_POS	(24)
#define EFLCHK_MSK	(0xff << EFLCHK_POS)
#define EFLCHK_EN	(0x5a << EFLCHK_POS)
#define EFLCHK_DIS	(0x00 << EFLCHK_POS )
#define EFLCHK_TIMES_MSK	(0xffffff)


/// SCLKCR
#define SYSCLK_SRC_MSK (0x7ul)
#define SC_IMOSC 	0
#define SC_EMOSC 	1
#define SC_HFOSC 	2
#define SC_PLL	    3
#define SC_ISOSC 	4
#define SC_ESOSC 	5

#define HCLK_DIV_MSK (0xful<<8)
#define SCLK_KEY (0xd22dul<<16)


/// PCLKCR
#define PCLK_DIV_MSK (0xf<<8)
#define PCLK_KEY (0xC33Cul<<16)

/// EXI FLT
#define	EXI_FLT_POS(n)	(4*n)
#define	EXI_FLT_MSK(n)	(0x0ful << EXI_FLT_POS(n))
typedef enum{
	EXIFLT_NONE = 0,
	EXIFLT_3THF,
	EXIFLT_6THF,
	EXIFLT_9THF,
	EXIFLT_12THF,
	EXIFLT_18THF,
	EXIFLT_24THF,
	EXIFLT_36THF,
	EXIFLT_48THF,
	EXIFLT_60THF,
	EXIFLT_72THF,
	EXIFLT_84THF,
	EXIFLT_96THF,
	EXIFLT_120THF,
	EXIFLT_168THF,
	EXIFLT_216THF
}exi_flt_e;

/// OSTR: EXTernal OSC stable time
#define EM_LF   		(0x01ul << 10)
#define EM_GM_POS		(11)
#define EM_GM_MSK		(0x1Ful << EM_GM_POS)

#define ES_SMTDIS_POS	(24)
#define ES_SMTDIS_MSK	(0x01ul << ES_SMTDIS_POS)

#define EM_FLTEN_POS	(25)
#define EM_FLTEN_MSK	(0x01ul << EM_FLTEN_POS)

#define EM_FLTSEL_POS	(26)
#define EM_FLTSEL_MSK	(0x03ul << EM_FLTSEL_POS)
typedef enum {
	EM_FLT_5NS = 0,
	EM_FLT_10NS,
	EM_FLT_15NS,
	EM_FLT_20NS,
}em_fltsel_e;


#define ES_GM_POS		(28)
#define ES_GM_MSK		(0x0Ful << ES_GM_POS)

///LVDCR 
#define LVDEN_MSK (0xf)
#define LVD_DIS	(0xa)
#define LVDEN (0x0ul<<0)
#define LVDINT_POL_POS (0x6)
#define LVDINT_POL_MSK (0x3 << LVDINT_POL_POS)
#define LVDINT_DET_POL(r) ((r&0x3) << LVDINT_POL_POS)
#define LVD_FLAG    (0x1 << 15)

typedef enum {
	LVDINT_F = 1,
	LVDINT_R,
	LVDINT_BOTH	
}lvdint_pol_e;

#define LVD_LV_POS (0x8)
#define LVD_LV_MSK  (0x7 << LVD_LV_POS)
typedef enum{
	LVD21 = 0,
	LVD24,
	LVD27,
	LVD30,
	LVD33,
	LVD36,
	LVD39,
	LVDIN10	
}lvd_level_e;

#define LVR_SRT_POS	(4)
#define LVR_SRT_MSK	(0x01ul << LVR_SRT_POS)

#define LVR_LV_POS	(12)
#define LVR_LV_MSK  (0x7 << LVR_LV_POS)

typedef enum {
	LVR19 = 0,
	LVR22,
	LVR25,
	LVR28,
	LVR31,
	LVR34,
	LVR37,
	LVR40
}lvr_level_e;
#define LVD_KEY (0xB44Bul<<16)

///CLCR: osc tuning
#define HFO_TUNE_MSK	(0x1ff)
#define HFO_TRM_MSK 	(0xffff01ff)
#define IMO_TRM_MSK 	(0xff00ffff)
#define ISO_TRM_MSK		(0x00ffffff)

//VOS, PWRCR: power management fine setting
#define RUN_VOSEN (1ul<<0)
#define RUN_VOSDIS (0ul<<0)
#define SLP_VOSEN (1ul<<1)
#define DSL_VOSEN (1ul<<2)
#define VOS_BGRON (1ul<<8)
#define VOS_REGPD (0<<1ul)
#define VOS_REGLP (1<<1ul)
#define VOS_REGLV (2<<1ul)
#define VOS_REGON (3<<1ul)
#define PWRKEY_PWRKEY	  (0xa67a << 16)
#define PWRKEY_VOSLCK	  (0x6cc7)

//PLLCR
#define PLL_CLK_SEL_MSK 	(0x3ul << PLL_CLK_SEL_POS)
#define PLL_CLK_SEL_POS 	(0)

typedef enum{
	PLL_CLK_SEL_NONE0 = 0,
	PLL_CLK_SEL_NONE1,
	PLL_CLK_SEL_HFOSC,
	PLL_CLK_SEL_EMOSC
}pll_clk_sel_e;

#define PLL_DIVM_MSK 	(0xful << PLL_DIVM_POS)
#define PLL_DIVM_POS 	(2)


#define PLL_NUL_MSK 	(0x7ful << PLL_NUM_POS)
#define PLL_NUM_POS 	(8)

#define PLL_CKPEN_MSK 	(0x1ul << PLL_CKPEN_POS)
#define PLL_CKPEN_POS 	(16)

#define PLL_CKP_DIV_MSK 	(0x3ful << PLL_CKP_DIV_POS)
#define PLL_CKP_DIV_POS 	(17)

#define PLL_CKQEN_MSK 	(0x1ul << PLL_CKQEN_POS)
#define PLL_CKQEN_POS 	(24)

#define PLL_CKQ_DIV_MSK 	(0x3ul << PLL_CKQ_DIV_POS)
#define PLL_CKQ_DIV_POS 	(25)

#define PLL_CKREN_MSK 	(0x1ul << PLL_CKREN_POS)
#define PLL_CKREN_POS 	(28)

///OPT1: clo/osc freq/Flash LP mode/EXI filter/EM clock monitoring config
#define IMO_MSK (0x3ul)
#define HFO_MSK (0x3ul<<4)
#define CLO_SRC_MSK (0xful << 8)
#define CLO_SRC_POS (0x8)
typedef enum{
	CLO_SRC_ISCLK = 0,
	CLO_SRC_IMCLK,
	CLO_SRC_ESCLK,
	CLO_SRC_EMCLK = 3,
	CLO_SRC_HFCLK,
	CLO_SRC_RTCCLK = 6,
	CLO_SRC_PCLK,
	CLO_SRC_HCLK,
	CLO_SRC_IWDTCLK,
	CLO_SRC_PLL_PCLK,
	CLO_SRC_PLL_QCLK,
	CLO_SRC_SYSCLK = 0xd
}clo_src_e;

#define CLO_DIV_MSK 	(0x7ul << 12)
#define CLO_DIV_POS 	(12)

typedef enum{
	CLO_CLK_DIV1 = 1,
	CLO_CLK_DIV2,	
	CLO_CLK_DIV4,	
	CLO_CLK_DIV8,	
	CLO_CLK_DIV16	
}clo_div_e;

#define FLASH_LPMODE_POS	15
#define FLASH_LPMODE_MSK 	(0x01ul << FLASH_LPMODE_POS)

#define EXIFLT_EN_POS 		(16)
#define EXIFLT_EN_MSK 		(0x01ul << EXIFLT_EN_POS)

#define EXIFLT_DIV_POS		(17)
#define EXIFLT_DIV_MSK 		(0x03ul << EXIFLT_DIV_POS)
typedef enum{
	EXIFILT_DIV_1 = 0,
	EXIFILT_DIV_2,
	EXIFILT_DIV_3,
	EXIFILT_DIV_4
}exifilt_div_e;

#define EMCKM_DUR_POS		(21)
#define EMCKM_DUR_MSK 		(0x07ul << EMCKM_DUR_POS)
typedef enum{
	EMCKM_DUR_18 = 0,
	EMCKM_DUR_14,
	EMCKM_DUR_10,
	EMCKM_DUR_8,
	EMCKM_DUR_6,
	EMCKM_DUR_5,
	EMCKM_DUR_4,
	EMCKM_DUR_3
}emckm_dur_e;

#define SRAM1FUNCCTRL_POS 		(24)
#define SRAM1FUNCCTRL_MSK 		(0x01ul << SRAM1FUNCCTRL_POS)
typedef enum {
	SRAM1_DSARM = 0,
	SRAM1_ISRAM
}sram1_func_e;

#define SRAMBLKCTRL_POS 		(26)
#define SRAMBLKCTRL_MSK 		(0x01ul << SRAMBLKCTRL_POS)
typedef enum {
	SRAM_24KRAM0_8KRAM1 = 0,
	SRAM_16KRAM0_16KRAM1
}sram_blk_e;

#define NMI_POS 		(27)
#define NMI_MSK 		(0xFul << NMI_POS)
typedef enum
{
	NMI_LVD_INT    	=	(0x01ul << 27), 
	NMI_MEMERY_ERR  =	(0x01ul << 28),     
	NMI_EXI0_INT    =	(0x01ul << 29),  
	NMI_ECLK_ERR    =	(0x01ul << 30)
}nmi_sel_e;


/// OPT0: read user option infor (IWDT/EXIRST...)
#define IWDT_DFT_S	(0x1)
#define EXIRST_S	(0x2)
#define CPUFTRST_S  (0x4)
#define CIPVALID_S  (0x1 << 7)
#define DBP_S  		(0x1 << 8)
#define HDP_ALL_S	(0x1 << 16)
#define HDP_4K_S  (0x1 << 17)

//PWRCR
#define VOS_RUN_EN_P0S	         (0)
#define VOS_RUN_EN_P0S_MSK	     (0x1ul << VOS_RUN_EN_P0S)

#define VOS_SLEEP_EN_P0S	     (1)
#define VOS_SLEEP_EN_P0S_MSK	 (0x1ul << VOS_SLEEP_EN_P0S)

#define VOS_DSLEEP_EN_P0S	     (2)
#define VOS_DSLEEP_EN_P0S_MSK	 (0x1ul << VOS_DSLEEP_EN_P0S)

#define VOS_SLEEP_CFG_P0S	     (16)
#define VOS_SLEEP_CFG_MSK        (0xful << VOS_SLEEP_CFG_P0S)

#define VOS_SLEEP_VCREF_P0S	     (16+0)
typedef enum{
	BGR_1V		= 0,                   // BGR(1.0V)
	CMOS_09V	= 1                    // CMOS(0.9V)
}sleep_vcref_e;

#define VOS_SLEEP_VDDCORE_P0S	 (16+1)
typedef enum{
	VCREF_12		= 0,              //VDDCORE = VCref x 1.2
	VCREF_10	    = 1               //VDDCORE = VCref x 1.0
}sleep_vddcore_e;

#define VOS_SLEEP_SLP_CTRL_P0S	     (16+2)
#define VOS_SLEEP_SLP_CTRL_P0S_MSK	 (0x1ul << VOS_SLEEP_SLP_CTRL_P0S)

#define VOS_SLEEP_PD_CTRL_P0S	     (16+3)
#define VOS_SLEEP_PD_CTRL_P0S_MSK	 (0x1ul << VOS_SLEEP_PD_CTRL_P0S)

#define PWRCR_KEY                     0xA67A6CC7       

/// WKCR: wakeup(from deep-sleep) source register
#define IWDT_WKEN	(0x1<<8)
#define RTC_WKEN	(0x1<<9)
#define LPT_WKEN	(0x1<<10)
#define LVD_WKEN	(0x1<<11)
#define TKEY_WKEN	(0x1<<12)
#define WKI_WKEN	(0x1<<13)

#define DPSLEEP_MODE_P0S	(16)
#define DPSLEEP_MODE_MSK	(0xfful << DPSLEEP_MODE_P0S)
typedef enum{
	SNOOZE_MODE		= (0xAaul),
	SHUTDOWN_MODE	= (0xA5ul),
	DPSLEEP_MODE	= (0x05ul),
}deepsleep_mode_e;

#define SNOOZE_TOUCH_P0S	(24)
#define SNOOZE_TOUCH_MSK	(0x01ul << SNOOZE_TOUCH_P0S)
#define SNOOZE_LCD_P0S		(25)
#define SNOOZE_LCD_MSK		(0x01ul << SNOOZE_LCD_P0S)

///INTERRUPT related regs: IMER/IMDR/IAR/ICR/IMCR/RISR/MISR
/*#define ISOSC_ST	(0x1)
#define IMOSC_ST	(0x1<<1)
#define EMOSC_ST	(0x1<<3)
#define HFOSC_ST	(0x1<<4)
#define SYSCLK_ST 	(0x1<<7)
#define IWDT_INT	(0x1<<8)
#define WKI_INT		(0x1<<9)
#define RAMERR_INT	(0x1<<10)
#define LVD_INT		(0x1<<11)
#define EM_CMFAIL	(0x1<<18)
#define CMD_ERR		(0x1<<29)*/
typedef enum{
	ISOSC_ST_INT 	= (0x1<<0),
	IMOSC_ST_INT 	= (0x1<<1),
	ESOSC_ST_INT 	= (0x1<<2),
	EMOSC_ST_INT 	= (0x1<<3),
	HFOSC_ST_INT 	= (0x1<<4),
	PLL_ST_INT 	    = (0x1<<5),
	SYSTICK_ST_INT 	= (0x1<<7),
	IWDT_INT 		= (0x1<<8),
	RAM_ERR_INT 	= (0x1<<10),
	LVD_INT 		= (0x1<<11),
	EFL_ERR_INT 	= (0x1<<13),
	OPL_ERR_INT 	= (0x1<<14),
	PLL_ERR_INT 	= (0x1<<15),
	ESFAIL_INT 		= (0x1<<16),
	CQCDNE_INT 		= (0x1<<17),
	EMFAIL_INT 		= (0x1<<18),
	EV0TRG_INT 		= (0x1<<19),
	EV1TRG_INT 		= (0x1<<20),
	EV2TRG_INT 		= (0x1<<21),
	EV3TRG_INT 		= (0x1<<22),
	CMD_ERR_INT		= (0x1<<29),
}syscon_int_e;

/// EXI interrupt regs: EXIER/EXIDR/EXIRS/EXIAR/EXICR/EXIRS

/// RSR reset register
#define RSR_MSK		(0x3fff)
typedef enum{
	PORST	= 0,
	LVRST,
	EXTRST,
	IWDTRST = 4,
	EMCMRST = 6,
	CPURST,
	SWRST,
	CPUFAULTRST,
	SRAMRST = 11,
	EFLRST,
	WWDTRST
}rst_st_e;


///IWDCR

#define IWDTCR_KEY (0x8778ul << 16)

#define IWDT_INTV_POS	(2)
#define IWDT_INTV_MSK (0x3f << IWDT_INTV_POS)

typedef enum{
	IWDT_INTW_7_8 = 0,
	IWDT_INTW_6_8,
	IWDT_INTW_5_8, 
	IWDT_INTW_4_8, 
	IWDT_INTW_3_8, 
	IWDT_INTW_2_8, 
	IWDT_INTW_1_8
}iwdt_intv_e;


#define IWDT_OVT_POS	(8)
#define IWDT_OVT_MSK (0x7 << IWDT_OVT_POS)
typedef enum{
	IWDT_TIME_0128 = 0x0,
	IWDT_TIME_0256,
	IWDT_TIME_0512,
	IWDT_TIME_1000,
	IWDT_TIME_2000,
	IWDT_TIME_3100,
	IWDT_TIME_4100,
	IWDT_TIME_8200
}iwdt_ovt_e;

#define IWDT_DBGEN_POS (11)
#define IWDT_DBGEN_MSK (0x1 << IWDT_DBGEN_POS)
#define IWDT_ST		(0x1 << 12)
#define IWDT_BUSY	(0x1 << 12)
///IWDCNT
#define IWDT_CLR_BUSY	(0x1<<31)
#define IWDT_CLR_POS	(24)
#define IWDT_CLR_MSK	(0x7f << IWDT_CLR_POS)
#define IWDT_CLR		(0x5a)
#define IWDT_CNT		(0xff)

///IWDEDR: IWDT enable control reg
#define IWDTE_KEY	(0x7887 << 16)
#define DIS_IWDT	(0x55aa)
#define EN_IWDT		(0x0)

///IOMAP0/1:
#define CFGVAL_POS(n)	(n<<2)
#define CFGVAL_MSK(n)	(0xf << CFGVAL_POS(n))

///Exernal Interrupt: EXIRT/EXIFT/EXIER/EXIFT/EXIER/EXIDR/EXIMR
#define SYSCON_EXI_POS(n)	(n)
#define SYSCON_EXI_MSK(n)	(0x1 << SYSCON_EXI_POS(n))


///PWROPT
#define PWROPT_KEY	(0xB6 << 24) 
#define EFL_PD		(0x3 << 16)
#define EFLR_PD		(0x3 << 18)
#define EFLR_CTL_M	(0x0<<20)
#define EFLR_CTL_SLP	(0x1<<20)
#define EFLR_CTL_SLP_LP	(0x2<<20)

#define DSP_PRCVTIM_POS		(0x00)
#define DSP_PRCVTIM_MSK		(0x03ul << DSP_PRCVTIM_POS)
#define SLP_PRCVTIM_POS		(0x02)
#define SLP_PRCVTIM_MSK		(0x03ul << SLP_PRCVTIM_POS)
///System Protection Information Mirror Register Access
#define PROT_ACCESS_KEY  0x6996ul
#define IDCCR_KEY (0xE11Eul<<16)


///EVTRG: event triggger conig reg
#define TRG_SRC0_3_POS(n)  ((n) << 2)
#define TRG_SRC0_3_MSK(n)  (0xf << TRG_SRC0_3_POS(n))
#define TRG_SRC4_5_POS(n)  (16 + ((n-4) << 1))
#define TRG_SRC4_5_MSK(n)  (0x3 << TRG_SRC4_5_POS(n))


typedef enum{
	EXI0_TRSRC = 0,
	EXI1_TRSRC,
	EXI2_TRSRC,
	EXI3_TRSRC,
	EXI4_TRSRC,
	EXI5_TRSRC,
	EXI6_TRSRC,
	EXI7_TRSRC,
	EXI8_TRSRC,
	EXI9_TRSRC,
	EXI10_TRSRC,
	EXI11_TRSRC,
	EXI12_TRSRC,
	EXI13_TRSRC,
	EXI14_TRSRC,
	EXI15_TRSRC,
	EXI16_TRSRC,
	EXI17_TRSRC,
	EXI18_TRSRC,
	EXI19_TRSRC
}exi_trg_src_e;


#define ENDIS_ESYNC_POS(n)  (20+ (n))
#define ENDIS_ESYNC_MSK(n)  (0x1 << ENDIS_ESYNC_POS(n))


#define TRG_EVCNT_CLR_POS(n) 	(28+(n))
#define TRG_EVCNT_CLR_MSK(n)	(0x1 << TRG_EVCNT_CLR_POS(n))


///EVPS: event trigger cnt
#define TRG_EVPRD_POS(n) 	((n)<<2)
#define TRG_EVPRD_MSK(n) 	(0xf << TRG_EVPRD_POS(n))

#define TRG_EVCNT_POS(n)	((n<<2) + 16)
#define TRG_EVCNT_MSK(n)	(0xf << TRG_EVCNT_POS(n))


///EVSWF: Software force outpuevent
#define SWFC_EV_POS(n)	(n)
#define SWFC_EV_MSK(n)  (0x1 << (n))


#define CMRTRIM_VULUE_MSK             (0x1f)  
#define CMRTRIML_VULUE_POS            (5) 
#define CMRTRIML_VULUE_MSK            (0x1f << CMRTRIML_VULUE_POS)  

#define CMRTRIM_CTL_POS               (10)
#define CMRTRIM_CTL_MSK               (1 << 10)
#define CMRTRIML_CTL_POS              (11)
#define CMRTRIML_CTL_MSK              (1 << 11)


//CQCR
#define CQCR_EN_POS                   (0) 
#define CQCR_EN_MSK                   (0x1 << CQCR_EN_POS)  

#define CQCR_REFSEL_POS               (4) 
#define CQCR_REFSEL_MSK               (0x3 << CQCR_REFSEL_POS)  

typedef enum{
	CQCR_REFSEL_EMOSC,
	CQCR_REFSEL_ESOSC,	
}cqcr_refsel_e;

#define CQCR_SRCSEL_POS               (6) 
#define CQCR_SRCSEL_MSK               (0x3 << CQCR_SRCSEL_POS)  
typedef enum{
	CQCR_SRCSEL_IMOSC,
	CQCR_SRCSEL_ESOSC,
	CQCR_SRCSEL_ISOSC,
	CQCR_SRCSEL_HFOSC
}cqcr_srcsel_e;

#define CQCR_CQRVAL_POS               (8) 
#define CQCR_CQRVAL_MSK               (0x3FFFF << CQCR_CQRVAL_POS)  


///definition of syscon functions
#define csp_get_ckst(SYSCONx)							((uint32_t) ((SYSCONx->CKST) & 0x13f))

#define csp_get_gcsr(SYSCONx)							((uint32_t) ((SYSCONx->GCSR) & 0x0ffb1f))

#define csp_set_clksrc(SYSCONx,wClkSrc)					({\
															SYSCONx->SCLKCR = (SYSCONx->SCLKCR & (~SYSCLK_SRC_MSK)) | SCLK_KEY | wClkSrc;\
															while((SYSCONx->CKST & SYSCLK) == 0);\
														})
												
#define csp_pll_clk_sel(SYSCONx,eClkSel)				(SYSCONx->PLLCR = (SYSCONx->PLLCR & (~PLL_CLK_SEL_MSK)) | eClkSel << PLL_CLK_SEL_POS)   ///\param eClkSel \ref pll_clk_sel_e

#define csp_pll_set_div_m(SYSCONx,byDivM)				(SYSCONx->PLLCR = (SYSCONx->PLLCR & (~PLL_DIVM_MSK)) | byDivM << PLL_DIVM_POS)

#define csp_pll_set_nul(SYSCONx,byNul)					(SYSCONx->PLLCR = (SYSCONx->PLLCR & (~PLL_NUL_MSK)) | byNul << PLL_NUM_POS)

#define csp_pll_set_ckp_div(SYSCONx,byCkp_Div)			(SYSCONx->PLLCR = (SYSCONx->PLLCR & (~PLL_CKP_DIV_MSK)) | byCkp_Div << PLL_CKP_DIV_POS)

#define csp_pll_clk_enable(SYSCONx)						(SYSCONx->PLLCR = (SYSCONx->PLLCR & ~PLL_CKPEN_MSK)| (1 << PLL_CKPEN_POS))

#define csp_pll_clk_disable(SYSCONx)					(SYSCONx->PLLCR = (SYSCONx->PLLCR & ~PLL_CKPEN_MSK)| (0 << PLL_CKPEN_POS))

#define csp_pll_set_ckq_div(SYSCONx,byCkp_Div)			(SYSCONx->PLLCR = (SYSCONx->PLLCR & (~PLL_CKQ_DIV_MSK)) | byCkp_Div << PLL_CKQ_DIV_POS)

#define csp_set_hfosc_fre(SYSCONx,wFreq)				(SYSCONx->OPT1 = (SYSCONx->OPT1 & (~HFO_MSK)) | wFreq << 4)

#define csp_get_hfosc_fre(SYSCONx)						((uint32_t) (((SYSCONx->OPT1) & HFO_MSK) >> 4))

#define csp_set_imosc_fre(SYSCONx,wFreq)				(SYSCONx->OPT1 = (SYSCONx->OPT1 & (~IMO_MSK)) | wFreq)

#define csp_get_imosc_fre(SYSCONx)						((uint32_t) ((SYSCONx->OPT1) & IMO_MSK))

#define csp_set_sdiv(SYSCONx,wSdiv)						(SYSCONx->SCLKCR =  (SYSCONx->SCLKCR & (~HCLK_DIV_MSK)) | SCLK_KEY | (wSdiv << 8))

#define csp_set_pdiv(SYSCONx,wSdiv)						(SYSCONx->PCLKCR =  (SYSCONx->PCLKCR & (~PCLK_DIV_MSK)) | PCLK_KEY | (wSdiv << 8))
	
#define csp_get_pdiv(SYSCONx)							((uint32_t ) (((SYSCONx->PCLKCR) & PCLK_DIV_MSK) >> 8))

#define csp_get_clksrc(SYSCONx)							((uint32_t) ((SYSCONx->SCLKCR) & SYSCLK_SRC_MSK))

#define csp_get_hclk_div(SYSCONx)						((uint32_t) (((SYSCONx->SCLKCR) & HCLK_DIV_MSK) >> 8))

#define csp_get_imsrc(SYSCONx)							((uint8_t) (SYSCONx->OPT1 & IMO_MSK))

#define csp_set_clo_src(SYSCONx,wCloSrc)				(SYSCONx->OPT1 = (SYSCONx->OPT1 & (~CLO_SRC_MSK)) | wCloSrc << 8)

#define csp_set_clo_div(SYSCONx,wCloDiv)				(SYSCONx->OPT1 = (SYSCONx->OPT1 & (~CLO_DIV_MSK)) | wCloDiv << 12)

#define csp_pcer0_clk_enable(SYSCONx,wIdx)				(SYSCONx->PCER0 = (0x01ul << wIdx))

#define csp_pcer1_clk_enable(SYSCONx,wIdx)				(SYSCONx->PCER1 = (0x01ul << wIdx))

#define csp_pder0_clk_disable(SYSCONx,wIdx)				(SYSCONx->PCDR0 = (0x01ul << wIdx))

#define csp_pder1_clk_disable(SYSCONx,wIdx)				(SYSCONx->PCDR1 = (0x01ul << wIdx))

#define csp_clk_pm_enable(SYSCONx,eClk)					(SYSCONx->GCER = 0x1 << eClk)   			///\param eClk \ref clk_pm_e

#define csp_clk_pm_disable(SYSCONx,eClk)				(SYSCONx->GCDR = 0x1 << eClk)				///\param eClk \ref clk_pm_e

#define csp_em_lfmd_enable(SYSCONx)						(SYSCONx->OSTR |= EM_LF)

#define csp_em_lfmd_disable(SYSCONx)					(SYSCONx->OSTR &= ~EM_LF)

#define csp_em_flt_enable(SYSCONx)						(SYSCONx->OSTR = (SYSCONx->OSTR & ~EM_FLTEN_MSK)| (1 << EM_FLTEN_POS))

#define csp_em_flt_disable(SYSCONx)						(SYSCONx->OSTR = (SYSCONx->OSTR & ~EM_FLTEN_MSK)| (0 << EM_FLTEN_POS))

#define csp_em_flt_sel(SYSCONx,eFltSel)					(SYSCONx->OSTR = (SYSCONx->OSTR & ~EM_FLTSEL_MSK)| (eFltSel << EM_FLTSEL_POS))  			///\param eFltSel \ref  em_fltsel_e

#define csp_eflash_lpmd_enable(SYSCONx)					(SYSCONx->OPT1 = (SYSCONx->OPT1 & ~FLASH_LPMODE_MSK)| (1 << FLASH_LPMODE_POS))

#define csp_eflash_lpmd_disable(SYSCONx)				(SYSCONx->OPT1 = (SYSCONx->OPT1 & ~FLASH_LPMODE_MSK)| (0 << FLASH_LPMODE_POS))

#define csp_sram1_func_ctrl(SYSCONx,eSram1Func)			(SYSCONx->OPT1 = (SYSCONx->OPT1 & ~SRAM1FUNCCTRL_MSK)| (eSram1Func << SRAM1FUNCCTRL_POS))	///\param eSram1Func \ref sram1_func_e

#define csp_sram_blk_ctrl(SYSCONx,eSramBlk)				(SYSCONx->OPT1 = (SYSCONx->OPT1 & ~SRAMBLKCTRL_MSK)| (eSramBlk << SRAMBLKCTRL_POS))			///\param eSramBlk \ref sram_blk_e

#define csp_nmi_int_enable(SYSCONx,eNmiSel)				(SYSCONx->OPT1 |=  eNmiSel)			///\param eNmiSel \ref nmi_sel_e

#define csp_nmi_int_disable(SYSCONx,eNmiSel)			(SYSCONx->OPT1 &= ~eNmiSel)			///\param eNmiSel \ref nmi_sel_e

#define csp_set_em_gain(SYSCONx,byGn)					(SYSCONx->OSTR = (SYSCONx->OSTR & (~EM_GM_MSK)) | (byGn << EM_GM_POS))

#define csp_set_es_gain(SYSCONx,byGn)					(SYSCONx->OSTR = (SYSCONx->OSTR & (~ES_GM_MSK)) | (byGn << ES_GM_POS))

#define csp_set_lvr_level(SYSCONx,eLvl)					(SYSCONx->LVDCR = (SYSCONx->LVDCR & (~LVR_LV_MSK)) | (eLvl) << LVR_LV_POS | LVD_KEY)		///\param eLvl \ref lvr_level_e

#define csp_set_lvd_level(SYSCONx,eLvl)					(SYSCONx->LVDCR = (SYSCONx->LVDCR & (~LVD_LV_MSK)) | (eLvl) << LVD_LV_POS | LVD_KEY)		///\param eLvl \ref lvd_level_e

#define csp_set_lvd_int_pol(SYSCONx,ePol)				(SYSCONx->LVDCR = (SYSCONx->LVDCR & (~LVDINT_POL_MSK)) | ePol << LVDINT_POL_POS | LVD_KEY)	///\param ePol \ref lvdint_pol_e

#define csp_lvd_lvr_enable(SYSCONx)						(SYSCONx->LVDCR = (SYSCONx->LVDCR &(~LVDEN_MSK)) | 5 | LVD_KEY)

#define csp_lvd_lvr_disable(SYSCONx)					(SYSCONx->LVDCR = (SYSCONx->LVDCR &(~LVDEN_MSK))| LVD_DIS | LVD_KEY)

#define csp_lvr_rst_enable(SYSCONx)						(SYSCONx->LVDCR = (SYSCONx->LVDCR &(~LVR_SRT_MSK)) | LVD_KEY)

#define csp_lvr_rst_disable(SYSCONx)					(SYSCONx->LVDCR |= LVR_SRT_MSK | LVD_KEY)

#define csp_lvd_reset_regs(SYSCONx)						(SYSCONx -> LVDCR = LVD_KEY | LVD_DIS)

#define csp_lvd_flag(SYSCONx)							((SYSCONx->LVDCR) & LVD_FLAG)

#define csp_syscon_int_enable(SYSCONx,eInt)				(SYSCONx->IMER |= eInt)			///\param eInt \ref syscon_int_e

#define csp_syscon_int_disable(SYSCONx,eInt)			(SYSCONx->IMDR |= eInt)			///\param eInt \ref syscon_int_e

#define csp_syscon_clr_isr(SYSCONx,eInt)				(SYSCONx->ICR = eInt)			///\param eInt \ref syscon_int_e

#define csp_syscon_get_isr(SYSCONx)						(SYSCONx->MISR)

#define csp_iwdt_set_ovt(SYSCONx,eLvl)					(SYSCONx -> IWDCR = (SYSCONx -> IWDCR & (~IWDT_OVT_MSK)) | eLvl << IWDT_OVT_POS | IWDTCR_KEY)	///\param eLvl \ref iwdt_ovt_e

#define csp_iwdt_set_int(SYSCONx,eLvl)					(SYSCONx -> IWDCR = (SYSCONx -> IWDCR & (~IWDT_INTV_MSK)) | eLvl << IWDT_INTV_POS | IWDTCR_KEY)	///\param eLvl \ref iwdt_intv_e

#define csp_iwdt_disable(SYSCONx)						(SYSCONx -> IWDEDR = DIS_IWDT | IWDTE_KEY)

#define csp_iwdt_clr(SYSCONx)							({\
															SYSCONx -> IWDCNT = (SYSCONx -> IWDCNT & (~IWDT_CLR_MSK)) | IWDT_CLR << IWDT_CLR_POS;\
															while((SYSCONx->IWDCNT & IWDT_CLR_BUSY) == IWDT_CLR_BUSY);\
														})

#define csp_iwdt_get_cnt(SYSCONx)						(SYSCONx->IWDCNT & 0xfff)
	
#define csp_iwdt_rd_st(SYSCONx)							((SYSCONx->IWDCR & IWDT_ST) >> 12)

#define csp_rst_clr_st(SYSCONx)							(SYSCONx->RSR = RSR_MSK)

#define csp_iwdt_debug_enable(SYSCONx)					(SYSCONx->IWDCR = (SYSCONx->IWDCR & (~IWDT_DBGEN_MSK)) | (1 << IWDT_DBGEN_POS) |IWDTCR_KEY)

#define csp_iwdt_debug_disable(SYSCONx)					(SYSCONx->IWDCR = (SYSCONx->IWDCR & (~IWDT_DBGEN_MSK)) | (0 << IWDT_DBGEN_POS) |IWDTCR_KEY)

#define csp_rst_rd_st(SYSCONx)							(SYSCONx->RSR)

#define csp_rst_clr_rsr(SYSCONx,bySrcMsk)				(SYSCONx->RSR = bySrcMsk)

#define csp_sramcheck_set_times(SYSCONx,wVal)			(SYSCONx -> RAMCHK = (SYSCONx -> RAMCHK & (~RAMCHK_TIMES_MSK)) | wVal)

#define csp_sramcheck_enable(SYSCONx)					(SYSCONx -> RAMCHK = (SYSCONx -> RAMCHK & (~RAMCHK_MSK)) | RAMCHK_EN)

#define csp_sramcheck_disable(SYSCONx)					(SYSCONx -> RAMCHK = (SYSCONx -> RAMCHK & (~RAMCHK_MSK)) | RAMCHK_DIS)

#define csp_sramcheck_rst(SYSCONx)						(SYSCONx -> RAMCHK = (SYSCONx -> RAMCHK & (~RAMCHK_RST_MSK)) | RAMCHK_RST)

#define csp_sramcheck_int(SYSCONx)						(SYSCONx -> RAMCHK = (SYSCONx -> RAMCHK & (~RAMCHK_RST_MSK)) | RAMCHK_INT)

#define csp_flashcheck_set_times(SYSCONx,wVal)			(SYSCONx -> EFLCHK = (SYSCONx -> EFLCHK & (~EFLCHK_TIMES_MSK)) | wVal)
	
#define csp_flashcheck_enable(SYSCONx)					(SYSCONx -> EFLCHK = (SYSCONx -> EFLCHK & (~EFLCHK_MSK)) | EFLCHK_EN)

#define csp_flashcheck_disable(SYSCONx)					(SYSCONx -> EFLCHK = (SYSCONx -> EFLCHK & (~EFLCHK_MSK)) | EFLCHK_DIS)

#define csp_emcm_enable(SYSCONx)						(SYSCONx -> GCER = EM_CM_EN)

#define csp_emcm_disable(SYSCONx)						(SYSCONx -> GCDR = EM_CM_EN)

#define csp_emcm_rst_enable(SYSCONx)					(SYSCONx -> GCER = EM_CMRST)

#define csp_emcm_rst_disable(SYSCONx)					(SYSCONx -> GCDR = EM_CMRST)

#define csp_escm_enable(SYSCONx)						(SYSCONx -> GCER = ES_CM_EN)

#define csp_escm_disable(SYSCONx)						(SYSCONx -> GCDR = ES_CM_EN)

#define csp_escm_rst_enable(SYSCONx)					(SYSCONx -> GCER = ES_CMRST)

#define csp_escm_rst_disable(SYSCONx)					(SYSCONx -> GCDR = ES_CMRST)

//pwrcr
///\param eVcref,eVddcore \ref sleep_vcref_e ,sleep_vddcore_e
#define csp_sleep_vos_config(SYSCONx,eVcref,eVddcore)	({\
															SYSCONx->PWRKEY = PWRCR_KEY;\
															SYSCONx->PWRCR = (SYSCONx->PWRCR & (~VOS_SLEEP_CFG_MSK)) | (eVcref << VOS_SLEEP_VCREF_P0S)| (eVddcore << VOS_SLEEP_VDDCORE_P0S);\
														)}				 

#define csp_sleep_vos_config_enable(SYSCONx)			({\
															SYSCONx->PWRKEY = PWRCR_KEY;\
															SYSCONx->PWRCR = (SYSCONx->PWRCR & (~VOS_SLEEP_PD_CTRL_P0S_MSK)) | (VOS_SLEEP_SLP_CTRL_P0S_MSK);\
														})

#define csp_sleep_vos_enable(SYSCONx)					({\
															SYSCONx->PWRKEY = PWRCR_KEY;\
															SYSCONx->PWRCR |=  VOS_SLEEP_EN_P0S_MSK;\
														})

#define csp_sleep_vos_disable(SYSCONx)					({\
															SYSCONx->PWRKEY = PWRCR_KEY;\
															SYSCONx->PWRCR &= (~VOS_SLEEP_EN_P0S_MSK);\
														})


//wkcr 
#define csp_set_deepsleep_mode(SYSCONx,eDpSleep)		(SYSCONx->WKCR = (SYSCONx->WKCR & (~DPSLEEP_MODE_MSK)) | (eDpSleep  << DPSLEEP_MODE_P0S))		///\param eDpSleep \ref deepsleep_mode_e

#define csp_snooze_touch_power_enable(SYSCONx)			(SYSCONx->WKCR |= SNOOZE_TOUCH_MSK)

#define csp_snooze_touch_power_disable(SYSCONx)			(SYSCONx->WKCR &= (~SNOOZE_TOUCH_MSK))

#define csp_snooze_lcd_power_enable(SYSCONx)			(SYSCONx->WKCR |= SNOOZE_LCD_MSK)

#define csp_snooze_lcd_power_disable(SYSCONx)			(SYSCONx->WKCR &= (~SNOOZE_LCD_MSK)

//UREG0/1/2/3
#define csp_get_ureg(SYSCONx,byNum)						(*(uint32_t *)((uint32_t)&SYSCONx->UREG0 + (byNum * 4)))

#define csp_set_ureg(SYSCONx,byNum,wVal)				(*(uint32_t *)((uint32_t)&SYSCONx->UREG0 + (byNum * 4)) = wVal)

//IDCCR
#define csp_set_swrst(SYSCONx,eSwRst)					(SYSCONx->IDCCR |= (SYSCON_IDKEY | (eSwRst << SYSCON_SWRST_POS)))		///\param eSwRst \ref sw_rst_e

#define csp_exi_flt(SYSCONx,byFltNum,eFlt)				(SYSCONx->EXICFG = (SYSCONx->EXICFG & ~EXI_FLT_MSK(byFltNum)) | (eFlt << EXI_FLT_POS(byFltNum)))	///\param eFlt \ref exi_flt_e

#define csp_set_swd_lock(SYSCONx)						(SYSCONx->DBGCR = 0x00)
	
#define csp_set_swd_unlock(SYSCONx)						(SYSCONx->DBGCR = 0x5a)

#define csp_cqcr_enable(SYSCONx)						(SYSCONx->CQCR |=  CQCR_EN_MSK)

#define csp_cqcr_disable(SYSCONx)						(SYSCONx->CQCR &= (~CQCR_EN_MSK))

#define csp_set_cqcr_ref_sel(SYSCONx,eRefSel)			(SYSCONx->CQCR = (SYSCONx->CQCR & (~CQCR_REFSEL_MSK)) | (eRefSel  << CQCR_REFSEL_POS))		///\param eRefSel \ref cqcr_refsel_e

#define csp_set_cqcr_src_sel(SYSCONx,eSrcSel)			(SYSCONx->CQCR = (SYSCONx->CQCR & (~CQCR_SRCSEL_MSK)) | (eSrcSel  << CQCR_SRCSEL_POS))		///\param eSrcSel \ref cqcr_srcsel_e

#define csp_set_cqcr_value(SYSCONx,wVal)				(SYSCONx->CQCR = (wVal  << CQCR_CQRVAL_POS))

#define csp_get_cqsr(SYSCONx)							((uint32_t) ((SYSCONx->CQSR) & 0xFFFFF))

#endif  /* _CSP_SYSCON_H*/

