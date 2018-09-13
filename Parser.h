/*
 * an header file that includes the methods used to parse use commands
 */

#ifndef PARSER_H_
#define PARSER_H_

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

/*
 * new structure containing the command type, int arguments if necessary and string arg if necessary
 * and indication if overall command is valid
 */

struct Command{
	int command; /* command type */
	/* command arguments */
	int X;
	int Y;
	int Z;
	char path[256];
	int valid; /* indication if command is valid according to type and number of arguments needed */
};

/*
 * get_command receives a command from stdin and returns a Command structure
 * representing the relevant parameters given by the user such as the command type as int,
 * any int arguments if needed, any string arguments if needed and validity index
 */

struct Command get_command();

/*
 * Function that receives a Command structure with a specific command type and a char array from stdin
 * that requires additional arguments and updates them accordingly
 */

void get_args(struct Command *command, char *tokens);

#endif /* PARSER_H_ */
