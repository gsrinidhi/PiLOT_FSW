#ifndef __GMC_H__
#define __GMC_H__

#include"AHAN.h"
#include"memory.h"
#include"peripherals.h"
#include"global.h"
#define GMC_Req_Data 0x50
#define GMC_I2C &g_core_i2c3
#define GMC_COUNT_INIT      0x88
#define GMC_COUNT_STOP      0x48
struct gmc_packet_t{
    mss_rtc_calendar_t gmc_time;
    uint16_t rcount;
};

uint8_t GMCInit();
uint8_t getGMCData();
uint8_t gmc_get_count();
uint8_t gmc_count_init();
#endif
