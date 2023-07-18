# board组件信息
系统初始化，板级配置参数，ld文件，svc文件等，依赖于CHIP组件

# Release Notes

======================================================
Version: V1.0.1
Release Date: 2023.7.17

修改linker文件，将0x104区域分配为空置状态，保证该地址的数据为0
	  
======================================================	  


======================================================
Version: V1.0.0
Release Date: 2023.3.14

Release Notes
初始版本。

173x包含两种型号：1731和1732
(1)、如果使用1731，可根据需求选择以下ld文件
      gcc_flash_1731.ld，ROM代码区为64K,RAM数据区只用到SRAM0，大小为16K。
      gcc_sram_1731.ld，代码和数据都在RAM中运行，RAM只用到SRAM0，大小为16K。
(2)、如果使用1732，可根据需求选择以下ld文件
      gcc_flash_1732.ld，ROM代码区为128K,RAM数据区使用SRAM0加上SRAM1，SRAM0和SRAM1均配置为DSRAM，大小合计为32K。
      gcc_sram_1732.ld，代码和数据都在RAM中运行,RAM使用SRAM0加上SRAM1，SRAM0和SRAM1均配置为DSRAM，大小合计为32K。
      gcc_flash_dram24k_iram8k_1732.ld，ROM代码区为128K,RAM数据区使用SRAM0，大小为16k，SRAM1配置为ISRAM，大小为16K。
      gcc_flash_dram16k_iram16k_1732.ld，ROM代码区为128K,RAM数据区使用SRAM0，大小为24k， SRAM1配置为ISRAM，大小为8K。
	  
======================================================	  