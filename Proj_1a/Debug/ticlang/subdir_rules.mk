################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
ticlang/%.o: ../ticlang/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccstheia131/ccs/tools/compiler/ti-cgt-armllvm_3.2.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"/Users/rylan/Desktop/mspm0_workspace/Proj_1a" -I"/Users/rylan/Desktop/mspm0_workspace/Proj_1a/Debug" -I"/Applications/ti/mspm0_sdk_2_00_00_03/source/third_party/CMSIS/Core/Include" -I"/Applications/ti/mspm0_sdk_2_00_00_03/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"ticlang/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


