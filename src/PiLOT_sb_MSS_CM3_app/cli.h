


#ifndef _CLI_DEFINITIONS_
#define _CLI_DEFINITIONS_
#include<drivers/mss_uart/mss_uart.h>
#define BAUD_RATE_ID 1;
#define TX_INV_ENA_ID 2;
#define RX_INV_ENA_ID 3;
#define BAUD_RATE_MSG "setbaudrate"
#define TX_INV_MSG "tx_inv_enable"
#define RX_INV_MSG "rx_inv_enable"
#define CMD_NOT_EXISTS "\n\rCommand Does not exists\0"

#define BAUD_RATE_FEEDBACK "\n \r BAUDRATE SET SUCCESSFULL!!\0"
#define TX_INV_FEEDBACK "\r TX_INV SUCCESSFULL!!"
#define RX_INV_FEEDBACK "\r RX_INV SUCCESSFULL!!"

 char c[25];
 char argu[25];

 uint16_t val;
 uint8_t command_index ;
 uint8_t command_flag , argument_flag ;
 uint8_t uart0_irq_rx_buffer[3];
 char all_msg[50][50];// = {BAUD_RATE_MSG, TX_INV_MSG, RX_INV_MSG};

  uint8_t  argu_index , msg_index , uart0_irq_size;

typedef struct {
	  uint8_t id;
	  char name[100];
	  void (*work)(char *arg,uint8_t size);
	  char feedback[100];
} command_t;
  void set_baud_rate(char* arg, uint8_t s);
  void get_ADC_correct_values(char *arg,uint8_t size);
  void get_imu_acc(char *data,uint8_t size);
  void get_imu_gyro(char *data,uint8_t size);
   void (*SET_BAUD_RATE)(char* arg, uint8_t s);// = &set_baud_rate;


   void scpy(char *src,char *dest);
   uint8_t scmp(char *s1,char *s2,uint8_t size);
void cli_init();
void feedback(uint8_t command_flag);
void call_function(uint8_t ID, char* data, uint8_t size);
void uart0_rx_handler(mss_uart_instance_t* this_uart);
void serial_responder();

#endif
