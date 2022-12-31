#ifndef _PSLV_INTERFACE_
#define _PSLV_INTERFACE_

#include <drivers/CoreI2C/core_i2c.h>
#include "peripherals.h"

#define VC1 0x40
#define VC2 0x41
#define VC3 0x43
#define VC_SENSOR_I2C &g_core_i2c2
// Reg Addresses



#define VC_BUSV_CHx(x)                  ((0x2 * x))
#define VC_SHUNTV_CHx(x)                ((0x2 * x - 1))

#define VC_INIT_CMD                     0x7000
#define VC_CONFIG_REG 					0x00

uint8_t vc_write(uint8_t addr, uint8_t *tx, uint8_t tx_size);

uint8_t vc_read(uint8_t addr, uint8_t *rx, uint8_t rx_size);

uint8_t vc_read_reg(uint8_t vc_addr, uint8_t reg_addr,uint8_t *rx);

uint8_t vc_write_reg(uint8_t vc_addr, uint8_t reg_addr,uint8_t *tx,uint8_t tx_size);

uint8_t vc_init(uint8_t addr);

uint16_t read_bus_voltage(uint8_t addr, uint8_t chx,uint8_t *flag);


#endif
