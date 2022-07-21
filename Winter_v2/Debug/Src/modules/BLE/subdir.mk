################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/modules/BLE/ble.c 

OBJS += \
./Src/modules/BLE/ble.o 

C_DEPS += \
./Src/modules/BLE/ble.d 


# Each subdirectory must supply rules for building sources it contributes
Src/modules/BLE/%.o: ../Src/modules/BLE/%.c Src/modules/BLE/subdir.mk
	arm-none-eabi-gcc "$<" -std=gnu11 -g -c -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src-2f-modules-2f-BLE

clean-Src-2f-modules-2f-BLE:
	-$(RM) ./Src/modules/BLE/ble.d ./Src/modules/BLE/ble.o

.PHONY: clean-Src-2f-modules-2f-BLE

