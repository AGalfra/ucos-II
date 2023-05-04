################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Micrium/Software/uCOS-II/Source/os_core.c \
../Micrium/Software/uCOS-II/Source/os_flag.c \
../Micrium/Software/uCOS-II/Source/os_mbox.c \
../Micrium/Software/uCOS-II/Source/os_mem.c \
../Micrium/Software/uCOS-II/Source/os_mutex.c \
../Micrium/Software/uCOS-II/Source/os_q.c \
../Micrium/Software/uCOS-II/Source/os_sem.c \
../Micrium/Software/uCOS-II/Source/os_task.c \
../Micrium/Software/uCOS-II/Source/os_time.c \
../Micrium/Software/uCOS-II/Source/os_tmr.c 

OBJS += \
./Micrium/Software/uCOS-II/Source/os_core.o \
./Micrium/Software/uCOS-II/Source/os_flag.o \
./Micrium/Software/uCOS-II/Source/os_mbox.o \
./Micrium/Software/uCOS-II/Source/os_mem.o \
./Micrium/Software/uCOS-II/Source/os_mutex.o \
./Micrium/Software/uCOS-II/Source/os_q.o \
./Micrium/Software/uCOS-II/Source/os_sem.o \
./Micrium/Software/uCOS-II/Source/os_task.o \
./Micrium/Software/uCOS-II/Source/os_time.o \
./Micrium/Software/uCOS-II/Source/os_tmr.o 

C_DEPS += \
./Micrium/Software/uCOS-II/Source/os_core.d \
./Micrium/Software/uCOS-II/Source/os_flag.d \
./Micrium/Software/uCOS-II/Source/os_mbox.d \
./Micrium/Software/uCOS-II/Source/os_mem.d \
./Micrium/Software/uCOS-II/Source/os_mutex.d \
./Micrium/Software/uCOS-II/Source/os_q.d \
./Micrium/Software/uCOS-II/Source/os_sem.d \
./Micrium/Software/uCOS-II/Source/os_task.d \
./Micrium/Software/uCOS-II/Source/os_time.d \
./Micrium/Software/uCOS-II/Source/os_tmr.d 


# Each subdirectory must supply rules for building sources it contributes
Micrium/Software/uCOS-II/Source/%.o: ../Micrium/Software/uCOS-II/Source/%.c Micrium/Software/uCOS-II/Source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Micrium/Template/OS2 -I../Micrium/Software/uC-CPU/ARM-Cortex-M/ARMv7-M/GNU -I../Micrium/Template -I../Micrium/Software/uCOS-II/Source -I../Micrium/Software/uC-LIB -I../Micrium/Software/uC-CPU -I../Micrium/Software/uCOS-II/Ports/ARM-Cortex-M/ARMv7-M/GNU -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"

