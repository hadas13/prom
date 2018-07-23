#ifndef PARSER_H_
#define PARSER_H_

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
 * new structure to mimic tuple containing the command type, int arguments if necessary
 * and indication if overall command is valid
 */

struct Command{
	int command; /* command type */
	/* command arguments */
	int X;
	int Y;
	int Z;
	int valid; /* indication if command is valid */
};

/*
 * get_command receives a command from stdin and returns a Command structure
 * representing the relevant parameters given by the user such as the command type as int,
 * any int arguments if needed for specific commands and validity index
 */

struct Command get_command();

/*
 * Function that receives a Command structure with a specific command type and a char array from stdin
 * that requires additional arguments and updates them accordingly
 */

void get_args(struct Command, char *tokens);

#endif /* PARSER_H_ */
