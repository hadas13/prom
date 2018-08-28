#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "MainAux.h"


/**** MACROS ****/

#define Command_Size 1025

/* COMMAND MACROS */

#define SET_COM "set"
#define HINT_COM "hint"
#define VALIDATE_COM "validate"
#define RESTART_COM "restart"
#define EXIT_COM "exit"

/* COMMAND INDEXES MACROS */

#define SET 1
#define HINT 2
#define VALIDATE 3
#define RESTART 4
#define EXIT 5
#define BLANK_LINE 9

/*
 * structure to represent a command
 */

struct Command{
	int command; /* command type */
	/* command arguments */
	int X;
	int Y;
	int Z;
	int valid; /* indication if command is valid */
};

void get_args(struct Command *command, char *tokens){
	/* Set command, receiving 3 int args or indicating the command is invalid. valid equals to 0 */
	/* if any one argument is invalid */
	if(command->command == SET){
		if (tokens != NULL){
			command->X = string_to_int(tokens);
			tokens = strtok(NULL, " \t\r\n");
			if (tokens != NULL){
				command->Y = string_to_int(tokens);
				tokens = strtok(NULL, " \t\r\n");
				if (tokens != NULL){
					command->Z = string_to_int(tokens);
					command->valid = 1;
				}
				else{
					command->valid = 0;
				}
			}
			else{
				command->valid = 0;
			}
		}
		else{
			command->valid = 0;
		}
	}
	/* Hint command, receiving 2 int args or indicating the command is invalid. valid equals to 0 */
	/* if any one argument is invalid */
	else{
		if (tokens != NULL){
			command->X = string_to_int(tokens);
			tokens = strtok(NULL, " \t\r\n");
			if (tokens != NULL){
				command->Y = string_to_int(tokens);
				command->valid = 1;
			}
			else{
				command->valid = 0;
			}
		}
		else{
			command->valid = 0;
		}
	}
}

struct Command get_command(){
	struct Command com;
	char command_line[Command_Size];
	char *tokens;

	fgets(command_line, sizeof(command_line), stdin);
	tokens = strtok(command_line, " \t\r\n");
	if (feof(stdin)){ /* EOF received */
		com.command = EXIT;
		com.valid = 1;
		return (com);
	}

	if(tokens == NULL){
		 /* blank line received */
		com.command = BLANK_LINE;
		com.valid = 1;
		return (com);
	}
	else if(strcmp(tokens, SET_COM) == 0){ /* set command */
		com.command = SET;
		tokens = strtok(NULL, " \t\r\n");
		get_args(&com, tokens);
		return (com);
	}
	else if(strcmp(tokens, HINT_COM) == 0){ /* hint command */
		com.command = HINT;
		tokens = strtok(NULL, " \t\r\n");
		get_args(&com, tokens);
		return (com);
	}
	else if(strcmp(tokens, VALIDATE_COM) == 0){ /* validate command */
		com.command = VALIDATE;
		com.valid = 1;
		return (com);
	}
	else if(strcmp(tokens, RESTART_COM) == 0){ /* restart command */
		com.command = RESTART;
		com.valid = 1;
		return (com);
	}
	else if(strcmp(tokens, EXIT_COM) == 0){ /* exit command */
		com.command = EXIT;
		com.valid = 1;
		return (com);
	}
	else{ /* invalid command */
		com.valid = 0;
		return (com);
	}
}
