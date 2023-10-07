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

/* externs function--------------------------------------------------------*/
/* Private function--------------------------------------------------------*/
static void spi_flash_write_enable(void);
static uint8_t spi_flash_read_status(void);
static void spi_flash_wait_busy(void);
static void spi_flash_write_Status(uint8_t byStatus);
static uint32_t spi_flash_read_id(void);
static void spi_flash_chip_erase(void);
static void spi_flash_sector_erase(uint32_t wAddr);
static void spi_flash_read_bytes(uint8_t *pbyBuf, uint32_t wAddr, uint16_t hwNum);
static void spi_flash_write_bytes(uint8_t *pbyBuf, uint32_t wAddr, uint16_t hwNum);
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/
static uint8_t s_bySendBuf[16]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};	
static uint8_t s_byRecvBuf[16]={0};	

#if (USE_SPI_CALLBACK == 0)	
static uint16_t hwSendLen = 0;

/** \brief  spi0_int_handler: SPI0中断服务函数
 * 
    \brief  SPI0发生中断时会调用此函数，函数在interrupt.c里定义为弱(weak)属性，默认不做处理;用户用到中断
 * 			时，请重新定义此函数，在此函数中进行对应中断处理，也可直接在interrupt.c里的函数里进行处理。
 * 
 *  \param[in] none
 *  \return none
 */
ATTRIBUTE_ISR void spi0_int_handler(void) 
{
	//TXFIFO中断 
	if(csp_spi_get_isr(SPI0) & SPI_INTSRC_TXFIFO)		
	{
		//发送16字节数据，发送完毕关闭中断(停止发送)
		if(hwSendLen < 16)
			csp_spi_set_data(SPI0, s_bySendBuf[hwSendLen++]);
		else
		{
			hwSendLen = 0;
			csp_spi_int_disable(SPI0, SPI_INT_TXFIFO);		//关闭txfifo/中断	
		}
	}
	
	//RXFIFO中断 
	if(csp_spi_get_isr(SPI0) & SPI_INTSRC_RXFIFO)
	{
		//添加用户处理
		s_byRecvBuf[0] = csp_spi_get_data(SPI0);			//接收数据,RXFIFO中断状态不需要专门清除，读数据时自动清除
	}
	
	//RXFIFO溢出中断 
	if(csp_spi_get_isr(SPI0) & SPI_INTSRC_RX_OV)
	{
		csi_spi_clr_isr(SPI0, SPI_INTSRC_RX_OV);
	}
	
	//RXFIFO接收超时中断 
	if(csp_spi_get_isr(SPI0) & SPI_INTSRC_RXTO)
	{
		csi_spi_clr_isr(SPI0, SPI_INTSRC_RXTO);
	}
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
	csi_spi_config_t tSpiCfg;  //spi初始化参数配置结构体
	
#if (USE_GUI == 0)
	//端口配置
	csi_gpio_set_mux(GPIOB, PB12, PB12_SPI0_NSS);
	csi_gpio_set_mux(GPIOB, PB13, PB13_SPI0_SCK);
	csi_gpio_set_mux(GPIOB, PB14, PB14_SPI0_MISO);
	csi_gpio_set_mux(GPIOB, PB15, PB15_SPI0_MOSI);
#endif
	
	tSpiCfg.eMode 		= SPI_MODE_MASTER;	//SPI作为主机
	tSpiCfg.eWorkMode 	= SPI_WORK_MODE_0; 	//SPI Mode0:CPOL=0,CPOH=0
	tSpiCfg.eDataLen 	= SPI_DATA_LEN_8;  	//SPI数据帧长度为8bit
	tSpiCfg.wSpiBaud 	= 1000000; 			//SPI通讯速率1Mbps			
	csi_spi_init(SPI0,&tSpiCfg);

	csi_spi_start(SPI0);
	
	while(1)
	{
		csi_spi_send(SPI0, s_bySendBuf, 16);
		mdelay(10);
	}
	return iRet;
}

/** \brief spi master send demo,interrupt mode
 * 	\brief spi 主机发送一串数据,TX使用中断
 *  \param[in] none
 *  \return error code
 */
 int spi_master_send_int_demo(void)
{
	int iRet = 0;
	csi_spi_config_t tSpiCfg;  //spi初始化参数配置结构体

#if (USE_GUI == 0)
	//端口配置
	csi_gpio_set_mux(GPIOB, PB12, PB12_SPI0_NSS);
	csi_gpio_set_mux(GPIOB, PB13, PB13_SPI0_SCK);
	csi_gpio_set_mux(GPIOB, PB14, PB14_SPI0_MISO);
	csi_gpio_set_mux(GPIOB, PB15, PB15_SPI0_MOSI);
#endif
	
	tSpiCfg.eMode 		= SPI_MODE_MASTER;	//SPI作为主机
	tSpiCfg.eWorkMode 	= SPI_WORK_MODE_0; 	//SPI Mode0:CPOL=0,CPOH=0
	tSpiCfg.eDataLen 	= SPI_DATA_LEN_8;  	//SPI数据帧长度为8bit
	tSpiCfg.wSpiBaud 	= 1000000; 			//SPI通讯速率1Mbps			
	csi_spi_init(SPI0,&tSpiCfg);
	
	csi_spi_int_enable(SPI0, SPI_INTSRC_TXFIFO);
	csi_spi_start(SPI0);
	
	while(1)
	{
		nop;		
	}
	return iRet;
}

/*  \brief spi slave receive a bunch of data; interrupt mode
 *  \brief spi 从机接收一串数据，RX使用中断
 *  \param[in] none
 *  \return error code
 */
int spi_slave_receive_int_demo(void)
{
	int iRet = 0;
	csi_spi_config_t tSpiCfg;  //spi初始化参数配置结构体

#if (USE_GUI == 0)
	//端口配置
	csi_gpio_set_mux(GPIOB, PB12, PB12_SPI0_NSS);
	csi_gpio_set_mux(GPIOB, PB13, PB13_SPI0_SCK);
	csi_gpio_set_mux(GPIOB, PB14, PB14_SPI0_MISO);
	csi_gpio_set_mux(GPIOB, PB15, PB15_SPI0_MOSI);
#endif
	
	tSpiCfg.eMode 		= SPI_MODE_SLAVE;		//SPI作为从机
	tSpiCfg.eWorkMode 	= SPI_WORK_MODE_0; 		//SPI Mode0:CPOL=0,CPOH=0
	tSpiCfg.eDataLen 	= SPI_DATA_LEN_8;  		//SPI数据帧长度为8bit
	tSpiCfg.eRxFifoTrg	= SPI_RXFIFOTRG_ONE;	//SPI接收FIFO的中断触发点设置为1，即RXFIFO接收到1个数据,就会触发RXFIFO中断；支持三种配置(1/2/4)		
	csi_spi_init(SPI0,&tSpiCfg);				

	csi_spi_int_enable(SPI0, SPI_INTSRC_RXFIFO);
	csi_spi_start(SPI0);
	
	while(1)
	{	
		nop;
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
	csi_spi_config_t tSpiCfg;  //spi初始化参数配置结构体

#if (USE_GUI == 0)
	//端口配置
	csi_gpio_set_mux(GPIOB, PB12, PB12_SPI0_NSS);
	csi_gpio_set_mux(GPIOB, PB13, PB13_SPI0_SCK);
	csi_gpio_set_mux(GPIOB, PB14, PB14_SPI0_MISO);
	csi_gpio_set_mux(GPIOB, PB15, PB15_SPI0_MOSI);
#endif

	tSpiCfg.eMode 		= SPI_MODE_MASTER;	//SPI作为主机
	tSpiCfg.eWorkMode 	= SPI_WORK_MODE_0; 	//SPI Mode0:CPOL=0,CPOH=0
	tSpiCfg.eDataLen 	= SPI_DATA_LEN_8;  	//SPI数据帧长度为8bit
	tSpiCfg.wSpiBaud 	= 1000000; 			//SPI通讯速率1Mbps				
	csi_spi_init(SPI0,&tSpiCfg);				

	csi_spi_start(SPI0);
	while(1)
	{
		csi_spi_send_receive(SPI0, s_bySendBuf, s_byRecvBuf, 16);
		mdelay(10);
	}	
	return iRet;
}

 /** \brief	spi_send_dma_demo: spi通过DMA发送数据demo
 *  \brief	spi DMA发送数据，需使用ETCB模块，配置对应触发源和触发目标
 * 
 *  \param[in] none
 *  \return error code
 */
int spi_send_dma_demo(void)
{	
	int iRet = 0;
	csi_dma_ch_config_t tDmaSendCfg;				//DMA参数配置结构体
	csi_etcb_config_t 	tEtcbSendCfg;				//ETCB参数配置结构体
	csi_spi_config_t 	tSpiCfg;					//SPI初始化参数配置结构体
	
#if (USE_GUI == 0)
	//SPI端口配置
	csi_gpio_set_mux(GPIOB, PB12, PB12_SPI0_NSS);
	csi_gpio_set_mux(GPIOB, PB13, PB13_SPI0_SCK);
	csi_gpio_set_mux(GPIOB, PB14, PB14_SPI0_MISO);
	csi_gpio_set_mux(GPIOB, PB15, PB15_SPI0_MOSI);
#endif
	//SPI参数配置
	tSpiCfg.eMode 		  = SPI_MODE_MASTER;		//SPI作为主机
	tSpiCfg.eWorkMode 	  = SPI_WORK_MODE_0; 		//SPI Mode0:CPOL=0,CPOH=0
	tSpiCfg.eDataLen 	  = SPI_DATA_LEN_8;  		//SPI数据帧长度为8bit
	tSpiCfg.wSpiBaud 	  = 1000000; 				//SPI通讯速率1Mbps				  
	csi_spi_init(SPI0,&tSpiCfg);					//初始化并启动spi

	csi_spi_set_send_dma(SPI0, SPI_DMA_TXFIFO_NFULL);//设置send DMA工作模式并使能
	csi_spi_start(SPI0);
	
	//DMA发送通道参数配置
	tDmaSendCfg.eSrcLinc 	= DMA_ADDR_CONSTANT;	  //低位传输原地址固定不变
	tDmaSendCfg.eSrcHinc 	= DMA_ADDR_INC;			  //高位传输原地址自增
	tDmaSendCfg.eDetLinc 	= DMA_ADDR_CONSTANT;	  //低位传输目标地址固定不变
	tDmaSendCfg.eDetHinc 	= DMA_ADDR_CONSTANT;	  //高位传输目标地址固定不变
	tDmaSendCfg.eDataWidth 	= DMA_DSIZE_8_BITS;		  //传输数据宽度8bit
	tDmaSendCfg.eReload 	= DMA_RELOAD_DISABLE;	  //禁止自动重载
	tDmaSendCfg.eRunMode 	= DMA_RUN_ONCE;			  //DMA服务模式(传输模式)，连续服务
	tDmaSendCfg.eTsizeMode  = DMA_TSIZE_ONE_DSIZE;	  //传输数据大小，一个 DSIZE , 即DSIZE定义大小
	tDmaSendCfg.eReqMode	= DMA_REQ_HARDWARE;		  //DMA请求模式，硬件请求（硬件触发）
	csi_dma_ch_init(DMA0, DMA_CH1, &tDmaSendCfg);

	csi_dma_int_enable(DMA0, DMA_CH0,DMA_INTSRC_TCIT);//使用TCIT中断

	//ETCB发送通道参数配置
	tEtcbSendCfg.eChType 	= ETCB_ONE_TRG_ONE_DMA;	  //单个源触发单个目标，DMA方式
	tEtcbSendCfg.eSrcIp   	= ETCB_SPI0_TXSRC;		  //SPI0 TXSRC作为触发源
	tEtcbSendCfg.eDstIp   	= ETCB_DMA0_CH1;		  //ETCB DMA0通道1作为目标实际
	tEtcbSendCfg.eTrgMode 	= ETCB_HARDWARE_TRG;	  //通道触发模式采样软件触发
	iRet = csi_etcb_ch_init(ETCB_CH20, &tEtcbSendCfg);//初始化ETCB，DMA ETCB CHANNEL 大于 ETCB_CH19_ID
	if(iRet < CSI_OK)
		return CSI_ERROR;

	while(1)
	{
		csi_spi_send_dma(SPI0, DMA0, DMA_CH1, (void *)s_bySendBuf, sizeof(s_bySendBuf));	
		mdelay(50);
		
		if(csi_dma_get_msg(DMA0, DMA_CH1, ENABLE))				//获取发送完成消息，并清除消息
		{
			//添加用户代码
			nop;
		}
	}
	return iRet;
}

/** \brief	spi_receive_dma_demo: spi通过DMA接收数据demo
 *  \brief	spi DMA接收数据，需使用ETCB模块，配置对应触发源和触发目标
 * 
 *  \param[in] none
 *  \return error code
 */
int spi_receive_dma_demo(void)
{
	int iRet = 0;
	csi_dma_ch_config_t tDmaRecvCfg;				//DMA参数配置结构体
	csi_etcb_config_t 	tEtcbRecvCfg;				//ETCB参数配置结构体
	csi_spi_config_t 	tSpiCfg;					//SPI初始化参数配置结构体
	
#if (USE_GUI == 0)
	//SPI端口配置
	csi_gpio_set_mux(GPIOB, PB12, PB12_SPI0_NSS);
	csi_gpio_set_mux(GPIOB, PB13, PB13_SPI0_SCK);
	csi_gpio_set_mux(GPIOB, PB14, PB14_SPI0_MISO);
	csi_gpio_set_mux(GPIOB, PB15, PB15_SPI0_MOSI);
#endif
	//SPI参数配置
	tSpiCfg.eMode 		  = SPI_MODE_SLAVE;			//SPI作为从机
	tSpiCfg.eWorkMode 	  = SPI_WORK_MODE_0; 		//SPI Mode0:CPOL=0,CPOH=0
	tSpiCfg.eDataLen 	  = SPI_DATA_LEN_8;  		//SPI数据帧长度为8bit			  
	csi_spi_init(SPI0,&tSpiCfg);					//初始化并启动spi
	
	csi_spi_set_receive_dma(SPI0, SPI_DMA_RXFIFO_NSPACE);//设置receive DMA工作模式并使能	
	csi_spi_start(SPI0);
	
	//DMA参数配置
	tDmaRecvCfg.eSrcLinc 	= DMA_ADDR_CONSTANT;		//低位传输原地址固定不变
	tDmaRecvCfg.eSrcHinc 	= DMA_ADDR_CONSTANT;		//高位传输原地址固定不变
	tDmaRecvCfg.eDetLinc 	= DMA_ADDR_CONSTANT;		//低位传输目标地址固定不变
	tDmaRecvCfg.eDetHinc 	= DMA_ADDR_INC;				//高位传输目标地址自增
	tDmaRecvCfg.eDataWidth  = DMA_DSIZE_8_BITS;		    //传输数据宽度8bit
	tDmaRecvCfg.eReload 	= DMA_RELOAD_DISABLE;		//禁止自动重载
	tDmaRecvCfg.eRunMode  	= DMA_RUN_ONCE;				//DMA服务模式(传输模式)，连续服务
	tDmaRecvCfg.eTsizeMode  = DMA_TSIZE_ONE_DSIZE;		//传输数据大小，一个 DSIZE , 即DSIZE定义大小
	tDmaRecvCfg.eReqMode	= DMA_REQ_HARDWARE;			//DMA请求模式，硬件请求
	csi_dma_ch_init(DMA0, DMA_CH0, &tDmaRecvCfg);		//初始化DMA,选择CH0
	
	csi_dma_int_enable(DMA0, DMA_CH0, DMA_INTSRC_TCIT);	//使用TCIT中断
	
	//ETCB 配置初始化
	tEtcbRecvCfg.eChType 	= ETCB_ONE_TRG_ONE_DMA;		//单个源触发单个目标，DMA方式
	tEtcbRecvCfg.eSrcIp 	= ETCB_SPI0_RXSRC;			//SPI0 RXSRC作为触发源
	tEtcbRecvCfg.eDstIp 	= ETCB_DMA0_CH0;			//ETB DMA0通道0作为目标实际
	tEtcbRecvCfg.eTrgMode 	= ETCB_HARDWARE_TRG;		//通道触发模式采样软件触发
	iRet = csi_etcb_ch_init(ETCB_CH21, &tEtcbRecvCfg);	//初始化ETCB，DMA ETCB CHANNEL > ETCB_CH19
	if(iRet < CSI_OK)
		return CSI_ERROR;
		
	csi_spi_recv_dma(SPI0, DMA0, DMA_CH0, (void*)s_byRecvBuf, 16);
	
	while(1)
	{
		if(csi_dma_get_msg(DMA0, DMA_CH0, ENABLE))		//获取接收完成消息，并清除消息
		{
			//添加用户代码
			nop;
		}							
	}
	return iRet;
}

/** \brief spi读flash示例代码
 * 1、读取flash ID，ID如果匹配，则开始读测试
 * 2、在0x1000地址读16字节数据
 *  \param[in] none
 *  \return error code
 */
int spi_flash_read_demo(void)
{
	int iRet = 0;	
	csi_spi_config_t tSpiCfg;  //spi初始化参数配置结构体
	
#if (USE_GUI == 0)	
	//NSS端口配置（使用PB12手动控制）
	csi_gpio_set_mux(GPIOB, PB12, PB12_OUTPUT);         //PB12设置为输出模式
	csi_gpio_output_mode(GPIOB, PB12, GPIO_PUSH_PULL);  //PB12设置为推挽输出
	csi_gpio_set_high(GPIOB, PB12);						//PB12设输出高		    
	//其他端口配置
	csi_gpio_set_mux(GPIOB, PB13, PB13_SPI0_SCK);
	csi_gpio_set_mux(GPIOB, PB14, PB14_SPI0_MISO);
	csi_gpio_set_mux(GPIOB, PB15, PB15_SPI0_MOSI);
#endif
	
	tSpiCfg.eMode 		= SPI_MODE_MASTER;				//SPI作为主机
	tSpiCfg.eWorkMode 	= SPI_WORK_MODE_0; 				//SPI Mode0:CPOL=0,CPOH=0
	tSpiCfg.eDataLen 	= SPI_DATA_LEN_8;  				//SPI数据帧长度为8bit
	tSpiCfg.wSpiBaud 	= 1000000; 						//SPI通讯速率1Mbps			  
	csi_spi_init(SPI0,&tSpiCfg);							

	csi_spi_start(SPI0);
	while(1)
	{
		iRet = spi_flash_read_id();
		if(iRet == 0xef14)	
		{
			spi_flash_read_bytes((uint8_t *)s_byRecvBuf, 0x1000, 16);//在0x1000地址读16字节数据
		}
		mdelay(10);
	}
	return iRet;
}

/** \brief spi写flash示例代码
 * 1、读取flash ID，ID如果匹配，则开始写测试
 * 2、写数据前先擦除0x1000地址，然后再往0x1000地址连续写入16字节数据
 *  \param[in] none
 *  \return error code
 */
int spi_flash_write_demo(void)
{
	int iRet = 0;	
	csi_spi_config_t tSpiCfg;  //spi初始化参数配置结构体
	
#if (USE_GUI == 0)	
	//NSS端口配置（使用PB12手动控制）
	csi_gpio_set_mux(GPIOB, PB12, PB12_OUTPUT);         //PB12设置为输出模式
	csi_gpio_output_mode(GPIOB, PB12, GPIO_PUSH_PULL);  //PB12设置为推挽输出
	csi_gpio_set_high(GPIOB, PB12);						//PB12设输出高		    
	//其他端口配置
	csi_gpio_set_mux(GPIOB, PB13, PB13_SPI0_SCK);
	csi_gpio_set_mux(GPIOB, PB14, PB14_SPI0_MISO);
	csi_gpio_set_mux(GPIOB, PB15, PB15_SPI0_MOSI);
#endif
	
	tSpiCfg.eMode 		= SPI_MODE_MASTER;				//SPI作为主机
	tSpiCfg.eWorkMode 	= SPI_WORK_MODE_0; 				//SPI Mode0:CPOL=0,CPOH=0
	tSpiCfg.eDataLen 	= SPI_DATA_LEN_8;  				//SPI数据帧长度为8bit
	tSpiCfg.wSpiBaud 	= 1000000; 						//SPI通讯速率1Mbps			  
	csi_spi_init(SPI0,&tSpiCfg);							

	csi_spi_start(SPI0);
	while(1)
	{
		iRet = spi_flash_read_id();
		if(iRet == 0xef14)	
		{
			spi_flash_sector_erase(0x1000);									//擦除0x1000地址
			spi_flash_write_bytes(s_bySendBuf, 0x1000, sizeof(s_bySendBuf));//往0x1000地址写16字节数据
		}
		mdelay(1);
	}
	return iRet;
}

//--------------------spi flash operate interface------------------------------------------------
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
static uint8_t spi_flash_read_status(void)
{
	uint8_t bySend = RDSR0_CMD;		//read status cmd0 = 0x05
	uint8_t byRecv;

	csi_gpio_set_low(GPIOB, PB12);
	csi_spi_send_receive(SPI0, (void *)&bySend, NULL, 1);
	csi_spi_send_receive(SPI0, NULL, (void *)&byRecv, 1);
	csi_gpio_set_high(GPIOB, PB12);
	
	return byRecv;
}

/** \brief flash wait idle
 *  \param[in] none
 *  \return none
 */
static void spi_flash_wait_busy(void)
{
	while((spi_flash_read_status() & 0x01) == 0x01);  // 1: busy, 0: idle
}

/** \brief flash write status 
 *  写flash状态寄存器，手动控制CS线
 *  \param[in] byStatus: flash status of write
 *  \return none
 */
//SPR0,TB,BP2,BP1,BP0(bit 7,5,4,3,2) 
static void spi_flash_write_Status(uint8_t byStatus)
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
 *  \return chip id (chip id = 0xef14)
 */
static uint32_t spi_flash_read_id(void)
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
static void spi_flash_chip_erase(void)
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
static void spi_flash_sector_erase(uint32_t wAddr)
{
	wAddr = wAddr & 0xfff000;
	uint8_t bySend[4] = {SECTORERASE_CMD, (wAddr >> 16), (wAddr >> 8),wAddr};
	
	spi_flash_write_enable();		//write enable
	csi_gpio_set_low(GPIOB, PB12);
	csi_spi_send_receive(SPI0, (void *)bySend, NULL, sizeof(bySend));		//send sector erase cmd and data three bytes addr 
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
static void spi_flash_read_bytes(uint8_t *pbyBuf, uint32_t wAddr, uint16_t hwNum)
{
	uint8_t bySend[4] = {READ_CMD, (wAddr >> 16), (wAddr >> 8),wAddr};
	
	csi_gpio_set_low(GPIOB, PB12);
	csi_spi_send_receive(SPI0, (void *)bySend, NULL, sizeof(bySend));		//send read bytes cmd and data three bytes addr 
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
static void spi_flash_write_bytes(uint8_t *pbyBuf, uint32_t wAddr, uint16_t hwNum)
{
	uint8_t bySend[4] = {PGPRO_CMD, (wAddr >> 16), (wAddr >> 8),wAddr};

	spi_flash_write_enable();		//write enable
	csi_gpio_set_low(GPIOB, PB12);
	csi_spi_send_receive(SPI0, (void *)bySend, NULL, sizeof(bySend));		//send write bytes cmd and data three bytes addr 
	csi_spi_send_receive(SPI0, (void *)pbyBuf, NULL, hwNum);	//write hwNum bytes
	csi_gpio_set_high(GPIOB, PB12);
	spi_flash_wait_busy();
}