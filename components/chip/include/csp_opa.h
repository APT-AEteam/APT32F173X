/***********************************************************************//** 
 * \file  csp_opa.h
 * \brief OPA description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2022 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2022-02-17 <td>V0.0  <td>XB    <td>initial
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CSP_OPA_H
#define _CSP_OPA_H

/* Includes ------------------------------------------------------------------*/
#include <soc.h>

/// \struct csp_opa_t
/// \brief OPA reg description   

typedef struct
{
   __IOM uint32_t  CR;  	//0x0000	OPAx Config Register

} csp_opa_t;

/******************************************************************************
* CR : OPA Control Register	
******************************************************************************/
// CR : OPA Control Register		
#define	OPA_OPAEN_POS			(0)			
#define	OPA_OPAEN_MSK			(0x01ul << OPA_OPAEN_POS)								
typedef enum{
	OPA_DIS			= 0,
	OPA_EN	
}opa_cr_e;


//#define	OPA_PGAEN_POS			(1)		
//#define	OPA_PGAEN_MSK			(0x01ul << OPA_PGAEN_POS)								
//typedef enum{
//	OPA_PGA_DIS			= 0,
//	OPA_PGA_EN	
//}opa_pga_e;

#define	OPA_PGAC_POS			(2)			
#define	OPA_PGAC_MSK			(0x07ul << OPA_PGAC_POS)								
typedef enum{
	OPA_PGAC_X2			= 0,
	OPA_PGAC_X4,
	OPA_PGAC_X8,
	OPA_PGAC_X16,
	OPA_PGAC_X21,
	OPA_PGAC_X32,
	OPA_PGAC_X42,
	OPA_PGAC_X64
}opa_pgac_singel_e;

typedef enum{
	OPA_PGAC_DIF_X1			= 0,
	OPA_PGAC_DIF_X2,
	OPA_PGAC_DIF_X3,
	OPA_PGAC_DIF_X5,
	OPA_PGAC_DIF_X7,
	OPA_PGAC_DIF_X9,
	OPA_PGAC_DIF_X11,
	OPA_PGAC_DIF_X19
}opa_pgac_difference_e;

#define	OPA_EXEN_POS			(16)			
#define	OPA_EXEN_MSK			(0x01ul << OPA_EXEN_POS)								
typedef enum{
	OPA_EXN_DIS			= 0,
	OPA_EXN_EN	
}opa_exn_e;

#define	OPA_INEN_POS			(17)			
#define	OPA_INEN_MSK			(0x01ul << OPA_INEN_POS)								
typedef enum{
	OPA_INN_DIS			= 0,
	OPA_INN_EN	
}opa_inn_e;

#define	OPA_DIFEN_POS			(18)				 
#define	OPA_DIFEN_MSK			(0x01ul << OPA_DIFEN_POS)								
typedef enum{
	OPA_DIF_DIS			= 0,
	OPA_DIF_EN	
}opa_dif_e;

#define	OPA_OUTEN_POS			(20)				 
#define	OPA_OUTEN_MSK			(0x01ul << OPA_OUTEN_POS)								
typedef enum{
	OPA_OUT_DIS			= 0,
	OPA_OUT_EN	
}opa_out_e;

//inline function
static inline void csp_opa_en(csp_opa_t *ptOpaBase)
{
	ptOpaBase->CR |=  OPA_OPAEN_MSK;
}

static inline void csp_opa_dis(csp_opa_t *ptOpaBase)
{
	ptOpaBase->CR &= ~OPA_OPAEN_MSK;
}

static inline void csp_opa_pgac_singel_int(csp_opa_t *ptOpaBase,opa_pgac_singel_e ePcgaSinggel)
{
	ptOpaBase->CR = (ptOpaBase->CR & (~OPA_PGAC_MSK)) | ((ePcgaSinggel << OPA_PGAC_POS));
}

static inline void csp_opa_pgac_difference_int(csp_opa_t *ptOpaBase,opa_pgac_difference_e ePcgaDifference)
{
	ptOpaBase->CR = (ptOpaBase->CR & (~OPA_PGAC_MSK)) | ((ePcgaDifference << OPA_PGAC_POS));
}

static inline void csp_opa_ex_en(csp_opa_t *ptOpaBase)
{
	ptOpaBase->CR |=  OPA_EXEN_MSK;
}

static inline void csp_opa_ex_dis(csp_opa_t *ptOpaBase)
{
	ptOpaBase->CR &= ~OPA_EXEN_MSK;
}

static inline void csp_opa_in_en(csp_opa_t *ptOpaBase)
{
	ptOpaBase->CR |=  OPA_INEN_MSK;
}

static inline void csp_opa_in_dis(csp_opa_t *ptOpaBase)
{
	ptOpaBase->CR &= ~OPA_INEN_MSK;
}

static inline void csp_opa_dif_en(csp_opa_t *ptOpaBase)
{
	ptOpaBase->CR |=  OPA_DIFEN_MSK;
}

static inline void csp_opa_dif_dis(csp_opa_t *ptOpaBase)
{
	ptOpaBase->CR &= ~OPA_DIFEN_MSK;
}

static inline void csp_opa_out_en(csp_opa_t *ptOpaBase)
{
	ptOpaBase->CR |=  OPA_OUTEN_MSK;
}

static inline void csp_opa_out_dis(csp_opa_t *ptOpaBase)
{
	ptOpaBase->CR &= ~OPA_OUTEN_MSK;
}

#endif


