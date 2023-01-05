/*
 * packet_definitions.h
 *
 *  Created on: 30-Dec-2022
 *      Author: gonah
 */

#ifndef _PACKET_DEFINITIONS_
#define _PACKET_DEFINITIONS_
//#define THERMISTOR_Version_ID		0x1
#define THERMISTOR_API_ID			0x16
#define THERMISTOR_PKT_LENGTH		sizeof(thermistor_pkt_t)
#define THERMISTOR_FLETCHER_CODE	0x00

//#define LOGS_Version_ID		0x2
#define LOGS_API_ID			0x17
#define LOGS_PKT_LENGTH		sizeof(log_packet_t)
#define LOGS_FLETCHER_CODE	0x00

//#define HK_Version_ID		0x3
#define HK_API_ID			0x18
#define HK_PKT_LENGTH		sizeof(hk_pkt_t)
#define HK_FLETCHER_CODE	0x00

#define SD_HK_API_ID    0x19
#define SD_HK_PKT_LENGTH  0x30
#define SD_HK_FLETCHER_CODE 0x00



typedef struct {
    //CCSDS


//    Uncomment the primary fields and implement bit shifting in packet formation functions for all packets
	uint16_t ccsds_p1;
	uint16_t ccsds_p2;
	uint16_t ccsds_p3;

    uint16_t thermistor_set_A[8];
    uint16_t thermistor_set_B[8];
    uint16_t thermistor_set_C[8];
    uint16_t thermistor_set_D[8];

    uint16_t Fletcher_Code;
}thermistor_pkt_t;

typedef struct {
    //CCSDS

    uint16_t ccsds_p1;
    uint16_t ccsds_p2;
    uint16_t ccsds_p3;


    //	uint16_t primary_1;
    //	uint16_t primary_2;
    //	uint16_t primary_3;

    uint8_t CDH_Periph_Status; //For all the 8 flags
    uint16_t Acc[3];  // X,Y,Z Axis
    uint16_t Angular_Rate[3]; //Pitch, Roll, Yaw
    uint16_t Sensor_Board_VC[2];
    uint16_t CDH_VC[2];
    uint16_t Comms_VC[2];

    uint16_t Fletcher_Code;
}hk_pkt_t;

/**
 * @brief Each log entry has the below four fields for every task that occurs
 * 
 */
typedef struct {
	uint32_t time_L;//lower time value
	uint32_t time_H;//upper time value
	uint8_t task_id;//task id
	uint8_t task_status;//task status;
}log_entry_t;

typedef struct {
    //CCSDS

    uint16_t ccsds_p1;
    uint16_t ccsds_p2;
    uint16_t ccsds_p3;

    //	uint16_t primary_1;
    //	uint16_t primary_2;
    //	uint16_t primary_3;

    log_entry_t logs[10];

    uint16_t Fletcher_Code;
}log_packet_t;

typedef struct {
    //CCSDS

    uint16_t ccsds_p1;
    uint16_t ccsds_p2;
    uint16_t ccsds_p3;

    uint32_t HK_Read_Pointer;
    uint32_t HK_Write_Pointer;
    uint32_t Thermistor_Read_Pointer;
    uint32_t Thermistor_Write_Pointer;
    uint32_t Logs_Read_Pointer;
    uint32_t Logs_Write_Pointer;
    uint32_t SD_Test_Read_Pointer;
    uint32_t SD_Test_Write_Pointer;

    uint16_t Fletcher_Code;
}SD_HK_pkt_t;

#endif
