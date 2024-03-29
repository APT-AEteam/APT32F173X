/***********************************************************************//** 
 * \file  bt.c
 * \brief  csi bt driver
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-13 <td>V0.0 <td>ZJY     <td>initial 
 * <tr><td> 2023-9-21 <td>V0.1 <td>ZJY     <td>code normalization 
 * </table>
 * *********************************************************************
*/
#include "drv/bt.h"

/* private macro------------------------------------------------------*/
/* private function---------------------------------------------------*/
static uint8_t apt_get_bt_idx(csp_bt_t *ptBtBase);

/* global variablesr--------------------------------------------------*/
csi_bt_ctrl_t g_tBtCtrl[BT_IDX];

/* Private variablesr-------------------------------------------------*/


/** \brief bt interrupt handler function
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \param[in] byIdx: bt idx(0/1/2/3)
 *  \return none
 */ 
void csi_bt_irqhandler(csp_bt_t *ptBtBase, uint8_t byIdx)
{
	bt_int_e eIsr = csp_bt_get_isr(ptBtBase);
	
	if(g_tBtCtrl[byIdx].callback)
			g_tBtCtrl[byIdx].callback(ptBtBase, eIsr);
			
	csp_bt_clr_isr(ptBtBase, eIsr);
}

/** \brief initialize bt data structure
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \param[in] ptBtTimCfg：pointer of bt timing parameter config structure
 * 			   	- wTimeVal: timing value, unit: us
 * 			   	- eWkMode: bt count work mode, \ref csi_bt_wkmode_e
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_bt_timer_init(csp_bt_t *ptBtBase, csi_bt_time_config_t *ptBtTimCfg)
{
	uint32_t wTmLoad;
	uint32_t wClkDiv;
	uint8_t byIdx = apt_get_bt_idx(ptBtBase);
	
	csi_clk_enable((uint32_t *)ptBtBase);			//bt clk enable
	csp_bt_sw_rst(ptBtBase);						//reset bt
	
	wClkDiv = (csi_get_pclk_freq() / 100000 * ptBtTimCfg->wTimeVal / 600000);			//bt clk div value
	if(wClkDiv < 0xfffe)	
		wClkDiv  += 1;
		
	wTmLoad = (csi_get_pclk_freq() / wClkDiv /20000) * ptBtTimCfg->wTimeVal / 50;		//bt prdr load value
	if(wTmLoad > 0xffff)
		wTmLoad = 0xffff;
		
	csp_bt_set_cr(ptBtBase, (BT_SHDOW << BT_SHDW_POS) | (ptBtTimCfg->eRunMode << BT_OPM_POS) |		//bt work mode
			(BT_PCLKDIV << BT_EXTCKM_POS) | (BT_CNTRLD_EN << BT_CNTRLD_POS) | BT_CLK_EN );
	
	csp_bt_set_pscr(ptBtBase, (uint16_t)wClkDiv - 1);				//bt clk div	
	csp_bt_set_prdr(ptBtBase, (uint16_t)wTmLoad);					//bt prdr load value
	csp_bt_set_cmp(ptBtBase, (uint16_t)(wTmLoad >> 1));				//bt prdr load value
	
	//callback init 
	g_tBtCtrl[byIdx].callback = NULL;
	
	return CSI_OK;
}

/** \brief  register bt interrupt callback function
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \param[in] callback: bt interrupt handle function
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_bt_register_callback(csp_bt_t *ptBtBase, void  *callback)
{
	uint8_t byIdx = apt_get_bt_idx(ptBtBase);
	if(byIdx == 0xff)
		return CSI_ERROR;
		
	g_tBtCtrl[byIdx].callback = callback;
	
	return CSI_OK;
}

/** \brief start bt
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \return none
 */ 
void csi_bt_start(csp_bt_t *ptBtBase)
{
	csp_bt_start(ptBtBase);
}
/** \brief stop bt
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \return none
 */ 
void csi_bt_stop(csp_bt_t *ptBtBase)
{
    csp_bt_stop(ptBtBase);
}
/** \brief enable bt interrupt 
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \param[in] eIntSrc: bt interrupt source
 *  \return none
 */ 
void csi_bt_int_enable(csp_bt_t *ptBtBase, csi_bt_intsrc_e eIntSrc)
{
	csp_bt_clr_isr(ptBtBase, (bt_int_e)eIntSrc);
	csp_bt_int_enable(ptBtBase, (bt_int_e)eIntSrc);	
}
/** \brief disable bt interrupt 
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \param[in] eIntSrc: bt interrupt source
 *  \return none
 */ 
void csi_bt_int_disable(csp_bt_t *ptBtBase, csi_bt_intsrc_e eIntSrc)
{
	csp_bt_int_disable(ptBtBase, (bt_int_e)eIntSrc);	
}
/** \brief clear bt interrupt 
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \param[in] eIntSrc: bt interrupt source
 *  \return none
 */ 
void csi_bt_clr_isr(csp_bt_t *ptBtBase, csi_bt_intsrc_e eIntSrc)
{
	csp_bt_clr_isr(ptBtBase, (bt_int_e)eIntSrc);	
}
/** \brief get bt remaining value
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \return none
 */ 
uint32_t csi_bt_get_remaining_value(csp_bt_t *ptBtBase)
{
    uint32_t wTimeVal = csp_bt_get_prdr(ptBtBase) - csp_bt_get_cnt(ptBtBase);

	return wTimeVal;
}
/** \brief get bt load value
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \return none
 */ 
uint32_t csi_bt_get_load_value(csp_bt_t *ptBtBase)
{
	return csp_bt_get_prdr(ptBtBase);
}
/** \brief check bt is running
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \return rue: running; false: stopped
 */ 
bool csi_bt_is_running(csp_bt_t *ptBtBase)
{
	CSI_PARAM_CHK(ptBtBase, false);
	 
	if(ptBtBase->RSSR & BT_START)
		return true;
	else 
		return false;
}

/** \brief bt pwm init 
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \param[in] ptBtPwmCfg: pointer of bt pwm parameter config structure
 *             	- eIdleLevel: pwm output level of bt idel, \ref csi_bt_pwmlev_e
 *             	- eStartLevel: pwm output level of bt start, \ref csi_bt_wkmode_e
 * 			   	- eWkMode: bt count work mode, \ref csi_bt_pwmlev_e
 * 			   	- byDutyCycle: pwm duty cycle, 0~100
 *             	- wFreq: pwm frequency
 * 			   
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_bt_pwm_init(csp_bt_t *ptBtBase, csi_bt_pwm_config_t *ptBtPwmCfg)
{
	uint32_t wClkDiv;
	uint32_t wCrVal;
	uint32_t wCmpLoad; 
	uint32_t wPrdrLoad; 
	uint8_t byIdx = apt_get_bt_idx(ptBtBase);
	
	if(ptBtPwmCfg->wFreq == 0)
		return CSI_ERROR;
	
	csi_clk_enable((uint32_t *)ptBtBase);								//bt clk enable
	csp_bt_sw_rst(ptBtBase);											//reset bt
		
	wClkDiv = (csi_get_pclk_freq() / ptBtPwmCfg->wFreq / 30000);		//bt clk div value
	if(wClkDiv == 0)
		wClkDiv = 1;
	
	wPrdrLoad  = csi_get_pclk_freq() / (wClkDiv * ptBtPwmCfg->wFreq);	//prdr load value
	if(ptBtPwmCfg->byDutyCycle >= 100)
		wCmpLoad = wPrdrLoad + 1;
	else
		wCmpLoad = wPrdrLoad * ptBtPwmCfg->byDutyCycle / 100;			//cmp load value	
	
	wCrVal = BT_CLK_EN | (BT_SHDOW << BT_SHDW_POS) | (ptBtPwmCfg->eRunMode << BT_OPM_POS) | (BT_PCLKDIV << BT_EXTCKM_POS) |
				(BT_CNTRLD_EN << BT_CNTRLD_POS) | (ptBtPwmCfg->eIdleLevel << BT_IDLEST_POS) | (ptBtPwmCfg->eStartLevel << BT_STARTST_POS);
	csp_bt_set_cr(ptBtBase, wCrVal);									//set bt work mode
	csp_bt_set_pscr(ptBtBase, (uint16_t)wClkDiv - 1);					//bt clk div
	csp_bt_set_prdr(ptBtBase, (uint16_t)wPrdrLoad);						//bt prdr load value
	csp_bt_set_cmp(ptBtBase, (uint16_t)wCmpLoad);						//bt cmp load value
		
	//callback init 
	g_tBtCtrl[byIdx].callback = NULL;
	
	return CSI_OK;
}
/** \brief  updata bt cmp reg value
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \param[in] hwCmp: bt cmp reg value
 *  \return none
 */
void csi_bt_cmp_update(csp_bt_t *ptBtBase, uint16_t hwCmp) 
{
	csp_bt_set_cmp(ptBtBase, hwCmp);						//bt cmp load value
}
/** \brief  updata bt prdr reg value
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \param[in] hwPrdr: bt padr reg  value
 *  \return none
 */
void csi_bt_prdr_update(csp_bt_t *ptBtBase, uint16_t hwPrdr) 
{
	csp_bt_set_prdr(ptBtBase, hwPrdr);						//bt prdr load value
}

/** \brief  immediately updata bt parameter
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \param[in] hwPrdr: bt padr reg  value
 *  \return none
 */
void csi_bt_immediate_update(csp_bt_t *ptBtBase) 
{
	csp_bt_immediate_update(ptBtBase);						//immediately updata
}

/** \brief  updata bt pwm freq and duty cycle
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \param[in] wFreq: pwm frequency  
 *  \param[in] byDutyCycle: pwm duty cycle(0 -> 100)
 *  \return none
 */
void csi_bt_pwm_update(csp_bt_t *ptBtBase, uint32_t wFreq, uint8_t byDutyCycle) 
{
	uint32_t wCmpLoad;
	uint32_t wPrdrLoad; 
	uint16_t hwClkDiv = (csi_get_pclk_freq() / wFreq / 30000);		//bt clk div value
	
	if(0 == hwClkDiv)
		hwClkDiv = 1;

	wPrdrLoad  = (csi_get_pclk_freq() / (hwClkDiv * wFreq));		//prdr value
	
	if(byDutyCycle >= 100)
		wCmpLoad = wPrdrLoad + 1;
	else
		wCmpLoad = wPrdrLoad * byDutyCycle / 100;			//cmp load value	

	csp_bt_set_pscr(ptBtBase, hwClkDiv - 1);				//bt clk div
	csp_bt_set_prdr(ptBtBase, (uint16_t)wPrdrLoad);			//bt prdr load value
	csp_bt_set_cmp(ptBtBase, (uint16_t)wCmpLoad);			//bt cmp load value
}

/** \brief bt sync input evtrg config  
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \param[in] eSyncIn: bt sync input channel, \ref csi_bt_syncin_e
 *  \param[in] eSyncMode: bt sync trigger mode, \ref csi_bt_trgmode_e
 *  \param[in] eAutoRearm: auto rearm, \ref \ref csi_bt_arearm_e
 *  \return none
 */
csi_error_t csi_bt_set_sync(csp_bt_t *ptBtBase, csi_bt_syncin_e eSyncIn, csi_bt_syncmode_e eSyncMode, csi_bt_arearm_e eAutoRearm)
{
	if(eSyncIn > BT_SYNCIN2)
		return CSI_ERROR;
	
	csp_bt_set_sync_mode(ptBtBase, (bt_sync_in_e)eSyncIn, (bt_ostmd_e)eSyncMode);				//mode
	
	if((eSyncIn == BT_SYNCIN0) || (eSyncIn == BT_SYNCIN1))
		csp_bt_set_arearm(ptBtBase ,(bt_sync_in_e)eSyncIn, (bt_arearm_e)eAutoRearm);	//rearm
	
	if(eSyncIn == BT_SYNCIN2)
		csp_bt_set_clk(ptBtBase, BT_SYNCTRG);		//sync tirgger as clk source
	else
		csp_bt_set_clk(ptBtBase, BT_PCLKDIV);;		//pclk div as clk source
		
	return CSI_OK; 
}

/** \brief bt sync enable
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \param[in] eSyncIn: bt sync evtrg input channel, \ref csi_bt_syncin_e
 *  \return none
 */
void csi_bt_sync_enable(csp_bt_t *ptBtBase, csi_bt_syncin_e eSyncIn)
{
	csp_bt_sync_enable(ptBtBase, (bt_sync_in_e)eSyncIn);
}
/** \brief bt sync disable
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \param[in] eSyncIn: bt sync evtrg input channel, \ref csi_bt_syncin_e
 *  \return none
 */
void csi_bt_sync_disable(csp_bt_t *ptBtBase, csi_bt_syncin_e eSyncIn)
{
	csp_bt_sync_disable(ptBtBase, (bt_sync_in_e)eSyncIn);
}
/** \brief rearm bt sync evtrg  
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \param[in] eSyncIn: bt sync evtrg input channel, \ref csi_bt_syncin_e
 *  \return none
 */
void csi_bt_sync_rearm(csp_bt_t *ptBtBase,csi_bt_syncin_e eSyncIn)
{
	csp_bt_sync_rearm(ptBtBase, (bt_sync_in_e)eSyncIn);
}
/** \brief bt evtrg output config
 * 
 *  \param[in] ptBtBase:pointer of bt register structure
 *  \param[in] eTrgOut: evtrg out, \ref csi_bt_trgout_e
 *  \param[in] eTrgSrc: evtrg source, \ref csi_bt_trgsrc_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_bt_set_evtrg(csp_bt_t *ptBtBase, csi_bt_trgsrc_e eTrgSrc)
{
	if(eTrgSrc == BT_TRGSRC_DIS)
		csp_bt_set_evtrg(ptBtBase,0x00);
	else
		csp_bt_set_evtrg_src(ptBtBase, (bt_evtrg_src_e)eTrgSrc);
		
	return CSI_OK;
}
/** \brief bt evtrg output enable
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \return none
 */
void csi_bt_evtrg_enable(csp_bt_t *ptBtBase)
{
	csp_bt_evtrg_enable(ptBtBase);
}
/** \brief bt evtrg output disable
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \return none
 */
void csi_bt_evtrg_disable(csp_bt_t *ptBtBase)
{
	csp_bt_evtrg_enable(ptBtBase);
}
/** \brief bt software generates a trigger event
 * 
 *  \param[in] ptBtBase:pointer of bt register structure
 *  \return error code \ref csi_error_t
 */
void csi_bt_sw_evtrg(csp_bt_t *ptBtBase)
{
	csp_bt_sw_evtrg(ptBtBase);
}

/** \brief get bt number 
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \return bt number 0/1/2/3
 */ 
static uint8_t apt_get_bt_idx(csp_bt_t *ptBtBase)
{
	switch((uint32_t)ptBtBase)
	{
		case APB_BT0_BASE:		//bt0
			return 0;		
		case APB_BT1_BASE:		//bt1
			return 1;
		case APB_BT2_BASE:		//bt2
			return 2;		
		case APB_BT3_BASE:		//bt3
			return 3;
		default:
			return 0xff;		//error
	}
}
