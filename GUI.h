#ifndef GUI_H_
#define GUI_H_

#include "defines.h"

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

int print_malloc_failed_err();

int print_board_contains_error();

void print_frame(Board *board);

int print_cell(Cell cell, int mark_err);

void print_board(Board *board, int mark_err);

int print_err_is_mark_val();

int print_err_value_not_in_range();

int print_err_cell_is_fixed();

int print_err_row_col_not_in_range();

int print_err_board_contains_error();

int print_err_cell_assigned();
#endif /* GAME_H_ */
