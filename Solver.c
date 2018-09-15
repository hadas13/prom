#include "Solver.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>


/* function that frees the memory of matrix */
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
	
/*
 * function that find block position of row_pos and col_pos cell. 
 * returns an array of 2 - arr[0]: row block position, arr[1]: column block position
 */
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

/* checks if number is legal in row
 */
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


/* checks if number is legal in column */
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

/* check if block is valid, if num will be insert */
int is_block_valid(Board board, Cell **board_to_check, int num, int n_row_p, int n_col_p){
	int r = 0;
	int c = 0;
	int start_r = 0;
	int start_c = 0;
	int *block_pos = NULL;

	block_pos = get_num_block_position(n_row_p, n_col_p, board.m_rows, board.m_cols);
	start_r = block_pos[0] * board.m_rows;
	start_c = block_pos[1] * board.m_cols;

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

/* check if num is valid in board */
int is_cell_valid(Board *board, Cell **board_to_check, int num, int n_row_p, int n_col_p){
	if (num == UNASSIGNED) {
		/* removing or not changing the value of the cell */
		return VALID;
	}

	/* check if legal in row and column */
	if (!is_row_valid(*board, board_to_check, n_row_p, n_col_p, num) || 
			!is_col_valid(*board, board_to_check, n_row_p, n_col_p, num) ||
		       	!is_block_valid(*board, board_to_check, num, n_row_p, n_col_p)){
		return NOT_VALID;
	}

	return VALID;
}

/* functions that checks which values are legal for row_pos and col_pos, and fills the array
 * numbers in TRUE or FALSE in the index of the number. arr[n] - number of possible numbers 
 */
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
