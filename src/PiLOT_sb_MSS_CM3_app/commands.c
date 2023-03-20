
#include "commands.h"

//#include<drivers/mss_uart/mss_uart.h>

extern void uart0_rx_handler(mss_uart_instance_t* this_uart);
extern uint8_t interface_count;
extern uint16_t get_ADC_value(i2c_instance_t *i2c_chx,uint8_t address,uint8_t chx,uint8_t *flag);
extern uint8_t get_IMU_acc(uint16_t *a_x,uint16_t *a_y,uint16_t *a_z);
extern uint8_t get_IMU_gyro(uint16_t *roll_rate, uint16_t *pitch_rate,uint16_t *yaw_rate);
extern int pilot(uint16_t addr,uint8_t tx_gpio,uint8_t rx_gpio,uint8_t debug_flag);
extern uint8_t get_IMU_temp(uint16_t *temp);
char interface_debug[] = "debug\0";
char interface_pslv[] = "pslv\0";
timer_instance_t temp_timer;
uint16_t count = 0;
//void ftos(double s,char *value) {
//	uint16_t k = (uint16_t)s;
//	char stk[10];
//	uint8_t i = 0,j = 0;
//	while(k > 0) {
//		stk[i] = k%10;
//		k/=10;
//		i++;
//	}
//	for(j = 0;j<i;j++) {
//		value[j] = stk[i-1] + 48;
//	}
//	value[j] = '.';
//	for(i = 0;i<2;i++) {
//		k = (uint16_t)(s * 10);
//		value[j+i+1] = (k%10) + 48;
//		s*=10;
//	}
//	value[j+i+1] = '\0';
//}
void set_baud_rate(char* arg, uint8_t s){
    uint8_t i=0;
    uint8_t k = 0;
    for(;i<s-2;i++){
    	if(arg[i] == '\r'){
    		break;
    	}

        val = val*10;
        k = (int)(arg[i] - 48);
        val = val+k;

    }
//    MSS_UART_init(&g_mss_uart0, val, MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT );
//    MSS_UART_set_rx_handler(&g_mss_uart0,uart0_rx_handler,MSS_UART_FIFO_SINGLE_BYTE);
    MSS_UART_init(&g_mss_uart1,val,MSS_UART_DATA_8_BITS | MSS_UART_STICK_PARITY_0 | MSS_UART_ONE_STOP_BIT);
}

void change_interface_mode(char *arg,uint8_t size) {
	if(scmp(arg,interface_debug,5) == 0) {
		interface_count = 1;
	} else if(scmp(arg,interface_pslv,4) == 0) {
		interface_count = 0;
	}
}

void get_ADC_correct_values(char *arg,uint8_t size) {
	uint8_t chx = (uint8_t)arg[0];
	uint8_t addr = (uint8_t)arg[1];
	i2c_instance_t *dbg;
	if(chx == '0') {
		dbg = &i2c_3;
	} else if(chx == '1') {
		dbg = &i2c_5;
	} else if(chx == '2') {
		dbg = &g_core_i2c4;
	}
	if(addr == '0') {
		addr = ADC_I2CU1_ADDR;
	} else if(addr == '1') {
		addr = ADC_I2CU2_ADDR;
	}
	uint8_t i = 0,flag;
	uint16_t value = 0;
	char float_value[20];
	double fv = 0;
	MSS_UART_polled_tx_string(&g_mss_uart0,"\n\r\0");
	for(;i<8;i++) {
		value = get_ADC_value(dbg,addr,i,&flag);
		ftos(flag,float_value,2);
		MSS_UART_polled_tx_string(&g_mss_uart0,float_value);
		MSS_UART_polled_tx_string(&g_mss_uart0," \0");
		fv = 3.3/1024 * value;
		ftos(fv,float_value,2);
		MSS_UART_polled_tx_string(&g_mss_uart0,float_value);
		MSS_UART_polled_tx_string(&g_mss_uart0,"\n\r\0");
	}
}

void get_imu_acc(char *data,uint8_t size) {
	short ax,ay,az;
	char acc[10];
	uint8_t res = get_IMU_acc(&ax,&ay,&az);
	double xa = ax * 0.0005987,ya = ay * 0.0005987,za = az * 0.0005987;
	ftos(xa,acc,2);
	MSS_UART_polled_tx_string(&g_mss_uart0,"\n\r\0");
	MSS_UART_polled_tx_string(&g_mss_uart0,"Ax: \0");
	MSS_UART_polled_tx_string(&g_mss_uart0,acc);
	MSS_UART_polled_tx_string(&g_mss_uart0,"\n\r\0");
	ftos(ya,acc,2);
	MSS_UART_polled_tx_string(&g_mss_uart0,"Ay: \0");
	MSS_UART_polled_tx_string(&g_mss_uart0,acc);
	MSS_UART_polled_tx_string(&g_mss_uart0,"\n\r\0");
	ftos(za,acc,2);
	MSS_UART_polled_tx_string(&g_mss_uart0,"Az: \0");
	MSS_UART_polled_tx_string(&g_mss_uart0,acc);
	MSS_UART_polled_tx_string(&g_mss_uart0,"\n\r\0");
}

void get_imu_gyro(char *data,uint8_t size) {
	short ax,ay,az;
	char acc[10];
	uint8_t res = get_IMU_gyro(&ax,&ay,&az);
	double xa = ax * 0.00875,ya = ay * 0.00875,za = az * 0.00875;
	ftos(xa,acc,2);
	MSS_UART_polled_tx_string(&g_mss_uart0,"\n\r\0");
	MSS_UART_polled_tx_string(&g_mss_uart0,"Ax: \0");
	MSS_UART_polled_tx_string(&g_mss_uart0,acc);
	MSS_UART_polled_tx_string(&g_mss_uart0,"\n\r\0");
	ftos(ya,acc,2);
	MSS_UART_polled_tx_string(&g_mss_uart0,"Ay: \0");
	MSS_UART_polled_tx_string(&g_mss_uart0,acc);
	MSS_UART_polled_tx_string(&g_mss_uart0,"\n\r\0");
	ftos(za,acc,2);
	MSS_UART_polled_tx_string(&g_mss_uart0,"Az: \0");
	MSS_UART_polled_tx_string(&g_mss_uart0,acc);
	MSS_UART_polled_tx_string(&g_mss_uart0,"\n\r\0");
}

void i2c_test(i2c_argu_t *argu,i2c_results_t *res,uint8_t *Tx_Buffer,uint8_t *Rx_Buffer) {
	uint16_t i;
	uint32_t before = 0xFFFFFFFF,after;
	for(i=0;i<10;i++){
		Tx_Buffer[i] = i;
	}

	if(argu->msg_type == 0){
		argu->custom_msg = Tx_Buffer;
	}

	if(argu->i2c_chx == 0 || argu->i2c_chx == 1){
//		mss_i2c_instance_t *i2c;
//		mss_i2c_status_t status;
//		i2c = (argu->i2c_chx == 0) ? &g_mss_i2c0 : &g_mss_i2c1;
//		MSS_I2C_init(i2c,DUMMY_I2C_ADDR,argu->clock_speed);
//		TMR_init(&timer, CORETIMER_0_0, TMR_ONE_SHOT_MODE, PRESCALER_DIV_2, before);
//		TMR_start(&timer);
//		MSS_I2C_write(i2c,I2C_SLAVE_ADDRESS,argu->custom_msg,argu->no_bytes,MSS_I2C_RELEASE_BUS);
//		status = MSS_I2C_wait_complete(i2c,MSS_I2C_NO_TIMEOUT);
//		TMR_stop(&timer);
//		after = TMR_current_value(&timer);
//		res->Tx_time = (before - after)/50;
//		res->Tx_Status = !status;
//
//		TMR_reload(&timer,0xFFFFFFFF);
//		TMR_start(&timer);
//		MSS_I2C_read(i2c,I2C_SLAVE_ADDRESS,Rx_Buffer,argu->no_bytes,MSS_I2C_REENABLE_SLAVE_RX);
//		status = MSS_I2C_wait_complete(i2c,MSS_I2C_NO_TIMEOUT);
//		TMR_stop(&timer);
//		after = TMR_current_value(&timer);
//		res->Rx_time = (before - after)/50;
//		res->Rx_Status = !status;
//		res->no_Incorrect_bytes = 0;
//		for(i=0;i<argu->no_bytes;i++){
//			if(argu->custom_msg[i] != Rx_Buffer[i]){
//				res->no_Incorrect_bytes++;
//			}
//
//		}
//		res->Total_time = res->Rx_time + res->Tx_time;
//		res->Rx_msg = Rx_Buffer;
	}
	else {
		i2c_instance_t *i2c;
		i2c_status_t status;
		uint32_t base_addr = 0;
		switch(argu->i2c_chx) {
		case 2:
			i2c = &g_core_i2c0;
			base_addr = COREI2C_0_0;
			break;
		case 3:
			i2c = &g_core_i2c1;
			base_addr = COREI2C_1_0;
			break;
		case 4:
			i2c = &g_core_i2c2;
			base_addr = COREI2C_2_0;
			break;
		case 5:
			i2c = &g_core_i2c3;
			base_addr = COREI2C_3_0;
			break;
		default:
			i2c = &g_core_i2c0;
			base_addr = COREI2C_0_0;
			break;
		}
		I2C_init(i2c,base_addr,DUMMY_I2C_ADDR,argu->clock_speed);
		TMR_init(&timer, CORETIMER_0_0, TMR_ONE_SHOT_MODE, PRESCALER_DIV_2, before);
		TMR_start(&timer);
		I2C_write(i2c,I2C_SLAVE_ADDRESS,argu->custom_msg,argu->no_bytes,I2C_RELEASE_BUS);
		status = I2C_wait_complete(i2c,I2C_NO_TIMEOUT);
		TMR_stop(&timer);
		after = TMR_current_value(&timer);
		res->Tx_time = (before - after)/50;

		TMR_init(&timer, CORETIMER_0_0, TMR_ONE_SHOT_MODE, PRESCALER_DIV_2, before);
		TMR_start(&timer);
		I2C_read(i2c,I2C_SLAVE_ADDRESS,Rx_Buffer,argu->no_bytes,I2C_RELEASE_BUS);
		status = I2C_wait_complete(i2c,I2C_NO_TIMEOUT);
		TMR_stop(&timer);
		after = TMR_current_value(&timer);
		res->Rx_time = (before - after)/50;

		res->no_Incorrect_bytes = 0;
		for(i=0;i<argu->no_bytes;i++){
			if(argu->custom_msg[i] != Rx_Buffer[i]){
				res->no_Incorrect_bytes++;
			}

		}
		res->Total_time = res->Rx_time + res->Tx_time;
		res->Rx_msg = Rx_Buffer;
	}
}

void i2c_test_cmd(char *data,uint8_t size){
	i2c_argu_t i2c_argu;
	i2c_results_t result;
	i2c_packet_t pkt;
	uint8_t tx[10],rx[10];
	pkt.argu = &i2c_argu;
	pkt.results = &result;
	argu_to_i2cargu(data,&i2c_argu);
	i2c_test(&i2c_argu,&result,tx,rx);
	pkt.ID = I2C_PKT_ID;
	pkt.length = I2C_PKT_LEN(pkt);

	COSMOS_UART_SEND(&COSMOS_UART,(uint8_t* )&(pkt),2);
	COSMOS_UART_SEND(&COSMOS_UART, (uint8_t * )(pkt.argu),6);
	COSMOS_UART_SEND(&COSMOS_UART,(uint8_t *)(pkt.argu->custom_msg),pkt.argu->no_bytes);
	COSMOS_UART_SEND(&COSMOS_UART, (uint8_t * )(pkt.results),9);
	COSMOS_UART_SEND(&COSMOS_UART,(uint8_t *)(pkt.results->Rx_msg),pkt.argu->no_bytes);

}


void echo(char *data,uint8_t size) {
	DEBUG_UART_SEND(&DEBUG_UART,"\n\r",2);
	DEBUG_UART_SEND(&DEBUG_UART,(uint8_t*)data,size);

}

void echo_str(char *data) {
	DEBUG_UART_SEND_STR(&DEBUG_UART,data);
}

void sd_test_cmd(char *data,uint8_t size) {
	uint32_t addr = s_to_i(data);
	uint8_t w_data[512],r_data[512];

	uint16_t state = 0;
	for(state = 0;state < 512;state++) {
		w_data[state] = state;
	}
	state = 0;
	state = !SD_Init();
	print_num("\n\rAddress: \0",(double)addr);
	print_num("SD Init \0",(double)state);
	state = !SD_Write(addr,w_data);
	print_num("SD Write \0",(double)state);
	state = !SD_Read(addr,r_data);
	print_num("SD Read \0",(double)state);
	uint16_t count = 0;
	for(state = 0;state < 512;state++){
		if(w_data[state] != r_data[state]) {
			count++;
		}
	}
	print_num("Mismatch: \0",(double)count);
	MSS_GPIO_set_output(SD_CARD_GPIO,0);


}

void i2c_signal_check(char *data, uint8_t size) {
	uint8_t i2c_sig = 0;
	uint8_t i2c_chx = data[0] - 48;
	uint8_t clock_rate = data[2] - 48;
	void uart0_i2c_signal_handler (mss_uart_instance_t* this_uart) {
		MSS_UART_get_rx(&g_mss_uart0,&i2c_sig,1);
		i2c_sig = 1;
	}
	i2c_instance_t *i2c;
	uint32_t addr;
	uint8_t tx[] = {'S'};
	switch(i2c_chx) {
	case 2:
		i2c = &g_core_i2c0;
		addr = COREI2C_0_0;
		break;
	case 3:
		i2c = &g_core_i2c1;
		addr = COREI2C_1_0;
		break;
	case 4:
		i2c = &g_core_i2c2;
		addr = COREI2C_2_0;
		break;
	case 5:
		i2c = &g_core_i2c3;
		addr = COREI2C_3_0;
		break;
	}
	I2C_init(i2c,addr,DUMMY_I2C_ADDR,clock_rate);
	MSS_UART_set_rx_handler(&g_mss_uart0,uart0_i2c_signal_handler,MSS_UART_FIFO_SINGLE_BYTE);
	echo_str("\n\rI2C signal transmitting\n\r");
	while(i2c_sig == 0) {
		I2C_write(i2c,ADC_I2CU1_ADDR,tx,1,I2C_RELEASE_BUS);
	}
	MSS_UART_set_rx_handler(&g_mss_uart0,uart0_rx_handler,MSS_UART_FIFO_SINGLE_BYTE);


}

void start_pilot(char *data, uint8_t size) {
	uint8_t pilot_reset;
	void uart0_pilot_reset_handler (mss_uart_instance_t* this_uart) {
		MSS_UART_get_rx(&g_mss_uart0,&pilot_reset,1);
		pilot_reset = 1;
		NVIC_SystemReset();
	}

	uint16_t addr;
	uint8_t tx_en,rx_en,dbg;
	argu_to_pilotargu(data,&addr,&tx_en,&rx_en,&dbg);
	MSS_UART_set_rx_handler(&g_mss_uart0,uart0_pilot_reset_handler,MSS_UART_FIFO_SINGLE_BYTE);
	pilot(addr,tx_en,rx_en,dbg);
}

void get_temp(char *data,uint8_t size) {
	uint16_t temp;
	double val = 0;
	uint8_t res = get_IMU_temp(&temp);
	if(res!= 0) {
		echo_str("\n\rIMU I2D Failed");
	}
	val = (double)(temp) / 16.0 + 25.0;
	if(size == 0) {
		print_num("\n\rTemperature is :\0",val);
	} else if(size == 1) {
		print_num("\n\rRaw Value is :\0",temp);
		print_num("\n\r Temperature is :\0",val);
	}

}

void FabricIrq11_IRQHandler(void) {
	TMR_clear_int(&temp_timer);
	print_num("\n\r",(double)count);
	get_temp(NULL,1);
	count++;
}
void start_get_temp(char *data, uint8_t temp) {



	uint32_t load_value;
	if(data[0] == '0') {
		load_value = 5000 * TIMER_COUNT_PER_MS/2;
	} else if(data[0] == '1') {
		load_value = 10000 * TIMER_COUNT_PER_MS/2;
	}
	void uart0_i2c_signal_handler (mss_uart_instance_t* this_uart) {
		MSS_UART_get_rx(&g_mss_uart0,&count,1);
		TMR_stop(&temp_timer);
	}
	NVIC_EnableIRQ( FabricIrq11_IRQn );
	NVIC_SetPriority(FabricIrq11_IRQn,0xFE);
	TMR_init(&temp_timer,CORETIMER_2_0,TMR_CONTINUOUS_MODE,PRESCALER_DIV_2,load_value);
	TMR_enable_int(&temp_timer);
	TMR_start(&temp_timer);
	MSS_UART_set_rx_handler(&g_mss_uart0,uart0_i2c_signal_handler,MSS_UART_FIFO_SINGLE_BYTE);
	echo_str("\n\rCollecting IMU temp\n\r");
	MSS_UART_set_rx_handler(&g_mss_uart0,uart0_rx_handler,MSS_UART_FIFO_SINGLE_BYTE);
}

void test_reset(char *data,uint8_t size) {
	echo_str("\n\rPerforming Reset\0");
	NVIC_SystemReset();
}

void rs485_tx_test(char *data,uint8_t size) {
	uint8_t tx_inv,rx_inv;
	tx_inv = (uint8_t)(data[0] - 48);
	rx_inv = (uint8_t)(data[2] - 48);
	MSS_GPIO_set_output(TX_INV_EN,tx_inv);
	MSS_GPIO_set_output(RX_INV_EN,rx_inv);
	uint8_t rs485_check = 0;
	uint8_t tx_buff[] = {0x88,0x44,0x66};
	void uart0_rs485_signal_handler (mss_uart_instance_t* this_uart) {
		MSS_UART_get_rx(&g_mss_uart0,&rs485_check,1);
		rs485_check = 1;
	}
	echo_str("\n\rTransmitting through UART 1\0");
	MSS_UART_set_rx_handler(&g_mss_uart0,uart0_rs485_signal_handler,MSS_UART_FIFO_SINGLE_BYTE);
	MSS_GPIO_set_output(EN_UART,1);
	while(rs485_check == 0) {
		MSS_UART_polled_tx(&g_mss_uart1,tx_buff,3);
	}
	MSS_GPIO_set_output(EN_UART,0);
	echo_str("\n\rFinished Transmitting through UART 1\0");
	MSS_UART_set_rx_handler(&g_mss_uart0,uart0_rx_handler,MSS_UART_FIFO_SINGLE_BYTE);
}

void read_vc_sensor(char *data,uint8_t size) {
	echo_str("\n\rTesting VC sensor\0");
	uint8_t flag = 0;
	flag = vc_init(VC1);
	print_num("\n\rVC Init: \0",flag);
	flag = 0;
	double vval = 0;
	uint16_t c1;
	c1 = read_bus_voltage(VC1,1,&flag);
	vval = c1 * 0.001;
	print_num("\n\rCh1 flag = \0",flag);
	print_num("Ch1 voltage = \0",vval);
	c1 = read_bus_voltage(VC1,2,&flag);
	vval = c1 * 0.001;
	print_num("\n\rCh2 flag = \0",flag);
	print_num("Ch2 voltage = \0",vval);
	c1 = read_bus_voltage(VC1,3,&flag);
	vval = c1 * 0.001;
	print_num("\n\rCh3 flag = \0",flag);
	print_num("Ch3 voltage = \0",vval);

}

void read_vc_sensor_i(char *data,uint8_t size) {
	echo_str("\n\rTesting VC sensor\0");
	uint8_t flag = 0;
	flag = vc_init(VC1);
	print_num("\n\rVC Init: \0",flag);
	flag = 0;
	double vval = 0;
	uint16_t c1;
	c1 = read_shunt_voltage(VC1,1,&flag);
	vval = c1 * 0.5;
	print_num("\n\rCh1 flag = \0",flag);
	print_num("Ch1 current = \0",vval);
	c1 = read_shunt_voltage(VC1,2,&flag);
	vval = c1 * 0.5;
	print_num("\n\rCh2 flag = \0",flag);
	print_num("Ch2 current = \0",vval);
	c1 = read_shunt_voltage(VC1,3,&flag);
	vval = c1 * 0.5;
	print_num("\n\rCh3 flag = \0",flag);
	print_num("Ch3 current = \0",vval);

}

void adc_test(char *data,uint8_t size){
	echo_str("\n\rTesting Sensor Board ADC\0");
	uint8_t i2c_inst;
	uint8_t i2c_addr;
	uint8_t chx, flag;
	uint8_t rx_buf[2], m_flag=0;
	uint16_t value,chx_value, sense_Val;
	i2c_instance_t* i;

	void uart_adc_int_handler(mss_uart_instance_t* this_uart){
		MSS_UART_get_rx(&g_mss_uart0, rx_buf, 1);
		if(rx_buf[0] == 32){
			m_flag = 1;
		}
		else{
			m_flag = 2;
		}
	}

	i2c_inst = (uint8_t)(data[0] - 48);
	i2c_addr = (uint8_t)(data[2] -48);
	chx = (uint8_t)(data[4] -48);

	if(i2c_inst == 0){
		i = &i2c_3;
	}
	else if(i2c_inst == 1){
		i = &i2c_5;
	} else {
		i = &g_core_i2c4;
	}

	if(i2c_addr == 0){
		i2c_addr = ADC_I2CU1_ADDR;
	}
	else if(i2c_addr == 1){
		i2c_addr = ADC_I2CU2_ADDR;
	} else {
		i2c_addr = ADC_I2C_ADDR;
	}

	value = get_ADC_value(i, i2c_addr, chx, &flag);
	sense_Val = read_bus_voltage(VC1, 1, &flag);
	chx_value = (value & 0x7000) >> 12;
	value &= 0x0FFF;
	print_num("\n\rSensor_Board_Voltage: \0",sense_Val*0.001);
	print_num("\n\rChannel from ADC: \0",chx_value);
	print_num("Voltage: \0", (value*3.3)/4096);
	print_num("ADC_Flag: \0", flag);
	flag = 0;
	MSS_UART_set_rx_handler(&g_mss_uart0,uart_adc_int_handler,MSS_UART_FIFO_SINGLE_BYTE);
	while(1){

		if(m_flag == 1){
			MSS_UART_set_rx_handler(&g_mss_uart0, uart0_rx_handler, MSS_UART_FIFO_SINGLE_BYTE);
			break;
		}
		else if(m_flag == 2){
			value = get_ADC_value(i, i2c_addr, chx, &flag);
			sense_Val = read_bus_voltage(VC1, 1, &flag);
			chx_value = (value & 0x7000) >> 12;
			value &= 0x0FFF;
			print_num("\n\rSensor_Board_Voltage: \0",sense_Val*0.001);
			print_num("\n\rChannel from ADC: \0",chx_value);
			print_num("Voltage: \0", (value*3.3)/4096);
			print_num("ADC_Flag: \0", flag);
			flag = 0;
			m_flag = 0;

		}

	}
}
