#include"eps.h"

extern uint8_t timer_flag;
eps_data_t eps_data;

uint8_t vc_write(uint8_t addr, uint8_t *tx, uint8_t tx_size) {
    uint8_t count = 0;
    //uint8_t tx1[] = {0x00};
    while(count < 10) {
        MSS_I2C_write(VC_SENSOR_I2C,addr,tx,tx_size,MSS_I2C_RELEASE_BUS);
        ACDH.I2C_0 = MSS_I2C_wait_complete(VC_SENSOR_I2C,MSS_I2C_NO_TIMEOUT);
        if(ACDH.I2C_0 == 0) {
            return 0;
        }
        count++;
    }
    return count;
    
}

uint8_t vc_read(uint8_t addr, uint8_t *rx, uint8_t rx_size) {
    uint8_t count = 0;
    while(count < 10) {
        MSS_I2C_read(VC_SENSOR_I2C,addr,rx,rx_size,MSS_I2C_RELEASE_BUS);
        ACDH.I2C_0 = MSS_I2C_wait_complete(VC_SENSOR_I2C,MSS_I2C_NO_TIMEOUT);
        if(ACDH.I2C_0 == 0) {
            return 0;
        }
        count++;
    }
    return count;
    
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

uint8_t fg_write(uint8_t addr, uint8_t reg_addr,uint8_t *tx, uint8_t tx_size) {
    uint8_t tx_data[MAX_TX_SIZE];
    tx_data[0] = reg_addr;
    uint8_t i;
    for(i = 1;i<=tx_size;i++) {
        tx_data[i] = tx[i-1];
    }
    i = 0;
    while(i<10) {
        I2C_write(FG_SENSOR_I2C,addr,tx_data,sizeof(tx_data),I2C_RELEASE_BUS);
        ACDH.I2C_3 = I2C_wait_complete(FG_SENSOR_I2C,I2C_NO_TIMEOUT);
        if(ACDH.I2C_3 == 0) {
            return 0;
        }
        i++;
    }

    return 1;
    
}

uint8_t fg_write_read(uint8_t addr, uint8_t reg_addr,uint8_t *rx, uint8_t rx_size) {
    uint16_t i = 0;
    while(i < 10) {
        I2C_write_read(FG_SENSOR_I2C,addr,&reg_addr,1,rx,rx_size,I2C_RELEASE_BUS);
        ACDH.I2C_3 = I2C_wait_complete(FG_SENSOR_I2C,I2C_NO_TIMEOUT);
        if(ACDH.I2C_3 == 0) {
            return 0;
        }
        i++;
    }

    return 1;
    
}

uint8_t fg_init(uint8_t addr) {
    uint8_t mode[2] = {0x40,0x00};
    uint8_t config[2] = {0x00,0x40};
    if(fg_write(addr,FG_MODE_REG,(uint8_t*)&mode,sizeof(mode)) != 0) {
        return 1;
    }
    if(fg_write(addr,FG_CONFIG_REG,(uint8_t*)&config,sizeof(config)) != 0) {
        return 1;
    }
    return 0;
}

uint8_t eps_init() {
    uint8_t init_mask = 0;
    init_mask = vc_init(VC1);
    init_mask |= (vc_init(VC2) << 1);
    init_mask |= (vc_init(VC3) << 2);
    //init_mask |= (fg_init(FG) << 3);
    return init_mask;
}

void eps_init_test() {
    uint8_t tx[] = {0x02},mode[] = {0x23,0x29},read[2] = {0x00,0x00},taddr[] = {0x02};
    uint16_t m = 0x7007;
    uint16_t bvol = 0;
    mss_i2c_status_t status;
    double voltage;
//    vc_write(0x43,tx,1);
//    vc_write(0x43,mode,2);
//    vc_write(0x43,tx,1);
//    vc_read(0x43,read,2);
    MSS_I2C_write(VC_SENSOR_I2C,0x43,tx,1,MSS_I2C_RELEASE_BUS);
    status = MSS_I2C_wait_complete(VC_SENSOR_I2C,MSS_I2C_NO_TIMEOUT);
    delay_ms(1);
    while(1) {
    	//delay_flag(10000);
        while(1) {
            MSS_I2C_write(VC_SENSOR_I2C,0x43,tx,1,MSS_I2C_RELEASE_BUS);
            status = MSS_I2C_wait_complete(VC_SENSOR_I2C,MSS_I2C_NO_TIMEOUT);
            //delay_ms(1);
//            MSS_I2C_write(VC_SENSOR_I2C,0x43,mode,2,MSS_I2C_RELEASE_BUS);
//            ACDH.I2C_0 = MSS_I2C_wait_complete(VC_SENSOR_I2C,MSS_I2C_NO_TIMEOUT);
//            MSS_I2C_write(VC_SENSOR_I2C,0x43,tx,1,MSS_I2C_RELEASE_BUS);
//            ACDH.I2C_0 = MSS_I2C_wait_complete(VC_SENSOR_I2C,MSS_I2C_NO_TIMEOUT);
            MSS_I2C_read(VC_SENSOR_I2C,0x43,read,2,MSS_I2C_RELEASE_BUS);
            status = MSS_I2C_wait_complete(VC_SENSOR_I2C,MSS_I2C_NO_TIMEOUT);
            bvol = read[0]<<8 | read[1];
			voltage = bvol * 0.001;
			voltage++;
        }
        //delay_ms(2000);
    }

    MSS_I2C_write(VC_SENSOR_I2C,0x43,tx,1,MSS_I2C_RELEASE_BUS);
    ACDH.I2C_0 = MSS_I2C_wait_complete(VC_SENSOR_I2C,MSS_I2C_NO_TIMEOUT);
    delay_ms(1);
    MSS_I2C_write(VC_SENSOR_I2C,0x43,mode,2,MSS_I2C_RELEASE_BUS);
    ACDH.I2C_0 = MSS_I2C_wait_complete(VC_SENSOR_I2C,MSS_I2C_NO_TIMEOUT);
    delay_ms(1);
    MSS_I2C_write(VC_SENSOR_I2C,0x43,taddr,1,MSS_I2C_RELEASE_BUS);
	ACDH.I2C_0 = MSS_I2C_wait_complete(VC_SENSOR_I2C,MSS_I2C_NO_TIMEOUT);
	delay_ms(1);
    MSS_I2C_read(VC_SENSOR_I2C,0x43,read,2,MSS_I2C_RELEASE_BUS);
    ACDH.I2C_0 = MSS_I2C_wait_complete(VC_SENSOR_I2C,MSS_I2C_NO_TIMEOUT);

    uint8_t k = 0;
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

double read_bus_voltage(uint8_t addr, uint8_t chx) {
//	uint8_t bmode[2] = {0x00,0x00},bbus[2] = {0x00,0x00};
//    uint16_t mode,bs_ss,bs_ss_n;
//    uint16_t bus_voltage = 0,ch1_shunt = 0;
//    uint8_t flag = 0;
//    double bus_voltage_value = 0;
//    bs_ss = (uint16_t)VC_MODE_BS_SS;
//    bs_ss_n = ~bs_ss;
//    if((vc_read_reg(addr,VC_CONFIG_REG,bmode))) {
//        return -1;
//    }
//    mode = bmode[0]<<8;
//    mode |= bmode[1];
//    bus_voltage_value = mode * 0.001;
//    uint16_t get_zero = 0x4;
//    mode |= 0x1;
//    mode |= 0x2;
//    mode &= ~(get_zero);
//    bmode[0] = mode & 0xFF;
//    bmode[1] = mode >> 8;
//    if((vc_write_reg(addr,VC_CONFIG_REG,bmode,sizeof(bmode)))) {
//		return -1;
//	}
//    if((vc_read_reg(addr,VC_CONFIG_REG,bmode))) {
//            return -1;
//        }
//    //delay_ms(1);
//    if((vc_read_reg(0x43,0x02,bbus))) {
//		return -1;
//	}
	uint8_t read[2] = {0x00,0x00};
	uint16_t bvol = 0;
	double voltage;
	vc_read_reg(addr,VC_BUSV_CHx(chx),read);
	bvol = read[0]<<8 | read[1];
	voltage = bvol * 0.001;
	return voltage;
//    bus_voltage = bbus[0] << 8;
//    bus_voltage |= bbus[1];
//    bus_voltage_value = bus_voltage * 0.001;
//    if((bus_voltage & 0x8000) != 0) {
//        flag = 1;
//    }
//    bus_voltage&=0x7FFF;
//    bus_voltage-=1;
//    bus_voltage = ~bus_voltage;
//    bus_voltage &= 0x7FFF;
//    bus_voltage = bus_voltage >> 3;
//    bus_voltage_value = bus_voltage * 8e-3;
//    if(flag) {
//        bus_voltage_value *= -1;
//    }
    //return bus_voltage_value;
//    mode = 0;
//
//    mode &= VC_MODE_BS_SS;
//    mode |= VC_MODE_BS_SS;

}

double get_soc() {
    uint8_t rx[2] = {0x00,0x00};
    fg_write_read(FG,FG_SOC_REG,rx,2);
    return (rx[0] + rx[1]/256);
}

uint8_t get_voltages(void) {
    eps_data.vc1v1 = read_bus_voltage(VC1,(1));
    eps_data.vc1v2 = read_bus_voltage(VC1,(2));
    eps_data.vc1v3 = read_bus_voltage(VC1,(3));
    eps_data.vc2v1 = read_bus_voltage(VC2,(1));
    eps_data.vc2v2 = read_bus_voltage(VC2,(2));
    eps_data.vc2v3 = read_bus_voltage(VC2,(3));
    eps_data.vc3v1 = read_bus_voltage(VC3,(1));
    eps_data.vc3v2 = read_bus_voltage(VC3,(2));
    eps_data.vc3v3 = read_bus_voltage(VC3,(3));
    //eps_data.soc = get_soc();

    eps_store_data((uint8_t*)(&eps_data));

    return 0;

}
