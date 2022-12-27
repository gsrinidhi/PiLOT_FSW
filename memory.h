#ifndef MEMORY_H
#define MEMORY_H

#include<stdint.h>

#define INIT_BLOCK 512
#define CURRENT_BLOCK 1024

#define COMMS_BLOCK_INIT        100
#define POWER_BLOCK_INIT        150
#define GMC_BLOCK_INIT          200
#define LOGS_BLOCK_INIT         250 
struct block_table_t{
    uint16_t payload;
    uint16_t power;
    uint16_t comms;
    uint16_t task_logs;
};

uint8_t memory_init();

uint8_t gmc_store_data(uint8_t *data);

uint8_t eps_store_data(uint8_t *data);
#endif
