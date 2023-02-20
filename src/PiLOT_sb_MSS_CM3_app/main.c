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
//#define DEBUG 		1
#include "memory.h"
#include "pilot.h"
#include "peripherals.h"
#include "testing.h"
#include "cli.h"
#include "commands.h"
#define MAX_COUNT		0xFFFFFFFF

#define TESTING		0
#if TESTING == 0
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
uint8_t aris_packet_data[512],aris_temp_data[512];

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
 * @brief Packet to store current counter values
 */
timer_pkt sync_time;
/**
 * @brief These two variable hold the current counts of the 64 bit counter
 * 
 */
uint32_t current_time_lower,current_time_upper;
uint64_t current_time;
/**
 * @brief These variables hold the upper and lower periods of all the tasks
 * 
 */
uint32_t payload_period_L,payload_period_H,hk_period_H,hk_period_L,sd_hk_period_L,sd_hk_period_H,aris_period_L,aris_period_H,sd_dump_period_L,sd_dump_period_H,timer_period_H,timer_period_L;
uint64_t payload_period,hk_period,sd_hk_period,aris_period,sd_dump_period,timer_period;
/**
 * @brief These variables are used to keep track of the last counts at which a task was run
 * 
 */
uint32_t payload_last_count_L,payload_last_count_H,hk_last_count_H,hk_last_count_L,sd_hk_last_count_L,sd_hk_last_count_H,aris_last_count_L,aris_last_count_H,sd_dump_last_count_L,sd_dump_last_count_H,timer_last_count_H,timer_last_count_L;
uint64_t payload_last_count,hk_last_count,sd_hk_last_count,aris_last_count,sd_dump_last_count,timer_last_count;
/**
 * @brief These varibles are used to hold the sequence numbers of all the packets
 * 
 */
uint16_t thermistor_seq_no,logs_seq_no,hk_seq_no,sd_hk_seq_no,aris_seq_no,timer_seq_no;

/**
 * @brief These variables are used for queue operations
 * 
 */
uint16_t q_head,q_tail,q_in_i,aris_miss,hk_miss,payload_miss;

timer_instance_t aris_timer;
/**
 * @brief Miscellaneous variables used to keep track of various counts and states
 * 
 */
uint16_t aris_collect;
uint8_t log_count,result_global,api_id,sd_state,aris_sample_no;
uint8_t uart_irq_rx_buffer[3],uart_irq_tx_buffer[2];
uint8_t uart_irq_size;
uint8_t sd_count;
uint8_t interface_count;
sd_test sd;
volatile uint8_t uart_irq_addr_flag;

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
uint64_t no;
void set_baud_rate(char* arg, uint8_t s);
void (*SET_BAUD_RATE)(char* arg, uint8_t s) = &set_baud_rate;
uint8_t add_command(char[],uint8_t);
/**
 * @brief This is the interrupt handler which gets triggered whenever a byte is received from the UART Rx line. This function first checks if the receved byte is an address byte by reading the PE bit of the Line Status Register (LSR) of MSS UART 1. Then it checks if the received address is that of PiLOT. If the address matches, two bytes of data are transmitted from the pslv_queue
 * 
 * @param this_uart : The uart from which the interrupt was triggered.
 */
#if DEBUG == 0
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
#endif
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
 * @brief This function adds the given data to the queue if the required number of bytes are free in the queue. Otherwise the data is written to the SD card.
 *
 * @param size	:	The size of the data to be written
 * @param p 	: 	The partiton of the sd card to which the data must be written in case the queue is full.
 * @param data 	:	The data to be written
 */
void add_to_queue(uint8_t size,partition_t *p,uint8_t *data,uint16_t *miss) {
	q_in_i = 0;
	MSS_UART_enable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
	//while(!((q_head > q_tail && (2048 - q_head + q_tail) >= size) || (q_head < q_tail && (q_tail - q_head) >= size)));
	MSS_UART_disable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
	if((q_head > q_tail && (2048 - q_head + q_tail) >= size) || (q_head < q_tail && (q_tail - q_head) >= size)) {
		for(;q_in_i<size;q_in_i+=(interface_count+1)) {
			pslv_queue[q_head] = data[q_in_i];
			pslv_queue[q_head+1] = data[q_in_i+interface_count];
			q_head+=2;
			if(q_head >= 2048) {
				//q_head reached limit
				q_head = 0;
			}
		}
	} else {
		(*miss)+=1;
//		if((*miss) > 10) {
//			store_data(p,data);
//		}
//			if(sd_state == 0x7) {//If sd card is working
//
//			result_global = store_data(p,data);
//			if(result_global == 1) {//
//				sd_state = 0x0;//so that no furhther operations are done on the SD card until its functionality has been checked again
//			}
//		}
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

/**
 * @brief This function initialises all the global variables in this file.
 * 
 * @return uint8_t 
 */
uint8_t Flags_Init() {
//	sync_time.sync = 0xA0A0A0A0;
	/**
	 * @brief Set the MSS UART 1 interrupt handler to the one defined above
	 *
	 */
#if DEBUG == 0
    MSS_UART_set_rx_handler(&g_mss_uart1,
                            uart1_rx_handler,
                            MSS_UART_FIFO_SINGLE_BYTE);
#endif
	timer_seq_no = 1;
	sync_time.ccsds_p1 = PILOT_REVERSE_BYTE_ORDER(ccsds_p1(tlm_pkt_type,TIME_API_ID));
	sync_time.ccsds_p2 = PILOT_REVERSE_BYTE_ORDER(ccsds_p2(timer_seq_no));
	sync_time.ccsds_p3 = PILOT_REVERSE_BYTE_ORDER(ccsds_p3(TIME_PKT_LENGTH));
	MSS_TIM64_get_current_value(&sync_time.upper_count,&sync_time.lower_count);
	sync_time.tail = 0xBCBC;
	sync_time.reset = 1;
    q_head = 2;
    q_tail = 0;
    initialise_partition(&timer_p,0,0);
    hk_miss = 0;
	MSS_UART_disable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
	add_to_queue(TIME_PKT_LENGTH,&timer_p,(uint8_t*)&sync_time,&hk_miss);
	MSS_UART_enable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
#if DEBUG == 1
			downlink((uint8_t*)&sync_time,TIME_PKT_LENGTH);
#endif
	/**
	 * @brief Initialise the period upper and lower counters for all tasks
	 * 
	 */
	time_to_count(DEFAULT_HK_PERIOD,&hk_period_H,&hk_period_L);
	time_to_count(DEFAULT_PAYLOAD_PERIOD,&payload_period_H,&payload_period_L);
	time_to_count(10000,&sd_hk_period_H,&sd_hk_period_L);
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
	timer_seq_no = 2;
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
	timer_last_count_H = 0xFFFFFFFF;
	timer_last_count_L = 0xFFFFFFFF;
	hk_packet = (hk_pkt_t*)packet_data;
	aris_miss = 0;
	payload_miss = 0;
	hk_miss = 0;
	//hk_last_count = 0xFFFFFFFFFFFFFFFF;
	//packet_data = 0xFFFFFFFFFFFFFFFF;


	/**
	 * @brief Initialise the queue variables. The queue is empty when the q_tail is two units behind the q_head. The queue is full when the q_head and q_tail are equal
	 * 
	 */
    sd_count = SD_TIMEOUT;
    TMR_init(&aris_timer,CORETIMER_0_0,TMR_CONTINUOUS_MODE,PRESCALER_DIV_2,aris_period_L/2);
    NVIC_EnableIRQ( FabricIrq10_IRQn );
    NVIC_SetPriority(FabricIrq10_IRQn,0xFE);
    //NVIC_SetPriority(UART1_IRQn,0xFD);
    TMR_enable_int(&aris_timer);
    TMR_start(&aris_timer);
	return 0;
}

void FabricIrq10_IRQHandler(void) {
	TMR_clear_int(&aris_timer);
	if(aris_sample_no < 20) {
		if(aris_sample_no == 0) {
			aris_packet->start_time = current_time_upper;
			aris_packet->ccsds_s2 = current_time_lower;
			aris_packet->ccsds_s1 = current_time_upper;
		}
		result_global = get_aris_sample(aris_packet,current_time_lower,aris_sample_no);
		aris_sample_no++;
	}

}
uint32_t time_us;
int main()
{
	//Initialise all the peripherals and devices connected to the OBC in PiLOT
	timer_start();
	result_global = Pilot_Init();
	time_us = get_time_in_us();
	sd_state = result_global & 0x1;
	sd_state = !sd_state;
	if(sd_state == 1){
		sd_state = 0x7;
	}
	//Initialise all the global variables in main.c
	timer_start();
	Flags_Init();
	time_us = get_time_in_us();
	log_packet = (log_packet_t*)log_data;
	aris_packet = (aris_pkt_t*)aris_packet_data;
	cli_init();
//	MSS_UART_disable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
//	add_to_queue(TIME_PKT_LENGTH,&timer_p,(uint8_t*)&sync_time,&hk_miss);
//	MSS_UART_enable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
	while(1) {
		//Reload the watchdog counter
		MSS_WD_reload();
		//Get the current values of the 64 bit counter
		MSS_TIM64_get_current_value(&current_time_upper,&current_time_lower);
		//Checking if it is time to take thermistor readings (must be verified)
		if(((payload_last_count_H - current_time_upper >= payload_period_H) && (payload_last_count_L - current_time_lower >= payload_period_L)) || ((payload_last_count_H - current_time_upper > payload_period_H) && (current_time_lower > payload_last_count_L) && (current_time_lower - payload_last_count_L < MAX_COUNT - payload_period_L) )) {
			timer_start();
			log_packet->logs[log_count].task_id = THERMISTOR_TASK_ID;
			log_packet->logs[log_count].time_H = current_time_upper;
			log_packet->logs[log_count].time_L = current_time_lower;
			thermistor_packet = (thermistor_pkt_t*)packet_data;
			result_global = get_thermistor_vals(thermistor_packet,thermistor_seq_no);
			thermistor_packet->ccsds_s2 = current_time_lower;
			thermistor_packet->ccsds_s1 = current_time_upper;
			MSS_UART_disable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
			add_to_queue(THERMISTOR_PKT_LENGTH,&payload_p,packet_data,&payload_miss);
			MSS_UART_enable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
			log_packet->logs[log_count].task_status = result_global;
			thermistor_seq_no++;
			log_count++;
			payload_last_count_H = current_time_upper;
			payload_last_count_L = current_time_lower;
#if DEBUG == 1
			downlink(packet_data,THERMISTOR_PKT_LENGTH);
#endif
			time_us = get_time_in_us();
			dummy = 8;
		}

		//If 10 log entries have been recorded, write the logs to the queue and reset the log counter
		if(log_count >= 10) {
			timer_start();
			log_packet->ccsds_p1 = PILOT_REVERSE_BYTE_ORDER(ccsds_p1(tlm_pkt_type,LOGS_API_ID));
			log_packet->ccsds_p2 = PILOT_REVERSE_BYTE_ORDER(ccsds_p2((logs_seq_no)));
			log_packet->ccsds_p3 = PILOT_REVERSE_BYTE_ORDER(ccsds_p3(LOGS_PKT_LENGTH));
			log_packet->Fletcher_Code = LOGS_FLETCHER_CODE;
			log_packet->ccsds_s2 = current_time_lower;
			log_packet->ccsds_s1 = current_time_upper;
			MSS_UART_disable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
			add_to_queue(LOGS_PKT_LENGTH,&log_p,log_data,&payload_miss);
			MSS_UART_enable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
#if DEBUG == 1
			downlink(log_data,LOGS_PKT_LENGTH);
#endif
			log_count = 0;
			logs_seq_no++;
			time_us = get_time_in_us();
			dummy = 8;
		}

		// For HK Packet

		if(((hk_last_count_H - current_time_upper >= hk_period_H) && (hk_last_count_L - current_time_lower >= hk_period_L)) || ((hk_last_count_H - current_time_upper > hk_period_H) && (current_time_lower > hk_last_count_L) && (current_time_lower - hk_last_count_L < MAX_COUNT - hk_period_L) )) {
			timer_start();
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
            log_packet->logs[log_count].task_status = result_global;
			MSS_UART_disable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
//			q_tail = 0;
//			q_head = 2;
			hk_packet->q_head = q_head;
			hk_packet->q_tail = q_tail;
			add_to_queue(HK_PKT_LENGTH,&hk_p,packet_data,&hk_miss);
			MSS_UART_enable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
			if(hk_seq_no%20 == 1) {
				hk_packet->sd_dump = 1;
				store_data(&hk_p,packet_data);
			}
            hk_seq_no++;
            log_count++;
            hk_last_count_H = current_time_upper;
            hk_last_count_L = current_time_lower;
#if DEBUG == 1
			downlink(packet_data,HK_PKT_LENGTH);
#endif
			time_us = get_time_in_us();
			//while(q_tail!=(q_head-2));
			dummy = 8;
		 }

		//If 10 log entries have been recorded, write the logs to the SD card and reset the log counter
		if(log_count >= 10) {
			timer_start();
			log_packet->ccsds_p1 = PILOT_REVERSE_BYTE_ORDER(ccsds_p1(tlm_pkt_type,LOGS_API_ID));
			log_packet->ccsds_p2 = PILOT_REVERSE_BYTE_ORDER(ccsds_p2((logs_seq_no)));
			log_packet->ccsds_p3 = PILOT_REVERSE_BYTE_ORDER(ccsds_p3(LOGS_PKT_LENGTH));
			log_packet->Fletcher_Code = LOGS_FLETCHER_CODE;
			log_packet->ccsds_s2 = current_time_lower;
			log_packet->ccsds_s1 = current_time_upper;
			MSS_UART_disable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
			add_to_queue(LOGS_PKT_LENGTH,&log_p,log_data,&payload_miss);
			MSS_UART_enable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
			log_count = 0;
			logs_seq_no++;
#if DEBUG == 1
			downlink(log_data,LOGS_PKT_LENGTH);
#endif
			time_us = get_time_in_us();
			dummy = 8;
		}

		if(aris_sample_no >= 20) {
			timer_start();
			TMR_stop(&aris_timer);
			for(aris_collect = 0;aris_collect < 512;aris_collect++) {
				aris_temp_data[aris_collect] = aris_packet_data[aris_collect];
			}
			result_global = 0;
			aris_seq_no++;
			aris_sample_no = 0;
			//TMR_start(&aris_timer);
			//Form Aris packet and add to queue
			log_packet->logs[log_count].task_id = ARIS_TASK_ID;
			log_packet->logs[log_count].time_L = current_time_lower;
			log_packet->logs[log_count].time_H = current_time_upper;
			aris_packet->ccsds_p1 = PILOT_REVERSE_BYTE_ORDER(ccsds_p1(tlm_pkt_type,ARIS_API_ID));
			aris_packet->ccsds_p2 = PILOT_REVERSE_BYTE_ORDER(ccsds_p2(aris_seq_no));
			aris_packet->ccsds_p3 = PILOT_REVERSE_BYTE_ORDER(ccsds_p3(ARIS_PKT_LENGTH));
			aris_packet->Fletcher_Code = ARIS_FLETCHER_CODE;
			log_packet->logs[log_count].task_status = 0;
			MSS_UART_disable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
			add_to_queue(ARIS_PKT_LENGTH,&aris_p,aris_packet_data,&aris_miss);
			MSS_UART_enable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
			TMR_start(&aris_timer);
			//Reset sample count


			log_count++;
			//TMR_start(&aris_timer);
#if DEBUG == 1
			downlink(aris_packet_data,ARIS_PKT_LENGTH);
#endif
			time_us = get_time_in_us();
			dummy = 8;
		}

//		if(((aris_last_count_H - current_time_upper >= aris_period_H) && (aris_last_count_L - current_time_lower >= aris_period_L)) || ((aris_last_count_H - current_time_upper > aris_period_H) && (current_time_lower > aris_period_L) && (current_time_lower - aris_last_count_L < MAX_COUNT - aris_period_L) )) {
//			timer_start();
//			aris_packet = (aris_pkt_t*)aris_packet_data;
//			if(aris_sample_no == 0) {
//				aris_packet->start_time = current_time_upper;
//				aris_packet->ccsds_s2 = current_time_lower;
//				aris_packet->ccsds_s1 = current_time_upper;
//			}
//			result_global = get_aris_sample(aris_packet,current_time_lower,aris_sample_no);
//			aris_sample_no++;
//			aris_last_count_H = current_time_upper;
//			aris_last_count_L = current_time_lower;
//			time_us = get_time_in_us();
//			dummy = 8;
//		}

		if(log_count >= 10) {
			timer_start();
			log_packet->ccsds_p1 = PILOT_REVERSE_BYTE_ORDER(ccsds_p1(tlm_pkt_type,LOGS_API_ID));
			log_packet->ccsds_p2 = PILOT_REVERSE_BYTE_ORDER(ccsds_p2((logs_seq_no)));
			log_packet->ccsds_p3 = PILOT_REVERSE_BYTE_ORDER(ccsds_p3(LOGS_PKT_LENGTH));
			log_packet->Fletcher_Code = LOGS_FLETCHER_CODE;
			log_packet->ccsds_s2 = current_time_lower;
			log_packet->ccsds_s1 = current_time_upper;
			MSS_UART_disable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
			add_to_queue(LOGS_PKT_LENGTH,&log_p,log_data,&payload_miss);
			MSS_UART_enable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
			log_count = 0;
			logs_seq_no++;
//#if DEBUG == 1
//			downlink(log_data,LOGS_PKT_LENGTH);
//#endif
			time_us = get_time_in_us();
			dummy = 8;
		}

		if((sd_hk_last_count_H - current_time_upper >= sd_hk_period_H) && (sd_hk_last_count_L - current_time_lower >= sd_hk_period_L)) {
			timer_start();
			sd.ccsds_p1 = PILOT_REVERSE_BYTE_ORDER(ccsds_p1(tlm_pkt_type,SD_HK_API_ID));
			sd.ccsds_p2 = PILOT_REVERSE_BYTE_ORDER(ccsds_p2((sd_hk_seq_no)));
			sd.ccsds_p3 = PILOT_REVERSE_BYTE_ORDER(ccsds_p3(SD_HK_PKT_LENGTH));
			result_global = sd_hk_test(&sd,packet_data,sd_hk_seq_no);
			sd.Fletcher_Code = SD_HK_FLETCHER_CODE;
			sd_hk_seq_no++;
			time_us = get_time_in_us();
			MSS_UART_disable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
			add_to_queue(SD_HK_PKT_LENGTH,&payload_p,(uint8_t*)&sd,&payload_miss);
			MSS_UART_enable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
			sd.time = time_us;
			sd_hk_last_count_H = current_time_upper;
			sd_hk_last_count_L = current_time_lower;
			dummy = 8;
		}

		if(((sd_dump_last_count_H - current_time_upper >= sd_dump_period_H) && (sd_dump_last_count_L - current_time_lower >= sd_dump_period_L)) || ((sd_dump_last_count_H - current_time_upper > sd_dump_period_H) && (current_time_lower > sd_dump_last_count_L) && (current_time_lower - sd_dump_last_count_L < MAX_COUNT - sd_dump_period_L) )) {
			timer_start();
			add_to_queue_from_sd(HK_PKT_LENGTH,&hk_p,packet_data);
			sd_dump_last_count_H = current_time_upper;
			sd_dump_last_count_L = current_time_lower;
			time_us = get_time_in_us();
			dummy = 8;
		}

		if(((timer_last_count_H - current_time_upper >= timer_period_H) && (timer_last_count_L - current_time_lower >= timer_period_L)) || ((timer_last_count_H - current_time_upper > timer_period_H) && (current_time_lower > timer_last_count_L) && (current_time_lower - timer_last_count_L < MAX_COUNT - timer_period_L) )) {
			MSS_UART_disable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
//			q_tail = 0;
//			q_head = 2;
			sync_time.ccsds_p2 = PILOT_REVERSE_BYTE_ORDER(ccsds_p2(timer_seq_no));
			sync_time.reset = 0;
			MSS_TIM64_get_current_value(&sync_time.upper_count,&sync_time.lower_count);
			add_to_queue(TIME_PKT_LENGTH,&timer_p,(uint8_t*)&sync_time,&payload_miss);
			MSS_UART_enable_irq(&g_mss_uart1,MSS_UART_RBF_IRQ);
			timer_last_count_H = current_time_upper;
			timer_last_count_L = current_time_lower;
#if DEBUG == 1
			downlink((uint8_t*)&sync_time,TIME_PKT_LENGTH);
#endif

			timer_seq_no++;
		}
        if(command_index > 1){
            if(c[command_index - 1] == '\r'){
                serial_responder();
            }

        }
	}
}
#else
#include "testing.h"
int main() {

	/**
	 * code to get the running time of the main tasks
	 */
//	while(1) {
//		testing_init();
//		get_hk_time();
//		get_thermistor_time();
//		get_aris_time();
//	}

		uint8_t w_data[512], r_data[512];
		uint16_t i;
		uint32_t addr1 = 512 * 8, addr2 = 512 * 9;
		uint8_t stat = Pilot_Init();

//		for(i = 0; i<512; i++){
//			if(i<256)
//				w_data[i] = i;
//			else
//				w_data[i] = i - 256;
//			r_data[i] = 2;
//		}
//
//		stat = SD_Init();
//
//		stat = SD_Write(addr1,w_data);
//
//
//		for(i = 0; i<512; i++){
//				if(i<256)
//					w_data[i] = 256 - i;
//				else
//					w_data[i] = i - 256;
//				r_data[i] = 2;
//			}
//		stat = SD_Write(addr2,w_data);
//
//		MSS_GPIO_set_output(SD_CARD_GPIO,0);
//		stat = SD_Init();
//
//		stat = SD_Read(addr1,r_data);
//		stat = SD_Read(addr2,r_data);
//
//		for(i = 0; i<512; i++){
//			w_data[i] = 1;
//			r_data[i] = 2;
//		}

		//Initialising sd card 2500 times for testing
//		uint16_t count = 0;
//		while(1) {
//			MSS_WD_reload();
//			stat = SD_Init();
//			MSS_GPIO_set_output(SD_CARD_GPIO,0);
//			if(stat) {
//				count = 0;
//			}
//			count++;
//			if(count > 2500) {
//				count = 0;
//			}
//
//		}

		//Writing to all blocks
//		uint32_t addr = 0;
//		uint8_t data[512];
//		for(addr = 0;addr < 512;addr++) {
//			data[addr] = addr & 0xFF;
//		}
//		addr = 0;
//		stat = SD_Init();
//		uint32_t fail_addr[2048];
//		uint32_t count = 0;
//		while(1) {
//			MSS_WD_reload();
//			stat = SD_Write(addr,data);
//			if(stat) {
//
////				MSS_GPIO_set_output(SD_CARD_GPIO,0);
////				stat = SD_Init();
////				dummy = 0;
////				fail_addr[count] = addr;
////				count++;
//			}
//			if(addr%500 == 0) {
//				dummy = 0;
//			}
//			addr++;
//		}

		uint32_t addr = 0;
		stat = SD_Init();
		uint8_t rdata[512],wdata[512];
		i = 0;
		for(;i<512;i++) {
			wdata[i] = i%255;
			rdata[i] = 0;
		}
		uint32_t thr = 0x80;
		uint8_t k = 0;
		for(addr = thr;addr < 0x80000000;addr = addr * 2) {
			for(i = 0;i<512;i++) {
				wdata[i] = k;
			}
			stat = SD_Write(addr-1,wdata);
			stat = SD_Read(addr-1,rdata);
			k++;
			i = 0;
		}
		while(1) {

		}
}
#endif
