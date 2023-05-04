################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Micrium/Software/uCOS-II/Ports/ARM-Cortex-M/ARMv7-M/GNU/os_dbg.c 

S_UPPER_SRCS += \
../Micrium/Software/uCOS-II/Ports/ARM-Cortex-M/ARMv7-M/GNU/os_cpu_a.S 

OBJS += \
./Micrium/Software/uCOS-II/Ports/ARM-Cortex-M/ARMv7-M/GNU/os_cpu_a.o \
./Micrium/Software/uCOS-II/Ports/ARM-Cortex-M/ARMv7-M/GNU/os_dbg.o 

S_UPPER_DEPS += \
./Micrium/Software/uCOS-II/Ports/ARM-Cortex-M/ARMv7-M/GNU/os_cpu_a.d 

C_DEPS += \
./Micrium/Software/uCOS-II/Ports/ARM-Cortex-M/ARMv7-M/GNU/os_dbg.d 


# Each subdirectory must supply rules for building sources it contributes
Micrium/Software/uCOS-II/Ports/ARM-Cortex-M/ARMv7-M/GNU/%.o: ../Micrium/Software/uCOS-II/Ports/ARM-Cortex-M/ARMv7-M/GNU/%.S Micrium/Software/uCOS-II/Ports/ARM-Cortex-M/ARMv7-M/GNU/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m3 -g3 -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@" "$<"
Micrium/Software/uCOS-II/Ports/ARM-Cortex-M/ARMv7-M/GNU/%.o: ../Micrium/Software/uCOS-II/Ports/ARM-Cortex-M/ARMv7-M/GNU/%.c Micrium/Software/uCOS-II/Ports/ARM-Cortex-M/ARMv7-M/GNU/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Micrium/Template/OS2 -I../Micrium/Software/uC-CPU/ARM-Cortex-M/ARMv7-M/GNU -I../Micrium/Template -I../Micrium/Software/uCOS-II/Source -I../Micrium/Software/uC-LIB -I../Micrium/Software/uC-CPU -I../Micrium/Software/uCOS-II/Ports/ARM-Cortex-M/ARMv7-M/GNU -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"

