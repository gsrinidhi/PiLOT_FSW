/**
 * @file 	:			main.c
 * @author 	:			Srinidhi G, Pratik Aher
 * @brief 	:			The main() in this file is the entry point of the application code for the PiLOT Flight Software.
 * @version :			1.0
 * @date 	:			2023-01-23
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "memory.h"
#include "pilot.h"
#include "peripherals.h"
/**
 * @brief All the required partitions are declared below
 * payload_p	:		partition for thermistor data
 * hk_p			:		partition for housekeeping data
 * log_p		:		partition for logs
 * aris_p		: 		partitons for aris data
 * 
 */
partition_t payload_p,hk_p,log_p, sd_hk_p,aris_p;

/**
 * @brief The packet data variable is used to store the housekeeping packets and payload packets. The same variable can be used as the data from housekeeping and payload are not generated simultaneously.
 * 
 */
uint8_t packet_data[512];

/**
 * @brief The log_data variable stores the logs generated. Each log consists of the task id, task start time and task result
 * 
 */
uint8_t log_data[512];

/**
 * @brief The aris_packet_data stores the aris data generated.
 * 
 */
uint8_t aris_packet_data[512];

/**
 * @brief The pslv_queue is the queue from which data is provided to the pslv stage 4 data processing unit
 * 
 */
uint8_t pslv_queue[2048];

/**
 * @brief A pointer to the thermistor packet type. Payload data is stored in packet_data using this pointer
 * 
 */
thermistor_pkt_t *thermistor_packet;
/**
 * @brief A pointer to the housekeeping packet type. Housekeeping data is stored in packet_data using this pointer
 * 
 */
hk_pkt_t *hk_packet;

/**
 * @brief A pointer to the log packet type. Log data are stored in log_data using this pointer.
 * 
 */
log_packet_t *log_packet;

/**
 * @brief A pointer to the aris packet type. Aris data is stored in aris_packet_data using this pointer.
 * 
 */
aris_pkt_t *aris_packet;

/**
 * @brief These two variable hold the current counts of the 64 bit counter
 * 
 */
uint32_t current_time_lower,current_time_upper;

/**
 * @brief These variables hold the upper and lower periods of all the tasks
 * 
 */
uint32_t payload_period_L,payload_period_H,hk_period_H,hk_period_L,sd_hk_period_L,sd_hk_period_H,aris_period_L,aris_period_H,sd_dump_period_L,sd_dump_period_H;

/**
 * @brief These variables are used to keep track of the last counts at which a task was run
 * 
 */
uint32_t payload_last_count_L,payload_last_count_H,hk_last_count_H,hk_last_count_L,sd_hk_last_count_L,sd_hk_last_count_H,aris_last_count_L,aris_last_count_H,sd_dump_last_count_L,sd_dump_last_count_H;

/**
 * @brief These varibles are used to hold the sequence numbers of all the packets
 * 
 */
uint16_t thermistor_seq_no,logs_seq_no,hk_seq_no,sd_hk_seq_no,aris_seq_no;

/**
 * @brief These variables are used for queue operations
 * 
 */
uint16_t q_head,q_tail,q_in_i;

/**
 * @brief Miscellaneous variables used to keep track of various counts and states
 * 
 */
uint8_t log_count,result_global,api_id,sd_state,aris_sample_no;
uint8_t uart_irq_rx_buffer[3],uart_irq_tx_buffer[2];
uint8_t uart_irq_size;
uint8_t sd_count;
volatile uint8_t uart_irq_addr_flag;

/**
 * @brief This is to be used only if the packets are to be sent over uart as they are formed and not from the queue. This is only for testing purposes.
 * 
 * @param data The data to be sent
 * @param size The number of bytes to be sent
 * @return uint8_t returns 0
 */
uint8_t downlink(uint8_t *data,uint8_t size) {
		MSS_GPIO_set_output(EN_UART,LOGIC_HIGH);
		MSS_UART_polled_tx(&g_mss_uart1,data,size);
		MSS_GPIO_set_output(EN_UART,LOGIC_LOW);
		return 0;
}

/**
 * @brief This is the interrupt handler which gets triggered whenever a byte is received from the UART Rx line. This function first checks if the receved byte is an address byte by reading the PE bit of the Line Status Register (LSR) of MSS UART 1. Then it checks if the received address is that of PiLOT. If the address matches, two bytes of data are transmitted from the pslv_queue
 * 
 * @param this_uart : The uart from which the interrupt was triggered.
 */
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
		if(((q_tail+2)%2048) == q_head) {
			pslv_queue[q_tail] = 0xFF;
			pslv_queue[q_tail+1] = 0xFF;
		} else {
			q_tail = ((q_tail+2)%2048);
		}
	}

}

/**
 * @brief This function populates the read and write pointer fields of the housekeeping data packet.
 * 
 * @param hk_pkt 	:	The pointer to the housekeeping packet data
 * @return uint8_t 
 */
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

/**
 * @brief This function initialises all the global variables in this file.
 * 
 * @return uint8_t 
 */
uint8_t Flags_Init() {
	/**
	 * @brief Initialise the period upper and lower counters for all tasks
	 * 
	 */
	time_to_count(DEFAULT_HK_PERIOD,&hk_period_H,&hk_period_L);
	time_to_count(DEFAULT_PAYLOAD_PERIOD,&payload_period_H,&payload_period_L);
	time_to_count(600000,&sd_hk_period_H,&sd_hk_period_L);
	time_to_count(10,&aris_period_H,&aris_period_L);
	time_to_count(300000,&sd_dump_period_H,&sd_dump_period_L);
	/**
	 * @brief Initialise all sequence numbers to one
	 * 
	 */
	
	thermistor_seq_no = 1;
	hk_seq_no = 1;
	logs_seq_no = 1;
	log_count = 0;
	sd_hk_seq_no = 1;
	aris_seq_no = 1;
	/**
	 * @brief Initialise all the partitions by setting their starting and ending blocks
	 * 
	 */
	initialise_partition(&payload_p,PAYLOAD_BLOCK_INIT,PAYLOAD_BLOCK_END);
	initialise_partition(&hk_p,HK_BLOCK_INIT,HK_BLOCK_END);
	initialise_partition(&log_p,LOGS_BLOCK_INIT,LOGS_BLOCK_END);
	initialise_partition(&sd_hk_p,SD_BLOCK_INIT,SD_BLOCK_END);
	initialise_partition(&aris_p,ARIS_BLOCK_INIT,ARIS_BLOCK_END);
	/**
	 * @brief Initialise all the last counts to the maximum value of the counter
	 * 
	 */
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
	/**
	 * @brief Set the MSS UART 1 interrupt handler to the one defined above
	 * 
	 */
    MSS_UART_set_rx_handler(&g_mss_uart1,
                            uart1_rx_handler,
                            MSS_UART_FIFO_SINGLE_BYTE);

	/**
	 * @brief Initialise the queue variables. The queue is empty when the q_tail is two units behind the q_head. The queue is full when the q_head and q_tail are equal
	 * 
	 */
    q_head = 2;
    q_tail = 0;
    sd_count = SD_TIMEOUT;
	return 0;
}

/**
 * @brief This function adds the given data to the queue if the required number of bytes are free in the queue. Otherwise the data is written to the SD card.
 * 
 * @param size	:	The size of the data to be written
 * @param p 	: 	The partiton of the sd card to which the data must be written in case the queue is full.
 * @param data 	:	The data to be written
 */
void add_to_queue(uint8_t size,partition_t *p,uint8_t *data) {
	q_in_i = 0;
	if((q_head > q_tail && (2048 - q_head + q_tail) >= size) || (q_head < q_tail && (q_tail - q_head) >= size)) {
		for(;q_in_i<size;q_in_i+=2) {
			pslv_queue[q_head] = data[q_in_i];
			pslv_queue[q_head+1] = data[q_in_i+1];
			q_head+=2;
			if(q_head >= 2048) {
				//q_head reached limit
				q_head = 0;
			}
		}
	} else if(sd_state == 0x7) {
		result_global = store_data(p,data);
		if(result_global == 1) {
			sd_state = 0x0;
		}
	}
}

void add_to_queue_from_sd(uint8_t size,partition_t *p,uint8_t *data) {
	q_in_i = 0;
	if(sd_state == 0x7) {
		result_global = read_data(p,data);
		while(!((q_head > q_tail && (2048 - q_head + q_tail) >= size) || (q_head < q_tail && (q_tail - q_head) >= size)));
		if((q_head > q_tail && (2048 - q_head + q_tail) >= size) || (q_head < q_tail && (q_tail - q_head) >= size)) {
			for(;q_in_i<size;q_in_i+=2) {
				pslv_queue[q_head] = data[q_in_i];
				pslv_queue[q_head+1] = data[q_in_i+1];
				q_head+=2;
				if(q_head >= 2048) {
					//q_head reached limit
					q_head = 0;
				}
			}
		}
	}


}
int main()
{
	//Initialise all the peripherals and devices connected to the OBC in PiLOT
	result_global = Pilot_Init();
	sd_state = result_global & 0x1;
	sd_state = !sd_state;
	if(sd_state == 1){
		sd_state = 0x7;
	}
	//Initialise all the global variables in main.c
	Flags_Init();
	log_packet = (log_packet_t*)log_data;
	while(1) {
		//Reload the watchdog counter
		MSS_WD_reload();
		//Get the current values of the 64 bit counter
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

		//If 10 log entries have been recorded, write the logs to the queue and reset the log counter
		if(log_count >= 10) {
			log_packet->ccsds_p1 = PILOT_REVERSE_BYTE_ORDER(ccsds_p1(tlm_pkt_type,LOGS_API_ID));
			log_packet->ccsds_p2 = PILOT_REVERSE_BYTE_ORDER(ccsds_p2((logs_seq_no)));
			log_packet->ccsds_p3 = PILOT_REVERSE_BYTE_ORDER(ccsds_p3(LOGS_PKT_LENGTH));
			log_packet->Fletcher_Code = LOGS_FLETCHER_CODE;
			//MSS_UART_disable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
			add_to_queue(LOGS_PKT_LENGTH,&log_p,log_data);
			//MSS_UART_enable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
			log_count = 0;
			logs_seq_no++;
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
			logs_seq_no++;
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
			logs_seq_no++;
		}

		if((sd_hk_last_count_H - current_time_upper >= sd_hk_period_H) && (sd_hk_last_count_L - current_time_lower >= sd_hk_period_L)) {
			result_global = sd_status(&sd_state,packet_data);
			sd_hk_last_count_H = current_time_upper;
			sd_hk_last_count_L = current_time_lower;
			if(sd_state == 0x8 && sd_count == SD_TIMEOUT) {
				sd_count = SD_TIMEOUT - 1;
			}
			if(sd_count > 0 && sd_state == 0x9) {
				sd_count--;
			}else if(sd_state == 0x9){
				sd_state = 0;
				sd_count = SD_TIMEOUT;
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
