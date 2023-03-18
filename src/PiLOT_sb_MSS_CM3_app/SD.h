/**
 * @file    :   SD.h
* @author   :   Srinidhi G
 * @brief   :   This file contains the drivers to interface with the SD card
 * @version :   1.0
 * @date    :   2023-01-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SD_H_
#define SD_H_

#include "peripherals.h"

#define SD_INIT_MASK	0x1
#define SD_WRITE_MASK	0x2
#define SD_READ_MASK	0x4
#define SD_WORKING		(SD_INIT_MASK | SD_WRITE_MASK | SD_READ_MASK)
#define SD_TESTED		0x8
//#define SD_TIMEOUT	2

#define SD_THRESHOLD	10
/**
 * @brief Initialises the SD card
 * 
 * @return uint8_t : returns 0 if successful
 */
uint8_t SD_Init();

/**
 * @brief Function to write to the SD card
 * 
 * @param addr  : The address to be written to. This must be a multiple of 512
 * @param buff  : The data to be written to. This must be 512 bytes
 * @return uint8_t : returns 0 if successful
 */
uint8_t SD_Write(uint32_t addr,uint8_t *buff);

/**
 * @brief Fuction to read from the SD card
 * 
 * @param addr  : The address to be read from. This must be a multiple of 512
 * @param buff  : The buffer to store the read data in
 * @return uint8_t : returns 0 if successful
 */
uint8_t SD_Read(uint32_t addr,uint8_t *buff);
#endif
