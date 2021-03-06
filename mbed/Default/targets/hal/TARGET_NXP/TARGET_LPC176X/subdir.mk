################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../targets/hal/TARGET_NXP/TARGET_LPC176X/analogin_api.c \
../targets/hal/TARGET_NXP/TARGET_LPC176X/analogout_api.c \
../targets/hal/TARGET_NXP/TARGET_LPC176X/can_api.c \
../targets/hal/TARGET_NXP/TARGET_LPC176X/ethernet_api.c \
../targets/hal/TARGET_NXP/TARGET_LPC176X/gpio_api.c \
../targets/hal/TARGET_NXP/TARGET_LPC176X/gpio_irq_api.c \
../targets/hal/TARGET_NXP/TARGET_LPC176X/i2c_api.c \
../targets/hal/TARGET_NXP/TARGET_LPC176X/pinmap.c \
../targets/hal/TARGET_NXP/TARGET_LPC176X/port_api.c \
../targets/hal/TARGET_NXP/TARGET_LPC176X/pwmout_api.c \
../targets/hal/TARGET_NXP/TARGET_LPC176X/rtc_api.c \
../targets/hal/TARGET_NXP/TARGET_LPC176X/serial_api.c \
../targets/hal/TARGET_NXP/TARGET_LPC176X/sleep.c \
../targets/hal/TARGET_NXP/TARGET_LPC176X/spi_api.c \
../targets/hal/TARGET_NXP/TARGET_LPC176X/us_ticker.c 

C_DEPS += \
./targets/hal/TARGET_NXP/TARGET_LPC176X/analogin_api.d \
./targets/hal/TARGET_NXP/TARGET_LPC176X/analogout_api.d \
./targets/hal/TARGET_NXP/TARGET_LPC176X/can_api.d \
./targets/hal/TARGET_NXP/TARGET_LPC176X/ethernet_api.d \
./targets/hal/TARGET_NXP/TARGET_LPC176X/gpio_api.d \
./targets/hal/TARGET_NXP/TARGET_LPC176X/gpio_irq_api.d \
./targets/hal/TARGET_NXP/TARGET_LPC176X/i2c_api.d \
./targets/hal/TARGET_NXP/TARGET_LPC176X/pinmap.d \
./targets/hal/TARGET_NXP/TARGET_LPC176X/port_api.d \
./targets/hal/TARGET_NXP/TARGET_LPC176X/pwmout_api.d \
./targets/hal/TARGET_NXP/TARGET_LPC176X/rtc_api.d \
./targets/hal/TARGET_NXP/TARGET_LPC176X/serial_api.d \
./targets/hal/TARGET_NXP/TARGET_LPC176X/sleep.d \
./targets/hal/TARGET_NXP/TARGET_LPC176X/spi_api.d \
./targets/hal/TARGET_NXP/TARGET_LPC176X/us_ticker.d 


# Each subdirectory must supply rules for building sources it contributes
targets/hal/TARGET_NXP/TARGET_LPC176X/%.o: ../targets/hal/TARGET_NXP/TARGET_LPC176X/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Linux GCC C Compiler'
	arm-none-eabi-gcc -O2 -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m4 -mthumb -g -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


