#ifndef MEMORY_H
#define MEMORY_H

#include<stdint.h>

#define INIT_BLOCK 512
#define CURRENT_BLOCK 1024

#define COMMS_BLOCK_INIT        100
#define POWER_BLOCK_INIT        150
#define GMC_BLOCK_INIT          200
#define LOGS_BLOCK_INIT         250 
//typedef struct {
//    uint16_t payload;
//    uint16_t cdh_hk;
//    uint16_t sd_test;
//    uint16_t pslv_interface;
//    uint16_t imu;
//    uint16_t task_logs;
//}block_table_t;

// We need to define the HK parameters in the below struct : -

//struct HK_PKT
//    {
//        //CCSDS
//
//        uint8_t Version_ID;
//        uint8_t APID;
//        uint16_t Seq_no;
//        uint16_t PL;
//
//        //CDH
//        uint8_t CDH_Primary_info;
//        uint8_t CDH_Subsystem_ON_Stat;
//        uint8_t CDH_System_Data_Valid_flags;
//        uint32_t Command_Accept_count;
//        uint32_t Command_Reject_count;
//
//        //EPS
//
//        uint16_t EPS_Bat_Volt;
//        uint16_t EPS_Bat_Curr;
//        uint16_t EPS_Bus_Volt;
//        uint16_t EPS_Bus_Curr;
//        uint16_t EPS_Bat_Soc;
//        uint16_t EPS_Bat_Temp1;
//        uint16_t EPS_Bat_Temp2;
//        uint8_t SubSystem_Temp_Valid_flags;
//        uint8_t SubSystem_Curr_Valid_flags;
//        uint8_t SubSystem_Volt_Valid_flags;
//
//        //UHF
//
//        uint16_t Status_and_Control_Word;
//        uint16_t Num_Transmit_Packets;
//        uint16_t Num_Receive_Packets;
//
//        //Fletcher Code
//
//        uint16_t Fletcher_Code;
//    };

typedef enum block_type {
	PAYLOAD = 0u,
	CDH_HK,
	SD_TEST,
	PSLV_INTERFACE,
	IMU,
	TASK_LOGS
} block_type_t;

//uint8_t memory_init();

uint8_t gmc_store_data(block_table_t current_block,uint8_t *data);

uint8_t eps_store_data(block_table_t current_block,uint8_t *data);

/**
 * @brief stores the given data in the SD card according to which type of data it is
 * 
 * @param block     : the block holding the current block numbers
 * @param data      : the data to be written to the SD card
 * @param type      : the type of block 
 * @return uint8_t  : returns 0 if successful, 1 otherwise
 */
uint8_t store_data(block_table_t *block,uint8_t *data,block_type_t type);
#endif
