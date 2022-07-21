################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/modules/io/io.c 

OBJS += \
./Src/modules/io/io.o 

C_DEPS += \
./Src/modules/io/io.d 


# Each subdirectory must supply rules for building sources it contributes
Src/modules/io/%.o: ../Src/modules/io/%.c Src/modules/io/subdir.mk
	arm-none-eabi-gcc "$<" -std=gnu11 -g -c -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src-2f-modules-2f-io

clean-Src-2f-modules-2f-io:
	-$(RM) ./Src/modules/io/io.d ./Src/modules/io/io.o

.PHONY: clean-Src-2f-modules-2f-io

