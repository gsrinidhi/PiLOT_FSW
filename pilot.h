#ifndef _PILOT_H_
#define _PILOT_H_
#include"peripherals.h"
#include "SD.h"
#include "packet_definitions.h"
#include "pslv_interface.h"
#include "ccsds.h"
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
#define EN_UART					MSS_GPIO_13
#define EN_COMMS				MSS_GPIO_14
#define RESET_GPIO				MSS_GPIO_17

//IMU Macros
#define IMU_ADDR 0x6b
#define IMU_WHO_AM_I_REG 0x0F

//PERIOD Macros
#define DEFAULT_PAYLOAD_PERIOD	10000
#define DEFAULT_HK_PERIOD		1000
#define ONE_SPP_RATE			1000
#define TWO_SPP_RATE			2000
#define FIVE_SPP_RATE			5000
#define TEN_SPP_RATE			10000
#define PAYLOAD_PERIOD_L		0xFFFF
#define PAYLOAD_PERIOD_H		0xFFFF
#define HK_PERIOD_H				0xFFFF
#define HK_PERIOD_L				0xFFFF

//TASK IDs
#define THERMISTOR_TASK_ID	1
#define HK_TASK_ID			2
#define SD_HK_TASK_ID		3

//Packet rate options
typedef enum PACKET_RATES {
	RATE_ONE_SPP = 0u,
	RATE_TWO_SPP,
	RATE_FIVE_SPP,
	RATE_TEN_SPP
}PACKET_RATES_t;

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
uint16_t get_ADC_value(i2c_instance_t i2c_chx,uint8_t address,uint8_t chx,uint8_t *flag);

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

void Pilot_Peripherals_Init();

/**
 * @brief Initialisation of Pilot CDH board
 *
 */
void Pilot_Init();

/**
 * @brief Get the thermistor packet
 * 
 * @param pkt       : thermistor packet pointer
 * @param seq_no    : sequence number of packet
 * @return uint8_t  : Returns 0 if successful, 1 otherwise
 */
uint8_t get_thermistor_vals(thermistor_pkt_t *pkt,uint16_t seq_no);

/**
 * @brief Function to obtain CDH peripheral health
 * 
 * @return uint8_t 
 * Bit 1(LSB)   : i2c_3
 * Bit 2        : i2c_5
 * Bit 3        : IMU
 * Bit 4        : SD card write
 * Bit 5        : SD card read
 */
uint8_t test_peripherals();

uint8_t get_IMU_acc(uint16_t *a_x,uint16_t *a_y,uint16_t *a_z);

uint8_t get_IMU_gyro(uint16_t *roll_rate, uint16_t *pitch_rate,uint16_t *yaw_rate);

void time_to_count(uint32_t ms,uint32_t *upper_count,uint32_t *lower_count);

uint8_t get_hk(hk_pkt_t *hk_pkt, uint16_t seq_no);
#endif
