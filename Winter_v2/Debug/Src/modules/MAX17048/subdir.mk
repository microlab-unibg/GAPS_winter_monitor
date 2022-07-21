################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/modules/MAX17048/MAX17048.c 

OBJS += \
./Src/modules/MAX17048/MAX17048.o 

C_DEPS += \
./Src/modules/MAX17048/MAX17048.d 


# Each subdirectory must supply rules for building sources it contributes
Src/modules/MAX17048/%.o: ../Src/modules/MAX17048/%.c Src/modules/MAX17048/subdir.mk
	arm-none-eabi-gcc "$<" -std=gnu11 -g -c -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src-2f-modules-2f-MAX17048

clean-Src-2f-modules-2f-MAX17048:
	-$(RM) ./Src/modules/MAX17048/MAX17048.d ./Src/modules/MAX17048/MAX17048.o

.PHONY: clean-Src-2f-modules-2f-MAX17048

