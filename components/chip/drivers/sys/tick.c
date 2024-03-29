
/***********************************************************************//** 
 * \file  tick.c
 * \brief  Source File for tick
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-5-13<td>V0.0  <td>ZJY     <td>initial
 * <tr><td> 2023-9-22 <td>V0.1  <td>GQQ   <td>code normalization
 * </table>
 * *********************************************************************
*/
#include "drv/tick.h"
#include "drv/bt.h"
#include "board_config.h"

/* Private macro------------------------------------------------------*/
#define __WEAK	__attribute__((weak))

/* externs function---------------------------------------------------*/
extern void systick_handler(void);
extern void xPortSysTickHandler(void);
extern void OSTimeTick(void);
/* externs variablesr-------------------------------------------------*/
/* Private variablesr-------------------------------------------------*/

static volatile uint32_t s_wTick = 0U;
static volatile uint32_t s_wLastTimeMs = 0U;



//static inline unsigned long long apt_coret_get_value(void)
//{
//	return  CORETIMER->MTIME;
//}

/** \brief  bt3_int_handler
 *  			- bt3 interrupt handler
 *  \return none
 */
ATTRIBUTE_ISR void  bt3_int_handler(void) 
{
    // ISR content ...
	volatile uint32_t wMisr = csp_bt_get_isr(BT3);
	
	if(wMisr & BT_INT_PEND)					//PEND interrupt
	{
	
		s_wTick++;
#if (USE_KERNEL_FREERTOS == 1)
		xPortSysTickHandler();
#endif
		csp_bt_clr_isr(BT3, BT_INT_PEND);			
	}
}

/** \brief  csi_tick_increase
 *  			- tick increase
 *  \return none
 */
void csi_tick_increase(void)
{
    s_wTick++;
}

/** \brief  csi_tick_init
 *  			- init tick
 *  \return csi_error_t: CSI_OK
 */
csi_error_t csi_tick_init(void)
{
	csi_bt_time_config_t tTimConfig;
	
	s_wTick = 0U;
	
	tTimConfig.wTimeVal = 10000;				//BT定时值，单位：us；即定时值 = 10000us
	tTimConfig.eRunMode  = BT_RUN_CONT;			//BT计数器工作模式：连续/单次
	csi_bt_timer_init(BT3, &tTimConfig);		//初始化BT	
	csi_bt_int_enable(BT3,BT_INTSRC_PEND);		//使能BT PEND中断	
	csi_bt_start(BT3);
	
    return CSI_OK;
}

//void csi_tick_uninit(void)
//{
//	csi_irq_enable((uint32_t *)BT3);	
//}

/** \brief  csi_tick_get
 *  			- get current tick
 *  \return time: current tick
 */
uint32_t csi_tick_get(void)
{
    return s_wTick;
}

/** \brief  csi_tick_get_ms
 *  			- get current time
 *  \return time: current time (ms)
 */ 
uint32_t csi_tick_get_ms(void)
{
    uint32_t time;
	uint16_t cnttm;
	
    while (1) 
	{
		cnttm = (csp_bt_get_cnt(BT3) * (csp_bt_get_pscr(BT3)+1) * 1000) / csi_get_pclk_freq();
		time = (s_wTick * (1000U / CONFIG_SYSTICK_HZ))+ cnttm;
        if (time >= s_wLastTimeMs) 
            break;
    }

    s_wLastTimeMs = time;
    return time;
}

/** \brief  _500usdelay
 *  			- delay 500us
 *  \return none
 */ 
static void _500usdelay(void)
{
    uint32_t start = csp_bt_get_cnt(BT3);
    uint32_t load = csp_bt_get_prdr(BT3);
    uint32_t cur;
    volatile  uint32_t cnt = csi_get_pclk_freq() / (csp_bt_get_pscr(BT3)+1) /2000U  ;

    while (1) {
        cur = csp_bt_get_cnt(BT3);

        if (start <= cur) {
            if ((cur - start) >= cnt) {
                break;
            }
        } else {
            if (((load - start) + cur) > cnt) {
                break;
            }
        }
    }
}
/** \brief  _10udelay
 *  			- delay 10us
 *  \return none
 */ 
void _10udelay(void)
{
    uint32_t start = csp_bt_get_cnt(BT3);
    uint32_t load  = csp_bt_get_prdr(BT3);
    uint32_t cnt   = ((long long)csi_get_pclk_freq() /(csp_bt_get_pscr(BT3)+1) / 1000U / 100U);

    while (1) {
        uint32_t cur = csp_bt_get_cnt(BT3);

        if (start <= cur) {
            if ((cur - start) >= cnt) {
                break;
            }
        } else {
            if (((load - start) + cur) > cnt) {
                break;
            }
        }
    }
}
/** \brief  delay
 * 
 *  \param[in] ms: delay timer,unit: ms 
 *  \return none
 */ 
__WEAK void mdelay(uint32_t ms)
{
    while (ms) {
        ms--;
        _500usdelay();
        _500usdelay();
    }
}
/**
 * Ps: At least delay over 20us
*/
void udelay(uint32_t us)
{
	us /= 10U;
	
    while (us) {
        us--;
        _10udelay();
    }
}
/** \brief  delay, imprecise delay timer
 * 
 *  \param[in] t: delay timer; unit: 10us systick = 48M
 *  \return none
 */ 
void delay_ums(uint32_t t)
{
	volatile uint32_t i,j ,k=0;
	j = 25* t;
	for ( i = 0; i < j; i++ )
	{
		k++;
	}
}