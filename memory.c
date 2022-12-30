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

//#include "memory.h"
//
//uint8_t store_data(block_table_t *block,uint8_t *data,block_type_t type) {
//	uint32_t address = 0;
//	switch(type) {
//	case PAYLOAD:
//		address = block->payload * 512;
//		block->payload = block->payload + 1;
//		break;
//	case CDH_HK:
//		address = block->cdh_hk * 512;
//		block->cdh_hk = block->cdh_hk + 1;
//		break;
//	case SD_TEST:
//		address = block->sd_test * 512;
//		block->sd_test = block->sd_test + 1;
//		break;
//	case PSLV_INTERFACE:
//		address = block->pslv_interface * 512;
//		block->pslv_interface = block->pslv_interface + 1;
//		break;
//	case IMU:
//		address = block->imu * 512;
//		block->imu = block->imu + 1;
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
