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

int print_err_unsolvable();

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

int print_err_invalid_command();

int print_err_row_col_not_in_range();

int print_err_board_contains_error();

int print_err_cell_assigned();

int print_err_no_moves_to_undo();

int print_undo_move(Move_l *curr_move);

int print_err_no_moves_to_redo();

int print_redo_move(Move_l *curr_move);

int print_number_of_solutions(int num_sol);

int print_notify_one_solution();

int print_notify_more_solutions();

int print_err_cant_created_or_modified();

int print_err_board_validate_failed();

int print_save_file_to();

int print_err_value_not_int_range_E(int e);

int print_err_board_not_empty();

int print_err_generator_failed();

int print_hint(int clue);
#endif /* GAME_H_ */
