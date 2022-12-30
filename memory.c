//#ifndef MEMORY_H_
//#define MEMORY_H_
//
//#include"memory.h"
//#include"SD.h"
//#include"global.h"
//
////block_table start_block,current_block;
////uint8_t memory_init(block_table_t start_block,) {
////    mem_status = SD_Init();
////    if(mem_status) {
////        return 1;
////    }
//////    mem_status = SD_Read(0,&mem_check);
//////    if(mem_check == 0) {
//////        start_block.comms = COMMS_BLOCK_INIT;
//////        start_block.payload = GMC_BLOCK_INIT;
//////        start_block.power = POWER_BLOCK_INIT;
//////        start_block.task_logs = LOGS_BLOCK_INIT;
//////        mem_check = 1;
//////        if(SD_Write(INIT_BLOCK,(uint8_t*)&start_block)!= 0) {
//////            return 1;
//////        }
//////        if(SD_Write(0,&mem_check) != 0) {
//////            return 1;
//////        }
//////    }
//////    mem_status = SD_Read(INIT_BLOCK,(uint8_t*)&start_block);
////    start_block.comms = COMMS_BLOCK_INIT;
////	start_block.payload = GMC_BLOCK_INIT;
////	start_block.power = POWER_BLOCK_INIT;
////	start_block.task_logs = LOGS_BLOCK_INIT;
////    current_block.comms = start_block.comms;
////    current_block.payload = start_block.payload;
////    current_block.power = start_block.power;
////    current_block.task_logs = start_block.task_logs;
////    if(mem_status) {
////        return 1;
////    }
////    return 0;
////}
//
//uint8_t gmc_store_data(block_table_t current_block,uint8_t *data) {
//    if(SD_Write(current_block.payload * 512,data) == 0) {
//        current_block.payload++;
//        return 0;
//    }
//    return 1;
//}
//
//uint8_t eps_store_data(block_table_t current_block,uint8_t *data) {
//    if(SD_Write(current_block.pslv_interface * 512,data) == 0) {
//        current_block.pslv_interface++;
//        if(current_block.pslv_interface >= 199) {
//        	current_block.pslv_interface = 150;
//        }
//        return 0;
//    }
//    return 1;
//}
//
//#endif

#include "memory.h"

// void initialise_block(block_table_t *block) {
// 	block->payload = PAYLOAD_BLOCK_INIT;
// 	block->hk = HK_BLOCK_INIT;
// 	block->task_logs = LOGS_BLOCK_INIT;
// }

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
	if((partition->read_pointer >= partition->end_block) || (partition->read_pointer < partition->start_block)) {
		return 1;
	}

	if(SD_Read(partition->read_pointer * 512,data) == 0) {
		partition->read_pointer++;
		return 0;
	}

	return 1;
}
