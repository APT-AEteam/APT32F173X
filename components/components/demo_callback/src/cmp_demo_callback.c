/***********************************************************************//** 
 * \file  cmp_demo.c
 * \brief  CMP_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-11-25 <td>V0.0 <td>YYM     <td>initial
 * <tr><td> 2023-09-07 <td>V0.1 <td>LHY     <td>modify
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include "csi_drv.h"
#include "board_config.h"

/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

#if (USE_CMP_CALLBACK == 1)	
/** \brief     cmp0_callback：cmp中断回调函数
 * 
 *  \brief     用户定义，使用csi标准库，中断发生时会自动调用用户注册的回调函
 *             数，用户可在回调函数里做自己的处理，而不需要关注具体的底层中断处理。
 * 
 *  \param[in] ptCmpBase:  CMP0寄存器结构体指针，指向CMP的基地址 
 *  \param[in] byIsr: 	   CMP0中断状态
 *  \return    无返回
 */ 
void cmp0_callback(csp_cmp_t *ptCmpBase, uint8_t byIsr)
{
	if(byIsr & CMP_EDGEDET0_INT)
	{
		//user code
	}
}


/** \brief     比较器基本功能测试demo
 *  \brief     N-和P+输入不同的电平值，如果P+大于N-，将输出高电平，如果P+小于N-，将输出低电平
 * 
 *  \param[in] 无参数
 *  \return    错误码
 */
int cmp_base_int_callback_demo(void)
{
	int iRet = 0;
	csi_cmp_config_t tCmpCfg;

#if (USE_GUI == 0)		
	csi_gpio_set_mux(GPIOA, PA8, PA8_CPIN1P);		
	csi_gpio_set_mux(GPIOA, PA9, PA9_CPIN1N);	
	csi_gpio_set_mux(GPIOB, PB2, PB2_CP0_OUT);	
#endif
	
	tCmpCfg.byNsel = CMP_N_SEL_CP1;                   //N- 端口选择
	tCmpCfg.byPsel = CMP_P_SEL_CP1;	                  //P+ 端口选择
	tCmpCfg.byPhystpol = CMP_PHYST_POL_0mv;           //比较器输入迟滞
	tCmpCfg.byNhystpol = CMP_PHYST_POL_0mv;	          //比较器输入迟滞特性极性选择
	tCmpCfg.byPolarity = CMP_POL_OUT_DIRECT;          //比较器输出极性选择 0:不反向
	tCmpCfg.byCpoSel  = CMP_CPOS_OUT_IN;	          //CMP_OUT管脚上输出信号选择 0h：滤波前信号直接输出 	1h：滤波后信号输出 
	csi_cmp_init(CMP0,&tCmpCfg);
	
	csi_cmp_register_callback(CMP0, cmp0_callback);	  //注册中断回调函数
	csi_cmp_int_enable(CMP0, CMP_INTSRC_EDGEDET);     //若需使用中断，请调该接口使能对应中断，这里使用PENDL中断
	
	csi_cmp_start(CMP0);
	
	return iRet;	
}

#endif
