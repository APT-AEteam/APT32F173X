/***********************************************************************//** 
 * \file  cnta.c
 * \brief  csi cnta driver
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version   <th>Author  <th>Description
 * <tr><td> 2020-9-09 <td>V0.0  <td>ZJY     <td>initial
 * <tr><td> 2021-1-09 <td>V0.1  <td>ZJY     <td>modify
 * <tr><td> 2021-5-27 <td>V0.2  <td>LQ      <td>modify
 * </table>
 * *********************************************************************
*/

#include <sys_clk.h>
#include <drv/cnta.h>
#include <drv/irq.h>
#include <drv/gpio.h>
#include <drv/pin.h>
//#include <drv/porting.h>
#include <drv/tick.h>

#include "csp_cnta.h"
/* Private macro------------------------------------------------------*/
/* externs function---------------------------------------------------*/
/* externs variablesr-------------------------------------------------*/
/* Private variablesr-------------------------------------------------*/


/** \brief initialize cnta data structure
 * 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \param[in] ptContaTimerCfg:pointer of timer parameter config
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_cnta_timer_init(csp_cnta_t *ptCntaBase,csi_cnta_timer_config_t *ptContaTimerCfg)
{
	uint8_t byDivTemp = 1;
	uint32_t wTempLoad = 1;
	byDivTemp  = ( 0x01 << (ptContaTimerCfg->eClkDiv) );
	wTempLoad  = (csi_get_pclk_freq() / (byDivTemp * 1000000)) * ptContaTimerCfg->wTime - 1; // (timeout_us / 1000000) = (byDivTemp / pclk) * (tmp_load + 1)
	
	csi_clk_enable((uint32_t *)ptCntaBase);		//cnta clk enable
    csp_cnta_soft_rst(ptCntaBase);				//default init valu
	csp_cnta_set_ckdiv(ptCntaBase,(cnta_ckdiv_e)ptContaTimerCfg->eClkDiv,(cnta_mode_e)ptContaTimerCfg->eRunMode);	//cnta clk = pclk/eClkDiv
	
	csp_cnta_set_datal(ptCntaBase, wTempLoad);				//set CADATAL data
//    csp_cnta_set_datah(ptCntaBase, wTempLoad);			    //set CADATAH data

	
	apt_cnta_int_arrt_set(CLIC_INTATTR_TRIG_UP); 
	csp_cnta_soft_updata(ptCntaBase);	                    //updata CADATAH CADATAL value 
	csp_cnta_set_int(ptCntaBase,(cnta_int_e)ptContaTimerCfg->byInt, true);//set intrrupt
	csi_irq_enable((uint32_t *)ptCntaBase);					//enable cnta irq
	
	return CSI_OK;
}

/** \brief start cnta
 * 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \return none
 */ 
void csi_cnta_start(csp_cnta_t *ptCntaBase)
{		
	csp_cnta_start(ptCntaBase);								//cnta start	
}

/** \brief stop cnta
 * 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \return None
 */ 
void csi_cnta_stop(csp_cnta_t *ptCntaBase)
{	
    csp_cnta_stop(ptCntaBase);
}

/** \brief get cnta datah load value
 * 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \return cnta datah load value
 */ 
uint32_t csi_cnta_get_datah_value(csp_cnta_t *ptCntaBase)
{	
	return csp_cnta_get_datah(ptCntaBase);
}

/** \brief get cnta datal load value
 * 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \return cnta datal load value
 */ 
uint32_t csi_cnta_get_datal_value(csp_cnta_t *ptCntaBase)
{	
	return csp_cnta_get_datal(ptCntaBase);
}

/** \brief cnta pwm init 
 * 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \param[in] ptContaPwmCfg:point of pwm parameter config
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_cnta_pwm_init(csp_cnta_t *ptCntaBase,csi_cnta_pwm_config_t *ptContaPwmCfg)
{	
	csi_error_t ret = CSI_OK;
	volatile uint32_t wDatahLoad; 
	volatile uint32_t wDatalLoad;
	uint32_t wPeriod;
	//uint8_t byClkDiv;
	cnta_osp_e eOsp = 0;
	cnta_remstat_e eRemStat = 0;	
	
	if(ptContaPwmCfg->wFreq == 0 || ptContaPwmCfg->byDutyCycle == 0 || ptContaPwmCfg->byDutyCycle == 100)
		return CSI_ERROR;
		
	csi_clk_enable((uint32_t *)ptCntaBase);		//cnta clk enable
    csp_cnta_soft_rst(ptCntaBase);				//default init valu
	
	//byClkDiv = ptContaPwmCfg->eClkDiv;
	wPeriod = (csi_get_pclk_freq() / ptContaPwmCfg->wFreq) >> ptContaPwmCfg->eClkDiv;
	
	if(wPeriod * ptContaPwmCfg->byDutyCycle < 300)
		wDatahLoad = 0x00;
	else
		wDatahLoad = wPeriod * ptContaPwmCfg->byDutyCycle / 100 - 3 ;//转换计数模式需要3个周期
		
	if(wPeriod * (100 - ptContaPwmCfg->byDutyCycle) < 300)	
		wDatalLoad = 0x00;
	else
		wDatalLoad = wPeriod * (100 - ptContaPwmCfg->byDutyCycle) / 100 - 3 ;//转换计数模式需要3个周期
	
	if(ptContaPwmCfg->byStartLevel == CNTA_POLAR_LOW)			//initial polarity
		eOsp = CNTA_OSP_LOW;
	else if(ptContaPwmCfg->byStartLevel == CNTA_POLAR_HIGH)
		eOsp = CNTA_OSP_HIGH;
		
	if(ptContaPwmCfg->byStopLevel == CNTA_STOP_LOW)				//stop output level
		eRemStat = CNTA_REMSTAT_LOW;
	else if(ptContaPwmCfg->byStopLevel == CNTA_STOP_HIGH)
		eRemStat = CNTA_REMSTAT_HIGH;
			
	csp_cnta_set_ckdiv(ptCntaBase, (cnta_ckdiv_e)ptContaPwmCfg->eClkDiv,(cnta_mode_e)CNTA_REPEAT_MODE);		//cnta clk = pclk/eClkDiv	
	csp_cnta_set_carrier(ptCntaBase, (cnta_carrier_e)CNTA_CARRIER_EN, (cnta_envelope_e)CNTA_PWM_CARRIER, (cnta_remstat_e)eRemStat,(cnta_osp_e) eOsp); //载波输出	
	//csp_cnta_set_carrier(ptCntaBase, (cnta_carrier_e)CNTA_CARRIER_EN, (cnta_envelope_e)PWM_ENVELOPE,(cnta_remstat_e) eRemStat, (cnta_osp_e)eOsp);  //包络输出

	csp_cnta_set_datah(ptCntaBase, wDatahLoad);
	csp_cnta_set_datal(ptCntaBase, wDatalLoad);	
	apt_cnta_int_arrt_set(CLIC_INTATTR_TRIG_UP); 
	csp_cnta_soft_updata(ptCntaBase);
	csp_cnta_set_int(ptCntaBase, (cnta_int_e)ptContaPwmCfg->byInt  , true);
	csi_irq_enable((uint32_t *)ptCntaBase);					    //enable cnta irq
	
	return ret;
}

/** \brief updata cnta pwm freq para: (datah and datal value)
 * 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \param[in] hwDatah: rem ouput high time long 
 *  \param[in] hwDatal: rem ouput low time long  
 *  \param[in] eUpdata: data reg soft updata control
 *  \return none
 */
void csi_cnta_pwm_para_updata(csp_cnta_t *ptCntaBase, uint16_t hwDatah, uint16_t hwDatal, csi_cnta_sw_updata_e eUpdata)
{	
	
	csp_cnta_set_datah(ptCntaBase,hwDatah);	
	csp_cnta_set_datal(ptCntaBase,hwDatal);
	if(eUpdata == CNTA_SW_EN)
	{
		csp_cnta_soft_updata(ptCntaBase);
	}
}

/** \brief set cnta tc sync 
 * 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \param[in] tcpend_rem: carrieron control of tc pend end 
 *  \param[in] tcmatch_rem: carrieron control of tc pulse match 
 *  \param[in] hw_updata: cadata reg auto updata control
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_cnta_bt0_sync(csp_cnta_t *ptCntaBase, csi_cnta_tcpend_e eTcpendRem, csi_cnta_tcmatch_e eTcmatchRem,csi_cnta_hw_updata_e eHwUpdata)
{
	csp_cnta_set_sync(ptCntaBase, (cnta_pendrem_e) eTcpendRem, (cnta_matchrem_e)eTcmatchRem, (cnta_hwstrobe_e)eHwUpdata);	
	return CSI_OK;
}

