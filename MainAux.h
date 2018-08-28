#ifndef MAINAUX_H_
#define MAINAUX_H_

#include "Game.h"

/*
 * Function that receives the string representation of an int and parses it to int type
 */

int string_to_int(char *string);

int save_file_to(FILE *fd, Board *board, int mode);

#endif /* MAINAUX_H_ */
