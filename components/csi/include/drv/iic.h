/***********************************************************************//** 
 * \file  iic.h
 * \brief  IIC head file 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-9-01 <td>V0.0  <td>ZJY   <td>initial
 * <tr><td> 2021-1-21 <td>V0.1  <td>ZJY   <td> modified in 110
 * <tr><td> 2021-1-22 <td>V0.2  <td>WNN   <td> move to 102
 * <tr><td> 2023-9-14 <td>V0.3  <td>YT    <td>code normalization
 * </table>
 * *********************************************************************
*/

#ifndef _DRV_IIC_H_
#define _DRV_IIC_H_
#include "csp.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \enum     csi_iic_callbackid_e
 * \brief    IIC callback id
 */
typedef enum{
	IIC_CALLBACK_RECV	=	0,		//iic rteceive callback id
	IIC_CALLBACK_SEND,				//iic send callback id
	IIC_CALLBACK_STATE,				//iic other state callback id
}csi_iic_callback_id_e;



/**
  \enum        csi_iic_speed_e
  \brief       iic speed mode
 */
typedef enum {
    IIC_BUS_SPEED_STANDARD        = 1U,     ///< Standard Speed  (<=100kHz)
    IIC_BUS_SPEED_FAST,                     ///< Fast Speed      (<=400kHz)
    IIC_BUS_SPEED_FAST_PLUS,                ///< Fast plus Speed (<=  1MHz)
} csi_iic_speed_e;


/**
  \enum        csi_iic_address_mode_t
  \brief       iic address mode
 */
typedef enum {
    IIC_ADDRESS_7BIT               = 0U,    ///< 7-bit address mode
    IIC_ADDRESS_10BIT                       ///< 10-bit address mode
} csi_iic_addr_mode_e;

typedef enum
{
	I2C_QUALMASK	= 0,
	I2C_QUALEXTEND    
}csi_iic_qual_e;

/**
  \struct       csi_iic_master_config_t
  \brief       iic master parameter configuration
 */
typedef struct csi_iic_master_config {
	csi_iic_speed_e		  eSpeedMode;    //IIC Speed mode
	csi_iic_addr_mode_e	  eAddrMode;	 //IIC ADDR mode 7/10 bit
	bool                  bReStart;	     //IIC restart enable/disable
	uint32_t	          wSdaTimeout;   //IIC SDA timeout SET
	uint32_t	          wSclTimeout;   //IIC SCL timeout SET
}csi_iic_master_config_t;

/**
  \struct       csi_iic_slave_config_t
  \brief       iic slave parameter configuration
 */
typedef struct csi_iic_slave_config {
	uint16_t	         hwSlaveAddr;   //IIC Slave address
	uint16_t	         hwMaskAddr ;   //IIC Slave address
	csi_iic_qual_e       eQualMode;     //IIC Qual mode
	csi_iic_speed_e		 eSpeedMode;    //IIC Speed mode
	csi_iic_addr_mode_e	 eAddrMode;	    //IIC ADDR mode 7/10 bit
	uint32_t	         wSdaTimeout;   //IIC SDA timeout SET
	uint32_t	         wSclTimeout;   //IIC SCL timeout SET
}csi_iic_slave_config_t;



/**
  \struct       csi_iic_slave_t
  \brief       iic slave buffer 
 */
typedef struct {	
	volatile uint8_t	*pbySlaveRxBuf;	//slave  receive buffer
	volatile uint8_t	*pbySlaveTxBuf;	//slave  send buffer
	uint16_t        hwRxSize;	//receive buffer size
	uint16_t        hwTxSize;	//send buffer size	
} csi_iic_slave_t;

extern csi_iic_slave_t g_tSlave;	
 


/**
  \enum        csi_iic_event_t
  \brief       iic event signaled by iic driver
 */
typedef enum {
    IIC_EVENT_SEND_COMPLETE        = 0U,     ///< Master/slave Send finished
    IIC_EVENT_RECEIVE_COMPLETE,              ///< Master/slave Receive finished
	IIC_EVENT_SLAVE_RECEIVE_ADDR_CMD,        ///< slave Receive RESTART DET
    IIC_EVENT_ERROR_OVERFLOW,                ///< Master/slave fifo overflow error
    IIC_EVENT_ERROR_UNDERFLOW,               ///< Master/slave fifo underflow error
    IIC_EVENT_ERROR                          ///< The receive buffer was completely filled to FIFO and more data arrived. That data is lost
} csi_iic_event_e;

/**
  \enum        csi_iic_intsrc_e
  \brief       iic interrupt source
 */
typedef enum
{
	IIC_INTSRC_NONE     	= (0x00ul << 0),
	IIC_INTSRC_RX_UNDER		= (0x01ul << 0),   //IIC Interrupt Status  
	IIC_INTSRC_RX_OVER      = (0x01ul << 1),   //IIC Interrupt Status   
	IIC_INTSRC_RX_FULL      = (0x01ul << 2),   //IIC Interrupt Status         
	IIC_INTSRC_TX_OVER      = (0x01ul << 3),   //IIC Interrupt Status         
	IIC_INTSRC_TX_EMPTY     = (0x01ul << 4),   //IIC Interrupt Status         
	IIC_INTSRC_RD_REQ       = (0x01ul << 5),   //IIC Interrupt Status         
	IIC_INTSRC_TX_ABRT      = (0x01ul << 6),   //IIC Interrupt Status         
	IIC_INTSRC_RX_DONE      = (0x01ul << 7),   //IIC Interrupt Status         
	IIC_INTSRC_BUSY       	= (0x01ul << 8),   //IIC Interrupt Status         
	IIC_INTSRC_STOP_DET     = (0x01ul << 9),   //IIC Interrupt Status         
	IIC_INTSRC_START_DET    = (0x01ul <<10),   //IIC Interrupt Status         
	IIC_INTSRC_GEN_CALL     = (0x01ul <<11),   //IIC Interrupt Status         
	IIC_INTSRC_RESTART_DET	= (0x01ul <<12),   //IIC Interrupt Status               
	IIC_INTSRC_SCL_SLOW     = (0x01ul <<14),   //IIC Interrupt Status  
	IIC_INTSRC_ALL          = (0xFFFFul << 0)  //ALL interrupt
} csi_iic_intsrc_e;

/** \brief initialize iic slave
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 *  \param[in] ptIicSlaveCfg: pointer of iic slave config structure
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_iic_slave_init(csp_iic_t *ptIicBase, csi_iic_slave_config_t *ptIicSlaveCfg);

/** \brief initialize iic slave
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 *  \param[in] ptIicMasterCfg: pointer of iic master config structure
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_iic_master_init(csp_iic_t *ptIicBase, csi_iic_master_config_t *ptIicMasterCfg);



/** \brief IIC interrupt enable control
 * 
 *  \param[in] ptLedBase: pointer of bt register structure
 *  \param[in] eIntSrc: led interrupt source \ref csi_led_intsrc_e
 *  \return none
 */ 
void csi_iic_int_enable(csp_iic_t *ptIicBase, csi_iic_intsrc_e eIntSrc);

/** \brief IIC interrupt disable control
 * 
 *  \param[in] ptLedBase: pointer of bt register structure
 *  \param[in] eIntSrc: iic interrupt source \ref csi_iic_intsrc_e
 *  \return none
 */ 
void csi_iic_int_disable(csp_iic_t *ptIicBase, csi_iic_intsrc_e eIntSrc);

/** \brief enable iic 
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 *  \return none
 */ 
void csi_iic_enable(csp_iic_t *ptIicBase);

/** \brief disable iic 
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 *  \return none
 */ 
void csi_iic_disable(csp_iic_t *ptIicBase);

/** \brief  iic  master  write n byte data
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 * 	\param[in] wDevAddr: Addrress of slave device
 *  \param[in] wWriteAdds: Write address
 * 	\param[in] byWriteAddrNumByte: Write address length (unit byte)
 * 	\param[in] pbyIicData: pointer of Write data
 * 	\param[in] wNumByteToWrite: Write data length
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_iic_master_write_nbyte(csp_iic_t *ptIicBase,uint32_t wDevAddr, uint32_t wWriteAdds, uint8_t byWriteAddrNumByte,volatile uint8_t *pbyIicData,uint32_t wNumByteToWrite);


/** \brief  iic  master  read n byte data
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 * 	\param[in] wDevAddr: Addrress of slave device
 *  \param[in] wReadAdds: Read address
 * 	\param[in] wReadAddrNumByte: Read address length (unit byte)
 * 	\param[in] pbyIicData: Read the address pointer of the data storage array
 * 	\param[in] wNumByteRead: Read data length
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_iic_master_read_nbyte(csp_iic_t *ptIicBase,uint32_t wDevAddr, uint32_t wReadAdds, uint8_t wReadAddrNumByte,volatile uint8_t *pbyIicData,uint32_t wNumByteRead);



/** \brief  iic  master  read n byte data by dma
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 * 	\param[in] wDevAddr: Addrress of slave device
 *  \param[in] wReadAdds: Read address
 * 	\param[in] byReadAddrNumByte: Read address length (unit byte)
 * 	\param[in] pbyIicData: Read the address pointer of the data storage array
 * 	\param[in] wNumByteRead: Read data length
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_iic_read_nbyte_dma(csp_iic_t *ptIicBase,uint32_t wDevAddr, uint32_t wReadAdds, uint8_t byReadAddrNumByte,volatile uint8_t *pbyIicData,uint32_t wNumByteRead);

/** \brief  IIC slave handler
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 *  \return none
 */ 
void csi_iic_slave_receive_send(csp_iic_t *ptIicBase);

/** \brief  iic  master  read n byte data
 * 
 * 	\param[in] pbyIicRxBuf: pointer of iic RX data buffer
 *  \param[in] hwIicRxSize: Rx buffer size
 * 	\param[in] pbyIicTxBuf: pointer of iic TX data buffer
 * 	\param[in] hwIicTxSize: Tx buffer size
 *  \return none
 */ 
void csi_iic_set_slave_buffer(volatile uint8_t *pbyIicRxBuf,uint16_t hwIicRxSize,volatile uint8_t *pbyIicTxBuf,uint16_t hwIicTxSize);


/** \brief set iic SPKLEN
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 *  \param[in] bySpklenCfg: filter set 
 *  \return none
 */ 
void csi_iic_spklen_set(csp_iic_t *ptIicBase, uint8_t bySpklen);



/** \brief  set iic slave address qualifier mode
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 *  \param[in] eQualmode: iic slave address qualifier mode
 *  \return error code \ref csi_error_t
 */ 
void csi_iic_qualmode_set(csp_iic_t *ptIicBase,iic_qual_e eQualmode);


/** \brief  set iic slave address qualifier value
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 *  \param[in] wSlvqual: iic slave address qualifier value
 *  \return error code \ref csi_error_t
 */ 
void csi_iic_slvqual_set(csp_iic_t *ptIicBase,uint32_t wSlvqual);

/** \brief  register iic interrupt callback function
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 *  \param[in] callback: iic interrupt handle function
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_iic_register_callback(csp_iic_t *ptIicBase, csi_iic_callback_id_e eCallBkId,void  *callback);

/// \struct csi_iic_ctrl_t
/// \brief  bt control handle, not open to users  
typedef struct 
{
	void(*recv_callback)(csp_iic_t *ptIicBase, volatile uint8_t *pbyBuf, uint16_t *hwSzie);
	void(*send_callback)(csp_iic_t *ptIicBase);
	void(*state_callback)(csp_iic_t *ptIicBase, uint16_t hwIsr);
} csi_iic_ctrl_t;

extern csi_iic_ctrl_t g_tIicCtrl[IIC_IDX];


/** \brief uart interrupt handler function
 * 
 *  \param[in] ptUartBase: pointer of uart register structure
 *  \param[in] byIdx: uart idx(0/1/2)
 *  \return none
 */ 
void csi_iic_irqhandler(csp_iic_t *ptIicBase, uint8_t byIdx);

/** \brief iic taddr set
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 *  \return none
 */ 

void csi_iic_set_taddr(csp_iic_t *ptI2cBase, uint32_t wTAddr);



/** \brief  set iic rxdma
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 *  \param[in] eRxDmaEn: RX DMA enable
 *  \param[in] eRxDmaSel: RX DMA function select
 *  \return none
 */ 
void csi_iic_set_receive_dma(csp_iic_t *ptI2cBase, iic_rdma_en_e eRxDmaEn, iic_rdma_sel_e eRxDmaSel); 

/** \brief  set iic txdma
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 *  \param[in] eTxDmaEn: TX DMA enable
 *  \param[in] eTxDmaSel: TX DMA function select
 *  \return none
 */ 
void csi_iic_set_send_dma(csp_iic_t *ptI2cBase, iic_tdma_en_e eTxDmaEn, iic_tdma_sel_e eTxDmaSel);

/** \brief clear iic interrupt 
 * 
 *  \param[in] ptI2cBase: pointer of iic register structure
 *  \param[in] eIntSrc: iic interrupt source
 *  \return none
 */ 
void csi_iic_clr_isr(csp_iic_t *ptI2cBase, csi_iic_intsrc_e eIntSrc);

#ifdef __cplusplus
}
#endif

#endif /* _DRV_IIC_H_ */
