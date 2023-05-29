/***********************************************************************//** 
 * \file  cmp_test.c
 * \brief  CMP_TEST description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-5-25 <td>V1.0 <td>YYM     <td>initial
 * </table>
 * *********************************************************************
*/
// <<< Use Configuration Wizard in Context Menu >>>

// <q> LVR Level (in Bytes)<2>[LVR_19//LVR_22//LVR_25//LVR_28//LVR_31//LVR_34//LVR_37//LVR_40] 
// <i> Config LVR Level for the application
#define		LVR_LEVEL		LVR_31

// <q> SRC Freq (in Bytes)<2>[CQCR_SRCSEL_IM//CQCR_SRCSEL_ES//CQCR_SRCSEL_IS//CQCR_SRCSEL_HF] 
// <i> Config SRC Freq for the application
#define		SRC_FREQ		CQCR_SRCSEL_IM

// <q> REF Freq(in Bytes)<2>[CQCR_REFSEL_EM//CQCR_REFSEL_ES]  
// <i> Config REF Freq for the application
#define	   REF_FREQ        CQCR_REFSEL_ES 

// <o> REF Value (in Bytes) <0x0-0x3FFFF:2>
// <i> Config REF Value for the application
#define		REF_VALUE		0x3f2


/* include ----------------------------------------------------------------*/
#include "reliability.h"
#include "pin.h"
#include "iostring.h"

/** \brief 函数示例LVR的配置方法,及复位源查询方式
 * 
 *  \param[in] none
 *  \return error code
 */
void lvr_wizard_demo(void)
{
	uint8_t byLevel;
	uint8_t byRstSrc;	

	csi_lvr_enable(LVR_LEVEL);				  	//VDD掉电到设定LVR电压，芯片复位	
	byLevel = csi_get_lvrlevel();
	my_printf("lvr level: %d\n", byLevel);	    //执行board_init()对串口进行配置后才有打印
	byRstSrc = csi_get_rst_reason(); 		    //查询复位源，值为csi_rst_rsr_e枚举量之一
	my_printf("rst source: %d\n", byRstSrc);
	while(1);
	
}

/** \brief cqcr测试 必须外接晶振。
 *   参考时钟选择 EM,主时钟选择EM  {SRC_EMOSC, EMOSC_VALUE, SCLK_DIV2, PCLK_DIV2, EMOSC_VALUE/2, EMOSC_VALUE/4};
 *   注意EM管脚需要配置  参考时钟配置为0x3ff，即EM 24M的计数值
 *   如果源时钟选择IM 5.556M 获取到的csi_get_cqsr()值大概为0x1149.	
 *  \param[in] none
 *  \return error code
 */
int cqcr_wizard_demo(void)
{
	int iRet = 0;
	uint32_t wCqsrValue = 0;
	
	csi_set_cqcr(REF_FREQ,SRC_FREQ,REF_VALUE);   // 参考时钟选择 EM,  源时钟选择控 IM

	csi_cqcr_enable();

	wCqsrValue = csi_get_cqsr();                //读取源时钟计数值
	my_printf("cqsr value =%d",wCqsrValue);
	
	return iRet;	
}
