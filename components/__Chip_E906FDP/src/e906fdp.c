/*============================================================================
 * Name        : chip_name.c
 * Author      : $(UserName)
 * Copyright   : Your copyright notice
 * Description : System Initialization before main
 * 1 CPU initialization
 * 2 memory & IO base address initialization
 * 3 flash size & address initialization
 * 4 interrupt resource initialization for the soc
 *============================================================================
 */
#include <string.h>
/**
 * Globl symbols in ckcpu.ld 
 */
extern char __s_ram_data_1[];
extern char __e_rom[];
extern char __e_ram_data_1[];
extern char __e_ram_bss_1[];
extern char __s_ram_bss_1[];

static void MemoryInit( void ) 
{
	  char *dst = __s_ram_data_1;
	  char *src = __e_rom;

	  /**
	   * if the start of data (__s_ram_data_1)
	   * is not equal to end of rtdata (__e_rom) then
	   * copy it, else it's already in the right place
	   */
	  if( __s_ram_data_1 != __e_rom )
	  {
		memcpy( dst, src, (__e_ram_data_1 - __s_ram_data_1));
	  }

	  /* zero the bss 
	   */
	  if( __e_ram_bss_1 - __s_ram_bss_1 ) 
	  {
		memset( __s_ram_bss_1, 0x00, ( __e_ram_bss_1 - __s_ram_bss_1 ));
	  }
}


/**
 * @brief entry of whole chip initialization
 * Since here SystemInit is a weak symbol, any vendor can override this symbol on its own wishes.
 */
__attribute__((weak)) void  SystemInit(void) 
{
	/**
	 * Initial the bss section and data decompression from LMA to VMA
	 */
	MemoryInit();
	
	/**
	 * TODO: initial IO, memory, flash... 
	 */
	
    return;
}

__attribute__((weak)) void __main()
{
	// got to main 
	extern int main(void);
	main();
	// loop here;
	while(1);
}

void __attribute__((weak)) handle_trap(unsigned int cause, unsigned int epc, unsigned int regs[32])
{

}
