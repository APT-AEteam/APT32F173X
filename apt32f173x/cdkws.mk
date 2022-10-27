.PHONY: clean All Project_Title apt32f173x_demo apt32f173x_chip apt32f173x_evb apt_console apt_csi Project_Build

All: Project_Title apt32f173x_demo apt32f173x_chip apt32f173x_evb apt_console apt_csi Project_Build

Project_Title:
	@echo "----------Building project:[ apt32f173x - BuildSet ]----------"

apt32f173x_demo:
	@make -r -f Obj/BuildSet/Packages/apt32f173x_demo/v1.0.0/Makefile -j 12 -C  ./ 

apt32f173x_chip:
	@make -r -f Obj/BuildSet/Packages/apt32f173x_chip/v1.0.0/Makefile -j 12 -C  ./ 

apt32f173x_evb:
	@make -r -f Obj/BuildSet/Packages/apt32f173x_evb/v1.0.0/Makefile -j 12 -C  ./ 

apt_console:
	@make -r -f Obj/BuildSet/Packages/apt_console/v1.0.0/Makefile -j 12 -C  ./ 

apt_csi:
	@make -r -f Obj/BuildSet/Packages/apt_csi/v1.0.0/Makefile -j 12 -C  ./ 


Project_Build:
	@make -r -f apt32f173x.mk -j 12 -C  ./ 


clean:
	@echo "----------Cleaning project:[ apt32f173x - BuildSet ]----------"

