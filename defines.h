#ifndef DEFINES_H_
#define DEFINES_H_

#define TRUE       1
#define FALSE      0
#define VALID      1
#define NOT_VALID  0
#define UNASSIGNED 0

#define RUN_VALIDATE 2
#define RUN_GENERATE 3
#define RUN_HINT     1

/***** moves linked-list structures *******/
 typedef struct move_action_t {
	int row;
	int col;
	int val_before;
 	int is_err_before;
	int val_after;
	int is_err_after;
	void *subchain;
} MoveInfo;

typedef struct _game_moves_list_t {
	MoveInfo move;
	struct _game_moves_list_t *next;
	struct _game_moves_list_t *prev;
} MoveList;

typedef struct _cell_item {
	int row;
	int col;
	int val;
} Cell_Item;

typedef enum _mode{INIT_MODE = 0, EDIT_MODE = 1, SOLVE_MODE = 2, EXIT_MODE = 3, ERROR_EXIT_MODE = 4} Mode;

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
 	MoveList *curr_move;
} Game;


#endif
