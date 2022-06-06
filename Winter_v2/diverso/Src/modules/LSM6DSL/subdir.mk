################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/modules/LSM6DSL/LSM6DSL.c 

OBJS += \
./Src/modules/LSM6DSL/LSM6DSL.o 

C_DEPS += \
./Src/modules/LSM6DSL/LSM6DSL.d 


# Each subdirectory must supply rules for building sources it contributes
Src/modules/LSM6DSL/%.o: ../Src/modules/LSM6DSL/%.c Src/modules/LSM6DSL/subdir.mk
	arm-none-eabi-gcc -c "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DBLUENRG_MS=1 -DSTM32L475xx -c -I../Inc -I../Src/modules/BlueNRG/includes -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src-2f-modules-2f-LSM6DSL

clean-Src-2f-modules-2f-LSM6DSL:
	-$(RM) ./Src/modules/LSM6DSL/LSM6DSL.d ./Src/modules/LSM6DSL/LSM6DSL.o

.PHONY: clean-Src-2f-modules-2f-LSM6DSL

