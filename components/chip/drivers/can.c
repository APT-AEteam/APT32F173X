/***********************************************************************//** 
 * \file  can.c
 * \brief  CAN description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-7-09 <td>V0.0  <td>ZJY   <td>initial
 * </table>
 * *********************************************************************
*/

/* Includes ---------------------------------------------------------------*/
#include <sys_clk.h>
#include <drv/irq.h>
#include <drv/can.h>

#include "csp_can.h"

/* Private macro-----------------------------------------------------------*/
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/
extern 	csi_can_bittime_t  tBitTime[];

//const csi_can_bittime_t  tBitTime[] = {
//	//pclk = 6MHz
//	//Baudrate			SyncJump	PhaseSeg1	PhaseSeg2	BaudDiv
//	{CAN_BDR_200K, 		1, 			3,			4,			2		},			//Tq num = 10, T_bit = 5us	
//	{CAN_BDR_250K, 		1, 			4,			5,			1		},			//Tq num = 12, T_bit = 4us
//	{CAN_BDR_500K, 		1, 			4,			5,			0		},			//Tq num = 12, T_bit = 2us
//	{0, 				0, 			0,			0,			0		},				
//};

/** \brief get can bit time config
 * 
 *  \param[in] eBaudRate: can transfer baud rate
 *  \return pointer of can bit time structure
 */ 
static csi_can_bittime_t *apt_can_get_bittime(csi_can_baudRate_e eBaudRate)
{
	csi_can_bittime_t *ptBitTm = (csi_can_bittime_t *)tBitTime;

    while(ptBitTm->wBaudRate) 
	{
        if(eBaudRate == ptBitTm->wBaudRate) 
		{
            return ptBitTm;
        }
        ptBitTm++;
    }
	
	return NULL;
}

/** \brief can receive interrupt handle function
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \return none
 */ 
__attribute__((weak)) void can_irqhandler(csp_can_t *ptCanBase)
{
	volatile uint16_t hwMcrVal = 0x00;
	volatile uint16_t hwIntNum = csp_can_get_hpir(ptCanBase);			//get interrupt pointer							
	volatile uint32_t wData[2] = {0x00 ,0x00};
	
	switch(hwIntNum)		//receive handle
	{
		case 0x00:			//end handle, 
			break;
		case 0x8000:		//status change interrupt handle
			//
			if(csp_can_get_sr(ptCanBase) & CAN_ERWARNTR_INT)		//error passive warning transition interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_ERWARNTR_INT);	
			}
			//
			if(csp_can_get_sr(ptCanBase) & CAN_ERPASSTR_INT)		//error passive transition interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_ERPASSTR_INT);
			}
			//
			if(csp_can_get_sr(ptCanBase) & CAN_BUSOFFTR_INT)		//bus off transition interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_BUSOFFTR_INT);
			}
			//
			if(csp_can_get_sr(ptCanBase) & CAN_ACTVT_INT)			//activity interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_ACTVT_INT);
			}
			//
			if(csp_can_get_sr(ptCanBase) & CAN_RXOK_INT)			//successfully received a message interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_RXOK_INT);
			}
			//
			if(csp_can_get_sr(ptCanBase) & CAN_TXOK_INT)			//successfully transmit a message interrupt.
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_TXOK_INT);
			}
			//
			if(csp_can_get_sr(ptCanBase) & CAN_STUFF_INT)			//stuff error interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_STUFF_INT);
			}
			//
			if(csp_can_get_sr(ptCanBase) & CAN_FORM_INT)			//form error interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_FORM_INT);
			}
			//
			if(csp_can_get_sr(ptCanBase) & CAN_ACK_INT)				//acknowledge error interrupt.
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_ACK_INT);
			}
			//
			if(csp_can_get_sr(ptCanBase) & CAN_BIT1_INT)			//bit to one error interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_BIT1_INT);
			}
			//
			if(csp_can_get_sr(ptCanBase) & CAN_BIT0_INT)			//bit to zero error interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_BIT0_INT);
			}
			//
			if(csp_can_get_sr(ptCanBase) & CAN_CRC_INT)				//CRC error interrupt
			{
				nop;
				csp_can_clr_isr(ptCanBase, CAN_CRC_INT);
			}
			
			break;
		default:			//message channel handle
		
			while(csp_can_get_sr(ptCanBase) & CAN_BUSY1_S);											//If1 Busy?												
			csp_can_set_tmr(ptCanBase, hwIntNum, 1, CAN_AMCR_MSK | CAN_CLRIT_MSK | CAN_TRND_MSK);	//Write If1 command request, clear NAWDATA and ITPND flag
			hwMcrVal = csp_can_get_mcr(ptCanBase);													//Read If1 message control reg, Read first and clean up NAWDATA and ITPND
			
			if(hwMcrVal & CAN_NEWDAT_MSK)		//NEWDAT flag = 1 ?
			{
				wData[0] = csi_can_get_ifx(ptCanBase, hwIntNum, CAN_IFX_DAR);			//DATA A
				wData[1] = csi_can_get_ifx(ptCanBase, hwIntNum, CAN_IFX_DBR);			//DATA B
				
				hwMcrVal = csi_can_get_ifx(ptCanBase, hwIntNum, CAN_IFX_MCR);			//MCR
				hwMcrVal = csi_can_get_ifx(ptCanBase, hwIntNum, CAN_IFX_MSKR);			//MSKR
				hwMcrVal = csi_can_get_ifx(ptCanBase, hwIntNum, CAN_IFX_IR);			//IR
				nop;

			}
			
			//csp_can_clr_isr(ptCanBase, CAN_RXOK_INT);		//clear RXOK flag
			nop;
			break;
		
	}
	
}
/** \brief initialize can work parameter structure
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \param[in] ptCanCfg: pointer of can init parameter config structure
 *  \return error code \ref csi_error_t
 */ 
static void apt_can_msg_clr(csp_can_t *ptCanBase)
{
	for(uint8_t i = 1; i < 33; i++)
	{
		csp_can_set_dar(ptCanBase, 0x00);
		csp_can_set_dbr(ptCanBase, 0x00);
		csp_can_set_mskr(ptCanBase, 0x00);
		csp_can_set_ir(ptCanBase, 0x00);
		csp_can_set_mcr(ptCanBase, 0x00);
		while(csp_can_get_sr(ptCanBase) & CAN_BUSY0_S);
		csp_can_set_tmr(ptCanBase, i, 0, CAN_WR_MSK | CAN_TMR_ALL_TRAN);
	}
}

/** \brief initialize can work parameter structure
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \param[in] ptCanCfg: pointer of can init parameter config structure
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_can_init(csp_can_t *ptCanBase, csi_can_config_t *ptCanCfg)
{
	csi_can_bittime_t *ptBitTime = NULL;
				
	//csi_clk_enable((uint32_t *)ptCanBase);	//can peripheral clk enable
	csp_can_clk_en(ptCanBase);					//clk enable
	csp_can_soft_rst(ptCanBase);				//soft reset
	

	ptBitTime = apt_can_get_bittime(ptCanCfg->wBaudRate);		//get can bit timer config
	if(NULL == ptBitTime)
		return CSI_ERROR;
	
	if(ptCanCfg->byClkSrc == CAN_CLKSRC_PCLK)					//clk source = pclk,
	{
		if((csi_get_pclk_freq() >> 3) < ptCanCfg->wBaudRate)	//canclk >= 8 * ptCanCfg->wBitRate
				return CSI_ERROR;
	}
	else
		return CSI_ERROR;										//clk source = emclk,
		
	apt_can_msg_clr(ptCanBase);
	
	csp_can_ccen(ptCanBase);
	csp_can_set_mode(ptCanBase, ptBitTime->byBdrDiv, ptCanCfg->byClkSrc, CAN_AR_DIS, ptBitTime->bySyncJw, ptBitTime->byPhSeg1, ptBitTime->byPhSeg2);	
	csp_can_ccdis(ptCanBase);
	
	//set can interrupt
	if(ptCanCfg->hwStaInter || ptCanCfg->wChnlInter)
	{
		csi_irq_enable((uint32_t *)ptCanBase);
		
		if(ptCanCfg->hwStaInter)
			csp_can_int_enable(ptCanBase, ptCanCfg->hwStaInter, ENABLE);
		if(ptCanCfg->wChnlInter)
			csp_can_sint_enable(ptCanBase, ptCanCfg->wChnlInter, ENABLE);
	}
	
	return CSI_OK;
}
/** \brief open can module(enable can)
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \return none
 */ 
void csi_can_open(csp_can_t *ptCanBase)
{
	csp_can_en(ptCanBase);
}
/** \brief close can module(disable can)
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \return none
 */
void csi_can_close(csp_can_t *ptCanBase)
{
	csp_can_dis(ptCanBase);
}
/** \brief initialize can tx parameter structure
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \param[in] eChNum: number of message
 *  \param[in] ptTxCfg: pointer of can tx config structure
 *  \return error code \ref csi_error_t
 */ 
void csi_can_tx_config(csp_can_t *ptCanBase, csi_can_chnl_e eChNum, csi_can_tx_config_t *ptTxCfg)
{
	//csp_can_get_mcr(ptCanBase);	
	csp_can_set_adata(ptCanBase, (uint8_t *)ptTxCfg->tDataA.bydata);	
	csp_can_set_bdata(ptCanBase, (uint8_t *)ptTxCfg->tDataB.bydata);	
	csp_can_ir_config(ptCanBase, ptTxCfg->tIr.wExtId, ptTxCfg->tIr.hwStdId, CAN_MDIR_SEND, ptTxCfg->tIr.byIdmode, CAN_MSGVAL_VALID);
	csp_can_set_mcr_wr(ptCanBase, ptTxCfg->tMcr.byDataLen, ptTxCfg->tMcr.bTxReqEn, ptTxCfg->tMcr.bTxIeEn, ptTxCfg->tMcr.bRmtEn);
	
	while(csp_can_get_sr(ptCanBase) & CAN_BUSY0_S);
	csp_can_set_tmr(ptCanBase, eChNum, CAN_IF0, CAN_TMR_TX_NORMAL);
}

/** \brief initialize can rx parameter structure
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \param[in] eChNum: number of message
 *  \param[in] ptRxCfg: pointer of can rx config structure
 *  \return error code \ref csi_error_t
 */ 
void csi_can_rx_config(csp_can_t *ptCanBase, csi_can_chnl_e eChNum, csi_can_rx_config_t *ptRxCfg)
{
	csp_can_ir_config(ptCanBase, ptRxCfg->tIr.wExtId, ptRxCfg->tIr.hwStdId, CAN_MDIR_RECV, ptRxCfg->tIr.byIdmode, CAN_MSGVAL_VALID);
	csp_can_mskr_config(ptCanBase, ptRxCfg->tMskr.wExtIdMsk, ptRxCfg->tMskr.hwStdIdMsk, CAN_MMDIR_DIS, ptRxCfg->tMskr.byIdMdMsk);
	csp_can_set_mcr_rd(ptCanBase, ptRxCfg->tMcr.byDataLen, ptRxCfg->tMcr.bOverWrEn, ptRxCfg->tMcr.bRxIeEn, ptRxCfg->tMcr.bMskEn);
	
	while(csp_can_get_sr(ptCanBase) & CAN_BUSY0_S);
	csp_can_set_tmr(ptCanBase, eChNum, CAN_IF0, CAN_TMR_RX_NORMAL);
}
/** \brief can msg channel send enable
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \param[in] eChNum: number of message
 *  \param[in] eTxIe: TX interrupt ITPAND SET enable
 *  \param[in] byDataLen: data length of message
 *  \return none
 */
void csi_can_chnl_send(csp_can_t *ptCanBase, csi_can_chnl_e eChNum, csi_can_mcrx_e eTxIe, uint8_t byDataLen)
{
	uint32_t wMcrVal;
	//read MCR reg
	while(csp_can_get_sr(ptCanBase) & CAN_BUSY1_S);
	csp_can_set_tmr(ptCanBase, eChNum, CAN_IF1, CAN_AMCR_MSK);
	wMcrVal = (csp_can_get_mcr(ptCanBase) & 0xd7f0);
	//write MCR reg
	while(csp_can_get_sr(ptCanBase) & CAN_BUSY0_S);
	//csp_can_set_mcr_wr(ptCanBase, byDataLen, CAN_TXREQST_DOING, eTxIe, CAN_RMTEN_SET);
	csp_can_set_mcr(ptCanBase, (wMcrVal | CAN_DLC(byDataLen) | (CAN_TXREQST_DOING << CAN_TXREQST_POS) | (eTxIe << CAN_TXIE_POS)));
	csp_can_set_tmr(ptCanBase, eChNum, CAN_IF0, CAN_DIR_WRITE | CAN_AMCR_MSK);
}
/** \brief  can transfer manage register control 
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \param[in] eChNum: number of message
 *  \param[in] eIfx: Interface register select, IF0/IF1(TX/RX)
 *  \param[in] wTmrCmd: transfer manage register command
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_can_tmr_ctrl(csp_can_t *ptCanBase, csi_can_chnl_e eChNum, csi_can_ifx_e eIfx, uint32_t wTmrCmd)
{
	
	switch(eIfx)
	{
		case CAN_IF0:
			while(csp_can_get_sr(ptCanBase) & CAN_BUSY0_S);
			break;
		case CAN_IF1:
			while(csp_can_get_sr(ptCanBase) & CAN_BUSY1_S);
			break;
		default:
			return CSI_ERROR;
	}
	
	csp_can_set_tmr(ptCanBase, eChNum, eIfx, wTmrCmd);
	return CSI_OK;
}
/** \brief updata can message data A bytes
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \param[in] eChNum: number of message
 *  \param[in] pbyData: point of data a
 *  \return none
 */ 
void csi_can_set_data_a(csp_can_t *ptCanBase, csi_can_chnl_e eChNum, uint8_t *pbyData)
{
	csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_ADAR_MSK);				//first read
	while(csp_can_get_sr(ptCanBase) & CAN_BUSY1_S);
	//csp_can_get_dar(ptCanBase);
	csp_can_set_adata(ptCanBase, pbyData);						
	while(csp_can_get_sr(ptCanBase) & CAN_BUSY0_S);
	csp_can_set_tmr(ptCanBase, eChNum, 0, CAN_WR_MSK | CAN_ADAR_MSK);	//write
}
/** \brief set can message data A bytes
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \param[in] eChNum: number of message
 *  \param[in] pbyData: point of data b
 *  \return none
 */ 
void csi_can_set_data_b(csp_can_t *ptCanBase, csi_can_chnl_e eChNum, uint8_t *pbyData)
{
	csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_ADBR_MSK);				//first read
	while(csp_can_get_sr(ptCanBase) & CAN_BUSY1_S);
	//csp_can_get_dbr(ptCanBase);
	csp_can_set_bdata(ptCanBase, pbyData);						
	while(csp_can_get_sr(ptCanBase) & CAN_BUSY0_S);
	csp_can_set_tmr(ptCanBase, eChNum, 0, CAN_WR_MSK | CAN_ADBR_MSK);	//write
}
/** \brief set can message mskr 
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \param[in] eChNum: number of message
 *  \param[in] ptMskr: point of mskr config structure
 *  \return none
 */ 
void csi_can_set_mskr(csp_can_t *ptCanBase, csi_can_chnl_e eChNum, csi_can_mskr_config_t *ptMskr)
{
	csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_AMSKR_MSK);				//first read
	while(csp_can_get_sr(ptCanBase) & CAN_BUSY1_S);
	//csp_can_get_mskr(ptCanBase);
	csp_can_mskr_config(ptCanBase, ptMskr->wExtIdMsk, ptMskr->hwStdIdMsk, CAN_DIRMSK_DIS, ptMskr->byIdMdMsk);
	while(csp_can_get_sr(ptCanBase) & CAN_BUSY0_S);
	csp_can_set_tmr(ptCanBase, eChNum, 0, CAN_WR_MSK | CAN_AMSKR_MSK);	//write
}
/** \brief set can message ir
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \param[in] eChNum: number of message
 *  \param[in] ptIr: point of ir config structure
 *  \return none
 */ 
void csi_can_set_ir(csp_can_t *ptCanBase, csi_can_chnl_e eChNum, csi_can_ir_config_t *ptIr)
{
	csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_AIR_MSK);				//first read
	while(csp_can_get_sr(ptCanBase) & CAN_BUSY1_S);
	//csp_can_get_ir(ptCanBase);
	csp_can_ir_config(ptCanBase, ptIr->wExtId, ptIr->hwStdId, ptIr->byMsgDir, ptIr->byIdmode, CAN_MSGVAL_VALID);
	while(csp_can_get_sr(ptCanBase) & CAN_BUSY0_S);
	csp_can_set_tmr(ptCanBase, eChNum, 0, CAN_WR_MSK | CAN_AIR_MSK);//write
}
/** \brief set can message tx mcr
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \param[in] eChNum: number of message
 *  \param[in] ptMcr: point of tx mcr config structure
 *  \return none
 */ 
void csi_can_set_tx_mcr(csp_can_t *ptCanBase, csi_can_chnl_e eChNum, csi_can_tx_mcr_config_t *ptMcr)
{
	csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_AMCR_MSK);				//first read
	while(csp_can_get_sr(ptCanBase) & CAN_BUSY1_S);
	//csp_can_get_mcr(ptCanBase);
	csp_can_set_mcr_wr(ptCanBase, ptMcr->byDataLen, ptMcr->bTxReqEn, ptMcr->bTxIeEn, ptMcr->bRmtEn);
	while(csp_can_get_sr(ptCanBase) & CAN_BUSY0_S);
	csp_can_set_tmr(ptCanBase, eChNum, 0, CAN_WR_MSK | CAN_AMCR_MSK);	//write
}
/** \brief set can message rx mcr
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \param[in] eChNum: number of message
 *  \param[in] ptMcr: point of rx mcr config structure
 *  \return none
 */ 
void csi_can_set_rx_mcr(csp_can_t *ptCanBase, csi_can_chnl_e eChNum, csi_can_rx_mcr_config_t *ptMcr)
{
	csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_AMCR_MSK);				//first read
	while(csp_can_get_sr(ptCanBase) & CAN_BUSY1_S);
	//csp_can_get_ir(ptCanBase);
	csp_can_set_mcr_rd(ptCanBase, ptMcr->byDataLen, ptMcr->bOverWrEn, ptMcr->bRxIeEn, ptMcr->bMskEn);
	while(csp_can_get_sr(ptCanBase) & CAN_BUSY0_S);
	csp_can_set_tmr(ptCanBase, eChNum, 0, CAN_WR_MSK | CAN_AMCR_MSK);	//write
}
/** \brief set can message valid 
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \param[in] eChNum: number of message
 *  \param[in] bEnbale: ENABLE/DISABLE(valid/invalid)
 *  \return none
 */ 
void csi_can_set_msg_valid(csp_can_t *ptCanBase, csi_can_chnl_e eChNum, bool bEnbale)
{
	uint32_t wIrVal;
	csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_AIR_MSK);					//first read
	while(csp_can_get_sr(ptCanBase) & CAN_BUSY1_S);
	wIrVal = csp_can_get_ir(ptCanBase) ;
	
	csp_can_set_ir(ptCanBase, ((wIrVal & ~CAN_MSGVAL_MSK) | (bEnbale << CAN_MSGVAL_POS)));
	while(csp_can_get_sr(ptCanBase) & CAN_BUSY0_S);
	csp_can_set_tmr(ptCanBase, eChNum, 0, CAN_WR_MSK | CAN_AIR_MSK);	//write
}
/** \brief can message txrqst enable
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \param[in] eChNum: number of message
 *  \return none
 */ 
void csi_can_txrqst_enable(csp_can_t *ptCanBase, csi_can_chnl_e eChNum)
{
	uint32_t wMcrVal;
	csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_AMCR_MSK);			//first read
	while(csp_can_get_sr(ptCanBase) & CAN_BUSY1_S);
	wMcrVal = csp_can_get_mcr(ptCanBase);
	
	csp_can_set_mcr(ptCanBase, (wMcrVal | CAN_TXREQST_MSK));
	while(csp_can_get_sr(ptCanBase) & CAN_BUSY0_S);
	csp_can_set_tmr(ptCanBase, eChNum, 0, CAN_WR_MSK | CAN_AMCR_MSK);//write
}
/** \brief  can set interface corresponding register value
 * 
 *  \param[in] ptCanBase: pointer of can register structure\
 *  \param[in] eChNum: number of message
 *  \param[in] eIfxReg: interface corresponding register(DAR/DBR/MSKR/IR/MCR)
 *  \return value of interface register or err value
 */
uint32_t csi_can_get_ifx(csp_can_t *ptCanBase, csi_can_chnl_e eChNum, csi_can_ifx_reg_e eIfxReg)
{
	while(csp_can_get_sr(ptCanBase) & CAN_BUSY1_S);				//wait if1 idle
	switch(eIfxReg)
	{
		case CAN_IFX_DAR:			//data A
			csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_ADAR_MSK);
			while(csp_can_get_sr(ptCanBase) & CAN_BUSY1_S);
			return csp_can_get_dar(ptCanBase);
		case CAN_IFX_DBR:			//data B
			csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_ADBR_MSK);
			while(csp_can_get_sr(ptCanBase) & CAN_BUSY1_S);
			return csp_can_get_dbr(ptCanBase);
		case CAN_IFX_MSKR:			//mask
			csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_AMSKR_MSK);
			while(csp_can_get_sr(ptCanBase) & CAN_BUSY1_S);
			return csp_can_get_mskr(ptCanBase);
		case CAN_IFX_IR:			//ir
			csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_AIR_MSK);
			while(csp_can_get_sr(ptCanBase) & CAN_BUSY1_S);
			return csp_can_get_ir(ptCanBase);
		case CAN_IFX_MCR:			//mask
			csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_AMCR_MSK);
			while(csp_can_get_sr(ptCanBase) & CAN_BUSY1_S);
			return csp_can_get_mcr(ptCanBase);
		case CAN_IFX_STRP:			//strp
			csp_can_set_tmr(ptCanBase, eChNum, 1, 0);
			while(csp_can_get_sr(ptCanBase) & CAN_BUSY1_S);
			return csp_can_get_stpr(ptCanBase);			
			break;
		default:
			return 0xffffffff;
	}
}
/** \brief  can set interface corresponding register value
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \param[in] eIfxReg: interface corresponding register(DAR/DBR/MSKR/IR/MCR)
 *  \param[in] eChNum: number of message
 *  \param[in] wValue: value of write ifx register
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_can_set_ifx(csp_can_t *ptCanBase, csi_can_chnl_e eChNum, csi_can_ifx_reg_e eIfxReg, uint32_t wValue)
{
	switch(eIfxReg)
	{
		case CAN_IFX_DAR:			//data A
			csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_ADAR_MSK);			//first read
			while(csp_can_get_sr(ptCanBase) & CAN_BUSY1_S);
			//
			csp_can_set_dar(ptCanBase, wValue);
			while(csp_can_get_sr(ptCanBase) & (CAN_BUSY0_S | CAN_BUSY1_S));
			csp_can_set_tmr(ptCanBase, eChNum, 0, CAN_WR_MSK | CAN_ADAR_MSK);
			break; 
		case CAN_IFX_DBR:			//data B
			csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_ADBR_MSK);			//first read
			while(csp_can_get_sr(ptCanBase) & CAN_BUSY1_S);
			//
			csp_can_set_dbr(ptCanBase, wValue);
			while(csp_can_get_sr(ptCanBase) & (CAN_BUSY0_S | CAN_BUSY1_S));
			csp_can_set_tmr(ptCanBase, eChNum, 0, CAN_WR_MSK | CAN_ADBR_MSK);
			break;
		case CAN_IFX_MSKR:			//mask
			csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_AMSKR_MSK);			//first read
			while(csp_can_get_sr(ptCanBase) & CAN_BUSY1_S);
			//
			csp_can_set_mskr(ptCanBase, wValue);
			while(csp_can_get_sr(ptCanBase) & (CAN_BUSY0_S | CAN_BUSY1_S));
			csp_can_set_tmr(ptCanBase, eChNum, 0, CAN_WR_MSK | CAN_AMSKR_MSK);
			break;
		case CAN_IFX_IR:			//ir
			csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_AIR_MSK);				//first read
			while(csp_can_get_sr(ptCanBase) & CAN_BUSY1_S);
			//
			csp_can_set_ir(ptCanBase, wValue);
			while(csp_can_get_sr(ptCanBase) & (CAN_BUSY0_S | CAN_BUSY1_S));
			csp_can_set_tmr(ptCanBase, eChNum, 0, CAN_WR_MSK | CAN_AIR_MSK);
			break;
		case CAN_IFX_MCR:			//mcr
			csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_AMCR_MSK);			//first read
			while(csp_can_get_sr(ptCanBase) & CAN_BUSY1_S);
			//
			csp_can_set_mcr(ptCanBase, wValue);
			while(csp_can_get_sr(ptCanBase) & (CAN_BUSY0_S | CAN_BUSY1_S));
			csp_can_set_tmr(ptCanBase, eChNum, 0, CAN_WR_MSK | CAN_AMCR_MSK);
			break;
		default:
			return CSI_ERROR;
	}
	
	return CSI_OK;
}
/** \brief  get receive message NEWDAT and clear NEWDAT and ITPND 
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \param[in] eChNum: number of message
 *  \return error code \ref csi_error_t
 */
uint32_t csi_can_get_clr_recv_flg(csp_can_t *ptCanBase, csi_can_chnl_e eChNum)
{
	while(csp_can_get_sr(ptCanBase) & CAN_BUSY1_S);											//If1 Busy?												
	csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_AMCR_MSK | CAN_CLRIT_MSK | CAN_TRND_MSK);		//Write If1 command request, clear NAWDATA and ITPND flag
	return csp_can_get_mcr(ptCanBase);	
}

