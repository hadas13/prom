#include "defines.h"
#include "Game.h"
#include "GUI.h"

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
	struct _move *next;
	struct _move *prev;
} Move_l;

typedef struct _dll {
	/*int num_node;*/
	Move_l *first;
	Move_l *curr_move;
} Dll;

