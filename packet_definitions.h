/*
 * packet_definitions.h
 *
 *  Created on: 30-Dec-2022
 *      Author: gonah
 */

#ifndef _PACKET_DEFINITIONS_
#define _PACKET_DEFINITIONS_
#define THERMISTOR_Version_ID
#define THERMISTOR_API_ID
#define THERMISTOR_PKT_LENGTH
#define THERMISTOR_FLETCHER_CODE

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

#endif
