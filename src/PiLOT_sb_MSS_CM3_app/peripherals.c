/*
 * peripherals.c
 *
 *  Created on: 19-Mar-2023
 *      Author: gonah
 */

#include "peripherals.h"

i2c_instance_t g_core_i2c0;
i2c_instance_t g_core_i2c1;
i2c_instance_t g_core_i2c2;
i2c_instance_t g_core_i2c3;
i2c_instance_t g_core_i2c4;
i2c_instance_t g_core_i2c5;

i2c_status_t core_i2c_status;

timer_instance_t sd_timer;
