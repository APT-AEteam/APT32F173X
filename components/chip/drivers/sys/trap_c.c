/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     trap_c.c
 * @brief    source file for the trap process
 * @version  V1.0
 * @date     12. December 2017
 ******************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
//#include <csi_config.h>
#include <csi_core.h>
#include <iostring.h>

void (*trap_c_callback)(void);

void trap_c(uint32_t *pwRegs)
{
    int i;
    uint32_t vec = 0;

    vec = __get_MCAUSE() & 0x3FF;

    my_printf("CPU Exception: NO.%d", vec);
    my_printf("\n");

    for (i = 0; i < 31; i++) {
        my_printf("x%d: %08x\t", i + 1, pwRegs[i]);

        if ((i % 4) == 3) {
            my_printf("\n");
        }
    }

    my_printf("\n");
    my_printf("mepc   : %08x\n", pwRegs[31]);
    my_printf("mstatus: %08x\n", pwRegs[32]);

    if (trap_c_callback) {
        trap_c_callback();
    }

    while (1);
}

