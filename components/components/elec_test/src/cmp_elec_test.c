/***********************************************************************//** 
 * \file  cmp_elec_test.c
 * \brief  cmp电气参数测试代码
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2023-4-6 <td>V0.0  <td>YYM   <td>initial
 * </table>
 * *********************************************************************
*/


/* include ----------------------------------------------------------------*/
#include "reliability.h"
#include "pin.h"
#include "cmp.h"

/** \brief CMP0电气参数测试
 *	1、测试失调电压
 *  关闭迟滞功能,N-接固定1.5V输入电压，调节P+端电压，记录输出端PB2翻转稳定后的 P+端电压值
 * 
 *	2、输入迟滞电压测试
 *  N-接固定1.5V输入电压，同时设置N-端和P+端的迟滞电压，调节P+端电压，
 *  记录输出端PB2为低且稳定后的P+端电压值，即为VINL，输出端PB2为高且稳定后的P+端电压值，即为VINH
 *  
 * 3、输入共模电压测试测试
 *  关闭迟滞功能,N-分别接固定10mv和4.5V输入电压，然后调节P+端电压，查看输出端PB2是否能够正常输出
 * 
 * 4、响应时间测试
 * 关闭迟滞功能，同时打开管脚输出功能，差分信号输入为100mV(N-输入200mv,P+输入300mv),
 * 通过示波器测量输入与输出的响应时间
 * 
 *  \param[in] none
 *  \return none
 */
int cmp0_offset_voltage_test(void)
{
	int iRet = 0;
	csi_cmp_config_t tCmpCfg;
	  
	csi_pin_set_mux(PA8,PA8_CPIN1P);		
	csi_pin_set_mux(PA9,PA9_CPIN1N);	
	csi_pin_set_mux(PB2,PB2_CP0_OUT);	
	
	tCmpCfg.byNsel = CMP_N_SEL_CP1;                   //N- 端口选择
	tCmpCfg.byPsel = CMP_P_SEL_CP1;	                  //P+ 端口选择
	tCmpCfg.byPhystpol = CMP_PHYST_POL_0mv;           //比较器输入迟滞
	tCmpCfg.byNhystpol = CMP_PHYST_POL_0mv;	          //比较器输入迟滞特性极性选择
	tCmpCfg.byPolarity = CMP_POL_OUT_DIRECT;          //比较器输出极性选择 0:不反向
	tCmpCfg.byCpoSel  = CMP_CPOS_OUT_IN;	          //CMP_OUT管脚上输出信号选择 0h：滤波前信号直接输出 	1h：滤波后信号输出 
	tCmpCfg.wInt = CMP_INTSRC_EDGEDET;	      	  //中断模式
	csi_cmp_init(CMP0,&tCmpCfg);
	csi_cmp_start(CMP0);
	return iRet;	
}

/** \brief CMP1电气参数测试
 *	1、测试失调电压
 *  关闭迟滞功能,N-接固定1.5V输入电压，调节P+端电压，记录输出端PB10翻转稳定后的 P+端电压值
 * 
 *	2、输入迟滞电压测试
 *  N-接固定1.5V输入电压，同时设置N-端和P+端的迟滞电压，调节P+端电压，
 *  记录输出端PB10为低且稳定后的P+端电压值，即为VINL，输出端PB10为高且稳定后的P+端电压值，即为VINH
 *  
 * 3、输入共模电压测试测试
 *  关闭迟滞功能,N-分别接固定10mv和4.5V输入电压，然后调节P+端电压，查看输出端PB10是否能够正常输出
 * 
 * 4、响应时间测试
 * 关闭迟滞功能，同时打开管脚输出功能，差分信号输入为100mV(N-输入200mv,P+输入300mv),
 * 通过示波器测量输入与输出的响应时间
 * 
 *  \param[in] none
 *  \return none
 */
int cmp1_offset_voltage_test(void)
{
	int iRet = 0;
	csi_cmp_config_t tCmpCfg;
	  
	csi_pin_set_mux(PA8,PA8_CPIN1P);		
	csi_pin_set_mux(PA9,PA9_CPIN1N);	
	csi_pin_set_mux(PB10,PB10_CP1_OUT);	
	
	tCmpCfg.byNsel = CMP_N_SEL_CP1;                   //N- 端口选择
	tCmpCfg.byPsel = CMP_P_SEL_CP1;	                  //P+ 端口选择
	tCmpCfg.byPhystpol = CMP_PHYST_POL_0mv;           //比较器输入迟滞
	tCmpCfg.byNhystpol = CMP_PHYST_POL_0mv;	          //比较器输入迟滞特性极性选择
	tCmpCfg.byPolarity = CMP_POL_OUT_DIRECT;          //比较器输出极性选择 0:不反向
	tCmpCfg.byCpoSel  = CMP_CPOS_OUT_IN;	          //CMP_OUT管脚上输出信号选择 0h：滤波前信号直接输出 	1h：滤波后信号输出 
	tCmpCfg.wInt = CMP_INTSRC_EDGEDET;	      	  //中断模式
	csi_cmp_init(CMP1,&tCmpCfg);
	csi_cmp_start(CMP1);
	return iRet;	
}

/** \brief CMP2电气参数测试
 *	1、测试失调电压
 *  关闭迟滞功能,N-接固定1.5V输入电压，调节P+端电压，记录输出端PB11翻转稳定后的 P+端电压值
 * 
 *	2、输入迟滞电压测试
 *  N-接固定1.5V输入电压，同时设置N-端和P+端的迟滞电压，调节P+端电压，
 *  记录输出端PB11为低且稳定后的P+端电压值，即为VINL，输出端PB11为高且稳定后的P+端电压值，即为VINH
 *  
 * 3、输入共模电压测试测试
 *  关闭迟滞功能,N-分别接固定10mv和4.5V输入电压，然后调节P+端电压，查看输出端PB11是否能够正常输出
 * 
 * 4、响应时间测试
 * 关闭迟滞功能，同时打开管脚输出功能，差分信号输入为100mV(N-输入200mv,P+输入300mv),
 * 通过示波器测量输入与输出的响应时间
 * 
 *  \param[in] none
 *  \return none
 */
int cmp2_offset_voltage_test(void)
{
	int iRet = 0;
	csi_cmp_config_t tCmpCfg;

	csi_pin_set_mux(PA8,PA8_CPIN1P);		
	csi_pin_set_mux(PA9,PA9_CPIN1N);	
	csi_pin_set_mux(PB11,PB11_CP2_OUT);	
	
	tCmpCfg.byNsel = CMP_N_SEL_CP1;                   //N- 端口选择
	tCmpCfg.byPsel = CMP_P_SEL_CP1;	                  //P+ 端口选择
	tCmpCfg.byPhystpol = CMP_PHYST_POL_0mv;           //比较器输入迟滞
	tCmpCfg.byNhystpol = CMP_PHYST_POL_0mv;	          //比较器输入迟滞特性极性选择
	tCmpCfg.byPolarity = CMP_POL_OUT_DIRECT;          //比较器输出极性选择 0:不反向
	tCmpCfg.byCpoSel  = CMP_CPOS_OUT_IN;	          //CMP_OUT管脚上输出信号选择 0h：滤波前信号直接输出 	1h：滤波后信号输出 
	tCmpCfg.wInt = CMP_INTSRC_EDGEDET;	      	      //中断模式
	csi_cmp_init(CMP2,&tCmpCfg);
	csi_cmp_start(CMP2);
	return iRet;	
}