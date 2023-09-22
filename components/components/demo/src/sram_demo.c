
/* include ----------------------------------------------------------------*/

#include "gpio.h"
#include "reliability.h"
#include "board_config.h"

/* externs function--------------------------------------------------------*/
/* private function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/


/** \brief user_fast_func，本函数会被写到SRAM1中，从而加速代码运行	
 *	
 *  	   此处用PA6翻转作为简单示例，用户可替换成自己需要加速的代码		
 *
 * 	注意：SRAM1最大可配置为16K ISRAM。
 *      
 *  \param[in] none
 *  \return error code
 */
__attribute__((section("func")))void user_fast_func(void)
{  
	//用户需要加速的代码
	csi_gpio_set_mux(GPIOA, PA6, PA6_OUTPUT);
	
	while(1)
	{
		csi_gpio_toggle(GPIOA, PA6);
		mdelay(100);
	
	}		
	
}


/** \brief  1. 1732芯片有两块SRAM:SRAM0和SRAM1。
 * 
 * 				1）SRAM0和SRAM1大小有以下两种选择：
 * 
 * 					SRAM_24KRAM0_8KRAM1_CTRL ： SRAM0 24K。 地址范围0x20000000~0x20005FFF
 *                                         		SRAM1 8K。  地址范围0x20006000~0x20007FFF   
 * 				
 *    				SRAM_16KRAM0_16KRAM1_CTRL : SRAM0 16K。 地址范围0x20000000~0x20003FFF
 *                                          	SRAM1 16K。 地址范围0x20006000~0x20009FFF   
 * 				
 * 				2）SRAM0只能作为DSRAM
 *      
 *   			3）SRAM1可作为DSRAM，也可以作为ISRAM
 * 
 * 					SRAM1_ISRAM ：SRAM1 作为ISRAM使用
 *    
 * 					SRAM1_DSRAM ：SRAM1 作为DSRAM使用
 *            
 * 			
 * 			2.本例程提供了两种情况： SRAM1 8k作为ISRAM 和 SRAM1 16k作为ISRAM。
 * 			
 * 			3.例程使用步骤：
 * 
 * 				1）在board_config.h中打开USE_SRAM1_8K_AS_IRAM（或者USE_SRAM1_16K_AS_IRAM）定义
 * 				
 * 				2）链接文件选择gcc_flash_dram24k_iram8k_1732.ld（或者gcc_flash_dram24k_iram16k_1732.ld）
 * 
 * 				3）在user_fast_func中添加需要加速的代码。
 * 
 *  \param[in] none
 *  \return error code
 */
 


void use_sram1_as_isram_demo(void)
{

#if(USE_SRAM1_8K_AS_IRAM ==1)

	csi_set_sram(SRAM_24KRAM0_8KRAM1_CTRL,SRAM1_ISRAM);  
  
#elif(USE_SRAM1_16K_AS_IRAM ==1)
	csi_set_sram(SRAM_16KRAM0_16KRAM1_CTRL,SRAM1_ISRAM);	
	
#endif

	user_fast_func();
	
}
