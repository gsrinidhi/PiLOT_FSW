/*
 * cli_packet_definitions.h
 *
 *  Created on: 04-Mar-2023
 *      Author: S-SPACE
 */

#ifndef CLI_PACKET_DEFINITIONS_H_
#define CLI_PACKET_DEFINITIONS_H_

#define I2C_PKT_ID 1
#define I2C_PKT_LEN(pkt)	(17 + (((pkt)).argu->no_bytes*2))

typedef struct {
	uint8_t i2c_chx;
	uint8_t clock_speed;
	uint8_t msg_type;
	uint8_t output_type;
	uint16_t no_bytes;
	uint8_t *custom_msg;

} __attribute__((packed)) i2c_argu_t;

typedef struct {
	uint8_t Tx_Status;
	uint8_t Rx_Status;
	uint8_t no_Incorrect_bytes;
	uint16_t Tx_time ;
	uint16_t Rx_time;
	uint16_t Total_time;
	uint8_t *Rx_msg;

} __attribute__((packed)) i2c_results_t;

typedef struct{

	uint8_t ID;
	uint8_t length;
	i2c_argu_t *argu;
	i2c_results_t *results;

} __attribute__((packed)) i2c_packet_t;

#endif /* CLI_PACKET_DEFINITIONS_H_ */
