################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/modules/BlueNRG/hci/hci.c \
../Src/modules/BlueNRG/hci/hci_le.c 

OBJS += \
./Src/modules/BlueNRG/hci/hci.o \
./Src/modules/BlueNRG/hci/hci_le.o 

C_DEPS += \
./Src/modules/BlueNRG/hci/hci.d \
./Src/modules/BlueNRG/hci/hci_le.d 


# Each subdirectory must supply rules for building sources it contributes
Src/modules/BlueNRG/hci/%.o: ../Src/modules/BlueNRG/hci/%.c Src/modules/BlueNRG/hci/subdir.mk
	arm-none-eabi-gcc "$<" -std=gnu11 -g -c -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src-2f-modules-2f-BlueNRG-2f-hci

clean-Src-2f-modules-2f-BlueNRG-2f-hci:
	-$(RM) ./Src/modules/BlueNRG/hci/hci.d ./Src/modules/BlueNRG/hci/hci.o ./Src/modules/BlueNRG/hci/hci_le.d ./Src/modules/BlueNRG/hci/hci_le.o

.PHONY: clean-Src-2f-modules-2f-BlueNRG-2f-hci

