#ifndef MAINAUX_H_
#define MAINAUX_H_

#include "Game.h"
#include <ctype.h>

/*
 * Function that receives the string representation of an int and parses it to int type
 */

int string_to_int(char *string);

int save_file_to(FILE *fd, Board *board, int mode);

/*
 * Function that receives a string and checks if it contains only digits
 */

int is_numeric_string(char *string);

/* empty board from values */
int clean_vals_from_board(Board *board);

/*
 * receives a string and sets it's values to \0
 */
void clear_path(char *path);

/* 
 * functions that copy game_table from source_board to dest_board
 * */
int copy_board(Cell **source_b, Cell **dest_b, int len);

#endif /* MAINAUX_H_ */
