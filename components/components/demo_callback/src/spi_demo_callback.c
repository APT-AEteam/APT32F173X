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

#if (USE_SPI_CALLBACK == 1)	
static uint8_t s_bySendBuf[16]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};	
static uint8_t s_byRecvBuf[16]={0};	

/** \brief  user_send_callback：SPI中断发送回调函数
 * 
 * 	\brief	用户定义，使用csi标准库进行中断发送时，发送完毕后回自动调用用户注册的回调函数；用户可在回调
 * 			函数里做自己的处理，而不需要关注具体的底层中断处理。
 * 		
 *  \param[in] ptSpiBase: SPIx寄存器结构体指针，指向SPIx的基地址 
 *  \return none
 */
static void	user_send_callback(csp_spi_t *ptSpiBase)
{
	//添加用户处理
	nop;
}

static void user_receive_callback(csp_spi_t *ptSpiBase, csi_spi_state_e eState, uint8_t *pbyBuf, uint32_t *wLen)
{
	switch(eState) 
	{
		case SPI_STATE_RX_DNE:								//指定长度接收
			//添加用户处理
			csi_spi_receive_int(ptSpiBase, s_byRecvBuf, 16);//重新开启一次新的接收,接收长度可以重新指定
			break;
			
		case SPI_STATE_RX_TO:								//动态长度接收
			//添加用户处理
			break;
			
		default:
			break;
	}
}

/** \brief spi master send demo,interrupt(async) mode
 * 	\brief spi 主机发送一串数据,TX使用中断
 *  \param[in] none
 *  \return error code
 */
 int spi_master_send_int_callback_demo(void)
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
	
	tSpiCfg.eMode 			= SPI_MODE_MASTER;		//SPI作为主机
	tSpiCfg.eWorkMode 		= SPI_WORK_MODE_0; 		//SPI Mode0:CPOL=0,CPOH=0
	tSpiCfg.eDataLen 		= SPI_DATA_LEN_8;  		//SPI数据帧长度为8bit
	tSpiCfg.wSpiBaud 		= 1000000; 				//SPI通讯速率1Mbps			
	csi_spi_init(SPI0,&tSpiCfg);
	
	//注册SPI0发送中断回调函数
	csi_spi_register_callback(SPI0, SPI_CALLBACK_SEND, user_send_callback);
	csi_spi_start(SPI0);
	while(1)
	{
		csi_spi_send_int(SPI0, s_bySendBuf, sizeof(s_bySendBuf));
		mdelay(10);		
	}
	return iRet;
}

/*  \brief spi slave receive a bunch of data; interrupt(async) mode
 *  \brief spi 从机接收一串数据，RX使用中断
 *  \param[in] none
 *  \return error code
 */
int spi_slave_receive_int_callback_demo(void)
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
	tSpiCfg.eRxFifoTrg	= SPI_RXFIFOTRG_FOUR;	//SPI接收FIFO的中断触发点设置为4，即RXFIFO接收到>=4个数据,就会触发RXFIFO中断；支持三种配置(1/2/4)		
	csi_spi_init(SPI0,&tSpiCfg);				

	//注册SPI0接收中断回调函数
	csi_spi_register_callback(SPI0, SPI_CALLBACK_RECV, user_receive_callback);
	csi_spi_start(SPI0);
	
	csi_spi_receive_int(SPI0, s_byRecvBuf, 16);
	
	while(1)
	{	
		nop;
	}
	return iRet;
}
#endif