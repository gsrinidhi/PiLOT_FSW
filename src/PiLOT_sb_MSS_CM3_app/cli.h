


#ifndef _CLI_DEFINITIONS_
#define _CLI_DEFINITIONS_
#include<drivers/mss_uart/mss_uart.h>
#define BAUD_RATE_ID 1;
#define TX_INV_ENA_ID 2;
#define RX_INV_ENA_ID 3;
#define BAUD_RATE_MSG "setbaudrate"
#define TX_INV_MSG "tx_inv_enable"
#define RX_INV_MSG "rx_inv_enable"
#define CMD_NOT_EXISTS "Command Does not exists"

#define BAUD_RATE_FEEDBACK "\n \r BAUDRATE SET SUCCESSFULL!!"
#define TX_INV_FEEDBACK "\r TX_INV SUCCESSFULL!!"
#define RX_INV_FEEDBACK "\r RX_INV SUCCESSFULL!!"

 char c[25];
 char argu[25];

 uint16_t val;
 uint8_t command_index ;
 uint8_t command_flag , argument_flag ;
 uint8_t uart0_irq_rx_buffer[3];
 char all_msg[50][50];// = {BAUD_RATE_MSG, TX_INV_MSG, RX_INV_MSG};
 char cmd_not_exists[50];// = CMD_NOT_EXISTS;
  uint8_t  argu_index , msg_index , uart0_irq_size;


  void set_baud_rate(char* arg, uint8_t s);
   void (*SET_BAUD_RATE)(char* arg, uint8_t s);// = &set_baud_rate;



void cli_init();
void feedback(uint8_t command_flag);
void call_function(uint8_t ID, char* data, uint8_t size);
void uart0_rx_handler(mss_uart_instance_t* this_uart);
void serial_responder();

#endif
