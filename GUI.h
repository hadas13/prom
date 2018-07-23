#ifndef GUI_H_
#define GUI_H_

#define NOT_VALID 0
#define ERROR -1
#define VALID 1 

#define CONTAINS_ERRORNEOUS_ERR "Error: board contains errorneous values\n"
#define CELL_FIXED_ERR		"Error: cell is fixed\n"
#define UNSOLVABLE_ERR		"Error: board is unsolvable\n"


/*********** functions ***********/
int print_errorneous_err();

int print_cell_fixed_err();

int print_unsolvable_err();

int print_num_of_solutions(int num);

int print_calloc_failed_err();
