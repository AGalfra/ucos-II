################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/COM.c \
../Core/Src/DIO.c \
../Core/Src/fonts.c \
../Core/Src/ftostr.c \
../Core/Src/gui.c \
../Core/Src/main.c \
../Core/Src/onewire.c \
../Core/Src/sensor.c \
../Core/Src/ssd1306.c \
../Core/Src/stm32f1xx_hal_msp.c \
../Core/Src/stm32f1xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f1xx.c \
../Core/Src/tareas.c \
../Core/Src/test.c 

OBJS += \
./Core/Src/COM.o \
./Core/Src/DIO.o \
./Core/Src/fonts.o \
./Core/Src/ftostr.o \
./Core/Src/gui.o \
./Core/Src/main.o \
./Core/Src/onewire.o \
./Core/Src/sensor.o \
./Core/Src/ssd1306.o \
./Core/Src/stm32f1xx_hal_msp.o \
./Core/Src/stm32f1xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f1xx.o \
./Core/Src/tareas.o \
./Core/Src/test.o 

C_DEPS += \
./Core/Src/COM.d \
./Core/Src/DIO.d \
./Core/Src/fonts.d \
./Core/Src/ftostr.d \
./Core/Src/gui.d \
./Core/Src/main.d \
./Core/Src/onewire.d \
./Core/Src/sensor.d \
./Core/Src/ssd1306.d \
./Core/Src/stm32f1xx_hal_msp.d \
./Core/Src/stm32f1xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f1xx.d \
./Core/Src/tareas.d \
./Core/Src/test.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Micrium/Template/OS2 -I../Micrium/Software/uC-CPU/ARM-Cortex-M/ARMv7-M/GNU -I../Micrium/Template -I../Micrium/Software/uCOS-II/Source -I../Micrium/Software/uC-LIB -I../Micrium/Software/uC-CPU -I../Micrium/Software/uCOS-II/Ports/ARM-Cortex-M/ARMv7-M/GNU -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"

