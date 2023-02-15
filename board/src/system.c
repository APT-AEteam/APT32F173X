
 
/***********************************************************************//** 
 * \file  system.c
 * \brief  CSI Device System Source File
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-13 <td>V0.0 <td>ZJY     <td>initial
 * </table>
 * *********************************************************************
*/  

#include <soc.h>
#include "sys_clk.h"
#include "csp_gpio.h"
#include "drv/tick.h"
#include "drv/iwdt.h" 
#include "drv/pin.h" 
#include "csp.h"


/**
  \brief       initialize the system
  \param[in]   none
  \return      None
*/

extern void irq_vectors_init(void);

void system_init(void)		//__attribute__((weak))
{
	uint32_t i;
	__disable_excp_irq();
	
    /* enable mstatus FS */
    uint32_t mstatus = __get_MSTATUS();
    mstatus |= (1 << 13);
    __set_MSTATUS(mstatus);

	
	/* get interrupt level from info */
    CLIC->CLICCFG = (((CLIC->CLICINFO & CLIC_INFO_CLICINTCTLBITS_Msk) >> CLIC_INFO_CLICINTCTLBITS_Pos) << CLIC_CLICCFG_NLBIT_Pos);

    for (i = 0; i < 64; i++) {
        CLIC->CLICINT[i].IP = 0;
        CLIC->CLICINT[i].ATTR = 1; /* use vector interrupt */
    }

    /* tspend use positive interrupt */
    //CLIC->CLICINT[SOFTWARE_IRQn].ATTR = 0x3;
	
	
#ifdef	CONFIG_IRQ_LOOKUP		//Table lookup method for interrupt processing 
	irq_vectors_init();
#endif
	
//	csi_reliability_init();
	csi_iwdt_close();				//close iwdt
	csi_sysclk_config();			//sysclk config	
	csi_get_sclk_freq();
	csi_get_pclk_freq();
	csi_tick_init();
	
	__enable_excp_irq();
	
}

