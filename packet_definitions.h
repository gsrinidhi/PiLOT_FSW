/*
 * packet_definitions.h
 *
 *  Created on: 30-Dec-2022
 *      Author: gonah
 */

#ifndef _PACKET_DEFINITIONS_
#define _PACKET_DEFINITIONS_
#define THERMISTOR_Version_ID		0x1
#define THERMISTOR_API_ID			0x16
#define THERMISTOR_PKT_LENGTH		76
#define THERMISTOR_FLETCHER_CODE	0x00

#define LOGS_Version_ID		0x2
#define LOGS_API_ID			0x17
#define LOGS_PKT_LENGTH		108
#define LOGS_FLETCHER_CODE	0x00

typedef struct {
    //CCSDS

    uint8_t Version_ID;
    uint8_t APID;
    uint16_t Seq_no;
    uint16_t PL;

    uint16_t thermistor_set_A[8];
    uint16_t thermistor_set_B[8];
    uint16_t thermistor_set_C[8];
    uint16_t thermistor_set_D[8];

    uint16_t Fletcher_Code;
}thermistor_pkt_t;

typedef struct {
    //CCSDS

    uint8_t Version_ID;
    uint8_t APID;
    uint16_t Seq_no;
    uint16_t PL;

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

    uint8_t Version_ID;
    uint8_t APID;
    uint16_t Seq_no;
    uint16_t PL;

    log_entry_t logs[10];

    uint16_t Fletcher_Code;
}log_packet_t;

#endif
