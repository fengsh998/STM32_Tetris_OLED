################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Drivers/gpio.c \
../Core/Drivers/oled.c \
../Core/Drivers/spi.c 

OBJS += \
./Core/Drivers/gpio.o \
./Core/Drivers/oled.o \
./Core/Drivers/spi.o 

C_DEPS += \
./Core/Drivers/gpio.d \
./Core/Drivers/oled.d \
./Core/Drivers/spi.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Drivers/%.o Core/Drivers/%.su Core/Drivers/%.cyclo: ../Core/Drivers/%.c Core/Drivers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32F103xB -c -I../Core/Inc -I../Core/Drivers -I../Core/Game -I../Core/Inc/clock -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Drivers

clean-Core-2f-Drivers:
	-$(RM) ./Core/Drivers/gpio.cyclo ./Core/Drivers/gpio.d ./Core/Drivers/gpio.o ./Core/Drivers/gpio.su ./Core/Drivers/oled.cyclo ./Core/Drivers/oled.d ./Core/Drivers/oled.o ./Core/Drivers/oled.su ./Core/Drivers/spi.cyclo ./Core/Drivers/spi.d ./Core/Drivers/spi.o ./Core/Drivers/spi.su

.PHONY: clean-Core-2f-Drivers

