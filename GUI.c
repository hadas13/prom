#include <stdio.h>
#include "GUI.h"


int print_errorneous_err(){
	printf("Error: board contains errorneous values\n");
	return VALID;
}

int print_cell_fixed_err(){
	printf("Error: cell is fixed\n");
	return VALID;
}

int print_err_unsolvable(){
	printf("Error: board is unsolvable\n");
	return VALID;
}

int print_num_of_solutions(int num){
	if (num < 0){
		return NOT_VALID;
	}

	printf("Number of solutions: %d\n", num);
	if (num == 1){
		/* single solution */
		printf("This is a good board!\n");
	}

	else if (num > 1){
		printf("The puzzle has more that 1 solution, try to edit it further\n");
	}

	return VALID;
}

int print_calloc_failed_err(){
	printf("Error: calloc has failed\n");
	return VALID;
}

int print_malloc_failed_err(){
	printf("Error: malloc has failed\n");
	return VALID;
}

int print_board_contains_error() {
	printf("Error: board contains errorneous values\n");
	return VALID;
}

void print_frame(Board *board){
	/* prints "-" according to number of cells in block */
	int i = 0;
	int num = (((3 * board->m_cols) + 2) * board->m_rows) + 1;
	for (i = 1; i < num; i++){
		printf("-");
	}
	printf("-\n");
}

int print_cell(Cell cell, int mark_err) {
	if (mark_err != FALSE && mark_err != TRUE) {
		return NOT_VALID;
	}

	else if (cell.is_fixed){ /* fixed cell */
		printf(" %2d.", cell.val);
	}

	else if (cell.is_err && mark_err) {
		/* mark errors */
		printf(" %2d*", cell.val);
	}

	else if (cell.val == 0){ /* empty cell */
		printf("   ");
	}

	else{ /* filled a regullar val */
		printf("  %d", cell.val);
	}
	return VALID;
}

void print_board(Board *board, int mark_err){
	int n = board->n; /* num of cells in a row of board */
	int m_c = board->m_cols; /* num of cells in a column of block */
	int m_r = board->m_rows; /* num of cells in a row of block */
	int i = 0;
	int j = 0;

	print_frame(board); /* print the top frame line */
	for (i = 0; i < n; i++){
		printf("|"); /* start a block */
		for (j = 0; j < n;j++){
			print_cell(board->game_table[i][j], mark_err);
			if ((j+1) % m_c == 0){ /* frame of block */
				printf(" |");
			}
		}
		printf("\n");
		if ((i+1) % m_r == 0){ /* end of a block */
			print_frame(board);
		}
	}
}

int print_err_is_mark_val() {
	printf("Error: the value should be 0 or 1\n");
	return VALID;
}

 int print_err_value_not_in_range() {
	 printf("Error: value not in range 0-N\n");
	 return VALID;
 }

int print_err_cell_is_fixed() {
	printf("Error: cell is fixed\n");
	return VALID;
}

int print_err_invalid_command() {
	printf("Error: invalid command\n");
	return VALID;
}

int print_err_row_col_not_in_range() {
	printf("Error: value not in range 1-N\n");
	return VALID;
}

int print_err_board_contains_error() {
	printf("Error: board contains erroneous values\n");
	return VALID;
}

int print_err_cell_assigned() {
	printf("Error: cell already contains a value\n");
	return VALID;
}

int print_err_no_moves_to_undo() {
	printf("Error: no moves to undo\n");
	return VALID;
}

int print_undo_move(MoveInfo move) {
	MoveList *chain;

	if (move.row != EMPTY_VALUE_FOR_INIT || move.col != EMPTY_VALUE_FOR_INIT ||
			move.val_before != EMPTY_VALUE_FOR_INIT ||
			move.val_after != EMPTY_VALUE_FOR_INIT) {
		/* not a subchain of moves */
		printf("Undo %d,%d: from ", move.row, move.col);
		if (move.val_after == UNASSIGNED) {
			printf("_ to ");
		} else {
			printf("%d to ", move.val_after);
		}

		if (move.val_before == UNASSIGNED) {
			printf("_\n");
		} else {
			printf("%d\n", move.val_before);
		}
	} else if (move.subchain != NULL) {
		/* subchain of moves */
		chain = move.subchain;
		while (chain != NULL) {
			print_undo_move(chain->move);
			chain = chain->next;
		}
	}
	return VALID;
}

int print_err_no_moves_to_redo() {
	printf("Error: no moves to redo\n");
	return VALID;
}

int print_redo_move(MoveInfo move) {
	MoveList *chain;

	if (move.row != EMPTY_VALUE_FOR_INIT || move.col != EMPTY_VALUE_FOR_INIT ||
			move.val_before != EMPTY_VALUE_FOR_INIT ||
			move.val_after != EMPTY_VALUE_FOR_INIT) {
		/* not a subchain of moves */
		printf("Redo %d,%d: from ", move.row, move.col);
		if (move.val_before == UNASSIGNED) {
			printf("_ to ");
		} else {
			printf("%d to ", move.val_before);
		}

		if (move.val_after == UNASSIGNED) {
			printf("_\n");
		} else {
			printf("%d\n", move.val_after);
		}
	} else if (move.subchain != NULL) {
		/* subchain of moves */
		chain = move.subchain;
		while (chain != NULL) {
			print_redo_move(chain->move);
			chain = chain->next;
		}
	}
	return VALID;
}

int print_number_of_solutions(int num_sol) {
	printf("Number of solutions: %d\n", num_sol);
	return VALID;
}

int print_notify_one_solution() {
	printf("This is a good board!\n");
	return VALID;
}

int print_notify_more_solutions() {
	printf("The puzzle has more that 1 solution, try to edit it further\n");
	return VALID;
}

int print_err_cant_created_or_modified() {
	printf("Error: File cannot be created or modified\n");
	return VALID;
}

int print_save_file_to(char *path) {
	printf("Saved to: %s\n", path);
	return VALID;
}

int print_err_value_not_int_range_E(int e) {
	printf("Error: value not in range 0-%d\n", e);
	return VALID;
}

int print_err_board_not_empty() {
	printf("Error: board is not empty\n");
	return VALID;
}

int print_err_generator_failed() {
	printf("Error: puzzle generator failed\n");
	return VALID;
}

int print_hint(int clue) {
	printf("Hint: set cell to %d\n", clue);
	return VALID;
}

int print_validation_failed() {
	printf("Validation failed: board is unsolvable\n");
	return VALID;
}

int print_validation_passed() {
	printf("Validation passed: board is solvable\n");
	return VALID;
}
