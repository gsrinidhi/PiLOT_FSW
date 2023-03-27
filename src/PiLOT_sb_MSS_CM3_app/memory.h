/**
 * @file 	:	memory.h
 * @author 	:	Srinidhi G
 * @brief 	:	This file contains the functions needed to access the SD card using partitions
 * @version :	1.0
 * @date 	:	2023-01-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef MEMORY_H
#define MEMORY_H

#include<stdint.h>
#include "SD.h"
#define INIT_BLOCK 512
#define CURRENT_BLOCK 1024

//Define the initial block numbers of all partitions
#define PAYLOAD_BLOCK_INIT        	0x13C6EE
#define HK_BLOCK_INIT        		0xA
#define LOGS_BLOCK_INIT          	0x15C192
#define SD_BLOCK_INIT				0x298876
#define ARIS_BLOCK_INIT				0x2B831A

//Define the ending block numbers of all the partitions
#define PAYLOAD_BLOCK_END			0x15C12E
#define HK_BLOCK_END				0x13C68A
#define LOGS_BLOCK_END				0x298812
#define SD_BLOCK_END				0x2B82B6
#define ARIS_BLOCK_END				0x700000

//All the Partitions used
#define HK_PARTITION 				0x01
#define PAYLOAD_PARTITION			0x02
#define SD_PARTITION				0x03
#define LOGS_PARTITION				0x04
#define ARIS_PARTITION				0x05

#define POINTER_ERROR_MASK			0x80

/**
 * @brief To store data in the SD card, partitions are created, with each partition having the below four parameters.
 * 
 */
typedef struct {
	uint32_t start_block;//The starting of the partition
	uint32_t end_block;//The ending of the partion
	uint32_t read_pointer;//Block number currently reading
	uint32_t write_pointer;//Block number currently writing
}partition_t;

/**
 * @brief Function to initialise a partition
 * 
 * @param partition 	: The partition to be initialised
 * @param start_b 		: The starting block of the partion
 * @param end_b 		: The ending block of the partion
 */
void initialise_partition(partition_t *partition, uint32_t start_b,uint32_t end_b);

/**
 * @brief Function to write data to a partition
 * 
 * @param partition 	: The partition to which data has to be written
 * @param data 			: The data to be written
 * @return uint8_t 		: Returns 0 if successful, 1 if SD card operation failed, 2 if pointers are not valid
 */
uint8_t store_data(partition_t *partition,uint8_t *data);

/**
 * @brief Function to read data from a partition
 * 
 * @param partition 	: The partition from which data has to be read
 * @param data 			: Buffer to store the read data
 * @return uint8_t 		: Returns 0 if successful, 1 if SD card operation failed, 2 if pointers are not valid
 */
uint8_t read_data(partition_t *partition,uint8_t *data);
#endif
