################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Game/graphics.c \
../Core/Game/tetris.c 

OBJS += \
./Core/Game/graphics.o \
./Core/Game/tetris.o 

C_DEPS += \
./Core/Game/graphics.d \
./Core/Game/tetris.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Game/%.o Core/Game/%.su Core/Game/%.cyclo: ../Core/Game/%.c Core/Game/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32F103xB -c -I../Core/Inc -I../Core/Drivers -I../Core/Game -I../Core/Inc/clock -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Game

clean-Core-2f-Game:
	-$(RM) ./Core/Game/graphics.cyclo ./Core/Game/graphics.d ./Core/Game/graphics.o ./Core/Game/graphics.su ./Core/Game/tetris.cyclo ./Core/Game/tetris.d ./Core/Game/tetris.o ./Core/Game/tetris.su

.PHONY: clean-Core-2f-Game

