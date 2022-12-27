/*
 * AHAN.h
 *
 *  Created on: 25-Jun-2022
 *      Author: S-SPACE
 */

#ifndef AHAN_H_
#define AHAN_H_
#include<drivers/mss_gpio/mss_gpio.h>
#include<drivers/mss_timer/mss_timer.h>
#include<drivers/CoreGPIO/core_gpio.h>
#include<drivers/CoreI2C/core_i2c.h>
#include<drivers/mss_spi/mss_spi.h>
#include<drivers/mss_uart/mss_uart.h>
#include<drivers/CoreUARTapb/core_uart_apb.h>
#include<drivers/mss_i2c/mss_i2c.h>
#include<drivers/mss_rtc/mss_rtc.h>
#include<sys_config_mss_clocks.h>
#include<AHAN_hw_platform.h>
#include "peripherals.h"
//#include"memory.h"





uint8_t uart_rx_buff[20],uart_rx_size;
//GPIO
void GPIO_Init();

//ADC MACROS
#define ADC_I2C_ADDR 0x21
#define DATAHIGH_MAX_H 0x0F
#define DATAHIGH_MAX_L 0xFC
#define DATA_LOW_MIN_H 0x00
#define DATA_LOW_MIN_L 0x00

#define CONV_REG 0x00
#define AS_REG 0x01
#define CONFIG_REG 0x02
#define CYCLE_TIMER_REG 0x03
#define DATA_LOW_REG(x) ((x) * 3 + 0x4)
#define DATA_HIGH_REG(x) ((x) * 3 + 0x5)
#define HYST_REG(x) ((x) * 3 + 0x6)
#define ADC_CHX(x) (((x) + 0x8)<<4)


//UART macros

void delay_ms(uint16_t ms);
void delay_flag(uint16_t ms);
void AHAN_Init();
uint8_t ADC_Init();
double get_ADC_value(uint8_t chx);

//SD card functions
//uint8_t SD_Init();
//uint8_t SD_Write(uint32_t addr,uint8_t *buff);
//uint8_t SD_Read(uint32_t addr,uint8_t *buff);
//IMU macros and functions

#define TX_LENGTH 		1u
#define RX_LENGTH		1u
uint8_t IMU_Init();


//Interrupts




//FSW from here
//Memory card 
//P_SECTOR: sector which stores the pointers to all sectors
#define P_SECTOR 1024
//The GMC Sector number is stored in the first two bytes of the 1024 sector
#define GMC_SECT                    0
//The offset in the sector is stored in the second-third bytes of 1024 sector
#define GMC_OFFSET                  2


//GPIO MACROS
#define GMC_EN_PIN GPIO_24
#define EPS_PIN1 GPIO_14
#define EPS_PIN2 GPIO_15
#define EPS_PIN3 GPIO_16
#define EPS_PIN4 GPIO_3

//I2C Macros
#define GMC_ADDR 0x25



//CDH MACROS
#define I2C_INITIALISED                     1
#define GPIO_INITIALISED                    1
#define UART_INITIALISED                    1
#define SPI_INITIALISED                     1



//TBD
#define RTC_PRESCALER 1000

//CDH Structure
typedef struct CDH {
    uint8_t I2C_0;
    uint8_t I2C_1;
    uint8_t I2C_2;
    uint8_t I2C_3;
    uint8_t I2C_4;
    uint8_t I2C_5;
    uint8_t I2C_6;
    uint8_t I2C_7;
    uint8_t UART_0;
    uint8_t UART_1;
    uint8_t UART_2;
    uint8_t UART_3;
    uint8_t UART_4;
    uint8_t SPI_0;
    uint8_t SPI_1;
    uint8_t GPIO_OUT3;
    uint8_t GPIO_OUT4;
    uint8_t GPIO_OUT5;
    uint8_t GPIO_OUT6;
    uint8_t GPIO_OUT7;
    uint8_t GPIO_OUT8;
    uint8_t GPIO_OUT9;
    uint8_t GPIO_IN1;
    uint8_t GPIO_IN2;
    uint8_t GPIO_IN3;
    uint8_t GPIO_IN4;
    uint8_t A_RTC;
    uint8_t SD;
} CDH_t;

//AHAN structure
typedef struct AHAN {
    CDH_t AHAN_CDH;
} AHAN_t;

//AHAN
AHAN_t Ahan;

#define ACDH Ahan.AHAN_CDH

uint8_t test_peripherals();

#define BUFFER_SIZE 10u      

void time_to_count(uint32_t ms,uint32_t *upper_count,uint32_t *lower_count);

#endif /* AHAN_H_ */
