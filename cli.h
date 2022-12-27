#ifndef CLI_H
#define CLI_H

#include<stdint.h>
#include<string.h>
#include"AHAN.h"
#include"commands.h"


/* The command structure *//**
 * @brief This structure stores the information about a command
 * cid: The command id
 * name: The name of the command
 * work: The function attached to the command
 */
typedef struct {
    uint8_t cid;
    char name[MAX_CMD_NAME_LEN];
    uint8_t (*work)(void);
}cli_t;

/* Command list array *//**
 * @brief This array contains all the commands which can be used. To add 
 * a new command, add a new entry to this array using the add_command function
 * 
 */
extern cli_t command_list[MAX_NUMBER_OF_COMMANDS];

/* Command string *//**
 * @brief The input command string is stored in this variable
 * 
 */
extern char cli_str[MAX_CMD_NAME_LEN+1];

/* Input function for the CLI *//**
 * @brief Used to get the input from the CLI using CoreUART
 * 
 * @return uint8_t 
 * returns 1 if a carriage return is detected, 0 otherwise
 */
uint8_t getip_cli(void);

/* Add command function *//**
 * @brief This functions adds a new command by adding a new entry to the command_list array.
 * Use this function to add a new command
 * 
 * @param name : The name of the command
 * @param work : The function associated with the command
 */
void add_command(char *name,uint8_t (*work)(void));

/* CLI Initailisation function *//**
 * @brief This function calls the add_function routine to add all the commands and set the command_list array
 * 
 */
void cli_init();

/* Command line routine *//**
 * @brief This is the main command line function. Call this in main() to start the CLI
 * This is a blocking function
 */
void cli();

/* Command line task routine *//**
 * @brief This is used to run the CLI as a task along with other tasks in the scheduler.
 * 
 */
uint8_t cli_task(void);



#endif
