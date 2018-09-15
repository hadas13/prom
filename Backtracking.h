/* this module counts by backtracking solve the number of solution to a board.
* it uses stack as the data structure that holds the possible values for the cells.
*/

#ifndef BACKTRACKING_H_
#define BACKTRACKING_H_

#include "Game.h"
#include "defines.h"
#include "Solver.h"
/******* stack structure ***********/

typedef struct _item {
	int row;
	int col;
	int val;
	struct _item *next;
}Item;

typedef struct _stack {
	Item *top_item;
	int counter;
}Stack;

/******** function *********/
int count_solutions(Board *board);

#endif /* BACKTRACKING_H_ */
