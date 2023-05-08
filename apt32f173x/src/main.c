/***********************************************************************//** 
 * \file  main.c
 * \brief  MAIN description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  		<th>Version <th>Author	<th>Description
 * <tr><td> 2021-02-21  <td>V0.0  	<td>ZJY   	<td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include <csi_drv.h>
#include <iostring.h>

#include "demo.h"
/* Private macro-----------------------------------------------------------*/

/* externs function--------------------------------------------------------*/
extern void system_init(void);
extern void board_init(void);
extern void user_demo(void);

/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

int main()
{
    system_init();
	board_init();	
	csi_pin_set_mux(PA6, PA6_OUTPUT);		//PA6 output
	csi_pin_set_high(PA6);					//PA6 output high;
	
	my_printf("Hello World~~~~~~~\n");		//print message
	user_demo();							//demo

	while(1)
	{	
		mdelay(100);
		csi_pin_toggle(PA6);				//PA6 toggle
	}
	
    return 0;
}
