#ifndef TASK_H
#define TASK_H

#include<stddef.h>
#include<stdint.h>
#include<string.h>
#include<drivers/mss_rtc/mss_rtc.h>
#define MODE1 0x1
#define MODE2 0x2
#define MODE3 0x4
#define MODE4 0x8

#define MAX_TASKS           10
#define MAX_LOG_SIZE        10

//typedef struct task_struct task_struct_t;
//typedef struct log log_t;
/* Task Structure *//**
 * @brief This structure stores the information about a particular task
 * id: task id
 * name: name of the task
 * work: the function associated with the task
 * mode: the mode in which the task can be carried out
 * period: the time period after which the task should be run
 * current_count: the last count of the timer when the task was run
 * active: set to zero if the task should not be run
 */
typedef struct task_struct{
    int id;
    char name[20];
    uint8_t (*work)(void);
    uint8_t mode;
    uint32_t period_L;
    uint32_t period_H;
    uint32_t last_count_L;
    uint32_t last_count_H;
    uint8_t active;
} task_struct_t;


//extern task_struct_t run_queue[MAX_TASKS];

/* Log structure *//**
 * @brief This structure is used to store the logs of the tasks run
 * id: the id of the task 
 * status: the return status of the task
 * start_time: the start time of the task
 * end_time: the end time of the task
 */
typedef struct log{
    uint8_t id;
	uint8_t status;
    mss_rtc_calendar_t start_time;
    mss_rtc_calendar_t end_time;
} log_t;


//extern log_t logs[MAX_LOG_SIZE];



#endif
