//#include"cli.h"
//
//cli_t command_list[MAX_NUMBER_OF_COMMANDS];
//
//static uint8_t command_id = 0;
//
////uint8_t getip_cli(char *str);
//
//char cli_str[MAX_CMD_NAME_LEN+1];
//
//uint8_t cli_char_idx;
//
//uint8_t cli_flag;
//uint8_t getip_cli(void) {
//	char c;
//	char cr[2] = "\n\r";
//	uint8_t f = 0;
//    f = DEBUG_UART_IP(DEBUG_UART,(uint8_t*)&c,1);
//    if(f) {
//        if(c == '\r') {
//            DEBUG_UART_PRINT(DEBUG_UART,(uint8_t*)cr,2);
//            cli_str[cli_char_idx] = '\0';
//            cli_char_idx = 0;
//            return 1;
//        }
//        cli_str[cli_char_idx] = c;
//        cli_char_idx++;
//        DEBUG_UART_PRINT(DEBUG_UART,(uint8_t*)&c,1);
//    }
//    return 0;
////    i = DEBUG_UART_IP(DEBUG_UART,(uint8_t*)str,(MAX_CMD_NAME_LEN + 1));
////    //DEBUG_UART_PRINT(DEBUG_UART,(uint8_t*)str,12);
////    return i;
//}
//
//void add_command(char *name,uint8_t (*work)(void)) {
//    command_list[command_id].cid = command_id;
//    strcpy(command_list[command_id].name,name);
//    command_list[command_id].work = work;
//    command_id++;
//}
//
//void cli_init() {
//    //AHAN_Init();
//    cli_char_idx = 0;
//    add_command("ping",&ping);
//    add_command("i2c_test",&i2c_test_cli);
//    add_command("logs",&display_logs);
//    add_command("eps_hk",&display_eps_data);
//    print("AHAN OBC> ");
//}
//
//void cli() {
//    char cmd[MAX_CMD_NAME_LEN + 1];
//    uint8_t cmd_idx = 0,flag = 0;
//    cli_init();
//    print("\n\r");
//    while(1) {
//        print("AHAN OBC> ");
//        while(getip(cmd) == 0);
//        print(cmd);
//        print("\n\r");
//        flag = 0;
//        for(cmd_idx = 0;cmd_idx < command_id;cmd_idx++) {
//            if((strcmp(cmd,command_list[cmd_idx].name) == 0)) {
//                command_list[cmd_idx].work();
//                flag = 1;
//            }
//        }
//        if(!flag) {
//            print("Command not found\n\r");
//        }
//        print("\n\rAHAN OBC> ");
//        return 0;
//    }
//}
//
//uint8_t cli_task(void) {
//    char cmd[MAX_CMD_NAME_LEN + 1];
//    uint8_t cmd_idx = 0,flag = 0;
//    //cli_init();
//    //print("\n\r");
//    while(1) {
//        //print("AHAN OBC> ");
//        if(getip_cli() == 0) {
//            return 0;
//        }
//        print(cli_str);
//        print("\n\r");
//        flag = 0;
//        for(cmd_idx = 0;cmd_idx < command_id;cmd_idx++) {
//            if((strcmp(cli_str,command_list[cmd_idx].name) == 0)) {
//                command_list[cmd_idx].work();
//                flag = 1;
//            }
//        }
//        if(!flag) {
//            print("Command not found\n\r");
//        }
//        print("\n\rAHAN OBC> ");
//        return 0;
//    }
//}
