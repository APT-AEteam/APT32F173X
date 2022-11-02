##
## Auto Generated makefile by CDK
## Do not modify this file, and any manual changes will be erased!!!   
##
## BuildSet
ProjectName            :=apt32f173x
ConfigurationName      :=BuildSet
WorkspacePath          :=./
ProjectPath            :=./
IntermediateDirectory  :=Obj
OutDir                 :=$(IntermediateDirectory)
User                   :=yuanym
Date                   :=02/11/2022
CDKPath                :=D:/C-Sky/CDK
ToolchainPath          :=D:/C-Sky/CDKRepo/Toolchain/XTGccElfNewlib/V2.2.4/R/
LinkerName             :=riscv64-unknown-elf-gcc
LinkerNameoption       :=
SIZE                   :=riscv64-unknown-elf-size
READELF                :=riscv64-unknown-elf-readelf
CHECKSUM               :=crc32
SharedObjectLinkerName :=
ObjectSuffix           :=.o
DependSuffix           :=.d
PreprocessSuffix       :=.i
DisassemSuffix         :=.asm
IHexSuffix             :=.ihex
BinSuffix              :=.bin
ExeSuffix              :=.elf
LibSuffix              :=.a
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
ElfInfoSwitch          :=-hlS
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
UnPreprocessorSwitch   :=-U
SourceSwitch           :=-c 
ObjdumpSwitch          :=-S
ObjcopySwitch          :=-O ihex
ObjcopyBinSwitch       :=-O binary
OutputFile             :=$(ProjectName)
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :=apt32f173x.txt
MakeDirCommand         :=mkdir
LinkOptions            :=   -nostartfiles  -Wl,--gc-sections  -T../board/gcc_flash.ld  -mabi=ilp32d -mtune=e906 -march=rv32imafdcpzpsfoperand_xtheade 
LinkOtherFlagsOption   :=  
IncludePackagePath     :=
IncludeCPath           :=$(IncludeSwitch)../components/components/demo/include $(IncludeSwitch)../components/chip/include $(IncludeSwitch)../components/chip/drivers/sys $(IncludeSwitch)../board/include $(IncludeSwitch)../components/console/include $(IncludeSwitch)../components/csi/include $(IncludeSwitch)../components/csi/include/core $(IncludeSwitch)../components/csi/include/drv  $(IncludeSwitch)include $(IncludeSwitch).  
IncludeAPath           :=$(IncludeSwitch)../components/components/demo/include $(IncludeSwitch)../components/chip/include $(IncludeSwitch)../components/chip/drivers/sys $(IncludeSwitch)../board/include $(IncludeSwitch)../components/console/include $(IncludeSwitch)../components/csi/include $(IncludeSwitch)../components/csi/include/core $(IncludeSwitch)../components/csi/include/drv  $(IncludeSwitch)include $(IncludeSwitch).  
Libs                   := -Wl,--start-group $(LibrarySwitch)apt32f173x_demo $(LibrarySwitch)apt32f173x_chip $(LibrarySwitch)apt32f173x_evb $(LibrarySwitch)apt_console $(LibrarySwitch)apt_csi  -Wl,--end-group  
ArLibs                 := 
PackagesLibPath        :=$(LibraryPathSwitch)../components/components/demo/lib $(LibraryPathSwitch)../components/chip/lib $(LibraryPathSwitch)../board/lib $(LibraryPathSwitch)../components/console/lib $(LibraryPathSwitch)../components/csi/lib $(LibraryPathSwitch)../components/sdk_173x/lib 
LibPath                :=$(LibraryPathSwitch)Obj  $(PackagesLibPath) 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       :=riscv64-unknown-elf-ar rcu
CXX      :=riscv64-unknown-elf-g++
CC       :=riscv64-unknown-elf-gcc
AS       :=riscv64-unknown-elf-gcc
OBJDUMP  :=riscv64-unknown-elf-objdump
OBJCOPY  :=riscv64-unknown-elf-objcopy
CXXFLAGS := $(PreprocessorSwitch)USE_APT32F173X_DEMO $(PreprocessorSwitch)USE_APT32F173X_CHIP $(PreprocessorSwitch)USE_APT32F173X_EVB $(PreprocessorSwitch)USE_APT_CONSOLE $(PreprocessorSwitch)USE_APT_CSI $(PreprocessorSwitch)USE_SDK_173X  -Og  -g3  -ffunction-sections -Wno-unused-function -Og  -g  -Wall  -ffunction-sections -nostdlib -Wpointer-arith -Wl,-EL -fdata-sections -fdata-sections -g $(PreprocessorSwitch)CONFIG_ARCH_INTERRUPTSTACK=1024 $(PreprocessorSwitch)CONFIG_DEBUG_MODE=1 $(PreprocessorSwitch)CONFIG_KERNEL_NONE=1 $(PreprocessorSwitch)CONFIG_SUPPORT_TSPEND=1 $(PreprocessorSwitch)CONFIG_XIP=1   -mabi=ilp32d -mtune=e906 -march=rv32imafdcpzpsfoperand_xtheade   -Og  -g3 
CFLAGS   := $(PreprocessorSwitch)USE_APT32F173X_DEMO $(PreprocessorSwitch)USE_APT32F173X_CHIP $(PreprocessorSwitch)USE_APT32F173X_EVB $(PreprocessorSwitch)USE_APT_CONSOLE $(PreprocessorSwitch)USE_APT_CSI $(PreprocessorSwitch)USE_SDK_173X  -Og  -g3  -ffunction-sections -Wno-unused-function -Og  -g  -Wall  -ffunction-sections -nostdlib -Wpointer-arith -Wl,-EL -fdata-sections -fdata-sections -g $(PreprocessorSwitch)CONFIG_ARCH_INTERRUPTSTACK=1024 $(PreprocessorSwitch)CONFIG_DEBUG_MODE=1 $(PreprocessorSwitch)CONFIG_KERNEL_NONE=1 $(PreprocessorSwitch)CONFIG_SUPPORT_TSPEND=1 $(PreprocessorSwitch)CONFIG_XIP=1   -mabi=ilp32d -mtune=e906 -march=rv32imafdcpzpsfoperand_xtheade   -Og  -g3 
ASFLAGS  := $(PreprocessorSwitch)USE_APT32F173X_DEMO $(PreprocessorSwitch)USE_APT32F173X_CHIP $(PreprocessorSwitch)USE_APT32F173X_EVB $(PreprocessorSwitch)USE_APT_CONSOLE $(PreprocessorSwitch)USE_APT_CSI $(PreprocessorSwitch)USE_SDK_173X   -Wa,--gdwarf2 $(PreprocessorSwitch)CONFIG_ARCH_INTERRUPTSTACK=1024 $(PreprocessorSwitch)CONFIG_DEBUG_MODE=1 $(PreprocessorSwitch)CONFIG_KERNEL_NONE=1 $(PreprocessorSwitch)CONFIG_SUPPORT_TSPEND=1 $(PreprocessorSwitch)CONFIG_XIP=1   -mabi=ilp32d -mtune=e906 -march=rv32imafdcpzpsfoperand_xtheade   -Wa,--gdwarf2 


Objects0=$(IntermediateDirectory)/src_main$(ObjectSuffix) $(IntermediateDirectory)/src_user_demo$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all
all: $(IntermediateDirectory)/$(OutputFile)

$(IntermediateDirectory)/$(OutputFile):  $(Objects) Always_Link 
	$(LinkerName) $(OutputSwitch) $(IntermediateDirectory)/$(OutputFile)$(ExeSuffix) $(LinkerNameoption) $(LinkOtherFlagsOption)  -Wl,-Map=$(ProjectPath)/Lst/$(OutputFile).map  @$(ObjectsFileList)  $(LinkOptions) $(LibPath) $(Libs)
	@mv $(ProjectPath)/Lst/$(OutputFile).map $(ProjectPath)/Lst/$(OutputFile).temp && $(READELF) $(ElfInfoSwitch) $(ProjectPath)/Obj/$(OutputFile)$(ExeSuffix) > $(ProjectPath)/Lst/$(OutputFile).map && echo ====================================================================== >> $(ProjectPath)/Lst/$(OutputFile).map && cat $(ProjectPath)/Lst/$(OutputFile).temp >> $(ProjectPath)/Lst/$(OutputFile).map && rm -rf $(ProjectPath)/Lst/$(OutputFile).temp
	$(OBJDUMP) $(ObjdumpSwitch) $(ProjectPath)/$(IntermediateDirectory)/$(OutputFile)$(ExeSuffix)  > $(ProjectPath)/Lst/$(OutputFile)$(DisassemSuffix) 
	@echo size of target:
	@$(SIZE) $(ProjectPath)$(IntermediateDirectory)/$(OutputFile)$(ExeSuffix) 
	@echo -n checksum value of target:  
	@$(CHECKSUM) $(ProjectPath)/$(IntermediateDirectory)/$(OutputFile)$(ExeSuffix) 
	@$(ProjectName).modify.bat $(IntermediateDirectory) $(OutputFile)$(ExeSuffix) 

Always_Link:


##
## Objects
##
$(IntermediateDirectory)/src_main$(ObjectSuffix): src/main.c  
	$(CC) $(SourceSwitch) src/main.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/src_main$(ObjectSuffix) -MF$(IntermediateDirectory)/src_main$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/src_main$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/src_main$(PreprocessSuffix): src/main.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/src_main$(PreprocessSuffix) src/main.c

$(IntermediateDirectory)/src_user_demo$(ObjectSuffix): src/user_demo.c  
	$(CC) $(SourceSwitch) src/user_demo.c $(CFLAGS) -MMD -MP -MT$(IntermediateDirectory)/src_user_demo$(ObjectSuffix) -MF$(IntermediateDirectory)/src_user_demo$(DependSuffix) $(ObjectSwitch)$(IntermediateDirectory)/src_user_demo$(ObjectSuffix) $(IncludeCPath) $(IncludePackagePath)
Lst/src_user_demo$(PreprocessSuffix): src/user_demo.c
	$(CC) $(CFLAGS)$(IncludeCPath) $(PreprocessOnlySwitch) $(OutputSwitch) Lst/src_user_demo$(PreprocessSuffix) src/user_demo.c


-include $(IntermediateDirectory)/*$(DependSuffix)
