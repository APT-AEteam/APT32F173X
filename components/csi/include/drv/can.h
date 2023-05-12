
/***********************************************************************//** 
 * \file  can.h
 * \brief  csi can driver head file
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-12 <td>V0.0  <td>ZJY   <td>initial
 * <tr><td> 2021-1-8  <td>V0.1  <td>WNN   <td>modify
 * </table>
 * *********************************************************************
*/
#ifndef _DRV_CAN_H_
#define _DRV_CAN_H_

#include <stdint.h>
#include <stdbool.h>
#include <drv/common.h>

#include "csp.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \enum	csi_can_clksrc_e
 * \brief   can clk source
 */
typedef enum
{
	CAN_CLKSRC_PCLK	= 0,		//clk source = pclk
	CAN_CLKSRC_EMCLK				//clk source = emclk    
}csi_can_clksrc_e;

/**
 * \enum	csi_can_clksrc_e
 * \brief   can clk source
 */
typedef enum
{
	CAN_IF0			= 0,	//CAN Interface 0, send reg
	CAN_IF1					//CAN Interface 1, receive reg
}csi_can_ifx_e;

/**
 * \enum	csi_can_dir_e
 * \brief   can msg dir send/receive
 */
typedef enum
{
	CAN_DIR_RECV	= 0,	//CAN message dir receive
	CAN_DIR_SEND			//CAN message dir send
}csi_can_dir_e;

/**
 * \enum	csi_can_id_e
 * \brief   can id mode std/exi
 */
typedef enum
{
	CAN_STD_ID	= 0,		//CAN standard ID mode
	CAN_EXT_ID				//CAN extend ID mode
}csi_can_id_e;

/**
 * \enum	csi_can_msg_e
 * \brief   can message valid/invalid
 */
typedef enum
{
	CAN_MSG_INVALID	= 0,	//CAN msg invalid
	CAN_MSG_VALID			//CAN msg valid
}csi_can_msg_e;

/**
 * \enum	csi_can_dir_msk_e
 * \brief   can msg dir mask
 */
typedef enum
{
	CAN_DIR_MSK_DIS	= 0,	//CAN message dir receive
	CAN_DIR_MSK_EN			//CAN message dir send
}csi_can_dirmsk_e;

/**
 * \enum	csi_can_id_msk_e
 * \brief   can msg id mode mask
 */
typedef enum
{
	CAN_STD_ID_MSK	= 0,	//CAN message standard ID mask
	CAN_EXT_ID_MSK			//CAN message extend ID mask 
}csi_can_idmsk_e;

/**
 * \enum	csi_can_clksrc_e
 * \brief   can clk source
 */
typedef enum
{
	CAN_BDR_5K		= 5000,		//baud rate = 5000 = 5k
	CAN_BDR_10K		= 10000,	//baud rate = 10000 =10k  
	CAN_BDR_20K		= 20000,
	CAN_BDR_40K		= 40000,
	CAN_BDR_50K		= 50000,
	CAN_BDR_80K		= 80000,
	CAN_BDR_100K	= 100000,
	CAN_BDR_125K	= 125000,
	CAN_BDR_200K	= 200000,
	CAN_BDR_250K	= 250000,
	CAN_BDR_400K	= 400000,
	CAN_BDR_500K	= 500000,
	CAN_BDR_660K	= 600000,
	CAN_BDR_750K	= 750000,
	CAN_BDR_800K	= 800000,
	CAN_BDR_1000K	= 1000000,
}csi_can_baudRate_e;

/**
 * \enum	csi_can_chnl_e
 * \brief   can cource channel
 */
typedef enum
{
	CAN_CH1			= 1,		//CAN Channel; 1~32
	CAN_CH2,	
	CAN_CH3,
	CAN_CH4,	
	CAN_CH5,	
	CAN_CH6,	
	CAN_CH7,
	CAN_CH8,	
	CAN_CH9,	
	CAN_CH10,	
	CAN_CH11,	
	CAN_CH12,	
	CAN_CH13,
	CAN_CH14,	
	CAN_CH15,	
	CAN_CH16,	
	CAN_CH17,	
	CAN_CH18,	
	CAN_CH19,
	CAN_CH20,	
	CAN_CH21,
	CAN_CH22,
	CAN_CH23,	
	CAN_CH24,	
	CAN_CH25,	
	CAN_CH26,	
	CAN_CH27,	
	CAN_CH28,
	CAN_CH29,	
	CAN_CH30,
	CAN_CH31,	
	CAN_CH32
}csi_can_chnl_e;

#define	CAN_MSG_CH_POS(ch)	(ch-1)					
#define	CAN_MSG_CH(ch)		(0x01ul << (ch-1))

/**
 * \enum	csi_can_tmr_e
 * \brief   can trensfer manage
 */
typedef enum
{
	CAN_DIR_READ		= (0x00ul << 7),			//read
	CAN_DIR_WRITE		= (0x01ul << 7),			//write		
	CAN_ADATA_TRANS		= (0x01ul << 8),
	CAN_BDATA_TRANS		= (0x01ul << 9),
	CAN_MASK_TRANS		= (0x01ul << 10),
	CAN_IR_TRANS		= (0x01ul << 11),
	CAN_MC_TRANS		= (0x01ul << 12),	
	CAN_TRN_SET_CLR		= (0x01ul << 14),		
	CAN_ITPAND_CLR		= (0x01ul << 15),	
}csi_can_tmrx_cmd_e;

/**
 * \enum	csp_can_mcr_e
 * \brief   can message control
 */
//typedef enum
//{
//	CAN_OVWRITE_DIS		= 0,
//	CAN_OVWRITE_EN		= 1,
//	CAN_TXREQ_DIS		= 0,
//	CAN_TXREQ_EN		= 1,
//	CAN_RMTEN_DIS		= 0,
//	CAN_RMTEN_EN		= 1,
//	CAN_RXIE_DIS		= 0,
//	CAN_RXIE_EN			= 1,
//	CAN_TXIE_DIS		= 0,
//	CAN_TXIE_EN			= 1,
//	CAN_MASK_DIS		= 0,
//	CAN_MASK_EN			= 1,
//}
//csi_can_mcrx_e;


/**
 * \enum	csi_can_ifx_e
 * \brief   can IFX REG
 */
typedef enum
{
	CAN_IFX_DAR			= 0,		//DATA A	
	CAN_IFX_DBR			= 1,		//DATA B			
	CAN_IFX_MSKR		= 2,		//MASK	
	CAN_IFX_IR			= 3,		//IR			
	CAN_IFX_MCR			= 4,		//MCR	
	CAN_IFX_STRP		= 5			//STRT				
}csi_can_ifx_reg_e;

/**
 * \enum	csi_can_status_intsrc_e
 * \brief   can status interrupt source
 */
typedef enum
{
	CAN_INTSRC_NONE		= (0x00uL << 0),			//no interrupt
	CAN_INTSRC_ERWARNTR	= (0x01uL << 1),
	CAN_INTSRC_ERPASSTR	= (0x01uL << 2),
	CAN_INTSRC_BUSOFFTR	= (0x01uL << 3),
	CAN_INTSRC_ACTVT	= (0x01uL << 4),
	CAN_INTSRC_RXOK		= (0x01uL << 8),
	CAN_INTSRC_TXOK		= (0x01uL << 9),
	CAN_INTSRC_STUFF	= (0x01uL << 10),
	CAN_INTSRC_FORM		= (0x01uL << 11),			
	CAN_INTSRC_ACK		= (0x01uL << 12),
	CAN_INTSRC_BIT1		= (0x01uL << 13),
	CAN_INTSRC_BIT0		= (0x01uL << 14),	
	CAN_INTSRC_CRC		= (0x01uL << 15),
	CAN_INTSRC_STA_ALL	= (0xff1e)
}csi_can_sta_intsrc_e;

//message channel interrupt source
#define	CAN_CH_INTSRC_NONE		(0)
#define	CAN_CH_INTSRC_POS(ch)	(ch-1)					
#define	CAN_CH_INTSRC(ch)		(0x01ul << (ch-1))
#define	CAN_CH_STATUS_POS(ch)	(ch-1)
#define	CAN_CH_STATUS(ch)		(0x01ul << (ch-1))

/**
 * \enum	csi_can_msg_mode_e
 * \brief   can interrupt msg mode: status/source
 */
typedef enum
{
	CAN_MSG_STATUS		= 0,					//Status Interrupt Msg
	CAN_MSG_RECV		= 1,					//Receive Source(channel) Interrupt Msg
	CAN_MSG_SEND		= 2						//Send Source(channel) Interrupt Msg
}csi_can_msg_mode_e;

/**
 * \enum	csi_can_status_msg_e
 * \brief   can status interrupt source
 */
typedef enum
{
	CAN_MSG_ERWARNTR	= (0x01uL << 1),		//被动错误警告
	CAN_MSG_ERPASSTR	= (0x01uL << 2),		//被动错误发送或者回复
	CAN_MSG_BUSOFFTR	= (0x01uL << 3),		//总线关闭
	CAN_MSG_ACTVT		= (0x01uL << 4),		//RX接收活动状态
	CAN_MSG_RXOK		= (0x01uL << 8),		//接收OK
	CAN_MSG_TXOK		= (0x01uL << 9),		//发送OK
	CAN_MSG_STUFF		= (0x01uL << 10),		//报文填充错误
	CAN_MSG_FORM		= (0x01uL << 11),		//报文形式错误			
	CAN_MSG_ACK			= (0x01uL << 12),		//报文应答错误
	CAN_MSG_BIT1		= (0x01uL << 13),		//报文高位错误		
	CAN_MSG_BIT0		= (0x01uL << 14),		//报文低位错误	
	CAN_MSG_CRC			= (0x01uL << 15),		//报文CRC校验错误
	CAN_MSG_STATUS_ERR	= (0xfc0e),
	CAN_MSG_STATUS_ALL	= (0xff1e)
}csi_can_status_msg_e;


typedef struct {
	uint32_t			wBaudRate;			//baud rate
	uint8_t				bySyncJw;           //sync jmp length
	uint8_t				byPhSeg1;           //Phase segment1 
	uint8_t				byPhSeg2;           //Phase segmen2
	uint8_t				byBdrDiv;			
} csi_can_bittime_t;

/// \struct csi_can_ir_config_t
typedef struct {
	uint32_t			wExtId;        		//extend identifier
	uint16_t			hwStdId;	    	//standard identifier
	uint8_t				byMsgDir;			//message dir	
//	uint8_t				byMsgVal;			//message status, valid/invalid
	uint8_t				byIdmode;			//XTD BIT, identifier, stdid/extid	
} csi_can_ir_config_t;


/// \struct csi_can_mskr_config_t
typedef struct {
	uint8_t				byIdMdMsk;			//message identifier mode mask
	uint8_t				byIdDirMsk;			//message identifier dir mask
	uint16_t			hwStdIdMsk;	    	//standard identifier mask
	uint32_t			wExtIdMsk;         	//extend identifier mask
} csi_can_mskr_config_t;

/// \struct csi_can_tx_mcr_config_t
typedef struct {
	bool				bTxIeEn;			//tx interrupt ITPAND enable 	
	bool				bRmtEn;				//receive remote frame, set TXRQST
	bool				bTxReqEn;			//message identifier dir mask
	uint8_t				byDataLen;	    	//tx data length
} csi_can_tx_mcr_config_t;

/// \struct csi_can_rx_mcr_config_t
typedef struct {
	bool				bMskEn;				//umask enable
	bool				bRxIeEn;			//tx interrupt ITPAND enable 	
	bool				bOverWrEn;			//message identifier dir mask
	uint8_t				byDataLen;	    	//rx data length
} csi_can_rx_mcr_config_t;

/// \struct csi_can_data_config_t
typedef struct {
	uint8_t				bydata[4];	    	//tx data a 
} csi_can_data_config_t;


/// \struct csi_can_config_t
/// \brief  can parameter configuration, open to users  
typedef struct {
	bool				bAuReTran;			//auto retransmission
	uint8_t				byClkSrc;           //clk source 	
	uint16_t            hwStaInter;			//status interrupt
	uint32_t            wChnlInter;			//source channel interrupt
	uint32_t			wBaudRate;			//baud rate
} csi_can_config_t;


/// \struct csi_can_config_t
/// \brief  can tx parameter configuration, open to users  
typedef struct {
	csi_can_data_config_t	tDataA;			//data A and B	
	csi_can_data_config_t	tDataB;			//data A and B	
	csi_can_ir_config_t		tIr;			//IR
	csi_can_tx_mcr_config_t	tMcr;			//MCR
} csi_can_tx_config_t;


/// \struct csi_can_rx_config_t
/// \brief  can rx parameter configuration, open to users  
typedef struct {
	csi_can_ir_config_t		tIr;			//IR
	csi_can_mskr_config_t	tMskr;			//MSKR
	csi_can_rx_mcr_config_t tMcr;			//MCR
} csi_can_rx_config_t;


/// \struct csi_can_recv_t
/// \brief  can receive handle, open to users 
typedef struct {
	uint8_t				byChnlNum;			//message channel number
	uint8_t				byDataLen;			//message data length
	uint32_t			wRecvId;			//receive id
	uint32_t			wRecvData[2];		//receive status
} csi_can_recv_t;

/// \struct csi_can_trans_t
/// \brief  can receive handle, not open to users 
typedef struct {
	uint8_t				byStrChnl;			//receive start channel
	uint8_t 			byChTolNum;			//receive number of channels
	csi_can_recv_t 		*ptCanRecv;
} csi_can_trans_t;

extern csi_can_trans_t 	g_tCanTran;	


/**
  \brief       Initialize can Interface. Initialize the resources needed for the can interface
  \param[in]   ptCanBase	pointer of can register structure
  \param[in]   ptCanCfg    	pointer of can parameter config structure
  \return      error code \ref csi_error_t
*/
csi_error_t csi_can_init(csp_can_t *ptCanBase, csi_can_config_t *ptCanCfg);

/** 
  \brief 	   open can module(enable can)
  \param[in]   ptCanBase	pointer of can register structure
  \return	   none
 */ 
void csi_can_open(csp_can_t *ptCanBase);

/** 
  \brief 	   close can module(enable can)
  \param[in]   ptCanBase	pointer of can register structure
  \return	   none
 */ 
void csi_can_close(csp_can_t *ptCanBase);

/** 
  \brief 	   can msg channel send enable
  \param[in]   ptCanBase	pointer of can register structure
  \param[in]   eChNum		number of message
  \param[in]   byDataLen	data length of message
  \return 	   none
 */
void csi_can_chnl_send(csp_can_t *ptCanBase, csi_can_chnl_e eChNum, uint8_t byDataLen);

/** 
  \brief can read message channel send enable
  \param[in] pRecvBuf: pointer of  receive buffer
  \param[in] eChNum: number of message channel
  \return message length
 */
uint8_t csi_can_chnl_read(csp_can_t *ptCanBase, uint8_t *pRecvBuf, csi_can_chnl_e eChNum);

/** 
  \brief 	   initialize can tx parameter structure
  \param[in]   ptCanBase	pointer of can register structure
  \param[in]   eChNum		number of message
  \param[in]   ptTxCfg		pointer of can tx config structure
  \return 	   none
 */ 
void csi_can_tx_config(csp_can_t *ptCanBase, csi_can_chnl_e eChNum, csi_can_tx_config_t *ptTxCfg);

/** 
  \brief 	   initialize can rx parameter structure
  \param[in]   ptCanBase: pointer of can register structure
  \param[in]   eChNum		number of message
  \param[in]   ptRxCfg: pointer of can rx config structure
  \return 	   none
 */ 
void csi_can_rx_config(csp_can_t *ptCanBase, csi_can_chnl_e eChNum, csi_can_rx_config_t *ptRxCfg);

/** 
  \brief initialize can rx parameter structure
  \param[in] ptCanRecv: pointer of can receive structure
  \param[in] eStrChNum: start channel of receive message
  \param[in] byChnlNum: channel number of eceive message
  \return none
 */ 
void csi_can_recv_init(csi_can_recv_t *ptCanRecv, csi_can_chnl_e eChNum, uint8_t byChnlNum);

/** 
  \brief can message status interrupt enable
  \param[in] ptCanBase: pointer of can register structure
  \param[in] eStaIntSrc: can status interrupt source
  \param[in] bEnable: enable/disable interrupt
  \return none
 */
void csi_can_sta_int_enable(csp_can_t *ptCanBase, csi_can_sta_intsrc_e eStaIntSrc, bool bEnable);

/** 
  \brief can message channel interrupt enable
  \param[in] ptCanBase: pointer of can register structure
  \param[in] eChNum: number of message
  \param[in] bEnable: enable/disable interrupt
  \return none
 */
void csi_can_chnl_int_enable(csp_can_t *ptCanBase, csi_can_chnl_e eChNum, bool bEnable);

/** 
  \brief  	   can transfer manage register control 
  \param[in]   ptCanBase	pointer of can register structure
  \param[in]   eChNum		number of message
  \param[in]   wTmrCmd		transfer manage register command
  \return 	   none
 */
csi_error_t csi_can_tmr_ctrl(csp_can_t *ptCanBase, csi_can_chnl_e eChNum, csi_can_ifx_e eIfx, uint32_t wTmrCmd);

/** 
  \brief 	   get can receive data
  \param[in]   ptCanBase	pointer of can register structure
  \param[in]   eChNum		number of message
  \param[in]   eIfxReg		interface corresponding register(DAR/DBR)
  \return 	   value of DAR/DBR
 */ 
//uint32_t csi_can_get_data(csp_can_t *ptCanBase, csi_can_chnl_e eChNum, csi_can_ifx_reg_e eIfxReg);

/** 
  \brief 	   set can message data A bytes
  \param[in]   ptCanBase	pointer of can register structure
  \param[in]   eChNum		number of message
  \param[in]   pbyData		point of data a
  \return 	   none
 */ 
void csi_can_set_data_a(csp_can_t *ptCanBase, csi_can_chnl_e eChNum, uint8_t *pbyData);

/** 
  \brief 	   set can message data A bytes
  \param[in]   ptCanBase	pointer of can register structure
  \param[in]   eChNum		number of message
  \param[in]   pbyData		point of data b
  \return 	   none
 */ 
void csi_can_set_data_b(csp_can_t *ptCanBase, csi_can_chnl_e eChNum, uint8_t *pbyData);

/** 
  \brief 	   set can message mskr 
  \param[in]   ptCanBase	pointer of can register structure
  \param[in]   eChNum		number of message
  \param[in]   ptMskr		point of mskr config structure
  \return 	   none
 */ 
void csi_can_set_mskr(csp_can_t *ptCanBase, csi_can_chnl_e eChNum, csi_can_mskr_config_t *ptMskr);

/** 
  \brief 	   set can message ir
  \param[in]   ptCanBase	pointer of can register structure
  \param[in]   eChNum		number of message
  \param[in]   ptIr			point of ir config structure
  \return 	   none
 */ 
void csi_can_set_ir(csp_can_t *ptCanBase, csi_can_chnl_e eChNum, csi_can_ir_config_t *ptIr);

/** 
  \brief 	   set can message tx mcr
  \param[in]   ptCanBase	pointer of can register structure
  \param[in]   eChNum		number of message
  \param[in]   ptMcr		point of tx mcr config structure
  \return 	   none
 */ 
void csi_can_set_tx_mcr(csp_can_t *ptCanBase, csi_can_chnl_e eChNum, csi_can_tx_mcr_config_t *ptMcr);

/** 
  \brief 	   set can message rx mcr
  \param[in]   ptCanBase	pointer of can register structure
  \param[in]   eChNum		number of message
  \param[in]   ptMcr		point of rx mcr config structure
  \return 	   none
 */ 
void csi_can_set_rx_mcr(csp_can_t *ptCanBase, csi_can_chnl_e eChNum, csi_can_rx_mcr_config_t *ptMcr);


/** 
  \brief 	   can message txrqst enable
  \param[in]   ptCanBase	pointer of can register structure
  \param[in]   eChNum		number of message
  \return 	   none
 */ 
void csi_can_txrqst_enable(csp_can_t *ptCanBase, csi_can_chnl_e eChNum);

/** 
  \brief 	   set can message valid 
  \param[in]   ptCanBase	pointer of can register structure
  \param[in]   eChNum		number of message
  \param[in]   bEnbale		ENABLE/DISABLE(valid/invalid)
  \return 	   none
 */ 
void csi_can_msg_valid_enable(csp_can_t *ptCanBase, csi_can_chnl_e eChNum, bool bEnbale);

/** 
  \brief  	   can set interface corresponding register value
  \param[in]   ptCanBase	pointer of can register structure
  \param[in]   eIfxReg		interface corresponding register(DAR/DBR/MSKR/IR/MCR)
  \param[in]   eChNum		number of message
  \return 	   value of interface register
 */
uint32_t csi_can_get_ifx(csp_can_t *ptCanBase, csi_can_chnl_e eChNum, csi_can_ifx_reg_e eIfxReg);

/**
  \brief  	   can set interface corresponding register value
  \param[in]   ptCanBase	pointer of can register structure
  \param[in]   eChNum		number of message 
  \param[in]   eIfxReg		interface corresponding register(DAR/DBR/MSKR/IR/MCR)
  \param[in]   wValue		value of write ifx register
  \return 	   error code \ref csi_error_t
 */
csi_error_t csi_can_set_ifx(csp_can_t *ptCanBase, csi_can_chnl_e eChNum, csi_can_ifx_reg_e eIfxReg, uint32_t wValue);

/** 
  \brief  	   get receive message NEWDAT and clear NEWDAT and ITPND 
  \param[in]   ptCanBase	pointer of can register structure
  \param[in]   eChNum		number of message
  \return 	   mcr reg value
 */
uint32_t csi_can_get_clr_recvflg(csp_can_t *ptCanBase, csi_can_chnl_e eChNum);

/** 
  \brief can post(set) Receive/Send and Status msg 
  \param[in] eMsgMode: can msg mode, \ref csi_can_msg_mode_e
  \param[in] wMsg: status and channel msg
  \return true/false
 */ 
bool csi_can_post_msg(csi_can_msg_mode_e eMsgMode, uint32_t wMsg);

/** 
  \brief  get msg of receive/send channel receive/send message
  \param[in] eMsgMode: can msg mode(recv and send chnl), \ref csi_can_msg_mode_e 
  \param[in] eChNum: number of channel 
  \return true/false
 */
bool csi_can_get_msg(csi_can_msg_mode_e eMsgMode, csi_can_chnl_e eChNum);

/** 
  \brief  clr msg of can receive/send message channel 
  \param[in] eMsgMode: can msg mode(recv and send chnl), \ref csi_can_msg_mode_e
  \param[in] eChNum: number of channel 
  \return none
 */
void csi_can_clr_msg(csi_can_msg_mode_e eMsgMode, csi_can_chnl_e eChNum);

/** 
  \brief  get msg of receive channel message
  \param[in] none
  \return message channel mask
 */
uint32_t csi_can_get_recv_msg(void);

/** 
  \brief  clr msg of can status msg
  \param[in] none
  \return message status mask
 */
uint32_t csi_can_get_status_msg(void);

/** 
  \brief  clr msg of can status msg
  \param[in] wStaMsg：status message
  \return none
 */
void csi_can_clr_status_msg(csi_can_status_msg_e eStaMsg);


#ifdef __cplusplus
}
#endif

#endif /* _DRV_CAN_H_ */