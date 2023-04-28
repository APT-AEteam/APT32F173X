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
//#include <csi_config.h>
	
	
.section .vectors, "aw", @progbits
    .align  6
    .globl  __Vectors
    .type   __Vectors, @object
__Vectors:
#ifdef	CONFIG_IRQ_LOOKUP				//Lookup table processing interrupt 
    .long   Default_Handler
    .long   Default_Handler
    .long   Default_Handler
    .long   tspend_handler
    .long   Default_Handler
    .long   Default_Handler
    .long   Default_Handler
    .long   Default_IRQHandler
    .long   Default_Handler
    .long   Default_Handler
    .long   Default_Handler
    .long   Default_Handler
    .long   Default_Handler
    .long   Default_Handler
    .long   Default_Handler
    .long   Default_Handler

    /* External interrupts */
    .long   Default_IRQHandler			//16
    .long   Default_IRQHandler
    .long   Default_IRQHandler
    .long   Default_IRQHandler
    .long   Default_IRQHandler
    .long   Default_IRQHandler
    .long   Default_IRQHandler
    .long   Default_IRQHandler
    .long   Default_IRQHandler			//24
    .long   Default_IRQHandler
    .long   Default_IRQHandler
    .long   Default_IRQHandler
    .long   Default_IRQHandler
    .long   Default_IRQHandler
    .long   Default_IRQHandler
    .long   Default_IRQHandler			
    .long   Default_IRQHandler			//32
    .long   Default_IRQHandler
    .long   Default_IRQHandler
    .long   Default_IRQHandler
    .long   Default_IRQHandler
    .long   Default_IRQHandler
    .long   Default_IRQHandler
    .long   Default_IRQHandler
    .long   Default_IRQHandler			//40
    .long   Default_IRQHandler
    .long   Default_IRQHandler			
    .long   Default_IRQHandler
    .long   Default_IRQHandler
    .long   Default_IRQHandler
    .long   Default_IRQHandler
    .long   Default_IRQHandler
	.long   Default_IRQHandler			//48
    .long   Default_IRQHandler
    .long   Default_IRQHandler
    .long   Default_IRQHandler
    .long   Default_IRQHandler
    .long   Default_IRQHandler
    .long   Default_IRQHandler
    .long   Default_IRQHandler			
    .long   Default_IRQHandler			//56
    .long   Default_IRQHandler
    .long   Default_IRQHandler
    .long   Default_IRQHandler
    .long   Default_IRQHandler
    .long   Default_IRQHandler
    .long   Default_IRQHandler
    .long   Default_IRQHandler			//63
#else									//Handling interrupts separately 
    .long   Reset_Handler				
    .long   Default_Handler
    .long   Default_Handler
    .long   sw_int_handler
    .long   Default_Handler
    .long   Default_Handler
    .long   Default_Handler
    .long   CORET_IRQHandler		
    .long   Default_Handler
    .long   Default_Handler
    .long   Default_Handler
    .long   Default_Handler
    .long   Default_Handler
    .long   Default_Handler
    .long   Default_Handler
    .long   Default_Handler

    /* External interrupts */
    .long   SYSCON_IRQHandler		//16
    .long   IFC_IRQHandler
    .long   ADC0_IRQHandler
    .long   ADC1_IRQHandler
    .long   DMA0_IRQHandler			
    .long   DMA1_IRQHandler
    .long   EXI0_IRQHandler
    .long   EXI1_IRQHandler
    .long   GPTA0_IRQHandler		//24
    .long   GPTA1_IRQHandler
    .long   GPTA2_IRQHandler
    .long   GPTA3_IRQHandler
    .long   GPTB0_IRQHandler		
	.long   GPTB1_IRQHandler
    .long   GPTB2_IRQHandler
    .long   GPTB3_IRQHandler
    .long   GPTB4_IRQHandler		//32
    .long   GPTB5_IRQHandler
    .long   DAC0_IRQHandler			
    .long   Default_Handler			
    .long   USART0_IRQHandler
    .long   USART1_IRQHandler
    .long   UART0_IRQHandler
    .long   UART1_IRQHandler					
    .long   UART2_IRQHandler		//40
    .long   Default_Handler
    .long   SIO0_IRQHandler
    .long   SIO1_IRQHandler			
    .long   I2C_IRQHandler
    .long   SPI0_IRQHandler
    .long   SPI1_IRQHandler
    .long   CAN_IRQHandler		
	.long   EXI2_3_IRQHandler		//48
    .long   EXI4_9_IRQHandler
    .long   EXI10_15_IRQHandler
    .long   CNTA_IRQHandler			
    .long   LPT_IRQHandler
    .long   WWDT_IRQHandler
    .long   RTC_IRQHandler
    .long   CMP0_IRQHandler
    .long   CMP1_IRQHandler			//56
    .long   CMP2_IRQHandler			
    .long   LED_IRQHandler
    .long   Default_Handler			
    .long   BT0_IRQHandler			
    .long   BT1_IRQHandler
    .long   BT2_IRQHandler
    .long   BT3_IRQHandler
#endif

    .text
    .align  2
_start:
    .text
    .align  2
    .globl  Reset_Handler
    .type   Reset_Handler, %function
Reset_Handler:
.option push
.option norelax							// do not replace  any branches or calls
    //la      gp, __global_pointer$
.option pop
    la      a0, Default_Handler			//Exception handling entrance 
    ori     a0, a0, 3
    csrw    mtvec, a0

    la      a0, __Vectors				//Interrupt vector base address 
    csrw    mtvt, a0

    la      sp, g_top_irqstack
    csrw    mscratch, sp

    /* Load data section */
    la      a0, _end_rodata
    la      a1, _start_data
    la      a2, _end_data
    bgeu    a1, a2, 2f					//a1 >= a2, jmp -> 2
1:
    lw      t0, (a0)					//data of a0 memory -> t0
    sw      t0, (a1)					//data 0f t0 -> memory 0f a1
    addi    a0, a0, 4
    addi    a1, a1, 4
    bltu    a1, a2, 1b					//a1 <= a2, jmp -> 1
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
/*
#ifndef __NO_SYSTEM_INIT
    jal     system_init 
#endif

#ifndef __NO_BOARD_INIT
    jal     board_init   
#endif
*/
    jal     main	
    .size   Reset_Handler, . - Reset_Handler

__exit:
    j      __exit

.section .stack
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
	
