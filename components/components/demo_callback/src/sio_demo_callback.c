/***********************************************************************//** 
 * \file  sio_demo.c
 * \brief  SIO_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-8-12 <td>V0.0 <td>ZJY     <td>initial
 * <tr><td> 2023-9-18 <td>V0.1 <td>ZJY     <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include "csi_drv.h"
#include "board_config.h"

#if (USE_SIO_CALLBACK == 1)	

/* Private macro-----------------------------------------------------------*/


/* Private function--------------------------------------------------------*/
static void set_led_rgb_store(uint32_t *pwLeddData, uint8_t *pbyDisp, uint16_t hwLedNum);


/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/
static uint8_t	s_byDipData[24] = 
{
	//R		G		B
	0x03,	0x07,	0x0a,	//rgb1
	0x04,	0x07,	0x05,	//rgb2
	0x05,	0x07,	0x01,	//rgb3
	0x06,	0x07,	0x02,	//rgb4
	0x07,	0x07,	0x03,	//rgb5
	0x08,	0x07,	0x04,	//rgb6
	0x09,	0x07,	0x05,	//rgb7
	0x0A,	0x07,	0x06 	//rgb8
};

/** \brief  user_send_callback：SIO中断发送回调函数
 * 
 * 	\brief	用户定义，使用csi标准库进行中断发送时，发送完毕后回自动调用用户注册的回调函数；用户可在回调
 * 			函数里做自己的处理，而不需要关注具体的底层中断处理。
 * 		
 *  \param[in] ptSioBase: 	SIOx寄存器结构体指针，指向SIOx的基地址 
 *  \return none
 */
static void	user_send_callback(csp_sio_t *ptSioBase)
{
	//添加用户处理
	nop;
}

/** \brief 	sio_led_rgb_int_callback_demo：SIO驱动RGB三色LED的demo, 使用callback
 * 
 *  \brief	sio 驱动RGB LED(ws2812), RGB DATA = 24bit; 驱动数据输出排列方式:GRB; 采用中断方式(TXBUFEMPT)
 * 
 *  \param[in] none
 *  \return error code
 */
int sio_led_rgb_int_callback_demo(void)
{
	int iRet = 0;
	uint8_t byCount = 0;
	uint32_t wDipDataEnd[24];
	csi_sio_tx_config_t tSioTxCfg;

#if (USE_GUI == 0)									//用户未选择图形化编程	
	csi_gpio_set_mux(GPIOB, PB0, PB0_SIO0);	
#endif

	//SIO TX 参数配置
	tSioTxCfg.byD0Len 		= 1;					//D0 对象序列长度(bit个数)，这里不用D0								
	tSioTxCfg.byD1Len 		= 1;					//D1 对象序列长度(bit个数)，这里不用D1
	tSioTxCfg.byDLLen 		= 4;					//DL 对象序列长度(bit个数)
	tSioTxCfg.byDHLen 		= 4;					//DH 对象序列长度(bit个数)
	tSioTxCfg.byDLLsq 		= 0x01;					//DL 对象序列具体定义: bit= 0  -     
													//					          | |___|					
	tSioTxCfg.byDHHsq 		= 0x07;					//DH 对象序列具体定义: bit= 1  ___
													//					          |   |_| 	
	tSioTxCfg.byTxBufLen 	= 8;					//发送数据缓存长度(bit个数 = 8)，txbuf 一次发送bit数量，len <= 16
	tSioTxCfg.byTxCnt 		= 8;					//SIO一次发送总的数据长度(bit个数 = 8)，byTxCnt >= byTxBufLen，byTxCnt < 256(最大32bytes)
	tSioTxCfg.eIdleLev 		= SIO_IDLE_LOW;			//SIO空闲时刻IO管脚输出电平
	tSioTxCfg.eTxDir 		= SIO_TXDIR_LSB;		//MSB->LSB, txbuf 数据按照bit[1:0]...[31:30]方式移出
	tSioTxCfg.wTxFreq 		= 4000000;				//tx clk =4MHz, Ttxshift = 1/4 = 250ns；发送每bit(1/0)是250ns*4 = 1us
	csi_sio_tx_init(SIO0, &tSioTxCfg);
	
	//注册SIO0发送中断回调函数
	csi_sio_register_callback(SIO0, SIO_CALLBACK_SEND, user_send_callback);
	
	//数据转换
	for(byCount = 0; byCount < 8; byCount++ )
	{
		set_led_rgb_store(wDipDataEnd, s_byDipData, byCount);
	}
	
	while(1)
	{
		csi_sio_send_int(SIO0, wDipDataEnd, 24);	//数据发送
		mdelay(10);
	}
	
	return iRet;
}

/** \brief 	sio_led_data_conver: 数据转换为SIO格式
 * 
 *  \param[in] byData：需要转换的数据
 *  \return  SIO格式数据
 */
static uint32_t sio_led_data_conversion(uint8_t byData)
{
	uint8_t i;
	uint32_t wData = 0xaaaa;
	for(i = 0; i < 8; i++)
	{
		if(byData & 0x80)
			wData |= (0x01 << (2*i));	
		byData = byData << 1;
	}
	return wData;
}

/** \brief 	set_led_rgb_store:  RGB LED const数组中数据转换装载
 * 
 *  \param[in] pwLeddData：转换后的数据buf指针
 *  \param[in] pbyDisp：需要转换的RGB LED 数组数据
 *  \param[in] hwLedNum：转换的数量
 *  \return  无
 */
static void set_led_rgb_store(uint32_t *pwLedData, uint8_t *pbyDisp, uint16_t hwLedNum)
{
	//led
	*(pwLedData+hwLedNum*3) = sio_led_data_conversion(pbyDisp[hwLedNum*3+1]) ;		//G
	*(pwLedData+hwLedNum*3+1) = sio_led_data_conversion(pbyDisp[hwLedNum*3]);		//R		
	*(pwLedData+hwLedNum*3+2) = sio_led_data_conversion(pbyDisp[hwLedNum*3+2]);		//B	
}

/** \brief 	led_rgb_display:  RGB LED数据显示，发送数据(驱动LED)
 * 
 *  \param[in] pwLeddData：要显示的RGB LED 数据buf指针
 *  \param[in] hwLedNum：LED数量
 *  \return  无
 */
void led_rgb_display(uint8_t *byColData, uint16_t hwLedNum)
{
	uint16_t i;
	uint32_t wRgbData[24];
	for(i = 0; i < hwLedNum; i++)
	{
		set_led_rgb_store(wRgbData,byColData, i);
	}
	for(i = 0; i < hwLedNum; i++)
	{
		csi_sio_send(SIO0, &wRgbData[3*i],3);
	}
}

#endif
