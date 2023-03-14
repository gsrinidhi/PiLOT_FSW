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

#include "peripherals.h"
#include "pilot.h"
#include "memory.h"
#define MAX_COUNT		0xFFFFFFFF
#define QUEUE_BYTES		0
/**
 * @brief All the required partitions are declared below
 * payload_p	:		partition for thermistor data
 * hk_p			:		partition for housekeeping data
 * log_p		:		partition for logs
 * aris_p		: 		partitons for aris data
 * 
 */
partition_t payload_p,hk_p,log_p, sd_hk_p,aris_p,timer_p;

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
uint8_t aris_packet_data1[512],aris_packet_data2[512];

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
aris_pkt_t *aris_packet_collecting,*aris_packet_add_to_queue;

/**
 * @brief These two variable hold the current counts of the 64 bit counter
 * 
 */
uint64_t current_time_lower,current_time_upper;

/**
 * @brief These variables hold the upper and lower periods of all the tasks
 * 
 */
uint64_t payload_period_L,payload_period_H,hk_period_H,hk_period_L,sd_hk_period_L,sd_hk_period_H,aris_period_L,aris_period_H,sd_dump_period_L,sd_dump_period_H,timer_period_H,timer_period_L;

/**
 * @brief These variables are used to keep track of the last counts at which a task was run
 * 
 */
uint32_t payload_last_count_L,payload_last_count_H,hk_last_count_H,hk_last_count_L,sd_hk_last_count_L,sd_hk_last_count_H,aris_last_count_L,aris_last_count_H,sd_dump_last_count_L,sd_dump_last_count_H,timer_last_count_L,timer_last_count_H;

/**
 * @brief These varibles are used to hold the sequence numbers of all the packets
 * 
 */
uint16_t thermistor_seq_no,logs_seq_no,hk_seq_no,sd_hk_seq_no,aris_seq_no;

/**
 * @brief These variables are used for queue operations
 * 
 */
uint16_t q_head,q_tail,q_in_i,aris_miss,hk_miss,payload_miss,sd_hk_miss,logs_miss;

/**
 * @brief Miscellaneous variables used to keep track of various counts and states
 * 
 */
uint8_t log_count,result_global,api_id,sd_state,aris_sample_no,aris_result;
uint8_t uart_irq_rx_buffer[3],uart_irq_tx_buffer[2];
uint8_t uart_irq_size;
uint8_t sd_count;
sd_test sd;
uint16_t aris_count,timer_seq_no;
volatile uint8_t uart_irq_addr_flag;
uint8_t sd_fail_count;
uint16_t queue_lost;

/**
 * @brief Timer instance to collect aris data
 *
 */
timer_instance_t aris_timer;

timer_pkt sync_time;

uint64_t hk_period,current_time,payload_period,sd_hk_period,sd_dump_period,timer_period;
uint64_t hk_last_count,payload_last_count,sd_hk_last_count,sd_dump_last_count,timer_last_count;

uint16_t miss_margin,sd_hk_sample_no;

sd_hk_t sd_hk;

reset_pkt_t *check_reset,put_reset;

uint16_t aris_sample_miss,aris_reset_count;

uint8_t sensor_board_status,sensor_board_fail_count;
/**
 * @brief This is to be used only if the packets are to be sent over uart as they are formed and not from the queue. This is only for testing purposes.
 * 
 * @param data The data to be sent
 * @param size The number of bytes to be sent
 * @return uint8_t returns 0
 */
uint8_t downlink(uint8_t *data,uint8_t size) {
		MSS_GPIO_set_output(EN_UART,1);
		MSS_UART_polled_tx(&g_mss_uart1,data,size);
		MSS_GPIO_set_output(EN_UART,0);
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

void toggle_aris_pointer() {
	aris_packet_collecting = (aris_packet_collecting == (aris_pkt_t*)aris_packet_data1) ? ((aris_pkt_t*)aris_packet_data2) : ((aris_pkt_t*)aris_packet_data1);
	aris_packet_add_to_queue = (aris_packet_collecting == (aris_pkt_t*)aris_packet_data1) ? ((aris_pkt_t*)aris_packet_data2) : ((aris_pkt_t*)aris_packet_data1);
}

/**
 * @brief This function initialises all the global variables in this file.
 * 
 * @return uint8_t 
 */
uint8_t Flags_Init(uint32_t reset_count, uint8_t wd_reset) {
	/**
	 * @brief Initialise the period upper and lower counters for all tasks
	 * 
	 */
	time_to_count(DEFAULT_HK_PERIOD,&hk_period_H,&hk_period_L);
	time_to_count(DEFAULT_PAYLOAD_PERIOD,&payload_period_H,&payload_period_L);
	time_to_count(3000,&sd_hk_period_H,&sd_hk_period_L);
	time_to_count(10,&aris_period_H,&aris_period_L);
	time_to_count(60000,&sd_dump_period_H,&sd_dump_period_L);
	time_to_count(3000,&timer_period_H,&timer_period_L);
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
	initialise_partition(&timer_p,0,9);
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
    TMR_init(&aris_timer,CORETIMER_0_0,TMR_CONTINUOUS_MODE,PRESCALER_DIV_2,aris_period_L/2);
    NVIC_EnableIRQ( FabricIrq9_IRQn );
    NVIC_EnableIRQ( FabricIrq10_IRQn );
    NVIC_SetPriority(FabricIrq9_IRQn,0xFE);
    NVIC_SetPriority(FabricIrq10_IRQn,0xFD);
    //NVIC_SetPriority(UART1_IRQn,0xFD);
    TMR_enable_int(&aris_timer);
    TMR_start(&aris_timer);
	timer_seq_no = 1;
	sync_time.ccsds_p1 = PILOT_REVERSE_BYTE_ORDER(ccsds_p1(tlm_pkt_type,TIME_API_ID));
	sync_time.ccsds_p2 = PILOT_REVERSE_BYTE_ORDER(ccsds_p2(timer_seq_no));
	sync_time.ccsds_p3 = PILOT_REVERSE_BYTE_ORDER(ccsds_p3(TIME_PKT_LENGTH));
	MSS_TIM64_get_current_value(&sync_time.upper_count,&sync_time.lower_count);
	sync_time.q_head = 2;
	sync_time.q_tail = 0;
	sync_time.tail = 0xBCBC;
	sync_time.reset = 1;
	sync_time.wd_reset =(wd_reset!=0)? 1:0 ;
	sync_time.reset_count = reset_count;

	hk_last_count = 0xFFFFFFFFFFFFFFFF;
	payload_last_count = 0xFFFFFFFFFFFFFFFF;
	sd_hk_last_count = 0xFFFFFFFFFFFFFFFF;
	sd_dump_last_count = 0xFFFFFFFFFFFFFFFF;
	timer_last_count = 0xFFFFFFFFFFFFFFFF;
	hk_period = hk_period_L | (hk_period_H << 32);
	payload_period = (payload_period_L) | (payload_period_H << 32);
	sd_hk_period = (sd_hk_period_L) | (sd_hk_period_H << 32);
	sd_dump_period = (sd_dump_period_L) | (sd_dump_period_H << 32);
	timer_period = (timer_period_L) | (timer_period_H << 32);

	sd_fail_count = 0;

	//Initialise ARIS collect and add to queue pointers
	aris_packet_collecting = (aris_pkt_t*)aris_packet_data1;
	aris_packet_add_to_queue = (aris_pkt_t*)aris_packet_data2;

	aris_miss = 0;
	hk_miss = 0;
	payload_miss = 0;
	sd_hk_miss = 0;
	logs_miss = 0;

	aris_sample_miss = 0;
	aris_reset_count = 0;

	return 0;
}

/**
 * @brief This function adds the given data to the queue if the required number of bytes are free in the queue. Otherwise the data is written to the SD card.
 * 
 * @param size	:	The size of the data to be written
 * @param p 	: 	The partiton of the sd card to which the data must be written in case the queue is full.
 * @param data 	:	The data to be written
 */
void add_to_queue(uint8_t size,partition_t *p,uint8_t *data,uint16_t *miss,uint8_t task_id) {
	q_in_i = 0;
	queue_lost = 0;
	while(!((q_head > q_tail && (2048 - q_head + q_tail) >= (size * (2 - QUEUE_BYTES))) || (q_head < q_tail && (q_tail - q_head) >= (size * (2 - QUEUE_BYTES)))) && ((queue_lost++) < 500));
	MSS_UART_disable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
	if((q_head > q_tail && (2048 - q_head + q_tail) >= (size * (2 - QUEUE_BYTES))) || (q_head < q_tail && (q_tail - q_head) >= (size * (2 - QUEUE_BYTES)))) {
		for(;q_in_i<size;q_in_i+=(QUEUE_BYTES + 1)) {
			pslv_queue[q_head] = data[q_in_i];
			pslv_queue[q_head+1] = data[q_in_i + QUEUE_BYTES];
			q_head+=2;
			if(q_head >= 2048) {
				//q_head reached limit
				q_head = 0;
			}
		}
		MSS_UART_enable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
	} else {
		miss_margin = (q_head > q_tail) ? ((size * (2 - QUEUE_BYTES)) - (2048 -(q_head - q_tail))) : ((size * (2 - QUEUE_BYTES)) - (q_tail - q_head));
		MSS_UART_enable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
		(*miss)+=1;
		if(sd_state & SD_WORKING_MASK) {
			sd_state |= store_data(p,data);
			if(!(sd_state & SD_WORKING_MASK)) {
				sd_fail_count++;
				if(sd_fail_count > SD_THRESHOLD) {
					sd_fail_count = 0;
					start_sd_timer(&sd_state);
				}
			}
		}
		if(sd_hk_sample_no <20) {
			sd_hk.samples[sd_hk_sample_no].sd_state = sd_state;
			sd_hk.samples[sd_hk_sample_no].task_id = task_id;
			sd_hk.samples[sd_hk_sample_no++].miss_margin = miss_margin;
		}
	}

}

/**
 * @brief This function reads from the SD card and puts the data in the queue
 * 
 * @param size 	:	The size of data to be read from SD card
 * @param p 	:	The partition in the SD card to be read from
 * @param data 	:	Buffer to store the data from the SD card 
 */
void add_to_queue_from_sd(uint8_t size,partition_t *p,uint8_t *data) {
	q_in_i = 0;
	if(sd_state == 0x7) {//read from the SD card only if it is operational
		result_global = read_data(p,data);
		while(!((q_head > q_tail && (2048 - q_head + q_tail) >= (size * (2 - QUEUE_BYTES))) || (q_head < q_tail && (q_tail - q_head) >= (size * (2 - QUEUE_BYTES)))));
		MSS_UART_disable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
		if((q_head > q_tail && (2048 - q_head + q_tail) >= (size * (2 - QUEUE_BYTES))) || (q_head < q_tail && (q_tail - q_head) >= (size * (2 - QUEUE_BYTES)))) {
			for(;q_in_i<size;q_in_i+=(QUEUE_BYTES + 1)) {
				pslv_queue[q_head] = data[q_in_i];
				pslv_queue[q_head+1] = data[q_in_i+ QUEUE_BYTES];
				q_head+=2;
				if(q_head >= 2048) {
					//q_head reached limit
					q_head = 0;
				}
			}
		}
		MSS_UART_enable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
	}


}

/**
 * @brief: Interrupt handler for aris sample collection
 */
void FabricIrq9_IRQHandler(void) {
	TMR_clear_int(&aris_timer);
	if(aris_sample_no < 20) {
		if(aris_sample_no == 0) {
			aris_packet_collecting->start_time = current_time_upper;
			aris_packet_collecting->ccsds_s2 = current_time_lower;
			aris_packet_collecting->ccsds_s1 = current_time_upper;
		}
		aris_result = get_aris_sample(aris_packet_collecting,current_time_lower,aris_sample_no);
		if(aris_result != 0) {
			sensor_board_fail_count++;
		}
//		if(sensor_board_fail_count > 10) {
//			sensor_board_status =
//		}
		aris_sample_no++;
	} else {
		aris_sample_miss++;
	}

}
void FabricIrq10_IRQHandler(void) {
	TMR_clear_int(&sd_timer);
	sd_state = 7;
	MSS_GPIO_set_output(SD_CARD_GPIO,1);
	result_global = SD_Init();
}
void inline form_log_packet() {
	log_packet->ccsds_p1 = PILOT_REVERSE_BYTE_ORDER(ccsds_p1(tlm_pkt_type,LOGS_API_ID));
	log_packet->ccsds_p2 = PILOT_REVERSE_BYTE_ORDER(ccsds_p2((logs_seq_no)));
	log_packet->ccsds_p3 = PILOT_REVERSE_BYTE_ORDER(ccsds_p3(LOGS_PKT_LENGTH));
	log_packet->Fletcher_Code = LOGS_FLETCHER_CODE;
	log_packet->ccsds_s2 = current_time_lower;
	log_packet->ccsds_s1 = current_time_upper;
	add_to_queue(LOGS_PKT_LENGTH,&log_p,log_data,&logs_miss,LOGS_TASK_ID);
	log_count = 0;
	logs_seq_no++;
}

uint8_t inline can_run(uint64_t *period,uint64_t *last_count) {
	MSS_TIM64_get_current_value((uint32_t*)&current_time_upper,(uint32_t*)&current_time_lower);
	current_time = (current_time_lower) | (current_time_upper << 32);
	if((*last_count) - current_time > (*period)) {
		(*last_count) = current_time;
		return 1;
	}
	return 0;
}

void store_sd_pointers() {
	check_reset = (reset_pkt_t*)ENVM_RESET_PKT_ADDR;
	put_reset.reset_count = check_reset->reset_count;
	put_reset.ARIS_Read_Pointer = aris_p.read_pointer;
	put_reset.ARIS_Write_Pointer = aris_p.write_pointer;
	put_reset.HK_Read_Pointer = hk_p.read_pointer;
	put_reset.HK_Write_Pointer = hk_p.write_pointer;
	put_reset.Logs_Read_Pointer = log_p.read_pointer;
	put_reset.Logs_Write_Pointer = log_p.write_pointer;
	put_reset.SD_Test_Read_Pointer = sd_hk_p.read_pointer;
	put_reset.SD_Test_Write_Pointer = sd_hk_p.write_pointer;
	put_reset.Thermistor_Read_Pointer = payload_p.read_pointer;
	put_reset.Thermistor_Write_Pointer = payload_p.write_pointer;
	nvm_status_t nvm_status = NVM_write(ENVM_RESET_PKT_ADDR,(const uint8_t *)&put_reset,sizeof(reset_pkt_t),NVM_DO_NOT_LOCK_PAGE);

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
	envm_init(check_reset,&put_reset);
	//Initialise all the global variables in main.c
	Flags_Init(put_reset.reset_count,(result_global & WD_RESET));
	add_to_queue(TIME_PKT_LENGTH,&timer_p,(uint8_t*)&sync_time,&payload_miss,TIMER_TASK_ID);
	log_packet = (log_packet_t*)log_data;
	while(1) {
		//Reload the watchdog counter
		MSS_WD_reload();
		//Checking if it is time to take thermistor readings (must be verified)
		if(can_run(&payload_period,&payload_last_count)) {
			log_packet->logs[log_count].task_id = THERMISTOR_TASK_ID;
			log_packet->logs[log_count].time_H = current_time_upper;
			log_packet->logs[log_count].time_L = current_time_lower;
			thermistor_packet = (thermistor_pkt_t*)packet_data;
			result_global = get_thermistor_vals(thermistor_packet,thermistor_seq_no);
			thermistor_packet->ccsds_s2 = current_time_lower;
			thermistor_packet->ccsds_s1 = current_time_upper;
			add_to_queue(THERMISTOR_PKT_LENGTH,&payload_p,packet_data,&hk_miss,THERMISTOR_TASK_ID);
			log_packet->logs[log_count].task_status = result_global;
			thermistor_seq_no++;
			log_count++;
			payload_last_count_H = current_time_upper;
			payload_last_count_L = current_time_lower;
		}

		//If 10 log entries have been recorded, write the logs to the queue and reset the log counter
		if(log_count >= 10) {
			form_log_packet();
		}

		// For HK Packet

		//if((hk_last_count_H - current_time_upper >= hk_period_H) && (hk_last_count_L - current_time_lower >= hk_period_L)) {
		if(can_run(&hk_period,&hk_last_count)) {
            log_packet->logs[log_count].task_id = HK_TASK_ID;
            log_packet->logs[log_count].time_H = current_time_upper;
            log_packet->logs[log_count].time_L = current_time_lower;
            hk_packet = (hk_pkt_t*)packet_data;
            result_global = get_hk(hk_packet,hk_seq_no,&sd_state);
			result_global = get_sd_hk(hk_packet);
			hk_packet->sd_dump = 0;
			hk_packet->ccsds_s1 = current_time_upper;
			hk_packet->ccsds_s2 = current_time_lower;
			hk_packet->aris_miss = aris_miss;
			hk_packet->payload_miss = payload_miss;
			hk_packet->hk_miss = hk_miss;
			hk_packet->logs_miss = logs_miss;
			hk_packet->sd_hk_miss = sd_hk_miss;
            log_packet->logs[log_count].task_status = result_global;
			hk_packet->q_head = q_head;
			hk_packet->q_tail = q_tail;
			hk_packet->sd_fail_count = sd_fail_count;
			hk_packet->sensor_board_fail_count = sensor_board_fail_count;
			hk_packet->sensor_board_status = sensor_board_status;
			add_to_queue(HK_PKT_LENGTH,&hk_p,packet_data,&hk_miss,HK_TASK_ID);
			if(hk_seq_no%20 == 1) {
				hk_packet->sd_dump = 1;
				store_data(&hk_p,packet_data);
			}
            hk_seq_no++;
            log_count++;
            hk_last_count_H = current_time_upper;
            hk_last_count_L = current_time_lower;
            hk_last_count = current_time;
		 }

		//If 10 log entries have been recorded, write the logs to the SD card and reset the log counter
		if(log_count >= 10) {
			form_log_packet();
		}

		if(aris_sample_no >= 20) {
			//Form Aris packet and add to queue
			TMR_stop(&aris_timer);
			TMR_stop(&sd_timer);
			toggle_aris_pointer();
			aris_sample_no = 0;
			TMR_start(&aris_timer);
			TMR_start(&sd_timer);
			//Form Aris packet and add to queue
			log_packet->logs[log_count].task_id = ARIS_TASK_ID;
			log_packet->logs[log_count].time_L = current_time_lower;
			log_packet->logs[log_count].time_H = current_time_upper;
			aris_packet_add_to_queue->ccsds_p1 = PILOT_REVERSE_BYTE_ORDER(ccsds_p1(tlm_pkt_type,ARIS_API_ID));
			aris_packet_add_to_queue->ccsds_p2 = PILOT_REVERSE_BYTE_ORDER(ccsds_p2(aris_seq_no++));
			aris_packet_add_to_queue->ccsds_p3 = PILOT_REVERSE_BYTE_ORDER(ccsds_p3(ARIS_PKT_LENGTH));
			aris_packet_add_to_queue->aris_reset_count = aris_reset_count;
			aris_packet_add_to_queue->Fletcher_Code = ARIS_FLETCHER_CODE;
			log_packet->logs[log_count].task_status = 0;
			add_to_queue(ARIS_PKT_LENGTH,&aris_p,(uint8_t*)aris_packet_add_to_queue,&aris_miss,ARIS_TASK_ID);
			if(aris_seq_no == 16383) {
				aris_reset_count++;
			}
			log_count++;
		}

		if(log_count >= 10) {
			form_log_packet();
		}

		if(sd_hk_sample_no >= 20) {
			sd_hk_sample_no = 0;

			//Form SD HK packet and add to queue
			log_packet->logs[log_count].task_id = SD_HK_TASK_ID;
			log_packet->logs[log_count].time_L = current_time_lower;
			log_packet->logs[log_count].time_H = current_time_upper;
			sd_hk.ccsds_p1 = PILOT_REVERSE_BYTE_ORDER(ccsds_p1(tlm_pkt_type,SD_HK_API_ID));
			sd_hk.ccsds_p2 = PILOT_REVERSE_BYTE_ORDER(ccsds_p2(sd_hk_seq_no++));
			sd_hk.ccsds_p3 = PILOT_REVERSE_BYTE_ORDER(ccsds_p3(SD_HK_PKT_LENGTH));
			sd_hk.end_sequence = SD_HK_FLETCHER_CODE;
			log_packet->logs[log_count].task_status = 0;
			add_to_queue(SD_HK_PKT_LENGTH,&sd_hk_p,(uint8_t*)&sd_hk,&sd_hk_miss,SD_HK_TASK_ID);
			log_count++;
		}

		if(log_count >= 10) {
			form_log_packet();
		}

		if(can_run(&sd_dump_period,&sd_dump_last_count)) {
			add_to_queue_from_sd(HK_PKT_LENGTH,&hk_p,packet_data);
			add_to_queue_from_sd(ARIS_PKT_LENGTH,&aris_p,packet_data);
			sd_dump_last_count_H = current_time_upper;
			sd_dump_last_count_L = current_time_lower;
		}

		if(can_run(&timer_period,&timer_last_count)) {
			sync_time.ccsds_p2 = PILOT_REVERSE_BYTE_ORDER(ccsds_p2(timer_seq_no));
			sync_time.reset = 0;
			MSS_TIM64_get_current_value(&sync_time.upper_count,&sync_time.lower_count);
			add_to_queue(TIME_PKT_LENGTH,&timer_p,(uint8_t*)&sync_time,&payload_miss,TIMER_TASK_ID);
			MSS_UART_disable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
			sync_time.q_head = q_head;
			sync_time.q_tail = q_tail;
			MSS_UART_enable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
			timer_last_count_H = current_time_upper;
			timer_last_count_L = current_time_lower;
			timer_seq_no++;
		}
	}
}
