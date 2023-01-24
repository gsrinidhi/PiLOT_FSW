#include "memory.h"
#include "pilot.h"
#include "peripherals.h"
partition_t payload_p,hk_p,log_p, sd_hk_p,aris_p;
thermistor_pkt_t *thermistor_packet;
hk_pkt_t *hk_packet;
log_packet_t *log_packet;
aris_pkt_t *aris_packet;
uint8_t packet_data[512];
uint8_t downlink_data[512];
uint8_t log_data[512];
uint8_t aris_packet_data[512];
uint8_t pslv_queue[1024];
uint32_t wd_value;
uint32_t current_time_lower,current_time_upper;
uint32_t payload_period_L,payload_period_H,hk_period_H,hk_period_L,sd_hk_period_L,sd_hk_period_H,aris_period_L,aris_period_H,sd_dump_period_L,sd_dump_period_H;
uint32_t payload_last_count_L,payload_last_count_H,hk_last_count_H,hk_last_count_L,sd_hk_last_count_L,sd_hk_last_count_H,aris_last_count_L,aris_last_count_H,sd_dump_last_count_L,sd_dump_last_count_H;
uint16_t thermistor_seq_no,logs_seq_no,hk_seq_no,sd_hk_seq_no,aris_seq_no,downlink_data_count,current_location,q_head,q_tail,q_in_i;
uint8_t log_count,result_global,api_id,sd_state,downlink_flag,downlink_packet_size,uart_state,aris_sample_no;
uint8_t uart_irq_rx_buffer[3],uart_irq_tx_buffer[2];
uint8_t uart_irq_size;
uint8_t queue_head,queue_tail,q_empty;
uint8_t sd_count;
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

	if(read_bit_reg8(&uart_irq_addr_flag,PE) && uart_irq_rx_buffer[0] == PSLV_TO_PILOT_ADDR) {

		MSS_GPIO_set_output(EN_UART,1);
		MSS_UART_polled_tx(&g_mss_uart1,&pslv_queue[q_tail],2);
		while(!(MSS_UART_TEMT & MSS_UART_get_tx_status(&g_mss_uart1)))
		{
			;
		}
		MSS_GPIO_set_output(EN_UART,0);
		if(((q_tail+2)%1024) == q_head) {
			pslv_queue[q_tail] = 0xFF;
			pslv_queue[q_tail+1] = 0xFF;
		} else {
			q_tail = ((q_tail+2)%1024);
		}
	}

}

uint8_t get_sd_hk(hk_pkt_t *hk_pkt) {
	hk_pkt->Thermistor_Read_Pointer = payload_p.read_pointer;
    hk_pkt->Thermistor_Write_Pointer = payload_p.write_pointer;

    hk_pkt->HK_Read_Pointer = hk_p.read_pointer;
    hk_pkt->HK_Write_Pointer = hk_p.write_pointer;

    hk_pkt->Logs_Read_Pointer = log_p.read_pointer;
    hk_pkt->Logs_Write_Pointer = log_p.write_pointer;

    hk_pkt->SD_Test_Read_Pointer = sd_hk_p.read_pointer;
    hk_pkt->SD_Test_Write_Pointer = sd_hk_p.write_pointer;

    hk_pkt->ARIS_Read_Pointer = aris_p.read_pointer;
    hk_pkt->ARIS_Write_Pointer = aris_p.write_pointer;

    return 0;
}


uint8_t Flags_Init() {
	time_to_count(DEFAULT_HK_PERIOD,&hk_period_H,&hk_period_L);
	time_to_count(DEFAULT_PAYLOAD_PERIOD,&payload_period_H,&payload_period_L);
	time_to_count(FIVE_SPP_RATE,&sd_hk_period_H,&sd_hk_period_L);
	time_to_count(10,&aris_period_H,&aris_period_L);
	time_to_count(1000,&sd_dump_period_H,&sd_dump_period_L);
	thermistor_seq_no = 1;
	hk_seq_no = 1;
	logs_seq_no = 1;
	log_count = 0;
	sd_hk_seq_no = 1;
	aris_seq_no = 1;
	initialise_partition(&payload_p,PAYLOAD_BLOCK_INIT,PAYLOAD_BLOCK_END);
	initialise_partition(&hk_p,HK_BLOCK_INIT,HK_BLOCK_END);
	initialise_partition(&log_p,LOGS_BLOCK_INIT,LOGS_BLOCK_END);
	initialise_partition(&sd_hk_p,SD_BLOCK_INIT,SD_BLOCK_END);
	initialise_partition(&aris_p,ARIS_BLOCK_INIT,ARIS_BLOCK_END);
	hk_last_count_H = 0xFFFFFFFF;
	hk_last_count_L = 0xFFFFFFFF;
	payload_last_count_H = 0xFFFFFFFF;
	payload_last_count_L = 0xFFFFFFFF;
	sd_hk_last_count_H = 0xFFFFFFFF;
	sd_hk_last_count_L = 0xFFFFFFFF;
	aris_last_count_H = 0xFFFFFFFF;
	aris_last_count_L = 0xFFFFFFFF;
	sd_dump_last_count_H = 0xFFFFFFFF;
	sd_dump_last_count_L = 0xFFFFFFFF;
    MSS_UART_set_rx_handler(&g_mss_uart1,
                            uart1_rx_handler,
                            MSS_UART_FIFO_SINGLE_BYTE);
    uart_irq_rx_buffer[1] = 0x44;
    uart_irq_rx_buffer[2] = 0x44;
    uart_irq_tx_buffer[0] = 0xFF;
    uart_irq_tx_buffer[1] = 0xFF;
    q_head = 2;
    q_tail = 0;
    sd_count = 14;
	return 0;
}

void add_to_queue(uint8_t size,partition_t *p,uint8_t *data) {
	q_in_i = 0;
	if((q_head > q_tail && (1024 - q_head + q_tail) >= size) || (q_head < q_tail && (q_tail - q_head) >= size)) {
		for(;q_in_i<size;q_in_i+=2) {
			pslv_queue[q_head] = data[q_in_i];
			pslv_queue[q_head+1] = data[q_in_i+1];
			q_head+=2;
			if(q_head >= 1024) {
				//q_head reached limit
				q_head = 0;
			}
		}
	} else if(sd_state != 0) {
		store_data(p,data);
	}
}

void add_to_queue_from_sd(uint8_t size,partition_t *p,uint8_t *data) {
	q_in_i = 0;
	result_global = read_data(p,data);
	while(!((q_head > q_tail && (1024 - q_head + q_tail) >= size) || (q_head < q_tail && (q_tail - q_head) >= size)));
	if((q_head > q_tail && (1024 - q_head + q_tail) >= size) || (q_head < q_tail && (q_tail - q_head) >= size)) {
		for(;q_in_i<size;q_in_i+=2) {
			pslv_queue[q_head] = data[q_in_i];
			pslv_queue[q_head+1] = data[q_in_i+1];
			q_head+=2;
			if(q_head >= 1024) {
				//q_head reached limit
				q_head = 0;
			}
		}
	}
}
int main()
{
	result_global = Pilot_Init();
	sd_state = result_global & 0x1;
	sd_state = !sd_state;
	Flags_Init();
	log_packet = (log_packet_t*)log_data;
	while(1) {
		MSS_WD_reload();
		MSS_TIM64_get_current_value(&current_time_upper,&current_time_lower);
		//Checking if it is time to take thermistor readings (must be verified)
		if((payload_last_count_H - current_time_upper >= payload_period_H) && (payload_last_count_L - current_time_lower >= payload_period_L)) {
			log_packet->logs[log_count].task_id = THERMISTOR_TASK_ID;
			log_packet->logs[log_count].time_H = current_time_upper;
			log_packet->logs[log_count].time_L = current_time_lower;
			thermistor_packet = (thermistor_pkt_t*)packet_data;
			result_global = get_thermistor_vals(thermistor_packet,thermistor_seq_no);
			//MSS_UART_disable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
			add_to_queue(THERMISTOR_PKT_LENGTH,&payload_p,packet_data);
			//MSS_UART_enable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
			log_packet->logs[log_count].task_status = result_global;
			thermistor_seq_no++;
			log_count++;
			payload_last_count_H = current_time_upper;
			payload_last_count_L = current_time_lower;
		}

		//If 10 log entries have been recorded, write the logs to the SD card and reset the log counter
		if(log_count >= 10) {
			log_packet->ccsds_p1 = PILOT_REVERSE_BYTE_ORDER(ccsds_p1(tlm_pkt_type,LOGS_API_ID));
			log_packet->ccsds_p2 = PILOT_REVERSE_BYTE_ORDER(ccsds_p2((logs_seq_no)));
			log_packet->ccsds_p3 = PILOT_REVERSE_BYTE_ORDER(ccsds_p3(LOGS_PKT_LENGTH));
			log_packet->Fletcher_Code = LOGS_FLETCHER_CODE;
			//MSS_UART_disable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
			add_to_queue(LOGS_PKT_LENGTH,&log_p,log_data);
			//MSS_UART_enable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
			log_count = 0;
		}

		// For HK Packet

		if((hk_last_count_H - current_time_upper >= hk_period_H) && (hk_last_count_L - current_time_lower >= hk_period_L)) {
            log_packet->logs[log_count].task_id = HK_TASK_ID;
            log_packet->logs[log_count].time_H = current_time_upper;
            log_packet->logs[log_count].time_L = current_time_lower;
            hk_packet = (hk_pkt_t*)packet_data;
            result_global = get_hk(hk_packet,hk_seq_no,&sd_state);
			result_global = get_sd_hk(hk_packet);
            log_packet->logs[log_count].task_status = result_global;
			//MSS_UART_disable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
			add_to_queue(HK_PKT_LENGTH,&hk_p,packet_data);
			hk_packet->q_head = q_head;
			hk_packet->q_tail = q_tail;
			//MSS_UART_enable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
            hk_seq_no++;
            log_count++;
            hk_last_count_H = current_time_upper;
            hk_last_count_L = current_time_lower;
		 }

		//If 10 log entries have been recorded, write the logs to the SD card and reset the log counter
		if(log_count >= 10) {
			log_packet->ccsds_p1 = PILOT_REVERSE_BYTE_ORDER(ccsds_p1(tlm_pkt_type,LOGS_API_ID));
			log_packet->ccsds_p2 = PILOT_REVERSE_BYTE_ORDER(ccsds_p2((logs_seq_no)));
			log_packet->ccsds_p3 = PILOT_REVERSE_BYTE_ORDER(ccsds_p3(LOGS_PKT_LENGTH));
			log_packet->Fletcher_Code = LOGS_FLETCHER_CODE;
			//MSS_UART_disable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
			add_to_queue(LOGS_PKT_LENGTH,&log_p,log_data);
			//MSS_UART_enable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
			log_count = 0;
		}

		if(aris_sample_no >= 20) {
			//Form Aris packet and add to queue
			aris_packet = (aris_pkt_t*)aris_packet_data;
			aris_packet->ccsds_p1 = PILOT_REVERSE_BYTE_ORDER(ccsds_p1(tlm_pkt_type,ARIS_API_ID));
			aris_packet->ccsds_p2 = PILOT_REVERSE_BYTE_ORDER(ccsds_p2(aris_seq_no));
			aris_packet->ccsds_p3 = PILOT_REVERSE_BYTE_ORDER(ccsds_p3(ARIS_PKT_LENGTH));
			aris_packet->Fletcher_Code = ARIS_FLETCHER_CODE;
			//MSS_UART_disable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
			add_to_queue(ARIS_PKT_LENGTH,&aris_p,aris_packet_data);
			//MSS_UART_enable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
			//Reset sample count
			aris_sample_no = 0;
			aris_seq_no++;
		}

		if((aris_last_count_H - current_time_upper >= aris_period_H) && (aris_last_count_L - current_time_lower >= aris_period_L)) {
			log_packet->logs[log_count].task_id = ARIS_TASK_ID;
			log_packet->logs[log_count].time_H = current_time_upper;
			log_packet->logs[log_count].time_L = current_time_lower;
			aris_packet = (aris_pkt_t*)aris_packet_data;
			if(aris_sample_no == 0) {
				aris_packet->start_time = current_time_lower;
			}
			result_global = get_aris_sample(aris_packet,current_time_lower,aris_sample_no);
			log_packet->logs[log_count].task_status = result_global;
			aris_sample_no++;
			log_count++;
			aris_last_count_H = current_time_upper;
			aris_last_count_L = current_time_lower;
		}

		if(log_count >= 10) {
			log_packet->ccsds_p1 = PILOT_REVERSE_BYTE_ORDER(ccsds_p1(tlm_pkt_type,LOGS_API_ID));
			log_packet->ccsds_p2 = PILOT_REVERSE_BYTE_ORDER(ccsds_p2((logs_seq_no)));
			log_packet->ccsds_p3 = PILOT_REVERSE_BYTE_ORDER(ccsds_p3(LOGS_PKT_LENGTH));
			log_packet->Fletcher_Code = LOGS_FLETCHER_CODE;
			//MSS_UART_disable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
			add_to_queue(LOGS_PKT_LENGTH,&log_p,log_data);
			//MSS_UART_enable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
			log_count = 0;
		}

		if((sd_hk_last_count_H - current_time_upper >= sd_hk_period_H) && (sd_hk_last_count_L - current_time_lower >= sd_hk_period_L)) {
			result_global = sd_status(&sd_state,packet_data);
			sd_hk_last_count_H = current_time_upper;
			sd_hk_last_count_L = current_time_lower;
			if(sd_state == 0x8 && sd_count == 14) {
				sd_count = 13;
			}
			if(sd_count > 0 && sd_state == 0x8) {
				sd_count--;
			}else if(sd_state == 0x8){
				sd_state = 0;
				sd_count = 14;
			}

		}

		if((sd_dump_last_count_H - current_time_upper >= sd_dump_period_H) && (sd_dump_last_count_L - current_time_lower >= sd_dump_period_L)) {
			if(aris_p.read_pointer != aris_p.write_pointer) {
				add_to_queue_from_sd(ARIS_PKT_LENGTH,&aris_p,aris_packet_data);
			}
			sd_dump_last_count_H = current_time_upper;
			sd_dump_last_count_L = current_time_lower;
		}
	}
}
