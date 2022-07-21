################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/modules/BlueNRG/utils/gp_timer.c \
../Src/modules/BlueNRG/utils/list.c \
../Src/modules/BlueNRG/utils/osal.c 

OBJS += \
./Src/modules/BlueNRG/utils/gp_timer.o \
./Src/modules/BlueNRG/utils/list.o \
./Src/modules/BlueNRG/utils/osal.o 

C_DEPS += \
./Src/modules/BlueNRG/utils/gp_timer.d \
./Src/modules/BlueNRG/utils/list.d \
./Src/modules/BlueNRG/utils/osal.d 


# Each subdirectory must supply rules for building sources it contributes
Src/modules/BlueNRG/utils/%.o: ../Src/modules/BlueNRG/utils/%.c Src/modules/BlueNRG/utils/subdir.mk
	arm-none-eabi-gcc "$<" -std=gnu11 -g -c -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src-2f-modules-2f-BlueNRG-2f-utils

clean-Src-2f-modules-2f-BlueNRG-2f-utils:
	-$(RM) ./Src/modules/BlueNRG/utils/gp_timer.d ./Src/modules/BlueNRG/utils/gp_timer.o ./Src/modules/BlueNRG/utils/list.d ./Src/modules/BlueNRG/utils/list.o ./Src/modules/BlueNRG/utils/osal.d ./Src/modules/BlueNRG/utils/osal.o

.PHONY: clean-Src-2f-modules-2f-BlueNRG-2f-utils

