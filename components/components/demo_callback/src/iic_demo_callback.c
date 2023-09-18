/***********************************************************************//** 
 * \file  iic_demo.c
 * \brief IIC_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-9-14 <td>V0.0  <td>YT     <td>initial

 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include "csp.h"
#include "iic.h"
#include "irq.h"
#include "pin.h"
#include "demo.h"
#include "etcb.h"
#include "board_config.h"


#if (USE_I2C_CALLBACK == 1)	

csi_iic_master_config_t  g_tIicMasterCfg;	//主机初始化结构体变量
csi_iic_slave_config_t  g_tIicSlaveCfg;	//从机初始化结构体变量

volatile uint8_t g_bySendBuffer[32]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
volatile uint8_t g_byWriteBuffer[32];


/** \brief  user_iic_receive_callback：iic中断回调函数
 * 
 * 	\brief	用户定义，支持IIC十五种中断处理，使用csi标准库，中断发生时会自动调用用户注册的回调函
 * 			数，用户可在回调函数里做自己的处理，而不需要关注具体的底层中断处理。
 * 
 *  \param[out] ptLedBase: 	IICx寄存器结构体指针，指向IICx的基地址 
 *  \param[out] hwIsr: 		IICx中断状态
 *  \param[out] pbyBuf: 	接收buf，指向接收数据缓存数组首地址
 *  \param[out] hwSzie: 		接收长度，
 *  \return none
 */ 
void user_iic_receive_callback(csp_i2c_t *ptIicBase, volatile uint8_t *pbyBuf, uint16_t *hwSzie)
{
	//user code
}

/** \brief i2c slave demo  
 *  \param[in] none
 *  \return none
 */
void iic_slave_callback_demo(void)
{
#if (USE_GUI == 0)		
	csi_gpio_output_mode(GPIOC, PC4, GPIO_OPEN_DRAIN);
	csi_gpio_output_mode(GPIOC, PC5, GPIO_OPEN_DRAIN);
	csi_gpio_set_mux(GPIOC, PC4, PC4_I2C_SCL);
	csi_gpio_set_mux(GPIOC, PC5,PC5_I2C_SDA);
	csi_gpio_pull_mode(GPIOC, PC4, GPIO_PULLUP);	
	csi_gpio_pull_mode(GPIOC, PC5, GPIO_PULLUP);	
#endif	
	
	g_tIicSlaveCfg.eAddrMode = IIC_ADDRESS_7BIT;		//设置从机地址模式 
	g_tIicSlaveCfg.eSpeedMode = IIC_BUS_SPEED_STANDARD;	//设置从机速度	IIC_BUS_SPEED_STANDARD <=100kHz   IIC_BUS_SPEED_FAST <=400kHz    IIC_BUS_SPEED_FAST_PLUS <=  1MHz
	g_tIicSlaveCfg.hwSlaveAddr = 0xa0;				//设置从机地址					
	csi_iic_set_slave_buffer(g_byWriteBuffer,32,g_bySendBuffer,32); //从机就是数组和发送数组设置
	
	csi_iic_register_callback(I2C0,IIC_CALLBACK_RECV, user_iic_receive_callback);	//注册中断回调函数
	csi_iic_int_enable(I2C0, IIC_INTSRC_SCL_SLOW | IIC_INTSRC_STOP_DET |IIC_INTSRC_RD_REQ | IIC_INTSRC_RX_FULL | IIC_INTSRC_TX_ABRT);     //使能需要的中断
	csi_iic_slave_init(I2C0,&g_tIicSlaveCfg);		//初始化从机
	
	while(1);
	
}

#endif