/* Solver.h
 * 
 * created: 05 2018
 * author: hadas
 */

#ifndef SOLVER_H_
#define SOLVER_H_

/************** solver's includes ***********/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "defines.h"
#include "Game.h"
#include "GUI.h"


/************** solver's MACROS ***********/

#define NOT_VALID     0
#define VALID         1 
#define NON_OPTION    0
#define SINGLE_OPTION 1
#define FIXED_VAL    -1
/************** solver's functions ***********/


/*
 * note: we are holding the fixed numbers as negative numbers.
 * @param num: number
 * @return value: return number as positive number.
 */
int get_positive(int num);


/*
 * @param mat: two-dimensions array to free.
 * @param n: length of the array.
 * @return value: VALID.
 */
int free_matrix(Cell **mat, int n);

/* 
 * freeing all the allocated memory of board
 * @param board: the game struct to free.
 * @return value: after freeing - returns VALID.
 */
int free_board(Board *board);


/* 
 * @param row_pos: row index of num
 * @param col_pos: column index of num
 * @param m_rows: number of rows in block
 * @param m_cols: number of cols in block
 * @return value: returns bloack position array -
 *	[0]:  number of row's block; [1]: number of column's block.
 */
int *get_num_block_position(int row_pos, int col_pos, int m_rows, int m_cols);


/* 
 * @param board: game board.
 * @param board_to_check: two-dimensions array to validate.
 * @param row: rows' index to check.
 * @param col_p: cell row position.
 * @param num: checking this number.
 * @return value: VALID if legal, else - NOT_VALID.
 */
int is_row_valid(Board board, Cell **board_to_check, int row, int col_p, int number);


/* 
 * @param board: game board.
 * @param board_to_check: two-dimensions array to validate.
 * @param row_p: cell row position.
 * @param col: columns' index to check.
 * @param num: checking this number.
 * @return value: VALID if legal, else - NOT_VALID.
 */
int is_col_valid(Board board, Cell **board_to_check, int row_p, int col, int number);


/*
 * @param board: game board.
 * @param board_to_check: two-dimensions array to validate.
 * @param num: checking this number.
 * @param n_row_p: the rows' block position to check.
 * @param n_col_p: the columns' block position to check.
 * @return value: VALID if valid, else - NOT_VALID.
 */
int is_block_valid(Board board, Cell **board_to_check, int num, int n_row_p, int n_col_p);

/*
 * checks if num can be in a specific position
 * @param num: the number that we are checking
 * @param board: game board.
 * @param n_row_p: the num row position.
 * @param n_col_p: the num col position.
 * @param m_cols: block columns' length.
 * @param m_rows: block rows' length.
 * @return value: NOT_VALID if num can't be in this position, else - VALID.
 */
int is_cell_valid(Board *board, Cell **board_to_check, int num, int n_row_p, int n_col_p);

/* 
 * the deterministic solving algorithme.
 * @param board: game board.
 * @param board_to_check: two-dimensions array to validate.
 * @param row: rows' index to solve.
 * @param col: columns' index to solve.
 */
int solve_cell_deterministically(Cell **check_board, Board *board, int row, int column);


/*
 * finds all the possible numbers for game_table[row_pos][col_pos] -
 * if "num" is a possible number, it insert 1 to arr[num].
 * @param board: game struct.
 * @param check_board: the board that we check the numbers to insert to.
 * @param row_pos: number of row for the cell.
 * @param col_pos: number of column for the cell.
 * @param ret_arr: 1's in all the possible positions. ret_arr[n] = num of numbers.
 * @return value: NOT_LEGAL for failure, else - VALID.
 */
int get_possible_vals(Board *board, Cell **check_board, int row_pos, int col_pos, int *arr);


/*
 * @param board_to_check: checks this two-dimensions array.
 * @param n: length of the row or column in the board.
 */
int is_full(Cell **board_to_check, int n);


/* 
 * @param num_arr: array of zeroes and ones.
 * @param n: the length of the possible 1s.
 * @return value: cell's index int the array with the value of 1. not found - NOT_VALID.
 */
int get_first_option(int *opt_arr, int n, int from);


/*
 * @param num_arr: num_arr[n] - number of possible digits for a cell.
 * @param n: length of row or column in the game.
 * @return value: the next random number to insert a cell.
 */
int get_random_option(int *num_arr, int n);


/*
 * updating the possible numbers array for a cell.
 * @param num_arr: array of possible number for cell.
 * @param n: num_arr[n] is number of possible digits.
 * @param next_num: index in num_arr to change to zero
 * @return value: valid.
 */
int update_num_arr(int *num_arr, int n, int next_num);


/* 
 * @param source_b: two-dimensions array of Cells to copy from.
 * @param dest_b: two-dimentions array of Cells to copy to.
 * @param len: number of cells in in each array.
 * @return value: for success - VALID, else - NOT_VALID. 
 */
int copy_board(Cell **source_b, Cell **dest_b, int len);


/*
 * checks if board->game_table can be solved.
 * @param board: game board.
 * @param validation_b: checks if this board is solvable.
 * @return value: VALID if it can be solved, else - NOT_VALID.
 */
int is_solvable(Board *board, Cell **validation_b);


/*
 * @param board: game board.
 * @return value: if valid: update board->solution and return VALID, else - NOT_VALID.
 */
int validate_game_table(Board *board);

#endif
