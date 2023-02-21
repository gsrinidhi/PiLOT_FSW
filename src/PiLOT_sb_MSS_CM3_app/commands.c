
#include "commands.h"

//#include<drivers/mss_uart/mss_uart.h>

extern void uart0_rx_handler(mss_uart_instance_t* this_uart);
extern uint8_t interface_count;
extern uint16_t get_ADC_value(i2c_instance_t *i2c_chx,uint8_t address,uint8_t chx,uint8_t *flag);
extern uint8_t get_IMU_acc(uint16_t *a_x,uint16_t *a_y,uint16_t *a_z);
extern uint8_t get_IMU_gyro(uint16_t *roll_rate, uint16_t *pitch_rate,uint16_t *yaw_rate);
char interface_debug[] = "debug\0";
char interface_pslv[] = "pslv\0";
void ftos(double s,char *value) {
	uint16_t k = (uint16_t)s;
	char stk[10];
	uint8_t i = 0,j = 0;
	while(k > 0) {
		stk[i] = k%10;
		k/=10;
		i++;
	}
	for(j = 0;j<i;j++) {
		value[j] = stk[i-1] + 48;
	}
	value[j] = '.';
	for(i = 0;i<2;i++) {
		k = (uint16_t)(s * 10);
		value[j+i+1] = (k%10) + 48;
		s*=10;
	}
	value[j+i+1] = '\0';
}
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
    MSS_UART_init(&g_mss_uart0, val, MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT );
    MSS_UART_set_rx_handler(&g_mss_uart0,uart0_rx_handler,MSS_UART_FIFO_SINGLE_BYTE);
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
	if(addr == '1') {
		addr = ADC_I2CU1_ADDR;
	} else if(addr == '2') {
		addr = ADC_I2CU2_ADDR;
	}
	uint8_t i = 0,flag;
	uint16_t value = 0;
	char float_value[20];
	double fv = 0;
	MSS_UART_polled_tx_string(&g_mss_uart0,"\n\r\0");
	for(;i<8;i++) {
		value = get_ADC_value(dbg,addr,i,&flag);
		ftos(flag,float_value);
		MSS_UART_polled_tx_string(&g_mss_uart0,float_value);
		MSS_UART_polled_tx_string(&g_mss_uart0," \0");
		fv = 3.3/1024 * value;
		ftos(fv,float_value);
		MSS_UART_polled_tx_string(&g_mss_uart0,float_value);
		MSS_UART_polled_tx_string(&g_mss_uart0,"\n\r\0");
	}
}

void get_imu_acc(char *data,uint8_t size) {
	short ax,ay,az;
	char acc[10];
	uint8_t res = get_IMU_acc(&ax,&ay,&az);
	double xa = ax * 0.0005987,ya = ay * 0.0005987,za = az * 0.0005987;
	ftos(xa,acc);
	MSS_UART_polled_tx_string(&g_mss_uart0,"\n\r\0");
	MSS_UART_polled_tx_string(&g_mss_uart0,"Ax: \0");
	MSS_UART_polled_tx_string(&g_mss_uart0,acc);
	MSS_UART_polled_tx_string(&g_mss_uart0,"\n\r\0");
	ftos(ya,acc);
	MSS_UART_polled_tx_string(&g_mss_uart0,"Ay: \0");
	MSS_UART_polled_tx_string(&g_mss_uart0,acc);
	MSS_UART_polled_tx_string(&g_mss_uart0,"\n\r\0");
	ftos(za,acc);
	MSS_UART_polled_tx_string(&g_mss_uart0,"Az: \0");
	MSS_UART_polled_tx_string(&g_mss_uart0,acc);
	MSS_UART_polled_tx_string(&g_mss_uart0,"\n\r\0");
}

void get_imu_gyro(char *data,uint8_t size) {
	short ax,ay,az;
	char acc[10];
	uint8_t res = get_IMU_gyro(&ax,&ay,&az);
	double xa = ax * 0.00875,ya = ay * 0.00875,za = az * 0.00875;
	ftos(xa,acc);
	MSS_UART_polled_tx_string(&g_mss_uart0,"\n\r\0");
	MSS_UART_polled_tx_string(&g_mss_uart0,"Ax: \0");
	MSS_UART_polled_tx_string(&g_mss_uart0,acc);
	MSS_UART_polled_tx_string(&g_mss_uart0,"\n\r\0");
	ftos(ya,acc);
	MSS_UART_polled_tx_string(&g_mss_uart0,"Ay: \0");
	MSS_UART_polled_tx_string(&g_mss_uart0,acc);
	MSS_UART_polled_tx_string(&g_mss_uart0,"\n\r\0");
	ftos(za,acc);
	MSS_UART_polled_tx_string(&g_mss_uart0,"Az: \0");
	MSS_UART_polled_tx_string(&g_mss_uart0,acc);
	MSS_UART_polled_tx_string(&g_mss_uart0,"\n\r\0");
}
