/***********************************************************************//** 
 * \file  ifc.c
 * \brief  flash driver
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-28 <td>V2.0 <td>WNN    <td>initial
 * <tr><td> 2022-8-28 <td>V3.0 <td>WNN    <td> add page erase/PGM unction; support SWD/user option PGM; bug fix
 * <tr><td> 2029-9-27 <td>V4.0 <td>WNN    <td> support protection PGM,bug fix, code normalization
 * </table>
 * *********************************************************************
*/

/* include ----------------------------------------------------------------*/
#include "ifc.h"

/* global variables--------------------------------------------------------*/
volatile bool g_bFlashCheckPass = 1;
volatile bool g_bFlashPgmDne = 1;

/* private function--------------------------------------------------------*/
static void apt_ifc_step_sync(csp_ifc_t * ptIfcBase, ifc_cmd_e eCmd, uint32_t wPageStAddr);
static void apt_ifc_step_async(csp_ifc_t * ptIfcBase, ifc_cmd_e eCmd, uint32_t wPageStAddr);
static csi_error_t apt_ifc_wr_nword(csp_ifc_t * ptIfcBase, ifc_type_e eFlashType, uint32_t wAddr, uint32_t wDataNum, uint32_t *pwData);

/* Private variables------------------------------------------------------*/
volatile static uint32_t s_wPageStAddr;
volatile static uint32_t s_wBuffForCheck[IFC_DFLASH_PAGE_SZ];


/** \brief ifc interrupt handle function,only used for para-mode PGM of DFLASH
 * 
 *  \param none
 *  \return none
 */ 
void ifc_irqhandler(void)
{
	uint32_t i;

	if (csp_ifc_get_isr(IFC) == IFC_INT_ERS_END)
	{
		csp_ifc_int_disable(IFC, IFC_INT_ERS_END);
		csp_ifc_clr_int(IFC, IFC_INT_ERS_END);
		///DFLASH step6
		apt_ifc_step_async(IFC, IFC_PROGRAM, s_wPageStAddr);
	}
	if (csp_ifc_get_isr(IFC) == IFC_INT_PGM_END)
	{
		csp_ifc_int_disable(IFC, IFC_INT_PGM_END);
		csp_ifc_clr_int(IFC, IFC_INT_PGM_END);
		///whole page check, only DFlash Write would use INT scheme
		g_bFlashCheckPass = 1;
		for (i=0; i<IFC_DFLASH_PAGE_SZ; i++)
		{
			if ((*(uint32_t *)(s_wPageStAddr+4*i)) !=  s_wBuffForCheck[i]) {
				g_bFlashCheckPass = 0;
				g_bFlashPgmDne = 1;
				break;
				
			}
		}
		g_bFlashPgmDne = 1;
	}
}

/** \brief config data flash PGM mode
 * 
 *  \param[in] ptIfcBase: 	pointer of ifc register structure
 *  \param[in] bEnable: 	ENABLE/DISABLE
 *  \return none
 */
/** \brief       config data flash workmode
 *
 * \param[in]   ptIfcBase   pointer of ifc register structure
 * \param[in]   bEnable	   	0: normal mode
							1: para mode
  \return none
*/

void csi_ifc_dflash_paramode_enable(csp_ifc_t *ptIfcBase, bool bEnable)
{
	csp_ifc_clk_enable(ptIfcBase);
	csp_ifc_dflash_paramode_enable(IFC);
}

/** \brief Read data from Flash.
 * 
 *  \param[in] 	ptIfcBase：	pointer of ifc register structure.
 *  \param[in] 	wAddr：		data address（(SHOULD BE WORD ALLIGNED)）
 *  \param[out] wData：		data  Pointer to a buffer storing the data read from Flash.
 *  \param[in] 	wDataNum：	number of data（WORDs）to read.
 *  \return error code:		return error when start address is not word alligned or address beyonds accessable range \ref csi_error_t
 */
csi_error_t csi_ifc_read(csp_ifc_t *ptIfcBase, uint32_t wAddr, uint32_t *wData, uint32_t wDataNum)
{
	
	uint32_t i, *wDataBuff = (uint32_t *)wData;
	
	if (wAddr % 4 != 0 )
	{
		return CSI_ERROR;
	}	
	else if (((wAddr < DFLASH_BASE) && ((wAddr + (wDataNum<<2))>PFLASH_LIMIT) )|| ((wAddr>=DFLASH_BASE)&& ((wAddr + (wDataNum<<2))> DFLASH_LIMIT))) {
		return CSI_ERROR;
	}
	
	csp_ifc_clk_enable(ptIfcBase);
	
	wDataBuff = (uint32_t *)wData;
	
	for(i=0; i<wDataNum ; i++)
	{

		*wDataBuff = (*(uint32_t *)wAddr);
		wDataBuff ++;
		wAddr += 4;
	}
	return CSI_OK;
}


/** \brief Program data (<page size)to DFLASH. 
 *  \brief NOTE!!! Extra ERASE is NOT needed before programming.
 * 
 *  \param[in] ptIfcBase：	pointer of ifc register structure
 *  \param[in] wAddr：		data address (SHOULD BE WORD ALLIGNED)
 *  \param[in] pwData: 		data  Pointer to a buffer containing the data to be programmed to Flash.
 *  \param[in] wDataWordNum:number of data(WORDS) items to program.
 *  \return error code: 	return CSI_ERROR when address is not valid or PGM verify fails  \ref csi_error_t
 */
csi_error_t csi_ifc_dflash_page_program(csp_ifc_t *ptIfcBase, uint32_t wAddr, uint32_t *pwData, uint32_t wDataWordNum)
{
	csi_error_t tRet = CSI_OK;
	uint32_t wPageLimit = (wAddr & (IFC_DFLASH_PAGE_MSK)) + IFC_DFLASH_PAGE_SZ*4;
	
	//return error when address is not word alligned, or addr goes beyond space size or addr goes across pages
	if (wAddr % 4 != 0 || (wPageLimit > DFLASH_LIMIT) || ((wAddr + (wDataWordNum<<2))> wPageLimit) ) 
	{
		tRet = CSI_ERROR;
		return tRet;
	}
	
	csp_ifc_clk_enable(ptIfcBase);
	
	tRet = (csi_error_t)apt_ifc_wr_nword(ptIfcBase, IFC_DFLASH, wAddr, wDataWordNum, pwData);	
	if (tRet == CSI_ERROR)
		return tRet;
	return CSI_OK;
}



/** \brief Program data (<page size)to PFLASH. 
 *  \brief NOTE!!! Extra ERASE is NOT needed before programming.
 * 
 *  \param[in] ptIfcBase：	pointer of ifc register structure
 *  \param[in] wAddr：		Data address (SHOULD BE WORD ALLIGNED)
 *  \param[in] pwData: 		data  Pointer to a buffer containing the data to be programmed to Flash.
 *  \param[in] wDataWordNum:number of data(WORDS) items to program.
 *  \return error code: 	return CSI_ERROR when address is not valid or PGM verify fails \ref csi_error_t
 */
csi_error_t csi_ifc_pflash_page_program(csp_ifc_t *ptIfcBase, uint32_t wAddr, uint32_t *pwData, uint32_t wDataWordNum)
{
	csi_error_t tRet = CSI_OK;
	uint32_t wPageLimit = (wAddr & (IFC_PFLASH_PAGE_MSK)) + IFC_PFLASH_PAGE_SZ*4;
	
	//return error when address is not word alligned, or addr goes beyond space size or addr goes across pages
	if (wAddr % 4 != 0 || (wPageLimit > PFLASH_LIMIT) || ((wAddr + (wDataWordNum<<2))> wPageLimit) ) 
	{
		tRet = CSI_ERROR;
		return tRet;
	}
	
	csp_ifc_clk_enable(ptIfcBase);
	
	tRet = (csi_error_t)apt_ifc_wr_nword(ptIfcBase, IFC_PFLASH, wAddr, wDataWordNum, pwData);	
	if (tRet == CSI_ERROR)
		return tRet;
	return CSI_OK;
}


/** \brief get flash status
 * 
 *  \param ptIfcBase:	pointer of ifc register structure.
 *  \return tStatus:	ifc status, \ref ifc_status_t
 */
csi_ifc_status_t csi_ifc_get_status(csp_ifc_t *ptIfcBase)
{
	csi_ifc_status_t tStatus;
   
	csp_ifc_clk_enable(ptIfcBase);
	
	if (g_bFlashPgmDne)
		tStatus.busy = 0;
	else
		tStatus.busy =  1;
	if (csp_ifc_get_risr(ptIfcBase) & (IFC_INT_PROT_ERR|IFC_INT_UDEF_ERR|IFC_INT_OVW_ERR|IFC_INT_ADDR_ERR)) {
		tStatus.error = 1;
		g_bFlashPgmDne = 1;
		tStatus.busy = 0;
	}
	else
		tStatus.error = 0;
		
	return tStatus;
}

/** \brief erase one page(DFLASH or PFLASH). 
 *  \brief NOTE!!! Extra ERASE is NOT needed before programming.
 * 
 *  \param[in] ptIfcBase：	pointer of ifc register structure
 *  \param[in] wPageStAddr: Page start address
 *  \return error code:		return CSI_ERROR when wPageStAddr beyonds accessable range
 */
csi_error_t csi_ifc_page_erase(csp_ifc_t *ptIfcBase, uint32_t wPageStAddr)
{
	 if (((wPageStAddr < DFLASH_BASE) && ((wPageStAddr )>PFLASH_LIMIT) )|| ((wPageStAddr>=DFLASH_BASE)&& ((wPageStAddr )> DFLASH_LIMIT))) {
		return CSI_ERROR;
	}
	csp_ifc_clk_enable(ptIfcBase);	
	apt_ifc_step_sync(ptIfcBase, IFC_PAGE_ERASE, wPageStAddr);
	return CSI_OK;
	
}


/** \brief Change user option
 * 
 *  \param ptIfcBase:	pointer of ifc register structure.
 *  \param wData:		data that to be written into USER OPTION area
 *  \return error code: return CSI_ERROR when PGM verify fails \ref csi_error_t
 */
csi_error_t csi_ifc_set_useroption(csp_ifc_t *ptIfcBase, uint32_t wData)
{
	
	csi_error_t tRet = CSI_OK;
    uint32_t i,wBuff[16],wPageStAddr,wOffsetAddr,bPageSize = 16;
	
    wPageStAddr = USEROPTION_ADDR;
    wOffsetAddr = 0;
	
	csp_ifc_clk_enable(IFC);
	///step1
	apt_ifc_step_sync(ptIfcBase, IFC_PAGE_LAT_CLR_HIDM1, 0);
	///step2
	for(i=0; i< bPageSize; i++) {
      if( i == wOffsetAddr )
	  {
		wBuff[i++] = wData;
		
	  }
      else {
        wBuff[i] = *(uint32_t *)(wPageStAddr+4*i);
      }
    }
	for(i=0; i<bPageSize; i++) {
        *(uint32_t *)(wPageStAddr+4*i) = wBuff[i];
    }
	///step3
	apt_ifc_step_sync(ptIfcBase, IFC_PRE_PGM_HIDM1, 0);
	///step4
	apt_ifc_step_sync(ptIfcBase, IFC_USEROPTION_PGM, 0);
	///step5
	apt_ifc_step_sync(ptIfcBase, IFC_USEROPTION_ERASE, 0);
	///step6
	apt_ifc_step_sync(ptIfcBase, IFC_USEROPTION_PGM, 0);
	///check
	
	
	if (*(uint32_t *)USEROPTION_ADDR != wData) {
		g_bFlashCheckPass = 0;
		tRet = CSI_ERROR;

	}
	else {
		g_bFlashCheckPass = 1;
		tRet = CSI_OK;
	}
	
	g_bFlashPgmDne = 1;	
	return tRet;
}


/** \brief enable flash protection
 * 
 *  \param ptIfcBase：	ptIfcBase pointer of ifc register structure.
 *  \param eProt：		Flash protection mode \ref ifc_prot_e
 *  \return none 
 */
void csi_ifc_set_protection(csp_ifc_t *ptIfcBase, ifc_prot_e eProt)
{
    uint16_t i, hwPageSize = 16;
	uint32_t wBuff[16];
	uint32_t wPageStAddr = USEROPTION_ADDR;
	
	csp_ifc_clk_enable(IFC);
	///step1
	apt_ifc_step_sync(ptIfcBase, IFC_PAGE_LAT_CLR_HIDM1, 0);
	///step2
	csp_ifc_unlock(ptIfcBase);
	csp_ifc_wr_cmd(ptIfcBase, IFC_USEROPTION_PGM| eProt);
	
	wBuff[0] = *(uint32_t *)USEROPTION_ADDR;
	wBuff[0] = 0x5;
	
	for(i=0; i<hwPageSize; i++) {
        *(uint32_t *)(wPageStAddr+4*i) = wBuff[i];
    }
	///step3
	apt_ifc_step_sync(ptIfcBase, IFC_PRE_PGM_HIDM1, 0);
	///step4
	apt_ifc_step_sync(ptIfcBase, IFC_USEROPTION_PGM, 0);

	///step5
	apt_ifc_step_sync(ptIfcBase, IFC_USEROPTION_ERASE, 0);
	///step6
	apt_ifc_step_sync(ptIfcBase, IFC_USEROPTION_PGM| eProt, 0);
	
}


/** \brief     IFC operation step, used for non-paramode operations
 * 
 *  \param[in] ptIfcBase：	ptIfcBase pointer of ifc register structure.
 *  \param[in] eCmd:		ifc command \ref ifc_cmd_e
 *  \param[in] wPageStAddr:	page address
 *  \return none
 */
static void apt_ifc_step_sync(csp_ifc_t * ptIfcBase, ifc_cmd_e eCmd, uint32_t wPageStAddr)
{
	csp_ifc_unlock(ptIfcBase);
	csp_ifc_wr_cmd(ptIfcBase, eCmd);
	csp_ifc_set_addr(ptIfcBase, wPageStAddr);
	csp_ifc_start(ptIfcBase);
	
	if (((ptIfcBase -> MR) & IFC_DFLASH_PMODE) && (wPageStAddr >= DFLASH_BASE))
	{
		switch (eCmd)
		{
			case (IFC_PROGRAM): while(ptIfcBase->RISR != IFC_INT_PEP_END){}; 	// Wait for operation done
							csp_ifc_clr_int(ptIfcBase, IFC_INT_PEP_END);
							break;
			case (IFC_PAGE_ERASE): while(ptIfcBase->RISR != IFC_INT_ERS_END){}; // Wait for operation done
							csp_ifc_clr_int(ptIfcBase, IFC_INT_ERS_END);
							break;
			default: break;
		}
	}
	else {
		while(ptIfcBase->CR != 0);
	}
} 

/** \brief IFC operation step, used for paramode operations
 * 
 *  \param[in] ptIfcBase：	ptIfcBase pointer of ifc register structure.
 *  \param[in] eCmd:		ifc command \ref ifc_cmd_e
 *  \param[in] wPageStAddr:	Page address
 *  \return none
 */
void apt_ifc_step_async(csp_ifc_t * ptIfcBase, ifc_cmd_e eCmd, uint32_t wPageStAddr)
{
	csp_ifc_unlock(ptIfcBase);
	switch (eCmd)
	{
		case (IFC_PAGE_ERASE):
			csp_ifc_int_enable(ptIfcBase, IFC_INT_ERS_END);
			break;
		case (IFC_PROGRAM):
			csp_ifc_int_enable(ptIfcBase, IFC_INT_PGM_END);
			break;
		default: 
			break;
	}
	
	csp_ifc_wr_cmd(ptIfcBase, eCmd);
	csp_ifc_set_addr(ptIfcBase, wPageStAddr);
	csp_ifc_start(ptIfcBase);
} 

/** \brief 		write n words
 * 
 *  \param[in]	ptIfcBase：	ptIfcBase pointer of ifc register structure.
 *  \param[in] 	eFlashType:	ifc type, \ref ifc_type_e
 *  \param[in] 	wAddr:		PGM start address
 * 	\param[in] 	wDataNum:	number of words to be written
 *  \param[in]	pwData:		point points to data to be written
 *  \return error code:		return CSI_ERROR when PGM verify fails \ref csi_error_t	
 */
static csi_error_t apt_ifc_wr_nword(csp_ifc_t * ptIfcBase, ifc_type_e eFlashType, uint32_t wAddr, uint32_t wDataNum, uint32_t *pwData)
{
	uint32_t i, j, wPageStAddr, wBuff[IFC_PFLASH_PAGE_SZ];
	uint8_t bPageSize = IFC_DFLASH_PAGE_SZ;
	csi_error_t tRet = CSI_OK;
	
	csi_ifc_get_status(IFC);
	while(!g_bFlashPgmDne);
	g_bFlashPgmDne = 0;
	
	if (eFlashType == IFC_PFLASH) {
		bPageSize = IFC_PFLASH_PAGE_SZ;
		wPageStAddr = wAddr & IFC_PFLASH_PAGE_MSK;
	}
	else {
		bPageSize = IFC_DFLASH_PAGE_SZ;
		wPageStAddr = wAddr & IFC_DFLASH_PAGE_MSK;
	}
	
	wAddr -= wPageStAddr;
	wAddr = wAddr >> 2;	

	///step1
	apt_ifc_step_sync(ptIfcBase, IFC_PAGE_LAT_CLR, wPageStAddr);
	///step2
	for(i=0; i< bPageSize; i++) {
      if( i == wAddr )
	  {
		for(j = 0; j<wDataNum; j++)
			wBuff[i++] = pwData[j];
		i--;
	  }
      else {
        wBuff[i] = *(uint32_t *)(wPageStAddr+4*i);
      }
    }
	for(i=0; i<bPageSize; i++) {
        *(uint32_t *)(wPageStAddr+4*i) = wBuff[i];
    }
	///step3
	apt_ifc_step_sync(ptIfcBase, IFC_PRE_PGM, wPageStAddr);
	///step4
	apt_ifc_step_sync(ptIfcBase, IFC_PROGRAM, wPageStAddr);

	if (eFlashType == IFC_DFLASH && csp_ifc_get_dflash_paramode(ptIfcBase) == 1)
	{
		///DFLASH step4 
		for (i=0; i< IFC_DFLASH_PAGE_SZ;i++)
		{
			s_wBuffForCheck[i] = wBuff[i];
		}
		s_wPageStAddr = wPageStAddr;
		apt_ifc_step_async(ptIfcBase, IFC_PAGE_ERASE, wPageStAddr);
	}
	else 
	{
	///step5
		apt_ifc_step_sync(ptIfcBase, IFC_PAGE_ERASE, wPageStAddr);
	///step6
		apt_ifc_step_sync(ptIfcBase, IFC_PROGRAM, wPageStAddr);
	///whole page check
		for (i=0; i<bPageSize; i++)
		{
			if (*(uint32_t *)(wPageStAddr+4*i) != wBuff[i]){
				tRet = CSI_ERROR;
				g_bFlashCheckPass = 0;
				g_bFlashPgmDne = 1;
				return tRet;
			}
		}
		if (tRet != CSI_ERROR)
			g_bFlashPgmDne = 1;
	}
	return tRet;
}

