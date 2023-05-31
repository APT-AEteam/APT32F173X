/***********************************************************************//** 
 * \file  iic_demo.c
 * \brief IIC_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-6-7 <td>V0.0  <td>XB     <td>initial
 * <tr><td> 2023-3-15 <td>V0.1  <td>YT     <td>modify
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include "csp.h"
#include "iic.h"
#include "irq.h"
#include "pin.h"
#include "demo.h"


csi_iic_master_config_t  g_tIicMasterCfg;	//主机初始化结构体变量
csi_iic_slave_config_t  g_tIicSlaveCfg;	//从机初始化结构体变量

volatile uint8_t g_bySendBuffer[32]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
volatile uint8_t g_byWriteBuffer[32];

volatile uint32_t g_wTxBuff[32] = {0,1,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};// 前两个为wWriteAddrs
volatile uint8_t g_bRxBuff[32] = {0};

/** \brief IIC master eeprom demo
 * IIC主机向eeprom中写数据，并且读取eeprom中的数据
 *  \param[in] none
 *  \return error code
 */
void iic_master_eeprom_demo(void)
{
	volatile uint8_t data[9] = {1,2,3,4,5,6,7,8,9};
	volatile uint8_t data1[9] = {0};

	csi_pin_output_mode(PC4,GPIO_OPEN_DRAIN);
	csi_pin_output_mode(PA14,GPIO_OPEN_DRAIN);
	csi_pin_pull_mode(PC4, GPIO_PULLUP);
	csi_pin_pull_mode(PA14, GPIO_PULLUP);
	csi_pin_set_mux(PC4,PC4_I2C_SCL);//PIN2 ->	I2C_SCL
	csi_pin_set_mux(PA14,PA14_I2C_SDA);//PIN3 -> 	I2C_SDA

	g_tIicMasterCfg.byAddrMode = IIC_ADDRESS_7BIT;			//设置主机地址模式 7/10 bit
	g_tIicMasterCfg.byReStart = ENABLE;						//使能重复起始位
	g_tIicMasterCfg.bySpeedMode = IIC_BUS_SPEED_STANDARD;	//设置主机速度模式	IIC_BUS_SPEED_STANDARD <=100kHz   IIC_BUS_SPEED_FAST <=400kHz    IIC_BUS_SPEED_FAST_PLUS <=  1MHz
	g_tIicMasterCfg.hwInt = IIC_INTSRC_NONE ;;					//使能需要的中断
	g_tIicMasterCfg.wSdaTimeout = 0XFFFF;						//SDA 超时时间设置，  1/主频 * g_tIicMasterCfg.wSdaTimeout  ms
	g_tIicMasterCfg.wSclTimeout = 0XFFFF;						//SCL 超时时间设置，  1/主频 * g_tIicMasterCfg.wSdaTimeout  ms
	csi_iic_master_init(I2C0,&g_tIicMasterCfg);				//主机初始化

	//该例程为主机读写AT24C04的EEPROM的示例，因AT24C04每次写入的数据不能超过16byte
	//请使用时注意写的长度。不同的EEPROM会有不同的写入长度限制，请参考相应的数据手册
	while(1)
	{
		csi_iic_write_nbyte(I2C0,0xa0,0x0001,2,&data[0],9);
		mdelay(100);
		csi_iic_read_nbyte(I2C0,0xa0,0x0001,2,&data1[0],9);
		
		mdelay(100);
	}
}

/** \brief IIC master demo
 * IIC主机程序，读取从机中的数据
 *  \param[in] none
 *  \return error code
 */
void iic_master_demo(void)
{
	
	volatile uint8_t data1[19] = {4,4,4,4,4,4,4,4,4};
	
	csi_pin_output_mode(PC4,GPIO_OPEN_DRAIN);
	csi_pin_output_mode(PC5,GPIO_OPEN_DRAIN);
	csi_pin_pull_mode(PC4, GPIO_PULLUP);
	csi_pin_pull_mode(PC5, GPIO_PULLUP);
	csi_pin_set_mux(PC4,PC4_I2C_SCL);//PIN2 ->	I2C_SCL
	csi_pin_set_mux(PC5,PC5_I2C_SDA);//PIN3 -> 	I2C_SDA

	g_tIicMasterCfg.byAddrMode = IIC_ADDRESS_7BIT;			//设置主机地址模式 7/10 bit
	g_tIicMasterCfg.byReStart = ENABLE;						//使能重复起始位
	g_tIicMasterCfg.bySpeedMode = IIC_BUS_SPEED_FAST_PLUS;	//设置主机速度模式	IIC_BUS_SPEED_STANDARD <=100kHz   IIC_BUS_SPEED_FAST <=400kHz    IIC_BUS_SPEED_FAST_PLUS <=  1MHz
	g_tIicMasterCfg.hwInt = IIC_INTSRC_NONE;					//使能需要的中断
	g_tIicMasterCfg.wSdaTimeout = 0XFFFF;						//SDA 超时时间设置，  1/主频 * g_tIicMasterCfg.wSdaTimeout  ms
	g_tIicMasterCfg.wSclTimeout = 0XFFFF;						//SCL 超时时间设置，  1/主频 * g_tIicMasterCfg.wSdaTimeout  ms
	csi_iic_master_init(I2C0,&g_tIicMasterCfg);				//主机初始化

	
	while(1)
	{
		
		for(int i = 0;i!=19;i++)
		{
			data1[i] = 0;
		}
	
		csi_iic_read_nbyte(I2C0,0xa0,0x01,1,&data1[0],19);
		mdelay(1000);
		
	}
}


/** \brief i2c slave demo  
 *	作为从机时需要在IIC中断里调用 i2c_irqhandler(I2C0) 函数；
 * 	如下：
 *	void i2c_int_handler(void) 
 *	{
 *		// ISR content ...
 *		i2c_irqhandler(I2C0);
 *	}
 *  \param[in] none
 *  \return error code
 */
void iic_slave_demo(void)
{
	csi_pin_output_mode(PC4,GPIO_OPEN_DRAIN);
	csi_pin_output_mode(PC5,GPIO_OPEN_DRAIN);
	csi_pin_pull_mode(PC4, GPIO_PULLUP);
	csi_pin_pull_mode(PC5, GPIO_PULLUP);
	csi_pin_set_mux(PC4,PC4_I2C_SCL);//PIN2 ->	I2C_SCL
	csi_pin_set_mux(PC5,PC5_I2C_SDA);//PIN3 -> 	I2C_SDA
	
	g_tIicSlaveCfg.byAddrMode = IIC_ADDRESS_7BIT;		//设置从机地址模式 
	g_tIicSlaveCfg.bySpeedMode = IIC_BUS_SPEED_STANDARD;	//设置从机速度	IIC_BUS_SPEED_STANDARD <=100kHz   IIC_BUS_SPEED_FAST <=400kHz    IIC_BUS_SPEED_FAST_PLUS <=  1MHz
	g_tIicSlaveCfg.hwSlaveAddr = 0xa0;				//设置从机地址
	g_tIicSlaveCfg.hwInt = IIC_INTSRC_SCL_SLOW | IIC_INTSRC_STOP_DET | 
					IIC_INTSRC_RD_REQ | IIC_INTSRC_RX_FULL | IIC_INTSRC_TX_ABRT|IIC_INTSRC_TX_OVER; //使能相应中断
	csi_iic_set_slave_buffer(g_byWriteBuffer,32,g_bySendBuffer,32); //从机就是数组和发送数组设置
	g_tIicMasterCfg.wSdaTimeout = 0XFFFF;						//SDA 超时时间设置，  1/主频 * g_tIicMasterCfg.wSdaTimeout  ms
	g_tIicMasterCfg.wSclTimeout = 0XFFFF;						//SCL 超时时间设置，  1/主频 * g_tIicMasterCfg.wSdaTimeout  ms
	csi_iic_slave_init(I2C0,&g_tIicSlaveCfg);		//初始化从机

	
	while(1);
	
}



/** \brief i2c multiple slave address demo  
 *	作为从机时需要在IIC中断里调用 i2c_irqhandler(I2C0) 函数；
 * 	如下：
 *	void i2c_int_handler(void) 
 *	{
 *		// ISR content ...
 *		i2c_irqhandler(I2C0);
 *	}
 *  \param[in] none
 *  \return error code
 */

void iic_multi_slave_address_demo(void)
{
	csi_pin_output_mode(PC4,GPIO_OPEN_DRAIN);
	csi_pin_output_mode(PC5,GPIO_OPEN_DRAIN);
	csi_pin_set_mux(PC4,PC4_I2C_SCL);//PIN2 ->	I2C_SCL
	csi_pin_set_mux(PC5,PC5_I2C_SDA);//PIN3 -> 	I2C_SDA
	csi_pin_pull_mode(PC4, GPIO_PULLUP);
	csi_pin_pull_mode(PC5, GPIO_PULLUP);
	
	g_tIicSlaveCfg.byAddrMode = IIC_ADDRESS_7BIT;		//设置从机地址模式 
	g_tIicSlaveCfg.bySpeedMode = IIC_BUS_SPEED_STANDARD;	//设置从机速度	IIC_BUS_SPEED_STANDARD <=100kHz   IIC_BUS_SPEED_FAST <=400kHz    IIC_BUS_SPEED_FAST_PLUS <=  1MHz
	g_tIicSlaveCfg.hwSlaveAddr = 0x30;				//设置从机地址,SADDR>>1
	g_tIicSlaveCfg.hwInt = IIC_INTSRC_SCL_SLOW | IIC_INTSRC_STOP_DET | 
					IIC_INTSRC_RD_REQ | IIC_INTSRC_RX_FULL | IIC_INTSRC_TX_ABRT; //使能相应中断
	csi_iic_set_slave_buffer(g_byWriteBuffer,32,g_bySendBuffer,32); //从机就是数组和发送数组设置
	g_tIicMasterCfg.wSdaTimeout = 0XFFFF;						//SDA 超时时间设置，  1/主频 * g_tIicMasterCfg.wSdaTimeout  ms
	g_tIicMasterCfg.wSclTimeout = 0XFFFF;						//SCL 超时时间设置，  1/主频 * g_tIicMasterCfg.wSdaTimeout  ms
	csi_iic_slave_init(I2C0,&g_tIicSlaveCfg);		//初始化从机
	
	
	csi_iic_qualmode_set(I2C0,I2C_QUAL_EXTEND);   //QUALMODE=1,地址扩展模式
//	csi_iic_qualmode_set(I2C0,I2C_QUAL_MASK);    //QUALMODE=0,地址屏蔽模式模式
	uint32_t maskaddr = 0x70;           //配置SLVQUAL
	csi_iic_slvqual_set(I2C0,maskaddr);
	

	while(1);
}





/** \brief IIC DMA tx demo
 * 
 *  \param[in] none
 *  \return error code
 */
 void iic_dma_tx_demo(void)
{
	
	csi_dma_ch_config_t tDmaConfig;    
	csi_etb_config_t  tEtbConfig;
	
	csi_pin_output_mode(PC4,GPIO_OPEN_DRAIN);
	csi_pin_output_mode(PC5,GPIO_OPEN_DRAIN);
	csi_pin_set_mux(PC4,PC4_I2C_SCL);//PIN2 ->	I2C_SCL
	csi_pin_set_mux(PC5,PC5_I2C_SDA);//PIN3 -> 	I2C_SDA
	csi_pin_pull_mode(PC4, GPIO_PULLUP);
	csi_pin_pull_mode(PC5, GPIO_PULLUP);
	
	// tx etcb config
	csi_etb_init();      //使能ETB模块
	tEtbConfig.byChType = ETB_ONE_TRG_ONE_DMA;     //单个源触发单个目标，DMA方式
	tEtbConfig.bySrcIp  = ETB_I2C0_TXSRC;			//I2C0_TXSRC作为触发源
	tEtbConfig.bySrcIp1 = 0xff;      
	tEtbConfig.bySrcIp2 = 0xff;
	tEtbConfig.byDstIp  = ETB_DMA0_CH0 ; //ETB DMA通道 作为目标实际
	tEtbConfig.byDstIp1 = 0xff;
	tEtbConfig.byDstIp2 = 0xff;
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;     //通道触发模式采样硬件触发
	csi_etb_ch_config(ETB_CH21, &tEtbConfig);    //初始化ETB，DMA ETB CHANNEL > ETB_CH19_ID 
	csi_dma_soft_rst(DMA0);    //复位DMA模块
	
	//iic tx dma config
	tDmaConfig.bySrcLinc  = DMA_ADDR_INC;  			//低位传输原地址固定不变
	tDmaConfig.bySrcHinc  = DMA_ADDR_INC;		//高位传输原地址自增
	tDmaConfig.byDetLinc  = DMA_ADDR_CONSTANT;  			//低位传输目标地址固定不变
	tDmaConfig.byDetHinc  = DMA_ADDR_CONSTANT; 			//高位传输目标地址固定不变
	tDmaConfig.byDataWidth  = DMA_DSIZE_32_BITS;   	//传输数据宽度8bit
	tDmaConfig.byReload  = DMA_RELOAD_DISABLE;  	//禁止自动重载
	tDmaConfig.byTransMode  = DMA_TRANS_ONCE;		//DMA服务模式(传输模式)，连续服务
	tDmaConfig.byTsizeMode  = DMA_TSIZE_ONE_DSIZE;  //传输数据大小，一个 DSIZE , 即DSIZE定义大小
	tDmaConfig.byReqMode = DMA_REQ_HARDWARE;   		//DMA请求模式，软件请求（软件触发）
	tDmaConfig.wInt  = DMA_INTSRC_NONE;   		//使用TCIT中断
		
	csi_dma_ch_init(DMA0, DMA_CH0, &tDmaConfig);  //初始化DMA
	csi_dma_ch_start(DMA0, DMA_CH0, (void *)g_wTxBuff, (void *)0x400A0010,9,1);

	// iic congif
	g_tIicMasterCfg.byAddrMode = IIC_ADDRESS_7BIT;			//设置主机地址模式 7/10 bit
	g_tIicMasterCfg.byReStart = ENABLE;						//使能重复起始位
	g_tIicMasterCfg.bySpeedMode = IIC_BUS_SPEED_STANDARD;	//设置主机速度模式	IIC_BUS_SPEED_STANDARD <=100kHz   IIC_BUS_SPEED_FAST <=400kHz    IIC_BUS_SPEED_FAST_PLUS <=  1MHz
	g_tIicMasterCfg.hwInt = IIC_INTSRC_NONE;							//使能需要的中断
	g_tIicMasterCfg.wSdaTimeout = 0XFFFFFFF;						//SDA 超时时间设置，  1/主频 * g_tIicMasterCfg.wSdaTimeout  ms
	g_tIicMasterCfg.wSclTimeout = 0XFFFFFFF;						//SCL 超时时间设置，  1/主频 * g_tIicMasterCfg.wSdaTimeout  ms
	csi_iic_master_init(I2C0,&g_tIicMasterCfg);				//主机初始化
	
	I2C0->DMACR = (0X1<<0) | (0X1<<1) | (0X0<<2) | (0X1<<3);
	g_wTxBuff[8] |=  I2C_CMD_STOP;     // 最后一个数添加stop
	
	csi_iic_disable(I2C0);
	csp_i2c_set_taddr(I2C0,0xa0 >> 1);
	csi_iic_enable(I2C0);
	while(1);
}



/** \brief IIC DMA rx demo
 * 
 *  \param[in] none
 *  \return error code
 */
 void iic_dma_rx_demo(void)
{
	
	csi_dma_ch_config_t tDmaConfig;    
	csi_etb_config_t  tEtbConfig;
	
	csi_pin_output_mode(PC4,GPIO_OPEN_DRAIN);
	csi_pin_output_mode(PC5,GPIO_OPEN_DRAIN);
	csi_pin_set_mux(PC4,PC4_I2C_SCL);   //PIN4 ->	I2C_SCL
	csi_pin_set_mux(PC5,PC5_I2C_SDA);   //PIN5 -> 	I2C_SDA
	csi_pin_pull_mode(PC4, GPIO_PULLUP);
	csi_pin_pull_mode(PC5, GPIO_PULLUP);

	// rx etcb config
	csi_etb_init();      //使能ETB模块
	tEtbConfig.byChType = ETB_ONE_TRG_ONE_DMA;     //单个源触发单个目标，DMA方式
	tEtbConfig.bySrcIp  = ETB_I2C0_RXSRC;			//I2C0_TXSRC作为触发源
	tEtbConfig.bySrcIp1 = 0xff;      
	tEtbConfig.bySrcIp2 = 0xff;
	tEtbConfig.byDstIp  = ETB_DMA0_CH1; //ETB DMA通道 作为目标实际
	tEtbConfig.byDstIp1 = 0xff;
	tEtbConfig.byDstIp2 = 0xff;
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;     //通道触发模式采样硬件触发
	
	csi_etb_ch_config(ETB_CH22, &tEtbConfig);    //初始化ETB，DMA ETB CHANNEL > ETB_CH19_ID 
	csi_dma_soft_rst(DMA0);    //复位DMA模块
	
	// ic rx dma config
	tDmaConfig.bySrcLinc  = DMA_ADDR_CONSTANT;  			//低位传输原地址固定不变
	tDmaConfig.bySrcHinc  = DMA_ADDR_CONSTANT;			//高位传输原地址自增
	tDmaConfig.byDetLinc  = DMA_ADDR_INC;  			//低位传输目标地址固定不变
	tDmaConfig.byDetHinc  = DMA_ADDR_INC; 		//高位传输目标地址固定不变
	tDmaConfig.byDataWidth  = DMA_DSIZE_8_BITS;   	//传输数据宽度8bit
	tDmaConfig.byReload  = DMA_RELOAD_DISABLE;  	//禁止自动重载
	tDmaConfig.byTransMode  = DMA_TRANS_ONCE;		//DMA服务模式(传输模式)，连续服务
	tDmaConfig.byTsizeMode  = DMA_TSIZE_ONE_DSIZE;  //传输数据大小，一个 DSIZE , 即DSIZE定义大小
	tDmaConfig.byReqMode = DMA_REQ_HARDWARE;   		//DMA请求模式，软件请求（软件触发）
	tDmaConfig.wInt  = DMA_INTSRC_NONE;   		//使用TCIT中断

	csi_dma_ch_init(DMA0, DMA_CH1, &tDmaConfig);  //初始化DMA
	csi_dma_ch_start(DMA0, DMA_CH1, (void *)0x400A0010, (void *)g_bRxBuff,9,1);

	// iic config
	g_tIicMasterCfg.byAddrMode = IIC_ADDRESS_7BIT;			//设置主机地址模式 7/10 bit
	g_tIicMasterCfg.byReStart = ENABLE;						//使能重复起始位
	g_tIicMasterCfg.bySpeedMode = IIC_BUS_SPEED_STANDARD;	//设置主机速度模式	IIC_BUS_SPEED_STANDARD <=100kHz   IIC_BUS_SPEED_FAST <=400kHz    IIC_BUS_SPEED_FAST_PLUS <=  1MHz
	g_tIicMasterCfg.hwInt = IIC_INTSRC_NONE;							//使能需要的中断
	g_tIicMasterCfg.wSdaTimeout = 0XFFFFFFF;						//SDA 超时时间设置，  1/主频 * g_tIicMasterCfg.wSdaTimeout  ms
	g_tIicMasterCfg.wSclTimeout = 0XFFFFFFF;						//SCL 超时时间设置，  1/主频 * g_tIicMasterCfg.wSdaTimeout  ms
	csi_iic_master_init(I2C0,&g_tIicMasterCfg);				//主机初始化
	
	I2C0->DMACR = (0X1<<0) | (0X1<<1) | (0X0<<2) | (0X1<<3);

	
	csi_iic_read_nbyte_dma(I2C0,0xa0,0x0001,2,&g_bRxBuff[0],9);        
	

	
	while(1);
	
	

}

/** \brief i2c interrupt handle 
 * 
 *  \param[in] ptSioBase: pointer of i2c register structure
 *  \return none
 */
__attribute__((weak)) void i2c_irqhandler(csp_i2c_t *ptIicBase)
{
	
	csi_iic_slave_receive_send(ptIicBase);
	
}


