#ifndef DEFINES_H_
#define DEFINES_H_

#include "linked_list.h"

#define TRUE       1
#define FALSE      0
#define VALID      1
#define NOT_VALID  0
#define UNASSIGNED 0

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
} Game;
	
#endif
