/***********************************************************************//** 
 * \file  can.c
 * \brief  CAN description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-7-09 <td>V0.0  <td>ZJY   <td>initial
 * <tr><td> 2021-9-21 <td>V0.1  <td>ZJY   <td>code normalization
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
extern 	csi_can_bittime_t  g_tBitTime[];

/* Private variablesr------------------------------------------------------*/
csi_can_trans_t 	g_tCanTran;	

static uint32_t s_wCanStaMsg	= 0;			//Status Interrupt Msg
static uint32_t s_wCanRecvMsg	= 0;			//Receive Source Channel Interrupt Msg
static uint32_t s_wCanSendMsg	= 0;			//Receive Source Channel Interrupt Msg


/** \brief get can bit time config
 * 
 *  \param[in] eBaudRate: can transfer baud rate
 *  \return pointer of can bit time structure
 */ 
static csi_can_bittime_t *apt_can_get_bittime(csi_can_baudRate_e eBaudRate)
{
	csi_can_bittime_t *ptBitTm = (csi_can_bittime_t *)g_tBitTime;

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
		while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY0);
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
				
	csi_clk_enable((uint32_t *)ptCanBase);		//can peripheral clk enable
	csp_can_clk_enable(ptCanBase);				//clk enable
	csp_can_sw_rst(ptCanBase);					//soft reset
	csp_can_disable(ptCanBase);					//can dis

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
	
	csp_can_cc_enable(ptCanBase);								//ccen
	csp_can_set_mode(ptCanBase, ptBitTime->byBdrDiv, ptCanCfg->byClkSrc, ptCanCfg->bAuReTran, ptBitTime->bySyncJw, ptBitTime->byPhSeg1, ptBitTime->byPhSeg2);	
	csp_can_cc_disable(ptCanBase);								//ccdis
	
	//set can interrupt
	if(ptCanCfg->hwStaInter || ptCanCfg->wChnlInter)
	{
		csi_irq_enable(ptCanBase);
		
		if(ptCanCfg->hwStaInter)
			csp_can_int_enable(ptCanBase, ptCanCfg->hwStaInter);
		if(ptCanCfg->wChnlInter)
			csp_can_ch_int_enable(ptCanBase, ptCanCfg->wChnlInter);
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
	csp_can_enable(ptCanBase);
}
/** \brief close can module(disable can)
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \return none
 */
void csi_can_close(csp_can_t *ptCanBase)
{
	csp_can_disable(ptCanBase);
}
/** \brief config can tx channel parameter structure
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \param[in] eChNum: number of message
 *  \param[in] ptTxCfg: pointer of can tx config structure
 *  \return error code \ref csi_error_t
 */ 
void csi_can_set_tx_ch(csp_can_t *ptCanBase, csi_can_ch_e eChNum, csi_can_tx_config_t *ptTxCfg)
{
	//csp_can_get_mcr(ptCanBase);	
	csp_can_set_adata(ptCanBase, (uint8_t *)ptTxCfg->tDataA.bydata);	
	csp_can_set_bdata(ptCanBase, (uint8_t *)ptTxCfg->tDataB.bydata);	
	csp_can_set_id_mode(ptCanBase, ptTxCfg->tId.wExtId, ptTxCfg->tId.hwStdId, CAN_MDIR_SEND, (can_xtd_e)ptTxCfg->tId.eIdMode, CAN_MSGVAL_VALID);
	csp_can_set_mcr_wr(ptCanBase, ptTxCfg->tMc.byDataLen, ptTxCfg->tMc.bTxReqEn, ptTxCfg->tMc.bTxIeEn, ptTxCfg->tMc.bRmtEn);
	
	while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY0);
	csp_can_set_tmr(ptCanBase, eChNum, CAN_IF0, CAN_TMR_TX_NORMAL);
}

/** \brief config can tx channel parameter structure
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \param[in] eChNum: number of message
 *  \param[in] ptRxCfg: pointer of can rx config structure
 *  \return error code \ref csi_error_t
 */ 
void csi_can_set_rx_ch(csp_can_t *ptCanBase, csi_can_ch_e eChNum, csi_can_rx_config_t *ptRxCfg)
{
	csp_can_set_id_mode(ptCanBase, ptRxCfg->tId.wExtId, ptRxCfg->tId.hwStdId, CAN_MDIR_RECV, (can_xtd_e)ptRxCfg->tId.eIdMode, CAN_MSGVAL_VALID);
	csp_can_set_msk_mode(ptCanBase, ptRxCfg->tMsk.wExtIdMsk, ptRxCfg->tMsk.hwStdIdMsk, CAN_MMDIR_DIS, ptRxCfg->tMsk.byIdMdMsk);
	csp_can_set_mcr_rd(ptCanBase, ptRxCfg->tMc.byDataLen, ptRxCfg->tMc.bOverWrEn, ptRxCfg->tMc.bRxIeEn, ptRxCfg->tMc.bMskEn);
	
	while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY0);
	csp_can_set_tmr(ptCanBase, eChNum, CAN_IF0, CAN_TMR_RX_NORMAL);
}

/** \brief initialize can rx parameter structure
 * 
 *  \param[in] ptCanRecv: pointer of can receive structure
 *  \param[in] eStrChNum: start channel of receive message
 *  \param[in] byChTolNum: total number of channel
 *  \return none
 */ 
void csi_can_recv_init(csi_can_recv_t *ptCanRecv, csi_can_ch_e eStrChNum, uint8_t byChTolNum)
{
	g_tCanTran.ptCanRecv = ptCanRecv;
	g_tCanTran.byStrChnl = eStrChNum;
	g_tCanTran.byChTolNum = byChTolNum;
}
/** \brief can interrupt(message status) enable
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \param[in] eStaIntSrc: can status interrupt source
 *  \return none
 */
void csi_can_int_enable(csp_can_t *ptCanBase, csi_can_intsrc_e eStaIntSrc)
{
	csp_can_int_enable(ptCanBase, (can_int_e)eStaIntSrc);
}
/** \brief can interrupt(message status) disable
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \param[in] eStaIntSrc: can status interrupt source
 *  \return none
 */
void csi_can_int_disable(csp_can_t *ptCanBase, csi_can_intsrc_e eStaIntSrc)
{
	csp_can_int_disable(ptCanBase, (can_int_e)eStaIntSrc);
}
/** \brief can message channel interrupt enable
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \param[in] eChNum: number of message
 *  \return none
 */
void csi_can_ch_int_enable(csp_can_t *ptCanBase, csi_can_ch_e eChNum)
{
	csp_can_ch_int_enable(ptCanBase, (0x01ul << (eChNum -1)));
}
/** \brief can message channel interrupt enable
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \param[in] eChNum: number of message
 *  \return none
 */
void csi_can_ch_int_disable(csp_can_t *ptCanBase, csi_can_ch_e eChNum)
{
	csp_can_ch_int_disable(ptCanBase, (0x01ul << (eChNum -1)));
}
/** \brief can message channel send 
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \param[in] eChNum: number of message channel
 *  \param[in] eTxIe: TX interrupt ITPAND SET enable
 *  \param[in] byDataLen: data length of message
 *  \return none
 */
void csi_can_ch_send(csp_can_t *ptCanBase, csi_can_ch_e eChNum, uint8_t byDataLen)
{
	uint32_t wMcrVal;
	//read MCR reg
	//while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY1);
	csp_can_set_tmr(ptCanBase, eChNum, CAN_IF1, CAN_AMCR_MSK);
	while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY1);
	wMcrVal = (csp_can_get_mcr(ptCanBase) & 0xfe80);
	//write MCR reg
	csp_can_set_mcr(ptCanBase, (wMcrVal | CAN_DLC(byDataLen) | (CAN_TXREQST_EN << CAN_TXREQST_POS)));
	while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY0);
	csp_can_set_tmr(ptCanBase, eChNum, CAN_IF0, CAN_DIR_WRITE | CAN_AMCR_MSK);
}

/** \brief can read message channel send enable
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \param[in] pRecvBuf: pointer of  receive buffer
 *  \param[in] eChNum: number of message channel
 *  \return message length
 */
uint8_t csi_can_ch_read(csp_can_t *ptCanBase, uint8_t *pRecvBuf, csi_can_ch_e eChNum)
{
	uint8_t byBufNum = eChNum - g_tCanTran.byStrChnl;						//buf number
	uint8_t byMsgLen = 4 + g_tCanTran.ptCanRecv[byBufNum].byDataLen;	
	uint8_t byRet = 0;
	
	csi_irq_disable(ptCanBase);
	if(g_tCanTran.ptCanRecv[byBufNum].wRecvId != 0)		
	{
		
		pRecvBuf[0] = g_tCanTran.ptCanRecv[byBufNum].byDataLen;
		memcpy((pRecvBuf+1), (uint8_t *)&g_tCanTran.ptCanRecv[byBufNum].wRecvId, byMsgLen);
		g_tCanTran.ptCanRecv[byBufNum].wRecvId = 0;							//clear id
		s_wCanRecvMsg &= ~(0x01ul << (eChNum - 1));							//clear receive channel msg
		
		byRet =  g_tCanTran.ptCanRecv[byBufNum].byDataLen;
	}
	csi_irq_enable(ptCanBase);
	
	return byRet;
}

/** \brief  can transfer manage register operate 
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \param[in] eChNum: number of message
 *  \param[in] eIfx: Interface register select, IF0/IF1(TX/RX)
 *  \param[in] wTmrCmd: transfer manage register command
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_can_set_tmr(csp_can_t *ptCanBase, csi_can_ch_e eChNum, csi_can_ifx_e eIfx, uint32_t wTmrCmd)
{
	
	switch(eIfx)
	{
		case CAN_IF0:
			while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY0);
			break;
		case CAN_IF1:
			while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY1);
			break;
		default:
			return CSI_ERROR;
	}
	
	csp_can_set_tmr(ptCanBase, eChNum, eIfx, wTmrCmd);
	return CSI_OK;
}
/** \brief updata can message data A bytes(0~3bytes)
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \param[in] eChNum: number of message
 *  \param[in] pbyData: point of data a
 *  \return none
 */ 
void csi_can_set_data_byte0_3(csp_can_t *ptCanBase, csi_can_ch_e eChNum, uint8_t *pbyData)
{
	csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_ADAR_MSK);				//first read
	while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY1);
	//csp_can_get_dar(ptCanBase);
	csp_can_set_adata(ptCanBase, pbyData);						
	while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY0);
	csp_can_set_tmr(ptCanBase, eChNum, 0, CAN_WR_MSK | CAN_ADAR_MSK);	//write
}
/** \brief set can message data B bytes((4~7bytes))
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \param[in] eChNum: number of message
 *  \param[in] pbyData: point of data b
 *  \return none
 */ 
void csi_can_set_data_byte4_7(csp_can_t *ptCanBase, csi_can_ch_e eChNum, uint8_t *pbyData)
{
	csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_ADBR_MSK);				//first read
	while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY1);
	//csp_can_get_dbr(ptCanBase);
	csp_can_set_bdata(ptCanBase, pbyData);						
	while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY0);
	csp_can_set_tmr(ptCanBase, eChNum, 0, CAN_WR_MSK | CAN_ADBR_MSK);	//write
}
/** \brief set can message msk 
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \param[in] eChNum: number of message
 *  \param[in] ptMsk: point of mskr config structure
 *  \return none
 */ 
void csi_can_set_msk(csp_can_t *ptCanBase, csi_can_ch_e eChNum, csi_can_msk_config_t *ptMsk)
{
	csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_AMSKR_MSK);				//first read
	while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY1);
	//csp_can_get_mskr(ptCanBase);
	csp_can_set_msk_mode(ptCanBase, ptMsk->wExtIdMsk, ptMsk->hwStdIdMsk, CAN_MMDIR_DIS, ptMsk->byIdMdMsk);
	while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY0);
	csp_can_set_tmr(ptCanBase, eChNum, 0, CAN_WR_MSK | CAN_AMSKR_MSK);	//write
}
/** \brief set can message identifier
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \param[in] eChNum: number of message
 *  \param[in] ptId: point of ir config structure
 *  \return none
 */ 
void csi_can_set_id(csp_can_t *ptCanBase, csi_can_ch_e eChNum, csi_can_id_config_t *ptId)
{
	csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_AIR_MSK);				//first read
	while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY1);
	//csp_can_get_ir(ptCanBase);
	csp_can_set_id_mode(ptCanBase, ptId->wExtId, ptId->hwStdId, (can_mdir_e)ptId->eMsgDir, (can_xtd_e)ptId->eIdMode, CAN_MSGVAL_VALID);
	while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY0);
	csp_can_set_tmr(ptCanBase, eChNum, 0, CAN_WR_MSK | CAN_AIR_MSK);//write
}
/** \brief set can message tx mcr
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \param[in] eChNum: number of message
 *  \param[in] ptMcr: point of tx mcr config structure
 *  \return none
 */ 
void csi_can_set_tx_mc(csp_can_t *ptCanBase, csi_can_ch_e eChNum, csi_can_tx_mc_config_t *ptMcr)
{
	csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_AMCR_MSK);				//first read
	while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY1);
	//csp_can_get_mcr(ptCanBase);
	csp_can_set_mcr_wr(ptCanBase, ptMcr->byDataLen, ptMcr->bTxReqEn, ptMcr->bTxIeEn, ptMcr->bRmtEn);
	while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY0);
	csp_can_set_tmr(ptCanBase, eChNum, 0, CAN_WR_MSK | CAN_AMCR_MSK);	//write
}
/** \brief set can message contorl for rx
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \param[in] eChNum: number of message
 *  \param[in] ptMcr: point of rx mcr config structure
 *  \return none
 */ 
void csi_can_set_rx_mc(csp_can_t *ptCanBase, csi_can_ch_e eChNum, csi_can_rx_mc_config_t *ptMcr)
{
	csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_AMCR_MSK);				//first read
	while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY1);
	//csp_can_get_ir(ptCanBase);
	csp_can_set_mcr_rd(ptCanBase, ptMcr->byDataLen, ptMcr->bOverWrEn, ptMcr->bRxIeEn, ptMcr->bMskEn);
	while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY0);
	csp_can_set_tmr(ptCanBase, eChNum, 0, CAN_WR_MSK | CAN_AMCR_MSK);	//write
}
/** \brief set can message valid 
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \param[in] eChNum: number of message
 *  \param[in] bEnbale: ENABLE/DISABLE(valid/invalid)
 *  \return none
 */ 
void csi_can_msg_valid_enable(csp_can_t *ptCanBase, csi_can_ch_e eChNum, bool bEnbale)
{
	uint32_t wIrVal;
	csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_AIR_MSK);					//first read
	while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY1);
	wIrVal = csp_can_get_ir(ptCanBase) ;
	
	csp_can_set_ir(ptCanBase, ((wIrVal & ~CAN_MSGVAL_MSK) | (bEnbale << CAN_MSGVAL_POS)));
	while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY0);
	csp_can_set_tmr(ptCanBase, eChNum, 0, CAN_WR_MSK | CAN_AIR_MSK);	//write
}
/** \brief can message txrqst enable
 * 
 *  \param[in] ptCanBase: pointer of can register structure
 *  \param[in] eChNum: number of message
 *  \return none
 */ 
void csi_can_txrqst_enable(csp_can_t *ptCanBase, csi_can_ch_e eChNum)
{
	uint32_t wMcrVal;
	csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_AMCR_MSK);			//first read
	while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY1);
	wMcrVal = csp_can_get_mcr(ptCanBase);
	
	csp_can_set_mcr(ptCanBase, (wMcrVal | CAN_TXREQST_MSK));
	while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY0);
	csp_can_set_tmr(ptCanBase, eChNum, 0, CAN_WR_MSK | CAN_AMCR_MSK);//write
}
/** \brief  can set interface corresponding register value
 * 
 *  \param[in] ptCanBase: pointer of can register structure\
 *  \param[in] eChNum: number of message
 *  \param[in] eIfxReg: interface corresponding register(DAR/DBR/MSKR/IR/MCR)
 *  \return value of interface register or err value
 */
uint32_t csi_can_get_ifx(csp_can_t *ptCanBase, csi_can_ch_e eChNum, csi_can_ifx_reg_e eIfxReg)
{
	while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY1);				//wait if1 idle
	switch(eIfxReg)
	{
		case CAN_IFX_DAR:			//data A
			csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_ADAR_MSK);
			while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY1);
			return csp_can_get_dar(ptCanBase);
		case CAN_IFX_DBR:			//data B
			csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_ADBR_MSK);
			while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY1);
			return csp_can_get_dbr(ptCanBase);
		case CAN_IFX_MSKR:			//mask
			csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_AMSKR_MSK);
			while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY1);
			return csp_can_get_mskr(ptCanBase);
		case CAN_IFX_IR:			//ir
			csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_AIR_MSK);
			while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY1);
			return csp_can_get_ir(ptCanBase);
		case CAN_IFX_MCR:			//mcr
			csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_AMCR_MSK);
			while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY1);
			nop;nop;
			return csp_can_get_mcr(ptCanBase);
		case CAN_IFX_STRP:			//strp
			csp_can_set_tmr(ptCanBase, eChNum, 1, 0);
			while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY1);
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
csi_error_t csi_can_set_ifx(csp_can_t *ptCanBase, csi_can_ch_e eChNum, csi_can_ifx_reg_e eIfxReg, uint32_t wValue)
{
	switch(eIfxReg)
	{
		case CAN_IFX_DAR:			//data A
			csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_ADAR_MSK);			//first read
			while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY1);
			//
			csp_can_set_dar(ptCanBase, wValue);
			while(csp_can_get_sr(ptCanBase) & (CAN_STA_BUSY0 | CAN_STA_BUSY1));
			csp_can_set_tmr(ptCanBase, eChNum, 0, CAN_WR_MSK | CAN_ADAR_MSK);
			break; 
		case CAN_IFX_DBR:			//data B
			csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_ADBR_MSK);			//first read
			while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY1);
			//
			csp_can_set_dbr(ptCanBase, wValue);
			while(csp_can_get_sr(ptCanBase) & (CAN_STA_BUSY0 | CAN_STA_BUSY1));
			csp_can_set_tmr(ptCanBase, eChNum, 0, CAN_WR_MSK | CAN_ADBR_MSK);
			break;
		case CAN_IFX_MSKR:			//mask
			csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_AMSKR_MSK);			//first read
			while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY1);
			//
			csp_can_set_mskr(ptCanBase, wValue);
			while(csp_can_get_sr(ptCanBase) & (CAN_STA_BUSY0 | CAN_STA_BUSY1));
			csp_can_set_tmr(ptCanBase, eChNum, 0, CAN_WR_MSK | CAN_AMSKR_MSK);
			break;
		case CAN_IFX_IR:			//ir
			csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_AIR_MSK);				//first read
			while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY1);
			//
			csp_can_set_ir(ptCanBase, wValue);
			while(csp_can_get_sr(ptCanBase) & (CAN_STA_BUSY0 | CAN_STA_BUSY1));
			csp_can_set_tmr(ptCanBase, eChNum, 0, CAN_WR_MSK | CAN_AIR_MSK);
			break;
		case CAN_IFX_MCR:			//mcr
			csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_AMCR_MSK);			//first read
			while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY1);
			//
			csp_can_set_mcr(ptCanBase, wValue);
			while(csp_can_get_sr(ptCanBase) & (CAN_STA_BUSY0 | CAN_STA_BUSY1));
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
 *  \param[in] eChNum: number of message channel
 *  \return error code \ref csi_error_t
 */
uint32_t csi_can_get_clr_recvflg(csp_can_t *ptCanBase, csi_can_ch_e eChNum)
{
	csp_can_set_tmr(ptCanBase, eChNum, 1, CAN_AMCR_MSK | CAN_CLRIT_MSK | CAN_TRND_MSK);			//Write If1 command request, clear NAWDATA and ITPND flag
	while(csp_can_get_sr(ptCanBase) & CAN_STA_BUSY1);											//If1 Busy?												
	return csp_can_get_mcr(ptCanBase);	
}

/** \brief can post(set) Receive/Send and Status msg 
 * 
 *  \param[in] eMsgMode: can msg mode, \ref csi_can_msg_mode_e
 *  \param[in] wMsg: status and channel msg
 *  \return true/false
 */ 
bool csi_can_post_msg(csi_can_msg_mode_e eMsgMode, uint32_t wMsg)
{
	switch(eMsgMode)
	{
		case CAN_MSG_STATUS:
			s_wCanStaMsg |= wMsg;			//Status Msg
			break;
		case CAN_MSG_RECV:
			s_wCanRecvMsg |= wMsg;			//Receive Source Msg
			break;
		case CAN_MSG_SEND:
			s_wCanSendMsg |= wMsg;			//Send Source Msg
			break;
		default:
			return false;
	}
	return true;
}

/** \brief  get msg of receive/send channel receive/send message
 *
 *  \param[in] eMsgMode: can msg mode(recv and send chnl), \ref csi_can_msg_mode_e 
 *  \param[in] eChNum: number of channel 
 *  \return true/false
 */
bool csi_can_get_msg(csi_can_msg_mode_e eMsgMode, csi_can_ch_e eChNum)
{
	uint32_t wStatus = 0;
	
	switch(eMsgMode)
	{
		case CAN_MSG_RECV:						//Receive Channel
			wStatus = s_wCanRecvMsg;
			break;
		case CAN_MSG_SEND:						//Send Channel
			wStatus = s_wCanSendMsg;
			break;
		default:
			return false;
	}
		
	if(wStatus & (0x01ul << (eChNum -1)))
		return true;
	else
		 return false;
}

/** \brief  clr msg of can receive/send message channel 
 * 
 *  \param[in] eMsgMode: can msg mode(recv and send chnl), \ref csi_can_msg_mode_e
 *  \param[in] eChNum: number of channel 
 *  \return none
 */
void csi_can_clr_msg(csi_can_msg_mode_e eMsgMode, csi_can_ch_e eChNum)
{
	
	switch(eMsgMode)
	{
		case CAN_MSG_RECV:								//Receive Channel
			s_wCanRecvMsg &= ~(0x01ul << (eChNum -1));
			break;
		case CAN_MSG_SEND:								//Send Channel
			s_wCanSendMsg &= ~(0x01ul << (eChNum -1));
			break;
		default:
			break;
	}
}

/** \brief  get msg of receive channel message
 * 
 *  \param[in] none
 *  \return message channel mask
 */
uint32_t csi_can_get_recv_msg(void)
{
	return s_wCanRecvMsg;
}

/** \brief  clr msg of can status msg
 * 
 *  \param[in] none
 *  \return message status mask
 */
uint32_t csi_can_get_status_msg(void)
{
	return s_wCanStaMsg;
}

/** \brief  clr msg of can status msg
 * 
 *  \param[in] eStaMsgï¼status message,\ref csi_can_status_msg_e
 *  \return none
 */
void csi_can_clr_status_msg(csi_can_status_msg_e eStaMsg)
{
	s_wCanStaMsg &= ~eStaMsg;
}
