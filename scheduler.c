//#include"scheduler.h"
//
//#define EPS_HK_TASK_ID          3
//#define GMC_TASK_ID             0
//
//#define EPS_HK_F1               50
//#define EPS_HK_F2               1000
//uint8_t current_mode;
//uint8_t task_counter;
//task_struct_t run_queue[MAX_TASKS];
//uint8_t current_task_status;
//
//uint8_t log_counter;
//
//log_t logs[MAX_LOG_SIZE];
//
//uint8_t task_idx;
//
//uint8_t timer_epsilon;
//
//uint32_t timer_lower_value,timer_upper_value;
//
//void scheduler_init() {
//    task_idx = 0;
//    task_counter = 0;
//    current_mode = MODE1;
//    log_counter = 0;
//    timer_epsilon = 10;
//    timer_lower_value = 0;
//    timer_upper_value = 0;
//    //timer_value = 0;
//}
//
//void add_task(uint8_t id,char *name,uint8_t (*work)(void), uint8_t mode,uint32_t period,uint8_t active) {
//    run_queue[task_idx].id = id;
//    run_queue[task_idx].mode = mode;
//    strcpy(run_queue[task_idx].name,name);
//    run_queue[task_idx].work = work;
//    time_to_count(period,&run_queue[task_idx].period_H,&run_queue[task_idx].period_L);
//    run_queue[task_idx].active = active;
//    task_idx++;
//}
//
//uint8_t store_logs(void) {
//    uint8_t logi = 0;
//    for(;logi<MAX_LOG_SIZE;logi++) {
//        SD_Write(current_block.task_logs*512,(uint8_t*)(&logs[logi]));
//        current_block.task_logs++;
//    }
//    return 0;
//}
//
//void task_addition() {
//    add_task(0,"GMC_INIT_COUNT",&gmc_count_init,MODE3,(5000),1);
//    add_task(1,"GMC_GET_COUNT",&gmc_get_count,MODE3,(5000),1);
//    add_task(2,"CLI",&cli_task,MODE1 | MODE2 | MODE3,(1000),2);
//    add_task(3,"EPS_HK",&get_voltages,MODE1 | MODE2 | MODE3,(5000),1);
//}
//
//void timer_64_init() {
//    MSS_TIM64_init(MSS_TIMER_PERIODIC_MODE);
//    MSS_TIM64_load_background(0xFFFFFFF,0xFFFFFFFF);
//    uint8_t i = 0;
//    for(;i<task_idx;i++) {
//        run_queue[i].last_count_L = 0xFFFFFFFF;
//        run_queue[i].last_count_H = 0xFFFFFFFF;
//    }
//    MSS_TIM64_start();
//}
//
//uint8_t can_run(uint8_t i) {
//    if(!((current_mode | run_queue[i].mode))) {
//        return 1;
//    }
//    if(!(run_queue[i].active)) {
//        return 1;
//    }
//    if(run_queue[i].active == 2) {
//        return 0;
//    }
//    if(run_queue[i].id == 3) {
//    	uint8_t k = 0;
//    }
//    MSS_TIM64_get_current_value(&timer_upper_value,&timer_lower_value);
//    if((run_queue[i].last_count_L - timer_lower_value >= run_queue[i].period_L) && (run_queue[i].last_count_H - timer_upper_value >= run_queue[i].period_H)) {
//        return 0;
//    }
//    return 1;
//}
//
//void config_science_mode() {
//    //set current mode to mode 3
//    current_mode = MODE3;
//
//    //set eps hk time interval
//    time_to_count(EPS_HK_F1,&run_queue[EPS_HK_TASK_ID].period_H,&run_queue[EPS_HK_TASK_ID].period_L);
//
//}
//
//void config_safe_mode() {
//    //set current mode to mode 2
//    current_mode = MODE2;
//
//    //set eps hk time interval
//    time_to_count(EPS_HK_F1,&run_queue[EPS_HK_TASK_ID].period_H,&run_queue[EPS_HK_TASK_ID].period_L);
//}
//
//void config_phoenix_mode() {
//    //set current mode to mode 1
//    current_mode = MODE1;
//
//    //set eps hk time interval
//    time_to_count(EPS_HK_F2,&run_queue[EPS_HK_TASK_ID].period_H,&run_queue[EPS_HK_TASK_ID].period_L);
//}
//
//void check_mode() {
//    double soc = get_soc();
//    if(soc < SOC_TH1) {
//        config_phoenix_mode();
//    } else if(soc > SOC_TH1 && soc < SOC_TH2) {
//        config_safe_mode();
//    } else {
//        config_science_mode();
//    }
//}
//void scheduler() {
//	AHAN_Init();
//    eps_init();
//    scheduler_init();
//    task_addition();
//    memory_init();
//    cli_init();
//    timer_64_init();
//    while(1) {
//        if(can_run(task_counter) == 0) {
//            logs[log_counter].id = run_queue[task_counter].id;
//            MSS_RTC_get_calendar_count(&logs[log_counter].start_time);
//            current_task_status = run_queue[task_counter].work();
//            run_queue[task_counter].last_count_L = timer_lower_value;
//            run_queue[task_counter].last_count_H = timer_upper_value;
//            MSS_RTC_get_calendar_count(&logs[log_counter].end_time);
//            logs[log_counter].status = current_task_status;
//            log_counter++;
//            if(log_counter >= MAX_LOG_SIZE) {
//                store_logs();
//                log_counter = 0;
//            }
//        }
//        task_counter = (task_counter + 1)%task_idx;
//        //check_mode();
//    }
//}
