/***********************************************************************//** 
 * \file  csp_etcb.h
 * \brief ETCB description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-9-4 <td>V0.0  <td>WNN     <td>initial
 * <tr><td> 2021-5-14 td>V0.0  <td>ZJY     <td>initial
 * </table>
 * *********************************************************************
*/

#ifndef _CSP_ETCB_H
#define _CSP_ETCB_H

#include <soc.h>

/*
 * Usage
 * 1. Configure dest trigger user IP1, and enable the IP1
 * 2. Configure channel in the ETCB
 *      1)source-trigger select
 *      2)dest-trigger select
 *      3)mode select
 *      4)channel enable
 *      5)ETCB enable
 *      6)softtrigger if need
 * 3. Configure source trigger generator IP2, and enable the IP2
 * For example, etcb channel3 for timer trigger adc working, and etb channel4 for adc trigger dma to carry data
 * 1)configure adc and dma IP first and waiting trigger signal.
 * 2)configure etcb channel3 and channel4 include source,dest,mode select.
 * 3)enable etcb channel3 and channel4
 * 4)enable etcb
 * 5)configure timer and enable
 * Note:The GPIO can be configured as level or edge mode. when using GPIO trigger, the GPIO must be level mode.
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    __IOM uint32_t CFG0;
    __IOM uint32_t CFG1;
} wj_etcb_ch1_2_regs_t;

/// \struct csp_etcb_t
/// \brief ETCB Reg Description     
typedef struct {
    __IOM uint32_t ETCB_EN;          //0x00 (R/W) ETCB enable Register                   
    __IOM uint32_t SOFTTRIG;        //0x04 (R/W) Software trigger Configure Register   
    __IOM uint32_t CFG0_CH0;        //0x08 (R/W) ETCB Channel0 Configure0 Register       
    __IOM uint32_t CFG1_CH0;        //0x0c (R/W) ETCB Channel0 Configure1 Register      
    wj_etcb_ch1_2_regs_t CH1_2[2];   //0x10 ~ 0x14 (R/W) ETCB Channel1 Configure0~1 Register; 0x18 ~ 0x1C (R/W) ETCB Channel2 Configure0~1 Register 
    uint32_t RESERVED0[4];
    __IOM uint32_t CFG_CHX[29];     // 0x30~0xa0 (R/W) ETCB CH_3~CH_31 Configure Register */
} csp_etcb_t;

/******************************************************************************
* XXX : ETCB trigger mode and channel enable
******************************************************************************/
#define	ETCB_CH_EN_POS			(0U)	//CH(0->7) enable/disable		
#define	ETCB_CH_EN_MSK			(0x01ul << ETCB_CH_EN_POS)
typedef enum
{
	ETCB_CH_DIS			= 0,	
	ETCB_CH_EN	
}etcb_ch_en_e;

#define	ETCB_CH_TRG_MODE_POS	(1U)		//CH(0->7) trigger mode select
#define	ETCB_CH_TRG_MODE_MSK	(0x01ul << ETCB_CH_TRG_MODE_POS)
typedef enum
{
	ETCB_CH_TRG_HARD	= 0,				//Hardware trigger	
	ETCB_CH_TRG_SOFT						//Software trigger	
}etcb_ch_trg_mode_e;

/******************************************************************************
* ENABLE : ETCB Enable Register
******************************************************************************/
#define	ETCB_ENABLE_POS			(0U)	//ETCB enable/disable
#define	ETCB_ENABLE_MSK			(0x01ul << ETCB_ENABLE_POS)
typedef enum
{
	ETCB_DISABLE		= 0,	
	ETCB_ENABLE	
}etcb_enable_e;

/******************************************************************************
*SWTRG : ETCB Software Trigger Register
******************************************************************************/
#define ETCB_SWTRG_CH_POS(n)		(n)
#define ETCB_SWTRG_CH_MSK(n)		(0x01ul << ETCB_SWTRG_CH_POS(n))
#define ETCB_SWTRG_CH_SET(n)     	ETCB_SWTRG_CH_MSK(n)

/******************************************************************************
*CH0CON0 : ETCB Channel0 Control Register0
******************************************************************************/
//source 0
#define	ETCB_CH0_SRC0_EN_POS		(0U)	//CH0 trigger source0 enable/disable		
#define	ETCB_CH0_SRC0_EN_MSK		(0x01ul << ETCB_CH0_SRC0_EN_POS)
typedef enum
{
	ETCB_CH0_SRC0_DIS	= 0,	
	ETCB_CH0_SRC0_EN	
}etcb_ch0_src0_en_e;
#define	ETCB_CH0_TRG_SRC0_POS		(1U)	//CH0 trigger source0 select
#define	ETCB_CH0_TRG_SRC0_MSK		(0xFFul << ETCB_CH0_TRG_SRC0_POS)
#define	ETCB_CH0_TRG_SRC0(val)		(((val) & 0xFFul) << ETCB_CH0_TRG_SRC0_POS)

//source 1
#define	ETCB_CH0_SRC1_EN_POS		(10U)	//CH0 trigger source1 enable/disable		
#define	ETCB_CH0_SRC1_EN_MSK		(0x01ul << ETCB_CH0_SRC1_EN_POS)
typedef enum
{
	ETCB_CH0_SRC1_DIS	= 0,	
	ETCB_CH0_SRC1_EN	
}etcb_ch0_src1_en_e;

#define	ETCB_CH0_TRG_SRC1_POS		(11U)	//CH0 trigger source1 select
#define	ETCB_CH0_TRG_SRC1_MSK		(0xFFul << ETCB_CH0_TRG_SRC1_POS)
#define	ETCB_CH0_TRG_SRC1(val)		(((val) & 0xFFul) << ETCB_CH0_TRG_SRC1_POS)

//source 2
#define	ETCB_CH0_SRC2_EN_POS		(20U)	//CH0 trigger source2 enable/disable		
#define	ETCB_CH0_SRC2_EN_MSK		(0x01ul << ETCB_CH0_SRC2_EN_POS)
typedef enum
{
	ETCB_CH0_SRC2_DIS	= 0,	
	ETCB_CH0_SRC2_EN	
}etcb_ch0_src2_en_e;

#define	ETCB_CH0_TRG_SRC2_POS		(21U)	//CH0 trigger source2 select
#define	ETCB_CH0_TRG_SRC2_MSK		(0xFFul << ETCB_CH0_TRG_SRC2_POS)
#define	ETCB_CH0_TRG_SRC2(val)		(((val) & 0xFFul) << ETCB_CH0_TRG_SRC2_POS)

/******************************************************************************
*CH0CON1 : ETCB Channel0 Control Register1
******************************************************************************/
//#define	ETCB_CH0_EN_POS			(0U)	//CH0 enable/disable		
//#define	ETCB_CH0_EN_MSK			(0x01ul << ETCB_CH0_EN_POS)
//typedef enum
//{
//	ETCB_CH0_DIS			= 0,	
//	ETCB_CH0_EN	
//}etcb_ch0_en_e;
//
//#define	ETCB_CH0_TRG_MODE_POS	(1U)	//CH0 trigger mode select
//#define	ETCB_CH0_TRG_MODE_MSK	(0x01ul << ETCB_CH0_TRG_MODE_POS)
//typedef enum
//{
//	ETCB_CH0_TRG_HARD	= 0,			//Hardware trigger	
//	ETCB_CH0_TRG_SOFT					//Software trigger	
//}etcb_ch0_trg_mode_e;

#define	ETCB_CH0_TRG_DST_POS		(25U)	//CH0 trigger destination select
#define	ETCB_CH0_TRG_DST_MSK		(0x7Ful << ETCB_CH0_TRG_DST_POS)
#define	ETCB_CH0_TRG_DST(val)		(((val) & 0x7Ful) << ETCB_CH0_TRG_DST_POS)

/******************************************************************************
*CHxCON0 : ETCB Channel x Control Register0,(x = 1->2)
******************************************************************************/
//destination 0
#define	ETCB_CH1_2_DST0_EN_POS		(0U)	//CH1_2 trigger destination0 enable/disable		
#define	ETCB_CH1_2_DST0_EN_MSK		(0x01ul << ETCB_CH1_2_DST0_EN_POS)
typedef enum
{
	ETCB_CH1_2_DST0_DIS	= 0,	
	ETCB_CH1_2_DST0_EN	
}etcb_chx_dst0_en_e;
#define	ETCB_CH1_2_TRG_DST0_POS		(1U)	//CH1_2trigger destination0 select
#define	ETCB_CH1_2_TRG_DST0_MSK		(0xFFul << ETCB_CH1_2_TRG_DST0_POS)
#define	ETCB_CH1_2_TRG_DST0(val)	(((val) & 0xFFul) << ETCB_CH1_2_TRG_DST0_POS)

//destination 1
#define	ETCB_CH1_2_DST1_EN_POS		(10U)	//CH1_2 trigger destination1 enable/disable		
#define	ETCB_CH1_2_DST1_EN_MSK		(0x01ul << ETCB_CH1_2_DST1_EN_POS)
typedef enum
{
	ETCB_CH1_2_DST1_DIS	= 0,	
	ETCB_CH1_2_DST1_EN	
}etcb_chx_dst1_en_e;
#define	ETCB_CH1_2_TRG_DST1_POS		(11U)	//CH1_2 trigger destination1 select
#define	ETCB_CH1_2_TRG_DST1_MSK		(0xFFul << ETCB_CH1_2_TRG_DST1_POS)
#define	ETCB_CH1_2_TRG_DST1(val)	(((val) & 0xFFul) << ETCB_CH1_2_TRG_DST1_POS)

//destination 2
#define	ETCB_CH1_2_DST2_EN_POS		(20U)	//CH1_2 trigger destination2 enable/disable		
#define	ETCB_CH1_2_DST2_EN_MSK		(0x01ul << ETCB_CH1_2_DST2_EN_POS)
typedef enum
{
	ETCB_CH1_2_DST2_DIS	= 0,	
	ETCB_CH1_2_DST2_EN	
}etcb_chx_dst2_en_e;
#define	ETCB_CH1_2_TRG_DST2_POS		(21U)	//CH1_2 trigger destination2 select
#define	ETCB_CH1_2_TRG_DST2_MSK		(0xFFul << ETCB_CH1_2_TRG_DST2_POS)
#define	ETCB_CH1_2_TRG_DST2(val)	(((val) & 0xFFul) << ETCB_CH1_2_TRG_DST2_POS)

/******************************************************************************
*CHxCON1 : ETCB Channel x Control Register1,(x = 0->2)
******************************************************************************/
//#define	ETCB_CH1_2_EN_POS		(0U)	//CH1_2 enable/disable		
//#define	ETCB_CH1_2_EN_MSK		(0x01ul << ETCB_CH1_2_EN_POS)
//typedef enum
//{
//	ETCB_CH1_2_DIS		= 0,	
//	ETCB_CH1_2_EN	
//}etcb_ch1_2_en_e;
//
//#define	ETCB_CH1_2_TRG_MODE_POS	(1U)	//CH1_2 trigger mode select
//#define	ETCB_CH1_2_TRG_MODE_MSK	(0x01ul << ETCB_CH1_2_TRG_MODE_POS)
//typedef enum
//{
//	ETCB_CH1_2_TRG_HARD	= 0,			//Hardware trigger	
//	ETCB_CH1_2_TRG_SOFT					//Software trigger	
//}etcb_ch1_2_trg_mode_e;

#define	ETCB_CH1_2_TRG_SRC_POS		(25U)	//CH1_2 trigger source select
#define	ETCB_CH1_2_TRG_SRC_MSK		(0x7Ful << ETCB_CH1_2_TRG_SRC_POS)
#define	ETCB_CH1_2_TRG_SRC(val)		(((val) & 0x7Ful) << ETCB_CH1_2_TRG_SRC_POS)

/******************************************************************************
*CHxCON : ETCB Channel x Control Register,(x = 3->31)
******************************************************************************/
//source 
#define	ETCB_CHX_TRG_SRC_POS		(12U)	//CHx(3->31) trigger source select
#define	ETCB_CHX_TRG_SRC_MSK		(0x7Ful << ETCB_CHX_TRG_SRC_POS)
#define	ETCB_CHX_TRG_SRC(val)		(((val) & 0x7Ful) << ETCB_CHX_TRG_SRC_POS)

//destination
#define	ETCB_CHX_TRG_DST_POS		(24U)	//CHx(3->31) trigger destination select
#define	ETCB_CHX_TRG_DST_MSK		(0xFFul << ETCB_CHX_TRG_DST_POS)
#define	ETCB_CHX_TRG_DST(val)		(((val) & 0xFFul) << ETCB_CHX_TRG_DST_POS)

//dma
#define	ETCB_CHX_DMA_EN_POS			(2U)	//CHx(20->31) trigger destination select
#define	ETCB_CHX_DMA_EN_MSK			(0x01ul << ETCB_CHX_DMA_EN_POS)
#define	ETCB_CHX_DMA_EN				(0x01ul)

/******************************************************************************
********************** ETCB inline Functions Declaration **********************
******************************************************************************/
static inline void csp_etcb_enable(csp_etcb_t *ptEtbBase)				
{ 
	ptEtbBase->ETCB_EN |= ETCB_ENABLE_MSK;	
}
static inline void csp_etcb_disable(csp_etcb_t *ptEtbBase)		
{
	ptEtbBase->ETCB_EN &= ~ETCB_ENABLE_MSK;	
}

//chx enable: x > 2; channel num > 2
static inline void csp_etcb_chx_enable(csp_etcb_t *ptEtbBase, uint8_t byChNum)	
{
	ptEtbBase->CFG_CHX[byChNum-3] |= ETCB_CH_EN_MSK;
}

//dma channel: 20~31
static inline void csp_etcb_dma_enable(csp_etcb_t *ptEtbBase, uint8_t byChNum)
{
	ptEtbBase->CFG_CHX[byChNum-3] |= ETCB_CHX_DMA_EN_MSK;	
}
static inline void csp_etcb_dma_disable(csp_etcb_t *ptEtbBase, uint8_t byChNum)
{
	ptEtbBase->CFG_CHX[byChNum-3] &= ~ETCB_CHX_DMA_EN_MSK;	
}

//trg mode
static inline void csp_etcb_soft_trg_mode(csp_etcb_t *ptEtbBase, uint8_t byChNum)
{
	ptEtbBase->CFG_CHX[byChNum-3] |= ETCB_CH_TRG_MODE_MSK;
}
static inline void csp_etcb_hart_trg_mode(csp_etcb_t *ptEtbBase, uint8_t byChNum)
{
	ptEtbBase->CFG_CHX[byChNum-3] &= ~ETCB_CH_TRG_MODE_MSK;
}
static inline void csp_etcb_sw_trg(csp_etcb_t *ptEtbBase, uint8_t byChNum)	
{
	ptEtbBase->SOFTTRIG |= ETCB_SWTRG_CH_SET(byChNum);
}

//one trg one
static inline void csp_etcb_set_one_trg_one(csp_etcb_t *ptEtbBase, uint8_t byChNum, uint8_t bySrc, uint8_t byDst, etcb_ch_trg_mode_e eTrgMode)
{
	ptEtbBase->CFG_CHX[byChNum-3] = (eTrgMode << ETCB_CH_TRG_MODE_POS) | ETCB_CHX_TRG_SRC(bySrc) | ETCB_CHX_TRG_DST(byDst);
}

#ifdef __cplusplus
}
#endif

#endif 
