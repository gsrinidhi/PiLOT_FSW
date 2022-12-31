#include"pilot.h"
#include "memory.h"


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

uint16_t get_ADC_value(i2c_instance_t i2c_chx,uint8_t address,uint8_t chx,uint8_t *flag) {
	uint8_t adc_read_value[2];
	uint8_t ch_read[] = {chx};
	ch_read[0] |= 0x8;
	ch_read[0] = ch_read[0] << 4;
	uint8_t status;
	//uint16_t conv_res,value;
	uint16_t voltage;
	I2C_write_read(&i2c_chx,address,ch_read,1,adc_read_value,2,I2C_RELEASE_BUS);
	status = I2C_wait_complete(&i2c_chx, I2C_NO_TIMEOUT);
	if(status != 0) {
		*flag = 1;
	} else {
		voltage = (adc_read_value[0] << 8 ) | adc_read_value[1];
		*flag = 0;
//		conv_res = (adc_read_value[0] << 8 ) | adc_read_value[1];
//		value = conv_res & 0x0FFF;
//		value = value >> 2;
//		voltage = value * 5.0/1024;
	}
	return voltage;
}

// Function below would generate the payload packet containing 40 items : -

uint8_t get_thermistor_vals(thermistor_pkt_t *pkt,uint16_t seq_no){
   pkt->Version_ID = THERMISTOR_Version_ID;
   pkt->APID = THERMISTOR_API_ID;
   pkt->Seq_no = seq_no;
   pkt->PL = THERMISTOR_PKT_LENGTH;
   uint8_t i = 0,flag;
   uint8_t loss_count;
   for(;i<8;i++){
        pkt->thermistor_set_A[i] = get_ADC_value(i2c_2, ADC_I2CU1_ADDR, i,&flag);
        loss_count+=flag;
        pkt->thermistor_set_B[i] = get_ADC_value(i2c_2, ADC_I2CU2_ADDR, i,&flag);
        loss_count+=flag;
        pkt->thermistor_set_C[i] = get_ADC_value(i2c_3, ADC_I2CU3_ADDR, i,&flag);
        loss_count+=flag;
        pkt->thermistor_set_D[i] = get_ADC_value(i2c_3, ADC_I2CU4_ADDR, i,&flag);
        loss_count+=flag;
     }

    pkt->Fletcher_Code  = THERMISTOR_FLETCHER_CODE;

    return loss_count;

}

void set_pkt2sd(){

    // Store the packet values by defining the pointer(by timestamping the data)
    //in the SD_CARD.

}

void test_sd(){

   //As the above function would write the data into the sd-card, so for testing the sd_card
   //we can downlink at a certain interval from the same pointer values as define above.

   // NOTE: - We can downlink the following 3 pkts of data : -
   // 1) thermistor_pkt
   // 2) HK_pkt
   // 3) SD_pkt (Will contain the same data as sent by thermistor OR HK but it will be read
   //            from SD_CARD.)

}

void I2C_Init() {
	MSS_I2C_init( &g_mss_i2c0, DUMMY_I2C_ADDR, MSS_I2C_PCLK_DIV_960 );
	MSS_I2C_init(&g_mss_i2c1,DUMMY_I2C_ADDR,MSS_I2C_PCLK_DIV_960);
	I2C_init(&g_core_i2c0, COREI2C_0_0, DUMMY_I2C_ADDR, I2C_PCLK_DIV_960);
	I2C_init(&g_core_i2c1, COREI2C_1_0, DUMMY_I2C_ADDR, I2C_PCLK_DIV_960);
	I2C_init(&g_core_i2c2, COREI2C_2_0, DUMMY_I2C_ADDR, I2C_PCLK_DIV_960);
	I2C_init(&g_core_i2c3, COREI2C_3_0, DUMMY_I2C_ADDR, I2C_PCLK_DIV_960);
	I2C_init(&g_core_i2c4, COREI2C_4_0, DUMMY_I2C_ADDR, I2C_PCLK_DIV_960);
	I2C_init(&g_core_i2c5, COREI2C_5_0, DUMMY_I2C_ADDR, I2C_PCLK_DIV_960);

}

void GPIO_Init() {
	MSS_GPIO_init();
	gpio1_irq_cnt = gpio2_irq_cnt = gpio3_irq_cnt = gpio4_irq_cnt = 0;
	MSS_GPIO_config(MSS_GPIO_19,MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_EDGE_POSITIVE);
	MSS_GPIO_enable_irq(MSS_GPIO_19);
	MSS_GPIO_config(MSS_GPIO_20,MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_EDGE_POSITIVE);
	MSS_GPIO_enable_irq(MSS_GPIO_20);
	MSS_GPIO_config(MSS_GPIO_4,MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_EDGE_POSITIVE);
	MSS_GPIO_enable_irq(MSS_GPIO_4);
	MSS_GPIO_config(MSS_GPIO_26,MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_EDGE_POSITIVE);
	MSS_GPIO_enable_irq(MSS_GPIO_26);
	MSS_GPIO_config(MSS_GPIO_13,MSS_GPIO_OUTPUT_MODE);
	MSS_GPIO_config(GMC_EN_PIN,MSS_GPIO_OUTPUT_MODE);
}

void Uart_Init() {
	
	UART_init(&uart0,COREUARTAPB_0_0,UART_BAUD_115200,(DATA_8_BITS | NO_PARITY));
	UART_init(&uart1,COREUARTAPB_1_0,UART_BAUD_115200,(DATA_8_BITS | NO_PARITY));
	UART_init(&uart2,COREUARTAPB_2_0,UART_BAUD_115200,(DATA_8_BITS | NO_PARITY));
	UART_init(&uart3,COREUARTAPB_3_0,UART_BAUD_115200,(DATA_8_BITS | NO_PARITY));
	UART_init(&uart4,COREUARTAPB_4_0,UART_BAUD_115200,(DATA_8_BITS | NO_PARITY));
}

void Pilot_Peripherals_Init() {
	GPIO_Init();
	I2C_Init();
	Uart_Init();
	SD_Init();
	MSS_TIM64_init(MSS_TIMER_ONE_SHOT_MODE);
	MSS_TIM64_load_immediate(0xFFFFFFFF,0xFFFFFFFF);
}
void Pilot_Init() {
	Pilot_Peripherals_Init();
	ADC_Init(i2c_2,ADC_I2CU1_ADDR);
	ADC_Init(i2c_2,ADC_I2CU2_ADDR);
	ADC_Init(i2c_3,ADC_I2CU1_ADDR);
	ADC_Init(i2c_3,ADC_I2CU2_ADDR);
	SD_Init();
	vc_init(VC1);
}

uint8_t test_peripherals() {
	uint8_t count = 0;
	uint8_t ch_read[] = {0x80};
	uint8_t adc_read_value[2];
	uint8_t result = 0x00,temp_result = 1;
	uint8_t tx[] = {IMU_WHO_AM_I_REG};
	uint8_t sd_test[512];
	for(;count<512;count++) {
		sd_test[count] = 0;
	}
	count = 0;
	i2c_status_t status;
	//Testing i2c_3 in cdh
	while(count < 10) {
		I2C_write_read(&g_core_i2c1,ADC_I2CU1_ADDR,ch_read,1,adc_read_value,2,I2C_RELEASE_BUS);
		status = I2C_wait_complete(&g_core_i2c1, I2C_NO_TIMEOUT);
		if(status == I2C_SUCCESS) {
			result |= 0x01;
			break;
		}
		count++;
	}
	//Testing i2c_5 in cdh
	count = 0;
	while(count < 10) {
		I2C_write_read(&g_core_i2c3,ADC_I2CU1_ADDR,ch_read,1,adc_read_value,2,I2C_RELEASE_BUS);
		status = I2C_wait_complete(&g_core_i2c3, I2C_NO_TIMEOUT);
		if(status == I2C_SUCCESS) {
			result |= 0x02;
			break;
		}
		count++;
	}
	//Testing IMU
	count = 0;
	while(count < 10) {
		I2C_write_read(&g_core_i2c5,IMU_ADDR,tx,1,ch_read,1,I2C_RELEASE_BUS);
		status = I2C_wait_complete(&g_core_i2c5, I2C_NO_TIMEOUT);
		if((status == I2C_SUCCESS) && (ch_read[0] == 0x68)) {
			result |= 0x04;
			break;
		}
		count++;
	}
	//SD card write test
	count = 0;
	while(count < 10) {
		temp_result = SD_Write(1024,sd_test);
		if(temp_result == 0) {
			result |= 0x08;
			temp_result = 1;
			break;
		}
		count++;
	}
	//SD card read test
	count = 0;
	while(count < 10) {
		temp_result = SD_Read(1024,sd_test);
		if(temp_result == 0) {
			result |= 0x10;
			temp_result = 1;
			break;
		}
		count++;
	}
	return result;
}

uint8_t get_IMU_acc(uint16_t *a_x,uint16_t *a_y,uint16_t *a_z) {
	uint8_t write_CTRL_REG6_XL[2] = {0x20,0x60};
	uint8_t read_ACC_out_X_L[] = {0x28};
	uint8_t read_ACC_out_Y_L[] = {0x2A};
	uint8_t read_ACC_out_Z_L[] = {0x2C};
	uint8_t read_ACC_out_X_H[] = {0x29};
	uint8_t read_ACC_out_Y_H[] = {0x2B};
	uint8_t read_ACC_out_Z_H[] = {0x2D};
	uint8_t IMU_slave_addr = 0x6b;
	uint8_t rx_buffer[1],rx_buffer_2[1];
	uint8_t result = 0,status;


	I2C_write_read(&g_core_i2c5,IMU_slave_addr,write_CTRL_REG6_XL,2,rx_buffer,
					1,I2C_RELEASE_BUS);
	status = I2C_wait_complete(&g_core_i2c5,I2C_NO_TIMEOUT);
	result+=status;

	I2C_write_read(&g_core_i2c5,IMU_slave_addr,read_ACC_out_Z_L,1,rx_buffer,
							1,I2C_RELEASE_BUS);

	status = I2C_wait_complete(&g_core_i2c5,I2C_NO_TIMEOUT);
	result+=status;

	I2C_write_read(&g_core_i2c5,IMU_slave_addr,read_ACC_out_Z_H,1,rx_buffer_2,
							1,I2C_RELEASE_BUS);

	status = I2C_wait_complete(&g_core_i2c5,I2C_NO_TIMEOUT);
	result+=status;

	*a_z = ((rx_buffer_2[0] << 8) | rx_buffer[0]);

	I2C_write_read(&g_core_i2c5,IMU_slave_addr,read_ACC_out_Y_L,1,rx_buffer,
							1,I2C_RELEASE_BUS);

	status = I2C_wait_complete(&g_core_i2c5,I2C_NO_TIMEOUT);
	result+=status;

	I2C_write_read(&g_core_i2c5,IMU_slave_addr,read_ACC_out_Y_H,1,rx_buffer_2,
							1,I2C_RELEASE_BUS);

	status = I2C_wait_complete(&g_core_i2c5,I2C_NO_TIMEOUT);
	result+=status;

	*a_y = ((rx_buffer_2[0] << 8) | rx_buffer[0]);

	I2C_write_read(&g_core_i2c5,IMU_slave_addr,read_ACC_out_X_L,1,rx_buffer,
							1,I2C_RELEASE_BUS);

	status = I2C_wait_complete(&g_core_i2c5,I2C_NO_TIMEOUT);
	result+=status;

	I2C_write_read(&g_core_i2c5,IMU_slave_addr,read_ACC_out_X_H,1,rx_buffer_2,
							1,I2C_RELEASE_BUS);

	status = I2C_wait_complete(&g_core_i2c5,I2C_NO_TIMEOUT);
	result+=status;

	*a_x = ((rx_buffer_2[0] << 8) | rx_buffer[0]);

	return status;

}


//void get_CDH_HK() {
//
//}



void FabricIrq0_IRQHandler(void)
{
    I2C_isr(&g_core_i2c0);
}

void FabricIrq1_IRQHandler(void)
{
    I2C_isr(&g_core_i2c1);
}

void FabricIrq2_IRQHandler(void)
{
    I2C_isr(&g_core_i2c2);
}

void FabricIrq3_IRQHandler(void)
{
    I2C_isr(&g_core_i2c3);
}

void FabricIrq4_IRQHandler(void)
{
    I2C_isr(&g_core_i2c4);
}

void FabricIrq5_IRQHandler(void)
{
    I2C_isr(&g_core_i2c5);
}

//void Timer1_IRQHandler(void) {
//	timer_flag = 0;
//	MSS_TIM1_clear_irq();
//}

void GPIO19_IRQHandler(void) {
	gpio1_irq_cnt++;
	MSS_GPIO_clear_irq( MSS_GPIO_19 );
}

void GPIO20_IRQHandler(void) {
	MSS_GPIO_clear_irq( MSS_GPIO_20 );
	gpio2_irq_cnt++;
}

void GPIO4_IRQHandler(void) {
	MSS_GPIO_clear_irq( MSS_GPIO_4 );
	gpio3_irq_cnt++;
}

void GPIO26_IRQHandler(void) {
	MSS_GPIO_clear_irq( MSS_GPIO_26 );
	gpio4_irq_cnt++;
}
