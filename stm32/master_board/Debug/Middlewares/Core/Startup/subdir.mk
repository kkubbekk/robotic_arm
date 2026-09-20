################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Middlewares/Core/Startup/startup_stm32l476rgtx.s 

OBJS += \
./Middlewares/Core/Startup/startup_stm32l476rgtx.o 

S_DEPS += \
./Middlewares/Core/Startup/startup_stm32l476rgtx.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Core/Startup/%.o: ../Middlewares/Core/Startup/%.s Middlewares/Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/BSP/STM32L4xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Middlewares-2f-Core-2f-Startup

clean-Middlewares-2f-Core-2f-Startup:
	-$(RM) ./Middlewares/Core/Startup/startup_stm32l476rgtx.d ./Middlewares/Core/Startup/startup_stm32l476rgtx.o

.PHONY: clean-Middlewares-2f-Core-2f-Startup

