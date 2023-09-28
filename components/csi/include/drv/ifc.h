/***********************************************************************//** 
 * \file  ifc.h
 * \brief  head file of ifc.c
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-18 <td>V0.0  <td>WNN   <td>initial for csi
 * <tr><td> 2022-8-28 <td>V3.0  <td>WNN    <td> add page erase/PGM unction; bug fix
 * </table>
 * *********************************************************************
*/
#ifndef _DRV_IFC_H_
#define _DRV_IFC_H_

#include "csp.h"

extern volatile bool g_bFlashCheckPass;
extern volatile bool g_bFlashPgmDne;

#ifdef __cplusplus
extern "C" {
#endif


/**
\brief Flash Status
*/
typedef struct {
    uint8_t busy  : 1;                   ///< Flash busy flag
    uint8_t error : 1;                   ///< Read/Program/Erase error flag (cleared on start of next operation)
} csi_ifc_status_t;



// Function Declaration

/**
  \brief       config data flash workmode
   * 
  \param[in]   ptIfcBase    pointer of ifc register structure
  \param[in]   bEnable	   	0: normal mode
							1: para mode
*/
void csi_ifc_dflash_paramode_enable(csp_ifc_t *ptIfcBase, bool bEnable);

/** \brief Read data from Flash.
 * 
 *  \param[in] 	ptIfcBase：	pointer of ifc register structure.
 *  \param[in] 	wAddr：		data address（(SHOULD BE WORD ALLIGNED)）
 *  \param[out] wData：		data  Pointer to a buffer storing the data read from Flash.
 *  \param[in] 	wDataNum：	number of data（WORDs）to read.
 *  \return error code:		return error when start address is not word alligned or address beyonds accessable range \ref csi_error_t
 */
csi_error_t csi_ifc_read(csp_ifc_t *ptIfcBase, uint32_t wAddr, uint32_t *data, uint32_t wDataNum);


/** \brief Program data (<page size)to DFLASH. 
 *  \brief NOTE!!! Extra ERASE is NOT needed before programming.
 * 
 *  \param[in] ptIfcBase：	pointer of ifc register structure
 *  \param[in] wAddr：		data address (SHOULD BE WORD ALLIGNED)
 *  \param[in] pwData: 		data  Pointer to a buffer containing the data to be programmed to Flash.
 *  \param[in] wDataWordNum:number of data(WORDS) items to program.
 *  \return error code: 	return CSI_ERROR when address is not valid or PGM verify fails  \ref csi_error_t
 */
csi_error_t csi_ifc_dflash_page_program(csp_ifc_t *ptIfcBase, uint32_t wAddr, uint32_t *pwData, uint32_t wDataWordNum);


/** \brief Program data (<page size)to PFLASH. 
 *  \brief NOTE!!! Extra ERASE is NOT needed before programming.
 * 
 *  \param[in] ptIfcBase：	pointer of ifc register structure
 *  \param[in] wAddr：		Data address (SHOULD BE WORD ALLIGNED)
 *  \param[in] pwData: 		data  Pointer to a buffer containing the data to be programmed to Flash.
 *  \param[in] wDataWordNum:number of data(WORDS) items to program.
 *  \return error code: 	return CSI_ERROR when address is not valid or PGM verify fails  \ref csi_error_t
 */
csi_error_t csi_ifc_pflash_page_program(csp_ifc_t *ptIfcBase, uint32_t wAddr, uint32_t *pwData, uint32_t wDataWordNum);


/** \brief erase one page(DFLASH or PFLASH). 
 *  \brief NOTE!!! Extra ERASE is NOT needed before programming.
 * 
 *  \param[in] ptIfcBase：	pointer of ifc register structure
 *  \param[in] wPageStAddr: Page start address
 *  \return error code:		return CSI_ERROR when wPageStAddr beyonds accessable range
 */
csi_error_t csi_ifc_page_erase(csp_ifc_t *ptIfcBase, uint32_t wPageStAddr);


/** \brief get flash status
 * 
 *  \param ptIfcBase:	pointer of ifc register structure.
 *  \return tStatus:	ifc status, \ref ifc_status_t
 */
csi_ifc_status_t csi_ifc_get_status(csp_ifc_t *ptIfcBase);


/** \brief Change user option
 * 
 *  \param ptIfcBase:	pointer of ifc register structure.
 *  \param wData:		data that to be written into USER OPTION area
 *  \return error code: return CSI_ERROR when PGM verify fails \ref csi_error_t
 */
csi_error_t csi_ifc_set_useroption(csp_ifc_t *ptIfcBase, uint32_t wData);


/** \brief enable flash protection
 * 
 *  \param ptIfcBase：	ptIfcBase pointer of ifc register structure.
 *  \param eProt：		Flash protection mode \ref ifc_prot_e
 *  \return none 
 */
void csi_ifc_set_protection(csp_ifc_t *ptIfcBase, ifc_prot_e eProt);


#ifdef __cplusplus
}
#endif

#endif /* _DRV_IFC_H_ */