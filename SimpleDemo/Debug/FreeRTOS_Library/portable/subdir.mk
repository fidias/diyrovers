################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS_Library/portable/heap_4.c \
../FreeRTOS_Library/portable/port.c 

OBJS += \
./FreeRTOS_Library/portable/heap_4.o \
./FreeRTOS_Library/portable/port.o 

C_DEPS += \
./FreeRTOS_Library/portable/heap_4.d \
./FreeRTOS_Library/portable/port.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS_Library/portable/%.o: ../FreeRTOS_Library/portable/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -I"/home/mes/Projects/diyrovers/SimpleDemo/FreeRTOS_Library/include" -I"/home/mes/Projects/diyrovers/SimpleDemo/FreeRTOS_Library/portable" -I"/home/mes/Projects/diyrovers/SimpleDemo/CMSISv1p30_LPC17xx/inc" -I"/home/mes/Projects/diyrovers/SimpleDemo" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


