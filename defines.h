#ifndef DEFINES_H_
#define DEFINES_H_

/*#include "linked_list.h"*/

#define TRUE       1
#define FALSE      0
#define VALID      1
#define NOT_VALID  0
#define UNASSIGNED 0

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

typedef struct _dll {
	/*int num_node;*/
	Move_l *first;
	Move_l *curr_move;
} Dll;

typedef enum _mode{INIT, EDIT, SOLVE} Mode; 

/***** game structures *******/
typedef struct _cell {
	int val;
	int is_fixed;
	int is_err;
} Cell;

typedef struct _board {
	Cell **game_table;
	Cell **solution;
	int n;
	int m_rows;
	int m_cols;
	int num_err;
	int filled;
} Board;

typedef struct _game_parameters {
	Mode game_mode;
	int mark_err;
	Dll play_list;
	Move_l *curr_move;
} Game;
	
#endif
