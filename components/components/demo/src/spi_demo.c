/***********************************************************************//** 
 * \file  SPI_demo.c
 * \brief  SPI示例代码 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version    <th>Author  <th>Description
 * <tr><td> 2020-11-24 <td>V0.0  <td>ZJY     <td>initial
 * <tr><td> 2021-07-02 <td>V0.1  <td>LQ      <td>modify
 * <tr><td> 2023-03-16 <td>V0.2  <td>WCH     <td>modify
 * <tr><td> 2023-9-25  <td>V0.3  <td>WCH     <td>code normalization
 * *********************************************************************
*/

/* include ----------------------------------------------------------------*/
#include "csi_drv.h"
#include "board_config.h"

#if (USE_SPI_CALLBACK == 0)	
	
static uint16_t hwSendLen = 0;	

/** \brief  spi0_int_handler: SPI中断服务函数
 * 
    \brief  SPI发生中断时会调用此函数，函数在interrupt.c里定义为弱(weak)属性，默认不做处理;用户用到中断
 * 			时，请重新定义此函数，在此函数中进行对应中断处理，也可直接在interrupt.c里的函数里进行处理。
 * 
 *  \param[in] none
 *  \return none
 */
ATTRIBUTE_ISR void spi0_int_handler(void) 
{
	
}
#endif

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
	
#if (USE_GUI == 0)
	//端口配置
	csi_gpio_set_mux(GPIOB, PB12, PB12_SPI0_NSS);
	csi_gpio_set_mux(GPIOB, PB13, PB13_SPI0_SCK);
	csi_gpio_set_mux(GPIOB, PB14, PB14_SPI0_MISO);
	csi_gpio_set_mux(GPIOB, PB15, PB15_SPI0_MOSI);
#endif
	
	tSpiConfig.eWorkMode 		= SPI_MODE_MASTER;			//SPI作为主机
	tSpiConfig.eCpolCpohMode 	= SPI_MODE_CPOL0_CPHA0; 	//SPI CPOL=0,CPOH=0
	tSpiConfig.eFrameLen 		= SPI_FRAME_LEN_8;          //SPI数据帧长度为8bit
	tSpiConfig.wSpiBaud 		= 1000000; 					//SPI通讯速率1Mbps			
	csi_spi_init(SPI0,&tSpiConfig);

	csi_spi_start(SPI0);
	
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

#if (USE_GUI == 0)
	//端口配置
	csi_gpio_set_mux(GPIOB, PB12, PB12_SPI0_NSS);
	csi_gpio_set_mux(GPIOB, PB13, PB13_SPI0_SCK);
	csi_gpio_set_mux(GPIOB, PB14, PB14_SPI0_MISO);
	csi_gpio_set_mux(GPIOB, PB15, PB15_SPI0_MOSI);
#endif
	
	tSpiConfig.eWorkMode 		= SPI_MODE_MASTER;			//SPI作为主机
	tSpiConfig.eCpolCpohMode 	= SPI_MODE_CPOL0_CPHA0; 	//SPI CPOL=0,CPOH=0
	tSpiConfig.eFrameLen 		= SPI_FRAME_LEN_8;          //SPI数据帧长度为8bit
	tSpiConfig.wSpiBaud 		= 1000000; 					//SPI通讯速率1Mbps			
	csi_spi_init(SPI0,&tSpiConfig);
	
	csi_spi_start(SPI0);
	while(1)
	{
		csi_spi_send(SPI0, bySendData, 8);
		//while(SPI_STATE_BUSY == csi_spi_get_state(SPI_SEND));
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
	csi_spi_config_t tSpiConfig;  //spi初始化参数配置结构体
	
	uint8_t bySendData[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	uint8_t byRecvData[16] = {0};

#if (USE_GUI == 0)
	//端口配置
	csi_gpio_set_mux(GPIOB, PB12, PB12_SPI0_NSS);
	csi_gpio_set_mux(GPIOB, PB13, PB13_SPI0_SCK);
	csi_gpio_set_mux(GPIOB, PB14, PB14_SPI0_MISO);
	csi_gpio_set_mux(GPIOB, PB15, PB15_SPI0_MOSI);
#endif

	tSpiConfig.eWorkMode 		= SPI_MODE_MASTER;			//SPI作为主机
	tSpiConfig.eCpolCpohMode 	= SPI_MODE_CPOL0_CPHA0; 	//SPI CPOL=0,CPOH=0
	tSpiConfig.eFrameLen 		= SPI_FRAME_LEN_8;          //SPI数据帧长度为8bit
	tSpiConfig.wSpiBaud 		= 1000000; 					//SPI通讯速率1Mbps				
	csi_spi_init(SPI0,&tSpiConfig);				

	csi_spi_start(SPI0);
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

#if (USE_GUI == 0)
	//端口配置
	csi_gpio_set_mux(GPIOB, PB12, PB12_SPI0_NSS);
	csi_gpio_set_mux(GPIOB, PB13, PB13_SPI0_SCK);
	csi_gpio_set_mux(GPIOB, PB14, PB14_SPI0_MISO);
	csi_gpio_set_mux(GPIOB, PB15, PB15_SPI0_MOSI);
#endif
	
	tSpiConfig.eWorkMode 		= SPI_MODE_SLAVE;			//SPI作为从机
	tSpiConfig.eCpolCpohMode 	= SPI_MODE_CPOL0_CPHA0; 	//SPI CPOL=0,CPOH=0
	tSpiConfig.eFrameLen 		= SPI_FRAME_LEN_8;          //SPI数据帧长度为8bit
	tSpiConfig.wSpiBaud 		= 0; 						//SPI通讯速率由主机决定			
	csi_spi_init(SPI0,&tSpiConfig);				

	csi_spi_start(SPI0);
	while(1)
	{	
		csi_spi_receive(SPI0,byRecvData,16);
		//while( SPI_STATE_BUSY==csi_spi_get_state(SPI_RECV) );
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

#if (USE_GUI == 0)
	//端口配置
	csi_gpio_set_mux(GPIOB, PB12, PB12_SPI0_NSS);
	csi_gpio_set_mux(GPIOB, PB13, PB13_SPI0_SCK);
	csi_gpio_set_mux(GPIOB, PB14, PB14_SPI0_MISO);
	csi_gpio_set_mux(GPIOB, PB15, PB15_SPI0_MOSI);
#endif
	
	tSpiConfig.eWorkMode 		= SPI_MODE_SLAVE;			//SPI作为从机
	tSpiConfig.eCpolCpohMode 	= SPI_MODE_CPOL0_CPHA0; 	//SPI CPOL=0,CPOH=0
	tSpiConfig.eFrameLen 		= SPI_FRAME_LEN_8;          //SPI数据帧长度为8bit
	tSpiConfig.wSpiBaud 		= 0; 						//SPI通讯速率由主机决定			
	csi_spi_init(SPI0,&tSpiConfig);				

	csi_spi_start(SPI0);
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
	csi_dma_ch_config_t tDmaConfig;	
	csi_etcb_config_t 	tEtcbConfig;	//ETCB 参数配置结构体
	csi_spi_config_t tSpiConfig; 		//SPI初始化参数配置结构体
	uint8_t bySdData[20]={0};

	for(uint8_t i = 0; i < sizeof(bySdData); i++)
	{
		bySdData[i] = i + 1;
	}
	
	//DMA para config
	tDmaConfig.eSrcLinc 	= DMA_ADDR_INC;			//低位传输原地址自增
	tDmaConfig.eSrcHinc 	= DMA_ADDR_CONSTANT;	//高位传输原地址固定不变
	tDmaConfig.eDetLinc 	= DMA_ADDR_CONSTANT;	//低位传输目标地址固定不变
	tDmaConfig.eDetHinc 	= DMA_ADDR_CONSTANT;	//高位传输目标地址固定不变
	tDmaConfig.eDataWidth 	= DMA_DSIZE_8_BITS;		//传输数据宽度8bit
	tDmaConfig.eReload 		= DMA_RELOAD_DISABLE;	//禁止自动重载
	tDmaConfig.eRunMode 	= DMA_RUN_ONCE;			//DMA服务模式(传输模式)，连续服务
	tDmaConfig.eTsizeMode  	= DMA_TSIZE_ONE_DSIZE;	//传输数据大小，一个 DSIZE , 即DSIZE定义大小
	tDmaConfig.eReqMode		= DMA_REQ_HARDWARE;		//DMA请求模式，硬件请求（硬件触发）
	csi_dma_sw_rst(DMA0);
	csi_dma_ch_init(DMA0, 0, &tDmaConfig);			//初始化DMA
	csp_dma_t *ptDmaChBase = (csp_dma_t *)DMA_REG_BASE(DMA0, 0);
	
	//ETCB para config
	tEtcbConfig.eChType  = ETCB_ONE_TRG_ONE_DMA;	//单个源触发单个目标，DMA方式
	tEtcbConfig.eSrcIp 	 = ETCB_SPI0_TXSRC;			//SPI0 TXSRC作为触发源
	tEtcbConfig.eDstIp 	 = ETCB_DMA0_CH0;			//ETCB DMA0通道0作为目标实际
	tEtcbConfig.eTrgMode = ETCB_HARDWARE_TRG;		//通道触发模式采样硬件触发
	csi_etcb_ch_init(ETCB_CH20, &tEtcbConfig);		//初始化ETCB，DMA ETCB CHANNEL > ETCB_CH19_ID

#if (USE_GUI == 0)
	//端口配置
	csi_gpio_set_mux(GPIOB, PB12, PB12_SPI0_NSS);
	csi_gpio_set_mux(GPIOB, PB13, PB13_SPI0_SCK);
	csi_gpio_set_mux(GPIOB, PB14, PB14_SPI0_MISO);
	csi_gpio_set_mux(GPIOB, PB15, PB15_SPI0_MOSI);
#endif
	
	//SPI para config
	tSpiConfig.eWorkMode 		= SPI_MODE_MASTER;			//SPI作为主机
	tSpiConfig.eCpolCpohMode 	= SPI_MODE_CPOL0_CPHA0; 	//SPI CPOL=0,CPOH=0
	tSpiConfig.eFrameLen 		= SPI_FRAME_LEN_8;          //SPI数据帧长度为8bit
	tSpiConfig.wSpiBaud 		= 1000000; 					//SPI通讯速率1Mbps				  
	csi_spi_init(SPI0,&tSpiConfig);							//初始化并启动spi
	
	csi_spi_start(SPI0);
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
	csi_dma_ch_config_t tDmaConfigSend;
	csi_dma_ch_config_t tDmaConfigRecv;
	csi_etcb_config_t 	tEtbConfigSend;		//ETCB 参数配置结构体
	csi_etcb_config_t 	tEtbConfigRecv;		//ETCB 参数配置结构体
	csi_spi_config_t 	tSpiConfig;			//spi初始化参数配置结构体
	
	uint8_t byDMAChnlSend = 0;
	uint8_t byDMAChnlRecv = 1;
	
	volatile uint8_t bySrcBuf[6] = {0x90, 0, 0, 0};//读flash id
	volatile uint8_t byDesBuf[2] = {0};
	
	for(uint8_t i = 4; i < sizeof(bySrcBuf); i++)
	{
		bySrcBuf[i] = i-3;
	}
	
	//send dma para config
	tDmaConfigSend.eSrcLinc 	= DMA_ADDR_INC;		      	//低位传输原地址自增
	tDmaConfigSend.eSrcHinc 	= DMA_ADDR_CONSTANT;		//高位传输原地址固定不变
	tDmaConfigSend.eDetLinc 	= DMA_ADDR_CONSTANT;		//低位传输目标地址固定不变
	tDmaConfigSend.eDetHinc 	= DMA_ADDR_CONSTANT;		//高位传输目标地址固定不变
	tDmaConfigSend.eDataWidth 	= DMA_DSIZE_8_BITS;		    //传输数据宽度8bit
	tDmaConfigSend.eReload 		= DMA_RELOAD_DISABLE;		//禁止自动重载
	tDmaConfigSend.eRunMode 	= DMA_RUN_ONCE;				//DMA服务模式(传输模式)，连续服务
	tDmaConfigSend.eTsizeMode  	= DMA_TSIZE_ONE_DSIZE;		//传输数据大小，一个 DSIZE , 即DSIZE定义大小
	tDmaConfigSend.eReqMode		= DMA_REQ_HARDWARE;			//DMA请求模式，硬件请求（硬件触发）
	
	//receive dma para config
	tDmaConfigRecv.eSrcLinc 	= DMA_ADDR_CONSTANT;		//低位传输原地址固定不变
	tDmaConfigRecv.eSrcHinc 	= DMA_ADDR_CONSTANT;		//高位传输原地址固定不变
	tDmaConfigRecv.eDetLinc 	= DMA_ADDR_INC;				//低位传输目标地址自增
	tDmaConfigRecv.eDetHinc 	= DMA_ADDR_CONSTANT;		//高位传输目标地址固定不变
	tDmaConfigRecv.eDataWidth  	= DMA_DSIZE_8_BITS;		    //传输数据宽度8bit
	tDmaConfigRecv.eReload 		= DMA_RELOAD_DISABLE;		//禁止自动重载
	tDmaConfigRecv.eRunMode  	= DMA_RUN_ONCE;				//DMA服务模式(传输模式)，连续服务
	tDmaConfigRecv.eTsizeMode  	= DMA_TSIZE_ONE_DSIZE;		//传输数据大小，一个 DSIZE , 即DSIZE定义大小
	tDmaConfigRecv.eReqMode		= DMA_REQ_HARDWARE;			//DMA请求模式，硬件请求
	
	//DMA init
	csi_dma_sw_rst(DMA0);
	csi_dma_ch_init(DMA0, byDMAChnlSend, &tDmaConfigSend);
	csi_dma_ch_init(DMA0, byDMAChnlRecv, &tDmaConfigRecv);
	csp_dma_t *ptDmaChBaseCh0 = (csp_dma_t *)DMA_REG_BASE(DMA0, 0);
	csp_dma_t *ptDmaChBaseCh1 = (csp_dma_t *)DMA_REG_BASE(DMA0, 1);
	
	//send etcb para config
	tEtbConfigSend.eChType = ETCB_ONE_TRG_ONE_DMA;			//单个源触发单个目标，DMA方式
	tEtbConfigSend.eSrcIp 	= ETCB_SPI0_TXSRC;				//SPI0 TXSRC作为触发源
	tEtbConfigSend.eDstIp 	= ETCB_DMA0_CH0;					//ETCB DMA0通道0作为目标实际
	tEtbConfigSend.eTrgMode = ETCB_HARDWARE_TRG;			//通道触发模式采样软件触发
	
	//receive etcb para config
	tEtbConfigRecv.eChType = ETCB_ONE_TRG_ONE_DMA;			//单个源触发单个目标，DMA方式
	tEtbConfigRecv.eSrcIp 	= ETCB_SPI0_RXSRC;				//SPI0 RXSRC作为触发源
	tEtbConfigRecv.eDstIp 	= ETCB_DMA0_CH1;					//ETCB DMA0通道0作为目标实际
	tEtbConfigRecv.eTrgMode = ETCB_HARDWARE_TRG;			//通道触发模式采样软件触发
	
	//ETCB init	
	csi_etcb_ch_init(ETCB_CH20, &tEtbConfigSend);			//初始化ETCB，DMA ETCB CHANNEL 大于 ETCB_CH19_ID
	csi_etcb_ch_init(ETCB_CH21, &tEtbConfigRecv);			//初始化ETCB，DMA ETCB CHANNEL 大于 ETCB_CH19_ID
	
	//SPI端口配置		
//	csi_gpio_set_mux(GPIOB, PB12, PB12_OUTPUT);                      //gpio_port as output
//	csi_gpio_output_mode(GPIOB, PB12, GPIO_PUSH_PULL);                //push pull mode
//	csi_gpio_set_high(GPIOB, PB12);		
	
#if (USE_GUI == 0)
	//端口配置
	csi_gpio_set_mux(GPIOB, PB12, PB12_SPI0_NSS);
	csi_gpio_set_mux(GPIOB, PB13, PB13_SPI0_SCK);
	csi_gpio_set_mux(GPIOB, PB14, PB14_SPI0_MISO);
	csi_gpio_set_mux(GPIOB, PB15, PB15_SPI0_MOSI);
#endif

	//spi para config
		tSpiConfig.eWorkMode 	= SPI_MODE_MASTER;			//SPI作为主机
	tSpiConfig.eCpolCpohMode 	= SPI_MODE_CPOL0_CPHA0; 	//SPI CPOL=0,CPOH=0
	tSpiConfig.eFrameLen 		= SPI_FRAME_LEN_8;          //SPI数据帧长度为8bit
	tSpiConfig.wSpiBaud 		= 1000000; 					//SPI通讯速率1Mbps				  
	csi_spi_init(SPI0,&tSpiConfig);							//初始化并启动spi

//	csi_spi_nss_low(SPI_NSS_PIN);
	csi_spi_recv_dma(SPI0, (void *)byDesBuf, sizeof(byDesBuf), DMA0, 1);
	csi_spi_send_dma(SPI0, (void *)bySrcBuf, sizeof(bySrcBuf), DMA0, 0);
	while(csp_dma_get_curr_htc(ptDmaChBaseCh0));		//等待直到dma发送完成
	while(csp_dma_get_curr_htc(ptDmaChBaseCh1));		//等待直到dma接收完成
	while( (SPI0->SR & SPI_BSY) );						//等到spi传输完成
//	csi_spi_nss_high(SPI_NSS_PIN);

	csi_spi_start(SPI0);
	while(1)
	{
//		csi_spi_nss_low(SPI_NSS_PIN);
		csi_spi_recv_dma(SPI0, (void *)byDesBuf, sizeof(byDesBuf), DMA0, 1);
		csi_spi_send_dma(SPI0, (void *)bySrcBuf, sizeof(bySrcBuf), DMA0, 0);
		while(csp_dma_get_curr_htc(ptDmaChBaseCh0));		//等待直到dma发送完成
		while(csp_dma_get_curr_htc(ptDmaChBaseCh1));		//等待直到dma接收完成
		while( (SPI0->SR & SPI_BSY) );						//等到spi传输完成
//		csi_spi_nss_high(SPI_NSS_PIN);
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
	
	csi_gpio_set_low(GPIOB, PB12);
	csi_spi_send_receive(SPI0,(void *)&byCmd, NULL, 1);	
	csi_gpio_set_high(GPIOB, PB12);
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

	csi_gpio_set_low(GPIOB, PB12);
	csi_spi_send_receive(SPI0, (void *)bySend, (void *)byRecv, 2);
	csi_gpio_set_high(GPIOB, PB12);
	
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
	csi_gpio_set_low(GPIOB, PB12);
	csi_spi_send_receive(SPI0, (void *)bySend, NULL, 2);
	csi_gpio_set_high(GPIOB, PB12);
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
	
	csi_gpio_set_low(GPIOB, PB12);
	csi_spi_send_receive(SPI0, (void *)bySend, NULL, 4);	//send read id cmd and three bytes addr	
	csi_spi_send_receive(SPI0, NULL, (void *)byRecv, 2);	//read id value; id value = 0xef14
	hwId = ((byRecv[0] << 8) |  byRecv[1]);
	csi_gpio_set_high(GPIOB, PB12);
   
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
	csi_gpio_set_low(GPIOB, PB12);
	csi_spi_send_receive(SPI0, (void *)&byCmd, NULL, 1);		//send chip erase cmd
	csi_gpio_set_high(GPIOB, PB12);
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
	csi_gpio_set_low(GPIOB, PB12);
	csi_spi_send_receive(SPI0, (void *)bySend, NULL, 4);		//send sector erase cmd and data three bytes addr 
	csi_gpio_set_high(GPIOB, PB12);
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
	
	csi_gpio_set_low(GPIOB, PB12);
	csi_spi_send_receive(SPI0, (void *)bySend, NULL, 4);		//send read bytes cmd and data three bytes addr 
	csi_spi_send_receive(SPI0, NULL,(void *)pbyBuf, hwNum);		//read hwNum bytes 
	csi_gpio_set_high(GPIOB, PB12);
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
	csi_gpio_set_low(GPIOB, PB12);
	csi_spi_send_receive(SPI0, (void *)bySend, NULL, 4);		//send write bytes cmd and data three bytes addr 
	csi_spi_send_receive(SPI0, (void *)pbyBuf, NULL, hwNum);	//write hwNum bytes
	csi_gpio_set_high(GPIOB, PB12);
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
	csi_spi_config_t tSpiConfig;  //spi初始化参数配置结构体
	
	for(uint8_t byIndex = 0;byIndex < 10; byIndex++)
	{
		s_byWrBuf[byIndex] = byIndex + 1;
	}
	
#if (USE_GUI == 0)	
	//NSS端口配置
	csi_gpio_set_mux(GPIOB, PB12, PB12_OUTPUT);                      //gpio_port as output
	csi_gpio_output_mode(GPIOB, PB12, GPIO_PUSH_PULL);                //push pull mode
	csi_gpio_set_high(GPIOB, PB12);									  //NSS init high			    
	//其他端口配置
	csi_gpio_set_mux(GPIOB, PB13, PB13_SPI0_SCK);
	csi_gpio_set_mux(GPIOB, PB14, PB14_SPI0_MISO);
	csi_gpio_set_mux(GPIOB, PB15, PB15_SPI0_MOSI);
#endif
	
	tSpiConfig.eWorkMode 		= SPI_MODE_MASTER;			//SPI作为主机
	tSpiConfig.eCpolCpohMode 	= SPI_MODE_CPOL0_CPHA0; 	//SPI CPOL=0,CPOH=0
	tSpiConfig.eFrameLen 		= SPI_FRAME_LEN_8;          //SPI数据帧长度为8bit
	tSpiConfig.wSpiBaud 		= 1000000; 					//SPI通讯速率1Mbps			  
	csi_spi_init(SPI0,&tSpiConfig);							

	csi_spi_start(SPI0);
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