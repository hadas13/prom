#ifndef GAME_H_
#define GAME_H_

#include <stdio.h>
#include <string.h>

#include "defines.h"
#include "Parser.h"
#include "Solver.h"
#include "GUI.h"
#include "Moves.h"
#include "Backtracking.h"
#include "ILP.h"
#include "MainAux.h"


#define MAX_ITER_GENERATE 1000 /* iterations num of generating x numbers in generate command */
#define NOT_POSSIBLE_VAL -1

/****** functions ********/
/*
 * Function that receives a struct containing the board game and its dimensions
 * and prints the a line of "-" that frames the board
 */
void print_frame(Board *board);


/* 
 * allocate two dim array in board->game_table and filled it with fixed numbers.
 * @param board: game board. insert to board->game_table.
 * @return value: failed allocate - NOT_VALID, else - VALID. 
 */
int init_game_table(Board *board);


/*
 * init board struct and game board.
 * @param n: number of cells in row and column.
 * @param m_cols: cells number in block's column.
 * @param m_rows: cells number in block's row.
 * @param fixed_nums: number of fixed cells.
 * @return value: new game board and solution. 
 */

Board *init_board(int n, int m_rows, int m_cols, int fixed_nums);

Game init_game();

/*
 * Function that changes the is_mark in game sturct */
int play_mark_errors(Game *game, int is_mark);

/*
 * Function that receives a Board structure and returns 1 if the player either asked to restart
 * or asked to exit, and 0 if the current games needs to continue
 */

int stop_game(Board *board); /* TODO delete */

/*
 * Function that manages playing the game
 */

void play();

/*
 * Function that receives a stuct Command and struct Board and performs
 * the "set" action on the current board game. adding new move to game->curr_move list.
 * returns valid or not_valid.
 */

int play_set(struct Command command, Board *board, Game *game);

/*
 * Function that receives a stuct Command and struct Board and performs
 * the "hint" action on the current board game
 */

int play_hint(struct Command command, Board *board);

/*
 * Function that receives the number of fixed cells from the player
 */

int get_fixed();

/*
 * Function that receives a board structure and prints the current board according to rules
 */

void print_board(Board *board, int print_err);

int update_errors_on_board(Board *board);

int play_autofill(Board **board, Game *game, int to_print);

int play_save(Board *board, Game *game, char *path);

int play_num_solutions(Board *board);

int play_generate(Game *game, Board *board, int x, int y);

int free_game(Game *game);

/*
 * function that receives a path to a sudoku file and a board and reads the sudoku from the file to the board
 */
int read_sudoku(char *path, Board *board);

/*
 * function the receives a board and a file path and loads the sudoku in the file to play in solve mode.
 * return 1 on success and 0 otherwise
 */
int play_solve(Board *board, char *path, Game *game);

/*
 * function the receives a board and an optional file path and loads the sudoku in the file to play in edit mode.
 * return 1 on success and 0 otherwise
 */
int play_edit(Board *board, char *path, Game *game);


/*
 * function the frees all allocated memory and exits the game
 */
int play_exit(Board *board, Game *game);

int play_undo(Board *board, Game *game);

int play_redo(Board *board, Game *game);
#endif /* GAME_H_ */
