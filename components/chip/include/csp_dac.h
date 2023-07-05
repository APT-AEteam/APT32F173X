/***********************************************************************//** 
 * \file  csp_dac.h
 * \brief  DAC description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 
 * <tr><td> 2021-10-8  <td>V0.1  <td>ljy   <td>modify(enum pattern, add)
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CSP_DAC_H
#define _CSP_DAC_H

/* Includes ------------------------------------------------------------------*/
#include <soc.h>
#include <stdint.h>

/// \struct csp_dac_t
/// \brief  DAC Reg Description
 typedef struct
 {
    __IOM  uint32_t  DAC_ECR;		//0x0000  ID & IP Enable/Disable Register        
    __IOM  uint32_t  DAC_DACR;      //0x0004  Analog module control Register                                        
    __IOM  uint32_t  DAC_HSCR;      //0x0008  Frequency division control of a clock used for conversion Register   
    __IOM  uint32_t  DATAINL;       //0x000C  DAC 12-bit left-aligned data holding Register
    __IOM  uint32_t  DATAINR;       //0x0010  DAC 12-bit right-aligned data holding Register               
    __IOM  uint32_t  DAC_SYNCR;     //0x0014  Synchronization Control Register                   
    __IOM  uint32_t  DAC_STEP;	    //0x0018  Incremental control Register
    __IM   uint32_t  DAC_SR;        //0x001C  Status Register                             
    __IOM  uint32_t  DAC_IMCR;      //0x0020  Interrupt Masking Control Register                 
    __IM   uint32_t  DAC_MISR;      //0x0024  Masked Interrupt Status Register    Defines 'read  only'
    __OM   uint32_t  DAC_ICR;       //0x0028  Interrupt Clear Register            Defines 'write only' 
  
 } csp_dac_t;


typedef enum{
	DAC_SYNCIN0			= (0x01uL << 0),
	DAC_SYNCIN1			= (0x01uL << 1),
	DAC_SYNCIN2			= (0x01uL << 2),
	
}dac_syncr_e;

typedef enum{
	DAC_EOC			    = (0x01uL << 0),
	DAC_WRERR			= (0x01uL << 1),
	DAC_SYNCERR			= (0x01uL << 2),
	
}dac_irq_e;
/******************************************************************************
* ECR, 
******************************************************************************/
#define DAC_DACEN_POS			(0)                 				/**< DAC            */
#define DAC_DACEN_MSK			(0x01ul <<DAC_DACEN_POS)            /**< DAC            */
#define DAC_IPIDCODE_POS    	(6)
#define DAC_IPIDCODE_MSK  		(0x3FFFFFFul << DAC_IPIDCODE_POS)   /**< DAC IPIDCODE mask   */


/******************************************************************************
* DACR, 
******************************************************************************/
#define DAC_CLRB_POS			(0)      						 /**< DAC            */
#define DAC_CLRB_MSK			(0x01ul << DAC_CLRB_POS)    	 /**< DAC            */
#define DAC_BUFFEN_POS			(1)       						 /**< DAC            */
#define DAC_BUFFEN_MSK			(0x01ul << DAC_BUFFEN_POS)       /**< DAC            */
#define DAC_REFSEL_POS          (2)       						 /**< DAC            */
#define DAC_REFSEL_MSK          (0x01ul << DAC_REFSEL_POS)       /**< DAC            */
#define DAC_PD_POS              (3)       						 /**< DAC            */
#define DAC_PD_MSK              (0x01ul << DAC_PD_POS)      	 /**< DAC            */
#define DAC_BST_POS             (6)      						 /**< DAC            */
#define DAC_BST_MSK             (0x01ul << DAC_BST_POS)       	 /**< DAC            */
/******************************************************************************
 DAC_HSCR
******************************************************************************/
#define DAC_HSC_POS			    (0)       /**< DAC            */
#define DAC_HSC_MSK				(0xFF <<  DAC_HSC_POS         )       /**< DAC            */
/******************************************************************************
DATAIN_L
******************************************************************************/
#define DAC_DATAINL_POS			(0)       /**< DAC            */
#define DAC_DATAINL_MSK			(0xFFFF<< DAC_DATAINL_POS      )       /**< DAC            */
/******************************************************************************
DATAIN_R
******************************************************************************/
#define DAC_DATAINR_POS			( 0)       /**< DAC            */
#define DAC_DATAINR_MSK			(0xFFFF<<  DAC_DATAINR_POS     )       /**< DAC            */
/******************************************************************************
 DAT_SYNC
******************************************************************************/
#define DAC_SYNCEN0_POS         (0) 
#define DAC_SYNCEN0_MSK     	(0x01ul<<  DAC_SYNCEN0_POS   )
#define DAC_SYNCEN1_POS         (1)
#define DAC_SYNCEN1_MSK     	(0x01ul<<  DAC_SYNCEN1_POS   )
#define DAC_SYNCEN2_POS         (2)
#define DAC_SYNCEN2_MSK     	(0x01ul<<  DAC_SYNCEN2_POS   )
/******************************************************************************
DAC_STEP
******************************************************************************/
#define DAC_STEP_POS            (0)
#define DAC_STEP_MSK        	(0xFFFul   )

/******************************************************************************
DAC_SR
******************************************************************************/
#define DAC_SR_POS          	(0)
#define DAC_SR_MSK          	(0x01ul<<  DAC_SR_POS   )

/******************************************************************************
 DAT_IMCR
******************************************************************************/
#define DAC_IMCR_ECO_POS             0 
#define DAC_IMCR_ECO_MSK          (0x01ul<<  DAC_IMCR_ECO_POS   )
#define DAC_IMCR_WRERR_POS           1
#define DAC_IMCR_WRERR_MSK        (0x01ul<<  DAC_IMCR_WRERR_POS   )
#define DAC_IMCR_SYNCERR_POS         2
#define DAC_IMCR_SYNCERR_MSK      (0x01ul<<  DAC_IMCR_SYNCERR_POS   )

/******************************************************************************
 DAT_IMSR
******************************************************************************/
#define DAC_IMSR_ECO_POS             (0) 
#define DAC_IMSR_ECO_MSK          (0x01ul<<  DAC_IMSR_ECO_POS   )
#define DAC_IMSR_WRERR_POS           (1)
#define DAC_IMSR_WRERR_MSK        (0x01ul<<  DAC_IMSR_WRERR_POS   )
#define DAC_IMSR_SYNCERR_POS         (2)
#define DAC_IMSR_SYNCERR_MSK      (0x01ul<<  DAC_IMSR_SYNCERR_POS   )

/******************************************************************************
 DAT_ICR
******************************************************************************/
#define DAC_ICR_ECO_POS             0 
#define DAC_ICR_ECO_MSK          	(0x01ul<<  DAC_ICR_ECO_POS   )
#define DAC_ICR_WRERR_POS           1
#define DAC_ICR_WRERR_MSK        	(0x01ul<<  DAC_ICR_WRERR_POS   )
#define DAC_ICR_SYNCERR_POS         2
#define DAC_ICR_SYNCERR_MSK      	(0x01ul<<  DAC_ICR_SYNCERR_POS   )




static inline void csp_dac_vic_irq_en(void)
{
    //NVIC_EnableIRQ(DAC0_IRQn);
}

static inline void csp_dac_vic_irq_dis(void)
{
	//NVIC_DisableIRQ(DAC0_IRQn);
}

static inline void csp_dac_start(csp_dac_t *ptDacBase)
{
	ptDacBase -> DAC_ECR |= 0x1 << DAC_DACEN_POS;
}
static inline void csp_dac_stop(csp_dac_t *ptDacBase)
{
	ptDacBase -> DAC_ECR &= ~(0x1 << DAC_DACEN_POS);
}
static inline void csp_dac_set_clk_div(csp_dac_t *ptDacBase, uint8_t byDiv)
{
	if(byDiv<=2){ }
	else{ptDacBase->DAC_HSCR = (ptDacBase->DAC_HSCR & (~DAC_HSC_MSK)) | ((byDiv-1) );}//FCK = FHCLK / (HSC+1)，
}

static inline void csp_dac_clr_da(csp_dac_t *ptDacBase)
{
	ptDacBase -> DAC_DACR |=  DAC_CLRB_MSK ;
}

static inline void csp_dac_buff_enable(csp_dac_t *ptDacBase,bool bEnable)
{
	if(bEnable)ptDacBase -> DAC_DACR |=   DAC_BUFFEN_MSK ;
	else{      ptDacBase -> DAC_DACR  =   ptDacBase -> DAC_DACR &(~DAC_BUFFEN_MSK) ;}
}

static inline void csp_dac_refsel_enable(csp_dac_t *ptDacBase, bool bEnable)
{
	if(bEnable)ptDacBase -> DAC_DACR |=   DAC_REFSEL_MSK ;
	else{      ptDacBase -> DAC_DACR  =   ptDacBase -> DAC_DACR &(~DAC_REFSEL_MSK) ;}

}

static inline void csp_dac_powerdown_enable(csp_dac_t *ptDacBase, bool bEnable)
{
	if(bEnable)ptDacBase -> DAC_DACR |=   DAC_PD_MSK ;
	else{         ptDacBase -> DAC_DACR  =   ptDacBase -> DAC_DACR &(~DAC_PD_MSK) ;}

}

static inline void csp_dac_set_datal(csp_dac_t *ptDacBase, uint16_t byDer)
{	
	ptDacBase->DATAINL = (ptDacBase->DATAINL & (~DAC_DATAINL_MSK)) | (byDer<<4 );
}

static inline void csp_dac_set_datar(csp_dac_t *ptDacBase, uint16_t byDer)
{	
	ptDacBase->DATAINR = (ptDacBase->DATAINR & (~DAC_DATAINR_MSK)) | (byDer );
}

static inline void csp_dac_syncr_enable(csp_dac_t *ptDacBase, dac_syncr_e byVal,bool bEnable)
{	
	switch(byVal)
	{
		case DAC_SYNCIN0:						//
		     ptDacBase->DAC_SYNCR = (ptDacBase->DAC_SYNCR & (~DAC_SYNCIN0)) | (bEnable<<0 );
			break;
		case DAC_SYNCIN1:						//
		     ptDacBase->DAC_SYNCR = (ptDacBase->DAC_SYNCR & (~DAC_SYNCIN1)) | (bEnable<<1 );
			break;
		case DAC_SYNCIN2:						//
		     ptDacBase->DAC_SYNCR = (ptDacBase->DAC_SYNCR & (~DAC_SYNCIN2)) | (bEnable<<2 );
			break;
		default:
					
			break;		
	}
}


static inline void csp_dac_step_val(csp_dac_t *ptDacBase, uint16_t byDer)
{	
	ptDacBase->DAC_STEP = (ptDacBase->DAC_STEP & (~DAC_STEP_MSK)) | (byDer );
}

static inline bool csp_dac_get_busy(csp_dac_t *ptDacBase)
{
	return (bool)(ptDacBase->DAC_SR & 0x01);
}

static inline void csp_dac_irq_enable(csp_dac_t *ptDacBase, dac_irq_e byVal,bool bEnable)
{	
	switch(byVal)
	{
		case DAC_EOC:						//
		     ptDacBase->DAC_IMCR = (ptDacBase->DAC_IMCR & (~DAC_IMCR_ECO_MSK)) | (bEnable<<0 );
			break;
		case DAC_WRERR:						//
		     ptDacBase->DAC_IMCR = (ptDacBase->DAC_IMCR & (~DAC_IMCR_WRERR_MSK)) | (bEnable<<1 );
			break;
		case DAC_SYNCERR:						//
		     ptDacBase->DAC_IMCR = (ptDacBase->DAC_IMCR & (~DAC_IMCR_SYNCERR_MSK)) | (bEnable<<2 );
			break;
		default:
					
			break;	
	}
}

static inline uint32_t csp_dac_irq_stata(csp_dac_t *ptDacBase, uint32_t byVal)
{	
	switch(byVal)
	{
		case DAC_EOC:						//
		     return (bool)( (ptDacBase->DAC_MISR  & DAC_IMSR_ECO_MSK)  );
			break;
		case DAC_WRERR:						//
		     return (bool)( (ptDacBase->DAC_MISR  & DAC_IMSR_WRERR_MSK) >> 1 );
			break;
		case DAC_SYNCERR:						//
		     return (bool)( (ptDacBase->DAC_MISR  & DAC_IMSR_SYNCERR_MSK) >> 2 );
			break;
		default:		
			return ptDacBase->DAC_MISR;
            break;		
	}
}

static inline void csp_dac_irq_clr(csp_dac_t *ptDacBase,dac_irq_e byVal)
{	
	switch(byVal)
	{
		case DAC_EOC:						//
		     ptDacBase->DAC_ICR  |= DAC_ICR_ECO_MSK;
			break;
		case DAC_WRERR:						//
		     ptDacBase->DAC_ICR  |= DAC_ICR_WRERR_MSK;
			break;
		case DAC_SYNCERR:						//
		     ptDacBase->DAC_ICR  |= DAC_ICR_SYNCERR_MSK;
			break;
		default:
					
			break;	
	}
}
static inline uint32_t csp_dac_get_misr(csp_dac_t *ptDacBase)
{
	return (uint32_t)(ptDacBase->DAC_MISR);
}
#endif

