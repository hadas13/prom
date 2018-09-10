#ifndef MOVES_H_
#define MOES_H_

#include "GUI.h"
#include "Solver.h"
#include <stdlib.h>

/***** consts for moves chain *********/
#define EMPTY_VALUE_FOR_INIT -1
#define TRUE       1
#define FALSE      0
#define VALID      1
#define NOT_VALID  0
#define UNASSIGNED 0
#define PROBBLY_IN_LOOP      20000


/***** functions **********/
/* checking if the move is fixed value, if not - inserting the new move to new_move */
int create_new_move(Board *board, MoveInfo *new_move, int row, int column, int value);

/* 
 * @param curr_move: the current move in the game that we are playing in.
 * @param new_move: the new information for the new move that the function will add to the new
 *		    move list.
 * @return value: valid or not_valid.
 */
int append_new_move_to_moves_list(MoveList *curr_move, MoveInfo new_move);

/* create chain of moves that connected to generate move */
/*int create_generate_chain(Board *game_board, Board *generate_board, MoveInfo move);*/

/*
 * @param **chain: the main chain of moves that going to be hold all the changes that
 *		   going to be made in autofill.
 * @return value: valid or not_valid.
 */
int create_autofill_chain(MoveList **chain);

/* finds the begining of chain, and removes all the moves */
int remove_moves_from_begining(MoveList **chain);

/* makes undo on board */
int undo_on_board(Board *board, MoveInfo current);

/* makes redo on board */
int redo_on_board(Board *board, MoveInfo current);

/* getting initialized move info struct
 * param move: the struct to initiliaze
 * @return value: VALID after initialized */
int init_empty_game_move_info(MoveInfo *move);
#endif
