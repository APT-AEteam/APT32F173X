/***********************************************************************//** 
 * \file  cmp.h
 * \brief header file for cmp driver
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-10-14 <td>V0.0  <td>YYM   <td>modify
 * <tr><td> 2023-09-07 <td>V1.0  <td>LHY   <td>code normalization
 * </table>
 * *********************************************************************
*/

#ifndef _DRV_CMP_H_
#define _DRV_CMP_H_

#include "csp.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CMP0_OUT     0            
#define CMP1_OUT     1            
#define CMP2_OUT     2            
#define CMP3_OUT     3            


 
#define CMP_EDGEDET0_INT         (1<<0)
#define CMP_EDGEDET1_INT         (1<<1)
#define CMP_EDGEDET2_INT         (1<<2)


/// \struct csi_cnta_ctrl_t
/// \brief  cnta control handle, not open to users  
typedef struct 
{
    void(*callback)(csp_cmp_t *ptCmpBase, uint8_t byIsr);
} csi_cmp_ctrl_t;

extern csi_cmp_ctrl_t g_tCmpCtrl[CMP_IDX];


typedef enum
{
	CMP_INTSRC_NONE     =  (0x00ul << 0), 
	CMP_INTSRC_EDGEDET  =  (0x01ul << 0),  
}csi_cmp_intsrc_e;

typedef enum
{
	CMP_PHYST_POL_0mv	     =	0x00,
	CMP_PHYST_POL_10mv,
	CMP_PHYST_POL_15mv,
	CMP_PHYST_POL_25mv,
	CMP_PHYST_POL_35mv,
	CMP_PHYST_POL_45mv,	
	CMP_PHYST_POL_55mv,	
	CMP_PHYST_POL_65mv				
}csi_cmp_pnhystpol_e;


typedef enum
{
    CMP_POL_OUT_DIRECT       = 0x00,
	CMP_POL_OUT_UNDIRECT 
}csi_cmp_polarity_e;


typedef enum
{
	CMP_CPOS_OUT_DIRECT		 =	0x00,
	CMP_CPOS_OUT_IN			 =	0x01
}csi_cmp_cr_cpos_e;

typedef enum
{
	CMP_TRGSRC_FALLING		     =	0x00,
	CMP_TRGSRC_RISING,
	CMP_TRGSRC_FALLING_RISING
}csi_cmp_trgsrc_e;

typedef enum
{
	CMP_DFCR_DEPTH1_8        =   0,
	CMP_DFCR_DEPTH1_16       =   1,
	CMP_DFCR_DEPTH1_32       =   2,
	CMP_DFCR_DEPTH1_64       =   3,
	CMP_DFCR_DEPTH1_128      =   4,
	CMP_DFCR_DEPTH1_256      =   5,
	CMP_DFCR_DEPTH1_512      =   6,
	CMP_DFCR_DEPTH1_1024     =   7
			
}csi_cmp_dfcr1_depth_e;

typedef enum
{
	CMP_DFCR_DEPTH2_16       =   0x00,
	CMP_DFCR_DEPTH2_32,
	CMP_DFCR_DEPTH2_64,
	CMP_DFCR_DEPTH2_128,
	CMP_DFCR_DEPTH2_256,
	CMP_DFCR_DEPTH2_512,
	CMP_DFCR_DEPTH2_1024,
	CMP_DFCR_DEPTH2_2048
			
}csi_cmp_dfcr2_depth_e;


typedef enum
{
	CMP_WFCR_ALIGN_DISALLOW  = 0x00,
	CMP_WFCR_ALIGN_ALLOW     = 0x01
}csi_cmp_wfcr_align_e;

typedef enum
{
	CMP_WFCR_OSET_DIS		 =	0x00,
	CMP_WFCR_OSET_HIGH,
	CMP_WFCR_OSET_LOW,
	CMP_WFCR_OSET_DIS1
}csi_cmp_wfcr_oset_e;

typedef enum
{
	CMP_N_SEL_CP0            =  0x00,
	CMP_N_SEL_CP1,
	CMP_N_SEL_CP2,
	CMP_N_SEL_CP3,	
	CMP_N_SEL_CP4,
	CMP_N_SEL_CP5,
	CMP_N_SEL_CP6,
	CMP_N_SEL_CP7,
	CMP_N_SEL_CP8,	
	CMP_N_SEL_CP9,
	CMP_N_SEL_CP10,  
	CMP_N_SEL_1_4INPUT,	
	CMP_N_SEL_2_4INPUT,	
	CMP_N_SEL_3_4INPUT,	
	CMP_N_SEL_4_4INPUT,		
	CMP_N_SEL_DAC		
}csi_cmp_nsel_e;

typedef enum
{
	CMP_P_SEL_CP0            = 0x00,
	CMP_P_SEL_CP1,
	CMP_P_SEL_CP2,
	CMP_P_SEL_CP3,	
	CMP_P_SEL_CP4,
	CMP_P_SEL_CP5,
	CMP_P_SEL_CP6,
	CMP_P_SEL_CP7,
	CMP_P_SEL_CP8,
	CMP_P_SEL_CP9,
	CMP_P_SEL_CP10,	
	CMP_P_SEL_CP11,
	CMP_P_SEL_CP12,
	CMP_P_SEL_CP13,
	CMP_P_SEL_CP14,
	CMP_P_SEL_CP15	
	
}csi_cmp_psel_e;


typedef struct
{
	uint8_t  byNsel;                 //N- pin
	uint8_t  byPsel;	             //P+ pin
	uint8_t  byPhystpol;		     
	uint8_t  byNhystpol;		
	uint8_t  byPolarity;		
	uint8_t  byCpoSel;
}csi_cmp_config_t;

typedef struct
{
	uint8_t byDepth1;
	uint8_t byDivn1;
	uint8_t byDivm1;	
	uint8_t byRev;
}csi_cmp_dflt1_config_t;

typedef struct
{	
	uint8_t byDepth2;
	uint8_t byDivn2;
	uint8_t byDivm2;
	uint8_t byRev;	
}csi_cmp_dflt2_config_t;

typedef struct
{
	uint8_t  byWfalign;
	uint8_t  byWoset;
	uint8_t  byClkDiv;	
	uint8_t  byDcnt;
	uint16_t hwWcnt;
	
}csi_cmp_wfcr_config_t;

/**
 *  \brief       init cmp
 *  \param[in]   ptCmpBase:pointer of cmp register structure
 *  \param[in]   ptCmpCfg:pointer of cmp parameter config structure
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_cmp_init(csp_cmp_t *ptCmpBase,csi_cmp_config_t *ptCmpCfg);

/**
 *  \brief       start cmp
 *  \param[in]   ptCmpBase:pointer of cmp register structure
 *  \return none
 */
void csi_cmp_start(csp_cmp_t *ptCmpBase);

/** \brief stop cmp
 * 
 *  \param[in] ptBtBase: pointer of cmp register structure
 *  \return none
 */ 
void csi_cmp_stop(csp_cmp_t *ptCmpBase);

/** \brief cmp dflt1 set
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] bEnable: dflt1 enable or disable
 *  \param[in] ptCmpDfltCfg: pointer of cmp dlft config structure
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_cmp_set_dflt1(csp_cmp_t *ptCmpBase,csi_cmp_dflt1_config_t *ptCmpDfltCfg,bool bEnable);

/** \brief cmp dflt2 set
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] bEnable: dflt2 enable or disable
 *  \param[in] ptCmpDfltCfg: pointer of cmp dlft config structure
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_cmp_set_dflt2(csp_cmp_t *ptCmpBase,csi_cmp_dflt2_config_t *ptCmpDfltCfg,bool bEnable);

/**
 *  \brief       cmp wflt set
 *  \param[in]   ptCmpBase:pointer of cmp register structure
 *  \param[in]   ptCmpWfcrCfg:pointer of cmp wflt config structure
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_cmp_set_wfcr(csp_cmp_t *ptCmpBase,csi_cmp_wfcr_config_t *ptCmpWfcrCfg);

/** \brief cmp evtrg output config
 * 
 *  \param[in] ptCmpBase:pointer of cmp register structure
 *  \param[in] eTrgSrc: evtrg eve sel(0~3) 
 *  \return none
 */
void csi_cmp_set_evtrg(csp_cmp_t *ptCmpBase,csi_cmp_trgsrc_e eTrgSrc);

/** \brief cmp evtrg enable
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \return none
 */
void csi_cmp_evtrg_enable(csp_cmp_t *ptCmpBase);
/** \brief cmp evtrg disable
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \return none
 */
void csi_cmp_evtrg_disable(csp_cmp_t *ptCmpBase);

/** \brief cmp out status
 * 
 *  \param[in] ptCmpBase:pointer of cmp register structure
 *  \param[in] byOutCh: cmp out ch sel(cmp0~cmp1)
 *  \return out status(0~1)
 */
uint8_t csi_cmp_get_out(csp_cmp_t *ptCmpBase,uint8_t byOutCh);

/** \brief clear cmp int
 * 
 *  \param[in] ptCmpBase:pointer of cmp register structure
 *  \return none
 */
void csi_cmp_clr_isr(csp_cmp_t *ptCmpBase);

/** \brief cmp interrupt enable control
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] eIntSrc: cmp interrupt source \ref csi_cmp_intsrc_e
 *  \return none
 */ 
void csi_cmp_int_enable(csp_cmp_t *ptCmpBase, csi_cmp_intsrc_e eIntSrc);

/** \brief cmp interrupt disable control
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] eIntSrc: cmp interrupt source \ref csi_cmp_intsrc_e
 *  \return none
 */ 
void csi_cmp_int_disable(csp_cmp_t *ptCmpBase, csi_cmp_intsrc_e eIntSrc);

/** \brief cmp interrupt handle weak function
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \return none
 */ 
void csi_cmp_irqhandler(csp_cmp_t *ptCmpBase, uint8_t byIdx);

/** \brief       register cmp interrupt callback function
 *  \param[in]   ptCmpBase    pointer of cmp register structure
 *  \param[in]   callback  cmp interrupt handle function
 *  \return      error code \ref csi_error_t
 */ 
csi_error_t csi_cmp_register_callback(csp_cmp_t *ptCmpBase, void  *callback);


#ifdef __cplusplus
}
#endif

#endif /* _DRV_CMP_H_ */