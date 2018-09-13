/*
 * a header file that defines the different sturcts of the game
 */

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

/*
 * sturct that represents the linked list
 */
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

/* enum that represents the different game modes */
typedef enum _mode{INIT_MODE = 0, EDIT_MODE = 1, SOLVE_MODE = 2, EXIT_MODE = 3, ERROR_EXIT_MODE = 4} Mode;

/***** game structures *******/

/*
 * struct that represents a cell:
 * its value, indicator if its fixed and an indicator if its erroneous for the current board
 */
typedef struct _cell {
	int val;
	int is_fixed;
	int is_err;
} Cell;

/*
 * struct that represents the board parameters:
 * game board, number of cells in a row, size of a block, number of erroneous cells and number of filled cells
 */
typedef struct _board {
	Cell **game_table;
	int n;
	int m_rows;
	int m_cols;
	int num_err;
	int filled;
} Board;

/*
 * struct that represents the game parameters other than those related to the board:
 * mark errors indicator, game mode and the linked list
 */
typedef struct _game_parameters {
	Mode game_mode;
	int mark_err;
 	MoveList *curr_move;
} Game;

#endif
