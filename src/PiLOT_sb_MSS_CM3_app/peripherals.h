/**
 * @file    :   peripherals.h
 * @author  :   Srinidhi G
 * @brief   :   This file contains all the peripherals used be the OBC
 * @version :   1.0
 * @date    :   2023-01-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef PERIPH_H
#define PERIPH_H

#include<CMSIS/hw_reg_io.h>
#include<drivers/CoreUARTapb/core_uart_apb.h>
#include<drivers/CoreI2C/core_i2c.h>
#include<drivers/mss_rtc/mss_rtc.h>
#include<drivers/mss_spi/mss_spi.h>
#include<drivers/mss_timer/mss_timer.h>
#include <PiLOT_hw_platform.h>
#include <drivers/mss_gpio/mss_gpio.h>
#include <drivers/mss_i2c/mss_i2c.h>
#include <drivers/mss_uart/mss_uart.h>
#include <drivers_config/sys_config/sys_config_mss_clocks.h>
#include <drivers/CoreTimer/core_timer.h>
#include <drivers/mss_watchdog/mss_watchdog.h>
#define WDOG_SYSREG_CR_ENABLE_MASK      0x00000001u
//Logic Levels
#define LOGIC_HIGH					1
#define LOGIC_LOW					0
//UART
//defining PE bit of LSR register for MSS_UART
#define PE							2u
#define TEMT						6u
//UART instances
 UART_instance_t uart0;
 UART_instance_t uart1;
 UART_instance_t uart2;
 UART_instance_t uart3;
 UART_instance_t uart4;

#define UART_BAUD_9600                  (((MSS_SYS_M3_CLK_FREQ)/(9600 * 16))  - 1)
#define UART_BAUD_115200                (((MSS_SYS_M3_CLK_FREQ)/(115200 * 16))  - 1)
#define MSS_UART_BAUD_2000000			2000000

#define LOWER_BYTE			0
#define UPPER_BYTE			1

//I2C instances

#define DUMMY_I2C_ADDR 0x88
i2c_instance_t g_core_i2c0;
i2c_instance_t g_core_i2c1;
i2c_instance_t g_core_i2c2;
i2c_instance_t g_core_i2c3;
i2c_instance_t g_core_i2c4;
i2c_instance_t g_core_i2c5;

i2c_status_t core_i2c_status;

//i2c_2 is the pin on the OBC
#define i2c_3 g_core_i2c1
#define i2c_5 g_core_i2c3

#define I2C_SLAVE_ADDRESS 0x12

//GPIO varialbes
uint8_t gpio1_irq_cnt;
uint8_t gpio2_irq_cnt;
uint8_t gpio3_irq_cnt;
uint8_t gpio4_irq_cnt;

//GPIO MACROS
#define EN_UART					    MSS_GPIO_13
#define EN_COMMS					MSS_GPIO_14
#define RESET_GPIO					MSS_GPIO_17
#define SD_CARD_GPIO				MSS_GPIO_10
#define TX_INV_EN					MSS_GPIO_8
#define RX_INV_EN					MSS_GPIO_7

#define TIMER_COUNT_PER_MS 100000
#define FULL_SCALE_TIME_MS 42949
#endif
