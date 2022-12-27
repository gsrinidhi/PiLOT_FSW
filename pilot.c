#include"pilot.h"

uint8_t ADC_Init(i2c_instance_t i2c_chx,uint8_t address){
	uint8_t status;
	uint8_t channel = 0;
	uint8_t return_value = 0;
	//Write max and min values to DATA HIGH and DATA LOW registers respectively for all channels
	uint8_t DATA_HIGH[] = {DATA_HIGH_REG(0),DATAHIGH_MAX_H,DATAHIGH_MAX_L};
	uint8_t DATA_LOW[] = {DATA_LOW_REG(0),DATA_LOW_MIN_H,DATA_LOW_MIN_L};
	for(;channel <= 3;channel++) {
		DATA_HIGH[0] = DATA_HIGH_REG(channel);
		DATA_LOW[0] = DATA_LOW_REG(channel);
		I2C_write(&i2c_chx,address,DATA_HIGH,sizeof(DATA_HIGH),I2C_RELEASE_BUS);
		status = I2C_wait_complete(&i2c_chx, I2C_NO_TIMEOUT);
		I2C_write(&i2c_chx,address,DATA_LOW,sizeof(DATA_LOW),I2C_RELEASE_BUS);
		status = I2C_wait_complete(&i2c_chx, I2C_NO_TIMEOUT);
		return_value |= (status << channel);
	}
	
	return return_value;
}

double get_ADC_value(i2c_instance_t i2c_chx,uint8_t address,uint8_t chx) {
	uint8_t adc_read_value[2];
	uint8_t ch_read[] = {chx};
	ch_read[0] |= 0x4;
	ch_read[0] = ch_read << 4;
	uint8_t status;
	uint16_t conv_res,value;
	double voltage;
	I2C_write_read(&i2c_chx,address,ch_read,1,adc_read_value,2,I2C_RELEASE_BUS);
	status = I2C_wait_complete(&i2c_chx, I2C_NO_TIMEOUT);
	if(status != 0) {
		voltage = -1;
	} else {
		conv_res = (adc_read_value[0] << 8 ) | adc_read_value[1];
		value = conv_res & 0x0FFF;
		value = value >> 2;
		voltage = value * 3.3/1024;
	}
	return voltage;
}