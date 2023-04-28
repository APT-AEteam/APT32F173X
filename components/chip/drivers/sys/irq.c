
/***********************************************************************//** 
 * \file  irq_port.c
 * \brief  CSI Source File for irq port
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-13 <td>V0.0 <td>ZJY     <td>initial
 * </table>
 * *********************************************************************
*/
//#include <csi_config.h>
#include <stdbool.h>
#include <irq.h>

extern csi_irqmap_t g_tIrqMap[];
extern void Default_Handler(void);
extern void (*g_irqvector[])(void);
extern void (*g_nmivector)(void);

/** \brief irq enable
 * 
 *  Enable irq in INTERRUPT
 * 
 *  \param[in] pIpBase: pointer of devices Base address
 *  \return none.
 */
void csi_irq_enable(void *pIpBase)
{
	csi_irqmap_t *ptMap = g_tIrqMap;

    while(ptMap->wRegBase) 
	{
        if((uint32_t)pIpBase == ptMap->wRegBase) 
		{
			csi_vic_enable_irq((int32_t)ptMap->wIrqNum);
            break;
        }
		
        ptMap++;
    }
}

/** \brief irq disable
 * 
 *  Disable irq in INTERRUPT
 * 
 *  \param[in] pIpBase: pointer of devices Base address
 *  \return none.
 */
void csi_irq_disable(void *pIpBase)
{
	csi_irqmap_t *ptMap = g_tIrqMap;

    while(ptMap->wRegBase) 
	{
        if((uint32_t)pIpBase == ptMap->wRegBase) 
		{
			csi_vic_disable_irq((int32_t)ptMap->wIrqNum);
            break;
        }
		
        ptMap++;
    }
}

/**
  \brief       register irq handler.
  \param[in]   irq_num Number of IRQ.
  \param[in]   irq_handler IRQ Handler.
  \return      None.
*/
void csi_irq_register(uint32_t irq_num, void *irq_handler)
{
    if (NMI_EXPn != irq_num) {
        g_irqvector[irq_num] = irq_handler;
    } else {
        g_nmivector = irq_handler;
    }
}

/**
  \brief       unregister irq handler.
  \param[in]   irq_num Number of IRQ.
  \return      None.
*/
void csi_irq_unregister(uint32_t irq_num)
{
    if (NMI_EXPn != irq_num) {
        g_irqvector[irq_num] = (void *)Default_Handler;
    } else {
        g_nmivector = (void *)Default_Handler;
    }
}

