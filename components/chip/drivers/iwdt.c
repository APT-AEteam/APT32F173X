/***********************************************************************//** 
 * \file  wdt.c
 * \brief  window WDT and indepedent WDT driver
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-24 <td>V0.0 <td>WNN     <td>initial
 * <tr><td> 2021-5-13 <td>V0.0 <td>ZJY     <td>support INT
 * <tr><td> 2023-9-27 <td>V0.1  <td>GQQ   <td>code normalization
 * </table>
 * *********************************************************************
*/

#include "sys_clk.h"
#include "drv/iwdt.h"
#include "drv/irq.h"


/* Private macro------------------------------------------------------*/
/* externs function---------------------------------------------------*/
/* Private function------------------------------------------------------*/
static uint8_t apt_get_iwdt_idx(csp_syscon_t * ptSysBase);
/* externs variablesr-------------------------------------------------*/
/* Private variablesr-------------------------------------------------*/
static uint32_t s_wIwdtTimeout	= 8200;
csi_iwdt_ctrl_t g_tIwdtCtrl[IWDT_IDX];


/** \brief iwdt interrupt handler function
 * 
 *  \param[in] ptWwdtBase: pointer of wwdt register structure
 *  \param[in] byIdx: wwdt idx 0 
 *  \return none
 */ 
void csi_iwdt_irqhandler(csp_syscon_t * ptSysBase, uint8_t byIdx)
{

	if(g_tIwdtCtrl[byIdx].callback)
			g_tIwdtCtrl[byIdx].callback(ptSysBase);
			
	csp_syscon_clr_isr(SYSCON, IWDT_INT);
}

/** \brief Initialize IWDT Interface. Initializes the resources needed for the WDT interface
 * 
 *  \param[in] eTimeOut: the timeout value (time length of system reset)
 *  \return error code \ref csi_error_t
*/
csi_error_t csi_iwdt_init(csi_iwdt_to_e eTimeOut)
{
   	uint8_t byOvt;
	
	if(!(csp_get_gcsr(SYSCON) & ISOSC))		//enable isosc
		csi_isosc_enable();		
	
	switch (eTimeOut)						//set iwdt time out(time long of reset)
	{
		case IWDT_TO_128:	byOvt = 0x0; s_wIwdtTimeout = 128;
			break;
		case IWDT_TO_256: 	byOvt = 0x1; s_wIwdtTimeout = 256;
			break;
		case IWDT_TO_512: 	byOvt = 0x2; s_wIwdtTimeout = 512;
			break;
		case IWDT_TO_1024: 	byOvt = 0x3; s_wIwdtTimeout = 1024;
			break;
		case IWDT_TO_2048:	byOvt = 0x4; s_wIwdtTimeout = 2048;
			break;
		case IWDT_TO_3072:	byOvt = 0x5; s_wIwdtTimeout = 3072;
			break;
		case IWDT_TO_4096: 	byOvt = 0x6; s_wIwdtTimeout = 4096;
			break;
		case IWDT_TO_8192:
		default:	byOvt = 0x7; s_wIwdtTimeout = 8192;
			break;
	}
	
	csp_iwdt_set_ovt(SYSCON, byOvt);
	csp_iwdt_clr(SYSCON);
	
    return CSI_OK;
}

/** \brief open(start) iwdt
 * 
 *  \param[in] none
 *  \return none
 */ 
void csi_iwdt_open(void)
{
	csp_iwdt_enable(SYSCON);
	csp_iwdt_clr(SYSCON);
}

/** \brief close(stop) iwdt
 * 
 *  \param[in] none
 *  \return none
 */ 
void csi_iwdt_close(void)
{
	while(!(csp_get_ckst(SYSCON)& ISOSC));
	csp_iwdt_disable(SYSCON);
}

/** \brief feed iwdt
 * 
 *  \param[in] none
 *  \return none
 */
void csi_iwdt_feed(void)
{
	csp_iwdt_clr(SYSCON);
}

/** \brief iwdt int enable
 * 
 *  \param[in] eAlarmTo: iwdt interrupt alarm timer length(timer out), 1/2/3/4/5/6/7_8
 *  \return none
 */
void csi_iwdt_int_enable(csi_iwdt_alarm_e eAlarmTo)
{
	csp_iwdt_set_int_lvl(SYSCON, (iwdt_intv_e)eAlarmTo);	//iwdt interrupt timer, 1/2/3/4/5/6/7_8
	
	csp_syscon_int_enable(SYSCON, IWDT_INT);				//enable iwdt interrupt
}

/** \brief iwdt int disable
 * 
 *  \param[in] eAlarmTo: iwdt interrupt alarm timer length(timer out), 1/2/3/4/5/6/7_8
 *  \return none
 */
void csi_iwdt_int_disable(csi_iwdt_alarm_e eAlarmTo)
{
	csp_iwdt_set_int_lvl(SYSCON, (iwdt_intv_e)eAlarmTo);					//iwdt interrupt timer, 1/2/3/4/5/6/7_8
	
	csp_syscon_int_disable(SYSCON, IWDT_INT);
}

/** \brief check if iwdt is running
 * 
 *  \return true->running, false->stopped
*/
bool csi_iwdt_is_running(void)
{
	return csp_iwdt_get_status(SYSCON);;
}

/** \brief get the remaining time to timeout
 * 
 *  \param[in] none
 *  \return the remaining time of iwdt, unit: ms
*/
uint32_t csi_iwdt_get_remaining_time(void)
{
	uint32_t wRTime, wCntMax = 0x3f;
	
	switch ((SYSCON->IWDCR & IWDT_OVT_MSK) >> IWDT_OVT_POS)
	{
		case (0): wCntMax = 0x3f;
			break;
		case (1): wCntMax = 0x7f;
			break;
		case (2): wCntMax = 0xff;
			break;
		case (3): wCntMax = 0x1ff;
			break;
		case (4): wCntMax = 0x3ff;
			break;
		case (5): wCntMax = 0x60c;
			break;
		case (6): wCntMax = 0x7ff;
			break;
		case (7): wCntMax = 0xfff;
			break;
		default :
			break;
	}
	wRTime = csp_iwdt_get_cnt(SYSCON) * s_wIwdtTimeout/wCntMax;

	return wRTime;
}

/** \brief enable iwdt when stop in debug mode
 * 
 *  \param[in] none 
 *  \return  none
*/
void csi_iwdt_debug_enable(void)
{
	csp_iwdt_debug_enable(SYSCON);
}
/** \brief disable iwdt when stop in debug mode
 * 
 *  \param[in] none
 *  \return  none
*/
void csi_iwdt_debug_disable(void)
{
	csp_iwdt_debug_disable(SYSCON);
}

/** \brief  register iwdt interrupt callback function
 * 
 *  \param[in] ptSysBase: pointer of iwdt register structure
 *  \param[in] callback: iwdt interrupt handle function
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_iwdt_register_callback(csp_syscon_t * ptSysBase, void  *callback)
{
	uint8_t byIdx = apt_get_iwdt_idx(ptSysBase);
	if(byIdx == 0xff)
		return CSI_ERROR;
		
	g_tIwdtCtrl[byIdx].callback = callback;
	
	return CSI_OK;
}


/** \brief get ptSysBase number 
 * 
 *  \param[in] ptWwdtBase: pointer of iwdt register structure
 *  \return iwdt 0/error
 */ 
static uint8_t apt_get_iwdt_idx(csp_syscon_t * ptSysBase)
{
	switch((uint32_t)ptSysBase)
	{
		case APB_SYS_BASE:		//IWDT
			return 0;		
		default:
			return 0xff;		//error
	}
}