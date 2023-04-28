/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */
 /******************************************************************************
 * @file     vectors.S
 * @brief    define default vector handlers. Should use with
 *           GCC for CSKY Embedded Processors
 * @version  V1.0
 * @date     28. Nove 2017
 ******************************************************************************/

//#include <csi_config.h>

/* Enable interrupts when returning from the handler */
#define MSTATUS_PRV1 0x1880
	
	//stack size
	.equ 	STACK_SIZE, CONFIG_ARCH_INTERRUPTSTACK			
	.globl 	STACK_SIZE
	
.section .bss
    .align  2
    .globl  g_trapstackalloc
    .global g_trapstackbase
    .global g_top_trapstack
g_trapstackalloc:
g_trapstackbase:
    .space 768
g_top_trapstack:

    .align 2
    .globl g_trap_sp
    .type  g_trap_sp, object
g_trap_sp:
    .long 0
    .size g_trap_sp, .-g_trap_sp

irq_nested_level:
.long 0
irq_mstatus_fs_flag:
.long 0

.text

/******************************************************************************
 * Functions:
 *     Default_IRQHandler(void);
 * ALL IRQ handler, Lookup table processing interrupt 
 ******************************************************************************/	
    .align  2
    .global Default_IRQHandler
    .weak   Default_IRQHandler
    .type   Default_IRQHandler, %function
Default_IRQHandler:
	ipush
	
//	csrr    t1, mstatus
//    srli    t1, t1, 13
//    andi    t1, t1, 0x3
//    la      t3, irq_mstatus_fs_flag
//    sw      t1, (t3)
//    li      t0, 0x3
//    bne     t1, t0,  .F_RegNotSave
//
//    addi    sp,  sp, -160
//    fsd     ft0, (0 +0 )(sp)
//    fsd     ft1, (4 +4 )(sp)
//    fsd     ft2, (8 +8 )(sp)
//    fsd     ft3, (12+12)(sp)
//    fsd     ft4, (16+16)(sp)
//    fsd     ft5, (20+20)(sp)
//    fsd     ft6, (24+24)(sp)
//    fsd     ft7, (28+28)(sp)
//    fsd     fa0, (32+32)(sp)
//    fsd     fa1, (36+36)(sp)
//    fsd     fa2, (40+40)(sp)
//    fsd     fa3, (44+44)(sp)
//    fsd     fa4, (48+48)(sp)
//    fsd     fa5, (52+52)(sp)
//    fsd     fa6, (56+56)(sp)
//    fsd     fa7, (60+60)(sp)
//    fsd     ft8, (64+64)(sp)
//    fsd     ft9, (68+68)(sp)
//    fsd     ft10,(72+72)(sp)
//    fsd     ft11,(76+76)(sp)
.F_RegNotSave:

    csrr    t1, mcause					//t1 <- mcause
    andi    t1, t1, 0x3FF				//t1 = t1 & 0x3f; get interrupt number
    slli    t1, t1, 2					//t1 = t1 << 2
    la      t0, g_irqvector				//t0 <- g_irqvector
    add     t0, t0, t1					//t0 = t0 + t1; t0 = g_irqvector + (interrupt number << 2)
    lw      t2, (t0)					//t2 <- data of addr for t0 save
    jalr    t2							//jmp addr for t2 save
	
	li      t0, MSTATUS_PRV1
    csrs    mstatus, t0
    
//    la      t0, irq_mstatus_fs_flag
//    lw      t1, (t0)
//    li      t0, 0x3
//    bne     t1, t0,  .F_RegNotLoad
//
//    fld     ft0, (0 +0 )(sp)
//    fld     ft1, (4 +4 )(sp)
//    fld     ft2, (8 +8 )(sp)
//    fld     ft3, (12+12)(sp)
//    fld     ft4, (16+16)(sp)
//    fld     ft5, (20+20)(sp)
//    fld     ft6, (24+24)(sp)
//    fld     ft7, (28+28)(sp)
//    fld     fa0, (32+32)(sp)
//    fld     fa1, (36+36)(sp)
//    fld     fa2, (40+40)(sp)
//    fld     fa3, (44+44)(sp)
//    fld     fa4, (48+48)(sp)
//    fld     fa5, (52+52)(sp)
//    fld     fa6, (56+56)(sp)
//    fld     fa7, (60+60)(sp)
//    fld     ft8, (64+64)(sp)
//    fld     ft9, (68+68)(sp)
//    fld     ft10,(72+72)(sp)
//    fld     ft11,(76+76)(sp)
//    addi    sp, sp, 160
.F_RegNotLoad:
    ipop
	
#ifndef	CONFIG_IRQ_LOOKUP			//Handling interrupts separately 
/******************************************************************************
 * Functions:
 * Irq_Entry(void) and Irq_Exit(void);;
 * IRQ ENTRY and IRQ EXIT
 ******************************************************************************/	
	.align  2
    .global Irq_Entry
    .weak   Irq_Entry
    .type   Irq_Entry, %function
Irq_Entry:
//    csrr    t1, mstatus
//    srli    t1, t1, 13
//    andi    t1, t1, 0x3
//    la      t3, irq_mstatus_fs_flag
//    sw      t1, (t3)
//    li      t0, 0x3
//    bne     t1, t0,  .F_RegNotSave1
//
//	addi    sp,  sp, -160
//    fsd     ft0, (0 +0 )(sp)
//    fsd     ft1, (4 +4 )(sp)
//    fsd     ft2, (8 +8 )(sp)
//    fsd     ft3, (12+12)(sp)
//    fsd     ft4, (16+16)(sp)
//    fsd     ft5, (20+20)(sp)
//    fsd     ft6, (24+24)(sp)
//    fsd     ft7, (28+28)(sp)
//    fsd     fa0, (32+32)(sp)
//    fsd     fa1, (36+36)(sp)
//    fsd     fa2, (40+40)(sp)
//    fsd     fa3, (44+44)(sp)
//    fsd     fa4, (48+48)(sp)
//    fsd     fa5, (52+52)(sp)
//    fsd     fa6, (56+56)(sp)
//    fsd     fa7, (60+60)(sp)
//    fsd     ft8, (64+64)(sp)
//    fsd     ft9, (68+68)(sp)
//    fsd     ft10,(72+72)(sp)
//    fsd     ft11,(76+76)(sp)
.F_RegNotSave1:
	ret			//jalr	x1, x1 = ra
	
	.align  2
    .global Irq_Exit
    .weak   Irq_Exit
    .type   Irq_Exit, %function
Irq_Exit:
	li      t0, MSTATUS_PRV1
    csrs    mstatus, t0
	
//	la      t0, irq_mstatus_fs_flag
//    lw      t1, (t0)
//    li      t0, 0x3
//    bne     t1, t0,  .F_RegNotLoad1
//
//    fld     ft0, (0 +0 )(sp)
//    fld     ft1, (4 +4 )(sp)
//    fld     ft2, (8 +8 )(sp)
//    fld     ft3, (12+12)(sp)
//    fld     ft4, (16+16)(sp)
//    fld     ft5, (20+20)(sp)
//    fld     ft6, (24+24)(sp)
//    fld     ft7, (28+28)(sp)
//    fld     fa0, (32+32)(sp)
//    fld     fa1, (36+36)(sp)
//    fld     fa2, (40+40)(sp)
//    fld     fa3, (44+44)(sp)
//    fld     fa4, (48+48)(sp)
//    fld     fa5, (52+52)(sp)
//    fld     fa6, (56+56)(sp)
//    fld     fa7, (60+60)(sp)
//    fld     ft8, (64+64)(sp)
//    fld     ft9, (68+68)(sp)
//    fld     ft10,(72+72)(sp)
//    fld     ft11,(76+76)(sp)
//    addi    sp, sp, 160
.F_RegNotLoad1:
    ipop
	ret			//jalr	x1, x1 = ra
	
/******************************************************************************
 * Functions:
 *     xxx_IRQHandler(void);
 * ALL IRQ handler list, Handling interrupts separately 
 ******************************************************************************/	
	.align  2
    .global CORET_IRQHandler
    .weak   CORET_IRQHandler
    .type   CORET_IRQHandler, %function
CORET_IRQHandler:
	ipush
	jal		Irq_Entry
//  la     	t2 , coret_int_handler
//	jalr   	t2
	jal		coret_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global SYSCON_IRQHandler
    .weak   SYSCON_IRQHandler
    .type   SYSCON_IRQHandler, %function
SYSCON_IRQHandler:
	ipush
	jal 	Irq_Entry
	jal		syscon_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global IFC_IRQHandler
    .weak   IFC_IRQHandler
    .type   IFC_IRQHandler, %function
IFC_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal     ifc_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global ADC0_IRQHandler
    .weak   ADC0_IRQHandler
    .type   ADC0_IRQHandler, %function
ADC0_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal     adc0_int_handler
	jal 	Irq_Exit

	.align  2
    .global ADC1_IRQHandler
    .weak   ADC1_IRQHandler
    .type   ADC1_IRQHandler, %function
ADC1_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal     adc1_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global DMA0_IRQHandler
    .weak   DMA0_IRQHandler
    .type   DMA0_IRQHandler, %function
DMA0_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal     dma0_int_handler
	jal 	Irq_Exit

	.align  2
    .global DMA1_IRQHandler
    .weak   DMA1_IRQHandler
    .type   DMA1_IRQHandler, %function
DMA1_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal     dma1_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global EXI0_IRQHandler
    .weak   EXI0_IRQHandler
    .type   EXI0_IRQHandler, %function
EXI0_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal     exi0_int_handler
	jal 	Irq_Exit

	.align  2
    .global EXI1_IRQHandler
    .weak   EXI1_IRQHandler
    .type   EXI1_IRQHandler, %function
EXI1_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal     exi1_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global GPTA0_IRQHandler
    .weak   GPTA0_IRQHandler
    .type   GPTA0_IRQHandler, %function
GPTA0_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal		gpta0_int_handler
	jal 	Irq_Exit

	.align  2
    .global GPTA1_IRQHandler
    .weak   GPTA1_IRQHandler
    .type   GPTA1_IRQHandler, %function
GPTA1_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal		gpta1_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global GPTA2_IRQHandler
    .weak   GPTA2_IRQHandler
    .type   GPTA2_IRQHandler, %function
GPTA2_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal		gpta2_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global GPTA3_IRQHandler
    .weak   GPTA3_IRQHandler
    .type   GPTA3_IRQHandler, %function
GPTA3_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal     gpta3_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global GPTB0_IRQHandler
    .weak   GPTB0_IRQHandler
    .type   GPTB0_IRQHandler, %function
GPTB0_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal     gptb0_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global GPTB1_IRQHandler
    .weak   GPTB1_IRQHandler
    .type   GPTB1_IRQHandler, %function
GPTB1_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal		gptb1_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global GPTB2_IRQHandler
    .weak   GPTB2_IRQHandler
    .type   GPTB2_IRQHandler, %function
GPTB2_IRQHandler:
	ipush
	jal 	Irq_Entry
	jal		gptb2_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global GPTB3_IRQHandler
    .weak   GPTB3_IRQHandler
    .type   GPTB3_IRQHandler, %function
GPTB3_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal		gptb3_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global GPTB4_IRQHandler
    .weak   GPTB4_IRQHandler
    .type   GPTB4_IRQHandler, %function
GPTB4_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal		gptb4_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global GPTB5_IRQHandler
    .weak   GPTB5_IRQHandler
    .type   GPTB5_IRQHandler, %function
GPTB5_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal		gptb5_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global DAC0_IRQHandler
    .weak   DAC0_IRQHandler
    .type   DAC0_IRQHandler, %function
DAC0_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal		dac0_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global USART0_IRQHandler
    .weak   USART0_IRQHandler
    .type   USART0_IRQHandler, %function
USART0_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal 	usart0_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global USART1_IRQHandler
    .weak   USART1_IRQHandler
    .type   USART1_IRQHandler, %function
USART1_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal		usart1_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global UART0_IRQHandler
    .weak   UART0_IRQHandler
    .type   UART0_IRQHandler, %function
UART0_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal		uart0_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global UART1_IRQHandler
    .weak   UART1_IRQHandler
    .type   UART1_IRQHandler, %function
UART1_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal		uart1_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global UART2_IRQHandler
    .weak   UART2_IRQHandler
    .type   UART2_IRQHandler, %function
UART2_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal		uart2_int_handler
	jalr   	t2
	jal 	Irq_Exit
	
	.align  2
    .global SIO0_IRQHandler
    .weak   SIO0_IRQHandler
    .type   SIO0_IRQHandler, %function
SIO0_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal		sio0_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global SIO1_IRQHandler
    .weak   SIO1_IRQHandler
    .type   SIO1_IRQHandler, %function
SIO1_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal		sio1_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global I2C_IRQHandler
    .weak   I2C_IRQHandler
    .type   I2C_IRQHandler, %function
I2C_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal		i2c_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global SPI0_IRQHandler
    .weak   SPI0_IRQHandler
    .type   SPI0_IRQHandler, %function
SPI0_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal		spi0_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global SPI1_IRQHandler
    .weak   SPI1_IRQHandler
    .type   SPI1_IRQHandler, %function
SPI1_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal		spi1_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global CAN_IRQHandler
    .weak   CAN_IRQHandler
    .type   CAN_IRQHandler, %function
CAN_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal	 	can_int_handler
	jal 	Irq_Exit

	.align  2
    .global EXI2_3_IRQHandler
    .weak   EXI2_3_IRQHandler
    .type   EXI2_3_IRQHandler, %function
EXI2_3_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal		exi2_3_int_handler
	jal 	Irq_Exit

	.align  2
    .global EXI4_9_IRQHandler
    .weak   EXI4_9_IRQHandler
    .type   EXI4_9_IRQHandler, %function
EXI4_9_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal		exi4_9_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global EXI10_15_IRQHandler
    .weak   EXI10_15_IRQHandler
    .type   EXI10_15_IRQHandler, %function
EXI10_15_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal		exi10_15_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global CNTA_IRQHandler
    .weak   CNTA_IRQHandler
    .type   CNTA_IRQHandler, %function
CNTA_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal		cnta_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global LPT_IRQHandler
    .weak   LPT_IRQHandler
    .type   LPT_IRQHandler, %function
LPT_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal		lpt_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global WWDT_IRQHandler
    .weak   WWDT_IRQHandler
    .type   WWDT_IRQHandler, %function
WWDT_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal		wwdt_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global RTC_IRQHandler
    .weak   RTC_IRQHandler
    .type   RTC_IRQHandler, %function
RTC_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal		rtc_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global CMP0_IRQHandler
    .weak   CMP0_IRQHandler
    .type   CMP0_IRQHandler, %function
CMP0_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal		cmp0_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global CMP1_IRQHandler
    .weak   CMP1_IRQHandler
    .type   CMP1_IRQHandler, %function
CMP1_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal		cmp1_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global CMP2_IRQHandler
    .weak   CMP2_IRQHandler
    .type   CMP2_IRQHandler, %function
CMP2_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal		cmp2_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global LED_IRQHandler
    .weak   LED_IRQHandler
    .type   LED_IRQHandler, %function
LED_IRQHandler:
	ipush
	jal 	Irq_Entry
    jal		led_int_handler
	jal 	Irq_Exit
	
    .align  2
    .global BT0_IRQHandler
    .weak   BT0_IRQHandler
    .type   BT0_IRQHandler, %function
BT0_IRQHandler:
	ipush
	jal 	Irq_Entry
	jal     bt0_int_handler
	jal 	Irq_Exit

	.align  2
    .global BT1_IRQHandler
    .weak   BT1_IRQHandler
    .type   BT1_IRQHandler, %function
BT1_IRQHandler:
	ipush
    jal 	Irq_Entry
	jal		bt1_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global BT2_IRQHandler
    .weak   BT2_IRQHandler
    .type   BT2_IRQHandler, %function
BT2_IRQHandler:
	ipush
    jal 	Irq_Entry
    jal     bt2_int_handler
	jal 	Irq_Exit
	
	.align  2
    .global BT3_IRQHandler
    .weak   BT3_IRQHandler
    .type   BT3_IRQHandler, %function
BT3_IRQHandler:
	ipush
    jal		Irq_Entry
    jal     bt3_int_handler
	jal		Irq_Exit

#endif

/******************************************************************************
 * Functions:
 *     void trap(void);
 * default exception handler
 ******************************************************************************/
    .align  2
    .global trap
    .type   trap, %function
trap:
    /* Check for interrupt */
    addi    sp, sp, -4
    sw      t0, 0x0(sp)
    csrr    t0, mcause

    blt     t0, x0, .Lirq

    addi    sp, sp, 4

    la      t0, g_trap_sp
    addi    t0, t0, -132
    sw      x1, 0(t0)
    sw      x2, 4(t0)
    sw      x3, 8(t0)
    sw      x4, 12(t0)
    sw      x6, 20(t0)
    sw      x7, 24(t0)
    sw      x8, 28(t0)
    sw      x9, 32(t0)
    sw      x10, 36(t0)
    sw      x11, 40(t0)
    sw      x12, 44(t0)
    sw      x13, 48(t0)
    sw      x14, 52(t0)
    sw      x15, 56(t0)
    sw      x16, 60(t0)
    sw      x17, 64(t0)
    sw      x18, 68(t0)
    sw      x19, 72(t0)
    sw      x20, 76(t0)
    sw      x21, 80(t0)
    sw      x22, 84(t0)
    sw      x23, 88(t0)
    sw      x24, 92(t0)
    sw      x25, 96(t0)
    sw      x26, 100(t0)
    sw      x27, 104(t0)
    sw      x28, 108(t0)
    sw      x29, 112(t0)
    sw      x30, 116(t0)
    sw      x31, 120(t0)
    csrr    a0, mepc
    sw      a0, 124(t0)
    csrr    a0, mstatus
    sw      a0, 128(t0)

    mv      a0, t0
    lw      t0, -4(sp)
    mv      sp, a0
    sw      t0, 16(sp)

    jal     trap_c


.Lirq:
    lw      t0, 0x0(sp)
    addi    sp, sp, 4
    j       Default_IRQHandler


    .align  6
    .weak   Default_Handler
    .global Default_Handler
    .type   Default_Handler, %function
Default_Handler:
    /* Check for nmi */
    addi    sp, sp, -8
    sw      t0, 0x0(sp)
    sw      t1, 0x4(sp)
    csrr    t0, mcause
    andi    t0, t0, 0x3FF
    li      t1, 24
    beq     t0, t1, .NMI_Handler
    lw      t0, 0x0(sp)
    lw      t1, 0x4(sp)
    addi    sp, sp, 8
    j      trap

.NMI_Handler:
    lw      t0, 0x0(sp)
    lw      t1, 0x4(sp)
    addi    sp, sp, 8

    addi    sp, sp, -64
    sw      ra, 0(sp)
    sw      t0, 4(sp)
    sw      t1, 8(sp)
    sw      t2, 12(sp)
    sw      a0, 16(sp)
    sw      a1, 20(sp)
    sw      a2, 24(sp)
    sw      a3, 28(sp)
    sw      a4, 32(sp)
    sw      a5, 36(sp)
    sw      a6, 40(sp)
    sw      a7, 44(sp)
    sw      t3, 48(sp)
    sw      t4, 52(sp)
    sw      t5, 56(sp)
    sw      t6, 60(sp)

    la      t0, nmi_int_handler
    jalr    t0

    lw      ra, 0(sp)
    lw      t0, 4(sp)
    lw      t1, 8(sp)
    lw      t2, 12(sp)
    lw      a0, 16(sp)
    lw      a1, 20(sp)
    lw      a2, 24(sp)
    lw      a3, 28(sp)
    lw      a4, 32(sp)
    lw      a5, 36(sp)
    lw      a6, 40(sp)
    lw      a7, 44(sp)
    lw      t3, 48(sp)
    lw      t4, 52(sp)
    lw      t5, 56(sp)
    lw      t6, 60(sp)
    addi    sp, sp, 64
    mret


    .size   Default_Handler, . - Default_Handler

/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
    .macro  def_irq_handler handler_name
    .weak   \handler_name
    .globl  \handler_name
    .set    \handler_name, Default_Handler
    .endm

    def_irq_handler tspend_handler
    def_irq_handler coret_int_handler
//    def_irq_handler syscon_int_handler
//	def_irq_handler ifc_int_handler
//    def_irq_handler adc0_int_handler
//    def_irq_handler adc1_int_handler
//    def_irq_handler dma0_int_handler
//	def_irq_handler dma1_int_handler
//    def_irq_handler gpta0_int_handler
//    def_irq_handler gpta1_int_handler
//    def_irq_handler gpta2_int_handler
//	def_irq_handler gpta3_int_handler
//	def_irq_handler gptb0_int_handler
//    def_irq_handler gptb1_int_handler
//    def_irq_handler gptb2_int_handler
//	def_irq_handler gptb3_int_handler
//	def_irq_handler gptb4_int_handler
//	def_irq_handler gptb5_int_handler
//    def_irq_handler dac0_int_handler
//    def_irq_handler usart0_int_handler
//    def_irq_handler usart1_int_handler
//    def_irq_handler uart0_int_handler
//    def_irq_handler uart1_int_handler
//    def_irq_handler uart2_int_handler
//    def_irq_handler sio0_int_handler
//	def_irq_handler sio1_int_handler
//    def_irq_handler i2c_int_handler
//    def_irq_handler spi0_int_handler
//	def_irq_handler spi1_int_handler
//	def_irq_handler can_int_handler
//    def_irq_handler exi0_int_handler
//    def_irq_handler exi1_int_handler
//    def_irq_handler exi2_3_int_handler
//    def_irq_handler exi4_9_int_handler
//    def_irq_handler exi10_15_int_handler
//	def_irq_handler cnta_int_handler
//	def_irq_handler lpt_int_handler
//	def_irq_handler wwdt_int_handler
//	def_irq_handler rtc_int_handler
//    def_irq_handler cmp0_int_handler
//	def_irq_handler cmp1_int_handler
//	def_irq_handler cmp2_int_handler
//	def_irq_handler led_int_handler
//	def_irq_handler bt0_int_handler
//	def_irq_handler bt1_int_handler
//	def_irq_handler bt2_int_handler
//	def_irq_handler bt3_int_handler
	

