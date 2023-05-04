################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Micrium/Software/uC-CPU/ARM-Cortex-M/ARMv7-M/GNU/cpu_a.s 

OBJS += \
./Micrium/Software/uC-CPU/ARM-Cortex-M/ARMv7-M/GNU/cpu_a.o 

S_DEPS += \
./Micrium/Software/uC-CPU/ARM-Cortex-M/ARMv7-M/GNU/cpu_a.d 


# Each subdirectory must supply rules for building sources it contributes
Micrium/Software/uC-CPU/ARM-Cortex-M/ARMv7-M/GNU/%.o: ../Micrium/Software/uC-CPU/ARM-Cortex-M/ARMv7-M/GNU/%.s Micrium/Software/uC-CPU/ARM-Cortex-M/ARMv7-M/GNU/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m3 -g3 -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@" "$<"

