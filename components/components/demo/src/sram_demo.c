
/* include ----------------------------------------------------------------*/
#include "reliability.h"
#include "iostring.h"

/* externs function--------------------------------------------------------*/
/* private function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

#define SRAMSTARTADD      (0X20006100)
#define SRAMENDADD        (0X20007FFF)
#define SRAM_TEST_DATA0    0x55
#define SRAM_TEST_DATA1    0xAA

static uint8_t   *s_pbySramBase = (uint8_t *)(0X20006000);
static volatile  uint8_t byCh;

/** \brief SRAM测试，SRAM0 24K + SRAM1 8K 均为DSRAM使用
 *   此函数使用，需要配置相应的linker文件gcc_flash_dram24k_iram8k_1732.ld,指定代码区域
 *   分区为：SRAM 分区控制配置0表示： SRAM0 24K。 地址范围0x20000000~0x20005FFF
 *                                     SRAM1 8K。 地址范围0x20006000~0x20007FFF    
 *   		
 *    SRAM1指令存取功能控制，配置为0，表示SRAM1作为DSRAM用（作为普通SRAM使用） 		
 *          
 *  \param[in] none
 *  \return error code
 */
__attribute__((section("func")))void sram_dsram24k_dsram8k_demo0(void)
{  
	csi_set_sram(SRAM_24KRAM0_8KRAM1_CTRL,SRAM1_DSARM_CTRL);
	while (1){
		  NOP;
		  byCh = 2;
		  mdelay(100);	
	}		
	
}
/** \brief SRAM测试,对SRAM 0X20006000区域进行相应的读写操作
 * 
 *    SRAM0 24K 均为DSRAM使用,SRAM1 8K 均为ISRAM使用,配置为DSRAM，可对0x20006000~0x20007FF区域进行写操作
 * 
 *    分区为：SRAM 分区控制配置0表示： SRAM0 24K。 地址范围0x20000000~0x20005FFF
 *                                     SRAM1 8K。 地址范围0x20006000~0x20007FFF    
 *   		
 *    SRAM1指令存取功能控制，配置为0，表示SRAM1作为DSRAM用（作为普通SRAM使用） 	
 * 
 *    测试结果：SRAM 0X20006000区域配置成ISRAM后，此区域不能进行相应的读写操作         
 *  \param[in] none
 *  \return error code
 */
void sram_dsram24k_dsram8k_demo1(void)
{
	uint32_t i, wDataLeng;
	
	csi_set_sram(SRAM_24KRAM0_8KRAM1_CTRL,SRAM1_DSARM_CTRL);
	wDataLeng=SRAMENDADD - SRAMSTARTADD;																   
										
	for(i=0;i<=wDataLeng;i++)
	{ 
		s_pbySramBase[i]=SRAM_TEST_DATA0;                                
		//NOP;
	}	 
	for(i=0;i<=wDataLeng;i++)
    { 
		if(s_pbySramBase[i] != SRAM_TEST_DATA0){break;}			 
	}		
	if(i!=wDataLeng+1)
	{
		while (1){
				NOP;
				byCh=0;
		}
	}else{			
		for(i=0;i<=wDataLeng;i++)
			{ 
				s_pbySramBase[i]=SRAM_TEST_DATA1;						
				NOP;
			}
		i=0;	 
		for(i=0;i<=wDataLeng;i++)
			{ 
				if(s_pbySramBase[i] != SRAM_TEST_DATA1){break;}			 
			}		
		if(i!=wDataLeng+1)
		{
			while (1){
				NOP;
				byCh=1;
			}
		}
						
	}
	while (1){
		NOP;
		byCh=2;
	}		
}

/** \brief SRAM测试，对SRAM 0X20006000区域进行相应的读写操作
 * 
 *    SRAM0 24K 均为DSRAM使用,SRAM1 8K 均为ISRAM使用,配置为ISRAM，将不能对0x20006000~0x20007FF区域进行写操作
 * 
 *    分区为：SRAM 分区控制配置0表示： SRAM0 24K。 地址范围0x20000000~0x20005FFF
 *                                     SRAM1 8K。 地址范围0x20006000~0x20007FFF    
 *   		
 *    SRAM1指令存取功能控制，配置为1，SRAM1作为ISRAM用，不可写（作为指令空间使用）	
 * 
 *    测试结果：SRAM 0X20006000区域配置成ISRAM后，此区域不能进行相应的读写操作     
 *  \param[in] none
 *  \return error code
 */
void sram_dsram24k_iram8k_demo(void)
{
	uint32_t i, wDataLeng;
	
	csi_set_sram(SRAM_24KRAM0_8KRAM1_CTRL,SRAM1_ISRAM_CTRL);
	
	wDataLeng=SRAMENDADD - SRAMSTARTADD;																   
										
	for(i=0;i<=wDataLeng;i++)
	{ 
		s_pbySramBase[i]=SRAM_TEST_DATA0;                                  //程序将会异常
		//NOP;
	}	 
	for(i=0;i<=wDataLeng;i++)
    { 
		if(s_pbySramBase[i] != SRAM_TEST_DATA0){break;}			 
	}		
	if(i!=wDataLeng+1)
	{
		while (1){
				NOP;
				byCh=0;
		}
	}else{			
		for(i=0;i<=wDataLeng;i++)
			{ 
				s_pbySramBase[i]=SRAM_TEST_DATA1;						
				NOP;
			}
		i=0;	 
		for(i=0;i<=wDataLeng;i++)
			{ 
				if(s_pbySramBase[i] != SRAM_TEST_DATA1){break;}			 
			}		
		if(i!=wDataLeng+1)
		{
			while (1){
				NOP;
				byCh=1;
			}
		}
						
	}
	while (1){
		NOP;
		byCh=2;
	}		
}