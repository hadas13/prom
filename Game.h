/*
 * a header file that includes methods that related to the logic of a sudoku game
 * and the management of the different user command options
 */

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
#define FINISHED_UNDO 3

 /****** functions ********/

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

/*
 * Function that changes the is_mark in game sturct
 */
int play_mark_errors(Game *game, int is_mark);

/*
 * a function that runs the game, creates all of its parameters and runs each turn according to uesr commands
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
 * function that goes over the board and mark each cell as erroneous or valid according to sudoku rules
 */
int update_errors_on_board(Board *board);

/*
 * function that plays the autofill command. to_print indicated if additional info needs to be printed or not
 */
int play_autofill(Board **board, Game *game, int to_print);

/*
 * function that plays the save command and on success saves the current sudoku to a file who's path is char *path
 */
int play_save(Board *board, Game *game, char *path);

/*
 * function that plays the num solutions command
 */
int play_num_solutions(Board *board);

/*
 * function that plays the generate command
 */
int play_generate(Game *game, Board *board, int x, int y);

/*
 * function that recieves a pointer to the game struct and free all its allocated memory
 */
int free_game(Game *game);

/*
 * function that receives a path to a sudoku file and a board and reads the sudoku from the file to the board
 * in addition, it receives an indication if it need to pay attention to fixed cells (SOLVE) or not (EDIT)
 */
int read_sudoku(char *path, Board *board, int use_fixed);

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

/*
 * function that plays the undo command
 */
int play_undo(Board *board, Game *game);

/*
 * function that plays the redo command
 */
int play_redo(Board *board, Game *game);

/*
 * function that plays the reset command
 */
int play_reset(Board *board, Game *game);

/*
 * function that plays the validate command
 */
int play_validate(Board *board, int to_print);
#endif /* GAME_H_ */
