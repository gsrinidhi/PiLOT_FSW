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
#define DEBUG_ON 		0
#include "memory.h"
#include "pilot.h"

partition_t payload_p,hk_p,log_p, sd_hk_p;
thermistor_pkt_t *thermistor_packet;
hk_pkt_t *hk_packet;
SD_HK_pkt_t *sd_hk_packet;
log_packet_t *log_packet;
cmd_packet_t *cmd;
uint8_t packet_data[512];
uint8_t log_data[512];
uint32_t current_time_lower,current_time_upper;
uint32_t payload_period_L,payload_period_H,hk_period_H,hk_period_L,sd_hk_period_L,sd_hk_period_H;
uint32_t payload_last_count_L,payload_last_count_H,hk_last_count_H,hk_last_count_L,sd_hk_last_count_L,sd_hk_last_count_H;
uint16_t thermistor_seq_no,logs_seq_no,hk_seq_no,sd_hk_seq_no;
uint8_t log_count,result_global,api_id,sd_state;

void uart1_rx_handler(mss_uart_instance_t * this_uart) {
	uint8_t rx_buffer[3],size;
	size = MSS_UART_get_rx(this_uart,rx_buffer,1);
	if(rx_buffer[0] == PSLV_ADDR) {

	}else {

	}
}

uint8_t downlink_sd(partition_t *p,uint8_t size) {
	uint8_t result;
	result = read_data(p,packet_data);
	MSS_GPIO_set_output(EN_UART,1);
	MSS_UART_polled_tx(&g_mss_uart1,packet_data,size);
	MSS_GPIO_set_output(EN_UART,0);
	return result;
}

uint8_t downlink(uint8_t *data,uint8_t size) {
		//MSS_GPIO_set_output(EN_UART,1);
		uint8_t k = 0;
		//MSS_UART_set_tx_endian(&g_mss_uart1, MSS_UART_BIGEND);
		MSS_UART_polled_tx(&g_mss_uart1,data,size);
		//MSS_GPIO_set_output(EN_UART,0);
//	char ping[] = "pingpingpingpingpingpingpingpingpingpingpingpingpingping\n";
//	uint8_t tx[] = {10};
//	MSS_UART_polled_tx(&g_mss_uart1,(uint8_t*)tx,1);
//	MSS_UART_polled_tx(&g_mss_uart1,(uint8_t*)ping,58);
		return 0;
}

uint8_t command() {
	uint8_t size = 0,opcode = 0,result;
	MSS_GPIO_set_output(EN_UART,0);
	uint32_t *period_L,*period_H,rate,rate2;
	partition_t *part;
	size = MSS_UART_get_rx(&g_mss_uart1,packet_data,CMD_PKT_LENGTH);
	if(size == 0) {
		return 0;
	} else {
		cmd = (cmd_packet_t*)packet_data;
		opcode = cmd->cmd_opcaode;

		switch(opcode){
		case 0x01:
			break;
		case 0x02:
			break;
		case 0x03:
			if(cmd->cmd_arg[0] == THERMISTOR_API_ID) {
				period_L = &payload_period_L;
				period_H = &payload_period_H;
			}else if(cmd->cmd_arg[0] == HK_API_ID) {
				period_L = &hk_period_L;
				period_H = &hk_period_H;
			}
			if(cmd->cmd_arg[1] == RATE_ONE_SPP) {
				rate = ONE_SPP_RATE;
				result = 1;
			}else if(cmd->cmd_arg[1] == RATE_TWO_SPP) {
				rate = TWO_SPP_RATE;
				result = 1;
			}else if(cmd->cmd_arg[1] == RATE_FIVE_SPP) {
				rate = FIVE_SPP_RATE;
				result = 1;
			}else if(cmd->cmd_arg[1] == RATE_TEN_SPP) {
				rate = TEN_SPP_RATE;
				result = 1;
			}else {
				rate = DEFAULT_PAYLOAD_PERIOD;
				result = 2;
			}
			time_to_count(rate,period_H,period_L);
			break;
		case 0x04:
			break;
		case 0x05:
			if(cmd->cmd_arg[0] == HK_PARTITION) {
				part = &hk_p;
				rate = HK_BLOCK_INIT;
				rate2 = HK_BLOCK_END;
				result = 1;
			}else if(cmd->cmd_arg[0] == PAYLOAD_PARTITION) {
				part = &payload_p;
				rate = PAYLOAD_BLOCK_INIT;
				rate2 = PAYLOAD_BLOCK_END;
				result = 1;
			}else if(cmd->cmd_arg[0] == SD_PARTITION) {
				part = &sd_hk_p;
				rate = SD_BLOCK_INIT;
				rate2 = SD_BLOCK_END;
				result = 1;
			}else if(cmd->cmd_arg[0] == LOGS_PARTITION) {
				part = &log_p;
				rate = LOGS_BLOCK_INIT;
				rate2 = LOGS_BLOCK_END;
				result = 1;
			}else {
				result = 2;
			}
			initialise_partition(part,rate,rate2);
			break;
		case 0x06:
			break;
		default:
			result = 2;
		}
	}
	return result;

}
uint8_t Flags_Init() {
	time_to_count(DEFAULT_HK_PERIOD,&hk_period_H,&hk_period_L);
	time_to_count(DEFAULT_PAYLOAD_PERIOD,&payload_period_H,&payload_period_L);
	time_to_count(FIVE_SPP_RATE,&sd_hk_period_H,&sd_hk_period_L);
	thermistor_seq_no = 1;
	hk_seq_no = 1;
	logs_seq_no = 1;
	log_count = 0;
	sd_hk_seq_no = 1;
	initialise_partition(&payload_p,PAYLOAD_BLOCK_INIT,PAYLOAD_BLOCK_END);
	initialise_partition(&hk_p,HK_BLOCK_INIT,HK_BLOCK_END);
	initialise_partition(&log_p,LOGS_BLOCK_INIT,LOGS_BLOCK_END);
	initialise_partition(&sd_hk_p,SD_BLOCK_INIT,SD_BLOCK_END);
	hk_last_count_H = 0xFFFFFFFF;
	hk_last_count_L = 0xFFFFFFFF;
	payload_last_count_H = 0xFFFFFFFF;
	payload_last_count_L = 0xFFFFFFFF;
	sd_hk_last_count_H = 0xFFFFFFFF;
	sd_hk_last_count_L = 0xFFFFFFFF;
//    MSS_UART_set_rx_handler(&g_mss_uart1,
//                            uart1_rx_handler,
//                            MSS_UART_FIFO_SINGLE_BYTE);
	return 0;
}

uint8_t get_sd_hk(SD_HK_pkt_t *sd_hk_pkt, uint16_t seq_no){

   sd_hk_pkt->ccsds_p1 = PILOT_REVERSE_BYTE_ORDER(ccsds_p1(tlm_pkt_type, SD_HK_API_ID));
   sd_hk_pkt->ccsds_p2 = PILOT_REVERSE_BYTE_ORDER(ccsds_p2(seq_no));
   sd_hk_pkt->ccsds_p3 = PILOT_REVERSE_BYTE_ORDER(ccsds_p3(SD_HK_PKT_LENGTH-7));

   sd_hk_pkt->Thermistor_Read_Pointer = payload_p.read_pointer;
   sd_hk_pkt->Thermistor_Write_Pointer = payload_p.write_pointer;

   sd_hk_pkt->HK_Read_Pointer = hk_p.read_pointer;
   sd_hk_pkt->HK_Write_Pointer = hk_p.write_pointer;

   sd_hk_pkt->Logs_Read_Pointer = log_p.read_pointer;
   sd_hk_pkt->Logs_Write_Pointer = log_p.write_pointer;

   sd_hk_pkt->SD_Test_Read_Pointer = sd_hk_p.read_pointer;
   sd_hk_pkt->SD_Test_Write_Pointer = sd_hk_p.write_pointer;

   sd_hk_pkt->Fletcher_Code = SD_HK_FLETCHER_CODE;

   return 0;
}

#if DEBUG_ON == 1
#define DEBUG_UART                  &g_mss_uart1
#define DEBUG_UART_PRINT            MSS_UART_polled_tx
#define DEBUG_UART_IP               MSS_UART_get_rx
#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>
#include<string.h>
char null_print[] = "Trying to print null";
void print(uint8_t *str,uint8_t size) {
	DEBUG_UART_PRINT(DEBUG_UART,(uint8_t*)str,size);

}
uint8_t k = 0;
void disp_hk_pkt(hk_pkt_t *pkt) {
//	print("Hello\n");
//	print("CCSDS P1: ");
//	print((char*)&pkt->ccsds_p1);
//	print("\n");
//	print("CCSDS P2: ");
//	print((char*)&pkt->ccsds_p2);
//	print("\n");
//	print("CCSDS P3: ");
//	print((char*)&pkt->ccsds_p3);
//	print("\n");
	k = 0x50;
	print(&k,1);
	k = pkt->CDH_Periph_Status;
	print(&k,1);
}
#endif


int main()
{
	result_global = Pilot_Init();
	sd_state = result_global & 0x1;
	Flags_Init();
	log_packet = (log_packet_t*)log_data;
	MSS_GPIO_set_output(EN_UART,1);
	while(1) {
		//result_global = command();
		MSS_TIM64_get_current_value(&current_time_upper,&current_time_lower);
		//Checking if it is time to take thermistor readings (must be verified)
		if((payload_last_count_H - current_time_upper >= payload_period_H) && (payload_last_count_L - current_time_lower >= payload_period_L)) {
			log_packet->logs[log_count].task_id = THERMISTOR_TASK_ID;
			log_packet->logs[log_count].time_H = current_time_upper;
			log_packet->logs[log_count].time_L = current_time_lower;
			thermistor_packet = (thermistor_pkt_t*)packet_data;
			result_global = get_thermistor_vals(thermistor_packet,thermistor_seq_no);
			log_packet->logs[log_count].task_status = result_global;
			if(sd_state == 0) {
				store_data(&payload_p,packet_data);
				result_global = downlink_sd(&payload_p,THERMISTOR_PKT_LENGTH);
			} else {
				downlink(packet_data,THERMISTOR_PKT_LENGTH);
			}
			//
			thermistor_seq_no++;
			log_count++;
			payload_last_count_H = current_time_upper;
			payload_last_count_L = current_time_lower;
		}

		//If 10 log entries have been recorded, write the logs to the SD card and reset the log counter
		if(log_count >= 10) {
			log_packet->ccsds_p1 = ccsds_p1(tlm_pkt_type,LOGS_API_ID);
			log_packet->ccsds_p2 = ccsds_p2((logs_seq_no));
			log_packet->ccsds_p3 = ccsds_p3(LOGS_PKT_LENGTH);
			log_packet->Fletcher_Code = LOGS_FLETCHER_CODE;
			store_data(&log_p,log_data);
			log_count = 0;
		}

		// For HK Packet
		if((hk_last_count_H - current_time_upper >= hk_period_H) && (hk_last_count_L - current_time_lower >= hk_period_L)) {
            log_packet->logs[log_count].task_id = HK_TASK_ID;
            log_packet->logs[log_count].time_H = current_time_upper;
            log_packet->logs[log_count].time_L = current_time_lower;
            hk_packet = (hk_pkt_t*)packet_data;
            result_global = get_hk(hk_packet,hk_seq_no,sd_state);
            log_packet->logs[log_count].task_status = result_global;
            if(sd_state == 0) {
            	store_data(&hk_p,packet_data);
            	downlink_sd(&hk_p,HK_PKT_LENGTH);
            }else {
            	downlink(packet_data,HK_PKT_LENGTH);
            }
            hk_seq_no++;
            log_count++;
            hk_last_count_H = current_time_upper;
            hk_last_count_L = current_time_lower;
		 }

		//If 10 log entries have been recorded, write the logs to the SD card and reset the log counter
		if(log_count >= 10) {
			log_packet->ccsds_p1 = ccsds_p1(tlm_pkt_type,LOGS_API_ID);
			log_packet->ccsds_p2 = ccsds_p2((logs_seq_no));
			log_packet->ccsds_p3 = ccsds_p3(LOGS_PKT_LENGTH);
			log_packet->Fletcher_Code = LOGS_FLETCHER_CODE;
			store_data(&log_p,log_data);
			log_count = 0;
		}

		//For SD_HK Packet
		if((sd_hk_last_count_H - current_time_upper >= sd_hk_period_H) && (sd_hk_last_count_L - current_time_lower >= sd_hk_period_L)) {
            log_packet->logs[log_count].task_id = SD_HK_TASK_ID;
            log_packet->logs[log_count].time_H = current_time_upper;
            log_packet->logs[log_count].time_L = current_time_lower;
            sd_hk_packet = (SD_HK_pkt_t*)packet_data;
            result_global = get_sd_hk(sd_hk_packet,sd_hk_seq_no);
            log_packet->logs[log_count].task_status = result_global;
            if(sd_state == 0) {
                store_data(&sd_hk_p,packet_data);
                result_global = downlink_sd(&sd_hk_p,SD_HK_PKT_LENGTH);
            } else {
            	downlink(packet_data,SD_HK_PKT_LENGTH);
            }

            sd_hk_seq_no++;
            log_count++;
            sd_hk_last_count_H = current_time_upper;
            sd_hk_last_count_L = current_time_lower;
		 }


		//If 10 log entries have been recorded, write the logs to the SD card and reset the log counter
		if(log_count >= 10) {
			log_packet->ccsds_p1 = ccsds_p1(tlm_pkt_type,LOGS_API_ID);
			log_packet->ccsds_p2 = ccsds_p2((logs_seq_no));
			log_packet->ccsds_p3 = ccsds_p3(LOGS_PKT_LENGTH);
			log_packet->Fletcher_Code = LOGS_FLETCHER_CODE;
			store_data(&log_p,log_data);
			log_count = 0;
		}
	}
//	while(1) {
//		uint8_t tx[] = {0x01};
//		MSS_UART_polled_tx(&g_mss_uart1,tx,1);
//	}







}
