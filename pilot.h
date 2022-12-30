#ifndef _PILOT_H_
#define _PILOT_H_
#include"peripherals.h"
#include "SD.h"
#include "packet_definitions.h"

//CCSDS Sub-Packet
//struct CCSDS
//    {
//       uint8_t Version_ID;
//        uint8_t APID;
//        uint16_t Seq_no;
//        uint16_t PL;
//    };
////Fletcher Code
//struct Flet_Code
//    {
//        uint16_t Fletcher_Code;
//    };
//
////For a single thermistor which contains 8 parameters.
//struct thermistor{
//
//    uint8_t E[8];
//
//};

//typedef struct {
//    //CCSDS
//
//    uint8_t Version_ID;
//    uint8_t APID;
//    uint16_t Seq_no;
//    uint16_t PL;
//
//    uint16_t thermistor_A[8];
//    uint16_t thermistor_B[8];
//    uint16_t thermistor_C[8];
//    uint16_t thermistor_D[8];
//
//    uint16_t Fletcher_Code;
//}thermistor_pkt_t;

//typedef struct {
//    //CCSDS
//
//    uint8_t Version_ID;
//    uint8_t APID;
//    uint16_t Seq_no;
//    uint16_t PL;
//
//    //OBC HK
//
//};

//ADC Macros
#define ADC_I2C_ADDR 0x21
#define ADC_I2CU1_ADDR 0x24
#define ADC_I2CU2_ADDR 0x23
#define ADC_I2CU3_ADDR 0x24
#define ADC_I2CU4_ADDR 0x23

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

//GPIO MACROS
#define GMC_EN_PIN MSS_GPIO_24
#define EPS_PIN1 GPIO_14
#define EPS_PIN2 GPIO_15
#define EPS_PIN3 GPIO_16
#define EPS_PIN4 GPIO_3

//IMU Macros
#define IMU_ADDR 0x6b
#define IMU_WHO_AM_I_REG 0x0F

/** Function to initialise ADC
 * @brief Initialises the ADC corresponding to the given address
 * 
 * @param i2c_chx: The i2c channel through which the ADC is connected
 * @param address: The address of the ADC
 * 
 */
uint8_t ADC_Init(i2c_instance_t i2c_chx,uint8_t address);

/**
 * @brief Get the ADC value 
 * 
 * @param i2c_chx   : the i2c channel used
 * @param address   : the address of the ADC
 * @param chx       : the channel to convert  
 * @return double   : returns the value given by the ADC
 */
uint16_t get_ADC_value(i2c_instance_t i2c_chx,uint8_t address,uint8_t chx);

/**
 * @brief Initialises the I2Cs in Pilot
 *
 */
void I2C_Init();

/**
 * @brief Initialises the GPIOs in Pilot
 *
 */
void GPIO_Init();

/**
 * @brief Initialises the Uarts in Pilot
 *
 */
void Uart_Init();

/**
 * @brief Initialisation of Pilot CDH board
 *
 */
void Pilot_Init();

#endif
