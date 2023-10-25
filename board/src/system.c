
 
/***********************************************************************//** 
 * \file  system.c
 * \brief  CSI Device System Source File
 * \copyright Copyright (C) 2015-2023 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-13 <td>V0.0 <td>ZJY     <td>initial
 * </table>
 * *********************************************************************
*/  

#include "sys_clk.h"
#include "drv/tick.h"
#include "drv/iwdt.h" 

#include "board_config.h"


/**
  \brief       initialize the system
  \param[in]   none
  \return      None
*/

extern void irq_vectors_init(void);

#if ((USE_SRAM1_8K_AS_IRAM == 1) | (USE_SRAM1_16K_AS_IRAM == 1))
	
extern char _end_rodata[];
extern char _start_data[],_end_data[];
extern char _start_fastfunc[],_end_fastfunc[];
   
void csi_iram_init(void) 
{
	char *dst = (char *)START_SRAM1_ADDR;
	char *src = _end_rodata + (_end_data - _start_data);
	if(_end_fastfunc != _start_fastfunc)
	{
		memcpy( dst, src, (_end_fastfunc - _start_fastfunc + 4));
		csp_sram1_set_func(SYSCON,SRAM1_ISRAM);  //dram remap to iram
	}
}
#endif

/**
  * @brief  initialize system map
  * @param  None
  * @return None
  */
void system_map_config(void)
{
	csi_sysmap_config_region(0, 0x00000000, SYSMAP_SYSMAPCFG_SO_Msk);
	csi_sysmap_config_region(1, 0x40000,    SYSMAP_SYSMAPCFG_B_Msk | SYSMAP_SYSMAPCFG_C_Msk);
	csi_sysmap_config_region(2, 0x10000000, SYSMAP_SYSMAPCFG_B_Msk | SYSMAP_SYSMAPCFG_C_Msk);
	csi_sysmap_config_region(3, 0x10002000, SYSMAP_SYSMAPCFG_B_Msk | SYSMAP_SYSMAPCFG_C_Msk);
	csi_sysmap_config_region(4, 0x20000000, SYSMAP_SYSMAPCFG_B_Msk | SYSMAP_SYSMAPCFG_C_Msk);
	csi_sysmap_config_region(5, 0x40000000, SYSMAP_SYSMAPCFG_B_Msk | SYSMAP_SYSMAPCFG_C_Msk);
	csi_sysmap_config_region(6, 0xeffff000, SYSMAP_SYSMAPCFG_SO_Msk);
	csi_sysmap_config_region(7, 0xfffff000, SYSMAP_SYSMAPCFG_SO_Msk);
}

/**
  * @brief  initialize system 
  * @param  None
  * @return None
  */
__attribute__((weak)) void system_init(void)		
{
	uint32_t i;
	
	system_map_config();
	csi_icache_enable ();
	
#if ((USE_SRAM1_8K_AS_IRAM == 1) | (USE_SRAM1_16K_AS_IRAM == 1))
	csi_iram_init();  //Need to work with gcc_flash_dram16k_iram16k.ld or gcc_flash_dram24k_iram8k.ld
#endif	
	
	__disable_excp_irq();

    /* enable mstatus FS */
    uint32_t mstatus = __get_MSTATUS();
    mstatus |= (1 << 13);
    __set_MSTATUS(mstatus);
	
#if (USE_KERNEL_FREERTOS == 1)
	/* enable mexstatus SPUSHEN and SPSWAPEN */
    uint32_t mexstatus = __get_MEXSTATUS();
    mexstatus |= (0x3 << 16);
    __set_MEXSTATUS(mexstatus);
#endif
	
	/* get interrupt level from info */
    CLIC->CLICCFG = (((CLIC->CLICINFO & CLIC_INFO_CLICINTCTLBITS_Msk) >> CLIC_INFO_CLICINTCTLBITS_Pos) << CLIC_CLICCFG_NLBIT_Pos);

    for (i = 0; i < 64; i++) {
        CLIC->CLICINT[i].IP = 0;
        CLIC->CLICINT[i].ATTR = 1; 		/* use vector interrupt */
		
		/* enable vector interrupt without exi*/
		if(i > 15)						/* interrupt number = 16~63 */					
		{
			if((i != 22) && (i != 23) && (i != 48) && (i != 49) && (i != 50)&& (i != 54))
				CLIC->CLICINT[i].IE |= CLIC_INTIE_IE_Msk;
		}
		
		/* cnta */
		if(i == 51)
		{
			CLIC->CLICINT[51].ATTR |= (0x1UL << CLIC_INTATTR_TRIG_Pos);
		}
    }
	
	/* tspend use positive interrupt */
    CLIC->CLICINT[SOFTWARE_IRQn].ATTR = 0x3;
	csi_vic_enable_irq(SOFTWARE_IRQn);
	
	csi_iwdt_close();
	csi_sysclk_config(g_tClkConfig);  //sysclk config	
	
	csi_tick_init(); //Use BT3(int enable) as systick! Without this function, "mdelay()" function is NOT reliable! 
#if (USE_KERNEL_FREERTOS == 0)
	__enable_excp_irq();
#endif
}

