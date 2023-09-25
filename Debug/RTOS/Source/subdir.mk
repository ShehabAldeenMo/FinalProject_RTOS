################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../RTOS/Source/croutine.c \
../RTOS/Source/event_groups.c \
../RTOS/Source/list.c \
../RTOS/Source/queue.c \
../RTOS/Source/stream_buffer.c \
../RTOS/Source/tasks.c \
../RTOS/Source/timers.c 

OBJS += \
./RTOS/Source/croutine.o \
./RTOS/Source/event_groups.o \
./RTOS/Source/list.o \
./RTOS/Source/queue.o \
./RTOS/Source/stream_buffer.o \
./RTOS/Source/tasks.o \
./RTOS/Source/timers.o 

C_DEPS += \
./RTOS/Source/croutine.d \
./RTOS/Source/event_groups.d \
./RTOS/Source/list.d \
./RTOS/Source/queue.d \
./RTOS/Source/stream_buffer.d \
./RTOS/Source/tasks.d \
./RTOS/Source/timers.d 


# Each subdirectory must supply rules for building sources it contributes
RTOS/Source/%.o RTOS/Source/%.su RTOS/Source/%.cyclo: ../RTOS/Source/%.c RTOS/Source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I../Inc -I"D:/Embedded Track/Embedded NGEN/03-STM Workspace/FinalProject_RTOS/RTOS/Source/portable/GCC/ARM_CM3" -I"D:/Embedded Track/Embedded NGEN/03-STM Workspace/FinalProject_RTOS/RTOS/Source" -I"D:/Embedded Track/Embedded NGEN/03-STM Workspace/FinalProject_RTOS/RTOS/Source/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-RTOS-2f-Source

clean-RTOS-2f-Source:
	-$(RM) ./RTOS/Source/croutine.cyclo ./RTOS/Source/croutine.d ./RTOS/Source/croutine.o ./RTOS/Source/croutine.su ./RTOS/Source/event_groups.cyclo ./RTOS/Source/event_groups.d ./RTOS/Source/event_groups.o ./RTOS/Source/event_groups.su ./RTOS/Source/list.cyclo ./RTOS/Source/list.d ./RTOS/Source/list.o ./RTOS/Source/list.su ./RTOS/Source/queue.cyclo ./RTOS/Source/queue.d ./RTOS/Source/queue.o ./RTOS/Source/queue.su ./RTOS/Source/stream_buffer.cyclo ./RTOS/Source/stream_buffer.d ./RTOS/Source/stream_buffer.o ./RTOS/Source/stream_buffer.su ./RTOS/Source/tasks.cyclo ./RTOS/Source/tasks.d ./RTOS/Source/tasks.o ./RTOS/Source/tasks.su ./RTOS/Source/timers.cyclo ./RTOS/Source/timers.d ./RTOS/Source/timers.o ./RTOS/Source/timers.su

.PHONY: clean-RTOS-2f-Source

