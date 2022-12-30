#ifndef MEMORY_H
#define MEMORY_H

#include<stdint.h>
#include "SD.h"
#define INIT_BLOCK 512
#define CURRENT_BLOCK 1024

#define PAYLOAD_BLOCK_INIT        	100
#define HK_BLOCK_INIT        		150
#define LOGS_BLOCK_INIT          	200

#define PAYLOAD_BLOCK_END			149
#define HK_BLOCK_END				199
#define LOGS_BLOCK_END				249
// typedef struct {
//     uint16_t payload;
//     uint16_t hk;
//     uint16_t task_logs;
// }block_table_t;

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

// We need to define the HK parameters in the below struct : -


typedef struct HK_PKT HK_PKT_t;

// typedef enum block_type {
// 	PAYLOAD = 0u,
// 	HK,
// 	TASK_LOGS
// } block_type_t;

//uint8_t memory_init();

//uint8_t gmc_store_data(block_table_t current_block,uint8_t *data);
//
//uint8_t eps_store_data(block_table_t current_block,uint8_t *data);
//

//void initialise_block(block_table_t *block);
/**
 * @brief stores the given data in the SD card according to which type of data it is
 *
 * @param block     : the block holding the current block numbers
 * @param data      : the data to be written to the SD card
 * @param type      : the type of block
 * @return uint8_t  : returns 0 if successful, 1 otherwise
 */
//uint8_t store_data(block_table_t *block,uint8_t *data,block_type_t type);

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
 * @return uint8_t 		: Returns 0 if successful, 1 otherwise
 */
uint8_t store_data(partition_t *partition,uint8_t *data);

/**
 * @brief Function to read data from a partition
 * 
 * @param partition 	: The partition from which data has to be read
 * @param data 			: Buffer to store the read data
 * @return uint8_t 		: Returns 0 if successful, 1 otherwise
 */
uint8_t read_data(partition_t *partition,uint8_t *data);
#endif
