#ifndef BACKTRACKING_H_
#define BACKTRACKING_H_

#include "Game.h"
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

/******** functions *********/

Stack *init_stack();

int push(Stack *stack, int row, int col, int val);

/* @return value: gets array of 3 elements: row, column and val of the top item */
Item *pop(Stack *stack);

void free_stack(Stack *stack);

int insert_first_empty_cell(Board *board, int *row, int *col);

int count_solutions(Board *board);

#endif /* BACKTRACKING_H_ */
