################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/portable/MemMang/heap_2.obj: C:/ti/tivaware_c_series_2_1_4_178/third_party/FreeRTOS/Source/portable/MemMang/heap_2.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -O2 --include_path="C:/ti/tivaware_c_series_2_1_4_178/third_party/FreeRTOS/Source/include" --include_path="C:/ti/tivaware_c_series_2_1_4_178/third_party/FreeRTOS/Source/portable/CCS/ARM_CM4F" --include_path="C:/ti/tivaware_c_series_2_1_4_178" --include_path="C:/ti/tivaware_c_series_2_1_4_178/inc" --include_path="C:/ti/tivaware_c_series_2_1_4_178/examples/boards/ek-tm4c129exl" --include_path="C:/Users/mahon/workspace_v12/LAB5A" --include_path="C:/ti/ccs1210/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --define=TARGET_IS_TM4C129_RA1 --define=DEBUG --define=ccs="ccs" --define=PART_TM4C129ENCPDT -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="FreeRTOS/portable/MemMang/$(basename $(<F)).d_raw" --obj_directory="FreeRTOS/portable/MemMang" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


