################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/modules/GattDB/comm.c \
../Src/modules/GattDB/gatt_db.c 

OBJS += \
./Src/modules/GattDB/comm.o \
./Src/modules/GattDB/gatt_db.o 

C_DEPS += \
./Src/modules/GattDB/comm.d \
./Src/modules/GattDB/gatt_db.d 


# Each subdirectory must supply rules for building sources it contributes
Src/modules/GattDB/%.o: ../Src/modules/GattDB/%.c Src/modules/GattDB/subdir.mk
	arm-none-eabi-gcc "$<" -std=gnu11 -g -c -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src-2f-modules-2f-GattDB

clean-Src-2f-modules-2f-GattDB:
	-$(RM) ./Src/modules/GattDB/comm.d ./Src/modules/GattDB/comm.o ./Src/modules/GattDB/gatt_db.d ./Src/modules/GattDB/gatt_db.o

.PHONY: clean-Src-2f-modules-2f-GattDB

