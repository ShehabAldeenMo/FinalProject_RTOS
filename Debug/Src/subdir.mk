################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/DIO_Prg.c \
../Src/Det.c \
../Src/Dio_Cfg.c \
../Src/LCD_Cfg.c \
../Src/LCD_Prg.c \
../Src/MCU_Cfg.c \
../Src/MCU_Prg.c \
../Src/Port_Cfg.c \
../Src/Port_Prg.c \
../Src/UART_Program.c \
../Src/UART_cfg.c \
../Src/main.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/DIO_Prg.o \
./Src/Det.o \
./Src/Dio_Cfg.o \
./Src/LCD_Cfg.o \
./Src/LCD_Prg.o \
./Src/MCU_Cfg.o \
./Src/MCU_Prg.o \
./Src/Port_Cfg.o \
./Src/Port_Prg.o \
./Src/UART_Program.o \
./Src/UART_cfg.o \
./Src/main.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/DIO_Prg.d \
./Src/Det.d \
./Src/Dio_Cfg.d \
./Src/LCD_Cfg.d \
./Src/LCD_Prg.d \
./Src/MCU_Cfg.d \
./Src/MCU_Prg.d \
./Src/Port_Cfg.d \
./Src/Port_Prg.d \
./Src/UART_Program.d \
./Src/UART_cfg.d \
./Src/main.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C8Tx -c -I../Inc -I"D:/Embedded Track/Embedded NGEN/03-STM Workspace/FinalProject_RTOS/RTOS/Source/portable/GCC/ARM_CM3" -I"D:/Embedded Track/Embedded NGEN/03-STM Workspace/FinalProject_RTOS/RTOS/Source" -I"D:/Embedded Track/Embedded NGEN/03-STM Workspace/FinalProject_RTOS/RTOS/Source/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/DIO_Prg.cyclo ./Src/DIO_Prg.d ./Src/DIO_Prg.o ./Src/DIO_Prg.su ./Src/Det.cyclo ./Src/Det.d ./Src/Det.o ./Src/Det.su ./Src/Dio_Cfg.cyclo ./Src/Dio_Cfg.d ./Src/Dio_Cfg.o ./Src/Dio_Cfg.su ./Src/LCD_Cfg.cyclo ./Src/LCD_Cfg.d ./Src/LCD_Cfg.o ./Src/LCD_Cfg.su ./Src/LCD_Prg.cyclo ./Src/LCD_Prg.d ./Src/LCD_Prg.o ./Src/LCD_Prg.su ./Src/MCU_Cfg.cyclo ./Src/MCU_Cfg.d ./Src/MCU_Cfg.o ./Src/MCU_Cfg.su ./Src/MCU_Prg.cyclo ./Src/MCU_Prg.d ./Src/MCU_Prg.o ./Src/MCU_Prg.su ./Src/Port_Cfg.cyclo ./Src/Port_Cfg.d ./Src/Port_Cfg.o ./Src/Port_Cfg.su ./Src/Port_Prg.cyclo ./Src/Port_Prg.d ./Src/Port_Prg.o ./Src/Port_Prg.su ./Src/UART_Program.cyclo ./Src/UART_Program.d ./Src/UART_Program.o ./Src/UART_Program.su ./Src/UART_cfg.cyclo ./Src/UART_cfg.d ./Src/UART_cfg.o ./Src/UART_cfg.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src

