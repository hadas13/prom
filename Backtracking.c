#include "Backtracking.h"

Stack *init_stack() {
	Stack *stack;

	stack = (Stack *)malloc(sizeof(Stack));
	if (stack == NULL){
		print_malloc_failed_err();
		exit(NOT_VALID);
	}
	stack->top_item = NULL;
	stack->counter = 0;
	return stack;
}

int push(Stack *stack, int row, int col, int val) {
	Item *item;

	item = (Item *)malloc(sizeof(Item));
	if (item == NULL){
		print_malloc_failed_err();
		exit(NOT_VALID);
	}

	item->row = row;
	item->col = col;
	item->val = val;
	/* next of the new top points to the last top */
	item->next = stack->top_item;
	stack->top_item = item;
	stack->counter += 1;
	return VALID;
}

Item *pop(Stack *stack) {
	Item *ret_item = NULL;

	ret_item = stack->top_item;
	stack->top_item = stack->top_item->next;
	stack->counter -= 1;
	ret_item->next = NULL;

	return ret_item;
}

void free_stack(Stack *stack) {
	if (stack == NULL){
		return;
	}

	while (stack->counter > 0) {
		pop(stack);
	}
	free(stack);
}

int insert_first_empty_cell(Board *board, int *row, int *col) {
	int is_empty_cell = FALSE;
	int r = 0, c = 0;

	for (r = 0; r < board->n; r++) {
		for (c = 0; c < board->n; c++) {
			if ((board->game_table[r][c].val == UNASSIGNED) &&
				       	(board->game_table[r][c].is_fixed == FALSE)) {
				/* the cell is empty and can be change */
				*row = r;
				*col = c;
				is_empty_cell = TRUE;
				return is_empty_cell;
			}
		}
	}

	is_empty_cell = FALSE;
	return is_empty_cell;
}


int mark_board_as_fixed(Board *board) {
	int r, c;

	for (r = 0; r < board->n; r++){
		for (c = 0; c < board->n; c++) {
			if (board->game_table[r][c].val != UNASSIGNED) {
				board->game_table[r][c].is_fixed = TRUE;
			}
		}
	}

	return VALID;
}

int push_possible_values_to_stack(Board *board, Stack *stack, int row, int col, int *how_many_added) {
	int possible_val = 0;

	for (possible_val = 1; possible_val < board->n + 1; possible_val++) {
		if (is_cell_valid(board, board->game_table, possible_val, row, col)) {
			/* added number as a possible number */
			(*how_many_added) += 1;
			if (push(stack, row, col, possible_val) != VALID) {
				return NOT_VALID;
			}
		}
	}
	return VALID;

}

int empty_all_next_cells(Board *board, Item *item) {
	int r, c;

	for(c = item->col; c < board->n; c++) {
		if (board->game_table[item->row][c].is_fixed == FALSE) {
			board->game_table[item->row][c].val = UNASSIGNED;
		}
	}

	for (r = item->row + 1; r < board->n; r++) {
		for (c = item->col; c < board->n; c++) {
			if (board->game_table[r][c].is_fixed == FALSE) {
				board->game_table[r][c].val = UNASSIGNED;
			}
		}
	}

	return VALID;
}

int solve_by_backtracking(Board *board, Stack *stack, int *counter) {
	Item *item_to_insert;
	int row = 0;
	int col = 0;
	int how_many_added = 0;
	int is_empty_cell = TRUE;
	int check_result = NOT_VALID;

	insert_first_empty_cell(board, &row, &col);
	if (push_possible_values_to_stack(board, stack, row, col, &how_many_added) != VALID) {
		return NOT_VALID;
	}

	while (stack->counter > 0) {
		/* there are still possible values for cells - continue backtracking */
		item_to_insert = pop(stack);
		/* cleaning all next possible numbers */
		empty_all_next_cells(board, item_to_insert);
		/* inserting the next possible value */
		board->game_table[item_to_insert->row][item_to_insert->col].val = 
			item_to_insert->val;

		is_empty_cell = insert_first_empty_cell(board, &row, &col);
		if (!is_empty_cell) {
			/* finish filling the board */
			(*counter) += 1;
		} else {
			check_result = push_possible_values_to_stack(
					board, stack, row, col, &how_many_added);
			if (check_result == NOT_VALID) {
				return NOT_VALID;
			}
		}
		free(item_to_insert);
	}
	return VALID;
}

int count_solutions(Board *board) {
	int r, c;
	int is_empty_cell = TRUE;
	int *sol_counter = NULL;
	Board *board_to_fill;
	Stack *stack;
	int test = 0;

	sol_counter = &test;

	board_to_fill = init_board(board->n, board->m_rows, board->m_cols, board->filled);
	copy_board(board->game_table, board_to_fill->game_table, board->n);
	if (board_to_fill == NULL) {
		return -1;
	}

	stack = init_stack();

	mark_board_as_fixed(board_to_fill);
	is_empty_cell = insert_first_empty_cell(board_to_fill, &r, &c);

	if (!is_empty_cell) {
		/* not empty cells - board is complete */
		*sol_counter = 1;
	}

	else {
		solve_by_backtracking(board_to_fill, stack, sol_counter);
	}

	free_stack(stack);
	free_board(board_to_fill);
	return *sol_counter;
	
}
