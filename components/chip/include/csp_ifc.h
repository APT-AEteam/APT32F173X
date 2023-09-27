

/***********************************************************************//** 
 * \file  csp_ifc.h
 * \brief  IFC description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-10 <td>V0.0  <td>WNN   <td>initial
 * <tr><td> 2023-9-10 <td>V0.0  <td>WNN   <td> add protection PGM, code normalization 
 * </table>
 * *********************************************************************
*/


#ifndef CSP_IFC_H
#define CSP_IFC_H

#include "soc.h"

/// \struct csp_ifc_t
/// \brief IFC register structure
typedef volatile struct {
        __IM uint32_t IDR ;
        __IOM uint32_t CEDR ;
        __IOM uint32_t SRR ;
        __IOM uint32_t CMR ;
        __IOM uint32_t CR ;
        __IOM uint32_t MR ;
        __IOM uint32_t ADDR ;
        uint32_t RESERVED ;
        __OM uint32_t KR ;
        __IOM uint32_t IMCR ;
        __IM uint32_t RISR ;
        __IM uint32_t MISR ;
        __OM uint32_t ICR ;
} csp_ifc_t ;


#define IFC_PFLASH	(0x0)
#define IFC_DFLASH  (0x1)
#define IFC_PFLASH_PAGE_SZ	(64)		//unit: word
#define IFC_DFLASH_PAGE_SZ	(16)		//unit: word

#define IFC_PFLASH_PAGE_MSK	(0xFFFFFF00 ) 
#define IFC_DFLASH_PAGE_MSK	(0xFFFFFFC0 ) 

///CEDR: clock enable disable reg
#define  IFC_CLKEN  (0x01ul) 

///SRR: software reg
#define  IFC_SWRST  (0x01ul) 

///CMR: IFC command reg
#define  IFC_HIDM0  ((0x0ul)<<8) 
#define  IFC_HIDM1  ((0x1ul)<<8) 

typedef enum{
	IFC_ENCRYPT = 0x1 << 20,
	IFC_SWDP	= 0x1 << 19,
	IFC_RDP		= 0x1 << 18,
	IFC_HDP_32K	= 0x1 << 22,
	IFC_HDP_16K	= 0x1 << 21,
	IFC_HDP_8K	= 0x1 << 16,
	IFC_HDP_FULL= 0x1 << 17,
}ifc_prot_e;

typedef enum{
	IFC_PROGRAM = 1,
	IFC_PAGE_ERASE = 2,
	IFC_PAGE_ERASE_HIDM1 = IFC_PAGE_ERASE | IFC_HIDM1,
	IFC_CHIP_ERASE = 4,
	IFC_USEROPTION_ERASE = 5|IFC_HIDM1,
	IFC_PRE_PGM = 6,
	IFC_PRE_PGM_HIDM1 = IFC_PRE_PGM | IFC_HIDM1,
	IFC_PAGE_LAT_CLR = 7,
	IFC_PAGE_LAT_CLR_HIDM1 = IFC_PAGE_LAT_CLR | IFC_HIDM1,
	IFC_SWDREMAP_ERASE = 0xd|IFC_HIDM1,
	IFC_SWDREMAP_ENABLE = 0xe|IFC_HIDM1,
	IFC_USEROPTION_PGM = 0xf|IFC_HIDM1
}ifc_cmd_e;

///CR: to start IFC operation
#define  IFC_START  (0x01ul) 


///MR: IFC operation mode
#define  IFC_PF_WAIT_POS (0)
#define  IFC_PF_WAIT_MSK (0x7ul << IFC_PF_WAIT_POS)
#define  IFC_PF_WAIT0 (0x0ul << IFC_PF_WAIT_POS) 
#define  IFC_PF_WAIT1 (0x1ul << IFC_PF_WAIT_POS) 
#define  IFC_PF_WAIT2 (0x2ul << IFC_PF_WAIT_POS) 
#define  IFC_PF_WAIT3 (0x3ul << IFC_PF_WAIT_POS) 
#define  IFC_PF_WAIT4 (0x4ul << IFC_PF_WAIT_POS) 
#define  IFC_PF_WAIT5 (0x5ul << IFC_PF_WAIT_POS) 
#define  IFC_PF_WAIT6 (0x6ul << IFC_PF_WAIT_POS) 
#define  IFC_PF_WAIT7 (0x7ul << IFC_PF_WAIT_POS) 

#define  IFC_DFLASH_PMODE_POS 8
#define  IFC_DFLASH_PMODE_MSK (0x1 <<IFC_DFLASH_PMODE_POS)
#define  IFC_DFLASH_PMODE (0x1 <<IFC_DFLASH_PMODE_POS)

#define  IFC_PF_SPEED_POS (16)
#define  IFC_PF_SPEED_MSK (0x1<<IFC_PF_SPEED_POS)
#define  IFC_HIGH_SPEED ((0x1ul)) 
#define  IFC_LOW_SPEED ((0x0ul)) 

///KR: ISP key
#define  IFC_USER_KEY (0x5A5A5A5Aul)


///Interrupt regs: IMCR, RISR, MISR, ICR
typedef enum{
	IFC_INT_ERS_END = 1,
	IFC_INT_PGM_END = 2,
	IFC_INT_PEP_END = 4,
	IFC_INT_PROT_ERR = (0x1ul << 12),
	IFC_INT_UDEF_ERR = (0x1ul << 13),
	IFC_INT_ADDR_ERR = (0x1ul << 14),
	IFC_INT_OVW_ERR = (0x1ul << 15),
	IFC_INT_ALL = IFC_INT_ERS_END|IFC_INT_PGM_END|IFC_INT_PEP_END|IFC_INT_PROT_ERR|IFC_INT_UDEF_ERR|IFC_INT_ADDR_ERR|IFC_INT_OVW_ERR
}ifc_int_e;

static inline void csp_ifc_clk_enable(csp_ifc_t *ptIfcBase)
{
	ptIfcBase->CEDR = IFC_CLKEN;
}

static inline void csp_ifc_clk_disable(csp_ifc_t *ptIfcBase)
{
	ptIfcBase->CEDR = (~IFC_CLKEN);
}


static inline void csp_ifc_sw_rst(csp_ifc_t *ptIfcBase)
{
	ptIfcBase->SRR = IFC_SWRST;
}
static inline void csp_ifc_start(csp_ifc_t *ptIfcBase)
{
	ptIfcBase->CR = IFC_START;
}

static inline void csp_ifc_wr_cmd(csp_ifc_t *ptIfcBase, ifc_cmd_e eCmd)
{
	ptIfcBase->CMR = eCmd;
}
static inline void csp_ifc_set_addr(csp_ifc_t *ptIfcBase, uint32_t wAddr)
{
	ptIfcBase->ADDR = wAddr;
}

static inline void csp_ifc_int_enable(csp_ifc_t *ptIfcBase, ifc_int_e eInt)
{
	ptIfcBase->ICR = eInt;
	ptIfcBase->IMCR |= eInt;
}

static inline void csp_ifc_int_disable(csp_ifc_t *ptIfcBase, ifc_int_e eInt)
{
	ptIfcBase->IMCR &= ~eInt;
}

static inline uint32_t csp_ifc_get_isr(csp_ifc_t *ptIfcBase)
{
	return (ptIfcBase->MISR);
}

static inline uint32_t csp_ifc_get_risr(csp_ifc_t *ptIfcBase)
{
	return (ptIfcBase->RISR);
}
static inline void csp_ifc_clr_int(csp_ifc_t *ptIfcBase, ifc_int_e eInt)
{
	ptIfcBase->ICR = eInt;
}
static inline void csp_ifc_unlock(csp_ifc_t *ptIfcBase)
{
	ptIfcBase->KR = IFC_USER_KEY;
}

static inline void csp_ifc_dflash_paramode_enable(csp_ifc_t *ptIfcBase)
{
	ptIfcBase->MR |= IFC_DFLASH_PMODE_MSK;
}

static inline void csp_ifc_dflash_paramode_disable(csp_ifc_t *ptIfcBase)
{
	ptIfcBase->MR &= ~IFC_DFLASH_PMODE_MSK;
}

static inline uint32_t csp_ifc_get_dflash_paramode(csp_ifc_t *ptIfcBase)
{
	return ((ptIfcBase -> MR & IFC_DFLASH_PMODE)>> IFC_DFLASH_PMODE_POS);
}


static inline void csp_ifc_flash_set_speed_wait(csp_ifc_t *ptIfcBase, uint8_t bySpeed,uint8_t byWait)
{
	ptIfcBase->MR = (ptIfcBase->MR &(~IFC_PF_SPEED_MSK)&(~IFC_PF_WAIT_MSK)) | (bySpeed << IFC_PF_SPEED_POS)| (byWait << IFC_PF_WAIT_POS);
}

#endif   /**< CSP_IFC_H */