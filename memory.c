#include "memory.h"

void initialise_partition(partition_t *partition, uint32_t start_b,uint32_t end_b) {
	partition->start_block = start_b;
	partition->end_block = end_b;
	partition->read_pointer = start_b;
	partition->write_pointer = start_b;
}

//uint8_t store_data(block_table_t *block,uint8_t *data,block_type_t type) {
//	uint32_t address = 0;
//	switch(type) {
//	case PAYLOAD:
//		address = block->payload * 512;
//		block->payload = block->payload + 1;
//		break;
//	case HK:
//		address = block->hk * 512;
//		block->hk = block->hk + 1;
//		break;
//	case TASK_LOGS:
//		address = block->task_logs * 512;
//		block->task_logs = block->task_logs + 1;
//		break;
//	default:
//		address = block->task_logs * 512;
//		block->task_logs = block->task_logs + 1;
//		break;
//	}
//	if(SD_Write(address,data) == 0) {
//		return 0;
//	} else {
//		return 1;
//	}
//}

uint8_t store_data(partition_t *partition,uint8_t *data) {
	if((partition->write_pointer >= partition->end_block) || (partition->write_pointer < partition->start_block)) {
		return 1;
	}

	if(SD_Write(partition->write_pointer * 512,data) == 0) {
		partition->write_pointer++;
		return 0;
	}

	return 1;
}

uint8_t read_data(partition_t *partition,uint8_t *data) {
	if((partition->read_pointer >= partition->write_pointer) || (partition->read_pointer < partition->start_block)) {
		return 1;
	}

	if(SD_Read(partition->read_pointer * 512,data) == 0) {
		partition->read_pointer++;
		return 0;
	}

	return 1;
}
