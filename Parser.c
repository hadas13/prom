#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "MainAux.h"
#include "SPBufferset.h"



/**** MACROS ****/

#define Command_Size 1025

/* COMMAND MACROS */

#define SET_COM "set"
#define HINT_COM "hint"
#define VALIDATE_COM "validate"
#define EXIT_COM "exit"
#define SOLVE_X_COM "solve"
#define EDIT_X_COM "edit"
#define MARK_ERRORS_X_COM "mark_errors"
#define PRINT_BOARD_COM "print_board"
#define UNDO_COM "undo"
#define REDO_COM "redo"
#define SAVE_COM "save"
#define NUM_SOLUTIONS_COM "num_solutions"
#define AUTOFILL_COM "autofill"
#define RESET_COM "reset"
#define GENERATE_X_Y_COM "generate"

/* COMMAND INDEXES MACROS */

#define SET 1
#define HINT 2
#define VALIDATE 3
#define EXIT 5
#define BLANK_LINE 9
#define SOLVE 4
#define EDIT 6
#define MARK_ERRORS 7
#define PRINT_BOARD 8
#define UNDO 10
#define REDO 11
#define SAVE 12
#define NUM_SOLUTIONS 13
#define AUTOFILL 14
#define RESET 15
#define GENERATE 16

void get_args(struct Command *command, char *tokens){
	/* get numeric args for mark_errors, set, generate and hint commands */
	if (command->command == MARK_ERRORS || command->command == SET || command->command == GENERATE || command->command == HINT){
		if (tokens != NULL){ /* first arg */
			if(is_numeric_string(tokens)){
				command->X = string_to_int(tokens);
				if (command->command != MARK_ERRORS){ /* set, generate of hint commands */
					tokens = strtok(NULL, " \t\r\n");
					if (tokens != NULL){ /* second arg */
						if (is_numeric_string(tokens)){
							command->Y = string_to_int(tokens);
							if (command->command == SET){ /* set command */
								tokens = strtok(NULL, " \t\r\n");
								if (tokens != NULL){ /* third arg */
									if (is_numeric_string(tokens)){
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
	else{ /* get string arg for solve, edit or save commands */
		if (tokens != NULL){
			strcpy(command->path, tokens);
			command->valid = 1;
		}
		else if (command->command == EDIT){
			command->valid = 1;
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

	if (fgets(command_line, sizeof(command_line), stdin) == NULL){

		if (feof(stdin)){ /* EOF received */
				com.command = EXIT;
				com.valid = 1;
				return (com);
		}
		else{ /* error in fgets */
			printf("Error: fgets has failed\n");
			exit(0);
		}
	}
	else if (strlen(command_line) > 256){ /* input longer than 256 chars */
			com.valid = 0;
			return (com);
	}

	tokens = strtok(command_line, " \t\r\n");

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
	else if(strcmp(tokens, MARK_ERRORS_X_COM) == 0){ /* mark_errors command */
		com.command = MARK_ERRORS;
		tokens = strtok(NULL, " \t\r\n");
		get_args(&com, tokens);
		return (com);
	}
	else if(strcmp(tokens, GENERATE_X_Y_COM) == 0){ /* generate command */
		com.command = GENERATE;
		tokens = strtok(NULL, " \t\r\n");
		get_args(&com, tokens);
		return (com);
	}
	else if(strcmp(tokens, VALIDATE_COM) == 0){ /* validate command */
		com.command = VALIDATE;
		com.valid = 1;
		return (com);
	}
	else if(strcmp(tokens, EXIT_COM) == 0){ /* exit command */
		com.command = EXIT;
		com.valid = 1;
		return (com);
	}
	else if(strcmp(tokens, SOLVE_X_COM) == 0){ /* solve command */
		com.command = SOLVE;
		tokens = strtok(NULL, " \t\r\n");
		get_args(&com, tokens);
		return (com);
	}
	else if(strcmp(tokens, EDIT_X_COM) == 0){ /* edit command */
		com.command = EDIT;
		tokens = strtok(NULL, " \t\r\n");
		get_args(&com, tokens);
		return (com);
	}
	else if(strcmp(tokens, SAVE_COM) == 0){ /* save command */
		com.command = SAVE;
		tokens = strtok(NULL, " \t\r\n");
		get_args(&com, tokens);
		return (com);
	}
	else if(strcmp(tokens, PRINT_BOARD_COM) == 0){ /* print board command */
		com.command = PRINT_BOARD;
		com.valid = 1;
		return (com);
	}
	else if(strcmp(tokens, UNDO_COM) == 0){ /* undo command */
		com.command = UNDO;
		com.valid = 1;
		return (com);
	}
	else if(strcmp(tokens, REDO_COM) == 0){ /* redo command */
		com.command = REDO;
		com.valid = 1;
		return (com);
	}
	else if(strcmp(tokens, NUM_SOLUTIONS_COM) == 0){ /* number of solutions command */
		com.command = NUM_SOLUTIONS;
		com.valid = 1;
		return (com);
	}
	else if(strcmp(tokens, AUTOFILL_COM) == 0){ /* autofill command */
		com.command = AUTOFILL;
		com.valid = 1;
		return (com);
	}
	else if(strcmp(tokens, RESET_COM) == 0){ /* reset command */
		com.command = RESET;
		com.valid = 1;
		return (com);
	}
	else{ /* invalid command */
		com.valid = 0;
		return (com);
	}
}
