//#ifndef COMMANDS_H_
//#define COMMANDS_H_
//
//#include<stdlib.h>
//#include<stdio.h>
//#include<stdint.h>
//#include"global.h"
//#include"AHAN.h"
//#include"task.h"
//#include"eps.h"
//#define MAX_CMD_NAME_LEN            10
//#define MAX_NUMBER_OF_COMMANDS      10
//#define DEBUG_UART                  &uart3
//#define DEBUG_UART_PRINT            UART_send
//#define DEBUG_UART_IP               UART_get_rx
//
//
///* CLI print function*//**
// * @brief Used to print to the command line interface using the CoreUART
// *
// * @param str :
// * The string to be printed
// */
//void print(char *str);
//
///* CLI print number function*//**
// * @brief Used to print numbers to the command line interface using the CoreUART
// *
// * @param num:
// * The number to be printed
// */
//void print_num(uint8_t num);
//
///* Ping command*//**
// * @brief This command displays "ping" in the CLI
// *
// * @return uint8_t :
// * returns 0
// */
//uint8_t ping(void);
//
///* Input function*//**
// * @brief This function gets an input from the CLI and stores it in str. It returns when it detects a carriage return character
// It is a blocking function
// *
// * @param str
// * @return uint8_t
// */
//uint8_t getip(char *str);
//
///* Fabric I2C test function*//**
// * @brief This command tests the fabric I2C in the AHAN OBC board and displays if the
//    I2Cs are working or not
// *
// * @return uint8_t
// * returns 0
// */
//uint8_t i2c_test_cli(void);
//
///* Command line memory initialisation function*//**
// * @brief This function calls memory_init(), which initialises the start block and current block
// *
// */
//void cli_mem_init();
//
///* Start block print function*//**
// * @brief This command prints the start block
// *
// * @return uint8_t
// * returns 0 if successful, else returns 1
// */
//uint8_t print_start_block(void);
//
///* Logs display function*//**
// * @brief This command prints the latest 10 logs from the SD card.
// *
// * @return uint8_t
// * returns 0 if successfull, else returns 1
// */
//uint8_t display_logs(void);
//
///* EPS housekeeping display function *//**
// * @brief This command prints the latest 10 eps hk data stored in the SD card
// *
// * @return uint8_t
// * returns 0 if successfull, else returns 1
// */
//uint8_t display_eps_data(void);
//#endif

