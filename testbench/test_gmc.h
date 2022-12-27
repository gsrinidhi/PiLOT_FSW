#ifndef _TEST_GMC_H_
#define _TEST_GMC_H_

#include<drivers/mss_i2c/mss_i2c.h>
#include<stdint.h>

#define test_gmc_i2c &g_mss_i2c0

#define test_gmc_addr       0x50

#define TEST_GMC_INIT       MSS_I2C_init

#define TEST_GMC_WRITE   MSS_I2C_write

#define BUFFER_SIZE 10u

typedef enum gmc_i2c_state
{
    GMC_IDLE = 0u,
    GMC_COUNTING
} gmc_i2c_state_t;

void i2c_slave_init();

#endif
