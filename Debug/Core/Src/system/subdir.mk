################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/system/syscalls.c \
../Core/Src/system/sysmem.c \
../Core/Src/system/system_stm32f1xx.c 

OBJS += \
./Core/Src/system/syscalls.o \
./Core/Src/system/sysmem.o \
./Core/Src/system/system_stm32f1xx.o 

C_DEPS += \
./Core/Src/system/syscalls.d \
./Core/Src/system/sysmem.d \
./Core/Src/system/system_stm32f1xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/system/%.o Core/Src/system/%.su Core/Src/system/%.cyclo: ../Core/Src/system/%.c Core/Src/system/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32F103xB -c -I../Core/Inc -I../Core/Drivers -I../Core/Game -I../Core/Inc/clock -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-system

clean-Core-2f-Src-2f-system:
	-$(RM) ./Core/Src/system/syscalls.cyclo ./Core/Src/system/syscalls.d ./Core/Src/system/syscalls.o ./Core/Src/system/syscalls.su ./Core/Src/system/sysmem.cyclo ./Core/Src/system/sysmem.d ./Core/Src/system/sysmem.o ./Core/Src/system/sysmem.su ./Core/Src/system/system_stm32f1xx.cyclo ./Core/Src/system/system_stm32f1xx.d ./Core/Src/system/system_stm32f1xx.o ./Core/Src/system/system_stm32f1xx.su

.PHONY: clean-Core-2f-Src-2f-system

