/***********************************************************************//** 
 * \file  csp_syscon.h
 * \brief  SYSCON description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-10 <td>V0.0  <td>WNN   <td>initial
 * <tr><td> 2023-9-22 <td>V0.1  <td>GQQ   <td>code normalization
 * </table>
 * *********************************************************************
*/

#ifndef _CSP_SYSCON_H_
#define _CSP_SYSCON_H_

#include "soc.h"
#include "csi_core.h"

/// \struct csp_syscon_t
/// \brief SYSCON reg description
typedef volatile struct {                   	  /*!< SYSCON Structure                         */
	__IOM uint32_t	IDCCR;                        /*!< 0x000: Identification & System Controller Clock Control Register */
	__OM  uint32_t	GCER;                         /*!< 0x004: System Controller General Control Enable Register */
	__OM  uint32_t	GCDR;                         /*!< 0x008: System Controller General Control Disable Register */
	__IM  uint32_t	GCSR;                         /*!< 0x00C: System Controller General Control Status Register */
	__IM  uint32_t	CKST;                         /*!< 0x010*/
	__IOM uint32_t	RAMCHK;                       /*!< 0x014*/
	__IOM uint32_t	EFLCHK;                       /*!< 0x018*/
	__IOM uint32_t	SCLKCR;                       /*!< 0x01C: System Controller System Clock Selection & Division Register */
	__OM  uint32_t	PCLKCR;                       /*!< 0x020: System Controller Peripheral Clock Selection & Division Register */
	__IOM uint32_t	EXICFG;                       /*!< 0x024*/
	__OM  uint32_t	PCER0;                        /*!< 0x028: System Controller Peripheral Clock Enable Register */
	__OM  uint32_t	PCDR0;                        /*!< 0x02C: System Controller Peripheral Clock Disable Register */
	__IM  uint32_t	PCSR0;                        /*!< 0x030: System Controller Peripheral Clock Status Register */
	__OM  uint32_t	PCER1;                        /*!< 0x034: System Controller Peripheral Clock Enable Register */
	__OM  uint32_t	PCDR1;                        /*!< 0x038: System Controller Peripheral Clock Disable Register */
	__IM  uint32_t	PCSR1;                        /*!< 0x03C: System Controller Peripheral Clock Status Register */
	__IOM uint32_t	OSTR;                         /*!< 0x040: System Controller External OSC Stable Time Control Register */
	__IOM uint32_t	PLLCR;                        /*!< 0x044: System Controller PLL Stable Time Control Register */
	__IM  uint32_t	_RSVD0;                       /*!< 0x048: System Controller PLL PMS Value Control Register */
	__IOM uint32_t	LVDCR;                        /*!< 0x04C: System Controller LVD Control Register */
	__IOM uint32_t	CLCR;                         /*!< 0x050: System Controller IMOSC Fine Adjustment Register*/
	__IOM uint32_t	PWRCR;                        /*!< 0x054: System Controller Power Control Register */
	__IOM uint32_t	PWRKEY;                       /*!< 0x058: System Controller Power Control Register */
	__IM  uint32_t	_RSVD1;                       /*!< 0x05C: */
	__IM  uint32_t	_RSVD2;                       /*!< 0x060: */
	__IOM uint32_t	OPT1;                         /*!< 0x064: System Controller OSC Trim Control Register */
	__IM  uint32_t	OPT0;                         /*!< 0x068: System Controller Protection Control Register */
	__IOM uint32_t	WKCR;                         /*!< 0x06C: System Controller Clock Quality Check Control Register */
	__IM  uint32_t	_RSVD3;                       /*!< 0x070: System Controller Clock Quality Check Control Register */
	__OM  uint32_t	IMER;                         /*!< 0x074: System Controller Interrupt Enable Register */
	__OM  uint32_t	IMDR;                         /*!< 0x078: System Controller Interrupt Disable Register */
	__IOM uint32_t	IMCR;                         /*!< 0x07C: System Controller Interrupt Mask Register */
	__OM  uint32_t	IAR;                          /*!< 0x080: System Controller Interrupt Active Register */
	__OM  uint32_t	ICR;                          /*!< 0x084: System Controller Clear Status Register */
	__IM  uint32_t	RISR;                         /*!< 0x088: System Controller Raw Interrupt Status Register */
	__IM  uint32_t	MISR;                         /*!< 0x08C: System Controller Raw Interrupt Status Register */
	__IOM uint32_t	RSR;                          /*!< 0x090: System Controller Raw Interrupt Status Register */
	__IOM uint32_t	EXIRT;                        /*!< 0x094: System Controller Reset Status Register */
	__IOM uint32_t	EXIFT;                        /*!< 0x098: System Controller External Interrupt Mode 1 (Positive Edge) Register */
	__OM  uint32_t	EXIER;                        /*!< 0x09C: System Controller External Interrupt Mode 2 (Negative Edge) Register */
	__OM  uint32_t	EXIDR;                        /*!< 0x0A0: System Controller External Interrupt Enable Register */
	__IM  uint32_t	EXIMR;                        /*!< 0x0A4: System Controller External Interrupt Disable Register */
	__IOM uint32_t	EXIAR;                        /*!< 0x0A8: System Controller External Interrupt Mask Register */
	__IOM uint32_t	EXICR;                        /*!< 0x0AC: System Controller External Interrupt Active Register */
	__IOM uint32_t	EXIRS;                        /*!< 0x0B0: System Controller External Interrupt Clear Status Register */
	__IOM uint32_t	IWDCR;                        /*!< 0x0B4: System Controller Independent Watchdog Control Register */
	__IOM uint32_t	IWDCNT;                       /*!< 0x0B8: SystCem Controller Independent Watchdog Counter Value Register */
	__IOM uint32_t	IWDEDR;                       /*!< 0x0BC: System Controller Independent Watchdog Enable/disable Register*/
	__IOM uint32_t	IOMAP0;                       /*!< 0x0C0: Customer Information Content mirror of 1st byte*/
	__IOM uint32_t	IOMAP1;                       /*!< 0x0C4: Customer Information Content mirror of 1st byte*/
	__IM  uint32_t	_RSVD4;                    	  /*!< 0x0C8: Customer Information Content mirror of 1st byte*/
	__IM  uint32_t	_RSVD5;                       /*!< 0x0CC: Customer Information Content mirror of 1st byte*/
	__IM  uint32_t	FINF0;                        /*!< 0x0D0: Customer Information Content mirror of 1st byte*/
	__IM  uint32_t	FINF1;                        /*!< 0x0D4: Customer Information Content mirror of 1st byte*/
	__IM  uint32_t	FINF2;                        /*!< 0x0D8: Customer Information Content mirror of 1st byte*/
	__IM  uint32_t	_RSVD6;                       /*!< 0x0DC: Customer Information Content mirror of 1st byte*/
	__IM  uint32_t	ERRINF;                       /*!< 0x0E0:*/
	__IM  uint32_t	UID0 ;                        /*!< 0x0E4: Customer Information Content mirror of 1st byte*/
	__IM  uint32_t	UID1 ;                        /*!< 0x0E8: Customer Information Content mirror of 1st byte*/
	__IM  uint32_t	UID2 ;                        /*!< 0x0EC: Customer Information Content mirror of 1st byte*/
	__IOM uint32_t	PWROPT;                       /*!< 0x0F0: Power recovery timmming control */
	__IOM uint32_t	EVTRG;                        /*!< 0x0F4: Trigger gen                                    */
	__IOM uint32_t	EVPS;                      	  /*!< 0x0F8: Trigger prs                                    */
	__OM  uint32_t	EVSWF;                        /*!< 0x0FC: Trigger software force                         */
	__IOM uint32_t	UREG0;                        /*!< 0x100: User defined reg0                              */
	__IOM uint32_t	UREG1;                        /*!< 0x104: User defined reg1                              */
	__IOM uint32_t	UREG2;                        /*!< 0x108: User defined reg0                              */
	__IOM uint32_t	TRIMUREG;                     /*!< 0x10C: User defined reg0                              */
	__IM  uint32_t	_RSVD7[2];					
	__IOM uint32_t	CQCR;                         /*!< 0x118: Clock Quality check control reg                */
	__IM  uint32_t	CQSR;						  /*!< 0x11C: Clock Qualifty check control reg  			 */
	__IM  uint32_t	_RSVD8[2];
	__IOM uint32_t	DBGCR;						  /*!< 0x128: Debug Control Register			   */
	__IOM uint32_t	BRPKEY;						  /*!< 0x12c: SRBREG, ERBREG  protect reg			   */	
	__IOM uint32_t	SRBREG;						  /*!< 0x130: SRBREG Register			   */	
	__IOM uint32_t	ERBREG;						  /*!< 0x134: ERBREG Register			   */	
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
//#define ISOSC 		(0x01ul)
//#define IMOSC 		(0x01ul << 1)
//#define ESOSC 		(0x01ul << 2)
//#define EMOSC 		(0x01ul << 3)
//#define HFOSC 		(0x01ul << 4)
//#define PLL 		(0x01ul << 5)
#define SYSCLK		(0x01ul << 8)
//#define SYSTICK 	(0x01ul << 11)

typedef enum{
	ISOSC  = 0x01ul,
	IMOSC  = (0x01ul << 1),
	ESOSC  = (0x01ul << 2), 
	EMOSC  = (0x01ul << 3),
	HFOSC  = (0x01ul << 4),
	PLL    = (0x01ul << 5)
}clk_src_e;

#define CKST_MSK	0x13f
#define GCSR_MSK	0x0ffb1f

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

#define HCLK_DIV_POS (8)
#define HCLK_DIV_MSK (0xful<<HCLK_DIV_POS)
#define SCLK_KEY (0xd22dul<<16)


/// PCLKCR
#define PCLK_DIV_POS (8)
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
#define HFO_POS (4)
#define HFO_MSK (0x3ul<<HFO_POS)
#define CLO_SRC_POS (8)
#define CLO_SRC_MSK (0xful << CLO_SRC_POS)

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

#define CLO_DIV_MSK 	(0x7ul << CLO_DIV_POS)
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


typedef enum {
	WKUP_IWDT = 8,
	WKUP_RTC,
	WKUP_LPT,
	WKUP_LVD
} wakeup_src_e;


typedef enum{
	ISOSC_INT_ST 	= (0x1<<0),
	IMOSC_INT_ST 	= (0x1<<1),
	ESOSC_INT_ST 	= (0x1<<2),
	EMOSC_INT_ST 	= (0x1<<3),
	HFOSC_INT_ST 	= (0x1<<4),
	PLL_INT_ST 	    = (0x1<<5),
	SYSTICK_INT_ST 	= (0x1<<7),
	IWDT_INT 		= (0x1<<8),
	RAM_INT_ERR 	= (0x1<<10),
	LVD_INT 		= (0x1<<11),
	EFL_INT_ERR 	= (0x1<<13),
	OPL_INT_ERR 	= (0x1<<14),
	PLL_INT_ERR 	= (0x1<<15),
	ESFAIL_INT 		= (0x1<<16),
	CQCDNE_INT 		= (0x1<<17),
	EMFAIL_INT 		= (0x1<<18),
	EV0TRG_INT 		= (0x1<<19),
	EV1TRG_INT 		= (0x1<<20),
	EV2TRG_INT 		= (0x1<<21),
	EV3TRG_INT 		= (0x1<<22),
	CMD_INT_ERR		= (0x1<<29),
	SYSCON_INT_ALL  = 0x207fedbf
}syscon_int_e;

/// EXI interrupt regs: EXIER/EXIDR/EXIRS/EXIAR/EXICR/EXIRS

/// RSR reset register
#define RSR_MSK		(0xffff)
typedef enum{
	PORST			= (0x1ul <<0),
	LVRST			= (0x1ul <<1),
	EXTRST			= (0x1ul <<2),
	IWDTRST			= (0x1ul <<4),
	ESCMRST 		= (0x1ul <<5),
	EMCMRST 		= (0x1ul <<6),
	CPURST			= (0x1ul <<7),
	SWRST			= (0x1ul <<8),
	CPUFAULTRST		= (0x1ul <<9),
	PLLUNLOCKRST	= (0x1ul <<10),
	SRAMRST 		= (0x1ul <<11),
	EFLRST			= (0x1ul <<12),
	WWDTRST			= (0x1ul <<13),
	CPU1RST			= (0x1ul <<15),
	ALLRSTST 		= 0xFFFF
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


#define CMRTRIM_VULUE_MSK        (0x1f)  
#define CMRTRIML_VULUE_POS       (5) 
#define CMRTRIML_VULUE_MSK       (0x1f << CMRTRIML_VULUE_POS)  

#define CMRTRIM_CTL_POS          (10)
#define CMRTRIM_CTL_MSK          (1 << 10)
#define CMRTRIML_CTL_POS         (11)
#define CMRTRIML_CTL_MSK         (1 << 11)


//CQCR
#define CQCR_EN_POS              (0) 
#define CQCR_EN_MSK              (0x1 << CQCR_EN_POS)  

#define CQCR_REFSEL_POS          (4) 
#define CQCR_REFSEL_MSK          (0x3 << CQCR_REFSEL_POS)  

typedef enum{
	CQCR_REFSEL_EMOSC,
	CQCR_REFSEL_ESOSC,	
}cqcr_refsel_e;

#define CQCR_SRCSEL_POS          (6) 
#define CQCR_SRCSEL_MSK          (0x3 << CQCR_SRCSEL_POS)  
typedef enum{
	CQCR_SRCSEL_IMOSC,
	CQCR_SRCSEL_ESOSC,
	CQCR_SRCSEL_ISOSC,
	CQCR_SRCSEL_HFOSC
}cqcr_srcsel_e;

#define CQCR_CQRVAL_POS         (8) 
#define CQCR_CQRVAL_MSK         (0x3FFFF << CQCR_CQRVAL_POS)  

#define CQSR_MSK				0xFFFFF

//dbgcr

#define SWD_LOCK			0
#define SWD_UNLOCK			0x5a

///inline functions

//CKST
static inline uint32_t csp_get_ckst(csp_syscon_t *ptSysconBase)
{
	return (uint32_t) ((ptSysconBase->CKST) & CKST_MSK);
}

//GCSR
static inline uint32_t csp_get_gcsr(csp_syscon_t *ptSysconBase)
{
	return (uint32_t) ((ptSysconBase->GCSR) & GCSR_MSK);
}

//SCLCK
static inline void csp_set_clksrc(csp_syscon_t *ptSysconBase, uint32_t wClkSrc)
{
	ptSysconBase->SCLKCR = (ptSysconBase->SCLKCR & (~SYSCLK_SRC_MSK)) | SCLK_KEY | wClkSrc;
	while((ptSysconBase->CKST & SYSCLK) == 0);
}

//PLLCR
static inline void csp_pll_set_clk_src(csp_syscon_t *ptSysconBase, pll_clk_sel_e eClkSel)
{
	ptSysconBase->PLLCR = (ptSysconBase->PLLCR & (~PLL_CLK_SEL_MSK)) | eClkSel << PLL_CLK_SEL_POS;
}

static inline uint8_t csp_pll_get_clk_src(csp_syscon_t *ptSysconBase)
{
	return (uint8_t)(ptSysconBase->PLLCR & PLL_CLK_SEL_MSK);
}

static inline void csp_pll_set_div_m(csp_syscon_t *ptSysconBase, uint8_t byDivM)
{
	ptSysconBase->PLLCR = (ptSysconBase->PLLCR & (~PLL_DIVM_MSK)) | byDivM << PLL_DIVM_POS;
}

static inline void csp_pll_set_nul(csp_syscon_t *ptSysconBase, uint8_t byNul)
{
	ptSysconBase->PLLCR = (ptSysconBase->PLLCR & (~PLL_NUL_MSK)) | byNul << PLL_NUM_POS;
}

static inline void csp_pll_set_ckp_div(csp_syscon_t *ptSysconBase, uint8_t byCkp_Div)
{
	ptSysconBase->PLLCR = (ptSysconBase->PLLCR & (~PLL_CKP_DIV_MSK)) | byCkp_Div << PLL_CKP_DIV_POS;
}

static inline void csp_pll_clk_enable(csp_syscon_t *ptSysconBase)
{
	ptSysconBase->PLLCR = (ptSysconBase->PLLCR & ~PLL_CKPEN_MSK)| (ENABLE << PLL_CKPEN_POS);
}

static inline void csp_pll_clk_disable(csp_syscon_t *ptSysconBase)
{
	ptSysconBase->PLLCR = (ptSysconBase->PLLCR & ~PLL_CKPEN_MSK)| (DISABLE << PLL_CKPEN_POS);
}

static inline void csp_pll_set_ckq_div(csp_syscon_t *ptSysconBase, uint8_t byCkp_Div)
{
	ptSysconBase->PLLCR = (ptSysconBase->PLLCR & (~PLL_CKQ_DIV_MSK)) | byCkp_Div << PLL_CKQ_DIV_POS;
}

//OPT1
static inline void csp_set_hfosc_freq(csp_syscon_t *ptSysconBase, uint32_t wFreq)
{
	ptSysconBase->OPT1 = (ptSysconBase->OPT1 & (~HFO_MSK)) | wFreq << HFO_POS;
}
static inline uint32_t csp_get_hfosc_freq(csp_syscon_t *ptSysconBase)
{
	return (uint32_t) (((ptSysconBase->OPT1) & HFO_MSK) >> HFO_POS);
}
static inline void csp_set_imosc_freq(csp_syscon_t *ptSysconBase, uint32_t wFreq)
{
	ptSysconBase->OPT1 = (ptSysconBase->OPT1 & (~IMO_MSK)) | wFreq;
}
static inline uint32_t csp_get_imosc_freq(csp_syscon_t *ptSysconBase)
{
	return (uint32_t) ((ptSysconBase->OPT1) & IMO_MSK);
}

static inline void csp_set_sdiv(csp_syscon_t *ptSysconBase, uint32_t wSdiv)
{
	ptSysconBase->SCLKCR =  (ptSysconBase->SCLKCR & (~HCLK_DIV_MSK)) | SCLK_KEY | (wSdiv << HCLK_DIV_POS);
}

static inline void csp_set_pdiv(csp_syscon_t *ptSysconBase, uint32_t wSdiv)
{
	ptSysconBase->PCLKCR =  (ptSysconBase->PCLKCR & (~PCLK_DIV_MSK)) | PCLK_KEY | (wSdiv << PCLK_DIV_POS);
}

static inline uint32_t	csp_get_pdiv(csp_syscon_t *ptSysconBase)
{
	return (uint32_t ) (((ptSysconBase->PCLKCR) & PCLK_DIV_MSK) >> PCLK_DIV_POS);
}

static inline uint32_t csp_get_clk_src(csp_syscon_t *ptSysconBase)
{
	return (uint32_t) ((ptSysconBase->SCLKCR) & SYSCLK_SRC_MSK);
}

static inline  uint32_t csp_get_hclk_div(csp_syscon_t *ptSysconBase)
{
	return (uint32_t) (((ptSysconBase->SCLKCR) & HCLK_DIV_MSK) >> HCLK_DIV_POS);
}

static inline  uint8_t csp_get_im_src(csp_syscon_t *ptSysconBase)
{
	return (uint8_t) (ptSysconBase->OPT1 & IMO_MSK);
}

static inline void csp_set_clo_src(csp_syscon_t *ptSysconBase, clo_src_e wCloSrc)
{
	ptSysconBase->OPT1 = (ptSysconBase->OPT1 & (~CLO_SRC_MSK)) | wCloSrc << CLO_SRC_POS;
}

static inline void csp_set_clo_div(csp_syscon_t *ptSysconBase, clo_div_e wCloDiv)
{
	ptSysconBase->OPT1 = (ptSysconBase->OPT1 & (~CLO_DIV_MSK)) | wCloDiv << CLO_DIV_POS;
}

static inline void csp_pcer0_clk_enable(csp_syscon_t *ptSysconBase, uint32_t wIdx)
{
	ptSysconBase->PCER0 = (0x01ul << wIdx);
}

static inline void csp_pcer1_clk_enable(csp_syscon_t *ptSysconBase, uint32_t wIdx)
{
	ptSysconBase->PCER1 = (0x01ul << wIdx);
}

static inline void csp_pder0_clk_disable(csp_syscon_t *ptSysconBase, uint32_t wIdx)
{
	ptSysconBase->PCDR0 = (0x01ul << wIdx);
}

static inline void csp_pder1_clk_disable(csp_syscon_t *ptSysconBase, uint32_t wIdx)
{
	ptSysconBase->PCDR1 = (0x01ul << wIdx);
}

static inline void csp_src_clk_enable(csp_syscon_t *ptSysconBase, clk_src_e eSrcClk)
{
	ptSysconBase->GCER = eSrcClk;
}

static inline void csp_src_clk_disable(csp_syscon_t *ptSysconBase,clk_src_e eSrcClk)
{
	ptSysconBase->GCDR = eSrcClk;
}

static inline void csp_clk_pm_enable(csp_syscon_t *ptSysconBase, clk_pm_e eClk)
{
	ptSysconBase->GCER = (0x1ul << eClk);
}

static inline void csp_clk_pm_disable(csp_syscon_t *ptSysconBase, clk_pm_e eClk)
{
	ptSysconBase->GCDR = (0x1ul << eClk);
}

static inline void csp_emcm_enable(csp_syscon_t *ptSysconBase)
{
	ptSysconBase -> GCER = EM_CM_EN;
}

static inline void csp_emcm_disable(csp_syscon_t *ptSysconBase)
{
	ptSysconBase -> GCDR = EM_CM_EN;
}

static inline void csp_emcm_rst(csp_syscon_t *ptSysconBase)
{
	ptSysconBase -> GCER = EM_CMRST;
}

static inline void csp_emcm_switch_im(csp_syscon_t *ptSysconBase)
{
	ptSysconBase -> GCDR = EM_CMRST;
}

static inline void csp_escm_enable(csp_syscon_t *ptSysconBase)
{
	ptSysconBase -> GCER = ES_CM_EN;
}

static inline void csp_escm_disable(csp_syscon_t *ptSysconBase)
{
	ptSysconBase -> GCDR = ES_CM_EN;
}

static inline void csp_escm_rst(csp_syscon_t *ptSysconBase)
{
	ptSysconBase -> GCER = ES_CMRST;
}

static inline void csp_escm_switch_im(csp_syscon_t *ptSysconBase)
{
	ptSysconBase -> GCDR = ES_CMRST;
}

static inline void  csp_em_lfmd_enable(csp_syscon_t *ptSysconBase)	
{
	ptSysconBase->OSTR |= EM_LF;
}

static inline void  csp_em_lfmd_disable(csp_syscon_t *ptSysconBase)	
{
	ptSysconBase->OSTR &= ~EM_LF;
}

static inline void csp_em_flt_enable(csp_syscon_t *ptSysconBase)
{
	ptSysconBase->OSTR = (ptSysconBase->OSTR & ~EM_FLTEN_MSK)| (ENABLE << EM_FLTEN_POS);	
}

static inline void csp_em_flt_disable(csp_syscon_t *ptSysconBase)
{
	ptSysconBase->OSTR = (ptSysconBase->OSTR & ~EM_FLTEN_MSK)| (DISABLE << EM_FLTEN_POS);	
}

static inline void csp_em_set_flt(csp_syscon_t *ptSysconBase,em_fltsel_e eFltSel)
{
	ptSysconBase->OSTR = (ptSysconBase->OSTR & ~EM_FLTSEL_MSK)| (eFltSel << EM_FLTSEL_POS);	
}

static inline void csp_eflash_lpmd_enable(csp_syscon_t *ptSysconBase)
{
	ptSysconBase->OPT1 = (ptSysconBase->OPT1 & ~FLASH_LPMODE_MSK)| (ENABLE << FLASH_LPMODE_POS);
}

static inline void csp_eflash_lpmd_disable(csp_syscon_t *ptSysconBase)
{
	ptSysconBase->OPT1 = (ptSysconBase->OPT1 & ~FLASH_LPMODE_MSK)| (DISABLE << FLASH_LPMODE_POS);
}

static inline void csp_sram1_set_func(csp_syscon_t *ptSysconBase, sram1_func_e eSram1Func)
{
	ptSysconBase->OPT1 = (ptSysconBase->OPT1 & ~SRAM1FUNCCTRL_MSK)| (eSram1Func << SRAM1FUNCCTRL_POS);
}

static inline void csp_sram_set_blk(csp_syscon_t *ptSysconBase, sram_blk_e eSramBlk)
{
	ptSysconBase->OPT1 = (ptSysconBase->OPT1 & ~SRAMBLKCTRL_MSK)| (eSramBlk << SRAMBLKCTRL_POS);
}

static inline void csp_nmi_int_enable(csp_syscon_t *ptSysconBase, nmi_sel_e eNmiSel)
{
	ptSysconBase->OPT1 |=  eNmiSel;
}

static inline void csp_nmi_int_disable(csp_syscon_t *ptSysconBase, nmi_sel_e eNmiSel)
{
	ptSysconBase->OPT1 &= ~eNmiSel;
}

static inline void csp_em_set_gain(csp_syscon_t *ptSysconBase, uint8_t byGn)
{
	ptSysconBase->OSTR = (ptSysconBase->OSTR & (~EM_GM_MSK)) | (byGn << EM_GM_POS);
}
static inline void csp_es_set_gain(csp_syscon_t *ptSysconBase, uint8_t byGn)
{
	ptSysconBase->OSTR = (ptSysconBase->OSTR & (~ES_GM_MSK)) | (byGn << ES_GM_POS);
}

//lvdcr
static inline void csp_lvr_set_lvl(csp_syscon_t *ptSysconBase,lvr_level_e eLvl)
{
	ptSysconBase->LVDCR = (ptSysconBase->LVDCR & (~LVR_LV_MSK)) | (eLvl) << LVR_LV_POS | LVD_KEY;
}

static inline void csp_lvd_set_lvl(csp_syscon_t *ptSysconBase,lvd_level_e eLvl)
{
	ptSysconBase->LVDCR = (ptSysconBase->LVDCR & (~LVD_LV_MSK)) | (eLvl) << LVD_LV_POS | LVD_KEY;
}

static inline void csp_lvd_set_int_pol(csp_syscon_t *ptSysconBase, lvdint_pol_e ePol)
{
	ptSysconBase->LVDCR = (ptSysconBase->LVDCR & (~LVDINT_POL_MSK)) | ePol << LVDINT_POL_POS | LVD_KEY;
}

static inline void csp_lvd_lvr_enable(csp_syscon_t *ptSysconBase)
{
	ptSysconBase->LVDCR = (ptSysconBase->LVDCR &(~LVDEN_MSK)) | LVD_KEY; 
}

static inline void csp_lvd_lvr_disable(csp_syscon_t *ptSysconBase)
{
	ptSysconBase->LVDCR = (ptSysconBase->LVDCR &(~LVDEN_MSK))| LVD_DIS | LVD_KEY;
}

static inline void csp_lvr_rst_enable(csp_syscon_t *ptSysconBase)
{
	ptSysconBase->LVDCR = (ptSysconBase->LVDCR &(~LVR_SRT_MSK)) | LVD_KEY;
}

static inline void csp_lvr_rst_disable(csp_syscon_t *ptSysconBase)
{
	ptSysconBase->LVDCR |= LVR_SRT_MSK | LVD_KEY;
}

static inline uint32_t csp_lvd_get_flag(csp_syscon_t *ptSysconBase)
{
	return ((ptSysconBase->LVDCR) & LVD_FLAG);
}

static inline uint16_t csp_lvd_get_int_lvl(csp_syscon_t *ptSysconBase)
{
	return (uint16_t)((ptSysconBase->LVDCR) & LVD_LV_MSK);
}

static inline uint16_t csp_lvr_get_lvl(csp_syscon_t *ptSysconBase)
{
	return (uint16_t)(ptSysconBase->LVDCR & LVR_LV_MSK);
}

//imer
static inline void csp_syscon_int_enable(csp_syscon_t *ptSysconBase, syscon_int_e eInt)
{
	ptSysconBase->IMER = eInt; 
}
static inline void csp_syscon_int_disable(csp_syscon_t *ptSysconBase, syscon_int_e eInt)
{
	ptSysconBase->IMDR = eInt;
}

static inline void csp_syscon_clr_isr(csp_syscon_t *ptSysconBase, syscon_int_e eInt)
{
	ptSysconBase->ICR = eInt; 
}

static inline uint32_t csp_syscon_get_isr(csp_syscon_t *ptSysconBase)
{
	return (ptSysconBase->MISR);
}

//iwdt
static inline void csp_iwdt_set_ovt(csp_syscon_t *ptSysconBase, iwdt_ovt_e eLvl)
{
	ptSysconBase -> IWDCR = (ptSysconBase -> IWDCR & (~IWDT_OVT_MSK)) | eLvl << IWDT_OVT_POS | IWDTCR_KEY;
}

static inline void csp_iwdt_set_int_lvl(csp_syscon_t *ptSysconBase, iwdt_intv_e eLvl)
{
	ptSysconBase -> IWDCR = (ptSysconBase -> IWDCR & (~IWDT_INTV_MSK)) | eLvl << IWDT_INTV_POS | IWDTCR_KEY;
}

static inline void csp_iwdt_disable(csp_syscon_t *ptSysconBase)
{
	ptSysconBase -> IWDEDR = DIS_IWDT | IWDTE_KEY;
}

static inline void csp_iwdt_enable(csp_syscon_t *ptSysconBase)
{
	ptSysconBase -> IWDEDR = EN_IWDT | IWDTE_KEY;
	while((ptSysconBase->IWDCR & IWDT_ST) != IWDT_BUSY);
}

static inline void csp_iwdt_clr(csp_syscon_t *ptSysconBase)
{
	ptSysconBase -> IWDCNT = (ptSysconBase -> IWDCNT & (~IWDT_CLR_MSK)) | IWDT_CLR << IWDT_CLR_POS;
	while((ptSysconBase->IWDCNT & IWDT_CLR_BUSY) == IWDT_CLR_BUSY);
}

static inline uint32_t csp_iwdt_get_cnt(csp_syscon_t *ptSysconBase)
{
	return (ptSysconBase->IWDCNT & 0xfff);
}

static inline bool csp_iwdt_get_status(csp_syscon_t *ptSysconBase)
{
	return ((ptSysconBase->IWDCR & IWDT_ST) >> 12);
}


static inline void csp_iwdt_debug_enable(csp_syscon_t *ptSysconBase)
{
	ptSysconBase->IWDCR = (ptSysconBase->IWDCR & (~IWDT_DBGEN_MSK)) | (ENABLE << IWDT_DBGEN_POS) |IWDTCR_KEY;
}

static inline void csp_iwdt_debug_disable(csp_syscon_t *ptSysconBase)
{
	ptSysconBase->IWDCR = (ptSysconBase->IWDCR & (~IWDT_DBGEN_MSK)) | (DISABLE<< IWDT_DBGEN_POS) |IWDTCR_KEY;
}

static inline uint32_t csp_get_rsr(csp_syscon_t *ptSysconBase)
{
	return (ptSysconBase->RSR);
}

static inline void csp_rst_clr_rsr(csp_syscon_t *ptSysconBase, rst_st_e eRstStatus)
{
	ptSysconBase->RSR = (ptSysconBase->RSR &(~eRstStatus))|eRstStatus;
}

static inline void csp_sramcheck_set_time(csp_syscon_t *ptSysconBase, uint32_t wVal)
{
	ptSysconBase -> RAMCHK = (ptSysconBase -> RAMCHK & (~RAMCHK_TIMES_MSK)) | wVal;
}

static inline void csp_sramcheck_enable(csp_syscon_t *ptSysconBase)
{
	ptSysconBase -> RAMCHK = (ptSysconBase -> RAMCHK & (~RAMCHK_MSK)) | RAMCHK_EN;
}

static inline void csp_sramcheck_disable(csp_syscon_t *ptSysconBase)
{
	ptSysconBase -> RAMCHK = (ptSysconBase -> RAMCHK & (~RAMCHK_MSK)) | RAMCHK_DIS;
}

static inline void csp_sramcheck_rst_enable(csp_syscon_t *ptSysconBase)
{
	ptSysconBase -> RAMCHK = (ptSysconBase -> RAMCHK & (~RAMCHK_RST_MSK)) | RAMCHK_RST;
}

static inline void csp_sramcheck_int_enable(csp_syscon_t *ptSysconBase)
{
	ptSysconBase -> RAMCHK = (ptSysconBase -> RAMCHK & (~RAMCHK_RST_MSK)) | RAMCHK_INT;
}

static inline void csp_flashcheck_set_time(csp_syscon_t *ptSysconBase, uint32_t wVal)
{
	ptSysconBase -> EFLCHK = (ptSysconBase -> EFLCHK & (~EFLCHK_TIMES_MSK)) | wVal;
}

static inline void csp_flashcheck_enable(csp_syscon_t *ptSysconBase)
{
	ptSysconBase -> EFLCHK = (ptSysconBase -> EFLCHK & (~EFLCHK_MSK)) | EFLCHK_EN;
}

static inline void csp_flashcheck_disable(csp_syscon_t *ptSysconBase)
{
	ptSysconBase -> EFLCHK = (ptSysconBase -> EFLCHK & (~EFLCHK_MSK)) | EFLCHK_DIS;
}


//pwrcr
///\param eVcref,eVddcore \ref sleep_vcref_e ,sleep_vddcore_e
static inline void csp_sleep_set_vos(csp_syscon_t *ptSysconBase, sleep_vcref_e eVcref,sleep_vddcore_e eVddcore)
{
	ptSysconBase->PWRKEY = PWRCR_KEY;
	ptSysconBase->PWRCR = (ptSysconBase->PWRCR & (~VOS_SLEEP_CFG_MSK)) | (eVcref << VOS_SLEEP_VCREF_P0S)| (eVddcore << VOS_SLEEP_VDDCORE_P0S);
}

static inline void csp_sleep_mainreg_power_enable(csp_syscon_t *ptSysconBase)
{
	ptSysconBase->PWRKEY = PWRCR_KEY;
	ptSysconBase->PWRCR = (ptSysconBase->PWRCR & (~VOS_SLEEP_SLP_CTRL_P0S_MSK)) | (VOS_SLEEP_SLP_CTRL_P0S_MSK);
}

static inline void csp_sleep_vos_enable(csp_syscon_t *ptSysconBase)
{
	ptSysconBase->PWRKEY = PWRCR_KEY;
	ptSysconBase->PWRCR |=  VOS_SLEEP_EN_P0S_MSK;		
}

static inline void csp_sleep_vos_disable(csp_syscon_t *ptSysconBase)
{
	ptSysconBase->PWRKEY = PWRCR_KEY;
	ptSysconBase->PWRCR &= (~VOS_SLEEP_EN_P0S_MSK); 	
}

//wkcr 

static inline void csp_wakeup_source_enable(csp_syscon_t *ptSysconBase,wakeup_src_e eWkupSrc)
{
	ptSysconBase->WKCR |= (0x1ul << eWkupSrc);
}

static inline void  csp_wakeup_source_disable(csp_syscon_t *ptSysconBase,wakeup_src_e eWkupSrc)
{
	ptSysconBase->WKCR &= ~(0x01ul << eWkupSrc);
}

//UREG0/1/2/3
static inline uint32_t csp_get_ureg(csp_syscon_t *ptSysconBase, uint8_t byNum)
{
	return  *(uint32_t *)((uint32_t)&ptSysconBase->UREG0 + (byNum * 4));
}

static inline void csp_set_ureg(csp_syscon_t *ptSysconBase, uint8_t byNum, uint32_t wVal)
{
	*(uint32_t *)((uint32_t)&ptSysconBase->UREG0 + (byNum * 4)) = wVal;
}

//IDCCR
static inline void csp_sw_rst(csp_syscon_t *ptSysconBase, sw_rst_e eSwRst)
{
	ptSysconBase->IDCCR |= (SYSCON_IDKEY | (eSwRst << SYSCON_SWRST_POS));
}

static inline void csp_exi_set_flt(csp_syscon_t *ptSysconBase, uint8_t byFltNum, exi_flt_e eFlt)
{
	ptSysconBase->EXICFG = (ptSysconBase->EXICFG & ~EXI_FLT_MSK(byFltNum)) | (eFlt << EXI_FLT_POS(byFltNum));
}

//dbgcr
static inline void csp_swd_lock(csp_syscon_t *ptSysconBase)
{
	ptSysconBase->DBGCR = SWD_LOCK;
}

static inline void csp_swd_unlock(csp_syscon_t *ptSysconBase)
{
	ptSysconBase->DBGCR = SWD_UNLOCK;  
}

//cqcr
static inline void csp_cqcr_enable(csp_syscon_t *ptSysconBase)
{

	ptSysconBase->CQCR |=  CQCR_EN_MSK;		
}

static inline void csp_cqcr_disable(csp_syscon_t *ptSysconBase)
{

	ptSysconBase->CQCR &= (~CQCR_EN_MSK); 	
}

static inline uint8_t csp_cqcr_get_status(csp_syscon_t *ptSysconBase)
{
	return (uint8_t) ((ptSysconBase->CQCR) & CQCR_EN_MSK);
}

static inline void csp_cqcr_set_ref(csp_syscon_t *ptSysconBase,cqcr_refsel_e eRefSel)
{
	ptSysconBase->CQCR = (ptSysconBase->CQCR & (~CQCR_REFSEL_MSK)) | (eRefSel  << CQCR_REFSEL_POS);
}

static inline void csp_cqcr_set_src(csp_syscon_t *ptSysconBase,cqcr_srcsel_e eSrcSel)
{
	ptSysconBase->CQCR = (ptSysconBase->CQCR & (~CQCR_SRCSEL_MSK)) | (eSrcSel  << CQCR_SRCSEL_POS);
}

static inline void csp_cqcr_set_value(csp_syscon_t *ptSysconBase,uint32_t wVal)
{
	ptSysconBase->CQCR = (wVal  << CQCR_CQRVAL_POS);
}


static inline uint32_t csp_get_cqsr(csp_syscon_t *ptSysconBase)
{
	return (uint32_t) ((ptSysconBase->CQSR) & CQSR_MSK);
}

#endif  /* _CSP_SYSCON_H*/

