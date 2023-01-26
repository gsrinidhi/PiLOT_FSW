################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/mss_spi/mss_spi.c 

OBJS += \
./drivers/mss_spi/mss_spi.o 

C_DEPS += \
./drivers/mss_spi/mss_spi.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/mss_spi/%.o: ../drivers/mss_spi/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU C Compiler'
	arm-none-eabi-gcc -mthumb -mcpu=cortex-m3 -DNDEBUG -IC:\D_Drive\Srinidhi\BTech\SSpace\CDH\PiLOT\Flight_Software\src\PiLOT_sb_MSS_CM3_hw_platform -IC:\D_Drive\Srinidhi\BTech\SSpace\CDH\PiLOT\Flight_Software\src\PiLOT_sb_MSS_CM3_hw_platform\CMSIS -IC:\D_Drive\Srinidhi\BTech\SSpace\CDH\PiLOT\Flight_Software\src\PiLOT_sb_MSS_CM3_hw_platform\CMSIS\startup_gcc -IC:\D_Drive\Srinidhi\BTech\SSpace\CDH\PiLOT\Flight_Software\src\PiLOT_sb_MSS_CM3_hw_platform\drivers -IC:\D_Drive\Srinidhi\BTech\SSpace\CDH\PiLOT\Flight_Software\src\PiLOT_sb_MSS_CM3_hw_platform\drivers\CoreGPIO -IC:\D_Drive\Srinidhi\BTech\SSpace\CDH\PiLOT\Flight_Software\src\PiLOT_sb_MSS_CM3_hw_platform\drivers\CoreI2C -IC:\D_Drive\Srinidhi\BTech\SSpace\CDH\PiLOT\Flight_Software\src\PiLOT_sb_MSS_CM3_hw_platform\drivers\CoreUARTapb -IC:\D_Drive\Srinidhi\BTech\SSpace\CDH\PiLOT\Flight_Software\src\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_gpio -IC:\D_Drive\Srinidhi\BTech\SSpace\CDH\PiLOT\Flight_Software\src\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_hpdma -IC:\D_Drive\Srinidhi\BTech\SSpace\CDH\PiLOT\Flight_Software\src\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_i2c -IC:\D_Drive\Srinidhi\BTech\SSpace\CDH\PiLOT\Flight_Software\src\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_nvm -IC:\D_Drive\Srinidhi\BTech\SSpace\CDH\PiLOT\Flight_Software\src\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_spi -IC:\D_Drive\Srinidhi\BTech\SSpace\CDH\PiLOT\Flight_Software\src\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_sys_services -IC:\D_Drive\Srinidhi\BTech\SSpace\CDH\PiLOT\Flight_Software\src\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_timer -IC:\D_Drive\Srinidhi\BTech\SSpace\CDH\PiLOT\Flight_Software\src\PiLOT_sb_MSS_CM3_hw_platform\drivers\mss_uart -IC:\D_Drive\Srinidhi\BTech\SSpace\CDH\PiLOT\Flight_Software\src\PiLOT_sb_MSS_CM3_hw_platform\drivers_config -IC:\D_Drive\Srinidhi\BTech\SSpace\CDH\PiLOT\Flight_Software\src\PiLOT_sb_MSS_CM3_hw_platform\drivers_config\sys_config -IC:\D_Drive\Srinidhi\BTech\SSpace\CDH\PiLOT\Flight_Software\src\PiLOT_sb_MSS_CM3_hw_platform\hal -IC:\D_Drive\Srinidhi\BTech\SSpace\CDH\PiLOT\Flight_Software\src\PiLOT_sb_MSS_CM3_hw_platform\hal\CortexM3 -IC:\D_Drive\Srinidhi\BTech\SSpace\CDH\PiLOT\Flight_Software\src\PiLOT_sb_MSS_CM3_hw_platform\hal\CortexM3\GNU -O0 -ffunction-sections -fdata-sections -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


