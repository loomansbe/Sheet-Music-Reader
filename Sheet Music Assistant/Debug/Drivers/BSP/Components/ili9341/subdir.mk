################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Components/ili9341/ili9341.c 

C_DEPS += \
./Drivers/BSP/Components/ili9341/ili9341.d 

OBJS += \
./Drivers/BSP/Components/ili9341/ili9341.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/ili9341/ili9341.o: ../Drivers/BSP/Components/ili9341/ili9341.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F413xx -DDEBUG -c -I../USB_HOST/App -I../Middlewares/Third_Party/FatFs -I../Drivers/BSP -I../USB_HOST/Target -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/bnloo/Documents/Working Build/Sheet Music Assistant/Drivers/BSP/STM32F413H-Discovery" -I"C:/Users/bnloo/Documents/Working Build/Sheet Music Assistant/Middlewares/Third_Party/FatFs/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/Components/ili9341/ili9341.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

