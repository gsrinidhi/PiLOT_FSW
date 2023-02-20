

#include "cli.h"
uint8_t prompt_msg[30] =  "\n\r PiLOT Commanding : \0" ;
char all_msg[50][50] = {BAUD_RATE_MSG, TX_INV_MSG, RX_INV_MSG};
char cmd_not_exists[50] = CMD_NOT_EXISTS;
command_t cmd_list[10];
uint8_t cmd_in;
uint8_t scmp(char *s1,char *s2,uint8_t size) {
	uint8_t k = 0;
	for(;k<size;k++) {
		if(s1[k] != s2[k]) {
			return 1;
		}
	}
	return 0;
}
void scpy(char *src,char *dest) {
	uint8_t ci = 0;
	while(1) {
		dest[ci] = src[ci];
		if(!src[ci]) {
			break;
		}
		ci++;
	}
}
void add_command(char name[],void (*work)(char *arg,uint8_t size),char fb[]) {
	cmd_list[cmd_in].id = cmd_in;
	scpy(name,cmd_list[cmd_in].name);
	cmd_list[cmd_in].work = work;
	scpy(fb,cmd_list[cmd_in].feedback);
	cmd_in++;
}
void uart0_rx_handler(mss_uart_instance_t* this_uart) {
    uart0_irq_size = MSS_UART_get_rx(this_uart,uart0_irq_rx_buffer,1);
    if(uart0_irq_rx_buffer[0] == 127){
        c[command_index] = '\0';
        c[command_index - 1] = 127;
        MSS_UART_polled_tx(&g_mss_uart0, &c[command_index - 1], 1);
        command_index = command_index - 1;
    }
    else{
        c[command_index] =  uart0_irq_rx_buffer[0];
        MSS_UART_polled_tx(&g_mss_uart0, &c[command_index], 1);
        command_index = command_index + 1;
    }

}
void cli_init(){
    MSS_UART_init(&g_mss_uart0, 9600, MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT );
    MSS_UART_set_rx_handler(&g_mss_uart0,uart0_rx_handler,MSS_UART_FIFO_SINGLE_BYTE);
	c[command_index - 1] = '\0';
	command_flag = 0;
	command_index = 0;
	argu_index = 0;
	argument_flag = 0;
	msg_index = 0;
	val = 0;
	add_command("setbaudrate\0",set_baud_rate,"baud rate set\0");
	MSS_UART_polled_tx_string(&g_mss_uart0,prompt_msg);

}
void cli_reset() {
    MSS_UART_init(&g_mss_uart0, 9600, MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY | MSS_UART_ONE_STOP_BIT );
    MSS_UART_set_rx_handler(&g_mss_uart0,uart0_rx_handler,MSS_UART_FIFO_SINGLE_BYTE);
	c[command_index - 1] = '\0';
	command_flag = 0;
	command_index = 0;
	argu_index = 0;
	argument_flag = 0;
	msg_index = 0;
	val = 0;

	MSS_UART_polled_tx_string(&g_mss_uart0,prompt_msg);
}
void feedback(uint8_t command_flag){
//    if(command_flag == 1){
//        uint8_t message[50] = BAUD_RATE_FEEDBACK;
//        MSS_UART_polled_tx_string(&g_mss_uart0, message);
//    }
	MSS_UART_polled_tx_string(&g_mss_uart0, cmd_list[command_flag].feedback);
    cli_reset();
}


void call_function(uint8_t ID, char* data, uint8_t size){

	if(ID < cmd_in) {
		cmd_list[ID].work(data,size);
	}

//	if(ID == 1){
//		(*SET_BAUD_RATE)(data,size);
//	}

//	else if(ID == 2){
//		(TX_INV_ENABLE)(data, size);
//	}
//
//	else if(ID == 3){
//		(RX_INV_ENABLE)(data, size);
//	}


	else{
		MSS_UART_polled_tx_string(&g_mss_uart0, cmd_not_exists);
	}

	feedback(ID);
}

void chk_msg(){



//    for(i = 0;i<msg_index;i++){
//                   if(c[i] != add_cmd_msg[i]){
//                	   command_flag = 0;
//                       break;
//                   }
//                   else{
//        				  if(i == msg_index -1){
//        					  MSS_UART_polled_tx_string(&g_mss_uart0, name_msg);
//        					  MSS_UART_set_rx_handler(&g_mss_uart0,uart0_rx_handler,MSS_UART_FIFO_SINGLE_BYTE);
//        				  }
//
//                       }
//                   }
uint8_t j,i;

	for(j = 0;j<cmd_in;j++) {
		if(scmp(c,cmd_list[j].name,msg_index) == 0) {
			command_flag = j;
			break;
		}
	}
//    for(j=0;j<5;j++){
//		for(i = 0;i<msg_index;i++){
//
//			   if(c[i] != all_msg[j][i]){
//				   command_flag = 0;
//				   break;
//			   }
//			   else{
//					  if(i == msg_index -1){
//						  command_flag = j + 1;
//						  break;
//					  }
//
//				   }
//			   }
//		if(command_flag == j + 1){
//			break;
//		}
//    }


    call_function(command_flag,argu,command_index - msg_index);

 }

void serial_responder(){
    uint8_t i=0;

    while(i <= command_index){
        if(c[i] == ' '){
            msg_index = i;
            for(argu_index = 0;argu_index < command_index - msg_index;argu_index++){
                argu[argu_index] = c[i+1];
                i = i + 1;
            }

        }
        i = i + 1;
    }
    chk_msg();

 }

