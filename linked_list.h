#include "defines.h"
#include "Game.h"
#include "GUI.h"
/******* functions *******/

Dll *init_list();

Changes_n *get_last_change_of_move(Move_l *move);

/*
 * @param move: the move node in the linked list that we are adding a change in the board to.
 * @param r: the row that the change was done to.
 * @param c: the column that the change was done to.
 * @param val_before: the value in the board before the change.
 * @param val_after: the value in the board after the change.
 * @return value: success or fauiler.
 */
int add_last_change_to_move(Move_l *move, int r, int c, int val_before, int val_after);

int free_all_next_moves(Move_l *move);

Move_l *add_new_move(Move_l *curr_move, int r, int c, int val_before, int val_after);

int undo(Move_l *curr_move, Board *board, Game *game);

int redo(Move_l *curr_move, Board *board, Game *game);
