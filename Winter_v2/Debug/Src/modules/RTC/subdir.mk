################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/modules/RTC/rtc.c 

OBJS += \
./Src/modules/RTC/rtc.o 

C_DEPS += \
./Src/modules/RTC/rtc.d 


# Each subdirectory must supply rules for building sources it contributes
Src/modules/RTC/%.o: ../Src/modules/RTC/%.c Src/modules/RTC/subdir.mk
	arm-none-eabi-gcc "$<" -std=gnu11 -g -c -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src-2f-modules-2f-RTC

clean-Src-2f-modules-2f-RTC:
	-$(RM) ./Src/modules/RTC/rtc.d ./Src/modules/RTC/rtc.o

.PHONY: clean-Src-2f-modules-2f-RTC

