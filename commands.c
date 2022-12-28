//#include"commands.h"
//#include<string.h>
//char null_print[] = "Trying to print null";
//void print(char *str) {
//	if(strlen(str) > 0) {
//		DEBUG_UART_PRINT(DEBUG_UART,(uint8_t*)str,strlen(str));
//	} else {
//		DEBUG_UART_PRINT(DEBUG_UART,(uint8_t*)null_print,strlen(null_print));
//	}
//
//}
//
//void print_num(uint8_t num) {
//	uint8_t r = 0;
//	while(num > 0) {
//		r = num%10 + 48;
//		DEBUG_UART_PRINT(DEBUG_UART,&r,1);
//		num/=10;
//	}
//}
//
//uint8_t ping(void) {
//    print("ping\n\r");
//    return 0;
//}
//
//
//
//uint8_t getip(char *str) {
//	char c;
//	char cr[2] = "\n\r";
//	uint8_t i = 0,f = 0;
//	while(1) {
//		f = DEBUG_UART_IP(DEBUG_UART,(uint8_t*)&c,1);
//		if(f) {
//			if(c == '\r') {
//				DEBUG_UART_PRINT(DEBUG_UART,(uint8_t*)cr,2);
//				str[i] = '\0';
//				return i;
//			}
//			str[i] = c;
//			i++;
//			DEBUG_UART_PRINT(DEBUG_UART,(uint8_t*)&c,1);
//		}
//
//	}
////    i = DEBUG_UART_IP(DEBUG_UART,(uint8_t*)str,(MAX_CMD_NAME_LEN + 1));
////    //DEBUG_UART_PRINT(DEBUG_UART,(uint8_t*)str,12);
////    return i;
//}
//
//uint8_t i2c_test_cli(void) {
//	uint8_t res = test_peripherals();
//	uint8_t i = 0,res1;
//	char str[2];
//	for(;i<4;i++) {
//		res1 = i+2+48;
//		str[0] = res1;
//		str[1] = '\0';
//		if((res & (1<<i)) == 0) {
//			print("I2C ");
//			print(str);
//			print(": Success\n\r");
//		} else {
//			print("I2C ");
//			print(str);
//			print(": Failed\n\r");
//		}
//	}
//	return 0;
//}
//
//void cli_mem_init() {
//    uint8_t s = memory_init();
//    if(!s) {
//        print("Memory Initialisation failed\n\r");
//    } else {
//        print("Memory Initialisation Successfull\n\r");
//    }
//}
//
//uint8_t print_start_block(void) {
//	uint8_t mem_status;
//    char texts[][30] = {"Comms starting block: ","Payload starting block","EPS starting block: ","Logging starting block: "};
//    mem_status = SD_Read(INIT_BLOCK,(uint8_t*)&start_block);
//    if(mem_status) {
//        print("INIT BLOCK read failed\n\r");
//        return 1;
//    } else {
//        char op[50];
//        uint8_t num[] = {start_block.comms,start_block.payload,start_block.power,start_block.task_logs};
//        uint8_t i = 0;
//        for(;i<4;i++) {
//            printf(op,"%s %d\n\r",texts[i],num[i]);
//            print(op);
//        }
//    }
// return 0;
//}
//
//void rtc_to_str(mss_rtc_calendar_t cal,char *str) {
//    sprintf(str,"Seconds: %d\n\rMinutes: %d\n\rDate: %d\n\rMonth: %d\n\r",cal.second,cal.minute,cal.day,cal.month);
//}
//
//uint8_t display_logs(void) {
//    log_t *l;
//    char log_str[500],cal_str[500],block[10];
//    uint8_t test[sizeof(log_t)];
//    uint16_t lno = current_block.task_logs,k = current_block.task_logs,p;
////    sprintf(block,"Start block = %d\n",lno);
////    print(block);
////    sprintf(block,"End block = %d\n",k);
////    print(block);
//    print("Lno = ");
//    print_num(lno);
//    print("\n\r");
//    print("k = ");
//	print_num(k);
//	print("\n\r");
//    while(lno >= k-10) {
//        print("In while Lno = ");
//        print_num(lno);
//        print("\n\r");
////        print("k = ");
////    	print_num(k);
////    	print("\n\r");
//    	if(lno < 1) {
//    		print("In if lno compare\n\r");
//    		break;
//    	}
//        print("In lno compare Lno = ");
//        print_num(lno);
//        print("\n\r");
//    	p = lno;
//        if(SD_Read((uint32_t)(p * 512),test)) {
//        	print("In if sd read\n\r");
//            sprintf(log_str,"SD read failed at %d\n\r",lno);
//            print(log_str);
//            break;
//        }
//        print("In sd read Lno = ");
//        print_num(lno);
//        print("\n\r");
//		l = (log_t*)&test;
//        sprintf(log_str,"Task ID: %d\n\r",l->id);
//        print(log_str);
//        rtc_to_str(l->start_time,cal_str);
//        sprintf(log_str,"Task Start time: \n\r%s\n\r",cal_str);
//        print(log_str);
//        rtc_to_str(l->end_time,cal_str);
//        sprintf(log_str,"Task End time: \n\r%s\n\r",cal_str);
//        print(log_str);
//        lno--;
//        print_num(lno);
//        print("Lno = ");
//        print_num(lno);
//        print("\n\r");
//        print("k = ");
//    	print_num(k);
//    	print("\n\r");
//		lno--;
//    }
//    return 0;
//}
//
//uint8_t display_eps_data(void) {
//    eps_data_t *e;
//    char log_str[500],cal_str[500],block[10];
//    uint8_t test[sizeof(eps_data_t)];
//    uint16_t lno = current_block.power,k = 0,p;
//    print("Lno = ");
//    print_num(lno);
//    print("\n\r");
//    print("k = ");
//	print_num(k);
//	print("\n\r");
//    while(1) {
//    	if(lno < 1) {
//    		break;
//    	}
//    	p = lno;
//        if(SD_Read((uint32_t)(p * 512),test)) {
//        	print("In if sd read\n\r");
//            sprintf(log_str,"SD read failed at %d\n\r",lno);
//            print(log_str);
//            break;
//        }
//		e = (eps_data_t*)&test;
//
//        sprintf(log_str,"VC1 Bus1 voltage: %f\n\r",e->vc1v1);
//        print(log_str);
//        log_str[0] = '\0';
//        sprintf(log_str,"VC1 Bus2 voltage: %f\n\r",e->vc1v2);
//        print(log_str);
//        log_str[0] = '\0';
//        sprintf(log_str,"VC1 Bus3 voltage: %f\n\r",e->vc1v3);
//        print(log_str);
//        log_str[0] = '\0';
//        sprintf(log_str,"VC2 Bus1 voltage: %f\n\r",e->vc2v1);
//        print(log_str);
//        log_str[0] = '\0';
//        sprintf(log_str,"VC2 Bus2 voltage: %f\n\r",e->vc2v2);
//        print(log_str);
//        log_str[0] = '\0';
//        sprintf(log_str,"VC2 Bus3 voltage: %f\n\r",e->vc2v3);
//        print(log_str);
//        log_str[0] = '\0';
//        sprintf(log_str,"VC3 Bus1 voltage: %f\n\r",e->vc3v1);
//        print(log_str);
//        log_str[0] = '\0';
//        sprintf(log_str,"VC3 Bus2 voltage: %f\n\r",e->vc3v2);
//        print(log_str);
//        log_str[0] = '\0';
//        sprintf(log_str,"VC3 Bus3 voltage: %f\n\r",e->vc3v3);
//        print(log_str);
//        log_str[0] = '\0';
//        sprintf(log_str,"SoC: %f\n\r",e->soc);
//        print(log_str);
//        log_str[0] = '\0';
//        lno--;
//        if(lno < 150) {
//        	lno = 199;
//        }
//        k++;
//        if(k >10) {
//        	break;
//        }
//    }
//    return 0;
//}
//

