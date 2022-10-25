
#include "csp.h"
//#include "soc.h" 
//#include <drv/pin.h>
//#include <drv/etb.h>
//#include <drv/gpta.h>
extern void udelay11(uint32_t us);

#define SRAMBLKCTRL_POS   (26)
#define SRAMBLKCTRL_MSK	  (0x01 << SRAMBLKCTRL_POS)
#define SRAM1FUNCCTRL_POS  (24)
#define SRAM1FUNCCTRL_MSK (0x01 << SRAM1FUNCCTRL_POS)

#define SRAMSTARTADD    (0X20008100)
#define SRAMENDADD      (0X2000BFFF)
uint32_t  buff[4];//  __attribute__ ((at(0X20004000)));
uint8_t SRAM_TEST_DATA0=0x55;
uint8_t SRAM_TEST_DATA1=0xAA;
uint32_t SRAMADD;
uint32_t TEST_NO;
uint8_t *pSramBase = (uint8_t *)(0X20008000);
//uint32_t ADD_deviation;

volatile uint8_t ch;
uint32_t i;
int iRet = 0;

__attribute__((section("func")))void Sram_16or32_Test1(void)
{   SYSCON->OPT1 = (SYSCON->OPT1 & ~(SRAMBLKCTRL_MSK)) |  ( 0 << SRAMBLKCTRL_POS );  //测试版本0h和1h是反的  FPGA为0827版本
	
                                                                                       /* SRAM 分区控制。SRAM0始终为DSRAM，SRAM1的功能受OPT1[24]位控制
																						0h： SRAM0 32K。 地址范围0x20000000~0x20007FFF
																							 SRAM1 16K。 地址范围0x20008000~0x2000BFFF
																							  * 此模式要LD文件的SRAM设为48K
																							  * 
																						1h:  SRAM0 16K。 地址范围0x20000000~0x20003FFF
																							 SRAM1 32K。 地址范围0x20004000~0x2000BFFF
																							  * 此模式要LD文件的SRAM设为16K
																					  */
	SYSCON->OPT1 = (SYSCON->OPT1 & ~(SRAM1FUNCCTRL_MSK)) | ( 0 << SRAM1FUNCCTRL_POS );
																					   /*SRAM1功能控制
																						0h：SRAM1作为DSRAM用
																						1h：SRAM1作为ISRAM用，不可写																  
																					   */
	udelay11(1);
	while (1){
					  NOP;
					  ch=2;
			  }		
	
}

void Sram_16or32_Test(void)
{
	SYSCON->OPT1 = (SYSCON->OPT1 & ~(SRAMBLKCTRL_MSK)) |  ( 0 << SRAMBLKCTRL_POS );  //测试版本0h和1h是反的  FPGA为0827版本
	
                                                                                       /* SRAM 分区控制。SRAM0始终为DSRAM，SRAM1的功能受OPT1[24]位控制
																						0h： SRAM0 32K。 地址范围0x20000000~0x20007FFF
																							 SRAM1 16K。 地址范围0x20008000~0x2000BFFF
																							  * 此模式要LD文件的SRAM设为48K
																							  * 
																						1h:  SRAM0 16K。 地址范围0x20000000~0x20003FFF
																							 SRAM1 32K。 地址范围0x20004000~0x2000BFFF
																							  * 此模式要LD文件的SRAM设为16K
																					  */
	SYSCON->OPT1 = (SYSCON->OPT1 & ~(SRAM1FUNCCTRL_MSK)) | ( 0 << SRAM1FUNCCTRL_POS );
																					   /*SRAM1功能控制
																						0h：SRAM1作为DSRAM用
																						1h：SRAM1作为ISRAM用，不可写																  
																					   */
	TEST_NO=SRAMENDADD - SRAMSTARTADD;																   
											  		
				for(i=0;i<=TEST_NO;i++)
					{ 
						pSramBase[i]=SRAM_TEST_DATA0;
						//NOP;
					}
				i=0;	 
				for(i=0;i<=TEST_NO;i++)
				   { 
						if(pSramBase[i] != SRAM_TEST_DATA0){break;}			 
					}		
				iRet =i;
		if(i!=TEST_NO+1)
		{
			while (1){
					  NOP;
					  ch=0;
				}
		}else{			
			    for(i=0;i<=TEST_NO;i++)
					{ 
						pSramBase[i]=SRAM_TEST_DATA1;						
						NOP;
					}
				i=0;	 
				for(i=0;i<=TEST_NO;i++)
				    { 
						if(pSramBase[i] != SRAM_TEST_DATA1){break;}			 
					}		
				iRet =i;
                if(i!=TEST_NO+1)
		          {
					while (1){
							  NOP;
							  ch=1;
				              }
				  }
//			    for(i=0;i<=TEST_NO;i++)
//					{ 						
//						pSramBase[i]=i;
//						NOP;
//					}
							
		}
//		buff[0]=*(uint32_t *)(0X20004000);
//		buff[1]=*(uint32_t *)(0X20004100);
//		buff[2]=*(uint32_t *)(0X20004200);
//		buff[3]=*(uint32_t *)(0X20004300);
	while (1){
					  NOP;
					  ch=2;
			  }		
}