/***********************************************************************//** 
 * \file  qspi.c
 * \brief  csi qspi driver
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-7-13 <td>V0.0  <td>LQ   <td>initial
 * </table>
 * *********************************************************************
*/

#include <drv/qspi.h>
#include <drv/gpio.h>
#include <drv/pin.h>
#include <drv/porting.h>
#include <drv/tick.h>
#include <iostring.h>
#include <uart.h>

#include "irq.h"
#include "csp_qspi.h"
/* Private macro------------------------------------------------------*/
/* externs function---------------------------------------------------*/
/* externs variablesr-------------------------------------------------*/
/* Private variablesr-------------------------------------------------*/

csi_qspi_transmit_t g_tQspiTransmit; 

/** \brief csi_qspi_nss_high 
 * 
 *  \param[in] ePinName:which pin use as nss
 *  \return none
 */ 
void csi_qspi_nss_high(pin_name_e ePinName)
{
	csi_pin_set_high(ePinName);
}

/** \brief csi_qspi_nss_low 
 * 
 *  \param[in] ePinName:which pin use as nss
 *  \return none
 */ 
void csi_qspi_nss_low(pin_name_e ePinName)
{
	csi_pin_set_low(ePinName);
}

/** \brief init qspi gpio 
 * 
 *  \param[in] none
 *  \return none
 */ 
void csi_qspi_gpio_init(void)
{
	
//	csi_pin_set_mux(PC7, PC7_OUTPUT);                         //gpio_port as output
//	csi_pin_output_mode(PC7, GPIO_PUSH_PULL);                 //push pull mode
//	csi_qspi_nss_high(PC7);									  //QSPI NSS init high		

	//csi_pin_set_mux(PA9,PA9_QSPI_Nss);					      // QSPI_NSS
	//csi_pin_set_mux(PC3,PC3_QSPI_nSS);					      // QSPI_NSS
	//csi_pin_set_mux(PA9, PA9_INPUT); 
	//csi_pin_pull_mode(PA9, GPIO_PULLUP);
	
	csi_pin_set_mux(PC7,PC7_QSPI_nCS0);					      // QSPI_nCS0
	csi_pin_set_mux(PC13,PC13_QSPI_CLK);					  // QSPI_SCK
	
	csi_pin_set_mux(PC14,PC14_QSPI_IO0);					  // QSPI_IO0
	csi_pin_set_mux(PC15,PC15_QSPI_IO1);					  // QSPI_IO1
	csi_pin_set_mux(PD0,PD0_QSPI_IO2);						  // QSPI_IO2
	csi_pin_set_mux(PD1,PD1_QSPI_IO3);						  // QSPI_IO3

}

/** \brief apt_qspi_int_set 
 * 
 *  \param[in] ptQspiBase: pointer of qspi register structure
 *  \param[in] byQspiInt:qspi interrupt source 
 *  \return none
 */ 
static void apt_qspi_int_set(csp_qspi_t *ptQspiBase,uint8_t byQspiInt)
{
	if(byQspiInt != QSPI_NONE_INT)//收发使用中断设置
	{
		csi_irq_enable((uint32_t *)ptQspiBase);
		csp_qspi_set_int(ptQspiBase, byQspiInt,true);
	}
	else//不使用中断则关闭
	{
		csi_irq_disable((uint32_t *)ptQspiBase);
		csp_qspi_set_int(ptQspiBase, 0x3f,false);
	}
}

/** \brief config qspi work frequence
 * 
 *  \param[in] ptQspiBase: pointer of qspi register structure
 *  \param[in] wBaud: qspi work baud
 *  \return qspi config frequency
 */
uint32_t csi_qspi_baud(csp_qspi_t *ptQspiBase, uint32_t wBaud)
{
    uint32_t wDiv;
    volatile uint32_t wFreq = 0U;
	
	if(wBaud > 0)
	{
		wDiv = csi_get_pclk_freq() / wBaud;	//baud = FPCLK/ CPSDVR 
		
		if( 2 > wDiv )
			wDiv = 2;
		
		if(65534 < wDiv)
			wDiv = 65534;
	}
	else
	{
		wDiv = 65534;
	}
	csp_qspi_set_cpsr(ptQspiBase, wDiv);
	wFreq = csi_get_pclk_freq() / wDiv ;
		
    return wFreq;
}

/** \brief config qspi Internal variables
 * 
 *  \param[in] ptQspiCfg: pointer of internal variables struct
 *  \return none
 */
void csi_qspi_Internal_variables_init(csi_qspi_config_t *ptQspiCfg)
{	
	g_tQspiTransmit.pbyRxData =NULL;
	g_tQspiTransmit.byRxSize =0;
	g_tQspiTransmit.pbyTxData =NULL;
	g_tQspiTransmit.byTxSize =0;
	g_tQspiTransmit.byRxFifoLength = ptQspiCfg->byRxFifoLevel;
	g_tQspiTransmit.byTxFifoLength = ptQspiCfg->byTxFifoLevel;
	g_tQspiTransmit.byInter = ptQspiCfg->byInter;
	g_tQspiTransmit.byWriteable = 0;
	g_tQspiTransmit.byReadable  = 0;
}


/** \brief initialize qspi data structure
 * 
 *  \param[in] ptQspiBase: pointer of qspi register structure
 *  \param[in] ptQspiCfg: pointer of user qspi parameter config
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_qspi_init(csp_qspi_t *ptQspiBase,csi_qspi_config_t *ptQspiCfg)
{
	csi_error_t tRet = CSI_OK;
	
	csi_qspi_gpio_init();			      	 				   //端口初始化

	//csi_clk_enable((uint32_t *)ptQspiBase);				   //打开时钟,FPGA测试默认打开
	
	csp_qspi_default_init(ptQspiBase);					   	   //寄存器初始值复位
	csp_qspi_dis(ptQspiBase);								   //关闭qspi
	csp_qspi_set_lbm(ptQspiBase, QSPI_SRLB_DIS);			   //enable qspi lbm, dbug
	csp_qspi_set_rxifl(ptQspiBase, ptQspiCfg->byRxFifoLevel);  //设置接收fifo阈值
	csp_qspi_set_txifl(ptQspiBase, ptQspiCfg->byTxFifoLevel);  //设置发送fifo阈值
	csp_qspi_set_cp_format(ptQspiBase, ptQspiCfg->eQspiPolarityPhase);   //极性和相位设置	
	csp_qspi_set_frame_len(ptQspiBase, ptQspiCfg->eQspiFrameLen);	     //格式帧长度设置
	csi_qspi_baud(ptQspiBase, ptQspiCfg->dwQspiBaud);                //通信速率
	apt_qspi_int_set(ptQspiBase,ptQspiCfg->byInter); 				 //中断配置
	csp_qspi_set_slvste(ptQspiBase, QSPI_SLVSTE_DIS); 				 //DIS TOGGLE
	csi_qspi_Internal_variables_init(ptQspiCfg);					 //内部使用，客户无需更改
			
							
//	csp_qspi_set_tmod(ptQspiBase, QSPI_SEND);	 		//发模式
//	csp_qspi_set_frameformat(ptQspiBase, QSPI_FRF_FOUR);		//1线收，1线发，其它(2线，4线)
//	csp_qspi_set_frametype(ptQspiBase, QSPI_INSSTD_ADDSTD);   //指令和地址都为标准spi
//	csp_qspi_set_instl(ptQspiBase, INSTL_8);		 		//设置指令长度
//	csp_qspi_set_addrl(ptQspiBase, ADDRL_8);		 		//设置地址长度
//	csp_qspi_set_ser(ptQspiBase, QSPI_S0EN,true);			//选择nCSNONE						
//	csp_qspi_set_slvste(ptQspiBase, QSPI_SLVSTE_DIS); //DIS TOGGLE
	
//	csp_qspi_set_tmod(ptQspiBase, QSPI_SEND);	 		//发模式
//	csp_qspi_set_frameformat(ptQspiBase, QSPI_FRF_FOUR);		//1线收，1线发，其它(2线，4线)
//	csp_qspi_set_frametype(ptQspiBase, QSPI_INSSTD_ADDSTD);   //指令和地址都为标准spi
//	csp_qspi_set_instl(ptQspiBase, INSTL_8);		 		//设置指令长度
//	csp_qspi_set_addrl(ptQspiBase, ADDRL_8);		 		//设置地址长度
//	csp_qspi_set_ser(ptQspiBase, QSPI_S0EN,true);			//选择nCS0						
//	csp_qspi_set_slvste(ptQspiBase, QSPI_SLVSTE_DIS); //DIS TOGGLE
						
	//csp_qspi_en(ptQspiBase);							             //打开qspi
	
	return tRet;
}

/** \brief uninit qspi data structure
 * 
 *  \param[in] ptQspiBase: pointer of qspi register structure
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_qspi_uninit(csp_qspi_t *ptQspiBase)
{
	csi_error_t tRet = CSI_OK;
	
	csi_clk_disable((uint32_t *)ptQspiBase);	
	csi_irq_disable((uint32_t *)ptQspiBase);
	csp_qspi_default_init(ptQspiBase);
	g_tQspiTransmit.pbyRxData =NULL;
	g_tQspiTransmit.byRxSize =0;
	g_tQspiTransmit.pbyTxData =NULL;
	g_tQspiTransmit.byTxSize =0;
	g_tQspiTransmit.byRxFifoLength = 0;
	g_tQspiTransmit.byTxFifoLength = 0;
	g_tQspiTransmit.byInter = 0;
	g_tQspiTransmit.byWriteable = 0;
	g_tQspiTransmit.byReadable  = 0;
	
	return tRet;	
}

/** \brief Wait status flag with timeout
 * 
 *  \param[in] ptQspiBase: pointer of qspi register structure
 *  \param[in] bySource: flag type
 *  \param[in] bySta: desired state
 *  \param[in] wTime: timeout
 *  \return 0:success  1:failure
 */ 
uint8_t csi_qspi_waitflag(csp_qspi_t *ptQspiBase,uint8_t bySource,uint8_t bySta,uint32_t wTime)
{
	uint8_t byFlagsta=0;
	while(wTime)
	{
		byFlagsta=( ptQspiBase->SR & bySource ) ? 1 : 0; 
		if(byFlagsta == bySta) break;
		wTime--;
	}
	if(wTime)return 0;
	else return 1;
}

/** \brief set waitcycle
 * 
 *  \param[in] ptQspiBase: pointer of qspi register structure
 *  \param[in] bySta: desired state
 *  \return none
 */ 
void csi_qspi_set_waitcycle(csp_qspi_t *ptQspiBase,uint8_t byCycle)
{
	uint8_t byTemp;
	
	if(byCycle > 31)
		byTemp = 31;
	else
		byTemp = byCycle;
		
	csp_qspi_set_waitcycle(ptQspiBase,byTemp);
}

/** \brief clr interrupt
 * 
 *  \param[in] ptQspiBase: pointer of qspi register structure
 *  \param[in] byInt: interrupt source
 *  \return none
 */ 
void csi_qspi_clr_int(csp_qspi_t *ptQspiBase,uint8_t byInt)
{
	switch(byInt)
	{
		case QSPI_NONE_INT:////none interrupt
			break;
		case QSPI_TXE_INT://send fifo empty interrupt
			break;
		case QSPI_TXO_INT://send fifo overflow interrupt
			csp_qspi_clr_int(ptQspiBase,0x02);	
			break;
		case QSPI_RXU_INT://receive fifo underflow interrupt
			csp_qspi_clr_int(ptQspiBase,0x08);	
			break;
		case QSPI_RXO_INT://receive fifo overflow interrupt
			csp_qspi_clr_int(ptQspiBase,0x04);	
			break;
		case QSPI_RXF_INT://receive fifo full interrupt
			break;
		case QSPI_MST_INT://Multi - host contention interrupt
			csp_qspi_clr_int(ptQspiBase,0x10);	
			break;
		case (QSPI_MST_INT | QSPI_TXO_INT | QSPI_RXU_INT | QSPI_RXO_INT) ://clear all int
			csp_qspi_clr_int(ptQspiBase,0x01);	
			break;
		default:
			csp_qspi_clr_int(ptQspiBase,0x00);
			break;
		
	}

}

/** \brief qspi receive
 * 
 *  \param[in] ptQspiBase: pointer of qspi register structure
 *  \param[in] byInst: instruct
 *  \param[in] wAddr: read start address
 * 	\param[in] ptRecv: pointer of receive data buff
 *  \param[in] wLen: data size
 *  \return 0:read failure   1:success
 */ 
uint8_t csi_qspi_receive(csp_qspi_t *ptQspiBase,uint8_t byInst,uint32_t wAddr,uint8_t *ptRecv,uint32_t wLen)
{
	uint8_t byStatus;
	uint8_t byRes = 1; 
    uint8_t byInstl = 0;
	uint8_t byAddrl = 0;
	
	byInstl = csp_qspi_get_instl(QSPI);
	byAddrl = csp_qspi_get_addrl(QSPI);
	csp_qspi_set_read_number(ptQspiBase,wLen);       //接收数据的个数
	
	csp_qspi_en(ptQspiBase);
	csi_qspi_waitflag(ptQspiBase,QSPI_BSY,0x00,0xffff);//忙的话等待
	
	if(byInstl)
	{
		csp_qspi_write_data(ptQspiBase,byInst);
	}
	
	if(byAddrl)
	{	
		csp_qspi_write_data(ptQspiBase,wAddr);
	}

	while(wLen)
	{
		byStatus = csi_qspi_waitflag(ptQspiBase,QSPI_RNE,0x01,0xffff);
		if(byStatus==0)						
		{
			*ptRecv++=csp_qspi_read_data(ptQspiBase);
			wLen--;
		}
		else
		{
			byRes =  0;
			break;
		}
	}

	return byRes;	
}

/** \brief qspi send
 * 
 *  \param[in] ptQspiBase: pointer of qspi register structure
 *  \param[in] byInst: instruct
 *  \param[in] wAddr: start addr of write data (qspi在一页（0-4095），写入少于256个字节的数据)
 *  \param[in] pbySend: pointer of write data buf
 *  \param[in] hwLen: length of write data(0-256)
 *  \return 0:send failure   1:success
 */
uint8_t csi_qspi_send(csp_qspi_t *ptQspiBase,uint8_t byInst,uint32_t wAddr,uint8_t *pbySend,uint16_t hwLen)
{		
	uint8_t byStatus;
	uint8_t byRes = 1;
	uint8_t byInstl = 0;
	uint8_t byAddrl = 0;
	
	byInstl = csp_qspi_get_instl(QSPI);
	byAddrl = csp_qspi_get_addrl(QSPI);
	csp_qspi_en(QSPI);
	
	if(byInstl)
	{
		csp_qspi_write_data(ptQspiBase,byInst);
	}
	
	if(byAddrl)
	{	
		csp_qspi_write_data(ptQspiBase,wAddr);
	}
	
	while(hwLen)
	{
		byStatus = csi_qspi_waitflag(QSPI,QSPI_TNF,0x01,0xffff);
		if(byStatus==0)						
		{
			csp_qspi_write_data( QSPI,*pbySend++ );
			hwLen--;
		}
		else
		{
			byRes =  0;
			break;
		}
	}
	
	csi_qspi_waitflag(ptQspiBase,QSPI_TFE,0x01,0xffff);//等待数据全部传完
	return byRes;	
}


/** \brief set send_receive or send only or receive only
 * 
 *  \param[in] ptQspiBase: pointer of qspi register structure
 *  \param[in] eMode: QSPI_SEND_RECV,QSPI_SEND,QSPI_RECV
 *  \param[in] eFrf: select standard,dual,or quad spi
 * 	\param[in] byPara: bit0:3(addre len)  bit4:5(instruct len)  bit6:7(addre and instruct frametype)
 *  \param[in] wLen: data size
 *  \return 0:read failure   1:success
 */ 
void csi_qspi_set_cmd(csp_qspi_t *ptQspiBase,qspi_tmod_e eMode,qspi_frf_e eFrf,uint8_t byPara,qspi_se_e eSlvnss)
{
	csi_qspi_waitflag(QSPI,QSPI_BSY,0x00,0xffff);
	csp_qspi_dis(ptQspiBase);
	csp_qspi_dis(ptQspiBase);
	csp_qspi_set_tmod(ptQspiBase, eMode);	 					//设置模式
	csp_qspi_set_tmod(ptQspiBase, eMode);	 					//设置模式
	csp_qspi_set_frameformat(ptQspiBase, eFrf);					//1线收，1线发，其它(2线，4线)
	csp_qspi_set_frametype(ptQspiBase, (byPara >> 6) & 0x03);   //指令和地址都为标准spi
	csp_qspi_set_instl(ptQspiBase, (byPara >> 4) & 0x03);		//设置指令长度
	csp_qspi_set_addrl(ptQspiBase, byPara & 0x0f);		 		//设置地址长度
	csi_qspi_set_waitcycle(ptQspiBase,0);                 		//等待周期设置为0
	csp_qspi_set_ser(ptQspiBase, eSlvnss,true);					//选择nCS0
	
}

/** \brief qspi async send
 * 
 *  \param[in] ptQspiBase: pointer of qspi register structure
 *  \param[in] pData:pointer of qspi send buff
 *  \param[in] wSize: data length
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_qspi_asyns_send(csp_qspi_t *ptQspiBase,void *pData,uint32_t wSize)
 {
	 csi_error_t tRet = CSI_OK;
	
	if((g_tQspiTransmit.byWriteable == 1U))		
        tRet = CSI_BUSY;
		
	if(tRet == CSI_OK)
	{		
		g_tQspiTransmit.byWriteable = 1U;
		g_tQspiTransmit.byTxSize = wSize;
		g_tQspiTransmit.pbyTxData = (uint8_t *)pData;	
		apt_qspi_int_set(ptQspiBase,QSPI_TXE_INT); 				 //中断配置
		
		csp_qspi_en(ptQspiBase);
	}
	else
	{
		tRet = CSI_ERROR;
	}
	return tRet;
 }


/** \brief qspi async receive
 * 
 *  \param[in] ptQspiBase: pointer of qspi register structure
 *  \param[in] pData:pointer of qspi send buff
 *  \param[in] wSize: data length
 *  \param[in] byInst: instruct
 *  \param[in] wAddr: read address
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_qspi_asyns_receive(csp_qspi_t *ptQspiBase,void *pData,uint32_t wSize,uint8_t byInst,uint32_t wAddr)
{
	csi_error_t tRet = CSI_OK;
	
	if((g_tQspiTransmit.byReadable == 1U))		
        tRet = CSI_BUSY;
		
	if(tRet == CSI_OK)
	{
		csp_qspi_set_read_number(ptQspiBase,wSize+1);      //接收数据的个数

		g_tQspiTransmit.byReadable = 1U;
		g_tQspiTransmit.byRxSize = wSize+1;
		g_tQspiTransmit.pbyRxData = (uint8_t *)pData;	
		apt_qspi_int_set(ptQspiBase,QSPI_RXF_INT); 		   //中断配置
		csp_qspi_en(ptQspiBase);
		
		csi_qspi_waitflag(ptQspiBase,QSPI_BSY,0x00,0xffff);//等待空闲
		csp_qspi_write_data(ptQspiBase,byInst);  		   //命令
		csp_qspi_write_data(ptQspiBase,wAddr);		       //地址
		
	}
	else
	{
		tRet = CSI_ERROR;
	}
	return tRet;
}

/** \brief qspi dma receive init
 * 
 *  \param[in] ptQspiBase: pointer of qspi register structure
 *  \param[in] wSize: data length
 *  \param[in] byInst: instruct
 *  \param[in] wAddr: read address
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_qspi_dma_receive_init(csp_qspi_t *ptQspiBase,uint32_t wSize,uint8_t byInst,uint32_t wAddr)
{
	csi_error_t tRet = CSI_OK;
	
	if((g_tQspiTransmit.byReadable == 1U))		
        tRet = CSI_BUSY;
		
	if(tRet == CSI_OK)
	{
		csi_qspi_set_cmd(QSPI,QSPI_RECV,QSPI_FRF_FOUR,(QSPI_INSSTD_ADDSTD<<6 | INSTL_8<<4 | ADDRL_24),QSPI_S0EN);//四线读id
		csi_qspi_set_waitcycle(QSPI,8);
		csp_qspi_set_read_number(ptQspiBase,wSize);        //接收数据的个数
		csp_qspi_en(ptQspiBase);
		
		csi_qspi_waitflag(ptQspiBase,QSPI_BSY,0x00,0xffff);//等待空闲
		csp_qspi_write_data(ptQspiBase,byInst);  		   //命令
		csp_qspi_write_data(ptQspiBase,wAddr);		       //地址
		
	}
	else
	{
		tRet = CSI_ERROR;
	}
	return tRet;
}

/** \brief send data of qspi by DMA
 * 
 *  \param[in] ptQspiBase: pointer of QSPI reg structure.
 *  \param[in] pData: pointer to buffer data of QSPI transmitter.
 *  \param[in] hwSize: number of data to send (byte).
 *  \param[in] ptDmaBase: pointer of DMA reg structure.
 *  \param[in] byDmaCh: channel of DMA(0 -> 5)
 *  \return  none
 */
void csi_qspi_send_dma(csp_qspi_t *ptQspiBase, const void *pData, uint16_t hwSize, csp_dma_t *ptDmaBase,uint8_t byDmaCh)
{
	//csp_qspi_set_txdma_level(ptQspiBase, QSPI_DMA_TX_LEVEL15);//类似发送未满
	//csp_qspi_set_txdma_enable(ptQspiBase, QSPI_TDMAE_EN);
	csi_irq_enable((uint32_t *)QSPI);
	csi_dma_ch_start(ptDmaBase, byDmaCh, (void *)pData, (void *)&(ptQspiBase->DRx[0]), hwSize,1);
}

/** \brief receive data of qspi by DMA
 * 
 *  \param[in] ptQspiBase: pointer of QSPI reg structure.
 *  \param[in] pbyRecv: pointer to buffer data of QSPI receive.
 *  \param[in] hwSize: number of data to receive (byte).
 *  \param[in] ptDmaBase: pointer of DMA reg structure.
 *  \param[in] byDmaCh: channel of DMA(0 -> 5)
 *  \return  none
 */
void csi_qspi_recv_dma(csp_qspi_t *ptQspiBase, void *pbyRecv, uint16_t hwSize,csp_dma_t *ptDmaBase, uint8_t byDmaCh)
{
	csp_qspi_set_rxdma_level(ptQspiBase, QSPI_DMA_RX_LEVEL1);//类似接收非空
	csp_qspi_set_rxdma_enable(ptQspiBase, QSPI_RDMAE_EN);
	csi_dma_ch_start(ptDmaBase, byDmaCh, (void *)&(ptQspiBase->DRx[0]),(void *)pbyRecv, hwSize,1);
}

/** \brief qspi interrupt handle weak function
 * 
 *  \param[in] ptQspiBase: pointer of qspi register structure
 *  \return none
 */ 
__attribute__((weak)) void qspi_irqhandler(csp_qspi_t *ptQspiBase)
{
	volatile uint8_t byIntMask;
	uint8_t byCount = 0;
	byIntMask = csp_qspi_get_intmsk(QSPI);
	if( byIntMask & QSPI_TXE_INT )//发送fifo空中断（小于等于设置发送阈值就会产生空中断）
	{
		if( (ptQspiBase->CR0 & 0x20) && (g_tQspiTransmit.pbyTxData) )//要确保使能已打开（没使能，仅使能了中断也会进来的）
		{
			
			if(g_tQspiTransmit.byTxSize >= (16 - g_tQspiTransmit.byTxFifoLength))//剩余传输数据大于阈值
			{
				for(byCount = 0; byCount < g_tQspiTransmit.byTxFifoLength;byCount++)
				{
					csp_qspi_write_data(ptQspiBase,*g_tQspiTransmit.pbyTxData);			//send data
					g_tQspiTransmit.pbyTxData ++;
				}
				g_tQspiTransmit.byTxSize = g_tQspiTransmit.byTxSize - g_tQspiTransmit.byTxFifoLength;	
			}
			else//剩余传输数据小于阈值
			{
				for(byCount = 0; byCount < g_tQspiTransmit.byTxSize;byCount++)
				{
					csp_qspi_write_data(ptQspiBase,*g_tQspiTransmit.pbyTxData);			//send data
					g_tQspiTransmit.pbyTxData ++;
				}
				g_tQspiTransmit.byTxSize = 0;	
			}
			
			if (g_tQspiTransmit.byTxSize <= 0U) 
			{
				g_tQspiTransmit.byWriteable = 0U;//qspi free
				csp_qspi_set_int(ptQspiBase, QSPI_TXE_INT, false);			//disable fifo tx int
			}
		}
	}
	
	if(byIntMask & QSPI_RXF_INT)//接收fifo满中断（大于等于设置接收阈值就会产生满中断）
	{
		if(g_tQspiTransmit.pbyRxData)	
		{
			for(uint8_t byIdx = 0; byIdx < g_tQspiTransmit.byRxFifoLength; byIdx++)
			{
				*g_tQspiTransmit.pbyRxData = csp_qspi_read_data(ptQspiBase);			//receive data
				g_tQspiTransmit.pbyRxData++;
				g_tQspiTransmit.byRxSize--;
				if(g_tQspiTransmit.byRxSize == 0)
				{
					break;
				}
			}
			
			if(g_tQspiTransmit.byRxSize == 0)
			{
				g_tQspiTransmit.byReadable = 0U;
				csp_qspi_set_int(ptQspiBase, QSPI_RXF_INT, false);			//disable fifo rx int
			}
			
		}
	}
	
	if(byIntMask & QSPI_TXO_INT)//发送fifo溢出中断（发送fifo深度完全满，这里为16）
	{
		csi_qspi_clr_int(QSPI,QSPI_TXO_INT);
	}

	if(byIntMask & QSPI_RXU_INT)//接收fifo下溢中断（接收fifo没有数据去读时产生）
	{
		csi_qspi_clr_int(QSPI,QSPI_RXU_INT);
	}
	
	if(byIntMask & QSPI_RXO_INT)//接收fifo溢出中断（接收fifo深度完全满，这里为16）
	{
		csi_qspi_clr_int(QSPI,QSPI_RXO_INT);
	}
	
	if(byIntMask & QSPI_MST_INT)//多主机竞争中断
	{
		csi_qspi_clr_int(QSPI,QSPI_MST_INT);
	}

}