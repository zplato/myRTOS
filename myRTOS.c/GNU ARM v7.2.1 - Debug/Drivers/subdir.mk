################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/caplesense.c \
../Drivers/segmentlcd.c 

OBJS += \
./Drivers/caplesense.o \
./Drivers/segmentlcd.o 

C_DEPS += \
./Drivers/caplesense.d \
./Drivers/segmentlcd.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/caplesense.o: ../Drivers/caplesense.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' -IC:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.7/platform/CMSIS/Include -IC:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.7/hardware/kit/common/bsp -IC:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.7/platform/emlib/inc -IC:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.7/hardware/kit/common/drivers -IC:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.7/platform/Device/SiliconLabs/EFM32GG/Include -IC:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.7/hardware/kit/EFM32GG_STK3700/config -IC:/FreeRTOS/FreeRTOSv202012.00/FreeRTOS/Source/include -IC:/FreeRTOS/FreeRTOSv202012.00/FreeRTOS/Source/portable/GCC/ARM_CM3 -IC:/Users/zachp/SimplicityStudio/v4_workspace/myRTOS/myRTOS.c/src -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"Drivers/caplesense.d" -MT"Drivers/caplesense.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/segmentlcd.o: ../Drivers/segmentlcd.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' -IC:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.7/platform/CMSIS/Include -IC:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.7/hardware/kit/common/bsp -IC:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.7/platform/emlib/inc -IC:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.7/hardware/kit/common/drivers -IC:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.7/platform/Device/SiliconLabs/EFM32GG/Include -IC:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.7/hardware/kit/EFM32GG_STK3700/config -IC:/FreeRTOS/FreeRTOSv202012.00/FreeRTOS/Source/include -IC:/FreeRTOS/FreeRTOSv202012.00/FreeRTOS/Source/portable/GCC/ARM_CM3 -IC:/Users/zachp/SimplicityStudio/v4_workspace/myRTOS/myRTOS.c/src -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"Drivers/segmentlcd.d" -MT"Drivers/segmentlcd.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


