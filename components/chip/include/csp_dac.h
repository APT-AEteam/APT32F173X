/***********************************************************************//** 
 * \file  csp_dac.h
 * \brief  DAC description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-10-8  <td>V0.1  <td>ljy   <td>modify(enum pattern, add)
 * <tr><td> 2023-9-11  <td>V0.2  <td>xs    <td>fix bug,code normalization
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CSP_DAC_H
#define _CSP_DAC_H

/* Includes ------------------------------------------------------------------*/
#include "soc.h"

/// \struct csp_dac_t
/// \brief  DAC Reg Description
 typedef struct
 {
    __IOM  uint32_t  ECR;			//0x0000  ID & IP Enable/Disable Register        
    __IOM  uint32_t  DACR;      	//0x0004  Analog module control Register                                        
    __IOM  uint32_t  HSCR;      	//0x0008  Frequency division control of a clock used for conversion Register   
    __IOM  uint32_t  DATAINL;       //0x000C  DAC 12-bit left-aligned data holding Register
    __IOM  uint32_t  DATAINR;       //0x0010  DAC 12-bit right-aligned data holding Register               
    __IOM  uint32_t  SYNCR;     	//0x0014  Synchronization Control Register                   
    __IOM  uint32_t  STEP;	   	 	//0x0018  Incremental control Register
    __IM   uint32_t  SR;        	//0x001C  Status Register                             
    __IOM  uint32_t  IMCR;      	//0x0020  Interrupt Masking Control Register                 
    __IM   uint32_t  MISR;      	//0x0024  Masked Interrupt Status Register    Defines 'read  only'
    __OM   uint32_t  ICR;       	//0x0028  Interrupt Clear Register            Defines 'write only' 
 } csp_dac_t;

/******************************************************************************
* ECR : DAC enable/disable and IPcode Register
******************************************************************************/
#define DAC_DACEN_POS			(0)	
#define DAC_DACEN_MSK			(0x01ul <<DAC_DACEN_POS)       
#define DAC_IPIDCODE_POS    	(6)
#define DAC_IPIDCODE_MSK  		(0x3FFFFFFul << DAC_IPIDCODE_POS)

/******************************************************************************
* DACR : Control Register
******************************************************************************/
#define DAC_CLRB_POS			(0)
#define DAC_CLRB_MSK			(0x01ul << DAC_CLRB_POS)
#define DAC_BUFFEN_POS			(1)
#define DAC_BUFFEN_MSK			(0x01ul << DAC_BUFFEN_POS)
#define DAC_REFSEL_POS          (2)
#define DAC_REFSEL_MSK          (0x01ul << DAC_REFSEL_POS)
#define DAC_PD_POS              (3)
#define DAC_PD_MSK              (0x01ul << DAC_PD_POS)
#define DAC_BST_POS             (6)
#define DAC_BST_MSK             (0x01ul << DAC_BST_POS)
/******************************************************************************
* HSCR : Drequency Division Register
******************************************************************************/
#define DAC_HSC_POS			    (0)
#define DAC_HSC_MSK				(0xFF <<  DAC_HSC_POS)
/******************************************************************************
* DATAINL : Data Register left Justified
******************************************************************************/
#define DAC_DATAINL_POS			(0)
#define DAC_DATAINL_MSK			(0xFFFF<< DAC_DATAINL_POS)
/******************************************************************************
* DATAINR : Data Register Right Justified
******************************************************************************/
#define DAC_DATAINR_POS			(0)
#define DAC_DATAINR_MSK			(0xFFFF<<  DAC_DATAINR_POS)
/******************************************************************************
* SYNC : Synchronization Control
******************************************************************************/
#define DAC_SYNC_POS(n)			(0 + n)
#define DAC_SYNC_MSK(n)			(0x01ul << DAC_SYNC_POS(n))

typedef enum{
	DAC_SYNCEN0					= 0,
	DAC_SYNCEN1					= 1,
	DAC_SYNCEN2					= 2,	
}dac_sync_in_e;
/******************************************************************************
* STEP : Incremental Control
******************************************************************************/
#define DAC_STEP_POS            (0)
#define DAC_STEP_MSK        	(0xFFFul)

/******************************************************************************
* SR : Status Register
******************************************************************************/
#define DAC_BUSY_POS          	(0)
#define DAC_BUSY_MSK          	(0x01ul<<  DAC_BUSY_POS)

/******************************************************************************
* IMCR/IMSR/ICR : Interrupt Mask/Status Register
******************************************************************************/
#define DAC_ECO_POS        		(0)
#define DAC_ECO_MSK        		(0x01ul<<  DAC_ECO_POS)
#define DAC_WRERR_POS      		(1)
#define DAC_WRERR_MSK      		(0x01ul<<  DAC_WRERR_POS)
#define DAC_SYNCERR_POS    		(2)
#define DAC_SYNCERR_MSK    		(0x01ul<<  DAC_SYNCERR_POS)

typedef enum{
	DAC_EOC_INT			    	= (0x01uL << 0),
	DAC_WRERR_INT				= (0x01uL << 1),
	DAC_SYNCERR_INT				= (0x01uL << 2),	
	DAC_ALL_INT					= (0x07uL << 0),	
}dac_int_e;
/******************************************************************************
********************* DAC inline Functions Declaration **********************
******************************************************************************/
//CR
static inline void csp_dac_start(csp_dac_t *ptDacBase)
{
	ptDacBase -> ECR |= DAC_DACEN_MSK;
}
static inline void csp_dac_stop(csp_dac_t *ptDacBase)
{
	ptDacBase -> ECR &= ~DAC_DACEN_MSK;
}
static inline void csp_dac_set_clk_div(csp_dac_t *ptDacBase, uint8_t byDiv)
{
	if(byDiv<=2){ }
	else{ptDacBase->HSCR = (ptDacBase->HSCR & (~DAC_HSC_MSK)) | ((byDiv-1) );}//FCK = FHCLK / (HSC+1)，
}
static inline void csp_dac_clr_dac(csp_dac_t *ptDacBase)
{
	ptDacBase -> DACR |=  DAC_CLRB_MSK ;
}
static inline void csp_dac_buff_enable(csp_dac_t *ptDacBase,bool bEnable)
{
	if(bEnable)ptDacBase -> DACR |=   DAC_BUFFEN_MSK ;
	else{      ptDacBase -> DACR &=  ~DAC_BUFFEN_MSK;}
}
static inline void csp_dac_refsel_enable(csp_dac_t *ptDacBase, bool bEnable)
{
	if(bEnable)ptDacBase -> DACR |=   DAC_REFSEL_MSK ;
	else{      ptDacBase -> DACR &=  ~DAC_REFSEL_MSK;}
}
static inline void csp_dac_powerdown_enable(csp_dac_t *ptDacBase, bool bEnable)
{
	if(bEnable)ptDacBase -> DACR |=   DAC_PD_MSK ;
	else{      ptDacBase -> DACR &=  ~DAC_PD_MSK;}
}
//DATA
static inline void csp_dac_set_datal(csp_dac_t *ptDacBase, uint16_t hwDatal)
{	
	ptDacBase->DATAINL = (ptDacBase->DATAINL & (~DAC_DATAINL_MSK)) | (hwDatal<<4 );
}
static inline void csp_dac_set_datar(csp_dac_t *ptDacBase, uint16_t hwDatar)
{	
	ptDacBase->DATAINR = (ptDacBase->DATAINR & (~DAC_DATAINR_MSK)) | (hwDatar);
}
//SYNC
static inline void csp_dac_sync_enable(csp_dac_t *ptDacBase, dac_sync_in_e eSyncIn)
{
	ptDacBase->SYNCR |= DAC_SYNC_MSK(eSyncIn);
}
static inline void csp_dac_sync_disable(csp_dac_t *ptDacBase, dac_sync_in_e eSyncIn)
{
	ptDacBase->SYNCR &= ~DAC_SYNC_MSK(eSyncIn);
}
static inline void csp_dac_set_step(csp_dac_t *ptDacBase, uint16_t hwData)
{	
	ptDacBase->STEP = (ptDacBase->STEP & (~DAC_STEP_MSK)) | (hwData);
}
//STATUDE
static inline bool csp_dac_get_busy(csp_dac_t *ptDacBase)
{
	return (bool)(ptDacBase->SR & 0x01);
}
//interrupt
static inline void csp_dac_int_enable(csp_dac_t *ptDacBase, dac_int_e eInt)
{
	ptDacBase->IMCR |= eInt;						
}
static inline void csp_dac_int_disable(csp_dac_t *ptDacBase, dac_int_e eInt)
{
	ptDacBase->IMCR &= ~eInt;
}
static inline uint32_t csp_dac_get_isr(csp_dac_t *ptDacBase)
{
	return (uint32_t)(ptDacBase->MISR);
}
static inline void csp_dac_clr_isr(csp_dac_t *ptDacBase, dac_int_e eInt)
{
	ptDacBase->ICR = eInt;
}
#endif


