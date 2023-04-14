
#include "csp.h"

#define SRAMSTARTADD      (0X20006100)
#define SRAMENDADD        (0X20007FFF)
#define SRAM_TEST_DATA0    0x55
#define SRAM_TEST_DATA1    0xAA

uint8_t   *pSramBase = (uint8_t *)(0X20006000);
volatile  uint8_t ch;

/** \brief SRAM测试，SRAM0 24K + SRAM1 8K 均为DSRAM使用
 *   此函数使用，需要配置相应的linker文件,指定代码区域
 *    分区为：SRAM 分区控制配置0表示： SRAM0 24K。 地址范围0x20000000~0x20005FFF
 *                                     SRAM1 8K。 地址范围0x20006000~0x20007FFF    
 *   		
 *    SRAM1指令存取功能控制，配置为0，表示SRAM1作为DSRAM用（作为普通SRAM使用） 		
 *          
 *  \param[in] none
 *  \return error code
 */
__attribute__((section("func")))void sram_dsram24k_dsram8k_test0(void)
{   
	SYSCON->OPT1 = (SYSCON->OPT1 & ~(SRAMBLKCTRL_MSK)) |  ( 0 << SRAMBLKCTRL_POS );  //测试版本0h和1h是反的  FPGA为0827版本
	
                                                                                       /* SRAM 分区控制。SRAM0始终为DSRAM，SRAM1的功能受OPT1[24]位控制
																						0h： SRAM0 24K。 地址范围0x20000000~0x20005FFF
																							 SRAM1 8K。 地址范围0x20006000~0x20007FFF
																							  * 此模式要LD文件的SRAM设为48K
																							  * 
																						1h:  SRAM0 16K。 地址范围0x20000000~0x20003FFF
																							 SRAM1 16K。 地址范围0x20006000~0x20009FFF
																							  * 此模式要LD文件的SRAM设为16K
																					  */
	SYSCON->OPT1 = (SYSCON->OPT1 & ~(SRAM1FUNCCTRL_MSK)) | ( 0 << SRAM1FUNCCTRL_POS );
																					   /*SRAM1功能控制
																						0h：SRAM1作为DSRAM用
																						1h：SRAM1作为ISRAM用，不可写																  
																					   */
	while (1){
		  NOP;
		  ch = 2;
		  mdelay(100);	
	}		
	
}
/** \brief SRAM测试
 *    SRAM0 24K 均为DSRAM使用,SRAM1 8K 均为ISRAM使用,配置为DSRAM，可对0x20006000~0x20007FF区域进行写操作
 * 
 *    分区为：SRAM 分区控制配置0表示： SRAM0 24K。 地址范围0x20000000~0x20005FFF
 *                                     SRAM1 8K。 地址范围0x20006000~0x20007FFF    
 *   		
 *    SRAM1指令存取功能控制，配置为0，表示SRAM1作为DSRAM用（作为普通SRAM使用） 	
 *          
 *  \param[in] none
 *  \return error code
 */
void sram_dsram24k_dsram8k_test1(void)
{
	uint32_t i, wDataLeng;
	
	SYSCON->OPT1 = (SYSCON->OPT1 & ~(SRAMBLKCTRL_MSK)) |  ( 0 << SRAMBLKCTRL_POS );  //测试版本0h和1h是反的  FPGA为0827版本
	
                                                                                       /* SRAM 分区控制。SRAM0始终为DSRAM，SRAM1的功能受OPT1[24]位控制
																						0h： SRAM0 24K。 地址范围0x20000000~0x20006FFF
																							 SRAM1 8K。 地址范围0x20006000~0x20007FFF
																							  * 此模式要LD文件的SRAM设为48K
																							  * 
																						1h:  SRAM0 16K。 地址范围0x20000000~0x20003FFF
																							 SRAM1 16K。 地址范围0x20006000~0x20009FFF
																							  * 此模式要LD文件的SRAM设为16K
																					  */
	SYSCON->OPT1 = (SYSCON->OPT1 & ~(SRAM1FUNCCTRL_MSK)) | ( 0 << SRAM1FUNCCTRL_POS );
																					   /*SRAM1功能控制
																						0h：SRAM1作为DSRAM用
																						1h：SRAM1作为ISRAM用，不可写																  
																					   */
	wDataLeng=SRAMENDADD - SRAMSTARTADD;																   
										
	for(i=0;i<=wDataLeng;i++)
	{ 
		pSramBase[i]=SRAM_TEST_DATA0;                                  //程序将会异常
		//NOP;
	}	 
	for(i=0;i<=wDataLeng;i++)
    { 
		if(pSramBase[i] != SRAM_TEST_DATA0){break;}			 
	}		
	if(i!=wDataLeng+1)
	{
		while (1){
				NOP;
				ch=0;
		}
	}else{			
		for(i=0;i<=wDataLeng;i++)
			{ 
				pSramBase[i]=SRAM_TEST_DATA1;						
				NOP;
			}
		i=0;	 
		for(i=0;i<=wDataLeng;i++)
			{ 
				if(pSramBase[i] != SRAM_TEST_DATA1){break;}			 
			}		
		if(i!=wDataLeng+1)
		{
			while (1){
				NOP;
				ch=1;
			}
		}
						
	}
	while (1){
		NOP;
		ch=2;
	}		
}

/** \brief SRAM测试
 *    SRAM0 24K 均为DSRAM使用,SRAM1 8K 均为ISRAM使用,配置为ISRAM，将不能对0x20006000~0x20007FF区域进行写操作
 * 
 *    分区为：SRAM 分区控制配置0表示： SRAM0 24K。 地址范围0x20000000~0x20005FFF
 *                                     SRAM1 8K。 地址范围0x20006000~0x20007FFF    
 *   		
 *    SRAM1指令存取功能控制，配置为1，SRAM1作为ISRAM用，不可写（作为指令空间使用）	
 *          
 *  \param[in] none
 *  \return error code
 */
void sram_dsram24k_iram8k_test(void)
{
	uint32_t i, wDataLeng;
	
	SYSCON->OPT1 = (SYSCON->OPT1 & ~(SRAMBLKCTRL_MSK)) |  ( 0 << SRAMBLKCTRL_POS );  //测试版本0h和1h是反的  FPGA为0827版本
	
                                                                                       /* SRAM 分区控制。SRAM0始终为DSRAM，SRAM1的功能受OPT1[24]位控制
																						0h： SRAM0 24K。 地址范围0x20000000~0x20006FFF
																							 SRAM1 8K。 地址范围0x20006000~0x20007FFF
																							  * 此模式要LD文件的SRAM设为48K
																							  * 
																						1h:  SRAM0 16K。 地址范围0x20000000~0x20003FFF
																							 SRAM1 16K。 地址范围0x20006000~0x20009FFF
																							  * 此模式要LD文件的SRAM设为16K
																					  */
	SYSCON->OPT1 = (SYSCON->OPT1 & ~(SRAM1FUNCCTRL_MSK)) | ( 1 << SRAM1FUNCCTRL_POS );
																					   /*SRAM1功能控制
																						0h：SRAM1作为DSRAM用
																						1h：SRAM1作为ISRAM用，不可写																  
																					   */
	wDataLeng=SRAMENDADD - SRAMSTARTADD;																   
										
	for(i=0;i<=wDataLeng;i++)
	{ 
		pSramBase[i]=SRAM_TEST_DATA0;                                  //程序将会异常
		//NOP;
	}	 
	for(i=0;i<=wDataLeng;i++)
    { 
		if(pSramBase[i] != SRAM_TEST_DATA0){break;}			 
	}		
	if(i!=wDataLeng+1)
	{
		while (1){
				NOP;
				ch=0;
		}
	}else{			
		for(i=0;i<=wDataLeng;i++)
			{ 
				pSramBase[i]=SRAM_TEST_DATA1;						
				NOP;
			}
		i=0;	 
		for(i=0;i<=wDataLeng;i++)
			{ 
				if(pSramBase[i] != SRAM_TEST_DATA1){break;}			 
			}		
		if(i!=wDataLeng+1)
		{
			while (1){
				NOP;
				ch=1;
			}
		}
						
	}
	while (1){
		NOP;
		ch=2;
	}		
}