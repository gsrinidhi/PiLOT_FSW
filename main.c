//#include"pilot.h"
////#include"eps.h"
////#include"cli.h"
//#include "drivers_config/sys_config/sys_config.h"
////#include"global.h"
////#include"testbench/test_gmc.h"
//#define DUMMY_I2C_ADDR 0x8
//#define BUFFER_SIZE 10u
//gpio_instance_t g_gpio;
//uint8_t rx_buffer[10],uart_rcount;
//uint8_t g_slave_rx_buffer[10u];
//uint8_t databuff[10u];
//
//#define TESTBENCH 0
//void find_tp() {
//	AHAN_Init();
//	ADC_Init();
//	double voltage,tp;
//	uint8_t flag = 0,cf = 0,lv = 0;
//	uint32_t cval,tdiff;
//	while(1) {
//		voltage = get_ADC_value(ADC_CHX(0));
//		if(voltage > 0.5 && flag == 0 && lv != 0) {
//			MSS_TIM1_init(MSS_TIMER_ONE_SHOT_MODE);
//			MSS_TIM1_load_immediate(0xFFFFFFFF);
//			MSS_TIM1_start();
//			MSS_TIM1_enable_irq();
//			flag = 1;
//		}
//		if(voltage < 0.5 && flag == 1) {
//			cval = MSS_TIM1_get_current_value();
//			tdiff = 0xFFFFFFFF - cval;
//			tp = 2 * (float)tdiff/TIMER_COUNT_PER_MS/1000.0;
//			cf = 1;
//			lv = 0;
//			flag = 0;
//		}
//		if(voltage < 0.5) {
//			lv = 1;
//		}
//	}
//}
//void uart1_rx_handler(mss_uart_instance_t * this_uart)
//{
//  //MSS_UART_get_rx(this_uart, rx_buffer, 1);
//  uint32_t rx_size  = 0;
//  rx_size = MSS_UART_get_rx(this_uart, rx_buffer, 5);
//  rx_size = 0;
//}
//void test_UART() {
//	GPIO_Init();
//	UART_instance_t g_uart;
//	uart_rcount = 0;
//	uint8_t tx_buffer[20] = "Jai Sri Ram";
//	MSS_UART_init(&g_mss_uart1,
//	                   MSS_UART_9600_BAUD,
//	                   MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT);
//
//	MSS_UART_set_rx_handler(&g_mss_uart1,
//	                              uart1_rx_handler,
//	                              MSS_UART_FIFO_FOURTEEN_BYTES);
//
//	UART_init(&uart1, COREUARTAPB_3_0,
//	                  650, (DATA_8_BITS | NO_PARITY));
//
//	while(1) {
//		MSS_GPIO_set_output(MSS_GPIO_13,1);
//		//tx_size = UART_fill_tx_fifo(&uart0,tx_buff,sizeof(tx_buff));
//		MSS_UART_polled_tx(&g_mss_uart1,tx_buffer,sizeof(tx_buffer));
//		delay_ms(1000);
//		MSS_GPIO_set_output(MSS_GPIO_13,0);
//		delay_ms(1000);
//	}
//}
//
//mss_i2c_slave_handler_ret_t slave_write_handler
//    (
//        mss_i2c_instance_t * this_i2c,
//        uint8_t * p_rx_data,
//        uint16_t rx_size
//    )
//{
//	if(rx_size > BUFFER_SIZE) {
//		rx_size = BUFFER_SIZE;
//	}
//
//	uint8_t loop_count;
//	for(loop_count = 0;loop_count < rx_size;loop_count++){
//		databuff[loop_count] = g_slave_rx_buffer[loop_count];
//	}
//
//	return MSS_I2C_REENABLE_SLAVE_RX;
//}
//
//void i2c_test() {
//	SYSREG->WDOG_CR = 0x00000000;
//	NVIC_SetPriority(SysTick_IRQn, 0xFFu);
//	//SysTick_Config(MSS_SYS_M3_CLK_FREQ / 100);
//	uint8_t slave_addr = 0x12;
//	mss_i2c_status_t status;
//	uint8_t tx_buffer[10] = "Hello";
//	//tx_buffer[0] = 0x02;
//	MSS_I2C_init( &g_mss_i2c1, slave_addr, MSS_I2C_PCLK_DIV_960 );
//	MSS_I2C_set_slave_rx_buffer( &g_mss_i2c1, g_slave_rx_buffer,
//										 sizeof(g_slave_rx_buffer) );
//	MSS_I2C_register_write_handler( &g_mss_i2c1, slave_write_handler );
//	MSS_I2C_enable_slave( &g_mss_i2c1 );
//	MSS_I2C_init( &g_mss_i2c0, DUMMY_I2C_ADDR, MSS_I2C_PCLK_DIV_960 );
//
//
//	while( 1 )
//	{
//		MSS_I2C_write(&g_mss_i2c0, slave_addr, tx_buffer, sizeof(tx_buffer), MSS_I2C_RELEASE_BUS);
//		status = MSS_I2C_wait_complete(&g_mss_i2c0, MSS_I2C_NO_TIMEOUT);
//
//		status = 0;
//	}
//}
//
//void uart_test() {
//	GPIO_Init();
//	UART_init(&uart0, COREUARTAPB_2_0,
//	                  650, (DATA_8_BITS | NO_PARITY));
//	UART_init(&uart1, COREUARTAPB_0_0,
//	                  650, (DATA_8_BITS | NO_PARITY));
//	uint8_t tx_buff[20] = {0x30,0x35,0x80,0x90};
//	uint8_t tx_size;
//	while(1){
//		MSS_GPIO_set_output(MSS_GPIO_13,1);
//		tx_size = UART_fill_tx_fifo(&uart0,tx_buff,sizeof(tx_buff));
//		delay_ms(1000);
//		MSS_GPIO_set_output(MSS_GPIO_13,0);
//		delay_ms(1000);
//	}
//
//}

#include "memory.h"
#include "pilot.h"

partition_t payload_p,hk_p,log_p;
thermistor_pkt_t *thermistor_packet;
hk_pkt_t *hk_packet;
log_packet_t *log_packet;
uint8_t packet_data[512];
uint8_t log_data[512];
uint32_t current_time_lower,current_time_upper;
uint32_t payload_period_L,payload_period_H;
uint32_t payload_last_count_L,payload_last_count_H;
uint16_t thermistor_seq_no,logs_seq_no,hk_seq_no;
uint8_t log_count,result;
int main()
{
	Pilot_Init();
	thermistor_seq_no = 0;
	logs_seq_no = 0;
	log_count = 0;
	initialise_partition(&payload_p,PAYLOAD_BLOCK_INIT,PAYLOAD_BLOCK_END);
	initialise_partition(&hk_p,HK_BLOCK_INIT,HK_BLOCK_END);
	initialise_partition(&log_p,LOGS_BLOCK_INIT,LOGS_BLOCK_END);
	payload_period_H = PAYLOAD_PERIOD_H;
	payload_period_L = PAYLOAD_PERIOD_L;
	log_packet = (log_packet_t*)log_data;
	while(1) {
		MSS_TIM64_get_current_value(&current_time_upper,&current_time_lower);
		//Checking if it is time to take thermistor readings (must be verified)
		if((payload_last_count_H - current_time_upper > payload_period_H) || ((payload_last_count_H - current_time_upper < payload_period_H) && (payload_last_count_L - current_time_lower > payload_period_L))) {
			log_packet->logs[log_count].task_id = THERMISTOR_TASK_ID;
			log_packet->logs[log_count].time_H = current_time_upper;
			log_packet->logs[log_count].time_L = current_time_lower;
			thermistor_packet = (thermistor_pkt_t*)packet_data;
			result = get_thermistor_vals(thermistor_packet,thermistor_seq_no);
			log_packet->logs[log_count].task_status = result;
			store_data(&payload_p,packet_data);
			thermistor_seq_no++;
			log_count++;
		}

		//If 10 log entries have been recorded, write the logs to the SD card and reset the log counter
		if(log_count == 10) {
			log_packet->APID = LOGS_API_ID;
			log_packet->PL = LOGS_PKT_LENGTH;
			log_packet->Version_ID = LOGS_Version_ID;
			log_packet->Seq_no = logs_seq_no;
			log_packet->Fletcher_Code = LOGS_FLETCHER_CODE;
			store_data(&log_p,log_data);
			log_count = 0;
		}





	}

}
