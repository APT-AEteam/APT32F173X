/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/*
 * .text(code)	Code segment, the area where the program execution code is stored 
 * .rodata(ro)	Read-only data segment, storing strings in C and define constants 
 * .data(rw) 	Initialize the data segment, store the initialized global variables and static variables in the program; 
 * .bss(zi)    	Uninitialized and initialized to 0 data segment, storing uninitialized(initialized to 0) global variables and static variables in the program; 
 * .heap		Stores the dynamically allocated memory segment during the running of the process 
 * .stack		Store local variables temporarily created by the program 
 *
 *  RO SIZE 	= (Code + RO Data) = (text + rodata)
 *  RW SIZE 	= (Rw Data + ZI Data) = (data + bss)
 *  ROM SIZE 	= (Code + RO Data + RW Data)
 */

/******************************************************************************
 * @file     gcc_csky.ld
 * @brief    csky linker file
 * @version  V1.0
 * @date     02. June 2017
 ******************************************************************************/
 
MEMORY
{
	ROM(RWX)  : ORIGIN = 0x00000000,  LENGTH = 128K
	RAM(RWX)  : ORIGIN = 0x20000000,  LENGTH = 24K
	RAM1(RWX) : ORIGIN = 0x20006000,  LENGTH = 8K
}

/*heap and stack*/
__heap_size  = 0x00;							/* heap for dynamically allocated memory */	
/*__stack_size = STACK_SIZE;*/
/*__stack_top = ORIGIN(RAM) + LENGTH(RAM);*/
/*__stack_start = __stack_top - STACK_SIZE - __heap_size; */ 

REGION_ALIAS("REGION_TEXT",    RAM);			/* Assign alias names to memory regions */
REGION_ALIAS("REGION_RODATA",  RAM);
REGION_ALIAS("REGION_DATA",    RAM);
REGION_ALIAS("REGION_BSS",     RAM);
REGION_ALIAS("REGION_FUNC",   RAM1);

ENTRY(Reset_Handler)							/* Entrance address */

SECTIONS {
    .text :								
    {
		. = ALIGN(0x4) ;						/* Align the start of the section */
        *startup.o (.text)						/* The file linked to the start position is startup.o */
        *(.text)
		*(.text.*)
    } > REGION_TEXT
	
	.func :
	{
        *(func)
	} >REGION_FUNC
	
    .rodata :
    {
		. = ALIGN(0x4) ;
        *(.rodata) 
        *(.rodata.*)
		. = ALIGN(0x4) ; 
        _end_rodata = . ;
    } > REGION_RODATA

    .data : 
    { 
		. = ALIGN(0x4) ;						/* Align the start of the section */
		_start_data = .;						/* Provide the name for the start of this section */
		KEEP(*csp.o(.data.*))					/* keep keyword to prevent it from being optimized */
		KEEP(*csp.o(.data))
    	*( .data );
		*( .data.* );
		. = ALIGN(0x4) ;						/* Align the start of the section */
    	_end_data = .; 							/* Provide the name for the end of this section */
    } > REGION_DATA AT > REGION_RODATA			

    .bss :
    {
		. = ALIGN(0x4) ;
		_bss_start = . ;
		*(.sbss)
		*(.sbss.*)
		*(.scommon)
		*(.bss)
		*(.bss.*)
		*(COMMON)
		. = ALIGN(0x4) ;
		_ebss = . ;
		_end = . ;
		end = . ;
    } > REGION_BSS AT > REGION_BSS
	
	._user_heap_stack (NOLOAD) : 
	{
		. = ALIGN(0x4) ;	
		*(.stack*)								/* stack region	*/	
		. = ALIGN(0x4) ;
		_end_stack = . ;
		__heap_start = . ;
		. += __heap_size ;						/* heap region for dynamically allocated memory */
		. = ALIGN(0x4) ;
	} > REGION_BSS AT > REGION_BSS
}

