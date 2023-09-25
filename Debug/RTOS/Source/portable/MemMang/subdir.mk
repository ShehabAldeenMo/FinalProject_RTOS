################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../RTOS/Source/portable/MemMang/heap_4.c 

OBJS += \
./RTOS/Source/portable/MemMang/heap_4.o 

C_DEPS += \
./RTOS/Source/portable/MemMang/heap_4.d 


# Each subdirectory must supply rules for building sources it contributes
RTOS/Source/portable/MemMang/%.o RTOS/Source/portable/MemMang/%.su RTOS/Source/portable/MemMang/%.cyclo: ../RTOS/Source/portable/MemMang/%.c RTOS/Source/portable/MemMang/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I../Inc -I"D:/Embedded Track/Embedded NGEN/03-STM Workspace/FinalProject_RTOS/RTOS/Source/portable/GCC/ARM_CM3" -I"D:/Embedded Track/Embedded NGEN/03-STM Workspace/FinalProject_RTOS/RTOS/Source" -I"D:/Embedded Track/Embedded NGEN/03-STM Workspace/FinalProject_RTOS/RTOS/Source/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-RTOS-2f-Source-2f-portable-2f-MemMang

clean-RTOS-2f-Source-2f-portable-2f-MemMang:
	-$(RM) ./RTOS/Source/portable/MemMang/heap_4.cyclo ./RTOS/Source/portable/MemMang/heap_4.d ./RTOS/Source/portable/MemMang/heap_4.o ./RTOS/Source/portable/MemMang/heap_4.su

.PHONY: clean-RTOS-2f-Source-2f-portable-2f-MemMang

