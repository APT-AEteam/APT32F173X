/***********************************************************************//** 
 * \file  iic_demo.c
 * \brief IIC_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-6-7 <td>V0.0  <td>XB     <td>initial
 * <tr><td> 2023-3-15 <td>V0.1  <td>YT     <td>modify
 * <tr><td> 2023-9-14 <td>V0.2  <td>YT     <td>modify
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include "csi_drv.h"
#include "board_config.h"	
	
csi_iic_master_config_t  g_tIicMasterCfg;	//主机初始化结构体变量
csi_iic_slave_config_t  g_tIicSlaveCfg;	//从机初始化结构体变量

volatile static uint8_t s_bySendBuffer[32]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
volatile static uint8_t s_byWriteBuffer[32];

volatile uint8_t g_wTxBuff[32] = {0,1,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};// 前两个为wWriteAddrs
volatile uint8_t g_bRxBuff[32] = {0};

volatile uint32_t g_wIicErrorCont = 0;
volatile uint8_t g_bySendIndex = 0;
volatile uint8_t g_byWriteIndex = 0;
volatile uint32_t g_wIicSlaveWriteAddress;
csi_iic_ctrl_t g_tIicCtrl[IIC_IDX];


#if (USE_IIC_CALLBACK == 0)

/** \brief	iic_int_handler: IIC中断服务函数
 * 
 *  \brief 	IIC发生中断时会调用此函数，函数在interrupt.c里定义为弱(weak)属性，默认不做处理；用户用到IIC中
 * 			断时，请重新定义此函数，在此函数中进行对应中断处理，也可直接在interrupt.c里的函数里进行处理
 * 
 *  \param[in] none
 *  \return none
 */
ATTRIBUTE_ISR  void iic_int_handler(void)
{
	// 用户直接在中断服务接口函数里处理中断，建议客户使用此模式
	// USER TODO
	
	//receive buffer 接收处理程序  添加
	
	if(csp_iic_get_isr(IIC0)&IIC_INT_RX_FULL)        //receive data full
	{
		if(g_bySendIndex == 0)
		{
			g_bySendIndex = 1;
			g_wIicSlaveWriteAddress = csp_iic_get_data(IIC0);
			g_byWriteIndex = g_wIicSlaveWriteAddress;
		}
		else
		{
			if(g_wIicSlaveWriteAddress<g_tSlave.hwRxSize)
			{
				*(g_tSlave.pbySlaveRxBuf+g_wIicSlaveWriteAddress)= csp_iic_get_data(IIC0);
			}
			g_wIicSlaveWriteAddress++;
		}
			csp_iic_clr_isr(IIC0,IIC_INT_RX_FULL);
			g_wIicErrorCont=0;
		}
}

#endif

/** \brief IIC master eeprom demo
 * IIC主机向eeprom中写数据，并且读取eeprom中的数据
 *  \param[in] none
 *  \return none
 */
void iic_master_eeprom_demo(void)
{
	volatile uint8_t data[9] = {1,2,3,4,5,6,7,8,9};
	volatile uint8_t data1[9] = {0};
	
#if (USE_GUI == 0)		
	csi_gpio_output_mode(GPIOC, PC4, GPIO_OPEN_DRAIN);
	csi_gpio_output_mode(GPIOC, PC5, GPIO_OPEN_DRAIN);
	csi_gpio_set_mux(GPIOC, PC4, PC4_IIC_SCL);
	csi_gpio_set_mux(GPIOC, PC5,PC5_IIC_SDA);
	csi_gpio_pull_mode(GPIOC, PC4, GPIO_PULLUP);	
	csi_gpio_pull_mode(GPIOC, PC5, GPIO_PULLUP);	  
#endif	

	g_tIicMasterCfg.eAddrMode = IIC_ADDRESS_7BIT;			//设置主机地址模式 7/10 bit
	g_tIicMasterCfg.bReStart = ENABLE;						//使能重复起始位
	g_tIicMasterCfg.eSpeedMode = IIC_BUS_SPEED_STANDARD;	//设置主机速度模式	IIC_BUS_SPEED_STANDARD <=100kHz   IIC_BUS_SPEED_FAST <=400kHz    IIC_BUS_SPEED_FAST_PLUS <=  1MHz
	g_tIicMasterCfg.wSdaTimeout = 0XFFFF;					//SDA 超时时间设置，  1/主频 * g_tIicMasterCfg.wSdaTimeout  ms
	g_tIicMasterCfg.wSclTimeout = 0XFFFF;					//SCL 超时时间设置，  1/主频 * g_tIicMasterCfg.wSdaTimeout  ms
	csi_iic_master_init(IIC0,&g_tIicMasterCfg);				//主机初始化
	
	//该例程为主机读写AT24C04的EEPROM的示例，因AT24C04每次写入的数据不能超过16byte
	//请使用时注意写的长度。不同的EEPROM会有不同的写入长度限制，请参考相应的数据手册
	while(1)
	{
		csi_iic_master_write_nbyte(IIC0,0xa0,0x0001,2,&data[0],9);
		mdelay(100);
		csi_iic_master_read_nbyte(IIC0,0xa0,0x0001,2,&data1[0],9);
		
		mdelay(100);
	}
}

/** \brief IIC master demo
 * IIC主机程序，读取从机中的数据
 *  \param[in] none
 *  \return none
 */
void iic_master_demo(void)
{
	
	volatile uint8_t data1[19] = {4,4,4,4,4,4,4,4,4};

#if (USE_GUI == 0)		
	csi_gpio_output_mode(GPIOC, PC4, GPIO_OPEN_DRAIN);
	csi_gpio_output_mode(GPIOC, PC5, GPIO_OPEN_DRAIN);
	csi_gpio_set_mux(GPIOC, PC4, PC4_IIC_SCL);
	csi_gpio_set_mux(GPIOC, PC5,PC5_IIC_SDA);
	csi_gpio_pull_mode(GPIOC, PC4, GPIO_PULLUP);	
	csi_gpio_pull_mode(GPIOC, PC5, GPIO_PULLUP);
#endif	

	g_tIicMasterCfg.eAddrMode = IIC_ADDRESS_7BIT;			//设置主机地址模式 7/10 bit
	g_tIicMasterCfg.bReStart = ENABLE;						//使能重复起始位
	g_tIicMasterCfg.eSpeedMode = IIC_BUS_SPEED_FAST_PLUS;	//设置主机速度模式	IIC_BUS_SPEED_STANDARD <=100kHz   IIC_BUS_SPEED_FAST <=400kHz    IIC_BUS_SPEED_FAST_PLUS <=  1MHz
	g_tIicMasterCfg.wSdaTimeout = 0XFFFF;					//SDA 超时时间设置，  1/主频 * g_tIicMasterCfg.wSdaTimeout  ms
	g_tIicMasterCfg.wSclTimeout = 0XFFFF;					//SCL 超时时间设置，  1/主频 * g_tIicMasterCfg.wSdaTimeout  ms
	csi_iic_master_init(IIC0,&g_tIicMasterCfg);				//主机初始化
	
	while(1)
	{
		for(int i = 0;i!=19;i++)
		{
			data1[i] = 0;
		}
		csi_iic_master_read_nbyte(IIC0,0xa0,0x01,1,&data1[0],19);
		mdelay(1000);
		
	}
}


/** \brief iic slave demo  
 *  接收主机发送的数据，并且发送数据给主机
 *  \param[in] none
 *  \return none
 */
void iic_slave_demo(void)
{
#if (USE_GUI == 0)		
	csi_gpio_output_mode(GPIOC, PC4, GPIO_OPEN_DRAIN);
	csi_gpio_output_mode(GPIOC, PC5, GPIO_OPEN_DRAIN);
	csi_gpio_set_mux(GPIOC, PC4, PC4_IIC_SCL);
	csi_gpio_set_mux(GPIOC, PC5,PC5_IIC_SDA);
	csi_gpio_pull_mode(GPIOC, PC4, GPIO_PULLUP);	
	csi_gpio_pull_mode(GPIOC, PC5, GPIO_PULLUP);
#endif		

	g_tIicSlaveCfg.eAddrMode = IIC_ADDRESS_7BIT;		//设置从机地址模式 
	g_tIicSlaveCfg.eSpeedMode = IIC_BUS_SPEED_STANDARD;	//设置从机速度	IIC_BUS_SPEED_STANDARD <=100kHz   IIC_BUS_SPEED_FAST <=400kHz    IIC_BUS_SPEED_FAST_PLUS <=  1MHz
	g_tIicSlaveCfg.hwSlaveAddr = 0xa0;				//设置从机地址
	g_tIicSlaveCfg.hwMaskAddr = 0x00;           //配置SLVQUAL
	g_tIicSlaveCfg.eQualMode=I2C_QUALMASK;        //从机地址限定模式， 地址扩展模式
	csi_iic_int_enable(IIC0, IIC_INTSRC_SCL_SLOW | IIC_INTSRC_STOP_DET | IIC_INTSRC_RD_REQ | IIC_INTSRC_RX_FULL | IIC_INTSRC_TX_ABRT|IIC_INTSRC_TX_OVER);   //使能需要的中断
//	csi_iic_set_slave_buffer(g_byWriteBuffer,32,g_bySendBuffer,32); //从机就是数组和发送数组设置
	csi_iic_slave_init(IIC0,&g_tIicSlaveCfg);		//初始化从机
	
	while(1);
	
}



/** \brief iic multiple slave address demo  
 *  接收主机发送的数据，并且发送数据给主机
 *  \param[in] none
 *  \return none
 */

void iic_multi_slave_demo(void)
{
//	uint32_t maskaddr = 0x70;           //配置SLVQUAL  放到结构体里面
#if (USE_GUI == 0)		
	csi_gpio_output_mode(GPIOC, PC4, GPIO_OPEN_DRAIN);
	csi_gpio_output_mode(GPIOC, PC5, GPIO_OPEN_DRAIN);
	csi_gpio_set_mux(GPIOC, PC4, PC4_IIC_SCL);
	csi_gpio_set_mux(GPIOC, PC5,PC5_IIC_SDA);
	csi_gpio_pull_mode(GPIOC, PC4, GPIO_PULLUP);	
	csi_gpio_pull_mode(GPIOC, PC5, GPIO_PULLUP);
#endif		

	g_tIicSlaveCfg.eAddrMode = IIC_ADDRESS_7BIT;		//设置从机地址模式 
	g_tIicSlaveCfg.eSpeedMode = IIC_BUS_SPEED_STANDARD;	//设置从机速度	IIC_BUS_SPEED_STANDARD <=100kHz   IIC_BUS_SPEED_FAST <=400kHz    IIC_BUS_SPEED_FAST_PLUS <=  1MHz
	g_tIicSlaveCfg.hwSlaveAddr = 0x30;				//设置从机地址,SADDR>>1
	g_tIicSlaveCfg.hwMaskAddr = 0x70;           //配置SLVQUAL
	g_tIicSlaveCfg.eQualMode=I2C_QUALEXTEND;        //从机地址限定模式， 地址扩展模式
//	csi_iic_set_slave_buffer(s_byWriteBuffer,32,s_bySendBuffer,32); //从机就是数组和发送数组设置
	g_tIicMasterCfg.wSdaTimeout = 0XFFFF;						//SDA 超时时间设置，  1/主频 * g_tIicMasterCfg.wSdaTimeout  ms
	g_tIicMasterCfg.wSclTimeout = 0XFFFF;						//SCL 超时时间设置，  1/主频 * g_tIicMasterCfg.wSdaTimeout  ms
	csi_iic_int_enable(IIC0, IIC_INTSRC_SCL_SLOW | IIC_INTSRC_STOP_DET |IIC_INTSRC_RD_REQ | IIC_INTSRC_RX_FULL | IIC_INTSRC_TX_ABRT);     //使能需要的中断
	
	csi_iic_slave_init(IIC0,&g_tIicSlaveCfg);		//初始化从机
	
	
	
	
//	csi_iic_qualmode_set(IIC0,IIC_QUAL_EXTEND);   //QUALMODE=1,地址扩展模式
////	csi_iic_qualmode_set(IIC0,IIC_QUAL_MASK);    //QUALMODE=0,地址屏蔽模式模式
//
//	csp_iic_set_slvqual(IIC0,maskaddr);
	
	while(1);
}

/** \brief IIC DMA tx demo
 * 
 *  \param[in] none
 *  \return none
 */
 void iic_dma_send_demo(void)
{
	
	csi_dma_ch_config_t tDmaConfig;    
	csi_etcb_config_t  tEtbConfig;
	
#if (USE_GUI == 0)		
	csi_gpio_output_mode(GPIOC, PC4, GPIO_OPEN_DRAIN);
	csi_gpio_output_mode(GPIOC, PC5, GPIO_OPEN_DRAIN);
	csi_gpio_set_mux(GPIOC, PC4, PC4_IIC_SCL);
	csi_gpio_set_mux(GPIOC, PC5,PC5_IIC_SDA);
	csi_gpio_pull_mode(GPIOC, PC4, GPIO_PULLUP);	
	csi_gpio_pull_mode(GPIOC, PC5, GPIO_PULLUP);
#endif		

	// tx etcb config

	tEtbConfig.eChType = ETCB_ONE_TRG_ONE_DMA;     //单个源触发单个目标，DMA方式
	tEtbConfig.eSrcIp  = ETCB_IIC0_TXSRC;		   //IIC0_TXSRC作为触发源

	tEtbConfig.eDstIp  = ETCB_DMA0_CH0 ;           //ETCB DMA通道 作为目标实际

	tEtbConfig.eTrgMode = ETCB_HARDWARE_TRG;       //通道触发模式采样硬件触发
	csi_etcb_ch_init(ETCB_CH21, &tEtbConfig);      //初始化ETCB，DMA ETCB CHANNEL > ETCB_CH19_ID 

	
	
	//iic tx dma config
	tDmaConfig.eSrcLinc  = DMA_ADDR_INC;  			//低位传输原地址固定不变
	tDmaConfig.eSrcHinc  = DMA_ADDR_INC;		    //高位传输原地址自增
	tDmaConfig.eDetLinc  = DMA_ADDR_CONSTANT;  		//低位传输目标地址固定不变
	tDmaConfig.eDetHinc  = DMA_ADDR_CONSTANT; 		//高位传输目标地址固定不变
	tDmaConfig.eDataWidth  = DMA_DSIZE_32_BITS;   	//传输数据宽度8bit
	tDmaConfig.eReload  = DMA_RELOAD_DISABLE;  	    //禁止自动重载
	tDmaConfig.eTransMode  = DMA_TRANS_ONCE;		//DMA服务模式(传输模式)，连续服务
	tDmaConfig.eTsizeMode  = DMA_TSIZE_ONE_DSIZE;   //传输数据大小，一个 DSIZE , 即DSIZE定义大小
	tDmaConfig.eReqMode = DMA_REQ_HARDWARE;   		//DMA请求模式，软件请求（软件触发）		
	csi_dma_ch_init(DMA0, DMA_CH0, &tDmaConfig);    //初始化DMA
	
	csi_dma_ch_start(DMA0, DMA_CH0, (void *)g_wTxBuff, (void *)0x400A0010,9,1);

	// iic congif
	g_tIicMasterCfg.eAddrMode = IIC_ADDRESS_7BIT;			//设置主机地址模式 7/10 bit
	g_tIicMasterCfg.bReStart = ENABLE;						//使能重复起始位
	g_tIicMasterCfg.eSpeedMode = IIC_BUS_SPEED_STANDARD;	//设置主机速度模式	IIC_BUS_SPEED_STANDARD <=100kHz   IIC_BUS_SPEED_FAST <=400kHz    IIC_BUS_SPEED_FAST_PLUS <=  1MHz
	g_tIicMasterCfg.wSdaTimeout = 0XFFFFFFF;				//SDA 超时时间设置，  1/主频 * g_tIicMasterCfg.wSdaTimeout  ms
	g_tIicMasterCfg.wSclTimeout = 0XFFFFFFF;				//SCL 超时时间设置，  1/主频 * g_tIicMasterCfg.wSdaTimeout  ms
	csi_iic_master_init(IIC0,&g_tIicMasterCfg);				//主机初始化
	csi_iic_set_receive_dma(IIC0, IIC_RDMA_EN, IIC_RDMA_FIFO_NSPACE);
	csi_iic_set_send_dma(IIC0, IIC_TDMA_EN, IIC_TDMA_FIF0_TX_FLSEL); 
	
	g_wTxBuff[8] |=  IIC_CMD_STOP;     // 最后一个数添加stop
	
	csi_iic_disable(IIC0);
	csi_iic_set_taddr(IIC0,0xa0 >> 1);
	
	csi_iic_enable(IIC0);
	while(1);
}



/** \brief IIC DMA rx demo
 * 
 *  \param[in] none
 *  \return none
 */
 void iic_dma_receive_demo(void)
{
	
	csi_dma_ch_config_t tDmaConfig;    
	csi_etcb_config_t  tEtbConfig;
#if (USE_GUI == 0)		
	csi_gpio_output_mode(GPIOC, PC4, GPIO_OPEN_DRAIN);
	csi_gpio_output_mode(GPIOC, PC5, GPIO_OPEN_DRAIN);
	csi_gpio_set_mux(GPIOC, PC4, PC4_IIC_SCL);
	csi_gpio_set_mux(GPIOC, PC5,PC5_IIC_SDA);
	csi_gpio_pull_mode(GPIOC, PC4, GPIO_PULLUP);	
	csi_gpio_pull_mode(GPIOC, PC5, GPIO_PULLUP);
#endif

	// rx etcb config
	tEtbConfig.eChType = ETCB_ONE_TRG_ONE_DMA;     //单个源触发单个目标，DMA方式
	tEtbConfig.eSrcIp  = ETCB_IIC0_RXSRC;		   //IIC0_TXSRC作为触发源
	tEtbConfig.eDstIp  = ETCB_DMA0_CH1;            //ETCB DMA通道 作为目标实际
	tEtbConfig.eTrgMode = ETCB_HARDWARE_TRG;       //通道触发模式采样硬件触发	
	csi_etcb_ch_init(ETCB_CH22, &tEtbConfig);      //初始化ETCB，DMA ETCB CHANNEL > ETCB_CH19_ID 
	
	// ic rx dma config
	tDmaConfig.eSrcLinc  = DMA_ADDR_CONSTANT;  	   //低位传输原地址固定不变
	tDmaConfig.eSrcHinc  = DMA_ADDR_CONSTANT;	   //高位传输原地址自增
	tDmaConfig.eDetLinc  = DMA_ADDR_INC;  		   //低位传输目标地址固定不变
	tDmaConfig.eDetHinc  = DMA_ADDR_INC; 		   //高位传输目标地址固定不变
	tDmaConfig.eDataWidth  = DMA_DSIZE_8_BITS;     //传输数据宽度8bit
	tDmaConfig.eReload  = DMA_RELOAD_DISABLE;  	   //禁止自动重载
	tDmaConfig.eTransMode  = DMA_TRANS_ONCE;	   //DMA服务模式(传输模式)，连续服务
	tDmaConfig.eTsizeMode  = DMA_TSIZE_ONE_DSIZE;  //传输数据大小，一个 DSIZE , 即DSIZE定义大小
	tDmaConfig.eReqMode = DMA_REQ_HARDWARE;   	   //DMA请求模式，软件请求（软件触发）
	csi_dma_ch_init(DMA0, DMA_CH1, &tDmaConfig);   //初始化DMA
	
	csi_dma_ch_start(DMA0, DMA_CH1, (void *)0x400A0010, (void *)g_bRxBuff,9,1);

	// iic config
	g_tIicMasterCfg.eAddrMode = IIC_ADDRESS_7BIT;			//设置主机地址模式 7/10 bit
	g_tIicMasterCfg.bReStart = ENABLE;						//使能重复起始位
	g_tIicMasterCfg.eSpeedMode = IIC_BUS_SPEED_STANDARD;	//设置主机速度模式	IIC_BUS_SPEED_STANDARD <=100kHz   IIC_BUS_SPEED_FAST <=400kHz    IIC_BUS_SPEED_FAST_PLUS <=  1MHz
	g_tIicMasterCfg.wSdaTimeout = 0XFFFFFFF;				//SDA 超时时间设置，  1/主频 * g_tIicMasterCfg.wSdaTimeout  ms
	g_tIicMasterCfg.wSclTimeout = 0XFFFFFFF;				//SCL 超时时间设置，  1/主频 * g_tIicMasterCfg.wSdaTimeout  ms
	csi_iic_master_init(IIC0,&g_tIicMasterCfg);				//主机初始化
	
	csi_iic_set_receive_dma(IIC0, IIC_RDMA_EN, IIC_RDMA_FIFO_NSPACE);
	csi_iic_set_send_dma(IIC0, IIC_TDMA_EN, IIC_TDMA_FIF0_TX_FLSEL);
	
	csi_iic_int_enable(IIC0, IIC_INTSRC_NONE);     //使能需要的中断
	csi_iic_read_nbyte_dma(IIC0,0xa0,0x0001,2,&g_bRxBuff[0],9);        
	
	while(1);
	
	

}

