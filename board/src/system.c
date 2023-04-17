
 
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
#include "board_config.h"


/**
  \brief       initialize the system
  \param[in]   none
  \return      None
*/

extern void irq_vectors_init(void);

#ifdef CODE_REMAP_TO_IRAM
extern char _end_rodata[];
extern char _start_data[],_end_data[];
extern char _start_code[],_end_code[];
   
void csi_iram_init(void) 
{
	char *dst = (char *)START_SRAM1_ADDR;
	char *src = _end_rodata + (_end_data - _start_data);
	if(_end_code != _start_code)
	{
		memcpy( dst, src, (_end_code - _start_code + 4));
		csp_sram1_func_ctrl(SYSCON,SRAM1_ISRAM);  //dram remap to iram
	}
}
#endif

void system_init(void)		//__attribute__((weak))
{
	uint32_t i;

	csi_icache_enable ();
	
#ifdef CODE_REMAP_TO_IRAM
	csi_iram_init();  //Need to work with gcc_flash_dram16k_iram16k.ld or gcc_flash_dram24k_iram8k.ld
#endif	
	
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
	
	csi_iwdt_close();				//close iwdt
	csi_sysclk_config(tClkConfig);			//sysclk config	
	csi_calc_clk_freq();			//calculate(updata) sclk and pclk
	csi_tick_init();
	
	__enable_excp_irq();
	
}

