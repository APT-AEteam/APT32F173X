/***********************************************************************//** 
 * \file  SPI_demo.c
 * \brief  SPI示例代码。
 * w25q16jvsiq(32 block == 512 sector == 2M Byte)
 * 1 page = 256 bytes
 * 1 sector = 16 page (4KB)
 * 1 block = 16 sector (64KB)
 * erasable(1sector,1/2block,1block,chip)
 * Device ID: (M7->M0 = 0xEF), (ID7->ID0 = 0x13)
 * Spi Mode:  MODE0 or MODE3
 * 
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version    <th>Author  <th>Description
 * <tr><td> 2020-11-24 <td>V0.0  <td>ZJY     <td>initial
 * <tr><td> 2021-07-02 <td>V0.1  <td>LQ      <td>modify
 * <tr><td> 2023-03-16 <td>V0.2  <td>WCH     <td>modify
 * *********************************************************************
*/

/* include ----------------------------------------------------------------*/
#include "spi.h"
#include "pin.h"
#include <iostring.h> 

#define SPI_NSS_PIN PB12   //define spi nss pin here.!!!!!only used in NSS SOFTWARE controled demo!!!!!users can change to other pins

/** \brief spi master send demo,polling mode
 * 	\brief spi 主机发送一串数据,TX使用轮询
 *  \param[in] none
 *  \return error code
 */
int spi_master_send_demo(void)
{
	int iRet = 0;
	uint8_t byData[8] = {1,2,3,4,5,6,7,8};
	csi_spi_config_t tSpiConfig;  //spi初始化参数配置结构体

	//端口配置
	csi_pin_set_mux(PB12,PB12_SPI0_NSS);
	csi_pin_set_mux(PB13,PB13_SPI0_SCK);
	csi_pin_set_mux(PB14,PB14_SPI0_MISO);
	csi_pin_set_mux(PB15,PB15_SPI0_MOSI);
	
	tSpiConfig.bySpiMode 			= SPI_MASTER;				//作为主机
	tSpiConfig.bySpiPolarityPhase 	= SPI_FORMAT_CPOL0_CPHA0; 	//clk空闲电平为0，相位为在第一个边沿采集数据
	tSpiConfig.bySpiFrameLen 		= SPI_FRAME_LEN_8;          //帧数据长度为8bit
	tSpiConfig.wSpiBaud 			= 1000000; 					//通讯速率1Mbps			
	tSpiConfig.byInt 				= (uint8_t)SPI_INTSRC_NONE;	//初始配置无中断
	tSpiConfig.byTxMode 			= SPI_TX_MODE_POLL;         //发送轮询模式	
	csi_spi_init(SPI0,&tSpiConfig);

	while(1)
	{
		csi_spi_send(SPI0,byData,8);
		mdelay(10);
	}
	return iRet;
}

/** \brief spi master send demo,interrupt(async) mode
 * 	\brief spi 主机发送一串数据,TX使用中断
 *  \param[in] none
 *  \return error code
 */
 int spi_master_send_int_demo(void)
{
	int iRet = 0;
	uint8_t bySendData[8] = {1,2,3,4,5,6,7,8};
	csi_spi_config_t tSpiConfig;  //spi初始化参数配置结构体
	
	//端口配置
	csi_pin_set_mux(PB12,PB12_SPI0_NSS);
	csi_pin_set_mux(PB13,PB13_SPI0_SCK);
	csi_pin_set_mux(PB14,PB14_SPI0_MISO);
	csi_pin_set_mux(PB15,PB15_SPI0_MOSI);
	
	tSpiConfig.bySpiMode 			= SPI_MASTER;				//作为主机
	tSpiConfig.bySpiPolarityPhase 	= SPI_FORMAT_CPOL0_CPHA0; 	//clk空闲电平为0，相位为在第一个边沿采集数据
	tSpiConfig.bySpiFrameLen 		= SPI_FRAME_LEN_8;          //帧数据长度为8bit
	tSpiConfig.wSpiBaud 			= 1000000; 					//通讯速率1Mbps			
	tSpiConfig.byInt 				= (uint8_t)SPI_INTSRC_TXIM;	//发送使用中断	  	
	tSpiConfig.byTxMode 			= SPI_TX_MODE_INT;          //发送使用中断模式
	csi_spi_init(SPI0,&tSpiConfig);

	while(1)
	{
		csi_spi_send(SPI0, bySendData, 8);
		while(SPI_STATE_BUSY == csi_spi_get_state(SPI_SEND));
		mdelay(10);		
	}
	return iRet;
}

/** \brief spi master send and receive demo,polling mode
 * 	\brief spi 主机收发一串数据，收发使用轮询
 *  \param[in] none
 *  \return error code
 */
int spi_master_send_receive_demo(void)
{
	int iRet = 0;
	uint8_t bySendData[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	uint8_t byRecvData[16] = {0};
	
	//端口配置
	csi_pin_set_mux(PB12,PB12_SPI0_NSS);
	csi_pin_set_mux(PB13,PB13_SPI0_SCK);
	csi_pin_set_mux(PB14,PB14_SPI0_MISO);
	csi_pin_set_mux(PB15,PB15_SPI0_MOSI);
	
	csi_spi_config_t tSpiConfig;  //spi初始化参数配置结构体
	tSpiConfig.bySpiMode 			= SPI_MASTER;				//作为主机
	tSpiConfig.bySpiPolarityPhase 	= SPI_FORMAT_CPOL0_CPHA0; 	//clk空闲电平为0，相位为在第一个边沿采集数据
	tSpiConfig.bySpiFrameLen		= SPI_FRAME_LEN_8;          //帧数据长度为8bit
	tSpiConfig.wSpiBaud 			= 1000000; 					//通讯速率1Mbps			
	tSpiConfig.byInt 				= (uint8_t)SPI_INTSRC_NONE;	//初始配置无中断
	tSpiConfig.byTxRxMode 			= SPI_TX_RX_MODE_POLL;      //收发使用轮询模式	
	csi_spi_init(SPI0,&tSpiConfig);				

	while(1)
	{
		csi_spi_send_receive(SPI0, bySendData, byRecvData, 16);
		mdelay(100);
	}	
	return iRet;
}

/*  \brief spi slave receive a bunch of data; interrupt(async) mode
 *  \brief spi 从机接收一串数据，RX使用中断
 *  \param[in] none
 *  \return error code
 */
int spi_slave_receive_int_demo(void)
{
	int iRet = 0;
	uint8_t byRecvData[16] = {};
	uint8_t byIndex = 0;
	csi_spi_config_t tSpiConfig;  //spi初始化参数配置结构体
	
	//端口配置
	csi_pin_set_mux(PB12,PB12_SPI0_NSS);
	csi_pin_set_mux(PB13,PB13_SPI0_SCK);
	csi_pin_set_mux(PB14,PB14_SPI0_MISO);
	csi_pin_set_mux(PB15,PB15_SPI0_MOSI);
	
	tSpiConfig.bySpiMode 			= SPI_SLAVE;				//作为从机	
	tSpiConfig.bySpiPolarityPhase 	= SPI_FORMAT_CPOL0_CPHA1; 	//clk空闲电平为0，相位为在第二个边沿采集数据
	tSpiConfig.bySpiFrameLen 		= SPI_FRAME_LEN_8;          //帧数据长度为8bit
	tSpiConfig.wSpiBaud 			= 0; 					    //作从机时，通讯速率取决于主机			
	tSpiConfig.byInt				= SPI_INTSRC_RXIM;			//初始配置接收中断	
	tSpiConfig.byRxMode 			= SPI_RX_MODE_INT;          //接收使用中断模式
	csi_spi_init(SPI0,&tSpiConfig);				

	while(1)
	{	
		csi_spi_receive(SPI0,byRecvData,16);
		while( SPI_STATE_BUSY==csi_spi_get_state(SPI_RECV) );
		my_printf("\nslave receive data is:");
		for(byIndex = 0;byIndex<16;byIndex++)
		{
			my_printf(" %d", byRecvData[byIndex]);
		}
	}
	return iRet;
}

/** \brief spi slave send and receive demo,polling mode
 * 	\brief spi 从机收发一串数据，收发使用轮询
 *  \param[in] none
 *  \return error code
 */
int spi_slave_send_receive_demo(void)//从机同步发，同步收
{
	int iRet = 0;
	uint8_t byReceData;
	csi_spi_config_t tSpiConfig;  //spi初始化参数配置结构体
	
	//端口配置
	csi_pin_set_mux(PB12,PB12_SPI0_NSS);
	csi_pin_set_mux(PB13,PB13_SPI0_SCK);
	csi_pin_set_mux(PB14,PB14_SPI0_MISO);
	csi_pin_set_mux(PB15,PB15_SPI0_MOSI);
	
	tSpiConfig.bySpiMode 			= SPI_SLAVE;				//作为从机
	tSpiConfig.bySpiPolarityPhase 	= SPI_FORMAT_CPOL0_CPHA0; 	//clk空闲电平为0，相位为在第一个边沿采集数据
	tSpiConfig.bySpiFrameLen 		= SPI_FRAME_LEN_8;          //帧数据长度为8bit
	tSpiConfig.wSpiBaud 			= 0; 					    //作从机时，通讯速率取决于主机			
	tSpiConfig.byInt 				= (uint8_t)SPI_INTSRC_NONE;	//初始配置无中断	
	csi_spi_init(SPI0,&tSpiConfig);				

	while(1)
	{
		if( (uint32_t)(SPI0->SR) & SPI_RNE )	
		{
			byReceData = csi_spi_receive_slave(SPI0);
			csi_spi_send_slave(SPI0, byReceData);
		}
		nop;
	}
	return iRet;
}

/** \brief spi DMA发送示例代码，使用DMA循环发送20字节数据
 *  \param[in] none
 *  \return error code
 */
int spi_etcb_dma_send_demo(void)
{
	int iRet = 0;
	uint8_t bySdData[20]={0};

	for(uint8_t i = 0; i < sizeof(bySdData); i++)
	{
		bySdData[i] = i + 1;
	}
	//DMA para config
	csi_dma_ch_config_t tDmaConfig;					//DMA 配置初始化
	tDmaConfig.bySrcLinc 	= DMA_ADDR_INC;			//低位传输原地址自增
	tDmaConfig.bySrcHinc 	= DMA_ADDR_CONSTANT;	//高位传输原地址固定不变
	tDmaConfig.byDetLinc 	= DMA_ADDR_CONSTANT;	//低位传输目标地址固定不变
	tDmaConfig.byDetHinc 	= DMA_ADDR_CONSTANT;	//高位传输目标地址固定不变
	tDmaConfig.byDataWidth 	= DMA_DSIZE_8_BITS;		//传输数据宽度8bit
	tDmaConfig.byReload 	= DMA_RELOAD_DISABLE;	//禁止自动重载
	tDmaConfig.byTransMode 	= DMA_TRANS_ONCE;		//DMA服务模式(传输模式)，连续服务
	tDmaConfig.byTsizeMode  = DMA_TSIZE_ONE_DSIZE;	//传输数据大小，一个 DSIZE , 即DSIZE定义大小
	tDmaConfig.byReqMode	= DMA_REQ_HARDWARE;		//DMA请求模式，硬件请求（硬件触发）
	tDmaConfig.wInt 		= DMA_INTSRC_NONE;		//不使用中断
	csi_dma_soft_rst(DMA0);
	csi_dma_ch_init(DMA0, 0, &tDmaConfig);			//初始化DMA
	csp_dma_t *ptDmaChBase = (csp_dma_t *)DMA_REG_BASE(DMA0, 0);
	
	//ETCB para config
	csi_etb_config_t 	tEtbConfig;					//ETB 参数配置结构体
	tEtbConfig.byChType  = ETB_ONE_TRG_ONE_DMA;		//单个源触发单个目标，DMA方式
	tEtbConfig.bySrcIp 	 = ETB_SPI0_TXSRC;			//SPI0 TXSRC作为触发源
	tEtbConfig.byDstIp 	 = ETB_DMA0_CH0;			//ETB DMA0通道0作为目标实际
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;		//通道触发模式采样硬件触发
	csi_etb_init();									//使能ETB模块
	csi_etb_ch_config(ETB_CH20, &tEtbConfig);		//初始化ETB，DMA ETB CHANNEL > ETB_CH19_ID
	
	//SPI端口配置	
	csi_pin_set_mux(PB12,PB12_SPI0_NSS);						    
	csi_pin_set_mux(PB13,PB13_SPI0_SCK);
	csi_pin_set_mux(PB14,PB14_SPI0_MISO);
	csi_pin_set_mux(PB15,PB15_SPI0_MOSI);
	
	//SPI para config
	csi_spi_config_t tSpiConfig; //spi初始化参数配置结构体
	tSpiConfig.bySpiMode 			= SPI_MASTER;				//作为主机
	tSpiConfig.bySpiPolarityPhase 	= SPI_FORMAT_CPOL0_CPHA0; 	//clk空闲电平为0，相位为在第一个边沿采集数据
	tSpiConfig.bySpiFrameLen	 	= SPI_FRAME_LEN_8;          //帧数据长度为8bit
	tSpiConfig.wSpiBaud 			= 1000000; 					//通讯速率1Mbps			
	tSpiConfig.byInt 				= (uint8_t)SPI_INTSRC_NONE;	//初始配置无中断	  
	csi_spi_init(SPI0,&tSpiConfig);						//初始化并启动spi
	
	while(1)
	{
		csi_spi_send_dma(SPI0, (void *)bySdData, sizeof(bySdData), DMA0, 0);
		while(csp_dma_get_curr_htc(ptDmaChBase));//等待直到dma传输完成
		mdelay(10);
	}
	return iRet;
}

/** \brief spi DMA发送接收示例代码，使用DMA循环发送20字节数据
 *  \brief 1、使用DMA向flash 0x1000地址写10字节数据
 *  \brief 2、使用DMA向flash 0x1000地址读10字节数据
 *	\param[in] none
 *  \return error code
 */
int spi_etcb_dma_send_receive_demo(void)
{	
	int iRet = 0;
	uint8_t byDMAChnlSend = 0;
	uint8_t byDMAChnlRecv = 1;
	
	volatile uint8_t bySrcBuf[6] = {0x90, 0, 0, 0};//读flash id
	volatile uint8_t byDesBuf[2] = {0};
	
	for(uint8_t i = 4; i < sizeof(bySrcBuf); i++)
	{
		bySrcBuf[i] = i-3;
	}
	
	//send dma para config
	csi_dma_ch_config_t tDmaConfigSend;
	tDmaConfigSend.bySrcLinc 	= DMA_ADDR_INC;		      	//低位传输原地址自增
	tDmaConfigSend.bySrcHinc 	= DMA_ADDR_CONSTANT;		//高位传输原地址固定不变
	tDmaConfigSend.byDetLinc 	= DMA_ADDR_CONSTANT;		//低位传输目标地址固定不变
	tDmaConfigSend.byDetHinc 	= DMA_ADDR_CONSTANT;		//高位传输目标地址固定不变
	tDmaConfigSend.byDataWidth 	= DMA_DSIZE_8_BITS;		    //传输数据宽度8bit
	tDmaConfigSend.byReload 	= DMA_RELOAD_DISABLE;		//禁止自动重载
	tDmaConfigSend.byTransMode 	= DMA_TRANS_ONCE;			//DMA服务模式(传输模式)，连续服务
	tDmaConfigSend.byTsizeMode  = DMA_TSIZE_ONE_DSIZE;		//传输数据大小，一个 DSIZE , 即DSIZE定义大小
	tDmaConfigSend.byReqMode	= DMA_REQ_HARDWARE;			//DMA请求模式，硬件请求（硬件触发）
	tDmaConfigSend.wInt 		= DMA_INTSRC_NONE;			//不使用中断
	
	//receive dma para config
	csi_dma_ch_config_t tDmaConfigRecv;
	tDmaConfigRecv.bySrcLinc 	= DMA_ADDR_CONSTANT;		//低位传输原地址固定不变
	tDmaConfigRecv.bySrcHinc 	= DMA_ADDR_CONSTANT;		//高位传输原地址固定不变
	tDmaConfigRecv.byDetLinc 	= DMA_ADDR_INC;				//低位传输目标地址自增
	tDmaConfigRecv.byDetHinc 	= DMA_ADDR_CONSTANT;		//高位传输目标地址固定不变
	tDmaConfigRecv.byDataWidth  = DMA_DSIZE_8_BITS;		    //传输数据宽度8bit
	tDmaConfigRecv.byReload 	= DMA_RELOAD_DISABLE;		//禁止自动重载
	tDmaConfigRecv.byTransMode  = DMA_TRANS_ONCE;			//DMA服务模式(传输模式)，连续服务
	tDmaConfigRecv.byTsizeMode  = DMA_TSIZE_ONE_DSIZE;		//传输数据大小，一个 DSIZE , 即DSIZE定义大小
	tDmaConfigRecv.byReqMode	= DMA_REQ_HARDWARE;			//DMA请求模式，硬件请求
	tDmaConfigRecv.wInt		    = DMA_INTSRC_NONE;			//不使用中断
	
	//DMA init
	csi_dma_soft_rst(DMA0);
	csi_dma_ch_init(DMA0, byDMAChnlSend, &tDmaConfigSend);
	csi_dma_ch_init(DMA0, byDMAChnlRecv, &tDmaConfigRecv);
	csp_dma_t *ptDmaChBaseCh0 = (csp_dma_t *)DMA_REG_BASE(DMA0, 0);
	csp_dma_t *ptDmaChBaseCh1 = (csp_dma_t *)DMA_REG_BASE(DMA0, 1);
	
	//send etcb para config
	csi_etb_config_t 	tEtbConfigSend;						//ETB 参数配置结构体
	tEtbConfigSend.byChType = ETB_ONE_TRG_ONE_DMA;			//单个源触发单个目标，DMA方式
	tEtbConfigSend.bySrcIp 	= ETB_SPI0_TXSRC;				//SPI0 TXSRC作为触发源
	tEtbConfigSend.byDstIp 	= ETB_DMA0_CH0;					//ETB DMA0通道0作为目标实际
	tEtbConfigSend.byTrgMode = ETB_HARDWARE_TRG;			//通道触发模式采样软件触发
	
	//receive etcb para config
	csi_etb_config_t 	tEtbConfigRecv;						//ETB 参数配置结构体
	tEtbConfigRecv.byChType = ETB_ONE_TRG_ONE_DMA;			//单个源触发单个目标，DMA方式
	tEtbConfigRecv.bySrcIp 	= ETB_SPI0_RXSRC;				//SPI0 RXSRC作为触发源
	tEtbConfigRecv.byDstIp 	= ETB_DMA0_CH1;					//ETB DMA0通道0作为目标实际
	tEtbConfigRecv.byTrgMode = ETB_HARDWARE_TRG;			//通道触发模式采样软件触发
	
	//ETCB init
	csi_etb_init();									    	//使能ETB模块
	csi_etb_ch_config(ETB_CH20, &tEtbConfigSend);			//初始化ETB，DMA ETB CHANNEL 大于 ETB_CH19_ID
	csi_etb_ch_config(ETB_CH21, &tEtbConfigRecv);			//初始化ETB，DMA ETB CHANNEL 大于 ETB_CH19_ID
	
	//SPI端口配置		
	csi_pin_set_mux(SPI_NSS_PIN, PIN_OUTPUT);                       //gpio_port as output
	csi_pin_output_mode(SPI_NSS_PIN, GPIO_PUSH_PULL);                //push pull mode
	csi_spi_nss_high(SPI_NSS_PIN);						    
	csi_pin_set_mux(PB13,PB13_SPI0_SCK);
	csi_pin_set_mux(PB14,PB14_SPI0_MISO);
	csi_pin_set_mux(PB15,PB15_SPI0_MOSI);
	
	//spi para config
	csi_spi_config_t tSpiConfig;//spi初始化参数配置结构体
	tSpiConfig.bySpiMode 			= SPI_MASTER;				//作为主机
	tSpiConfig.bySpiPolarityPhase 	= SPI_FORMAT_CPOL0_CPHA0; 	//clk空闲电平为0，相位为在第一个边沿采集数据
	tSpiConfig.bySpiFrameLen 		= SPI_FRAME_LEN_8;          //帧数据长度为8bit
	tSpiConfig.wSpiBaud 			= 1000000; 					//通讯速率1Mbps			
	tSpiConfig.byInt 				= (uint8_t)SPI_INTSRC_NONE; //初始配置无中断	  
	csi_spi_init(SPI0,&tSpiConfig);						//初始化并启动spi

	csi_spi_nss_low(SPI_NSS_PIN);
	csi_spi_recv_dma(SPI0, (void *)byDesBuf, sizeof(byDesBuf), DMA0, 1);
	csi_spi_send_dma(SPI0, (void *)bySrcBuf, sizeof(bySrcBuf), DMA0, 0);
	while(csp_dma_get_curr_htc(ptDmaChBaseCh0));		//等待直到dma发送完成
	while(csp_dma_get_curr_htc(ptDmaChBaseCh1));		//等待直到dma接收完成
	while( (SPI0->SR & SPI_BSY) );						//等到spi传输完成
	csi_spi_nss_high(SPI_NSS_PIN);

	while(1)
	{
		csi_spi_nss_low(SPI_NSS_PIN);
		csi_spi_recv_dma(SPI0, (void *)byDesBuf, sizeof(byDesBuf), DMA0, 1);
		csi_spi_send_dma(SPI0, (void *)bySrcBuf, sizeof(bySrcBuf), DMA0, 0);
		while(csp_dma_get_curr_htc(ptDmaChBaseCh0));		//等待直到dma发送完成
		while(csp_dma_get_curr_htc(ptDmaChBaseCh1));		//等待直到dma接收完成
		while( (SPI0->SR & SPI_BSY) );						//等到spi传输完成
		csi_spi_nss_high(SPI_NSS_PIN);
		mdelay(10);
	}
	return iRet;
}

//----------------------------------------------------------------------------------------------
//w25q16jvsiq(32 block == 512 sector == 2M Byte)
// 1 page = 256 bytes
// 1 sector = 16 page (4KB)
// 1 block = 16 sector (64KB)

//erasable(1sector,1/2block,1block,chip)

#define		PAGE_SIZE				256
//flash cmd
#define		WREN_CMD				0x06		//write enable
#define		WRDI_CMD				0x04		//write disable
#define		RDSR0_CMD				0x05		//Read Status0
#define		RDSR1_CMD				0x35		//Read Status1
#define		WRSR_CMD				0x01		//Write Status 
#define		READ_CMD				0x03		//ReadData
#define		FREAD_CMD				0x0B		//FastReadData  
#define		PGPRO_CMD				0x02		//PageProgram
#define		FPGPRO_CMD				0x32		//Fast PageProgram
#define		SECTORERASE_CMD			0x20		//setor erase
#define		BKERASE0_CMD			0xD8		//block erase0(64k)
#define		BKERASE1_CMD			0x52		//block erase1(32k)
#define		CHIPERASE_CMD			0xc7		//chip erase
#define		RDDEVICEID_CMD			0x90		//read chip ID ReadElectricManufacturerDeviceID 
#define		RDCHIPID_CMD			0x9F		//read chip ID
	

static uint8_t s_byWrBuf[100] = {26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11};
static uint8_t s_byRdBuf[100];
	
//flash status reg
//BIT7    6     5     4     3     2     1     0
//SPR0    SEC   TB   BP2    BP1  BP0   WEL   BUSY
//SPR0:Status Protect
//SEC: Sector/Block Protect
//TB:  TOP/BOTTOM Block Protect
//BP3, BP2,BP1,BP0:FLASH Block Protect
//WEL: Write Enable Latch
//BUSY:Busy flage（1 busy：0 idle）
//defualt:0x00

/** \brief flash write enable 
 * 	flash写使能，手动控制CS线
 *  \param[in] none
 *  \return none
 */
static void spi_flash_write_enable(void)
{
	uint8_t byCmd = WREN_CMD;		//write enable cmd = 0x06
	
	csi_spi_nss_low(SPI_NSS_PIN);
	csi_spi_send_receive(SPI0,(void *)&byCmd, NULL, 1);	
	csi_spi_nss_high(SPI_NSS_PIN);
}

/** \brief flash read status reg
 * 	读flash状态寄存器，手动控制CS线
//BIT7    6     5     4     3     2     1     0
//SPR0    SEC   TB   BP2    BP1  BP0   WEL   BUSY
//SPR0:Status Protect
//SEC: Sector/Block Protect
//TB:  TOP/BOTTOM Block Protect
//BP3, BP2,BP1,BP0:FLASH Block Protect
//WEL: Write Enable Latch
//BUSY:Busy flage（1 busy：0 idle）
//defualt:0x00
 *  \param[in] none
 *  \return flash status
 */

uint8_t spi_flash_read_status(void)
{
	uint8_t bySend[2] = {RDSR0_CMD, 0x00};		//read status cmd0 = 0x05
	uint8_t byRecv[2];

	csi_spi_nss_low(SPI_NSS_PIN);
	csi_spi_send_receive(SPI0, (void *)bySend, (void *)byRecv, 2);
	csi_spi_nss_high(SPI_NSS_PIN);
	
	return byRecv[1];
}

/** \brief flash wait idle
 *  \param[in] none
 *  \return none
 */
void spi_flash_wait_busy(void)
{
	while((spi_flash_read_status() & 0x01) == 0x01);  // 1: busy, 0: idle
}

/** \brief flash write status 
 *  写flash状态寄存器，手动控制CS线
 *  \param[in] byStatus: flash status of write
 *  \return none
 */
//SPR0,TB,BP2,BP1,BP0(bit 7,5,4,3,2) 
void spi_flash_write_Status(uint8_t byStatus)
{
	uint8_t bySend[2] = {WRSR_CMD, byStatus};		//write status cmd = 0x01

	spi_flash_write_enable();		//write enable cmd
	csi_spi_nss_low(SPI_NSS_PIN);
	csi_spi_send_receive(SPI0, (void *)bySend, NULL, 2);
	csi_spi_nss_high(SPI_NSS_PIN);
	spi_flash_wait_busy();
}

/** \brief flash read chip id
 * 	读flash id（不同flash型号的id不同），手动控制CS线
 *  \param[in] none
 *  \return chip id (chip id = 0xef13)
 */
uint32_t spi_flash_read_id(void)
{
	uint32_t hwId = 0x00;
	uint8_t bySend[6] = {RDDEVICEID_CMD, 0, 0, 0};
	uint8_t byRecv[2];
	
	csi_spi_nss_low(SPI_NSS_PIN); 
	csi_spi_send_receive(SPI0, (void *)bySend, NULL, 4);	//send read id cmd and three bytes addr	
	csi_spi_send_receive(SPI0, NULL, (void *)byRecv, 2);	//read id value; id value = 0xef14
	hwId = ((byRecv[0] << 8) |  byRecv[1]);
	csi_spi_nss_high(SPI_NSS_PIN);
   
	return hwId;
}

/** \brief flash chip erase
 * 	读flash id（不同flash型号的id不同），手动控制CS线	
 *  \param[in] none
 *  \return none
 */
void spi_flash_chip_erase(void)
{
	uint8_t byCmd = CHIPERASE_CMD;
	
	spi_flash_write_enable();		//write enable
	csi_spi_nss_low(SPI_NSS_PIN);
	csi_spi_send_receive(SPI0, (void *)&byCmd, NULL, 1);		//send chip erase cmd
	csi_spi_nss_high(SPI_NSS_PIN);
	spi_flash_wait_busy();
}

/** \brief flash sector erase (sector = 4k bytes)
 *  flash块擦除
 *  \param[in] wAddr: flash sector addr
 *  \return none
 */
void spi_flash_sector_erase(uint32_t wAddr)
{
	wAddr = wAddr & 0xfff000;
	uint8_t bySend[4] = {SECTORERASE_CMD, (wAddr >> 16), (wAddr >> 8), wAddr};
	
	spi_flash_write_enable();		//write enable
	csi_spi_nss_low(SPI_NSS_PIN);
	csi_spi_send_receive(SPI0, (void *)bySend, NULL, 4);		//send sector erase cmd and data three bytes addr 
	csi_spi_nss_high(SPI_NSS_PIN);
	spi_flash_wait_busy();
}

/** \brief flash read bytes
 *  读flash指定地址区域内的指定长度的数据
 *  \param[in] pbyBuf: pointer of read data buf 
 *  \param[in] wAddr: start addr of read data
 *  \param[in] hwNum: length of read data
 *  \return none
 */
void spi_flash_read_bytes(uint8_t *pbyBuf, uint32_t wAddr, uint16_t hwNum)
{
	uint8_t bySend[4] = {READ_CMD, (wAddr >> 16), (wAddr >> 8), wAddr};
	
	csi_spi_nss_low(SPI_NSS_PIN); 
	csi_spi_send_receive(SPI0, (void *)bySend, NULL, 4);		//send read bytes cmd and data three bytes addr 
	csi_spi_send_receive(SPI0, NULL,(void *)pbyBuf, hwNum);		//read hwNum bytes 
	csi_spi_nss_high(SPI_NSS_PIN);
}

/** \brief flash write bytes 
 * 	写flash指定地址区域内的指定长度的数据
 *  \param[in] pbyBuf: pointer of write data buf 
 *  \param[in] wAddr: start addr of write data
 *  \param[in] hwNum: length of write data
 *  \return none
 */
void spi_flash_write_bytes(uint8_t *pbyBuf, uint32_t wAddr, uint16_t hwNum)
{
	uint8_t bySend[4] = {PGPRO_CMD, (wAddr >> 16), (wAddr >> 8), wAddr};

	spi_flash_write_enable();		//write enable
	csi_spi_nss_low(SPI_NSS_PIN);
	csi_spi_send_receive(SPI0, (void *)bySend, NULL, 4);		//send write bytes cmd and data three bytes addr 
	csi_spi_send_receive(SPI0, (void *)pbyBuf, NULL, hwNum);	//write hwNum bytes
	csi_spi_nss_high(SPI_NSS_PIN);
	spi_flash_wait_busy();
}

/** \brief spi操作flash示例代码
 * 1、读取flash ID，ID如果匹配，则开始读写测试
 * 2、写数据前先擦除0x1000地址，然后再读回来
 * 3、往0x1000地址连续写入100字节数据，然后再读回来。
 *  \param[in] none
 *  \return error code
 */
int spi_flash_read_write_demo(void)
{
	int iRet = 0;	
	
	for(uint8_t byIndex = 0;byIndex < 10; byIndex++)
	{
		s_byWrBuf[byIndex] = byIndex + 1;
	}
	
	//主机端口配置
	csi_pin_set_mux(SPI_NSS_PIN, PIN_OUTPUT);                       //gpio_port as output
	csi_pin_output_mode(SPI_NSS_PIN, GPIO_PUSH_PULL);                //push pull mode
	csi_spi_nss_high(SPI_NSS_PIN);									  //NSS init high			    
	csi_pin_set_mux(PB13,PB13_SPI0_SCK);
	csi_pin_set_mux(PB14,PB14_SPI0_MISO);
	csi_pin_set_mux(PB15,PB15_SPI0_MOSI);
	
	csi_spi_config_t tSpiConfig;  //spi初始化参数配置结构体
	tSpiConfig.bySpiMode 			= SPI_MASTER;				//作为主机
	tSpiConfig.bySpiPolarityPhase 	= SPI_FORMAT_CPOL0_CPHA0; 	//clk空闲电平为0，相位为在第一个边沿采集数据
	tSpiConfig.bySpiFrameLen 		= SPI_FRAME_LEN_8;          //帧数据长度为8bit
	tSpiConfig.wSpiBaud 			= 1000000; 					//通讯速率1Mbps			
	tSpiConfig.byInt 				= (uint8_t)SPI_INTSRC_NONE;	//初始配置无中断	  
	csi_spi_init(SPI0,&tSpiConfig);							

	while(1)
	{
		iRet = spi_flash_read_id();
		if(iRet == 0xef13)	
		{
			spi_flash_sector_erase(0x1000);							//erase sector 1; start addr = 0x1000
			spi_flash_read_bytes((uint8_t *)s_byRdBuf, 0x1000, 100);	//read data = 0xff
			spi_flash_write_bytes(s_byWrBuf,0x1000,100);				//write 100 bytes 
			spi_flash_read_bytes((uint8_t *)s_byRdBuf, 0x1000, 100);	//read 100 bytes, read bytes = write bytes
		}
		mdelay(10);
	}
	return iRet;
}

/** \brief spi interrupt handle weak function
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \return none
 */ 
__attribute__((weak)) void spi_irqhandler(csp_spi_t *ptSpiBase)
{	
	uint32_t wStatus = csp_spi_get_isr(ptSpiBase);
	volatile uint8_t receive_data[4];
	//fifo rx 
	if(wStatus & SPI_RXIM_INT)
	{
		//for reference
		if(0==g_tSpiTransmit.byWorkMode)//主机
		{
			apt_spi_intr_recv_data(ptSpiBase);//when use"csi_spi_send_receive_async"function need open
		}
		else//从机
		{
			for(uint8_t byIdx = 0; byIdx < g_tSpiTransmit.byRxFifoLength; byIdx++)
			{
				receive_data[byIdx] = csi_spi_receive_slave(ptSpiBase);
				csi_spi_send_slave(ptSpiBase, receive_data[byIdx]);
			}
		}
	}
	//fifo tx 
	if(wStatus & SPI_TXIM_INT)		
	{
		//for reference
		apt_spi_intr_send_data(ptSpiBase);
	}
	
	//fifo overflow
	if(wStatus & SPI_ROTIM_INT)
	{	
		//for reference
		csp_spi_softreset(ptSpiBase,SPI_RXFIFO_RST);
		csp_spi_clr_isr(ptSpiBase, SPI_ROTIM_INT);
	}
	
	//fifo rx timeout
	if(wStatus & SPI_RTIM_INT)		
	{	
		//for reference
		csp_spi_clr_isr(ptSpiBase, SPI_RTIM_INT);
		
		for(uint8_t byIdx = 0; byIdx < g_tSpiTransmit.byRxFifoLength-1; byIdx++)
		{
			
			if(csp_spi_get_sr(ptSpiBase) & SPI_RNE)
			{
				*g_tSpiTransmit.pbyRxData = csp_spi_get_data(ptSpiBase);		//receive data
				g_tSpiTransmit.pbyRxData++;
			}
			else
			{
				break;
			}
			
		}		
		g_tSpiTransmit.byRxSize = 0;
		g_tSpiTransmit.byReadable = SPI_STATE_IDLE;
		csp_spi_set_int(ptSpiBase, SPI_RXIM_INT | SPI_RTIM_INT, false);			//disable fifo rx int
	}
}