/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

 /******************************************************************************
 * @file     startup.S
 * @brief    startup file. Should use with
 *           GCC for CSKY Embedded Processors
 * @version  V1.0
 * @date     29. July 2019
 ******************************************************************************/
 
.section .vectors, "aw", @progbits
    .align  6
    .globl  __Vectors
    .type   __Vectors, @object
__Vectors:
    .long   Default_Handler
    .long   Default_Handler
    .long   Default_Handler
    .long   tspend_handler
    .long   Default_Handler
    .long   Default_Handler
    .long   Default_Handler
    .long   coret_int_handler
    .long   Default_Handler
    .long   Default_Handler
    .long   Default_Handler
    .long   Default_Handler
    .long   Default_Handler
    .long   Default_Handler
    .long   Default_Handler
    .long   Default_Handler

    /* External interrupts */
    .long   syscon_int_handler		//16
    .long   ifc_int_handler
    .long   adc0_int_handler
    .long   adc1_int_handler
    .long   dma0_int_handler			
    .long   dma1_int_handler
    .long   exi0_int_handler
    .long   exi1_int_handler
    .long   gpta0_int_handler		//24
    .long   gpta1_int_handler
    .long   gpta2_int_handler
    .long   gpta3_int_handler
    .long   gptb0_int_handler		
	.long   gptb1_int_handler
    .long   gptb2_int_handler
    .long   gptb3_int_handler
    .long   gptb4_int_handler		//32
    .long   gptb5_int_handler
    .long   dac0_int_handler			
    .long   Default_Handler			
    .long   usart0_int_handler
    .long   usart1_int_handler
    .long   uart0_int_handler
    .long   uart1_int_handler					
    .long   uart2_int_handler		//40
    .long   Default_Handler
    .long   sio0_int_handler
    .long   sio1_int_handler			
    .long   iic_int_handler
    .long   spi0_int_handler
    .long   spi1_int_handler
    .long   can_int_handler		
	.long   exi2_3_int_handler		//48
    .long   exi4_9_int_handler
    .long   exi10_15_int_handler
    .long   cnta_int_handler			
    .long   lpt_int_handler
    .long   wwdt_int_handler
    .long   rtc_int_handler
    .long   cmp0_int_handler
    .long   cmp1_int_handler			//56
    .long   cmp2_int_handler			
    .long   led_int_handler
    .long   Default_Handler			
    .long   bt0_int_handler			
    .long   bt1_int_handler
    .long   bt2_int_handler
    .long   bt3_int_handler

    .size   __Vectors, . - __Vectors


    .text
    .align  2
_start:
    .text
    .align  2
    .globl  Reset_Handler
    .type   Reset_Handler, %function
Reset_Handler:
.option push
.option norelax
    //la      gp, __global_pointer$
.option pop
    la      a0, Default_Handler					//Exception handling entrance 
    ori     a0, a0, 3
    csrw    mtvec, a0

    la      a0, __Vectors						//Interrupt vector base address 	
    csrw    mtvt, a0

    la      sp, g_top_irqstack
    csrw    mscratch, sp

    /* Load data section */
	la      a0, _end_rodata
    la      a1, _start_data
    la      a2, _end_data
    bgeu    a1, a2, 2f							//a1 >= a2, jmp -> 2
1:
    lw      t0, (a0)							//data of a0 memory -> t0
    sw      t0, (a1)							//data 0f t0 -> memory 0f a1
    addi    a0, a0, 4
    addi    a1, a1, 4
    bltu    a1, a2, 1b							//a1 <= a2, jmp -> 1
2:

    /* Clear bss section */
    la      a0, _bss_start
    la      a1, _ebss
    bgeu    a0, a1, 2f
1:
    sw      zero, (a0)
    addi    a0, a0, 4
    bltu    a0, a1, 1b
2:

//#ifndef __NO_SYSTEM_INIT
//    jal     SystemInit
//#endif
//
//#ifndef __NO_BOARD_INIT
//    jal     board_init
//#endif

    jal     main

    .size   Reset_Handler, . - Reset_Handler

__exit:
    j      __exit

.section .bss

    .align  3
    .global g_base_irqstack
    .global g_top_irqstack
g_base_irqstack:
    .space CONFIG_ARCH_INTERRUPTSTACK
g_top_irqstack:

.section .por, "ax"					
.align 2
	la 		t2, Reset_Handler
	jalr	t2	