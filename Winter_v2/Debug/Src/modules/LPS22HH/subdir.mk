################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/modules/LPS22HH/LPS22HH.c 

OBJS += \
./Src/modules/LPS22HH/LPS22HH.o 

C_DEPS += \
./Src/modules/LPS22HH/LPS22HH.d 


# Each subdirectory must supply rules for building sources it contributes
Src/modules/LPS22HH/%.o: ../Src/modules/LPS22HH/%.c Src/modules/LPS22HH/subdir.mk
	arm-none-eabi-gcc "$<" -std=gnu11 -g -c -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src-2f-modules-2f-LPS22HH

clean-Src-2f-modules-2f-LPS22HH:
	-$(RM) ./Src/modules/LPS22HH/LPS22HH.d ./Src/modules/LPS22HH/LPS22HH.o

.PHONY: clean-Src-2f-modules-2f-LPS22HH

