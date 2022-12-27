#ifndef __GLOBAL_H__
#define __GLOBAL_H__
#include"memory.h"
#include"gmc.h"
////UART instances
// UART_instance_t uart0;
// UART_instance_t uart1;
// UART_instance_t uart2;
// UART_instance_t uart3;
// UART_instance_t uart4;
//
//
////I2C instances
// i2c_instance_t g_core_i2c0;
// i2c_instance_t g_core_i2c1;
// i2c_instance_t g_core_i2c2;
// i2c_instance_t g_core_i2c3;
// i2c_instance_t g_core_i2c4;
// i2c_instance_t g_core_i2c5;


 struct block_table_t start_block,current_block;


//RTC
 //mss_rtc_calendar_t now;

//GMC
 struct gmc_packet_t gmc;

 uint8_t gmc_count_init();

uint8_t gmc_get_count();

//CLI
 uint8_t cli_task(void);

 void scheduler();


#endif
