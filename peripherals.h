#ifndef PERIPH_H
#define PERIPH_H

#include<drivers/CoreUARTapb/core_uart_apb.h>
#include<drivers/CoreI2C/core_i2c.h>
#include<drivers/mss_rtc/mss_rtc.h>
#include<drivers/mss_spi/mss_spi.h>
#include<drivers/mss_timer/mss_timer.h>
#include <AHAN_hw_platform.h>
#include <drivers/mss_gpio/mss_gpio.h>
#include <drivers/mss_i2c/mss_i2c.h>
#include <drivers/mss_uart/mss_uart.h>
//UART
//UART instances
 UART_instance_t uart0;
 UART_instance_t uart1;
 UART_instance_t uart2;
 UART_instance_t uart3;
 UART_instance_t uart4;

#define COREUARTAPB_4_0                 0x5000B000U
#define UART_BAUD_9600                  (((MSS_SYS_M3_CLK_FREQ)/(9600 * 16))  - 1)
#define UART_BAUD_115200                26
#define MSS_UART_BAUD_2000000				2000000

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

 //RTC and timer
#define TIMER_COUNT_PER_MS 50000
#define FULL_SCALE_TIME_MS 85899
 //mss_rtc_calendar_t now;
#endif
