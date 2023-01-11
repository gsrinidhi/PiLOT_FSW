/*
 * packet_definitions.h
 *
 *  Created on: 30-Dec-2022
 *      Author: gonah
 */

#ifndef _PACKET_DEFINITIONS_
#define _PACKET_DEFINITIONS_

#define THERMISTOR_API_ID			20
#define THERMISTOR_PKT_LENGTH		sizeof(thermistor_pkt_t)
#define THERMISTOR_FLETCHER_CODE	0x01

#define ARIS_API_ID           50
#define ARIS_PKT_LENGTH       sizeof(aris_pkt_t)
#define ARIS_FLETCHER_CODE    0x01

#define LOGS_API_ID			30
#define LOGS_PKT_LENGTH		sizeof(log_packet_t)
#define LOGS_FLETCHER_CODE	0x00

#define HK_API_ID			10
#define HK_PKT_LENGTH		sizeof(hk_pkt_t)
#define HK_FLETCHER_CODE	0x01

#define SD_HK_API_ID    	40
#define SD_HK_PKT_LENGTH  sizeof(SD_HK_pkt_t)
#define SD_HK_FLETCHER_CODE 0x01

//Command packet parameters
#define CMD_API_ID			0x20
#define CMD_PKT_LENGTH		sizeof(cmd_packet_t)
#define CMD_FLETCHER_CODE	0x00



typedef struct {
    //CCSDS

	uint16_t ccsds_p1;
	uint16_t ccsds_p2;
	uint16_t ccsds_p3;

    uint32_t ccsds_s1;
    uint16_t ccsds_s2;

    uint16_t collection_time;
    uint16_t collection_location;

    uint16_t thermistor_set_A[8];
    uint16_t thermistor_set_B[8];
    uint16_t thermistor_set_C[8];

    uint16_t Fletcher_Code;
}__attribute__((packed)) thermistor_pkt_t;

typedef struct {
	uint16_t collect_time;
	uint16_t collect_location;
	uint16_t aris_data[3];
}__attribute__((packed)) aris_sample_t;

typedef struct{
    uint16_t ccsds_p1;
    uint16_t ccsds_p2;
    uint16_t ccsds_p3;

    uint32_t ccsds_s1;
    uint16_t ccsds_s2;

    uint32_t start_time;

    aris_sample_t aris_samples[20];

    uint16_t Fletcher_Code;

}__attribute__((packed)) aris_pkt_t;

typedef struct {
    //CCSDS

    uint16_t ccsds_p1;
    uint16_t ccsds_p2;
    uint16_t ccsds_p3;

    uint32_t ccsds_s1;
    uint16_t ccsds_s2;

    uint8_t CDH_Periph_Status; //For all the 8 flags
    uint16_t Acc[3];  // X,Y,Z Axis
    uint16_t Angular_Rate[3]; //Pitch, Roll, Yaw
    uint16_t Sensor_Board_VC[2];
    uint16_t CDH_VC[2];
    uint16_t Comms_VC[2];
//
    uint16_t Fletcher_Code;
}__attribute__((packed)) hk_pkt_t;

/**
 * @brief Each log entry has the below four fields for every task that occurs
 * 
 */
typedef struct {
	uint32_t time_L;//lower time value
	uint32_t time_H;//upper time value
	uint8_t task_id;//task id
	uint8_t task_status;//task status;
}__attribute__((packed)) log_entry_t;

typedef struct {
    //CCSDS

    uint16_t ccsds_p1;
    uint16_t ccsds_p2;
    uint16_t ccsds_p3;

    uint32_t ccsds_s1;
    uint16_t ccsds_s2;

    log_entry_t logs[10];

    uint16_t Fletcher_Code;
}__attribute__((packed)) log_packet_t;

typedef struct {
    //CCSDS

    uint16_t ccsds_p1;
    uint16_t ccsds_p2;
    uint16_t ccsds_p3;

    uint32_t ccsds_s1;
    uint16_t ccsds_s2;

    uint32_t HK_Read_Pointer;
    uint32_t HK_Write_Pointer;
    uint32_t Thermistor_Read_Pointer;
    uint32_t Thermistor_Write_Pointer;
    uint32_t Logs_Read_Pointer;
    uint32_t Logs_Write_Pointer;
    uint32_t SD_Test_Read_Pointer;
    uint32_t SD_Test_Write_Pointer;

    uint16_t Fletcher_Code;
}__attribute__((packed)) SD_HK_pkt_t;

typedef struct {
    //CCSDS

    uint16_t ccsds_p1;
    uint16_t ccsds_p2;
    uint16_t ccsds_p3;

    uint32_t ccsds_s1;
    uint16_t ccsds_s2;

    uint8_t cmd_opcaode;
    uint8_t cmd_arg[4];

    uint16_t Fletcher_Code;
}__attribute__((packed)) cmd_packet_t;

#endif
