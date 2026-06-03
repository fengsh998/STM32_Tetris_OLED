################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/clock/osc_clock.c \
../Core/Src/clock/systick.c 

OBJS += \
./Core/Src/clock/osc_clock.o \
./Core/Src/clock/systick.o 

C_DEPS += \
./Core/Src/clock/osc_clock.d \
./Core/Src/clock/systick.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/clock/%.o Core/Src/clock/%.su Core/Src/clock/%.cyclo: ../Core/Src/clock/%.c Core/Src/clock/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32F103xB -c -I../Core/Inc -I../Core/Drivers -I../Core/Game -I../Core/Inc/clock -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-clock

clean-Core-2f-Src-2f-clock:
	-$(RM) ./Core/Src/clock/osc_clock.cyclo ./Core/Src/clock/osc_clock.d ./Core/Src/clock/osc_clock.o ./Core/Src/clock/osc_clock.su ./Core/Src/clock/systick.cyclo ./Core/Src/clock/systick.d ./Core/Src/clock/systick.o ./Core/Src/clock/systick.su

.PHONY: clean-Core-2f-Src-2f-clock

