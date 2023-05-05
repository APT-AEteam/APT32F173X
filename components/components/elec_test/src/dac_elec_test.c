/***********************************************************************//** 
 * \file  dac_test.c
 * \brief  DAC_TEST description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-4-28 <td>V0.0 <td>xs     <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include "sys_clk.h"
#include <drv/dac.h>
#include "pin.h"
#include <drv/etb.h>
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/


/** \brief 用于测试DAC触发功能的ETCB函数
 * 
 *  \param[in] none
 *  \return none
 */
int dac_sync_test(void)
{
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				                //ETB 参数配置结构体				
	csi_exi_set_evtrg(1, TRGSRC_EXI0, 1);
	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  		//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_EXI_TRGOUT0 ;  	    //EXI1 触发输出0作为触发源
	tEtbConfig.byDstIp =  ETB_DAC0_SYNCIN0;   	    //BT0 同步输入作为目标事件
	tEtbConfig.byTrgMode = ETB_SOFTWARE_TRG;
   
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);	
	volatile uint8_t ch1;
	csi_etb_config_t tEtbConfig1;				                //ETB 参数配置结构体				
	csi_exi_set_evtrg(1, TRGSRC_EXI1, 1);
	tEtbConfig1.byChType = ETB_ONE_TRG_ONE;  		//单个源触发单个目标
	tEtbConfig1.bySrcIp  = ETB_EXI_TRGOUT1 ;  	    //EXI1 触发输出0作为触发源
	tEtbConfig1.byDstIp =  ETB_DAC0_SYNCIN2;   	    //BT0 同步输入作为目标事件
	tEtbConfig1.byTrgMode = ETB_SOFTWARE_TRG;
   
	csi_etb_init();
	ch1 = csi_etb_ch_alloc(tEtbConfig1.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch1 < 0)
		return -1;								    //ch < 0,则获取通道号失败
	csi_etb_ch_config(ch1, &tEtbConfig1);	
	
	return iRet;	
}
/** \brief DAC最大转换率测试
 * 
 *  \param[in] none
 *  \return none
 */
void dac_maxcov_test(void)	
{
	csi_pin_set_mux(PA8,PA8_DAC0_OUT);
	csi_pin_output_mode(PA8,GPIO_OPEN_DRAIN);
	csp_dac_clr_da(DAC0);//清除转换数据
	csp_dac_set_clk_div(DAC0,1);//设置时钟2分频
	csp_dac_buff_enable(DAC0,0);//关闭BUFF信号
	csp_dac_refsel_enable(DAC0,0);//关闭REF
	csp_dac_powerdown_enable(DAC0, 1);//开启powerdown
	csp_dac_set_datar(DAC0,0);//在DATAR中写入数据
	csp_dac_syncr_enable(DAC0, DAC_SYNCIN0,true);//开启DAC_SYNCIN0触发
	csp_dac_step_val(DAC0, 1);//设置触发增减值1
	csp_dac_start(DAC0);
	int i;
	while(1)
	{
	for(i=0;i<4096;i++)
	{
	if(i<=4095)
	ETCB->SOFTTRIG =0X8;
	}
	}
}

/** \brief DAC工作电流测试
 * 
 *  \param[in] none
 *  \return none
 */
void dac_current_test(void)	
{
	csi_pin_set_mux(PA8,PA8_DAC0_OUT);
	csi_pin_output_mode(PA8,GPIO_OPEN_DRAIN);
	csp_dac_clr_da(DAC0);//清除转换数据
	csp_dac_set_clk_div(DAC0,1);//设置时钟2分频
	csp_dac_buff_enable(DAC0,0);//关闭BUFF信号
	csp_dac_refsel_enable(DAC0,0);//关闭REF
	csp_dac_powerdown_enable(DAC0, 1);//开启powerdown,此处写入1和0开启关闭DAC模块，查看芯片工作电流差别
	csp_dac_set_datar(DAC0,0);//在DATAR中写入数据
	csp_dac_start(DAC0);
}

/** \brief DAC INL/DNL测试
 * 
 *  \param[in] none
 *  \return none
 */
void dac_inl_test(void)	
{
	csi_pin_set_mux(PA8,PA8_DAC0_OUT);
	csi_pin_output_mode(PA8,GPIO_OPEN_DRAIN);
	csp_dac_clr_da(DAC0);//清除转换数据
	csp_dac_set_clk_div(DAC0,1);//设置时钟2分频
	csp_dac_buff_enable(DAC0,0);//关闭BUFF信号
	csp_dac_refsel_enable(DAC0,0);//关闭REF
	csp_dac_powerdown_enable(DAC0, 1);//开启powerdown
	csp_dac_set_datar(DAC0,0);//在DATAR中写入数据
	csp_dac_syncr_enable(DAC0, DAC_SYNCIN0,true);//开启DAC_SYNCIN0触发
	csp_dac_syncr_enable(DAC0, DAC_SYNCIN2,true);//开启DAC_SYNCIN2触发
	csp_dac_step_val(DAC0, 1);//设置触发增减值1
	csp_dac_start(DAC0);
	int i,j;
	while(1)
	{
	for(i=0;i<4096;i++)
	{
		ETCB->SOFTTRIG =0X8;
		delay_ums(1);
	}
	for(j=0;j<4096;j++)
	{
		ETCB->SOFTTRIG =0X10;
		delay_ums(1);
	}
	i=0;
	}
}

/** \brief DAC偏移误差测试
 * 
 *  \param[in] none
 *  \return none
 */
void dac_offset_test(void)	
{
	csi_pin_set_mux(PA8,PA8_DAC0_OUT);
	csi_pin_output_mode(PA8,GPIO_OPEN_DRAIN);
	csp_dac_clr_da(DAC0);//清除转换数据
	csp_dac_set_clk_div(DAC0,1);//设置时钟2分频
	csp_dac_buff_enable(DAC0,0);//关闭BUFF信号
	csp_dac_refsel_enable(DAC0,0);//关闭REF
	csp_dac_powerdown_enable(DAC0, 1);//开启powerdown
	csp_dac_set_datar(DAC0,2048);//在DATAR中写入数据
	csp_dac_start(DAC0);
}