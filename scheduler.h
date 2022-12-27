#ifndef SCHEDULER_H
#define SCHEDULER_H

#include"task.h"
#include"peripherals.h"
#include"AHAN.h"
#include"SD.h"
#include"cli.h"
#include"eps.h"

#define TIMER_CLK                   50000000
#define TIMER_CLK_PER_SECOND        TIMER_CLK
#define TIMER_CLK_PER_MS            (TIMER_CLK_PER_SECOND/1000)
// The task list array
extern task_struct_t run_queue[MAX_TASKS];

//The logs array
extern log_t logs[MAX_LOG_SIZE];

/* Scheduler Initialisation routine *//**
 * @brief Initialises the scheduler variables
 * 
 */
void scheduler_init();

/* Add task routine *//**
 * @brief This function adds a new entry to the task list array. Use this function to add a new task
 * 
 * @param id:       task id
 * @param name:     name of the task
 * @param work:     the function associated with the task
 * @param mode:     the mode in which the task runs 
 * @param period:	the period of repetition of the task
 * @param active:	active flag
 */
void add_task(uint8_t id,char *name,uint8_t (*work)(void), uint8_t mode,uint32_t period,uint8_t active);

/* Store logs function *//**
 * @brief This function is used to store the contents of logs array to the SD card
 * 
 */
uint8_t store_logs(void);

/* Task addition function *//**
 * @brief This function calls the add_task function for all the tasks to add them to the task list array
 * 
 */
void task_addition();

/* Scheduler function*//**
 * @brief This is the scheduler function, in which the task list is looped through and each task is run depending on the conditions
 * 
 */
void scheduler();
#endif
