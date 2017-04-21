/*
 * Yash Bhutwala
 * CS 315 Lab 12
 * 12/1/2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define CMD_SEPARATOR ";"
#define ARG_SEPARATOR " \n"

#define ARGS_LEN 10
#define CMD_LEN 100
#define HIST_LEN 10

#define EXIT "exit"
#define ENTER "\n"
#define CD "cd"
#define HISTORY "history"
#define DATE "date"

struct hist_item {
	char item[CMD_LEN];
	int index;
};

/*
 * Takes input string and tokenizes it into the program + arguments.
 * If there is more than one command (i.e: the input string contains semicolons)
 * then this function will break it up and execute each program + arguments
 * one at a time until there are no more things to execute.
 */
void tokenize_and_exec(char* cmd);

/*
 * Wrapper for the strncmp function for readability
 */
int equals(const char* cmd, const char* other);

/*
 * Provides the built-in cd capability
 * 
 */
void change_dir(char* path);

/*
 * Takes a command as an argument. If the command is not null, it
 * saves it to the list of history items; if it is null, the function prints
 * the last HIST_LEN items
 */
void history(const char* cmd);

/*
 * Prints the current time in an human readable format.
 */
void get_date(void);

