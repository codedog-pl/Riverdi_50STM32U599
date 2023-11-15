################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Middlewares/USBX/UX\ Host\ CoreStack/subdir.mk
-include Middlewares/USBX/UX\ Host\ Controllers/subdir.mk
-include Middlewares/USBX/UX\ Host\ Class\ MSC/subdir.mk
-include Middlewares/USBX/Core\ System/subdir.mk
-include Middlewares/ThreadX/Core/subdir.mk
-include Middlewares/FileX/SD\ interface/subdir.mk
-include Middlewares/FileX/Core/subdir.mk
-include Drivers/STM32U5xx_HAL_Driver/subdir.mk
-include Drivers/Custom/subdir.mk
-include Drivers/CMSIS/subdir.mk
-include Drivers/BSP/Components/mx25lm51245g/subdir.mk
-include Application/User/gui/subdir.mk
-include Application/User/generated/subdir.mk
-include Application/User/USBX/Target/subdir.mk
-include Application/User/USBX/App/subdir.mk
-include Application/User/TouchGFX/target/generated/subdir.mk
-include Application/User/TouchGFX/target/subdir.mk
-include Application/User/TouchGFX/App/subdir.mk
-include Application/User/Startup/subdir.mk
-include Application/User/HMI/subdir.mk
-include Application/User/FileX/Target/subdir.mk
-include Application/User/FileX/App/subdir.mk
-include Application/User/Core/subdir.mk
-include Application/User/AZURE_RTOS/App/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := U599_Template
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
U599_Template.elf \

MAP_FILES += \
U599_Template.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
U599_Template.list \


# All Target
all: main-build

# Main-build Target
main-build: U599_Template.elf secondary-outputs

# Tool invocations
U599_Template.elf U599_Template.map: $(OBJS) $(USER_OBJS) D:\Source\Embedded\U599\U599_Template\STM32CubeIDE\STM32U599NJHXQ_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-g++ -o "U599_Template.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m33 -T"D:\Source\Embedded\U599\U599_Template\STM32CubeIDE\STM32U599NJHXQ_FLASH.ld" --specs=nosys.specs -Wl,-Map="U599_Template.map" -Wl,--gc-sections -static -LE:/TouchGFX/4.21.4/designer/Riverdi_50STM32U599_DoubleBuffer_24b-Copy/Middlewares/ST/touchgfx/lib/core/cortex_m33/gcc -LE:/TouchGFX/4.21.4/designer/Riverdi_50STM32U599_DoubleBuffer_24b/Middlewares/ST/touchgfx/lib/core/cortex_m33/gcc -LE:/TouchGFX/4.22.0/designer/Riverdi_50STM32U599_DoubleBuffer_24b/Middlewares/ST/touchgfx/lib/core/cortex_m33/gcc -LD:/Source/Embedded/Riverdi/Riverdi_U599_Test/Middlewares/ST/touchgfx/lib/core/cortex_m33/gcc -LD:/Source/Embedded/U599/U599_Template/Middlewares/ST/touchgfx/lib/core/cortex_m33/gcc -L../../Middlewares/ST/touchgfx_components/gpu2d/NemaGFX/lib/core/cortex_m33/gcc -L../../Middlewares/ST/touchgfx_components/gpu2d/TouchGFXNema/lib/cortex_m33/gcc --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

U599_Template.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "U599_Template.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) U599_Template.elf U599_Template.list U599_Template.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
