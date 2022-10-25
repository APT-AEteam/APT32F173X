
/***********************************************************************//** 
 * \file  tick.c
 * \brief  Source File for tick
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-5-13<td>V1.1  <td>ZJY     <td>initial
 * </table>
 * *********************************************************************
*/
//#include <csi_config.h>
#include <sys_clk.h>
#include <drv/tick.h>
#include <drv/pin.h>
#include <drv/uart.h>
#include <drv/irq.h>

/* Private macro------------------------------------------------------*/
#define __WEAK	__attribute__((weak))

/* externs function---------------------------------------------------*/

/* externs variablesr-------------------------------------------------*/
/* Private variablesr-------------------------------------------------*/

static volatile uint32_t csi_tick = 0U;
static volatile uint32_t last_time_ms = 0U;
static volatile uint64_t last_time_us = 0U;


static inline unsigned long long apt_coret_get_value(void)
{
	return  CORETIMER->MTIME;
}

void csi_tick_increase(void)
{
    csi_tick++;
}

void tick_irqhandler(void)
{
	
	csi_coret_config(tClkConfig.wSclk / CONFIG_SYSTICK_HZ, CORET_IRQn);
	csi_tick++;
}

csi_error_t csi_tick_init(void)
{
    csi_tick = 0U;

    csi_coret_config(tClkConfig.wSclk/ CONFIG_SYSTICK_HZ, CORET_IRQn);
	csi_vic_set_prio(CORET_IRQn, 0x00);									//0->15: low->high
    csi_vic_enable_irq((uint32_t)CORET_IRQn);
	
    return CSI_OK;
}

void csi_tick_uninit(void)
{
    csi_vic_disable_irq((uint32_t)CORET_IRQn);
}

uint32_t csi_tick_get(void)
{
    return csi_tick;
}

uint32_t csi_tick_get_ms(void)
{
    uint32_t time;
	unsigned long long cur;
	unsigned long long start = apt_coret_get_value();
	
    while (1) 
	{
		cur = apt_coret_get_value();
		
		if(cur > start)
			time = (csi_tick * (1000U / CONFIG_SYSTICK_HZ)) + ((cur - start) / (soc_get_coret_freq() / 1000U));
		else
			time = (csi_tick * (1000U / CONFIG_SYSTICK_HZ)) + ((0xffffffffffffffff - start + cur) / (soc_get_coret_freq() / 1000U));
			
        if (time >= last_time_ms) 
            break;
    }

    last_time_ms = time;
    return time;
}

static void _mdelay(void)
{
	unsigned long long start, cur;				
    uint32_t startl = csi_coret_get_value();
    uint32_t starth = csi_coret_get_valueh();
    uint32_t curl, curh;
    uint32_t cnt = (soc_get_coret_freq() / 1000);
    start = ((unsigned long long)starth << 32) | startl;

    while (1) 
	{
        curl = csi_coret_get_value();
        curh = csi_coret_get_valueh();
        cur = ((unsigned long long)curh << 32) | curl;
		if(cur > start)
		{
			if((cur - start) >= cnt)
				break;
		}
		else
		{
			if((0xffffffffffffffff - start + cur) >= cnt)
				break;
		}
    }
}

void mdelay(uint32_t ms)
{
    if (ms == 0) {
        return;
    }

    while (ms--) {
        _mdelay();
    }
}


void _10udelay(void)
{
	unsigned long long start, cur;
    uint32_t cnt = (soc_get_coret_freq() / 250000);
	start = apt_coret_get_value();

    while (1) 
	{
		cur = apt_coret_get_value();
       	if(cur > start)
		{
			if((cur - start) >= cnt)
				break;
		}
		else
		{
			if((0xffffffffffffffff - start + cur) >= cnt)
				break;
		}
    }
}

/**
 * Ps: At least delay over 10us
*/
void udelay(uint32_t us)
{
    //us /= 10U;
	  us /= 10U;

    while (us) {
        us--;
        _10udelay();
    }
}
