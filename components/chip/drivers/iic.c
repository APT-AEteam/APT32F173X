/***********************************************************************//** 
 * \file  iic.c
 * \brief  csi iic driver
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-9-03 <td>V0.0  <td>ZJY   <td>initial
 * <tr><td> 2021-1-21 <td>V0.1  <td>ZJY   <td> modified in 110
 * <tr><td> 2021-1-22 <td>V0.2  <td>WNN   <td> move to 102
 * <tr><td> 2023-9-14 <td>V0.3  <td>YT     <td>code normalization
 * </table>
 * *********************************************************************
*/

#include <drv/iic.h>

/* Private macro-----------------------------------------------------------*/
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/
/* Global variablesr------------------------------------------------------*/
csi_iic_slave_t g_tSlave;
volatile uint32_t g_wIicErrorCont = 0;
volatile uint8_t g_bySendIndex = 0;
volatile uint8_t g_byWriteIndex = 0;
volatile uint32_t g_wIicSlaveWriteAddress;
csi_iic_ctrl_t g_tIicCtrl[IIC_IDX];

/** \brief deinit iic 
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 *  \return none
 */  
void apt_iic_deinit(csp_iic_t *ptIicBase)
{
	csp_iic_disable(ptIicBase);
	csp_iic_int_disable(ptIicBase,IIC_INT_ALL);
	csp_iic_clr_isr(ptIicBase,IIC_INT_ALL);
}

/** \brief enable iic 
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 *  \return none
 */ 
void csi_iic_enable(csp_iic_t *ptIicBase)
{
	csp_iic_enable(ptIicBase);
}

/** \brief disable iic 
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 *  \return none
 */ 
void csi_iic_disable(csp_iic_t *ptIicBase)
{
	csp_iic_disable(ptIicBase);
}

/** \brief iic taddr set
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 *  \return none
 */ 

void csi_iic_set_taddr(csp_iic_t *ptI2cBase, uint32_t wTAddr)
{
	csp_iic_set_taddr(ptI2cBase, wTAddr);
}

/** \brief initialize iic slave
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 *  \param[in] ptIicSlaveCfg: pointer of iic slave config structure
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_iic_slave_init(csp_iic_t *ptIicBase, csi_iic_slave_config_t *ptIicSlaveCfg)
{
	uint32_t wIicClk;
	uint16_t hwSclL;
	uint16_t hwSclH;
	if((ptIicBase == NULL)||(ptIicSlaveCfg == NULL))
		return CSI_ERROR;
//	csp_pcer0_clk_enable(SYSCON,22);
	csi_clk_enable((uint32_t *)ptIicBase);
	
	apt_iic_deinit(ptIicBase);
	
	csp_iic_set_saddr(ptIicBase, ptIicSlaveCfg->hwSlaveAddr >> 1);

	wIicClk = csi_get_pclk_freq() / 1000000U;
	
	if(ptIicSlaveCfg->eSpeedMode == IIC_BUS_SPEED_STANDARD)
	{
		if(wIicClk<3)
			return CSI_ERROR;
		csp_iic_set_cr(ptIicBase,(IIC_BUS_SPEED_STANDARD<<1)|(ptIicSlaveCfg->eAddrMode<<3)| IIC_SLAVE);
		if(0 == wIicClk)
		{
			hwSclH = 8;
			hwSclL = 8;
		}
		else
		{
			hwSclH = wIicClk *4;
			hwSclL = wIicClk *47/10;
		}
		csp_iic_set_ss_sclh(ptIicBase,hwSclH);
		csp_iic_set_ss_scll(ptIicBase,hwSclL);
		csp_iic_set_sda_tsetup(ptIicBase,0xf);
		csp_iic_set_sda_thold(ptIicBase,0xf,0xf);
		
		
	}else if(ptIicSlaveCfg->eSpeedMode == IIC_BUS_SPEED_FAST){
		if(wIicClk<12)
			return CSI_ERROR;
		csp_iic_set_cr(ptIicBase,(IIC_BUS_SPEED_FAST<<1)|(ptIicSlaveCfg->eAddrMode<<3)| IIC_SLAVE);
		if(0 == wIicClk)
		{
			hwSclH = 8;
			hwSclL = 8;
		}
		else
		{
			hwSclH = wIicClk  * 60 / 100 + 1;			
			hwSclL = wIicClk  * 130 / 100 + 1;
			
		}
		
		csp_iic_set_fs_sclh(ptIicBase,hwSclH);
		csp_iic_set_fs_scll(ptIicBase,hwSclL);
		csp_iic_set_sda_tsetup(ptIicBase,0x2);
		csp_iic_set_sda_thold(ptIicBase,0x8,0x3);
	}else if(ptIicSlaveCfg->eSpeedMode == IIC_BUS_SPEED_FAST_PLUS){
		if(wIicClk<32)
			return CSI_ERROR;
		csp_iic_set_cr(ptIicBase,(IIC_BUS_SPEED_FAST<<1)|(ptIicSlaveCfg->eAddrMode<<3)| IIC_SLAVE);
		if(0 == wIicClk)
		{
			hwSclH = 8;
			hwSclL = 8;
		}
		else
		{
			hwSclH = wIicClk  * 26 / 100 + 1;			
			hwSclL = wIicClk  * 50 / 100 + 1;
			
		}
		
		csp_iic_set_fs_sclh(ptIicBase,hwSclH);
		csp_iic_set_fs_scll(ptIicBase,hwSclL);
		csp_iic_set_sda_tsetup(ptIicBase,0x2);
		csp_iic_set_sda_thold(ptIicBase,0x8,0x3);
	}
	csp_iic_set_rx_flsel(ptIicBase, 0x0);
	csp_iic_set_tx_flsel(ptIicBase, 0x7);
	csp_iic_set_sda_tout(ptIicBase,ptIicSlaveCfg->wSdaTimeout);
	csp_iic_set_scl_tout(ptIicBase,ptIicSlaveCfg->wSclTimeout);
	csp_iic_set_qualmode(ptIicBase,(iic_qual_e)ptIicSlaveCfg->eQualMode);
	csp_iic_set_slvqual(ptIicBase,ptIicSlaveCfg->hwMaskAddr);

	csi_iic_enable(ptIicBase);
    return CSI_OK;
}

/** \brief get iic number 
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 *  \return iic number0/ERROR
 */ 
static uint8_t apt_get_iic_idx(csp_iic_t *ptIicBase)
{
	switch((uint32_t)ptIicBase)
	{
		case APB_IIC0_BASE:		//IIC0
			return 0;		

		default:
			return 0xff;		//error
	}
}

/** \brief  register iic interrupt callback function
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 *  \param[in] eCallBkId: iic interrupt callback type, \ref csi_iic_callback_id_e
 *  \param[in] callback: iic interrupt handle function
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_iic_register_callback(csp_iic_t *ptIicBase, csi_iic_callback_id_e eCallBkId,void  *callback)
{
	uint8_t byIdx = apt_get_iic_idx(ptIicBase);
	if(byIdx == 0xff)
		return CSI_ERROR;
		
	
	switch(eCallBkId)
	{
		case IIC_CALLBACK_RECV:
			g_tIicCtrl[byIdx].recv_callback = callback;
			break;
		case IIC_CALLBACK_SEND:
			g_tIicCtrl[byIdx].send_callback = callback;
			break;
		case IIC_CALLBACK_STATE:
			g_tIicCtrl[byIdx].state_callback = callback;
			break;
		default:
			return CSI_ERROR;
	}
	
	return CSI_OK;
}

/** \brief iic interrupt handler function
 * 
 *  \param[in] ptUartBase: pointer of iic register structure
 *  \param[in] byIdx: iic idx(0)
 *  \return none
 */ 
void csi_iic_irqhandler(csp_iic_t *ptIicBase, uint8_t byIdx)
{
	uint8_t byIsr = csp_iic_get_isr(ptIicBase);
	
	if((csp_iic_get_isr(ptIicBase)&IIC_INT_SCL_SLOW)||(csp_iic_get_isr(ptIicBase)&IIC_INT_TX_ABRT))			 //SCLK SLOW or TX ABRT
	{
		csi_iic_disable(ptIicBase);
		csp_iic_set_data_cmd(ptIicBase, 0x00);
		csi_iic_enable(ptIicBase);
		g_bySendIndex=0;
		
		if(g_tIicCtrl[byIdx].state_callback)
			g_tIicCtrl[byIdx].state_callback(ptIicBase, byIsr);
		
		csp_iic_clr_isr(ptIicBase,IIC_INT_SCL_SLOW|IIC_INT_TX_ABRT);  
		g_wIicErrorCont=0;
		csp_iic_int_disable(ptIicBase,IIC_INT_TX_EMPTY);
		
	}else
	{
		if(csp_iic_get_isr(ptIicBase)&IIC_INT_RX_FULL)        //receive data full
		{
			if(g_bySendIndex == 0)
			{
				g_bySendIndex = 1;
				g_wIicSlaveWriteAddress = csp_iic_get_data(ptIicBase);
				g_byWriteIndex = g_wIicSlaveWriteAddress;
			}
			else
			{
				if(g_wIicSlaveWriteAddress<g_tSlave.hwRxSize)
				{
					*(g_tSlave.pbySlaveRxBuf+g_wIicSlaveWriteAddress)= csp_iic_get_data(ptIicBase);
				}
				g_wIicSlaveWriteAddress++;
			}
			
			
			if(g_tIicCtrl[byIdx].recv_callback)    
				g_tIicCtrl[byIdx].recv_callback(ptIicBase,g_tSlave.pbySlaveRxBuf ,&g_tSlave.hwRxSize);  
			
			csp_iic_clr_isr(ptIicBase,IIC_INT_RX_FULL);
			g_wIicErrorCont=0;
		}
		
		if(csp_iic_get_isr(ptIicBase)&IIC_INT_RD_REQ)			//read request 
		{
			if(g_bySendIndex==1)
			{
				g_bySendIndex=2;
				csp_iic_int_enable(ptIicBase,IIC_INT_TX_EMPTY);
				if(g_byWriteIndex<g_tSlave.hwTxSize)
				{
					csp_iic_set_data_cmd(ptIicBase, *(g_tSlave.pbySlaveTxBuf+g_byWriteIndex));
					
				}else{
					csp_iic_set_data_cmd(ptIicBase, 0xFF);
				}
				g_byWriteIndex++;
			}

			if(g_tIicCtrl[byIdx].send_callback)   
				g_tIicCtrl[byIdx].send_callback(ptIicBase);

			csp_iic_clr_isr(ptIicBase,IIC_INT_RD_REQ);
			g_wIicErrorCont=0;
		} 

		if(csp_iic_get_isr(ptIicBase)&IIC_INT_TX_EMPTY)				//IIC TX EMPTY
		{
			if(g_bySendIndex==2)
			{
				if(g_byWriteIndex<g_tSlave.hwTxSize)
				{
					csp_iic_set_data_cmd(ptIicBase, *(g_tSlave.pbySlaveTxBuf+g_byWriteIndex));
				}
				else{
					csp_iic_set_data_cmd(ptIicBase, 0xFF);
				}
				g_byWriteIndex++;
			}
			else
			{
				csp_iic_int_disable(ptIicBase,IIC_INT_TX_EMPTY);
				if(g_wIicErrorCont>10000)
				{
					csi_iic_disable(ptIicBase);
					csp_iic_set_data_cmd(ptIicBase, 0x00);
					csi_iic_enable(ptIicBase);
					g_wIicErrorCont=0;
				}
				else
				{
					g_wIicErrorCont++;
				}
			}
			
			if(g_tIicCtrl[byIdx].send_callback)      
				g_tIicCtrl[byIdx].send_callback(ptIicBase);
			
			
			csp_iic_clr_isr(ptIicBase,IIC_INT_TX_EMPTY);
		}
		if(csp_iic_get_isr(ptIicBase)&IIC_INT_STOP_DET)
		{
			csp_iic_int_disable(ptIicBase,IIC_INT_TX_EMPTY);
			
			if(g_tIicCtrl[byIdx].state_callback)       
				g_tIicCtrl[byIdx].state_callback(ptIicBase, byIsr);		
			
			
			csp_iic_clr_isr(ptIicBase,IIC_INT_STOP_DET);
			if(g_bySendIndex!=0)
			{
				g_bySendIndex=0;
			}
			g_wIicErrorCont=0;
		}
	}
}

/** \brief IIC interrupt enable control
 * 
 *  \param[in] ptLedBase: pointer of bt register structure
 *  \param[in] eIntSrc: iic interrupt source \ref csi_iic_intsrc_e
 *  \return none
 */ 
void csi_iic_int_enable(csp_iic_t *ptIicBase, csi_iic_intsrc_e eIntSrc)
{
	csp_iic_clr_isr(ptIicBase,(iic_int_e)eIntSrc);
	csp_iic_int_enable(ptIicBase, (iic_int_e)eIntSrc);
}

/** \brief IIC interrupt disable control
 * 
 *  \param[in] ptLedBase: pointer of bt register structure
 *  \param[in] eIntSrc: iic interrupt source \ref csi_iic_intsrc_e
 *  \return none
 */ 
void csi_iic_int_disable(csp_iic_t *ptIicBase, csi_iic_intsrc_e eIntSrc)
{
	csp_iic_clr_isr(ptIicBase,(iic_int_e)eIntSrc);
}


/** \brief initialize iic slave
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 *  \param[in] ptIicMasterCfg: pointer of iic master config structure
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_iic_master_init(csp_iic_t *ptIicBase, csi_iic_master_config_t *ptIicMasterCfg)
{
	
	uint32_t wIicClk;
	uint16_t hwSclL;
	uint16_t hwSclH ;
	if((ptIicBase == NULL)||(ptIicMasterCfg == NULL))
		return CSI_ERROR;
//	csp_pcer0_clk_enable(SYSCON,22);
	csi_clk_enable((uint32_t *)ptIicBase);
	apt_iic_deinit(ptIicBase);
	wIicClk = csi_get_pclk_freq() / 1000000U;
	
	if(ptIicMasterCfg->eSpeedMode == IIC_BUS_SPEED_STANDARD)
	{
		if(wIicClk<3)
			return CSI_ERROR;
		csp_iic_set_cr(ptIicBase,(IIC_BUS_SPEED_STANDARD<<1)|(ptIicMasterCfg->eAddrMode<<4) | IIC_MASTER |(ptIicMasterCfg->bReStart<<5));
		if(0 == wIicClk)
		{
			hwSclH = 8;
			hwSclL = 8;
		}
		else
		{
			hwSclH = wIicClk *4;
			hwSclL = wIicClk *47/10;
		}
		csp_iic_set_ss_sclh(ptIicBase,hwSclH);
		csp_iic_set_ss_scll(ptIicBase,hwSclL);
		csp_iic_set_sda_tsetup(ptIicBase,0xf);
		csp_iic_set_sda_thold(ptIicBase,0xf,0xf);
		
	}else if(ptIicMasterCfg->eSpeedMode == IIC_BUS_SPEED_FAST)
	{
		if(wIicClk<12)
			return CSI_ERROR;
		csp_iic_set_cr(ptIicBase,(IIC_BUS_SPEED_FAST<<1)|(ptIicMasterCfg->eAddrMode<<4) | IIC_MASTER |(ptIicMasterCfg->bReStart<<5));
		if(0 == wIicClk)
		{
			hwSclH = 8;
			hwSclL = 8;
		}
		else
		{
			hwSclH = wIicClk  * 60 / 100 + 1;			
			hwSclL = wIicClk  * 130 / 100 + 1;
			
		}
		
		csp_iic_set_fs_sclh(ptIicBase,hwSclH);
		csp_iic_set_fs_scll(ptIicBase,hwSclL);
		csp_iic_set_sda_tsetup(ptIicBase,0x2);
		csp_iic_set_sda_thold(ptIicBase,0x8,0x3);
	}else if(ptIicMasterCfg->eSpeedMode == IIC_BUS_SPEED_FAST_PLUS){
		if(wIicClk<32)
			return CSI_ERROR;
		csp_iic_set_cr(ptIicBase,(IIC_BUS_SPEED_FAST<<1)|(ptIicMasterCfg->eAddrMode<<4) | IIC_MASTER |(ptIicMasterCfg->bReStart<<5));
		if(0 == wIicClk)
		{
			hwSclH = 8;
			hwSclL = 8;
		}
		else
		{
			hwSclH = wIicClk  * 26 / 100 + 1;			
			hwSclL = wIicClk  * 50 / 100 + 1;
			
		}
		
		csp_iic_set_fs_sclh(ptIicBase,hwSclH);
		csp_iic_set_fs_scll(ptIicBase,hwSclL);
		csp_iic_set_sda_tsetup(ptIicBase,0x2);
		csp_iic_set_sda_thold(ptIicBase,0x8,0x3);
	}

	csp_iic_set_rx_flsel(ptIicBase, 0x7);
	csp_iic_set_tx_flsel(ptIicBase,0x9);
	csp_iic_set_sda_tout(ptIicBase,ptIicMasterCfg->wSdaTimeout);
	csp_iic_set_scl_tout(ptIicBase,ptIicMasterCfg->wSclTimeout);	

    return CSI_OK;
}


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
csi_error_t csi_iic_master_write_nbyte(csp_iic_t *ptIicBase,uint32_t wDevAddr, uint32_t wWriteAdds, uint8_t byWriteAddrNumByte,volatile uint8_t *pbyIicData,uint32_t wNumByteToWrite)
{
	uint16_t hwEerorCont=0;
	uint8_t byWriteAdds = 0;
	uint32_t i;
	
	if((ptIicBase == NULL)||(pbyIicData == NULL)||(wNumByteToWrite == 0)||(byWriteAddrNumByte == 0))
		return CSI_ERROR;
	
	csi_iic_disable(ptIicBase);
	csi_iic_set_taddr(ptIicBase, wDevAddr >> 1);
	csi_iic_enable(ptIicBase);
	
	switch(byWriteAddrNumByte)
	{
		case 1:
				byWriteAdds = wWriteAdds&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byWriteAdds);
			break;
		case 2:
				byWriteAdds = (wWriteAdds>>8)&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byWriteAdds);
				byWriteAdds = wWriteAdds&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byWriteAdds);
			break;
		case 3:
				byWriteAdds = (wWriteAdds>>16)&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byWriteAdds);
				byWriteAdds = (wWriteAdds>>8)&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byWriteAdds);
				byWriteAdds = wWriteAdds&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byWriteAdds);
				
			break;
		case 4:
				byWriteAdds = (wWriteAdds>>24)&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byWriteAdds);
				byWriteAdds = (wWriteAdds>>16)&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byWriteAdds);
				byWriteAdds = (wWriteAdds>>8)&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byWriteAdds);
				byWriteAdds = wWriteAdds&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byWriteAdds);
			break;	
		default:
			break;
	}
	for(i=0;i<wNumByteToWrite;i++)
	{
		if(i>=wNumByteToWrite-1)
		{
			csp_iic_set_data_cmd(ptIicBase,*(pbyIicData+i) |IIC_CMD_STOP);
		}
		else
		{
			csp_iic_set_data_cmd(ptIicBase,*(pbyIicData+i));
		}
		do
		{
			if(hwEerorCont++>=10000)
			{
				csi_iic_disable(ptIicBase);
				csi_iic_enable(ptIicBase);
				return CSI_ERROR;
			}
		}
		while( (csp_iic_get_sr(ptIicBase) & IIC_BUSY) != IIC_BUSY ); 		//Wait for FSM working
		do
		{
			if(hwEerorCont++>=10000)
			{
				csi_iic_disable(ptIicBase);
				csi_iic_enable(ptIicBase);
				return CSI_ERROR;
			}
		}
		while((csp_iic_get_sr(ptIicBase) & IIC_TFNF) != IIC_TFNF);
		hwEerorCont=0;
	}
	
	return CSI_OK;
}

/** \brief  iic  master  read n byte data
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 * 	\param[in] wDevAddr: Addrress of slave device
 *  \param[in] wReadAdds: Read address
 * 	\param[in] byReadAddrNumByte: Read address length (unit byte)
 * 	\param[in] pbyIicData: Read the address pointer of the data storage array
 * 	\param[in] wNumByteRead: Read data length
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_iic_master_read_nbyte(csp_iic_t *ptIicBase,uint32_t wDevAddr, uint32_t wReadAdds, uint8_t byReadAddrNumByte,volatile uint8_t *pbyIicData,uint32_t wNumByteRead)
{
	uint16_t hwEerorCont=0;
	uint32_t i;
	uint8_t byreadnum = 1;
	uint8_t byReadAdds = 0;
	if((ptIicBase == NULL)||(pbyIicData == NULL)||(wNumByteRead == 0)||(byReadAddrNumByte == 0))
		return CSI_ERROR;
	
	
	csi_iic_disable(ptIicBase);
	csi_iic_set_taddr(ptIicBase,wDevAddr >> 1);
	csi_iic_enable(ptIicBase);
	
	switch(byReadAddrNumByte)
	{
		case 1:
				byReadAdds = wReadAdds&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byReadAdds|IIC_CMD_RESTART1);
			break;
		case 2:
				byReadAdds = (wReadAdds>>8)&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byReadAdds|IIC_CMD_RESTART1);
				byReadAdds = wReadAdds;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byReadAdds);
			break;
		case 3:
				byReadAdds = (wReadAdds>>16)&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byReadAdds|IIC_CMD_RESTART1);
				byReadAdds = (wReadAdds>>8)&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byReadAdds);
				byReadAdds = wReadAdds&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byReadAdds);
				
			break;
		case 4:
				byReadAdds = (wReadAdds>>24)&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byReadAdds|IIC_CMD_RESTART1);
				byReadAdds = (wReadAdds>>16)&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byReadAdds);
				byReadAdds = (wReadAdds>>8)&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byReadAdds);
				byReadAdds = wReadAdds&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byReadAdds);
			break;	
		default:
			break;
	}
	for(i=0;i<wNumByteRead;i++)
	{
		
		if(byreadnum == 1)
		{	
			if(wNumByteRead > 1)
			{
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_READ);
				byreadnum = 0;
			}				
		}	
		if(wNumByteRead == 1)
		{
			csp_iic_set_data_cmd(ptIicBase,IIC_CMD_READ|IIC_CMD_STOP);
		}else{
		
			if(i>=wNumByteRead-2)
			{
				if(i == wNumByteRead-2)
					csp_iic_set_data_cmd(ptIicBase,IIC_CMD_READ|IIC_CMD_STOP);
			}
			else
			{
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_READ);
			}
		}

		do
		{
			if(hwEerorCont++>=10000)
			{
				return CSI_ERROR;
			}
		}
		while( (csp_iic_get_sr(ptIicBase) & IIC_BUSY) != IIC_BUSY ); 		//Wait for FSM working
		do
		{
			if(hwEerorCont++>=10000)
			{
				return CSI_ERROR;
			}
		}
		while( (csp_iic_get_sr(ptIicBase) & IIC_RFNE) != IIC_RFNE ); 		//Wait for RX done
		
		*(pbyIicData+i)=csp_iic_get_data(ptIicBase);

	}
	return CSI_OK;
}

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
csi_error_t csi_iic_master_read_nbyte_dma(csp_iic_t *ptIicBase,uint32_t wDevAddr, uint32_t wReadAdds, uint8_t byReadAddrNumByte,volatile uint8_t *pbyIicData,uint32_t wNumByteRead)
{
	uint32_t i;
	uint8_t byreadnum = 1;
	uint8_t byReadAdds = 0;
	if((ptIicBase == NULL)||(pbyIicData == NULL)||(wNumByteRead == 0)||(byReadAddrNumByte == 0))
		return CSI_ERROR;
	
	
	csi_iic_disable(ptIicBase);
	csi_iic_set_taddr(ptIicBase,wDevAddr >> 1);
	csi_iic_enable(ptIicBase);
	
	switch(byReadAddrNumByte)
	{
		case 1:
				byReadAdds = wReadAdds&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byReadAdds|IIC_CMD_RESTART1);
			break;
		case 2:
				byReadAdds = (wReadAdds>>8)&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byReadAdds|IIC_CMD_RESTART1);
				byReadAdds = wReadAdds;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byReadAdds);
			break;
		case 3:
				byReadAdds = (wReadAdds>>16)&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byReadAdds|IIC_CMD_RESTART1);
				byReadAdds = (wReadAdds>>8)&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byReadAdds);
				byReadAdds = wReadAdds&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byReadAdds);
				
			break;
		case 4:
				byReadAdds = (wReadAdds>>24)&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byReadAdds|IIC_CMD_RESTART1);
				byReadAdds = (wReadAdds>>16)&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byReadAdds);
				byReadAdds = (wReadAdds>>8)&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byReadAdds);
				byReadAdds = wReadAdds&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byReadAdds);
			break;	
		default:
			break;
	}
	for(i=0;i<wNumByteRead;i++)
	{
		
		if(byreadnum == 1)
		{	
			if(wNumByteRead > 1)
			{
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_READ);
				byreadnum = 0;
			}				
		}	
		if(wNumByteRead == 1)
		{
			csp_iic_set_data_cmd(ptIicBase,IIC_CMD_READ|IIC_CMD_STOP);
		}else{
		
			if(i>=wNumByteRead-2)
			{
				if(i == wNumByteRead-2)
					csp_iic_set_data_cmd(ptIicBase,IIC_CMD_READ|IIC_CMD_STOP);
			}
			else
			{
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_READ);
				mdelay(1);
			}
		}

	}
	return CSI_OK;
}

/** \brief  set iic rxdma
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 *  \param[in] eRxDmaEn: RX DMA enable
 *  \param[in] eRxDmaSel: RX DMA function select
 *  \return none
 */ 
void csi_iic_set_receive_dma(csp_iic_t *ptI2cBase, iic_rdma_en_e eRxDmaEn, iic_rdma_sel_e eRxDmaSel) 
{
	csp_iic_set_receive_dma(ptI2cBase, eRxDmaEn, eRxDmaSel);
}

/** \brief  set iic txdma
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 *  \param[in] eTxDmaEn: TX DMA enable
 *  \param[in] eTxDmaSel: TX DMA function select
 *  \return none
 */ 
void csi_iic_set_send_dma(csp_iic_t *ptI2cBase, iic_tdma_en_e eTxDmaEn, iic_tdma_sel_e eTxDmaSel) 
{
	csp_iic_set_send_dma(ptI2cBase,eTxDmaEn,eTxDmaSel);
}


/** \brief clear iic interrupt 
 * 
 *  \param[in] ptI2cBase: pointer of iic register structure
 *  \param[in] eIntSrc: iic interrupt source
 *  \return none
 */ 
void csi_iic_clr_isr(csp_iic_t *ptI2cBase, csi_iic_intsrc_e eIntSrc)
{
	csp_iic_clr_isr(ptI2cBase, (iic_int_e)eIntSrc);	
}

/** \brief  set iic slave  tx/rx buffer
 * 
 * 	\param[in] pbyIicRxBuf: pointer of iic RX data buffer
 *  \param[in] hwIicRxSize: Rx buffer size
 * 	\param[in] pbyIicTxBuf: pointer of iic TX data buffer
 * 	\param[in] hwIicTxSize: Tx buffer size
 *  \return none
 */ 
void csi_iic_set_slave_buffer(volatile uint8_t *pbyIicRxBuf,uint16_t hwIicRxSize,volatile uint8_t *pbyIicTxBuf,uint16_t hwIicTxSize)
{
	g_tSlave.pbySlaveRxBuf = pbyIicRxBuf;
	g_tSlave.pbySlaveTxBuf = pbyIicTxBuf;
	g_tSlave.hwRxSize = hwIicRxSize;
	g_tSlave.hwTxSize = hwIicTxSize;
}



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
csi_error_t csi_iic_read_nbyte_dma(csp_iic_t *ptIicBase,uint32_t wDevAddr, uint32_t wReadAdds, uint8_t byReadAddrNumByte,volatile uint8_t *pbyIicData,uint32_t wNumByteRead)
{
	uint32_t i;
	uint8_t byreadnum = 1;
	uint8_t byReadAdds = 0;
	if((ptIicBase == NULL)||(pbyIicData == NULL)||(wNumByteRead == 0)||(byReadAddrNumByte == 0))
		return CSI_ERROR;
	
	
	csi_iic_disable(ptIicBase);
	csp_iic_set_taddr(ptIicBase,wDevAddr >> 1);
	csi_iic_enable(ptIicBase);
	
	switch(byReadAddrNumByte)
	{
		case 1:
				byReadAdds = wReadAdds&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byReadAdds|IIC_CMD_RESTART1);
			break;
		case 2:
				byReadAdds = (wReadAdds>>8)&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byReadAdds|IIC_CMD_RESTART1);
				byReadAdds = wReadAdds;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byReadAdds);
			break;
		case 3:
				byReadAdds = (wReadAdds>>16)&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byReadAdds|IIC_CMD_RESTART1);
				byReadAdds = (wReadAdds>>8)&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byReadAdds);
				byReadAdds = wReadAdds&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byReadAdds);
				
			break;
		case 4:
				byReadAdds = (wReadAdds>>24)&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byReadAdds|IIC_CMD_RESTART1);
				byReadAdds = (wReadAdds>>16)&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byReadAdds);
				byReadAdds = (wReadAdds>>8)&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byReadAdds);
				byReadAdds = wReadAdds&0xff;
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_WRITE|byReadAdds);
			break;	
		default:
			break;
	}
	for(i=0;i<wNumByteRead;i++)
	{
		
		if(byreadnum == 1)
		{	
			if(wNumByteRead > 1)
			{
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_READ);
				byreadnum = 0;
			}				
		}	
		if(wNumByteRead == 1)
		{
			csp_iic_set_data_cmd(ptIicBase,IIC_CMD_READ|IIC_CMD_STOP);
		}else{
		
			if(i>=wNumByteRead-2)
			{
				if(i == wNumByteRead-2)
					csp_iic_set_data_cmd(ptIicBase,IIC_CMD_READ|IIC_CMD_STOP);
			}
			else
			{
				csp_iic_set_data_cmd(ptIicBase,IIC_CMD_READ);
				mdelay(1);
			}
		}

	}
	return CSI_OK;
}
