#ifndef DEFINES_H_
#define DEFINES_H_

/*#include "linked_list.h"*/

#define TRUE       1
#define FALSE      0
#define VALID      1
#define NOT_VALID  0
#define UNASSIGNED 0

#define RUN_VALIDATE 2
#define RUN_GENERATE 3
#define RUN_HINT     1

/***** moves linked-list structures *******/
typedef struct _move_node {
	int row;
	int col;
	int val_before;
	int val_after;
	struct _move_node *next;
} Changes_n;

typedef struct _move_list {
	int num_changes;
	Changes_n *first_change;
	struct _move_list *next;
	struct _move_list *prev;
} Move_l;


/******* board position structure *******/
typedef struct _cell_item {
	int row;
	int col;
	int val;
} Cell_Item;


typedef enum _mode{INIT, EDIT, SOLVE} Mode; 

/***** game structures *******/
typedef struct _cell {
	int val;
	int is_fixed;
	int is_err;
} Cell;

typedef struct _board {
	Cell **game_table;
	int n;
	int m_rows;
	int m_cols;
	int num_err;
	int filled;
} Board;

typedef struct _game_parameters {
	Mode game_mode;
	int mark_err;
	Move_l *curr_move;
} Game;

#endif
