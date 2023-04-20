/***********************************************************************//** 
 * \file  ifc_test.c
 * \brief  Flash测试代码
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-3-14 <td>V0.0  <td>WNN   <td>initial
 * </table>
 * *********************************************************************
*/


/* include ----------------------------------------------------------------*/
#include "ifc.h"
#include "soc.h"
#include "csp.h"
#include "iostring.h"
#include "sys_clk.h"
#include "iwdt.h"
#include "reliability.h"
#include "pin.h"
#include "board_config.h"
 
/* externs function--------------------------------------------------------*/
extern csi_error_t csi_iwdt_init(csi_iwdt_to_e eTimeOut);
extern void csi_iwdt_open(void);
extern void csi_pin_set_mux(pin_name_e ePinName, pin_func_e ePinFunc);
extern void csi_pin_toggle(pin_name_e ePinName);

/* private function--------------------------------------------------------*/

/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/
/* device instance---------------------------------------------------------*/


uint32_t wData[5] = {0x4, 0x5, 0x33333333, 0x44444444, 0x55555555};


/**
 \brief 将代码放到SRAM中执行，写满整个Pflash和Dflash。
  * 考虑到sram大小有限，未使用printf函数打印测试结果。可以查看tRet的值或检查memory中的内容得到测试结果。
  * 
  * 还有一种辅助确认memeory大小的方式：CDK memory窗口如果刷到非真实地址，
  * 会报warning：WARNING: After reading/writing memory operations get ABSTRCTCS.cmderr 3. The result of the operation will not be trusted.

  * \return      csi_error_t
*/
csi_error_t eflash_fullrange_test(void)
{
	csi_error_t tRet = CSI_OK;
	uint32_t i;
	uint32_t wFillData[64] = {0};
	
	
	for(i=1;i<64;i++) 
	{
		wFillData[i] = wFillData[i-1]+1;
	}
	for(i = 0; i<PFLASHPAGENUM; i++)
	{
		csi_ifc_program(IFC, PFLASHBASE+PFLASH_PAGE_SZ*4*i, wFillData, 64);//64);	
		if (g_bFlashCheckPass == 0)	{						//第一种判断方法
			tRet =  CSI_ERROR;
		}
	}
	
	for(i = 0; i<DFLASHPAGENUM; i++)
	{
		csi_ifc_program(IFC, DFLASHBASE+DFLASH_PAGE_SZ*4*i, wFillData, 16);	
		if (g_bFlashCheckPass == 0)							//第一种判断方法
		tRet |=  CSI_ERROR;
	}
	
	
	return tRet;
}

/**
 \brief 并行模式，查看打印信息获得测试结果
  * \return      csi_error_t
*/
extern csi_clk_config_t tClkConfig;   

csi_error_t eflash_parallel_test(void)
{
	csi_error_t tRet = CSI_OK;
	uint32_t i, buff[5];
	
	csi_ifc_dflash_paramode_enable(IFC, ENABLE);
	
	tClkConfig.eClkSrc = SRC_AUTO_HF_PLL;
	tClkConfig.wFreq = PLL_VALUE;
	tClkConfig.eSdiv = 1;
	tClkConfig.ePdiv = 1;
	csi_sysclk_config(tClkConfig);	
	
	csi_ifc_program(IFC, 0x10000010, wData, 5);		//将wData数组的前5个数据写入flash，起始地址0x10000010
	while (csi_ifc_get_status(IFC).busy);
	csi_ifc_read(IFC, 0x10000010, buff, 5) ;
	for (i=0; i<5; i++) {
	if (buff[i] != wData[i])
		tRet =  CSI_ERROR;
	}
	if (g_bFlashCheckPass == 0)
		tRet =  CSI_ERROR;
	
	printf("wait %d ",  (uint8_t)((IFC->MR) & 0x7));
	if (tRet == 0)
		printf("Pass!\n");
	else
		printf("Fail!\n");
	
	
	
	tClkConfig.eClkSrc = SRC_HFOSC;
	tClkConfig.wFreq = HFOSC_24M_VALUE;
	tClkConfig.eSdiv = 1;
	tClkConfig.ePdiv = 1;
	csi_sysclk_config(tClkConfig);	
	
	csi_ifc_program(IFC, 0x10000010, wData, 5);		//将wData数组的前5个数据写入flash，起始地址0x10000010
	while (csi_ifc_get_status(IFC).busy);
	csi_ifc_read(IFC, 0x10000010, buff, 5) ;
	for (i=0; i<5; i++) {
	if (buff[i] != wData[i])
		tRet =  CSI_ERROR;
	}
	if (g_bFlashCheckPass == 0)
		tRet =  CSI_ERROR;
	
	if (tRet == 0)
		printf("wait2 Pass!\n");
	else
		printf("wait2 Fail!\n");
		
		
	tClkConfig.eClkSrc = SRC_HFOSC;
	tClkConfig.wFreq = HFOSC_24M_VALUE;
	tClkConfig.eSdiv = 2;
	tClkConfig.ePdiv = 1;
	csi_sysclk_config(tClkConfig);	
	
	csi_ifc_program(IFC, 0x10000010, wData, 5);		//将wData数组的前5个数据写入flash，起始地址0x10000010
	while (csi_ifc_get_status(IFC).busy);
	csi_ifc_read(IFC, 0x10000010, buff, 5) ;
	for (i=0; i<5; i++) {
	if (buff[i] != wData[i])
		tRet =  CSI_ERROR;
	}
	if (g_bFlashCheckPass == 0)
		tRet =  CSI_ERROR;
	
	printf("wait %d ",  (uint8_t)((IFC->MR) & 0x7));
	if (tRet == 0)
		printf("Pass!\n");
	else
		printf("Fail!\n");
	

	
	//while(1);
	return tRet;
	
}




/**
 \brief HMODE 特权模式验证:写user option
  * [3:0] 	EXTRST  5: 		PD5外部复位管脚
  *         	    other：	PD5普通IO
  * \测试说明：	1）需要修改wData值，做2次测试
  * 			
  *    				
  * \return      csi_error_t
*/

void eflash_useroption_extrst_test(void)
{
	//uint32_t wData = 0xaa000000;			//PD5 IO，测试效果：POR 无论PD5电平，PA一直toggle	
	uint32_t wData = 0xaa000005;			//PD5 EXTRST，测试效果：POR, PD5 force低则PA0停止toggle（进入reset状态）；PD5 force高则PA0持续toggle
	
	if(*(uint32_t *)USEROPTION_ADDR != wData) {
		csi_pin_set_mux(PA1, PA1_OUTPUT);
		
		while(1);
	}
	else {
		while(1){			
			csi_pin_set_mux(PA0, PA0_OUTPUT);
			csi_pin_toggle(PA0);
			mdelay(1);
	}
	}	
	while(1);
}

/**
 \brief HMODE 特权模式验证:写user option

  * [31:24] IWDT	0xaa: 禁用IWDT（默认值，改掉后记得改回来或用烧片机擦除）
  * 				other： 使能IWDT
  * \测试说明：	1）确保system_init里不操作iwdt
  * 			2）需要修改代码（wData）做2次测试
  *    				
  * \return      csi_error_t
*/

void eflash_useroption_iwdt_test(void)
{
	
	//uint32_t wData = 0xaa000000;			//IWDT disable,测试效果：POR, PA0一直toggle
	uint32_t wData = 0x00000000;			//IWDT enable，测试效果：POR, PA0 波形持续2s,断500ms(PA1 toggle)
	
	
	uint32_t n = 500;
	while(n--) {
		csi_pin_set_mux(PA1, PA1_OUTPUT);
		csi_pin_toggle(PA1);
		mdelay(1);
	}
	
	if(*(uint32_t *)USEROPTION_ADDR != wData) {
		csi_ifc_wr_useroption(IFC,wData);
		while(1);
	}
	else {
		csi_iwdt_feed();
		csi_iwdt_init(IWDT_TO_2048);
		while(1){
			csi_pin_set_mux(PA0, PA0_OUTPUT);
			csi_pin_toggle(PA0);
			mdelay(1);
	}
	}	
	while(1);
}


/**
 \brief PROT模式验证,操作步骤: 
  * 1) 工程大小确保可以全部放入SRAM，选择sram ld
  * 2）擦除芯片
  * 3）烧写器protection写入 HDP_4K/HDP_FULL  
  * 4）SRAM debug看是否能写入
  * \return      void
*/
void eflash_hdp_test(void)
{
	
	csi_ifc_program(IFC, 0x2000, wData, 5);		//将wData数组的前5个数据写入flash，起始地址0x2000 (4K外)
	csi_ifc_program(IFC, 0x900, wData, 5);		//将wData数组的前5个数据写入flash，起始地址0x900 (4K内)
	
	while(1);
}




