#include"pslv_interface.h"
static i2c_status_t status;
uint8_t count;
uint8_t vc_write(uint8_t addr, uint8_t *tx, uint8_t tx_size) {
     count = 0;
    while(count < 10) {
        I2C_write(VC_SENSOR_I2C,addr,tx,tx_size,I2C_RELEASE_BUS);
        status = I2C_wait_complete(VC_SENSOR_I2C,I2C_NO_TIMEOUT);
        if(status == 0) {
            return 0;
        }
        count++;
    }
    return count;

}

uint8_t vc_read(uint8_t addr, uint8_t *rx, uint8_t rx_size) {
    count = 0;
    while(count < 10) {
        I2C_read(VC_SENSOR_I2C,addr,rx,rx_size,I2C_RELEASE_BUS);
        status = I2C_wait_complete(VC_SENSOR_I2C,I2C_NO_TIMEOUT);
        if(status == 0) {
            return 0;
        }
        count++;
    }
    return count;

}

uint8_t vc_read_reg(uint8_t vc_addr, uint8_t reg_addr,uint8_t *rx) {
    if((vc_write(vc_addr,&reg_addr,sizeof(reg_addr))) >= 10) {
        return 1;
    }
    if((vc_read(vc_addr,(uint8_t*)rx,2)) >= 10) {
        return 1;
    }

    return 0;
}

uint8_t vc_write_reg(uint8_t vc_addr, uint8_t reg_addr,uint8_t *tx,uint8_t tx_size) {
    if((vc_write(vc_addr,&reg_addr,sizeof(reg_addr))) >= 10) {
        return 1;
    }
    if((vc_write(vc_addr,(uint8_t*)tx,tx_size)) >= 10) {
        return 1;
    }

    return 0;
}

uint8_t vc_init(uint8_t addr) {
    uint16_t init_command = VC_INIT_CMD;
    uint8_t config_reg = VC_CONFIG_REG;
    if(vc_write(addr,&config_reg,sizeof(config_reg)) >= 10) {
        return 1;
    }
    if(vc_write(addr,(uint8_t*)(&init_command),sizeof(init_command)) >= 10) {
        return 1;
    }

    return 0;
}

uint16_t read_bus_voltage(uint8_t addr, uint8_t chx,uint8_t *flag) {
	uint8_t read[2] = {0x00,0x00};
	uint16_t bvol = 0;
	if(vc_read_reg(addr,VC_BUSV_CHx(chx),read) == 0) {
		bvol = read[0]<<8 | read[1];
		*flag = 0;
		return bvol;
	} else {
		*flag = 1;
		bvol = 0;
	}

}
