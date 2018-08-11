#include "linked_list.h"

int init_list(Board *board) {
	Dll *play_list;
	Move_l *init_move;
	Changes_n *init_change;

	init_move = (Move_l *) malloc(sizeof(Move_l *));
	if (init_move == NULL){
		print_malloc_failed_err();
		exit(NOT_VALID);
	}

	play_list = (Dll *) malloc(sizeof(Dll *));
	if (play_list == NULL){
		print_malloc_failed_err();
		exit(NOT_VALID);
	}

	init_move->num_changes = 0;
	init_move->first_change = NULL;
	init_move->next = NULL;
	init_move->prev = NULL;

	play_list->first = init_move;
	play_list->curr_move = init_move;
	return VALID;
}

Changes_n *get_last_change_of_move(Move_l *move) {
	Changes_n *last_change;

	last_change = move->first_change->next;

	while (last_change->next != NULL) {
		/* continue to the last node of the move */
		last_change = last_change->next;
	}

	return last_change;
}

int add_change_node_to_move_last(Move_l *move, int r, int c, int val_before, int val_after) {
	Changes_n *new_change;
	Changes_n *last_change;

	last_change = get_last_change_of_move(move);
	new_change = (Changes_n *)malloc(sizeof(Changes_n *));
	if (new_change == NULL){
		print_malloc_failed_err();
		exit(NOT_VALID);
	}

	new_change->row = r;
	new_change->col = c;
	new_change->val_before = val_before;
	new_change->val_after = val_after;
	new_change->next = NULL;
	last_change->next = new_change;

	return VALID;
}


int remove_move(Move_l *move) {
	Changes_n *curr;
	Changes_n *next_change;

	next_change = move->first_change;
	/* go over the list and free all the nodes. stop if move is empty */
	while ((curr = next_change) != NULL) {
		next_change = next_change->next;
		free(curr);
	}

	return VALID;
}

int free_all_next_moves(Move_l *move) {
	Move_l *tmp;

	while (move->next != NULL) {
		tmp = move;
		move = &move->next;
		if (remove_move(tmp) != VALID) {
			return NOT_VALID;
		}

	
	return VALID;
}

int add_new_move(Move_l *last_move) {
	Move_l *new_move;

	free_all_next_moves(last_move);

	new_move = (Move_l *) malloc(sizeof(Move_l *));
	if (new_move == NULL){
		print_malloc_failed_err();
		exit(NOT_VALID);
	}
	return VALID;
}

