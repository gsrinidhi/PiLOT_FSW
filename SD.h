#ifndef SD_H_
#define SD_H_

#include "peripherals.h"
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
