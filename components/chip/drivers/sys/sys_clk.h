
/***********************************************************************//** 
 * \file  sys_clk.h
 * \brief  head file for sys_clk.c  
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-10 <td>V0.0 <td>WNN     <td>initial for csi
 * <tr><td> 2020-5-13 <td>V0.0 <td>ZJY     <td>initial for csi
 * </table>
 * *********************************************************************
*/
 
#ifndef _SYS_CLK_H_
#define _SYS_CLK_H_

#include <stdint.h>
#include <soc.h>
#include "csp_syscon.h"

typedef enum {
    SRC_IMOSC = 0,
    SRC_EMOSC,
    SRC_HFOSC,
	SRC_AUTO_HF_PLL,
	SRC_AUTO_EM_PLL,
	SRC_MANUAL_PLL,
	SRC_ISOSC,
	SRC_ESOSC	
} cclk_src_e;

typedef enum{
	PCLK_PM = 8,
	HCLK_PM,
	ISOSC_PM = 12, 
	IMOSC_PM,
	EMOSC_PM = 15
}cclk_pm_e;

typedef enum {
	IM_5M = IMOSC_5M_VALUE,
	IM_4M = IMOSC_4M_VALUE,
	IM_2M = IMOSC_2M_VALUE, 
	IM_131K = IMOSC_131K_VALUE
}imo_freq_e;

typedef enum{
	HF_24M  = HFOSC_24M_VALUE,
	HF_12M  = HFOSC_12M_VALUE,
	HF_6M   = HFOSC_6M_VALUE
}hfo_freq_e;

typedef enum{
	PCLK_DIV1	= 0,
	PCLK_DIV2,
	PCLK_DIV4,
	PCLK_DIV8 	= 4,
	PCLK_DIV16 	= 8
}pclk_div_e;

typedef enum{
	SCLK_DIV1	= 1,
	SCLK_DIV2,
}hclk_div_e;

/// \struct system_clk_config_t
/// members to be used in system clock management, including osc src, osc freq(if seletable), HCLK and PCLK divider
typedef struct {
	cclk_src_e		eClkSrc;	//clock source
	uint32_t		wFreq;		//clock frequency
	hclk_div_e		eSdiv;		//SDIV
	pclk_div_e		ePdiv;		//PDIV
	uint32_t		wSclk;		//SCLK
	uint32_t		wPclk;
}csi_clk_config_t;

extern csi_clk_config_t tClkConfig;

typedef enum{
	PLL_SEL_HFOSC_24M,	
	PLL_SEL_HFOSC_12M,	
	PLL_SEL_HFOSC_6M,	
	PLL_SEL_EMOSC_24M
}pll_manual_sel_e;

typedef enum{
	PLL_HFOSC_24M_AUTO,
	PLL_EMOSC_24M_AUTO
}pll_auto_sel_e;

typedef enum{
	CKQ_DIV2,	
	CKQ_DIV4,	
	CKQ_DIV6,
	CKQ_DIV8,
}pll_ckqdiv_e;

typedef struct {
	pll_manual_sel_e  eClkSel;	//clock frequency
	uint8_t           byDivM;
	uint8_t           byNul;
	uint8_t           byCkp_Div;
	pll_ckqdiv_e      byCkq_Div;
}csi_pll_manual_config_t;

extern csi_pll_manual_config_t tPllClkConfig;

//typedef struct {
//    cclk_src_e	eSysClkSrc;      /* select sysclk source clock */
//	uint32_t 	wOscFreq;        /* select frequence */
//    hclk_div_e 	eHclkDivider;    /* ratio between fs_mclk clock and mclk clock */
//    pclk_div_e 	ePclkDivider;    /* ratio between mclk clock and apb0 clock */
//} system_clk_config_t;

/// \enum clk_module_t
/// \brief all the peri clock enable bits in SYSCON level
/// \todo  clk_module_t or clk_module_e

typedef enum {
    IFC_SYS_CLK		= 0U,
	FVR_SYS_CLK		= 3U,
    ADC0_SYS_CLK	= 4U,
    ADC1_SYS_CLK	= 5U,
    ETCB_SYS_CLK	= 7U,
    UART0_SYS_CLK	= 8U,
	UART1_SYS_CLK	= 9U,
	UART2_SYS_CLK	= 10U,
	USART0_SYS_CLK	= 13U,
	USART1_SYS_CLK	= 14U,
    SPI0_SYS_CLK	= 16U,
	SPI1_SYS_CLK	= 17U,
    SIO0_SYS_CLK	= 20U,
	SIO1_SYS_CLK	= 21U,
    I2C_SYS_CLK		= 22U,
    CAN_SYS_CLK		= 24U,
	
	GPTA0_SYS_CLK	= 33U,
	GPTA1_SYS_CLK	= 34U,
	GPTA2_SYS_CLK	= 35U,
	GPTA3_SYS_CLK	= 36U,

	GPTB0_SYS_CLK	= 37U,
	GPTB1_SYS_CLK	= 38U,
	GPTB2_SYS_CLK	= 39U,
	GPTB3_SYS_CLK	= 40U,
	GPTB4_SYS_CLK	= 41U,
	GPTB5_SYS_CLK	= 42U,	
    WWDT_SYS_CLK	= 43U,
    LPT_SYS_CLK		= 44U,
	CNTA_SYS_CLK	= 45U,
    RTC_SYS_CLK		= 46U,
	BT0_SYS_CLK		= 47U,
	BT1_SYS_CLK		= 48U,
	LED_SYS_CLK		= 49U,
	
	CMP0_SYS_CLK	= 50U,	
	CMP1_SYS_CLK	= 51U,		
	CMP2_SYS_CLK	= 52U,		
	
	OPA0_SYS_CLK	= 55U,	
	OPA1_SYS_CLK	= 56U,	
	OPA2_SYS_CLK	= 57U,	
	OPA3_SYS_CLK	= 58U,	

	BT2_SYS_CLK		= 62U,
	BT3_SYS_CLK		= 63U,

} clk_module_t;


//global variable: sysclkclk 
extern uint32_t g_wSystemClk;


/** 
  \brief sysctem clock (HCLK) configuration
   To set CPU frequence according to tClkConfig
  \param[in] none.
  \return csi_error_t.
 */ 
csi_error_t csi_sysclk_config(csi_clk_config_t tClkCfg);

/** \brief PLL clk manual config
 * 
 *  \param[in] ePllCfg: pll clock configuration 
 *  \param[in] wFreq: pll clk freq 
 *  \return csi_error_t.
 */
 csi_error_t csi_pll_manual_config(csi_pll_manual_config_t ePllCfg,uint32_t wFreq);
 
/** \brief PLL clk auto config
 * 
 *  \param[in] ePllAutoSel: auto clk source
 *  \param[in] wFreq: pll clk freq 
 *  \return csi_error_t.
 */
 csi_error_t csi_pll_auto_config(pll_auto_sel_e ePllAutoSel,uint32_t wFreq);
 
/** 
  \brief Clock output configuration
  \param[in] eCloSrc: source to output
  \param[in] eCloDiv: clo divider 
  \param[in] tPin: output pin
  \return csi_error_t.
 */

csi_error_t csi_clo_config(clo_src_e, clo_div_e, pin_name_e);

/** 
  \brief to set clock status in PM mode 
   when IWDT is enabled, trying to stop ISOSC in stop mode would be invalid
   refer to GCER in SYSCON chapter for detailed description
  \param[in] eClk: clock to be configured
  \param[in] bEnable: enable or disable
  \return none.
 */ 
void csi_clk_pm_enable(clk_pm_e eClk, bool bEnable);


/** 
  \brief to calculate SCLK and PCLK frequence according to the current reg content
   tClkConfig.wSclk and tClkConfig.wPclk will be updated after excuting this function
  \param[in] none.
  \return csi_error_t.
 */
csi_error_t csi_calc_clk_freq(void);

/**
  \brief       Soc get sclk frequence.
  \param[in]   none
  \return      system clk frequence
*/
uint32_t csi_get_sclk_freq(void);

/**
  \brief       Soc get pclk frequence.
  \param[in]   none
  \return      Peripherals clk frequence
*/
uint32_t csi_get_pclk_freq(void);

/**
  \brief       Soc get coret frequence.
  \param[in]   none
  \return      coret frequence
*/
uint32_t soc_get_coret_freq(void);

/**
  \brief       Soc get bt frequence.
  \param[in]   byIdx	id of bt
  \return      bt frequence
*/
uint32_t soc_get_bt_freq(uint8_t byIdx);

/*
  \brief       Soc enable device clock
  \param[in]   module   clock module, defined in sys_clk.h, \ref clk_module_t
  \return      none
*/
void soc_clk_enable(int32_t module);

/*
  \brief       Soc disable device clock
  \param[in]   module   clock module, defined in sys_clk.h, \ref clk_module_t
  \return      none
*/
void soc_clk_disable(int32_t module);

/**
  \brief       Soc get device frequence.
  \param[in]   freq     cpu frequence
  \return      none
*/
//void soc_set_sys_freq(uint32_t freq);

/**
  \brief       SOC Dcache clean & invalid by range.
  \return      none
*/
//void soc_dcache_clean_invalid_range(uint32_t addr, uint32_t size);

/**
  \brief       SOC Dcache clean & invalid all.
  \return      none
*/
//void soc_dcache_clean_invalid_all(void);

/**
  \brief       SOC Dcache invalid by range.
  \return      none
*/
//void soc_dcache_invalid_range(uint32_t addr, uint32_t size);

/**
  \brief       SOC dma address remap.
  \return      remaped address
*/
//extern uint32_t soc_dma_address_remap(uint32_t addr);


//#ifdef CONFIG_PM
/**
  \brief       SoC enter low-power mode, each chip's implementation is different
               called by csi_pm_enter_sleep
  \param[in]   mode        low-power mode
  \return      error code
*/
//csi_error_t soc_pm_enter_sleep(csi_pm_mode_t mode);




#endif /* _SYS_CLK_H_ */