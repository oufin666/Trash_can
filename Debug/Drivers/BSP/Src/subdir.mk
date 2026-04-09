################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Src/motor.c \
../Drivers/BSP/Src/pid.c 

OBJS += \
./Drivers/BSP/Src/motor.o \
./Drivers/BSP/Src/pid.o 

C_DEPS += \
./Drivers/BSP/Src/motor.d \
./Drivers/BSP/Src/pid.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Src/%.o Drivers/BSP/Src/%.su Drivers/BSP/Src/%.cyclo: ../Drivers/BSP/Src/%.c Drivers/BSP/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/BSP/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Src

clean-Drivers-2f-BSP-2f-Src:
	-$(RM) ./Drivers/BSP/Src/motor.cyclo ./Drivers/BSP/Src/motor.d ./Drivers/BSP/Src/motor.o ./Drivers/BSP/Src/motor.su ./Drivers/BSP/Src/pid.cyclo ./Drivers/BSP/Src/pid.d ./Drivers/BSP/Src/pid.o ./Drivers/BSP/Src/pid.su

.PHONY: clean-Drivers-2f-BSP-2f-Src

