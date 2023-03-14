/**
 * @file 	:	memory.c
 * @author 	:	Srinidhi G
 * @brief 	:	This file implements all the functions in memory.h
 * @version	:	1.0
 * @date 	:	2023-01-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "memory.h"

void initialise_partition(partition_t *partition, uint32_t start_b,uint32_t end_b) {
	partition->start_block = start_b;
	partition->end_block = end_b;
	partition->read_pointer = start_b;
	partition->write_pointer = start_b;
}

uint8_t store_data(partition_t *partition,uint8_t *data) {
	if((partition->write_pointer < partition->start_block)) {
		return POINTER_ERROR_MASK;
	}

	if(SD_Write(partition->write_pointer,data) == 0) {
		partition->write_pointer++;
		if((partition->write_pointer > partition->end_block)) {
			partition->write_pointer = partition->start_block;
		}
		return SD_WORKING_MASK;
	}


	return SD_INIT_MASK | SD_READ_MASK;
}

uint8_t read_data(partition_t *partition,uint8_t *data) {
	if((partition->read_pointer < partition->start_block)) {
		return POINTER_ERROR_MASK;
	}

	if(SD_Read(partition->read_pointer,data) == 0) {
		partition->read_pointer++;
		if((partition->read_pointer > partition->end_block)) {
			partition->read_pointer = partition->start_block;
		}
		return SD_WORKING_MASK;
	}


	return SD_INIT_MASK | SD_WRITE_MASK;
}
