################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Core/can/arm.c 

OBJS += \
./Middlewares/Core/can/arm.o 

C_DEPS += \
./Middlewares/Core/can/arm.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Core/can/%.o Middlewares/Core/can/%.su Middlewares/Core/can/%.cyclo: ../Middlewares/Core/can/%.c Middlewares/Core/can/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/BSP/STM32L4xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Core-2f-can

clean-Middlewares-2f-Core-2f-can:
	-$(RM) ./Middlewares/Core/can/arm.cyclo ./Middlewares/Core/can/arm.d ./Middlewares/Core/can/arm.o ./Middlewares/Core/can/arm.su

.PHONY: clean-Middlewares-2f-Core-2f-can

