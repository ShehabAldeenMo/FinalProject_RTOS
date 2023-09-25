################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../RTOS/Source/portable/GCC/ARM_CM3/port.c 

OBJS += \
./RTOS/Source/portable/GCC/ARM_CM3/port.o 

C_DEPS += \
./RTOS/Source/portable/GCC/ARM_CM3/port.d 


# Each subdirectory must supply rules for building sources it contributes
RTOS/Source/portable/GCC/ARM_CM3/%.o RTOS/Source/portable/GCC/ARM_CM3/%.su RTOS/Source/portable/GCC/ARM_CM3/%.cyclo: ../RTOS/Source/portable/GCC/ARM_CM3/%.c RTOS/Source/portable/GCC/ARM_CM3/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I../Inc -I"D:/Embedded Track/Embedded NGEN/03-STM Workspace/FinalProject_RTOS/RTOS/Source/portable/GCC/ARM_CM3" -I"D:/Embedded Track/Embedded NGEN/03-STM Workspace/FinalProject_RTOS/RTOS/Source" -I"D:/Embedded Track/Embedded NGEN/03-STM Workspace/FinalProject_RTOS/RTOS/Source/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-RTOS-2f-Source-2f-portable-2f-GCC-2f-ARM_CM3

clean-RTOS-2f-Source-2f-portable-2f-GCC-2f-ARM_CM3:
	-$(RM) ./RTOS/Source/portable/GCC/ARM_CM3/port.cyclo ./RTOS/Source/portable/GCC/ARM_CM3/port.d ./RTOS/Source/portable/GCC/ARM_CM3/port.o ./RTOS/Source/portable/GCC/ARM_CM3/port.su

.PHONY: clean-RTOS-2f-Source-2f-portable-2f-GCC-2f-ARM_CM3

