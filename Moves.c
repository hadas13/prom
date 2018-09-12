#include "Moves.h"

int create_new_move(Board *board, MoveInfo *new_move, int row, int column, int value) {
	if (board->game_table[row][column].is_fixed) {
		print_err_cell_is_fixed();
		return NOT_VALID;
	}

	init_empty_game_move_info(new_move);
	new_move->row = row + 1;
	new_move->col = column + 1;
	new_move->val_before = board->game_table[row][column].val;
	new_move->is_err_before = board->game_table[row][column].is_err;
	new_move->val_after = value;
	new_move->is_err_after = is_cell_valid(board, board->game_table, value, row, column);

	return VALID;
}

int find_last_move(MoveList *chain, MoveList **ending) {
	int not_in_loop = 0;
	MoveList *current = chain;

	while (current->next != NULL && not_in_loop < PROBBLY_IN_LOOP) {
		current = current->next;
		not_in_loop += 1;
	}

	if (current->next == NULL) {
		*ending = current;
		return VALID;
	}

	return NOT_VALID;
}

int find_first_move(MoveList *chain, MoveList **ret_first) {
	MoveList *curr = chain;
	int not_in_loop = FALSE;

	while (curr->prev != NULL && not_in_loop < PROBBLY_IN_LOOP) {
		curr = curr->prev;
		not_in_loop += 1;
	}

	if (curr->prev == NULL) {
		*ret_first = curr;
		return VALID;
	}

	printf("Error: couldn't find first move in find_first_move\n");
	return NOT_VALID;
}
int remove_next_moves(MoveList *begining) {
	int not_in_loop = 0;
	MoveList *current;

	if (find_last_move(begining, &current) != VALID) {
		printf("Error: couldn't find last move in remove_next_moves.\n");
		return NOT_VALID;
	}

	while (current != begining && not_in_loop < PROBBLY_IN_LOOP) {
		if (current->move.subchain != NULL) {
			/* in current move there is more than one change - autofill move */
			remove_next_moves(((MoveList *) current->move.subchain));
			free(current->move.subchain);
		}
		/* there is one change */
		current = current->prev;
		free(current->next);
		current->next = NULL;
		not_in_loop += 1;
	}

	if (current == begining) {
		return VALID;
	}

	else {
		printf("Error: something wrong happend in remove_next_moves\n");
		return NOT_VALID;
	}
}

int append_new_move_to_moves_list(MoveList *curr_move, MoveInfo new_move) {
	if (remove_next_moves(curr_move) != VALID) {
		printf("Error: something bad happend in remove_next_moves.\n");
		return NOT_VALID;
	}

	curr_move->next = (MoveList *) calloc(1, sizeof(MoveList));
	if (curr_move->next == NULL) {
		print_malloc_failed_err();
		exit(NOT_VALID);
	}
	curr_move->next->move.row = new_move.row;
	curr_move->next->move.col = new_move.col;
	curr_move->next->move.val_before = new_move.val_before;
	curr_move->next->move.is_err_before = new_move.is_err_before;
	curr_move->next->move.val_after = new_move.val_after;
	curr_move->next->move.is_err_after = new_move.is_err_after;
	curr_move->next->move.subchain = new_move.subchain;
	curr_move->next->next = NULL;
	curr_move->next->prev = curr_move;

	return VALID;
}

int create_empty_chain(MoveList **chain) {
	MoveInfo init_move;
	if (init_empty_game_move_info(&init_move) != VALID) {
		printf("Error: couldn't init empty game move in create_autofill_chain\n");
		return NOT_VALID;
	}
	(* chain) = (MoveList *) malloc(sizeof(MoveList));
	if (chain == NULL) {
		print_malloc_failed_err();
		exit(NOT_VALID);
	}
	(* chain)->move.row = init_move.row;
	(* chain)->move.col = init_move.col;
	(* chain)->move.val_before = init_move.val_before;
	(* chain)->move.is_err_before = init_move.is_err_before;
	(* chain)->move.val_after = init_move.val_after;
	(* chain)->move.is_err_after = init_move.is_err_after;
	(* chain)->move.subchain = init_move.subchain;
	(* chain)->next = NULL;
	(* chain)->prev = NULL;
	return VALID;
}

int create_generate_chain(MoveInfo *main_move,Board *board, Board *generate_board) {
	MoveList *generate_chain;
	MoveInfo new_move;
	int r = 0, c = 0, new_val = UNASSIGNED;

	if (create_empty_chain(&generate_chain) != VALID) {
		printf("Error: couldn't init generate chain\n");
		return NOT_VALID;
	}
	main_move->subchain = generate_chain;

	for (c = 0; c < board->n; c++) {
		for (r = 0; r < board->n; r++) {
			new_val = generate_board->game_table[r][c].val;
			if (board->game_table[r][c].val != new_val) {
				create_new_move(board, &new_move, r, c, new_val);
				if (append_new_move_to_moves_list(generate_chain, new_move) !=
						VALID) {
					printf("Error: couldn't append new move to generate chain\n");
					return NOT_VALID;
				}
				generate_chain = generate_chain->next;
			}
		}
	}
	return VALID;
}



int remove_moves_from_begining(MoveList **chain) {
	MoveList *start;

	if (find_first_move(*chain, &start) != VALID) {
		printf("Error: remove_moves_from_begining failed - couldn't find begining\n");
		return NOT_VALID;
	}

	if (remove_next_moves(start) != VALID) {
		printf("Error: couldn't remove all moves from start point\n");
		return NOT_VALID;
	}

	init_empty_game_move_info(&(start->move));
	*chain = start;
	return VALID;
}

int undo_more_that_one_changes(Board *board, MoveList *curr_chain) {
	MoveList *chain_for_changes;
	MoveInfo move;

	if (find_last_move(curr_chain, &chain_for_changes) != VALID) {
		printf("Error: couldn't find the last change in the chain - undo_more_than_one_changes\n");
		return NOT_VALID;
	}

	while (chain_for_changes != NULL) {
		move = chain_for_changes->move;
		if (move.row != EMPTY_VALUE_FOR_INIT || move.col != EMPTY_VALUE_FOR_INIT ||
				move.val_before != EMPTY_VALUE_FOR_INIT ||
			       	move.val_after != EMPTY_VALUE_FOR_INIT ||
			       	move.subchain != NULL){
			/* not finished the chain of changes */
			if (undo_on_board(board, move) != VALID) {
				printf("Error: couldn't undo one change in the board\n");
				return NOT_VALID;
			}
		}
		chain_for_changes = chain_for_changes->prev;
	}

	return VALID;
}

int undo_on_board(Board *board, MoveInfo current) {
	if (current.subchain == NULL) {
		/* undo of one move/change */
		board->game_table[current.row - 1][current.col - 1].val = current.val_before;
		board->game_table[current.row - 1][current.col - 1].is_err = current.is_err_before;
		update_errors_on_board(board);
		return VALID;
	}
	else {
		/* undo of more than one changes/moves */
		return undo_more_that_one_changes(board, current.subchain);
	}
}

int redo_more_that_one_changes(Board *board, MoveList *curr_chain) {
	MoveList *chain_for_changes;
	MoveInfo move;
	if (find_first_move(curr_chain, &chain_for_changes) != VALID) {
		printf("Error: couldn't find the first change in the chain - redo_more_than_one_changes\n");
		return NOT_VALID;
	}
	while (chain_for_changes != NULL) {
		move = chain_for_changes->move;
		if (move.row != EMPTY_VALUE_FOR_INIT || move.col != EMPTY_VALUE_FOR_INIT ||
				move.val_before != EMPTY_VALUE_FOR_INIT ||
			       	move.val_after != EMPTY_VALUE_FOR_INIT ||
			       	move.subchain != NULL){
			/* not finished the chain of changes */
			if (redo_on_board(board, move) != VALID) {
				printf("Error: couldn't redo one change in the board\n");
				return NOT_VALID;
			}
		}
		chain_for_changes = chain_for_changes->next;
	}

	return VALID;
}

int redo_on_board(Board *board, MoveInfo current) {
	if (current.subchain == NULL) {
		/* redo one move/change */
		board->game_table[current.row - 1][current.col - 1].val = current.val_after;
		board->game_table[current.row - 1][current.col - 1].is_err = current.is_err_after;
		update_errors_on_board(board);
		return VALID;
	}
	else {
		/* undo of more than one changes/moves */
		return redo_more_that_one_changes(board, current.subchain);
	}
}

int init_empty_game_move_info(MoveInfo *move) {
	move->row = EMPTY_VALUE_FOR_INIT;
	move->col = EMPTY_VALUE_FOR_INIT;
	move->val_before = EMPTY_VALUE_FOR_INIT;
	move->is_err_before = EMPTY_VALUE_FOR_INIT;
	move->val_after = EMPTY_VALUE_FOR_INIT;
	move->is_err_after = EMPTY_VALUE_FOR_INIT;
	move->subchain = NULL;

	return VALID;
}
