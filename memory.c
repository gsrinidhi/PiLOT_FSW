#ifndef MEMORY_H_
#define MEMORY_H_

#include"memory.h"
#include"SD.h"
#include"global.h"

//block_table start_block,current_block;
static uint8_t mem_status,mem_check;
uint8_t memory_init() {
    mem_status = SD_Init();
    if(mem_status) {
        return 1;
    }
//    mem_status = SD_Read(0,&mem_check);
//    if(mem_check == 0) {
//        start_block.comms = COMMS_BLOCK_INIT;
//        start_block.payload = GMC_BLOCK_INIT;
//        start_block.power = POWER_BLOCK_INIT;
//        start_block.task_logs = LOGS_BLOCK_INIT;
//        mem_check = 1;
//        if(SD_Write(INIT_BLOCK,(uint8_t*)&start_block)!= 0) {
//            return 1;
//        }
//        if(SD_Write(0,&mem_check) != 0) {
//            return 1;
//        }
//    }
//    mem_status = SD_Read(INIT_BLOCK,(uint8_t*)&start_block);
    start_block.comms = COMMS_BLOCK_INIT;
	start_block.payload = GMC_BLOCK_INIT;
	start_block.power = POWER_BLOCK_INIT;
	start_block.task_logs = LOGS_BLOCK_INIT;
    current_block.comms = start_block.comms;
    current_block.payload = start_block.payload;
    current_block.power = start_block.power;
    current_block.task_logs = start_block.task_logs;
    if(mem_status) {
        return 1;
    }
    return 0;
}

uint8_t gmc_store_data(uint8_t *data) {
    if(SD_Write(current_block.payload * 512,data) == 0) {
        current_block.payload++;
        return 0;
    }
    return 1;
}

uint8_t eps_store_data(uint8_t *data) {
    if(SD_Write(current_block.power * 512,data) == 0) {
        current_block.power++;
        if(current_block.power >= 199) {
        	current_block.power = 150;
        }
        return 0;
    }
    return 1;
}

#endif
