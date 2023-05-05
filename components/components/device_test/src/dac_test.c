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

/** \brief dac interrupt handle function
 * 
 *  \param[in] ptDacBase: pointer of dac register structure
 *  \return none
 */ 
__attribute__((weak)) void dac_irqhandler(csp_dac_t *ptDacBase)
{
	volatile uint32_t wVal = csp_dac_get_misr(ptDacBase);	
	csp_dac_irq_clr(ptDacBase,wVal);
}

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
	int iRet1 = 0;
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
	iRet1= csi_etb_ch_config(ch1, &tEtbConfig1);	
	
	return iRet;	
}
/** \brief DAC各个寄存器功能测试
 * 
 *  \param[in] none
 *  \return none
 */
void dac_test(void)	
{
	csi_pin_set_mux(PA8,PA8_DAC0_OUT);
	csi_pin_output_mode(PA8,GPIO_OPEN_DRAIN);
	csi_pin_set_mux(PA4,PA4_DAC0_OUT);
	//DACR、HSCR寄存器测试
	csp_dac_clr_da(DAC0);//清除转换数据
	csp_dac_set_clk_div(DAC0,1);//设置时钟16分频
	csp_dac_buff_enable(DAC0,0);//使能BUFF信号
	csp_dac_refsel_enable(DAC0,0);//关闭REF
	csp_dac_powerdown_enable(DAC0, 1);//开启powerdown
	//DATAINR\DATAINL寄存器测试
	csp_dac_set_datar(DAC0,2048);//在DATAR中写入数据
//	csp_dac_set_datal(DAC0,4095);//在DATAR中写入数据
	//IMCR\MISR\ICR寄存器测试
	csp_dac_irq_enable(DAC0, DAC_EOC,1);	//使能EOC中断
	csp_dac_irq_enable(DAC0, DAC_WRERR,1);	//使能WRERR中断
	csp_dac_irq_enable(DAC0, DAC_SYNCERR,1);//使能SYNCERR中断
	//SYNCR寄存器测试
	csp_dac_syncr_enable(DAC0, DAC_SYNCIN0,true);//开启DAC_SYNCIN0触发
	csp_dac_syncr_enable(DAC0, DAC_SYNCIN1,true);//开启DAC_SYNCIN1触发
	csp_dac_syncr_enable(DAC0, DAC_SYNCIN2,true);//开启DAC_SYNCIN2触发
	//STEP寄存器测试
	csp_dac_step_val(DAC0, 409);//设置触发增减值
	//ECR寄存器测试
	csp_dac_start(DAC0);
}

