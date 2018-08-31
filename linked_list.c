#include "linked_list.h"

Move_l *init_move(){
	Move_l *move = NULL;
	Move_l *next = NULL;
	Move_l *prev = NULL;
	Changes_n *empty_change = NULL;

	move = (Move_l *) malloc(sizeof(Move_l *));
	if (move == NULL){
		print_malloc_failed_err();
		exit(NOT_VALID);
	}

	move->num_changes = 0; /* didn't change anything yet */
	move->first_change = empty_change;
	move->next = next;
	move->prev = prev;
	
	return move;
}

Changes_n *get_last_change_of_move(Move_l *move) {
	Changes_n *last_change;

	if (move->first_change == NULL) {
		/* already the last change */

		return move->first_change;
	}

	last_change = move->first_change->next;

	while (last_change->next != NULL) {
		/* continue to the last node of the move */
		last_change = last_change->next;
	}

	return last_change;
}

Changes_n *init_change(int r, int c, int val_before, int val_after) {
	Changes_n *new_change = (Changes_n *)malloc(sizeof(Changes_n *)) ;
	if (new_change == NULL) {
		print_malloc_failed_err();
		return NOT_VALID;
	}

	new_change->row = r;
	new_change->col = c;
	new_change->val_before = val_before;
	new_change->val_after = val_after;
	new_change->next = NULL;
	return new_change;
}

int add_last_change_to_move(Move_l *move, int r, int c, int val_before, int val_after) {
	Changes_n *new_change;
	Changes_n *last_change;
	int is_first = FALSE;

	if (move->first_change == NULL) {
		/* first change in the move */
		is_first = TRUE;
	}

	new_change = init_change(r, c, val_before, val_after);

	if (is_first) {
		move->first_change = new_change;
	} else {
		last_change = get_last_change_of_move(move);
		last_change->next = new_change;
	}
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

	if (move == NULL) {
		/* allready freed all next moves */
		return VALID;
	}

	while (move->next != NULL) {
		tmp = move;
		move = move->next;
		if (remove_move(tmp) != VALID) {
			return NOT_VALID;
		}

	}

	return VALID;
}

int free_all_moves(Move_l *curr_move) {
	if (curr_move == NULL) {
		/* all moves are all ready free */
		return VALID;
	}
	/* find first move */
	while (curr_move->prev != NULL) {
		curr_move = curr_move->prev;
	}
	/* curr_move is the first move */
	free_all_next_moves(curr_move);

	if (curr_move != NULL){
		return NOT_VALID;
	}

	return VALID;
}

/* for adding changes to the new move, return the new move */
Move_l *add_new_move(Move_l *curr_move, int r, int c, int val_before, int val_after) {
	Move_l *new_move;

	if (free_all_next_moves(curr_move) == NOT_VALID) {
		return NULL;
	}
	new_move = (Move_l *) malloc(sizeof(Move_l *));
	if (new_move == NULL){
		print_malloc_failed_err();
		exit(NOT_VALID);
	}

	add_last_change_to_move(new_move, r, c, val_before, val_after);

	return new_move;
}

int undo_curr_move(Move_l *curr_move, Board *board, Game *game) {
	Changes_n *change;
	int r, c, v_before;

	change = curr_move->first_change;
	
	while (change->next != NULL) {
		r = change->row;
		c = change->col;
		v_before = change->val_before;
		board->game_table[r][c].val = v_before;
		board->game_table[r][c].is_fixed = FALSE;
		change = change->next;
	}
	update_errors_on_board(board);
	game->curr_move = curr_move->prev;
	return VALID;
}

int undo(Move_l *curr_move, Board *board, Game *game) {

	/* check if there is move to undo to */
	if (curr_move->prev == NULL) {
		print_err_no_moves_to_undo();
	}

	undo_curr_move(curr_move, board, game);
	print_board(board, game->mark_err);
	print_undo_move(curr_move);
	game->curr_move = curr_move->prev;

	return VALID;
}

int redo_curr_move(Move_l *curr_move, Board *board, Game *game) {
	Changes_n *change;
	int r, c, v_after;

	change = curr_move->first_change;
	
	while (change->next != NULL) {
		r = change->row;
		c = change->col;
		v_after = change->val_after;
		board->game_table[r][c].val = v_after;
		board->game_table[r][c].is_fixed = FALSE;
		change = change->next;
	}
	update_errors_on_board(board);
	if (curr_move->next != NULL) {
		game->curr_move = curr_move->next;
	}
	return VALID;
}

int redo(Move_l *curr_move, Board *board, Game *game) {

	/* check if there is move to redo */
	if (curr_move->next == NULL) {
		print_err_no_moves_to_redo();
	}

	game->curr_move = curr_move->next;
	redo_curr_move(curr_move, board, game);
	print_board(board, game->mark_err);
	print_redo_move(curr_move);

	return VALID;
}

