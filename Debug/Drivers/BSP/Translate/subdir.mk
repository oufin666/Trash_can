################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Translate/Motor.c \
../Drivers/BSP/Translate/PID.c 

OBJS += \
./Drivers/BSP/Translate/Motor.o \
./Drivers/BSP/Translate/PID.o 

C_DEPS += \
./Drivers/BSP/Translate/Motor.d \
./Drivers/BSP/Translate/PID.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Translate/%.o Drivers/BSP/Translate/%.su Drivers/BSP/Translate/%.cyclo: ../Drivers/BSP/Translate/%.c Drivers/BSP/Translate/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Translate

clean-Drivers-2f-BSP-2f-Translate:
	-$(RM) ./Drivers/BSP/Translate/Motor.cyclo ./Drivers/BSP/Translate/Motor.d ./Drivers/BSP/Translate/Motor.o ./Drivers/BSP/Translate/Motor.su ./Drivers/BSP/Translate/PID.cyclo ./Drivers/BSP/Translate/PID.d ./Drivers/BSP/Translate/PID.o ./Drivers/BSP/Translate/PID.su

.PHONY: clean-Drivers-2f-BSP-2f-Translate

