################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include emlib/subdir.mk
-include Drivers/subdir.mk
-include CMSIS/EFM32GG/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: myRTOS.c.axf

# Tool invocations
myRTOS.c.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m3 -mthumb -T "myRTOS.c.ld" -Xlinker --gc-sections -Xlinker -Map="myRTOS.c.map" --specs=nano.specs -o myRTOS.c.axf "./CMSIS/EFM32GG/startup_efm32gg.o" "./CMSIS/EFM32GG/system_efm32gg.o" "./Drivers/caplesense.o" "./Drivers/segmentlcd.o" "./emlib/em_acmp.o" "./emlib/em_cmu.o" "./emlib/em_core.o" "./emlib/em_emu.o" "./emlib/em_gpio.o" "./emlib/em_int.o" "./emlib/em_lcd.o" "./emlib/em_lesense.o" "./emlib/em_system.o" "./src/context.o" "./src/main.o" "./src/myRTOS.o" "C:\Users\zachp\SimplicityStudio\v4_workspace\myRTOS\myRTOS.c\src\tasks8.o" -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: myRTOS.c.hex'
	arm-none-eabi-objcopy -O ihex "myRTOS.c.axf" "myRTOS.c.hex"
	@echo ' '

	@echo 'Building bin file: myRTOS.c.bin'
	arm-none-eabi-objcopy -O binary "myRTOS.c.axf" "myRTOS.c.bin"
	@echo ' '

	@echo 'Building s37 file: myRTOS.c.s37'
	arm-none-eabi-objcopy -O srec "myRTOS.c.axf" "myRTOS.c.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "myRTOS.c.axf" -A
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) myRTOS.c.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
