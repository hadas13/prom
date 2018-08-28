#include "Solver.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>


int free_matrix(Cell **mat, int n){
	int i;

	for (i = 0; i < n; i++) {
		free(mat[i]);
	}

	free(mat);
	return VALID;
}

int free_board(Board *board){
	free_matrix(board->game_table, board->n);
	free(board);

	return VALID;
}
	

int insert_fixed_cell(Board *board){
	int counter = 0;
	int num_fixed = board->filled;
	int n = board->n;
	int ran_col = 0;
	int ran_row = 0;

	while (counter != num_fixed){
		/* insert fixed cells */
		ran_col = rand() % n; 
		ran_row = rand() % n;

		if (board->game_table[ran_row][ran_col].val == UNASSIGNED){
			/* empty cell - insert fixed num */
			board->game_table[ran_row][ran_col].val = board->solution[ran_row][ran_col].val;
			board->game_table[ran_row][ran_col].is_fixed = TRUE;
			counter++;
		}
	}
	return VALID;
}


int init_game_table(Board *board){
	int i = 0;
	int n = board->n;

	board->game_table = calloc(n, sizeof *board->game_table);
	if (board->solution == NULL) {
		print_calloc_failed_err();
		free_board(board);
		exit(0);
	}

	for (i = 0; i < n; i++){
	       board->game_table[i] = (Cell *)calloc(n, sizeof(*board->game_table[i]));	
	       if (board->game_table[i] == NULL) {
		       print_calloc_failed_err();
		       free_board(board);
		       exit(0);
	       }
	}

	insert_fixed_cell(board);
	return VALID;
}


int *get_num_block_position(int row_pos, int col_pos, int m_rows, int m_cols){
	int *ret_arr = (int *) malloc(sizeof (int) * 2);

	if (ret_arr == NULL) {
	       printf("Error: malloc has failed\n");
	       free(ret_arr);
	       exit(0);
       }

	ret_arr[0] = (row_pos / m_rows);
	ret_arr[1] = (col_pos / m_cols);
	return ret_arr;
}

int is_row_valid(Board board, Cell **board_to_check, int row, int col_p, int number){
	int col = 0;

	for (col = 0; col < board.n; col++){
		if (col_p == col) {
			/* the same cell as checked */
			continue;
		}
		else if (board_to_check[row][col].val == number){
			return NOT_VALID;
		}
	}

	return VALID;
}



int is_col_valid(Board board, Cell **board_to_check, int row_p, int col, int number){
	int row = 0;

	for (row = 0; row < board.n; row++){
		if (row == row_p) {
			/* the same cell as checked */
			continue;
		}

		else if (board_to_check[row][col].val == number){
			return NOT_VALID;
		}
	}

	return VALID;
}

int is_block_valid(Board board, Cell **board_to_check, int num, int n_row_p, int n_col_p){
	int r = 0;
	int c = 0;
	int start_r = 0;
	int start_c = 0;
	int *block_pos = NULL;

	block_pos = get_num_block_position(n_row_p, n_col_p, board.m_rows, board.m_cols);
	start_r = block_pos[0] * board.m_rows;
	start_c = block_pos[1] * board.m_rows;

	/* check if legal in block */ 
	for (r = start_r; r < start_r + board.m_rows; r++){
		for (c = start_c; c < start_c + board.m_cols; c++){
			if (r == n_row_p && c == n_col_p) {
				/* the same cell as checked */
				continue;
			}
			else if (board_to_check[r][c].val == num){
				/* there is num in the block */
				free(block_pos);
				return NOT_VALID;
			}
		}
	}

	free(block_pos);
	return VALID;
}


int is_cell_valid(Board *board, Cell **board_to_check, int num, int n_row_p, int n_col_p){

	/* check if legal in row and column */
	if (!is_row_valid(*board, board_to_check, n_row_p, n_col_p, num) || 
			!is_col_valid(*board, board_to_check, n_row_p, n_col_p, num) ||
		       	!is_block_valid(*board, board_to_check, num, n_row_p, n_col_p)){
		return NOT_VALID;
	}

	return VALID;
}

int solve_cell_deterministically(Cell **check_board, Board *board, int row, int column){
	int next_num = 0;
	int n = board->n;

	if (n == row) {
		/* end of insertion */
		return VALID;
	}

	if (check_board[row][column].val != UNASSIGNED) {
		if (column == n - 1) {
			if (solve_cell_deterministically(check_board, board, row + 1, 0)){
				return VALID;
			}
		}
		else {
			if (solve_cell_deterministically(check_board, board, row, column + 1)){
				return VALID;
			}
		}
		return NOT_VALID;
	}

	for (next_num = 1; next_num < n + 1; next_num++){
		if (is_cell_valid(board, check_board, next_num, row, column)){
			check_board[row][column].val = next_num;
			
			if (column == (n - 1)) {
				 /*we got to last column in the row - turn to the next row */
				if (solve_cell_deterministically(check_board, board, row + 1, 0)) {
					return VALID;
				}
				
			}
			else {
				/*not last column - continue on the same row */
				if (solve_cell_deterministically(check_board, board, row, column + 1)){
					return VALID;
				}
			}

			/* didn't find a valid value for this cell */
			check_board[row][column].val = UNASSIGNED;
		}
	}

	return NOT_VALID;
}


int get_possible_vals(Board *board, Cell **check_board, int row_pos, int col_pos, int *arr){
	int index = 0;
	int counter = 0;

	if (arr == NULL){
		return NOT_VALID;
	}

	for (index = 0; index < board->n; index++){
		if (is_cell_valid(board, check_board, index + 1, row_pos, col_pos)){
			arr[index] = 1;
			counter++;
		}

		else{
			/* illegal number */ 
			arr[index] = 0;
		}
	}

	arr[board->n] = counter;

	return VALID;
}


int is_full(Cell **board_to_check, int n){
	int i = 0;
	int j = 0;

	for (i = 0; i < n; i++){
		for (j = 0; j < n; j++){
			if (board_to_check[i][j].val == UNASSIGNED){
				return NOT_VALID;
			}
		}
	}

	return VALID;
}


int get_first_option(int *cell_arr, int n, int from){

	for (; from < n + 1; from++){
		if (cell_arr[from] == 1){
			return (from + 1);
		}
	}

	return NOT_VALID;
}

/*
 * @param num_arr: num_arr_[n] - number of options.
 * @param n: number of options.
 * @return value: the next random number 
 */
int get_random_option(int *num_arr, int n){
	int random_pos = 0;
	int i = 0;
	int legal_digit_num = num_arr[n];
	int count_ones = 0;

	random_pos = rand() % legal_digit_num;

	for (i = 0; i < n; i++){
		if (count_ones == random_pos){
			/* the next 1 in the array is the random number */
			return get_first_option(num_arr, n, i);
		}

		else if (num_arr[i] == 1) {
			count_ones++;
		}
	}

	return NOT_VALID;
}


int update_num_arr(int *num_arr, int n, int next_num){
	num_arr[n]--;
	num_arr[next_num - 1] = 0;
	return VALID;
}


int solve_cell_randomly(Board *board, int row, int column) {
	int *num_arr = NULL;
	int end_flag = -1;
	int n = board->n;
	int next_num = 0;

	if (n == row) {
		/* end of insertion */
		return VALID;
	}

	if (board->solution[row][column].val != UNASSIGNED) {
		/* cell is already set - insert to solution board */
		if (column == n - 1) {
			if (solve_cell_randomly(board, row + 1, 0)){
				free(num_arr);
				return VALID;
			}
		} 
		else {
			if (solve_cell_randomly(board, row, column + 1)){
				free(num_arr);
				return VALID;
			}
		}
		return NOT_VALID;
	}

	num_arr = (int *)calloc((n + 1), sizeof(int));
	if (num_arr == NULL) {
		printf("Error: calloc has failed\n");
		free_board(board);
		exit(NOT_VALID);
	}

	get_possible_vals(board, board->solution, row, column, num_arr);
	while(end_flag != 0){
		switch(num_arr[n]) { /* num_arr[n] is the counter of possible nums for the cell*/
			case (NON_OPTION):
				/* no possible number for this cell */
				free(num_arr);
				board->solution[row][column].val = 0;
				end_flag = 0;
				return NOT_VALID;

			case (SINGLE_OPTION):
				next_num = get_first_option(num_arr, board->n, 0);
				break;

			default:
				/* more then one possible option - choose randomly a number */
				next_num = get_random_option(num_arr, n);
				break;
		}
	
		board->solution[row][column].val = next_num;
		board->solution[row][column].is_err = FALSE;
		board->solution[row][column].is_fixed = FALSE;
		
		if (column == (n - 1)) {
			 /*we got to last column in the row - turn to the next row */
			if (solve_cell_randomly(board, row + 1, 0)) {
				free(num_arr);
				return VALID;
			}
			
		}
		else {
			if (solve_cell_randomly(board, row, column + 1)){
				free(num_arr);
				return VALID;
			}
		}

		/* didn't find a valid value for this cell */
		board->solution[row][column].val = UNASSIGNED;
		board->solution[row][column].is_err = FALSE;
		board->solution[row][column].is_fixed = FALSE;
		update_num_arr(num_arr, n, next_num);
	}

	free(num_arr);
	return NOT_VALID;
}


int init_solution_board(Board *board){
	int i = 0;
	int j = 0;
	int n = board->n;

	board->solution = calloc(n, sizeof *board->solution);
	if (board->solution == NULL) {
		printf("Error: calloc has failed\n");
		free(board);
		exit(0);
	}

	for (i = 0; i < n; i++){
	       board->solution[i] = calloc(n, sizeof(*board->solution[i]));	
		if (board->solution[i] == NULL) {
			printf("Error: calloc has failed\n");
			free_board(board);
			exit(0);
		}
	}

	for (i = 0; i < n; i++){
		for (j = 0; j < n; j++){
			if (solve_cell_randomly(board, i, j) && is_full(board->solution, n)){
				return VALID;
			}
		}
	}

	return NOT_VALID;
}


Board *init_board(int n, int m_rows, int m_cols, int fixed_nums){
	Board * board = NULL;

	board = malloc(sizeof(Board));
	if (board == NULL){
		print_malloc_failed_err();
		free_board(board);
		return NOT_VALID;
	}

	board->n = n;
	board->m_rows = m_rows;
	board->m_cols = m_cols;
	board->filled = fixed_nums;
	if (init_solution_board(board) != VALID){
		free_board(board);
		return NULL;
	}

	if (init_game_table(board) != VALID){
		free_board(board);
		return NULL;
	}

	return board;
}

int copy_board(Cell **source_b, Cell **dest_b, int len){
	int i = 0;
	int j = 0;

	if (source_b == NULL || dest_b == NULL) {
		return NOT_VALID;
	}

	for (i = 0; i < len; i++) {
		for (j = 0; j < len; j++) {
			/* copy cell struct from source to dest */
			dest_b[i][j].val = source_b[i][j].val;
			dest_b[i][j].is_fixed = source_b[i][j].is_fixed;
			dest_b[i][j].is_err = source_b[i][j].is_err;
		}
	}

	return VALID;
}

int is_solvable(Board *board, Cell **validation_b) {
	int row = 0;
	int col = 0;
	int n = board->n;


	/* check if the board is valid */
	for (row = 0; row < n; row++){
		for (col = 0; col < n; col++){
			if (solve_cell_deterministically(validation_b, board, row, col) && is_full(validation_b, n)){
				return VALID;
			}
		}
	}

	return NOT_VALID;
}


int validate_game_table(Board *board) {
	Cell **validation_board = NULL;
	int i = 0;

	validation_board = calloc(board->n, sizeof *validation_board);
	if (validation_board == NULL) {
		printf("Error: calloc has failed\n");
		free_board(board);
		exit(NOT_VALID);
	}

	for (i = 0; i < board->n; i++){
	       validation_board[i] = (Cell *)calloc(board->n, sizeof(*validation_board[i]));	
	       if (validation_board[i] == NULL) {
		       printf("Error: calloc has failed\n");
		       free_board(board);
		       exit(NOT_VALID);
	       }
	}

	copy_board(board->game_table, validation_board, board->n);

	if (is_solvable(board, validation_board) != VALID ) {
		/* the game_table is not a valid board */
		free_matrix(validation_board, board->n);
		return NOT_VALID;
	}

	/* it is solvable */
	copy_board(validation_board, board->solution, board->n);
	free_matrix(validation_board, board->n);
	return VALID;
}
