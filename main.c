#define DEBUG_ON 		0
#include "memory.h"
#include "pilot.h"
partition_t payload_p,hk_p,log_p, sd_hk_p,aris_p;
thermistor_pkt_t *thermistor_packet;
hk_pkt_t *hk_packet;
SD_HK_pkt_t *sd_hk_packet;
log_packet_t *log_packet;
cmd_packet_t *cmd;
aris_pkt_t *aris_packet;
uint8_t packet_data[512];
uint8_t downlink_data[512];
uint8_t log_data[512];
uint8_t aris_packet_data[512];
uint32_t current_time_lower,current_time_upper;
uint32_t payload_period_L,payload_period_H,hk_period_H,hk_period_L,sd_hk_period_L,sd_hk_period_H,aris_period_L,aris_period_H;
uint32_t payload_last_count_L,payload_last_count_H,hk_last_count_H,hk_last_count_L,sd_hk_last_count_L,sd_hk_last_count_H,aris_last_count_L,aris_last_count_H;
uint16_t thermistor_seq_no,logs_seq_no,hk_seq_no,sd_hk_seq_no,aris_seq_no,downlink_data_count,current_location;
uint8_t log_count,result_global,api_id,sd_state,downlink_flag,downlink_packet_size,uart_state,aris_sample_no;
//volatile uint8_t addr_flag = ((&g_mss_uart1)->hw_reg->LSR);
uint8_t uart_irq_rx_buffer[3],uart_irq_tx_buffer[2];
uint8_t uart_irq_size;
pslv_queue_t pslv_queue;
uint8_t queue_head,queue_tail;
volatile uint8_t uart_irq_addr_flag;

uint8_t downlink(uint8_t *data,uint8_t size) {
		MSS_GPIO_set_output(EN_UART,LOGIC_HIGH);
		MSS_UART_polled_tx(&g_mss_uart1,data,size);
		MSS_GPIO_set_output(EN_UART,LOGIC_LOW);
		return 0;
}
void uart1_rx_handler(mss_uart_instance_t * this_uart) {
	uart_irq_addr_flag = (&g_mss_uart1)->hw_reg->LSR;
	uart_irq_size = MSS_UART_get_rx(this_uart,uart_irq_rx_buffer,1);

	if(read_bit_reg8(&uart_irq_addr_flag,PE)) {
		if(uart_irq_rx_buffer[0] == PSLV_TO_PILOT_ADDR) {
			MSS_GPIO_set_output(EN_UART,LOGIC_HIGH);
			//If we have to send data
//			if(downlink_data_count < (downlink_packet_size-1)) {
//				uart_irq_rx_buffer[LOWER_BYTE] = downlink_data[downlink_data_count];
//				uart_irq_rx_buffer[UPPER_BYTE] = downlink_data[downlink_data_count+1];
//				downlink(uart_irq_rx_buffer,2);
//				downlink_data_count+=2;
//			}else {
//				//Reset downlink flag
//				downlink_flag = 0;
//			}
			MSS_UART_polled_tx(&g_mss_uart1,uart_irq_tx_buffer,2);
		    while(!(MSS_UART_TEMT & MSS_UART_get_tx_status(&g_mss_uart1)))
		    {
		        ;
		    }
			MSS_GPIO_set_output(EN_UART,LOGIC_LOW);

		}

		else {

		}
	}

}



uint8_t Flags_Init() {
	time_to_count(DEFAULT_HK_PERIOD,&hk_period_H,&hk_period_L);
	time_to_count(DEFAULT_PAYLOAD_PERIOD,&payload_period_H,&payload_period_L);
	time_to_count(FIVE_SPP_RATE,&sd_hk_period_H,&sd_hk_period_L);
	time_to_count(10,&aris_period_H,&aris_period_L);
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
	aris_last_count_H = 0xFFFFFFFF;
	aris_last_count_L = 0xFFFFFFFF;
    MSS_UART_set_rx_handler(&g_mss_uart1,
                            uart1_rx_handler,
                            MSS_UART_FIFO_SINGLE_BYTE);
    uart_irq_rx_buffer[1] = 0x44;
    uart_irq_rx_buffer[2] = 0x44;
    uart_irq_tx_buffer[0] = 0x88;
    uart_irq_tx_buffer[1] = 0x44;
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

void FabricIrq10_IRQHandler(void)
{
    uint8_t demon = 1;
}

int main()
{
	result_global = Pilot_Init();
	sd_state = result_global & 0x1;
	Flags_Init();
	log_packet = (log_packet_t*)log_data;
	timer_instance_t time_count;
	NVIC_EnableIRQ(FabricIrq10_IRQn);
	TMR_init(&time_count, CORETIMER_0_0, TMR_CONTINUOUS_MODE, PRESCALER_DIV_2, 0xEE6B280);
	TMR_enable_int(&time_count);
//	TMR_start(&time_count);
	uint32_t curr_val;
	while(1){
		curr_val = TMR_current_value(&time_count);
	}

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
//			if(sd_state == 0) {
//				store_data(&payload_p,packet_data);
//				result_global = downlink_sd(&payload_p,THERMISTOR_PKT_LENGTH);
//			} else {
//				downlink(packet_data,THERMISTOR_PKT_LENGTH);
//			}
			uint8_t kl = sizeof(thermistor_pkt_t);
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
			TMR_start(&time_count);
            log_packet->logs[log_count].task_id = HK_TASK_ID;
            log_packet->logs[log_count].time_H = current_time_upper;
            log_packet->logs[log_count].time_L = current_time_lower;
            hk_packet = (hk_pkt_t*)packet_data;
            result_global = get_hk(hk_packet,hk_seq_no,&sd_state);
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

            TMR_stop(&time_count);

			curr_val = TMR_current_value(&time_count);
			uint8_t dummy = 10;
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
//            if(sd_state == 0) {
//                store_data(&sd_hk_p,packet_data);
//                result_global = downlink_sd(&sd_hk_p,SD_HK_PKT_LENGTH);
//            } else {
//            	downlink(packet_data,SD_HK_PKT_LENGTH);
//            }

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

		if(aris_sample_no >= 20) {
			//Form Aris packet and store in sd card
			aris_packet = (aris_pkt_t*)aris_packet_data;
			aris_packet->ccsds_p1 = ccsds_p1(tlm_pkt_type,ARIS_API_ID);
			aris_packet->ccsds_p2 = ccsds_p2(aris_seq_no);
			aris_packet->ccsds_p3 = ccsds_p3(ARIS_PKT_LENGTH);
			aris_packet->Fletcher_Code = ARIS_FLETCHER_CODE;
			store_data(&aris_p,aris_packet_data);
			//Reset sample count
			aris_sample_no = 0;
		}

		if((aris_last_count_H - current_time_upper >= aris_period_H) && (aris_last_count_L - current_time_lower >= aris_period_L)) {
			log_packet->logs[log_count].task_id = ARIS_TASK_ID;
			log_packet->logs[log_count].time_H = current_time_upper;
			log_packet->logs[log_count].time_L = current_time_lower;
			aris_packet = (aris_pkt_t*)aris_packet_data;
			if(aris_sample_no == 0) {
				aris_packet->start_time = current_time_lower;
			}
//			result_global = get_aris_vals(aris_packet,aris_seq_no);
			result_global = get_aris_sample(aris_packet,(current_time_lower & 0xFFFF),current_location,aris_sample_no);
			log_packet->logs[log_count].task_status = result_global;
//			downlink(packet_data,ARIS_PKT_LENGTH);
			uint8_t ka = sizeof(aris_pkt_t);
//			aris_seq_no++;
			aris_sample_no++;
			log_count++;
			aris_last_count_H = current_time_upper;
			aris_last_count_L = current_time_lower;
		}
	}

//	while(1) {
//		uint8_t tx[] = {0x03};
//		MSS_UART_polled_tx(&g_mss_uart1,tx,1);
//	}
}
