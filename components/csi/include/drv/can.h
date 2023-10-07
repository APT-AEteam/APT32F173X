
/***********************************************************************//** 
 * \file  can.h
 * \brief  csi can driver head file
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-12 <td>V0.0  <td>ZJY   <td>initial
 * <tr><td> 2021-9-27 <td>V0.1  <td>ZJY   <td>code normalization
 * </table>
 * *********************************************************************
*/
#ifndef _DRV_CAN_H_
#define _DRV_CAN_H_

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
	CAN_ID_STD	= 0,		//CAN standard ID mode
	CAN_ID_EXT				//CAN extend ID mode
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
	CAN_DIRMSK_DIS	= 0,	//CAN message dir receive
	CAN_DIRMSK_EN			//CAN message dir send
}csi_can_dirmsk_e;

/**
 * \enum	csi_can_id_msk_e
 * \brief   can msg id mode mask
 */
typedef enum
{
	CAN_IDMSK_STD	= 0,	//CAN message standard ID mask
	CAN_IDMSK_EXT			//CAN message extend ID mask 
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
 * \enum	csi_can_ch_e
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
}csi_can_ch_e;

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
	CAN_INTSRC_ERR_ALL	= (0xfc1e),
	CAN_INTSRC_ALL		= (0xff1e)
}csi_can_intsrc_e;

//message channel interrupt source
#define	CAN_CH_INTSRC_NONE		(0)
#define	CAN_CH_INTSRC_POS(ch)	(ch-1)					
#define	CAN_CH_INTSRC(ch)		(0x01ul << (ch-1))
#define	CAN_CH_STATUS_POS(ch)	(ch-1)
#define	CAN_CH_STATUS(ch)		(0x01ul << (ch-1))


/**
 * \enum	csi_can_status_msg_e
 * \brief   can status interrupt source
 */
typedef enum
{
	CAN_STA_ERWARNTR	= (0x01uL << 1),		//error passive warning
	CAN_STA_ERPASSTR	= (0x01uL << 2),		//error passive transition
	CAN_STA_BUSOFFTR	= (0x01uL << 3),		//bus off transition
	CAN_STA_ACTVT		= (0x01uL << 4),		//RX activity
	CAN_STA_RXOK		= (0x01uL << 8),		//successfully received a message
	CAN_STA_TXOK		= (0x01uL << 9),		//successfully transmit a message
	CAN_STA_STUFF		= (0x01uL << 10),		//stuff error
	CAN_STA_FORM		= (0x01uL << 11),		//form error			
	CAN_STA_ACK			= (0x01uL << 12),		//acknowledge error
	CAN_STA_BIT1		= (0x01uL << 13),		//bit to one error		
	CAN_STA_BIT0		= (0x01uL << 14),		//bit to zero error	
	CAN_STA_CRC			= (0x01uL << 15),		//CRC error
	CAN_STA_CANENS		= (0x01ul << 16),		//Only Status
	CAN_STA_ERWARN		= (0x01ul << 17),		
	CAN_STA_ERPASS		= (0x01ul << 18),		
	CAN_STA_BUSOFF		= (0x01ul << 19),		
	CAN_STA_BUSY0		= (0x01ul << 20),
	CAN_STA_BUSY1		= (0x01ul << 21),
	CAN_STA_RS			= (0x01ul << 22),
	CAN_STA_TS			= (0x01ul << 23),	
	CAN_STA_CCENS		= (0x01ul << 24),
	CAN_STA_BTXPD		= (0x01ul << 25),
	CAN_STA_ERR			= (0x3fffc0e),
	CAN_STA_ALL			= (0x3ffff1f)
}csi_can_status_e;

/**
 * \enum     csi_can_callbackid_e
 * \brief    CAN callback id
 */
typedef enum{
	CAN_CALLBACK_RECV	=	0,				//can rteceive callback id
	CAN_CALLBACK_SEND,						//can rteceive callback id
	CAN_CALLBACK_ERR						//can error status callback id
}csi_can_callback_id_e;


typedef struct {
	uint32_t			wBaudRate;			//baud rate
	uint8_t				bySyncJw;           //sync jmp length
	uint8_t				byPhSeg1;           //Phase segment1 
	uint8_t				byPhSeg2;           //Phase segmen2
	uint8_t				byBdrDiv;			
} csi_can_bittime_t;

/// \struct csi_can_id_config_t
typedef struct {
	uint32_t			wExtId;        		//extend identifier
	uint16_t			hwStdId;	    	//standard identifier
	uint8_t				byMsgVal;			//message status, valid/invalid
	csi_can_dir_e		eMsgDir;			//message dir	
	csi_can_id_e		eIdMode;			//XTD BIT, identifier, stdid/extid	
} csi_can_id_config_t;


/// \struct csi_can_msk_config_t
typedef struct {
	csi_can_idmsk_e		eIdMdMsk;			//message identifier mode mask
	csi_can_dirmsk_e	eIdDirMsk;			//message identifier dir mask
	uint16_t			hwStdIdMsk;	    	//standard identifier mask
	uint32_t			wExtIdMsk;         	//extend identifier mask
} csi_can_msk_config_t;

/// \struct csi_can_tx_mc_config_t
typedef struct {
	bool				bTxIeEn;			//tx interrupt ITPAND enable 	
	bool				bRmtEn;				//receive remote frame, set TXRQST
	bool				bTxReqEn;			//message identifier dir mask
	uint8_t				byDataLen;	    	//tx data length
} csi_can_mc_tx_config_t;

/// \struct csi_can_rx_mc_config_t
typedef struct {
	bool				bMskEn;				//umask enable
	bool				bRxIeEn;			//tx interrupt ITPAND enable 	
	bool				bOverWrEn;			//message identifier dir mask
	uint8_t				byDataLen;	    	//rx data length
} csi_can_mc_rx_config_t;

/// \struct csi_can_data_config_t
typedef struct {
	uint8_t				bydata[4];	    	//tx data a 
} csi_can_data_config_t;


/// \struct csi_can_config_t
/// \brief  can parameter configuration, open to users  
typedef struct {
	csi_can_clksrc_e	eClkSrc;           	//clk source 	
	uint32_t			wBaudRate;			//baud rate
	bool				bAuReTran;			//auto retransmission
} csi_can_config_t;


/// \struct csi_can_config_t
/// \brief  can tx parameter configuration, open to users  
typedef struct {
	csi_can_data_config_t	tDataA;			//data A,byte0~3	
	csi_can_data_config_t	tDataB;			//data B,byte4~7	
	csi_can_id_config_t		tId;			//identifier reg
	csi_can_mc_tx_config_t	tMc;			//message contrl reg
} csi_can_tx_config_t;


/// \struct csi_can_rx_config_t
/// \brief  can rx parameter configuration, open to users  
typedef struct {
	csi_can_id_config_t		tId;			//IDR
	csi_can_msk_config_t	tMsk;			//MSKR
	csi_can_mc_rx_config_t 	tMc;			//MCR
} csi_can_rx_config_t;


/// \struct csi_can_recv_t
/// \brief  can receive handle, open to users 
typedef struct {
	uint8_t				byChNum;			//message channel number
	uint8_t				byDataLen;			//message data length
	uint32_t			wRecvId;			//receive id
	uint32_t			wRecvData[2];		//receive status
} csi_can_recv_t;

/// \struct csi_can_ctrl_t
/// \brief  can receive handle, not open to users 
typedef struct {
	uint8_t				byStrCh;			//receive start channel
	uint8_t 			byTolChNum;			//receive number of channels
	csi_can_recv_t 		*ptCanRecv;
	
	//CallBack		
	void(*recv_callback)(csp_can_t *ptCanBase, csi_can_recv_t *ptRecv);
	void(*send_callback)(csp_can_t *ptCanBase, uint8_t byCh);
	void(*err_callback)(csp_can_t *ptCanBase, uint32_t wIsr);
	
} csi_can_ctrl_t;

extern csi_can_ctrl_t 	g_tCanCtrl[CAN_IDX];	

/** 
  \brief 	   can interrupt handle function
  \param[in]   ptCanBase	pointer of can register structure
  \param[in]   byIdx		can idx(0)
  \return 	   none
 */ 
void csi_can_irqhandler(csp_can_t *ptCanBase, uint8_t byIdx);

/**
  \brief       Initialize can Interface. Initialize the resources needed for the can interface
  \param[in]   ptCanBase	pointer of can register structure
  \param[in]   ptCanCfg    	pointer of can parameter config structure
 				- eClkSrc: clk source
  				- wBaudRate: can Baud rate
 				- bAuReTran: automatic retransmission
  \return      error code \ref csi_error_t
*/
csi_error_t csi_can_init(csp_can_t *ptCanBase, csi_can_config_t *ptCanCfg);

/** 
  \brief  	   register can interrupt callback function
  \param[in]   ptCanBase	pointer of can register structure
  \param[in]   eCallBkId	can interrupt callback type, \ref csi_can_callback_id_e
  \param[in]   callback		can interrupt handle function
  \return 	   error code \ref csi_error_t
 */ 
csi_error_t csi_can_register_callback(csp_can_t *ptCanBase, csi_can_callback_id_e eCallBkId, void  *callback);

/** 
  \brief 	   open can module(enable can)
  \param[in]   ptCanBase	pointer of can register structure
  \return	   none
 */ 
void csi_can_start(csp_can_t *ptCanBase);

/** 
  \brief 	   close can module(enable can)
  \param[in]   ptCanBase	pointer of can register structure
  \return	   none
 */ 
void csi_can_stop(csp_can_t *ptCanBase);

/** 
  \brief 	   can message send 
  \param[in]   ptCanBase	pointer of can register structure
  \param[in]   eChNum		channel number of message, \ref csi_can_ch_e
  \param[in]   byDataLen	data length of message, 1~8
  \return 	   none
 */
void csi_can_msg_send(csp_can_t *ptCanBase, csi_can_ch_e eChNum, uint8_t byDataLen);

/** 
  \brief 	   can message send, interrupt mode
  \param[in]   ptCanBase	pointer of can register structure
  \param[in]   eChNum		channel number of message, \ref csi_can_ch_e
  \param[in]   byDataLen	data length of message, 1~8
  \return 	   none
 */
void csi_can_msg_send_int(csp_can_t *ptCanBase, csi_can_ch_e eChNum, uint8_t byDataLen);

/** 
  \brief can read message channel send enable
  \param[in] ptCanBase: pointer of can register structure
  \param[in] pRecv: pointer of receive buffer, \ref csi_can_recv_t
  \param[in] byStrCh: the start channel of message receive config
  \param[in] byTolChNum: the total number of message receive channels
  \return none
 */
void csi_can_msg_receive_int(csp_can_t *ptCanBase, csi_can_recv_t *ptRecv, uint8_t byStrCh, uint8_t byTolChNum);


/** 
  \brief 	   config can tx message channel parameter structure
  \param[in]   ptCanBase	pointer of can register structure
  \param[in]   eChNum		number of message
  \param[in]   ptTxCfg		pointer of can tx config structure
  				- tDataA: data a, byte0~3, \ref csi_can_data_config_t
  				- tDataB: data b, byte4~7, \ref csi_can_data_config_t
  				- tId: identifier config, \ref csi_can_id_config_t
  				- tMc: message contrl, \ref csi_can_mc_tx_config_t
  \return 	   none
 */ 
void csi_can_set_msg_tx(csp_can_t *ptCanBase, csi_can_ch_e eChNum, csi_can_tx_config_t *ptTxCfg);

/** 
  \brief 	   config can rx message channel parameter structure
  \param[in]   ptCanBase: pointer of can register structure
  \param[in]   eChNum		number of message
  \param[in]   ptRxCfg: pointer of can rx config structure
  				- tId: identifier config, \ref csi_can_id_config_t
  				- tMsk: identifier mask(filtering), \ref csi_can_msk_config_t
  				- tMc: message contrl, \ref csi_can_mc_tx_config_t
  \return 	   none
 */ 
void csi_can_set_msg_rx(csp_can_t *ptCanBase, csi_can_ch_e eChNum, csi_can_rx_config_t *ptRxCfg);

/** 
  \brief can message status interrupt enable
  \param[in] ptCanBase: pointer of can register structure
  \param[in] eStaIntSrc: can status interrupt source, \ref csi_can_intsrc_e
  \return none
 */
void csi_can_int_enable(csp_can_t *ptCanBase, csi_can_intsrc_e eStaIntSrc);

/** 
  \brief can message status interrupt disable
  \param[in] ptCanBase: pointer of can register structure
  \param[in] eStaIntSrc: can status interrupt source, \ref csi_can_intsrc_e
  \return none
 */
void csi_can_int_disable(csp_can_t *ptCanBase, csi_can_intsrc_e eStaIntSrc);

/** 
  \brief can message channel interrupt enable
  \param[in] ptCanBase: pointer of can register structure
  \param[in] eChNum: channel number of message, \ref csi_can_ch_e
  \return none
 */
void csi_can_ch_int_enable(csp_can_t *ptCanBase, csi_can_ch_e eChNum);

/** 
  \brief can message channel interrupt disable
  \param[in] ptCanBase: pointer of can register structure
  \param[in] eChNum: channel number of message, \ref csi_can_ch_e
  \return none
 */
void csi_can_ch_int_disable(csp_can_t *ptCanBase, csi_can_ch_e eChNum);

/** 
  \brief 	   set can message data A bytes(0~3bytes)
  \param[in]   ptCanBase	pointer of can register structure
  \param[in]   eChNum		channel number of message, \ref csi_can_ch_e
  \param[in]   pbyData		point of data a buffer
  \return 	   none
 */ 
void csi_can_set_adata(csp_can_t *ptCanBase, csi_can_ch_e eChNum, uint8_t *pbyData);

/** 
  \brief 	   set can message data A bytes(4~7bytes)
  \param[in]   ptCanBase	pointer of can register structure
  \param[in]   eChNum		channel number of message, \ref csi_can_ch_e
  \param[in]   pbyData		point of data b buffer
  \return 	   none
 */ 
void csi_can_set_bdata(csp_can_t *ptCanBase, csi_can_ch_e eChNum, uint8_t *pbyData);

/** 
  \brief 	   set can message identifier msk 
  \param[in]   ptCanBase	pointer of can register structure
  \param[in]   eChNum		channel number of message, \ref csi_can_ch_e
  \param[in]   ptMskr		point of mskr config structure, \ref csi_can_msk_config_t
  \return 	   none
 */ 
void csi_can_set_msk(csp_can_t *ptCanBase, csi_can_ch_e eChNum, csi_can_msk_config_t *ptMskr);

/** 
  \brief 	   set can message identifier
  \param[in]   ptCanBase	pointer of can register structure
  \param[in]   eChNum		channel number of message, \ref csi_can_ch_e
  \param[in]   ptIr			point of ir config structure, \ref csi_can_id_config_t
  \return 	   none
 */ 
void csi_can_set_id(csp_can_t *ptCanBase, csi_can_ch_e eChNum, csi_can_id_config_t *ptIr);

/** 
  \brief 	   set can message tx message control
  \param[in]   ptCanBase	pointer of can register structure
  \param[in]   eChNum		channel number of message, \ref csi_can_ch_e
  \param[in]   ptMcr		point of tx mcr config structure, \ref csi_can_mc_tx_config_t
  \return 	   none
 */ 
void csi_can_set_tx_mc(csp_can_t *ptCanBase, csi_can_ch_e eChNum, csi_can_mc_tx_config_t *ptMcr);

/** 
  \brief 	   set can message rx message control
  \param[in]   ptCanBase	pointer of can register structure
  \param[in]   eChNum		channel number of message, \ref csi_can_ch_e
  \param[in]   ptMcr		point of rx mcr config structure, \ref csi_can_mc_rx_config_t
  \return 	   none
 */ 
void csi_can_set_rx_mc(csp_can_t *ptCanBase, csi_can_ch_e eChNum, csi_can_mc_rx_config_t *ptMcr);


/** 
  \brief 	   can message txrqst enable
  \param[in]   ptCanBase	pointer of can register structure
  \param[in]   eChNum		channel number of message, \ref csi_can_ch_e
  \return 	   none
 */ 
void csi_can_txrqst_enable(csp_can_t *ptCanBase, csi_can_ch_e eChNum);

/** 
  \brief 	   set can message valid 
  \param[in]   ptCanBase	pointer of can register structure
  \param[in]   eChNum		channel number of message, \ref csi_can_ch_e
  \param[in]   bEnbale		ENABLE/DISABLE(valid/invalid)
  \return 	   none
 */ 
void csi_can_msg_valid_enable(csp_can_t *ptCanBase, csi_can_ch_e eChNum, bool bEnbale);

/** 
  \brief  	   can set interface corresponding register value
  \param[in]   ptCanBase	pointer of can register structure
  \param[in]   eChNum		channel number of message, \ref csi_can_ch_e
  \param[in]   eIfxReg		interface corresponding register(DAR/DBR/MSKR/IR/MCR)
  \return 	   value of interface register
 */
uint32_t csi_can_get_ifx(csp_can_t *ptCanBase, csi_can_ch_e eChNum, csi_can_ifx_reg_e eIfxReg);

/**
  \brief  	   can set interface corresponding register value
  \param[in]   ptCanBase	pointer of can register structure
  \param[in]   eChNum		number of message 
  \param[in]   eIfxReg		interface corresponding register(DAR/DBR/MSKR/IR/MCR)
  \param[in]   wValue		value of write ifx register
  \return 	   error code \ref csi_error_t
 */
csi_error_t csi_can_set_ifx(csp_can_t *ptCanBase, csi_can_ch_e eChNum, csi_can_ifx_reg_e eIfxReg, uint32_t wValue);

/** 
  \brief  get can transfer status 
  \param[in] ptCanBase: pointer of can register structure
  \return can status
 */
uint32_t csi_can_get_sr(csp_can_t *ptCanBase);

/** 
  \brief  clear can transfer status 
  \param[in] ptCanBase: pointer of can register structure
  \param[in] eStatus: can transfer status, \ref csi_can_status_e
  \return none
 */
void csi_can_clr_sr(csp_can_t *ptCanBase, csi_can_status_e eStatus);

/** 
  \brief  	   get receive message NEWDAT and clear NEWDAT and ITPND 
  \param[in]   ptCanBase	pointer of can register structure
  \param[in]   eChNum		channel number of message, \ref csi_can_ch_e
  \return 	   mcr reg value
 */
uint32_t csi_can_get_mcr_clr_flg(csp_can_t *ptCanBase, csi_can_ch_e eChNum);


#ifdef __cplusplus
}
#endif

#endif /* _DRV_CAN_H_ */