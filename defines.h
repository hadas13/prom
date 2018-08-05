#ifndef DEFINES_H_
#define DEFINES_H_

#define TRUE       1
#define FALSE      0
#define VALID      1
#define NOT_VALID  0
#define UNASSIGNED 0

typedef enum _mode{Init, Edit, Solve} Mode; 

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
	int filled;
	int num_err;
} Board;

struct _node {
	Board *board;
	int pos;
	struct _node *next;
	struct _node *prev;
};

typedef struct _node Node;

typedef struct _dll {
	int num_node;
	Node first;
	Node last;
	Node curr_pos;
} Dll;


typedef struct _game_parameters {
	Mode game_mode;
	int mark_err;
	Dll play_list;
} Game;
	
#endif
