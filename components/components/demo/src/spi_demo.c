/***********************************************************************//** 
 * \file  SPI_demo.c
 * \brief  SPI示例代码。
 * w25q16jvsiq(32 block == 512 sector == 2M Byte)
 * 1 page = 256 bytes
 * 1 sector = 16 page (4KB)
 * 1 block = 16 sector (64KB)
 * erasable(1sector,1/2block,1block,chip)
 * Device ID: (M7->M0 = 0xEF), (ID7->ID0 = 0x14)
 * Spi Mode:  MODE0 or MODE3
 * 
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version    <th>Author  <th>Description
 * <tr><td> 2020-11-24 <td>V0.0  <td>ZJY     <td>initial
 * <tr><td> 2021-07-02 <td>V0.1  <td>LQ      <td>modify
 * </table>
 * *********************************************************************
*/

/* include ----------------------------------------------------------------*/
#include "spi.h"
#include "pin.h"
#include <iostring.h>

//--------------------173 spi port reference-----------------------
//spi0 pin configuration
// NSS:   PA4,  PA11,  PA15,  PB12,  PD2
// CLK:   PA5,  PA12,  PB3,   PB13
// MISO:  PA6,      ,  PB4,   PB14
// MOSI:  PA7,      ,  PB5,   PB15

//spi1 pin configuration
// NSS:      ,  PA15,  PC2,       ,  PD1,   PD2
// CLK:      ,  PB3,   PC3,   PC10,     ,   PD5
// MISO:  PA0,  PB4,      ,   PC11,  
// MOSI:  PA1,  PB5,      ,   PB12, 
//----------------------------------------------------------------- 

/** \brief spi sync mode send buff
 * 
 *  \param[in] none
 *  \return error code
 */
 int spi_sync_sendbuff(void)
{
	int iRet = 0;
	uint8_t byData[8] = {1,2,3,4,5,6,7,8};
	csi_spi_config_t t_SpiConfig;  //spi初始化参数配置结构体
	
	//端口配置
	//csi_pin_set_mux(PA4, PA4_OUTPUT);                         //gpio_port as output
	//csi_pin_output_mode(PA4, GPIO_PUSH_PULL);                 //push pull mode
	//csi_spi_nss_high(PA4);									  //NSS init high
	//csi_pin_set_mux(PA11, PA11_OUTPUT);                         //gpio_port as output
	//csi_pin_output_mode(PA11, GPIO_PUSH_PULL);                 //push pull mode
	//csi_spi_nss_high(PA11);									  //NSS init high
	//csi_pin_set_mux(PD2, PD2_OUTPUT);                         //gpio_port as output
	//csi_pin_output_mode(PD2, GPIO_PUSH_PULL);                 //push pull mode
	//csi_spi_nss_high(PD2);									  //NSS init high
	//csi_pin_set_mux(PA15, PA15_OUTPUT);                         //gpio_port as output
	//csi_pin_output_mode(PA15, GPIO_PUSH_PULL);                 //push pull mode
	//csi_spi_nss_high(PA15);									  //NSS init high
	csi_pin_set_mux(PA15, PA15_OUTPUT);                         //gpio_port as output
	csi_pin_output_mode(PA15, GPIO_PUSH_PULL);                 //push pull mode
	csi_spi_nss_high(PA15);									  //NSS init high
			    
	//csi_pin_set_mux(PA5,PA5_SPI0_SCK);						  //PA5 = SPI_SCK
	//csi_pin_set_mux(PA12,PA12_SPI0_SCK);
	//csi_pin_set_mux(PA6,PA6_SPI0_MISO);						  //PA6 = SPI_MISO
	//csi_pin_set_mux(PA7,PA7_SPI0_MOSI);						  //PA7 = SPI_MOSI
	//csi_pin_set_mux(PB3,PB3_SPI0_SCK);
	//csi_pin_set_mux(PB4,PB4_SPI0_MISO);						  //PA6 = SPI_MISO
	//csi_pin_set_mux(PB5,PB5_SPI0_MOSI);						  //PA7 = SPI_MOSI
	
	//csi_pin_set_mux(PA15,PA15_SPI0_NSS);
	//csi_pin_set_mux(PB3,PB3_SPI1_SCK);
	
	csi_pin_set_mux(PB3,PB3_SPI1_SCK);
	//csi_pin_set_mux(PC11,PC11_SPI1_MISO);						  //PA6 = SPI_MISO
	//csi_pin_set_mux(PC12,PC12_SPI1_MOSI);						  //PA7 = SPI_MOSI
	csi_pin_set_mux(PA0,PA0_SPI1_MISO);						  //PA6 = SPI_MISO
	csi_pin_set_mux(PA1,PA1_SPI1_MOSI);						  //PA7 = SPI_MOSI
	
	t_SpiConfig.eSpiMode = SPI_MASTER;						//作为主机
	t_SpiConfig.eSpiPolarityPhase = SPI_FORMAT_CPOL0_CPHA1; //clk空闲电平为0，相位为在第一个边沿采集数据
	t_SpiConfig.eSpiFrameLen = SPI_FRAME_LEN_8;             //帧数据长度为8bit
	t_SpiConfig.dwSpiBaud = 16000000; 						//通讯速率16兆			
    t_SpiConfig.eSpiRxFifoLevel = SPI_RXFIFO_1_2;  			//接收fifo中断阈值，1/2*8=4个
	t_SpiConfig.byInter = (uint8_t)SPI_NONE_INT;			//初始配置无中断

	iRet = csi_spi_init(SPI1,&t_SpiConfig);			
	if(iRet < 0)
	{
		return -1;
	}	
	//my_printf("the spi sync send count is:%d!\n",8);
	//mdelay(500);

	while(1)
	{
		csi_spi_nss_low(PA15);
		csi_spi_send(SPI1,byData,8);//29.12us
		csi_spi_buff_send(SPI1,byData,8);   //12.72us
		csi_spi_nss_high(PA15);
		//mdelay(100);
		nop;
	}
	return iRet;
}

/** \brief spi async mode send buff
 * 
 *  \param[in] none
 *  \return error code
 */
 int spi_async_sendbuff(void)
{
	int iRet = 0;
	uint8_t bySendData[8] = {9,10,11,12,13,14,15,16};
	csi_spi_config_t t_SpiConfig;  //spi初始化参数配置结构体
	
	//端口配置
	csi_pin_set_mux(PA4, PA4_OUTPUT);                         //gpio_port as output
	csi_pin_output_mode(PA4, GPIO_PUSH_PULL);                 //push pull mode
	csi_spi_nss_high(PA4);									  //NSS init high			    
	csi_pin_set_mux(PA5,PA5_SPI0_SCK);						  //PA5 = SPI_SCK
	csi_pin_set_mux(PA6,PA6_SPI0_MISO);						  //PA6 = SPI_MISO
	csi_pin_set_mux(PA7,PA7_SPI0_MOSI);						  //PA7 = SPI_MOSI
	
	t_SpiConfig.eSpiMode = SPI_MASTER;						//作为主机
	t_SpiConfig.eSpiPolarityPhase = SPI_FORMAT_CPOL0_CPHA1; //clk空闲电平为0，相位为在第二个边沿采集数据
	t_SpiConfig.eSpiFrameLen = SPI_FRAME_LEN_8;             //帧数据长度为8bit
	t_SpiConfig.dwSpiBaud = 16000000; 						//通讯速率16兆			
    t_SpiConfig.eSpiRxFifoLevel = SPI_RXFIFO_1_2;  			//接收fifo中断阈值，1/2*8=4个//（当阈值为fifo深度的一半时效率高）
	t_SpiConfig.byInter = (uint8_t)SPI_TXIM_INT;			//发送使用中断	  	
	iRet = csi_spi_init(SPI0,&t_SpiConfig);
	if(iRet < 0)
	{
		return -1;
	}
	
	my_printf("the spi async send count is:%d!\n",8);	
	mdelay(500);	

	while(1)
	{
		csi_spi_nss_low(PA4);
		iRet = csi_spi_send_async(SPI0, (void *) bySendData, 8);//send first, 22.66us
		//while(g_tSpiTransmit.byTxSize);//等待异步传输完成
		//csi_spi_nss_high(PA4);  //when async send,put "set nss high" to interrupt
		mdelay(100);		
		nop;
	}
	return iRet;
}

/** \brief spi speed test
 * 
 *  \param[in] none
 *  \return error code
 */
int spi_sync_test_speed(void)
{
	int iRet = 0;
	uint8_t bySendData[17] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17};
	uint8_t byReceData[17] = {0};
	csi_spi_config_t t_SpiConfig;  //spi初始化参数配置结构体
	
	//端口配置
	csi_pin_set_mux(PA4, PA4_OUTPUT);                         //gpio_port as output
	csi_pin_output_mode(PA4, GPIO_PUSH_PULL);                 //push pull mode
	csi_spi_nss_high(PA4);									  //NSS init high			    
	csi_pin_set_mux(PA5,PA5_SPI0_SCK);						  //PA5 = SPI_SCK
	csi_pin_set_mux(PA6,PA6_SPI0_MISO);						  //PA6 = SPI_MISO
	csi_pin_set_mux(PA7,PA7_SPI0_MOSI);						  //PA7 = SPI_MOSI
	
	t_SpiConfig.eSpiMode = SPI_MASTER;						  //作为主机
	t_SpiConfig.eSpiPolarityPhase = SPI_FORMAT_CPOL0_CPHA1;   //clk空闲电平为0，相位为在第二个边沿采集数据
	t_SpiConfig.eSpiFrameLen = SPI_FRAME_LEN_8;               //帧数据长度为8bit
	t_SpiConfig.dwSpiBaud = 16000000; 						  //通讯速率16兆			
    t_SpiConfig.eSpiRxFifoLevel = SPI_RXFIFO_1_2;  			  //接收fifo中断阈值，1/2*8=4个	 
	t_SpiConfig.byInter = (uint8_t)SPI_NONE_INT;			  //初始配置无中断 
	iRet = csi_spi_init(SPI0,&t_SpiConfig);				
	if(iRet < 0)
	{
		return -1;
	}	
	my_printf("the spi test speed end!\n");		
	mdelay(500);

	while(1)
	{
		csi_spi_nss_low(PA4);
		csi_spi_send_receive_x8(SPI0,bySendData,byReceData,7);//3.47us
		csi_spi_nss_high(PA4);
		
		csi_spi_nss_low(PA4);
		csi_spi_send_receive(SPI0, (void*)bySendData, (void *)byReceData, 7);//48.47 us
		csi_spi_nss_high(PA4);
		
		csi_spi_nss_low(PA4);
		csi_spi_send_receive_d8(SPI0,bySendData,byReceData,7);//(3.47 us #if 1)
		csi_spi_nss_high(PA4);
		
		csi_spi_nss_low(PA4);
		csi_spi_send_receive(SPI0, (void*)bySendData, (void *)byReceData, 17);//128.46 us
		csi_spi_nss_high(PA4);
		
		csi_spi_nss_low(PA4);
		csi_spi_send_receive_d8(SPI0,bySendData,byReceData,17);//(25.5us)
		csi_spi_nss_high(PA4);
		mdelay(100);
		nop;
	}
	return iRet;
}


/** \brief spi master sync send and sync receive
 * 
 *  \param[in] none
 *  \return none
 */
void spi_master_sync_send_sync_receive(void)//主机同步发，同步收
{
	uint8_t bySendData[17] = {19,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17};
	uint8_t byReceData[17] = {0};
	csi_spi_config_t t_SpiConfig;  //spi初始化参数配置结构体
	
	//端口配置
	csi_pin_set_mux(PA4, PA4_OUTPUT);                         //gpio_port as output
	csi_pin_output_mode(PA4, GPIO_PUSH_PULL);                 //push pull mode
	csi_spi_nss_high(PA4);									  //NSS init high			    
	csi_pin_set_mux(PA5,PA5_SPI0_SCK);						  //PA5 = SPI_SCK
	csi_pin_set_mux(PA6,PA6_SPI0_MISO);						  //PA6 = SPI_MISO
	csi_pin_set_mux(PA7,PA7_SPI0_MOSI);						  //PA7 = SPI_MOSI
	
	t_SpiConfig.eSpiMode = SPI_MASTER;					//作为主机
	t_SpiConfig.eSpiPolarityPhase = SPI_FORMAT_CPOL0_CPHA1; //clk空闲电平为0，相位为在第二个边沿采集数据
	t_SpiConfig.eSpiFrameLen = SPI_FRAME_LEN_8;             //帧数据长度为8bit
	t_SpiConfig.dwSpiBaud = 2000000; 						//通讯速率2兆			
    t_SpiConfig.eSpiRxFifoLevel = SPI_RXFIFO_1_2;  			//接收fifo中断阈值，1/2*8=4个
	t_SpiConfig.byInter = (uint8_t)SPI_NONE_INT;			//初始配置无中断	
	csi_spi_init(SPI0,&t_SpiConfig);				

	my_printf("the spi master mode1!\n");
	mdelay(500);
	while(1)
	{
		csi_spi_nss_low(PA4);
		csi_spi_send_receive(SPI0, bySendData, byReceData, 16);
		csi_spi_nss_high(PA4);
		mdelay(100);
		nop;
	}	
}

/** \brief spi slave sync send and sync receive
 * 
 *  \param[in] none
 *  \return none
 */
void spi_slave_sync_send_sync_receive(void)//从机同步发，同步收
{
	uint8_t byReceData[17] = {0};
	csi_spi_config_t t_SpiConfig;  //spi初始化参数配置结构体
	
	//从机端口配置
	csi_pin_set_mux(PA4,PA4_SPI0_NSS);					//PA4 = SPI_NSS
	csi_pin_set_mux(PA5,PA5_SPI0_SCK);					//PA5 = SPI_SCK
	csi_pin_set_mux(PA6,PA6_SPI0_MISO);					//PA6 = SPI_MISO
	csi_pin_set_mux(PA7,PA7_SPI0_MOSI);					//PA7 = SPI_MOSI
	
	t_SpiConfig.eSpiMode = SPI_SLAVE;					//作为从机
	t_SpiConfig.eSpiPolarityPhase = SPI_FORMAT_CPOL0_CPHA1; //clk空闲电平为0，相位为在第二个边沿采集数据
	t_SpiConfig.eSpiFrameLen = SPI_FRAME_LEN_8;             //帧数据长度为8bit
	t_SpiConfig.dwSpiBaud = 2000000; 						//通讯速率2兆			
    t_SpiConfig.eSpiRxFifoLevel = SPI_RXFIFO_1_2;  			//接收fifo中断阈值，1/2*8=4个
	t_SpiConfig.byInter = (uint8_t)SPI_NONE_INT;			//初始配置无中断	
	csi_spi_init(SPI0,&t_SpiConfig);				

	my_printf("the spi slave mode1!\n");
	mdelay(100);
	while(1)
	{
		while( (uint32_t)(SPI0->SR) & SPI_RNE )	
		{
			byReceData[0] = csi_spi_receive_slave(SPI0);
			csi_spi_send_slave(SPI0, byReceData[0] );
		}
		nop;
	}
}

/** \brief spi master sync send and async receive
 * 
 *  \param[in] none
 *  \return none
 */
void spi_master_sync_send_async_receive(void)//主机同步发，异步收
{
	uint8_t bySendData[17] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17};
	uint8_t byReceData[17] = {0};
	csi_spi_config_t t_SpiConfig;  //spi初始化参数配置结构体
	
	//端口配置
	csi_pin_set_mux(PA4, PA4_OUTPUT);                         //gpio_port as output
	csi_pin_output_mode(PA4, GPIO_PUSH_PULL);                 //push pull mode
	csi_spi_nss_high(PA4);									  //NSS init high			    
	csi_pin_set_mux(PA5,PA5_SPI0_SCK);						  //PA5 = SPI_SCK
	csi_pin_set_mux(PA6,PA6_SPI0_MISO);						  //PA6 = SPI_MISO
	csi_pin_set_mux(PA7,PA7_SPI0_MOSI);						  //PA7 = SPI_MOSI
	
	t_SpiConfig.eSpiMode = SPI_MASTER;					         //作为主机
	t_SpiConfig.eSpiPolarityPhase = SPI_FORMAT_CPOL0_CPHA1;      //clk空闲电平为0，相位为在第二个边沿采集数据
	t_SpiConfig.eSpiFrameLen = SPI_FRAME_LEN_8;               	 //帧数据长度为8bit
	t_SpiConfig.dwSpiBaud = 2000000; 						  	 //通讯速率2兆			
    t_SpiConfig.eSpiRxFifoLevel = SPI_RXFIFO_1_2;  			     //接收fifo中断阈值，1/2*8=4个
	t_SpiConfig.byInter = (uint8_t)(SPI_RXIM_INT | SPI_RTIM_INT);//初始配置接收中断和接收超时中断	
	csi_spi_init(SPI0,&t_SpiConfig);				
	
	my_printf("the spi master mode2!\n");
	mdelay(500);
	while(1)
	{
		csi_spi_receive_async(SPI0, byReceData, 16);//异步接收buff和大小设置
		csi_spi_nss_low(PA4);
		csi_spi_send(SPI0, bySendData, 16);
		csi_spi_nss_high(PA4);

		mdelay(100);
		nop;
	}	
}

/** \brief spi slave async send and async receive
 * 
 *  \param[in] none
 *  \return none
 */
void spi_slave_async_send_async_receive(void)//从机异步发，异步收
{
	csi_spi_config_t t_SpiConfig;  //spi初始化参数配置结构体
	
	//从机端口配置
	csi_pin_set_mux(PA4,PA4_SPI0_NSS);					//PA4 = SPI_NSS
	csi_pin_set_mux(PA5,PA5_SPI0_SCK);					//PA5 = SPI_SCK
	csi_pin_set_mux(PA6,PA6_SPI0_MISO);					//PA6 = SPI_MISO
	csi_pin_set_mux(PA7,PA7_SPI0_MOSI);					//PA7 = SPI_MOSI
	
	t_SpiConfig.eSpiMode = SPI_SLAVE;					//作为从机
	t_SpiConfig.eSpiPolarityPhase = SPI_FORMAT_CPOL0_CPHA1; //clk空闲电平为0，相位为在第二个边沿采集数据
	t_SpiConfig.eSpiFrameLen = SPI_FRAME_LEN_8;             //帧数据长度为8bit
	t_SpiConfig.dwSpiBaud = 2000000; 						//通讯速率2兆			
    t_SpiConfig.eSpiRxFifoLevel = SPI_RXFIFO_1_2;  			//接收fifo中断阈值，1/2*8=4个
	t_SpiConfig.byInter = (uint8_t)SPI_RXIM_INT;			//初始配置接收中断	
	csi_spi_init(SPI0,&t_SpiConfig);				

	my_printf("the spi slave mode2!\n");
	mdelay(100);
	while(1)
	{
		nop;
	}
}

void test_clr_txrxfifo(void)
{
	uint8_t bySendData[17] = {1,2,3,4,55,6,7,8,9,10,11,12,13,14,15,16,17};
	uint8_t byReceData[17] = {0};
	csi_spi_config_t t_SpiConfig;  //spi初始化参数配置结构体
	
	//端口配置
	csi_pin_set_mux(PA4, PA4_OUTPUT);                         //gpio_port as output
	csi_pin_output_mode(PA4, GPIO_PUSH_PULL);                 //push pull mode
	csi_spi_nss_high(PA4);									  //NSS init high			    
	csi_pin_set_mux(PA5,PA5_SPI0_SCK);						  //PA5 = SPI_SCK
	csi_pin_set_mux(PA6,PA6_SPI0_MISO);						  //PA6 = SPI_MISO
	csi_pin_set_mux(PA7,PA7_SPI0_MOSI);						  //PA7 = SPI_MOSI
	
	
	t_SpiConfig.eSpiMode = SPI_MASTER;					//作为主机
	t_SpiConfig.eSpiPolarityPhase = SPI_FORMAT_CPOL0_CPHA1; //clk空闲电平为0，相位为在第二个边沿采集数据
	t_SpiConfig.eSpiFrameLen = SPI_FRAME_LEN_8;             //帧数据长度为8bit
	t_SpiConfig.dwSpiBaud = 1000000; 						//通讯速率2兆			
    t_SpiConfig.eSpiRxFifoLevel = SPI_RXFIFO_1_2;  			//接收fifo中断阈值，1/2*8=4个
	t_SpiConfig.byInter = (uint8_t)SPI_NONE_INT ;			//初始配置接收中断	
	csi_spi_init(SPI0,&t_SpiConfig);				
	
	my_printf("the test_clr_txrxfifo!\n");
	mdelay(500);
	while(1)
	{
		#if 1
			csi_spi_nss_low(PA4);
			csp_spi_softreset(SPI0,SPI_RXFIFO_RST);
			csi_spi_send(SPI0, bySendData, 7);
			while( ( (uint32_t)(SPI0->SR) & SPI_BSY )  ){;} 
			csp_spi_softreset(SPI0,SPI_RXFIFO_RST);
			for(uint8_t j=0;j<7;j++)
			{
				byReceData[j] = SPI0->DR;
			}
			csi_spi_nss_high(PA4);
			my_printf("the receive data is:\n");
			for(uint8_t i=0;i<7;i++)
			{
				my_printf("%d is:%d\n",i,byReceData[i]);
			}
			//my_printf("the spi rxfifo is:%d!\n",csp_spi_get_rxfifo_level(SPI0));
			//csp_spi_softreset(SPI0,SPI_RXFIFO_RST);
			//my_printf("the spi rxfifo is:%d!\n",csp_spi_get_rxfifo_level(SPI0));
		#else
			csp_spi_dis(SPI0);							           //关闭spi
			SPI0->DR = 0x01;
			SPI0->DR = 0x02;
			SPI0->DR = 0x03;
			SPI0->DR = 0x04;
			my_printf("the spi txfifo is:%d!\n",csp_spi_get_txfifo_level(SPI0));
			csp_spi_softreset(SPI0,SPI_TXFIFO_RST);
			my_printf("the spi txfifo is:%d!\n",csp_spi_get_txfifo_level(SPI0));
		#endif
		
		mdelay(100);
		nop;
	}	
	
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
	

static uint8_t byWrBuf[100] = {26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11};
volatile static uint8_t byRdBuf[100];
	
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
 * 
 *  \param[in] none
 *  \return none
 */
static void SPI_flash_write_enable(void)
{
	uint8_t byCmd = WREN_CMD;		//write enable cmd = 0x06
	
	csi_spi_nss_low(PA4);
	csi_spi_send_receive(SPI0,(void *)&byCmd, NULL, 1);	
	csi_spi_nss_high(PA4);
}

/** \brief flash read status reg
 * 
 *  \param[in] none
 *  \return flash status
 */
//BIT7    6     5     4     3     2     1     0
//SPR0    SEC   TB   BP2    BP1  BP0   WEL   BUSY
//SPR0:Status Protect
//SEC: Sector/Block Protect
//TB:  TOP/BOTTOM Block Protect
//BP3, BP2,BP1,BP0:FLASH Block Protect
//WEL: Write Enable Latch
//BUSY:Busy flage（1 busy：0 idle）
//defualt:0x00

uint8_t SPI_flash_read_status(void)
{
	uint8_t bySend[2] = {RDSR0_CMD, 0x00};		//read status cmd0 = 0x05
	uint8_t byRecv[2];

	csi_spi_nss_low(PA4);
	csi_spi_send_receive(SPI0, (void *)bySend, (void *)byRecv, 2);
	csi_spi_nss_high(PA4);
	
	return byRecv[1];
}
/** \brief flash wait idle
 * 
 *  \param[in] none
 *  \return none
 */
void SPI_flash_wait_busy(void)
{
	while((SPI_flash_read_status() & 0x01) == 0x01);	// 1: busy, 0: idle
}

/** \brief flash write status 
 * 
 *  \param[in] byStatus: flash status of write
 *  \return none
 */
//SPR0,TB,BP2,BP1,BP0(bit 7,5,4,3,2) 
void SPI_flash_write_Status(uint8_t byStatus)
{
	uint8_t bySend[2] = {WRSR_CMD, byStatus};		//write status cmd = 0x01

	SPI_flash_write_enable();		//write enable cmd
	csi_spi_nss_low(PA4);
	csi_spi_send_receive(SPI0, (void *)bySend, NULL, 2);
	csi_spi_nss_high(PA4);
	SPI_flash_wait_busy();
}
/** \brief flash read chip id
 * 
 *  \param[in] none
 *  \return chip id (chip id = 0xef13)
 */
uint32_t SPI_flash_read_id(void)
{
	uint32_t hwId = 0x00;
	uint8_t bySend[6] = {RDDEVICEID_CMD, 0, 0, 0};
	uint8_t byRecv[2];
	
	csi_spi_nss_low(PA4); 
	csi_spi_send_receive(SPI0, (void *)bySend, NULL, 4);	//send read id cmd and three bytes addr	
	csi_spi_send_receive(SPI0, NULL, (void *)byRecv, 2);	//read id value; id value = 0xef14
	hwId = ((byRecv[0] << 8) |  byRecv[1]);
	csi_spi_nss_high(PA4);
   
	return hwId;
}
/** \brief flash chip erase
 * 
 *  \param[in] none
 *  \return none
 */
void SPI_flash_chip_erase(void)
{
	uint8_t byCmd = CHIPERASE_CMD;
	
	SPI_flash_write_enable();		//write enable
	csi_spi_nss_low(PA4);
	csi_spi_send_receive(SPI0, (void *)&byCmd, NULL, 1);		//send chip erase cmd
	csi_spi_nss_high(PA4);
	SPI_flash_wait_busy();
}
/** \brief flash sector erase (sector = 4k bytes)
 * 
 *  \param[in] wAddr: flash sector addr
 *  \return none
 */
void SPI_flash_sector_erase(uint32_t wAddr)
{
	wAddr = wAddr & 0xfff000;
	uint8_t bySend[4] = {SECTORERASE_CMD, (wAddr >> 16), (wAddr >> 8), wAddr};
	
	SPI_flash_write_enable();		//write enable
	csi_spi_nss_low(PA4);
	csi_spi_send_receive(SPI0, (void *)bySend, NULL, 4);		//send sector erase cmd and data three bytes addr 
	csi_spi_nss_high(PA4);
	SPI_flash_wait_busy();
}

/** \brief flash read bytes
 * 
 *  \param[in] pbyBuf: pointer of read data buf 
 *  \param[in] wAddr: start addr of read data
 *  \param[in] hwNum: length of read data
 *  \return none
 */
void SPI_flash_read_bytes(uint8_t *pbyBuf, uint32_t wAddr, uint16_t hwNum)
{
	uint8_t bySend[4] = {READ_CMD, (wAddr >> 16), (wAddr >> 8), wAddr};
	
	csi_spi_nss_low(PA4); 
	csi_spi_send_receive(SPI0, (void *)bySend, NULL, 4);		//send read bytes cmd and data three bytes addr 
	csi_spi_send_receive(SPI0, NULL,(void *)pbyBuf, hwNum);		//read hwNum bytes 
	csi_spi_nss_high(PA4);
}

/** \brief flash write enable 
 * 
 *  \param[in] pbyBuf: pointer of write data buf 
 *  \param[in] wAddr: start addr of write data
 *  \param[in] hwNum: length of write data
 *  \return none
 */
void SPI_flash_write_bytes(uint8_t *pbyBuf, uint32_t wAddr, uint16_t hwNum)
{
	uint8_t bySend[4] = {PGPRO_CMD, (wAddr >> 16), (wAddr >> 8), wAddr};

	SPI_flash_write_enable();		//write enable
	csi_spi_nss_low(PA4);
	csi_spi_send_receive(SPI0, (void *)bySend, NULL, 4);		//send write bytes cmd and data three bytes addr 
	csi_spi_send_receive(SPI0, (void *)pbyBuf, NULL, hwNum);	//write hwNum bytes
	csi_spi_nss_high(PA4);
	SPI_flash_wait_busy();
}

/** \brief spi示例代码
 * 
 *  \param[in] none
 *  \return error code
 */
int spi_w25q16jvsiq_write_read(void)
{
	int iRet = 0;	
	csi_spi_config_t t_SpiConfig;  //spi初始化参数配置结构体
	uint8_t byIndex=0;
	for(byIndex=0;byIndex<100;byIndex++)
	{
		byWrBuf[byIndex] = byIndex + 1;
	}
	
	//主机端口配置
	csi_pin_set_mux(PA4, PA4_OUTPUT);                         //gpio_port as output
	csi_pin_output_mode(PA4, GPIO_PUSH_PULL);                 //push pull mode
	csi_spi_nss_high(PA4);									  //NSS init high			    
	csi_pin_set_mux(PA5,PA5_SPI0_SCK);						  //PA5 = SPI_SCK
	csi_pin_set_mux(PA6,PA6_SPI0_MISO);						  //PA6 = SPI_MISO
	csi_pin_set_mux(PA7,PA7_SPI0_MOSI);						  //PA7 = SPI_MOSI
	
	t_SpiConfig.eSpiMode = SPI_MASTER;						//作为主机
	t_SpiConfig.eSpiPolarityPhase = SPI_FORMAT_CPOL0_CPHA0; //clk空闲电平为0，相位为在第二个边沿采集数据
	t_SpiConfig.eSpiFrameLen = SPI_FRAME_LEN_8;             //帧数据长度为8bit
	t_SpiConfig.dwSpiBaud = 16000000; 						//通讯速率12兆			
    t_SpiConfig.eSpiRxFifoLevel = SPI_RXFIFO_1_2;  			//接收fifo中断阈值，1/2*8=4个
	t_SpiConfig.byInter = (uint8_t)SPI_NONE_INT;			//初始配置无中断	  
	iRet = csi_spi_init(SPI0,&t_SpiConfig);							
	if(iRet < 0)
	{
		return -1;
	}
	//my_printf("the spi w25q16 test!\n");	
	mdelay(200);
	iRet = SPI_flash_read_id();					//read chip id, chip id = 0xef14
	
	if(iRet == 0xef14)
	{
	
		//iRet = SPI_flash_read_status();		//read status reg
		
		SPI_flash_sector_erase(0x1000);			//erase sector 1; start addr = 0x1000

		SPI_flash_read_bytes((uint8_t *)byRdBuf, 0x1000, 100);	//read data = 0xff
	
		SPI_flash_write_bytes(byWrBuf,0x1000,100);				//write 16 bytes 
		
		SPI_flash_read_bytes((uint8_t *)byRdBuf, 0x1000, 100);	//read 16 bytes, read bytes = write bytes
		
		iRet = SPI_flash_read_id();					//read chip id, chip id = 0xef14
	}
	while(1)
	{
		nop;
		SPI_flash_read_bytes((uint8_t *)byRdBuf, 0x1000, 100);	//read 16 bytes, read bytes = write bytes
	}
	return iRet;
}

//spi_etcb_dma send
void spi_etcb_dma_send(void)
{
	uint8_t bySdData[100]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};

	csi_dma_ch_config_t tDmaConfig;		//DMA 配置初始化
	csi_etb_config_t 	tEtbConfig;		//ETB 参数配置结构体	
	csi_spi_config_t t_SpiConfig;  //spi初始化参数配置结构体
	
	for(uint8_t i=0;i<100;i++)
	{
		bySdData[i] = i;
	}
	
	//dma para config
	tDmaConfig.bySrcLinc 	= DMA_ADDR_INC;				//低位传输原地址自增
	tDmaConfig.bySrcHinc 	= DMA_ADDR_CONSTANT;		//高位传输原地址固定不变
	tDmaConfig.byDetLinc 	= DMA_ADDR_CONSTANT;		//低位传输目标地址固定不变
	tDmaConfig.byDetHinc 	= DMA_ADDR_CONSTANT;		//高位传输目标地址固定不变
	tDmaConfig.byDataWidth 	= DMA_DSIZE_8_BITS;			//传输数据宽度8bit
	tDmaConfig.byReload 	= DMA_RELOAD_DISABLE;		//禁止自动重载
	tDmaConfig.byTransMode 	= DMA_TRANS_ONCE;			//DMA服务模式(传输模式)，连续服务
	tDmaConfig.byTsizeMode  = DMA_TSIZE_ONE_DSIZE;		//传输数据大小，一个 DSIZE , 即DSIZE定义大小
	tDmaConfig.byReqMode	= DMA_REQ_HARDWARE;			//DMA请求模式，硬件请求（硬件触发）
	tDmaConfig.wInt 		= DMA_INTSRC_NONE;			//不使用中断
	
	//etcb para config
	tEtbConfig.byChType = ETB_ONE_TRG_ONE_DMA;			//单个源触发单个目标，DMA方式
	tEtbConfig.bySrcIp 	= ETB_SPI0_TXSRC;				//SPI0 TXSRC作为触发源
	tEtbConfig.bySrcIp1 = 0xff;						
	tEtbConfig.bySrcIp2 = 0xff;
	tEtbConfig.byDstIp 	= ETB_DMA0_CH0;					//ETB DMA0通道0作为目标实际
	tEtbConfig.byDstIp1 = 0xff;
	tEtbConfig.byDstIp2 = 0xff;
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;			//通道触发模式采样硬件触发
	
	//spi主机端口配置
	csi_pin_set_mux(PA4, PA4_OUTPUT);                         //gpio_port as output
	csi_pin_output_mode(PA4, GPIO_PUSH_PULL);                 //push pull mode
	csi_spi_nss_high(PA4);									  //NSS init high			    
	csi_pin_set_mux(PA5,PA5_SPI0_SCK);						  //PA5 = SPI_SCK
	csi_pin_set_mux(PA6,PA6_SPI0_MISO);						  //PA6 = SPI_MISO
	csi_pin_set_mux(PA7,PA7_SPI0_MOSI);						  //PA7 = SPI_MOSI
	
	//spi para config
	t_SpiConfig.eSpiMode = SPI_MASTER;						//作为主机
	t_SpiConfig.eSpiPolarityPhase = SPI_FORMAT_CPOL0_CPHA1; //clk空闲电平为0，相位为在第二个边沿采集数据
	t_SpiConfig.eSpiFrameLen = SPI_FRAME_LEN_8;             //帧数据长度为8bit
	t_SpiConfig.dwSpiBaud = 16000000; 						//通讯速率12兆			
    t_SpiConfig.eSpiRxFifoLevel = SPI_RXFIFO_1_2;  			//接收fifo中断阈值，1/2*8=4个
	t_SpiConfig.byInter = (uint8_t)SPI_NONE_INT;			//初始配置无中断	  
					
	
	csi_etb_init();									//使能ETB模块
	csi_etb_ch_config(ETB_CH20, &tEtbConfig);		//初始化ETB，DMA ETB CHANNEL > ETB_CH19_ID
	
	
	csi_dma_soft_rst(DMA0);
	csi_dma_ch_init(DMA0, 0, &tDmaConfig);				//初始化DMA
	
	csi_spi_init(SPI0,&t_SpiConfig);					//初始化并启动spi
	
	csi_spi_nss_low(PA4);
	csi_spi_send_dma(SPI0,(void *)bySdData,20,DMA0, 0);  //使能dma通道，等待对应dma请求并传输
	csp_dma_t *ptDmaChBase = (csp_dma_t *)DMA_REG_BASE(DMA0, 0);
	while(csp_dma_get_curr_htc(ptDmaChBase));//等待直到dma传输完成
	//ETCB->SOFTTRIG = 0X00100000;
	//while(csp_spi_get_sr(SPI0) & SPI_BSY );		//wait for transmition finish
	//while(DMA0->MTRX & 0x02);
	
    csi_spi_nss_high(PA4);
	nop;
	
	while(1)
	{
		csi_spi_nss_low(PA4);
		csi_spi_send_dma(SPI0,(void *)bySdData,100,DMA0, 0);
		//while(csp_spi_get_sr(SPI0) & SPI_BSY );		//wait for transmition finish
		//while(DMA0->MTRX & 0x02)
		//csp_dma_t *ptDmaChBase = (csp_dma_t *)DMA_REG_BASE(DMA0, 0);
		while(csp_dma_get_curr_htc(ptDmaChBase));//等待直到dma传输完成
		csi_spi_nss_high(PA4);
		nop;
	}

}

void spi_dma_send(void)
{
	
	uint8_t byChnl = 0;
	uint8_t bySrcBuf[18] = {1, 2 ,3, 4, 5, 6, 7,8,9,10,11,12,13,14,15,16,17,18};
	csi_dma_ch_config_t tDmaConfig;
	csi_spi_config_t t_SpiConfig;   //spi初始化参数配置结构体
	
	//etcb para config
	tDmaConfig.bySrcLinc 	= DMA_ADDR_INC;				//低位传输原地址自增
	tDmaConfig.bySrcHinc 	= DMA_ADDR_CONSTANT;		//高位传输原地址固定不变
	tDmaConfig.byDetLinc 	= DMA_ADDR_CONSTANT;		//低位传输目标地址固定不变
	tDmaConfig.byDetHinc 	= DMA_ADDR_CONSTANT;		//高位传输目标地址固定不变
	tDmaConfig.byDataWidth 	= DMA_DSIZE_8_BITS;			//传输数据宽度8bit
	tDmaConfig.byReload 	= DMA_RELOAD_DISABLE;		//禁止自动重载
	tDmaConfig.byTransMode 	= DMA_TRANS_CONTINU;		//DMA服务模式(传输模式)，连续服务
	tDmaConfig.byTsizeMode  = DMA_TSIZE_ONE_DSIZE;		//传输数据大小，一个 DSIZE , 即DSIZE定义大小
	tDmaConfig.byReqMode	= DMA_REQ_SOFTWARE;			//DMA请求模式，软件请求（软件触发）
	tDmaConfig.wInt 		= DMA_INTSRC_NONE;			//不使用中断
	
	//spi主机端口配置
	csi_pin_set_mux(PA4, PA4_OUTPUT);                         //gpio_port as output
	csi_pin_output_mode(PA4, GPIO_PUSH_PULL);                 //push pull mode
	csi_spi_nss_high(PA4);									  //NSS init high			    
	csi_pin_set_mux(PA5,PA5_SPI0_SCK);						  //PA5 = SPI_SCK
	csi_pin_set_mux(PA6,PA6_SPI0_MISO);						  //PA6 = SPI_MISO
	csi_pin_set_mux(PA7,PA7_SPI0_MOSI);						  //PA7 = SPI_MOSI
	
	//spi para config
	t_SpiConfig.eSpiMode = SPI_MASTER;						//作为主机
	t_SpiConfig.eSpiPolarityPhase = SPI_FORMAT_CPOL0_CPHA1; //clk空闲电平为0，相位为在第二个边沿采集数据
	t_SpiConfig.eSpiFrameLen = SPI_FRAME_LEN_8;             //帧数据长度为8bit
	t_SpiConfig.dwSpiBaud = 3000000; 						//通讯速率3兆			
    t_SpiConfig.eSpiRxFifoLevel = SPI_RXFIFO_1_2;  			//接收fifo中断阈值，1/2*8=4个
	t_SpiConfig.byInter = (uint8_t)SPI_NONE_INT;			//初始配置无中断	  
	
	csi_dma_soft_rst(DMA0);				
	csi_dma_ch_init(DMA0, byChnl, &tDmaConfig);
	
	csi_spi_init(SPI0,&t_SpiConfig);					//初始化并启动spi
	
	
	csi_spi_nss_low(PA4);
	csi_dma_ch_start(DMA0, byChnl, (void *)bySrcBuf, (void *)&(SPI0->DR), 18,1);
	//while(DMA0->MTRX & 0x02);
	//while(DMA0->SRX & 0xfff);
	//while( !(SPI0->SR & SPI_TFE) );
	while( (SPI0->SR & SPI_BSY) );
	//while(csp_spi_get_sr(SPI0) & SPI_BSY );		//wait for transmition finish
	csi_spi_nss_high(PA4);

	
	while(1)
	{
		csi_spi_nss_low(PA4);
		csi_dma_ch_start(DMA0, byChnl, (void *)bySrcBuf, (void *)&(SPI0->DR), 18,1);
		while( (SPI0->SR & SPI_BSY) );
		csi_spi_nss_high(PA4);
		nop;
	}

}


//

void spi_dma_send_receive(void)
{	
	uint8_t byChnl = 0;
	uint8_t byChnl1 = 1;
	volatile uint8_t bySrcBuf[17] = {1, 2 ,3, 4, 5, 6, 7,8,9,10,11,12,13,14,15,16,17};
	volatile uint8_t byDesBuf[17] = {19};
	csi_dma_ch_config_t tDmaConfig;
	csi_dma_ch_config_t tDmaConfig1;
	csi_spi_config_t t_SpiConfig;   //spi初始化参数配置结构体
	
	//send dma para config
	tDmaConfig.bySrcLinc 	= DMA_ADDR_INC;				//低位传输原地址自增
	tDmaConfig.bySrcHinc 	= DMA_ADDR_CONSTANT;		//高位传输原地址固定不变
	tDmaConfig.byDetLinc 	= DMA_ADDR_CONSTANT;		//低位传输目标地址固定不变
	tDmaConfig.byDetHinc 	= DMA_ADDR_CONSTANT;		//高位传输目标地址固定不变
	tDmaConfig.byDataWidth 	= DMA_DSIZE_8_BITS;			//传输数据宽度8bit
	tDmaConfig.byReload 	= DMA_RELOAD_DISABLE;		//禁止自动重载
	tDmaConfig.byTransMode 	= DMA_TRANS_CONTINU;		    //DMA服务模式(传输模式)，连续服务
	tDmaConfig.byTsizeMode  = DMA_TSIZE_ONE_DSIZE;		//传输数据大小，一个 DSIZE , 即DSIZE定义大小
	tDmaConfig.byReqMode	= DMA_REQ_SOFTWARE;			//DMA请求模式，软件请求（软件触发）
	tDmaConfig.wInt 		= DMA_INTSRC_NONE;			//不使用中断
	
	//receive dma para config
	tDmaConfig1.bySrcLinc 	= DMA_ADDR_CONSTANT;		//低位传输原地址固定不变
	tDmaConfig1.bySrcHinc 	= DMA_ADDR_CONSTANT;		//高位传输原地址固定不变
	tDmaConfig1.byDetLinc 	= DMA_ADDR_INC;				//低位传输目标地址固定不变
	tDmaConfig1.byDetHinc 	= DMA_ADDR_CONSTANT;		//高位传输目标地址固定不变
	tDmaConfig1.byDataWidth = DMA_DSIZE_8_BITS;			//传输数据宽度8bit
	tDmaConfig1.byReload 	= DMA_RELOAD_DISABLE;		//禁止自动重载
	tDmaConfig1.byTransMode = DMA_TRANS_CONTINU;		//DMA服务模式(传输模式)，连续服务
	tDmaConfig1.byTsizeMode  = DMA_TSIZE_ONE_DSIZE;		//传输数据大小，一个 DSIZE , 即DSIZE定义大小
	tDmaConfig1.byReqMode	= DMA_REQ_SOFTWARE;			//DMA请求模式，软件请求（软件触发）
	tDmaConfig1.wInt 		= DMA_INTSRC_NONE;			//不使用中断
	
	//spi主机端口配置
	csi_pin_set_mux(PA4, PA4_OUTPUT);                         //gpio_port as output
	csi_pin_output_mode(PA4, GPIO_PUSH_PULL);                 //push pull mode
	csi_spi_nss_high(PA4);									  //NSS init high			    
	csi_pin_set_mux(PA5,PA5_SPI0_SCK);						  //PA5 = SPI_SCK
	csi_pin_set_mux(PA6,PA6_SPI0_MISO);						  //PA6 = SPI_MISO
	csi_pin_set_mux(PA7,PA7_SPI0_MOSI);						  //PA7 = SPI_MOSI
	
	//spi para config
	t_SpiConfig.eSpiMode = SPI_MASTER;						//作为主机
	t_SpiConfig.eSpiPolarityPhase = SPI_FORMAT_CPOL0_CPHA1; //clk空闲电平为0，相位为在第二个边沿采集数据
	t_SpiConfig.eSpiFrameLen = SPI_FRAME_LEN_8;             //帧数据长度为8bit
	t_SpiConfig.dwSpiBaud = 3000000; 						//通讯速率3兆			
    t_SpiConfig.eSpiRxFifoLevel = SPI_RXFIFO_1_2;  			//接收fifo中断阈值，1/2*8=4个
	t_SpiConfig.byInter = (uint8_t)SPI_NONE_INT;			//初始配置无中断	  
	
	csi_dma_soft_rst(DMA0);
				
	csi_dma_ch_init(DMA0, byChnl, &tDmaConfig);
	csi_dma_ch_init(DMA0, byChnl1, &tDmaConfig1);

	
	csi_spi_init(SPI0,&t_SpiConfig);					//初始化并启动spi
	
	
	csi_spi_nss_low(PA4);
	csi_dma_ch_start(DMA0, byChnl, (void *)bySrcBuf, (void *)&(SPI0->DR), 17,1);//send
	csi_dma_ch_start(DMA0, byChnl1, (void *)&(SPI0->DR), (void *)byDesBuf,17,1);//receive
	while( (SPI0->SR & SPI_BSY) );
	csi_spi_nss_high(PA4);
	
	while(1)
	{
		nop;
		nop;
		nop;
	}

}

void spi_etcb_dma_send_receive(void)
{	
	uint8_t byChnl = 0;
	uint8_t byChnl1 = 1;
	//volatile uint8_t byTemp = 0;
	//volatile uint8_t bySrcBuf[104] = {0x03,0x00,0x10,0x00,1, 2 ,3, 4, 5, 6, 7,8,9,10,11,12,13,14,15,16,17};
	volatile uint8_t bySrcBuf[104] = {0x00,0x03,0x00,0x10,1, 2 ,3, 4, 5, 6, 7,8,9,10,11,12,13,14,15,16,17};
	volatile uint8_t byDesBuf[104] = {0};
	csi_dma_ch_config_t tDmaConfig;
	csi_dma_ch_config_t tDmaConfig1;
	csi_spi_config_t t_SpiConfig;   //spi初始化参数配置结构体
	csi_etb_config_t 	tEtbConfig;		//ETB 参数配置结构体	
	csi_etb_config_t 	tEtbConfig1;		//ETB 参数配置结构体	
	
	for(uint8_t i=4;i<104;i++)
	{
		bySrcBuf[i]=i-3;
	}
	
	//send dma para config
	//tDmaConfig.bySrcLinc 	= DMA_ADDR_INC;				//低位传输原地址自增
	//tDmaConfig.bySrcHinc 	= DMA_ADDR_CONSTANT;		//高位传输原地址固定不变
	tDmaConfig.bySrcLinc 	= DMA_ADDR_CONSTANT;		//低位传输原地址固定不变
	tDmaConfig.bySrcHinc 	= DMA_ADDR_INC;		        //高位传输原地址自增
	tDmaConfig.byDetLinc 	= DMA_ADDR_CONSTANT;		//低位传输目标地址固定不变
	tDmaConfig.byDetHinc 	= DMA_ADDR_CONSTANT;		//高位传输目标地址固定不变
	tDmaConfig.byDataWidth 	= DMA_DSIZE_16_BITS;			//传输数据宽度8bit
	tDmaConfig.byReload 	= DMA_RELOAD_DISABLE;		//禁止自动重载
	tDmaConfig.byTransMode 	= DMA_TRANS_ONCE;			//DMA服务模式(传输模式)，连续服务
	tDmaConfig.byTsizeMode  = DMA_TSIZE_ONE_DSIZE;		//传输数据大小，一个 DSIZE , 即DSIZE定义大小
	tDmaConfig.byReqMode	= DMA_REQ_HARDWARE;			//DMA请求模式，硬件请求（硬件触发）
	tDmaConfig.wInt 		= DMA_INTSRC_NONE;			//不使用中断
	
	//receive dma para config
	tDmaConfig1.bySrcLinc 	= DMA_ADDR_CONSTANT;			//低位传输原地址固定不变
	tDmaConfig1.bySrcHinc 	= DMA_ADDR_CONSTANT;			//高位传输原地址固定不变
	//tDmaConfig1.byDetLinc 	= DMA_ADDR_INC;				//低位传输目标地址自增
	//tDmaConfig1.byDetHinc 	= DMA_ADDR_CONSTANT;		//高位传输目标地址固定不变
	tDmaConfig1.byDetLinc 	= DMA_ADDR_CONSTANT;			//低位传输目标地址固定不变
	tDmaConfig1.byDetHinc 	= DMA_ADDR_INC;					//高位传输目标地址自增
	tDmaConfig1.byDataWidth = DMA_DSIZE_16_BITS;				//传输数据宽度8bit
	tDmaConfig1.byReload 	= DMA_RELOAD_DISABLE;			//禁止自动重载
	tDmaConfig1.byTransMode = DMA_TRANS_ONCE;				//DMA服务模式(传输模式)，连续服务
	tDmaConfig1.byTsizeMode  = DMA_TSIZE_ONE_DSIZE;			//传输数据大小，一个 DSIZE , 即DSIZE定义大小
	tDmaConfig1.byReqMode	= DMA_REQ_HARDWARE;				//DMA请求模式，硬件请求
	tDmaConfig1.wInt		= DMA_INTSRC_NONE;				//不使用中断
	
	//send etcb para config
	tEtbConfig.byChType = ETB_ONE_TRG_ONE_DMA;			//单个源触发单个目标，DMA方式
	tEtbConfig.bySrcIp 	= ETB_SPI0_TXSRC;				//SPI0 TXSRC作为触发源
	tEtbConfig.bySrcIp1 = 0xff;						
	tEtbConfig.bySrcIp2 = 0xff;
	tEtbConfig.byDstIp 	= ETB_DMA0_CH0;					//ETB DMA0通道0作为目标实际
	tEtbConfig.byDstIp1 = 0xff;
	tEtbConfig.byDstIp2 = 0xff;
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;			//通道触发模式采样软件触发
	
	//receive etcb para config
	tEtbConfig1.byChType = ETB_ONE_TRG_ONE_DMA;			//单个源触发单个目标，DMA方式
	tEtbConfig1.bySrcIp 	= ETB_SPI0_RXSRC;				//SPI0 RXSRC作为触发源
	tEtbConfig1.bySrcIp1 = 0xff;						
	tEtbConfig1.bySrcIp2 = 0xff;
	tEtbConfig1.byDstIp 	= ETB_DMA0_CH1;					//ETB DMA0通道0作为目标实际
	tEtbConfig1.byDstIp1 = 0xff;
	tEtbConfig1.byDstIp2 = 0xff;
	tEtbConfig1.byTrgMode = ETB_HARDWARE_TRG;			//通道触发模式采样软件触发
	
	//spi主机端口配置
	csi_pin_set_mux(PA4, PA4_OUTPUT);                         //gpio_port as output
	csi_pin_output_mode(PA4, GPIO_PUSH_PULL);                 //push pull mode
	csi_spi_nss_high(PA4);									  //NSS init high			    
	csi_pin_set_mux(PA5,PA5_SPI0_SCK);						  //PA5 = SPI_SCK
	csi_pin_set_mux(PA6,PA6_SPI0_MISO);						  //PA6 = SPI_MISO
	csi_pin_set_mux(PA7,PA7_SPI0_MOSI);						  //PA7 = SPI_MOSI
	
	//spi para config
	t_SpiConfig.eSpiMode = SPI_MASTER;						//作为主机
	t_SpiConfig.eSpiPolarityPhase = SPI_FORMAT_CPOL0_CPHA0; //clk空闲电平为0，相位为在第二个边沿采集数据
	t_SpiConfig.eSpiFrameLen = SPI_FRAME_LEN_16;             //帧数据长度为8bit
	t_SpiConfig.dwSpiBaud = 16000000; 						//通讯速率3兆			
    t_SpiConfig.eSpiRxFifoLevel = SPI_RXFIFO_1_2;  			//接收fifo中断阈值，1/2*8=4个
	t_SpiConfig.byInter = (uint8_t)SPI_NONE_INT;			//初始配置无中断	  
	
	
	csi_etb_init();									//使能ETB模块
	csi_etb_ch_config(ETB_CH20, &tEtbConfig);		//初始化ETB，DMA ETB CHANNEL 大于 ETB_CH19_ID
	csi_etb_ch_config(ETB_CH21, &tEtbConfig1);		//初始化ETB，DMA ETB CHANNEL 大于 ETB_CH19_ID
	
	csi_dma_soft_rst(DMA0);
	csi_dma_ch_init(DMA0, byChnl, &tDmaConfig);
	csi_dma_ch_init(DMA0, byChnl1, &tDmaConfig1);

	
	csi_spi_init(SPI0,&t_SpiConfig);					//初始化并启动spi
	
	
	csi_spi_nss_low(PA4);
	csi_spi_recv_dma(SPI0,(void *)byDesBuf,104,DMA0, byChnl1);
	csi_spi_send_dma(SPI0,(void *)bySrcBuf,104,DMA0, byChnl);

	csp_dma_t *ptDmaChBase_01 = (csp_dma_t *)DMA_REG_BASE(DMA0, 1);
	csp_dma_t *ptDmaChBase_00 = (csp_dma_t *)DMA_REG_BASE(DMA0, 0);
	
	while(csp_dma_get_curr_htc(ptDmaChBase_00));	//等待直到dma发送完成
	while(csp_dma_get_curr_htc(ptDmaChBase_01));	//等待直到dma接收完成
	while( (SPI0->SR & SPI_BSY) );					//等到spi传输完成
	csi_spi_nss_high(PA4);

	
	while(1)
	{
		csi_spi_nss_low(PA4);
		csi_spi_recv_dma(SPI0,(void *)byDesBuf,104,DMA0, 1);
		csi_spi_send_dma(SPI0,(void *)bySrcBuf,104,DMA0, 0);
		while(csp_dma_get_curr_htc(ptDmaChBase_00));			//等待直到dma发送完成
		while(csp_dma_get_curr_htc(ptDmaChBase_01));			//等待直到dma接收完成
		while( (SPI0->SR & SPI_BSY) );							//等到spi传输完成
		csi_spi_nss_high(PA4);
		nop;
	}

}